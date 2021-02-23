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
#ifndef IMAGEEDITORVIEW_H
#define IMAGEEDITORVIEW_H

#include <QMenu>
#include "../drawer/drawer.h"
#include "../drawer/drawerholder.h"
#include "../env/imageenvironment.h"
#include "../history/history.h"
#include "../image/color.h"
#include "../image/modifyablepixeldata.h"
#include "../image/point.h"
#include "../cursor.h"
#include "customcursorwidget.h"
#include "imageview.h"

namespace pixpaint
{
  class SelectionWidget;
  class ZoomableScrollArea;

  class ImageEditorView : public CustomCursorWidget<ImageView>,
                          public LayerPreviewDrawerHolder
  {
  public:
    ImageEditorView(ZoomableScrollArea* parent,
                    QMenu* rightClickMenu,
                    Image& image,
                    double pixelSize);
    void updateSize();
    void updateDrawers();

    void setTemporaryImage(const Image& image) noexcept;
    bool isTemporarySet() const noexcept;
    void clearTemporaryImage();

    void zoomIn();
    void zoomOut();

    ModifyablePixelData& getPreviewLayer() noexcept;
    const ModifyablePixelData& getPreviewLayer() const noexcept;

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

  private:
    friend class SelectionWidget;
    friend class ZoomableScrollArea;

    void paintImage(QPainter& painter) override final;
    void paintStartEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown);
    void paintMoveEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown);
    void paintEndEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown );
    void paintFinalizePreview();

    bool isMouseInsideDrawingArea(position_t mx, position_t my) const;
    std::pair<MaskablePixelData&, ModifyablePixelData&> getTargetLayer(position_t& px, position_t& py);
    Point getMousePositionInView(position_t mx, position_t my) const;
    DoubleRect adjustToSelection(DoubleRect refreshRect);

    QMenu*                  m_rightClickMenu;
    const Image*            m_temporaryImage;

    ModifyablePixelData     m_previewLayer;
    Point                   m_currentDrawPoint;
    Point                   m_previousDrawPoint;
    Point                   m_globalDrawPoint;

    bool                    m_startDraw;
    Point                   m_startDrawPoint;
    Point                   m_endDrawPoint;
  };
}

#endif // IMAGEEDITORVIEW_H
