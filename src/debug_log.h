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
#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifndef NDEBUG
namespace pixpaint
{
namespace detail
{
  void debug_log_impl();

  template<class Arg, class... Args>
  void debug_log_impl(Arg&& arg, Args&&... args);
}
  template<class... Args>
  void debug_log(Args&&... args);
}
#endif // NDEBUG

#ifndef NDEBUG
#define PIXPAINT_DEBUG_LOG(...) ::pixpaint::debug_log(__VA_ARGS__)
#else
#define PIXPAINT_DEBUG_LOG(...)
#endif // NDEBUG

#ifndef NDEBUG
#include "debug_log.inl"
#endif // NDEBUG

#endif // DEBUG_LOG_H
