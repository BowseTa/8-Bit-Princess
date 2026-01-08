# Game Framework Test1
# 8-Bit Princess  
*(Game Framework Test1)*

**Author:** Ranwei Zhang  
**Student ID:** 14839719  
**Course:** COMP710 – Game Programming  
**Submission Date:** 16 May 2025  

---

## Overview
**8-Bit Princess** is a 2D game developed in C++ using a custom game framework as part of the COMP710 Game Programming course.  
The project demonstrates core gameplay systems including real-time player movement, combat mechanics, scene transitions, input handling, and basic game state management.

This repository contains **source code only**. Generated build files and third-party runtime binaries are intentionally excluded to follow standard version control and software development practices.

---

## Technologies Used
- C++
- OpenGL
- SDL2
- SDL2_image
- SDL2_ttf
- GLEW
- FMOD
- Dear ImGui
- Microsoft Visual Studio (Windows)

---

## Controls

### Keyboard
- **Arrow Keys** – Move  
- **A** – Attack  
- **Enter** – Interact with door (requires key)  
- **Q** – Quit game  
- **R** – Restart game  

### Xbox Controller
- **Left Stick** – Move  
- **A Button** – Attack  
- **B Button** – Interact with door  
- **X Button** – Quit game  
- **Y Button** – Restart game  

---

## Project Structure
```text
.
├─ assets/            # Game assets (textures, audio, fonts, maps)
├─ shaders/           # OpenGL shader source files
├─ imgui/             # Dear ImGui source files
├─ game/              # Game-specific logic and scene code
├─ docs/              # Documentation (if applicable)
├─ *.cpp / *.h        # Core engine and gameplay source files
├─ README.md
├─ .gitignore
└─ Game Framework Test1.sln / .vcxproj

---

## Build Instructions
**Requirements**
-**Windows**
-**Microsoft Visual Studio (recommended: Visual Studio 2019 or later)**
-**The following third-party libraries installed and configured in Visual Studio:**
--**SDL2**
--**SDL2_image**
--**SDL2_ttf**
--**GLEW**
--**FMOD**

Note: Runtime DLLs (e.g. SDL2.dll, fmod.dll) and compiled binaries are not included in this repository.

---

## Steps to Build
-**Clone the repository:**
git clone <repository-url>
-**Open the solution file:**
Game Framework Test1.sln
-**Configure include directories and linker settings for the required libraries.**
-**Build the project using:**
Configuration: Debug or Release
Platform: x64
-**Run the game from Visual Studio.**

The required DLLs will be resolved based on the local development environment.

---

## About Excluded Files

The following files and folders are intentionally excluded from version control:
-**Visual Studio generated folders (.vs/, Debug/, Release/, x64/)**
-**Compiled binaries (.exe, .dll, .pdb)**
-**User-specific configuration files (e.g. .vcxproj.user, imgui.ini)**

This keeps the repository lightweight and focused on source code.

---

## Notes
-**This project is intended to be built from source.**
-**A pre-built executable is not included in the repository.**
-**The repository structure follows standard C++ and game development practices.**

---

Author

Ranwei Zhang