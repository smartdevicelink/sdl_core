/**
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SHARED_PTR_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SHARED_PTR_H_

#include <assert.h>
#include <stddef.h>

#include "utils/macro.h"

namespace utils {
/**
 * @brief Shared pointer.
 *
 * Pointer to an object with reference counting.
 * Object will be automatically deallocated when last shared
 * pointer is destroyed.
 *
 * @tparam ObjectType Type of wrapped object.
 **/
template<typename ObjectType>
class SharedPtr {
  public:
    /**
     * @brief Constructor.
     *
     * Initialize shared pointer with wrapped object.
     * Reference counter will be initialized to 1.
     *
     * @param Object Wrapped object.
     **/
    SharedPtr(ObjectType* Object);

    SharedPtr();

    /**
     * @brief Copy constructor.
     *
     * Initialize shared pointer with another shared pointer.
     * Reference counter will be incremented.
     *
     * @param Other Other shared pointer.
     **/
    SharedPtr(const SharedPtr<ObjectType>& Other);

    /**
     * @brief Copy constructor.
     *
     * Initialize shared pointer with another shared pointer.
     * Reference counter will be incremented.
     *
     * @tparam OtherObjectType Type of other object pointer. This
     *                         allows creating a shared pointer to an
     *                         intstance of a base class from a shared
     *                         pointer to an instance of a class
     *                         inherited from this base class.
     *                         If OtherObjectType is not implicitly
     *                         convertible to ObjectType it will
     *                         cause a compile error.
     *
     * @param Other Other shared pointer.
     **/
    template<typename OtherObjectType>
    SharedPtr(const SharedPtr<OtherObjectType>& Other);

    /**
     * @brief Destructor.
     *
     * Decrement reference counter and destroy wrapped object
     * if reference counter reaches zero.
     **/
    ~SharedPtr(void);

    /**
     * @brief Assignment operator.
     *
     * Drop reference to currently referenced object and add
     * reference to assigned object.
     *
     * @param Other Shared pointer to an object
     *              that must be referenced.
     *
     * @return Reference to this shared pointer.
     **/
    SharedPtr<ObjectType>& operator =(const SharedPtr<ObjectType>& Other);

    bool operator ==(const SharedPtr<ObjectType>& Other) const;

    /**
     * @brief Assignment operator.
     *
     * Drop reference to currently referenced object and add
     * reference to assigned object.
     *
     * @tparam OtherObjectType Type of other object pointer. This
     *                         allows creating a shared pointer to an
     *                         intstance of a base class from a shared
     *                         pointer to an instance of a class
     *                         inherited from this base class.
     *                         If OtherObjectType is not implicitly
     *                         convertible to ObjectType it will
     *                         cause a compile error.
     *
     * @param Other Shared pointer to an object
     *              that must be referenced.
     *
     * @return Reference to this shared pointer.
     **/
    template<typename OtherObjectType>
    SharedPtr<ObjectType>& operator =(const SharedPtr<OtherObjectType>& Other);

    /**
     * @brief Member access operator.
     *
     * @return Wrapped object.
     **/
    ObjectType* operator->(void) const;

    ObjectType& operator*() const;
    explicit operator bool() const;
    void reset();
    void reset(ObjectType* other);
    ObjectType* get();

    /**
     * @return true if mObject not NULL
     */
    bool valid() const;

  private:
    // TSharedPtr needs access to other TSharedPtr private members
    // for shared pointers type casts.
    template<typename OtherObjectType>
    friend class SharedPtr;

    /**
     * @brief Drop reference to wrapped object.
     *
     * If reference counter reaches zero object and its reference
     * counter will be deallocated.
     **/
    void dropReference(void);

    /**
     * @brief Wrapped object.
     **/
    ObjectType* mObject;

    /**
     * @brief Pointer to reference counter.
     **/
    unsigned int* mReferenceCounter;
};

template<typename ObjectType>
inline utils::SharedPtr<ObjectType>::SharedPtr(ObjectType* Object)
  : mObject(NULL),
    mReferenceCounter(new unsigned int(1)) {
  DCHECK(Object);
  mObject = Object;
}

template<typename ObjectType>
inline utils::SharedPtr<ObjectType>::SharedPtr()
  : mObject(0),
    mReferenceCounter(0) {
}

template<typename ObjectType>
inline utils::SharedPtr<ObjectType>::SharedPtr(
  const SharedPtr<ObjectType>& Other)
  : mObject(0),
    mReferenceCounter(0) {
  *this = Other;
}

template<typename ObjectType>
template<typename OtherObjectType>
inline utils::SharedPtr<ObjectType>::SharedPtr(
  const SharedPtr<OtherObjectType>& Other)
  : mObject(0),
    mReferenceCounter(0) {
  *this = Other;
}

template<typename ObjectType>
inline utils::SharedPtr<ObjectType>::~SharedPtr(void) {
  dropReference();
}

template<typename ObjectType>
inline utils::SharedPtr<ObjectType>&
utils::SharedPtr<ObjectType>::operator=(const SharedPtr<ObjectType>& Other) {
  return operator=<ObjectType>(Other);
}

template<typename ObjectType>
inline bool utils::SharedPtr<ObjectType>::operator ==(
  const SharedPtr<ObjectType>& Other) const {
  return (mObject == Other.mObject);
}

template<typename ObjectType>
template<typename OtherObjectType>
inline utils::SharedPtr<ObjectType>&
utils::SharedPtr<ObjectType>::operator=(
  const SharedPtr<OtherObjectType>& Other) {
  dropReference();

  mObject = Other.mObject;
  mReferenceCounter = Other.mReferenceCounter;

  if (0 != mReferenceCounter) {
    ++*mReferenceCounter;
  }

  return *this;
}

template<typename ObjectType> ObjectType*
utils::SharedPtr<ObjectType>::operator->(void) const {
  return mObject;
}

template<typename ObjectType> ObjectType&
utils::SharedPtr<ObjectType>::operator*() const {
  // TODO (PV) : change when decided upon DCHECK
  assert(mObject);
  return *mObject;
}

template<typename ObjectType>
utils::SharedPtr<ObjectType>::operator bool() const {
  return 0 != mObject;
}

template<typename ObjectType> void utils::SharedPtr<ObjectType>::reset() {
  reset(0);
}

template<typename ObjectType> void utils::SharedPtr<ObjectType>::reset(
  ObjectType* other) {
  DCHECK(other);
  dropReference();
  mObject = other;
  mReferenceCounter = new unsigned int(1);
}

template<typename ObjectType>
inline void SharedPtr<ObjectType>::dropReference(void) {
  if (0 != mReferenceCounter) {
    if (0 == --*mReferenceCounter) {
      delete mObject;
      mObject = 0;

      delete mReferenceCounter;
      mReferenceCounter = 0;
    }
  }
}

template<typename ObjectType>
ObjectType* SharedPtr<ObjectType>::get() {
  delete mReferenceCounter;
  mReferenceCounter = 0;
  ObjectType* res = mObject;
  mObject = NULL;
  return res;
}

template<typename ObjectType>
inline bool SharedPtr<ObjectType>::valid() const {
  return (mObject != NULL);
}

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SHARED_PTR_H_
