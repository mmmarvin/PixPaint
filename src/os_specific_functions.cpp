#include "os_specific_functions.h"

#include <boost/process.hpp>
#include "macro.h"

namespace pixpaint
{
  namespace os_specific
  {
    bool call_process(const std::string& process_name,
                      const std::string& process_parameters)
    {
      if(process_parameters.empty()) {
        try {
          boost::process::child c(process_name);

          if(c.running()) {
            c.detach();
            return true;
          }
        } catch(...) {}

        return false;
      }

      try {
        boost::process::child c(process_name, process_parameters);

        if(c.running()) {
          c.detach();
          return true;
        }
      } catch(...) {}

      return false;
    }
  }
}
