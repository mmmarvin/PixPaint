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
#include "painttoolregistrar.h"

#include "../3rdparty/gengine/configuration.h"
#include "../manager/consolemanager.h"
#include "../define.h"

namespace pixpaint
{
namespace
{
  void write_exceed_limit_error(const std::string& name,
                                const std::string& field,
                                int size_limit)
  {
    getConsoleManager().writeMessage("Cannot register tool \"" +
                                     name +
                                     "\" because " +
                                     field +
                                     " exceeds limit (>"+
                                     std::to_string(size_limit) +
                                     " characters long)");
  }

  bool check_fields(const std::string& name,
                    const std::string& shortcut)
  {
    if(name.size() > registerbase_detail::NAME_SIZE_LIMIT || shortcut.size() > registerbase_detail::SHORTCUT_SIZE_LIMIT) {
      if(name.size() > registerbase_detail::NAME_SIZE_LIMIT) {
        write_exceed_limit_error(name, name, registerbase_detail::NAME_SIZE_LIMIT);
      } else if(shortcut.size() > registerbase_detail::SHORTCUT_SIZE_LIMIT) {
        write_exceed_limit_error(name, shortcut, registerbase_detail::SHORTCUT_SIZE_LIMIT);
      }

      return false;
    }

    return true;
  }
}
  void PaintToolRegistrar::registerTool(std::string name,
                                        std::string iconFilename,
                                        std::string id,
                                        std::string shortcut,
                                        std::unique_ptr<PaintToolBase> paintTool)
  {
    id = "id_tool_" + id;
    if(!check_fields(name, shortcut)) {
      return;
    }

    if(RegistrarBase::registerBuiltIn(id,
                                      PaintToolInformation(std::move(name),
                                                           std::move(iconFilename),
                                                           id,
                                                           shortcut,
                                                           std::make_unique<BuiltInPaintToolHandler>(std::move(paintTool))))) {
      gengine2d::getConfigurationManager().addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT,
                                                       "tool_shortcut_" + std::move(id),
                                                       std::move(shortcut),
                                                       gengine2d::EVariableType::String);
    }
  }

  void PaintToolRegistrar::registerCustomTool(std::string name,
                                              std::string iconFilename,
                                              std::string id,
                                              std::string shortcut,
                                              boost::python::object paintToolObject)
  {
    id = "id_tool_" + id;
    if(!check_fields(name, shortcut)) {
      return;
    }

    if(RegistrarBase::registerCustom(id,
                                     PaintToolInformation(std::move(name),
                                                          std::move(iconFilename),
                                                          id,
                                                          shortcut,
                                                          std::make_unique<CustomPaintToolHandler>(paintToolObject)))) {
      gengine2d::getConfigurationManager().addVariable(CONFIG_SECTION_KEYBOARD_SHORTCUT,
                                                       "tool_shortcut_" + std::move(id),
                                                       std::move(shortcut),
                                                       gengine2d::EVariableType::String);
    }
  }

  PIXPAINT_SINGLETON_FUNC_DEF(PaintToolRegistrar)
}
