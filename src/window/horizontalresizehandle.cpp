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
#include "horizontalresizehandle.h"

#include <QMouseEvent>
#include <QPainter>
#include "../history/animationresizeaction.h"
#include "../image/image.h"
#include "../manager/documentmanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../debug_log.h"
#include "../document.h"
#include "imageeditorview.h"
#include "hvresizehandle.h"
#include "verticalresizehandle.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  HorizontalResizeHandle::HorizontalResizeHandle(ZoomableScrollArea* parent,
                                                 ImageEditorView* view) :
    QWidget(parent),
    m_parent(parent),
    m_view(view)
  {
    this->setCursor(Qt::SizeVerCursor);
    updateResizeHandle();
  }

  void HorizontalResizeHandle::updateResizeHandle()
  {
    auto pixelSize = m_view->getPixelSize();
    auto width = m_view->getImage().getWidth();
    auto height = m_view->getImage().getHeight();

    this->setGeometry(m_view->geometry().x() + 2,
                      m_view->geometry().y() + ((static_cast<double>(height) * pixelSize) + 2),
                      (static_cast<double>(width) * pixelSize) + 1,
                      10);
  }

  void HorizontalResizeHandle::mousePressEvent(QMouseEvent* event)
  {
    auto x = event->globalPos().x();
    auto y = event->globalPos().y();

    m_resizeStartDiff = Point(x, y) - Point(this->geometry().x(), this->geometry().y());
    m_resizeStart = true;
    event->accept();
  }

  void HorizontalResizeHandle::mouseMoveEvent(QMouseEvent* event)
  {
    if(m_resizeStart) {
      auto y = event->globalPos().y();
      auto geometry = this->geometry();
      auto newY = y - m_resizeStartDiff.y + 3;

      if(newY > static_cast<position_t>(1.0 * m_view->getPixelSize())) {
        this->setGeometry(geometry.x(),
                          newY,
                          geometry.width(),
                          geometry.height());

        auto vertGeometry = m_parent->m_vertResizeHandle->geometry();
        m_parent->m_vertResizeHandle->setGeometry(vertGeometry.x(),
                                                  vertGeometry.y(),
                                                  vertGeometry.width(),
                                                  newY);

        auto hvGeometry = m_parent->m_hvResizeHandle->geometry();
        m_parent->m_hvResizeHandle->setGeometry(hvGeometry.x(),
                                                newY,
                                                hvGeometry.width(),
                                                hvGeometry.height());
      }
      event->accept();
    }
  }

  void HorizontalResizeHandle::mouseReleaseEvent(QMouseEvent* event)
  {
    m_resizeStart = false;
    auto view_geometry = m_view->geometry();
    auto geometry = this->geometry();

    auto old_height = m_view->getImage().getHeight();
    position_t width = m_view->getImage().getWidth();
    position_t height = static_cast<double>(geometry.y() - view_geometry.y()) / m_view->getPixelSize();
    if(old_height != height) {
      auto& animation = getDocumentManager().getDocument().getAnimation();

      emitHistoryAction(AnimationResizeAction(animation));

      for(std::size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
        animation.getFrame(i).resize(width, height);
      }

      auto& preview_manager = getPreviewManager();

      m_view->updateSize();
      m_view->updateDrawers();
      getImageManager().refresh();
      preview_manager.refreshAll();
      preview_manager.refreshResizeAll();
    }
    m_parent->updateResizeHandles();
    event->accept();
  }

  void HorizontalResizeHandle::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);
    painter.setPen(QPen(QColor(100, 100, 100, 255)));
    painter.setBrush(QBrush(QColor(100, 100, 100, 255)));
    painter.drawRect(0, 0, this->geometry().width() - 1, this->geometry().height() - 1);
  }
}
