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
#include "statusbar.h"

namespace pixpaint
{
  void StatusBar::setMousePositionText(int x, int y)
  {
    mousePosition->setText((std::string("[") +
                            std::to_string(x) +
                            std::string(",") +
                            std::to_string(y) +
                            std::string("]")).c_str());
  }

  void StatusBar::setDrawSize(int x, int y)
  {
    drawSize->setText((std::string("[") +
                      std::to_string(x) +
                      std::string(",") +
                      std::to_string(y) +
                      std::string("]")).c_str());
  }

  void StatusBar::setZoomLevelText(int level)
  {
    imageZoomLevel->setText((std::to_string(static_cast<int>(level)) +
                             std::string("%")).c_str());
  }
}
