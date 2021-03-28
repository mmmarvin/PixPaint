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

#include <QMessageBox>
#include "../dialog/aboutdialog.h"
#include "../http/updater.h"

namespace pixpaint
{
  void popHelpMenu(MainMenu::HelpMenu* helpMenu, QWidget* parent, bool)
  {
    helpMenu->checkForUpdatesAction = helpMenu->menu->addAction(QObject::tr("Check for updates..."));
    helpMenu->menu->addSeparator();
    helpMenu->aboutHelpAction = helpMenu->menu->addAction(QObject::tr("About"));

    QObject::connect(helpMenu->checkForUpdatesAction, &QAction::triggered, [parent](bool) {
      auto res = getUpdater().updateAvailable();
      switch(std::get<0>(res)) {
      case 0:
        QMessageBox::information(parent,
                                 "Update",
                                 "PixPaint is up-to-date!");
        break;
      case 1:
      {
        auto res = QMessageBox::information(parent,
                                            "Update",
                                            "There is a new version available! Do you want to update?",
                                            QMessageBox::StandardButton::Yes |
                                            QMessageBox::StandardButton::No,
                                            QMessageBox::StandardButton::No);
        if(res == QMessageBox::StandardButton::Yes) {
          parent->close();
        }
        break;
      }
      case -1:
        QMessageBox::critical(parent,
                              "Update",
                              "Cannot check for a new version!");
        break;
      default:
        break;
      }
    });

    QObject::connect(helpMenu->aboutHelpAction, &QAction::triggered, [parent](bool) {
      AboutDialog dialog(parent);
      dialog.exec();
    });
  }
}
