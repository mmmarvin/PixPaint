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
#include "righttoolbox.h"

#include <QGroupBox>
#include <QPainter>
#include <QVBoxLayout>
#include "../env/imageenvironment.h"
#include "../event/gui/animation_events.h"
#include "../event/gui/frame_events.h"
#include "../event/gui/tab_events.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "layerwidget.h"
#include "previewview.h"
#include "previewwidget.h"

namespace pixpaint
{
  RightToolbox::RightToolbox(QWidget* parent) :
    QWidget(parent)
  {
    auto& previewManager = getPreviewManager();
    auto& imageManager = getImageManager();

    auto* previewGroup = new QGroupBox(this);
    previewGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    previewGroup->setTitle(tr("Preview"));
    previewGroup->setMinimumHeight(250);
    previewGroup->setFlat(true);
    m_preview = new PreviewWidget(previewGroup, imageManager.getImage());
    previewManager.registerPreview(*m_preview->getPreviewView());

    auto* previewLayout = new QHBoxLayout(previewGroup);
    previewLayout->addWidget(m_preview);

    auto* layerGroup = new QGroupBox(this);
    layerGroup->setTitle(tr("Layers"));
    layerGroup->setFlat(true);
    m_layer = new LayerWidget(layerGroup);

    auto* layerLayout = new QHBoxLayout(layerGroup);
    layerLayout->addWidget(m_layer);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(previewGroup, 1);
    layout->addWidget(layerGroup, 9);

    this->setMinimumWidth(250);
    this->setLayout(layout);
  }

  PreviewWidget& RightToolbox::getPreviewWidget()
  {
    return *m_preview;
  }

  LayerWidget& RightToolbox::getLayerWidget()
  {
    return *m_layer;
  }

  void RightToolbox::onEmit(const gui_events::TabAddEvent& event)
  {
    if(!this->isEnabled() && event.numberOfTabs) {
      this->setEnabled(true);
    }
  }

  void RightToolbox::onEmit(const gui_events::TabRemoveEvent& event)
  {
    if(!event.numberOfTabs) {
      getPreviewManager().deregisterPreview(*m_preview->getPreviewView());
      m_preview->setVisible(false);
      this->setEnabled(false);
    }
  }

  void RightToolbox::onEmit(const gui_events::FrameChangeEvent&)
  {
    if(!m_preview->isVisible()) {
      getPreviewManager().registerPreview(*m_preview->getPreviewView());
      m_preview->setVisible(true);
    }
    m_preview->getPreviewView()->setImage(getImageManager().getImage());
    m_preview->repaint();
  }

  void RightToolbox::onEmit(const gui_events::AnimationPlayEvent&)
  {
    this->setEnabled(false);
  }

  void RightToolbox::onEmit(const gui_events::AnimationPauseEvent&)
  {
    this->setEnabled(true);
  }

  void RightToolbox::onEmit(const gui_events::AnimationStopEvent&)
  {
    this->setEnabled(true);
  }
}
