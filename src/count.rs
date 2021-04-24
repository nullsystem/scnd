use crate::{config, notify, server};
use futures::future::join_all;
use std::{thread, time};
use tokio::task;

pub async fn main_loop(cfg: &config::Config, single_check: bool) -> Result<(), Box<dyn std::error::Error>> {
    let cfg = cfg.clone();
    let mut tasks: Vec<task::JoinHandle<()>> = vec![];

    for game in &cfg.games {
        let game = game.clone();
        let cfg = cfg.clone();

        tasks.push(tokio::spawn(async move {
            loop {
                match game_count_get(&cfg, &game, single_check).await {
                    Ok(_) => (),
                    Err(why) => {
                        eprintln!("{}", why);
                    },
                }

                if single_check {
                    break;
                }
            }
        }));
    }

    for server in &cfg.servers {
        let cfg = cfg.clone();
        let server = server.clone();
        tasks.push(tokio::spawn(async move {
            loop {
                server_get(&cfg, &server, single_check);

                if single_check {
                    break;
                }
            }
        }));
    }

    join_all(tasks).await;

    Ok(())
}

fn server_get(cfg: &config::Config, server: &config::ConfigServer, single_check: bool) {
    match server::get_info(&server.address) {
        Ok(info) => {
            let current_interval = if info.players >= server.threshold || cfg.ignore_thresholds {
                let display_name = if server.name == "" {
                    &info.name
                } else {
                    &server.name
                };

                if single_check {
                    println!("SERVER: {} ({}): {} - {}/{} | steam://connect/{}",
                        display_name,
                        info.game,
                        info.map,
                        info.players,
                        info.max_players,
                        &server.address
                    );
                } else {
                    notify::server(&info, cfg.notify_timeout, display_name, &server.address, cfg.get_action_type());
                }
                cfg.threshold_interval
            } else {
                cfg.interval
            };

            if !single_check {
                println!(
                    "{} - {}/{} - Waiting for {} mins...",
                    server.address, info.players, server.threshold, current_interval
                );
                thread::sleep(time::Duration::from_secs((60 * current_interval) as u64));
            }
        }
        Err(why) => {
            eprintln!("ERROR: {}", why);
            if !single_check {
                eprintln!("Retrying in {} mins...", cfg.interval);
                thread::sleep(time::Duration::from_secs((60 * cfg.interval) as u64));
            }
        }
    }
}

async fn game_count_get(cfg: &config::Config, game: &config::ConfigGame, single_check: bool) -> Result<(), Box<dyn std::error::Error>> {
    let client = reqwest::Client::builder()
        .timeout(time::Duration::from_secs(cfg.connection_timeout as u64))
        .build()?;

    let url = format!(
            "https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid={appid}",
            appid = game.appid
        );

    match client.get(&url).send().await {
        Ok(resp) => match resp.text().await {
            Ok(text) => match serde_json::from_str(&text) {
                Ok(text_json) => {
                    let text_json: serde_json::Value = text_json;
                    let count = text_json["response"]["player_count"]
                        .to_string()
                        .parse::<u32>()
                        .unwrap();

                    let current_interval = if count >= game.threshold || cfg.ignore_thresholds {
                        if single_check {
                            println!("GAME:   {} - {} Players Online", game.name, count);
                        } else {
                            match notify::new(
                                &game.name,
                                &format!("{} - {} Players Online", game.name, count),
                                cfg.notify_timeout,
                                game.appid,
                                cfg.get_action_type(),
                            )
                            .await
                            {
                                Err(why) => println!(
                                    "{} - {}: cannot notify: {}",
                                    game.name, game.appid, why
                                ),
                                Ok(_) => (),
                            }
                        }

                        cfg.threshold_interval
                    } else {
                        cfg.interval
                    };

                    if !single_check {
                        println!(
                            "{} - {}: {}/{}: Now waiting for {} mins...",
                            game.name, game.appid, count, game.threshold, current_interval
                        );
                        thread::sleep(time::Duration::from_secs(
                            (60 * current_interval) as u64,
                        ));
                    }
                }
                Err(why) => eprintln!("ERROR: Cannot extract json from text - {}", why),
            },
            Err(why) => eprintln!("ERROR: Cannot get the response text - {}", why),
        },
        Err(why) => {
            eprintln!("ERROR: Cannot download appid '{}' - {}", game.appid, why)
        }
    }

    Ok(())
}


