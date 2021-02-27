use std::error::Error;

#[derive(Clone)]
pub struct Info {
    pub name: String,
    pub map: String,
    pub game: String,
    pub players: u32,
    pub max_players: u32,
}

pub fn get_info(address: &str) -> Result<Info, Box<dyn Error>> {
    match a2s::A2SClient::new() {
        Ok(client) => {
            match client.info(address) {
                Ok(result) => {
                    Ok(Info {
                        name: result.name,
                        map: result.map,
                        game: result.game,
                        players: result.players.to_string().parse::<u32>().unwrap(),
                        max_players: result.max_players.to_string().parse::<u32>().unwrap(),
                    })
                }
                Err(why) => {
                    return Err(format!("server::get_info ERROR: {}", why).into());
                }
            }
        }
        Err(why) => {
            return Err(format!("server::get_info ERROR: {}", why).into());
        }
    }
}

