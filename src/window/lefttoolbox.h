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
#ifndef LEFTTOOLBOX_H
#define LEFTTOOLBOX_H

#include <QPushButton>
#include <QWidget>
#include "../manager/eventmanager.h"
#include "painttooloptionframe.h"

namespace pixpaint
{
  class PaintToolManager;
  class PaintToolRegistrar;
  class StatusBar;
namespace gui_events
{
  struct TabAddEvent;
  struct TabRemoveEvent;
}
  class LeftToolbox : public QWidget,
                      EventListener<gui_events::TabAddEvent>,
                      EventListener<gui_events::TabRemoveEvent>
  {
    static constexpr auto PAINT_TOOL_BTN_SIZE = 40;

  public:
    LeftToolbox(QWidget* parent);

    void switchToDefaultTool();
    void switchToDefaultSelectionTool();
    PaintToolOptionFrame& getOptionFrame();

  private:
    void onEmit(const gui_events::TabAddEvent& event) override;
    void onEmit(const gui_events::TabRemoveEvent& event) override;

    QWidget* createToolbox();
    QWidget* createOptionFrame();

    PaintToolOptionFrame* m_optionFrame;
    QPushButton*          m_defaultTool;
    QPushButton*          m_defaultSelectionTool;
  };
}

#endif // LEFTTOOLBOX_H
