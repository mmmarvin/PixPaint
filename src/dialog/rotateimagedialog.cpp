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
#include "rotateimagedialog.h"

#include <QDoubleValidator>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include "../utility/general_utility.h"

namespace pixpaint
{
  RotateImageDialog::RotateImageDialog(QWidget* parent) :
    QDialog(parent),
    m_rotation(0),
    m_direction(ERotationDirection::CLOCKWISE)
  {
    this->setWindowTitle(tr("Rotate Image"));

    auto* layout = new QVBoxLayout(this);
    auto* direction_control_box = new QGroupBox(this);
    direction_control_box->setTitle("Direction");

    auto* direction_control_box_layout = new QHBoxLayout(direction_control_box);
    direction_control_box_layout->addLayout(createDirectionWidgets(direction_control_box));
    direction_control_box->setLayout(direction_control_box_layout);
    layout->addWidget(direction_control_box);

    auto* angle_control_box = new QGroupBox(this);
    angle_control_box->setTitle("Angle");

    auto* angle_control_box_layout = new QHBoxLayout(angle_control_box);
    angle_control_box_layout->addLayout(createAngleWidgets(angle_control_box));
    angle_control_box->setLayout(angle_control_box_layout);
    layout->addWidget(angle_control_box);
    layout->addLayout(createButtons());

    this->setLayout(layout);
  }

  float RotateImageDialog::getRotation() const noexcept
  {
    return m_rotation;
  }

  ERotationDirection RotateImageDialog::getRotationDirection() const noexcept
  {
    return m_direction;
  }

  QLayout* RotateImageDialog::createDirectionWidgets(QWidget* parent)
  {
    auto* layout = new QHBoxLayout;

    auto* clockwise_option_image = new QLabel(parent);
    clockwise_option_image->setPixmap(QPixmap(tr("res/option_rotate_clockwise.png")));
    auto* counter_clockwise_option_image = new QLabel(parent);
    counter_clockwise_option_image->setPixmap(QPixmap(tr("res/option_rotate_counter_clockwise.png")));

    auto* clockwise_option_btn = new QRadioButton(parent);
    clockwise_option_btn->setText(tr("Clockwise"));
    clockwise_option_btn->setChecked(true);
    QObject::connect(clockwise_option_btn, &QRadioButton::toggled, [this]{
      m_direction = ERotationDirection::CLOCKWISE;
    });
    auto* counter_clockwise_option_btn = new QRadioButton(parent);
    counter_clockwise_option_btn->setText(tr("Counter-Clockwise"));
    QObject::connect(counter_clockwise_option_btn, &QRadioButton::toggled, [this]{
      m_direction = ERotationDirection::COUNTERCLOCKWISE;
    });

    auto* clockwise_layout = new QVBoxLayout;
    clockwise_layout->addWidget(clockwise_option_image, 1, Qt::AlignCenter);
    clockwise_layout->addWidget(clockwise_option_btn, 1, Qt::AlignCenter);

    auto* counter_clockwise_layout = new QVBoxLayout;
    counter_clockwise_layout->addWidget(counter_clockwise_option_image, 1, Qt::AlignCenter);
    counter_clockwise_layout->addWidget(counter_clockwise_option_btn, 1, Qt::AlignCenter);

    layout->addLayout(clockwise_layout);
    layout->addLayout(counter_clockwise_layout);

    return layout;
  }

  QLayout* RotateImageDialog::createAngleWidgets(QWidget* parent)
  {
    auto* layout = new QHBoxLayout;

    auto* angle_lbl = new QLabel(parent);
    angle_lbl->setText(tr("Angle:"));

    auto* angle_txt = new QLineEdit(parent);
    angle_txt->setValidator(new QDoubleValidator(0.0, 360.0, 1, this));
    angle_txt->setText("0.0");
    QObject::connect(angle_txt, &QLineEdit::textChanged, [this](const QString& s){
      m_rotation = general_utils::clamp(s.toDouble(), 0.0, 360.0);
    });

    layout->addWidget(angle_lbl);
    layout->addWidget(angle_txt);

    return layout;
  }

  QLayout* RotateImageDialog::createButtons()
  {
    auto* layout = new QHBoxLayout;
    auto* ok_btn = new QPushButton(tr("Ok"), this);
    auto* cancel_btn = new QPushButton(tr("Cancel"), this);

    QObject::connect(ok_btn, &QPushButton::clicked, [this](){
      this->accept();
    });
    QObject::connect(cancel_btn, &QPushButton::clicked, [this](){
      this->reject();
    });

    layout->addSpacerItem(new QSpacerItem(50, 0));
    layout->addWidget(ok_btn);
    layout->addWidget(cancel_btn);

    return layout;
  }
}
