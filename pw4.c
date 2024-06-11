/* ------------------------------CONNECT FOUR GAME -------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables for scores
int humanScore = 0;
int computerScore = 0;
// Function declarations
void board_Representation(char grid[6][7]) ;
void clear_Grid(char grid[6][7]) ;
void display_Grid(char grid[6][7]) ;
int move_is_possible(char grid[6][7], int column_choice) ;
void drop_disc(char grid[6][7], int column_choice, char player) ;
char play_turn(int turn);
int longest_alignment(char grid[6][7], char player) ;
int random_move(char grid[6][7]);
int Smart_Move(char grid[6][7],char player) ;
void Comp_Play(char grid[6][7]) ;
int Human_Play(char grid[6][7]) ;
int Game_Over(char grid[6][7], char player) ;
void Play_Game(int turn, char grid[6][7]) ;
int full_board(char grid[6][7]);

int main() 
{
    printf("\033[1mWelcome to Connect 4!\033[0m\n");
    printf("-----------------------\n");
    char grid[6][7];
    int turn = 0;
    char reply;
    do 
    {
        board_Representation(grid);//displays board configuration
        Play_Game(turn++, grid); //starts the game
        printf("\033[31m\nScores -  Human: %d      Computer: %d\n\n\033[0m", humanScore, computerScore); // Display scores
        printf("Would you like to play again? (y for yes, n for no): "); // Asks user whether to end or continue playing.
        scanf(" %c", &reply);
    } while (reply == 'y' || reply == 'Y'); // If yes, game starts again
    return 0;
}
/* Prints the Connect 4 board. */
void board_Representation(char grid[6][7]) 
{
    printf("  1 2 3 4 5 6 7 \n");
    for (int i = 0; i<6; i++)
    {
        printf("%d",6 - i);
        for (int j = 0; j < 7; j++) 
        {
            grid[i][j] = ' ';
            printf("  ");
        }
        printf(" %d",6 - i);
        printf("\n");
    }
    printf("  1 2 3 4 5 6 7 \n");
}
/*Clears the board by replacing all elements with empty character*/
void clear_Grid(char grid[6][7]) 
{
    for (int i = 0; i<6; i++) 
    {
        for (int j = 0; j < 7; j++) 
        {
            grid[i][j] = ' ';
        }
    }
}
/*Displays the board - loops through all cells of grid and prints each disc in its color */
void display_Grid(char grid[6][7]) 
{
    printf("\n  1 2 3 4 5 6 7 \n");
    for (int k = 0;k<6;k++)
    {    
        printf("%d ",6-k);
        for (int j = 0; j < 7; j++) 
        {
           if (grid[k][j] == '*') 
            {
                printf("\033[31m* \033[0m");  
            }
            else if (grid[k][j] == '0')
            {
                printf("\033[33m0 \033[0m");
            }
            else
            {
                printf("%c ", grid[k][j]);//if there is no disc in the cell
            }           
        }
        printf("%d",6-k);
        printf("\n");
    }              
    printf("  1 2 3 4 5 6 7 \n");
    printf("\n");
}
/*Checks whether a move is possible or not. Returns row if it's possible, -1 otherwise.*/
int move_is_possible(char grid[6][7], int column_choice) 
{
    int col = column_choice - 1;
    if (grid[5][col] == ' ')
        return 5;
    else 
    {
        for (int row = 4; row >= 0; row--) 
        {
            if (grid[row][col] == ' ' && grid[row + 1][col]!= ' ') 
            {
                return row;
            }
        }
    }
    return -1;
}
/* drop_disc function - Takes the value returned by move_is_possible as input and if player  can make that move then makes it.
 If move is not possible(-1 returned by move_is_possible), then the program considers whether this is human or computer, 
 if this is computer then it finds another value .
 If this is human,then program asks player to enter another column number until player enters valid(possible one which is 
 checked by move_is_possible) one. 
 After obtaining valid column number and returned row number, the program places the '0' or '*' (depending on player)
in that position on the board and calls function for displaying board */
void drop_disc(char grid[6][7], int column_choice, char player) 
{
    int possible_move_row = move_is_possible(grid, column_choice);
    while (possible_move_row == -1) 
    {
        if (player == '0')
        {
            printf("Move is not possible. Try another one: ");
            scanf("%d", &column_choice);
            while (column_choice < 1 || column_choice > 7) 
            {
                printf("Invalid input. Column number must be between 1 and 7 (inclusive). Try again: ");
                scanf("%d", &column_choice);
            }
        }
        if (player == '*')
        {
            column_choice = random_move(grid); 
        }
        possible_move_row = move_is_possible(grid, column_choice);
    }
    if (possible_move_row!= -1) 
    {
        if (player == '*') // comp
        {
            grid[possible_move_row][column_choice - 1] = '*';
        }
        if (player == '0') // user
        {
            grid[possible_move_row][column_choice - 1] = '0';
        }
    }
    display_Grid(grid);
}
/*Smart_Move- generates smart moves for players(both comp and human (as an advice)).Takes also player as an input.
Firstly,checks for winning moves. If a winning move(which will lead to victory) exists, then returns the column 
number of that position.
If winning move doesnt exist, then checks moves for preventing the opponent from winning
If none of these happens, then chooses column number randomly (according to protocol)*/
int Smart_Move(char grid[6][7], char player) 
{
    char opponent = (player == '0') ? '*' : '0';
    // Check for winning moves
    for (int col = 1; col <= 7; col++) 
    {
        char grid_copy[6][7];//create new grid as a copy of real one
        for (int i = 0; i < 6; i++) 
        {
            for (int j = 0; j < 7; j++) 
            {
                grid_copy[i][j] = grid[i][j];
            }
        }
        int result = move_is_possible(grid_copy, col);
        if (result != -1) 
        {
            grid_copy[result][col - 1] = player; // Simulate placing a disc
            if (longest_alignment(grid_copy, player) >= 4) 
            {
                return col; // Winning move found, return the column
            }
        }
    }
    // Check for blocking moves for the opponent
    for (int col = 1; col <= 7; col++) 
    {
        char grid_copy[6][7];
        for (int i = 0; i < 6; i++) 
        {
            for (int j = 0; j < 7; j++) 
            {
                grid_copy[i][j] = grid[i][j];
            }
        }
        int result = move_is_possible(grid_copy, col);
        if (result != -1) 
        {
            grid_copy[result][col - 1] = opponent; // Simulate opponent's move
            if (longest_alignment(grid_copy, opponent) >= 4) 
            {
                return col; // Blocking move found, return the column
            }
        }
    }
    // If no winning or blocking moves, return a random move
    return random_move(grid);
}
/* Manages turns in the game- if in the first game human starts, then second game starts with comp move... */
char play_turn(int turn)
{
    if (turn % 2== 0) 
    {
        return '*'; // red(comp)
    } 
    else 
    {
        return'0'; // yellow(human)
    }
}
/*Returns the length of longest alignment checking in all directions*/
int longest_alignment(char grid[6][7], char player) 
{
    int max_count = 0;
    // Check vertically
    for (int col = 0; col < 7; col++) 
    {
        for (int row = 0; row < 6; row++) 
        {
            if (grid[row][col] == player) 
            {
                int count = 1;
                for (int k = row + 1; k < 6 && grid[k][col] == player; k++) 
                {
                    count++;
                }
                if (count > max_count)
                    max_count = count;
            }
        }
    }  
    //check rows(horizontally)
    for (int row = 0; row<6;row++)
    {
        for (int col = 0;col<7;col++)
        {
            if (grid[row][col] == player)
            {
                int count = 1;
                for (int k = col+1; k<7 && grid[row][k]==player;k++)
                {
                    count++;
                }
                if (max_count < count)
                {
                    max_count = count;
                }
            }
        }
    }
    //check diagonals(from left bottom to right top)
    for (int row = 0; row<6;row++)
    {
        for (int col = 0; col<7;col++)
        {
            if (grid[row][col] == player)
            {
                int count = 1;
                int m = row - 1;
                int n = col +1;
                while (m >= 0 && n < 7 && grid[m][n] == player) 
                {
                    count++;
                    m--;
                    n++;
                }
                if (max_count<count)
                {
                    max_count = count;
                }
            }
        }
    }
    //check diagonals(from right top to left bottom)

    for (int row = 0; row<6;row++)
    {
        for (int col = 0; col<7;col++)
        {
            if (grid[row][col] == player)
            {
                int count = 1;
                int m = row+1;
                int n = col +1;
                while (n<7 && m<6 && grid[m][n] == player)
                {
                    count++;
                    m++;
                    n++;
                }
                if (max_count<count)
                {
                    max_count = count;
                }
            }
        }
    }
    return max_count;
}
/*random_move generates random move for comp play(if no no winning or blocking move exists)*/
int random_move(char grid[6][7])
{
    // Seed the random number generator with the current time
    srand(time(NULL));
    // Generate a random number between  1 to 7
    int random_number = rand() % 7 + 1;
    while (move_is_possible(grid,random_number)==-1)
    {
        random_number = rand() % 7 + 1;
    }
    return random_number;
}
/*Comp_Play manages computer play*/
void Comp_Play(char grid[6][7]) 
{
    char player = '*';
    int comp_move = Smart_Move(grid,'*');
    drop_disc(grid, comp_move, player);
}
/*Manages Human play-asks player to enter column number, checks its validity and calls function for dropping disc(0) into the grid.*/
int Human_Play(char grid[6][7]) 
{
    char player = '0';
    int column_choice;
    printf("\nEnter column number to place your disc: ");
    scanf("%d", &column_choice);
    while (column_choice < 1 || column_choice > 7) 
    {
        printf("Invalid input. Column number must be between 1 and 7 (inclusive). Try again: ");
        scanf("%d", &column_choice);
    }
    drop_disc(grid, column_choice, player);
}

/*Checks whether board is completely full (with '0' and '*')- by only checking the row
 on the above(row=0). Related to game over, because if longest alignment is not 4 and 
board is full, then game automatically overs */
int full_board(char grid[6][7])
{
    int row = 0;
    for (int i = 0; i<7;i++)
    {
        if (grid[row][i] == ' ')
        {
            return 0;//Board is not full
        }
    }
    return 1;//Board is full.  
}
/*Checks the length of longest alignment ,if >= 4 , declares winner. Consider full board case */
int Game_Over(char grid[6][7], char player) 
{
    int alignment = longest_alignment(grid, player);
    if (alignment >= 4) 
    {      
        printf("\033[1mGame Over!\033[0m\n\n"); // Declares end of the game
        if (player == '*')
        {
            printf("\033[34mYou lost! Better luck next time.\033[0m\n");
            computerScore++; // Increment computer's score

        }
        else{
            printf("\033[34mYou won!\033[0m\n");
            humanScore++; // Increment human player's score

        }
        return 1;
    }
    else if (full_board(grid)==1)
    {
        printf("\033[34mThe game ends in a draw. No winner this time.\033[0m\n");
        return 1;
    }
    return 0;
}
/* Manages game - Controls turns between players. */
void Play_Game(int turn, char grid[6][7]) 
{
    clear_Grid(grid);
    turn++;
    char player = play_turn(turn);
    while (1) 
    {
        if (player == '*') 
        {
            printf("Computer's Move:\n");
            Comp_Play(grid);
            if (Game_Over(grid,player) != 1)
            {
            player = '0';
            }
            else{break;}
        } 
        else 
        {
            printf("\nPlayer's Move:\n");
            printf("Recommended column: %d\n",Smart_Move(grid,'0'));//advices smart move(column) to player 
            Human_Play(grid);
            if (Game_Over(grid,player) != 1)
            {
            player = '*';
            }
            else{break;}
        }
    }    
}
