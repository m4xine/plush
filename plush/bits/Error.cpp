// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#include "bits/Error.h"

namespace plush {

[[nodiscard]] BasicError::BasicError(std::string &&errMsg)
  : mErrorMessage {std::move(errMsg)} {}

[[nodiscard]] BasicError::BasicError(std::string const &errMsg)
  : mErrorMessage {errMsg} {}

[[nodiscard]] std::string BasicError::userFriendlyMessage() {
 return mErrorMessage;
}

} // namespace plush
