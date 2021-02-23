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
#include "addlayeraction.h"

#include "../image/image.h"
#include "../image/maskablepixeldata.h"
#include "../manager/imagemanager.h"
#include "../assert.h"

namespace pixpaint
{
  AddLayerAction::AddLayerAction(size_t layerIndex) :
    HistoryAction("Add Layer"),
    m_index(layerIndex)
  {
  }

  AddLayerAction::EActionResult AddLayerAction::undo()
  {
    auto& image = getImageManager().getImage();
    auto& l = image.getLayer(m_index);

    m_layer = std::make_unique<MaskablePixelData>(std::move(l));
    image.removeLayer(m_index);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_LAYER_LIST);
  }

  AddLayerAction::EActionResult AddLayerAction::redo()
  {
    PIXPAINT_ASSERT(m_layer.get(), "Layer was not set");

    auto& image = getImageManager().getImage();

    image.addLayer(m_index, Color::TRANSPARENT);
    image.getLayer(m_index) = std::move(*m_layer.get());
    image.setCurrentLayerIndex(m_index);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_LAYER_LIST);
  }
}
