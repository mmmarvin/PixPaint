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
#ifndef EFFECTWRAPPER_H
#define EFFECTWRAPPER_H

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../effect/effectbase.h"

namespace pixpaint
{
  class EffectWrapper : public EffectBase,
                        public boost::python::wrapper<EffectBase>
  {
    using wrapper_base_type = boost::python::wrapper<EffectBase>;

  public:
    EffectWrapper() {}

    void applyLayerEffect(const IntRect& rect, MaskablePixelData& layer) override;
    void applySelectionEffect(const IntRect& rect, MaskablePixelData& layer) override;
  };
}

#endif // EFFECTWRAPPER_H
