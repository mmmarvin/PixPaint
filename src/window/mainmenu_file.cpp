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

#include "../dialog/newimagedialog.h"
#include "../env/imageenvironment.h"
#include "../assert.h"
#include "mainwindow.h"

namespace pixpaint
{
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
}
