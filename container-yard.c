/* The following is the code for a game I made during my first year of my engineering degree,
in the game you enter in the letter for the container you want to move until one has successfully reached the entry or exit*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

/*
The function InitialiseFloor initialises a floor grid. The nested for loops initialise all elements within the grid to 0, then change all elements on the outer border of the grid to -1.
These for loops are then followed by a series of if statements that check the value of the entryboundary character for which sides to place the entry and exit values, then places them in positions determined by the index.
*/
void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
  //Initialising the floor grid and assigning boundary values
	int i, j;
  for (i = 0; i < NUM_ROWS; i++) {
    for (j = 0; j < NUM_COLS; j++) {
      floor[i][j] = VACANT;
      floor[0][j] = BOUNDARY;
      floor[NUM_ROWS - 1][j] = BOUNDARY;
    }
  floor[i][0] = BOUNDARY;
  floor[i][NUM_COLS - 1] = BOUNDARY;
  }

  //Adding entry and exit values
  if (entryboundary == 84) {
    floor[0][index] = ENTRY;
    floor[NUM_ROWS - 1][index] = EXIT;
  } else if (entryboundary == 66) {
    floor[NUM_ROWS - 1][index] = ENTRY;
    floor[0][index] = EXIT;
  } else if (entryboundary == 76) {
    floor[index][0] = ENTRY;
    floor[index][NUM_COLS - 1] = EXIT;
  } else if (entryboundary == 82) {
    floor[index][NUM_COLS -1] = ENTRY;
    floor[index][0] = EXIT;
  }
}

/*
The PrintFloor function prints a visual representation of the floor grid. It prints each element in a position corresponding to its position in the 2D array. When the element is printed it is represented by a character corresponding to its value. An empty space for a VACANT value, an @ character for a BOUNDARY value, a U character for an ENTRY value, and a X character for an EXIT value. For any other value, the corresponding ASCII character is printed.
*/
void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
  int i,j;
  for (i = 0; i < NUM_ROWS; i++) {
    for (j = 0; j < NUM_COLS; j++) {
      if (floor[i][j] == VACANT) {
        printf(" ");
      } else if (floor[i][j] == BOUNDARY) {
        printf("@");
      } else if (floor[i][j] == ENTRY) {
        printf("U");
      } else if (floor[i][j] == EXIT) {
        printf("X");
      } else {
        char container = (char)floor[i][j];
        printf("%c", container);
      }
    }
    printf("\n");
  }
}

/*
The function FloorAreaAvailable returns the magnitude of the unused area in the floor grid,
The number of free cells and the area of each grid cell are calculated then multiplied together to obtain this value.
*/
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width)
{
	double free = 0;
  //calculate how many free cells there are
  int i,j,n = 0;
  for (i = 0; i < NUM_ROWS; i++) {
    for (j = 0; j < NUM_COLS; j++) {
      if (floor[i][j] == VACANT) {
        n++;
      }
    }
  }

	//calculate the area of each grid cell
	double cellArea;
  cellArea = length * width;

  //calculate the total free area
  free = n * cellArea;

	return free;
}

/*
The AddContainer function inserts a container onto the floor grid at a specified position and orientation,
the function replaces the value at each cell occupied by the container with a ASCII value corresponding to the container.
Each time the function is run this ASCII value changes.
*/
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction)
{
  //Finding the coordinates of the starting position
  int x,y;
  x = position / NUM_COLS;
  y = position % NUM_COLS;

  //Determining the letter to represent this container
  int a,b;
  int container = 65;
  for (a = 0; a < NUM_ROWS; a++) {
    for (b = 0; b < NUM_COLS; b++) {
      if (container <= floor[a][b]) {
        container = floor[a][b] + 1;
      }
    }
  }

  //Inserting the container onto the floor grid
  int i,j;
	if (direction == 1) {
    for (i = x; i < (x + size); i++) {
      //Checking for obstructions
      if (floor[i][y] != 0) {
        return;
      }
    }
    for (i = x; i < (x + size); i++) {
      floor[i][y] = container;
    }
  } else if (direction == 0) {
    for (j = y; j < (y + size); j++) {
      //Checking for obstructions
      if (floor[x][j] != 0) {
        return;
      }
    }
    for (j = y; j < (y + size); j++) {
      floor[x][j] = container;
    }
  }

	return;
}

/*
The LocateContainer function determines the first and last positions of a specified container,
it also returns a value representing whether or not the container can be moved in any direction.
*/
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int* rowStart, int* colStart, int* rowEnd, int* colEnd)
{
	int i, j;
  int moveable = 0;

  //Finding the left-most or upper-most position of the container
  for (i = 0; i < NUM_ROWS; i++) {
    for (j = 0; j < NUM_COLS; j++) {
      if (floor[i][j] == move) {
        *rowStart = i;
        *colStart = j;
        break;
      }
    }
    if (floor[i][j] == move) {
      break;
    }
  }

  //Finding the right-most or lower-most position of the container
  int orientation;
  i = *rowStart;
  j = *colStart;
  if (floor[*rowStart + 1][*colStart] == move) {
    //The container is vertical
    *colEnd = j;
    orientation = 1;
    while (floor[i][j] == move) {
      *rowEnd = i;
      i++;
    }
  } else {
    //The container is horizontal
    *rowEnd = i;
    orientation = 0;
    while (floor[i][j] == move) {
      *colEnd = j;
      j++;
    }
  }

  //Determining if the container is stuck
  if (orientation == 1 && (floor[*rowEnd + 1][*colEnd] == 0 || floor[*rowStart - 1][*colStart] == 0)) {
    moveable = 1;
  } else if (orientation == 0 && (floor[*rowEnd][*colEnd + 1] == 0 || floor[*rowStart][*colStart - 1] == 0)) {
    moveable = 1;
  }
	
	return moveable;
}

/*
The MoveContainer function moves a specified container as far as it can in any direction with leftwards preferred
for horizontal containers and upwards preferred for vertical containers. It also returns a value to indicate whether
the container has been moved to an entry, exit, or is unable to move.
*/
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked)
{
  //Determining the positions of the entry and exit locations
  int a,b,entryRow = 0,entryCol = 0,exitRow = 0,exitCol = 0;
  for (a = 0; a < NUM_ROWS; a++) {
    for (b = 0; b < NUM_COLS; b++) {
      if (floor[a][b] == ENTRY) {
        entryRow = a;
        entryCol = b;
      } else if (floor[a][b] == EXIT) {
        exitRow = a;
        exitCol = b;
      }
    }
  }

  //Checking if movement is blocked
  if (isBlocked == 0) {
    if ((exitRow == r0 && (exitCol == c0 - 1 || exitCol == c1 + 1))
    || (exitCol == c0 && (exitRow == r0 - 1 || exitRow == r1 + 1))) {
      return 2;
    } else if ((entryRow == r0 && (entryCol == c0 - 1 || entryCol == c1 + 1))
    || (entryCol == c0 && (entryRow == r0 - 1 || entryRow == r1 + 1))) {
      return 1;
    } else {
      return -1;
    }
  }

  //Determining the orientation of the container and the character that represents it
	int container = floor[r0][c0];
  int orientation = 0;
  if (c0 == c1) {
    // The container is vertical
    orientation = 1;
  }

  //Moving a vertical container
  //Checking what direction it can move in 
  int direction = 0;
  if (floor[r0 - 1][c0] != 0) {
    direction = 1;
  }
  if (orientation == 1 && direction == 0) {
    int i = r0 - 1;
    int n = 0;
    while (floor[i][c0] == 0) {
      i--;
      n++;
    }
    int x;
    for (x = r0 - 1; x > i; x--) {
      floor[x][c0] = container;
    }
    for (x = r1; x > r1 - n; x--) {
      floor[x][c0] = VACANT;
    }
    if (floor[entryRow + 1][entryCol] == container) {
      return 1;
    } else if (floor[exitRow + 1][exitCol] == container) {
      return 2;
    }
  } else if (orientation == 1 && direction == 1) {
    int i = r1 + 1;
    int n = 0;
    while (floor[i][c0] == 0) {
      i++;
      n++;
    }
    int x;
    for (x = r1 + 1; x < i; x++) {
      floor[x][c0] = container;
    }
    for (x = r0; x < r0 + n; x++) {
      floor[x][c0] = VACANT;
    }
    if (floor[entryRow - 1][entryCol] == container) {
      return 1;
    } else if (floor[exitRow - 1][exitCol] == container) {
      return 2;
    }
  }
  //Moving a horizontal container
  //Checking what direction it can move in 
  direction = 0;
  if (floor[r0][c0 - 1] != 0) {
    direction = 1;
  }
  if (orientation == 0 && direction == 0) {
    int j = c0 - 1;
    int n = 0;
    while (floor[r0][j] == 0) {
      j--;
      n++;
    }
    int y;
    for (y = c0 - 1; y > j; y--) {
      floor[r0][y] = container;
    }
    for (y = c1; y > c1 - n; y--) {
      floor[r0][y] = VACANT;
    }
    if (floor[entryRow][entryCol + 1] == container) {
      return 1;
    } else if (floor[exitRow][exitCol + 1] == container) {
      return 2;
    }
  } else if (orientation == 0 && direction == 1) {
    int j = c1 + 1;
    int n = 0;
    while (floor[r0][j] == 0) {
      j++;
      n++;
    }
    int y;
    for (y = c1 + 1; y < j; y++) {
      floor[r0][y] = container;
    }
    for (y = c0; y < c0 + n; y++) {
      floor[r0][y] = VACANT;
    }
    if (floor[entryRow][entryCol - 1] == container) {
      return 1;
    } else if (floor[exitRow][exitCol - 1] == container) {
      return 2;
    }
  }

	return 0;
}

/* Function to obtain capital letter as input */
char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

/* The main Container Yard simulation */
int main(void)
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");
	
	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 28, 2, 0);
	AddContainer(floor, 11, 3, 1);
	AddContainer(floor, 41, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 34, 2, 0);
	AddContainer(floor, 36, 3, 1);
	AddContainer(floor, 37, 2, 1);
	AddContainer(floor, 53, 2, 0);
	AddContainer(floor, 30, 3, 1);

	/* Print status */
	printf("%.2f sq ft of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");

	return 0;
}