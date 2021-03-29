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
#ifndef UPDATERWINDOW_H
#define UPDATERWINDOW_H

#include <thread>
#include <QDialog>
#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>

namespace pixpaint_updater
{
  class UpdaterWindow : public QDialog
  {
  public:
    UpdaterWindow(std::string url, std::string checksum);
    ~UpdaterWindow();

  private:
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent*) override;

  private:
    void dowloaderThread(std::string url, std::string checksum);

    std::atomic_bool  m_ready;
    std::atomic_bool  m_done;
    std::thread       m_downloaderThread;

    QLabel*           m_label;
    QProgressBar*     m_progressBar;
  };
}

#endif // UPDATERWINDOW_H
