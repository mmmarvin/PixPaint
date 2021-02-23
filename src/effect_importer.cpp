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
#include "effect_importer.h"

#include "effect/brightnesscontrasteffect.h"
#include "effect/huesaturationlightness.h"
#include "registrar/effectregistrar.h"

namespace pixpaint
{
  void register_fixed_effects()
  {
    auto& registrar = getEffectRegistrar();
    registrar.registerEffect("Brightness / Contrast",
                             "",
                             "brgt",
                             std::make_unique<BrightnessContrastEffect>(),
                             true);
    registrar.registerEffect("Hue / Saturation",
                             "",
                             "hues",
                             std::make_unique<HueSaturationLightness>(),
                             true);
  }
}
