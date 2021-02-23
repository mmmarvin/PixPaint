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
#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

namespace pixpaint
{
  class Image;
  class PreviewView;

  class PreviewWidget : public QWidget
  {
    static constexpr auto PREVIEW_WIDTH = 200;
    static constexpr auto PREVIEW_HEIGHT = 200;

  public:
    PreviewWidget(QWidget* parent,
                  Image& image);

  protected:
    void paintEvent(QPaintEvent*) override;

  public:
    PreviewView* getPreviewView() noexcept;
    const PreviewView* getPreviewView() const noexcept;

  private:
    PreviewView* m_preview;
  };

}

#endif // PREVIEWWIDGET_H
