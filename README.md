# DNX-Note-Width-Changer
A tool for changing the note width in a Dynamix Fanmade Chart.

Created by AXIS5.

Special thanks to [i0ntempest](https://github.com/i0ntempest) for helping me debugging.

The application is a console application.

# Installation

### Windows

Download `\Dynamix chart width control\x64\Release\Dynamix chart width control.exe`  and run in command prompt to use it, or build manually with Visual Studio.

### macOS & Linux

Install using [MacPorts](https://www.macports.org): `sudo port install dynamix-chart-width-control`

Or build manually:

1. Download the tarball from release, or clone the project
2. Using terminal, `cd DNX-Note-Width-Changer/Dynamix\ chart\ width\ control/`
3. `make && sudo make install` installs the binary to `/usr/local/bin` by default

# Arguments

filename [-w width_multiplier] [-o output_filename] [-?]


-w width_multiplier     change the width of a chart, width_multiplier is a decimal number

-o output_filename      specify the filename of the changed chart

-?                      help

-h                      help, same as -?
