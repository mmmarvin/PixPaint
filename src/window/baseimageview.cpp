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
#include "baseimageview.h"

#include <QPainter>
#include "../image/image.h"
#include "../utility/qt_utility.h"
#include "../assert.h"

namespace pixpaint
{
  BaseImageView::BaseImageView(QWidget* parent,
                               Image& image,
                               double pixelSizeX,
                               double pixelSizeY) :
    QWidget(parent),
    m_image(&image),
    m_pixelSizeX(pixelSizeX),
    m_pixelSizeY(pixelSizeY)
  {
  }

  void BaseImageView::setPixelSize(double pixelSizeX, double pixelSizeY)
  {
    m_pixelSizeX = pixelSizeX;
    m_pixelSizeY = pixelSizeY;
  }

  double BaseImageView::getPixelSizeX() const noexcept
  {
    return m_pixelSizeX;
  }

  double BaseImageView::getPixelSizeY() const noexcept
  {
    return m_pixelSizeY;
  }

  void BaseImageView::setImage(Image& image) noexcept
  {
    m_image = &image;
  }

  Image& BaseImageView::getImage() noexcept
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");
    return *m_image;
  }

  const Image& BaseImageView::getImage() const noexcept
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");
    return *m_image;
  }

  void BaseImageView::paintEvent(QPaintEvent*)
  {
    PIXPAINT_ASSERT(m_image, "Image was not initialzied");

    QPainter painter(this);
    painter.setWorldTransform(QTransform().scale(m_pixelSizeX, m_pixelSizeY));

    paintBackground(painter);
    this->paintImage(painter);
  }

  void BaseImageView::paintImage(QPainter& painter)
  {
    for(std::size_t i = 0, isize = m_image->getLayerCount(); i < isize; ++i) {
      if(m_image->isVisible(i)) {
        paintLayer(painter, m_image->getLayer(i));
      }
    }
  }

  void BaseImageView::paintBackground(QPainter& painter)
  {
    qt_utils::drawTransparentBackground(painter,
                                        0,
                                        0,
                                        m_image->getWidth(),
                                        m_image->getHeight(),
                                        10);
  }

  void BaseImageView::paintLayer(QPainter& painter, const PixelData& layer)
  {
    paintLayer(painter, 0, 0, layer);
  }

  void BaseImageView::paintLayer(QPainter& painter, int x, int y, const PixelData& layer)
  {
    painter.setOpacity(static_cast<float>(layer.getOpacity()) / 100.f);

    auto qimage = qt_utils::createQImage(layer);
    painter.drawImage(x, y, qimage);

    painter.setOpacity(1.f);
  }
}
