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
#include "selectionregionrestoreaction.h"

#include "../manager/selectionmanager.h"

namespace pixpaint
{
  SelectionRegionRestoreAction::SelectionRegionRestoreAction(std::string effectName,
                                                             const MaskablePixelData& layer) :
    HistoryAction(std::move(effectName)),
    m_layer(std::make_unique<MaskablePixelData>(layer))
  {
  }

  SelectionRegionRestoreAction::EActionResult SelectionRegionRestoreAction::undo()
  {
    auto& selection_manager = getSelectionManager();
    auto temp = selection_manager.getSelectionLayer();
    selection_manager.setSelectionLayer(*m_layer.get());
    m_layer = std::make_unique<MaskablePixelData>(std::move(temp));

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  SelectionRegionRestoreAction::EActionResult SelectionRegionRestoreAction::redo()
  {
    return undo();
  }
}
