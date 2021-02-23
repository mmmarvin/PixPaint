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
#ifndef LAYERWIDGET_H
#define LAYERWIDGET_H

#include <memory>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>

#include "../manager/eventmanager.h"

namespace pixpaint
{
  class LayerWidgetItem;
  class LayerPreviewView;
namespace gui_events
{
  struct TabRemoveEvent;
  struct FrameChangeEvent;
  struct ImageFlattenEvent;
  struct ImageClearEvent;
  struct HistoryLayerChangeEvent;
  struct HistoryRefreshLayerEvent;
}
  class LayerWidget : public QWidget,
                      EventListener<gui_events::TabRemoveEvent>,
                      EventListener<gui_events::FrameChangeEvent>,
                      EventListener<gui_events::ImageFlattenEvent>,
                      EventListener<gui_events::ImageClearEvent>,
                      EventListener<gui_events::HistoryLayerChangeEvent>,
                      EventListener<gui_events::HistoryRefreshLayerEvent>
  {
    static constexpr auto ITEM_HEIGHT = 100;

  public:
    explicit LayerWidget(QWidget* parent);

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent*) override;

  private:
    void onEmit(const gui_events::TabRemoveEvent& event) override;
    void onEmit(const gui_events::FrameChangeEvent& event) override;
    void onEmit(const gui_events::ImageFlattenEvent& event) override;
    void onEmit(const gui_events::ImageClearEvent& event) override;
    void onEmit(const gui_events::HistoryLayerChangeEvent& event) override;
    void onEmit(const gui_events::HistoryRefreshLayerEvent& event) override;

    LayerWidgetItem* createItem(std::size_t index, bool addAtIndex = false);
    void createItems();
    void clearItems();
    LayerWidgetItem* addItem(std::size_t index);
    void removeItem(std::size_t index);

  private:
    friend class LayerWidgetItem;
    friend class MainWindow;

    void addLayer();
    void removeLayer();
    void resizeHolderWidget();

  private:
    void slotContextMenu(LayerWidgetItem* layerItem, const QPoint& pos);
    void slotClicked(LayerWidgetItem* layerItem, std::size_t layerIndex);

  private:
    QWidget*                      m_itemHolderWidget;
    QVBoxLayout*                  m_itemHolderLayout;

    QScrollArea*                  m_scrollArea;
    QPushButton*                  m_addButton;
    QPushButton*                  m_removeButton;

    LayerWidgetItem*              m_selectedLayerItem;
    std::size_t                   m_selectedLayerIndex;

    QPoint                        m_movePosition;
    LayerWidgetItem*              m_moveDstLayer;
    std::size_t                   m_moveDstLayerIndex;
  };
}

#endif // LAYERWIDGET_H
