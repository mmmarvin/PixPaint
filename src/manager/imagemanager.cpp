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
#include "imagemanager.h"

#include "../env/imageenvironment.h"
#include "../utility/qt_utility.h"
#include "../window/imageeditorview.h"
#include "../assert.h"
#include "../document.h"

namespace pixpaint
{
  void ImageManager::refresh()
  {
    PIXPAINT_ASSERT(getImageEnvironment().isViewSet(), "View is not set");
    getImageEnvironment().getView().repaint();
  }

  void ImageManager::refresh(const IntRect& rect)
  {
    PIXPAINT_ASSERT(getImageEnvironment().isViewSet(), "View is not set");
    getImageEnvironment().getView().repaint(qt_utils::convertToQTRect(rect));
  }

  Image& ImageManager::getImage() noexcept
  {
    PIXPAINT_ASSERT(getImageEnvironment().isViewSet(), "View is not set!");
    return getImageEnvironment().getView().getImage();
  }

  const Image& ImageManager::getImage() const noexcept
  {
    PIXPAINT_ASSERT(getImageEnvironment().isViewSet(), "View is not set!");
    return getImageEnvironment().getView().getImage();
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ImageManager)
}
