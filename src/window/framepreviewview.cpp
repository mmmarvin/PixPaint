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
#include "framepreviewview.h"

#include <QPainter>
#include "../image/image.h"
#include "../manager/documentmanager.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../utility/qt_utility.h"
#include "../document.h"
#include "../assert.h"
#include "imageeditorview.h"

namespace pixpaint
{
  FrameView::FrameView(QWidget* parent, Image& image, std::size_t frameIndex) :
    PreviewView(parent, image),
    m_frameIndex(frameIndex)
  {
  }

  void FrameView::paintImage(QPainter &painter)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto& document_manager = getDocumentManager();
    auto& selection_manager = getSelectionManager();

    PIXPAINT_ASSERT(image_env.isViewSet(), "View is not set");

    auto current_layer_index = image_manager.getImage().getCurrentLayerIndex();
    auto& image = getImage();

    if(selection_manager.selectionExists() && selection_manager.layerExists()) {
      for(std::size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
        if(image.isVisible(i)) {
          auto qimage = qt_utils::createQImage(image.getLayer(i));
          painter.drawImage(0, 0, qimage);
          if(m_frameIndex == document_manager.getDocument().getAnimation().getCurrentFrameIndex() &&
             i == current_layer_index) {
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
}
