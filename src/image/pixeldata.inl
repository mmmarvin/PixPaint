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

#include <functional>

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
        position_t src_x_off = 0, src_y_off = 0;
        if(!pixeldata_detail::calculate_offset(x,
                                               y,
                                               src_x_off,
                                               src_y_off,
                                               w,
                                               h,
                                               layer_width,
                                               layer_height)) {
          return;
        }

        auto* src = pixelData.getData() + (src_y_off * 4 * pixelData.getWidth()) + (src_x_off * 4);
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
        position_t src_x_off = 0, src_y_off = 0;
        if(!pixeldata_detail::calculate_offset(x,
                                               y,
                                               src_x_off,
                                               src_y_off,
                                               w,
                                               h,
                                               layer_width,
                                               layer_height)) {
          return;
        }

        auto* src = pixelData.getData() + (src_y_off * 4 * pixelData.getWidth()) + (src_x_off * 4);
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

  template<class Func>
  void PixelData::composite(const PixelData& pixelData,
                            position_t x,
                            position_t y,
                            Func condFunc)
  {
    auto layer_width = static_cast<std::make_signed_t<dimension_t>>(getWidth());
    auto layer_height = static_cast<std::make_signed_t<dimension_t>>(getHeight());
    if(x < layer_width && y < layer_height) {
      std::function<void(color_channel_t*, color_channel_t*)> blend_func = &color_detail::alphaBlend;
      switch(pixelData.getBlendMode()) {
      case EBlendMode::MULTIPLY:
        blend_func = &color_detail::multiplyBlend;
        break;
      case EBlendMode::SCREEN:
        blend_func = &color_detail::screenBlend;
        break;
      case EBlendMode::OVERLAY:
        blend_func = &color_detail::overlayBlend;
        break;
      case EBlendMode::ADDITION:
        blend_func = &color_detail::additionBlend;
        break;
      case EBlendMode::DIFFERENCE:
        blend_func = &color_detail::differenceBlend;
        break;
      case EBlendMode::DARKEN:
        blend_func = &color_detail::darkenBlend;
        break;
      case EBlendMode::LIGHTEN:
        blend_func = &color_detail::lightenBlend;
        break;
      case EBlendMode::COLOR_DODGE:
        blend_func = &color_detail::colorDodgeBlend;
        break;
      case EBlendMode::COLOR_BURN:
        blend_func = &color_detail::colorBurnBlend;
        break;
      default:
        break;
      }

      std::make_signed_t<dimension_t> w = pixelData.getWidth();
      std::make_signed_t<dimension_t> h = pixelData.getHeight();
      position_t src_x_off = 0, src_y_off = 0;
      if(!pixeldata_detail::calculate_offset(x,
                                             y,
                                             src_x_off,
                                             src_y_off,
                                             w,
                                             h,
                                             layer_width,
                                             layer_height)) {
        return;
      }

      auto* src = pixelData.getData() + (src_y_off * 4 * pixelData.getWidth()) + (src_x_off * 4);
      auto* dst = getData() + (y * 4 * layer_width) + (x * 4);
      dimension_t src_width = pixelData.getWidth() * 4;
      dimension_t dst_width = layer_width * 4;
      auto src_layer_opacity = pixelData.getOpacity();
      for(std::size_t j = 0; j < h; ++j) {
        auto* src_ptr = src;
        auto* dst_ptr = dst;
        for(std::size_t i = 0; i < w; ++i) {
          Color src_color = *reinterpret_cast<const Color*>(src_ptr);
          uint32_t src_color_a = uint32_t(src_color.a) * src_layer_opacity / 100;
          src_color.a = src_color_a;

          auto* src_color_ptr = reinterpret_cast<color_channel_t*>(&src_color);
          if(condFunc(dst_ptr, src_color_ptr, i + x, j + y)) {
            blend_func(dst_ptr, src_color_ptr);
          }
          src_ptr += 4;
          dst_ptr += 4;
        }

        src += src_width;
        dst += dst_width;
      }
    }
  }
}
