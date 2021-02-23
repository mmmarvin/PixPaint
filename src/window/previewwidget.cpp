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
#include "previewwidget.h"

#include <QHBoxLayout>
#include <QPainter>
#include "previewview.h"

namespace pixpaint
{
  PreviewWidget::PreviewWidget(QWidget* parent,
                               Image& image) :
    QWidget(parent)
  {
    auto* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);

    m_preview = new PreviewView(this, image);
    m_preview->setFixedSize(PREVIEW_WIDTH, PREVIEW_HEIGHT);
    layout->addWidget(m_preview);

    this->setLayout(layout);
  }

  void PreviewWidget::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);

    painter.setPen(QPen(QColor(0, 0, 0, 255)));
    painter.setBrush(QBrush(QColor(252, 255, 215, 255)));
    painter.drawRect(0, 0, this->geometry().width() - 1, this->geometry().height() - 1);
  }

  PreviewView* PreviewWidget::getPreviewView() noexcept
  {
    return m_preview;
  }

  const PreviewView* PreviewWidget::getPreviewView() const noexcept
  {
    return m_preview;
  }
}
