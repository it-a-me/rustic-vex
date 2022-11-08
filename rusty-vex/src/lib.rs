#![no_std]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
mod motor;

mod rust_usercontrol;

use panic_halt as _;

#[no_mangle]
pub extern "C" fn add(a: usize, b: usize) -> usize {
    a + b + 5
}

#[no_mangle]
pub extern "C" fn rust_move_motor(motor: usize, speed: i8) -> i32{
    unsafe {
        motor::motor_move(motor as u8, speed as i32);
    }
    0
}

pub use rust_usercontrol::rust_usercontrol;
