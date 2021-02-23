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
#include "dummyselectionwidget.h"

#include <QPainter>
#include <QPen>
#include <QWheelEvent>
#include "../env/imageenvironment.h"
#include "../manager/dummyselectionmanager.h"
#include "imageeditorview.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  DummySelectionWidget::DummySelectionWidget(QWidget* parent) :
    QWidget(parent),
    m_dashOffset(1)
  {
    QObject::connect(&m_dashTimer, &QTimer::timeout, this, &DummySelectionWidget::updateDash);
  }

  void DummySelectionWidget::updateSelection()
  {
    auto& img_env = getImageEnvironment();
    auto& dummy_selection_manager = getDummySelectionManager();

    auto selection_rect = dummy_selection_manager.getSelectionRect();
    auto pixel_size = img_env.getView().getPixelSize();
    selection_rect.x = double(selection_rect.x) * pixel_size;
    selection_rect.y = double(selection_rect.y) * pixel_size;
    selection_rect.width = double(selection_rect.width) * pixel_size;
    selection_rect.height = double(selection_rect.height) * pixel_size;
    this->resize(selection_rect.width + (SELECTION_HANDLE_WIDTH * 2),
                 selection_rect.height + (SELECTION_HANDLE_HEIGHT * 2));

    auto adj_offset = img_env.getScrollArea().frameWidth() + img_env.getScrollArea().lineWidth();
    this->setPosition(img_env.getView().frameGeometry().x() + selection_rect.x + adj_offset,
                      img_env.getView().frameGeometry().y() + selection_rect.y + adj_offset);
  }

  void DummySelectionWidget::setPosition(int x, int y)
  {
    auto g = this->geometry();
    this->setGeometry(x - SELECTION_HANDLE_WIDTH, y - SELECTION_HANDLE_WIDTH, g.width(), g.height());
  }

  void DummySelectionWidget::wheelEvent(QWheelEvent* event)
  {
    if(event->modifiers() & Qt::ControlModifier) {
      getImageEnvironment().getView().wheelEvent(event);
    } else {
      getImageEnvironment().getScrollArea().wheelEvent(event);
    }
  }

  void DummySelectionWidget::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);

    QPen dashed_line(QColor(10, 10, 10, 255), 2);
    dashed_line.setStyle(Qt::CustomDashLine);
    dashed_line.setDashOffset(m_dashOffset);
    dashed_line.setDashPattern({ 5.0, 5.0 });

    painter.setPen(dashed_line);
    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));

    auto width = this->geometry().width() - ((SELECTION_HANDLE_WIDTH * 2) + 1);
    auto height = this->geometry().height() - ((SELECTION_HANDLE_HEIGHT * 2) + 1);
    painter.drawRect(SELECTION_HANDLE_WIDTH, SELECTION_HANDLE_HEIGHT, width, height);
  }

  QTimer& DummySelectionWidget::getDashTimer() noexcept
  {
    return m_dashTimer;
  }

  const QTimer& DummySelectionWidget::getDashTimer() const noexcept
  {
    return m_dashTimer;
  }

  void DummySelectionWidget::updateDash()
  {
    if(this->isVisible()) {
      m_dashOffset = (m_dashOffset + 1) % 10;
      this->repaint();
    }
  }
}
