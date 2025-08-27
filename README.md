# Ammo Patcher
A simple SKSE plugin to patch All AMMO at runtime. Supports All Runtimes.

- [NG](https://www.nexusmods.com/skyrimspecialedition/mods/109061/)

## Requirements

- [Git](https://git-scm.com/)
  - Download latest version from [Here](https://git-scm.com/downloads)
- [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
  - Desktop development with C++
  
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
### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```

## License

[MIT](LICENSE)
