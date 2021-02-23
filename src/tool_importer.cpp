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
#include "tool_importer.h"

#include "registrar/painttoolregistrar.h"
#include "tool/brushtool.h"
#include "tool/circletool.h"
#include "tool/colorpickertool.h"
#include "tool/custombrushtool.h"
#include "tool/ditherfilltool.h"
#include "tool/erasertool.h"
#include "tool/filltool.h"
#include "tool/linetool.h"
#include "tool/magicwandtool.h"
#include "tool/penciltool.h"
#include "tool/recttool.h"
#include "tool/rectselectiontool.h"
#include "tool/textselectiontool.h"

namespace pixpaint
{
  void register_fixed_tool()
  {
    auto& paintToolRegistry = getPaintToolRegistrar();
    // Rectangle Selection Tool should always be the first to be imported, since it
    // is used as the default selection tool
    paintToolRegistry.registerTool("Rectangle Selection Tool",
                                   "res/icons/rectselection_tool.png",
                                   "rsel",
                                   "S",
                                   std::make_unique<RectSelectionTool>());
    paintToolRegistry.registerTool("Pencil Tool",
                                   "res/icons/pencil_tool.png",
                                   "penc",
                                   "P",
                                   std::make_unique<PencilTool>());
    paintToolRegistry.registerTool("Eraser Tool",
                                   "res/icons/eraser_tool.png",
                                   "eras",
                                   "E",
                                   std::make_unique<EraserTool>());
    paintToolRegistry.registerTool("Brush Tool",
                                   "res/icons/brush_tool.png",
                                   "brus",
                                   "B",
                                   std::make_unique<BrushTool>());
    paintToolRegistry.registerTool("Magic Wand Tool",
                                   "res/icons/magic_wand_tool.png",
                                   "wand",
                                   "W",
                                   std::make_unique<MagicWandTool>());
    paintToolRegistry.registerTool("Custom Brush Tool",
                                   "res/icons/custom_brush_tool.png",
                                   "cbru",
                                   "U",
                                   std::make_unique<CustomBrushTool>());
    paintToolRegistry.registerTool("Text Tool",
                                   "res/icons/text_selection_tool.png",
                                   "text",
                                   "T",
                                   std::make_unique<TextSelectionTool>());
    paintToolRegistry.registerTool("Color Picker Tool",
                                   "res/icons/color_picker_tool.png",
                                   "pick",
                                   "K",
                                   std::make_unique<ColorPickerTool>());
    paintToolRegistry.registerTool("Fill Tool",
                                   "res/icons/fill_tool.png",
                                   "fill",
                                   "F",
                                   std::make_unique<FillTool>());
    paintToolRegistry.registerTool("Dither Fill Tool",
                                   "res/icons/dither_fill_tool.png",
                                   "dith",
                                   "D",
                                   std::make_unique<DitherFillTool>());
    paintToolRegistry.registerTool("Line Tool",
                                   "res/icons/line_tool.png",
                                   "line",
                                   "L",
                                   std::make_unique<LineTool>());
    paintToolRegistry.registerTool("Rectangle Tool",
                                   "res/icons/rect_tool.png",
                                   "rect",
                                   "R",
                                   std::make_unique<RectTool>());
    paintToolRegistry.registerTool("Circle Tool",
                                   "res/icons/circle_tool.png",
                                   "circ",
                                   "O",
                                   std::make_unique<CircleTool>());
  }
}
