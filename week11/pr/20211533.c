#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// macro
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// wall bit flag
#define WALL_NONE 0b0000
#define WALL_UP 0b1000
#define WALL_RIGHT 0b0100
#define WALL_DOWN 0b0010
#define WALL_LEFT 0b0001
#define WALL_ALL 0b1111

// wall char
#define WALL_VERTICAL "|"
#define WALL_HORIZONTAL "-"
#define WALL_CORNER "+"
#define WALL_BLANK " "
#define MAZE_SPACE " "

int getRandBool(int seed, int i, int j, int w);
void createMaze(int i, int h, int w, int* maze, int* maze_set, int seed);
void printMaze(FILE* outputFile, int h, int w, int* maze, int* maze_set);

int main() {

    int h, w;

    // input file
    FILE* inputFile = fopen("input.txt", "r");
    fscanf(inputFile, "%d %d", &w, &h);

    // malloc
    int* maze = (int*)malloc(sizeof(int) * h * w);
    int* maze_set = (int*)malloc(sizeof(int) * h * w);

    // init 
    for (int i = 0; i < h * w; i++) {
        maze[i] = WALL_UP | WALL_DOWN | WALL_LEFT | WALL_RIGHT;
        maze_set[i] = 0;
    }
    int set = 1;

    // seed
    int seed = time(NULL);

    // createMaze
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (maze_set[i * w + j] > 0) continue;
            maze_set[i * w + j] = set++;
        }
        createMaze(i, h, w, maze, maze_set, seed);
    }

    // printMaze
    FILE* outputFile = fopen("output.txt", "w");
    printMaze(outputFile, h, w, maze, maze_set);

    free(maze);
    free(maze_set);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int getRandBool(int seed, int i, int j, int w) {
    srand(seed + i * w + j);
    return rand() % 2 == 1;
}

void createMaze(int i, int h, int w, int* maze, int* maze_set, int seed) {

    for (int j = 0; j < w - 1; j++) {
        if (abs(maze_set[i * w + j]) != abs(maze_set[i * w + j + 1]) && (i == h - 1 || getRandBool(seed, i, j, w))) {
            maze[i * w + j] &= (WALL_ALL ^ WALL_RIGHT);
            maze[i * w + j + 1] &= (WALL_ALL ^ WALL_LEFT);
            int change_maze_set = maze_set[i * w + j + 1];
            for(int k=0;k<w;k++) {
                if(maze_set[i*w+k] == change_maze_set) {
                    maze_set[i*w+k] = maze_set[i * w + j];
                }
            }
        }
    }

    if (i == h - 1) return;

    for (int j = 0; j < w; j++) {

        if (maze_set[i * w + j] < 0) continue;

        int set = maze_set[i * w + j];
        maze_set[i * w + j] = -set;

        int last_index = j;
        int flag = 0;

        for (int jj = j + 1; jj < w; jj++) {

            if (maze_set[i * w + jj] == set) {
                maze_set[i * w + jj] = -set;
                last_index = jj;

                if (getRandBool(seed, i, jj, w)) {
                    maze[i * w + jj] &= (WALL_ALL ^ WALL_DOWN);
                    maze[(i + 1) * w + jj] &= (WALL_ALL ^ WALL_UP);
                    maze_set[(i + 1) * w + jj] = set;
                    flag = 1;
                }

            }

        }

        if (!flag) {
            maze[i * w + last_index] &= (WALL_ALL ^ WALL_DOWN);
            maze[(i + 1) * w + last_index] &= (WALL_ALL ^ WALL_UP);
            maze_set[(i + 1) * w + last_index] = set;
        }

    }

}

void printMaze(FILE* outputFile, int h, int w, int* maze, int* maze_set) {

    // 맨 윗줄
    for (int j = 0; j < w; j++) {
        fprintf(outputFile, WALL_CORNER);
        if (maze[j] & WALL_UP) {
            fprintf(outputFile, WALL_HORIZONTAL);
        }
        else {
            fprintf(outputFile, WALL_BLANK);
        }
    }

    fprintf(outputFile, WALL_CORNER);
    fprintf(outputFile, "\n");

    for (int i = 0; i < h; i++) {

        // 맨 왼쪽 줄
        if (maze[i * w] & WALL_LEFT) {
            fprintf(outputFile, WALL_VERTICAL);
        }

        for (int j = 0; j < w; j++) {
            fprintf(outputFile, MAZE_SPACE);
            if (maze[i * w + j] & WALL_RIGHT) {
                fprintf(outputFile, WALL_VERTICAL);
            }
            else {
                fprintf(outputFile, WALL_BLANK);
            }
        }

        fprintf(outputFile, "\n");

        for (int j = 0; j < w; j++) {
            fprintf(outputFile, WALL_CORNER);
            if (maze[i * w + j] & WALL_DOWN) {
                fprintf(outputFile, WALL_HORIZONTAL);
            }
            else {
                fprintf(outputFile, WALL_BLANK);
            }
        }
        fprintf(outputFile, WALL_CORNER);

        fprintf(outputFile, "\n");

    }

}