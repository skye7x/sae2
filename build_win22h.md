## Build the C++ Client on Windows

### 1. Install prerequisites

Download and install these:
- **CMake**: https://cmake.org/download/ — pick the `.msi` installer
- **Visual Studio 2022**: https://visualstudio.microsoft.com/downloads/ — during install, select **"Desktop development with C++"**
- **vcpkg** (package manager for C++ libraries):

Open **PowerShell as Administrator** and run:
```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

---

### 2. Install curl and nlohmann-json via vcpkg

```powershell
cd C:\vcpkg
.\vcpkg install curl:x64-windows
.\vcpkg install nlohmann-json:x64-windows
```

---

### 3. Build the client

Open **PowerShell** and run:
```powershell
cd ~\sae2\client
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -A x64
cmake --build . --config Release
```

---

### 4. Run

```powershell
.\Release\sae2_client.exe
```

It will ask for hostname, website, login, password — then type `Send` to submit.

---

### Note
The client sends to `https://localhost` by default — so your Linux server must be reachable from Windows. If the server has a different IP, edit `main.cpp` line:
```cpp
std::string url = "https://localhost/api/submissions";
```
Replace `localhost` with your server's IP before building.