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
#ifndef DRAWER_H
#define DRAWER_H

#include <memory>
#include "../image/rect.h"
#include "../image/pixeldata.h"
#include "../linestyle.h"

namespace pixpaint
{
  class Mask;
  class MaskablePixelData;

  struct DrawParam
  {
    DrawParam(bool antialiased_, LineStyle lineStyle_);

    bool      antialiased;
    LineStyle lineStyle;
  };

  class Drawer
  {
  public:
    class DrawerCommand
    {
    public:
      virtual ~DrawerCommand() {}

      virtual void undo(MaskablePixelData& target) = 0;
      virtual void redo(MaskablePixelData& target) = 0;
    };

  private:
    class DrawerRestorePixelsCommand : public DrawerCommand
    {
    public:
      DrawerRestorePixelsCommand(std::vector<std::pair<Point, Color>> points);

      void undo(MaskablePixelData& target) override;
      void redo(MaskablePixelData& target) override;

    private:
      std::vector<std::pair<Point, Color>> m_points;
    };

    class DrawerRestorePixelCommand : public DrawerCommand
    {
    public:
      DrawerRestorePixelCommand(position_t x, position_t y, MaskablePixelData* target);

      void undo(MaskablePixelData& target) override;
      void redo(MaskablePixelData& target) override;

    private:
      Point               m_position;
      Color               m_color;
    };

    class DrawerRestoreRegionCommand : public DrawerCommand
    {
    public:
      DrawerRestoreRegionCommand(position_t x,
                                 position_t y,
                                 position_t width,
                                 position_t height,
                                 MaskablePixelData* target);

      void undo(MaskablePixelData& target) override;
      void redo(MaskablePixelData& target) override;

    private:
      IntRect             m_rect;
      PixelData           m_region;
    };

    class DrawerCommander
    {
    public:
      void addCommand(position_t x,
                      position_t y,
                      position_t width,
                      position_t height);
      void addCommand(const IntRect& rect);
      void addCommand(position_t x,
                      position_t y);
      void addCommand(const Point& point);
      void addCommand(std::vector<std::pair<Point, Color>> points);

      bool flushable() const;
      std::vector<std::unique_ptr<DrawerCommand>> flush();

    private:
      friend class Drawer;

      MaskablePixelData*                          m_layer;
      std::vector<std::unique_ptr<DrawerCommand>> m_commands;
    };

  protected:
    explicit Drawer(bool addCommands);

  public:
    virtual ~Drawer() {}

    virtual void drawLine(const Point& p1,
                          const Point& p2,
                          const Color& color,
                          const DrawParam& param) = 0;
    virtual void drawLineBlend(const Point& p1,
                               const Point& p2,
                               const Color& color,
                               const DrawParam& param) = 0;
    virtual void drawCircleBlend(const Point& p1,
                                 const Point& p2,
                                 const Color& lineColor,
                                 const Color& backgroundColor,
                                 const DrawParam& param) = 0;
    virtual void drawRectBlend(const Point& p1,
                               const Point& p2,
                               const Color& lineColor,
                               const Color& backgroundColor,
                               const DrawParam& param) = 0;
    virtual void drawPath(const std::vector<Point>& points,
                          const Color& lineColor,
                          const DrawParam& param) = 0;
    virtual void drawText(std::string text,
                          const std::string& fontFamily,
                          int fontSize,
                          bool antialiased,
                          const Point& dimension,
                          const Color& textColor,
                          const Color& backgroundColor) = 0;

    virtual std::shared_ptr<Mask> createMaskFromLayer(const PixelData& layer) = 0;

    DrawerCommander* getDrawerCommander();
    const DrawerCommander* getDrawerCommander() const;

  protected:
    virtual void setTargetLayer(MaskablePixelData* layer) = 0;
    virtual void setMask(const std::shared_ptr<Mask>& mask) = 0;
    virtual void clearMask() = 0;

  private:
    friend class LayerDrawerHolder;
    friend class DrawerHolder;

    std::unique_ptr<DrawerCommander>  m_commander;
  };
}

#endif // DRAWER_H
