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
#include "aboutdialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include "../config.h"

namespace pixpaint
{
  AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent)
  {
    this->setWindowTitle(tr("About"));

    auto* layout = new QVBoxLayout(this);

    auto* title_layout = new QHBoxLayout;
    title_layout->setAlignment(Qt::AlignCenter);
    auto* title_icon = new QLabel(this);
    title_icon->setPixmap(QPixmap("res/pixpaint.png"));
    auto* title_label = new QLabel(this);
    auto title_label_font = title_label->font();
    title_label_font.setPixelSize(16);
    title_label_font.setPointSize(16);
    title_label->setFont(title_label_font);
    title_label->setText((std::string(APP_TITLE) +
                         std::string(" v.") +
                         std::to_string(APP_VERSION_MAJOR) +
                         std::string(".") +
                         std::to_string(APP_VERSION_MINOR) +
                         std::to_string(APP_VERSION_PATCH) +
                         std::string("-") +
                         std::string(APP_RELEASE_TYPE) +
                         std::string(".") +
                         std::to_string(APP_VERSION_RELEASE)).c_str());

    title_layout->addWidget(title_icon);
    title_layout->addWidget(title_label);
    layout->addLayout(title_layout);

    auto* built_version_layout = new QHBoxLayout;
    built_version_layout->setAlignment(Qt::AlignCenter);
    auto* built_version_label = new QLabel(this);
    built_version_label->setText("Built on Qt " + QString(QT_VERSION_STR));
    built_version_layout->addWidget(built_version_label);

    auto* copyright_label1_layout = new QHBoxLayout;
    copyright_label1_layout->setAlignment(Qt::AlignCenter);
    auto* copyright_label1 = new QLabel(this);
    copyright_label1->setText("Copyright (c) 2020-2021 Marvin Manese.");
    copyright_label1_layout->addWidget(copyright_label1);

    auto* copyright_label2_layout = new QHBoxLayout;
    copyright_label2_layout->setAlignment(Qt::AlignCenter);
    auto* copyright_label2 = new QLabel(this);
    copyright_label2->setText("All rights reserved.");
    copyright_label2_layout->addWidget(copyright_label2);

    auto* copyright_label3_layout = new QHBoxLayout;
    copyright_label3_layout->setAlignment(Qt::AlignJustify);
    auto* copyright_label3 = new QLabel(this);
    copyright_label3->setText("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, \n"
                             "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n"
                             "MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.");
    copyright_label3_layout->addWidget(copyright_label3);

    layout->addLayout(built_version_layout);
    layout->addLayout(copyright_label1_layout);
    layout->addLayout(copyright_label2_layout);
    layout->addSpacerItem(new QSpacerItem(25, 25));
    layout->addLayout(copyright_label3_layout);

    this->setLayout(layout);
  }
}
