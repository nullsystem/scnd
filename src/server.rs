#[derive(Clone)]
pub struct Info {
    pub name: String,
    pub map: String,
    pub game: String,
    pub players: u32,
    pub max_players: u32,
}

pub fn get_info(address: &str) -> Info {
    let client = a2s::A2SClient::new().unwrap();
    let result = client.info(address).unwrap();

    return Info {
        name: result.name,
        map: result.map,
        game: result.game,
        players: result.players.to_string().parse::<u32>().unwrap(),
        max_players: result.max_players.to_string().parse::<u32>().unwrap(),
    };
}
