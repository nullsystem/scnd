use clap::Clap;

#[derive(Clap, Debug)]
#[clap(version = "0.0.3 Alpha", author = "mtcw <mtcw@disroot.org>")]
pub struct Opts {
    /// Run the program in daemonized/background mode
    #[clap(short, long)]
    pub daemonize: bool,

    /// How long in minutes to delay between each player count fetch
    #[clap(short, default_value = "1")]
    pub interval: u32,

    /// When the threshold's achieved, what interval gets extended to
    #[clap(short, default_value = "2")]
    pub threshold_interval: u32,

    /// How long in seconds the connection stay puts
    #[clap(short, default_value = "10")]
    pub connection_timeout: u32,

    /// How long in seconds the notification stays shown
    #[clap(short, default_value = "10")]
    pub notify_timeout: u32,

    /// Which type of clickable notification should use
    #[clap(short, default_value = "1")]
    pub action_type: u32,
}

pub fn parse() -> Opts {
    return Opts::parse();
}

