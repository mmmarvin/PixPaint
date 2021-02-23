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
#include "brightnesscontrasteffect.h"

#include "../debug_log.h"
#include "../utility/general_utility.h"

namespace pixpaint
{
namespace
{
  std::uint8_t applyBrightness(std::uint8_t c, int brightness)
  {
    return general_utils::clamp<int>(static_cast<int>(c) + brightness, 0, 255);
  }

  // Code based on:
  // https://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
  std::uint8_t applyContrast(std::uint8_t c, float contrastFactor)
  {
    return general_utils::clamp<float>((contrastFactor * (c - 128)) + 128, 0, 255);
  }

  // Code based on:
  // https://www.dfstudios.co.uk/articles/programming/image-programming-algorithms/image-processing-algorithms-part-5-contrast-adjustment/
  float getContrastFactor(int contrast)
  {
    return static_cast<float>(259 * (contrast + 255)) / static_cast<float>(255 * (259 - contrast));
  }
}
  BrightnessContrastEffect::BrightnessContrastEffect()
  {
    this->addIntegerValueOption(&m_brightness, "Brightness", -255, 255);
    this->addIntegerValueOption(&m_contrast, "Contrast", -255, 255);
    this->addStringChoice(&m_channel, "Channel", { "All", "Red", "Green", "Blue" });
  }

  void BrightnessContrastEffect::reset()
  {
    m_channel = "All";
    m_brightness = 0;
    m_contrast = 0;
  }

  void BrightnessContrastEffect::applyLayerEffect(const IntRect&, MaskablePixelData& layer)
  {
    auto contrast_factor = getContrastFactor(m_contrast);
    if(m_channel == "Red") {
      for(int y = 0, ysize = layer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = layer.getWidth(); x < xsize; ++x) {
          auto c = layer.getPixel(x, y);
          if(c.a > 0) {
            c.r = applyContrast(applyBrightness(c.r, m_brightness), contrast_factor);
            layer.setPixel(x, y, c);
          }
        }
      }
    } else if(m_channel == "Green") {
      for(int y = 0, ysize = layer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = layer.getWidth(); x < xsize; ++x) {
          auto c = layer.getPixel(x, y);
          if(c.a > 0) {
            c.g = applyContrast(applyBrightness(c.g, m_brightness), contrast_factor);
            layer.setPixel(x, y, c);
          }
        }
      }
    } else if(m_channel == "Blue") {
      for(int y = 0, ysize = layer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = layer.getWidth(); x < xsize; ++x) {
          auto c = layer.getPixel(x, y);
          if(c.a > 0) {
            c.b = applyContrast(applyBrightness(c.b, m_brightness), contrast_factor);
            layer.setPixel(x, y, c);
          }
        }
      }
    } else {
      for(int y = 0, ysize = layer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = layer.getWidth(); x < xsize; ++x) {
          auto c = layer.getPixel(x, y);
          if(c.a > 0) {
            c.r = applyContrast(applyBrightness(c.r, m_brightness), contrast_factor);
            c.g = applyContrast(applyBrightness(c.g, m_brightness), contrast_factor);
            c.b = applyContrast(applyBrightness(c.b, m_brightness), contrast_factor);
            layer.setPixel(x, y, c);
          }
        }
      }
    }
  }

  void BrightnessContrastEffect::applySelectionEffect(const IntRect&, MaskablePixelData& selectionLayer)
  {
    auto contrast_factor = getContrastFactor(m_contrast);
    if(m_channel == "Red") {
      for(int y = 0, ysize = selectionLayer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = selectionLayer.getWidth(); x < xsize; ++x) {
          auto c = selectionLayer.getPixel(x, y);
          if(c.a > 0) {
            c.r = applyContrast(applyBrightness(c.r, m_brightness), contrast_factor);
            selectionLayer.setPixel(x, y, c);
          }
        }
      }
    } else if(m_channel == "Green") {
      for(int y = 0, ysize = selectionLayer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = selectionLayer.getWidth(); x < xsize; ++x) {
          auto c = selectionLayer.getPixel(x, y);
          if(c.a > 0) {
            c.g = applyContrast(applyBrightness(c.g, m_brightness), contrast_factor);
            selectionLayer.setPixel(x, y, c);
          }
        }
      }
    } else if(m_channel == "Blue") {
      for(int y = 0, ysize = selectionLayer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = selectionLayer.getWidth(); x < xsize; ++x) {
          auto c = selectionLayer.getPixel(x, y);
          if(c.a > 0) {
            c.b = applyContrast(applyBrightness(c.b, m_brightness), contrast_factor);
            selectionLayer.setPixel(x, y, c);
          }
        }
      }
    } else {
      for(int y = 0, ysize = selectionLayer.getHeight(); y < ysize; ++y) {
        for(int x = 0, xsize = selectionLayer.getWidth(); x < xsize; ++x) {
          auto c = selectionLayer.getPixel(x, y);
          if(c.a > 0) {
            c.r = applyContrast(applyBrightness(c.r, m_brightness), contrast_factor);
            c.g = applyContrast(applyBrightness(c.g, m_brightness), contrast_factor);
            c.b = applyContrast(applyBrightness(c.b, m_brightness), contrast_factor);
            selectionLayer.setPixel(x, y, c);
          }
        }
      }
    }
  }
}
