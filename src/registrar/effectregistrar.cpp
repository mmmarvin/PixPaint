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
#include "effectregistrar.h"

namespace pixpaint
{
  void EffectRegistrar::registerEffect(std::string name,
                                       std::string category,
                                       std::unique_ptr<EffectBase> effect,
                                       bool showPreview)
  {
    RegistrarBase::registerBuiltIn(EffectInformation(std::move(name),
                                                     std::move(category),
                                                     std::make_unique<BuiltInEffectHandler>(std::move(effect)),
                                                     showPreview));
  }

  void EffectRegistrar::registerEffect(std::string name,
                                       std::string category,
                                       boost::python::object effectObject,
                                       bool showPreview)
  {
    RegistrarBase::registerCustom(EffectInformation(std::move(name),
                                                    std::move(category),
                                                    std::make_unique<CustomEffectHandler>(std::move(effectObject)),
                                                    showPreview));
  }

  PIXPAINT_SINGLETON_FUNC_DEF(EffectRegistrar)
}
