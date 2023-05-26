// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_DRIVER_INTERPRET_H
#define PLUSH_DRIVER_INTERPRET_H

#include "bits/Expect.h"
#include "driver/Options.h"

namespace plush::driver {

// Runs Plush as an interpreter with the provided options.
Expect<> interpret(Options const &options);

} // namespace plush::driver

#endif // PLUSH_DRIVER_INTERPRET_H
