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
#include "consoletoolbox.h"

#include <QHBoxLayout>
#include "../define.h"

namespace pixpaint
{
  ConsoleToolbox::ConsoleToolbox(QWidget* parent) :
    QWidget(parent)
  {
    this->setMinimumSize(300, 100);

    auto* layout = new QHBoxLayout(this);
    m_text = new QTextEdit(this);
    m_text->setReadOnly(true);
    layout->addWidget(m_text);
  }

  void ConsoleToolbox::writeMessage(std::string msg)
  {
    if(m_messages.size() == MAX_CONSOLE_LINES) {
      m_messages.erase(m_messages.begin());
    }
    m_messages.push_back(msg);

    std::string new_message;
    for(const auto& msg : m_messages) {
      new_message += msg + "\n";
    }

    m_text->setText(new_message.c_str());
  }

  void ConsoleToolbox::clearMessages()
  {
    m_messages.clear();
    m_text->setText("");
  }
}
