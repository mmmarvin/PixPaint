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
#include "mainwindow.h"

#include <QMessageBox>
#include <QTabBar>
#include "../dialog/optiondialog.h"
#include "../embed/headerstream.h"
#include "../manager/documentmanager.h"
#include "../utility/filetype_utility.h"
#include "../document.h"
#include "documentpanel.h"

namespace pixpaint
{
  template<class RegistrarGetterT, class FileOpenerT, class FileTypeSetterT>
  bool MainWindow::openImpl(std::string filename,
                            bool shouldSetFilename,
                            bool shouldAddToRecent,
                            bool showError)
  {
    auto extension = std::string(os_specific::to_string(os_specific::filesystem::path(filename).extension()));
    if(!extension.empty() && extension[0] == '.') {
      extension = extension.substr(1, extension.size() - 1);
    }

    HeaderStream stream;
    stream.m_stream.open(filename, std::ios_base::binary);
    if(!stream.m_stream.is_open()) {
      if(showError) {
        QMessageBox::critical(this, tr("Error"), tr("There was an error opening the file!"));
      }
      return false;
    }

    Document animationDocument(64, 64);

    // try opening first using canReadHeader
    auto& document_manager = getDocumentManager();
    auto& image_manager = getImageManager();
    for(auto& file_type_information : RegistrarGetterT()()) {
      stream.reset();

      auto& file_type_obj = file_type_information.getType();
      if(file_type_obj.canReadHeader(stream)) {
        auto res = FileOpenerT()(filename, file_type_obj, animationDocument);
        if(res == ImageFileTypeBase::EOR_NO_OPEN) {
          return false;
        }
        if(res == ImageFileTypeBase::EOR_SUCCESS) {
          m_documentPanel->addPage(std::move(animationDocument));
          m_documentPanel->tabBar()->setTabText(m_documentPanel->getDocumentIndex(document_manager.getDocument()), filename.c_str());

          FileTypeSetterT()(document_manager, file_type_obj);

          image_manager.refresh();

          if(shouldSetFilename) {
            document_manager.getDocument().setFilename(filename);
          }
          if(shouldAddToRecent) {
            addToRecent(filename);
          }

          m_lastImageWidth = image_manager.getImage().getWidth();
          m_lastImageHeight = image_manager.getImage().getHeight();
          return true;
        }
      }
    }

    // else, try opening using file extension
    for(auto& file_type_information : RegistrarGetterT()()) {
      if(extension == file_type_information.getExtension()) {
        auto& file_type_obj = file_type_information.getType();
        auto res = FileOpenerT()(filename, file_type_obj, animationDocument);
        if(res == ImageFileTypeBase::EOR_ERROR) {
          if(showError) {
            QMessageBox::critical(this, tr("Error"), tr("There was an error opening the file!"));
          }
        } else if(res == ImageFileTypeBase::EOR_SUCCESS) {
          m_documentPanel->addPage(std::move(animationDocument));
          m_documentPanel->tabBar()->setTabText(m_documentPanel->getDocumentIndex(document_manager.getDocument()),
                                                filename.c_str());

          FileTypeSetterT()(document_manager, file_type_obj);

          image_manager.refresh();

          if(shouldSetFilename) {
            document_manager.getDocument().setFilename(filename);
          }
          if(shouldAddToRecent) {
            addToRecent(filename);
          }

          m_lastImageWidth = image_manager.getImage().getWidth();
          m_lastImageHeight = image_manager.getImage().getHeight();
        }
        return true;
      }
    }

    // if all else fails throw an error
    if(showError) {
      QMessageBox::critical(this, tr("Warning"), tr("Cannot open unknown file type"));
    }
    return false;
  }

  template<class RegistrarGetterT, class FileSaverT, class FileTypeSetterT, class FilePreSaverT>
  void MainWindow::saveImpl(std::string filename,
                            std::string mimeType,
                            bool shouldSetFilename,
                            bool shouldSetTabText,
                            bool shouldAddToRecent)
  {
    auto& document_manager = getDocumentManager();

    auto extension = filetype_utils::grabExtension(mimeType);
//    if(extension.empty()) {
//      QMessageBox::critical(this, tr("Error"), tr("Unknown image extension!"));
//      return;
//    }
    for(auto& file_type_information : RegistrarGetterT()()) {
      if(extension == file_type_information.getExtension()) {
        auto& file_type_obj = file_type_information.getType();
        if(!file_type_obj.getOptions().empty()) {
          FilePreSaverT()(document_manager, file_type_obj);

          OptionDialog dialog(this, file_type_obj.getOptions());
          auto diag_res = dialog.exec();

          if(diag_res == QDialog::Accepted) {
            auto res = FileSaverT()(filetype_utils::addExtension(filename, file_type_information),
                                    file_type_obj,
                                    document_manager.getDocument());
            if(res == ImageFileTypeBase::EOR_ERROR) {
              QMessageBox::critical(this, tr("Error"), tr("There was an error saving!"));
              break;
            }

            FileTypeSetterT()(document_manager, file_type_obj);

            if(shouldSetFilename) {
              document_manager.getDocument().setFilename(filename);
            }

            if(shouldSetTabText) {
              m_documentPanel->tabBar()->setTabText(m_documentPanel->getDocumentIndex(document_manager.getDocument()),
                                                    filename.c_str());
            }
            if(shouldAddToRecent) {
              addToRecent(filename);
            }

            auto& animation = document_manager.getDocument().getAnimation();
            for(size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
              animation.getFrame(i).setModified(false);
            }
            break;
          }
        } else {
          auto res = FileSaverT()(filetype_utils::addExtension(filename, file_type_information),
                                  file_type_obj,
                                  document_manager.getDocument());
          if(res == ImageFileTypeBase::EOR_ERROR) {
            QMessageBox::critical(this, tr("Error"), tr("There was an error saving!"));
            break;
          }

          FileTypeSetterT()(document_manager, file_type_obj);

          if(shouldSetFilename) {
            document_manager.getDocument().setFilename(filename);
          }

          if(shouldSetTabText) {
            m_documentPanel->tabBar()->setTabText(m_documentPanel->getDocumentIndex(getDocumentManager().getDocument()),
                                                  filename.c_str());
          }
          if(shouldAddToRecent) {
            addToRecent(filename);
          }

          auto& animation = document_manager.getDocument().getAnimation();
          for(size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
            animation.getFrame(i).setModified(false);
          }
          break;
        }
      }
    }
  }
}
