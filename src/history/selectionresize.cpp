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
#include "selectionresize.h"

#include "../manager/selectionmanager.h"

namespace pixpaint
{
  SelectionResize::SelectionResize(const IntRect& rect,
                                   const MaskablePixelData& layer,
                                   const PixelData& mask) :
    m_rect(rect),
    m_layer(std::make_unique<MaskablePixelData>(layer)),
    m_mask(std::make_unique<PixelData>(mask))
  {

  }

  SelectionResize::SelectionResize(const IntRect& rect, const MaskablePixelData& layer) :
    m_rect(rect),
    m_layer(std::make_unique<MaskablePixelData>(layer))
  {
  }

  SelectionResize::EActionResult SelectionResize::undo()
  {
    auto& selection_manager = getSelectionManager();
    auto temp_rect = selection_manager.getSelectionRect();
    auto temp_layer = selection_manager.getSelectionLayer();
    std::unique_ptr<PixelData> temp_mask;
    if(selection_manager.layerMaskExists()) {
      temp_mask = std::make_unique<PixelData>(selection_manager.getSelectionLayerMask());
    }

    selection_manager.setSelectionRect(m_rect, true, false);
    if(m_mask) {
      selection_manager.setSelectionLayer(*m_layer.get(), *m_mask.get());
    } else {
      selection_manager.setSelectionLayer(*m_layer.get());
    }

    m_rect = temp_rect;
    m_layer = std::make_unique<MaskablePixelData>(temp_layer);
    if(temp_mask) {
      m_mask = std::move(temp_mask);
    } else {
      m_mask.reset();
    }

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  SelectionResize::EActionResult SelectionResize::redo()
  {
    return undo();
  }
}
