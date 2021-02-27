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
#include "colortoolbox.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include "../3rdparty/gengine/configuration.h"
#include "../env/imageenvironment.h"
#include "../helper/tool_helpers.h"
#include "../image/image.h"
#include "../image/point.h"
#include "../manager/colormanager.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"
#include "../tool/painttoolbase.h"
#include "../utility/qt_utility.h"
#include "../colorpalette.h"
#include "../debug_log.h"
#include "../define.h"
#include "../gui_define.h"
#include "colorbutton.h"
#include "imageeditorview.h"

namespace pixpaint
{
namespace
{
  void on_color_click()
  {
    auto& imageEnv = getImageEnvironment();
    if(imageEnv.isViewSet()) {
      auto& view = imageEnv.getView();
      auto& currentPaintTool = getPaintToolManager().getCurrentTool();

      if(getTextManager().tryUpdateText()) {
        auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(getTextSelectionManager().getSelectionRect()) *
                                                          view.getPixelSize());
        view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
      }

      tool_helpers::onToolChange(currentPaintTool, &PaintToolHandlerBase::onColorChange);
    }
  }
}
  ColorToolbox::ColorSelectionWidget::ColorSelectionWidget(QWidget* parent) :
    QWidget(parent)
  {
    this->setFixedWidth(100);

    m_backgroundColorButton = new ColorButton(this, Color::WHITE, true);
    connect(m_backgroundColorButton, &ColorButton::doubleClicked, [backgroundColorButton = m_backgroundColorButton]{
      auto& colorManager = getColorManager();
      auto qcolor = QColorDialog::getColor(qt_utils::convertToQTColor(backgroundColorButton->getBackgroundColor()),
                                           nullptr,
                                           "Select Color",
                                           QColorDialog::ShowAlphaChannel |
                                           QColorDialog::DontUseNativeDialog);
      if(qcolor.isValid()) {
        auto color = qt_utils::convertToColor(qcolor);
        colorManager.setBackgroundColor(color);
      }
    });

    m_foregroundColorButton = new ColorButton(this, Color::BLACK, true);
    connect(m_foregroundColorButton, &ColorButton::doubleClicked, [foregroundColorButton = m_foregroundColorButton]{
      auto& colorManager = getColorManager();
      auto qcolor = QColorDialog::getColor(qt_utils::convertToQTColor(foregroundColorButton->getBackgroundColor()),
                                           nullptr,
                                           "Select Color",
                                           QColorDialog::ShowAlphaChannel |
                                           QColorDialog::DontUseNativeDialog);
      if(qcolor.isValid()) {
        auto color = qt_utils::convertToColor(qcolor);
        colorManager.setForegroundColor(color);
      }
    });
  }

  void ColorToolbox::ColorSelectionWidget::setSelectedForegroundColor(const Color& color)
  {
    m_foregroundColorButton->setBackgroundColor(color);
    m_foregroundColorButton->repaint();
  }

  void ColorToolbox::ColorSelectionWidget::setSelectedBackgroundColor(const Color& color)
  {
    m_backgroundColorButton->setBackgroundColor(color);
    m_backgroundColorButton->repaint();
  }

  void ColorToolbox::ColorSelectionWidget::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);
    qt_utils::drawTransparentBackground(painter,
                                        0,
                                        0,
                                        this->geometry().width(),
                                        this->geometry().height(),
                                        4);

    painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    painter.setPen(QPen(QColor(0, 0, 0, 255), 1));
    painter.drawRect(0, 0, this->geometry().width() - 1, this->geometry().height() - 1);
  }

  void ColorToolbox::ColorSelectionWidget::resizeEvent(QResizeEvent* event)
  {
    const auto color_selection_widget_width = COLOR_BUTTON_WIDTH + 6;
    const auto color_selection_widget_height = COLOR_BUTTON_HEIGHT + 6;

    const auto cumulative_width = color_selection_widget_width + (color_selection_widget_width / 2);
    const auto cumulative_height = color_selection_widget_height + (color_selection_widget_height / 2);

    const auto spacer_x = (geometry().width() - cumulative_width) / 2;
    const auto spacer_y = (geometry().height() - cumulative_height) / 2;

    const auto background_color_x = geometry().width() - spacer_x - color_selection_widget_width;
    const auto background_color_y = geometry().height() - spacer_y - color_selection_widget_height;
    const auto foreground_color_x = spacer_x;
    const auto foreground_color_y = spacer_y;

    m_backgroundColorButton->setGeometry(background_color_x,
                                         background_color_y,
                                         color_selection_widget_width,
                                         color_selection_widget_height);

    m_foregroundColorButton->setGeometry(foreground_color_x,
                                         foreground_color_y,
                                         color_selection_widget_width,
                                         color_selection_widget_height);

    QWidget::resizeEvent(event);
  }

  ColorToolbox::ColorToolbox(QWidget* parent) :
    QWidget(parent)
  {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);
    layout->addSpacerItem(new QSpacerItem(25, 1));

    m_colorSelectionWidget = static_cast<ColorSelectionWidget*>(createColorSelectionWidget());
    layout->addWidget(m_colorSelectionWidget);
    layout->addWidget(createColorGridWidget());

    auto* btn_layout = new QVBoxLayout(this);
    auto* save_btn = new QPushButton("Save Palette", this);
    auto* load_btn = new QPushButton("Load Palette", this);

    this->connect(save_btn, &QPushButton::clicked, [this]() {
      auto filename = QFileDialog::getSaveFileName(this,
                                                   tr("Save Palette..."),
                                                   gengine2d::getConfigurationManager().getString(CONFIG_SECTION_SETTINGS,
                                                                                                  "last_location")->c_str(),
                                                   "PixPaint Palette (*.ppa)");
      if(filename.size()) {
        auto filename_s = std::string(filename.toUtf8().constData());
        auto filename_p = os_specific::filesystem::path(filename_s);
        if(filename_p.extension().string().empty() ||
           filename_p.extension().string() != "ppa") {
          filename_s += ".ppa";
        }

        if(!getColorPalette().save(filename_s)) {
          QMessageBox::critical(this, tr("Error"), tr("There was an error saving the color palette!"));
        }
      }
    });

    this->connect(load_btn, &QPushButton::clicked, [this]() {
      auto filename = QFileDialog::getOpenFileName(this,
                                                   tr("Load Palette..."),
                                                   gengine2d::getConfigurationManager().getString(CONFIG_SECTION_SETTINGS,
                                                                                                  "last_location")->c_str(),
                                                   "PixPaint Palette (*.ppa)");
      if(filename.size()) {
        if(!getColorPalette().load(filename.toUtf8().constData())) {
          QMessageBox::critical(this, tr("Error"), tr("There was an error saving the color palette!"));
          return;
        }

        clearColorGrid();
        updateColorGrid();
      }
    });

    btn_layout->addWidget(save_btn);
    btn_layout->addWidget(load_btn);
    layout->addLayout(btn_layout);

    this->setLayout(layout);
  }

  void ColorToolbox::setSelectedForegroundColor(const Color& color)
  {
    m_colorSelectionWidget->setSelectedForegroundColor(color);
  }

  void ColorToolbox::setSelectedBackgroundColor(const Color& color)
  {
    m_colorSelectionWidget->setSelectedBackgroundColor(color);
  }

  QWidget* ColorToolbox::createColorSelectionWidget()
  {
    return new ColorSelectionWidget(this);
  }

  QWidget* ColorToolbox::createColorGridWidget()
  {
    m_colorGridSurface = new QWidget(this);
    m_colorGrid = new QGridLayout(m_colorGridSurface);

    updateColorGrid();

    m_colorGridSurface->setLayout(m_colorGrid);
    return m_colorGridSurface;
  }

  void ColorToolbox::clearColorGrid()
  {
    while(m_colorGrid->itemAt(0)) {
      delete m_colorGrid->itemAt(0)->widget();
    }
  }

  void ColorToolbox::updateColorGrid()
  {
    int x = 0, y = 0, i = 0;
    for(const auto& color : getColorPalette()) {
      auto* color_btn = new ColorButton(m_colorGridSurface, color, true);
      color_btn->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
      color_btn->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);

      this->connect(color_btn, &ColorButton::clicked, [color_btn] {
        getColorManager().setForegroundColor(color_btn->getBackgroundColor());
        on_color_click();
      });

      this->connect(color_btn, &ColorButton::customContextMenuRequested, [color_btn] {
        getColorManager().setBackgroundColor(color_btn->getBackgroundColor());
        on_color_click();
      });

      this->connect(color_btn, &ColorButton::middleClicked, [this, color_btn, i] {
        auto btn = QMessageBox::question(this,
                                         "Remove Color",
                                         "Do you want to remove this color?",
                                         QMessageBox::StandardButton::Yes,
                                         QMessageBox::StandardButton::No);
        if(btn == QMessageBox::Yes) {
          getColorPalette().removeColor(i);

          clearColorGrid();
          updateColorGrid();
        }
      });

      this->connect(color_btn, &ColorButton::doubleClicked, [color_btn, i]{
        // Must have atleast one color
        if(getColorPalette().size() > 2) {
          auto color = QColorDialog::getColor(qt_utils::convertToQTColor(color_btn->getBackgroundColor()),
                                              nullptr,
                                              "Select Color",
                                              QColorDialog::ShowAlphaChannel |
                                              QColorDialog::DontUseNativeDialog);
          if(color.isValid()) {
            color_btn->setBackgroundColor(qt_utils::convertToColor(color));
            getColorPalette().setColor(i, qt_utils::convertToColor(color));
          }
        }
      });

      m_colorGrid->addWidget(color_btn, y, x, 1, 1);
      if(y == 1) {
        y = 0; ++x;
      } else {
        ++y;
      }
      ++i;
    }

    auto* add_btn = new QPushButton(tr("+"), this);
    add_btn->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);
    this->connect(add_btn, &QPushButton::clicked, [this]() {
      getColorPalette().addColor(Color::BLACK);

      clearColorGrid();
      updateColorGrid();
    });

    if(getColorPalette().size() < ColorPalette::MAX_COLOR_COUNT - 1) {
      add_btn->setEnabled(true);
    } else {
      add_btn->setEnabled(false);
    }

    m_colorGrid->addWidget(add_btn, y, x, 1, 1);
  }
}
