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
#include "../image/image.h"
#include "../image/pixeldata.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "layerwidget.h"
#include "previewview.h"
#include "previewwidget.h"

namespace pixpaint
{
namespace
{
  static const std::vector<std::pair<std::string, PixelData::EBlendMode>> STRING_TO_BLENDMODE =
  {
    { "Normal", PixelData::EBlendMode::NORMAL },
    { "Multiply", PixelData::EBlendMode::MULTIPLY },
    { "Screen", PixelData::EBlendMode::SCREEN },
    { "Overlay", PixelData::EBlendMode::OVERLAY },
  //      { "Hard Light", PixelData::EBlendMode::HARDLIGHT },
  //      { "Soft Light", PixelData::EBlendMode::SOFTLIGHT },
    { "Addition", PixelData::EBlendMode::ADDITION },
    { "Difference", PixelData::EBlendMode::DIFFERENCE },
    { "Darken", PixelData::EBlendMode::DARKEN },
    { "Lighten", PixelData::EBlendMode::LIGHTEN },
    { "Color Dodget", PixelData::EBlendMode::COLOR_DODGE },
    { "Color Burn", PixelData::EBlendMode::COLOR_BURN },

//    { "Source In", PixelData::EBlendMode::SRC_IN },
//    { "Destination In", PixelData::EBlendMode::DST_IN },
//    { "Source Out", PixelData::EBlendMode::SRC_OUT },
//    { "Destination Out", PixelData::EBlendMode::DST_OUT }
  };

  static const std::unordered_map<std::string, PixelData::EBlendMode> STRING_TO_BLENDMODE_LOOKUP =
  []() {
    std::unordered_map<std::string, PixelData::EBlendMode> ret;
    for(const auto& p : STRING_TO_BLENDMODE) {
      ret.emplace(p.first, p.second);
    }

    return ret;
  } ();

  static const std::unordered_map<PixelData::EBlendMode, std::string> BLENDMODE_TO_STRING_LOOKUP =
      []() {
    std::unordered_map<PixelData::EBlendMode, std::string> ret;
    for(const auto& p : STRING_TO_BLENDMODE) {
      ret.emplace(p.second, p.first);
    }

    return ret;
  } ();
}
  RightToolbox::RightToolbox(QWidget* parent) :
    QWidget(parent)
  {
    auto* preview_group = createPreviewGroup();
    auto* layer_group = createLayerGroup();

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(preview_group, 1);
    layout->addWidget(layer_group, 9);

    this->setMinimumWidth(250);
    this->setLayout(layout);
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

    auto blend_mode = getImageManager().getImage().getCurrentLayer().getBlendMode();
    m_blendCombo->setCurrentText(BLENDMODE_TO_STRING_LOOKUP.at(blend_mode).c_str());
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

  QGroupBox* RightToolbox::createPreviewGroup()
  {
    auto& preview_manager = getPreviewManager();
    auto& image_manager = getImageManager();

    auto* preview_group = new QGroupBox(this);
    preview_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    preview_group->setTitle(tr("Preview"));
    preview_group->setMinimumHeight(200);
    preview_group->setFlat(true);
    m_preview = new PreviewWidget(preview_group, image_manager.getImage());
    preview_manager.registerPreview(*m_preview->getPreviewView());

    auto* preview_layout = new QHBoxLayout(preview_group);
    preview_layout->addWidget(m_preview);

    return preview_group;
  }

  QGroupBox* RightToolbox::createLayerGroup()
  {
    auto* layer_group = new QGroupBox(this);
    layer_group->setTitle(tr("Layers"));
    layer_group->setFlat(true);

    auto* blend_layout = new QHBoxLayout;
    auto* blend_mode_label = new QLabel("Mode:", layer_group);
    m_blendCombo = new QComboBox(layer_group);

    for(const auto& p : STRING_TO_BLENDMODE) {
      m_blendCombo->addItem(p.first.c_str());
      connect(m_blendCombo, &QComboBox::currentIndexChanged, [this](int index) {
        auto blend_mode_str = std::string(m_blendCombo->itemText(index).toUtf8().constData());
        auto blend_mode = STRING_TO_BLENDMODE_LOOKUP.at(blend_mode_str);
        getImageManager().getImage().getCurrentLayer().setBlendMode(blend_mode);
        getImageManager().refresh();
      });
    }
    blend_layout->addWidget(blend_mode_label, 1);
    blend_layout->addWidget(m_blendCombo, 10);

    m_layer = new LayerWidget(layer_group);

    auto* layer_layout = new QVBoxLayout(layer_group);
    layer_layout->addLayout(blend_layout);
    layer_layout->addWidget(m_layer);

    return layer_group;
  }
}
