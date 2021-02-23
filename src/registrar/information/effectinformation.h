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
#ifndef EFFECTINFORMATION_H
#define EFFECTINFORMATION_H

#include <string>
#include "../../effect/effectbase.h"

namespace pixpaint
{
  class EffectInformation
  {
  public:
    EffectInformation(std::string name,
                      std::string category,
                      std::unique_ptr<EffectHandlerBase> effect,
                      bool showPreview = false);

    bool showPreview() const;

    const std::string& getName() const;
    const std::string& getCategory() const;
    EffectHandlerBase& getEffect();
    const EffectHandlerBase& getEffect() const;

  private:
    bool                                m_showPreview;
    std::unique_ptr<EffectHandlerBase>  m_effect;
    std::string                         m_name;
    std::string                         m_category;
  };
}

#endif // EFFECTINFORMATION_H
