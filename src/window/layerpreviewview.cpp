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
#include "layerpreviewview.h"

#include <QPainter>
#include "../env/imageenvironment.h"
#include "../image/image.h"
#include "../assert.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../utility/qt_utility.h"
#include "imageeditorview.h"

namespace pixpaint
{
  LayerPreviewView::LayerPreviewView(QWidget* parent, Image& image) :
    PreviewView(parent, image),
    m_layerIndex(0)
  {
  }

  void LayerPreviewView::setLayerIndex(std::size_t layerIndex)
  {
    m_layerIndex = layerIndex;
  }

  std::size_t LayerPreviewView::getLayerIndex() const noexcept
  {
    return m_layerIndex;
  }

  void LayerPreviewView::paintImage(QPainter& painter)
  {
    paintLayer(painter, this->getImage().getLayer(m_layerIndex));

    auto& image_env = getImageEnvironment();
    auto& selection_manager = getSelectionManager();

    PIXPAINT_ASSERT(image_env.isViewSet(), "View is not set");

    auto current_layer_index = getImageManager().getImage().getCurrentLayerIndex();
    if(selection_manager.selectionExists() && selection_manager.layerExists() &&
       current_layer_index == m_layerIndex) {
      this->paintSelection(painter);
    }
  }
}
