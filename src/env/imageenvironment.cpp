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
#include "imageenvironment.h"

#include "../window/imageeditorview.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
  ImageEnvironment::ImageEnvironment() :
    m_view(nullptr),
    m_scrollArea(nullptr)
  {
  }

  void ImageEnvironment::setView(ImageEditorView* view) noexcept
  {
    m_view = view;
  }

  void ImageEnvironment::setScrollArea(ZoomableScrollArea* scrollArea) noexcept
  {
    m_scrollArea = scrollArea;
  }

  bool ImageEnvironment::isViewSet() const noexcept
  {
    return m_view != nullptr;
  }

  bool ImageEnvironment::isScrollAreaSet() const noexcept
  {
    return m_scrollArea != nullptr;
  }

  ImageEditorView& ImageEnvironment::getView() noexcept
  {
    return *m_view;
  }

  const ImageEditorView& ImageEnvironment::getView() const noexcept
  {
    return *m_view;
  }

  ZoomableScrollArea& ImageEnvironment::getScrollArea() noexcept
  {
    return *m_scrollArea;
  }

  const ZoomableScrollArea& ImageEnvironment::getScrollArea() const noexcept
  {
    return *m_scrollArea;
  }

  SelectionWidget& ImageEnvironment::getSelection() noexcept
  {
    return *m_scrollArea->m_selectionWidget;
  }

  const SelectionWidget& ImageEnvironment::getSelection() const noexcept
  {
    return *m_scrollArea->m_selectionWidget;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ImageEnvironment)
}
