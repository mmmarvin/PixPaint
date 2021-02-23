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
#include "customcursorwidget.h"

#include "../env/imageenvironment.h"
#include "../debug_log.h"
#include "imageeditorview.h"

namespace pixpaint
{
namespace customcursorwidget_detail
{
  void scaleCursorPixmap(QPixmap& cursorPixmap)
  {
    double pixelSize = getImageEnvironment().getView().getPixelSize();
    auto scale_x = pixelSize * double(cursorPixmap.width());
    auto scale_y = pixelSize * double(cursorPixmap.height());
    if(scale_x < 1 || scale_y < 1) {
      QImage new_empty_cursor(1, 1, QImage::Format_RGBA8888);
      new_empty_cursor.setPixelColor(0, 0, QColor(0, 0, 0, 0));
      cursorPixmap = QPixmap::fromImage(new_empty_cursor);
    } else {
      cursorPixmap = cursorPixmap.scaled(scale_x, scale_y);
    }
  }
}
}
