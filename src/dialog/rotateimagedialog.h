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
#ifndef ROTATEIMAGEDIALOG_H
#define ROTATEIMAGEDIALOG_H

#include <QDialog>
#include "../image/pixeldata.h"

namespace pixpaint
{
  class RotateImageDialog : public QDialog
  {
  public:
    explicit RotateImageDialog(QWidget* parent);

    float getRotation() const noexcept;
    ERotationDirection getRotationDirection() const noexcept;

  private:
    QLayout* createDirectionWidgets(QWidget* parent);
    QLayout* createAngleWidgets(QWidget* parent);
    QLayout* createButtons();

    float               m_rotation;
    ERotationDirection  m_direction;
  };
}

#endif // ROTATEIMAGEDIALOG_H
