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
#include "layerwidgetitem.h"

#include <QHBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "layerpreviewview.h"

namespace pixpaint
{
  LayerWidgetItem::LayerWidgetItem(QWidget* parent, std::size_t layerIndex) :
    QWidget(parent),
    m_layerIndex(layerIndex),
    m_selected(false),
    m_moveDestination(false)
  {
    auto& imageManager = getImageManager();

    auto* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);

    m_view = new LayerPreviewView(this, imageManager.getImage());
    m_view->setFixedSize(LAYER_PREVIEW_WIDTH, LAYER_PREVIEW_HEIGHT);
    m_view->setLayerIndex(layerIndex);

    layout->addWidget(m_view, 1);

    m_label = new QLabel(imageManager.getImage().getLayerName(layerIndex).c_str(), this);
    layout->addWidget(m_label, 8);

    m_checkbox = new QCheckBox(this);
    m_checkbox->setChecked(true);
    QObject::connect(m_checkbox, &QCheckBox::clicked, [this](bool toggled) {
      if(toggled) {
        // HISTORY FIX: Show Layer
      } else {
        // HISTORY FIX: Hide Layer
      }
      LayerWidgetItem::toggled(toggled);
    });
    layout->addWidget(m_checkbox, 1, Qt::AlignRight);
  }

  void LayerWidgetItem::setSelected(bool selected) noexcept
  {
    m_selected = selected;
  }

  bool LayerWidgetItem::isSelected() const noexcept
  {
    return m_selected;
  }

  void LayerWidgetItem::setMoveDestination(bool destination) noexcept
  {
    m_moveDestination = destination;
  }

  bool LayerWidgetItem::isMoveDestination() const noexcept
  {
    return m_moveDestination;
  }

  void LayerWidgetItem::setLayerIndex(std::size_t index)
  {
    m_view->setLayerIndex(index);
    m_layerIndex = index;
  }

  void LayerWidgetItem::setLayerName(std::string name)
  {
    m_label->setText(name.c_str());
  }

  void LayerWidgetItem::setLayerVisibility(bool visible)
  {
    m_checkbox->setChecked(visible);
  }

  std::size_t LayerWidgetItem::getLayerIndex() const noexcept
  {
    return m_layerIndex;
  }

  std::string LayerWidgetItem::getLayerName() const
  {
    return std::string(m_label->text().toUtf8().constData());
  }

  bool LayerWidgetItem::isLayerVisible() const noexcept
  {
    return m_checkbox->isChecked();
  }

  void LayerWidgetItem::click()
  {
    Q_EMIT clicked();
  }

  void LayerWidgetItem::paintEvent(QPaintEvent*)
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

    if(m_moveDestination) {
      painter.setBrush(QBrush(QColor(0, 0, 0, 255)));
      painter.drawRect(0, 0, this->geometry().width(), 5);
    }
  }

  void LayerWidgetItem::mousePressEvent(QMouseEvent* event)
  {
    if(event->button() == Qt::MouseButton::LeftButton) {
      this->click();
      event->accept();
    } else {
      QWidget::mousePressEvent(event);
    }
  }

  void LayerWidgetItem::toggled(bool clicked)
  {
    m_view->getImage().setVisible(m_layerIndex, clicked);

    getPreviewManager().refreshAll();
    getImageManager().refresh();
  }
}
