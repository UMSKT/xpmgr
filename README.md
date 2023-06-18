# xpmgr
Command line interface for Windows XP license management, inspired by Microsoft's "slmgr".

## Usage
`--GetProductID`: Gets the product ID (ex: `76477-755-3803223-21229`).

`--SetProductKey`: Sets the product key (tested to work with dashes).

`--GetInstallationID`: Gets an Installation ID with **no dashes** (ex: `253286028742154311079061239762245184619981623171292574`). 

`--SetConfirmationID`: Sets a Confirmation ID (tested to work with dashes).

`--GetWPALeft`: Gets the number of days before activation is required, in days.

`--GetEvalLeft`: Gets the number of days before the evaluation period expires, in days. In most cases, this should be 2147483647.

Make sure you do commands right, since, as of now, there's no checking to make sure it's correct or not!

## Releases
https://github.com/UMSKT/xpmgr/releases

## Building
Due to GitHub not wanting to cooperate with auto-building for Windows XP (who would have thought?), you need to build it yourself.

Make sure Visual Studio 2017 or later is installed, with the v141_xp toolchain. Then, `git clone`, open in VS, build, done.
