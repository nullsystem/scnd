use notify_rust::{Notification, Hint};
use std::process::Command;
use std::thread;

pub async fn new(summary: &str, body: &str, timeout: u32, appid: u32) -> Result<(), Box<dyn std::error::Error>> {
    let summary: String = summary.into();
    let body: String = body.into();

    thread::spawn(move || {
        Notification::new()
            .summary(&summary)
            .body(&body)
            .action("run", "Launch game")
            .hint(Hint::Resident(true))
            .timeout(timeout as i32)
            .show()
            .unwrap()
            .wait_for_action({|action|
                match action {
                    "run" => {
                        thread::spawn(move || {
                            Command::new("steam")
                                .arg(format!("steam://rungameid/{}", appid))
                                .spawn()
                                .expect("Cannot launch steam game");
                        });
                    },
                    "__closed" => (),
                    _ => ()
                }
            });
    });

    Ok(())
}

