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
#ifndef RIGHTTOOLBOX_H
#define RIGHTTOOLBOX_H

#include <QWidget>
#include "../manager/eventmanager.h"

namespace pixpaint
{
namespace gui_events
{
  struct TabAddEvent;
  struct TabRemoveEvent;
  struct FrameChangeEvent;
  struct AnimationPauseEvent;
  struct AnimationPlayEvent;
  struct AnimationStopEvent;
}
  class LayerWidget;
  class PreviewWidget;

  class RightToolbox : public QWidget,
                       EventListener<gui_events::TabAddEvent>,
                       EventListener<gui_events::TabRemoveEvent>,
                       EventListener<gui_events::FrameChangeEvent>,
                       EventListener<gui_events::AnimationPlayEvent>,
                       EventListener<gui_events::AnimationStopEvent>,
                       EventListener<gui_events::AnimationPauseEvent>
  {
  public:
    explicit RightToolbox(QWidget* parent);

    PreviewWidget& getPreviewWidget();
    LayerWidget& getLayerWidget();

  private:
    void onEmit(const gui_events::TabAddEvent& event) override;
    void onEmit(const gui_events::TabRemoveEvent& event) override;
    void onEmit(const gui_events::FrameChangeEvent& event) override;
    void onEmit(const gui_events::AnimationPlayEvent& event) override;
    void onEmit(const gui_events::AnimationPauseEvent& event) override;
    void onEmit(const gui_events::AnimationStopEvent& event) override;

    PreviewWidget*  m_preview;
    LayerWidget*    m_layer;
  };
}

#endif // RIGHTTOOLBOX_H
