# steamcountsnotifyd
SteamCountsNotifyD is a multithreaded notification daemon written in Rust that notifies you when your selected games gets some player activity

Current release: v0.0.2 - Alpha

## License
SteamCountsNotifyD is released under a the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) a free software copyleft license.

## Dependencies

## Requirements to build
* [rust](https://www.rust-lang.org/)

## Instructions
### Compile
* `cd steamcountsnotifyd`
* `cargo build`
### Install
* `cargo install`
### Uninstall
* `cargo uninstall`

## Usage
* Refer to `steamcountsnotifyd -h`

## Configuration
* Uses xdg directory: `$HOME/.config/steamcountsnotifyd/config.toml`
* Program should automatically generate the configuration file if not available
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
interval = 1
threshold_interval = 2
connection_timeout = 10
notify_timeout = 10
action_type = 1

[[games]]
appid = 244630
name = "NEOTOKYO"
threshold = 0

[[games]]
appid = 282440
name = "Quake Live"
threshold = 100
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
### v0.0.3 Alpha (Rust Rewrite)
* Re-written to Rust
### v0.0.3 Alpha
* Notification hint set to "resident", meaning the game launcher is clickable even if after the notification goes away
  * However only usable if only "default" (default anyway) is set
* Notifies that the daemon started up
### v0.0.2 Alpha
* counts to count (grammar fix)
* systemd service file available, but buggy at the moment (need `restart` after a `start`)
* fixed parameters not passing through command line and used
* non-daemonize mode added - paramter: `-n=0`; configuration: `daemonize 0`
* Added the ability to launch the game (`default` or `clicked` options available)
  * Added `actiontype` to configuration and command-line paramter
  * 0 = default, 1 = clicked
### v0.0.1 Alpha
* First release
* Basic daemon implementation implemented

