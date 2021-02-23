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
#ifndef HORIZONTALRESIZEHANDLE_H
#define HORIZONTALRESIZEHANDLE_H

#include <QWidget>
#include "../image/point.h"

namespace pixpaint
{
  class ImageEditorView;
  class ZoomableScrollArea;

  class HorizontalResizeHandle : public QWidget
  {
  public:
    HorizontalResizeHandle(ZoomableScrollArea* parent,
                           ImageEditorView* view);

    void updateResizeHandle();

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:
    ZoomableScrollArea* m_parent;
    ImageEditorView*    m_view;
    Point               m_resizeStartDiff;
    bool                m_resizeStart;
  };
}

#endif // HORIZONTALRESIZEHANDLE_H
