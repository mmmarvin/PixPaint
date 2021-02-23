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
#ifndef MACRO_H
#define MACRO_H

#define PIXPAINT_SINGLETON_FUNC(className) \
  className& get ## className();
#define PIXPAINT_SINGLETON_FUNC_DEF(className) \
  className& get ## className() { static className instance; return instance; }
#define PIXPAINT_SINGLETON_FUNC_DEFP(className, ...) \
  className& get ## className() { static className instance(__VA_ARGS__); return instance; }
#define PIXPAINT_SINGLETON_FUNC_FRIEND(className) \
  friend className& get ## className();

#endif // MACRO_H
