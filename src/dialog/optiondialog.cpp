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
#include "optiondialog.h"

#include <QPushButton>
#include "../window/optioncheckbox.h"
#include "../window/optionchoice.h"
#include "../window/optiontextbox.h"
#include "../window/optionslider.h"

namespace pixpaint
{
  OptionDialog::OptionDialog(QWidget* parent) :
    QDialog(parent)
  {
  }

  OptionDialog::OptionDialog(QWidget* parent, const ImageFileTypeBase::option_container_type& options) :
    QDialog(parent)
  {
    this->setWindowTitle(tr("Save Options"));

    auto* layout = new QVBoxLayout(this);
    createOptionWidgets(layout,
                        options,
                        [](int) {},
                        [](int) {},
                        [](const std::string&) {},
                        [](const std::string&) {});
  }

  void OptionDialog::createOptionWidgets(QVBoxLayout* layout,
                                         const ImageFileTypeBase::option_container_type& options,
                                         std::function<void(int)> sliderFunc,
                                         std::function<void(int)> checkboxFunc,
                                         std::function<void(const std::string&)> textFunc,
                                         std::function<void(const std::string&)> choiceFunc)
  {
    for(auto& option : options) {
      switch(option.getType()) {
      case OptionValue::EValueOptionType::INTEGER_VALUE:
      {
        auto* widget = new OptionSlider(this,
                                        option.getName(),
                                        static_cast<int*>(option.getValue()),
                                        *static_cast<int*>(option.getValue()),
                                        option.getFloor(),
                                        option.getCeil(),
                                        sliderFunc);
        layout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::FLAG:
      {
        auto* widget = new OptionCheckbox(this,
                                          option.getName(),
                                          static_cast<bool*>(option.getValue()),
                                          *static_cast<bool*>(option.getValue()),
                                          checkboxFunc);
        layout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::STRING_VALUE:
      {
        auto* widget = new OptionTextbox(this,
                                         option.getName(),
                                         static_cast<std::string*>(option.getValue()),
                                         *static_cast<std::string*>(option.getValue()),
                                         textFunc);
        layout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::STRING_CHOICE:
      {
        auto* widget = new OptionChoice(this,
                                        option.getName(),
                                        static_cast<std::string*>(option.getValue()),
                                        option.getChoices(),
                                        option.getChoices().begin(),
                                        choiceFunc);
        layout->addWidget(widget);
        break;
      }
      default:
        break;
      }
    }

    auto* button_layout = new QHBoxLayout;
    auto* ok_btn = new QPushButton(this);
    ok_btn->setText(tr("Ok"));
    QObject::connect(ok_btn, &QPushButton::clicked, [this](){
      this->accept();
    });

    auto* cancel_btn = new QPushButton(this);
    cancel_btn->setText(tr("Cancel"));
    QObject::connect(cancel_btn, &QPushButton::clicked, [this](){
      this->reject();
    });

    button_layout->addWidget(ok_btn);
    button_layout->addWidget(cancel_btn);
    layout->addLayout(button_layout);
  }
}
