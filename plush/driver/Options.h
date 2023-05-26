// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_DRIVER_OPTIONS_H
#define PLUSH_DRIVER_OPTIONS_H

#include <filesystem>
#include <vector>

#include "bits/Expect.h"

namespace plush::driver {

struct Options {
 bool                               debugEnabled {false};
 std::vector<std::filesystem::path> filePaths;

 static Expect<Options> parseArgs(int argc, char **argv);
};

} // namespace plush::driver

#endif // PLUSH_DRIVER_OPTIONS_H
