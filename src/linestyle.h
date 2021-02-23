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
#ifndef LINESTYLE_H
#define LINESTYLE_H

#include <QPainter>

namespace pixpaint
{
  struct LineStyle
  {
    enum ELineStyle
    {
      SolidLine = Qt::SolidLine,
      DashedLine = Qt::DashLine
    };

    LineStyle(int width_, int lineOffset_, bool rounded_, ELineStyle style_) :
      width(width_),
      lineOffset(lineOffset_),
      rounded(rounded_),
      style(style_)
    {
    }

    int         width;
    int         lineOffset;
    bool        rounded;
    ELineStyle  style;
  };
}

#endif // LINESTYLE_H
