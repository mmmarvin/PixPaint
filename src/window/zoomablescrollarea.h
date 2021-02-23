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
#ifndef ZOOMABLESCROLLAREA_H
#define ZOOMABLESCROLLAREA_H

#include <QScrollArea>
#include "../env/imageenvironment.h"
#include "../image/point.h"
#include "../cursor.h"
#include "customcursorwidget.h"
#include "mainmenu.h"

namespace pixpaint
{
  class DocumentPanel;
  class ImageEditorView;
  class SelectionWidget;
  class DummySelectionWidget;

  class HorizontalResizeHandle;
  class HVResizeHandle;
  class VerticalResizeHandle;

  class ZoomableScrollArea : public CustomCursorWidget<QScrollArea>
  {
  public:
    explicit ZoomableScrollArea(QWidget* parent);
    void setWidget(ImageEditorView* view);

    void updateSelectionWidget();
    void updateDummySelectionWidget();
    void updateResizeHandles();

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

  private:
    friend class DocumentPanel;
    friend class HorizontalResizeHandle;
    friend class HVResizeHandle;
    friend class ImageEnvironment;
    friend class DummySelectionWidget;
    friend class SelectionWidget;
    friend class VerticalResizeHandle;

    void createSelectionWidget(MainMenu::EditMenu* rightClickMenu);
    void createResizeHandles();

    ImageEditorView*        m_view;
    SelectionWidget*        m_selectionWidget;
    DummySelectionWidget*   m_dummySelectionWidget;

    VerticalResizeHandle*   m_vertResizeHandle;
    HorizontalResizeHandle* m_horResizeHandle;
    HVResizeHandle*         m_hvResizeHandle;
  };
}

#endif // ZOOMABLESCROLLAREA_H
