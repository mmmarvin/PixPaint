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
#ifndef LAYERWIDGETITEM_H
#define LAYERWIDGETITEM_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

namespace pixpaint
{
  class LayerPreviewView;
  class LayerWidget;

  class LayerWidgetItem : public QWidget
  {
    Q_OBJECT

    static constexpr auto LAYER_PREVIEW_WIDTH = 75;
    static constexpr auto LAYER_PREVIEW_HEIGHT = 75;

  public:
    LayerWidgetItem(QWidget* parent, std::size_t layerIndex);

    void setSelected(bool selected) noexcept;
    bool isSelected() const noexcept;

    void setMoveDestination(bool destination) noexcept;
    bool isMoveDestination() const noexcept;

    void setLayerIndex(std::size_t index);
    void setLayerName(std::string name);
    void setLayerVisibility(bool visible);
    std::size_t getLayerIndex() const noexcept;
    std::string getLayerName() const;
    bool isLayerVisible() const noexcept;

  public:
    Q_SIGNAL void clicked();
    Q_SLOT void click();

  protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

  private:
    friend class LayerWidget;

    void toggled(bool clicked);

//    LayerWidget*      m_parentWidget;

    LayerPreviewView* m_view;
    QLabel*           m_label;
    QCheckBox*        m_checkbox;
    std::size_t       m_layerIndex;

    bool              m_selected;
    bool              m_moveDestination;
  };
}

#endif // LAYERWIDGETITEM_H
