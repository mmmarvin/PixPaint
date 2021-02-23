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
#include "customcursorwidget.h"

namespace pixpaint
{
  template<class ParentWidgetT>
  template<class... Args>
  CustomCursorWidget<ParentWidgetT>::CustomCursorWidget(Args&&... args) :
    ParentWidgetT(std::forward<Args>(args)...)
  {
    static_assert(std::is_base_of<QWidget, ParentWidgetT>(), "ParentWidgetT must inherit from QWidget");
    this->setMouseTracking(true);
  }

  template<class ParentWidgetT>
  void CustomCursorWidget<ParentWidgetT>::setCursor(const Cursor& cursor)
  {
    m_cursor = cursor;
    setCursorImpl(cursor);
  }

  template<class ParentWidgetT>
  Cursor& CustomCursorWidget<ParentWidgetT>::getCursor()
  {
    return m_cursor;
  }

  template<class ParentWidgetT>
  const Cursor& CustomCursorWidget<ParentWidgetT>::getCursor() const
  {
    return m_cursor;
  }

  template<class ParentWidgetT>
  void CustomCursorWidget<ParentWidgetT>::setCursorImpl(const Cursor& cursor)
  {
    const auto& c = cursor.getCursor();
    if(c.shape() == Qt::BitmapCursor && cursor.isAutoScale()) {
      auto cursorPixmap = c.pixmap();
      customcursorwidget_detail::scaleCursorPixmap(cursorPixmap);

      QWidget::setCursor(cursorPixmap);
    } else {
      QWidget::setCursor(cursor.getCursor());
    }
  }
}
