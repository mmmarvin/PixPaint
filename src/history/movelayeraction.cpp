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
#include "movelayeraction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"

namespace pixpaint
{
  MoveLayerAction::MoveLayerAction(size_t from, size_t to) :
    HistoryAction("Move Layer"),
    m_from(from),
    m_to(to)
  {
  }

  MoveLayerAction::EActionResult MoveLayerAction::undo()
  {
    auto& image = getImageManager().getImage();
    image.moveLayer(m_to, m_from);
    image.setCurrentLayerIndex(m_from);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_LAYER_LIST);
  }

  MoveLayerAction::EActionResult MoveLayerAction::redo()
  {
    auto& image = getImageManager().getImage();
    image.moveLayer(m_from, m_to);
    image.setCurrentLayerIndex(m_to);

    return toEActionResult(EActionResult::EAR_REFRESH_IMAGE | EActionResult::EAR_RECREATE_LAYER_LIST);
  }
}
