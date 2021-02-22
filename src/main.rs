mod config;

use std::fs::File;
use std::io::prelude::*;

fn main() {
    let mut cfg = Default::default();

    // Read configuration file (if found)
    if let Some(cfgdir) = dirs_next::config_dir() {
        let mut cfgfilepath = cfgdir;
        cfgfilepath.push("steamcountsnotifyd");
        cfgfilepath.push("config");
        cfgfilepath.set_extension("toml");

        let display = cfgfilepath.display();

        if cfgfilepath.exists() {
            let mut file = match File::open(&cfgfilepath) {
                Err(why) => panic!("couldn't open {}: {}", display, why),
                Ok(file) => file,
            };

            let mut file_as_str = String::new();
            match file.read_to_string(&mut file_as_str) {
                Err(why) => panic!("couldn't read {}: {}", display, why),
                Ok(_) => cfg = config::from_str(&file_as_str),
            }
        }
    }

    println!("Cfg: {} {} {} {} {}",
        cfg.interval,
        cfg.threshold_interval,
        cfg.connection_timeout,
        cfg.notify_timeout,
        cfg.action_type);

    for game in &cfg.games {
        println!("{}: {} {}",
            game.name,
            game.appid,
            game.threshold);
    }

    //println!("{}", toml::to_string(&cfg).unwrap());
}

