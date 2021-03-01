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
#include "zoomablescrollarea.h"

#include <QPainter>
#include <QWheelEvent>
#include "../debug_log.h"
#include "../manager/dummyselectionmanager.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textselectionmanager.h"
#include "dummyselectionwidget.h"
#include "imageeditorview.h"
#include "horizontalresizehandle.h"
#include "hvresizehandle.h"
#include "selectionwidget.h"
#include "verticalresizehandle.h"

namespace pixpaint
{
  ZoomableScrollArea::ZoomableScrollArea(QWidget* parent) :
    CustomCursorWidget<QScrollArea>(parent),
    m_selectionWidget(nullptr)
  {
  }

  void ZoomableScrollArea::setWidget(ImageEditorView* view)
  {
    m_view = view;
    QScrollArea::setWidget(view);
  }

  void ZoomableScrollArea::updateSelectionWidget()
  {
    if(getImageEnvironment().isViewSet()) {
      if(getSelectionManager().selectionExists() || getTextSelectionManager().selectionExists()) {
        m_selectionWidget->updateSelection();
        if(!m_selectionWidget->isVisible()) {
          m_selectionWidget->setVisible(true);
          m_selectionWidget->setMode(SelectionWidget::ESelectionMode::NORMAL);
        }
      } else {
        m_selectionWidget->setVisible(false);
      }
    } else {
      m_selectionWidget->setVisible(false);
    }
  }

  void ZoomableScrollArea::updateDummySelectionWidget()
  {
    if(getDummySelectionManager().selectionExists()) {
      m_dummySelectionWidget->updateSelection();
      if(!m_dummySelectionWidget->isVisible()) {
        m_dummySelectionWidget->setVisible(true);
      }
    } else {
      m_dummySelectionWidget->setVisible(false);
    }
  }

  void ZoomableScrollArea::updateResizeHandles()
  {
    m_vertResizeHandle->updateResizeHandle();
    m_horResizeHandle->updateResizeHandle();
    m_hvResizeHandle->updateResizeHandle();
  }

  void ZoomableScrollArea::keyPressEvent(QKeyEvent* event)
  {
    auto key = event->key();
    auto& selection_manager = getSelectionManager();

    if(getTextSelectionManager().selectionExists()) {
      event->ignore();
    } else if(selection_manager.selectionExists()) {
      switch(key) {
      case Qt::Key_Left:
      {
        auto selectionRect = selection_manager.getSelectionRect();
        selection_manager.moveSelectionRect(selectionRect.x - 1,
                                           selectionRect.y,
                                           true);
        m_selectionWidget->updateSelection();
        event->accept();
      }
        break;
      case Qt::Key_Right:
      {
        auto selectionRect = selection_manager.getSelectionRect();
        selection_manager.moveSelectionRect(selectionRect.x + 1,
                                           selectionRect.y,
                                           true);
        m_selectionWidget->updateSelection();
        event->accept();
      }
        break;
      case Qt::Key_Up:
      {
        auto selectionRect = selection_manager.getSelectionRect();
        selection_manager.moveSelectionRect(selectionRect.x,
                                           selectionRect.y - 1,
                                           true);
        m_selectionWidget->updateSelection();
        event->accept();
      }
        break;
      case Qt::Key_Down:
      {
        auto selectionRect = selection_manager.getSelectionRect();
        selection_manager.moveSelectionRect(selectionRect.x,
                                           selectionRect.y + 1,
                                           true);
        m_selectionWidget->updateSelection();
        event->accept();
      }
        break;
      default:
        event->ignore();
        break;
      }
    } else {
      if(key != Qt::Key_Left && key != Qt::Key_Right && key != Qt::Key_Up && key != Qt::Key_Down) {
        event->ignore();
      } else {
        QScrollArea::keyPressEvent(event);
      }
    }
  }

  void ZoomableScrollArea::mousePressEvent(QMouseEvent* event)
  {
    m_view->mousePressEvent(event);
    event->accept();
  }

  void ZoomableScrollArea::mouseMoveEvent(QMouseEvent* event)
  {
    m_view->mouseMoveEvent(event);
    event->accept();
  }

  void ZoomableScrollArea::mouseReleaseEvent(QMouseEvent* event)
  {
    m_view->mouseReleaseEvent(event);
    event->accept();
  }

  void ZoomableScrollArea::wheelEvent(QWheelEvent* event)
  {
    // FIXME: Zooming action sometimes does the opposite action for a short period of time
    // (ie. zooming-in will cause a zoom-out for 1 sec, then do the actual zooming-in action)
    auto controlDown = event->modifiers() & Qt::ControlModifier;
    if(!controlDown) {
      QScrollArea::wheelEvent(event);
    } else {
      m_selectionWidget->wheelEvent(event);
      updateResizeHandles();
    }
  }

  void ZoomableScrollArea::scrollContentsBy(int dx, int dy)
  {
    QScrollArea::scrollContentsBy(dx, dy);
    updateSelectionWidget();
    updateResizeHandles();
  }

  void ZoomableScrollArea::createSelectionWidget(MainMenu::EditMenu* rightClickMenu)
  {
    m_selectionWidget = new SelectionWidget(this, rightClickMenu->menu);
    m_selectionWidget->resize(100, 100);
    m_selectionWidget->setPosition(3, 3);
    m_selectionWidget->setVisible(false);

    m_dummySelectionWidget = new DummySelectionWidget(this);
    m_dummySelectionWidget->resize(100, 100);
    m_dummySelectionWidget->setPosition(3, 3);
    m_dummySelectionWidget->setVisible(false);
  }

  void ZoomableScrollArea::createResizeHandles()
  {
    m_vertResizeHandle = new VerticalResizeHandle(this, m_view);
    m_horResizeHandle = new HorizontalResizeHandle(this, m_view);
    m_hvResizeHandle = new HVResizeHandle(this, m_view);
  }
}
