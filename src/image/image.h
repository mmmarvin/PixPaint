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
#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "maskablepixeldata.h"

namespace pixpaint
{
  class Image
  {
  public:
    Image(position_t width, position_t height, const Color& color = Color::WHITE);
    void clear(const Color& color);

    void resize(position_t width,
                position_t height,
                const Color& color = Color::WHITE,
                MaskablePixelData::EResizeMode mode = MaskablePixelData::EResizeMode::RESIZE);
    position_t getWidth() const noexcept;
    position_t getHeight() const noexcept;

    void addLayer(const Color& color = Color::TRANSPARENT);
    void addLayer(std::size_t index, const Color& color = Color::TRANSPARENT);
    void removeLayer(std::size_t index);
    void renameLayer(std::size_t index, std::string name);
    void moveLayer(std::size_t srcIndex, std::size_t dstIndex);
    void setVisible(std::size_t index, bool visible);

    void rotate(float angle, ERotationDirection direction);

    const std::string& getLayerName(std::size_t index) const noexcept;
    MaskablePixelData& getLayer(std::size_t index) noexcept;
    const MaskablePixelData& getLayer(std::size_t index) const noexcept;

    const std::string& getCurrentLayerName() const noexcept;
    MaskablePixelData& getCurrentLayer() noexcept;
    const MaskablePixelData& getCurrentLayer() const noexcept;

    void setCurrentLayerIndex(std::size_t index);
    std::size_t getCurrentLayerIndex() const noexcept;
    std::size_t getLayerCount() const noexcept;

    bool isVisible(std::size_t layerIndex) const noexcept;
    bool isCurrentLayerVisible() const noexcept;

    void flattenLayers(bool modified = true);

    void setModified(bool modified);
    bool isModified() const noexcept;

  private:
    std::vector<std::tuple<MaskablePixelData, std::string, bool>> m_layers;
    std::size_t                                                   m_currentLayerIndex;
    position_t                                                    m_width;
    position_t                                                    m_height;
    bool                                                          m_modified;
  };
}

#endif // IMAGE_H
