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
#ifndef QTDRAWER_H
#define QTDRAWER_H

#include <memory>
#include <QImage>
#include <QPainter>
#include "../drawer.h"

namespace pixpaint
{
  class QtDrawer : public Drawer
  {
  public:
    explicit QtDrawer(bool addCommands);

    void drawLine(const Point& p1,
                  const Point& p2,
                  const Color& color,
                  const DrawParam& param) override;
    void drawLineBlend(const Point& p1,
                       const Point& p2,
                       const Color& color,
                       const DrawParam& param) override;
    void drawCircleBlend(const Point& p1,
                         const Point& p2,
                         const Color& lineColor,
                         const Color& backgroundColor,
                         const DrawParam& param) override;
    void drawRectBlend(const Point& p1,
                       const Point& p2,
                       const Color& lineColor,
                       const Color& backgroundColor,
                       const DrawParam& param) override;
    void drawPath(const std::vector<Point>& points,
                  const Color& lineColor,
                  const DrawParam& param) override;
    void drawText(std::string text,
                  const std::string& fontFamily,
                  int fontSize,
                  bool antialiased,
                  const Point& dimension,
                  const Color& textColor,
                  const Color& backgroundColor) override;

    std::shared_ptr<Mask> createMaskFromLayer(const PixelData& layer) override;

  protected:
    void setTargetLayer(MaskablePixelData* layer) override;
    void setMask(const std::shared_ptr<Mask>& mask) override;
    void clearMask() override;

  private:
    void drawLineImpl(const Point& p1, const Point& p2);
    void drawCircleImpl(const Point& p1, const Point& dimension);
    void drawRectImpl(const Point& p1, const Point& dimension);
    void setAntialiasing(bool antialiased);
    void tryAddCommand(const Point& p1, const Point& p2, int size);

    std::unique_ptr<QImage>   m_image;
    std::unique_ptr<QPainter> m_painter;
  };
}

#endif // QTDRAWER_H
