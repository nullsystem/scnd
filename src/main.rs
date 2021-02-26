mod cli;
mod config;
mod count;
mod notify;
mod server;

use futures::executor::block_on;
use std::fs::{create_dir, File};
use std::io::prelude::*;

#[cfg(unix)]
use daemonize::Daemonize;

#[cfg(unix)]
fn daemonize() {
    let stdout = File::create("/tmp/steamcountsnotifyd.out").unwrap();
    let stderr = File::create("/tmp/steamcountsnotifyd.err").unwrap();

    let daemonize = Daemonize::new()
        .pid_file("/tmp/steamcountsnotifyd.pid")
        .chown_pid_file(true)
        .working_directory("/tmp")
        .umask(0o777)
        .stdout(stdout)
        .stderr(stderr);

    match daemonize.start() {
        Ok(_) => {
            println!("Running in daemonize mode...");
            notify::daemon_startup();
        },
        Err(why) => eprintln!("ERROR: Cannot daemonize: {}", why),
    }
}

#[cfg(windows)]
fn daemonize() {
}

#[tokio::main]
async fn main() -> std::io::Result<()> {
    let opts: cli::Opts = cli::parse();
    //println!("{:#?}", opts);

    let mut cfg: config::Config = Default::default();
    cfg.from_opts(&opts);

    if let Some(cfgdir) = dirs_next::config_dir() {
        let mut cfgfilepath = cfgdir;
        cfgfilepath.push("steamcountsnotifyd");
        cfgfilepath.push("config");
        cfgfilepath.set_extension("toml");

        let display = cfgfilepath.display();

        if cfgfilepath.exists() {
            if opts.generate_config {
                println!("NOTICE: File already exists. Did nothing.");
                return Ok(());
            }

            let mut file = match File::open(&cfgfilepath) {
                Err(why) => panic!("couldn't open {}: {}", display, why),
                Ok(file) => file,
            };

            let mut file_as_str = String::new();
            match file.read_to_string(&mut file_as_str) {
                Err(why) => panic!("couldn't read {}: {}", display, why),
                Ok(_) => cfg.from_toml_str(&file_as_str),
            }
        } else if opts.generate_config {
            let mut cfgfiledir = cfgfilepath.clone();
            cfgfiledir.pop();
            match create_dir(&cfgfiledir) {
                Err(_) => (),
                Ok(_) => (),
            };

            match File::create(&cfgfilepath) {
                Err(why) => panic!("couldn't write to {}: {}", display, why),
                Ok(mut file) => {
                    file.write_all(cfg.to_toml_str().as_bytes())?;
                    println!("Generated configuration file in: {}", display);
                    return Ok(());
                }
            };
        } else {
            println!("NOTICE: Configuration file not found. Using defaults.");
        }
    }

    if opts.daemonize {
        daemonize();
    }

    match block_on(count::main_loop(&cfg)) {
        Err(_) => {
            eprintln!("ERROR: An error in the main loop occured.");
            Ok(())
        }
        Ok(_) => Ok(()),
    }
}

