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
#ifndef NEWIMAGEDIALOG_H
#define NEWIMAGEDIALOG_H

#include <QDialog>
#include <string>
#include "../image/maskablepixeldata.h"

namespace pixpaint
{
  class NewImageDialog : public QDialog
  {
  public:
    explicit NewImageDialog(QWidget* parent,
                            std::size_t startingWidth,
                            std::size_t startingHeight,
                            bool showScaleOptions = false);

    std::size_t getWidth() const noexcept;
    std::size_t getHeight() const noexcept;
    MaskablePixelData::EResizeMode getResizeMode() const noexcept;

  private:
    QLayout* createDimensionWidgets(QWidget* parent,
                                    const std::string& dimension,
                                    std::size_t initialValue,
                                    std::size_t* value);
    QLayout* createScaleOptionWidgets(QWidget* parent);
    QLayout* createButtons(bool showScaleOptions);

    std::size_t             m_width;
    std::size_t             m_height;
    MaskablePixelData::EResizeMode  m_resizeMode;
  };
}

#endif // NEWIMAGEDIALOG_H
