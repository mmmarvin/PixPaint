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
#include "effectbase.h"

namespace pixpaint
{
  /**********
   * EffectBase
   **********/
  EffectBase::EffectBase()
  {
    this->reset();
  }

  /**********
   * Built-In Effect
   **********/
  BuiltInEffectHandler::BuiltInEffectHandler(std::unique_ptr<EffectBase> effect) :
    m_effect(std::move(effect))
  {
  }

  void BuiltInEffectHandler::reset()
  {
    return m_effect->reset();
  }

  void BuiltInEffectHandler::applyLayerEffect(const IntRect& rect, MaskablePixelData& layer)
  {
    m_effect->applyLayerEffect(rect, layer);
  }

  void BuiltInEffectHandler::applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer)
  {
    m_effect->applySelectionEffect(rect, selectionLayer);
  }

  const EffectBase::option_container_type& BuiltInEffectHandler::getOptions() const
  {
    return m_effect->getOptions();
  }

  /**********
   * Custom Effect
   **********/
  CustomEffectHandler::CustomEffectHandler(boost::python::object effectObject) :
    m_effectObject(effectObject),
    m_effect(boost::python::extract<EffectBase*>(effectObject)())
  {
  }

  void CustomEffectHandler::reset()
  {
    return m_effect->reset();
  }

  void CustomEffectHandler::applyLayerEffect(const IntRect& rect, MaskablePixelData& layer)
  {
    m_effect->applyLayerEffect(rect, layer);
  }

  void CustomEffectHandler::applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer)
  {
    m_effect->applySelectionEffect(rect, selectionLayer);
  }

  const EffectBase::option_container_type& CustomEffectHandler::getOptions() const
  {
    return m_effect->getOptions();
  }
}
