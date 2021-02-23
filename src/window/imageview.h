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
#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QWidget>
#include "../image/rect.h"

namespace pixpaint
{
  class Image;
  class ImageView : public QWidget
  {
  public:
    ImageView(QWidget* parent,
              Image& image,
              double pixelSize);

    void showGrid(bool show);
    bool isGridShown() const noexcept;

    void setPixelSize(double pixelSize);
    double getPixelSize() const noexcept;

    void setImage(Image& image) noexcept;
    Image& getImage() noexcept;
    const Image& getImage() const noexcept;

  protected:
    void paintEvent(QPaintEvent*) override;

    DoubleRect getDrawableRegion() const;

  public:
    DoubleRect getSmallestDrawableRegion(const DoubleRect& option);

  protected:
    void paintBackground(QPainter& painter);
    virtual void paintImage(QPainter& painter);
    void paintGrid(QPainter& painter);

  private:
    Image*      m_image;
    double      m_pixelSize;
    bool        m_showGrid;
  };
}

#endif // IMAGECANVAS_H
