// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_HASH_H
#define PLUSH_BITS_HASH_H

#include <cstdint>
#include <string_view>

#include "bits/platform.h"

namespace plush::hash {

// Hash a string with the FNV-1a hashing algorithm.
constexpr std::size_t fnv1a(std::string_view string) {
 constexpr std::size_t SEED =
#ifdef PLUSH_64BIT
   14695981039346656037ULL
#else
   2166136261U
#endif
   ;
 constexpr std::size_t PRIME =
#ifdef PLUSH_64BIT
   1099511628211ULL
#else
   16777619
#endif
   ;

 std::uint64_t hash = SEED;
 for (char c : string) hash = (static_cast<std::uint8_t>(c) ^ hash) * PRIME;
 return hash;
}

} // namespace plush::hash

#endif // PLUSH_BITS_HASH_H
