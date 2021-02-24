use crate::{config, notify};
use std::{thread, time};
use tokio::task;
use futures::future::join_all;

pub async fn main_loop(cfg: &config::Config) {
    let notify_timeout = cfg.notify_timeout.clone();
    let interval = cfg.interval.clone();

    loop {
        let mut tasks: Vec<task::JoinHandle<()>> = vec![];

        for game in &cfg.games {
            let game = game.clone();
            let url = format!(
                    "https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid={appid}",
                    appid = game.appid
                );

            tasks.push(tokio::spawn(async move {
                match reqwest::get(&url).await {
                    Ok(resp) => {
                        match resp.text().await {
                            Ok(text) => {
                                match serde_json::from_str(&text) {
                                    Ok(text_json) => {
                                        let text_json: serde_json::Value = text_json;
                                        let count = text_json["response"]["player_count"].clone();

                                        match notify::new(
                                            &game.name,
                                            &format!("{} - {} Players Online", game.name, count),
                                            notify_timeout,
                                        ) {
                                            Err(why) => println!("{} - {}: cannot notify: {}", game.name, game.appid, why),
                                            Ok(_) => (),
                                        }
                                        thread::sleep(time::Duration::from_secs((60 * interval) as u64));
                                    }
                                    Err(why) => println!("ERROR: Cannot extract json from text - {}", why),
                                }
                            }
                            Err(why) => println!("ERROR: Cannot get the response text - {}", why),
                        }
                    }
                    Err(why) => println!("ERROR: Cannot download appid '{}' - {}", game.appid, why),
                }
            }));
        }

        join_all(tasks).await;
    }
}

