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
#ifndef SELECTION_HELPERS_H
#define SELECTION_HELPERS_H

namespace pixpaint
{
namespace selection_helpers
{
  void tryFinalizeAllSelections(bool addToHistory = false);
  void tryFinalizeSelection(bool addToHistory = false);
  void tryFinalizeTextSelection(bool addToHistory = false);

  void tryCut(bool addToHistory = false);
  void tryCopy();
  void tryPaste(bool addToHistory = false);
}
}

#endif // SELECTION_HELPERS_H
