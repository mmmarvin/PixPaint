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
    QWidget(parent),
    m_image(&image),
    m_pixelSize(pixelSize),
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

  void ImageView::setPixelSize(double pixelSize)
  {
    m_pixelSize = pixelSize;
  }

  double ImageView::getPixelSize() const noexcept
  {
    return m_pixelSize;
  }

  void ImageView::setImage(Image& image) noexcept
  {
    m_image = &image;
  }

  Image& ImageView::getImage() noexcept
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");
    return *m_image;
  }

  const Image& ImageView::getImage() const noexcept
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");
    return *m_image;
  }

  void ImageView::paintEvent(QPaintEvent*)
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");

    QPainter painter(this);
    painter.setWorldTransform(QTransform().scale(m_pixelSize, m_pixelSize));

//    PIXPAINT_PROFILER_START_TIME("ImageView paintEvent", general_utils::pointerToInt(this));

    paintBackground(painter);
    this->paintImage(painter);
    if(m_showGrid && m_pixelSize >= 3) {
      paintGrid(painter);
    }

//    PIXPAINT_PROFILER_END_TIME("ImageView paintEvent", general_utils::pointerToInt(this));
  }

  DoubleRect ImageView::getDrawableRegion() const
  {
    return paint_helpers::paint_helpers_detail::getDrawableRegion(*m_image,
                                                                  Point(this->geometry().width(), this->geometry().height()),
                                                                  m_pixelSize);
  }

  DoubleRect ImageView::getSmallestDrawableRegion(const DoubleRect& option)
  {
    return paint_helpers::paint_helpers_detail::getSmallestDrawableRegion(*m_image,
                                                                          option,
                                                                          Point(this->geometry().width(), this->geometry().height()),
                                                                          m_pixelSize);
  }

  void ImageView::paintBackground(QPainter& painter)
  {
    qt_utils::drawTransparentBackground(painter,
                                        0,
                                        0,
                                        m_image->getWidth(),
                                        m_image->getHeight(),
                                        10);
  }

  void ImageView::paintImage(QPainter& painter)
  {
    for(std::size_t i = 0, isize = m_image->getLayerCount(); i < isize; ++i) {
      if(m_image->isVisible(i)) {
        auto qimage = qt_utils::createQImage(m_image->getLayer(i));
        painter.drawImage(0, 0, qimage);
      }
    }
  }

  void ImageView::paintGrid(QPainter&)
  {
    // TODO: find more efficient way of drawing grid?
    QPainter painter(this);

    QPen pen(QColor(128, 128, 128, 255));
    pen.setWidth(1);
    painter.setPen(pen);

    auto adjusted_image_width = m_image->getWidth() * m_pixelSize;
    auto adjusted_image_height = m_image->getHeight() * m_pixelSize;

    for(position_t y = 0; y <= adjusted_image_height; y += m_pixelSize) {
      painter.drawLine(QPoint(0, y), QPoint(adjusted_image_width, y));
    }
    for(position_t x = 0; x <= adjusted_image_width; x += m_pixelSize) {
      painter.drawLine(QPoint(x, 0), QPoint(x, adjusted_image_height));
    }
  }
}
