/*
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_HELPERS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_HELPERS_H_
#include <algorithm>
/**
 * These helpers allows to simplify compare strategy between some objects.
 * Suppose user has some enum with value E with some numbers of possible values
 * enum E {V1, V2, V3, V5};
 * So we want to know if some user-input value is belong to one of the enum's
 *subset.
 * Usually user has to do next routine
 *
 * E input_value = V3;
 * if (input_value == V1 || input_value == V2 || input_value == V3) {
 *     do_some_stuff_here;
 * }
 *
 * How it suppose to be with these helpers methods:
 *
 *  E input_value = V3;
 * if (Compare<E, EQ, ONE>(input_value, V1, V2, V3)) {
 *     do_some_stuff;
 * }
 *
 * Also User is able to easily change compare startegy by using some set of
 * predefined startegies.
 */
namespace helpers {

template <typename T>
bool EQ(T what, T to) {
  return what == to;
}

template <typename T>
bool NEQ(T what, T to) {
  return !EQ<T>(what, to);
}

template <class U = bool>
bool ALL(U what, U to) {
  return what && to;
}

template <class U = bool>
bool ONE(U what, U to) {
  return what || to;
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to) {
  return CompareType(what, to);
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to, T to1) {
  return CmpStrategy(Compare<T, CompareType, CmpStrategy>(what, to),
                     Compare<T, CompareType, CmpStrategy>(what, to1));
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to, T to1, T to2) {
  return CmpStrategy(Compare<T, CompareType, CmpStrategy>(what, to, to1),
                     Compare<T, CompareType, CmpStrategy>(what, to2));
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to, T to1, T to2, T to3) {
  return CmpStrategy(Compare<T, CompareType, CmpStrategy>(what, to, to1, to2),
                     Compare<T, CompareType, CmpStrategy>(what, to3));
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to, T to1, T to2, T to3, T to4) {
  return CmpStrategy(
      Compare<T, CompareType, CmpStrategy>(what, to, to1, to2, to3),
      Compare<T, CompareType, CmpStrategy>(what, to4));
}

template <typename T,
          bool (*CompareType)(T, T),
          bool (*CmpStrategy)(bool, bool)>
bool Compare(T what, T to, T to1, T to2, T to3, T to4, T to5) {
  return CmpStrategy(
      Compare<T, CompareType, CmpStrategy>(what, to, to1, to2, to3),
      Compare<T, CompareType, CmpStrategy>(what, to4, to5));
}

template <typename Container>
bool in_range(const Container& container,
              const typename Container::value_type& value) {
  return std::find(container.begin(), container.end(), value) !=
         container.end();
}
}

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_HELPERS_H_
