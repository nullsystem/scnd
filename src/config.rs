use crate::cli;
use serde::{Deserialize, Serialize};
use std::default::Default;

#[derive(Serialize, Deserialize, Clone)]
pub struct Config {
    pub interval: u32,
    pub threshold_interval: u32,
    pub connection_timeout: u32,
    pub notify_timeout: u32,
    pub action_type: u32,
    pub games: Option<Vec<ConfigGame>>,
    pub servers: Option<Vec<ConfigServer>>,
    pub games_servers: Option<Vec<ConfigGameServer>>,

    #[serde(skip)]
    pub ignore_thresholds: bool,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct ConfigGame {
    pub appid: u32,
    pub name: String,
    pub threshold: u32,
    pub interval: Option<u32>,
    pub threshold_interval: Option<u32>,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct ConfigServer {
    pub address: String,
    pub threshold: u32,
    pub interval: Option<u32>,
    pub threshold_interval: Option<u32>,

    #[serde(default)]
    pub name: String,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct ConfigGameServer {
    pub appid: u32,
    pub name: String,
    pub threshold_game: u32,
    pub threshold_server: u32,
    pub interval: Option<u32>,
    pub threshold_interval: Option<u32>,
}

#[derive(Clone)]
pub enum ActionType {
    Click,
    Button,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            interval: 1,
            threshold_interval: 2,
            connection_timeout: 10,
            notify_timeout: 10,
            action_type: 1,
            ignore_thresholds: false,
            games: Some(vec![
                ConfigGame {
                    appid: 1234,
                    name: String::from("Example game"),
                    threshold: 100,
                    interval: None,
                    threshold_interval: None,
                },
            ]),
            servers: Some(vec![ConfigServer {
                address: String::from("0.0.0.0:1234"),
                name: String::from(""),
                threshold: 0,
                interval: None,
                threshold_interval: None,
            }]),
            games_servers: Some(vec![ConfigGameServer {
                appid: 1234,
                name: String::from("Example game"),
                threshold_game: 2,
                threshold_server: 1,
                interval: None,
                threshold_interval: None,
            }]),
        }
    }
}

impl Config {
    pub fn from_toml_str(&mut self, s: &str) {
        let tmp_ignore_thresholds = self.ignore_thresholds;
        *self = toml::from_str(s).unwrap();
        self.ignore_thresholds = tmp_ignore_thresholds;
    }

    pub fn to_toml_str(&self) -> String {
        return toml::to_string(self).unwrap();
    }

    pub fn from_opts(&mut self, opts: &cli::Opts) {
        self.interval = opts.interval;
        self.threshold_interval = opts.threshold_interval;
        self.connection_timeout = opts.connection_timeout;
        self.notify_timeout = opts.notify_timeout;
        self.action_type = opts.action_type;
        self.ignore_thresholds = opts.ignore_thresholds;
    }

    pub fn get_action_type(&self) -> ActionType {
        match self.action_type {
            0 => ActionType::Click,
            1 => ActionType::Button,
            _ => ActionType::Button,
        }
    }
}
