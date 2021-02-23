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
#include "colorbutton.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include "../image/point.h"
#include "../utility/qt_utility.h"

namespace pixpaint
{
  ColorButton::ColorButton(QWidget* parent,
                           const Color& color,
                           bool showBorder) :
    QPushButton(parent),
    m_color(color),
    m_showBorder(showBorder)
  {
    this->setFlat(true);
  }

  void ColorButton::setBackgroundColor(const Color& color) noexcept
  {
    m_color = color;
  }

  const Color& ColorButton::getBackgroundColor() const noexcept
  {
    return m_color;
  }

  void ColorButton::doubleClick()
  {
    Q_EMIT doubleClicked();
  }

  void ColorButton::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);

    auto w = this->geometry().width() - 1;
    auto h = this->geometry().height() - 1;
    qt_utils::drawTransparentBackground(painter, 0, 0, w, h, 4);
    painter.setPen(QPen(QColor(0, 0, 0, 0)));
    painter.setBrush(QBrush(qt_utils::convertToQTColor(m_color)));
    painter.drawRect(0, 0, w, h);

    if(m_showBorder) {
      painter.setPen(QPen(QColor(0, 0, 0, 255)));
      painter.drawRect(0, 0, w, h);
    }
  }

  void ColorButton::mouseDoubleClickEvent(QMouseEvent* event)
  {
    if(event->button() == Qt::MouseButton::LeftButton) {
      doubleClick();
    }
  }
}
