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
#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include "../pattern/singleton.h"

namespace pixpaint
{
  class Document;

  class DocumentManager
  {
    DocumentManager();

  public:
    void setDocument(Document* document);

    Document& getDocument() noexcept;
    const Document& getDocument() const noexcept;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(DocumentManager)

    Document* m_document;
  };

  PIXPAINT_SINGLETON_FUNC(DocumentManager)
}

#endif // DOCUMENTMANAGER_H
