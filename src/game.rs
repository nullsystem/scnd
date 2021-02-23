use crate::{config, notify};

#[tokio::main]
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
