# 2048 #
**A sliding tile puzzle game desktop application as a clone for the popular 2048 game written with Qt6 and C++20.**


---
<br>
<br>


## 🧰 Installation From Release ##
Download any of the following releases that fit your platform. Extract and run the executable.
- **Windows:** ``
- **macOS:** ``
- **Linux:** ``


---
<br>
<br>


## 🚀 Features ##
- **🎨 Clean, Intuitive GUI:** Minimalist design focused on gameplay with smooth animations, responsive controls, and clear tile feedback.
- **✨ Smooth Tile Animations:** Animated tile movements and merges with satisfying visual effects that enhance the gameplay experience.
- **🧩 Persistent Game State:** Save your high scores. Game statistics and window preferences persist between sessions.
- **🖼️ Responsive Window Layouts:** Adapts seamlessly to different window sizes and screen resolutions without breaking the user interface.
- **🛡️ Full Application Support:** Proper window icons, system tray integration, keyboard shortcuts, and native OS integration for a polished desktop experience.
- **🖥️ Cross-Platform Support (Windows, macOS, Linux):** Built with Qt6 to run natively on all major operating systems with consistent behavior.
- **📚 Fully Documented:** Well-commented source code, inline documentation, and developer guides for anyone wanting to contribute or modify the project.


---
<br>
<br>


## 🔥 Screenshots ##
<p align="left">
  <img src="media/game-start.png" width="400">
  <img src="media/game-play.png" width="400">
  <img src="media/game-over.png" width="400">
</p>


---
<br>
<br>


## 🛠️ Building From Source ##
### 🔹 Install Project Requirements ###
To build **2048** from source, install the following as they are all required.
- **Qt 6.10+** or higher development libraries.
- **C++ 20+** capabilities. Qt6 will come with the necessary compiler to build the project.
- **CMake 3.16+** for building the project.
- **Git** to clone the 2048 repository.
### 🔹 Install Qt Development Libraries ###
- **Windows:** Install Qt from [qt.io](https://www.qt.io/development/download)
- **macOS:**
    ```bash
    brew install qt6
    ```
- **Linux:**
    ```bash
    sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential
    ```
### 🔹 Clone the Repository ###
```bash
cd /path/to/your/desired/directory
git clone https://github.com/connortsempf/2048.git
cd 2048
```
### 🔹 Create the Build Directory ###
```bash
mkdir build
cd build
```
### 🔹 Build the Project Using CMake ###
```bash
## Configure the Build ##
cmake ..

## Only If The Qt Package isn't Visisble in the Path and Configuration Fails, Point to It Manually ##
cmake .. -DQT6_DIR="/path/to/your/Qt/package/6.10.x/compiler/"

## Generate the Application ##
cmake --build .
```
### 🔹 Run the Application ###
- The output binaries are generated in the `/bin` directory
- Find and run the executable from there


---
<br>
<br>


## 📄 License & Attribution ##
This is an unofficial recreation of the 2048 game. This project is licensed under the MIT License - see the LICENSE file for details.
- Built with [Qt](https://www.qt.io/)
- Inspired by [2048](https://www.2048.org/)
