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
#ifndef IMAGEENVIRONMENT_H
#define IMAGEENVIRONMENT_H

#include <cinttypes>
#include <cstddef>
#include "../pixpaint_macro.h"

namespace pixpaint
{
  class Document;
  class ImageEditorView;
  class SelectionWidget;
  class ZoomableScrollArea;

  class ImageEnvironment
  {
    ImageEnvironment();

  public:
    void setView(ImageEditorView* view) noexcept;
    void setScrollArea(ZoomableScrollArea* scrollArea) noexcept;

    bool isViewSet() const noexcept;
    bool isScrollAreaSet() const noexcept;

    ImageEditorView& getView() noexcept;
    const ImageEditorView& getView() const noexcept;
    ZoomableScrollArea& getScrollArea() noexcept;
    const ZoomableScrollArea& getScrollArea() const noexcept;
    SelectionWidget& getSelection() noexcept;
    const SelectionWidget& getSelection() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(ImageEnvironment)

    ImageEditorView*    m_view;
    ZoomableScrollArea* m_scrollArea;
  };

  PIXPAINT_SINGLETON_FUNC(ImageEnvironment)
}

#endif // IMAGEENVIRONMENT_H
