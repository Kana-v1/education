#![no_std]
#![no_main]

use microbit as _;
use panic_rtt_target as _;
use rtt_target::{rprintln, rtt_init_print};

#[cortex_m_rt::entry]
fn main() -> ! {
    rtt_init_print!();

    rprintln!("Hello, world!");

    let m = 1;

    let s = 14;
    let q = 25;
    let w = 25;

    loop {}
}
