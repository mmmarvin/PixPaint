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
#include "spritesheetanimationfiletype.h"

#include "../image/animation.h"
#include "../image/image.h"
#include "../filetype/bmpimagefiletype.h"
#include "../filetype/jpgimagefiletype.h"
#include "../filetype/pngimagefiletype.h"
#include "../utility/filetype_utility.h"

namespace pixpaint
{
  SpriteSheetAnimationFileType::SpriteSheetAnimationFileType() :
    m_width(800),
    m_height(600),
    m_horizontal(true)
  {
    this->addIntegerValueOption(&m_width, "Width", PixelData::MINIMUM_DIMENSION, PixelData::MAXIMUM_DIMENSION);
    this->addIntegerValueOption(&m_height, "Height", PixelData::MINIMUM_DIMENSION, PixelData::MAXIMUM_DIMENSION);
    this->addFlagOption(&m_horizontal, "Horizontal");
    this->addStringChoice(&m_fileType, "File Extension",
    std::vector<std::string> {
      "JPEG Image", "PNG Image", "Windows BMP Image"
    });
  }

  void SpriteSheetAnimationFileType::onPreSave(const Animation& animation)
  {
    m_width = animation.getFrame(0).getWidth() * animation.getFrameCount();
    m_height = animation.getFrame(0).getHeight();
  }

  bool SpriteSheetAnimationFileType::canReadHeader(HeaderStream&) const
  {
    return false;
  }

  int SpriteSheetAnimationFileType::saveAnimation(const std::string& filename, const Animation& animation)
  {
    if(animation.getFrameCount()) {
      auto image_width = animation.getFrame(0).getWidth();
      auto image_height = animation.getFrame(0).getHeight();

      Image temp(m_width, m_height, Color::TRANSPARENT);
      position_t x = 0, y = 0;
      if(m_horizontal) {
        for(size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
          auto frame = animation.getFrame(i);
          frame.flattenLayers(false);
          temp.getLayer(0).combine(frame.getLayer(0), x, y, true);

          x += image_width;
          if(x >= m_width) {
            x = 0;
            y += image_height;
          }
        }
      } else {
        for(size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
          auto frame = animation.getFrame(i);
          frame.flattenLayers(false);
          temp.getLayer(0).combine(frame.getLayer(0), x, y, true);

          y += image_height;
          if(y >= m_height) {
            y = 0;
            x += image_width;
          }
        }
      }

      if(m_fileType == "JPEG Image") {
        return JPGImageFileType().saveImage(filetype_utils::addExtension(filename, std::string("jpg")), temp);
      } else if(m_fileType == "PNG Image") {
        return PNGImageFileType().saveImage(filetype_utils::addExtension(filename, std::string("png")), temp);
      } else if(m_fileType == "Windows BMP Image") {
        return BMPImageFileType().saveImage(filetype_utils::addExtension(filename, std::string("bmp")), temp);
      }
    }

    return ESaveResult::ESR_ERROR;
  }

  int SpriteSheetAnimationFileType::openAnimation(const std::string&, Animation&) const
  {
    return EOpenResult::EOR_ERROR;
  }
}
