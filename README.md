# steamcountsnotifyd
SteamCountsNotifyD is a notification daemon written in Rust that notifies you when your selected game(s) gets some player activity

* Current release: v0.0.5 - Alpha
* (In-Development) Next release: v0.6.0 - Alpha

## License
SteamCountsNotifyD is released under a the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) a free software copyleft license.

## Dependencies
* serde, serde\_json, toml, dirs-next, notify-rust, clap, reqwest, tokio, futures, a2s, msq
  * All are available on [crates.io](https://crates.io/)

## Requirements to build
* [Rust](https://www.rust-lang.org/)
  * Just install with the instructions straight from the site. The compiler and cargo should be provided by default.
  * You can also use your distro's package manager also providing they have Rust available.

## Instructions
### Compile and Install
* `cargo install --path .`
### Uninstall
* `cargo uninstall`

## Usage
* Refer to `steamcountsnotifyd -h`

## Configuration
* Uses xdg directory: `$HOME/.config/steamcountsnotifyd/config.toml`
* Program will generate the configuration file if user requested via `--generate-config` and is not available

### Example
```
interval = 1
threshold_interval = 2
connection_timeout = 10
notify_timeout = 10
action_type = 1

[[games]]
appid = 1234
name = "Example Game"
threshold = 100

[[servers]]
address = "0.0.0.0:1234"
name = "Example Server"
threshold = 0

[[games_servers]]
appid = 1234
name = "Example Game"
threshold_game = 2
threshold_server = 1
```

### systemd
* Use the systemd daemon as under user control, more information: [systemd/User - ArchWiki](https://wiki.archlinux.org/index.php/systemd/User)

## TODO
* Edit configuration via the CLI
* Improving on the Windows port
* Better documentation and comments
* Making it work with systemd properly
  * Support reload function
* Support runit and openrc also, sysvinit won't be however
* Configuration: More controls can be given for per game/server
  * EX: Give interval for each game/server

## Releases
### v0.6.0 Alpha
* Change how version numbering used, moved from 0.0.X to 0.X.0
* 2021-05-05: [msq](https://github.com/nullsystem/msq-rs) library pulled in

### v0.0.5 Alpha
* 2021-04-24: Custom server name
  * Added in the new `name` field under `servers`
  * Empty `name` string will use the server's given name
  * Non-empty `name` string will use this string instead
* 2021-04-13: Ignore threshold: `--ignore-thresholds`
  * Ignores the threshold limits and just outputs the counters regardless
  * Useful with the CLI Single check
* 2021-04-05: CLI Single Check `-s, --single-check`
  * Can use the tool as just a single use CLI tool
  * Just check and print the counters to the terminal once

### v0.0.4 Alpha (First Version of Rust Rewrite)
* 2021-03-01: Daemon option and forking/daemonization process removed
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

