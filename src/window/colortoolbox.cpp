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
#include <QHBoxLayout>
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
#include "../debug_log.h"
#include "../gui_define.h"
#include "colorbutton.h"
#include "imageeditorview.h"

namespace pixpaint
{
namespace
{
  void onColorClick()
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

  ColorToolbox::ColorToolbox(QWidget* parent, const std::vector<Color>& colors) :
    QWidget(parent)
  {
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    auto* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft);
    layout->addSpacerItem(new QSpacerItem(25, 1));

    m_colorSelectionWidget = static_cast<ColorSelectionWidget*>(createColorSelectionWidget());
    layout->addWidget(m_colorSelectionWidget);
    layout->addWidget(createColorGridWidget(colors));

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

  QWidget* ColorToolbox::createColorGridWidget(const std::vector<Color>& colors)
  {
    auto* surface = new QWidget(this);
    auto* layout = new QGridLayout(surface);

    int x = 0, y = 0;
    for(const auto& color : colors) {
      auto* colorBtn = new ColorButton(surface, color, true);
      colorBtn->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
      colorBtn->setFixedSize(COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT);

      connect(colorBtn, &ColorButton::clicked, [colorBtn] {
        getColorManager().setForegroundColor(colorBtn->getBackgroundColor());
        onColorClick();
      });

      connect(colorBtn, &ColorButton::customContextMenuRequested, [colorBtn] {
        getColorManager().setBackgroundColor(colorBtn->getBackgroundColor());
        onColorClick();
      });

      connect(colorBtn, &ColorButton::doubleClicked, [colorBtn]{
        auto color = QColorDialog::getColor(qt_utils::convertToQTColor(colorBtn->getBackgroundColor()),
                                            nullptr,
                                            "Select Color",
                                            QColorDialog::ShowAlphaChannel |
                                            QColorDialog::DontUseNativeDialog);
        if(color.isValid()) {
          colorBtn->setBackgroundColor(qt_utils::convertToColor(color));
        }
      });

      layout->addWidget(colorBtn, y, x, 1, 1);
      if(x == (colors.size() / 2)) {
        ++y; x = 0;
      } else {
        ++x;
      }
    }

    surface->setLayout(layout);
    return surface;
  }
}
