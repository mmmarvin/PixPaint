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
#ifndef SELECTIONWIDGET_H
#define SELECTIONWIDGET_H

#include <QMenu>
#include <QTimer>
#include <QWidget>
#include "../env/imageenvironment.h"
#include "../image/point.h"
#include "../image/rect.h"
#include "customcursorwidget.h"

namespace pixpaint
{
  class ImageEditorView;
  class ZoomableScrollArea;

  class SelectionWidget : public CustomCursorWidget<QWidget>
  {
    static constexpr auto SELECTION_HANDLE_WIDTH = 15;
    static constexpr auto SELECTION_HANDLE_HEIGHT = 15;

  public:
    enum class ESelectionMode : unsigned char
    {
      NORMAL,
      RESIZE,
      MOVE
    };

    SelectionWidget(QWidget* parent, QMenu* rightClickMenu);
    void updateSelection();

    void setPosition(int x, int y);
    void setMode(ESelectionMode mode);

    Point getPosition() const;
    ESelectionMode getMode() const noexcept;

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

  private:
    friend class ZoomableScrollArea;

    void updateSelection(IntRect selectionRect);
    void updateSelectionUsingTemp();
    void updateDash();

    bool isResizeHandleSelected(int mx, int my);
    bool isTopLeftResizeHandleSelected(const IntRect& geometryRect, const Point& mousePosition) const;
    bool isTopRightResizeHandleSelected(const IntRect& geometryRect, const Point& mousePosition) const;
    bool isBottomLeftResizeHandleSelected(const IntRect& geometryRect, const Point& mousePosition) const;
    bool isBottomRightResizeHandleSelected(const IntRect& geometryRect, const Point& mousePosition) const;
    Point getMousePositionInView(position_t mx, position_t my) const;
    IntRect getSelectionRect() const;

    QTimer            m_timer;
    QMenu*            m_rightClickMenu;

    IntRect           m_temporarySelectionRect;
    Point             m_selectionManagerSelectStart;
    Point             m_selectionManagerSelectDimension;
    Point             m_movePoint;
    Point             m_moveStartPoint;

    int               m_dashOffset;

    ESelectionMode    m_mode;
    bool              m_moveStart;
    bool              m_selectionManagerTLSelect;
    bool              m_selectionManagerTRSelect;
    bool              m_selectionManagerBLSelect;
    bool              m_selectionManagerBRSelect;
  };
}

#endif // SELECTIONWIDGET_H
