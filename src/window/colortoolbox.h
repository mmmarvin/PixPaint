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
#ifndef COLORTOOLBOX_H
#define COLORTOOLBOX_H

#include <QGridLayout>
#include <QWidget>
#include "../image/color.h"

namespace pixpaint
{
  class ColorButton;
  class ColorToolbox : public QWidget
  {
    class ColorSelectionWidget : public QWidget
    {
    public:
      explicit ColorSelectionWidget(QWidget* parent);

      void setSelectedForegroundColor(const Color& color);
      void setSelectedBackgroundColor(const Color& color);

    protected:
      void paintEvent(QPaintEvent*) override;
      void resizeEvent(QResizeEvent* event) override;

    private:
      ColorButton* m_foregroundColorButton;
      ColorButton* m_backgroundColorButton;
    };

  public:
    explicit ColorToolbox(QWidget* parent);

    void setSelectedForegroundColor(const Color& color);
    void setSelectedBackgroundColor(const Color& color);

  private:
    QWidget* createColorSelectionWidget();
    QWidget* createColorGridWidget();

    void clearColorGrid();
    void updateColorGrid();

    QWidget*              m_colorGridSurface;
    QGridLayout*          m_colorGrid;
    ColorSelectionWidget* m_colorSelectionWidget;
  };
}

#endif // COLORTOOLBOX_H
