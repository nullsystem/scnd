use notify_rust::Notification;
use std::process::Command;
use std::thread;
use crate::config::ActionType;

#[cfg(unix)]
use notify_rust::Hint;

pub async fn new(summary: &str, body: &str, timeout: u32, appid: u32, action_type: ActionType) -> Result<(), Box<dyn std::error::Error>> {
    let summary: String = summary.into();
    let body: String = body.into();

    thread::spawn(move || {
        let mut notify = Notification::new();
        notify.summary(&summary)
            .body(&body)
            .timeout((timeout * 1000) as i32);

        #[cfg(unix)]
        {
            notify.hint(Hint::Resident(true));
        }

        let notify = match action_type {
            ActionType::Button => notify.action("run", "Launch game"),
            ActionType::Click => notify.action("default", "default"),
        };

        #[cfg(windows)]
        {
            notify.show()
                .unwrap();
        }

        #[cfg(unix)]
        {
            notify.show()
                .unwrap()
                .wait_for_action({|action|
                    match action {
                        "run" | "default" => {
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
        }

    });

    Ok(())
}

#[cfg(unix)]
pub fn daemon_startup() {
    Notification::new()
        .summary("steamcountsnotifyd")
        .body("steamcountsnotifyd daemon started up")
        .timeout(5000)
        .show()
        .unwrap();
}

