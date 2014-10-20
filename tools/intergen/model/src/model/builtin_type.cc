/**
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

#include "model/builtin_type.h"

#include "model/constant.h"

namespace codegen {

/*
 * Boolean type
 */
Boolean::Boolean() {
  constants_.push_back(Constant(this, "false"));
  constants_.push_back(Constant(this, "true"));
}

Boolean::~Boolean() {
}

TypeCodeGenerator* Boolean::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForBoolean(this);
  return code_generator;
}

const ConstantsCreator* Boolean::SupportsConstants() const {
  return this;
}

const codegen::Constant* Boolean::ConstantFor(
    const std::string& literal) const {
  Boolean::Constant constant(this, literal);
  if (constant.is_valid()) {
    return constant == constants_[0] ? &constants_[0] : &constants_[1];
  } else {
    return NULL;
  }
}

Integer::Integer(const Range& range)
    : range_(range) {
}

Integer::~Integer() {
}

const Integer::Range& Integer::range() const {
  return range_;
}

bool Integer::operator <(const Integer& that) const {
  if (range_.min() != that.range_.min())
    return range_.min() < that.range_.min();
  return range_.max() < that.range_.max();
}

TypeCodeGenerator* Integer::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForInteger(this);
  return code_generator;
}

const ConstantsCreator* Integer::SupportsConstants() const {
  return this;
}

const Constant* Integer::ConstantFor(const std::string& literal) const {
  Integer::Constant constant(this, literal);
  if (constant.is_valid()) {
    return &*constants_.insert(constant).first;
  } else {
    return NULL;
  }
}

Float::Float(const Range& range)
    : range_(range) {
}

Float::~Float() {
}

const Float::Range& Float::range() const {
  return range_;
}

bool Float::operator <(const Float& that) const {
  if (range_.min() != that.range_.min())
    return range_.min() < that.range_.min();
  return range_.max() < that.range_.max();
}

TypeCodeGenerator* Float::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForFloat(this);
  return code_generator;
}

const ConstantsCreator* Float::SupportsConstants() const {
  return this;
}

const Constant* Float::ConstantFor(const std::string& literal) const {
  Float::Constant constant(this, literal);
  if (constant.is_valid()) {
    return &*constants_.insert(constant).first;
  } else {
    return NULL;
  }
}

String::String(const Range& range)
    : length_range_(range) {
}

bool String::operator <(const String& that) const {
  if (length_range_.min() != that.length_range_.min())
    return length_range_.min() < that.length_range_.min();
  return length_range_.max() < that.length_range_.max();
}

const String::Range& String::length_range() const {
  return length_range_;
}

TypeCodeGenerator* String::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForString(this);
  return code_generator;
}

const ConstantsCreator* String::SupportsConstants() const {
  return NULL;
}

}  // namespace codegen
