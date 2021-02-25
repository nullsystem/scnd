# steamcountsnotifyd
SteamCountsNotifyD is a notification daemon written in Rust that notifies you when your selected game(s) gets some player activity

Development release: v0.0.4 - Alpha
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
* Program will generate the configuration file if user requested via `--generate-config` and is not available
### Parameters
CLI | Name | Description
---|---|---
`-i` | `interval` | How long in minutes to delay between each player count fetch.
`-t` | `threshold_interval` | When the threshold's achieved, what interval gets extended to.
`-c` | `connection_timeout` | How long in seconds the connection stay puts.
`-n` | `notify_timeout` | How long in seconds the notification stays shown.
`-a` | `action_type` | Which type of clickable notification should use: 0 - click, 1 - button
`-d` | `daemonize` | This flag makes the program daemonize
`-g` | `generate` | Generates configuration file at default directory 
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

[[servers]]
address = "172.107.97.234:26300"
threshold = 0
```

### systemd
* Use the systemd daemon as under user control, more information: [systemd/User - ArchWiki](https://wiki.archlinux.org/index.php/systemd/User)

## TODO (Possible future features)
* SOON: Per server notification
* Improving on the Windows port
* Better documentation and comments
* Making it work with systemd properly
  * Support reload function
* Support runit and openrc also, sysvinit won't be however

## Releases
### v0.0.4 Alpha (Rust Rewrite)
* Using [a2s-rs](https://github.com/rumblefrog/a2s-rs) library to do Source A2S Queries
  * Can now query from each server
* Roughly working and ported to Windows 10
  * Notification does not implement actions/buttons - Only used for viewing counters
  * No equivalent to daemon on Windows used yet
* 2021-02-25: Feature parity with previous C++ v0.0.2 Alpha version
  * NOTE: Daemon is there but counter notification not working and CLI flag different/switched
  * Non-daemon is by default
* 2021-02-22: Initial rewrite to Rust
### v0.0.3 Alpha (Final C++ Version, Unreleased)
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

