# Spotlight Launcher

A lightweight, fast, and modern Spotlight-style application launcher for Linux, built with **C++20** and **Qt 6**. Inspired by macOS Spotlight, it provides instant access to file searching, custom commands, app launching, Godot project management, and a high-performance math engine.

![Spotlight Preview](https://via.placeholder.com/800x400.png?text=Spotlight+App+Preview)

---

## Key Features

* **macOS-Style Positioning & Auto-Resizing**
  * Spawns floating in the top-center (~25% screen height).
  * Smoothly expands when search results appear and automatically shrinks back down to a compact capsule pill when cleared.
* **Single-Instance IPC Architecture**
  * Utilizes `QLocalServer` and `QLocalSocket` to ensure only one instance runs in the background.
  * Launching the executable again via hotkeys toggles the existing window instantly without stacking orphan background processes.
* **ExprTk High-Performance Math Engine**
  * Evaluates mathematical expressions in real time using the header-only **ExprTk** library.
  * Pre-configured with build optimization flags to keep binary bloat minimal without sacrificing execution speed.
  * Access by typing `=` followed by your expression (e.g., `= 2^8 * sin(0.5)`).
* **Smart Indexer & Search Engine**
  * **File Search**: Instantly indexes and searches your system's files.
  * **Application Launcher**: Search and launch installed `.desktop` applications and custom system actions via `>` (e.g., `> terminal`, `> wifi on`).
  * **Godot Integration**: Quick-launch Godot engine projects directly using `godot:`.
* **Theming & Global Styling**
  * Full Qt Stylesheet (QSS) support with dynamic container framing and border-radius cap rendering.
  * Built-in Settings dialog to switch themes on the fly.

---

## Keyboard Shortcuts

| Shortcut | Action |
| :--- | :--- |
| **`Esc`** | Hide / Close the Spotlight window |
| **`Down Arrow`** | Move focus from search bar to result list |
| **`Enter / Return`** | Activate/Launch selected item |

---

## Building from Source

### Prerequisites

Ensure you have the following installed on your system:
* **C++ Compiler**: GCC or Clang supporting C++17/C++20
* **Build System**: CMake (v3.16+) & Ninja/Make
* **Qt 6 Framework**: `Qt6::Widgets`, `Qt6::Network` (or Qt Creator)

On Ubuntu/Debian-based systems:
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev
```

## License

Copyright (C) 2026 Austin ADev

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.