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
#include "script_utility.h"

#include <cstring>
#include <string>
#include <clip/clip.h>
#include <QFontDatabase>
#include <QMessageBox>
#include "../image/image.h"

namespace pixpaint
{
namespace script_utils
{
namespace
{
  EMessageBoxButtons QButtonToEMessageBoxButton(int qbutton)
  {
    switch(qbutton) {
    case QMessageBox::Yes:
      return EMessageBoxButtons::EMBB_YES;
    case QMessageBox::No:
      return EMessageBoxButtons::EMBB_NO;
    case QMessageBox::Ok:
      return EMessageBoxButtons::EMBB_OK;
    case QMessageBox::Cancel:
      return EMessageBoxButtons::EMBB_CANCEL;
    default:
      break;
    }

    return EMBB_UNKNOWN;
  }
}
  EMessageBoxButtons spawnMessageBox(const std::string& msg, EMessageBoxButtons buttons)
  {
    QMessageBox msg_box;
    msg_box.setText(msg.c_str());

    int msg_btns = 0;
    if(buttons & EMessageBoxButtons::EMBB_YES) {
      msg_btns |= QMessageBox::Yes;
    }
    if(buttons & EMessageBoxButtons::EMBB_NO) {
      msg_btns |= QMessageBox::No;
    }
    if(buttons & EMessageBoxButtons::EMBB_OK) {
      msg_btns |= QMessageBox::Ok;
    }
    if(buttons & EMessageBoxButtons::EMBB_CANCEL) {
      msg_btns |= QMessageBox::Cancel;
    }
    msg_box.setStandardButtons(static_cast<QMessageBox::StandardButton>(msg_btns));
    return QButtonToEMessageBoxButton(msg_box.exec());
  }

  std::vector<std::uint8_t> convertImageToData(const Image& image, bool includeAlpha)
  {
    std::vector<std::uint8_t> ret;

    for(int y = 0; y < image.getHeight(); ++y) {
      for(int x = 0; x < image.getWidth(); ++x) {
        auto c = image.getLayer(0).getPixel(x, y);
        ret.push_back(c.r);
        ret.push_back(c.g);
        ret.push_back(c.b);
        if(includeAlpha) {
          ret.push_back(c.a);
        }
      }
    }

    return ret;
  }

  std::vector<std::uint8_t> convertLayerToData(const PixelData& layer, bool includeAlpha)
  {
    std::vector<std::uint8_t> ret;

    for(int y = 0; y < layer.getHeight(); ++y) {
      for(int x = 0; x < layer.getWidth(); ++x) {
        auto c = layer.getPixel(x, y);
        ret.push_back(c.r);
        ret.push_back(c.g);
        ret.push_back(c.b);
        if(includeAlpha) {
          ret.push_back(c.a);
        }
      }
    }

    return ret;
  }

  std::vector<std::uint32_t> convertImageToData32Bit(const Image& image)
  {
    std::vector<std::uint32_t> ret;

    for(int y = 0; y < image.getHeight(); ++y) {
      for(int x = 0; x < image.getWidth(); ++x) {
        auto c = image.getLayer(0).getPixel(x, y);

        std::uint32_t color = 0;
        color |= static_cast<uint32_t>(c.a);
        color = color << 8;
        color |= static_cast<uint32_t>(c.b);
        color = color << 8;
        color |= static_cast<uint32_t>(c.g);
        color = color << 8;
        color |= static_cast<uint32_t>(c.r);

        ret.push_back(color);
      }
    }

    return ret;
  }

  std::vector<std::uint32_t> convertLayerToData32Bit(const PixelData& layer)
  {
    std::vector<std::uint32_t> ret;

    for(int y = 0; y < layer.getHeight(); ++y) {
      for(int x = 0; x < layer.getWidth(); ++x) {
        auto c = layer.getPixel(x, y);

        std::uint32_t color = 0;
        color |= static_cast<uint32_t>(c.a);
        color = color << 8;
        color |= static_cast<uint32_t>(c.b);
        color = color << 8;
        color |= static_cast<uint32_t>(c.g);
        color = color << 8;
        color |= static_cast<uint32_t>(c.r);

        ret.push_back(color);
      }
    }

    return ret;
  }

  void copyLayerToClipboard(const MaskablePixelData& layer)
  {
    auto image_data = convertLayerToData32Bit(layer);

    clip::image_spec spec;
    spec.width = layer.getWidth();
    spec.height = layer.getHeight();
    spec.bits_per_pixel = 32;
    spec.bytes_per_row = spec.width * 4;
    spec.red_mask = 0xff;
    spec.green_mask = 0xff00;
    spec.blue_mask = 0xff0000;
    spec.alpha_mask = 0xff000000;
    spec.red_shift = 0;
    spec.green_shift = 8;
    spec.blue_shift = 16;
    spec.alpha_shift = 24;

    auto clipImage = clip::image(image_data.data(), spec);
    clip::set_image(clipImage);
  }

  Point pasteLayerFromClipboard(MaskablePixelData& layer)
  {
    clip::image clipboard_image;
    clip::get_image(clipboard_image);

    auto image_spec = clipboard_image.spec();
    layer = MaskablePixelData(image_spec.width, image_spec.height);

    auto* image_data = clipboard_image.data();
    auto* layer_data = layer.getData();
    std::memcpy(layer_data, image_data, image_spec.width* image_spec.height * 4);

    return Point(image_spec.width, image_spec.height);
  }

  std::vector<std::string> getFontList()
  {
    std::vector<std::string> font_family_list;
    auto qt_font_family_list = QFontDatabase().families();
    for(const auto& fontFamily : qt_font_family_list) {
      auto font = fontFamily.toUtf8().data();
      font_family_list.push_back(font);
    }

    return font_family_list;
  }
}
}
