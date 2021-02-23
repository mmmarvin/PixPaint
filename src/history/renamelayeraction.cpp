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
#include "renamelayeraction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"

namespace pixpaint
{
  RenameLayerAction::RenameLayerAction(std::string from, std::string to) :
    HistoryAction("Rename Layer"),
    m_from(std::move(from)),
    m_to(std::move(to))
  {
  }

  RenameLayerAction::EActionResult RenameLayerAction::undo()
  {
    auto& image = getImageManager().getImage();
    auto layer_index = image.getCurrentLayerIndex();
    image.renameLayer(layer_index, m_from);

    return EActionResult::EAR_REFRESH_LAYER_LIST;
  }

  RenameLayerAction::EActionResult RenameLayerAction::redo()
  {
    auto& image = getImageManager().getImage();
    auto layer_index = image.getCurrentLayerIndex();
    image.renameLayer(layer_index, m_to);

    return EActionResult::EAR_REFRESH_LAYER_LIST;
  }
}
