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
#include "optiontextbox.h"

#include <QHBoxLayout>
#include <QLabel>
#include "../gui_define.h"

namespace pixpaint
{
  OptionTextbox::OptionTextbox(QWidget* parent,
                               const std::string& name,
                               std::string* valuePtr,
                               const std::string& initialValue,
                               std::function<void(const std::string&)> extraFunc) :
    QWidget(parent),
    m_extraFunc(std::move(extraFunc)),
    m_value(valuePtr)
  {
    auto* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);

    auto* label = new QLabel((name + ":").c_str(), this);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setMaximumHeight(TOOLBOX_WIDGET_HEIGHT);
    m_lineEdit->setText(initialValue.c_str());
    QObject::connect(m_lineEdit, &QLineEdit::textChanged, this, &OptionTextbox::textChanged);
    layout->addWidget(label);
    layout->addWidget(m_lineEdit);

    this->setLayout(layout);
  }

  void OptionTextbox::textChanged(const QString& str)
  {
    *m_value = std::string(str.toUtf8().data());
    if(m_extraFunc) {
      m_extraFunc(*m_value);
    }
  }
}
