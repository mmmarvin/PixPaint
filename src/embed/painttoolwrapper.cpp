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
#include "painttoolwrapper.h"

#include <boost/python/errors.hpp>
#include "../image/color.h"
#include "../image/point.h"
#include "../image/maskablepixeldata.h"
#include "../image/modifyablepixeldata.h"
#include "../debug_log.h"
#include "script_errors.h"

namespace pixpaint
{
  int PaintToolWrapper::onKeyPress(EKey key,
                                   const Color& color,
                                   ControlState controlState,
                                   ModifyablePixelData& previewLayer,
                                   MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override(("onKeyPress"))) {
      try {
        return f(key,
                 color,
                 controlState,
                 boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onKeyPress(key, color, controlState, previewLayer, currentLayer);
  }

  int PaintToolWrapper::onKeyRelease(EKey key,
                                     const Color& color,
                                     ControlState controlState,
                                     ModifyablePixelData& previewLayer,
                                     MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override(("onKeyRelease"))) {
      try {
        return f(key,
                 color,
                 controlState,
                 boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onKeyRelease(key, color, controlState, previewLayer, currentLayer);
  }

  void PaintToolWrapper::onPreMousePress()
  {
    if(auto f = wrapper_base_type::get_override("onPreMousePress")) {
      try {
        f();
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onPreMousePress();
  }

  bool PaintToolWrapper::onMousePress(const Point& currentPoint,
                                      const Point& previousPoint,
                                      const Point& globalPoint,
                                      const Color& color,
                                      ControlState controlState,
                                      ModifyablePixelData& previewLayer,
                                      MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onMousePress")) {
      try {
        return f(currentPoint,
                 previousPoint,
                 globalPoint,
                 color,
                 controlState,
                 boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onMousePress(currentPoint,
                                       previousPoint,
                                       globalPoint,
                                       color,
                                       controlState,
                                       previewLayer,
                                       currentLayer);
  }

  bool PaintToolWrapper::onMouseMove(const Point& currentPoint,
                                     const Point& previousPoint,
                                     const Point& globalPoint,
                                     const Color& color,
                                     ControlState controlState,
                                     ModifyablePixelData& previewLayer,
                                     MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onMouseMove")) {
      try {
        return f(currentPoint,
                 previousPoint,
                 globalPoint,
                 color,
                 controlState,
                 boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onMouseMove(currentPoint,
                                      previousPoint,
                                      globalPoint,
                                      color,
                                      controlState,
                                      previewLayer,
                                      currentLayer);
  }

  int PaintToolWrapper::onMouseRelease(const Point& currentPoint,
                                       const Point& previousPoint,
                                       const Point& globalPoint,
                                       const Color& color,
                                       ControlState controlState,
                                       ModifyablePixelData& previewLayer,
                                       MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onMouseRelease")) {
      try {
        return f(currentPoint,
                 previousPoint,
                 globalPoint,
                 color,
                 controlState,
                 boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onMouseRelease(currentPoint,
                                         previousPoint,
                                         globalPoint,
                                         color,
                                         controlState,
                                         previewLayer,
                                         currentLayer);
  }

  bool PaintToolWrapper::onFinalize(ModifyablePixelData& previewLayer, MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onFinalize")) {
      try {
        return f(boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onFinalize(previewLayer, currentLayer);
  }

  bool PaintToolWrapper::onResize(ModifyablePixelData& previewLayer, MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onResize")) {
      try {
        return f(boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onResize(previewLayer, currentLayer);
  }

  int PaintToolWrapper::onToolChange(ModifyablePixelData& previewLayer,
                                     MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onToolChange")) {
      try {
        return f(boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onToolChange(previewLayer, currentLayer);
  }

  int PaintToolWrapper::onColorChange(ModifyablePixelData& previewLayer,
                                      MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onColorChange")) {
      try {
        return f(boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onColorChange(previewLayer, currentLayer);
  }

  int PaintToolWrapper::onOptionChange(ModifyablePixelData& previewLayer,
                                       MaskablePixelData& currentLayer)
  {
    if(auto f = wrapper_base_type::get_override("onOptionChange")) {
      try {
        return f(boost::ref(previewLayer),
                 boost::ref(currentLayer));
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::onOptionChange(previewLayer, currentLayer);
  }

  bool PaintToolWrapper::hasFinalize() const
  {
    if(auto f = wrapper_base_type::get_override("hasFinalize")) {
      try {
        return f();
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::hasFinalize();
  }

  bool PaintToolWrapper::hasRightClick() const
  {
    if(auto f = wrapper_base_type::get_override("hasRightClick")) {
      try {
        return f();
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::hasRightClick();
  }

  bool PaintToolWrapper::isInstant() const
  {
    if(auto f = wrapper_base_type::get_override("isInstant")) {
      try {
        return f();
      } catch(const boost::python::error_already_set&) {
        printScriptError();
      }
    }

    return PaintToolBase::isInstant();
  }

  IntRect PaintToolWrapper::getDrawRect() const
  {
    return wrapper_base_type::get_override("getDrawRect")();
  }

  std::string PaintToolWrapper::getDescription() const
  {
    return wrapper_base_type::get_override("getDescription")();
  }

  void PaintToolWrapper::addFlagOptionWrapper(std::string name, bool value)
  {
    if(!optionExists(name)) {
      auto res = m_bools.emplace(name, value);
      if(res.second) {
        addFlagOption(&res.first->second, std::move(name));
      }
    }
  }

  void PaintToolWrapper::addIntegerOptionWrapper(std::string name, int value, int min, int max)
  {
    if(!optionExists(name)) {
      auto res = m_integers.emplace(name, value);
      if(res.second) {
        addIntegerValueOption(&res.first->second, std::move(name), min, max);
      }
    }
  }

  void PaintToolWrapper::addStringOptionWrapper(std::string name, std::string value)
  {
    if(!optionExists(name)) {
      auto res = m_strings.emplace(name, value);
      if(res.second) {
        addStringOption(&res.first->second, std::move(name));
      }
    }
  }

  void PaintToolWrapper::addStringChoiceWrapper(std::string name,
                                                size_t valueIndex,
                                                const std::vector<std::string>& choices)
  {
    if(!optionExists(name) && valueIndex < choices.size()) {
      auto res = m_strings.emplace(name, choices[valueIndex]);
      if(res.second) {
        addStringChoice(&res.first->second, std::move(name), choices);
      }
    }
  }

  bool PaintToolWrapper::getFlagOptionWrapper(const std::string& name)
  {
    return m_bools.at(name);
  }

  int PaintToolWrapper::getIntegerOptionWrapper(const std::string& name)
  {
    return m_integers.at(name);
  }

  const std::string& PaintToolWrapper::getStringOptionWrapper(const std::string& name)
  {
    return m_strings.at(name);
  }

  const std::string& PaintToolWrapper::getStringChoiceWrapper(const std::string& name)
  {
    return m_strings.at(name);
  }

  bool PaintToolWrapper::optionExists(const std::string& name) const
  {
    return m_bools.find(name) != m_bools.end() ||
           m_integers.find(name) != m_integers.end() ||
           m_strings.find(name) != m_strings.end();
  }
}
