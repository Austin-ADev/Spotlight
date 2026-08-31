# Spotlight Launcher

A lightweight, fast, and modern Spotlight-style application launcher **exclusively for Linux**, built with **C++20** and **Qt 6**. Inspired by macOS Spotlight, it provides instant access to file searching, custom commands, app launching, Godot project management, and a high-performance math engine.

![Spotlight Preview](https://via.placeholder.com/800x400.png?text=Spotlight+App+Preview)

---

## Platform Support & Global Hotkey Setup

> **Note**: This application is built **only for Linux** operating systems.

Because Spotlight runs as a lightweight single-instance background daemon, you need to map a keyboard shortcut in your Linux desktop environment to launch/toggle it:

1. Open your system settings (**Settings > Keyboard > Keyboard Shortcuts** or **Custom Shortcuts**).
2. Create a new custom shortcut:
   * **Name**: `Toggle Spotlight`
   * **Command**: `/path/to/your/built/Spotlight` (or `spotlight` if placed in your `PATH`)
   * **Shortcut**: Set your preferred key combination (e.g., `Super + Space` or `Ctrl + Space`).

---

## Key Features

* **macOS-Style Positioning & Auto-Resizing**
  * Spawns floating in the top-center (~25% screen height).
  * Smoothly expands when search results appear and automatically shrinks back down to a compact capsule pill when cleared.
* **Single-Instance IPC Architecture**
  * Utilizes `QLocalServer` and `QLocalSocket` to ensure only one instance runs in the background.
  * Triggering your global hotkey executes the binary, which sends a toggle message to the existing background process without spawning duplicate processes.
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

## Building & Developing

### Prerequisites

Ensure you have the following installed on your Linux system:
* **C++ Compiler**: GCC or Clang supporting C++17/C++20
* **Build Tools**: CMake (v3.16+) & Ninja/Make
* **Qt 6 Framework**: `Qt6::Widgets`, `Qt6::Network`

On Ubuntu/Debian-based systems:
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev
```
Developing with Qt Creator
You can open and develop this project directly inside Qt Creator:

Open Qt Creator.

Select File > Open File or Project... and select CMakeLists.txt.

Select your Qt 6 Kit and click Configure Project.

Press Ctrl + R to build and run directly within the IDE.

Building from the Command Line
Clone the repository:

```bash
git clone [https://github.com/yourusername/Spotlight.git](https://github.com/yourusername/Spotlight.git)
cd Spotlight
Configure CMake & Build:
```

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
Run Spotlight:

```bash
./Spotlight
```
## License
**Copyright (C) 2026 Austin ADev**

**This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.**