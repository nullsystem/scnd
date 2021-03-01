use crate::{config, notify, server};
use futures::future::join_all;
use std::{thread, time};
use tokio::task;

pub async fn main_loop(cfg: &config::Config) -> Result<(), Box<dyn std::error::Error>> {
    let notify_timeout = cfg.notify_timeout.clone();
    let interval = cfg.interval.clone();
    let threshold_interval = cfg.threshold_interval.clone();
    let connection_timeout = cfg.connection_timeout.clone();
    let action_type = cfg.get_action_type();

    let mut tasks: Vec<task::JoinHandle<()>> = vec![];

    for game in &cfg.games {
        let game = game.clone();
        let url = format!(
                "https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid={appid}",
                appid = game.appid
            );
        let client = reqwest::Client::builder()
            .timeout(time::Duration::from_secs(connection_timeout as u64))
            .build()?;
        let action_type = action_type.clone();

        tasks.push(tokio::spawn(async move {
            loop {
                match client.get(&url).send().await {
                    Ok(resp) => match resp.text().await {
                        Ok(text) => match serde_json::from_str(&text) {
                            Ok(text_json) => {
                                let text_json: serde_json::Value = text_json;
                                let count = text_json["response"]["player_count"]
                                    .to_string()
                                    .parse::<u32>()
                                    .unwrap();

                                let current_interval = if count >= game.threshold {
                                    match notify::new(
                                        &game.name,
                                        &format!("{} - {} Players Online", game.name, count),
                                        notify_timeout,
                                        game.appid,
                                        action_type.clone(),
                                    )
                                    .await
                                    {
                                        Err(why) => println!(
                                            "{} - {}: cannot notify: {}",
                                            game.name, game.appid, why
                                        ),
                                        Ok(_) => (),
                                    }

                                    threshold_interval
                                } else {
                                    interval
                                };

                                println!(
                                    "{} - {}: {}/{}: Now waiting for {} mins...",
                                    game.name, game.appid, count, game.threshold, current_interval
                                );
                                thread::sleep(time::Duration::from_secs(
                                    (60 * current_interval) as u64,
                                ));
                            }
                            Err(why) => eprintln!("ERROR: Cannot extract json from text - {}", why),
                        },
                        Err(why) => eprintln!("ERROR: Cannot get the response text - {}", why),
                    },
                    Err(why) => {
                        eprintln!("ERROR: Cannot download appid '{}' - {}", game.appid, why)
                    }
                }
            }
        }));
    }

    for server in &cfg.servers {
        let server = server.clone();
        let action_type = action_type.clone();

        tasks.push(tokio::spawn(async move {
            loop {
                match server::get_info(&server.address) {
                    Ok(info) => {
                        let current_interval = if info.players >= server.threshold {
                            notify::server(&info, notify_timeout, &server.address, action_type.clone());
                            threshold_interval
                        } else {
                            interval
                        };

                        println!(
                            "{} - {}/{} - Waiting for {} mins...",
                            server.address, info.players, server.threshold, current_interval
                        );
                        thread::sleep(time::Duration::from_secs((60 * current_interval) as u64));
                    }
                    Err(why) => {
                        eprintln!("ERROR: {}", why);
                        eprintln!("Retrying in {} mins...", interval);
                        thread::sleep(time::Duration::from_secs((60 * interval) as u64));
                    }
                }
            }
        }));
    }

    join_all(tasks).await;

    Ok(())
}

