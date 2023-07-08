# xpmgr
Command line interface for Windows XP license management, inspired by Microsoft's "slmgr".

## Usage
**You need to download the right version for your edition of Windows XP/2003!**

In most cases, you'll need `xpmgr_x86.exe`.

If you have Windows XP Professional x64 Edition or an x64 version of Windows Server 2003, you need `xpmgr_x64.exe`.

If you have an Itanium version of Windows XP or Server 2003, Windows Product Activation is not on those editions, so there is no need for this tool.

In general, don't use `xpmgr_debug_[arch].exe` unless you have a reason why.

`--GetProductID`: Gets the product ID (ex: `76477-755-3803223-21229`).

`--SetProductKey`: Sets the product key (tested to work with dashes). If already activated, this should give an error instead.

`--GetInstallationID`: Gets an Installation ID with **no dashes** (ex: `253286028742154311079061239762245184619981623171292574`). If already activated, this should give an error instead.

`--SetConfirmationID`: Sets a Confirmation ID (tested to work with dashes). If already activated, this should give an error instead. If successful, this also removes the "Activate Windows" shortcuts and kills the WPA notifier process (`wpabaln.exe`) if it's running.

`--GetWPALeft`: Gets the number of days before activation is required, in days. If already activated, this should give an error instead.

`--GetEvalLeft`: Gets the number of days before the evaluation period expires, in days. In non-evaluation copies of Windows, this should give an error instead.

`--GetUsage`: Outputs all the commands you can use.

## Releases
https://github.com/UMSKT/xpmgr/releases

## Building
Due to GitHub not wanting to cooperate with auto-building for Windows XP (who would have thought?), you need to build it yourself.

Make sure Visual Studio 2017 or later is installed, with the v141_xp toolchain. Then, `git clone`, open in VS, build, done.
