# xpmgr
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/UMSKT/xpmgr/windows.yml)

Command line interface for managing licenses of older versions of Windows/Office (starting with Windows and office XP), inspired by Microsoft's slmgr.

## Usage

In most cases, you'll need `xpmgr_x86.exe`.

If you are trying to manage Windows XP Professional x64 Edition or an x64 version of Windows Server 2003, you need `xpmgr_x64.exe`.

`/dli`: Gets the product ID (ex: `76477-755-3803223-21229`).

`/ipk`: Sets the product key (tested to work with dashes). If already activated, this should give an error instead.

`/dti`: Gets an Installation ID (ex: `253286028742154311079061239762245184619981623171292574`). If already activated, this should give an error instead.

`/atp`: Sets a Confirmation ID. If already activated, this should give an error instead. If successful, this also removes the "Activate Windows" shortcuts and kills the WPA notifier process (`wpabaln.exe`) if it's running.

`/xpr`: Gets the number of days before activation is required, in days. If already activated, this should give an error instead.

`/xpr-eval`: Gets the number of days before the evaluation period expires, in days. In non-evaluation copies of Windows, this should give an error instead.

`/?`: Outputs all the commands you can use.

`/ipk` and `/dti` also support reading from stdin via a pipe. So, you could do something like `echo FCKGW-RHQQ2-YXRKT-8TG6W-2B7Q8 | xpmgr_x86 /ipk` or `echo 253286028742154311079061239762245184619981623171292574 | xpmgr_x86 /atp` (replace echo with your own program).

This can also be used as a library; see the xpmgr.h file for details.

## Releases
https://github.com/UMSKT/xpmgr/releases

## Credits
Icon made by [TehAweseomestKitteh](https://kittehcreations.com/), released under Creative Commons CC-BY-SA 4.0. Modified by [TheTank20](https://github.com/thepwrtank18) (poorly) to differenciate from UMSKT.

## License
This code is licensed under the LGPLv3 license.
> [!IMPORTANT]  
> Versions from May 7th, 2025 and earlier are licensed under the EPL license.

## Building
Install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/about/). Then, use CMake to build:
```
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/TDM-GCC-64/bin/g++.exe
cmake --build build --config Debug
```
