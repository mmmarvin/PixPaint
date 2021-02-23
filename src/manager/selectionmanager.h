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
#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include "../drawer/drawerholder.h"
#include "../image/modifyablepixeldata.h"
#include "../image/maskablepixeldata.h"
#include "../image/rect.h"
#include "../pixpaint_macro.h"

namespace pixpaint
{
  class SelectionManager : public LayerPreviewDrawerHolder
  {
    SelectionManager();

  public:
    void setSelectionRect(position_t x,
                          position_t y,
                          position_t width,
                          position_t height,
                          bool refreshSelection = true,
                          bool addToHistory = true);
    void setSelectionRect(const IntRect& rect,
                          bool refreshSelection = true,
                          bool addToHistory = true);
    void setSelectionRectCenter(position_t center_x,
                                position_t center_y,
                                position_t width,
                                position_t height,
                                bool refreshSelection = true,
                                bool addToHistory = true);
    void setSelectionRectCenter(const IntRect& rect,
                                bool refreshSelection = true,
                                bool addToHistory = true);
    void moveSelectionRect(position_t x, position_t y, bool refreshSelection = true, bool addToHistory = true);
    void moveSelectionRect(const Point& position, bool refreshSelection = true, bool addToHistory = true);
    void resizeSelectionRect(position_t width,
                             position_t height,
                             const Color& color = Color::TRANSPARENT,
                             bool refreshSelection = true);
    void resizeSelectionRect(const Point& dimension,
                             const Color& color = Color::TRANSPARENT,
                             bool refreshSelection = true);

    void setSelectionLayer(const MaskablePixelData& selectionLayer);
    void setSelectionLayer(const MaskablePixelData& selectionLayer, const PixelData& mask);
    void setSelectionLayer(MaskablePixelData&& selectionLayer);
    void setSelectionLayer(MaskablePixelData&& selectionLayer, PixelData&& mask);
    void setResizeMode(MaskablePixelData::EResizeMode mode);

    void clear();

    bool selectionExists() const noexcept;
    bool layerExists() const noexcept;
    bool layerMaskExists() const noexcept;

    const IntRect& getSelectionRect() const noexcept;
    MaskablePixelData::EResizeMode getResizeMode() const noexcept;

    MaskablePixelData& getSelectionLayer() noexcept;
    const MaskablePixelData& getSelectionLayer() const noexcept;
    ModifyablePixelData& getSelectionPreviewLayer() noexcept;
    const ModifyablePixelData& getSelectionPreviewLayer() const noexcept;
    PixelData& getSelectionLayerMask() noexcept;
    const PixelData& getSelectionLayerMask() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(SelectionManager)
    friend class SelectionWidget;

    PixelData                       m_selectionLayerMask;
    MaskablePixelData               m_selectionLayer;
    ModifyablePixelData             m_previewLayer;

    IntRect                         m_selection;

    MaskablePixelData::EResizeMode  m_resizeMode;
    bool                            m_flagSelectionLayer;
    bool                            m_flagSelectionRect;
    bool                            m_flagSelectionMask;
  };

  PIXPAINT_SINGLETON_FUNC(SelectionManager)
}

#endif // SELECTIONMANAGER_H
