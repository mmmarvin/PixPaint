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
#ifndef TOOLOPTIONSLIDER_H
#define TOOLOPTIONSLIDER_H

#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include <functional>

namespace pixpaint
{
  class OptionSlider : public QWidget
  {
  public:
    OptionSlider(QWidget* parent,
                 const std::string& name,
                 int* valuePtr,
                 int initialValue,
                 int minValue,
                 int maxValue,
                 std::function<void(int)> extraFunc = [](int){});

  protected:
    void sliderMoved(int value);
    void valueChanged(int value);

  private:
    std::function<void(int)>  m_extraFunc;
    QSlider*                  m_slider;
    QSpinBox*                 m_text;
    int*                      m_valuePtr;
  };
}

#endif // TOOLOPTIONSLIDER_H
