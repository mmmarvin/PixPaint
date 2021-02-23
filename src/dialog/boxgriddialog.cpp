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
#include "boxgriddialog.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include "../env/imageenvironment.h"
#include "../image/pixeldata.h"
#include "../utility/general_utility.h"
#include "../window/imageeditorview.h"

namespace pixpaint
{
  BoxGridDialog::BoxGridDialog(QWidget* parent) :
    QDialog(parent)
  {
    this->setWindowTitle(tr("Box Grid"));
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* layout = new QVBoxLayout(this);
    auto* check_box = new QCheckBox(tr("Show box grid"), this);
    auto* control_box = new QGroupBox(tr("Dimension"), this);

    auto* control_box_layout = new QGridLayout(control_box);
    auto* width_label = new QLabel(tr("Width:"), this);
    auto* width_box = new QSpinBox(this);
    auto* height_label = new QLabel(tr("Height:"), this);
    auto* height_box = new QSpinBox(this);

    auto* button = new QPushButton(tr("OK"), this);

    width_box->setMinimum(PixelData::MINIMUM_DIMENSION);
    width_box->setMaximum(PixelData::MAXIMUM_DIMENSION);
    height_box->setMinimum(PixelData::MINIMUM_DIMENSION);
    height_box->setMaximum(PixelData::MAXIMUM_DIMENSION);

    QObject::connect(check_box, &QCheckBox::toggled, [this, control_box](bool checked) {
      m_grid = checked;
      control_box->setEnabled(m_grid);
    });
    QObject::connect(width_box, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
      m_width = v;
    });
    QObject::connect(height_box, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v) {
      m_height = v;
    });
    QObject::connect(button, &QPushButton::clicked, [this]() {
      this->accept();
    });

    const auto& img_env = getImageEnvironment();
    const auto& view = img_env.getView();

    width_box->setValue(general_utils::clamp<int>(view.getBoxGridSize().width,
                                                  PixelData::MINIMUM_DIMENSION,
                                                  PixelData::MAXIMUM_DIMENSION));
    height_box->setValue(general_utils::clamp<int>(view.getBoxGridSize().height,
                                                   PixelData::MINIMUM_DIMENSION,
                                                   PixelData::MAXIMUM_DIMENSION));
    if(view.isBoxGridShown()) {
      check_box->setChecked(true);
      m_grid = true;
      control_box->setEnabled(true);
    } else {
      check_box->setChecked(false);
      m_grid = false;
      control_box->setEnabled(false);
    }

    control_box_layout->addWidget(width_label, 0, 0, 1, 1);
    control_box_layout->addWidget(width_box, 0, 1, 1, 1);
    control_box_layout->addWidget(height_label, 1, 0, 1, 1);
    control_box_layout->addWidget(height_box, 1, 1, 1, 1);

    layout->addWidget(check_box, Qt::AlignLeft);
    layout->addWidget(control_box, Qt::AlignLeft);
    layout->addWidget(button, Qt::AlignCenter);
  }

  bool BoxGridDialog::showGrid() const
  {
    return m_grid;
  }

  int BoxGridDialog::getWidth() const
  {
    return m_width;
  }

  int BoxGridDialog::getHeight() const
  {
    return m_height;
  }
}
