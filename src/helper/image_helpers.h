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
#ifndef IMAGE_HELPERS_H
#define IMAGE_HELPERS_H

#include <functional>
#include <string>
#include "../image/rect.h"

namespace pixpaint
{
  class Image;
  class MaskablePixelData;
namespace image_helpers
{
  void applyEffect(std::string effectName,
                   Image& image,
                   std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                   std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect);
  void applyEffect(std::string effectName,
                   MaskablePixelData& layer,
                   std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                   std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect);
}
}

#endif // IMAGE_HELPERS_H
