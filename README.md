# 2048_game_C_terminal
An implementation of the 2048 game using ncurses library of the language C.

IF THE COLORS DON’T DISPLAY PROPERLY, GO TO 2048.c, save the file, and then reopen the executable without compiling it!

Next, I’ll detail the implementation of the game:

The entire game has a user interface, and I’ve implemented a color palette that’s well-matched.
The game contains an easter egg:
Easter egg: If the player scores 1000 points, a window with a cute kitten will appear :)

Important!
There are lines with more than 80 characters of code. Those lines contain the text art found in the interface.
Every multiple of 2 is implemented with text art.

Details per requirement:

Requirement 1:
The menu is made according to the requirement, and the “Resume” option can only be pressed after a game has been started.
Since it’s in an infinite loop, the program only continues if “New Game” or “Quit” is pressed.

Requirement 2:
The tiles are generated relative to the center of the window, using the displacement vectors: drow and dcol:

The drow and dcol vectors contain the coordinate variations relative to the center of the top-left corner of the respective square.
The vectors contain the corresponding variations of the elements in a logical order: from left to right, top to bottom.
Example: drow[0] and dcol[2] are the variations for the board[1][3] element.
Time is updated per second.
Requirement 3:
Movement in the four directions is done using the swipe UP function and clockwise rotation.
With these two functions, we can produce the other transformations we need.
Example: for right: rotate -> swipe -> rotate -> rotate -> rotate, equivalent to a hypothetical right swipe.
I use different colors for all multiples.

Requirement 4:
Implemented in the autoMove function, it looks for the most favorable move (UP, RIGHT, DOWN, LEFT) and applies it to the board.

Requirement 5:
The game displays "Try Again" or "Winner" if the player loses or wins, respectively.
Text Art
Easter egg: If the player scores 1000 points, a window with a cute kitten will appear :)

Explanation of functions and difficult aspects:

swipe_up and rotate work together to generate movements for each direction.
fill_block
Using the property of the drow and dcol vectors mentioned above, it fills the appropriate square with the correct number.
design_board
Uses two repetitive cells (strings) to generate the 16 squares.
kitten
Displays the Easter Egg.
new_game
Produces the game window, with a one-second timeout.
It waits for arrow inputs or calls autoMove if a number of seconds (3) pass.
hasResume is passed by reference between functions to determine when this option becomes active.
I spent around 25-30 hours on the implementation, especially for the interface.

Really enjoyable project!
