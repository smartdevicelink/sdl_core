/* Copyright (C) 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_THREADS_PTHREAD_ANDROID_H
#define SRC_COMPONENTS_INCLUDE_UTILS_THREADS_PTHREAD_ANDROID_H
#include "pt-internal.h"

#define PTHREAD_CANCELED ((void *) -1)
#define PTHREAD_CANCEL_ENABLE 0x00000010
#define PTHREAD_CANCEL_DISABLE 0x00000000

inline void pthread_cancel_handler(int signum) {
        pthread_exit(0);
    }

inline void pthread_init() {
        struct sigaction sa;
        struct pthread_internal_t *p = (struct pthread_internal_t *) pthread_self();

        if (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_HANDLER)
            return;

        // set thread status as pthread_create should do.
        // ASYNCROUNOUS is not set, see pthread_setcancelstate(3)
        p->attr.flags |= PTHREAD_ATTR_FLAG_CANCEL_HANDLER | PTHREAD_ATTR_FLAG_CANCEL_ENABLE;

        sa.sa_handler = pthread_cancel_handler;
        sigemptyset(&(sa.sa_mask));
        sa.sa_flags = 0;

        sigaction(SIGRTMIN, &sa, NULL);
    }

inline static void call_exit() {
        pthread_exit(0);
    }

inline int __pthread_do_cancel(struct pthread_internal_t *p) {

        if (p == (struct pthread_internal_t *) pthread_self())
            call_exit();
        else if (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_HANDLER)
            pthread_kill((pthread_t) p, SIGRTMIN);
        else
            pthread_kill((pthread_t) p, SIGTERM);

        return 0;
    }

inline void pthread_testcancel() {
        struct pthread_internal_t *p = (struct pthread_internal_t *) pthread_self();
        int cancelled;

        pthread_init();

        pthread_mutex_lock(&p->cancel_lock);
        cancelled = (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_ENABLE) &&
                    (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_PENDING);
        pthread_mutex_unlock(&p->cancel_lock);

        if (cancelled)
            pthread_exit(PTHREAD_CANCELED);
    }

inline int pthread_setcancelstate(int state, int *oldstate) {
        struct pthread_internal_t *p = (struct pthread_internal_t *) pthread_self();
        int newflags;

        pthread_init();

        switch (state) {
            default:
                return EINVAL;
            case PTHREAD_CANCEL_ENABLE:
            case PTHREAD_CANCEL_DISABLE:
                break;
        }

        pthread_mutex_lock(&p->cancel_lock);
        if (oldstate)
            *oldstate = p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_ENABLE;

        if (state == PTHREAD_ATTR_FLAG_CANCEL_ENABLE)
            p->attr.flags |= PTHREAD_ATTR_FLAG_CANCEL_ENABLE;
        else
            p->attr.flags &= ~PTHREAD_ATTR_FLAG_CANCEL_ENABLE;
        newflags = p->attr.flags;
        pthread_mutex_unlock(&p->cancel_lock);

        if ((newflags & PTHREAD_ATTR_FLAG_CANCEL_PENDING) &&
            (newflags & PTHREAD_ATTR_FLAG_CANCEL_ENABLE) &&
            (newflags & PTHREAD_ATTR_FLAG_CANCEL_ASYNCRONOUS))
            __pthread_do_cancel(p);

        return 0;
    }

inline int pthread_cancel(pthread_t t) {
        int err = 0;
        struct pthread_internal_t *p = (struct pthread_internal_t *) t;

        pthread_init();

        pthread_mutex_lock(&p->cancel_lock);
        if (p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_PENDING) {
            pthread_mutex_unlock(&p->cancel_lock);
            return 0;
        }

        p->attr.flags |= PTHREAD_ATTR_FLAG_CANCEL_PENDING;

        if (!(p->attr.flags & PTHREAD_ATTR_FLAG_CANCEL_ENABLE)) {
            pthread_mutex_unlock(&p->cancel_lock);
            return 0;
        }

        pthread_mutex_unlock(&p->cancel_lock);
        err = __pthread_do_cancel(p);

        return err;
    }
#endif //SRC_COMPONENTS_INCLUDE_UTILS_THREADS_PTHREAD_ANDROID_H
