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
#include "tool_helpers.h"

#include "../env/imageenvironment.h"
#include "../image/image.h"
#include "../image/size.h"
#include "../manager/colormanager.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"
#include "../window/imageeditorview.h"
#include "../window/selectionwidget.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
namespace tool_helpers
{
  void onToolChange(PaintToolHandlerBase& paintTool, int (PaintToolHandlerBase::*changeFunc)(ModifyablePixelData&, MaskablePixelData&))
  {
    auto& view = getImageEnvironment().getView();
    auto res = (paintTool.*changeFunc)(view.getPreviewLayer(),
                                       getImageManager().getImage().getCurrentLayer());
    if(res & PaintToolBase::EChangeResult::ECCR_UPDATEIMAGE) {
      auto refresh_rect = view.getSmallestDrawableRegion(castTo<double>(paintTool.getDrawRect()) *
                                                        view.getPixelSize());
      view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
    }
    if(res & PaintToolBase::EChangeResult::ECCR_UPDATECURSOR) {
      updateViewToolCursor();
    }
  }

  void setViewToolCursor(const Cursor& cursor)
  {
    auto& image_env = getImageEnvironment();
    image_env.getView().setCursor(cursor);
    image_env.getScrollArea().setCursor(cursor);
    image_env.getSelection().setCursor(cursor);
  }

  void updateViewToolCursor()
  {
    auto& image_env = getImageEnvironment();
    if(image_env.isViewSet()) {
      auto& currentPaintTool = getPaintToolManager().getCurrentTool();
      auto paintToolCursor = currentPaintTool.getCursor();
      if(paintToolCursor) {
        const auto& cursor = paintToolCursor.value();
        setViewToolCursor(cursor);
      } else {
        image_env.getView().setCursor(Cursor(QCursor(Qt::ArrowCursor)));
        image_env.getScrollArea().setCursor(Cursor(QCursor(Qt::ArrowCursor)));
        image_env.getSelection().setCursor(Cursor(QCursor(Qt::ArrowCursor)));
      }
    }
  }

  void drawShape(Drawer* drawer,
                 void (Drawer::*shapeFunc)(const Point&, const Point&, const Color&, const Color&, const DrawParam&),
                 ControlState controlState,
                 Point& startPoint,
                 Point& endPoint,
                 Point& finalStartPoint,
                 Point& finalEndPoint,
                 Point& min,
                 Point& max,
                 bool fill,
                 const DrawParam& param)
  {
    const auto& foreground_color = getColorManager().getForegroundColor();
    const auto& background_color = getColorManager().getBackgroundColor();

    if(startPoint == endPoint) {
      min.x = general_utils::minelement(std::initializer_list<position_t>{startPoint.x, endPoint.x, min.x});
      min.y = general_utils::minelement(std::initializer_list<position_t>{startPoint.y, endPoint.y, min.y});
      max.x = general_utils::maxelement(std::initializer_list<position_t>{startPoint.x, endPoint.x, max.x});
      max.y = general_utils::maxelement(std::initializer_list<position_t>{startPoint.y, endPoint.y, max.y});

      drawer->drawLineBlend(startPoint, endPoint, foreground_color, param);
    } else {
      auto sp = startPoint, ep = endPoint;
      bool neg_x = general_utils::swapIfLess(ep.x, sp.x), negY = general_utils::swapIfLess(ep.y, sp.y);

      Size dimension = ep - sp;
      if(controlState.shiftDown) {
        auto min = general_utils::min(dimension.width, dimension.height);
        dimension.width = dimension.height = min;
        if(neg_x) {
          sp.x = ep.x - dimension.width;
        } else {
          ep.x = sp.x + dimension.width;
        }
        if(negY) {
          sp.y = ep.y - dimension.height;
        } else {
          ep.y = sp.y + dimension.height;
        }
      }

      if(controlState.controlDown) {
        sp = startPoint - Point(dimension);
        ep = startPoint + Point(dimension);
      }

      Color fill_color = fill ? background_color : Color::TRANSPARENT;
      (drawer->*shapeFunc)(sp, ep, foreground_color, fill_color, param);

      min.x = general_utils::minelement(std::initializer_list<position_t>{sp.x, ep.x, min.x});
      min.y = general_utils::minelement(std::initializer_list<position_t>{sp.y, ep.y, min.y});
      max.x = general_utils::maxelement(std::initializer_list<position_t>{sp.x, ep.x, max.x});
      max.y = general_utils::maxelement(std::initializer_list<position_t>{sp.y, ep.y, max.y});
      finalStartPoint = sp;
      finalEndPoint = ep;
    }
  }
}
}
