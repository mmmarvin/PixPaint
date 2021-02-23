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
#include "effectinformation.h"

#include "../../assert.h"

namespace pixpaint
{
  EffectInformation::EffectInformation(std::string name,
                                       std::string category,
                                       std::unique_ptr<EffectHandlerBase> effect,
                                       bool showPreview) :
    m_showPreview(showPreview),
    m_effect(std::move(effect)),
    m_name(std::move(name)),
    m_category(std::move(category))
  {
    PIXPAINT_ASSERT(m_effect.get(), "Empty effect");
  }

  bool EffectInformation::showPreview() const
  {
    return m_showPreview;
  }

  const std::string& EffectInformation::getName() const
  {
    return m_name;
  }

  const std::string& EffectInformation::getCategory() const
  {
    return m_category;
  }

  EffectHandlerBase& EffectInformation::getEffect()
  {
    return *m_effect.get();
  }

  const EffectHandlerBase& EffectInformation::getEffect() const
  {
    return *m_effect.get();
  }
}
