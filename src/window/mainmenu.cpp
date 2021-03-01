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
#include "mainmenu.h"

#include "../3rdparty/gengine/string_utility.h"
#include "../dialog/effectoptiondialog.h"
#include "../dialog/newimagedialog.h"
#include "../dialog/rotateimagedialog.h"
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../event/gui/image_events.h"
#include "../helper/image_helpers.h"
#include "../helper/selection_helpers.h"
#include "../history/animationresizeaction.h"
#include "../image/image.h"
#include "../manager/documentmanager.h"
#include "../manager/eventmanager.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../manager/selectionmanager.h"
#include "../registrar/information/effectinformation.h"
#include "../registrar/effectregistrar.h"
#include "../document.h"
#include "imageeditorview.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
namespace
{
  void apply_image_effect_impl(std::string effectName,
                               std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                               std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    image_helpers::applyEffect(std::move(effectName), image_manager.getImage(), layerEffect, selectionEffect);

    emitEvent(gui_events::ImageEffectEvent());

    image_manager.refresh();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getPreviewManager().refreshResizeAll();
  }

  void apply_layer_effect_impl(std::string effectName,
                               std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                               std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto current_layer_index = image_manager.getImage().getCurrentLayerIndex();
    image_helpers::applyEffect(std::move(effectName), image_manager.getImage().getLayer(current_layer_index), layerEffect, selectionEffect);

    emitEvent(gui_events::ImageLayerEffectEvent{ current_layer_index });

    image_manager.refresh();
    image_env.getScrollArea().updateSelectionWidget();
    image_env.getScrollArea().updateResizeHandles();
    getPreviewManager().refreshResizeAll();
  }

  void apply_image_effect(std::string effectName,
                          std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
                          std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
  {
    auto& image_env = getImageEnvironment();
    if(image_env.isViewSet()) {
      apply_image_effect_impl(std::move(effectName), std::move(layerEffect), std::move(selectionEffect));
    }
  }

//  void applyLayerEffect(std::string effectName,
//                        std::function<void(const IntRect&, MaskablePixelData&)> layerEffect,
//                        std::function<void(const IntRect&, MaskablePixelData&)> selectionEffect)
//  {
//    auto& image_env = getImageEnvironment();
//    if(image_env.isViewSet()) {
//      applyLayerEffectImpl(std::move(effectName), std::move(layerEffect), std::move(selectionEffect));
//    }
//  }
}
  void popImageMenu(MainMenu::ImageMenu* imageMenu, QWidget* parent, bool)
  {
    imageMenu->resizeImageAction = imageMenu->menu->addAction(QObject::tr("Resize..."));
    imageMenu->menu->addSeparator();
    imageMenu->flipImageAction = imageMenu->menu->addAction(QObject::tr("Flip"));
    imageMenu->mirrorImageAction = imageMenu->menu->addAction(QObject::tr("Mirror"));
    imageMenu->rotateImageAction = imageMenu->menu->addAction(QObject::tr("Rotate"));
    imageMenu->menu->addSeparator();
    imageMenu->flattenImageAction = imageMenu->menu->addAction(QObject::tr("Flatten Image"));
    imageMenu->menu->addSeparator();
    imageMenu->invertImageAction = imageMenu->menu->addAction(QObject::tr("Invert"));
    imageMenu->clearImageAction = imageMenu->menu->addAction(QObject::tr("Clear"));

    QObject::connect(imageMenu->menu, &QMenu::aboutToShow, [imageMenu] {
      if(getImageEnvironment().isViewSet()) {
        imageMenu->resizeImageAction->setEnabled(true);
        imageMenu->flipImageAction->setEnabled(true);
        imageMenu->mirrorImageAction->setEnabled(true);
        imageMenu->rotateImageAction->setEnabled(true);
        if(getImageManager().getImage().getLayerCount() > 1) {
          imageMenu->flattenImageAction->setEnabled(true);
        } else {
          imageMenu->flattenImageAction->setEnabled(false);
        }
        imageMenu->invertImageAction->setEnabled(true);
        imageMenu->clearImageAction->setEnabled(true);
      } else {
        imageMenu->resizeImageAction->setEnabled(false);
        imageMenu->flipImageAction->setEnabled(false);
        imageMenu->mirrorImageAction->setEnabled(false);
        imageMenu->rotateImageAction->setEnabled(false);
        imageMenu->flattenImageAction->setEnabled(false);
        imageMenu->invertImageAction->setEnabled(false);
        imageMenu->clearImageAction->setEnabled(false);
      }
    });
    QObject::connect(imageMenu->menu, &QMenu::aboutToHide, [imageMenu] {
      imageMenu->resizeImageAction->setEnabled(true);
      imageMenu->flipImageAction->setEnabled(true);
      imageMenu->mirrorImageAction->setEnabled(true);
      imageMenu->rotateImageAction->setEnabled(true);
      imageMenu->flattenImageAction->setEnabled(true);
      imageMenu->invertImageAction->setEnabled(true);
      imageMenu->clearImageAction->setEnabled(true);
    });

    QObject::connect(imageMenu->resizeImageAction, &QAction::triggered, [parent](bool){
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        auto old_width = image_manager.getImage().getWidth();
        auto old_height = image_manager.getImage().getHeight();

        NewImageDialog newDialog(parent, old_width, old_height, true);
        newDialog.setWindowTitle(QObject::tr("Resize Image"));

        int res = newDialog.exec();
        if(res == QDialog::Accepted) {
          selection_helpers::tryFinalizeAllSelections(true);

          emitHistoryAction(AnimationResizeAction(getDocumentManager().getDocument().getAnimation()));

          auto width = newDialog.getWidth();
          auto height = newDialog.getHeight();
          auto resize_mode = newDialog.getResizeMode();

          auto& animation = getDocumentManager().getDocument().getAnimation();
          for(std::size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
            animation.getFrame(i).resize(width, height, Color::WHITE, resize_mode);
          }
          image_env.getView().updateSize();
          image_env.getView().updateDrawers();

          emitEvent(gui_events::ImageResizeEvent{ Point(old_width, old_height), Point(width, height) });

          image_manager.refresh();
          image_env.getScrollArea().updateSelectionWidget();
          image_env.getScrollArea().updateResizeHandles();

          getPreviewManager().refreshResizeAll();

          getImageEnvironment().getView().setFocus();
        }
      }
    });

    QObject::connect(imageMenu->flipImageAction, &QAction::triggered, [parent](bool) {
      if(getImageEnvironment().isViewSet()) {
        apply_image_effect("Flip Image",
        [](const IntRect&, MaskablePixelData& layer)
        {
          layer = layer.flipVertically();
        },
        [](const IntRect&, MaskablePixelData& selectionLayer)
        {
          selectionLayer = selectionLayer.flipVertically();
        });

        getImageEnvironment().getView().setFocus();
      }
    });
    QObject::connect(imageMenu->mirrorImageAction, &QAction::triggered, [parent](bool) {
      if(getImageEnvironment().isViewSet()) {
        apply_image_effect("Mirror Image",
        [](const IntRect&, MaskablePixelData& layer)
        {
          layer = layer.flipHorizontally();
        },
        [](const IntRect&, MaskablePixelData& selectionLayer)
        {
          selectionLayer = selectionLayer.flipHorizontally();
        });

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(imageMenu->rotateImageAction, &QAction::triggered, [parent](bool) {
      auto& image_env = getImageEnvironment();
      if(image_env.isViewSet()) {
        RotateImageDialog rotateDialog(parent);
        int res = rotateDialog.exec();
        if(res == QDialog::Accepted) {
          auto direction = rotateDialog.getRotationDirection();
          auto angle = rotateDialog.getRotation();

          apply_image_effect("Rotate Image",
          [direction, angle](const IntRect& , MaskablePixelData& layer)
          {
            layer.rotate(angle, direction);
          },
          [direction, angle](const IntRect& selection_rect, MaskablePixelData& selection_layer)
          {
            selection_layer.rotate(angle, direction);

            auto selectionRectCenter = center(selection_rect);
            getSelectionManager().setSelectionRectCenter(selectionRectCenter.x,
                                                         selectionRectCenter.y,
                                                         selection_layer.getWidth(),
                                                         selection_layer.getHeight(),
                                                         true,
                                                         false);
          });
        }

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(imageMenu->flattenImageAction, &QAction::triggered, [parent](bool){
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        image_manager.getImage().flattenLayers(true);

        image_manager.getImage().setCurrentLayerIndex(0);
        image_env.getView().updateDrawers();

        emitEvent(gui_events::ImageFlattenEvent());
        getPreviewManager().refreshAll();

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(imageMenu->invertImageAction, &QAction::triggered, [parent](bool) {
      if(getImageEnvironment().isViewSet()) {
        apply_image_effect("Invert Image",
        [](const IntRect& , MaskablePixelData& layer)
        {
          layer.invert();
        },
        [](const IntRect& , MaskablePixelData& selectionLayer)
        {
          selectionLayer.invert();
        });

        getImageEnvironment().getView().setFocus();
      }
    });

    QObject::connect(imageMenu->clearImageAction, &QAction::triggered, [](bool) {
      auto& image_env = getImageEnvironment();
      auto& image_manager = getImageManager();
      if(image_env.isViewSet()) {
        auto& image = image_manager.getImage();
        image = Image(image.getWidth(), image.getHeight());
        image.getLayer(0).clear(Color::WHITE);

        image.setCurrentLayerIndex(0);
        image_env.getView().updateDrawers();

        emitEvent(gui_events::ImageClearEvent());
        getPreviewManager().refreshAll();

        getImageEnvironment().getView().setFocus();
      }
    });
  }



  void popEffectMenu(MainMenu::EffectMenu* effectMenu, QWidget* parent, bool)
  {
    auto effectActionAdder = [](MainMenu::EffectMenu* effect_menu,
                                std::unordered_map<std::string, QMenu*>& menu_lists,
                                EffectInformation& effect_information)
    {
      auto category = gengine2d::string_utils::toLower(effect_information.getCategory());
      if(!category.empty()) {
        category[0] = static_cast<char>(toupper(static_cast<unsigned char>(category[0])));

        auto find_it = menu_lists.find(category);
        if(find_it == menu_lists.end()) {
          auto* category_menu = effect_menu->menu->addMenu(category.c_str());
          menu_lists.emplace(category, category_menu);

          return category_menu->addAction(effect_information.getName().c_str());
        } else {
          return find_it->second->addAction(effect_information.getName().c_str());
        }
      }

      return effect_menu->menu->addAction(effect_information.getName().c_str());
    };

    auto effectMenuAdder = [parent, effectActionAdder](MainMenu::EffectMenu* effect_menu,
                                                       std::unordered_map<std::string, QMenu*>& menu_lists,
                                                       EffectInformation& effect_information) {
      QAction* effect_action = effectActionAdder(effect_menu, menu_lists, effect_information);
      QObject::connect(effect_action, &QAction::triggered, [parent, &effect_information](bool){
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        if(image_env.isViewSet()) {
          effect_information.getEffect().reset();
          if(!effect_information.getEffect().getOptions().empty()) {
            EffectOptionDialog dialog(parent, effect_information);
            auto res = dialog.exec();
            if(res == QDialog::Accepted) {
              auto target = dialog.getTarget();
              if(target == EffectOptionDialog::EEffectTarget::IMAGE ||
                 target == EffectOptionDialog::EEffectTarget::SELECTION) {
                apply_image_effect_impl(effect_information.getName(),
                [&effect_information](const IntRect& rect, MaskablePixelData& layer)
                {
                  effect_information.getEffect().applyLayerEffect(rect, layer);
                },
                [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
                {
                  effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
                });
              } else if(target == EffectOptionDialog::EEffectTarget::LAYER) {
                apply_layer_effect_impl(effect_information.getName(),
                [&effect_information](const IntRect& rect, MaskablePixelData& layer)
                {
                  effect_information.getEffect().applyLayerEffect(rect, layer);
                },
                [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
                {
                  effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
                });
              }
            }
          } else {
            auto& image = image_manager.getImage();
            for(std::size_t i = 0, isize = image.getLayerCount(); i < isize; ++i) {
              apply_image_effect_impl(effect_information.getName(),
              [&effect_information](const IntRect& rect, MaskablePixelData& layer)
              {
                effect_information.getEffect().applyLayerEffect(rect, layer);
              },
              [&effect_information](const IntRect& rect, MaskablePixelData& selectionLayer)
              {
                effect_information.getEffect().applySelectionEffect(rect, selectionLayer);
              });
            }
          }

          getImageEnvironment().getView().setFocus();
        }
      });

      effect_menu->effectsActionList.push_back(effect_action);
    };

    QObject::connect(effectMenu->menu, &QMenu::aboutToShow, [effectMenu] {
      if(getImageEnvironment().isViewSet()) {
        for(auto* action : effectMenu->effectsActionList) {
          action->setEnabled(true);
        }
      } else {
        for(auto* action : effectMenu->effectsActionList) {
          action->setEnabled(false);
        }
      }
    });
    QObject::connect(effectMenu->menu, &QMenu::aboutToHide, [effectMenu] {
      for(auto* action : effectMenu->effectsActionList) {
        action->setEnabled(true);
      }
    });

    std::vector<EffectInformation*> categorized_effects;
    std::vector<EffectInformation*> nonCategorized_effects;

    // sort the effects by category first
    for(auto& effect_information : getEffectRegistrar()) {
      if(!effect_information.getCategory().empty()) {
        categorized_effects.push_back(&effect_information);
      } else {
        nonCategorized_effects.push_back(&effect_information);
      }
    }

    std::unordered_map<std::string, QMenu*> menuLists;
    for(auto& effect_information : categorized_effects) {
      effectMenuAdder(effectMenu, menuLists, *effect_information);
    }

    for(auto& effect_information : nonCategorized_effects) {
      effectMenuAdder(effectMenu, menuLists, *effect_information);
    }
  }
}
