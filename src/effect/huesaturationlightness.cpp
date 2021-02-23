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
#include "huesaturationlightness.h"

#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"

namespace pixpaint
{
namespace
{
  void apply_hsl(int hue, int saturation, int lightness, QColor& color)
  {
    if(color.alpha() > 0) {
      auto h = color.hslHue();
      auto s = color.hslSaturation();
      auto l = color.lightness();
      h = general_utils::clamp(h + hue, 0, 359);
      s = general_utils::clamp(s + saturation, 0, 255);
      l = general_utils::clamp(l + lightness, 0, 255);
      color.setHsl(h, s, l);
    }
  }

  bool is_color_in_channel(int mainChannel, int subChannel1, int subChannel2)
  {
    return mainChannel > subChannel1 &&
           mainChannel > subChannel2 &&
           std::abs(subChannel1 - subChannel2) <= 10;
  }

  static const auto determine_effect = [](const std::string& channel) -> std::function<QColor(QColor, int, int, int)>
  {
    static const auto applyEffectRed = [](QColor color, int hue, int saturation, int lightness)
    {
      auto r = color.red();
      auto g = color.green();
      auto b = color.blue();
      if(is_color_in_channel(r, g, b)) {
        apply_hsl(hue, saturation, lightness, color);
      }

      return color;
    };

    static const auto applyEffectGreen = [](QColor color, int hue, int saturation, int lightness)
    {
      auto r = color.red();
      auto g = color.green();
      auto b = color.blue();
      if(is_color_in_channel(g, r, b)) {
        apply_hsl(hue, saturation, lightness, color);
      }

      return color;
    };

    static const auto applyEffectBlue = [](QColor color, int hue, int saturation, int lightness)
    {
      auto r = color.red();
      auto g = color.green();
      auto b = color.blue();
      if(is_color_in_channel(b, r, g)) {
        apply_hsl(hue, saturation, lightness, color);
      }

      return color;
    };

    static const auto applyEffectAll = [](QColor color, int hue, int saturation, int lightness)
    {
      apply_hsl(hue, saturation, lightness, color);
      return color;
    };

    if(channel == "Red") {
      return applyEffectRed;
    } else if(channel == "Green") {
      return applyEffectGreen;
    } else if(channel == "Blue") {
      return applyEffectBlue;
    }

    return applyEffectAll;
  };
}
  HueSaturationLightness::HueSaturationLightness()
  {
    this->addIntegerValueOption(&m_hue, "Hue", -128, 128);
    this->addIntegerValueOption(&m_saturation, "Saturation", -128, 128);
    this->addIntegerValueOption(&m_lightness, "Lightness", -128, 128);
    this->addStringChoice(&m_channel, "Channel", { "All", "Red", "Green", "Blue" });
  }

  void HueSaturationLightness::reset()
  {
    m_channel = "All";
    m_hue = m_saturation = m_lightness = 0;
  }

  void HueSaturationLightness::applyLayerEffect(const IntRect&, MaskablePixelData& layer)
  {
    auto applyEffect = determine_effect(m_channel);

    for(int y = 0, ysize = layer.getHeight(); y < ysize; ++y) {
      for(int x = 0, xsize = layer.getWidth(); x < xsize; ++x) {
        auto c = applyEffect(qt_utils::convertToQTColor(layer.getPixel(x, y)),
                             m_hue,
                             m_saturation,
                             m_lightness);
        layer.setPixel(x, y, qt_utils::convertToColor(c));
      }
    }
  }

  void HueSaturationLightness::applySelectionEffect(const IntRect&, MaskablePixelData& selectionLayer)
  {
    auto applyEffect = determine_effect(m_channel);

    for(int y = 0, ysize = selectionLayer.getHeight(); y < ysize; ++y) {
      for(int x = 0, xsize = selectionLayer.getWidth(); x < xsize; ++x) {
        auto c = applyEffect(qt_utils::convertToQTColor(selectionLayer.getPixel(x, y)),
                             m_hue,
                             m_saturation,
                             m_lightness);
        selectionLayer.setPixel(x, y, qt_utils::convertToColor(c));
      }
    }
  }
}
