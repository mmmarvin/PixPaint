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
#ifndef EFFECTOPTIONDIALOG_H
#define EFFECTOPTIONDIALOG_H

#include <memory>
#include "optiondialog.h"

namespace pixpaint
{
  class EffectInformation;
  class PreviewView;

  class EffectOptionDialog : public OptionDialog
  {
  public:
    enum class EEffectTarget : unsigned char
    {
      IMAGE,
      LAYER,
      SELECTION
    };

    EffectOptionDialog(QWidget* parent, EffectInformation& effectInformation);

    EEffectTarget getTarget() const;

  private:
    void applyEffect();

    std::unique_ptr<Image>  m_previewImage;
    std::unique_ptr<Image>  m_originalImage;
    PreviewView*            m_previewView;

    EffectInformation*      m_effectInformation;

    EEffectTarget           m_target;
  };
}

#endif // EFFECTOPTIONDIALOG_H
