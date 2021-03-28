#ifndef OS_SPECIFIC_FUNCTIONS_H
#define OS_SPECIFIC_FUNCTIONS_H

#include <string>

namespace pixpaint
{
namespace os_specific
{
  bool callProcess(const std::string& process_name,
                   const std::string& process_parameters);
  std::string getHomePath();
}
}

#endif // OS_SPECIFIC_FUNCTIONS_H
