
#include "aoc.h"
#include <stdbool.h>
#include <stdlib.h>

#define NUM_NUMBERS 100
#define NUM_BOARDS 100

typedef int board_t[25];

void print_board(board_t board) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            printf("%2d ", board[row * 5 + col]);
        }
        printf("\n");
    }
    printf("\n");
}

char *parse_board(char *p, board_t board) {
    for (int i = 0; i < 25; i++) {
        board[i] = strtol(p, &p, 10);
    }
    return p;
}

int compute_result(int numbers[], int played_number_idx, board_t board) {
    int sum = 0;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            int n = board[row * 5 + col];
            bool is_n_played = false;
            for (int k = 0; k <= played_number_idx; k++) {
                if (n == numbers[k]) {
                    is_n_played = true;
                    break;
                }
            }
            if (!is_n_played) {
                sum += n;
            }
        }
    }

    return sum * numbers[played_number_idx];
}

bool is_winner(int numbers[], int played_number_idx, board_t board) {
    for (int horiz = 0; horiz < 2; horiz++) {
        for (int i = 0; i < 5; i++) {
            bool is_bingo = true;

            for (int j = 0; j < 5; j++) {
                int n = (horiz == 0) ? board[i * 5 + j] : board[j * 5 + i];
                bool is_n_played = false;

                for (int k = 0; k <= played_number_idx; k++) {
                    if (n == numbers[k]) {
                        is_n_played = true;
                        break;
                    }
                }

                if (!is_n_played)
                    is_bingo = false;
            }

            if (is_bingo) {
                return true;
            }
        }
    }
    return false;
}

aoc_result_t day4(char *input, UNUSED int len) {
    aoc_result_t result = {0};
    int numbers[NUM_NUMBERS] = {0};
    int winners[NUM_BOARDS] = {0};
    board_t boards[100];

    char *p = input;

    // Parse the list of numbers to play
    for (int i = 0; i < NUM_NUMBERS; i++) {
        numbers[i] = strtol(p, &p, 10);
        p++; // skip comma
    }

    // Parse the bingo boards
    for (int i = 0; i < NUM_BOARDS; i++) {
        p = parse_board(p, boards[i]);
    }

    // Play the numbers
    for (int i = 0; i < NUM_NUMBERS; i++) {
        for (int j = 0; j < NUM_BOARDS; j++) {
            if (winners[j])
                continue;

            if (is_winner(numbers, i, boards[j])) {
                winners[j] = 1;

                int res = compute_result(numbers, i, boards[j]);
                if (result.p1 == 0) {
                    // P1 solution: first winner
                    result.p1 = res;
                } else {
                    result.p2 = res;
                }
            }
        }
    }

    return result;
}
