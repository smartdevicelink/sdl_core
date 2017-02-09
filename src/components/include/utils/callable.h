#ifndef SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H
#define SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H

#include "utils/macro.h"

namespace utils {
/**
 * @brief The Callable class allows
 *  to create functor to call in other context
 */
class Callable {
 public:
  virtual void operator()() const = 0;
  virtual ~Callable() {}
};

/**
 * @brief The CallNothing class functior that to nothing
 */
class CallNothing : public Callable {
  // Callable interface
 public:
  void operator()() const OVERRIDE {}
};
}  // namespace utils
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_CALLABLE_H
