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
#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include <QWidget>
#include "baseimageview.h"

namespace pixpaint
{
  class Image;
  class ImageView;

  class PreviewView : public BaseImageView
  {
  public:
    PreviewView(QWidget* parent,
                Image& image);

    void refreshResize();

  protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    virtual void paintSelection(QPainter& painter);

  private:
    void paintImage(QPainter &painter) override;
    void handleResize(int width, int height);
  };
}

#endif // PREVIEWVIEW_H
