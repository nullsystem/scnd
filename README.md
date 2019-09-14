# steamcountsnotifyd
SteamCountsNotifyD is a multithreaded Linux daemon written in C++17 that notifies you when your selected games gets some player activity

## License
SteamCountsNotifyD is released under a free software license (MIT)

## Dependencies
Minimal dependencies needed:
* [libcurl](https://curl.haxx.se/libcurl/) - For downloading
* [libnotify](https://developer.gnome.org/libnotify/) - For notification
Optional:
* [systemd](https://freedesktop.org/wiki/Software/systemd/)

## Requirements to build
* [meson](https://mesonbuild.com/) - To generate ninja file compile/install script
* [ninja](https://ninja-build.org/) - To compile/install the project
* C++17 capable compiler
  * [GCC](https://gcc.gnu.org/) - Recommends 9.2 or 8.3
  * [Clang](https://clang.llvm.org/)
### C++ Requirements
* C++11 for `std::thread`
* C++11 for `std::function`
* C++17 for `std::filesystem`
* C++17 for cleaner for loop over `std::pair`

## Instructions
### Compile
* `cd steamcountsnotifyd`
* `meson build && cd build`
* `ninja`
### Install
* `cd steamcountsnotifyd/build/`
* `ninja install` - use `sudo` if necessary
### Uninstall
* `cd steamcountsnotifyd/build/`
* `ninja uninstall` - use `sudo` if necessary

## Usage
* Refer to `steamcountsnotifyd -h`

## Configuration
* Uses xdg directory: `$HOME/.config/steamcountsnotifyd/config`
### Example
```
interval 1
thresholdinterval 2
connectiontimeout 10
notifytimeout 10

newappid 244630 "NEOTOKYO" 0
newappid 282440 "Quake Live" 100
```

## TODO (Possible future features)
* Per server notification
* Click to load up game
* Windows port
* Better documentation and comments
* Making it work with systemd properly
  * Support reload function
* Support runit and openrc also, sysvinit won't be however

## Releases
### 0.0.2 Alpha release (Not final)
* counts to count (grammar fix)
* systemd service file available
* fixed paramters not passing through command line and used
* non-daemonize mode added
### 0.0.1 Alpha release
* First release
* Basic daemon implementation implemented

