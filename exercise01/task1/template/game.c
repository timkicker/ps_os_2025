#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printUsage(const char* programName) {
	printf("usage: %s <width> <height> <density> <steps>\n", programName);
}

// game of life
int main(int argc, char* argv[]) {
	if(argc != 5) {
		printUsage(argv[0]);
		return EXIT_FAILURE;
	}

	const int width = atoi(argv[1]);
	const int height = atoi(argv[2]);
	const double density = atof(argv[3]);
	const int steps = atoi(argv[4]);

	printf("width:   %4d\n", width);
	printf("height:  %4d\n", height);
	printf("density: %4.0f%%\n", density * 100);
	printf("steps:   %4d\n", steps);

	// Seeding the random number generator so we get a different starting field
	// every time.
	srand(time(NULL));
	int seed = rand();

	int **field = init_world(seed,density,width,height);

	return EXIT_SUCCESS;
}

// init world and cells 
int** init_world(int seed, int density, int width, int height){

	int **field = (int **)calloc(height, sizeof(int *));
    
    if (field == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    // alloc for width
    for (int i = 0; i < height; i++) {
        field[i] = (int *)calloc(width, sizeof(int));
        if (field[i] == NULL) {
            perror("Memory allocation failed");
            return 1;
        }
    }
	return field;
}

// simulate the next generation of the field and save it
void run_generation_cycle(int** field, int width, int height){
	for (int h = 0; h < height; h++){
		for (int w = 0; w < width; w++){

			char cell = field[h][w];
			char neighbors = get_neighbors(field,width,height,h,w);

			// fate dependent on neighbors
			switch(neighbors) {

				// die
				case 0: 
				case 1: 
					field[h][w] = 0;
					break;
				// live
				case 2: 
					if (!cell){
						break;
					}
						
				case 3:
					field[h][w] = 1;
					break;
				// overpopulated
				default: 
					field[h][w] = 0;
					break;
			}

		}
		printf("\n");	
	}
}

// get the 8 surrounding neighbors of a cell
char get_neighbors (int** field, int width, int height, int h_cell, int w_cell){

	char neighbors_top =  field[h_cell - 1][w_cell] + field[h_cell - 1][w_cell - 1] + field[h_cell - 1][w_cell + 1]; // first row
	char neighbors_bottom =  field[h_cell + 1][w_cell] + field[h_cell + 1][w_cell - 1] + field[h_cell + 1 ][w_cell + 1]; // first row
	char neighbors_side = field[h_cell][w_cell - 1] + field[h_cell][w_cell + 1];

	return neighbors_top + neighbors_bottom + neighbors_side;
}

void file_gen (int** field, int width, int height, int step){

	char filename[20];
    snprintf(filename, sizeof(filename), "gol_%05d.pbm", step);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write PBM header
    fprintf(file, "P1\n");
    fprintf(file, "%d %d\n", width, height);


	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			fprintf(file, "%d", field[i][j]);
		}

	}
	fclose(file);
}
