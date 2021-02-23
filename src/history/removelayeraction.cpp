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
#include "removelayeraction.h"

#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../manager/imagemanager.h"

namespace pixpaint
{
  RemoveLayerAction::RemoveLayerAction(size_t layerIndex) :
    HistoryAction("Remove Layer"),
    m_index(layerIndex)
  {
    auto& image = getImageManager().getImage();
    m_layer = std::make_unique<MaskablePixelData>(image.getLayer(layerIndex));
    m_layerName = image.getLayerName(layerIndex);
  }

  RemoveLayerAction::EActionResult RemoveLayerAction::undo()
  {
    auto& image = getImageManager().getImage();
    image.addLayer(m_index, Color::TRANSPARENT);
    image.getLayer(m_index) = std::move(*m_layer.get());
    image.renameLayer(m_index, std::move(m_layerName));
    image.setCurrentLayerIndex(m_index);
    m_layer.reset(nullptr);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_LAYER_LIST);
  }

  RemoveLayerAction::EActionResult RemoveLayerAction::redo()
  {
    auto& image = getImageManager().getImage();
    m_layer = std::make_unique<MaskablePixelData>(image.getLayer(m_index));
    m_layerName = image.getLayerName(m_index);
    image.removeLayer(m_index);

    if(m_index > 0) {
      image.setCurrentLayerIndex(m_index - 1);
    } else {
      image.setCurrentLayerIndex(0);
    }

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_LAYER_LIST);
  }
}
