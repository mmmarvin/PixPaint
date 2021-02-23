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
#ifndef DRAWERHOLDER_H
#define DRAWERHOLDER_H

#include <memory>
#include "drawer.h"

namespace pixpaint
{
  class LayerDrawerHolder
  {
  public:
    LayerDrawerHolder();

    template<class DrawerT> void createDrawers();

    Drawer& getLayerDrawer();

  protected:
    static void setTargetLayer(Drawer& drawer, MaskablePixelData* layer);
    static void setMask(Drawer& drawer, const std::shared_ptr<Mask>& mask);
    static void clearMask(Drawer& drawer);

  private:
    std::unique_ptr<Drawer> m_layerDrawer;
  };

  class LayerPreviewDrawerHolder : public LayerDrawerHolder
  {
  public:
    LayerPreviewDrawerHolder();

    template<class DrawerT> void createDrawers();

    Drawer& getPreviewDrawer();

  protected:
    using LayerDrawerHolder::setTargetLayer;
    using LayerDrawerHolder::setMask;
    using LayerDrawerHolder::clearMask;

  private:
    std::unique_ptr<Drawer> m_previewDrawer;
  };
}

#include "drawerholder.inl"

#endif // DRAWERHOLDER_H
