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
        auto qimage = qt_utils::createQImage(m_image->getLayer(i));
        painter.drawImage(0, 0, qimage);
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
}
