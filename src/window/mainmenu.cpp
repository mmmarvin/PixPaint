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

#include <unordered_map>
#include <clip/clip.h>
#include "../3rdparty/gengine/string_utility.h"
#include "../dialog/effectoptiondialog.h"
#include "../dialog/aboutdialog.h"
#include "../dialog/newimagedialog.h"
#include "../dialog/rotateimagedialog.h"
#include "../embed/script_utility.h"
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../event/gui/image_events.h"
#include "../event/gui/history_events.h"
#include "../image/image.h"
#include "../helper/image_helpers.h"
#include "../helper/selection_helpers.h"
#include "../history/animationresizeaction.h"
#include "../history/selectioncutaction.h"
#include "../manager/documentmanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textselectionmanager.h"
#include "../registrar/effectregistrar.h"
#include "../utility/qt_utility.h"
#include "../window/selectionwidget.h"
#include "../assert.h"
#include "../debug_log.h"
#include "../define.h"
#include "../manager/previewmanager.h"
#include "documentpanel.h"
#include "frametoolbox.h"
#include "imageeditorview.h"
#include "lefttoolbox.h"
#include "mainwindow.h"
#include "righttoolbox.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
namespace
{
  void applyImageEffectImpl(std::string effectName,
                            std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                            std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    image_helpers::applyEffect(std::move(effectName), image_manager.getImage(), layerEffect, selectionEffect);

    emitEvent(gui_events::ImageEffectEvent());

    image_manager.refresh();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getPreviewManager().refreshResizeAll();
  }

  void applyLayerEffectImpl(std::string effectName,
                            std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                            std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto current_layer_index = image_manager.getImage().getCurrentLayerIndex();
    image_helpers::applyEffect(std::move(effectName), image_manager.getImage().getLayer(current_layer_index), layerEffect, selectionEffect);

    emitEvent(gui_events::ImageLayerEffectEvent{ current_layer_index });

    image_manager.refresh();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getPreviewManager().refreshResizeAll();
  }

  void applyImageEffect(std::string effectName,
                        std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                        std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    if(image_env.isViewSet()) {
      applyImageEffectImpl(std::move(effectName), std::move(layerEffect), std::move(selectionEffect));
    }
  }

//  void applyLayerEffect(std::string effectName,
//                        std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
//                        std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
//  {
//    auto& image_env = getImageEnvironment();
//    if(image_env.isViewSet()) {
//      applyLayerEffectImpl(std::move(effectName), std::move(layerEffect), std::move(selectionEffect));
//    }
//  }

  void popResizeSubMenu(QMenu* mainMenu,
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
  void popFileMenu(MainMenu::FileMenu* fileMenu,
                   QWidget* parent,
                   bool)
  {
    PIXPAINT_ASSERT(dynamic_cast<MainWindow*>(parent) != nullptr, "Parent must be MainWindow for popFileMneu");
    auto* main_window = static_cast<MainWindow*>(parent);

    fileMenu->newFileAction = fileMenu->menu->addAction(QObject::tr("New"));
    fileMenu->menu->addSeparator();
    fileMenu->openFileAction = fileMenu->menu->addAction(QObject::tr("Open..."));
    fileMenu->openRecentMenu = fileMenu->menu->addMenu(QObject::tr("Open recent..."));
    fileMenu->saveFileAction = fileMenu->menu->addAction(QObject::tr("Save"));
    fileMenu->saveAsFileAction = fileMenu->menu->addAction(QObject::tr("Save As..."));
    fileMenu->menu->addSeparator();
    fileMenu->importMenu = fileMenu->menu->addMenu(QObject::tr("Import..."));
    fileMenu->exportMenu = fileMenu->menu->addMenu(QObject::tr("Export..."));
    fileMenu->importImageFileAction = fileMenu->importMenu->addAction(QObject::tr("Image..."));
    fileMenu->exportImageFileAction = fileMenu->exportMenu->addAction(QObject::tr("Image..."));
    fileMenu->importAnimationFileAction = fileMenu->importMenu->addAction(QObject::tr("Animation..."));
    fileMenu->exportAnimationFileAction = fileMenu->exportMenu->addAction(QObject::tr("Animation..."));
    fileMenu->menu->addSeparator();
    fileMenu->closeFileAction = fileMenu->menu->addAction(QObject::tr("Close"));
    fileMenu->closeAllFileAction = fileMenu->menu->addAction(QObject::tr("Close All..."));
    fileMenu->menu->addSeparator();
    fileMenu->exitFileAction = fileMenu->menu->addAction(QObject::tr("Exit"));

    QObject::connect(fileMenu->menu, &QMenu::aboutToShow, [fileMenu] {
      if(getImageEnvironment().isViewSet()) {
        if(getImageManager().getImage().isModified()) {
          fileMenu->saveFileAction->setEnabled(true);
        } else {
          fileMenu->saveFileAction->setEnabled(false);
        }
        fileMenu->saveAsFileAction->setEnabled(true);
        fileMenu->exportMenu->setEnabled(true);
//        fileMenu->exportImageFileAction->setEnabled(true);
        fileMenu->closeFileAction->setEnabled(true);
        fileMenu->closeAllFileAction->setEnabled(true);
      } else {
        fileMenu->saveFileAction->setEnabled(false);
        fileMenu->saveAsFileAction->setEnabled(false);
        fileMenu->exportMenu->setEnabled(false);
//        fileMenu->exportImageFileAction->setEnabled(false);
        fileMenu->closeFileAction->setEnabled(false);
        fileMenu->closeAllFileAction->setEnabled(false);
      }
    });

    QObject::connect(fileMenu->menu, &QMenu::aboutToHide, [fileMenu] {
      fileMenu->saveFileAction->setEnabled(true);
      fileMenu->saveAsFileAction->setEnabled(true);
      fileMenu->exportImageFileAction->setEnabled(true);
      fileMenu->closeFileAction->setEnabled(true);
      fileMenu->closeAllFileAction->setEnabled(true);
    });

    QObject::connect(fileMenu->newFileAction, &QAction::triggered, [main_window](bool) {
      auto image_width = main_window->m_lastImageWidth;
      auto image_height = main_window->m_lastImageWidth;
      image_width = image_width <= 0 ? DEFAULT_IMAGE_WIDTH : image_width;
      image_height = image_height <= 0 ? DEFAULT_IMAGE_HEIGHT : image_height;
      NewImageDialog newDialog(main_window, image_width, image_height);
      int res = newDialog.exec();
      if(res == QDialog::Accepted) {
        auto width = newDialog.getWidth();
        auto height = newDialog.getHeight();
        main_window->m_documentPanel->addPage(Document(width, height));
        main_window->m_lastImageWidth = width;
        main_window->m_lastImageHeight = height;
      }
    });
    QObject::connect(fileMenu->openFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotOpenFile);
    QObject::connect(fileMenu->saveFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotSaveFile);
    QObject::connect(fileMenu->saveAsFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotSaveAsFile);
    QObject::connect(fileMenu->importAnimationFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotImportAnimationFile);
    QObject::connect(fileMenu->exportAnimationFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotExportAnimationFile);
    QObject::connect(fileMenu->importImageFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotImportImageFile);
    QObject::connect(fileMenu->exportImageFileAction,
                     &QAction::triggered,
                     main_window,
                     &MainWindow::slotExportImageFile);
    QObject::connect(fileMenu->closeFileAction,
                  &QAction::triggered,
                  [main_window](bool) {
      auto index = main_window->m_documentPanel->currentIndex();
      if(index != -1) {
        main_window->m_documentPanel->closePage(index);
      }
    });
    QObject::connect(fileMenu->closeAllFileAction,
                  &QAction::triggered,
                  main_window,
                  &MainWindow::slotCloseAll);
    QObject::connect(fileMenu->exitFileAction,
                  &QAction::triggered,
                  [parent](bool) {
      parent->close();
    });
  }

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

          if(res & HistoryAction::EAR_REFRESH_FRAME_LIST) {
            emitEvent(gui_events::HistoryRefreshFrameEvent());
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

          if(res & HistoryAction::EAR_REFRESH_FRAME_LIST) {
            emitEvent(gui_events::HistoryRefreshFrameEvent());
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
      selection_helpers::tryCopy();
    });
    QObject::connect(editMenu->cutEditAction, &QAction::triggered, [](bool){
      selection_helpers::tryCut(true);
    });
    QObject::connect(editMenu->pasteEditAction, &QAction::triggered, [](bool) {
      selection_helpers::tryPaste(true);
    });
    QObject::connect(editMenu->deleteEditAction, &QAction::triggered, [](bool) {
      auto& selection_manager = getSelectionManager();
      if(selection_manager.layerExists() && selection_manager.selectionExists()) {
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
      }
    });
  }

  void popViewMenu(MainMenu::ViewMenu* viewMenu, QWidget* parent, bool)
  {
    viewMenu->leftToolbar = viewMenu->menu->addAction(QObject::tr("Toolbox"));
    viewMenu->rightToolbar = viewMenu->menu->addAction(QObject::tr("Layer Toolbar"));
    viewMenu->frameToolbar = viewMenu->menu->addAction(QObject::tr("Frames Toolbar"));
    viewMenu->consoleToolbar = viewMenu->menu->addAction(QObject::tr("Console"));
    viewMenu->menu->addSeparator();
    viewMenu->showGridAction = viewMenu->menu->addAction(QObject::tr("Show Grid"));
    viewMenu->menu->addSeparator();
    viewMenu->zoomInAction = viewMenu->menu->addAction(QObject::tr("Zoom In"));
    viewMenu->zoomOutAction = viewMenu->menu->addAction(QObject::tr("Zoom Out"));

    viewMenu->showGridAction->setCheckable(true);
    viewMenu->leftToolbar->setCheckable(true);
    viewMenu->rightToolbar->setCheckable(true);
    viewMenu->frameToolbar->setCheckable(true);
    viewMenu->consoleToolbar->setCheckable(true);
    QObject::connect(viewMenu->menu, &QMenu::aboutToShow, [viewMenu, parent]{
      if(getImageEnvironment().isViewSet()) {
        viewMenu->showGridAction->setEnabled(true);
        viewMenu->zoomInAction->setEnabled(true);
        viewMenu->zoomOutAction->setEnabled(true);
        viewMenu->showGridAction->setChecked(getImageEnvironment().getView().isGridShown());
      } else {
        viewMenu->showGridAction->setEnabled(false);
        viewMenu->zoomInAction->setEnabled(false);
        viewMenu->zoomOutAction->setEnabled(false);
      }

      auto& guiEnv = getGUIEnvironment();
      viewMenu->leftToolbar->setChecked(guiEnv.getLeftToolboxDock().isVisible());
      viewMenu->rightToolbar->setChecked(guiEnv.getRightToolboxDock().isVisible());
      viewMenu->frameToolbar->setChecked(guiEnv.getFrameToolboxDock().isVisible());
      viewMenu->consoleToolbar->setChecked(guiEnv.getConsoleToolboxDock().isVisible());
    });

    QObject::connect(viewMenu->menu, &QMenu::aboutToHide, [viewMenu]{
      viewMenu->showGridAction->setEnabled(true);
      viewMenu->zoomInAction->setEnabled(true);
      viewMenu->zoomOutAction->setEnabled(true);
    });

    QObject::connect(viewMenu->leftToolbar, &QAction::triggered, [viewMenu](bool) {
      auto& gui_env = getGUIEnvironment();

      auto visible = gui_env.getLeftToolboxDock().isVisible();
      gui_env.getLeftToolboxDock().setVisible(!visible);
      viewMenu->leftToolbar->setChecked(!visible);
    });

    QObject::connect(viewMenu->rightToolbar, &QAction::triggered, [viewMenu](bool) {
      auto& gui_env = getGUIEnvironment();

      auto visible = gui_env.getRightToolboxDock().isVisible();
      gui_env.getRightToolboxDock().setVisible(!visible);
      viewMenu->rightToolbar->setChecked(!visible);
    });

    QObject::connect(viewMenu->frameToolbar, &QAction::triggered, [viewMenu](bool) {
      auto& gui_env = getGUIEnvironment();

      auto visible = gui_env.getFrameToolboxDock().isVisible();
      gui_env.getFrameToolboxDock().setVisible(!visible);
      viewMenu->frameToolbar->setChecked(!visible);
    });

    QObject::connect(viewMenu->consoleToolbar, &QAction::triggered, [viewMenu](bool) {
      auto& gui_env = getGUIEnvironment();

      auto visible = gui_env.getConsoleToolboxDock().isVisible();
      gui_env.getConsoleToolboxDock().setVisible(!visible);
      viewMenu->consoleToolbar->setChecked(!visible);
    });

    QObject::connect(viewMenu->showGridAction, &QAction::triggered, [viewMenu](bool) {
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        image_env.getView().showGrid(!image_env.getView().isGridShown());
        image_manager.refresh();
      }
    });

    QObject::connect(viewMenu->zoomInAction, &QAction::triggered, [](bool) {
      auto& image_env = getImageEnvironment();
      if(image_env.isViewSet()) {
        image_env.getView().zoomIn();
      }
    });
    QObject::connect(viewMenu->zoomOutAction, &QAction::triggered, [](bool) {
      auto& image_env = getImageEnvironment();
      if(image_env.isViewSet()) {
        image_env.getView().zoomOut();
      }
    });
  }

  void popImageMenu(MainMenu::ImageMenu* imageMenu, QWidget* parent, bool)
  {
    imageMenu->resizeImageAction = imageMenu->menu->addAction(QObject::tr("Resize..."));
    imageMenu->menu->addSeparator();
    imageMenu->flipImageAction = imageMenu->menu->addAction(QObject::tr("Flip"));
    imageMenu->mirrorImageAction = imageMenu->menu->addAction(QObject::tr("Mirror"));
    imageMenu->rotateImageAction = imageMenu->menu->addAction(QObject::tr("Rotate"));
    imageMenu->menu->addSeparator();
    imageMenu->flattenImageAction = imageMenu->menu->addAction(QObject::tr("Flatten Image"));
    imageMenu->menu->addSeparator();
    imageMenu->invertImageAction = imageMenu->menu->addAction(QObject::tr("Invert"));
    imageMenu->clearImageAction = imageMenu->menu->addAction(QObject::tr("Clear"));

    QObject::connect(imageMenu->menu, &QMenu::aboutToShow, [imageMenu] {
      if(getImageEnvironment().isViewSet()) {
        imageMenu->resizeImageAction->setEnabled(true);
        imageMenu->flipImageAction->setEnabled(true);
        imageMenu->mirrorImageAction->setEnabled(true);
        imageMenu->rotateImageAction->setEnabled(true);
        if(getImageManager().getImage().getLayerCount() > 1) {
          imageMenu->flattenImageAction->setEnabled(true);
        } else {
          imageMenu->flattenImageAction->setEnabled(false);
        }
        imageMenu->invertImageAction->setEnabled(true);
        imageMenu->clearImageAction->setEnabled(true);
      } else {
        imageMenu->resizeImageAction->setEnabled(false);
        imageMenu->flipImageAction->setEnabled(false);
        imageMenu->mirrorImageAction->setEnabled(false);
        imageMenu->rotateImageAction->setEnabled(false);
        imageMenu->flattenImageAction->setEnabled(false);
        imageMenu->invertImageAction->setEnabled(false);
        imageMenu->clearImageAction->setEnabled(false);
      }
    });
    QObject::connect(imageMenu->menu, &QMenu::aboutToHide, [imageMenu] {
      imageMenu->resizeImageAction->setEnabled(true);
      imageMenu->flipImageAction->setEnabled(true);
      imageMenu->mirrorImageAction->setEnabled(true);
      imageMenu->rotateImageAction->setEnabled(true);
      imageMenu->flattenImageAction->setEnabled(true);
      imageMenu->invertImageAction->setEnabled(true);
      imageMenu->clearImageAction->setEnabled(true);
    });

    QObject::connect(imageMenu->resizeImageAction, &QAction::triggered, [parent](bool){
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        auto old_width = image_manager.getImage().getWidth();
        auto old_height = image_manager.getImage().getHeight();

        NewImageDialog newDialog(parent, old_width, old_height, true);
        newDialog.setWindowTitle(QObject::tr("Resize Image"));

        int res = newDialog.exec();
        if(res == QDialog::Accepted) {
          selection_helpers::tryFinalizeAllSelections(true);

          emitHistoryAction(AnimationResizeAction(getDocumentManager().getDocument().getAnimation()));

          auto width = newDialog.getWidth();
          auto height = newDialog.getHeight();
          auto resize_mode = newDialog.getResizeMode();

          auto& animation = getDocumentManager().getDocument().getAnimation();
          for(std::size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
            animation.getFrame(i).resize(width, height, Color::WHITE, resize_mode);
          }
          image_env.getView().updateSize();
          image_env.getView().updateDrawers();

          emitEvent(gui_events::ImageResizeEvent{ Point(old_width, old_height), Point(width, height) });

          image_manager.refresh();
          image_env.getScrollArea().updateSelectionWidget();
          image_env.getScrollArea().updateResizeHandles();

          getPreviewManager().refreshResizeAll();
        }
      }
    });

    QObject::connect(imageMenu->flipImageAction, &QAction::triggered, [parent](bool) {
      if(getImageEnvironment().isViewSet()) {
        applyImageEffect("Flip Image",
        [](const IntRect&, MaskablePixelData& layer)
        {
          layer = layer.flipVertically();
        },
        [](const IntRect&, MaskablePixelData& selectionLayer)
        {
          selectionLayer = selectionLayer.flipVertically();
        });
      }
    });
    QObject::connect(imageMenu->mirrorImageAction, &QAction::triggered, [parent](bool) {
      if(getImageEnvironment().isViewSet()) {
        applyImageEffect("Mirror Image",
        [](const IntRect&, MaskablePixelData& layer)
        {
          layer = layer.flipHorizontally();
        },
        [](const IntRect&, MaskablePixelData& selectionLayer)
        {
          selectionLayer = selectionLayer.flipHorizontally();
        });
      }
    });

    QObject::connect(imageMenu->rotateImageAction, &QAction::triggered, [parent](bool) {
      auto& image_env = getImageEnvironment();
      if(image_env.isViewSet()) {
        RotateImageDialog rotateDialog(parent);
        int res = rotateDialog.exec();
        if(res == QDialog::Accepted) {
          auto direction = rotateDialog.getRotationDirection();
          auto angle = rotateDialog.getRotation();

          applyImageEffect("Rotate Image",
          [direction, angle](const IntRect& , MaskablePixelData& layer)
          {
            layer.rotate(angle, direction);
          },
          [direction, angle](const IntRect& selection_rect, MaskablePixelData& selection_layer)
          {
            selection_layer.rotate(angle, direction);

            auto selectionRectCenter = center(selection_rect);
            getSelectionManager().setSelectionRectCenter(selectionRectCenter.x,
                                                         selectionRectCenter.y,
                                                         selection_layer.getWidth(),
                                                         selection_layer.getHeight(),
                                                         true,
                                                         false);
          });
        }
      }
    });

    QObject::connect(imageMenu->flattenImageAction, &QAction::triggered, [parent](bool){
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        image_manager.getImage().flattenLayers(true);

        image_manager.getImage().setCurrentLayerIndex(0);
        image_env.getView().updateDrawers();

        emitEvent(gui_events::ImageFlattenEvent());
        getPreviewManager().refreshAll();
      }
    });

    QObject::connect(imageMenu->invertImageAction, &QAction::triggered, [parent](bool) {
      applyImageEffect("Invert Image",
      [](const IntRect& , MaskablePixelData& layer)
      {
        layer.invert();
      },
      [](const IntRect& , MaskablePixelData& selectionLayer)
      {
        selectionLayer.invert();
      });
    });

    QObject::connect(imageMenu->clearImageAction, &QAction::triggered, [](bool) {
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        auto& image = image_manager.getImage();
        image = Image(image.getWidth(), image.getHeight());
        image.getLayer(0).clear(Color::WHITE);

        image.setCurrentLayerIndex(0);
        image_env.getView().updateDrawers();

        emitEvent(gui_events::ImageClearEvent());
        getPreviewManager().refreshAll();
      }
    });
  }

  void popSelectMenu(MainMenu::SelectMenu* selectMenu, QWidget*, bool menu)
  {
    popResizeSubMenu(selectMenu->menu,
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
      }
    });

    QObject::connect(selectMenu->moveSelectionAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet() &&
         (getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists())) {
        getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::MOVE);
        getGUIEnvironment().getLeftToolbox().switchToDefaultSelectionTool();
      }
    });

    QObject::connect(selectMenu->resizeSelectionAction, &QAction::triggered, [](bool) {
      if(getImageEnvironment().isViewSet() &&
         (getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists())) {
        getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::RESIZE);
        getGUIEnvironment().getLeftToolbox().switchToDefaultSelectionTool();
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
        }
      });
    }

    QObject::connect(selectMenu->deselectEditAction, &QAction::triggered, [](bool) {
      selection_helpers::tryFinalizeAllSelections(true);
    });
  }

  void popEffectMenu(MainMenu::EffectMenu* effectMenu, QWidget* parent, bool)
  {
    auto effectActionAdder = [](MainMenu::EffectMenu* effect_menu,
                                std::unordered_map<std::string, QMenu*>& menu_lists,
                                EffectInformation& effect_information)
    {
      auto category = gengine2d::string_utils::toLower(effect_information.getCategory());
      if(!category.empty()) {
        category[0] = static_cast<char>(toupper(static_cast<unsigned char>(category[0])));

        auto find_it = menu_lists.find(category);
        if(find_it == menu_lists.end()) {
          auto* category_menu = effect_menu->menu->addMenu(category.c_str());
          menu_lists.emplace(category, category_menu);

          return category_menu->addAction(effect_information.getName().c_str());
        } else {
          return find_it->second->addAction(effect_information.getName().c_str());
        }
      }

      return effect_menu->menu->addAction(effect_information.getName().c_str());
    };

    auto effectMenuAdder = [parent, effectActionAdder](MainMenu::EffectMenu* effect_menu,
                                                       std::unordered_map<std::string, QMenu*>& menu_lists,
                                                       EffectInformation& effect_information) {
      QAction* effect_action = effectActionAdder(effect_menu, menu_lists, effect_information);
      QObject::connect(effect_action, &QAction::triggered, [parent, &effect_information](bool){
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        if(image_env.isViewSet()) {
          effect_information.getEffect().reset();
          if(!effect_information.getEffect().getOptions().empty()) {
            EffectOptionDialog dialog(parent, effect_information);
            auto res = dialog.exec();
            if(res == QDialog::Accepted) {
              auto target = dialog.getTarget();
              if(target == EffectOptionDialog::EEffectTarget::IMAGE ||
                 target == EffectOptionDialog::EEffectTarget::SELECTION) {
                applyImageEffectImpl(effect_information.getName(),
                [&effect_information](const IntRect& rect, MaskablePixelData& layer)
                {
                  effect_information.getEffect().applyLayerEffect(rect, layer);
                },
                [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
                {
                  effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
                });
              } else if(target == EffectOptionDialog::EEffectTarget::LAYER) {
                applyLayerEffectImpl(effect_information.getName(),
                [&effect_information](const IntRect& rect, MaskablePixelData& layer)
                {
                  effect_information.getEffect().applyLayerEffect(rect, layer);
                },
                [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
                {
                  effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
                });
              }
            }
          } else {
            auto& image = image_manager.getImage();
            for(std::size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
              applyImageEffectImpl(effect_information.getName(),
              [&effect_information](const IntRect& rect, MaskablePixelData& layer)
              {
                effect_information.getEffect().applyLayerEffect(rect, layer);
              },
              [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
              {
                effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
              });
            }
          }
        }
      });

      effect_menu->effectsActionList.push_back(effect_action);
    };

    QObject::connect(effectMenu->menu, &QMenu::aboutToShow, [effectMenu] {
      if(getImageEnvironment().isViewSet()) {
        for(auto* action : effectMenu->effectsActionList) {
          action->setEnabled(true);
        }
      } else {
        for(auto* action : effectMenu->effectsActionList) {
          action->setEnabled(false);
        }
      }
    });
    QObject::connect(effectMenu->menu, &QMenu::aboutToHide, [effectMenu] {
      for(auto* action : effectMenu->effectsActionList) {
        action->setEnabled(true);
      }
    });

    std::vector<EffectInformation*> categorized_effects;
    std::vector<EffectInformation*> nonCategorized_effects;

    // sort the effects by category first
    for(auto& effect_information : getEffectRegistrar()) {
      if(!effect_information.getCategory().empty()) {
        categorized_effects.push_back(&effect_information);
      } else {
        nonCategorized_effects.push_back(&effect_information);
      }
    }

    std::unordered_map<std::string, QMenu*> menuLists;
    for(auto& effect_information : categorized_effects) {
      effectMenuAdder(effectMenu, menuLists, *effect_information);
    }

    for(auto& effect_information : nonCategorized_effects) {
      effectMenuAdder(effectMenu, menuLists, *effect_information);
    }
  }

  void popHelpMenu(MainMenu::HelpMenu* helpMenu, QWidget* parent, bool)
  {
    helpMenu->aboutHelpAction = helpMenu->menu->addAction(QObject::tr("About..."));
    QObject::connect(helpMenu->aboutHelpAction, &QAction::triggered, [parent](bool) {
      AboutDialog dialog(parent);
      dialog.exec();
    });
  }
}
