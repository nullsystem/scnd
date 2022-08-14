use crate::{config, notify, server};
use futures::future::join_all;
use msq::{Filter, MSQClient, Region};
use std::{thread, time};
use tokio::task;

pub async fn main_loop(
    cfg: &config::Config,
    single_check: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let cfg = cfg.clone();
    let mut tasks: Vec<task::JoinHandle<()>> = vec![];

    match &cfg.games {
        Some(games) => {
            for game in games {
                let game = game.clone();
                let cfg = cfg.clone();

                tasks.push(tokio::spawn(async move {
                    loop {
                        match game_count_get(&cfg, &game, single_check).await {
                            Ok(_) => (),
                            Err(why) => {
                                eprintln!("{}", why);
                            }
                        }

                        if single_check {
                            break;
                        }
                    }
                }));
            }
        },
        None => (),
    }

    match &cfg.games_servers {
        Some(games_servers) => {
            for game in games_servers {
                let game = game.clone();
                let cfg = cfg.clone();

                tasks.push(tokio::spawn(async move {
                    loop {
                        match master_game_count_get(&cfg, &game, single_check).await {
                            Ok (_) => (),
                            Err(why) => {
                                eprintln!("{}", why);
                            }
                        }

                        if single_check {
                            break;
                        }
                    }
                }));
            }
        },
        None => (),
    }

    match &cfg.servers {
        Some(servers) => {
            for server in servers {
                let cfg = cfg.clone();
                let server = server.clone();
                tasks.push(tokio::spawn(async move {
                    loop {
                        server_get(&cfg, &server, single_check).await;

                        if single_check {
                            break;
                        }
                    }
                }));
            }
        },
        None => (),
    }

    join_all(tasks).await;

    Ok(())
}

async fn server_get(cfg: &config::Config, server: &config::ConfigServer, single_check: bool) {
    match server::get_info(&server.address).await {
        Ok(info) => {
            let current_interval = if info.players >= server.threshold || cfg.ignore_thresholds {
                let display_name = if server.name == "" {
                    &info.name
                } else {
                    &server.name
                };

                if single_check {
                    println!(
                        "SERVER: {} ({}): {} - {}/{} | steam://connect/{}",
                        display_name,
                        info.game,
                        info.map,
                        info.players,
                        info.max_players,
                        &server.address
                    );
                } else {
                    notify::server(
                        &info,
                        cfg.notify_timeout,
                        display_name,
                        &server.address,
                        cfg.get_action_type(),
                    );
                }
                match server.threshold_interval {
                    None => cfg.threshold_interval,
                    Some(server_threshold_interval) => server_threshold_interval,
                }
            } else {
                match server.interval {
                    None => cfg.interval,
                    Some(server_interval) => server_interval,
                }
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
                let error_interval = match server.interval {
                    None => cfg.interval,
                    Some(server_interval) => server_interval,
                };

                eprintln!("Retrying in {} mins...", error_interval);
                thread::sleep(time::Duration::from_secs((60 * error_interval) as u64));
            }
        }
    }
}

async fn game_count_get(
    cfg: &config::Config,
    game: &config::ConfigGame,
    single_check: bool,
) -> Result<(), Box<dyn std::error::Error>> {
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


                        match game.threshold_interval {
                            None => cfg.threshold_interval,
                            Some(game_threshold_interval) => game_threshold_interval,
                        }
                    } else {
                        match game.interval {
                            None => cfg.interval,
                            Some(game_interval) => game_interval,
                        }
                    };

                    if !single_check {
                        println!(
                            "{} - {}: {}/{}: Now waiting for {} mins...",
                            game.name, game.appid, count, game.threshold, current_interval
                        );
                        thread::sleep(time::Duration::from_secs((60 * current_interval) as u64));
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

async fn master_game_count_get(
    cfg: &config::Config,
    game: &config::ConfigGameServer,
    single_check: bool,
) -> Result<(), std::io::Error> {
    let mut client = MSQClient::new().await?;
    client.connect("hl2master.steampowered.com:27011").await?;
    client.max_servers_on_query(32);
    let servers = client
        .query(Region::All, Filter::new().appid(game.appid))
        .await?;
    let mut player_counter = 0;
    let mut servers_info: Vec<server::Info> = vec![];
    for server in &servers {
        match server::get_info(server).await {
            Ok(info) => {
                player_counter += info.players;
                servers_info.push(info);
            }
            Err(why) => {
                eprintln!("ERROR: {}", why);
            }
        }
    }

    if single_check {
        if player_counter >= game.threshold_game || cfg.ignore_thresholds {
            println!("{} - {} Players Online (Queries: {})", game.name, player_counter, servers.len());

            for info in servers_info {
                if info.players >= game.threshold_server || (cfg.ignore_thresholds && info.players > 0) {
                    println!(
                        "{} | {} | {}/{} ({})",
                        info.name, info.map, info.players, info.max_players, info.bots
                    );
                }
            }
        }
    } else {
        if player_counter >= game.threshold_game || cfg.ignore_thresholds {
            let mut body_info = String::new();

            for info in servers_info {
                if info.players >= game.threshold_server || (cfg.ignore_thresholds && info.players > 0) {
                    body_info += &format!(
                        "{} | {} | {}/{} ({})\n",
                        info.name, info.map, info.players, info.max_players, info.bots
                    );
                }
            }

            match notify::new(
                &format!("{} - {} Players Online (Queries: {})", game.name, player_counter, servers.len()),
                &body_info,
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

        let current_interval = if player_counter >= game.threshold_game {
            match game.threshold_interval {
                None => cfg.threshold_interval,
                Some(game_threshold_interval) => game_threshold_interval,
            }
        } else {
            match game.interval {
                None => cfg.interval,
                Some(game_interval) => game_interval,
            }
        };

        println!("GAMESERVER: {}: {}/{} ({}): Now waiting for {} mins...",
            game.name, player_counter, game.threshold_game, servers.len(), current_interval);
        thread::sleep(time::Duration::from_secs((60 * current_interval) as u64));
    }

    Ok(())
}

