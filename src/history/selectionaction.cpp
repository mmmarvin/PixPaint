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
#include "selectionaction.h"

#include "../env/imageenvironment.h"
#include "../helper/selection_helpers.h"
#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../window/imageeditorview.h"

namespace pixpaint
{
  SelectionAction::SelectionAction(std::string name) :
    HistoryAction(std::move(name)),
    m_region(nullptr),
    m_mask(nullptr),
    m_background(nullptr)
  {
  }

  HistoryAction::EActionResult SelectionAction::undo()
  {
    auto& selection_manager = getSelectionManager();
    auto& image_manager = getImageManager();

    m_rect = selection_manager.getSelectionRect();
    m_region = std::make_unique<MaskablePixelData>(selection_manager.getSelectionLayer());
    if(selection_manager.layerMaskExists()) {
      m_mask = std::make_unique<PixelData>(selection_manager.getSelectionLayerMask());
    }
    m_background = std::make_unique<MaskablePixelData>(image_manager.getImage().getCurrentLayer().copy(m_rect.x,
                                                                                                       m_rect.y,
                                                                                                       m_rect.width,
                                                                                                       m_rect.height));
    selection_helpers::tryFinalizeAllSelections(false);

    return EAR_REFRESH_IMAGE;
  }

  HistoryAction::EActionResult SelectionAction::redo()
  {
    auto& selection_manager = getSelectionManager();
    auto& image_manager = getImageManager();

    selection_manager.setSelectionRect(m_rect.x, m_rect.y, m_rect.width, m_rect.height, true, false);
    if(m_mask) {
      selection_manager.setSelectionLayer(*m_region.get(), *m_mask.get());
    } else {
      selection_manager.setSelectionLayer(*m_region.get());
    }
    image_manager.getImage().getCurrentLayer().combine(*m_background.get(),
                                                       m_rect.x,
                                                       m_rect.y,
                                                       true);

    m_region.reset(nullptr);
    m_mask.reset(nullptr);
    m_background.reset(nullptr);

    return EAR_REFRESH_IMAGE;
  }
}
