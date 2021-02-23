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
#include "selectionwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include "../env/imageenvironment.h"
#include "../history/selectionmoveaction.h"
#include "../history/selectionresize.h"
#include "../image/image.h"
#include "../tool/painttoolbase.h"
#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"
#include "../assert.h"
#include "../debug_log.h"
#include "../manager/colormanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"
#include "imageeditorview.h"
#include "mainmenu.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  SelectionWidget::SelectionWidget(QWidget* parent, QMenu* rightClickMenu) :
    CustomCursorWidget<DummySelectionWidget>(parent),
    m_rightClickMenu(rightClickMenu),
    m_moveStart(false),
    m_selectionManagerTLSelect(false),
    m_selectionManagerTRSelect(false),
    m_selectionManagerBLSelect(false),
    m_selectionManagerBRSelect(false)
  {
  }

  void SelectionWidget::updateSelection()
  {
    const auto& selection_manager = getSelectionManager();
    const auto& text_selection_manager = getTextSelectionManager();
    if(selection_manager.selectionExists()) {
      updateSelection(selection_manager.getSelectionRect());
    } else if(text_selection_manager.selectionExists()) {
      updateSelection(text_selection_manager.getSelectionRect());
    }
  }

  void SelectionWidget::setMode(ESelectionMode mode)
  {
    m_mode = mode;
  }

  Point SelectionWidget::getPosition() const
  {
    auto g = this->geometry();
    return Point(g.x(), g.y());
  }

  SelectionWidget::ESelectionMode SelectionWidget::getMode() const noexcept
  {
    return m_mode;
  }

  void SelectionWidget::mousePressEvent(QMouseEvent* event)
  {
    auto mx = event->pos().x();
    auto my = event->pos().y();
    auto g_mx = event->globalPos().x();
    auto g_my = event->globalPos().y();
    auto t = getMousePositionInView(g_mx, g_my);
    auto px = t.x;
    auto py = t.y;

    if(event->button() == Qt::MouseButton::LeftButton) {
      if(m_mode == ESelectionMode::RESIZE && isResizeHandleSelected(mx, my)) {
        auto rect = getSelectionRect();
        m_selectionManagerSelectStart = Point(px, py);
        m_selectionManagerSelectDimension = Point(rect.width, rect.height);
        m_temporarySelectionRect = rect;
      } else {
        if(m_mode == ESelectionMode::MOVE) {
          m_moveStart = true;
          m_moveStartPoint = position(getSelectionManager().getSelectionRect());
          m_movePoint = Point(px, py);
        } else {
          auto& img_env = getImageEnvironment();
          auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
          auto view_geometry = img_env.getView().frameGeometry();
          QMouseEvent adjustedEvent(event->type(),
                                    QPointF(event->pos().x() + this->geometry().x() - adj_offset - view_geometry.x(),
                                            event->pos().y() + this->geometry().y() - adj_offset - view_geometry.y()),
                                    event->windowPos(),
                                    event->screenPos(),
                                    event->button(),
                                    event->buttons(),
                                    event->modifiers(),
                                    event->source());
          img_env.getView().mousePressEvent(&adjustedEvent);
        }
      }
    } else if(event->button() == Qt::MouseButton::RightButton) {
      const auto& currentTool = getPaintToolManager().getCurrentTool();
      if(!currentTool.hasRightClick()) {
        m_rightClickMenu->exec(QPoint(event->globalPos().x(), event->globalPos().y()));
      } else {
        auto& img_env = getImageEnvironment();
        auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
        auto view_geometry = img_env.getView().frameGeometry();
        QMouseEvent adjustedEvent(event->type(),
                                  QPointF(event->pos().x() + this->geometry().x() - adj_offset - view_geometry.x(),
                                          event->pos().y() + this->geometry().y() - adj_offset - view_geometry.y()),
                                  event->windowPos(),
                                  event->screenPos(),
                                  event->button(),
                                  event->buttons(),
                                  event->modifiers(),
                                  event->source());
        img_env.getView().mousePressEvent(&adjustedEvent);
      }
    }

    event->accept();
  }

  void SelectionWidget::mouseMoveEvent(QMouseEvent* event)
  {
    auto mx = event->pos().x();
    auto my = event->pos().y();
    auto g_mx = event->globalPos().x();
    auto g_my = event->globalPos().y();
    auto t = getMousePositionInView(g_mx, g_my);
    auto px = t.x;
    auto py = t.y;

    if(m_mode == ESelectionMode::RESIZE) {
      auto selection_rect = qt_utils::convertToRect(this->geometry());
      selection_rect.x = SELECTION_HANDLE_WIDTH;
      selection_rect.y = SELECTION_HANDLE_HEIGHT;
      selection_rect.width -= SELECTION_HANDLE_WIDTH * 2;
      selection_rect.height -= SELECTION_HANDLE_HEIGHT * 2;
      auto mouse_position = Point(mx, my);
      if(isTopLeftResizeHandleSelected(selection_rect, mouse_position) || m_selectionManagerTLSelect) {
        QWidget::setCursor(Qt::SizeFDiagCursor);
      } else if(isTopRightResizeHandleSelected(selection_rect, mouse_position) || m_selectionManagerTRSelect) {
        QWidget::setCursor(Qt::SizeBDiagCursor);
      } else if(isBottomLeftResizeHandleSelected(selection_rect, mouse_position) || m_selectionManagerBLSelect) {
        QWidget::setCursor(Qt::SizeBDiagCursor);
      } else if(isBottomRightResizeHandleSelected(selection_rect, mouse_position) || m_selectionManagerBRSelect) {
        QWidget::setCursor(Qt::SizeFDiagCursor);
      } else {
        QWidget::unsetCursor();
      }
    } else if(m_mode == ESelectionMode::MOVE) {
      QWidget::setCursor(Qt::OpenHandCursor);
    }

    if(m_selectionManagerBLSelect || m_selectionManagerBRSelect ||
       m_selectionManagerTLSelect || m_selectionManagerTRSelect) {
      auto diff = Point(px, py) - m_selectionManagerSelectStart;
      auto rect = getSelectionRect();

      if(m_selectionManagerTLSelect) {
        rect.x = px + 1;
        rect.y = py;
        rect.width = (m_selectionManagerSelectDimension.x - diff.x + 1);
        rect.height = (m_selectionManagerSelectDimension.y - diff.y + 1);
        m_temporarySelectionRect = rect;
        this->updateSelectionUsingTemp();
      } else if(m_selectionManagerTRSelect) {
        rect.y = py;
        rect.width = (m_selectionManagerSelectDimension.x + diff.x + 1);
        rect.height = (m_selectionManagerSelectDimension.y - diff.y + 1);
        m_temporarySelectionRect = rect;
        this->updateSelectionUsingTemp();
      } else if(m_selectionManagerBLSelect) {
        rect.x = px + 1;
        rect.width = (m_selectionManagerSelectDimension.x - diff.x);
        rect.height = (m_selectionManagerSelectDimension.y + diff.y);
        m_temporarySelectionRect = rect;
        this->updateSelectionUsingTemp();
      } else if(m_selectionManagerBRSelect) {
        rect.width = (m_selectionManagerSelectDimension.x + diff.x + 1);
        rect.height = (m_selectionManagerSelectDimension.y + diff.y + 1);
        m_temporarySelectionRect = rect;
        this->updateSelectionUsingTemp();
      }
    } else {
      if(m_mode == ESelectionMode::MOVE && m_moveStart) {
        auto diff = m_movePoint - position(getSelectionRect());
        m_movePoint = Point(px, py);
        auto& selection_manager = getSelectionManager();
        auto& text_selection_manager = getTextSelectionManager();
        if(selection_manager.selectionExists()) {
          selection_manager.moveSelectionRect(m_movePoint.x - diff.x,
                                              m_movePoint.y - diff.y,
                                              false,
                                              false);
          this->updateSelection();
        } else if(text_selection_manager.selectionExists()) {
          text_selection_manager.moveSelectionRect(m_movePoint.x - diff.x,
                                                   m_movePoint.y - diff.y,
                                                   false,
                                                   false);
          this->updateSelection();
        }
      } else {
        auto& img_env = getImageEnvironment();
        auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
        auto view_geometry = img_env.getView().frameGeometry();
        QMouseEvent adjustedEvent(event->type(),
                                  QPointF(event->pos().x() + this->geometry().x() - adj_offset - view_geometry.x(),
                                          event->pos().y() + this->geometry().y() - adj_offset - view_geometry.y()),
                                  event->windowPos(),
                                  event->screenPos(),
                                  event->button(),
                                  event->buttons(),
                                  event->modifiers(),
                                  event->source());
        img_env.getView().mouseMoveEvent(&adjustedEvent);
      }
    }

    event->accept();
  }

  void SelectionWidget::mouseReleaseEvent(QMouseEvent* event)
  {
    if(m_selectionManagerBLSelect || m_selectionManagerBRSelect ||
       m_selectionManagerTLSelect || m_selectionManagerTRSelect) {
      m_selectionManagerBLSelect = m_selectionManagerBRSelect = m_selectionManagerTLSelect = m_selectionManagerTRSelect = false;

      auto& selection_manager = getSelectionManager();
      auto& text_selection_manager = getTextSelectionManager();
      auto oldRect = getSelectionRect();
      if(oldRect.x != m_temporarySelectionRect.x || oldRect.y != m_temporarySelectionRect.y ||
         oldRect.width != m_temporarySelectionRect.width || oldRect.height != m_temporarySelectionRect.height) {
        if(selection_manager.selectionExists()) {
          if(selection_manager.layerMaskExists()) {
            emitHistoryAction(SelectionResize(selection_manager.getSelectionRect(),
                                              selection_manager.getSelectionLayer(),
                                              selection_manager.getSelectionLayerMask()));
          } else {
            emitHistoryAction(SelectionResize(selection_manager.getSelectionRect(), selection_manager.getSelectionLayer()));
          }

          selection_manager.moveSelectionRect(m_temporarySelectionRect.x, m_temporarySelectionRect.y, false, false);
          selection_manager.resizeSelectionRect(m_temporarySelectionRect.width, m_temporarySelectionRect.height, Color::TRANSPARENT, false);

          this->updateSelection();
        } else {
          text_selection_manager.moveSelectionRect(m_temporarySelectionRect.x, m_temporarySelectionRect.y, false, false);
          text_selection_manager.resizeSelectionRect(m_temporarySelectionRect.width, m_temporarySelectionRect.height, Color::TRANSPARENT, false);

          this->updateSelection();
        }

        auto& current_paint_tool = getPaintToolManager().getCurrentTool();
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        auto& view = image_env.getView();
        if(getTextManager().tryUpdateText()) {
          auto pixelSize = view.getPixelSize();
          auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(getTextSelectionManager().getSelectionRect()) * pixelSize);
          view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
        } else if(current_paint_tool.onResize(view.getPreviewLayer(),
                                              image_manager.getImage().getCurrentLayer())) {
          auto pixelSize = view.getPixelSize();
          auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(current_paint_tool.getDrawRect()) * pixelSize);
          view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
        }

        auto& img_env = getImageEnvironment();
        auto pixel_size = img_env.getView().getPixelSize();
        img_env.getView().repaint(QRect(general_utils::min(oldRect.x, m_temporarySelectionRect.x) * pixel_size,
                                       general_utils::min(oldRect.y, m_temporarySelectionRect.y) * pixel_size,
                                       general_utils::max(oldRect.width, m_temporarySelectionRect.width) * pixel_size,
                                       general_utils::max(oldRect.height, m_temporarySelectionRect.height) * pixel_size));
      }
    } else {
      if(m_moveStart) {
        emitHistoryAction(SelectionMoveAction(m_moveStartPoint, position(getSelectionManager().getSelectionRect())));
        m_moveStart = false;
      } else {
        auto& img_env = getImageEnvironment();
        auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
        auto view_geometry = img_env.getView().frameGeometry();
        QMouseEvent adjustedEvent(event->type(),
                                  QPointF(event->pos().x() + this->geometry().x() - adj_offset - view_geometry.x(),
                                          event->pos().y() + this->geometry().y() - adj_offset - view_geometry.y()),
                                  event->windowPos(),
                                  event->screenPos(),
                                  event->button(),
                                  event->buttons(),
                                  event->modifiers(),
                                  event->source());
        img_env.getView().mouseReleaseEvent(&adjustedEvent);
      }
    }

    event->accept();
  }

  void SelectionWidget::wheelEvent(QWheelEvent* event)
  {
    if(event->modifiers() & Qt::ControlModifier) {
      getImageEnvironment().getView().wheelEvent(event);
    } else {
      getImageEnvironment().getScrollArea().wheelEvent(event);
    }
  }

  void SelectionWidget::paintEvent(QPaintEvent* event)
  {
    DummySelectionWidget::paintEvent(event);

    QPainter painter(this);

    auto width = this->geometry().width() - ((SELECTION_HANDLE_WIDTH * 2) + 1);
    auto height = this->geometry().height() - ((SELECTION_HANDLE_HEIGHT * 2) + 1);

    if(m_mode == ESelectionMode::RESIZE) {
      painter.setPen(QPen(QColor(0, 0, 0, 255), 1, Qt::SolidLine));
      painter.setBrush(QBrush(QColor(200, 200, 255, 255)));
      painter.drawRect(0, 0, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT);
      painter.drawRect(width + SELECTION_HANDLE_WIDTH, 0, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT);
      painter.drawRect(0, height + SELECTION_HANDLE_HEIGHT, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT);
      painter.drawRect(width + SELECTION_HANDLE_WIDTH, height + SELECTION_HANDLE_HEIGHT, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT);
    }
  }

  void SelectionWidget::updateSelection(IntRect selectionRect)
  {
    auto& img_env = getImageEnvironment();
    auto pixel_size = img_env.getView().getPixelSize();
    selectionRect.x = double(selectionRect.x) * pixel_size;
    selectionRect.y = double(selectionRect.y) * pixel_size;
    selectionRect.width = double(selectionRect.width) * pixel_size;
    selectionRect.height = double(selectionRect.height) * pixel_size;
    this->resize(selectionRect.width + (SELECTION_HANDLE_WIDTH * 2),
                 selectionRect.height + (SELECTION_HANDLE_HEIGHT * 2));

    auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
    this->setPosition(img_env.getView().frameGeometry().x() + selectionRect.x + adj_offset,
                      img_env.getView().frameGeometry().y() + selectionRect.y + adj_offset);
  }

  void SelectionWidget::updateSelectionUsingTemp()
  {
    updateSelection(m_temporarySelectionRect);
  }

  bool SelectionWidget::isResizeHandleSelected(int mx, int my)
  {
    if(this->isVisible()) {
      auto mouse_point = Point(mx, my);
      auto selection_rect = qt_utils::convertToRect(this->geometry());
      selection_rect.x = SELECTION_HANDLE_WIDTH;
      selection_rect.y = SELECTION_HANDLE_HEIGHT;
      selection_rect.width -= SELECTION_HANDLE_WIDTH * 2;
      selection_rect.height -= SELECTION_HANDLE_HEIGHT * 2;

      if(isTopLeftResizeHandleSelected(selection_rect, mouse_point)) {
        m_selectionManagerTLSelect = true;
        return true;
      } else if(isTopRightResizeHandleSelected(selection_rect, mouse_point)) {
        m_selectionManagerTRSelect = true;
        return true;
      } else if(isBottomLeftResizeHandleSelected(selection_rect, mouse_point)) {
        m_selectionManagerBLSelect = true;
        return true;
      } else if(isBottomRightResizeHandleSelected(selection_rect, mouse_point)) {
        m_selectionManagerBRSelect = true;
        return true;
      }
    }

    return false;
  }

  bool SelectionWidget::isTopLeftResizeHandleSelected(const IntRect& geometryRect,
                                                      const Point& mousePosition) const
  {
    auto sx = geometryRect.x - SELECTION_HANDLE_WIDTH;
    auto sy = geometryRect.y - SELECTION_HANDLE_HEIGHT;

    return contains(IntRect(sx, sy, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT), mousePosition);
  }

  bool SelectionWidget::isTopRightResizeHandleSelected(const IntRect& geometryRect,
                                                       const Point& mousePosition) const
  {
    auto sy = geometryRect.y - SELECTION_HANDLE_HEIGHT;
    auto sw = geometryRect.x + geometryRect.width;

    return contains(IntRect(sw, sy, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT), mousePosition);
  }

  bool SelectionWidget::isBottomLeftResizeHandleSelected(const IntRect& geometryRect,
                                                         const Point& mousePosition) const
  {
    auto sx = geometryRect.x - SELECTION_HANDLE_WIDTH;
    auto sh = geometryRect.y + geometryRect.height;

    return contains(IntRect(sx, sh, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT), mousePosition);
  }

  bool SelectionWidget::isBottomRightResizeHandleSelected(const IntRect& geometryRect, const Point& mousePosition) const
  {
    auto sw = geometryRect.x + geometryRect.width;
    auto sh = geometryRect.y + geometryRect.height;

    return contains(IntRect(sw, sh, SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT), mousePosition);
  }

  Point SelectionWidget::getMousePositionInView(position_t mx, position_t my) const
  {
    auto& img_env = getImageEnvironment();
    auto pixel_size = img_env.getView().getPixelSize();
    auto mapped_pos = img_env.getView().mapFromGlobal(QPoint(mx, my));
    return Point(mapped_pos.x() / pixel_size, mapped_pos.y() / pixel_size);
  }

  IntRect SelectionWidget::getSelectionRect() const
  {
    auto& selection_manager = getSelectionManager();
    if(selection_manager.selectionExists()) {
      return selection_manager.getSelectionRect();
    }

    return getTextSelectionManager().getSelectionRect();
  }
}
