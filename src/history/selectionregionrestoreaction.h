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
#ifndef SELECTIONREGIONRESTOREACTION_H
#define SELECTIONREGIONRESTOREACTION_H

#include <memory>
#include "../image/maskablepixeldata.h"
#include "../image/rect.h"
#include "historyaction.h"

namespace pixpaint
{
  class SelectionRegionRestoreAction : public HistoryAction
  {
  public:
    SelectionRegionRestoreAction(std::string effectName, const MaskablePixelData& layer);

    EActionResult undo() override;
    EActionResult redo() override;

  private:
    std::unique_ptr<MaskablePixelData>  m_layer;
  };
}

#endif // SELECTIONREGIONRESTOREACTION_H
