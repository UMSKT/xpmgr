# xpmgr
Command line interface for Windows XP license management, inspired by Microsoft's "slmgr".

## Usage
**You need to download the right version for your edition of Windows XP/2003!**

In most cases, you'll need `xpmgr_x86.exe`.

If you have Windows XP Professional x64 Edition or an x64 version of Windows Server 2003, you need `xpmgr_x64.exe`.

If you have an Itanium version of Windows XP or Server 2003, Windows Product Activation is not on those editions, so there is no need for this tool.

If you're trying to manage products other than Windows, you need `xpmgr_x86.exe`, regardless of your architecture.

`--GetProductID`: Gets the product ID (ex: `76477-755-3803223-21229`).

`--SetProductKey`: Sets the product key (tested to work with dashes). If already activated, this should give an error instead.

`--GetInstallationID`: Gets an Installation ID with **no dashes** (ex: `253286028742154311079061239762245184619981623171292574`). If already activated, this should give an error instead.

`--SetConfirmationID`: Sets a Confirmation ID (tested to work with dashes). If already activated, this should give an error instead. If successful, this also removes the "Activate Windows" shortcuts and kills the WPA notifier process (`wpabaln.exe`) if it's running.

`--GetWPALeft`: Gets the number of days before activation is required, in days. If already activated, this should give an error instead.

`--GetEvalLeft`: Gets the number of days before the evaluation period expires, in days. In non-evaluation copies of Windows, this should give an error instead.

`--Office2003`: This provides **EXPERIMENTAL** support for Office 2003. When this parameter is used, all other parameters except `--GetInstallationID` and `--SetConfirmationID` are disabled, and those two (attempt) to go what they do on Office 2003, rather than Windows.

`--BypassInstallCheck`: Disables the check to see if Office 2003 is installed. Intended for debugging purposes only. If it doesn't work without this enabled, it probably won't work with it either.

`--GetUsage`: Outputs all the commands you can use.

## Releases
https://github.com/UMSKT/xpmgr/releases

## Building
Make sure Visual Studio 2017 or later is installed. Then, install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/about/). Then, run, at the source folder:
```
mkdir build
mkdir build\x86
mkdir build\x64
windres icon.rc -F pe-i386 -O coff -o build\x86\icon_x86.res
g++ -m32 -o build\x86\xpmgr_x86 xpmgr.cpp build\x86\icon_x86.res -lole32 -luuid -loleaut32
windres icon.rc -O coff -o build\x64\icon_x64.res
g++ -o build\x64\xpmgr_x64 xpmgr.cpp build\x64\icon_x64.res -lole32 -luuid -loleaut32
```

Or, if you like one-liners:
```
mkdir build & mkdir build\x86 & mkdir build\x64 & windres icon.rc -F pe-i386 -O coff -o build\x86\icon_x86.res & g++ -m32 -o build\x86\xpmgr_x86 xpmgr.cpp build\x86\icon_x86.res -lole32 -luuid -loleaut32 & windres icon.rc -O coff -o build\x64\icon_x64.res & g++ -o build\x64\xpmgr_x64 xpmgr.cpp build\x64\icon_x64.res -lole32 -luuid -loleaut32
```
