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
#ifndef GUIENVIRONMENT_H
#define GUIENVIRONMENT_H

#include <QDockWidget>
#include "../window/mainmenu.h"
#include "../window/statusbar.h"
#include "../pixpaint_macro.h"

namespace pixpaint
{
  class ColorToolbox;
  class ConsoleToolbox;
  class FrameToolbox;
  class LeftToolbox;
  class RightToolbox;

  class GUIEnvironment
  {
    GUIEnvironment();

  public:
    MainMenu& getMainMenu() noexcept;
    StatusBar& getStatusBar() noexcept;

    ColorToolbox& getColorToolbox() noexcept;

    LeftToolbox& getLeftToolbox() noexcept;
    QDockWidget& getLeftToolboxDock() noexcept;

    RightToolbox& getRightToolbox() noexcept;
    QDockWidget& getRightToolboxDock() noexcept;

    FrameToolbox& getFrameToolbox() noexcept;
    QDockWidget& getFrameToolboxDock() noexcept;

    ConsoleToolbox& getConsoleToolbox() noexcept;
    QDockWidget& getConsoleToolboxDock() noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(GUIEnvironment)
    friend class MainWindow;
    friend class DocumentPanel;

    MainMenu                m_mainMenu;
    StatusBar               m_statusBar;

    ColorToolbox*           m_colorToolbox;

    LeftToolbox*            m_leftToolbox;
    QDockWidget*            m_leftToolboxDock;

    RightToolbox*           m_rightToolbox;
    QDockWidget*            m_rightToolboxDock;

    FrameToolbox*           m_frameToolbox;
    QDockWidget*            m_frameToolboxDock;

    ConsoleToolbox*         m_consoleToolbox;
    QDockWidget*            m_consoleToolboxDock;
  };

  PIXPAINT_SINGLETON_FUNC(GUIEnvironment)
}

#endif // GUIENVIRONMENT_H
