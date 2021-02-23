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

#include <fstream>
#include "../registrar/animationfiletyperegistrar.h"
#include "../registrar/imagefiletyperegistrar.h"
#include "../registrar/projectfiletyperegistrar.h"

namespace pixpaint
{
namespace filetype_utils
{
  std::string combineNameFilter(const std::string& fileTypeDescription,
                                const std::string& fileTypeExtension)
  {
    return fileTypeDescription + " (*." + fileTypeExtension + ")";
  }

  std::string grabExtension(const std::string& mimeType)
  {
    if(mimeType.empty()) {
      return "";
    }

    std::size_t startPos, endPos;

    // find first *.
    for(std::size_t i = 0, isize = mimeType.size() - 1; i < isize; ++i) {
      auto c1 = mimeType[i];
      auto c2 = mimeType[i + 1];

      if(c1 == '*' && c2 == '.') {
        startPos = i;
        break;
      }
    }

    if(startPos == mimeType.size() - 1) {
      return "";
    }

    // find space or )
    for(std::size_t i = startPos, isize = mimeType.size(); i < isize; ++i) {
      if(isspace(mimeType[i]) || mimeType[i] == ')') {
        endPos = i;
        break;
      }
    }

    return mimeType.substr(startPos + 2, endPos - (startPos + 2));
  }

  std::string addExtension(const std::string& filename, const std::string& extension)
  {
    if(os_specific::filesystem::path(filename).extension() == extension) {
      return filename;
    }

    auto new_filename = os_specific::filesystem::path(filename).replace_extension(os_specific::filesystem::path(extension));
    return new_filename.string();
  }
}
namespace projectfiletype_utils
{
  std::string getFilterList()
  {
    std::string ret = "";
    for(const auto& projectFileTypeInformation : getProjectFileTypeRegistrar()) {
      if(!ret.empty()) {
        ret += ";;";
      }

      ret += filetype_utils::combineNameFilter(projectFileTypeInformation.getDescription(),
                                               projectFileTypeInformation.getExtension());
    }

    return ret;
  }
}
namespace imagefiletype_utils
{
  std::string getFilterList()
  {
    std::string ret = "";
    for(const auto& imageFileTypeInformation : getImageFileTypeRegistrar()) {
      if(!ret.empty()) {
        ret += ";;";
      }

      ret += filetype_utils::combineNameFilter(imageFileTypeInformation.getDescription(),
                                               imageFileTypeInformation.getExtension());
    }

    return ret;
  }
}
namespace animationtype_utils
{
  std::string getFilterList()
  {
    std::string ret = "";
    for(const auto& animationFileTypeInformation : getAnimationFileTypeRegistrar()) {
      if(!ret.empty()) {
        ret += ";;";
      }

      ret += filetype_utils::combineNameFilter(animationFileTypeInformation.getDescription(),
                                               animationFileTypeInformation.getExtension());
    }

    return ret;
  }
}
namespace file_utils
{
  std::vector<unsigned char> readFileToData(const std::string& filename)
  {
    std::vector<unsigned char> ret;

    std::ifstream in(filename);
    if(in.is_open()) {
      std::array<unsigned char, 255> buff;
      std::streamsize read = 0;

      in.read(reinterpret_cast<char*>(buff.data()), buff.size());
      read = in.gcount();
      while(read) {
        ret.insert(ret.end(), buff.data(), buff.data() + read);

        in.read(reinterpret_cast<char*>(buff.data()), buff.size());
        read = in.gcount();
      }
    }

    return ret;
  }
}
}
