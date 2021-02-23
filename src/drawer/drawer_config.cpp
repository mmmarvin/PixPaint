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
#include "drawer_config.h"

#include "qt/qtdrawer.h"
#include "drawerholder.h"

namespace pixpaint
{
namespace drawer_config
{
  // Change this to change the default drawer
  using drawer_t = QtDrawer;

  void createDrawers(LayerDrawerHolder& drawerHolder)
  {
    drawerHolder.createDrawers<drawer_t>();
  }

  void createDrawers(LayerPreviewDrawerHolder& drawerHolder)
  {
    drawerHolder.createDrawers<drawer_t>();
  }
}
}
