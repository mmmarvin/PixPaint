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
#include "pixeldata.h"

namespace pixpaint
{
  template<class HardFunc, class Func>
  void PixelData::combine(const PixelData& pixelData,
                          position_t x,
                          position_t y,
                          bool hard,
                          HardFunc hardFunc,
                          Func func)
  {
    auto layer_width = static_cast<std::make_signed_t<dimension_t>>(getWidth());
    auto layer_height = static_cast<std::make_signed_t<dimension_t>>(getHeight());
    if(x < layer_width && y < layer_height) {
      if(hard) {
        std::make_signed_t<dimension_t> w = pixelData.getWidth();
        std::make_signed_t<dimension_t> h = pixelData.getHeight();
        position_t srcXOff = 0, srcYOff = 0;
        if(!pixeldata_detail::calculate_offset(x, y, srcXOff, srcYOff, w, h, layer_width, layer_height)) {
          return;
        }

        auto* src = pixelData.getData() + (srcYOff * 4 * pixelData.getWidth()) + (srcXOff * 4);
        auto* dst = getData() + (y * 4 * layer_width) + (x * 4);
        dimension_t src_width = pixelData.getWidth() * 4;
        dimension_t dst_width = layer_width * 4;
        for(std::size_t j = 0; j < h; ++j) {
          auto* src_ptr = src;
          auto* dst_ptr = dst;
          for(std::size_t i = 0; i < w; ++i) {
            hardFunc(dst_ptr, src_ptr, i + x, j + y);
            src_ptr += 4;
            dst_ptr += 4;
          }

          src += src_width;
          dst += dst_width;
        }
      } else {
        std::make_signed_t<dimension_t> w = pixelData.getWidth();
        std::make_signed_t<dimension_t> h = pixelData.getHeight();
        position_t srcXOff = 0, srcYOff = 0;
        if(!pixeldata_detail::calculate_offset(x, y, srcXOff, srcYOff, w, h, layer_width, layer_height)) {
          return;
        }

        auto* src = pixelData.getData() + (srcYOff * 4 * pixelData.getWidth()) + (srcXOff * 4);
        auto* dst = getData() + (y * 4 * layer_width) + (x * 4);
        dimension_t src_width = pixelData.getWidth() * 4;
        dimension_t dst_width = layer_width * 4;
        for(std::size_t j = 0; j < h; ++j) {
          auto* src_ptr = src;
          auto* dst_ptr = dst;
          for(std::size_t i = 0; i < w; ++i) {
            func(dst_ptr, src_ptr, i + x, j + y);
            src_ptr += 4;
            dst_ptr += 4;
          }

          src += src_width;
          dst += dst_width;
        }
      }
    }
  }
}
