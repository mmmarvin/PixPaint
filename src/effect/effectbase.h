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
#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#include <string>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../image/maskablepixeldata.h"
#include "../image/rect.h"
#include "../value/optionvaluelist.h"

namespace pixpaint
{
  class MaskablePixelData;

  class EffectBase : public OptionValueList
  {
  public:
    EffectBase();
    virtual ~EffectBase() {}
    virtual void reset() {}

    virtual void applyLayerEffect(const IntRect& rect, MaskablePixelData& layer) = 0;
    virtual void applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer) = 0;
  };

  class EffectHandlerBase
  {
  public:
    EffectHandlerBase() {}
    virtual ~EffectHandlerBase() {}
    virtual void reset() = 0;

    virtual void applyLayerEffect(const IntRect& rect, MaskablePixelData& layer) = 0;
    virtual void applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer) = 0;

    virtual const EffectBase::option_container_type& getOptions() const = 0;
  };

  class BuiltInEffectHandler : public EffectHandlerBase
  {
  public:
    BuiltInEffectHandler(std::unique_ptr<EffectBase> effect);
    void reset() override final;

    void applyLayerEffect(const IntRect& rect, MaskablePixelData& layer) override final;
    void applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer) override final;

    const EffectBase::option_container_type& getOptions() const override final;

  private:
    std::unique_ptr<EffectBase> m_effect;
  };

  class CustomEffectHandler : public EffectHandlerBase
  {
  public:
    CustomEffectHandler(boost::python::object effectObject);
    void reset() override final;

    void applyLayerEffect(const IntRect& rect, MaskablePixelData& layer) override final;
    void applySelectionEffect(const IntRect& rect, MaskablePixelData& selectionLayer) override final;

    const EffectBase::option_container_type& getOptions() const override final;

  private:
    boost::python::object       m_effectObject;
    EffectBase*                 m_effect;
  };
}

#endif // EFFECTBASE_H
