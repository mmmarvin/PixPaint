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
#ifndef PAINTTOOLWRAPPER_H
#define PAINTTOOLWRAPPER_H

#include <boost/bind/bind.hpp>
#include <boost/python/object.hpp>
#include <boost/python/wrapper.hpp>
#include "../3rdparty/ska/unordered_map.hpp"
#include "../tool/painttoolbase.h"

namespace pixpaint
{
  class PaintToolWrapper : public PaintToolBase,
                           public boost::python::wrapper<PaintToolBase>
  {
    using wrapper_base_type = boost::python::wrapper<PaintToolBase>;

  public:
    PaintToolWrapper() {}

    int onKeyPress(EKey key,
                   const Color& color,
                   ControlState controlState,
                   ModifyablePixelData& previewLayer,
                   MaskablePixelData& currentLayer) override;
    int onKeyRelease(EKey key,
                     const Color& color,
                     ControlState controlState,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer) override;

    void onPreMousePress() override;

    bool onMousePress(const Point& currentPoint,
                      const Point& previousPoint,
                      const Point& globalPoint,
                      const Color& color,
                      ControlState controlState,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer) override;
    bool onMouseMove(const Point& currentPoint,
                     const Point& previousPoint,
                     const Point& globalPoint,
                     const Color& color,
                     ControlState controlState,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer) override;
    int onMouseRelease(const Point& currentPoint,
                       const Point& previousPoint,
                       const Point& globalPoint,
                       const Color& color,
                       ControlState controlState,
                       ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer) override;

    bool onFinalize(ModifyablePixelData& previewLayer,
                    MaskablePixelData& currentLayer) override;
    bool onResize(ModifyablePixelData& previewLayer,
                  MaskablePixelData& currentLayer) override;

    int onToolChange(ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer) override;
    int onColorChange(ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer) override;
    int onOptionChange(ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer) override;

    bool hasFinalize() const override;
    bool hasRightClick() const override;
    bool isInstant() const override;

    IntRect getDrawRect() const override;
    std::string getDescription() const override;

  public:
    void addFlagOptionWrapper(std::string name, bool value);
    void addIntegerOptionWrapper(std::string name, int value, int min, int max);
    void addStringOptionWrapper(std::string name, std::string value);
    void addStringChoiceWrapper(std::string name, size_t valueIndex, const std::vector<std::string>& choices);

    bool getFlagOptionWrapper(const std::string& name);
    int getIntegerOptionWrapper(const std::string& name);
    const std::string& getStringOptionWrapper(const std::string& name);
    const std::string& getStringChoiceWrapper(const std::string& name);

  private:
    bool optionExists(const std::string& name) const;

    ska::unordered_map<std::string, bool>         m_bools;
    ska::unordered_map<std::string, int>          m_integers;
    ska::unordered_map<std::string, std::string>  m_strings;
  };
}

#endif // PAINTTOOLWRAPPER_H
