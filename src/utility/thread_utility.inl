/**
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **/
#include "thread_utility.h"

#include <algorithm>
#include <future>
#include <thread>
#include "../3rdparty/ccia/join_threads.h"
#include "general_utility.h"

namespace pixpaint
{
namespace thread_utils
{
  template<class InputIt, class Func>
  void for_each(InputIt first, InputIt last, Func func)
  {
    static constexpr auto MIN_COUNT_PER_THREAD = 25;

    auto length = std::distance(first, last);
    if(!length) {
      return;
    }

    auto max_threads = (length + MIN_COUNT_PER_THREAD - 1) / MIN_COUNT_PER_THREAD;
    auto hardware_threads = std::thread::hardware_concurrency();
    auto number_of_threads = general_utils::min<decltype(std::thread::hardware_concurrency())>(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    auto block_size = length / number_of_threads;

    std::vector<std::future<void>> futures(number_of_threads - 1);
    std::vector<std::thread> threads(number_of_threads - 1);
    join_threads joiner(threads);

    auto block_start = first;
    for(unsigned int i = 0; i < (number_of_threads - 1); ++i) {
      auto block_end = block_start;
      std::advance(block_end, block_size);
      std::packaged_task<void()> task([block_start, block_end, &func]
      {
        std::for_each(block_start, block_end, func);
      });
      futures[i] = task.get_future();
      threads[i] = std::thread(std::move(task));
      block_start = block_end;
    }

    std::for_each(block_start, last, func);
    for(auto& f : futures) {
      f.get();
    }
  }
}
}
