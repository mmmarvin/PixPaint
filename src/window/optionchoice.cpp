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
#include "optionchoice.h"

#include <QLabel>
#include <QVBoxLayout>

namespace pixpaint
{
  OptionChoice::OptionChoice(QWidget* parent,
                             const std::string& name,
                             std::string* valuePtr,
                             const std::vector<std::string>& choices,
                             std::vector<std::string>::const_iterator initialChoice,
                             std::function<void(const std::string&)> extraFunc) :
    QWidget(parent),
    m_extraFunc(std::move(extraFunc)),
    m_valuePtr(valuePtr)
  {
    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel((name + ":").c_str(), this);

    m_comboBox = new QComboBox(this);
    for(const auto& choice : choices) {
      m_comboBox->addItem(choice.c_str());
    }
    m_comboBox->setCurrentIndex(initialChoice - choices.begin());
    QObject::connect(m_comboBox,
                  QOverload<int>::of(&QComboBox::currentIndexChanged),
                  this,
                  &OptionChoice::currentIndexChanged);

    layout->addWidget(label);
    layout->addWidget(m_comboBox);

    this->setLayout(layout);
  }

  void OptionChoice::currentIndexChanged(int index)
  {
    *m_valuePtr = std::string(m_comboBox->itemText(index).toUtf8().data());
    if(m_extraFunc) {
      m_extraFunc(*m_valuePtr);
    }
  }
}
