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
#include "optioncheckbox.h"

#include <QGridLayout>
#include <QLabel>
#include "../gui_define.h"

namespace pixpaint
{
  OptionCheckbox::OptionCheckbox(QWidget* parent,
                                 const std::string& name,
                                 bool* valuePtr,
                                 bool initialValue,
                                 std::function<void(int)> extraFunc) :
    QWidget(parent),
    m_extraFunc(std::move(extraFunc)),
    m_value(valuePtr)
  {
    auto* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);

    auto* label = new QLabel((name + ":").c_str(), this);

    m_checkbox = new QCheckBox(this);
    m_checkbox->setMaximumHeight(TOOLBOX_WIDGET_HEIGHT);
    m_checkbox->setChecked(initialValue);
    QObject::connect(m_checkbox, &QCheckBox::stateChanged, this, &OptionCheckbox::stateChanged);
    layout->addWidget(label);
    layout->addWidget(m_checkbox);

    this->setLayout(layout);
  }

  void OptionCheckbox::stateChanged(int state)
  {
    *m_value = (state == Qt::Checked);
    if(m_extraFunc) {
      m_extraFunc(state);
    }
  }
}
