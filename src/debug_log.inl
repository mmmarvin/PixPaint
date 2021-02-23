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
#ifndef NDEBUG
#include "debug_log.h"

#include <iostream>

namespace pixpaint
{
namespace detail
{
  template<class Arg, class... Args>
  void debug_log_impl(Arg&& arg, Args&&... args)
  {
    std::cout << arg << " ";
    debug_log_impl(std::forward<Args>(args)...);
  }
}
  template<class... Args>
  void debug_log(Args&&... args)
  {
    detail::debug_log_impl(std::forward<Args>(args)...);
  }
}
#endif // NDEBUG
