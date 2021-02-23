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

#include "../utility/qt_utility.h"

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
    const auto cursor_type = cursor.getCursorType();
    if(cursor_type == Cursor::ECursorType::ECT_IMAGE) {
      if(cursor.isAutoScale()) {
        auto cursorPixmap = qt_utils::convertToQTPixmap(cursor.getCursor());
        customcursorwidget_detail::scaleCursorPixmap(cursorPixmap);
        QWidget::setCursor(cursorPixmap);
      } else {
        QWidget::setCursor(qt_utils::convertToQTPixmap(cursor.getCursor()));
      }
    } else {
      switch(cursor_type) {
      case Cursor::ECursorType::ECT_ARROW:
        QWidget::setCursor(QCursor(Qt::ArrowCursor));
        break;
      case Cursor::ECursorType::ECT_CROSS:
        QWidget::setCursor(QCursor(Qt::CrossCursor));
        break;
      case Cursor::ECursorType::ECT_WAIT:
        QWidget::setCursor(QCursor(Qt::WaitCursor));
        break;
      case Cursor::ECursorType::ECT_IBEAM:
        QWidget::setCursor(QCursor(Qt::IBeamCursor));
        break;
      case Cursor::ECursorType::ECT_RESIZE_VER:
        QWidget::setCursor(QCursor(Qt::SizeVerCursor));
        break;
      case Cursor::ECursorType::ECT_RESIZE_HOR:
        QWidget::setCursor(QCursor(Qt::SizeHorCursor));
        break;
      case Cursor::ECursorType::ECT_RESIZE_BDIAG:
        QWidget::setCursor(QCursor(Qt::SizeBDiagCursor));
        break;
      case Cursor::ECursorType::ECT_RESIZE_FDIAG:
        QWidget::setCursor(QCursor(Qt::SizeFDiagCursor));
        break;
      case Cursor::ECursorType::ECT_BUSY:
        QWidget::setCursor(QCursor(Qt::BusyCursor));
        break;
      case Cursor::ECursorType::ECT_OPEN_HAND:
        QWidget::setCursor(QCursor(Qt::OpenHandCursor));
        break;
      case Cursor::ECursorType::ECT_CLOSED_HAND:
        QWidget::setCursor(QCursor(Qt::ClosedHandCursor));
        break;
      case Cursor::ECursorType::ECT_DRAG_COPY:
        QWidget::setCursor(QCursor(Qt::DragCopyCursor));
        break;
      case Cursor::ECursorType::ECT_DRAG_MOVE:
        QWidget::setCursor(QCursor(Qt::DragMoveCursor));
        break;
      default:
        break;
      }
    }
  }
}
