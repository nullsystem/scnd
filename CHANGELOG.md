# Change log
## v0.6.0 Beta
* 2022-08-14: Updates, changes, and improvements
  * Update dependencies
  * `interval` and `threshold_interval` now available per `games`/`servers`/`games_servers`
  * Change to a more permissive license (GPLv3 -> ISC)
  * Removed Windows WiX toolset files
  * Renamed to scnd
* 2021-05-05: msq and version numbering
  * [msq](https://github.com/nullsystem/msq-rs) library pulled in
  * Change how version numbering used, moved from 0.0.X to 0.X.0

## v0.0.5 Alpha
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

## v0.0.4 Alpha
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

