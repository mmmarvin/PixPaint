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
#include "filetype_utility.h"

#include "../os_specific_headers.h"

namespace pixpaint
{
namespace filetype_utils
{
  template<class FileInformationT,
           class>
  std::string addExtension(const std::string& filename, const FileInformationT& fileTypeInformation)
  {
    if(os_specific::filesystem::path(filename).extension() == fileTypeInformation.getExtension()) {
      return filename;
    }

    auto new_filename = os_specific::filesystem::path(filename).replace_extension(fileTypeInformation.getExtension());
    return new_filename.string();
  }
}
}
