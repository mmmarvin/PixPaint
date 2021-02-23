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
#ifndef LAYERPREVIEWVIEW_H
#define LAYERPREVIEWVIEW_H

#include "previewview.h"

namespace pixpaint
{
  class LayerPreviewView : public PreviewView
  {
  public:
    LayerPreviewView(QWidget* parent, Image& image);

    void setLayerIndex(std::size_t layerIndex);
    std::size_t getLayerIndex() const noexcept;

  protected:
    void paintImage(QPainter& painter) override;

  private:
    std::size_t m_layerIndex;
  };
}

#endif // LAYERPREVIEWVIEW_H
