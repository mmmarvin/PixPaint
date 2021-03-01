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
#include "lefttoolbox.h"

#include <QButtonGroup>
#include <QGridLayout>
#include "../env/guienvironment.h"
#include "../event/gui/tab_events.h"
#include "../helper/selection_helpers.h"
#include "../helper/tool_helpers.h"
#include "../layout/flowlayout.h"
#include "../registrar/painttoolregistrar.h"
#include "../utility/qt_utility.h"
#include "../manager/colormanager.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/previewmanager.h"
#include "../debug_log.h"
#include "../gui_define.h"
#include "imageeditorview.h"
#include "statusbar.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  LeftToolbox::LeftToolbox(QWidget* parent) :
    QWidget(parent)
  {
    m_optionFrame = static_cast<PaintToolOptionFrame*>(createOptionFrame());
    auto* toolbox = createToolbox();

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(toolbox, 1);
    layout->addWidget(m_optionFrame, 9);
    this->setLayout(layout);

    getPaintToolManager().registerObserver(*this);
  }

  void LeftToolbox::switchToDefaultTool()
  {
    getPaintToolManager().setCurrentTool(m_defaultToolIndex);
  }

  void LeftToolbox::switchToDefaultSelectionTool()
  {
    getPaintToolManager().setCurrentTool(m_defaultSelectionToolIndex);
  }

  PaintToolOptionFrame& LeftToolbox::getOptionFrame()
  {
    return *m_optionFrame;
  }

  void LeftToolbox::onEmit(const gui_events::TabAddEvent&)
  {
    if(!this->isEnabled()) {
      this->setEnabled(true);
    }
  }

  void LeftToolbox::onEmit(const gui_events::TabRemoveEvent& event)
  {
    if(!event.numberOfTabs) {
      this->setEnabled(false);
    }
  }

  void LeftToolbox::updateObserver(int)
  {
    const auto tool_index = getPaintToolManager().getCurrentToolIndex();

    auto* tool_btn = static_cast<QPushButton*>(m_toolLayout->itemAt(tool_index)->widget());
    tool_btn->setChecked(true);

    m_optionFrame->setOptions(getPaintToolManager().getCurrentTool().getOptions());
  }

  QWidget* LeftToolbox::createToolbox()
  {
    auto& paintToolManager = getPaintToolManager();
    auto& paintToolRegistrar = getPaintToolRegistrar();

    auto* widget = new QWidget(this);
    auto* buttonGroup = new QButtonGroup(widget);

    m_toolLayout = new FlowLayout(widget);
    m_toolLayout->setSpacing(1);
    m_toolLayout->setAlignment(Qt::AlignTop);

    for(auto it = paintToolRegistrar.begin(),  it_end = paintToolRegistrar.end(); it != it_end; ++it) {
      auto& paintToolInformation = *it;
      auto* toolBtn = new QPushButton(widget);
      toolBtn->setToolTip((paintToolInformation.getName() +
                           std::string(" (") +
                           paintToolInformation.getShortcut() +
                           std::string(")")).c_str());

      QObject::connect(toolBtn, &QPushButton::clicked,
      [it, &paintToolManager, &paintToolInformation, &paintToolRegistrar]() {
        auto& paintTool = paintToolInformation.getTool();
        if(paintToolManager.currentToolSet()) {
          if(&paintToolManager.getCurrentTool() != &paintTool) {
            paintToolManager.setCurrentTool(it - paintToolRegistrar.begin());
          }
        } else {
          paintToolManager.setCurrentTool(it - paintToolRegistrar.begin());
        }
      });
      toolBtn->setMaximumSize(PAINTTOOLBOX_BUTTON_WIDTH, PAINTTOOLBOX_BUTTON_HEIGHT);
      toolBtn->setIcon(QIcon(paintToolInformation.getIconFilename().c_str()));
      toolBtn->setIconSize(QSize(PAINTTOOLBOX_BUTTON_WIDTH, PAINTTOOLBOX_BUTTON_HEIGHT));
      toolBtn->setCheckable(true);

      buttonGroup->addButton(toolBtn);
      m_toolLayout->addWidget(toolBtn);

      if(m_toolLayout->count() == 1) {
        m_defaultSelectionToolIndex = it - paintToolRegistrar.begin();
      } else if(m_toolLayout->count() == 2) {
        m_defaultToolIndex = it - paintToolRegistrar.begin();
      }
    }
    widget->setLayout(m_toolLayout);
    widget->move(5, 5);

    return widget ;
  }

  QWidget* LeftToolbox::createOptionFrame()
  {
    return new PaintToolOptionFrame(this);
  }
}
