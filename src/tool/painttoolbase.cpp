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
#include "painttoolbase.h"

#include "../assert.h"

namespace pixpaint
{
  /**********
   * Paint Tool Base
   **********/
  bool PaintToolBase::onKeyPress(EKey,
                                 const Color&,
                                 ControlState,
                                 ModifyablePixelData&,
                                 MaskablePixelData&)
  {
    return false;
  }

  bool PaintToolBase::onKeyRelease(EKey,
                                   const Color&,
                                   ControlState,
                                   ModifyablePixelData&,
                                   MaskablePixelData&)
  {
    return false;
  }

  void PaintToolBase::onPreMousePress()
  {
  }

  bool PaintToolBase::onMousePress(const Point&,
                                   const Point&,
                                   const Point&,
                                   const Color&,
                                   ControlState,
                                   ModifyablePixelData&,
                                   MaskablePixelData&)
  {
    return false;
  }

  bool PaintToolBase::onMouseMove(const Point&,
                                  const Point&,
                                  const Point&,
                                  const Color&,
                                  ControlState,
                                  ModifyablePixelData&,
                                  MaskablePixelData&)
  {
    return false;
  }

  bool PaintToolBase::onMouseRelease(const Point&,
                                     const Point&,
                                     const Point&,
                                     const Color&,
                                     ControlState,
                                     ModifyablePixelData&,
                                     MaskablePixelData&)
  {
    return false;
  }

  bool PaintToolBase::onFinalize(ModifyablePixelData&,
                                 MaskablePixelData&)
  {
    return false;
  }

  bool PaintToolBase::onResize(ModifyablePixelData&,
                               MaskablePixelData&)
  {
    return false;
  }

  int PaintToolBase::onToolChange(ModifyablePixelData&,
                                  MaskablePixelData&)
  {
    return 0;
  }

  int PaintToolBase::onColorChange(ModifyablePixelData&,
                                   MaskablePixelData&)
  {
    return 0;
  }

  int PaintToolBase::onOptionChange(ModifyablePixelData&,
                                    MaskablePixelData&)
  {
    return 0;
  }

  bool PaintToolBase::hasFinalize() const
  {
    return false;
  }

  bool PaintToolBase::hasRightClick() const
  {
    return false;
  }

  bool PaintToolBase::isInstant() const
  {
    return false;
  }

  optional<IntRect> PaintToolBase::getOldDrawRect() const
  {
    // getOldRect is only useful for tools that incorporate moving (ex. selection tool) so that
    // it can repaint an old rect (previous rect position)
    return os_specific::nullopt;
  }

  optional<IntRect> PaintToolBase::getHistoryRect() const
  {
    return os_specific::nullopt;
  }

  optional<const Cursor&> PaintToolBase::getCursor() const
  {
    return os_specific::nullopt;
  }

  bool PaintToolHandlerBase::onKeyPress(EKey key,
                                        const Color& color,
                                        ControlState controlState,
                                        ModifyablePixelData& previewLayer,
                                        MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onKeyPress(key, color, controlState, previewLayer, currentLayer);
  }

  bool PaintToolHandlerBase::onKeyRelease(EKey key,
                                          const Color& color,
                                          ControlState controlState,
                                          ModifyablePixelData& previewLayer,
                                          MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onKeyRelease(key, color, controlState, previewLayer, currentLayer);
  }

  void PaintToolHandlerBase::onPreMousePress()
  {
    m_paintToolBase->onPreMousePress();
  }

  bool PaintToolHandlerBase::onMousePress(const Point& currentPoint,
                                          const Point& previousPoint,
                                          const Point& globalPoint,
                                          const Color& color,
                                          ControlState controlState,
                                          ModifyablePixelData& previewLayer,
                                          MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onMousePress(currentPoint,
                                         previousPoint,
                                         globalPoint,
                                         color,
                                         controlState,
                                         previewLayer,
                                         currentLayer);
  }

  bool PaintToolHandlerBase::onMouseMove(const Point& currentPoint,
                                         const Point& previousPoint,
                                         const Point& globalPoint,
                                         const Color& color,
                                         ControlState controlState,
                                         ModifyablePixelData& previewLayer,
                                         MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onMouseMove(currentPoint,
                                        previousPoint,
                                        globalPoint,
                                        color,
                                        controlState,
                                        previewLayer,
                                        currentLayer);
  }

  bool PaintToolHandlerBase::onMouseRelease(const Point& currentPoint,
                                            const Point& previousPoint,
                                            const Point& globalPoint,
                                            const Color& color,
                                            ControlState controlState,
                                            ModifyablePixelData& previewLayer,
                                            MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onMouseRelease(currentPoint,
                                           previousPoint,
                                           globalPoint,
                                           color,
                                           controlState,
                                           previewLayer,
                                           currentLayer);
  }

  bool PaintToolHandlerBase::onFinalize(ModifyablePixelData& previewLayer,
                                        MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onFinalize(previewLayer, currentLayer);
  }

  bool PaintToolHandlerBase::onResize(ModifyablePixelData& previewLayer,
                                      MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onResize(previewLayer, currentLayer);
  }

  int PaintToolHandlerBase::onToolChange(ModifyablePixelData& previewLayer,
                                         MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onToolChange(previewLayer, currentLayer);
  }

  int PaintToolHandlerBase::onColorChange(ModifyablePixelData& previewLayer,
                                          MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onColorChange(previewLayer, currentLayer);
  }

  int PaintToolHandlerBase::onOptionChange(ModifyablePixelData& previewLayer,
                                           MaskablePixelData& currentLayer)
  {
    return m_paintToolBase->onOptionChange(previewLayer, currentLayer);
  }

  bool PaintToolHandlerBase::hasFinalize() const
  {
    return m_paintToolBase->hasFinalize();
  }

  bool PaintToolHandlerBase::hasRightClick() const
  {
    return m_paintToolBase->hasRightClick();
  }

  bool PaintToolHandlerBase::isInstant() const
  {
    return m_paintToolBase->isInstant();
  }

  IntRect PaintToolHandlerBase::getDrawRect() const
  {
    return m_paintToolBase->getDrawRect();
  }

  optional<IntRect> PaintToolHandlerBase::getOldDrawRect() const
  {
    return m_paintToolBase->getOldDrawRect();
  }

  optional<IntRect> PaintToolHandlerBase::getHistoryRect() const
  {
    return m_paintToolBase->getHistoryRect();
  }

  std::string PaintToolHandlerBase::getDescription() const
  {
    return m_paintToolBase->getDescription();
  }

  optional<const Cursor&> PaintToolHandlerBase::getCursor() const
  {
    return m_paintToolBase->getCursor();
  }

  const PaintToolBase::option_container_type& PaintToolHandlerBase::getOptions() const
  {
    return m_paintToolBase->getOptions();
  }

  void PaintToolHandlerBase::setPaintToolBase(PaintToolBase* paintToolBase)
  {
    m_paintToolBase = paintToolBase;
  }

  /**********
   * Built-In Paint Tool Handler
   **********/
  BuiltInPaintToolHandler::BuiltInPaintToolHandler(std::unique_ptr<PaintToolBase> paintTool) :
    m_paintTool(std::move(paintTool))
  {
    PIXPAINT_ASSERT(m_paintTool.get(), "Cannot have null paint tool");
    this->setPaintToolBase(m_paintTool.get());
  }

  /**********
   * Custom Paint Tool Handler
   **********/
  // NOTE: Must try/catch on instantiation of this class,
  // as extract might throw if the object is not convertible to
  // PaintToolBase
  CustomPaintToolHandler::CustomPaintToolHandler(boost::python::object paintToolObject) :
    m_paintToolObject(paintToolObject)
  {
    auto* base_ptr = boost::python::extract<PaintToolBase*>(m_paintToolObject)();
    PIXPAINT_ASSERT(base_ptr, "Cannot have null paint tool");
    this->setPaintToolBase(base_ptr);
  }
}
