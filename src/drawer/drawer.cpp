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
#include "drawer.h"

#include "../image/mask.h"
#include "../image/maskablepixeldata.h"
#include "../assert.h"

namespace pixpaint
{
  DrawParam::DrawParam(bool antialiased_, LineStyle lineStyle_) :
    antialiased(antialiased_),
    lineStyle(lineStyle_)
  {
  }

  ////// DrawerRestorePixelsCommand
  Drawer::DrawerRestorePixelsCommand::DrawerRestorePixelsCommand(std::vector<std::pair<Point, Color>> points) :
    m_points(std::move(points))
  {
    m_points.shrink_to_fit();
  }

  void Drawer::DrawerRestorePixelsCommand::undo(MaskablePixelData& target)
  {
    for(auto& t : m_points) {
      const auto& pos = t.first;
      auto& col = t.second;

      auto* col_data = reinterpret_cast<Color*>(target.getData() + (pos.y * 4 * target.getWidth()) + (pos.x * 4));
      std::swap(*col_data, col);
    }
  }

  void Drawer::DrawerRestorePixelsCommand::redo(MaskablePixelData& target)
  {
    undo(target);
  }

  ////// DrawerRestorePixelCommand
  Drawer::DrawerRestorePixelCommand::DrawerRestorePixelCommand(position_t x,
                                                               position_t y,
                                                               MaskablePixelData* target) :
    m_position(x, y),
    m_color(target->getPixel(x, y))
  {
    PIXPAINT_ASSERT(target, "Target is null");
  }

  void Drawer::DrawerRestorePixelCommand::undo(MaskablePixelData& target)
  {
    auto* c = reinterpret_cast<Color*>(target.getData() + (m_position.y * 4 * target.getWidth()) + (m_position.x * 4));
    std::swap(*c, m_color);
  }

  void Drawer::DrawerRestorePixelCommand::redo(MaskablePixelData& target)
  {
    undo(target);
  }

  ////// DrawerRestoreRegionCommand
  Drawer::DrawerRestoreRegionCommand::DrawerRestoreRegionCommand(position_t x,
                                                                 position_t y,
                                                                 position_t width,
                                                                 position_t height,
                                                                 MaskablePixelData* target) :
    m_rect(x, y, width, height),
    m_region(target->copy(x, y, width, height))
  {
    PIXPAINT_ASSERT(target, "Target is null");
  }

  void Drawer::DrawerRestoreRegionCommand::undo(MaskablePixelData& target)
  {
    auto temp_region = target.copy(m_rect.x, m_rect.y, m_rect.width, m_rect.height);
    target.combine(m_region, m_rect.x, m_rect.y, true);
    m_region = std::move(temp_region);
  }

  void Drawer::DrawerRestoreRegionCommand::redo(MaskablePixelData& target)
  {
    undo(target);
  }

  ////// DrawerCommander
  void Drawer::DrawerCommander::addCommand(position_t x,
                                           position_t y,
                                           position_t width,
                                           position_t height)
  {
    PIXPAINT_ASSERT(m_layer, "Layer not set");
    m_commands.push_back(std::make_unique<DrawerRestoreRegionCommand>(x, y, width, height, m_layer));
  }

  void Drawer::DrawerCommander::addCommand(const IntRect& rect)
  {
    addCommand(rect.x, rect.y, rect.width, rect.height);
  }

  void Drawer::DrawerCommander::addCommand(position_t x,
                                           position_t y)
  {
    PIXPAINT_ASSERT(m_layer, "Layer not set");
    m_commands.push_back(std::make_unique<DrawerRestorePixelCommand>(x, y, m_layer));
  }

  void Drawer::DrawerCommander::addCommand(const Point& point)
  {
    addCommand(point.x, point.y);
  }

  void Drawer::DrawerCommander::addCommand(std::vector<std::pair<Point, Color>> points)
  {
    m_commands.push_back(std::make_unique<DrawerRestorePixelsCommand>(std::move(points)));
  }

  bool Drawer::DrawerCommander::flushable() const
  {
    return !m_commands.empty();
  }

  std::vector<std::unique_ptr<Drawer::DrawerCommand>> Drawer::DrawerCommander::flush()
  {
    auto ret = std::move(m_commands);
    m_commands.clear();

    return ret;
  }

  ////// Drawer
  Drawer::Drawer(bool addCommands) :
    m_commander(nullptr)
  {
    if(addCommands) {
      m_commander = std::make_unique<DrawerCommander>();
    }
  }

  Drawer::DrawerCommander* Drawer::getDrawerCommander()
  {
    return m_commander.get();
  }

  const Drawer::DrawerCommander* Drawer::getDrawerCommander() const
  {
    return m_commander.get();
  }

  void Drawer::setTargetLayer(MaskablePixelData* layer)
  {
    if(m_commander) {
      m_commander->m_layer = layer;
    }
  }

}
