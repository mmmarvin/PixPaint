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
#include "guienvironment.h"

namespace pixpaint
{
  GUIEnvironment::GUIEnvironment() :
    m_colorToolbox(nullptr),
    m_leftToolbox(nullptr),
    m_leftToolboxDock(nullptr),
    m_rightToolbox(nullptr),
    m_rightToolboxDock(nullptr),
    m_frameToolbox(nullptr),
    m_frameToolboxDock(nullptr),
    m_consoleToolbox(nullptr),
    m_consoleToolboxDock(nullptr)
  {
  }

  MainMenu& GUIEnvironment::getMainMenu() noexcept
  {
    return m_mainMenu;
  }

  StatusBar& GUIEnvironment::getStatusBar() noexcept
  {
    return m_statusBar;
  }

  ColorToolbox& GUIEnvironment::getColorToolbox() noexcept
  {
    return *m_colorToolbox;
  }

  LeftToolbox& GUIEnvironment::getLeftToolbox() noexcept
  {
    return *m_leftToolbox;
  }

  QDockWidget& GUIEnvironment::getLeftToolboxDock() noexcept
  {
    return *m_leftToolboxDock;
  }

  RightToolbox& GUIEnvironment::getRightToolbox() noexcept
  {
    return *m_rightToolbox;
  }

  QDockWidget& GUIEnvironment::getRightToolboxDock() noexcept
  {
    return *m_rightToolboxDock;
  }

  FrameToolbox& GUIEnvironment::getFrameToolbox() noexcept
  {
    return *m_frameToolbox;
  }

  QDockWidget& GUIEnvironment::getFrameToolboxDock() noexcept
  {
    return *m_frameToolboxDock;
  }

  ConsoleToolbox& GUIEnvironment::getConsoleToolbox() noexcept
  {
    return *m_consoleToolbox;
  }

  QDockWidget& GUIEnvironment::getConsoleToolboxDock() noexcept
  {
    return *m_consoleToolboxDock;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(GUIEnvironment)
}
