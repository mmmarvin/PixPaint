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
#include "imagerestoreaction.h"

#include "../image/image.h"
#include "../manager/imagemanager.h"

namespace pixpaint
{
  ImageRestoreAction::ImageRestoreAction(std::string effectName, const Image& image) :
    HistoryAction(std::move(effectName)),
    m_image(std::make_unique<Image>(image))
  {
  }

  ImageRestoreAction::EActionResult ImageRestoreAction::undo()
  {
    auto& image_manager = getImageManager();
    auto temp = image_manager.getImage();

    image_manager.getImage() = *m_image.get();
    m_image = std::make_unique<Image>(temp);

    return EActionResult::EAR_REFRESH_IMAGE;
  }

  ImageRestoreAction::EActionResult ImageRestoreAction::redo()
  {
    return undo();
  }
}
