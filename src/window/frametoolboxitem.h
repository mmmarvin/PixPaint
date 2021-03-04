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
#ifndef FRAMETOOLBOXITEM_H
#define FRAMETOOLBOXITEM_H

#include <QWidget>

namespace pixpaint
{
  class FrameView;
  class FrameToolboxItem : public QWidget
  {
    Q_OBJECT

  public:
    static constexpr auto FRAME_PREVIEW_WIDTH = 45;
    static constexpr auto FRAME_PREVIEW_HEIGHT = 45;

    FrameToolboxItem(QWidget* parent, std::size_t frameIndex);

    void setSelected(bool selected) noexcept;
    bool isSelected() const noexcept;

    void setMoveDestination(bool destination) noexcept;
    bool isMoveDestination() const noexcept;

    void setItemIndex(std::size_t index) noexcept;
    std::size_t getItemIndex() const noexcept;

  public:
    Q_SIGNAL void clicked();
    Q_SLOT void click();

  protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

  private:
    friend class FrameToolbox;

    std::size_t   m_frameIndex;
    FrameView*    m_view;

    bool          m_selected;
    bool          m_moveDestination;
  };
}

#endif // FRAMETOOLBOXITEM_H
