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
#include "frametoolboxitem.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionFocusRect>
#include "../manager/documentmanager.h"
#include "../document.h"
#include "framepreviewview.h"

namespace pixpaint
{
  FrameToolboxItem::FrameToolboxItem(QWidget* parent, std::size_t frameIndex) :
    QWidget(parent),
    m_selected(false),
    m_frameIndex(frameIndex)
  {
    auto* layout = new QHBoxLayout(this);
    m_view = new FrameView(this,
                           getDocumentManager().getDocument().getAnimation().getFrame(frameIndex),
                           frameIndex);
    m_view->setFixedSize(FRAME_PREVIEW_WIDTH, FRAME_PREVIEW_HEIGHT);
    layout->addWidget(m_view);
  }

  void FrameToolboxItem::setSelected(bool selected) noexcept
  {
    m_selected = selected;
  }

  bool FrameToolboxItem::isSelected() const noexcept
  {
    return m_selected;
  }

  void FrameToolboxItem::setItemIndex(std::size_t index) noexcept
  {
    m_frameIndex = index;
  }

  std::size_t FrameToolboxItem::getItemIndex() const noexcept
  {
    return m_frameIndex;
  }

  void FrameToolboxItem::click()
  {
    Q_EMIT clicked();
  }

  void FrameToolboxItem::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);
    if(m_selected) {
      painter.setPen(QPen(palette().color(QPalette::Highlight)));
      painter.setBrush(QBrush(palette().color(QPalette::Highlight)));
      painter.drawRect(0,
                       0,
                       this->geometry().width() - 1,
                       this->geometry().height() - 1);
    }
  }

  void FrameToolboxItem::mousePressEvent(QMouseEvent* event)
  {
    if(event->button() == Qt::MouseButton::LeftButton) {
      this->click();
      event->accept();
    } else {
      QWidget::mousePressEvent(event);
    }
  }
}
