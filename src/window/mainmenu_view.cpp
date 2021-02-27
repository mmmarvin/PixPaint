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

#include "../dialog/boxgriddialog.h"
#include "../env/guienvironment.h"
#include "../env/imageenvironment.h"
#include "../manager/imagemanager.h"
#include "imageeditorview.h"

namespace pixpaint
{
  void popViewMenu(MainMenu::ViewMenu* viewMenu, QWidget* parent, bool)
  {
    viewMenu->leftToolbar = viewMenu->menu->addAction(QObject::tr("Toolbox"));
    viewMenu->rightToolbar = viewMenu->menu->addAction(QObject::tr("Layer Toolbar"));
    viewMenu->frameToolbar = viewMenu->menu->addAction(QObject::tr("Frames Toolbar"));
    viewMenu->consoleToolbar = viewMenu->menu->addAction(QObject::tr("Console"));
    viewMenu->menu->addSeparator();
    viewMenu->showGridAction = viewMenu->menu->addAction(QObject::tr("Show Pixel Grid"));
    viewMenu->boxGridAction = viewMenu->menu->addAction(QObject::tr("Box Grid"));
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
        viewMenu->boxGridAction->setEnabled(true);
        viewMenu->zoomInAction->setEnabled(true);
        viewMenu->zoomOutAction->setEnabled(true);
        viewMenu->showGridAction->setChecked(getImageEnvironment().getView().isGridShown());
      } else {
        viewMenu->showGridAction->setEnabled(false);
        viewMenu->boxGridAction->setEnabled(false);
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

    QObject::connect(viewMenu->boxGridAction, &QAction::triggered, [parent](bool) {
      BoxGridDialog dialog(parent);

      auto res = dialog.exec();
      if(res == QDialog::Accepted) {
        auto& image_env = getImageEnvironment();
        auto& view = image_env.getView();

        if(dialog.showGrid()) {
          view.showBoxGrid(true, dialog.getWidth(), dialog.getHeight());
        } else {
          view.showBoxGrid(false, view.getBoxGridSize().width, view.getBoxGridSize().height);
        }
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
}
