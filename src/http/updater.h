#ifndef UPDATER_H
#define UPDATER_H

#include "../pattern/singleton.h"

namespace pixpaint
{
  class Updater
  {
    static constexpr auto* HOST = "mmmarvin.github.io";
    static constexpr auto* PORT = "https";
    static constexpr auto* TARGET = "/PixPaintWeb/version.json";

    Updater() {}

  public:
    int updateAvailable() const;

  private:
    PIXPAINT_SINGLETON_FUNC_FRIEND(Updater)
  };

  PIXPAINT_SINGLETON_FUNC(Updater)
}

#endif // UPDATER_H
