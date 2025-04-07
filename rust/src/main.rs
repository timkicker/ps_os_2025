use rand::Rng;
use std::env;
use std::fs::File;
use std::io::{self, BufWriter, Write};
use std::process;

fn main() {
    // Collect command-line arguments
    let args: Vec<String> = env::args().collect();

    if args.len() != 5 {
        print_usage(&args[0]);
    }

    // Parse arguments: using usize for width/height, f32 for density, and i32 for steps
    let width: usize = args[1].parse().unwrap();
    let height: usize = args[2].parse().unwrap();
    let density: f32 = args[3].parse().unwrap();
    let steps: i32 = args[4].parse().unwrap();

    println!("width: {}", width);
    println!("height: {}", height);
    println!("density: {}%", density * 100.0);
    println!("steps: {}", steps);

    // Create the initial field
    let mut field = init_field(width, height, density);

    // Run the simulation for the given number of steps
    for step in 0..steps {
        // Compute the next generation (new_field)
        field = run_gen_cycle(&field, width, height);
        // Write the current state to a PBM file
        if let Err(e) = file_gen(&field, step, width, height) {
            eprintln!("Error writing file at step {}: {}", step, e);
        }
    }
}

/// Initializes a 2D field (Vec<Vec<i32>>) of size height x width,
/// filling it with 0s or 1s based on the given density.
fn init_field(width: usize, height: usize, density: f32) -> Vec<Vec<i32>> {
    let mut field = vec![vec![0; width]; height];
    let mut rng = rand::rng();

    for h in 0..height {
        for w in 0..width {
            // Generate a random number between 0.0 and 1.0
            let rand_val: f32 = rng.random();
            field[h][w] = if rand_val < density { 1 } else { 0 };
        }
    }
    field
}

/// Computes the next generation of the field according to the Game of Life rules.
/// Returns a new field.
fn run_gen_cycle(field: &Vec<Vec<i32>>, width: usize, height: usize) -> Vec<Vec<i32>> {
    let mut new_field = vec![vec![0; width]; height];

    for h in 0..height {
        for w in 0..width {
            let cell = field[h][w];
            let neighbors = get_neighbors(field, width, height, h as i32, w as i32);
            // Apply the rules:
            // 0 or 1 neighbor: cell dies (underpopulation)
            // 2 neighbors: cell remains the same
            // 3 neighbors: cell becomes alive or remains alive (reproduction)
            // More than 3: cell dies (overpopulation)
            new_field[h][w] = match neighbors {
                0 | 1 => 0,
                2 => cell,
                3 => 1,
                _ => 0,
            };
        }
    }

    new_field
}

/// Returns the value of a neighbor cell located at (h_cell + h_neighbor, w_cell + w_neighbor).
/// Returns 0 if the neighbor is out of bounds.
fn get_single_neighbor_secure(
    field: &Vec<Vec<i32>>,
    width: usize,
    height: usize,
    h_cell: i32,
    w_cell: i32,
    h_neighbor: i32,
    w_neighbor: i32,
) -> i32 {
    let new_h = h_cell + h_neighbor;
    let new_w = w_cell + w_neighbor;
    if new_h < 0 || new_h >= height as i32 || new_w < 0 || new_w >= width as i32 {
        0
    } else {
        field[new_h as usize][new_w as usize]
    }
}

/// Counts and returns the number of live neighbors (8 directions) for the cell at (h_cell, w_cell).
fn get_neighbors(field: &Vec<Vec<i32>>, width: usize, height: usize, h_cell: i32, w_cell: i32) -> i32 {
    let mut neighbors = 0;
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1, -1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1,  0);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1,  1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell,  0, -1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell,  0,  1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell,  1, -1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell,  1,  0);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell,  1,  1);
    neighbors
}

/// Generates a PBM file of the field at a given generation step.
/// The file is named "gol_XXXXX.pbm", where XXXXX is the zero-padded step number.
fn file_gen(field: &Vec<Vec<i32>>, step: i32, width: usize, height: usize) -> io::Result<()> {
    let filename = format!("gol_{:05}.pbm", step);
    let path = format!("./{}", filename);
    let file = File::create(&path)?;
    let mut writer = BufWriter::new(file);

    // Write PBM header
    writeln!(writer, "P1")?;
    writeln!(writer, "{} {}", width, height)?;

    // Write the field: each cell separated by a space, each row on a new line
    for row in field {
        for &cell in row {
            write!(writer, "{} ", cell)?;
        }
        writeln!(writer)?;
    }
    Ok(())
}

/// Prints the usage message and exits the program.
fn print_usage(program_name: &str) {
    println!("usage: {} <width> <height> <density> <steps>", program_name);
    process::exit(1);
}
