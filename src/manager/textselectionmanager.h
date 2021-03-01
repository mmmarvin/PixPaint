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
#ifndef TEXTSELECTIONMANAGER_H
#define TEXTSELECTIONMANAGER_H

#include "../drawer/drawerholder.h"
#include "../image/maskablepixeldata.h"
#include "../image/rect.h"
#include "../pattern/singleton.h"

namespace pixpaint
{
  class TextSelectionManager : public LayerDrawerHolder
  {
    TextSelectionManager();

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
    void setSelectionRectCenter(position_t centerX,
                                position_t centerY,
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

    void clear();

    bool selectionExists() const noexcept;

    const IntRect&  getSelectionRect() const noexcept;

    MaskablePixelData& getTextLayer() noexcept;
    const MaskablePixelData& getTextLayer() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(TextSelectionManager)
    friend class SelectionWidget;

    MaskablePixelData m_selectionLayer;

    IntRect           m_selection;
    bool              m_flagSelection;
  };

  PIXPAINT_SINGLETON_FUNC(TextSelectionManager)
}

#endif // TEXTSELECTIONMANAGER_H
