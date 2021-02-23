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
#include "globalkeyreader.h"

#include <QKeyEvent>
#include "../3rdparty/gengine/configuration.h"
#include "../env/imageenvironment.h"
#include "../manager/documentmanager.h"
#include "../manager/imagemanager.h"
#include "../utility/qt_utility.h"
#include "../window/imageeditorview.h"
#include "../define.h"
#include "../document.h"

namespace pixpaint
{
namespace
{
  QKeySequence getKeySequence(QKeyEvent* event)
  {
    auto m = event->modifiers();
    auto k = event->key();

    QString modifier = "";
    if(m & Qt::ShiftModifier) {
      modifier += "Shift+";
    }
    if(m & Qt::ControlModifier) {
      modifier += "Ctrl+";
    }
    if(m & Qt::AltModifier) {
      modifier += "Alt+";
    }

    return QKeySequence(modifier + QKeySequence(k).toString());
  }
}
  bool GlobalKeyReader::eventFilter(QObject* watched, QEvent* event)
  {
    auto eventType = event->type();
    if(eventType == QEvent::KeyPress ) {
      auto* keyEvent = static_cast<QKeyEvent*>(event);
      if(!keyEvent->isAutoRepeat()) {
        auto& configurationManager = gengine2d::getConfigurationManager();
        auto& imageManager = getImageManager();
        auto& view = getImageEnvironment().getView();

        auto previousFrameKeySequence = QKeySequence(configurationManager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "previousframe")->c_str());
        auto nextFrameKeySequence = QKeySequence(configurationManager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "nextframe")->c_str());
        auto keySequence = getKeySequence(keyEvent);

        if(previousFrameKeySequence == keySequence) {
          auto& animation = getDocumentManager().getDocument().getAnimation();
          auto currentIndex = animation.getCurrentFrameIndex();
          if(currentIndex > 0) {
            auto& previousFrame = animation.getFrame(currentIndex - 1);
            view.setTemporaryImage(previousFrame);

            auto width = imageManager.getImage().getWidth() * view.getPixelSize();
            auto height = imageManager.getImage().getHeight() * view.getPixelSize();
            view.repaint(qt_utils::convertToQTRect(castTo<position_t>(view.getSmallestDrawableRegion(DoubleRect(0, 0, width, height)))));

            event->accept();
            return true;
          }
        } else if(nextFrameKeySequence == keySequence) {
          auto& animation = getDocumentManager().getDocument().getAnimation();
          auto currentIndex = animation.getCurrentFrameIndex();
          if(animation.getFrameCount() > 0 &&
             currentIndex < animation.getFrameCount() - 1) {
            auto& nextFrame = animation.getFrame(currentIndex + 1);
            view.setTemporaryImage(nextFrame);

            auto width = imageManager.getImage().getWidth() * view.getPixelSize();
            auto height = imageManager.getImage().getHeight() * view.getPixelSize();
            view.repaint(qt_utils::convertToQTRect(castTo<position_t>(view.getSmallestDrawableRegion(DoubleRect(0, 0, width, height)))));

            event->accept();
            return true;
          }
        }
      }
    } else if(eventType == QEvent::KeyRelease) {
      auto* keyEvent = static_cast<QKeyEvent*>(event);
      if(!keyEvent->isAutoRepeat()) {
        auto& configurationManager = gengine2d::getConfigurationManager();
        auto& imageManager = getImageManager();
        auto& view = getImageEnvironment().getView();

        const auto previousFrameKeySequence = QKeySequence(configurationManager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "previousframe")->c_str());
        const auto nextFrameKeySequence = QKeySequence(configurationManager.getString(CONFIG_SECTION_KEYBOARD_SHORTCUT, "nextframe")->c_str());
        auto keySequence = getKeySequence(keyEvent);

        if(previousFrameKeySequence == keySequence ||
           nextFrameKeySequence == keySequence) {
          view.clearTemporaryImage();
          auto width = imageManager.getImage().getWidth() * view.getPixelSize();
          auto height = imageManager.getImage().getHeight() * view.getPixelSize();
          view.repaint(qt_utils::convertToQTRect(castTo<position_t>(view.getSmallestDrawableRegion(DoubleRect(0, 0, width, height)))));

          event->accept();
          return true;
        }
      }
    }

    return QObject::eventFilter(watched, event);
  }
}
