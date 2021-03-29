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
#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include <string>

namespace pixpaint
{
namespace http_helper
{
  struct URLStructure
  {
    std::string protocol;
    std::string hostname;
    std::string pathname;
  };

  URLStructure parseURL(const std::string& url);
}
}

#endif // HTTP_HELPER_H
