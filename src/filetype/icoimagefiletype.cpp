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
#include "icoimagefiletype.h"

#include "../embed/headerstream.h"

namespace pixpaint
{
  static const ICOImageFileType::ICOHeader ICO_HEADER = { 0x00, 0x00, 0x01, 0x00 };

  bool ICOImageFileType::canReadHeader(HeaderStream& stream) const
  {
    std::array<unsigned char, 6> header;
    stream.read(reinterpret_cast<char*>(header.data()), header.size());
    if(stream.readCount() == header.size()) {
      return !strncmp(reinterpret_cast<char*>(header.data()), reinterpret_cast<const char*>(&ICO_HEADER), sizeof(ICOHeader)) &&
             (header[4] > 0 ||
              header[5] > 0);
    }

    return false;
  }

  int ICOImageFileType::saveImage(const std::string& filename, const Image& image)
  {
    std::ofstream out(filename, std::ios_base::binary);
    if(out.is_open()) {
      out.write(reinterpret_cast<const char*>(&ICO_HEADER), sizeof(ICOHeader));
    }

    return 0;
  }

  int ICOImageFileType::openImage(const std::string& filename, Image& image) const
  {
    return 0;
  }
}
