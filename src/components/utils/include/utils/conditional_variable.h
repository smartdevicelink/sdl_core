#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_

#if defined(OS_POSIX)
#include <pthread.h>
#else
#error Please implement conditional variable for your OS
#endif

#include "utils/macro.h"

namespace sync_primitives {
class AutoLock;

namespace impl {
#if defined(OS_POSIX)
typedef pthread_cond_t PlatformConditionalVariable;
#endif
} // namespace impl

/*
 * Conditional variable wrapper
 * Conditional variable is a thing that can be waited on
 * Wait is finished when other thread puts that thing in a signaled state
 * (or when timeout is over).
 * Data that is conditionally accessed should be protected by
 * a Lock and that lock must be taken before starting to Wait.
 * When wait is performed, Lock is temporarly released.
 * When wait is finished, Lock is captured back.
 * WARNING: Beware of Spurious wakeups
 * http://en.wikipedia.org/wiki/Spurious_wakeup
 * Thread can wake up from wait spuriously, without conditional
 * variable being actually set by other thread. This means
 * additional check should be made right after thread awakening
 * and if check fails thread should continue waiting.
 *
 * while(!DataReady()) cond_var.Wait(auto_lock);
 *
 */
class ConditionalVariable {
 public:
  enum WaitStatus { kNoTimeout, kTimeout };
  ConditionalVariable();
  ~ConditionalVariable();
  // Wakes up single thread that is waiting on this conditional variable
  void NotifyOne();
  // Wakes up all waiting threads
  void Broadcast();

  // Wait forever or up to milliseconds time limit
  void Wait(AutoLock& auto_lock);
  WaitStatus WaitFor(AutoLock& auto_lock, int milliseconds );
 private:
  impl::PlatformConditionalVariable cond_var_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ConditionalVariable);
};

} // namespace sync_primitives

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONDITIONAL_VARIABLE_H_