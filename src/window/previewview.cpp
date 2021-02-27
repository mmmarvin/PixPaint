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
#include "previewview.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include "../env/imageenvironment.h"
#include "../image/image.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../utility/qt_utility.h"
#include "../assert.h"
#include "../debug_log.h"
#include "imageeditorview.h"

namespace pixpaint
{
  PreviewView::PreviewView(QWidget* parent,
                           Image& image) :
    BaseImageView(parent, image, 1.0, 1.0)
  {
  }

  void PreviewView::refreshResize()
  {
    handleResize(this->geometry().width() - 1,
                 this->geometry().height() - 1);
  }

  void PreviewView::paintEvent(QPaintEvent*)
  {
    QPainter painter(this);
    qt_utils::drawTransparentBackground(painter,
                                        0,
                                        0,
                                        this->geometry().width() - 1,
                                        this->geometry().height() - 1,
                                        10);

    const auto pixel_size_x = this->getPixelSizeX();
    const auto pixel_size_y = this->getPixelSizeY();
    painter.setWorldTransform(QTransform().scale(pixel_size_x, pixel_size_y));
    this->paintImage(painter);
  }

  void PreviewView::resizeEvent(QResizeEvent*)
  {
    handleResize(this->geometry().width() - 1,
                 this->geometry().height() - 1);
  }

  void PreviewView::paintSelection(QPainter& painter)
  {
    auto& selection_manager = getSelectionManager();
    auto selection_rect = selection_manager.getSelectionRect();

    auto qimage = qt_utils::createQImage(selection_manager.getSelectionLayer());
    painter.drawImage(selection_rect.x,
                      selection_rect.y,
                      qimage);
  }

  void PreviewView::paintImage(QPainter &painter)
  {
    auto& image_env = getImageEnvironment();
    auto& selection_manager = getSelectionManager();
    auto& image = getImage();

    PIXPAINT_ASSERT(image_env.isViewSet(), "View is not set");

    if(selection_manager.selectionExists() && selection_manager.layerExists()) {
      auto current_layer_index = image.getCurrentLayerIndex();
      for(std::size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
        if(image.isVisible(i)) {
          auto qimage = qt_utils::createQImage(image.getLayer(i));
          painter.drawImage(0, 0, qimage);
          if(current_layer_index == i) {
            this->paintSelection(painter);
          }
        }
      }
    } else {
      for(std::size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
        if(image.isVisible(i)) {
          auto qimage = qt_utils::createQImage(image.getLayer(i));
          painter.drawImage(0, 0, qimage);
        }
      }
    }
  }

  void PreviewView::handleResize(int width, int height)
  { 
    const auto window_width = width;
    const auto window_height = height;
    const auto image_width = this->getImage().getWidth();
    const auto image_height = this->getImage().getHeight();

    const auto ratio_x = static_cast<double>(window_width) / static_cast<double>(image_width);
    const auto ratio_y = static_cast<double>(window_height) / static_cast<double>(image_height);

    this->setPixelSize(ratio_x, ratio_y);
    this->repaint();
  }
}
