# Spotlight User Guide

Spotlight is a lightweight, high-performance desktop launcher built in C++ and Qt. It allows you to search files, solve math equations live, run system commands, and trigger custom app workflows using intuitive search prefixes.

---

## 🚀 Basic Controls & Shortcuts

| Action | Shortcut / Trigger | Description |
| :--- | :--- | :--- |
| **Open / Close** | Global Shortcut | Toggle Spotlight from anywhere |
| **Dismiss** | `Escape` | Close the Spotlight window |
| **Navigate Results** | `Down Arrow` | Move focus into the search results list |
| **Execute Selected** | `Enter` | Launch or execute the highlighted item |

---

## 🔀 Search Prefixes & Features

Spotlight uses **Prefix Triggers** to switch between different operational modes.

### 1. Live Math Mode (`=`)
Type `=` followed by an arithmetic expression to calculate results live as you type.

* **Trigger:** `=`
* **Examples:**
  * `= 12 * 4` -> `48`
  * `= (100 - 25) / 5` -> `15`
  * `= 2^8` -> `256`
* **Supported Operators:** Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`), Modulo (`%`), Exponents (`^`), and Parentheses `()`.

---

### 2. Command Mode (`>`)
Type `>` to launch pre-defined actions or execute terminal commands directly in the system shell.

* **Trigger:** `>`
* **System Commands:**
  * `> spotlight settings` or `> settings` — Opens the Spotlight Configuration Window.
  * `> terminal` — Launches the default system terminal emulator.
  * `> wifi on` / `> wifi off` — Toggles network connectivity.
* **Direct Shell Execution:**
  * Type any terminal command (e.g., `> brave`, `> htop`, `> code .`) to launch processes directly.

---

### 3. Godot Integration (`godot:`)
Access Godot engine workflows and project directories directly.

* **Trigger:** `godot:`
* **Examples:**
  * `godot:` — Displays quick actions (Open Projects Folder, Launch Engine).
  * `godot: platformer` — Searches and opens a specific project.

---

### 4. System File Search (Default)
Typing without a prefix automatically searches your indexed system directories in real-time.

* **Trigger:** *None (default behavior)*
* **Examples:** `document.pdf`, `report`, `.cpp`
* **Indexed Paths:** User Home directory (`~/`), `/usr/bin/`, and `/usr/share/applications/`.
* **Behavior:** Pressing `Enter` opens the selected file or directory with your system's default application.

---

## ⚙️ Configuration & Customization

* Type `> settings` to open the Spotlight settings dialog.
* **Adding New Commands:** Open `searchengine.cpp` and locate the `commandRegistry` array to map custom keywords to actions.