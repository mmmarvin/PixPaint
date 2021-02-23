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
#ifndef FILETYPE_UTILITY_H
#define FILETYPE_UTILITY_H

#include <string>
#include <vector>

namespace pixpaint
{
namespace filetype_utils
{
  std::string combineNameFilter(const std::string& fileTypeDescription,
                                const std::string& fileTypeExtension);
  std::string grabExtension(const std::string& mimeType);

  template<class FileInformationT,
           class = std::enable_if_t<!std::is_same<FileInformationT, std::string>()>>
  std::string addExtension(const std::string& filename, const FileInformationT& fileTypeInformation);
  std::string addExtension(const std::string& filename, const std::string& extension);
}
namespace projectfiletype_utils
{
  std::string getFilterList();
}
namespace imagefiletype_utils
{
  std::string getFilterList();
}
namespace animationtype_utils
{
  std::string getFilterList();
}
namespace file_utils
{
  std::vector<unsigned char> readFileToData(const std::string& filename);
}
}

#include "filetype_utility.inl"

#endif // FILETYPE_UTILITY_H
