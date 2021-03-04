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
#include "layerwidget.h"

#include <QInputDialog>
#include <QListWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QToolTip>
#include "../env/imageenvironment.h"
#include "../event/gui/frame_events.h"
#include "../event/gui/history_events.h"
#include "../event/gui/image_events.h"
#include "../event/gui/tab_events.h"
#include "../image/image.h"
#include "../helper/selection_helpers.h"
#include "../history/addlayeraction.h"
#include "../history/movelayeraction.h"
#include "../history/removelayeraction.h"
#include "../history/renamelayeraction.h"
#include "../history/showlayeraction.h"
#include "../history/hidelayeraction.h"
#include "../manager/drawermanager.h"
#include "../manager/imagemanager.h"
#include "../manager/historymanager.h"
#include "../manager/previewmanager.h"
#include "../assert.h"
#include "../debug_log.h"
#include "../gui_define.h"
#include "imageeditorview.h"
#include "layerwidgetitem.h"
#include "layerpreviewview.h"

namespace pixpaint
{
  LayerWidget::LayerWidget(QWidget* parent) :
    QWidget(parent),
    m_selectedLayerItem(nullptr),
    m_selectedLayerIndex(0),
    m_moveDstLayer(nullptr),
    m_moveDstLayerIndex(0)
  {
    auto* outerLayer = new QVBoxLayout(this);
    m_scrollArea = new QScrollArea(this);
    m_itemHolderWidget = new QWidget(m_scrollArea);
    m_itemHolderLayout = new QVBoxLayout(m_itemHolderWidget);
    m_itemHolderLayout->setAlignment(Qt::AlignTop);
//    m_itemHolderLayout->setMargin(0);
    m_itemHolderWidget->setLayout(m_itemHolderLayout);
    m_scrollArea->setWidget(m_itemHolderWidget);

    auto* btnLayout = new QHBoxLayout;
    btnLayout->setAlignment(Qt::AlignLeft);

    m_addButton = new QPushButton(this);
    m_addButton->setIcon(QIcon("res/add_btn_icon.png"));
    m_addButton->setToolTip(tr("Add Layer"));
    m_addButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    QObject::connect(m_addButton, &QPushButton::clicked, this, &LayerWidget::addLayer);

    m_removeButton = new QPushButton(this);
    m_removeButton->setIcon(QIcon("res/rem_btn_icon.png"));
    m_removeButton->setToolTip(tr("Remove Layer"));
    m_removeButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    m_removeButton->setEnabled(false);
    QObject::connect(m_removeButton, &QPushButton::clicked, this, &LayerWidget::removeLayer);

    m_opacitySlider = new QSlider(Qt::Orientation::Horizontal, this);
    m_opacitySlider->setTickPosition(QSlider::TicksRight);
    m_opacitySlider->setMinimum(0);
    m_opacitySlider->setMaximum(100);
    m_opacitySlider->setTickInterval(1);
    m_opacitySlider->setToolTip("Opacity");
    connect(m_opacitySlider, &QSlider::sliderMoved, [](int value) {
      getImageManager().getImage().getCurrentLayer().setOpacity(value);
      getImageEnvironment().getView().repaint();
      getPreviewManager().refreshAll();

      QToolTip::showText(QCursor::pos(), (std::to_string(value) + std::string("%")).c_str(), nullptr);
    });

    btnLayout->addWidget(m_addButton, 1);
    btnLayout->addWidget(m_removeButton, 1);
    btnLayout->addWidget(m_opacitySlider, 10);

    outerLayer->addWidget(m_scrollArea, 9);
    outerLayer->addLayout(btnLayout, 1);

    this->setLayout(outerLayer);
  }

  void LayerWidget::keyPressEvent(QKeyEvent* event)
  {
    if(event->key() == Qt::Key_Delete) {
      this->removeLayer();
      event->accept();
      return;
    }

    event->ignore();
  }

  void LayerWidget::mouseMoveEvent(QMouseEvent* event)
  {
    auto mx = event->globalPos().x();
    auto my = event->globalPos().y();

    if(event->buttons() == Qt::MouseButton::LeftButton) {     
      for(int i = 0; i < m_itemHolderLayout->count(); ++i) {
        auto* widget = m_itemHolderLayout->itemAt(i)->widget();
        auto widgetMousePosition = widget->mapFromGlobal(QPoint(mx, my));

        if(widget->rect().contains(widgetMousePosition.x(), widgetMousePosition.y())) {
          if(widget != m_moveDstLayer) {
            if(widget != m_selectedLayerItem) {
              if(m_moveDstLayer) {
                m_moveDstLayer->setMoveDestination(false);
                m_moveDstLayer->repaint();
              }

              m_moveDstLayer = static_cast<LayerWidgetItem*>(widget);
              m_moveDstLayerIndex = i;

              m_moveDstLayer->setMoveDestination(true);
              m_moveDstLayer->repaint();
            } else {
              if(m_moveDstLayer) {
                m_moveDstLayer->setMoveDestination(false);
                m_moveDstLayer->repaint();
              }

              m_moveDstLayer = nullptr;
              m_moveDstLayerIndex = 0;
            }
          }
          break;
        }
      }
    }
  }

  void LayerWidget::mouseReleaseEvent(QMouseEvent* event)
  {
    auto& image_manager = getImageManager();

    if(event->button() == Qt::MouseButton::LeftButton) {
      if(m_selectedLayerItem && m_moveDstLayer) {
        selection_helpers::tryFinalizeAllSelections(true);

        auto& image = image_manager.getImage();
        image.moveLayer(m_selectedLayerIndex, m_moveDstLayerIndex);
        emitHistoryAction(MoveLayerAction(m_selectedLayerIndex, m_moveDstLayerIndex));

        clearItems();
        createItems();

        image_manager.refresh();
        getPreviewManager().refreshAll();

        slotClicked(static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(m_moveDstLayerIndex)->widget()),
                    m_moveDstLayerIndex);

        m_moveDstLayer = nullptr;
        m_moveDstLayerIndex = 0;
      }
    }
  }

  void LayerWidget::resizeEvent(QResizeEvent*)
  {
    resizeHolderWidget();
  }

  void LayerWidget::onEmit(const gui_events::TabRemoveEvent& event)
  {
    if(!event.numberOfTabs) {
      clearItems();
    }
  }

  void LayerWidget::onEmit(const gui_events::FrameChangeEvent&)
  {
    clearItems();
    createItems();
    m_opacitySlider->setValue(getImageManager().getImage().getCurrentLayer().getOpacity());

    if(getImageManager().getImage().getLayerCount() > 1) {
      m_removeButton->setEnabled(true);
    } else {
      m_removeButton->setEnabled(false);
    }
  }

  void LayerWidget::onEmit(const gui_events::ImageFlattenEvent&)
  {
    this->onEmit(gui_events::FrameChangeEvent());
  }

  void LayerWidget::onEmit(const gui_events::ImageClearEvent&)
  {
    this->onEmit(gui_events::FrameChangeEvent());
  }

  void LayerWidget::onEmit(const gui_events::HistoryLayerChangeEvent& event)
  {
    auto layer_index = event.layerIndex;
    slotClicked(static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(layer_index)->widget()), layer_index);
  }

  void LayerWidget::onEmit(const gui_events::HistoryRecreateLayerEvent&)
  {
    clearItems();
    createItems();

    auto layer_index = getImageManager().getImage().getCurrentLayerIndex();
    slotClicked(static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(layer_index)->widget()), layer_index);
    m_scrollArea->repaint();
  }

  void LayerWidget::onEmit(const gui_events::HistoryRefreshLayerEvent&)
  {
    PIXPAINT_ASSERT(static_cast<size_t>(m_itemHolderLayout->count()) == getImageManager().getImage().getLayerCount(),
                    "Count doesn't match");

    for(int i = 0, isize = m_itemHolderLayout->count(); i < isize; ++i) {
      auto* item = static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(i)->widget());
      const auto& layer_name = getImageManager().getImage().getLayerName(i);
      auto layer_visibility = getImageManager().getImage().isVisible(i);

      item->setLayerName(layer_name);
      item->setLayerVisibility(layer_visibility);
    }
  }

  LayerWidgetItem* LayerWidget::createItem(std::size_t index, bool addAtIndex)
  {
    auto& preview_manager = getPreviewManager();
    auto* layer_item = new LayerWidgetItem(m_itemHolderWidget, index);
    layer_item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layer_item->setFixedHeight(ITEM_HEIGHT);
    layer_item->setContextMenuPolicy(Qt::CustomContextMenu);

    if(!addAtIndex) {
      m_itemHolderLayout->addWidget(layer_item);
    } else {
      m_itemHolderLayout->insertWidget(index, layer_item);
    }
    preview_manager.registerPreview(*layer_item->m_view);

    QObject::connect(layer_item, &LayerWidgetItem::clicked, [layer_item, this](){
      this->slotClicked(layer_item, layer_item->getLayerIndex());
    });

    QObject::connect(layer_item, &LayerWidgetItem::customContextMenuRequested, [this, layer_item](const QPoint& pos) {
      layer_item->click();
      this->slotContextMenu(layer_item, pos);
    });

    return layer_item;
  }

  void LayerWidget::createItems()
  {
    auto& image = getImageManager().getImage();
    auto layer_index = image.getCurrentLayerIndex();
    for(size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
      auto* layer_item = createItem(i);
      if(i == layer_index) {
        layer_item->setSelected(true);
        m_selectedLayerItem = layer_item;
        m_selectedLayerIndex = i;

        getDrawerManager().updateDrawers();
      }
    }

    resizeHolderWidget();
  }

  void LayerWidget::clearItems()
  {
    auto& preview_manager = getPreviewManager();
    while(m_itemHolderLayout->count()) {
      auto* item = m_itemHolderLayout->itemAt(0);
      preview_manager.deregisterPreview(*static_cast<LayerWidgetItem*>(item->widget())->m_view);
      delete item->widget();
    }

    m_selectedLayerItem = nullptr;
    m_selectedLayerIndex = 0;
  }

  LayerWidgetItem* LayerWidget::addItem(std::size_t index)
  {
    auto* ret = createItem(index, true);

    // reseat the items after this added item
    for(int i = index + 1, isize = m_itemHolderLayout->count(); i < isize; ++i) {
      static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(i)->widget())->setLayerIndex(i);
    }

    return ret;
  }

  void LayerWidget::removeItem(std::size_t index)
  {
    auto* item = m_itemHolderLayout->itemAt(index);
    getPreviewManager().deregisterPreview(*static_cast<LayerWidgetItem*>(item->widget())->m_view);
    delete item->widget();

    // reseat the items after this removed item
    for(int i = index, isize = m_itemHolderLayout->count(); i < isize; ++i) {
      static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(i)->widget())->setLayerIndex(i);
    }
  }

  void LayerWidget::addLayer()
  {
    auto& preview_manager = getPreviewManager();
    auto& image_manager = getImageManager();
    auto layer_to_add_index = m_selectedLayerIndex + 1;

    image_manager.getImage().addLayer(layer_to_add_index);

    selection_helpers::tryFinalizeAllSelections(true);
    emitHistoryAction(AddLayerAction(layer_to_add_index));

    auto* newItem = addItem(layer_to_add_index);
    newItem->click();

    preview_manager.refreshAll();
    image_manager.refresh();
    resizeHolderWidget();

    if(!m_removeButton->isEnabled()) {
      m_removeButton->setEnabled(true);
    }
  }

  void LayerWidget::removeLayer()
  {
    auto& image_manager = getImageManager();
    if(image_manager.getImage().getLayerCount() > 1) {
      auto& preview_manager = getPreviewManager();
      auto layer_index = m_selectedLayerItem->getLayerIndex();

      selection_helpers::tryFinalizeAllSelections(true);
      emitHistoryAction(RemoveLayerAction(layer_index));

      image_manager.getImage().removeLayer(layer_index);
      removeItem(layer_index);
      m_selectedLayerItem = nullptr;
      m_selectedLayerIndex = 0;
      if(layer_index > 0) {
        static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(layer_index - 1)->widget())->click();
      } else {
        static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(0)->widget())->click();
      }

      preview_manager.refreshAll();
      image_manager.refresh();
      resizeHolderWidget();

      if(image_manager.getImage().getLayerCount() == 1) {
        m_removeButton->setEnabled(false);
      }
    }
  }

  void LayerWidget::resizeHolderWidget()
  {
    int left, right, top, bottom;
    m_itemHolderLayout->getContentsMargins(&left, &top, &right, &bottom);
    m_itemHolderWidget->resize(m_scrollArea->geometry().width() - 1 - left - right - m_itemHolderLayout->spacing(),
                               (ITEM_HEIGHT + top + bottom) * m_itemHolderLayout->count());
  }

  void LayerWidget::slotContextMenu(LayerWidgetItem* layerItem, const QPoint& pos)
  {
    QMenu menu(layerItem);
    auto* visibilityAction = [&menu, layerItem]() {
      if(getImageManager().getImage().isVisible(layerItem->getLayerIndex())) {
        return menu.addAction(tr("Hide Layer"));
      }
      return menu.addAction(tr("Show Layer"));
    } ();
    menu.addSeparator();
    auto* addAction = menu.addAction(tr("Add Layer"));
    auto* renameAction = menu.addAction(tr("Rename Layer"));

    QObject::connect(visibilityAction, &QAction::triggered, [layerItem](){
      auto layerIndex = layerItem->getLayerIndex();
      auto visible = getImageManager().getImage().isVisible(layerIndex);
      if(!visible) {
        emitHistoryAction(ShowLayerAction(layerIndex));
      } else {
        emitHistoryAction(HideLayerAction(layerIndex));
      }

      layerItem->toggled(!visible);
      layerItem->m_checkbox->setChecked(!visible);
    });
    QObject::connect(addAction, &QAction::triggered, [this](){
      addLayer();
    });

    QObject::connect(renameAction, &QAction::triggered, [this, layerItem](){
      bool ok;
      auto new_layer_name = QInputDialog::getText(this,
                                                  tr("Enter new layer name"),
                                                  tr("New layer name:"),
                                                  QLineEdit::Normal,
                                                  layerItem->getLayerName().c_str(),
                                                  &ok);

      if(ok && !new_layer_name.isEmpty()) {
        auto layer_index = layerItem->getLayerIndex();
        auto& image = getImageManager().getImage();
        auto new_layer_name_str = std::string(new_layer_name.toUtf8().constData());

        emitHistoryAction(RenameLayerAction(image.getLayerName(layer_index), new_layer_name_str));

        image.renameLayer(layer_index, new_layer_name_str);
        static_cast<LayerWidgetItem*>(m_itemHolderLayout->itemAt(layer_index)->widget())->setLayerName(new_layer_name_str);
      }
    });

    menu.exec(layerItem->mapToGlobal(pos));
  }

  void LayerWidget::slotClicked(LayerWidgetItem* layerItem, std::size_t layerIndex)
  {    
    selection_helpers::tryFinalizeAllSelections(true);

    if(m_selectedLayerItem) {
      m_selectedLayerItem->setSelected(false);
      m_selectedLayerItem->repaint();
    }

    layerItem->setSelected(true);
    m_selectedLayerItem = layerItem;
    m_selectedLayerIndex = layerIndex;
    m_selectedLayerItem->repaint();

    getImageManager().getImage().setCurrentLayerIndex(layerIndex);
    getDrawerManager().updateDrawers();
    m_opacitySlider->setValue(getImageManager().getImage().getCurrentLayer().getOpacity());
  }
}
