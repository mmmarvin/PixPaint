#ifndef BASEIMAGEVIEW_H
#define BASEIMAGEVIEW_H

#include <QWidget>

namespace pixpaint
{
  class Image;

  class BaseImageView : public QWidget
  {
  public:
    BaseImageView(QWidget* parent,
                  Image& image,
                  double pixelSizeX,
                  double pixelSizeY);

    void setPixelSize(double pixelSizeX, double pixelSizeY);
    double getPixelSizeX() const noexcept;
    double getPixelSizeY() const noexcept;

    void setImage(Image& image) noexcept;
    Image& getImage() noexcept;
    const Image& getImage() const noexcept;

  protected:
    void paintEvent(QPaintEvent*) override;
    virtual void paintImage(QPainter& painter);
    void paintBackground(QPainter& painter);

  private:
    Image* m_image;
    double m_pixelSizeX;
    double m_pixelSizeY;
  };
}

#endif // BASEIMAGEVIEW_H
