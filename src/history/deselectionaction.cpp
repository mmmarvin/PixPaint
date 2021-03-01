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
#include "deselectionaction.h"

namespace pixpaint
{
  DeselectionAction::DeselectionAction(const IntRect& rect,
                                       const MaskablePixelData& region,
                                       const MaskablePixelData& background) :
    SelectionAction("Deselect")
  {
    m_rect = rect;
    m_region = std::make_unique<MaskablePixelData>(region);
    m_background = std::make_unique<MaskablePixelData>(background);
  }

  DeselectionAction::DeselectionAction(const IntRect& rect,
                                       const MaskablePixelData& region,
                                       const PixelData& mask,
                                       const MaskablePixelData& background) :
    SelectionAction("Deselect")
  {
    m_rect = rect;
    m_region = std::make_unique<MaskablePixelData>(region);
    m_mask = std::make_unique<MaskablePixelData>(mask);
    m_background = std::make_unique<MaskablePixelData>(background);
  }

  HistoryAction::EActionResult DeselectionAction::undo()
  {
    return SelectionAction::redo();
  }

  HistoryAction::EActionResult DeselectionAction::redo()
  {
    return SelectionAction::undo();
  }
}
