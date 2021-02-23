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
#ifndef DUMMYSELECTIONWIDGET_H
#define DUMMYSELECTIONWIDGET_H

#include <QTimer>
#include <QWidget>

namespace pixpaint
{
  class DummySelectionWidget : public QWidget
  {
  protected:
    static constexpr auto SELECTION_HANDLE_WIDTH = 15;
    static constexpr auto SELECTION_HANDLE_HEIGHT = 15;

  public:
    explicit DummySelectionWidget(QWidget* parent);

    void setPosition(int x, int y);

  protected:
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    QTimer& getDashTimer() noexcept;
    const QTimer& getDashTimer() const noexcept;

  private:
    friend class ZoomableScrollArea;

    void updateDash();
    void updateSelection();

    QTimer  m_dashTimer;
    int     m_dashOffset;
  };
}

#endif // DUMMYSELECTIONWIDGET_H
