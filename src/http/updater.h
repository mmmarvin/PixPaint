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
#ifndef UPDATER_H
#define UPDATER_H

#include <string>
#include <tuple>
#include "../pattern/singleton.h"

namespace pixpaint
{
  class Updater
  {
    static constexpr auto* HOST = "mmmarvin.github.io";
    static constexpr auto* PORT = "https";
    static constexpr auto* TARGET = "/PixPaintWeb/version.json";

    Updater() {}

  public:
    std::tuple<int, std::string, std::string> updateAvailable() const;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(Updater)
  };

  PIXPAINT_SINGLETON_FUNC(Updater)
}

#endif // UPDATER_H
