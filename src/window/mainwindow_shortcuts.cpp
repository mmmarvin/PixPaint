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
#include "mainwindow.h"

#include "../3rdparty/gengine/configuration.h"
#include "../env/guienvironment.h"
#include "../utility/qt_utility.h"
#include "../define.h"
#include "mainmenu.h"

namespace pixpaint
{
  void MainWindow::createActionShortcuts()
  {
    auto& gui_env = getGUIEnvironment();
    const auto& configuration_manager = gengine2d::getConfigurationManager();
    auto& main_menu = gui_env.getMainMenu();

    main_menu.fileMenu.newFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "new")));
    main_menu.fileMenu.openFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "open")));
    main_menu.fileMenu.saveFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "save")));
    main_menu.fileMenu.saveAsFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "saveas")));
    main_menu.fileMenu.closeFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "close")));
    main_menu.fileMenu.closeAllFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "closeall")));
    main_menu.fileMenu.exitFileAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "exit")));

    main_menu.editMenu.undoEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "undo")));
    main_menu.editMenu.redoEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "redo")));
    main_menu.editMenu.copyEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "copy")));
    main_menu.editMenu.cutEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "cut")));
    main_menu.editMenu.pasteEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "paste")));
    main_menu.editMenu.deleteEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "delete")));

    main_menu.selectMenu.normalSelectionAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "normalselectionmode")));
    main_menu.selectMenu.moveSelectionAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "moveselectionmode")));
    main_menu.selectMenu.resizeSelectionAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "resizeselectionmode")));
    main_menu.selectMenu.selectAllEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "selectall")));
    main_menu.selectMenu.deselectEditAction->setShortcut(qt_utils::stringToKeySequence(*configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "deselect")));
  }
}
