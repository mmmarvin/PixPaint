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
#include "effectoptiondialog.h"

#include <QComboBox>
#include <QLabel>
#include "../env/imageenvironment.h"
#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../registrar/information/effectinformation.h"
#include "../window/imageeditorview.h"
#include "../window/nonselectionpreviewview.h"
#include "../window/previewview.h"
#include "../debug_log.h"

namespace pixpaint
{
  EffectOptionDialog::EffectOptionDialog(QWidget* parent, EffectInformation& effectInformation) :
    OptionDialog(parent),
    m_previewImage(nullptr),
    m_effectInformation(&effectInformation),
    m_target(EEffectTarget::LAYER)
  {
    this->setWindowTitle(effectInformation.getName().c_str());

    auto& selection_manager = getSelectionManager();
    auto* layout = new QVBoxLayout(this);

    auto* target_layout = new QHBoxLayout;
    auto* target_label = new QLabel(tr("Target:"), this);
    auto* target_choice = new QComboBox(this);
    target_choice->addItem(tr("Layer"));
    target_choice->addItem(tr("Image"));
    if(selection_manager.selectionExists() && selection_manager.layerExists()) {
      target_choice->addItem(tr("Selection"));
      target_choice->setCurrentIndex(2);
      target_choice->setEnabled(false);
      m_target = EEffectTarget::SELECTION;
    }
    QObject::connect(target_choice,
                  QOverload<int>::of(&QComboBox::currentIndexChanged),
                  [this, target_choice](int index) {
      auto choice = std::string(target_choice->itemText(index).toUtf8().data());
      if(choice == "Image") {
        this->m_target = EEffectTarget::IMAGE;
      } else if(choice == "Layer") {
        this->m_target = EEffectTarget::LAYER;
      } else {
        this->m_target = EEffectTarget::SELECTION;
      }

      this->applyEffect();
      this->m_previewView->repaint();
    });
    target_layout->setAlignment(Qt::AlignCenter);
    target_layout->addWidget(target_label);
    target_layout->addWidget(target_choice);
    layout->addLayout(target_layout);

    if(effectInformation.showPreview()) {
      if(selection_manager.selectionExists() && selection_manager.layerExists()) {
        const auto& selection_layer = selection_manager.getSelectionLayer();
        m_previewImage = std::make_unique<Image>(selection_layer.getWidth(),
                                                 selection_layer.getHeight());
        m_previewImage->getLayer(0) = selection_layer;
        m_originalImage = std::make_unique<Image>(*m_previewImage);
        m_previewView = new NonSelectionPreviewView(this, *m_previewImage.get());
      } else {
        m_previewImage = std::make_unique<Image>(getImageManager().getImage());
        m_originalImage = std::make_unique<Image>(getImageManager().getImage());
        m_previewView = new PreviewView(this, *m_previewImage.get());
      }
      m_previewView->setFixedSize(300, 300);
      m_previewView->refreshResize();

      auto* image_layout = new QHBoxLayout;
      image_layout->addWidget(m_previewView);
      image_layout->setAlignment(Qt::AlignCenter);

      layout->addLayout(image_layout);
    }

    createOptionWidgets(layout,
                        effectInformation.getEffect().getOptions(),
    [this](int) { this->applyEffect(); },
    [this](int) { this->applyEffect(); },
    [this](const std::string&) { this->applyEffect(); },
    [this](const std::string&) { this->applyEffect(); });
  }

  EffectOptionDialog::EEffectTarget EffectOptionDialog::getTarget() const
  {
    return m_target;
  }

  void EffectOptionDialog::applyEffect()
  {
    if(m_previewImage) {
      *m_previewImage.get() = *m_originalImage.get();
      if(m_target == EEffectTarget::IMAGE || m_target == EEffectTarget::SELECTION) {
        auto width = m_previewImage->getWidth();
        auto height = m_previewImage->getHeight();
        for(std::size_t i = 0, isize = m_previewImage->getLayerCount(); i < isize; ++i) {
          m_effectInformation->getEffect().applyLayerEffect(IntRect(0, 0, width, height),
                                                            m_previewImage->getLayer(i));
        }
      } else if(m_target == EEffectTarget::LAYER) {
        auto current_layer_index = getImageManager().getImage().getCurrentLayerIndex();
        m_effectInformation->getEffect().applyLayerEffect(IntRect(0,
                                                                  0,
                                                                  m_previewImage->getWidth(),
                                                                  m_previewImage->getHeight()),
                                                                  m_previewImage->getLayer(current_layer_index));
      }
      m_previewView->repaint();
    }
  }
}
