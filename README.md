# 🌋 **Volcano's ESP** 

## 📸 **Showcase - Screenshots**

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/1.png?raw=true)

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/2.png?raw=true)

![ESP](https://github.com/MaxPriani/volcano-esp/blob/main/src/assets/images/3.png?raw=true)

## 📜 **Project Description**

**Volcano's ESP** is a personal project developed to learn and deepen knowledge in **C++**, **reverse engineering**, and **memory manipulation** by accessing CS2 data through external memory reading.

## 🎖️ **Features**

- **Handle Hijacking** for undetected memory reading.
- **ESP Overlay** with player boxes, health-based colors, and distance display.
- **ImGui UI** for real-time customization.

## 💎 **Handle Hijacking**

Instead of using `OpenProcess` to obtain a handle to `CS2.exe`, which is commonly flagged by anti-cheats like VAC, Volcano's ESP hijacks a handle that another process—`steam.exe`—has already opened to `CS2.exe`.

Using `Process Explorer`, I analyzed which processes typically hold handles to `CS2.exe` and discovered that `steam.exe` consistently opens handles with the required permissions. The ESP duplicates an existing handle from `steam.exe`, effectively bypassing the need to call `OpenProcess` on `CS2.exe`.

### 💡 **How It Works**

- **Scan existing handles**: The program uses `NtQuerySystemInformation` to list all open handles in the system.
- **Filter handles from `steam.exe`**: It identifies handles belonging to `steam.exe` that reference `CS2.exe`.
- **Check permissions**: Only handles with `PROCESS_VM_READ | PROCESS_QUERY_INFORMATION` are considered.
- **Duplicate the handle**: If a valid handle is found, `NtDuplicateObject` is used to create a copy that `ESP.exe` can use to read CS2 memory.
- **Memory reading**: The ESP now has full read access to CS2 memory without ever calling `OpenProcess`.

## 📃 **Requirements**

To run this project on your machine, ensure that you have the following requirements:

- **Operating System**: Windows 10 or later.
- **Visual Studio 2022** or later with support for **C++20** and **DirectX 11**.
- **ImGui** for the graphical interface (must be included in **VC++ Directories**).
- **DirectX 11 SDK** for rendering interfaces.
- **CMake** (if you prefer to use it for build management, you will need to create your own `CMakeLists.txt`).

## ⚙️ **Building in Visual Studio**

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
   - After building, run the generated `ESP.exe`.