use notify_rust::Notification;

pub fn new(summary: &str, body: &str, timeout: u32) -> Result<(), Box<dyn std::error::Error>> {
    Notification::new()
        .summary(summary)
        .body(body)
        .timeout(timeout as i32)
        .show()?;

    Ok(())
}
