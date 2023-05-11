// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_ERROR_H
#define PLUSH_BITS_ERROR_H

#include <optional>
#include <string>

namespace plush {

// Flexible error class interface.
class Error {
public:
 virtual ~Error() {}
 [[nodiscard]] virtual std::string userFriendlyMessage() = 0;
};

// Basic error with a customizable message.
class BasicError final : public Error {
 std::string mErrorMessage;

public:
 [[nodiscard]] BasicError(std::string &&);
 [[nodiscard]] BasicError(std::string const &);

 [[nodiscard]] std::string userFriendlyMessage() override;
};

} // namespace plush

#endif // PLUSH_BITS_ERROR_H
