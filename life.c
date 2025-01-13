#include "life.h"
#include <unistd.h> // read
#include <stdlib.h> // calloc
#include <stdio.h>  // putchar

// --- Pen ---

void  pen_toggle(t_pen* p)
{
  p->is_drawing = !p->is_drawing;
}

void  pen_move(t_pen* p, int shift_x, int shift_y, t_board* b)
{
  if (p->is_drawing)
    b->board[p->pos_y][p->pos_x] = 1;

  int new_x = p->pos_x + shift_x;
  int new_y = p->pos_y + shift_y;

  if (!is_in_bounds(new_x, new_y, b))
     return ;

  p->pos_x = new_x;
  p->pos_y = new_y;

  if (p->is_drawing)
    b->board[p->pos_y][p->pos_x] = 1;
}

// --- Board ---

void  board_erase(int time)
{
  usleep(time * 1000);
  system("clear");
}

void  board_display(t_board* b)
{
  char  alive = 'o';
  char  dead = ' ';

  for (int i = 0; i != b->height; ++i)
  {
    for (int j = 0; j != b->width; ++j)
    {
      if (b->board[i][j])
        putchar(alive);
      else
        putchar(dead);
    }
    putchar('\n');
  }
}

int** board_new(int width, int height)
{
  int** board;

  board = calloc((size_t)height, sizeof(int*));
  for (int i = 0; i != height; ++i)
  {
    board[i] = calloc((size_t)width, sizeof(int));
  }
  return (board);
}

void  board_init(t_pen* p, t_board* b)
{
  char    buffer[2048];
  size_t  nbytes;
  ssize_t bytes_read;

  nbytes = sizeof(buffer);
  bytes_read = read(0, buffer, nbytes);

  for (size_t i = 0; i != bytes_read; ++i)
  {
    if (buffer[i] == 'w')
      pen_move(p, 0, -1, b);
    else if (buffer[i] == 'a')
      pen_move(p, -1, 0, b);
    else if (buffer[i] == 's')
      pen_move(p, 0, 1, b);
    else if (buffer[i] == 'd')
      pen_move(p, 1, 0, b);
    else if (buffer[i] == 'x')
      pen_toggle(p);
  }
}

int is_in_bounds(int x, int y, t_board* b)
{
  if ((x >= 0 && x <= b->width - 1) &&
      (y >= 0 && y <= b->height - 1))
    return (1);
  return(0);
}

void  board_free(t_board* b)
{
  for (int i = 0; i != b->height; ++i)
  {
    free(b->board[i]);
  }
  free(b->board);
};

// --- Game of life ---

/* 
 * Neighbors coordinates
 * (-1, +1) (0, +1) (+1, +1)
 * (-1, 0)  (0, 0)  (+1, 0)
 * (-1, -1) (0, -1) (+1, -1)
 */
int count_neighbors(int origin_x, int origin_y, t_board* b)
{
  int count = 0;
  int neighbors[8][2] = {{-1, 1}, {0, 1}, {1, 1}, {-1, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}};

  int neighbor_x = 0;
  int neighbor_y = 0;

  for (size_t i = 0; i != 8; ++i)
  {
    neighbor_x = origin_x + neighbors[i][0];
    neighbor_y = origin_y + neighbors[i][1];
    if (is_in_bounds(neighbor_x, neighbor_y, b) && 
        b->board[neighbor_y][neighbor_x])
      ++count;
  }
  return (count);
}

int must_live(int x, int y, t_board* b)
{
  int neighbors = count_neighbors(x, y, b);
  if (neighbors == 3 || (b->board[y][x] && neighbors == 2))
    return (1);
  return (0);
}

void  next_iteration(t_board* b)
{
  int** new_board;

  new_board = board_new(b->width, b->height);
  for (size_t i = 0; i != b->height; ++i)
  {
    for (size_t j = 0; j != b->width; ++j)
    {
      new_board[i][j] = must_live(j, i, b);
    }
  }

  board_free(b);
  b->board = new_board;
}

/*
 * Parameters
 * - argv[1] = board height
 * - argv[2] = board width
 * - argv[3] = number of iterations
 * - argv[4] = refresh time in ms (optional)
 */
int main(int argc, char** argv)
{
  if (argc != 4 && argc != 5)
    return (1);

  t_board b;
  t_pen   p;
  
  // Pen initial values
  p.pos_x = 0;
  p.pos_y = 0;
  p.is_drawing = 0;

  // Board initial values
  b.height = atoi(argv[1]);
  b.width = atoi(argv[2]);
  b.board = board_new(b.width, b.height);
  
  // General parameters
  int iterations = atoi(argv[3]);

  if (b.height < 1 || b.width < 1 || iterations < 1)
	return (1);

  int refresh = 1000;
  if (argc == 5)
    refresh = atoi(argv[4]);

  // Generate first board
  board_init(&p, &b); 

  // Display first board
  board_erase(refresh);
  board_display(&b);
  board_erase(refresh);

  // Display all iterations
  for (int i = 0; i != iterations; ++i)
  {
    next_iteration(&b);
    board_display(&b);
    board_erase(refresh);
  }

  board_free(&b);
  return (0);
}
