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
#include "selectioncutaction.h"

#include "../manager/selectionmanager.h"

namespace pixpaint
{
  SelectionCutAction::SelectionCutAction(const IntRect& rect,
                                         const MaskablePixelData& layer,
                                         const PixelData& mask) :
    HistoryAction("Cut"),
    m_rect(rect),
    m_layer(std::make_unique<MaskablePixelData>(layer)),
    m_mask(std::make_unique<PixelData>(mask))
  {
  }

  SelectionCutAction::SelectionCutAction(const IntRect& rect,
                                         const MaskablePixelData& layer) :
    m_rect(rect),
    m_layer(std::make_unique<MaskablePixelData>(layer)),
    m_mask(nullptr)
  {
  }

  SelectionCutAction::EActionResult SelectionCutAction::undo()
  {
    auto& selection_manager = getSelectionManager();
    selection_manager.setSelectionRect(m_rect.x,
                                       m_rect.y,
                                       m_rect.width,
                                       m_rect.height,
                                       true,
                                       false);
    if(m_mask) {
      selection_manager.setSelectionLayer(*m_layer.get(), *m_mask.get());
      m_mask.reset(nullptr);
    } else {
      selection_manager.setSelectionLayer(*m_layer.get());
    }
    m_layer.reset(nullptr);

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  SelectionCutAction::EActionResult SelectionCutAction::redo()
  {
    auto& selection_manager = getSelectionManager();
    m_rect = selection_manager.getSelectionRect();
    m_layer = std::make_unique<MaskablePixelData>(selection_manager.getSelectionLayer());
    if(selection_manager.layerMaskExists()) {
      m_mask = std::make_unique<PixelData>(selection_manager.getSelectionLayerMask());
    }

    selection_manager.clear();

    return EActionResult::EAR_REFRESH_IMAGE;
  }
}
