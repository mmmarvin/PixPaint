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
#ifndef CONSOLETOOLBOX_H
#define CONSOLETOOLBOX_H

#include <QTextEdit>
#include <QWidget>

namespace pixpaint
{
  class ConsoleToolbox : public QWidget
  {
  public:
    ConsoleToolbox(QWidget* parent);

    void writeMessage(std::string msg);
    void clearMessages();

  private:
    std::vector<std::string>  m_messages;
    QTextEdit*                m_text;
  };
}

#endif // CONSOLETOOLBOX_H
