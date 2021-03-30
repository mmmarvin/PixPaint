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
#ifndef OS_SPECIFIC_FUNCTIONS_H
#define OS_SPECIFIC_FUNCTIONS_H

#include <string>

namespace pixpaint
{
namespace os_specific
{
  bool callProcess(const std::string& process_name, const std::string& process_parameters);
  template<class... Args>
  bool callProcessArgs(const std::string& process_name,
                       Args&&... args);
  std::string getHomePath();
}
}

#include "os_specific_functions.inl"

#endif // OS_SPECIFIC_FUNCTIONS_H
