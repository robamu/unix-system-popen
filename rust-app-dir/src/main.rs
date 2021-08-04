use std::{thread, time};

fn main() {
    println!("Sleeping for two seconds!");
    thread::sleep(time::Duration::from_secs(2));
    println!("Sleeping done!");

}
