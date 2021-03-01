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
#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include "../image/color.h"

namespace pixpaint
{
  class ColorButton : public QPushButton
  {
    Q_OBJECT

  public:
    ColorButton(QWidget* parent,
                const Color& color,
                bool showBorder);

    void setBackgroundColor(const Color& color) noexcept;
    const Color& getBackgroundColor() const noexcept;

  public:
    Q_SIGNAL void doubleClicked();
    Q_SLOT void doubleClick();
    Q_SIGNAL void middleClicked();
    Q_SLOT void middleClick();

  protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

  private:
    Color m_color;
    bool  m_showBorder;
  };
}

#endif // COLORBUTTON_H
