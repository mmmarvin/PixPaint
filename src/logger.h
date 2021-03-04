#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include "pattern/singleton.h"

namespace pixpaint
{
  class Logger
  {
    Logger();

  public:
    void write(std::string msg);

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(Logger)

    std::ofstream m_out;
  };

  PIXPAINT_SINGLETON_FUNC(Logger)
}

#endif // LOGGER_H
