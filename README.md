# Ammo Patcher
A simple SKSE plugin to patch All AMMO at runtime. Supports All Runtimes.

- [NG](https://www.nexusmods.com/skyrimspecialedition/mods/109061/)

## Requirements

- [Git](https://git-scm.com/)
  - Download latest version from [Here](https://git-scm.com/downloads)
- [CMake](https://cmake.org/)
  - Download latest version from [Here](https://cmake.org/download/) and add to your `path` during installation. Must be above v3.24.0
- [Vcpkg](https://github.com/microsoft/vcpkg)
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
  - No Need To Install (or) Download Separately. vcpkg will install it
  
## User Requirements
- [Skyrim Script Extender](https://skse.silverlock.org/)
  - Needed For any SKSE Plugin
- [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
  - Needed for SSE/AE
- [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
  - Needed for VR

## Installation Instructions
Use Git GUI or At the Directory of your choice, open terminal (or) cmd (or) Powershell and enter
```
git clone https://github.com/JudahJL/Ammo-Patcher.git
```

## Extra
- It is Adviced to add "VsDevCmd.bat" from Visual Studio to your Terminal (or) IDE to execute automatically to avoid cmake errors like the one mentioned below.This was a HUGE Headache for me. If you want to know more about "VsDevCmd.bat", Google is your Friend.
```
  The CMAKE_CXX_COMPILER:

    cl.exe

  is not a full path and was not found in the PATH.
``` 

## License

[Apache License 2.0](LICENSE)

## Credits
- [CharmedBaryon](https://github.com/CharmedBaryon) For CommonLibSSE-NG
- [Skyrim Scripting](https://github.com/SkyrimScriptinghttps://github.com/SkyrimScripting) for the Youtube Tutorials
