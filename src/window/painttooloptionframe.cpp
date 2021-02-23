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
#include "painttooloptionframe.h"

#include "../env/imageenvironment.h"
#include "../helper/tool_helpers.h"
#include "../image/image.h"
#include "../tool/painttoolbase.h"
#include "../utility/qt_utility.h"
#include "../assert.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"
#include "imageeditorview.h"
#include "optioncheckbox.h"
#include "optionchoice.h"
#include "optionslider.h"
#include "optiontextbox.h"

namespace pixpaint
{
  PaintToolOptionFrame::PaintToolOptionFrame(QWidget* parent) :
    QScrollArea(parent)
  {
    auto* layout = new QVBoxLayout(this);

    m_mainWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_mainWidget);
    m_mainWidget->setLayout(m_mainLayout);
    layout->addWidget(m_mainWidget);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    this->setLayout(layout);
  }

  void PaintToolOptionFrame::setOptions(const PaintToolBase::option_container_type& options)
  {
    static const auto flagSetter = [](bool) {
      auto& imageEnv = getImageEnvironment();
      if(imageEnv.isViewSet()) {
        tool_helpers::onToolChange(getPaintToolManager().getCurrentTool(),
                                   &PaintToolHandlerBase::onOptionChange);
      }
    };

    static const auto intSetter = [](int) {
      auto& imageEnv = getImageEnvironment();
      if(imageEnv.isViewSet()) {
        tool_helpers::onToolChange(getPaintToolManager().getCurrentTool(),
                                   &PaintToolHandlerBase::onOptionChange);
      }
    };

    static const auto strSetter = [](const std::string&) {
      auto& imageEnv = getImageEnvironment();
      if(imageEnv.isViewSet()) {
        tool_helpers::onToolChange(getPaintToolManager().getCurrentTool(),
                                   &PaintToolHandlerBase::onOptionChange);
      }
    };

    auto* item = m_mainLayout->itemAt(0);
    while(item != nullptr) {
      PIXPAINT_ASSERT(item->widget(), "Item added was not widget!");
      delete item->widget();
      item = m_mainLayout->itemAt(0);
    }

    for(auto& option : options) {
      switch(option.getType()) {
      case OptionValue::EValueOptionType::INTEGER_VALUE:
      {
        auto* widget = new OptionSlider(m_mainWidget,
                                        option.getName(),
                                        static_cast<int*>(option.getValue()),
                                        *static_cast<int*>(option.getValue()),
                                        option.getFloor(),
                                        option.getCeil(),
                                        intSetter);

        m_mainLayout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::FLAG:
      {
        auto* widget = new OptionCheckbox(m_mainWidget,
                                          option.getName(),
                                          static_cast<bool*>(option.getValue()),
                                          *static_cast<bool*>(option.getValue()),
                                          flagSetter);
        m_mainLayout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::STRING_VALUE:
      {
        auto* widget = new OptionTextbox(this,
                                         option.getName(),
                                         static_cast<std::string*>(option.getValue()),
                                         *static_cast<std::string*>(option.getValue()),
                                         strSetter);
        m_mainLayout->addWidget(widget);
        break;
      }
      case OptionValue::EValueOptionType::STRING_CHOICE:
      {
        auto* widget = new OptionChoice(this,
                                        option.getName(),
                                        static_cast<std::string*>(option.getValue()),
                                        option.getChoices(),
                                        option.getChoices().begin(),
                                        strSetter);
        m_mainLayout->addWidget(widget);
        break;
      }
      default:
        break;
      }
    }
  }
}
