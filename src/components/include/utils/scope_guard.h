/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_SCOPE_GUARD_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_SCOPE_GUARD_H_

namespace utils {

/**
 * @brief The ScopeGuardImplBase class the class which allows to keep safety
 * raw pointer within local scope. The same ScopeGuard has been provided by
 * Andrej Alexandresku in the Loki library.
 *
 * The usage area example:
 *
 * Suppose user have to create some pointer in function call Init(). And in case
 * of some fail condition this pointer has to be freed. So in order to avoid
 * code duplicate as follows:
 *
 * bool SomeClass::Init() {
 *   memberObject_ = custom_allocate() // initialize member object with some
 *value
 *   if(!some_condition) {
 *     custom_release(memberObject();
 *     return false;
 *   }
 *
 * if(!other_condition) {
 *   custom_release(memberObject)
 *   return false;
 * }
 * return true;
 * }
 *
 * The user is able to use ScopeGuard, where possible to automatically call
 * custom release function. See example bellow:
 *
 * bool SomeClass::Init() {
 *   memberObject_ = custom_allocate() // initialize member object with some
 *value
 *   // The guard will call custom release function when it goes out of scope.
 *   ScopeGaurd guard = MakeGuard(custom_release, memberObject);
 *   if(!some_condition) {
 *     return false;
 *   }
 *
 * if(!other_condition) {
 *   return false;
 * }
 * // Here to avoid object releasing the user has to call Dismiss method.
 * // So no custom release function will be called after return statement.
 * guard.Dismiss();
 * return true;
 * }
 */
class ScopeGuardImplBase {
 public:
  /**
   * @brief ScopeGuardImplBase the ScopeGuards constructor.
   */
  ScopeGuardImplBase() : dismissed_(false) {}

  /**
   * @brief Dismiss function which allows to dismiss releasing of stored object.
   */
  void Dismiss() const {
    dismissed_ = true;
  }

 protected:
  /**
   * @brief ScopeGuardImplBase the copy constructor
   *
   * @param other the object that should be copied.
   */
  ScopeGuardImplBase(const ScopeGuardImplBase& other)
      : dismissed_(other.dismissed_) {
    other.Dismiss();
  }

  /**
   * Allows to safely execute release function(i.e. it will be called only
   * in case of releasing wasn't dismiss.)
   */
  template <typename T>
  void SafeExecute(T& t) {
    if (!t.dismissed_) {
      t.Execute();
    }
  }

  ~ScopeGuardImplBase() {}

  mutable bool dismissed_;

 private:
  // Disallow copying via assign operator.
  ScopeGuardImplBase& operator=(const ScopeGuardImplBase&);
};

/**
 * The class which allows to bind some parameter with free function.
 * I.E.
 * void release(char* p){delete p;}
 *
 * ScopeGuard guard = MakeGuard(release, p);
 *
 * So the parameter p will be passed to the specified function.
 */
template <typename Function, typename Parameter1>
class ScopeGuardImpl1 : public ScopeGuardImplBase {
 public:
  /**
   * @brief MakeGuard allows to create ScopeGuard object.
   *
   * @param fun function to call, when out of scope.
   *
   * @param p1 parameter which will be passed to the certain function.
   *
   * @return ScopeGuard object.
   */
  static ScopeGuardImpl1<Function, Parameter1> MakeGuard(Function fun,
                                                         const Parameter1& p1) {
    return ScopeGuardImpl1<Function, Parameter1>(fun, p1);
  }

  /**
    Execute the SafeExecute function in destructor.
   */
  ~ScopeGuardImpl1() {
    SafeExecute(*this);
  }

 protected:
  /**
   * @brief Execute allows to execute certain function with certain parameter.
   */
  void Execute() {
    fun_(p1_);
  }

  /**
   * @brief ScopeGuardImpl1 create ScopeGuard object.
   *
   * @param f function object.
   *
   * @param p1 parameter to to pass to the function object.
   */
  ScopeGuardImpl1(const Function& f, const Parameter1& p1) : fun_(f), p1_(p1) {}

 private:
  Function fun_;
  const Parameter1 p1_;

  friend class ScopeGuardImplBase;
};

/**
 * The class which allows to call some member function in case of
 * ScopeGuard object out of scope.
 * I.E.
 * vector<int> vec;
 *
 * // When function returns, the pop_back method for vector will be called.
 * void foo() {
 *   ScopeGuard guard = MakeObjGuard(vec, &vector::pop_back);
 *   vec.push_back(5);
 * }
 */
template <typename Obj, typename MemFun>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase {
 public:
  /**
   * @brief MakeObjGuard creates ScopeGuard object.
   *
   * @param obj object whose method will be called.
   *
   * @param memFun the method to call.
   *
   * @return ScopeGuard object.
   */
  static ObjScopeGuardImpl0<Obj, MemFun> MakeObjGuard(Obj& obj, MemFun memFun) {
    return ObjScopeGuardImpl0<Obj, MemFun>(obj, memFun);
  }

  /**
    Execute the SafeExecute function in destructor.
   */
  ~ObjScopeGuardImpl0() {
    SafeExecute(*this);
  }

 protected:
  /**
   * @brief Execute allows to execute certain function with certain parameter.
   */
  void Execute() {
    (obj_.*memFun_)();
  }

  /**
   * @brief ObjScopeGuardImpl0 Create ScopeGuard object.
   *
   * @param obj object whose method will be called.
   *
   * @param memFun the method to call.
   *
   * @return ScopeGuard object.
   */
  ObjScopeGuardImpl0(Obj& obj, MemFun memFun) : obj_(obj), memFun_(memFun) {}

 private:
  Obj& obj_;
  MemFun memFun_;
  friend class ScopeGuardImplBase;
};

/**
 * The class which allows to call some member function with certain parameter
 * in case of ScopeGuard object out of scope.

 */
template <typename Obj, typename MemFun, typename Parameter1>
class ObjScopeGuardImpl1 : public ScopeGuardImplBase {
 public:
  /**
   * @brief MakeObjGuard creates ScopeGuard object.
   *
   * @param obj object whose method will be called.
   *
   * @param memFun the method to call.
   *
   * @param p1 the parameter to pass to the member function.
   *
   * @return ScopeGuard object.
   */
  static ObjScopeGuardImpl1<Obj, MemFun, Parameter1> MakeObjGuard(
      Obj& obj, MemFun memFun, const Parameter1& p1) {
    return ObjScopeGuardImpl1<Obj, MemFun, Parameter1>(obj, memFun, p1);
  }

  /**
    Execute the SafeExecute function in destructor.
   */
  ~ObjScopeGuardImpl1() {
    SafeExecute(*this);
  }

 protected:
  /**
   * @brief Execute allows to execute certain function with certain parameter.
   */
  void Execute() {
    (obj_.*memFun_)(p1_);
  }

  /**
   * @brief MakeObjGuard creates ScopeGuard object.
   *
   * @param obj object whose method will be called.
   *
   * @param memFun the method to call.
   *
   * @param p1 the parameter to pass to the member function.
   *
   * @return ScopeGuard object.
   */
  ObjScopeGuardImpl1(Obj& obj, MemFun memFun, const Parameter1& p1)
      : obj_(obj), memFun_(memFun), p1_(p1) {}

 private:
  Obj& obj_;
  MemFun memFun_;
  const Parameter1 p1_;
  friend class ScopeGuardImplBase;
};

typedef const ScopeGuardImplBase& ScopeGuard;

template <typename F, typename P1>
ScopeGuardImpl1<F, P1> MakeGuard(F fun, P1 p1) {
  return ScopeGuardImpl1<F, P1>::MakeGuard(fun, p1);
}

template <typename Obj, typename MemFun>
ObjScopeGuardImpl0<Obj, MemFun> MakeObjGuard(Obj& obj, MemFun memFun) {
  return ObjScopeGuardImpl0<Obj, MemFun>::MakeObjGuard(obj, memFun);
}

template <typename Obj, typename MemFun, typename P1>
ObjScopeGuardImpl1<Obj, MemFun, P1> MakeObjGuard(Obj& obj,
                                                 MemFun memFun,
                                                 const P1& p1) {
  return ObjScopeGuardImpl1<Obj, MemFun, P1>::MakeObjGuard(obj, memFun, p1);
}
}
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_SCOPE_GUARD_H_
