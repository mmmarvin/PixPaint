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
#include "imageview.h"

#include <QPainter>
#include "../image/image.h"
#include "../helper/paint_helpers.h"
#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"
#include "../assert.h"

namespace pixpaint
{
  ImageView::ImageView(QWidget* parent,
                       Image& image,
                       double pixelSize) :
    BaseImageView(parent, image, pixelSize, pixelSize),
    m_boxGridDimension(64, 64),
    m_showBoxGrid(false),
    m_showGrid(true)
  {
  }

  void ImageView::showGrid(bool show)
  {
    m_showGrid = show;
    this->repaint(qt_utils::convertToQTRect(castTo<position_t>(getDrawableRegion())));
  }

  bool ImageView::isGridShown() const noexcept
  {
    return m_showGrid;
  }

  void ImageView::showBoxGrid(bool show, dimension_t width, dimension_t height)
  {
    m_showBoxGrid = show;
    if(show) {
      m_boxGridDimension.width = width;
      m_boxGridDimension.height = height;
    }
  }

  bool ImageView::isBoxGridShown() const noexcept
  {
    return m_showBoxGrid;
  }

  const Size& ImageView::getBoxGridSize() const noexcept
  {
    return m_boxGridDimension;
  }

  void ImageView::setPixelSize(double pixelSize)
  {
    BaseImageView::setPixelSize(pixelSize, pixelSize);
  }

  double ImageView::getPixelSize() const noexcept
  {
    return BaseImageView::getPixelSizeX();
  }

  void ImageView::paintEvent(QPaintEvent* event)
  {
    BaseImageView::paintEvent(event);

    QPainter painter(this);
    painter.setWorldTransform(QTransform().scale(this->getPixelSize(), this->getPixelSize()));

    if(m_showGrid && this->getPixelSize() >= 3) {
      paintGrid();
    }

    if(m_showBoxGrid) {
      paintBoxGrid();
    }
  }

  DoubleRect ImageView::getDrawableRegion() const
  {
    return paint_helpers::paint_helpers_detail::getDrawableRegion(getImage(),
                                                                  Point(this->geometry().width(), this->geometry().height()),
                                                                  this->getPixelSize());
  }

  DoubleRect ImageView::getSmallestDrawableRegion(const DoubleRect& option)
  {
    return paint_helpers::paint_helpers_detail::getSmallestDrawableRegion(getImage(),
                                                                          option,
                                                                          Point(this->geometry().width(), this->geometry().height()),
                                                                          this->getPixelSize());
  }

  void ImageView::paintGrid()
  {
    QPainter painter(this);

    QPen pen(QColor(128, 128, 128, 255));
    pen.setWidth(1);
    painter.setPen(pen);

    const auto pixel_size = this->getPixelSize();
    const auto adjusted_image_width = this->getImage().getWidth() * pixel_size;
    const auto adjusted_image_height = this->getImage().getHeight() * pixel_size;

    for(position_t y = 0; y <= adjusted_image_height; y += pixel_size) {
      painter.drawLine(QPoint(0, y), QPoint(adjusted_image_width, y));
    }
    for(position_t x = 0; x <= adjusted_image_width; x += pixel_size) {
      painter.drawLine(QPoint(x, 0), QPoint(x, adjusted_image_height));
    }
  }

  void ImageView::paintBoxGrid()
  {
    QPainter painter(this);

    QPen pen(QColor(64, 64, 64, 255));
    pen.setWidth(3);
    painter.setPen(pen);

    const auto pixel_size = this->getPixelSize();
    const auto adjusted_image_width = getImage().getWidth() * pixel_size;
    const auto adjusted_image_height = getImage().getHeight() * pixel_size;
    const auto w = m_boxGridDimension.width * pixel_size;
    const auto h = m_boxGridDimension.height * pixel_size;

    for(position_t y = 0; y <= adjusted_image_height; y += h) {
      painter.drawLine(QPoint(0, y), QPoint(adjusted_image_width, y));
    }
    for(position_t x = 0; x <= adjusted_image_width; x += w) {
      painter.drawLine(QPoint(x, 0), QPoint(x, adjusted_image_height));
    }
  }
}
