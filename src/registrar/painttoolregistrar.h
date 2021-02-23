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
#ifndef PAINTTOOLREGISTRAR_H
#define PAINTTOOLREGISTRAR_H

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../tool/painttoolbase.h"
#include "../pixpaint_macro.h"
#include "information/painttoolinformation.h"
#include "registrarbase.h"

namespace pixpaint
{
  class PaintToolRegistrar : public RegistrarBase<PaintToolInformation>
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(PaintToolRegistrar)
    PaintToolRegistrar() {}

  public:


    void registerTool(std::string name,
                      std::string iconFilename,
                      std::unique_ptr<PaintToolBase> paintTool);
    void registerCustomTool(std::string name,
                            std::string iconFilename,
                            boost::python::object paintToolObject);
  };

  PIXPAINT_SINGLETON_FUNC(PaintToolRegistrar)
}

#endif // PAINTTOOLREGISTRAR_H
