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
#ifndef OPTIONCHOICE_H
#define OPTIONCHOICE_H

#include <QComboBox>
#include <QWidget>

namespace pixpaint
{
  class OptionChoice : public QWidget
  {
  public:
    OptionChoice(QWidget* parent,
                 const std::string& name,
                 std::string* valuePtr,
                 const std::vector<std::string>& choices,
                 std::vector<std::string>::const_iterator initialChoice,
                 std::function<void(const std::string&)> extraFunc = [](const std::string&){});

  protected:
    void currentIndexChanged(int index);

  private:
    std::function<void(const std::string&)> m_extraFunc;
    QComboBox*                              m_comboBox;
    std::string*                            m_valuePtr;
  };
}

#endif // OPTIONCHOICE_H
