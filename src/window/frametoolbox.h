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
#ifndef FRAMETOOLBOX_H
#define FRAMETOOLBOX_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>
#include "../manager/eventmanager.h"
#include "frametoolboxitem.h"

namespace pixpaint
{
namespace gui_events
{
  struct TabAddEvent;
  struct TabRemoveEvent;
  struct TabChangeEvent;
  struct HistoryFrameChangeEvent;
  struct HistoryRefreshFrameEvent;
}
  class FrameToolbox : public QWidget,
                       EventListener<gui_events::TabAddEvent>,
                       EventListener<gui_events::TabRemoveEvent>,
                       EventListener<gui_events::TabChangeEvent>,
                       EventListener<gui_events::HistoryFrameChangeEvent>,
                       EventListener<gui_events::HistoryRefreshFrameEvent>
  {
    static constexpr auto ITEM_WIDTH = FrameToolboxItem::FRAME_PREVIEW_WIDTH + 40;
    static constexpr auto ITEM_HEIGHT = FrameToolboxItem::FRAME_PREVIEW_HEIGHT + 40;

  public:
    explicit FrameToolbox(QWidget* parent);

  protected:
    void resizeEvent(QResizeEvent* event);

  private:
    void onEmit(const gui_events::TabAddEvent& event) override;
    void onEmit(const gui_events::TabRemoveEvent& event) override;
    void onEmit(const gui_events::TabChangeEvent& event) override;
    void onEmit(const gui_events::HistoryFrameChangeEvent& event) override;
    void onEmit(const gui_events::HistoryRefreshFrameEvent& event) override;

    FrameToolboxItem* createItem(std::size_t index, bool addAtIndex = false);
    void createItems();
    void clearItems();
    FrameToolboxItem* addItem(std::size_t index);
    void removeItem(std::size_t index);

  private:
    void addFrame(bool copy = false);
    void removeFrame();
    void resizeHolderWidget();

  private:
    void slotClicked(FrameToolboxItem* item, std::size_t index);
    void slotTimeOut();

    QTimer                          m_playTimer;
    QHBoxLayout*                    m_itemHolderLayout;
    QWidget*                        m_itemHolderWidget;
    QScrollArea*                    m_scrollArea;

    QSpinBox*                       m_timerText;
    QPushButton*                    m_addButton;
    QPushButton*                    m_addCpyButton;
    QPushButton*                    m_removeButton;
    QPushButton*                    m_playButton;

    FrameToolboxItem*               m_selectedFrameItem;
    std::size_t                     m_selectedFrameIndex;
  };
}

#endif // FRAMETOOLBOX_H
