# DNX-Note-Width-Changer
![Github stars](https://img.shields.io/github/stars/AXIS5hacker/DNX-Note-Width-Changer?logo=github)
![GitHub](https://img.shields.io/github/license/AXIS5hacker/DNX-Note-Width-Changer)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/AXIS5hacker/DNX-Note-Width-Changer?color=violet)](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases/tag/v0.7.9)

A tool for changing the note width in a Dynamix Fanmade Chart.

Created by AXIS5 (a.k.a.AXIS5Hacker).

Special thanks to [i0ntempest](https://github.com/i0ntempest) for helping me debugging and making the Makefile.

The application is a console application.

# Installation

### Windows

Download `Dynamix chart width control_x86.exe` (for i386 users) or `Dynamix chart width control_x64.exe` (for x86_64 users) from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases) and run in command prompt to use it, or build manually with Visual Studio.

### macOS & Linux

Install using [MacPorts](https://www.macports.org): `sudo port install dynamix-chart-width-control`

Or download `dynamix-chart-width-control-macos` (x86_64+arm64 universal binary) from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases) and run in Terminal

Or build manually:

1. Download the tarball from release, or clone the project
2. Using terminal, `cd DNX-Note-Width-Changer/Dynamix\ chart\ width\ control/`
3. `make && sudo make install` installs the binary to `/usr/local/bin` by default

# Arguments

filename [-w width_multiplier] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h]


-w width_multiplier     change the width of a chart, width_multiplier is a decimal number

-o output_filename     specify the filename of the changed chart

-s start_time(bar)	   specify the start time of the time range you want to change, in the unit of bar (v0.6.x and newer)

-e end_time(bar)	   specify the end time of the time range you want to change, in the unit of bar (v0.6.x and newer)

-?                      help

-h                      help, same as -?
