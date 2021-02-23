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
#include "selectionmanager.h"

#include "../drawer/drawer_config.h"
#include "../env/imageenvironment.h"
#include "../history/history.h"
#include "../history/selectionaction.h"
#include "../history/selectionmoveaction.h"
#include "../manager/historymanager.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
  SelectionManager::SelectionManager() :
    m_selectionLayerMask(0, 0),
    m_selectionLayer(0, 0),
    m_previewLayer(0, 0),
    m_resizeMode(MaskablePixelData::EResizeMode::SCALE),
    m_flagSelectionLayer(false),
    m_flagSelectionRect(false),
    m_flagSelectionMask(false)
  {
    drawer_config::createDrawers(*this);
  }

  void SelectionManager::setSelectionRect(position_t x,
                                          position_t y,
                                          position_t width,
                                          position_t height,
                                          bool refreshSelection,
                                          bool addToHistory)
  {
    m_selection = IntRect(x, y, width, height);
    m_flagSelectionRect = true;

    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      emitHistoryAction(SelectionAction());
    }
  }

  void SelectionManager::setSelectionRect(const IntRect& rect,
                                          bool refreshSelection,
                                          bool addToHistory)
  {
    setSelectionRect(rect.x, rect.y, rect.width, rect.height, refreshSelection, addToHistory);
  }

  void SelectionManager::setSelectionRectCenter(position_t center_x,
                                                position_t center_y,
                                                position_t width,
                                                position_t height,
                                                bool refreshSelection,
                                                bool addToHistory)
  {
    m_selection = IntRect(center_x - (width / 2),
                              center_y - (height / 2),
                              width,
                              height);
    m_flagSelectionRect = true;

    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      emitHistoryAction(SelectionAction());
    }
  }

  void SelectionManager::setSelectionRectCenter(const IntRect& rect,
                                                bool refreshSelection,
                                                bool addToHistory)
  {
    setSelectionRectCenter(rect.x, rect.y, rect.width, rect.height, refreshSelection, addToHistory);
  }

  void SelectionManager::moveSelectionRect(position_t x,
                                           position_t y,
                                           bool refreshSelection,
                                           bool addToHistory)
  {
    auto old_selection_position = position(m_selection);
    m_selection.x = x;
    m_selection.y = y;

    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
    if(addToHistory) {
      emitHistoryAction(SelectionMoveAction(old_selection_position, position(m_selection)));
    }
  }

  void SelectionManager::moveSelectionRect(const Point& position,
                                           bool refreshSelection,
                                           bool addToHistory)
  {
    moveSelectionRect(position.x, position.y, refreshSelection, addToHistory);
  }

  void SelectionManager::resizeSelectionRect(position_t width,
                                             position_t height,
                                             const Color& color,
                                             bool refreshSelection)
  {
    if(m_selection.width != width ||
       m_selection.height != height) {
      m_selection.width = width;
      m_selection.height = height;
      getSelectionLayer().resize(width, height, color, getResizeMode());

      if(m_flagSelectionLayer) {
        setSelectionLayer(getSelectionLayer());
      }
    }
    if(refreshSelection) {
      getImageEnvironment().getScrollArea().updateSelectionWidget();
    }
  }

  void SelectionManager::resizeSelectionRect(const Point& dimension,
                                             const Color& color,
                                             bool refreshSelection)
  {
    resizeSelectionRect(dimension.x, dimension.y, color, refreshSelection);
  }

  void SelectionManager::setSelectionLayer(const MaskablePixelData& selectionLayer)
  {
    getSelectionLayer() = selectionLayer;
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getLayerDrawer(), &getSelectionLayer());

    auto& selection_preview_layer = getSelectionPreviewLayer();
    selection_preview_layer = ModifyablePixelData(selectionLayer.getWidth(), selectionLayer.getHeight());
    selection_preview_layer.clear(Color::TRANSPARENT);
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getPreviewDrawer(), &selection_preview_layer);

    m_flagSelectionLayer = true;
  }

  void SelectionManager::setSelectionLayer(const MaskablePixelData& selectionLayer, const PixelData& mask)
  {
    setSelectionLayer(selectionLayer);
    getSelectionLayerMask() = mask;

    auto m = getLayerDrawer().createMaskFromLayer(mask);
    LayerPreviewDrawerHolder::setMask(LayerPreviewDrawerHolder::getLayerDrawer(), m);
    LayerPreviewDrawerHolder::setMask(LayerPreviewDrawerHolder::getPreviewDrawer(), m);
    getSelectionLayer().setMask(m);

    m_flagSelectionMask = true;
  }

  void SelectionManager::setSelectionLayer(MaskablePixelData&& selectionLayer)
  {
    getSelectionLayer() = std::move(selectionLayer);
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getLayerDrawer(), &getSelectionLayer());

    auto& selection_preview_layer = getSelectionPreviewLayer();
    selection_preview_layer = ModifyablePixelData(getSelectionLayer().getWidth(),
                                                  getSelectionLayer().getHeight());
    selection_preview_layer.clear(Color::TRANSPARENT);
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getPreviewDrawer(), &selection_preview_layer);

    m_flagSelectionLayer = true;
  }

  void SelectionManager::setSelectionLayer(MaskablePixelData&& selectionLayer, PixelData&& mask)
  {
    setSelectionLayer(std::move(selectionLayer));
    getSelectionLayerMask() = mask;

    auto m = getLayerDrawer().createMaskFromLayer(mask);
    LayerPreviewDrawerHolder::setMask(LayerPreviewDrawerHolder::getLayerDrawer(), m);
    LayerPreviewDrawerHolder::setMask(LayerPreviewDrawerHolder::getPreviewDrawer(), m);
    getSelectionLayer().setMask(m);

    m_flagSelectionMask = true;
  }

  void SelectionManager::setResizeMode(MaskablePixelData::EResizeMode mode)
  {
    m_resizeMode = mode;
  }

  void SelectionManager::clear()
  {
    getSelectionLayer() = MaskablePixelData(0, 0);
    getSelectionPreviewLayer() = ModifyablePixelData(0, 0);
    getSelectionLayerMask() = PixelData(0, 0);
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getLayerDrawer(), nullptr);
    LayerPreviewDrawerHolder::setTargetLayer(LayerPreviewDrawerHolder::getPreviewDrawer(), nullptr);
    LayerPreviewDrawerHolder::clearMask(LayerPreviewDrawerHolder::getLayerDrawer());
    LayerPreviewDrawerHolder::clearMask(LayerPreviewDrawerHolder::getPreviewDrawer());

    m_flagSelectionLayer = false;
    m_flagSelectionRect = false;
    m_flagSelectionMask = false;

    getImageEnvironment().getScrollArea().updateSelectionWidget();
  }

  bool SelectionManager::selectionExists() const noexcept
  {
    return m_flagSelectionRect;
  }

  bool SelectionManager::layerExists() const noexcept
  {
    return m_flagSelectionLayer;
  }

  bool SelectionManager::layerMaskExists() const noexcept
  {
    return m_flagSelectionMask;
  }

  const IntRect& SelectionManager::getSelectionRect() const noexcept
  {
    return m_selection;
  }

  MaskablePixelData::EResizeMode SelectionManager::getResizeMode() const noexcept
  {
    return m_resizeMode;
  }

  MaskablePixelData& SelectionManager::getSelectionLayer() noexcept
  {
    return m_selectionLayer;
  }

  const MaskablePixelData& SelectionManager::getSelectionLayer() const noexcept
  {
    return m_selectionLayer;
  }

  ModifyablePixelData& SelectionManager::getSelectionPreviewLayer() noexcept
  {
    return m_previewLayer;
  }

  const ModifyablePixelData& SelectionManager::getSelectionPreviewLayer() const noexcept
  {
    return m_previewLayer;
  }

  PixelData& SelectionManager::getSelectionLayerMask() noexcept
  {
    return m_selectionLayerMask;
  }

  const PixelData& SelectionManager::getSelectionLayerMask() const noexcept
  {
    return m_selectionLayerMask;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(SelectionManager)
}
