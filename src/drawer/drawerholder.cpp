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
#include "drawerholder.h"

#include "../assert.h"

namespace pixpaint
{
  LayerDrawerHolder::LayerDrawerHolder() :
    m_layerDrawer(nullptr)
  {
  }

  Drawer& LayerDrawerHolder::getLayerDrawer()
  {
    PIXPAINT_ASSERT(m_layerDrawer.get(), "Layer Drawer was not initialized");
    return *m_layerDrawer;
  }

  void LayerDrawerHolder::setTargetLayer(Drawer& drawer, MaskablePixelData* layer)
  {
    drawer.setTargetLayer(layer);
  }

  void LayerDrawerHolder::setMask(Drawer& drawer, const std::shared_ptr<Mask>& mask)
  {
    drawer.setMask(mask);
  }

  void LayerDrawerHolder::clearMask(Drawer& drawer)
  {
    drawer.clearMask();
  }

  LayerPreviewDrawerHolder::LayerPreviewDrawerHolder() :
    m_previewDrawer(nullptr)
  {
  }

  Drawer& LayerPreviewDrawerHolder::getPreviewDrawer()
  {
    PIXPAINT_ASSERT(m_previewDrawer.get(), "Preview Drawer was not initialized");
    return *m_previewDrawer;
  }
}
