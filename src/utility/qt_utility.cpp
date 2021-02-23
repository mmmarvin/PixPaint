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
#include "qt_utility.h"

#include <QKeySequence>
#include "../image/pixeldata.h"

namespace pixpaint
{
namespace qt_utils
{
namespace qt_utils_detail
{
  static const auto QCOLOR_WHITE = QColor(255, 255, 255, 255);
  static const auto QCOLOR_GRAY = QColor(200, 200, 200, 255);
}
  QImage createQImage(PixelData& layer)
  {
    return QImage(layer.getData(), layer.getWidth(), layer.getHeight(), QImage::Format_RGBA8888);
  }

  QImage createQImage(const PixelData& layer)
  {
    return QImage(layer.getData(), layer.getWidth(), layer.getHeight(), QImage::Format_RGBA8888);
  }

  QColor convertToQTColor(const Color& color)
  {
    return QColor(color.r, color.g, color.b, color.a);
  }

  Color convertToColor(const QColor& color)
  {
    return Color(color.red(), color.green(), color.blue(), color.alpha());
  }

  QPoint convertToQTPoint(const Point& point)
  {
    return QPoint(point.x, point.y);
  }

  Point convertToPoint(const QPoint& point)
  {
    return Point(point.x(), point.y());
  }

  QRect convertToQTRect(const IntRect& rect)
  {
    return QRect(rect.x, rect.y, rect.width, rect.height);
  }

  IntRect convertToRect(const QRect& rect)
  {
    return IntRect(rect.left(), rect.top(), rect.width(), rect.height());
  }

  void drawTransparentBackground(QPainter& painter,
                                 position_t x,
                                 position_t y,
                                 position_t width,
                                 position_t height,
                                 position_t squareSizes)
  {
    if(squareSizes % 2 == 0) {
      ++squareSizes;
    }

    painter.setPen(QPen(QColor(0, 0, 0, 0)));
    painter.setBrush(QBrush(qt_utils_detail::QCOLOR_WHITE));
    painter.drawRect(x, y, width, height);

    for(position_t y_ = y; y_ < height; y_ += squareSizes) {
      for(position_t x_ = x; x_ < width; x_ += squareSizes) {
        if((x_ + y_) % 2 == 0) {
          painter.setBrush(QBrush(qt_utils_detail::QCOLOR_GRAY));
          painter.drawRect(x_,
                           y_,
                           x_ + squareSizes >= width ? width - x_ : squareSizes,
                           y_ + squareSizes >= height ? height - y_ : squareSizes);
        }
      }
    }
  }

  void drawTransparentBackground(QPainter& painter,
                                 position_t x,
                                 position_t y,
                                 position_t width,
                                 position_t height)
  {
    painter.setPen(QPen(QColor(0, 0, 0, 0)));
    painter.setBrush(QBrush(qt_utils_detail::QCOLOR_WHITE));
    painter.drawRect(x, y, width, height);

    for(position_t y_ = y; y_ < height; ++y_) {
      for(position_t x_ = x; x_ < width; ++x_) {
        if((x_ + y_) % 2 == 0) {
          painter.setPen(QPen(qt_utils_detail::QCOLOR_GRAY));
          painter.drawPoint(x_, y_);
        }
      }
    }
  }

  QKeySequence stringToKeySequence(const std::string& keySequence)
  {
    return QKeySequence(keySequence.c_str());
  }
}
}
