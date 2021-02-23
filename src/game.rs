use crate::{config, notify};
use std::{thread, time};

pub async fn req_then_notify(game: &config::ConfigGame, notify_timeout: u32) -> Result<(), Box<dyn std::error::Error>> {
    let resp = reqwest::get(&format!(
        "https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid={appid}",
        appid = game.appid
    )).await?;

    let resp_data = resp.text().await?;
    let resp_json: serde_json::Value = serde_json::from_str(&resp_data)?;
    let count = resp_json["response"]["player_count"].clone();

    match notify::new(
        &game.name,
        &format!("{} - {} Players Online", game.name, count),
        notify_timeout,
    ) {
        Err(why) => println!("{} - {}: cannot notify: {}", game.name, game.appid, why),
        Ok(_) => (),
    }

    Ok(())
}

pub async fn main_loop(cfg: &config::Config) {
    loop {
        for game in &cfg.games {
            match req_then_notify(&game, cfg.notify_timeout).await {
                Err(why) => println!("ERROR: {}: {}", &game.name, why),
                Ok(_) => (),
            };
        }
        thread::sleep(time::Duration::from_secs(60 * cfg.interval as u64));
    }
}

