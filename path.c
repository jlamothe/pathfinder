/**
   @file path.c
   @author Jonathan Lamothe
 */

/*

Pathfinder 1.1

Copyright (C) 2010
Jonathan Lamothe <jonathan@jlamothe.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <stdio.h>
#include <stdlib.h>

#define COLS 10			/**< The number of columns. */
#define ROWS 10			/**< The number of rows. */
#define START_X 0		/**< The starting x position. */
#define START_Y 0		/**< The starting y position. */

/**
   @brief A set of coordinates.
 */
struct Coords
{
  int x,			/**< The x coordinate.  */
    y;				/**< The y coordinate. */
};

/**
   @brief The default set of legal moves.
 */
struct Coords delta[] = {
  { 1, 2 },
  { 2, 1 },
  { -1, 2 },
  { -2, 1 },
  { 1, -2 },
  { 2, -1 },
  { -1, -2 },
  { -2, -1 }
};

#define DELTA_SIZE 8		/**< The number of legal moves. */

/**
   @brief An iteration of the pathfinder algorithm.

   @param table A pointer to the table of values.

   @param table_size Indicates the size of the table.

   @param delta A pointer to the list of legal moves.

   @param delta_size The number of legal moves.

   @param pos The current position on the table.

   @param moves The number of moves successfully completed.

   @param progress The estimated percentage of possible permutations
   that have been exhausted.

   @param last_prog A pointer to the last progress value reported.

   @return 1 if a path was found; 0 otherwise.
 */
int path_iter(int **table,
	      struct Coords table_size,
	      const struct Coords *delta,
	      int delta_size,
	      struct Coords pos,
	      int moves,
	      float progress,
	      int *last_prog)
{

  /* Declare local variables: */
  struct Coords new_pos;
  float increment;
  int i;

  /* Check to see if enough moves were successfully completed: */
  if(moves >= table_size.x * table_size.y)
    return 1;			/* Yes.  Hooray! */

  /* Calculate how much progress each sub-move represents: */
  increment = 1.0 / delta_size;
  for(i = 0; i < moves; i++)
    increment /= delta_size;

  /* If there has been more than a 1% progress increase, report it: */
  if(progress * 100 >= *last_prog + 1)
    {
      *last_prog = progress * 100;
      printf("%3d%% completed.\n", *last_prog);
    }

  /* Check to see if the current position is inside the table: */
  if(pos.x < 0 || pos.x >= table_size.x || pos.y < 0 ||
     pos.y >= table_size.y)
    return 0;			/* No, it's not. */

  /* Check to see if we've been on this space before: */
  if(table[pos.y][pos.x])
    return 0;			/* Yes, we have. */

  /* Increase the number of successful moves and mark this space as
     used: */
  moves++;
  table[pos.y][pos.x] = moves;

  /* Check for a successful path along each legal move: */
  for(i = 0; i < delta_size; i++)
    {
      new_pos.x = pos.x + delta[i].x;
      new_pos.y = pos.y + delta[i].y;
      if(path_iter(table,
		   table_size,
		   delta,
		   delta_size,
		   new_pos,
		   moves,
		   progress + i * increment,
		   last_prog))
	return 1;		/* Hooray!  We found a path. */
    }

  /* We haven't found any paths from this point forward; back up a
     step and try again: */
  table[pos.y][pos.x] = 0;
  return 0;

}

/**
   @brief Finds a path.

   @param table A pointer to the table of values.

   @param table_size Indicates the size of the table.

   @param delta A pointer to the list of legal moves.

   @param delta_size The number of legal moves.

   @param pos The initial position on the table.

   @return 1 if a path was found; 0 otherwise.
 */
int path_find(int **table,
	      struct Coords table_size,
	      const struct Coords *delta,
	      int delta_size,
	      struct Coords pos)
{

  /* Declare local variables: */
  int i, j, progress = 0;

  /* Fill the table with zeros: */
  for(i = 0; i < table_size.y; i++)
    for(j = 0; j < table_size.x; j++)
      table[i][j] = 0;

  /* Start the first move: */
  return path_iter(table,
		   table_size,
		   delta,
		   delta_size,
		   pos,
		   0,
		   0,
		   &progress);
}

int main()
{

  /* Declare local variables and allocate memory for the table: */
  int **table;
  struct Coords size, pos;
  int i, j;

  /* Allocate memory for the table. */
  table = calloc(ROWS, sizeof(int *));
  for(i = 0; i < ROWS; i++)
    {
      table[i] = calloc(COLS, sizeof(int));
    }

  /* Set the initial position: */
  size.x = COLS;
  size.y = ROWS;

  /* Set the table size: */
  pos.x = START_X;
  pos.y = START_Y;

  /* Check to see if we can find a path: */
  if(!path_find(table, size, delta, DELTA_SIZE, pos))
    {
      printf("No path found.\n"); /* No, we haven't. */
      return 0;
    }

  /* Yes we have; display the table: */
  for(i = 0; i < ROWS; i++)
    {
      for(j = 0; j < COLS; j++)
	printf("%5d", table[i][j]);
      putchar('\n');
    }

  /* Free the space allocated for the table (not necessary in this
     case, but a good habit to get into): */
  for(i = 0; i < ROWS; i++)
    free(table[i]);
  free(table);

  /* Exit normally: */
  return 0;

}

/* jl */
