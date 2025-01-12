#ifndef LIFE_H
# define LIFE_H

typedef struct s_board
{
  int** board;
  int   height;
  int   width;
} t_board;

typedef struct s_pen
{
  int pos_x;
  int pos_y;
  int is_drawing;
} t_pen;

// --- Pen ---
void  pen_toggle(t_pen* p);                                     // Activate deactivate pen 
void  pen_move(t_pen* p, int shift_x, int shift_y, t_board* b); // Move pen

// --- Board ---
void  board_erase(int time);                                    // Clear screen some time (in ms)
void  board_display(t_board* b, int time);                      // Display board for some time (in ms)
int** board_new(int width, int height);                         // New empty board
void  board_init(t_pen* p, t_board* b);

int   is_in_bounds(int x, int y, t_board* b);                   // Check if coordinates are in the board
void  board_free(int width, int height, int** board);           // Free memory

// --- Game of life ---
int   count_neighbors(int origin_x, int origin_y, t_board* b);  // Count alive neighbors
int   must_live(int x, int y, t_board* b);                      // Rules of the game of life
void  next_iteration(t_board* b);                               // Generate next iteration

#endif
