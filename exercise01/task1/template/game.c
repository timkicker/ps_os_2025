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


    srand(time(NULL));

    int **field = init_world(density, width, height);

    for (int i = 0; i < steps; i++) {
        run_generation_cycle(field, width, height);
        print_field(field, width, height);
    }

    return EXIT_SUCCESS;
}

// init world and cells 
int** init_world(double density, int width, int height) {
    int **field = (int **)calloc(height, sizeof(int *));
    if (field == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        field[i] = (int *)calloc(width, sizeof(int));
        if (field[i] == NULL) {
            perror("Memory allocation failed");
            return NULL;
        }
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {

            double random_value = rand() / (double)RAND_MAX;

			if (random_value < density) {
    			field[h][w] = 1;
			} else {
    			field[h][w] = 0;
			}
        }
    }

    return field;
}

// simulate the next generation of the field and save it
void run_generation_cycle(int** field, int width, int height) {
    int **new_field = (int **)calloc(height, sizeof(int *));
    for (int i = 0; i < height; i++) {
        new_field[i] = (int *)calloc(width, sizeof(int));
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            char cell = field[h][w];
            char neighbors = get_neighbors(field, width, height, h, w);

            switch (neighbors) {
                case 0:
                case 1:
                    new_field[h][w] = 0;
                    break;
                case 2:
                    new_field[h][w] = cell;
                    break;
                case 3:
                    new_field[h][w] = 1;
                    break;
                default:
                    new_field[h][w] = 0;
                    break;
            }
        }
    }

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            field[h][w] = new_field[h][w];
        }
    }

    for (int i = 0; i < height; i++) {
        free(new_field[i]);
    }
    free(new_field);
}

// get the 8 surrounding neighbors of a cell
char get_neighbors(int** field, int width, int height, int h_cell, int w_cell) {
    char neighbors = 0;

    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1, -1);	// bottom left
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1, 0);	// bottom middle
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, -1, 1);	
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, 0, -1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, 0, 1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, 1, -1);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, 1, 0);
    neighbors += get_single_neighbor_secure(field, width, height, h_cell, w_cell, 1, 1);

    return neighbors;
}

// gets single neighbor (dead one if out of bounds)
char get_single_neighbor_secure(int** field, int width, int height, int h_cell, int w_cell, int h_neighbor, int w_neighbor) {

	if (h_cell + h_neighbor < 0 || 
		h_cell + h_neighbor >= height || 
		w_cell + w_neighbor < 0 || 
		w_cell + w_neighbor >= width) {

        return 0;
    }
    return field[h_cell + h_neighbor][w_cell + w_neighbor];
}

// print entire state of the field 
void print_field(int** field, int width, int height) {
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            char state = field[h][w];
            if (state)
                printf("■");
            else
                printf("□");
        }
        printf("\n");
    }
}