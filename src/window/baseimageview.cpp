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

    switch(layer.getBlendMode()) {
    case PixelData::EBlendMode::NORMAL:
      painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
      break;
    case PixelData::EBlendMode::MULTIPLY:
      painter.setCompositionMode(QPainter::CompositionMode_Multiply);
      break;
    case PixelData::EBlendMode::SCREEN:
      painter.setCompositionMode(QPainter::CompositionMode_Screen);
      break;
    case PixelData::EBlendMode::OVERLAY:
      painter.setCompositionMode(QPainter::CompositionMode_Overlay);
      break;
    case PixelData::EBlendMode::ADDITION:
      painter.setCompositionMode(QPainter::CompositionMode_Plus);
      break;
    case PixelData::EBlendMode::DIFFERENCE:
      painter.setCompositionMode(QPainter::CompositionMode_Difference);
      break;
    case PixelData::EBlendMode::DARKEN:
      painter.setCompositionMode(QPainter::CompositionMode_Darken);
      break;
    case PixelData::EBlendMode::LIGHTEN:
      painter.setCompositionMode(QPainter::CompositionMode_Lighten);
      break;
    case PixelData::EBlendMode::COLOR_DODGE:
      painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
      break;
    case PixelData::EBlendMode::COLOR_BURN:
      painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
      break;
    default:
      break;
    }

    auto qimage = qt_utils::createQImage(layer);
    painter.drawImage(x, y, qimage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(1.f);
  }
}
