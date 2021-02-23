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
#ifndef ASSERT_H
#define ASSERT_H

#include <cassert>

#ifndef NDEBUG
#define PIXPAINT_ASSERT(cond, msg) assert(cond && msg)
#define PIXPAINT_ASSERT_RESULT(cond) assert(cond)
#else
#define PIXPAINT_ASSERT(cond, msg)
#define PIXPAINT_ASSERT_RESULT(cond) cond
#endif // NDEBUG

#endif // ASSERT_H
