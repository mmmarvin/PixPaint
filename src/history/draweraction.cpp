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
#include "draweraction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../window/imageeditorview.h"
#include "../debug_log.h"

namespace pixpaint
{
  DrawerAction::DrawerAction(std::string name,
                             std::vector<std::unique_ptr<Drawer::DrawerCommand>> flushedRects) :
    HistoryAction(std::move(name)),
    m_flushedRects(std::move(flushedRects))
  {
  }

  HistoryAction::EActionResult DrawerAction::undo()
  {
    auto& selection_manager = getSelectionManager();
    auto& current_layer = selection_manager.layerExists() ? selection_manager.getSelectionLayer() :
                                                            getImageManager().getImage().getCurrentLayer();
    for(auto it = m_flushedRects.rbegin(), it_end = m_flushedRects.rend(); it != it_end; ++it) {
      (*it)->undo(current_layer);
    }

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  HistoryAction::EActionResult DrawerAction::redo()
  {
    auto& selection_manager = getSelectionManager();
    auto& current_layer = selection_manager.layerExists() ? selection_manager.getSelectionLayer() :
                                                            getImageManager().getImage().getCurrentLayer();
    for(auto& c : m_flushedRects) {
      c->redo(current_layer);
    }

    return EActionResult::EAR_REFRESH_IMAGE;
  }
}
