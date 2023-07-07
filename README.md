# xpmgr
Command line interface for Windows XP license management, inspired by Microsoft's "slmgr".

## Usage
**You need to download the right version for your edition of Windows XP/2003!**

In most cases, you'll need `xpmgr_x86.exe`.

If you have Windows XP Professional x64 Edition or an x64 version of Windows Server 2003, you need `xpmgr_x64.exe`.

*If you have an Itanium edition of XP or Server 2003, or a version of XP/Server 2003 we don't even know about, we unfortunately don't have a build for you. We can use your help to make one, though! Contact us at https://umskt.zulipchat.com.*

`--GetProductID`: Gets the product ID (ex: `76477-755-3803223-21229`).

`--SetProductKey`: Sets the product key (tested to work with dashes). If successfully activated, this should give an error instead.

`--GetInstallationID`: Gets an Installation ID with **no dashes** (ex: `253286028742154311079061239762245184619981623171292574`). If successfully activated, this should give an error instead.

`--SetConfirmationID`: Sets a Confirmation ID (tested to work with dashes). If successfully activated, this should give an error instead.

`--GetWPALeft`: Gets the number of days before activation is required, in days. If successfully activated, this should give an error instead.

`--GetEvalLeft`: Gets the number of days before the evaluation period expires, in days. In non-evaluation copies of Windows, this should give an error instead.

`--GetUsage`: Outputs all the commands you can use.

Make sure you do commands right, since, as of now, there's no checking to make sure it's correct or not!

## Releases
https://github.com/UMSKT/xpmgr/releases

## Building
Due to GitHub not wanting to cooperate with auto-building for Windows XP (who would have thought?), you need to build it yourself.

Make sure Visual Studio 2017 or later is installed, with the v141_xp toolchain. Then, `git clone`, open in VS, build, done.
