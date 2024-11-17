#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COL_SIZE 9
#define ROW_SIZE 9

int move_count = 0; // Global variable to count the number of moves

// Function to print the Sudoku grid
void print_sudoku(int state[COL_SIZE][ROW_SIZE])
{
    printf("****** STATE ******\n");
    for (int i = 0; i < COL_SIZE; i++)
    {
        for (int j = 0; j < ROW_SIZE; j++)
        {
            if (state[i][j] == 0)
            {
                printf("_ ");
            }
            else
                printf("%d ", state[i][j]);
            if ((j + 1) % 3 == 0 && j + 1 < 9)
            {
                printf("| ");
            }
        }
        printf("\n");
        if ((i + 1) % 3 == 0 && i + 1 < 9)
        {
            printf("---------------------\n");
        }
    }
}

// Check if a move is valid
int is_valid_move(int state[COL_SIZE][ROW_SIZE], int row, int col, int num)
{
    // Check row
    for (int i = 0; i < COL_SIZE; i++)
    {
        if (state[row][i] == num && i != col)
        {
            return 0;
        }
    }

    // Check column
    for (int i = 0; i < ROW_SIZE; i++)
    {
        if (state[i][col] == num && i != row)
        {
            return 0;
        }
    }

    // Check the 3x3 subgrid
    int square_col_start = (col / 3) * 3;
    int square_row_start = (row / 3) * 3;
    for (int i = square_row_start; i < square_row_start + 3; i++)
    {
        for (int j = square_col_start; j < square_col_start + 3; j++)
        {
            if (state[i][j] == num)
            {
                return 0;
            }
        }
    }
    return 1;
}

// Function to generate a list of possible moves
void generate_moves(int state[COL_SIZE][ROW_SIZE], int moves[COL_SIZE * ROW_SIZE][3], int *move_count_out)
{
    int move_idx = 0;
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            if (state[i][j] != 0)
            {
                continue;
            }
            for (int num = 1; num <= 9; num++)
            {
                if (is_valid_move(state, i, j, num))
                {
                    moves[move_idx][0] = i;   // Row
                    moves[move_idx][1] = j;   // Col
                    moves[move_idx][2] = num; // Number
                    move_idx++;
                }
            }
        }
    }
    *move_count_out = move_idx;
}

// Check if the Sudoku puzzle is solved
int is_terminal(int state[COL_SIZE][ROW_SIZE])
{
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            if (state[i][j] == 0)
            {
                return 0; // Puzzle is not solved
            }
        }
    }
    return 1; // Puzzle is solved
}

// Backtracking function to solve the Sudoku puzzle
int backtrack_search(int state[COL_SIZE][ROW_SIZE])
{
    if (is_terminal(state))
    {
        return 1; // Puzzle is solved
    }

    int moves[COL_SIZE * ROW_SIZE][3]; // List of possible moves
    int num_moves;
    generate_moves(state, moves, &num_moves);

    for (int i = 0; i < num_moves; i++)
    {
        int row = moves[i][0];
        int col = moves[i][1];
        int number = moves[i][2];

        state[row][col] = number; // Try the move
        move_count++;             // Increment move count

        if (backtrack_search(state))
        {
            return 1; // Puzzle solved
        }

        // Undo the move
        state[row][col] = 0;
    }

    return 0; // No solution found
}

// Function to generate a random Sudoku puzzle by removing numbers from the solution
void generate_random_sudoku(int solution[COL_SIZE][ROW_SIZE], int left_grids)
{
    srand(time(NULL));
    int positions[COL_SIZE * ROW_SIZE][2];
    int idx = 0;

    // Create a list of all positions
    for (int i = 0; i < COL_SIZE; i++)
    {
        for (int j = 0; j < ROW_SIZE; j++)
        {
            positions[idx][0] = i;
            positions[idx][1] = j;
            idx++;
        }
    }

    // Shuffle the positions randomly
    for (int i = 0; i < COL_SIZE * ROW_SIZE; i++)
    {
        int j = rand() % (COL_SIZE * ROW_SIZE);
        int temp_row = positions[i][0], temp_col = positions[i][1];
        positions[i][0] = positions[j][0];
        positions[i][1] = positions[j][1];
        positions[j][0] = temp_row;
        positions[j][1] = temp_col;
    }

    // Remove 'left_grids' number of cells
    for (int i = 0; i < left_grids; i++)
    {
        int row = positions[i][0];
        int col = positions[i][1];
        solution[row][col] = 0;
    }
}

int main()
{
    int sudoku_puzzle_solution[COL_SIZE][ROW_SIZE] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}};

    // Create a puzzle by removing 15 cells from the solved puzzle
    generate_random_sudoku(sudoku_puzzle_solution, 15);

    // Print the generated Sudoku puzzle
    printf("Generated Sudoku Puzzle:\n");
    print_sudoku(sudoku_puzzle_solution);

    // Solve the Sudoku puzzle using backtracking
    move_count = 0;
    if (backtrack_search(sudoku_puzzle_solution))
    {
        printf("\nSolution:\n");
        print_sudoku(sudoku_puzzle_solution);
        printf("\nTotal moves made: %d\n", move_count);
    }
    else
    {
        printf("\nNo solution found.\n");
    }

    return 0;
}
