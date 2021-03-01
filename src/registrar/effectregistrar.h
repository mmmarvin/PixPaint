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
#ifndef EFFECTREGISTRAR_H
#define EFFECTREGISTRAR_H

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../effect/effectbase.h"
#include "../pattern/singleton.h"
#include "information/effectinformation.h"
#include "registrarbase.h"

namespace pixpaint
{
  class EffectRegistrar : public RegistrarBase<EffectInformation>
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(EffectRegistrar)
    EffectRegistrar() {}

  public:
    void registerEffect(std::string name,
                        std::string category,
                        std::string id,
                        std::unique_ptr<EffectBase> effect,
                        bool showPreview = false);
    void registerEffect(std::string name,
                        std::string category,
                        std::string id,
                        boost::python::object effectObject,
                        bool showPreview = false);
  };

  PIXPAINT_SINGLETON_FUNC(EffectRegistrar)
}

#endif // EFFECTREGISTRAR_H
