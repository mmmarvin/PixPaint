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
#include "hvresizehandle.h"

#include <QMouseEvent>
#include <QPainter>
#include "../history/animationresizeaction.h"
#include "../image/image.h"
#include "../manager/documentmanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../utility/general_utility.h"
#include "../debug_log.h"
#include "../document.h"
#include "imageeditorview.h"
#include "horizontalresizehandle.h"
#include "verticalresizehandle.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  HVResizeHandle::HVResizeHandle(ZoomableScrollArea* parent,
                                 ImageEditorView* view) :
    QWidget(parent),
    m_parent(parent),
    m_view(view)
  {
    this->setCursor(Qt::SizeFDiagCursor);
    updateResizeHandle();
  }

  void HVResizeHandle::updateResizeHandle()
  {
    auto pixelSize = m_view->getPixelSize();
    auto width = m_view->getImage().getWidth();
    auto height = m_view->getImage().getHeight();

    this->setGeometry(m_view->geometry().x() + ((static_cast<double>(width) * pixelSize) + 2),
                      m_view->geometry().x() + ((static_cast<double>(height) * pixelSize) + 2),
                      10,
                      10);
  }

  void HVResizeHandle::mousePressEvent(QMouseEvent* event)
  {
    auto x = event->globalPos().x();
    auto y = event->globalPos().y();

    m_resizeStartDiff = Point(x, y) - Point(this->geometry().x(), this->geometry().y());
    m_resizeStart = true;
    event->accept();
  }

  void HVResizeHandle::mouseMoveEvent(QMouseEvent* event)
  {
    if(m_resizeStart) {
      auto x = event->globalPos().x();
      auto y = event->globalPos().y();

      auto geometry = this->geometry();
      auto newX = general_utils::max<position_t>(1.0 * m_view->getPixelSize(), x - m_resizeStartDiff.x + 3);
      auto newY = general_utils::max<position_t>(1.0 * m_view->getPixelSize(), y - m_resizeStartDiff.y + 3);

      this->setGeometry(newX,
                        newY,
                        geometry.width(),
                        geometry.height());

      auto horGeometry = m_parent->m_horResizeHandle->geometry();
      m_parent->m_horResizeHandle->setGeometry(horGeometry.x(),
                                               newY,
                                               newX,
                                               horGeometry.height());

      auto vertGeometry = m_parent->m_vertResizeHandle->geometry();
      m_parent->m_vertResizeHandle->setGeometry(newX,
                                                vertGeometry.y(),
                                                vertGeometry.width(),
                                                newY);
      event->accept();
    }
  }

  void HVResizeHandle::mouseReleaseEvent(QMouseEvent* event)
  {
    m_resizeStart = false;
    auto view_geometry = m_view->geometry();
    auto geometry = this->geometry();

    auto old_width = m_view->getImage().getWidth();
    auto old_height = m_view->getImage().getHeight();
    position_t width = static_cast<double>(geometry.x() - view_geometry.x()) / m_view->getPixelSize();
    position_t height = static_cast<double>(geometry.y() - view_geometry.y()) / m_view->getPixelSize();

    if(old_width != width || old_height != height) {
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

  void HVResizeHandle::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);
    painter.setPen(QPen(QColor(100, 100, 100, 255)));
    painter.setBrush(QBrush(QColor(100, 100, 100, 255)));
    painter.drawRect(0, 0, this->geometry().width() - 1, this->geometry().height() - 1);
  }
}
