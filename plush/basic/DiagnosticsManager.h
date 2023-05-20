// SPDX-FileCopyrightText: Copyright (c) 2023, Maxine DeAndrade
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#ifndef PLUSH_BASIC_DIAGNOSTICSMANAGER_H
#define PLUSH_BASIC_DIAGNOSTICSMANAGER_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "basic/SourceManager.h"
#include "bits/Doc.h"

namespace plush {

// Interface for displaying user-friendly diagnostic information.
class Diagnostic {
public:
 using UPtr = std::unique_ptr<Diagnostic>;

 // Diagnostic severity.
 enum Level : std::uint8_t {
  // Informative diagnostic.
  NOTE,
  // Cautious diagnostic.
  WARNING,
  // Fatal diagnostic.
  ERROR
 };

 // Converts a Level to its string representation.
 static std::string levelToString(Level level);

 virtual ~Diagnostic() {}

 virtual Level                       level() const = 0;
 virtual SourceInfo                 *sourceInfo() const { return nullptr; }
 virtual std::optional<SourceRegion> sourceRegion() const {
  return std::nullopt;
 }
 virtual Doc doc() const = 0;
};

// Manages diagnostics and displaying them to the user.
class DiagnosticsManager {
 // Contains every added Diagnostic derivative.
 std::vector<Diagnostic::UPtr> mDiagnostics;

 // Displays each added diagnostic.
 void display() const;

public:
 DiagnosticsManager();
 DiagnosticsManager(DiagnosticsManager &&)                 = delete;
 DiagnosticsManager(DiagnosticsManager const &)            = delete;
 DiagnosticsManager &operator=(DiagnosticsManager &&)      = delete;
 DiagnosticsManager &operator=(DiagnosticsManager const &) = delete;

 // Add a derivative of Diagnostic to be displayed to the user.
 template <class Derived, typename = std::enable_if_t<std::is_base_of_v<
                            Diagnostic, std::decay_t<Derived>>>>
 void add(Derived &&derived) {
  mDiagnostics.push_back(
    std::make_unique<std::decay_t<Derived>>(std::forward<Derived>(derived)));
 }

 // Checks if this diagnostic manager contains a diagnostic of considerable
 // severity.
 bool status() const;

 // Displays and clears every stored diagnostic.
 bool dump();
};

} // namespace plush

#endif // PLUSH_BASIC_DIAGNOSTICSMANAGER_H
