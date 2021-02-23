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
#include "imageeditorview.h"

#include <QPaintEvent>
#include <QPainter>
#include "../drawer/drawer_config.h"
#include "../env/guienvironment.h"
#include "../helper/paint_helpers.h"
#include "../helper/selection_helpers.h"
#include "../helper/tool_helpers.h"
#include "../history/draweraction.h"
#include "../image/image.h"
#include "../manager/colormanager.h"
#include "../manager/drawermanager.h"
#include "../manager/historymanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textselectionmanager.h"
#include "../registrar/information/painttoolinformation.h"
#include "../utility/general_utility.h"
#include "../utility/qt_utility.h"
#include "../debug_log.h"
#include "../manager/previewmanager.h"
#include "statusbar.h"
#include "selectionwidget.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  ImageEditorView::ImageEditorView(ZoomableScrollArea* parent,
                                   QMenu* rightClickMenu,
                                   Image& image,
                                   double pixelSize) :
    CustomCursorWidget<ImageView>(parent, image, pixelSize),
    m_rightClickMenu(rightClickMenu),
    m_temporaryImage(nullptr),
    m_previewLayer(image.getWidth(), image.getHeight()),
    m_currentDrawPoint(Point(-1, -1)),
    m_previousDrawPoint(Point(-1, -1)),
    m_startDraw(false),
    m_startDrawPoint(Point(-1, -1)),
    m_endDrawPoint(Point(-1, -1))
  {
    drawer_config::createDrawers(*this);
    m_previewLayer.clear(Color::TRANSPARENT);

    updateSize();
    updateDrawers();
  }

  void ImageEditorView::updateSize()
  {
    auto pixel_size = this->getPixelSize();
    auto new_width = this->getImage().getWidth();
    auto new_height = this->getImage().getHeight();

    this->resize((new_width * pixel_size), (new_height * pixel_size));
    m_previewLayer.resize(new_width,
                          new_height,
                          Color::TRANSPARENT,
                          MaskablePixelData::EResizeMode::RESIZE);
  }

  void ImageEditorView::updateDrawers()
  {
    setTargetLayer(getLayerDrawer(), &getImage().getCurrentLayer());
    setTargetLayer(getPreviewDrawer(), &m_previewLayer);
  }

  void ImageEditorView::setTemporaryImage(const Image& image) noexcept
  {
    m_temporaryImage = &image;
  }

  bool ImageEditorView::isTemporarySet() const noexcept
  {
    return m_temporaryImage != nullptr;
  }

  void ImageEditorView::clearTemporaryImage()
  {
    m_temporaryImage = nullptr;
  }

  void ImageEditorView::zoomIn()
  {
    auto pixel_size = this->getPixelSize();
    auto pixel_inc_value = pixel_size >= 1.f ? 1.f : 0.1f;
    this->setPixelSize(general_utils::min<float>(pixel_size + pixel_inc_value, 30.f));

    auto refresh_rect = this->getDrawableRegion();
    this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
    this->resize((this->getImage().getWidth() * this->getPixelSize()),
                 (this->getImage().getHeight() * this->getPixelSize()));
    getPreviewManager().refreshAll();

    auto& image_env = getImageEnvironment();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getGUIEnvironment().getStatusBar().setZoomLevelText(pixel_size * 100);
    setCursorImpl(getCursor());
    image_env.getScrollArea().setCursor(image_env.getScrollArea().getCursor());
    image_env.getSelection().setCursor(image_env.getSelection().getCursor());
  }

  void ImageEditorView::zoomOut()
  {
    auto refresh_rect = this->getDrawableRegion();
    auto pixel_size = this->getPixelSize();
    auto pixel_dec_value = pixel_size <= 1.f ? 0.1f : 1.f;
    this->setPixelSize(general_utils::max<float>(pixel_size - pixel_dec_value, 0.1f));

    this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
    this->resize((this->getImage().getWidth() * this->getPixelSize()),
                 (this->getImage().getHeight() * this->getPixelSize()));
    getPreviewManager().refreshAll();

    auto& image_env = getImageEnvironment();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getGUIEnvironment().getStatusBar().setZoomLevelText(pixel_size * 100);
    setCursorImpl(getCursor());
    image_env.getScrollArea().setCursor(image_env.getScrollArea().getCursor());
    image_env.getSelection().setCursor(image_env.getSelection().getCursor());
  }

  ModifyablePixelData& ImageEditorView::getPreviewLayer() noexcept
  {
    return m_previewLayer;
  }

  const ModifyablePixelData& ImageEditorView::getPreviewLayer() const noexcept
  {
    return m_previewLayer;
  }

  void ImageEditorView::paintEvent(QPaintEvent* event)
  {
    ImageView::paintEvent(event);
    event->accept();
  }

  void ImageEditorView::mousePressEvent(QMouseEvent* event)
  {
    if(!m_temporaryImage) {
      const auto mx = event->pos().x();
      const auto my = event->pos().y();
      const auto g_mx = event->globalPos().x();
      const auto g_my = event->globalPos().y();
      const auto t = getMousePositionInView(mx, my);
      const auto gt = getMousePositionInView(g_mx, g_my);
      const auto px = t.x;
      const auto py = t.y;
      const auto g_px = gt.x;
      const auto g_py = gt.y;

      auto shift_down = event->modifiers() & Qt::ShiftModifier;
      auto control_down = event->modifiers() & Qt::ControlModifier;
      m_startDrawPoint = Point(px, py);
      m_startDraw = true;

      m_globalDrawPoint = Point(g_px, g_py);
      if(event->button() == Qt::MouseButton::LeftButton) {
        tool_helpers::setViewToolCursor(Cursor(Cursor::ECursorType::ECT_BUSY));
        paintStartEvent(px, py, getColorManager().getForegroundColor(), shift_down, control_down);
        tool_helpers::updateViewToolCursor();
      } else if(event->button() == Qt::MouseButton::RightButton) {
        if(getPaintToolManager().getCurrentTool().hasRightClick()) {
          tool_helpers::setViewToolCursor(Cursor(Cursor::ECursorType::ECT_BUSY));
          paintStartEvent(px, py, getColorManager().getBackgroundColor(), shift_down, control_down);
          tool_helpers::updateViewToolCursor();
        } else {
          if(getSelectionManager().selectionExists()) {
            m_rightClickMenu->exec(QPoint(event->globalPos().x(),
                                          event->globalPos().y()));
          }
        }
      }
    }

    event->accept();
  }

  void ImageEditorView::mouseMoveEvent(QMouseEvent* event)
  {
    const auto mx = event->pos().x();
    const auto my = event->pos().y();
    const auto t = getMousePositionInView(mx, my);
    const auto px = t.x;
    const auto py = t.y;

    getGUIEnvironment().getStatusBar().setMousePositionText(px, py);

    if(!m_temporaryImage && m_startDraw) {
      const auto g_mx = event->globalPos().x();
      const auto g_my = event->globalPos().y();
      const auto gt = getMousePositionInView(g_mx, g_my);
      const auto g_px = gt.x;
      const auto g_py = gt.y;
      const auto shift_down = event->modifiers() & Qt::ShiftModifier;
      const auto control_down = event->modifiers() & Qt::ControlModifier;
      m_endDrawPoint = Point(px, py);

      getGUIEnvironment().getStatusBar().setDrawSize(m_endDrawPoint.x - m_startDrawPoint.x,
                                                     m_endDrawPoint.y - m_startDrawPoint.y);

      m_globalDrawPoint = Point(g_px, g_py);
      if(event->buttons() == Qt::MouseButton::LeftButton) {
        paintMoveEvent(px, py, getColorManager().getForegroundColor(), shift_down, control_down);
      } else if(event->buttons() == Qt::MouseButton::RightButton) {
        if(getPaintToolManager().getCurrentTool().hasRightClick()) {
          paintMoveEvent(px, py, getColorManager().getBackgroundColor(), shift_down, control_down);
        }
      }
    }

    event->accept();
  }

  void ImageEditorView::mouseReleaseEvent(QMouseEvent* event)
  {
    if(!m_temporaryImage) {
      const auto mx = event->pos().x();
      const auto my = event->pos().y();
      const auto g_mx = event->globalPos().x();
      const auto g_my = event->globalPos().y();
      const auto t = getMousePositionInView(mx, my);
      const auto gt = getMousePositionInView(g_mx, g_my);
      const auto px = t.x;
      const auto py = t.y;
      const auto g_px = gt.x;
      const auto g_py = gt.y;
      const auto shift_down = event->modifiers() & Qt::ShiftModifier;
      const auto control_down = event->modifiers() & Qt::ControlModifier;
      m_startDraw = false;

      getGUIEnvironment().getStatusBar().setDrawSize(0, 0);

      m_globalDrawPoint = Point(g_px, g_py);
      if(event->button() == Qt::MouseButton::LeftButton) {
        auto& current_tool = getPaintToolManager().getCurrentTool();

        paintEndEvent(px, py, getColorManager().getForegroundColor(), shift_down, control_down);
        auto* drawer_commander = pixpaint::getLayerDrawer().getDrawerCommander();
        if(drawer_commander->flushable()) {
          emitHistoryAction(DrawerAction(current_tool.getDescription(), drawer_commander->flush()));
        }
      } else if(event->button() == Qt::MouseButton::RightButton) {
        if(getPaintToolManager().getCurrentTool().hasRightClick()) {
          auto& current_tool = getPaintToolManager().getCurrentTool();

          paintEndEvent(px, py, getColorManager().getBackgroundColor(), shift_down, control_down);
          auto* drawer_commander = pixpaint::getLayerDrawer().getDrawerCommander();
          if(drawer_commander->flushable()) {
            emitHistoryAction(DrawerAction(current_tool.getDescription(), drawer_commander->flush()));
          }
        }
      }
    }

    event->accept();
  }

  void ImageEditorView::wheelEvent(QWheelEvent* event)
  {
    if(event->modifiers() & Qt::ControlModifier) {
      auto scrollValue = event->angleDelta().y();
      if(scrollValue < 0) {
        zoomOut();
      } else if(scrollValue > 0) {
        zoomIn();
      }
    } else {
      event->ignore();
    }
  }

  void ImageEditorView::paintImage(QPainter& painter)
  {
    if(!m_temporaryImage) {
      auto& selection_manager = getSelectionManager();
      auto& text_selection_manager = getTextSelectionManager();

      auto current_layer_index = getImage().getCurrentLayerIndex();
      for(std::size_t i = 0, isize = this->getImage().getLayerCount(); i < isize; ++i) {
        if(this->getImage().isVisible(i)) {
          if(i == current_layer_index) {
            auto layer_qimage = qt_utils::createQImage(this->getImage().getLayer(i));
            painter.drawImage(0, 0, layer_qimage);
            if(selection_manager.selectionExists() &&
               selection_manager.layerExists()) {
              auto selection_rect = selection_manager.getSelectionRect();

              auto selection_qimage = qt_utils::createQImage(selection_manager.getSelectionLayer());
              auto selection_preview_qimage = qt_utils::createQImage(selection_manager.getSelectionPreviewLayer());
              painter.drawImage(selection_rect.x,
                                selection_rect.y,
                                selection_qimage);
              painter.drawImage(selection_rect.x,
                                selection_rect.y,
                                selection_preview_qimage);
            } else if(text_selection_manager.selectionExists()) {
              auto text_selection_qimage = qt_utils::createQImage(text_selection_manager.getTextLayer());
              auto selectionRect = text_selection_manager.getSelectionRect();
              painter.drawImage(selectionRect.x,
                                selectionRect.y,
                                text_selection_qimage);
            } else {
              auto preview_qimage = qt_utils::createQImage(m_previewLayer);
              painter.drawImage(0, 0, preview_qimage);
            }
          } else {
            auto layer_qimage = qt_utils::createQImage(this->getImage().getLayer(i));
            painter.drawImage(0, 0, layer_qimage);
          }
        }
      }
    } else {
      for(std::size_t i = 0, isize = m_temporaryImage->getLayerCount(); i < isize; ++i) {
        if(m_temporaryImage->isVisible(i)) {
          auto temp_qimage = qt_utils::createQImage(m_temporaryImage->getLayer(i));
          painter.drawImage(0, 0, temp_qimage);
        }
      }
    }
  }

  void ImageEditorView::paintStartEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown)
  {
    auto& current_tool = getPaintToolManager().getCurrentTool();
    current_tool.onPreMousePress();

    auto t = getTargetLayer(px, py);
    auto& target_layer = std::get<0>(t);
    auto& target_preview_layer = std::get<1>(t);
    if(paint_helpers::paintStartEvent(px,
                                      py,
                                      color,
                                      ControlState { shiftDown, controlDown },
                                      m_currentDrawPoint,
                                      m_previousDrawPoint,
                                      m_globalDrawPoint,
                                      target_preview_layer,
                                      target_layer)) {
      auto old_rect = current_tool.getOldDrawRect();
      if(old_rect) {
        auto old_rect_val = old_rect.value();
        if(old_rect_val.x >= 0 && old_rect_val.y >= 0) {
          auto old_refresh_rect = this->getSmallestDrawableRegion(castTo<double>(old_rect_val) * this->getPixelSize());
          old_refresh_rect = adjustToSelection(old_refresh_rect);
          this->repaint(qt_utils::convertToQTRect(castTo<position_t>(old_refresh_rect)));
        }
      }

      auto refresh_rect = this->getSmallestDrawableRegion(castTo<double>(current_tool.getDrawRect()) * this->getPixelSize());
      refresh_rect = adjustToSelection(refresh_rect);
      this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));

      getPreviewManager().refreshAll();
    }
  }

  void ImageEditorView::paintMoveEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown)
  {
    auto t = getTargetLayer(px, py);
    auto& target_layer = std::get<0>(t);
    auto& target_preview_layer = std::get<1>(t);
    if(paint_helpers::paintMoveEvent(px,
                                     py,
                                     color,
                                     ControlState { shiftDown, controlDown },
                                     m_currentDrawPoint,
                                     m_previousDrawPoint,
                                     m_globalDrawPoint,
                                     target_preview_layer,
                                     target_layer)) {
      auto& current_tool = getPaintToolManager().getCurrentTool();
      auto old_rect = current_tool.getOldDrawRect();
      if(old_rect) {
        auto old_rect_val = old_rect.value();
        if(old_rect_val.x >= 0 && old_rect_val.y >= 0) {
          auto old_refresh_rect = this->getSmallestDrawableRegion(castTo<double>(old_rect_val) * this->getPixelSize());
          old_refresh_rect = adjustToSelection(old_refresh_rect);
          this->repaint(qt_utils::convertToQTRect(castTo<position_t>(old_refresh_rect)));
        }
      }

      auto refresh_rect = this->getSmallestDrawableRegion(castTo<double>(current_tool.getDrawRect()) * this->getPixelSize());
      refresh_rect = adjustToSelection(refresh_rect);
      this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));

      getPreviewManager().refreshAll();
    }
  }

  void ImageEditorView::paintEndEvent(position_t px, position_t py, const Color& color, bool shiftDown, bool controlDown)
  {
    auto t = getTargetLayer(px, py);
    auto& target_layer = std::get<0>(t);
    auto& target_preview_layer = std::get<1>(t);

    auto res = paint_helpers::paintEndEvent(px,
                                            py,
                                            color,
                                            ControlState { shiftDown, controlDown },
                                            m_currentDrawPoint,
                                            m_previousDrawPoint,
                                            m_globalDrawPoint,
                                            target_preview_layer,
                                            target_layer);
    if(res & PaintToolBase::EChangeResult::ECCR_UPDATEIMAGE) {
      auto& current_tool = getPaintToolManager().getCurrentTool();
      auto old_rect = current_tool.getOldDrawRect();
      if(old_rect) {
        auto old_rect_val = old_rect.value();
        if(old_rect_val.x >= 0 && old_rect_val.y >= 0) {
          auto old_refresh_rect = this->getSmallestDrawableRegion(castTo<double>(old_rect_val) * this->getPixelSize());
          old_refresh_rect = adjustToSelection(old_refresh_rect);
          this->repaint(qt_utils::convertToQTRect(castTo<position_t>(old_refresh_rect)));
        }
      }

      auto refresh_rect = this->getSmallestDrawableRegion(castTo<double>(current_tool.getDrawRect()) * this->getPixelSize());
      refresh_rect = adjustToSelection(refresh_rect);
      this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));

      getPreviewManager().refreshAll();
    }
    if(res & PaintToolBase::EChangeResult::ECCR_UPDATECURSOR) {
      tool_helpers::updateViewToolCursor();
    }

    paintFinalizePreview();
  }

  void ImageEditorView::paintFinalizePreview()
  {
    auto& current_tool = getPaintToolManager().getCurrentTool();
    if(!current_tool.hasFinalize()) {
      auto& selection_manager = getSelectionManager();
      if(selection_manager.selectionExists() &&
         selection_manager.layerExists()) {
        if(selection_manager.getSelectionPreviewLayer().isModified()) {
          selection_manager.getSelectionLayer().combine(selection_manager.getSelectionPreviewLayer());
          selection_manager.getSelectionPreviewLayer().clear(Color::TRANSPARENT);

          auto refresh_rect = this->getSmallestDrawableRegion(castTo<double>(current_tool.getDrawRect()) * this->getPixelSize());
          refresh_rect = adjustToSelection(refresh_rect);
          this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
          getPreviewManager().refreshAll();
        }
      } else if(m_previewLayer.isModified()) {
        this->getImage().getCurrentLayer().combine(m_previewLayer);
        m_previewLayer.clear(Color::TRANSPARENT);

        auto refresh_rect = this->getSmallestDrawableRegion(castTo<double>(current_tool.getDrawRect()) * this->getPixelSize());
        refresh_rect = adjustToSelection(refresh_rect);
        this->repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
        getPreviewManager().refreshAll();
      }
    }
  }

  bool ImageEditorView::isMouseInsideDrawingArea(position_t mx, position_t my) const
  {
    auto drawable_region = this->getDrawableRegion();
    return mx >= 0 && mx < drawable_region.width &&
           my >= 0 && my < drawable_region.height;
  }

  std::pair<MaskablePixelData&, ModifyablePixelData&> ImageEditorView::getTargetLayer(position_t& px, position_t& py)
  {
    MaskablePixelData* draw_layer = &this->getImage().getCurrentLayer();
    ModifyablePixelData* preview_layer = &this->m_previewLayer;

    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists()) {
      auto selection_rect = selection_manager.getSelectionRect();
      auto mouse_position = pixpaint::mapTo(selection_rect, Point(px, py));
      px = mouse_position.x;
      py = mouse_position.y;
      draw_layer = &selection_manager.getSelectionLayer();
      preview_layer = &selection_manager.getSelectionPreviewLayer();
    }

    return std::pair<MaskablePixelData&, ModifyablePixelData&>(*draw_layer, *preview_layer);
  }

  Point ImageEditorView::getMousePositionInView(position_t mx, position_t my) const
  {
    auto pixel_size = this->getPixelSize();
    return Point(mx / pixel_size, my / pixel_size);
  }

  DoubleRect ImageEditorView::adjustToSelection(DoubleRect refreshRect)
  {
    auto pixel_size = this->getPixelSize();
    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists()) {
      auto& selection_rect = selection_manager.getSelectionRect();
      refreshRect += Point(selection_rect.x * pixel_size, selection_rect.y * pixel_size);
    }

    return refreshRect;
  }
}
