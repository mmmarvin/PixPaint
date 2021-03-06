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
#include "../utility/settings_file_utility.h"
#include "../assert.h"
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

  void MainWindow::initializeConfigValues()
  {
    auto& configurationManager = gengine2d::getConfigurationManager();

    // add keyboard shortcut variables
    // file menu
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "new", "ctrl+n", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "open", "ctrl+o", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "save", "ctrl+s", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "saveas", "ctrl+shift+s", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "close", "ctrl+q", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "closeall", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "exit", "", gengine2d::EVariableType::String);

    // edit menu
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "undo", "ctrl+z", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "redo", "ctrl+r", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "cut", "ctrl+x", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "copy", "ctrl+c", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "paste", "ctrl+v", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "delete", "del", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "preference", "", gengine2d::EVariableType::String);

    // select menu
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "normalselectionmode", "alt+n", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "moveselectionmode", "alt+m", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "resizeselectionmode", "alt+r", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "selectall", "ctrl+a", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "deselect", "ctrl+d", gengine2d::EVariableType::String);

    // image menu
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "resize", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "flip", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "mirror", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "rotate", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "invert", "", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "clear", "", gengine2d::EVariableType::String);

    // misc.
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "previousframe", "Z", gengine2d::EVariableType::String);
    configurationManager.addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT, "nextframe", "X", gengine2d::EVariableType::String);

    // add program configurations
    configurationManager.addVariable(CONFIG_SECTION_SETTINGS, "image_width", "64", gengine2d::EVariableType::Integer);
    configurationManager.addVariable(CONFIG_SECTION_SETTINGS, "image_height", "64", gengine2d::EVariableType::Integer);
    configurationManager.addVariable(CONFIG_SECTION_SETTINGS, "undo_max", "10", gengine2d::EVariableType::Integer);
    configurationManager.addVariable(CONFIG_SECTION_SETTINGS, "redo_max", "10", gengine2d::EVariableType::Integer);
    configurationManager.addVariable(CONFIG_SECTION_SETTINGS, "last_location", ".", gengine2d::EVariableType::String);

    // add recent files
    for(int i = 0; i < NUMBER_OF_RECENT_FILES; ++i) {
      configurationManager.addVariable(CONFIG_SECTION_RECENT_FILES, std::string("File") + std::to_string(i + 1), "", gengine2d::EVariableType::String);
    }
  }

  void MainWindow::loadConfigValues()
  {
    // load from settings.cfg
    auto& configuration_manager = gengine2d::getConfigurationManager();
    configuration_manager.loadFromFile(settings_file_utils::getSettingsFilename());

    // set the loaded shortcut to the tools
    for(auto& paint_tool : getPaintToolRegistrar()) {
      auto shortcut_id = "tool_shortcut_" + paint_tool.getID();
      auto shortcut = configuration_manager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, shortcut_id);
      PIXPAINT_ASSERT(shortcut, "Shortcut does not exist!");

      paint_tool.setShortcut(*shortcut);
    }
  }
}
