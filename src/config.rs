use serde::{Serialize, Deserialize};
use std::default::Default;

#[derive(Serialize, Deserialize)]
pub struct Config {
    pub interval: u32,
    pub threshold_interval: u32,
    pub connection_timeout: u32,
    pub notify_timeout: u32,
    pub action_type: u32,
    pub games: Vec<ConfigGame>,
}

#[derive(Serialize, Deserialize)]
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

pub fn from_str(s: &str) -> Config {
    return toml::from_str(s).unwrap();
}

