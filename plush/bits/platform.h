// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BITS_PLATFORM_H
#define PLUSH_BITS_PLATFORM_H

#if _WIN32 || _WIN64
#if _WIN64
#define PLUSH_64BIT
#else
#define PLUSH_32BIT
#endif
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define PLUSH_64BIT
#else
#define PLUSH_32BIT
#endif
#else
#error "Unsupported platform, Define PLUSH_64BIT or PLUSH_32BIT"
#endif

#endif // PLUSH_BITS_PLATFORM_H
