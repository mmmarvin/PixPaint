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
#ifndef SCRIPT_EMBEDS_H
#define SCRIPT_EMBEDS_H

#include <cstdint>
#include <vector>
#include <string>
#include "../image/point.h"

namespace pixpaint
{
  class Image;
  class MaskablePixelData;
  class PixelData;
namespace script_utils
{
  enum EMessageBoxButtons
  {
    EMBB_UNKNOWN = 0,
    EMBB_YES = 1 << 0,
    EMBB_NO= 1 << 1,
    EMBB_OK = 1 << 2,
    EMBB_CANCEL = 1 << 3
  };

  EMessageBoxButtons spawnMessageBox(const std::string& msg,
                                     EMessageBoxButtons buttons = EMessageBoxButtons::EMBB_OK);

  std::vector<std::uint8_t> convertImageToData(const Image& image, bool includeAlpha = true);
  std::vector<std::uint8_t> convertLayerToData(const PixelData& layer, bool includeAlpha = true);
  std::vector<std::uint32_t> convertImageToData32Bit(const Image& image);
  std::vector<std::uint32_t> convertLayerToData32Bit(const PixelData& layer);

  void copyLayerToClipboard(const MaskablePixelData& layer);
  Point pasteLayerFromClipboard(MaskablePixelData& layer);

  std::vector<std::string> getFontList();
}
}

#endif // SCRIPT_EMBEDS_H
