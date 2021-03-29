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
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScreen>
#include "../src/http/updater.h"
#include "updaterwindow.h"

int main(int, char** argv)
{
  int fargc = 1; std::array<const char*, 1> fargv = { argv[0] };
  QApplication app(fargc, const_cast<char**>(fargv.data()));

  auto res = pixpaint::getUpdater().updateAvailable();
  switch(std::get<0>(res)) {
  case 0:
    QMessageBox::information(nullptr, "Updater", "PixPaint is already up-to-date");
    return 0;
  case -1:
    QMessageBox::critical(nullptr, "Updater", "Cannot download new version!");
    return -1;
  default:
    break;
  }

  try {
    auto* main_window = new pixpaint_updater::UpdaterWindow(std::get<1>(res), std::get<2>(res));

    auto window_pos = QGuiApplication::screenAt(QCursor::pos())->geometry().center() -
                      main_window->geometry().center();
    main_window->move(window_pos.x(), window_pos.y());

    main_window->show();
  } catch(...) {
    QMessageBox::critical(nullptr, "Updater", "There was a problem running the updater!");
    return -1;
  }

  return app.exec();
}