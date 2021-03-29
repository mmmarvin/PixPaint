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
#include "http_helper.h"

#include <algorithm>

namespace pixpaint
{
namespace http_helper
{
  URLStructure parseURL(const std::string& url)
  {
    URLStructure ret { "", "", "" };

    auto protocol_index = url.find("://");
    if(protocol_index != std::string::npos) {
      ret.protocol = url.substr(0, protocol_index);
    } else {
      return ret;
    }

    auto path_index = std::find(std::next(url.begin(), protocol_index + 3), url.end(), '/');
    if(path_index != url.end()) {
      ret.hostname = std::string(std::next(url.begin(), protocol_index + 3), path_index);
    } else {
      return ret;
    }

    if(path_index != url.end()) {
      ret.pathname = std::string(path_index, url.end());
    }

    return ret;
  }
}
}
