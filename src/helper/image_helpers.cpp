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
#include "image_helpers.h"

#include <cstddef>
#include "../env/imageenvironment.h"
#include "../history/imagerestoreaction.h"
#include "../history/regionrestoreaction.h"
#include "../history/selectionregionrestoreaction.h"
#include "../image/image.h"
#include "../manager/drawermanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../window/imageeditorview.h"

namespace pixpaint
{
namespace image_helpers
{
  void applyEffect(std::string effectName,
                   Image& image,
                   std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                   std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists() &&
       selection_manager.selectionExists()) {
      auto selectionRect = selection_manager.getSelectionRect();
      auto selectionLayer = selection_manager.getSelectionLayer();

      emitHistoryAction(SelectionRegionRestoreAction(std::move(effectName), selectionLayer));

      selectionEffect(selectionRect, selectionLayer);
      selection_manager.setSelectionLayer(selectionLayer);
    } else {
      emitHistoryAction(ImageRestoreAction(std::move(effectName), getImageManager().getImage()));

      auto rect = IntRect(0, 0, image.getWidth(), image.getHeight());
      for(std::size_t i = 0, numberOfLayers = image.getLayerCount(); i < numberOfLayers; ++i) {
        layerEffect(rect, image.getLayer(i));
      }
    }

    getDrawerManager().updateDrawers();
  }

  void applyEffect(std::string effectName,
                   MaskablePixelData& layer,
                   std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                   std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists() &&
       selection_manager.selectionExists()) {
      auto selectionRect = selection_manager.getSelectionRect();
      auto selectionLayer = selection_manager.getSelectionLayer();

      emitHistoryAction(SelectionRegionRestoreAction(std::move(effectName), selectionLayer));

      selectionEffect(selectionRect, selectionLayer);
      selection_manager.setSelectionLayer(selectionLayer);
    } else {
      auto width = layer.getWidth();
      auto height = layer.getHeight();

      emitHistoryAction(RegionRestoreAction(std::move(effectName), layer));

      layerEffect(IntRect(0, 0, width, height), layer);
    }

    getDrawerManager().updateDrawers();
  }
}
}
