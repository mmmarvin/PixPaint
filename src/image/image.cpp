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
#include "image.h"

#include <string>
#include "../debug_log.h"

namespace pixpaint
{
  Image::Image(position_t width, position_t height, const Color& color) :
    m_currentLayerIndex(0),
    m_width(width),
    m_height(height)
  {
    addLayer(color);
    setModified(false);
  }

  void Image::clear(const Color& color)
  {
    for(auto& t : m_layers) {
      auto& layer = std::get<0>(t);
      layer.clear(color);
    }
  }

  void Image::resize(position_t width,
                     position_t height,
                     const Color& color,
                     MaskablePixelData::EResizeMode mode)
  {
    for(auto& t : m_layers) {
      auto& layer = std::get<0>(t);
      layer.resize(width, height, color, mode);
    }
    m_width = width;
    m_height = height;
    setModified(true);
  }

  position_t Image::getWidth() const noexcept
  {
    return m_width;
  }

  position_t Image::getHeight() const noexcept
  {
    return m_height;
  }

  void Image::addLayer(const Color& color)
  {
    m_layers.emplace_back(std::make_tuple(MaskablePixelData(m_width, m_height),
                                          "Layer " + std::to_string(m_layers.size()),
                                          true));
    std::get<0>(m_layers.back()).clear(color);
    setModified(true);
  }

  void Image::addLayer(std::size_t index, const Color& color)
  {
    m_layers.insert(m_layers.begin() + index,
                    std::make_tuple(MaskablePixelData(m_width, m_height),
                                    "Layer " + std::to_string(m_layers.size()),
                                    true));
    std::get<0>(m_layers[index]).clear(color);
    setModified(true);
  }

  void Image::removeLayer(std::size_t index)
  {
    m_layers.erase(m_layers.begin() + index);
    setModified(true);
  }

  void Image::renameLayer(std::size_t index, std::string name)
  {
    std::get<1>(m_layers[index]) = std::move(name);
    setModified(true);
  }

  void Image::moveLayer(std::size_t srcIndex, std::size_t dstIndex)
  {
    auto layer = std::move(m_layers[srcIndex]);
    m_layers.erase(m_layers.begin() + srcIndex);
    m_layers.insert(m_layers.begin() + dstIndex, std::move(layer));

    setModified(true);
  }

  void Image::setVisible(std::size_t index, bool visible)
  {
    std::get<2>(m_layers[index]) = visible;
    setModified(true);
  }

  void Image::rotate(float angle, ERotationDirection direction)
  {
    if(getLayerCount() > 0) {
      for(auto& t : m_layers) {
        auto& layer = std::get<0>(t);
        layer.rotate(angle, direction);
      }

      m_width = std::get<0>(*m_layers.begin()).getWidth();
      m_height = std::get<0>(*m_layers.begin()).getHeight();
    }
    setModified(true);
  }

  const std::string& Image::getLayerName(std::size_t index) const noexcept
  {
    return std::get<1>(m_layers[index]);
  }

  MaskablePixelData& Image::getLayer(std::size_t index) noexcept
  {
    return std::get<0>(m_layers[index]);
  }

  const MaskablePixelData& Image::getLayer(std::size_t index) const noexcept
  {
    return std::get<0>(m_layers[index]);
  }

  const std::string& Image::getCurrentLayerName() const noexcept
  {
    return getLayerName(m_currentLayerIndex);
  }

  MaskablePixelData& Image::getCurrentLayer() noexcept
  {
    return getLayer(m_currentLayerIndex);
  }

  const MaskablePixelData& Image::getCurrentLayer() const noexcept
  {
    return getLayer(m_currentLayerIndex);
  }

  void Image::setCurrentLayerIndex(std::size_t index)
  {
    m_currentLayerIndex = index;
  }

  std::size_t Image::getCurrentLayerIndex() const noexcept
  {
    return m_currentLayerIndex;
  }

  std::size_t Image::getLayerCount() const noexcept
  {
    return m_layers.size();
  }

  bool Image::isVisible(std::size_t index) const noexcept
  {
    return std::get<2>(m_layers[index]);
  }

  bool Image::isCurrentLayerVisible() const noexcept
  {
    return isVisible(m_currentLayerIndex);
  }

  void Image::flattenLayers(bool modified)
  {
    MaskablePixelData final_layer(m_width, m_height, Color::TRANSPARENT);
    for(auto& t : m_layers) {
      final_layer.composite(std::get<0>(t));
    }

    m_layers.clear();
    m_layers.push_back(std::make_tuple(std::move(final_layer),
                                       "Layer 0",
                                       true));
    if(modified) {
      setModified(true);
    }
  }

  void Image::setModified(bool modified)
  {
    m_modified = modified;
  }

  bool Image::isModified() const noexcept
  {
    return m_modified;
  }
}
