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
#include "showlayeraction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"

namespace pixpaint
{
  ShowLayerAction::ShowLayerAction(size_t layerIndex) :
    HistoryAction("Show Layer"),
    m_index(layerIndex)
  {
  }

  ShowLayerAction::EActionResult ShowLayerAction::undo()
  {
    getImageManager().getImage().setVisible(m_index, false);
    return this->toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_LAYER_LIST);
  }

  ShowLayerAction::EActionResult ShowLayerAction::redo()
  {
    getImageManager().getImage().setVisible(m_index, true);
    return this->toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_REFRESH_LAYER_LIST);
  }
}
