/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **********/
#include "mainmenu.h"

#include <clip/clip.h>
#include "../env/imageenvironment.h"
#include "../event/gui/history_events.h"
#include "../helper/selection_helpers.h"
#include "../history/selectioncutaction.h"
#include "../manager/eventmanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textselectionmanager.h"
#include "imageeditorview.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  void popEditMenu(MainMenu::EditMenu* editMenu,
                   QWidget* parent,
                   bool menu)
  {
    if(menu) {
      editMenu->undoEditAction = editMenu->menu->addAction(QObject::tr("Undo"));
      editMenu->redoEditAction = editMenu->menu->addAction(QObject::tr("Redo"));
    }
    editMenu->menu->addSeparator();
    editMenu->cutEditAction = editMenu->menu->addAction(QObject::tr("Cut"));
    editMenu->copyEditAction = editMenu->menu->addAction(QObject::tr("Copy"));
    editMenu->pasteEditAction = editMenu->menu->addAction(QObject::tr("Paste"));
    editMenu->deleteEditAction = editMenu->menu->addAction(QObject::tr("Delete"));
    editMenu->menu->addSeparator();
    if(menu) {
      editMenu->preferencesEditAction = editMenu->menu->addAction(QObject::tr("Preferences"));
      editMenu->preferencesEditAction->setEnabled(false);
    }

    QObject::connect(editMenu->menu, &QMenu::aboutToShow, [editMenu, menu, parent] {
      if(!getImageEnvironment().isViewSet()) {
        editMenu->undoEditAction->setEnabled(false);
        editMenu->redoEditAction->setEnabled(false);
        editMenu->copyEditAction->setEnabled(false);
        editMenu->cutEditAction->setEnabled(false);
        editMenu->pasteEditAction->setEnabled(false);
        editMenu->deleteEditAction->setEnabled(false);
  //        editMenu->preferencesEditAction->setEnabled(false);
      } else {
        auto& selection_manager = getSelectionManager();
        auto& history_manager = getHistoryManager();

        if(menu) {
          if(history_manager.canUndo()) {
            editMenu->undoEditAction->setEnabled(true);
            editMenu->undoEditAction->setText((std::string("Undo \"") +
                                               history_manager.getNextUndoName() +
                                               std::string("\"...")).c_str());
          } else {
            editMenu->undoEditAction->setEnabled(false);
            editMenu->undoEditAction->setText(QObject::tr("Undo"));
          }

          if(history_manager.canRedo()) {
            editMenu->redoEditAction->setEnabled(true);
            editMenu->redoEditAction->setText((std::string("Redo \"") +
                                               history_manager.getNextRedoName() +
                                               std::string("\"...")).c_str());
          } else {
            editMenu->redoEditAction->setEnabled(false);
            editMenu->redoEditAction->setText(QObject::tr("Redo"));
          }
        }

        if(selection_manager.layerExists() && selection_manager.selectionExists()) {
          editMenu->copyEditAction->setEnabled(true);
          editMenu->cutEditAction->setEnabled(true);
          editMenu->deleteEditAction->setEnabled(true);
        } else {
          editMenu->copyEditAction->setEnabled(false);
          editMenu->cutEditAction->setEnabled(false);
          editMenu->deleteEditAction->setEnabled(false);
        }

        if(clip::has(clip::image_format())) {
          editMenu->pasteEditAction->setEnabled(true);
        } else {
          if(clip::has(clip::text_format()) && getTextSelectionManager().selectionExists()) {
            editMenu->pasteEditAction->setEnabled(true);
          } else {
            editMenu->pasteEditAction->setEnabled(false);
          }
        }
      }
    });

    QObject::connect(editMenu->menu, &QMenu::aboutToHide, [editMenu, menu] {
      if(menu) {
        editMenu->undoEditAction->setEnabled(true);
        editMenu->redoEditAction->setEnabled(true);
  //        editMenu->preferencesEditAction->setEnabled(true);
      }
      editMenu->copyEditAction->setEnabled(true);
      editMenu->cutEditAction->setEnabled(true);
      editMenu->pasteEditAction->setEnabled(true);
      editMenu->deleteEditAction->setEnabled(true);
    });

    if(menu) {
      QObject::connect(editMenu->undoEditAction, &QAction::triggered, [](bool) {
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();

        if(image_env.isViewSet() && getHistoryManager().canUndo()) {
          auto res = getHistoryManager().undo();

          if(res & HistoryAction::EAR_RECREATE_FRAME_LIST) {
            emitEvent(gui_events::HistoryRecreateFrameEvent());
          }
          if(res & HistoryAction::EAR_RECREATE_LAYER_LIST) {
            emitEvent(gui_events::HistoryRecreateLayerEvent());
          }
          if(res & HistoryAction::EAR_REFRESH_LAYER_LIST) {
            emitEvent(gui_events::HistoryRefreshLayerEvent());
          }
          if(res & HistoryAction::EAR_REFRESH_IMAGE) {
            image_manager.refresh();
            getPreviewManager().refreshAll();
          }
          if(res & HistoryAction::EAR_REFRESH_IMAGE_SIZE) {
            image_env.getView().updateSize();
            image_env.getScrollArea().updateResizeHandles();
            getPreviewManager().refreshResizeAll();
          }
        }
      });

      QObject::connect(editMenu->redoEditAction, &QAction::triggered, [](bool) {
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        if(image_env.isViewSet() && getHistoryManager().canRedo()) {
          auto res = getHistoryManager().redo();

          if(res & HistoryAction::EAR_RECREATE_FRAME_LIST) {
            emitEvent(gui_events::HistoryRecreateFrameEvent());
          }
          if(res & HistoryAction::EAR_RECREATE_LAYER_LIST) {
            emitEvent(gui_events::HistoryRecreateLayerEvent());
          }
          if(res & HistoryAction::EAR_REFRESH_LAYER_LIST) {
            emitEvent(gui_events::HistoryRefreshLayerEvent());
          }
          if(res & HistoryAction::EAR_REFRESH_IMAGE) {
            image_manager.refresh();
            getPreviewManager().refreshAll();
          }
          if(res & HistoryAction::EAR_REFRESH_IMAGE_SIZE) {
            image_env.getView().updateSize();
            image_env.getScrollArea().updateResizeHandles();
            getPreviewManager().refreshResizeAll();
          }
        }
      });
    }

    QObject::connect(editMenu->copyEditAction, &QAction::triggered, [](bool){
      if(getImageEnvironment().isViewSet()) {
        selection_helpers::tryCopy();

        getImageEnvironment().getView().setFocus();
      }
    });
    QObject::connect(editMenu->cutEditAction, &QAction::triggered, [](bool){
      if(getImageEnvironment().isViewSet()) {
        selection_helpers::tryCut(true);

        getImageEnvironment().getView().setFocus();
      }
    });
    QObject::connect(editMenu->pasteEditAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet()) {
        selection_helpers::tryPaste(true);

        getImageEnvironment().getView().setFocus();
      }
    });
    QObject::connect(editMenu->deleteEditAction, &QAction::triggered, [](bool) {
      auto& selection_manager = getSelectionManager();
      if(getImageEnvironment().isViewSet() && selection_manager.layerExists() && selection_manager.selectionExists()) {
        auto selection_rect = selection_manager.getSelectionRect();
        auto selection_layer = selection_manager.getSelectionLayer();

        if(selection_manager.layerMaskExists()) {
          emitHistoryAction(SelectionCutAction(selection_rect,
                                         selection_layer,
                                         selection_manager.getSelectionLayerMask()));
        } else {
          emitHistoryAction(SelectionCutAction(selection_rect,
                                         selection_layer));
        }


        auto& image_env = getImageEnvironment();
        selection_manager.clear();
        getPreviewManager().refreshAll();
        image_env.getScrollArea().updateSelectionWidget();
        image_env.getView().repaint();

        getImageEnvironment().getView().setFocus();
      }
    });
  }
}
