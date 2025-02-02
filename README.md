# DNX-Note-Width-Changer
![Github stars](https://img.shields.io/github/stars/AXIS5hacker/DNX-Note-Width-Changer?logo=github)
![GitHub](https://img.shields.io/github/license/AXIS5hacker/DNX-Note-Width-Changer)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/AXIS5hacker/DNX-Note-Width-Changer?color=violet)](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases/tag/v1.0.5)

A tool for changing the note width in a Dynamix Fanmade Chart.

Created by AXIS5 (a.k.a.AXIS5Hacker).

Special thanks to [i0ntempest](https://github.com/i0ntempest) for helping me debugging and making the Makefile, and [NordlandeW](https://github.com/NagaseIori) for helping testing the chart repairing module.

The project contains a graphic application (Dynamix_note_width_changer_GUI.exe on Windows) and a console application (Dynamix_chart_width_control_x64.exe on Windows).

# Installation

### Windows

For console application, download `Dynamix_chart_width_control.zip` from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases) and run `Dynamix_chart_width_control_x86.exe` (for i386 users) or `Dynamix_chart_width_control_x64.exe` (for x86_64 users) in the "console" directory in command prompt to use it, or build manually with Visual Studio.

For GUI application, download `Dynamix_chart_width_control.zip` from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases) and run `Dynamix_note_width_changer_GUI.exe` in the "GUI" directory to use it, or build manually with Visual Studio and Qt6.

*Note: If you decided to build this program manually on Windows, you need to add a system variable called "CMAKE_PREFIX_PATH" and add your QT library path into this variable (i.e. ${your qt installation path}/${your qt version}/${your qt platform}/lib/cmake).*

Or you can download `DNX_width_changer_setup.exe` and run this setup program to install.

If you can't run the programs on your computer, try running `vc_redist.x64.exe` in the "GUI" directory to install a MICROSOFT VISUAL C++ 2019 RUNTIME on your computer to run the program.

### macOS

#### GUI Application

Download  `Dynamix Chart Width Changer.dmg` from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases), drag the app to Applications folder and open from Launchpad.

*Note: This app supports the latest major version of macOS and Apple silicon (arm64) Macs only. If you're running an older OS or using an Intel (x86_64) Mac, MacPorts can build the app for your platform.*

Or install using [MacPorts](https://www.macports.org): 

1. Install Xcode Command Line Tools: using terminal, `xcode-select --install`
2. [Install MacPorts](https://www.macports.org/install.php)
3. Install the app: using terminal, `sudo port install dynamix-chart-width-control-gui`, then open the app from Launchpad.

#### CLI Application

Install using [MacPorts](https://www.macports.org): `sudo port install dynamix-chart-width-control`

Or download `dynamix-chart-width-control-macos` (x86_64+arm64 universal binary, supports macOS 12 and later) from [Releases](https://github.com/AXIS5hacker/DNX-Note-Width-Changer/releases) and run in Terminal

### Building the GUI application on macOS & Linux

This application is built with Qt 6. To build the GUI application on macOS and Linux systems, you need to **download Qt 6**, and install CMake build system as well as support libraries for Qt applications.

#### macOS ([Xcode](https://developer.apple.com/xcode/) and [MacPorts](https://www.macports.org)):

Install Xcode Command Line Tools:

`xcode-select --install`

Then [install MacPorts](https://www.macports.org/install.php), then:

`sudo port install cmake qt6-qtbase qt6-qttools`

#### Debian/Ubuntu (apt-get):

`sudo apt-get install build-essential libgl1-mesa-dev`

#### Fedora/RHEL/CentOS (yum):

```
sudo yum groupinstall "C Development Tools and Libraries"
sudo yum install mesa-libGL-devel
```

#### openSUSE (zypper):

`sudo zypper install -t pattern devel_basis`

Then you can proceed with CMake:

1. Clone the project
2. Using terminal, `cd DNX-Note-Width-Changer/`
3. `cmake CMakeLists.txt -B ./exampleBuildDirectory`, exampleBuildDirectory is the directory name where you want to build the program, and you can customize it.
4. `cmake --build ./exampleBuildDirectory`, and you will get the program in the `DNX-Note-Width-Changer/exampleBuildDirectory/bin` directory.

### Building the CLI application on macOS & Linux

1. Download the tarball from release, or clone the project
2. (macOS only) Using terminal, `xcode-select --install`  will install Xcode Command Line Tools for you.
3. `cmake CMakeLists.txt -B ./exampleBuildDirectory`, exampleBuildDirectory is the directory name where you want to build the program, and you can customize it.
4. `cmake --build ./exampleBuildDirectory`, and you will get the program in the `DNX-Note-Width-Changer/exampleBuildDirectory/bin` directory.
5. If you want to install it on macOS or Linux, run `sudo cmake --install ./exampleBuildDirectory` after building.
6. If you want to uninstall the CLI application, run `cd ./exampleBuildDirectory && sudo make uninstall`.

# Arguments (for console application)

filename [-w width_multiplier|-rnd1|-rnd2] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h] [-m] [-l] [-r] [-c]


-w width_multiplier	   change the width of a chart, width_multiplier is a decimal number

-o output_filename     specify the filename of the changed chart

-s start_time(bar)	   specify the start time of the time range you want to change, in the unit of bar (v0.6.x and newer)

-e end_time(bar)	   specify the end time of the time range you want to change, in the unit of bar (v0.6.x and newer)

-m                   change the middle side. (v0.8.x and newer)

-l                   change the left side. (v0.8.x and newer)

-r                   change the right side. (v0.8.x and newer)

*If none of the triggers in "-m, -l, -r" is specified, all sides will be changed.

-rnd1                change the width randomly, using mode 1(will ignore the "-w" argument) (v1.1.x and newer)

-rnd2                change the width randomly, using mode 2(will ignore the "-w" argument) (v1.1.x and newer)

-?                      help

-h                      help, same as -?

-c                   show the detailed note quantity of this chart (v1.3.x and newer)

# About the random width modes

The program is now integrated with my width randomizer, and the randomizer has 2 modes.

Mode 1 is random width multiplier, that is, multiplying the note width by a random value.

Mode 2 is random width changer, that is, changing the note width to a random value.
