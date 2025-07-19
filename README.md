# xpmgr
Command line interface for Windows XP license management, inspired by Microsoft's "slmgr".

## Usage
**You need to download the right version for your edition of Windows XP/2003!**

In most cases, you'll need `xpmgr_x86.exe`.

If you have Windows XP Professional x64 Edition or a x64 version of Windows Server 2003, you need `xpmgr_x64.exe`.

If you have an Itanium version of Windows XP or Server 2003, Windows Product Activation is not on those editions, so there is no need for this tool.

`/dli`: Gets the product ID (ex: `76477-755-3803223-21229`).

`/ipk`: Sets the product key (tested to work with dashes). If already activated, this should give an error instead.

`/dti`: Gets an Installation ID with **no dashes** (ex: `253286028742154311079061239762245184619981623171292574`). If already activated, this should give an error instead.

`/atp`: Sets a Confirmation ID (tested to work with dashes). If already activated, this should give an error instead. If successful, this also removes the "Activate Windows" shortcuts and kills the WPA notifier process (`wpabaln.exe`) if it's running.

`/xpr`: Gets the number of days before activation is required, in days. If already activated, this should give an error instead.

`/xpr-eval`: Gets the number of days before the evaluation period expires, in days. In non-evaluation copies of Windows, this should give an error instead.

`/?`: Outputs all the commands you can use.

## Releases
https://github.com/UMSKT/xpmgr/releases

## Credits
Icon made by %fillin%, released under Creative Commons CC-BY-SA 4.0. Modified by [TheTank20](https://github.com/thepwrtank18) to differenciate from UMSKT.

## License
This code is licensed under the LGPLv3 license.
> [!IMPORTANT]  
> Versions from May 7th, 2025 and earlier are licensed under the EPL license.

## Building
Make sure Visual Studio 2017 or later is installed (recommended, might not be needed). Then, install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/about/). Then, use CMake to build:
```
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/TDM-GCC-64/bin/g++.exe
cmake --build build --config Debug
```
