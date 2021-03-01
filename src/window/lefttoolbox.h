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
#include "../pattern/observer.h"
#include "painttooloptionframe.h"

class FlowLayout;
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
                      public patterns::Observer,
                      EventListener<gui_events::TabAddEvent>,
                      EventListener<gui_events::TabRemoveEvent>
  {
  public:
    LeftToolbox(QWidget* parent);

    void switchToDefaultTool();
    void switchToDefaultSelectionTool();
    PaintToolOptionFrame& getOptionFrame();

  private:
    void onEmit(const gui_events::TabAddEvent& event) override;
    void onEmit(const gui_events::TabRemoveEvent& event) override;

    void updateObserver(int id) override;

    QWidget* createToolbox();
    QWidget* createOptionFrame();

    FlowLayout*           m_toolLayout;
    PaintToolOptionFrame* m_optionFrame;

    size_t                m_defaultToolIndex;
    size_t                m_defaultSelectionToolIndex;
  };
}

#endif // LEFTTOOLBOX_H
