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
#include "textselectionmanager.h"

#include "../drawer/drawer_config.h"
#include "../env/imageenvironment.h"
#include "../history/textselectionaction.h"
#include "../manager/historymanager.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
  TextSelectionManager::TextSelectionManager() :
    m_selectionLayer(0, 0),
    m_flagSelection(false)
  {
    drawer_config::createDrawers(*this);
  }

  void TextSelectionManager::setSelectionRect(position_t x,
                                              position_t y,
                                              position_t width,
                                              position_t height,
                                              bool refreshSelection,
                                              bool addToHistory)
  {
    setSelectionRect(IntRect(x, y, width, height), refreshSelection, addToHistory);
  }

  void TextSelectionManager::setSelectionRect(const IntRect& rect,
                                              bool refreshSelection,
                                              bool addToHistory)
  {
    m_selection = rect;
    m_selectionLayer = MaskablePixelData(rect.width, rect.height);
    m_flagSelection = true;

    LayerDrawerHolder::setTargetLayer(LayerDrawerHolder::getLayerDrawer(), &getTextLayer());

    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      emitHistoryAction(TextSelectionAction());
    }
  }

  void TextSelectionManager::setSelectionRectCenter(position_t centerX,
                                                    position_t centerY,
                                                    position_t width,
                                                    position_t height,
                                                    bool refreshSelection,
                                                    bool addToHistory)
  {
    m_selection = IntRect(centerX - (width / 2),
                          centerY - (height / 2),
                          width,
                          height);
    m_selectionLayer = MaskablePixelData(width, height);
    m_flagSelection = true;

    LayerDrawerHolder::setTargetLayer(LayerDrawerHolder::getLayerDrawer(), &getTextLayer());

    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      emitHistoryAction(TextSelectionAction());
    }
  }

  void TextSelectionManager::setSelectionRectCenter(const IntRect& rect,
                                                    bool refreshSelection,
                                                    bool addToHistory)
  {
    setSelectionRectCenter(rect.x, rect.y, rect.width, rect.height, refreshSelection, addToHistory);
  }

  void TextSelectionManager::moveSelectionRect(position_t x, position_t y, bool refreshSelection, bool addToHistory)
  {
    m_selection.x = x;
    m_selection.y = y;
    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      // HISTORY FIX: Add a history action
    }
  }

  void TextSelectionManager::moveSelectionRect(const Point& position, bool refreshSelection, bool addToHistory)
  {
    moveSelectionRect(position.x, position.y, refreshSelection, addToHistory);
  }

  void TextSelectionManager::resizeSelectionRect(position_t width,
                                                 position_t height,
                                                 const Color& color,
                                                 bool refreshSelection)
  {
    if(m_selection.width != width ||
       m_selection.height != height) {
      m_selection.width = width;
      m_selection.height = height;
      m_selectionLayer.resize(width, height, color, MaskablePixelData::EResizeMode::RESIZE);
      LayerDrawerHolder::setTargetLayer(LayerDrawerHolder::getLayerDrawer(), &getTextLayer());
    }
    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
  }

  void TextSelectionManager::resizeSelectionRect(const Point& dimension,
                                                 const Color& color,
                                                 bool refreshSelection)
  {
    resizeSelectionRect(dimension.x, dimension.y, color, refreshSelection);
  }

  void TextSelectionManager::clear()
  {
    m_selectionLayer = MaskablePixelData(0, 0);
    m_flagSelection = false;

    getImageEnvironment().getScrollArea().updateSelectionWidget();
  }

  bool TextSelectionManager::selectionExists() const noexcept
  {
    return m_flagSelection;
  }

  const IntRect&  TextSelectionManager::getSelectionRect() const noexcept
  {
    return m_selection;
  }

  MaskablePixelData& TextSelectionManager::getTextLayer() noexcept
  {
    return m_selectionLayer;
  }

  const MaskablePixelData& TextSelectionManager::getTextLayer() const noexcept
  {
    return m_selectionLayer;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(TextSelectionManager)
}
