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
#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <cinttypes>
#include "../image/rect.h"
#include "../pattern/singleton.h"

namespace pixpaint
{
  class Image;

  class ImageManager
  {
    ImageManager() {}

  public:
    void refresh();
    void refresh(const IntRect& rect);

    Image& getImage() noexcept;
    const Image& getImage() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(ImageManager)
  };

  PIXPAINT_SINGLETON_FUNC(ImageManager)
}

#endif // IMAGEMANAGER_H
