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
#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <functional>
#include <QDialog>
#include <QVBoxLayout>
#include "../filetype/imagefiletypebase.h"

namespace pixpaint
{
  class OptionDialog : public QDialog
  {
  public:
    OptionDialog(QWidget* parent);
    OptionDialog(QWidget* parent, const ImageFileTypeBase::option_container_type& options);

  protected:
    void createOptionWidgets(QVBoxLayout* layout,
                             const ImageFileTypeBase::option_container_type& options,
                             std::function<void(int)> sliderFunc,
                             std::function<void(int)> checkboxFunc,
                             std::function<void(const std::string&)> textFunc,
                             std::function<void(const std::string&)> choiceFunc);
  };
}

#endif // OPTIONDIALOG_H
