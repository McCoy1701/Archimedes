# Archimedes

A lightweight, cross-platform C game development framework built on SDL2 that provides essential 2D graphics, audio, input handling, and UI widget functionality.

## Build
To build natively
```
make native
./bin/native
```

To build the shared object file also install/uninstall them from usr/lib/ and usr/include
```
make shared

make install
make uninstall
```

To build Archimedes directly for Emscripten this will build a index/index.js
that can be use almost anywhere on the web.
*This isn't how you're supposed to use Archimedes, but it's a good way to test if everything is setup right*
```
make EM
python3 -m http.server 8000 //you can go to localhost:8000/index for a basic display
```

To build static archive for Emscripten use
*this is installed in your projects lib/*
```
make EMARCH
```

## Overview

Archimedes is a minimalist game development library written in C that abstracts common SDL2 operations into a clean, easy-to-use API. The framework is designed for developers who want to create 2D games and applications without dealing with the complexity of raw SDL2 programming. It supports both native compilation and web deployment through Emscripten, making it perfect for cross-platform game development.

The library follows a modular architecture with separate components for graphics rendering, audio playback, input management, text rendering, image handling, and UI widgets. All functionality is exposed through a single header file (`Archimedes.h`) and can be easily integrated into existing projects or used as the foundation for new games.

## Features

### 🎨 Graphics & Rendering
- **Primitive Drawing**: Points, lines, circles, triangles, and rectangles with filled variants
- **Image Support**: PNG image loading with caching system for optimized memory usage
- **Texture Management**: Automatic texture loading and management with SDL2 backend
- **Scene Management**: Simplified scene preparation and presentation workflow

### 🎵 Audio System
- **Sound Effects**: Load and play audio clips with SDL2_mixer integration
- **Audio Initialization**: Automatic audio device setup and configuration
- **Format Support**: Multiple audio format support through SDL2_mixer
- **Playback Control**: Simple API for triggering sound effects in games

### 🎮 Input Handling
- **Keyboard Input**: Full keyboard state tracking with 350+ key support
- **Mouse Support**: Complete mouse input including position, buttons, and scroll wheel
- **Event Processing**: Centralized input processing with easy state queries
- **Cross-platform**: Consistent input handling across different operating systems

### 🖼️ Text & UI
- **Font Rendering**: TTF font support with multiple font loading capabilities
- **Text Alignment**: Left, center, and right text alignment options
- **UI Widgets**: Button, slider, input field, select dropdown, and control widgets
- **Widget System**: JSON-based widget configuration with event handling

### ⚙️ Core Systems
- **Delta Time**: Frame-rate independent timing system for smooth animations
- **Cross-platform**: Native compilation for desktop and web deployment via Emscripten
- **Memory Management**: Efficient caching systems for images and textures
- **Error Handling**: Comprehensive logging system with multiple severity levels
