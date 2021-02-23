use serde::{Deserialize, Serialize};
use std::default::Default;
use crate::cli;

#[derive(Serialize, Deserialize, Clone)]
pub struct Config {
    pub interval: u32,
    pub threshold_interval: u32,
    pub connection_timeout: u32,
    pub notify_timeout: u32,
    pub action_type: u32,
    pub games: Vec<ConfigGame>,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct ConfigGame {
    pub appid: u32,
    pub name: String,
    pub threshold: u32,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            interval: 1,
            threshold_interval: 2,
            connection_timeout: 10,
            notify_timeout: 10,
            action_type: 1,
            games: vec![
                ConfigGame {
                    appid: 244630,
                    name: String::from("NEOTOKYO"),
                    threshold: 0,
                },
                ConfigGame {
                    appid: 282440,
                    name: String::from("Quake Live"),
                    threshold: 100,
                },
            ],
        }
    }
}

impl Config {
    pub fn from_toml_str(&mut self, s: &str) {
        *self = toml::from_str(s).unwrap();
    }

    pub fn to_toml_str(&self) -> String {
        return toml::to_string(self).unwrap();
    }

    pub fn from_opts(&mut self, opts: cli::Opts) {
        self.interval = opts.interval;
        self.threshold_interval = opts.threshold_interval;
        self.connection_timeout = opts.connection_timeout;
        self.notify_timeout = opts.notify_timeout;
        self.action_type = opts.action_type;
    }
}

