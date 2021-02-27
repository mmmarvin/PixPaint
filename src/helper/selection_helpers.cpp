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
#include "selection_helpers.h"

#include <clip/clip.h>
#include "../embed/script_utility.h"
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../history/deselectionaction.h"
#include "../history/selectioncutaction.h"
#include "../history/selectionpasteaction.h"
#include "../history/textdeselectionaction.h"
#include "../image/image.h"
#include "../manager/historymanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textmanager.h"
#include "../manager/textselectionmanager.h"
#include "../utility/qt_utility.h"
#include "../window/imageeditorview.h"
#include "../window/lefttoolbox.h"
#include "../window/selectionwidget.h"
#include "../window/zoomablescrollarea.h"

namespace pixpaint
{
namespace selection_helpers
{
  void tryFinalizeAllSelections(bool addToHistory)
  {
    if(getImageEnvironment().isViewSet()) {
      tryFinalizeSelection(addToHistory);
      tryFinalizeTextSelection(addToHistory);
    }
  }

  void tryFinalizeSelection(bool addToHistory)
  {
    auto& selection_manager = getSelectionManager();
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto& image = image_manager.getImage();

    if(image_env.isViewSet() && selection_manager.selectionExists()) {
      if(selection_manager.layerExists()) {
        auto selection_rect = selection_manager.getSelectionRect();
        auto selection_layer = selection_manager.getSelectionLayer();

        if(addToHistory) {
          auto background = image.getCurrentLayer().copy(selection_rect.x,
                                                         selection_rect.y,
                                                         selection_rect.width,
                                                         selection_rect.height);
          if(selection_manager.layerMaskExists()) {
            emitHistoryAction(DeselectionAction(selection_rect, selection_layer, background));
          } else {
            emitHistoryAction(DeselectionAction(selection_rect,
                                                selection_layer,
                                                selection_manager.getSelectionLayerMask(),
                                                background));
          }
        }

        image.getCurrentLayer().combine(std::move(selection_layer),
                                        selection_rect.x,
                                        selection_rect.y);
        selection_manager.clear();
        image_env.getView().repaint();
        getPreviewManager().refreshAll();
      } else {
        selection_manager.clear();
      }
    }
  }

  void tryFinalizeTextSelection(bool addToHistory)
  {
    auto& text_manager = getTextManager();
    auto& text_selection_manager = getTextSelectionManager();
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto& image = image_manager.getImage();

    if(text_selection_manager.selectionExists()) {
      getTextManager().tryUpdateText(false);

      auto selection_rect = text_selection_manager.getSelectionRect();
      auto text_layer = text_selection_manager.getTextLayer();

      if(addToHistory) {
        auto background = image.getCurrentLayer().copy(selection_rect.x,
                                                       selection_rect.y,
                                                       selection_rect.width,
                                                       selection_rect.height);
        emitHistoryAction(TextDeselectionAction(text_selection_manager.getSelectionRect(),
                                                background,
                                                text_manager.getText(),
                                                text_manager.getFontFamily(),
                                                text_manager.getFontSize(),
                                                text_manager.getAntialiasing()));
      }

      image.getCurrentLayer().combine(std::move(text_layer),
                                      selection_rect.x,
                                      selection_rect.y);
      image.setModified(true);

      text_selection_manager.clear();
      text_manager.clearText();

      image_env.getView().repaint();
      getPreviewManager().refreshAll();
    }
  }

  void tryCut(bool addToHistory)
  {
    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists() && selection_manager.selectionExists()) {
      auto& image_env = getImageEnvironment();
      auto selection_rect = selection_manager.getSelectionRect();
      auto selection_layer = selection_manager.getSelectionLayer();
      auto selection_mask = selection_manager.getSelectionLayerMask();
      script_utils::copyLayerToClipboard(selection_layer);

      if(addToHistory) {
        if(selection_manager.layerMaskExists()) {
          emitHistoryAction(SelectionCutAction(selection_rect, selection_layer, selection_mask));
        } else {
          emitHistoryAction(SelectionCutAction(selection_rect, selection_layer));
        }
      }

      selection_manager.clear();
      getPreviewManager().refreshAll();
      image_env.getScrollArea().updateSelectionWidget();
      image_env.getView().repaint();
    }
  }

  void tryCopy()
  {
    auto& selection_manager = getSelectionManager();
    if(selection_manager.layerExists() && selection_manager.selectionExists()) {
      script_utils::copyLayerToClipboard(selection_manager.getSelectionLayer());
    }
  }

  void tryPaste(bool addToHistory)
  {
    auto& image_env = getImageEnvironment();
    if(image_env.isViewSet()) {
      if(clip::has(clip::image_format())) {
        auto& gui_env = getGUIEnvironment();
        gui_env.getLeftToolbox().switchToDefaultSelectionTool();

        auto& selection_manager = getSelectionManager();
        selection_helpers::tryFinalizeAllSelections(true);

        auto mouse_pos = image_env.getView().mapFromGlobal(QCursor::pos());
        auto pixel_size = image_env.getView().getPixelSize();
        auto mouse_pos_x = static_cast<double>(mouse_pos.x()) / pixel_size;
        auto mouse_pos_y = static_cast<double>(mouse_pos.y()) / pixel_size;

        MaskablePixelData clipboard_image(1, 1);
        auto dimension = script_utils::pasteLayerFromClipboard(clipboard_image);
        selection_manager.setSelectionRect(mouse_pos_x - (dimension.x / 2),
                                           mouse_pos_y - (dimension.y / 2),
                                           dimension.x,
                                           dimension.y,
                                           true,
                                           false);
        selection_manager.setSelectionLayer(clipboard_image);
        image_env.getScrollArea().updateSelectionWidget();
        image_env.getView().repaint();

        if(addToHistory) {
          emitHistoryAction(SelectionPasteAction());
        }
      } else if(clip::has(clip::text_format()) && getTextSelectionManager().selectionExists()) {
        std::string text;
        clip::get_text(text);

        getTextManager().addText(text);

        auto& view = image_env.getView();
        auto pixel_size = view.getPixelSize();
        auto refresh_rect = view.getSmallestDrawableRegion(castTo<double>(getTextSelectionManager().getSelectionRect()) * pixel_size);
        view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refresh_rect)));
      }
    }
  }
}
}
