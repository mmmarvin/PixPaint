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
#ifndef QT_UTILITY_H
#define QT_UTILITY_H

#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include "../image/color.h"
#include "../image/point.h"
#include "../image/rect.h"
#include "../type.h"

namespace pixpaint
{
  class PixelData;
namespace qt_utils
{
  QImage createQImage(PixelData& layer);
  QImage createQImage(const PixelData& layer);

  QImage convertToQTImage(const PixelData& layer);
  PixelData convertToPixelData(const QImage& image);

  QPixmap convertToQTPixmap(const PixelData& layer);

  QColor convertToQTColor(const Color& color);
  Color convertToColor(const QColor& color);

  QPoint convertToQTPoint(const Point& point);
  Point convertToPoint(const QPoint& point);

  QRect convertToQTRect(const IntRect& rect);
  IntRect convertToRect(const QRect& rect);

  void drawTransparentBackground(QPainter& painter,
                                 position_t x,
                                 position_t y,
                                 position_t width,
                                 position_t height,
                                 position_t squareSizes);
  void drawTransparentBackground(QPainter& painter,
                                 position_t x,
                                 position_t y,
                                 position_t width,
                                 position_t height);

  QKeySequence stringToKeySequence(const std::string& keySequence);
}
}

#endif // QT_UTILITY_H
