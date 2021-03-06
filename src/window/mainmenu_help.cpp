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

#include "../dialog/aboutdialog.h"

namespace pixpaint
{
  void popHelpMenu(MainMenu::HelpMenu* helpMenu, QWidget* parent, bool)
  {
    helpMenu->aboutHelpAction = helpMenu->menu->addAction(QObject::tr("About..."));
    QObject::connect(helpMenu->aboutHelpAction, &QAction::triggered, [parent](bool) {
      AboutDialog dialog(parent);
      dialog.exec();
    });
  }
}
