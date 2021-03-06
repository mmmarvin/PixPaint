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
#include "optionslider.h"

#include <QPainter>
#include <QGridLayout>
#include "../gui_define.h"

namespace pixpaint
{
  OptionSlider::OptionSlider(QWidget* parent,
                             const std::string& name,
                             int* valuePtr,
                             int initialValue,
                             int minValue,
                             int maxValue,
                             std::function<void(int)> extraFunc) :
    QWidget(parent),
    m_extraFunc(std::move(extraFunc)),
    m_valuePtr(valuePtr)
  {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* label = new QLabel((name + ":").c_str(), this);

    m_text = new QSpinBox(this);
    m_text->setFixedSize(55, TOOLBOX_WIDGET_HEIGHT);
    m_text->setMinimum(minValue);
    m_text->setMaximum(maxValue);
    m_text->setValue(initialValue);
    QObject::connect(m_text, QOverload<int>::of(&QSpinBox::valueChanged), this, &OptionSlider::valueChanged);

    m_slider = new QSlider(this);
    m_slider->setMaximumHeight(TOOLBOX_WIDGET_HEIGHT);
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setTickInterval(1);
    m_slider->setTickPosition(QSlider::NoTicks);
    m_slider->setMinimum(minValue);
    m_slider->setMaximum(maxValue);
    m_slider->setValue(initialValue);
    QObject::connect(m_slider, &QSlider::sliderMoved, this, &OptionSlider::sliderMoved);

    auto* topLayout = new QVBoxLayout(this);
    auto* bottomLayout = new QGridLayout;
    bottomLayout->setHorizontalSpacing(0);

    topLayout->addWidget(label);
    bottomLayout->addWidget(m_slider, 0, 0, 1, 1);
    bottomLayout->addWidget(m_text, 0, 1, 1, 1);
    topLayout->addLayout(bottomLayout);

    this->setLayout(topLayout);
  }

  void OptionSlider::sliderMoved(int value)
  {
    *m_valuePtr = value;
    m_text->setValue(value);
    if(m_extraFunc) {
      m_extraFunc(value);
    }
  }

  void OptionSlider::valueChanged(int value)
  {
    *m_valuePtr = value;
    m_slider->setValue(value);
    if(m_extraFunc) {
      m_extraFunc(value);
    }
  }
}
