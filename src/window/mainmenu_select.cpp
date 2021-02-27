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

#include "../env/guienvironment.h"
#include "../env/imageenvironment.h"
#include "../helper/selection_helpers.h"
#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../manager/textselectionmanager.h"
#include "../manager/selectionmanager.h"
#include "imageeditorview.h"
#include "lefttoolbox.h"
#include "selectionwidget.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
namespace
{
  void pop_resize_sub_menu(QMenu* mainMenu,
                           QMenu** resizeMenu,
                           QAction** resizeScaleAction,
                           QAction** resizeSmoothScaleAction,
                           QAction** resizeResizeAction)
  {
    *resizeMenu = mainMenu->addMenu(QObject::tr("Resize Mode"));
    *resizeScaleAction = (*resizeMenu)->addAction(QObject::tr("Scale"));
    *resizeSmoothScaleAction = (*resizeMenu)->addAction(QObject::tr("Smooth Scale"));
    *resizeResizeAction = (*resizeMenu)->addAction(QObject::tr("Resize"));

    (*resizeScaleAction)->setCheckable(true);
    (*resizeSmoothScaleAction)->setCheckable(true);
    (*resizeResizeAction)->setCheckable(true);

    QObject::connect((*resizeMenu), &QMenu::aboutToShow,
    [rsa = *resizeScaleAction, rssa = *resizeSmoothScaleAction, rra = *resizeResizeAction] {
      rra->setChecked(false);
      rsa->setChecked(false);
      rssa->setChecked(false);

      auto resizeMode = getSelectionManager().getResizeMode();
      if(resizeMode == MaskablePixelData::EResizeMode::RESIZE) {
        rra->setChecked(true);
      } else if(resizeMode == MaskablePixelData::EResizeMode::SCALE) {
        rsa->setChecked(true);
      } else if(resizeMode == MaskablePixelData::EResizeMode::SMOOTH_SCALE) {
        rssa->setChecked(true);
      }
    });

    QObject::connect((*resizeScaleAction), &QAction::triggered,
    [](bool){
      getSelectionManager().setResizeMode(MaskablePixelData::EResizeMode::SCALE);
    });
    QObject::connect((*resizeSmoothScaleAction), &QAction::triggered,
    [](bool){
      getSelectionManager().setResizeMode(MaskablePixelData::EResizeMode::SMOOTH_SCALE);
    });
    QObject::connect((*resizeResizeAction), &QAction::triggered,
    [](bool){
      getSelectionManager().setResizeMode(MaskablePixelData::EResizeMode::RESIZE);
    });
  }
}
  void popSelectMenu(MainMenu::SelectMenu* selectMenu, QWidget*, bool menu)
  {
    pop_resize_sub_menu(selectMenu->menu,
                        &selectMenu->resizeMenu,
                        &selectMenu->resizeScaleAction,
                        &selectMenu->resizeSmoothScaleAction,
                        &selectMenu->resizeResizeAction);

    selectMenu->menu->addSeparator();
    selectMenu->normalSelectionAction = selectMenu->menu->addAction(QObject::tr("Normal Selection"));
    selectMenu->resizeSelectionAction = selectMenu->menu->addAction(QObject::tr("Resize Selection"));
    selectMenu->moveSelectionAction = selectMenu->menu->addAction(QObject::tr("Move Selection"));
    selectMenu->normalSelectionAction->setCheckable(true);
    selectMenu->resizeSelectionAction->setCheckable(true);
    selectMenu->moveSelectionAction->setCheckable(true);
    selectMenu->menu->addSeparator();
    if(menu) {
      selectMenu->selectAllEditAction = selectMenu->menu->addAction(QObject::tr("Select All..."));
    }
    selectMenu->deselectEditAction = selectMenu->menu->addAction(QObject::tr("Deselect"));

    QObject::connect(selectMenu->menu, &QMenu::aboutToShow, [selectMenu, menu] {
      if(getSelectionManager().selectionExists()) {
        selectMenu->resizeMenu->setEnabled(true);
      }

      if(getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists()) {
        selectMenu->normalSelectionAction->setEnabled(true);
        selectMenu->resizeSelectionAction->setEnabled(true);
        selectMenu->moveSelectionAction->setEnabled(true);

        auto selection_mode = getImageEnvironment().getSelection().getMode();
        switch(selection_mode) {
        case SelectionWidget::ESelectionMode::NORMAL:
          selectMenu->normalSelectionAction->setChecked(true);
          selectMenu->moveSelectionAction->setChecked(false);
          selectMenu->resizeSelectionAction->setChecked(false);
          break;
        case SelectionWidget::ESelectionMode::MOVE:
          selectMenu->normalSelectionAction->setChecked(false);
          selectMenu->moveSelectionAction->setChecked(true);
          selectMenu->resizeSelectionAction->setChecked(false);
          break;
        case SelectionWidget::ESelectionMode::RESIZE:
          selectMenu->normalSelectionAction->setChecked(false);
          selectMenu->moveSelectionAction->setChecked(false);
          selectMenu->resizeSelectionAction->setChecked(true);
          break;
        }
      } else {
        selectMenu->resizeMenu->setEnabled(false);
        selectMenu->normalSelectionAction->setChecked(true);
        selectMenu->moveSelectionAction->setChecked(false);
        selectMenu->resizeSelectionAction->setChecked(false);
        selectMenu->normalSelectionAction->setEnabled(false);
        selectMenu->resizeSelectionAction->setEnabled(false);
        selectMenu->moveSelectionAction->setEnabled(false);
      }

      if(!getImageEnvironment().isViewSet()) {
        if(menu) {
          selectMenu->selectAllEditAction->setEnabled(false);
        }
        selectMenu->deselectEditAction->setEnabled(false);
      } else {
        auto& selection_manager = getSelectionManager();
        auto& text_selection_manager = getTextSelectionManager();

        if(menu) {
          if(text_selection_manager.selectionExists()) {
            selectMenu->selectAllEditAction->setEnabled(false);
          } else {
            selectMenu->selectAllEditAction->setEnabled(true);
          }
        }

        if(selection_manager.selectionExists() || text_selection_manager.selectionExists()) {
          selectMenu->deselectEditAction->setEnabled(true);
        } else {
          selectMenu->deselectEditAction->setEnabled(false);
        }
      }
    });

    QObject::connect(selectMenu->menu, &QMenu::aboutToHide, [selectMenu, menu] {
      selectMenu->resizeMenu->setEnabled(true);
      selectMenu->normalSelectionAction->setChecked(true);
      selectMenu->moveSelectionAction->setChecked(true);
      selectMenu->resizeSelectionAction->setChecked(true);
      if(menu) {
        selectMenu->selectAllEditAction->setEnabled(true);
      }
      selectMenu->deselectEditAction->setEnabled(true);
    });

    QObject::connect(selectMenu->normalSelectionAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet() &&
         (getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists())) {
        getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::NORMAL);
        getGUIEnvironment().getLeftToolbox().switchToDefaultSelectionTool();

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(selectMenu->moveSelectionAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet() &&
         (getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists())) {
        getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::MOVE);
        getGUIEnvironment().getLeftToolbox().switchToDefaultSelectionTool();

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(selectMenu->resizeSelectionAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet() &&
         (getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists())) {
        getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::RESIZE);
        getGUIEnvironment().getLeftToolbox().switchToDefaultSelectionTool();

        getImageEnvironment().getView().setFocus();
      }
    });

    if(menu) {
      QObject::connect(selectMenu->selectAllEditAction, &QAction::triggered, [](bool) {
        selection_helpers::tryFinalizeAllSelections(true);

        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        auto& text_selection_manager = getTextSelectionManager();
        if(image_env.isViewSet() && !text_selection_manager.selectionExists()) {
          auto& gui_env = getGUIEnvironment();
          gui_env.getLeftToolbox().switchToDefaultSelectionTool();

          auto& selection_manager = getSelectionManager();
          auto& current_layer = image_manager.getImage().getCurrentLayer();
          selection_manager.setSelectionRect(0,
                                             0,
                                             current_layer.getWidth(),
                                             current_layer.getHeight(),
                                             true,
                                             false);
          selection_manager.setSelectionLayer(current_layer.cut(0,
                                                                0,
                                                                current_layer.getWidth(),
                                                                current_layer.getHeight()));

          getPreviewManager().refreshAll();
          image_env.getScrollArea().updateSelectionWidget();
          image_env.getView().repaint();

          getImageEnvironment().getView().setFocus();
        }
      });
    }

    QObject::connect(selectMenu->deselectEditAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet()) {
        selection_helpers::tryFinalizeAllSelections(true);

        getImageEnvironment().getView().setFocus();
      }
    });
  }
}
