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
#include "newimagedialog.h"

#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include "../utility/general_utility.h"

namespace pixpaint
{
  NewImageDialog::NewImageDialog(QWidget* parent,
                                 std::size_t startingWidth,
                                 std::size_t startingHeight,
                                 bool showScaleOptions) :
    QDialog(parent),
    m_resizeMode(MaskablePixelData::EResizeMode::RESIZE)
  {
    this->setWindowTitle(tr("Create a New Image"));

    auto* layout = new QVBoxLayout(this);
    auto* control_box = new QGroupBox(this);
    control_box->setTitle("Image Size");

    auto* control_box_layout = new QVBoxLayout(control_box);
    control_box_layout->addLayout(createDimensionWidgets(control_box, "Width", startingWidth, &m_width));
    control_box_layout->addLayout(createDimensionWidgets(control_box, "Height", startingHeight, &m_height));
    control_box->setLayout(control_box_layout);
    layout->addWidget(control_box);

    if(showScaleOptions) {
      this->setMinimumWidth(500);

      auto* resize_option_box = new QGroupBox(this);
      resize_option_box->setTitle(tr("Scale Option"));

      auto* resize_option_box_layout = new QHBoxLayout(resize_option_box);
      resize_option_box_layout->addLayout(createScaleOptionWidgets(resize_option_box));

      layout->addWidget(resize_option_box);
    }

    layout->addLayout(createButtons(showScaleOptions));

    this->setLayout(layout);
  }

  std::size_t NewImageDialog::getWidth() const noexcept
  {
    return m_width;
  }

  std::size_t NewImageDialog::getHeight() const noexcept
  {
    return m_height;
  }

  MaskablePixelData::EResizeMode NewImageDialog::getResizeMode() const noexcept
  {
    return m_resizeMode;
  }

  QLayout* NewImageDialog::createDimensionWidgets(QWidget* parent,
                                                  const std::string& dimension,
                                                  std::size_t initialValue,
                                                  std::size_t* value)
  {
    auto* layout = new QHBoxLayout;
    auto* dimension_text = new QLabel((dimension + ": ").c_str(), parent);
    auto* dimension_box = new QSpinBox(this);
    dimension_box->setMinimum(PixelData::MINIMUM_DIMENSION);
    dimension_box->setMaximum(PixelData::MAXIMUM_DIMENSION);
    dimension_box->setValue(general_utils::clamp<std::size_t>(initialValue, PixelData::MINIMUM_DIMENSION, PixelData::MAXIMUM_DIMENSION));
    *value = general_utils::clamp<std::size_t>(initialValue, PixelData::MINIMUM_DIMENSION, PixelData::MAXIMUM_DIMENSION);
    QObject::connect(dimension_box, QOverload<int>::of(&QSpinBox::valueChanged), [value](int v) {
      *value = v;
    });

    layout->addWidget(dimension_text, 1);
    layout->addWidget(dimension_box, 9);

    return layout;
  }

  QLayout* NewImageDialog::createScaleOptionWidgets(QWidget* parent)
  {
    auto* layout = new QHBoxLayout;
    auto* scale_option1 = new QRadioButton(parent);
    scale_option1->setText(tr("Resize"));
    auto* scale_option2 = new QRadioButton(parent);
    scale_option2->setText(tr("Scale"));
    auto* scale_option3 = new QRadioButton(parent);
    scale_option3->setText(tr("Anti-Aliased Scale"));

    auto* scale_option_image1 = new QLabel(parent);
    scale_option_image1->setPixmap(QPixmap(tr("res/option_resize.png")));
    auto* scale_option_image2 = new QLabel(parent);
    scale_option_image2->setPixmap(QPixmap(tr("res/option_scale.png")));
    auto* scale_option_image3 = new QLabel(parent);
    scale_option_image3->setPixmap(QPixmap(tr("res/option_ascale.png")));

    QObject::connect(scale_option1, &QRadioButton::toggled, [this](bool){
      this->m_resizeMode = MaskablePixelData::EResizeMode::RESIZE;
    });
    QObject::connect(scale_option2, &QRadioButton::toggled, [this](bool){
      this->m_resizeMode = MaskablePixelData::EResizeMode::SCALE;
    });
    QObject::connect(scale_option3, &QRadioButton::toggled, [this](bool){
      this->m_resizeMode = MaskablePixelData::EResizeMode::SMOOTH_SCALE;
    });
    scale_option1->setChecked(true);

    auto* layout1 = new QVBoxLayout;
    layout1->addWidget(scale_option_image1, 1, Qt::AlignCenter);
    layout1->addWidget(scale_option1, 1, Qt::AlignCenter);

    auto* layout2 = new QVBoxLayout;
    layout2->addWidget(scale_option_image2, 1, Qt::AlignCenter);
    layout2->addWidget(scale_option2, 1, Qt::AlignCenter);

    auto* layout3 = new QVBoxLayout;
    layout3->addWidget(scale_option_image3, 1, Qt::AlignCenter);
    layout3->addWidget(scale_option3, 1, Qt::AlignCenter);

    layout->addLayout(layout1);
    layout->addLayout(layout2);
    layout->addLayout(layout3);

    return layout;
  }

  QLayout* NewImageDialog::createButtons(bool showScaleOptions)
  {
    auto* layout = new QHBoxLayout;
    auto* ok_btn = new QPushButton(tr("Ok"), this);
    auto* cancel_btn = new QPushButton(tr("Cancel"), this);

    QObject::connect(ok_btn, &QPushButton::clicked, [this](){
      auto tot_num_pixels = static_cast<std::int64_t>(getWidth()) * static_cast<std::int64_t>(getHeight());
      if(tot_num_pixels > 12'000'000) {
        auto res = QMessageBox::warning(nullptr,
                                        tr("Warning"),
                                        (std::string("An image that is ") +
                                        std::to_string(getWidth()) +
                                        std::string("x") +
                                        std::to_string(getHeight()) +
                                        std::string(" will take substancial amount of memory, and might reduce the overall responsiveness of the whole system. Continue?")).c_str(),
                                        QMessageBox::StandardButton::Yes,
                                        QMessageBox::StandardButton::Cancel);
        if(res == QMessageBox::StandardButton::Yes) {
          this->accept();
        }
      } else {
        this->accept();
      }
    });
    QObject::connect(cancel_btn, &QPushButton::clicked, [this](){
      this->reject();
    });

    if(showScaleOptions) {
      layout->addSpacerItem(new QSpacerItem(100, 0));
    } else {
      layout->addSpacerItem(new QSpacerItem(50, 0));
    }
    layout->addWidget(ok_btn);
    layout->addWidget(cancel_btn);

    return layout;
  }
}
