# 🌋 **Volcano's ESP** 🔥

## 🖼️ **Showcase - Screenshots**

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/1.png?raw=true)

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/2.png?raw=true)

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/3.png?raw=true)

## 📚 **Project Description**

**Volcano's ESP** is a personal integrative project developed for educational purposes to learn and deepen knowledge in advanced **C++** and **reverse engineering** concepts. The main goal of this project is to create an **overlay** for a game using techniques such as **memory reading**, **process scanning**, **offsets**, and **rendering** with **DirectX 11**. 

⚠️ **Important**: This ESP **is not designed** to be **undetectable** and should not be used on servers with **VAC** (Valve Anti-Cheat). You may test it on offline servers by launching the game with the `-insecure` parameter.

## 🔧 **Requirements**

To run this project on your machine, ensure that you have the following requirements:

- **Operating System**: Windows 10 or later.
- **Visual Studio 2022** or later with support for **C++20** and **DirectX 11**.
- **ImGui** for the graphical interface (must be included in **VC++ Directories**).
- **DirectX 11 SDK** for rendering interfaces.
- **CMake** (if you prefer to use it for build management, you will need to create your own `CMakeLists.txt`).

## 🛠️ **Building in Visual Studio**

1. **Clone the repository** to your local machine:
   ```bash
   git clone https://github.com/MaxPriani/volcano-esp.git
   ```
   
2. **Open the project in Visual Studio**:
   - Open **Visual Studio** and select `File > Open > Project/Solution`.
   - Navigate to the directory where you cloned the repository and open the `.sln` solution file.

3. **Build the project**:
   - Once the project is configured, select `Build > Build Solution` to compile the project.
   - This will generate the executable file in the `build/` directory.

4. **Run the ESP**:
   - After building, run the generated `ESP.exe`. Keep in mind that the ESP only activates if the game is running, as it needs to scan the `cs2.exe` process and extract the base addresses from `client.dll` and `engine2.dll`.

## 🖥️ **ESP Features**

- **ESP Box Renderer** 📦: Displays rectangular boxes around players to indicate their position.
- **HP-Based Health Color** ❤️: Shows the player's health color based on their health level.
- **Distance** 🏃‍♂️: Displays the distance between the local player and enemies. Based on the distance set by the user in the UI (e.g., 3,000 units), the ESP will not render enemies that are farther than the specified distance.
- **Player Names** 🏷️: Displays the players' names above their head.
- **Interactive UI** ⚙️: Allows you to adjust values such as box thickness, text size, max enemy distance, colors, and more. You can toggle the UI on/off with [F4].

## 📝 **Overview**

### External ESP:
An **external ESP** is a program that does not inject itself into the game's memory or modify it. Instead, the ESP **reads the game's memory** to gather real-time information about players by using **offsets** to access variables containing this data. This information is then used to render a transparent window overlay on top of the game, displaying features such as rectangles over players' positions.

**Important**: The overlay **only works in Windowed or Borderless mode**. This is because external overlays like this one cannot interact with the game in **Exclusive Fullscreen** mode.

### Overlay:
An **overlay** is a screen that sits on top of the game. In this project, **DirectX 11** is used to create a transparent window that is placed over the game, which is useful to display additional information without interfering with the game's flow. This window continuously renders the ESP (real-time player information) while the game is running.

### Memory Reading:
The ESP uses the `Memory` class to read the game's process memory, looking for specific addresses (using **offsets**) that contain relevant information such as player position, health, name, etc.

### Coordinate Transformations:
Linear algebra is used to transform 3D world coordinates into 2D screen coordinates, allowing the overlay elements to be correctly drawn on the player's screen.

### ImGui:
**ImGui** is utilized to create an interactive graphical interface that enables users to adjust ESP settings in real-time. It is also used to render elements such as rectangles around players, text, and additional visuals.
