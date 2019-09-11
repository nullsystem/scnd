# steamcountsnotifyd
SteamCountsNotifyD is a daemon that notifies you when your selected games gets some player activity

## License
SteamCountsNotifyD is released under a free software license (MIT)

## Dependencies
Minimal dependencies needed, only libcurl to download and libnotify to notify.
* [libcurl](https://curl.haxx.se/libcurl/)
* [libnotify](https://developer.gnome.org/libnotify/)

## Requirements to build
* [meson](https://mesonbuild.com/)
* [ninja](https://ninja-build.org/)
* C++17 capable compiler
  * [GCC](https://gcc.gnu.org/) - Recommends 9.2 or 8.3
  * [Clang](https://clang.llvm.org/)

## Instructions
### Compile
* `cd steamcountsnotifyd`
* `meson build && cd build`
* `ninja`
### Install
* TODO

## Usage
* TODO

