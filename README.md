# steamcountsnotifyd
SteamCountsNotifyD is a multithreaded Linux daemon written in C++17 that notifies you when your selected games gets some player activity

Current release: v0.0.2 - Alpha

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
* You can just copy over the `config/conf_example` over to that configuration place
### Parameters
CLI | Name | Description
---|---|---
`-i` | `interval` | How long in minutes to delay between each player count fetch.
`-t` | `thresholdinterval` | When the threshold's achieved, what interval gets extended to.
`-c` | `connectiontimeout` | How long in seconds the connection stay puts.
`-n` | `notifytimeout` | How long in seconds the notification stays shown.
`-a` | `actiontype` | Which type of clickable notification should use: 0 - default, 1 - button
`-d` | `daemonize` | Defaults to daemonize, 0 to disable daemonization
### Example
```
interval 1
thresholdinterval 2
connectiontimeout 10
notifytimeout 10
actiontype 1

newappid 244630 "NEOTOKYO" 0
newappid 282440 "Quake Live" 100
```
### systemd
* Use the systemd daemon as under user control, more information: [systemd/User - ArchWiki](https://wiki.archlinux.org/index.php/systemd/User)

## TODO (Possible future features)
* Per server notification
* Windows port
* Better documentation and comments
* Making it work with systemd properly
  * Support reload function
* Support runit and openrc also, sysvinit won't be however

## Releases
### 0.0.2 Alpha
* counts to count (grammar fix)
* systemd service file available, but buggy at the moment (need `restart` after a `start`)
* fixed parameters not passing through command line and used
* non-daemonize mode added - paramter: `-n=0`; configuration: `daemonize 0`
* Added the ability to launch the game (`default` or `clicked` options available)
  * Added `actiontype` to configuration and command-line paramter
  * 0 = default, 1 = clicked
### 0.0.1 Alpha
* First release
* Basic daemon implementation implemented

