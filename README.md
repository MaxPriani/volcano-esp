# 🚀 **Volcano's ESP** 🔥

## 🖼️ **Showcase - Screenshots**

Here are some screenshots of how the **ESP** looks in action:

![ESP Overlay](https://via.placeholder.com/500x300.png)
*ESP displaying boxes around players.*

![ESP Health](https://via.placeholder.com/500x300.png)
*Player health displayed with dynamic colors.*

## 📚 **Project Description**

**Volcano's ESP** is a personal integrative project developed for educational purposes to learn and deepen knowledge in advanced **C++** and **reverse engineering** concepts. The main goal of this project is to create an **overlay** for a game using techniques such as **memory reading**, **process capturing**, **offsets**, and **rendering** with **DirectX 11**. Key concepts implemented during development include:

- **Process Capturing**: Capturing and interacting with running processes using the **Windows API**.
- **Memory Reading**: Reading and modifying the memory of a running process to retrieve real-time information.
- **Offsets**: Using specific offsets to access information within a game's memory.
- **Overlays and Rendering**: Using DirectX 11 to create an overlay that displays relevant information about players in the game.
- **Reverse Engineering**: Understanding how a game’s internal processes work to extract information without direct access.
- **Linear Algebra**: Implementing the **view matrix** and coordinate transformations to project 3D world positions to 2D screen coordinates.
- **Object-Oriented Programming**: The project is implemented using **C++** and **object-oriented programming** concepts to efficiently organize code.

The project was developed in **C++** using object-oriented programming to manage the different responsibilities of the overlay and its components.

⚠️ **Important**: This project was developed **only for educational purposes**. It is not designed to be **undetectable** nor intended for use on servers with protection systems like **VAC** (Valve Anti-Cheat). **It is not recommended for use in online games or to cheat in secure servers.**

## 🔧 **Requirements**

To run this project on your machine, ensure that you have the following requirements:

- **Operating System**: Windows 10 or later.
- **Visual Studio 2022** or later with support for **C++20** and **DirectX 11**.
- **ImGui** for the graphical interface (must be included in **VC++ Directories**).
- **DirectX 11 SDK** for rendering interfaces.
- **CMake** (if you prefer to use it for build management).

## 🛠️ **Build in Visual Studio**

1. **Clone the repository** to your local machine:
   ```bash
   git clone https://github.com/yourusername/volcano-esp.git

## 🖥️ **ESP Features**

The **Volcano's ESP** overlay offers several useful features for analyzing the game state, such as:

- **ESP Box** 📦: Displays boxes around players, indicating their position and size.
- **Player Health** ❤️: Shows the health of players based on their health level, with the text color changing according to health.
- **Distance** 🏃‍♂️: Displays the distance between the local player and nearby enemies.
- **Player Names** 🏷️: Displays the player's name above their head.
- **Dynamic Configuration** ⚙️: Adjust values such as box thickness, text size, colors, and more.

### Hotkeys:
- **[F4]**: Toggle the interactive graphical interface on/off.

## 📝 **How It Works**

### External ESP:
This project is an **external ESP**, meaning it is not injected directly into the game's memory nor does it interact with it. Instead of modifying the game's memory, the ESP **reads the game's memory** to obtain information about the players' states (such as health, position, team, etc.) using specific **offsets**. Based on this information, the overlay is rendered on top of the game using **DirectX 11**. This approach makes the ESP safer to use as it doesn't require modifying or interacting directly with the game's memory.

### Overlay:
An **overlay** is a screen that sits on top of the game, displaying additional information without interfering with the game's flow. In this project, **DirectX 11** is used to create a transparent window that is placed over the game. This window continuously renders the ESP (information about players) while the game is running.

### Memory Reading:
The project uses the `Memory` class to read the game's process memory, looking for specific addresses (using **offsets**) that contain relevant information such as player health, name, etc.

### Coordinate Transformations:
Linear algebra is used to transform 3D world coordinates into 2D screen coordinates, allowing the overlay elements to be correctly drawn on the player's screen.

### Rendering with DirectX 11:
Using the **DirectX 11 API**, the project renders the player boxes, names, health, and distance smoothly over the game.

### Graphical Interface with ImGui:
**ImGui** is used to create an interactive graphical interface that allows the user to adjust the ESP settings in real time.

## 📄 **License**

This project is for **educational use**. The code provided here should not be used to cheat in games or violate the terms of service of online games.
