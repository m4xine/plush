// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_UNIT_H
#define PLUSH_BITS_UNIT_H

namespace plush {

// Unit type.
struct Unit {
 constexpr explicit Unit() {}
};

// Unit constructor.
constexpr inline Unit unit {};

} // namespace plush

#endif // PLUSH_BITS_UNIT_H
