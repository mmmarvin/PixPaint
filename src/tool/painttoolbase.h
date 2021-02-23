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
#ifndef PAINTTOOLBASE_H
#define PAINTTOOLBASE_H

#include <cinttypes>
#include <memory>
#include <vector>
#include <tuple>
#include <QPushButton>
#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../extension/optional.h"
#include "../image/rect.h"
#include "../value/optionvaluelist.h"
#include "../cursor.h"
#include "../keys.h"

namespace pixpaint
{
  struct Color;
  class Image;
  class ModifyablePixelData;
  class MaskablePixelData;
  struct Point;

  struct ControlState
  {
    bool shiftDown;
    bool controlDown;
  };

  class PaintToolBase : public OptionValueList
  {
  public:
    enum EChangeResult
    {
      ECCR_NONE = 0,
      ECCR_UPDATECURSOR = 1 << 0,
      ECCR_UPDATEIMAGE = 1 << 1
    };

    PaintToolBase() {}
    virtual ~PaintToolBase() {}

    virtual int onKeyPress(EKey key,
                           const Color& color,
                           ControlState controlState,
                           ModifyablePixelData& previewLayer,
                           MaskablePixelData& currentLayer);
    virtual int onKeyRelease(EKey key,
                             const Color& color,
                             ControlState controlState,
                             ModifyablePixelData& previewLayer,
                             MaskablePixelData& currentLayer);
    virtual void onPreMousePress();
    virtual bool onMousePress(const Point& currentPoint,
                              const Point& previousPoint,
                              const Point& globalPoint,
                              const Color& color,
                              ControlState controlState,
                              ModifyablePixelData& previewLayer,
                              MaskablePixelData& currentLayer);
    virtual bool onMouseMove(const Point& currentPoint,
                             const Point& previousPoint,
                             const Point& globalPoint,
                             const Color& color,
                             ControlState controlState,
                             ModifyablePixelData& previewLayer,
                             MaskablePixelData& currentLayer);
    virtual int onMouseRelease(const Point& currentPoint,
                               const Point& previousPoint,
                               const Point& globalPoint,
                               const Color& color,
                               ControlState controlState,
                               ModifyablePixelData& previewLayer,
                               MaskablePixelData& currentLayer);

    virtual bool onFinalize(ModifyablePixelData& previewLayer,
                            MaskablePixelData& currentLayer);
    virtual bool onResize(ModifyablePixelData& previewLayer,
                          MaskablePixelData& currentLayer);

    virtual int onToolChange(ModifyablePixelData& previewLayer,
                             MaskablePixelData& currentLayer);
    virtual int onColorChange(ModifyablePixelData& previewLayer,
                              MaskablePixelData& currentLayer);
    virtual int onOptionChange(ModifyablePixelData& previewLayer,
                               MaskablePixelData& currentLayer);

    virtual bool hasFinalize() const;
    virtual bool hasRightClick() const;
    virtual bool isInstant() const;

    virtual IntRect getDrawRect() const = 0;
    virtual optional<IntRect> getOldDrawRect() const;
    virtual optional<IntRect> getHistoryRect() const;
    virtual std::string getDescription() const = 0;
    virtual optional<const Cursor&> getCursor() const;
  };

  class PaintToolButton
  {
  public:
    virtual void click() = 0;
  };

  class PaintToolHandlerBase
  {
  public:
    PaintToolHandlerBase() {}
    virtual ~PaintToolHandlerBase() {}

    int onKeyPress(EKey key,
                   const Color& color,
                   ControlState controlState,
                   ModifyablePixelData& previewLayer,
                   MaskablePixelData& currentLayer);
    int onKeyRelease(EKey key,
                     const Color& color,
                     ControlState controlState,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer);
    void onPreMousePress();
    bool onMousePress(const Point& currentPoint,
                      const Point& previousPoint,
                      const Point& globalPoint,
                      const Color& color,
                      ControlState controlState,
                      ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer);
    bool onMouseMove(const Point& currentPoint,
                     const Point& previousPoint,
                     const Point& globalPoint,
                     const Color& color,
                     ControlState controlState,
                     ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer);
    int onMouseRelease(const Point& currentPoint,
                       const Point& previousPoint,
                       const Point& globalPoint,
                       const Color& color,
                       ControlState controlState,
                       ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer);

    bool onFinalize(ModifyablePixelData& previewLayer,
                    MaskablePixelData& currentLayer);
    bool onResize(ModifyablePixelData& previewLayer,
                  MaskablePixelData& currentLayer);

    int onToolChange(ModifyablePixelData& previewLayer,
                     MaskablePixelData& currentLayer);
    int onColorChange(ModifyablePixelData& previewLayer,
                      MaskablePixelData& currentLayer);
    int onOptionChange(ModifyablePixelData& previewLayer,
                       MaskablePixelData& currentLayer);

    bool hasFinalize() const;
    bool hasRightClick() const;
    bool isInstant() const;

    IntRect getDrawRect() const;
    optional<IntRect> getOldDrawRect() const;
    optional<IntRect> getHistoryRect() const;
    std::string getDescription() const;
    optional<const Cursor&> getCursor() const;

    const PaintToolBase::option_container_type& getOptions() const;

  protected:
    void setPaintToolBase(PaintToolBase* paintToolBase);

  private:
    friend class LeftToolbox;
    friend class PaintToolManager;

    PaintToolBase*    m_paintToolBase;
    PaintToolButton*  m_button;
  };

  class BuiltInPaintToolHandler : public PaintToolHandlerBase
  {
  public:
    BuiltInPaintToolHandler(std::unique_ptr<PaintToolBase> paintTool);

  private:
    std::unique_ptr<PaintToolBase> m_paintTool;
  };

  class CustomPaintToolHandler : public PaintToolHandlerBase
  {
  public:
    CustomPaintToolHandler(boost::python::object paintToolObject);

  private:
    boost::python::object m_paintToolObject;
  };
}

#endif // PAINTTOOLBASE_H
