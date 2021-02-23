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
#include <QApplication>
#include <iostream>
#include "3rdparty/gengine/configuration.h"
#include "utility/settings_file_utility.h"
#include "window/globalkeyreader.h"
#include "window/mainwindow.h"
#include "define.h"

void initializeThreadPool();
void initializeKeyboardShortcuts();
void saveConfigurations();

int main(int argc, char** argv)
{
  int fargc = 1; std::array<const char*, 1> fargv = { argv[0] };
  QApplication app(fargc, const_cast<char**>(fargv.data()));

  initializeThreadPool();
  initializeKeyboardShortcuts();
  auto* mainWindow = new pixpaint::MainWindow(pixpaint::getRunParam(argc, argv));
  mainWindow->show();

  QObject::connect(&app, &QApplication::aboutToQuit, mainWindow, &pixpaint::MainWindow::exit);
  app.installEventFilter(new pixpaint::GlobalKeyReader);
  return app.exec();
}

void initializeThreadPool()
{
//  volatile auto& threadPool = pixpaint::getThreadPool();
//  threadPool;
}

void initializeKeyboardShortcuts()
{
  auto& configurationManager = gengine2d::getConfigurationManager();

  // add keyboard shortcut variables
  // file menu
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "new", "ctrl+n", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "open", "ctrl+o", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "save", "ctrl+s", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "saveas", "ctrl+shift+s", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "close", "ctrl+q", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "closeall", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "exit", "", gengine2d::EVariableType::String);

  // edit menu
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "undo", "ctrl+z", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "redo", "ctrl+r", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "cut", "ctrl+x", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "copy", "ctrl+c", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "paste", "ctrl+v", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "delete", "del", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "preference", "", gengine2d::EVariableType::String);

  // select menu
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "normalselectionmode", "alt+n", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "moveselectionmode", "alt+m", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "resizeselectionmode", "alt+r", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "selectall", "ctrl+a", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "deselect", "ctrl+d", gengine2d::EVariableType::String);

  // image menu
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "resize", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "flip", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "mirror", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "rotate", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "invert", "", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "clear", "", gengine2d::EVariableType::String);

  // misc.
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "previousframe", "A", gengine2d::EVariableType::String);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_KEYBOARD_SHORTCUT, "nextframe", "S", gengine2d::EVariableType::String);

  // add program configurations
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_SETTINGS, "image_width", "64", gengine2d::EVariableType::Integer);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_SETTINGS, "image_height", "64", gengine2d::EVariableType::Integer);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_SETTINGS, "undo_max", "10", gengine2d::EVariableType::Integer);
  configurationManager.addVariable(pixpaint::CONFIG_SECTION_SETTINGS, "redo_max", "10", gengine2d::EVariableType::Integer);

  // add recent files
  for(int i = 0; i < pixpaint::NUMBER_OF_RECENT_FILES; ++i) {
    configurationManager.addVariable(pixpaint::CONFIG_SECTION_RECENT_FILES, std::string("File") + std::to_string(i + 1), "", gengine2d::EVariableType::String);
  }

  // load from settings.cfg
  configurationManager.loadFromFile(pixpaint::settings_file_utils::getSettingsFilename());
}
