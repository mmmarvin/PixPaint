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
#include "frametoolbox.h"

#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../event/gui/animation_events.h"
#include "../event/gui/frame_events.h"
#include "../event/gui/history_events.h"
#include "../event/gui/tab_events.h"
#include "../helper/selection_helpers.h"
#include "../history/addframeaction.h"
#include "../history/removeframeaction.h"
#include "../manager/documentmanager.h"
#include "../manager/drawermanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../assert.h"
#include "../debug_log.h"
#include "../document.h"
#include "../gui_define.h"
#include "framepreviewview.h"
#include "frametoolboxitem.h"
#include "imageeditorview.h"
#include "righttoolbox.h"

namespace pixpaint
{
  FrameToolbox::FrameToolbox(QWidget* parent) :
    QWidget(parent),
    m_playTimer(this),
    m_selectedFrameItem(nullptr),
    m_selectedFrameIndex(0)
  {
    static const QIcon playIcon("res/play_btn_icon.png");
    static const QIcon stopIcon("res/pause_btn_icon.png");

    auto* outer_layer = new QVBoxLayout(this);
    auto* outer_surface = new QWidget(this);

    auto* layout = new QVBoxLayout(outer_surface);
    m_scrollArea = new QScrollArea(outer_surface);
    m_scrollArea->setMinimumHeight(90);
    layout->addWidget(m_scrollArea);

    auto* button_layout = new QHBoxLayout;
    button_layout->setAlignment(Qt::AlignLeft);
    m_addButton = new QPushButton(outer_surface);
    m_addButton->setIcon(QIcon("res/add_btn_icon.png"));
    m_addButton->setToolTip(tr("Add Frame"));
    m_addButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_addButton, &QPushButton::clicked, [this]{ this->addFrame(); });
    m_addCpyButton = new QPushButton(outer_surface);
    m_addCpyButton->setIcon(QIcon("res/add_cpy_btn_icon.png"));
    m_addCpyButton->setToolTip(tr("Copy Current Frame"));
    m_addCpyButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_addCpyButton, &QPushButton::clicked, [this]{ this->addFrame(true); });
    m_removeButton = new QPushButton(outer_surface);
    m_removeButton->setIcon(QIcon("res/rem_btn_icon.png"));
    m_removeButton->setToolTip(tr("Remove Frame"));
    m_removeButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    m_removeButton->setEnabled(false);
    connect(m_removeButton, &QPushButton::clicked, this, &FrameToolbox::removeFrame);
    m_timerText = new QSpinBox(outer_surface);
    m_timerText->setToolTip(tr("Frame Duration"));
    m_timerText->setMinimum(1);
    m_timerText->setMaximum(1000000);
    m_timerText->setMaximumHeight(TOOLBOX_BUTTON_HEIGHT);
    m_timerText->setValue(getDocumentManager().getDocument().getAnimation().getCurrentFrameDuration());
    connect(m_timerText, QOverload<int>::of(&QSpinBox::valueChanged), [](int v){
      getDocumentManager().getDocument().getAnimation().setCurrentFrameDuration(v);
    });

    m_backBeginButton = new QPushButton(outer_surface);
    m_backBeginButton->setIcon(QIcon("res/back_btn_icon.png"));
    m_backBeginButton->setToolTip(tr("Previous Frame"));
    m_backBeginButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_backBeginButton, &QPushButton::clicked, [this] {
      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(0)->widget())->click();
    });
    m_backButton = new QPushButton(outer_surface);
    m_backButton->setIcon(QIcon("res/back_btn_icon.png"));
    m_backButton->setToolTip(tr("Previous Frame"));
    m_backButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_backButton, &QPushButton::clicked, [this] {
      auto& document_manager = getDocumentManager();
      auto frame_index = document_manager.getDocument().getAnimation().getCurrentFrameIndex();
      if(frame_index == 0) {
        frame_index = document_manager.getDocument().getAnimation().getFrameCount() - 1;
      } else {
        --frame_index;
      }

      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index)->widget())->click();
    });
    m_playButton = new QPushButton(outer_surface);
    m_playButton->setIcon(QIcon("res/play_btn_icon.png"));
    m_playButton->setToolTip(tr("Play Animation"));
    m_playButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_playButton, &QPushButton::clicked, [this]{
      auto& document_manager = getDocumentManager();

      if(m_playTimer.isActive()) {
        m_addButton->setEnabled(true);
        m_addCpyButton->setEnabled(true);
        if(getDocumentManager().getDocument().getAnimation().getFrameCount() > 1) {
          m_removeButton->setEnabled(true);
        } else {
          m_removeButton->setEnabled(false);
        }
        m_timerText->setEnabled(true);
        m_playTimer.stop();
        m_playButton->setIcon(playIcon);
        emitEvent(gui_events::AnimationPauseEvent());

        auto current_index = document_manager.getDocument().getAnimation().getCurrentFrameIndex();
        static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(current_index)->widget())->click();
        getImageEnvironment().getView().clearTemporaryImage();
      } else {
        selection_helpers::tryFinalizeAllSelections(true);

        m_addButton->setEnabled(false);
        m_addCpyButton->setEnabled(false);
        m_removeButton->setEnabled(false);
        m_timerText->setEnabled(false);
        m_playTimer.setInterval(document_manager.getDocument().getAnimation().getCurrentFrameDuration());
        m_playTimer.start();
        m_playButton->setIcon(stopIcon);
        emitEvent(gui_events::AnimationPlayEvent());

        auto current_index = document_manager.getDocument().getAnimation().getCurrentFrameIndex();
        getImageEnvironment().getView().setTemporaryImage(document_manager.getDocument().getAnimation().getFrame(current_index));
      }
    });
    m_forwardButton = new QPushButton(outer_surface);
    m_forwardButton->setIcon(QIcon("res/forward_btn_icon.png"));
    m_forwardButton->setToolTip(tr("Next Frame"));
    m_forwardButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_forwardButton, &QPushButton::clicked, [this] {
      auto& document_manager = getDocumentManager();
      auto frame_index = document_manager.getDocument().getAnimation().getCurrentFrameIndex();
      frame_index = (frame_index + 1) % document_manager.getDocument().getAnimation().getFrameCount();

      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index)->widget())->click();
    });
    m_forwardEndButton = new QPushButton(outer_surface);
    m_forwardEndButton->setIcon(QIcon("res/forward_end_btn_icon.png"));
    m_forwardEndButton->setToolTip(tr("Last Frame"));
    m_forwardEndButton->setFixedSize(TOOLBOX_BUTTON_WIDTH, TOOLBOX_BUTTON_HEIGHT);
    connect(m_forwardEndButton, &QPushButton::clicked, [this] {
      const auto frame_index = getDocumentManager().getDocument().getAnimation().getFrameCount() - 1;
      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index)->widget())->click();
    });

    button_layout->addWidget(m_addButton);
    button_layout->addWidget(m_addCpyButton);
    button_layout->addWidget(m_removeButton);
    button_layout->addSpacerItem(new QSpacerItem(100, TOOLBOX_BUTTON_HEIGHT));
    button_layout->addWidget(m_timerText);

    button_layout->addWidget(m_backBeginButton);
    button_layout->addWidget(m_backButton);
    button_layout->addWidget(m_playButton);
    button_layout->addWidget(m_forwardButton);
    button_layout->addWidget(m_forwardEndButton);

    layout->addLayout(button_layout);
    outer_layer->addWidget(outer_surface);

    m_itemHolderWidget = new QWidget(m_scrollArea);
    m_itemHolderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_itemHolderLayout = new QHBoxLayout(m_itemHolderWidget);
    m_itemHolderLayout->setAlignment(Qt::AlignLeft);
//    m_itemHolderLayout->setMargin(0);
    m_scrollArea->setWidget(m_itemHolderWidget);

    connect(&m_playTimer, &QTimer::timeout, this, &FrameToolbox::slotTimeOut);
    m_playTimer.setInterval(1000);
  }

  void FrameToolbox::resizeEvent(QResizeEvent*)
  {
    resizeHolderWidget();
  }

  void FrameToolbox::onEmit(const gui_events::TabAddEvent&)
  {
    if(!this->isEnabled()) {
      this->setEnabled(true);
    }
  }

  void FrameToolbox::onEmit(const gui_events::TabRemoveEvent& event)
  {
    if(!event.numberOfTabs) {
      clearItems();
      this->setEnabled(false);
    }
  }

  void FrameToolbox::onEmit(const gui_events::TabChangeEvent&)
  {
    clearItems();
    createItems();
    if(getDocumentManager().getDocument().getAnimation().getFrameCount() > 1) {
      m_removeButton->setEnabled(true);
    } else {
      m_removeButton->setEnabled(false);
    }
  }

  void FrameToolbox::onEmit(const gui_events::HistoryFrameChangeEvent& event)
  {
    auto frame_index = event.frameIndex;
    auto previous_frame_index = m_selectedFrameIndex;
    slotClicked(static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index)->widget()), frame_index);
    emitEvent(gui_events::FrameChangeEvent { previous_frame_index, frame_index });
  }

  void FrameToolbox::onEmit(const gui_events::HistoryRecreateFrameEvent&)
  {
    clearItems();
    createItems();

    auto frame_index = getDocumentManager().getDocument().getAnimation().getCurrentFrameIndex();
    slotClicked(static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index)->widget()), frame_index);
    m_scrollArea->repaint();
  }

  FrameToolboxItem* FrameToolbox::createItem(std::size_t index, bool addAtIndex)
  {
    auto& preview_manager = getPreviewManager();
    auto* item = new FrameToolboxItem(this, index);
    item->setFixedSize(ITEM_WIDTH, ITEM_HEIGHT);
    if(!addAtIndex) {
      m_itemHolderLayout->addWidget(item);
    } else {
      m_itemHolderLayout->insertWidget(index, item);
    }
    preview_manager.registerPreview(*item->m_view);

    connect(item, &FrameToolboxItem::clicked, [this, item, index]() {
      slotClicked(item, index);
    });

    return item;
  }

  void FrameToolbox::createItems()
  {
    auto& document = getDocumentManager().getDocument();
    for(std::size_t i = 0, isize = document.getAnimation().getFrameCount(); i < isize; ++i) {
      auto* item = createItem(i);
      if(i == getDocumentManager().getDocument().getAnimation().getCurrentFrameIndex()) {
        item->setSelected(true);
        m_selectedFrameItem = item;
        m_selectedFrameIndex = i;

        emitEvent(gui_events::FrameChangeEvent { 0, i });
        getDrawerManager().updateDrawers();
      }
    }
    resizeHolderWidget();
  }

  void FrameToolbox::clearItems()
  {
    auto& preview_manager = getPreviewManager();

    while(m_itemHolderLayout->count()) {
      auto* item = m_itemHolderLayout->itemAt(0);
      preview_manager.deregisterPreview(*static_cast<FrameToolboxItem*>(item->widget())->m_view);
      delete item->widget();
    }

    m_selectedFrameItem = nullptr;
    m_selectedFrameIndex = 0;
  }

  FrameToolboxItem* FrameToolbox::addItem(std::size_t index)
  {
    auto* ret = createItem(index, true);

    // reseat the items after this added item
    for(int i = index + 1, isize = m_itemHolderLayout->count(); i < isize; ++i) {
      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(i)->widget())->setItemIndex(i);
    }

    return ret;
  }

  void FrameToolbox::removeItem(std::size_t index)
  {
    auto* item = m_itemHolderLayout->itemAt(index);
    getPreviewManager().deregisterPreview(*static_cast<FrameToolboxItem*>(item->widget())->m_view);
    delete item->widget();

    // reseat the items after this removed item
    for(int i = index, isize = m_itemHolderLayout->count(); i < isize; ++i) {
      static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(i)->widget())->setItemIndex(i);
    }
  }

  void FrameToolbox::addFrame(bool copy)
  {
    if(copy) {
      auto& animation = getDocumentManager().getDocument().getAnimation();
      auto frame_index = animation.getCurrentFrameIndex();
      auto image = animation.getFrame(frame_index);
      auto duration = animation.getFrameDuration(frame_index);

      emitHistoryAction(AddFrameAction(++frame_index));

      animation.addFrame(frame_index, std::move(image), duration);
      animation.setCurrentFrameIndex(frame_index);

      auto* new_item = addItem(frame_index);
      new_item->click();
    } else {
      auto& animation = getDocumentManager().getDocument().getAnimation();
      const auto& currentFrame = animation.getCurrentFrame();
      auto frame_index = animation.getCurrentFrameIndex();

      emitHistoryAction(AddFrameAction(++frame_index));

      animation.addFrame(frame_index, currentFrame.getWidth(), currentFrame.getHeight());
      animation.setCurrentFrameIndex(frame_index);

      auto* new_item = addItem(frame_index);
      new_item->click();
    }
    resizeHolderWidget();

    if(!m_removeButton->isEnabled()) {
      m_removeButton->setEnabled(true);
    }
  }

  void FrameToolbox::removeFrame()
  {
    auto& animation = getDocumentManager().getDocument().getAnimation();
    if(animation.getFrameCount() > 1) {
      auto frame_index = animation.getCurrentFrameIndex();

      emitHistoryAction(RemoveFrameAction(frame_index));

      animation.removeFrame(frame_index);
      removeItem(frame_index);
      m_selectedFrameItem = nullptr;
      m_selectedFrameIndex = 0;
      if(frame_index > 0 ) {
        static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(frame_index - 1)->widget())->click();
      } else {
        static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(0)->widget())->click();
      }

      if(animation.getFrameCount() == 1) {
        m_removeButton->setEnabled(false);
      }
    }
  }

  void FrameToolbox::resizeHolderWidget()
  {
    int left, right, top, bottom;
    m_itemHolderLayout->getContentsMargins(&left, &top, &right, &bottom);
    m_itemHolderWidget->resize((ITEM_WIDTH + left + right) * m_itemHolderLayout->count(),
                               m_scrollArea->geometry().height() - 1 - top - bottom - m_itemHolderLayout->spacing());
  }

  void FrameToolbox::slotClicked(FrameToolboxItem* item, std::size_t index)
  {
    auto previous_frame_index = m_selectedFrameIndex;

    selection_helpers::tryFinalizeAllSelections(true);

    if(m_selectedFrameItem) {
      m_selectedFrameItem->setSelected(false);
      m_selectedFrameItem->repaint();
    }

    item->setSelected(true);
    m_selectedFrameItem = item;
    m_selectedFrameIndex = index;
    m_selectedFrameItem->repaint();

    getDocumentManager().getDocument().getAnimation().setCurrentFrameIndex(item->getItemIndex());
    m_timerText->setValue(getDocumentManager().getDocument().getAnimation().getCurrentFrameDuration());

    auto& image = getDocumentManager().getDocument().getAnimation().getCurrentFrame();
    auto& view = getImageEnvironment().getView();
    view.setImage(image);
    view.repaint();

    getDrawerManager().updateDrawers();
    emitEvent(gui_events::FrameChangeEvent { previous_frame_index, index });
  }

  void FrameToolbox::slotTimeOut()
  {
    auto current_index = getDocumentManager().getDocument().getAnimation().getCurrentFrameIndex();
    current_index = (current_index + 1 ) % getDocumentManager().getDocument().getAnimation().getFrameCount();
    getDocumentManager().getDocument().getAnimation().setCurrentFrameIndex(current_index);
    getImageEnvironment().getView().setTemporaryImage(getDocumentManager().getDocument().getAnimation().getFrame(current_index));
    getImageEnvironment().getView().repaint();

    slotClicked(static_cast<FrameToolboxItem*>(m_itemHolderLayout->itemAt(current_index)->widget()), current_index);
    auto currentDuration = getDocumentManager().getDocument().getAnimation().getCurrentFrameDuration();
    m_timerText->setValue(currentDuration);
    m_playTimer.setInterval(currentDuration);
  }
}
