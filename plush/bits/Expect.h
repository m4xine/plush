// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_EXPECT_H
#define PLUSH_BITS_EXPECT_H

#include <cassert>
#include <memory>
#include <type_traits>
#include <variant>

#include "bits/Error.h"
#include "bits/Unit.h"

namespace plush {

// Tagged union holding a success value or an error.
template <class Success = Unit>
class Expect {
 // Either Success or a derivative of Error.
 std::variant<Success, std::unique_ptr<Error>> mVariant;

public:
 constexpr bool isSuccess() const {
  return std::holds_alternative<Success>(mVariant);
 }
 constexpr bool isError() const { return !isSuccess(); }

protected:
 Success *successPtr() {
  assert(isSuccess());
  return std::get_if<Success>(&mVariant);
 }
 Success const *successPtr() const {
  assert(isSuccess());
  return std::get_if<Success>(&mVariant);
 }

public:
 [[nodiscard]] constexpr Expect(Success &&value)
   : mVariant {std::forward<Success>(value)} {}
 [[nodiscard]] constexpr Expect(Success const &value) : mVariant {value} {}

 // Construct Expect as an error with any derivative of Error.
 template <class E, typename = std::enable_if_t<
                      std::is_base_of_v<Error, std::decay_t<E>>>>
 [[nodiscard]] constexpr Expect(E &&value)
   : mVariant {std::make_unique<std::decay_t<E>>(std::forward<E>(value))} {}

 constexpr Success        &success()        &{ return *successPtr(); }
 constexpr Success const  &success() const  &{ return *successPtr(); }
 constexpr Success       &&success()       &&{ return *successPtr(); }
 constexpr Success const &&success() const && { return *successPtr(); }

 // Take ownership of the contained error value.
 template <class E>
 [[nodiscard]] std::enable_if_t<std::is_base_of_v<Error, E>, E> takeError() {
  assert(isError());
  return *dynamic_cast<E *>(
    std::get_if<std::unique_ptr<Error>>(&mVariant)->release());
 }

 constexpr operator bool() const { return isSuccess(); }

 constexpr Success        &operator*()        &{ return success(); }
 constexpr Success const  &operator*() const  &{ return success(); }
 constexpr Success       &&operator*()       &&{ return success(); }
 constexpr Success const &&operator*() const && { return success(); }
 constexpr Success        &operator->() { return success(); }
 constexpr Success const  &operator->() const { return success(); }
};

} // namespace plush

#endif // PLUSH_BITS_EXPECT_H
