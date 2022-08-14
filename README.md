# scnd - Steam (Games) Counter Notification Daemon
scnd is a notification daemon written in Rust that notifies you when your selected game(s) gets some player activity

* Current release: v0.0.5 - Alpha
* (In-Development) Next release: v0.6.0 - Beta

## License
scnd is released under the [ISC License](https://www.isc.org/licenses/).

## Requirements to build
* [Rust](https://www.rust-lang.org/)
  * Just install with the instructions straight from the site. The compiler and cargo should be provided by default.
  * You can also use your distro's package manager also providing they have Rust available.
* For dependencies, see the `Cargo.toml` file. Cargo will fetch them.

## Instructions
### Compile and Install
* `cargo install --path .`
### Uninstall
* `cargo uninstall`

## Usage
* Refer to `scnd -h`

## Configuration
* Uses xdg directory: `$HOME/.config/scnd/config.toml`
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
interval = 5
threshold_interval = 10

[[games_servers]]
appid = 1234
name = "Example Game"
threshold_game = 2
threshold_server = 1
```

### systemd
* Service file available in `examples` directory
* For using the systemd daemon as under user control, read: [systemd/User - ArchWiki](https://wiki.archlinux.org/index.php/systemd/User)

