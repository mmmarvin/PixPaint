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
#include "qtdrawer.h"

#include <QBitmap>
#include <QPainterPath>
#include <QPixmap>
#include "../../assert.h"
#include "../../image/maskablepixeldata.h"
#include "../../utility/general_utility.h"
#include "../../utility/qt_utility.h"
#include "qtmask.h"

namespace pixpaint
{
  QtDrawer::QtDrawer(bool addCommands) :
    Drawer(addCommands)
  {
  }

  void QtDrawer::drawLine(const Point& p1,
                          const Point& p2,
                          const Color& color,
                          const DrawParam& param)
  {
    PIXPAINT_ASSERT(m_painter.get(), "Painter is not set");

    m_painter->setCompositionMode(QPainter::CompositionMode_Clear);
    drawLineBlend(p1, p2, color, param);

    m_painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    tryAddCommand(p1, p2, param.lineStyle.width);
    drawLineImpl(p1, p2);
  }

  void QtDrawer::drawLineBlend(const Point& p1,
                               const Point& p2,
                               const Color& color,
                               const DrawParam& param)
  {
    PIXPAINT_ASSERT(m_painter.get(), "Painter is not set");

    QPen pen(qt_utils::convertToQTColor(color));
    pen.setStyle(static_cast<Qt::PenStyle>(param.lineStyle.style));
    pen.setDashOffset(param.lineStyle.lineOffset);
    pen.setWidth(param.lineStyle.width);
    pen.setCapStyle(param.lineStyle.rounded ? Qt::RoundCap : Qt::SquareCap);
    m_painter->setPen(pen);

    setAntialiasing(param.antialiased);

    tryAddCommand(p1, p2, param.lineStyle.width);
    drawLineImpl(p1, p2);
  }

  void QtDrawer::drawCircleBlend(const Point& p1,
                                 const Point& p2,
                                 const Color& lineColor,
                                 const Color& backgroundColor,
                                 const DrawParam& param)
  {
    PIXPAINT_ASSERT(m_painter.get(), "Painter is not set");

    QPen pen(qt_utils::convertToQTColor(lineColor));
    pen.setStyle(static_cast<Qt::PenStyle>(param.lineStyle.style));
    pen.setDashOffset(param.lineStyle.lineOffset);
    pen.setWidth(param.lineStyle.width);
    pen.setCapStyle(param.lineStyle.rounded ? Qt::RoundCap : Qt::SquareCap);
    m_painter->setPen(pen);
    m_painter->setBrush(QBrush(qt_utils::convertToQTColor(backgroundColor)));

    setAntialiasing(param.antialiased);

    auto radius = p2 - p1;
    tryAddCommand(p1, p2, param.lineStyle.width);
    drawCircleImpl(p1, radius);
  }

  void QtDrawer::drawRectBlend(const Point& p1,
                               const Point& p2,
                               const Color& lineColor,
                               const Color& backgroundColor,
                               const DrawParam& param)
  {
    PIXPAINT_ASSERT(m_painter.get(), "Painter is not set");

    QPen pen(qt_utils::convertToQTColor(lineColor));
    pen.setStyle(static_cast<Qt::PenStyle>(param.lineStyle.style));
    pen.setDashOffset(param.lineStyle.lineOffset);
    pen.setWidth(param.lineStyle.width);
    pen.setCapStyle(param.lineStyle.rounded ? Qt::RoundCap : Qt::SquareCap);
    m_painter->setPen(pen);
    m_painter->setBrush(QBrush(qt_utils::convertToQTColor(backgroundColor)));

    setAntialiasing(param.antialiased);

    auto dimension = (p2 - p1);
    tryAddCommand(p1, p2, param.lineStyle.width);
    drawRectImpl(p1, dimension);
  }

  void QtDrawer::drawPath(const std::vector<Point>& points,
                          const Color& lineColor,
                          const DrawParam& param)
  {
    PIXPAINT_ASSERT(m_painter.get(), "Painter is not set");

    QPen pen(qt_utils::convertToQTColor(lineColor));
    pen.setStyle(static_cast<Qt::PenStyle>(param.lineStyle.style));
    pen.setDashOffset(param.lineStyle.lineOffset);
    pen.setWidth(param.lineStyle.width);
    pen.setCapStyle(param.lineStyle.rounded ? Qt::RoundCap : Qt::SquareCap);
    m_painter->setPen(pen);

    setAntialiasing(param.antialiased);

    auto point_count = points.size();
    if(point_count == 1) {
      auto p = points[0];
      auto qp = qt_utils::convertToQTPoint(p);

      tryAddCommand(p, p, param.lineStyle.width);
      m_painter->drawLine(qp, qp);
    } else if(point_count == 2) {
      auto p1 = points[0];
      auto p2 = points[1];
      auto qp1 = qt_utils::convertToQTPoint(p1);
      auto qp2 = qt_utils::convertToQTPoint(p2);

      tryAddCommand(p1, p2, param.lineStyle.width);
      m_painter->drawLine(qp1, qp2);
    } else if(point_count > 2) {
      Point min, max;
      QPainterPath path;

      auto p = points[0];
      path.moveTo(p.x, p.y);
      min = max = p;

      for(std::size_t i = 1, isize = points.size(); i < isize; ++i) {
        p = points[i];
        path.lineTo(p.x, p.y);

        min.x = general_utils::min(min.x, p.x);
        max.x = general_utils::max(max.x, p.x);
        min.y = general_utils::min(min.y, p.y);
        max.y = general_utils::max(max.y, p.y);
      }

      tryAddCommand(min, max, param.lineStyle.width);
      m_painter->drawPath(path);
    }
  }

  void QtDrawer::drawText(std::string text,
                          const std::string& fontFamily,
                          int fontSize,
                          bool antialiased,
                          const Point& dimension,
                          const Color& textColor,
                          const Color& backgroundColor)
  {
    auto font = m_painter->font();
    font.setFamily(fontFamily.c_str());
    font.setPixelSize(fontSize);
    if(antialiased) {
      font.setStyleStrategy(QFont::PreferAntialias);
      setAntialiasing(antialiased);
    } else {
      font.setStyleStrategy(QFont::NoAntialias);
      setAntialiasing(antialiased);
    }
    m_painter->setFont(font);

    m_painter->setBrush(QBrush(qt_utils::convertToQTColor(backgroundColor)));
    m_painter->setPen(QPen(qt_utils::convertToQTColor(backgroundColor)));
    m_painter->drawRect(0, 0, dimension.x - 1, dimension.y - 1);

    m_painter->setPen(QPen(qt_utils::convertToQTColor(textColor)));
    m_painter->drawText(0, 0, dimension.x, dimension.y, 0, text.c_str());
  }

  std::shared_ptr<Mask> QtDrawer::createMaskFromLayer(const PixelData& layer)
  {
    QImage mask_qimage = QImage(layer.getData(),
                                layer.getWidth(),
                                layer.getHeight(),
                                QImage::Format_RGBA8888).createAlphaMask();
    return std::make_shared<QtMask>(QRegion(QBitmap(QPixmap::fromImage(mask_qimage))));
  }

  void QtDrawer::setTargetLayer(MaskablePixelData* layer)
  {
    Drawer::setTargetLayer(layer);

    if(m_painter) {
      m_painter->end();
    }

    if(layer) {
      m_image = std::make_unique<QImage>(qt_utils::createQImage(*static_cast<PixelData*>(layer)));
      m_painter = std::make_unique<QPainter>(m_image.get());
    } else {
      m_image.reset();
      m_painter.reset();
    }
  }

  void QtDrawer::setMask(const std::shared_ptr<Mask>& mask)
  {
    if(m_painter) {
      m_painter->setClipping(true);

      auto* qt_mask = static_cast<QtMask*>(mask.get());
      m_painter->setClipRegion(qt_mask->getRegion());
    }
  }

  void QtDrawer::clearMask()
  {
    if(m_painter) {
      m_painter->setClipping(false);
    }
  }

  void QtDrawer::drawLineImpl(const Point& p1, const Point& p2)
  {
    m_painter->drawLine(qt_utils::convertToQTPoint(p1),
                        qt_utils::convertToQTPoint(p2));
  }

  void QtDrawer::drawCircleImpl(const Point& p1, const Point& radius)
  {
    if(radius.x == 1 || radius.y == 1) {
      m_painter->drawRect(p1.x,
                          p1.y,
                          radius.x,
                          radius.y);
    } else {
      m_painter->drawEllipse(p1.x,
                             p1.y,
                             radius.x,
                             radius.y);
    }
  }

  void QtDrawer::drawRectImpl(const Point& p1, const Point& dimension)
  {
    m_painter->drawRect(p1.x,
                        p1.y,
                        dimension.x,
                        dimension.y);
  }

  void QtDrawer::setAntialiasing(bool antialiased)
  {
    if(antialiased) {
      m_painter->setRenderHint(QPainter::Antialiasing, true);
    } else {
      m_painter->setRenderHint(QPainter::Antialiasing, false);
    }
  }

  void QtDrawer::tryAddCommand(const Point& p1, const Point& p2, int size)
  {
    auto* drawer_commander = getDrawerCommander();
    if(drawer_commander) {
      auto px = general_utils::minmax(p1.x, p2.x);
      auto py = general_utils::minmax(p1.y, p2.y);
      auto min = Point(px.first - size, py.first - size);
      auto max = Point(px.second + size, py.second + size);

      auto dim = max - min;
      drawer_commander->addCommand(min.x, min.y, dim.x, dim.y);
    }
  }
}
