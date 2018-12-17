#ifndef ERROR_OR_H
#define ERROR_OR_H
#include <string>
#include <memory>
#include "utils/macro.h"

namespace utils {

class StoragePolicy_CopyValue {};

class StoragePolicy_HandleReference {};

/**
 * @brief The Optional class is able to keep value, manage it it is empty and
 * specify error code.
 * Can be used as return value of function that is not guarantee that vaue will
 * be returned
 *
 */
template <typename ObjectType, typename ErrorType = std::string, typename StoragePolicy=StoragePolicy_HandleReference>
class Optional {
 public:
  /**
   * @brief The OptionalEmpty enum enum with one value to specify that Optional
   * is not initialized
   */
  enum OptionalEmpty { EMPTY };

  /**
   * @brief Optional constructor with object initialization
   * @param object object to initialize Optional
   */
  Optional(ObjectType& object)
      : object_(&object), error_(), is_initialized_(true) {}

  /**
   * @brief Optional constructor with object and error initialization
   * @param object object to initialize Optional
   * @param error error code initialization
   */
  Optional(ObjectType& object, ErrorType error)
      : object_(&object), error_(error), is_initialized_(true) {}

  /**
   * @brief Optional constructir without object initialization
   * @param empty manadatory parameter for explicit specifying that Optional is
   * empty
   * @param error error code initialization
   */
  Optional(OptionalEmpty empty, ErrorType error)
      : object_(nullptr), error_(error), is_initialized_(false) {}

  /**
   * @brief Optional empty optional initialization without specifying error code
   * @param empty manadatory parameter for explicit specifying that Optional is
   * empty
   */
  Optional(OptionalEmpty empty)
      : object_(nullptr), error_(), is_initialized_(false) {}

  /**
   * @brief operator bool operator for checking if optional is initialized
   */
  operator bool() const {
    return is_initialized_;
  }

  /**
   * @brief operator * access to object
   * @return
   */
  ObjectType& operator*() const {
    DCHECK(is_initialized_);
    return *object_;
  }

  ErrorType error() const {
    return error_;
  }

 private:
  ObjectType* object_;
  ErrorType error_;
  bool is_initialized_;
};



template <typename ObjectType, typename ErrorType>
class Optional <ObjectType, ErrorType, StoragePolicy_CopyValue>{
 typedef Optional <ObjectType, ErrorType, StoragePolicy_CopyValue> ClassName ;
 public:
  /**
   * @brief The OptionalEmpty enum enum with one value to specify that Optional
   * is not initialized
   */
  enum OptionalEmpty { EMPTY };

  /**
   * @brief Optional constructor with object initialization
   * @param object object to initialize Optional
   */
  Optional(const ClassName& copy)
      : object_(new ObjectType(*copy)), error_(copy.error()), is_initialized_(copy) {}

  /**
   * @brief Optional constructor with object initialization
   * @param object object to initialize Optional
   */
  Optional(const ObjectType& object)
      : object_(new ObjectType(object)), error_(), is_initialized_(true) {}

  /**
   * @brief Optional constructor with object and error initialization
   * @param object object to initialize Optional
   * @param error error code initialization
   */
  Optional(ObjectType& object, ErrorType error)
      : object_(new ObjectType(object)), error_(error), is_initialized_(true) {}

  /**
   * @brief Optional constructir without object initialization
   * @param empty manadatory parameter for explicit specifying that Optional is
   * empty
   * @param error error code initialization
   */
  Optional(OptionalEmpty empty, ErrorType error)
      : object_(nullptr), error_(error), is_initialized_(false) {}

  /**
   * @brief Optional empty optional initialization without specifying error code
   * @param empty manadatory parameter for explicit specifying that Optional is
   * empty
   */
  Optional(OptionalEmpty empty)
      : object_(nullptr), error_(), is_initialized_(false) {}

  /**
   * @brief operator bool operator for checking if optional is initialized
   */
  operator bool() const {
    return is_initialized_;
  }

  /**
   * @brief operator * access to object
   * @return
   */
  ObjectType& operator*() const {
    DCHECK(is_initialized_);
    return *object_;
  }

  ErrorType error() const {
    return error_;
  }

 private:
  std::unique_ptr<ObjectType> object_;
  ErrorType error_;
  bool is_initialized_;
};


template <typename ObjectType, typename ErrorType = std::string>
using OptionalVal = Optional<ObjectType, ErrorType, StoragePolicy_CopyValue>;
}  // utils utils
#endif  // ERROR_OR_H
