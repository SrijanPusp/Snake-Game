#include "snake_game.h"

int left_limit;
int right_limit;
int top_limit;
int bottom_limit;
int consoleWidth, consoleHeight;

// Methods from windows.h
void screen_init()
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;
  consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left;
  left_limit = csbi.srWindow.Left;
  right_limit = csbi.srWindow.Right;
  top_limit = csbi.srWindow.Top;
  bottom_limit = csbi.srWindow.Bottom;
}
//a data type to define position of stuff
struct point
{
  int xCoordinate, yCoordinate;
  point(){} //default constructor
  point(int x, int y) //parameterised constructor
  {
    xCoordinate = x;
    yCoordinate = y;
  }
};

// a class to define the properties of snake such as length, position, direction etc
class Snake
{
  private:
    int length;
    char direction;
  public:
    point body[Max_Length];  //array of type points (coordinates)
    Snake(int x, int y)   //constructor that creates snake of length 1 and sets it in motion 
    {
      length = 1;  
      body[0] = point(x, y);
      direction = RIGHT;
    }
    int getLength()
    {
      return length;  //this one will be used later
    }
    void changeDirection(char newDirection)  //this one changes the direction of snake as per input
    {
      if(newDirection == UP && direction != DOWN)
      {
        direction = newDirection;
      }
      else if(newDirection == RIGHT && direction != LEFT)
      {
        direction = newDirection;
      }
      else if(newDirection == DOWN && direction != UP)
      {
        direction = newDirection;
      }
      else if(newDirection == LEFT && direction != RIGHT)
      {
        direction = newDirection;
      }
    }
    bool move(point food)  
    {
      for(int i = length-1; i>0; i--) //this one makes the snake move by constantly changing the position of every 'o' in snake
      {
        body[i] = body[i-1];
      }
      switch (direction)    // switch case, to figure out movement by taking input from keyboard 
      {
        int val;
        case UP:
          val = body[0].yCoordinate;
          body[0].yCoordinate = val-1;
          break;
        case DOWN:
          val = body[0].yCoordinate;
          body[0].yCoordinate = val+1;
          break;
        case RIGHT:
          val = body[0].xCoordinate;
          body[0].xCoordinate = val+1;
          break;
        case LEFT:
          val = body[0].xCoordinate;
          body[0].xCoordinate = val-1;
          break;
      }
      //game over
      for(int i=1; i<length; i++)  //if snake bites itself
      {
        // First check boundary collision
        if (body[0].xCoordinate < left_limit || body[0].xCoordinate >= right_limit ||
          body[0].yCoordinate < top_limit || body[0].yCoordinate >= bottom_limit) 
        {
          return false;
        }

        // Then check self collision
        for(int i=1; i<length; i++)
        {
        if(body[0].xCoordinate == body[i].xCoordinate && body[0].yCoordinate == body[i].yCoordinate)
        {
          return false;
        }
      }
      }
      //eat and grow
      if(food.xCoordinate == body[0].xCoordinate && food.yCoordinate == body[0].yCoordinate)   //if eats food, it grows   
      {
        body[length] = point(body[length-1].xCoordinate, body[length-1].yCoordinate);
        length++;
      }
      return true;
    }
};

class Board  //contains, controles and displays all elements such as snake, food, etc
{
  private:
    Snake *snake;  //dynamically creates snake
    const char snakeBody = 'o';  //so the snake goes ooooooooo
    point food;  //creating food of point type so it has position on board
    int score;  //hmmmm
    const char FOOD = '+';   //so food is visible
  public:
    Board()   //constructor
    {
      spawnFood();  //random food spawn goooooo
      snake = new Snake(2,2);    //as the board gets created, we need a new snake at 2,2 coordinates and 
      score = 0; //new score 00
    }
    ~Board()
    {
      delete snake;  //since snake was created in heap memory
    }
    int getScore()
    {
      return score;   //will cone to use later
    }
    void printScore()
    {
      gotoXY(consoleWidth/2, 0);
      cout<<"score: "<< getScore();

    }
    void spawnFood()  // this makes sure food spawns randomly
    {
      int x = rand() % consoleWidth;
      int y = rand() % consoleHeight;
      food = point(x, y);
    }
    void gotoXY(int x, int y)  //sets the cursor
    {
      COORD coord;
      coord.X = x;
      coord.Y = y;
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    void draw()  //whatever you see on screen, is being done by this huehue
    {
      system("cls");  //clers the screen everytime
      for(int i=0; i<snake->getLength(); i++)   //prints the snake 
      {
        gotoXY(snake->body[i].xCoordinate, snake->body[i].yCoordinate);  //so basically, this moves the cursor, which prints snake at every x,y
        cout<<snakeBody;
      }
      gotoXY(food.xCoordinate, food.yCoordinate);    //sends cursor where food is
      cout<<FOOD;
    }
    bool update()    //keeps checking if you alive and maintains score and stuff
    {
      bool valid = snake->move(food);  //passes on the move function with food, which checks if we need to increase score or die
        if(valid == false)
        {
          return false;
        }
        if(food.xCoordinate == snake->body[0].xCoordinate && food.yCoordinate == snake->body[0].yCoordinate)      
        {
          score++;
          spawnFood();
        }

        return true;
    }
  
      void getInput()  //sets up for wasd key work
      {
        if(kbhit())
        {
            int key = getch();
            if(key == 'w' || key == 'W')
            {
                snake->changeDirection(UP);
            }
            else if(key == 'a' || key == 'A')
            {
                snake->changeDirection(LEFT);
            }
            else if(key == 's' || key == 'S')
            {
                snake->changeDirection(DOWN);
            }
            else if(key == 'd' || key == 'D')
            {
                snake->changeDirection(RIGHT);
            }
        }
      }
};

int main()
{
  srand(time(0));
  screen_init();
  Board *board = new Board();
  while(board->update())
  {
    board->getInput();
    board->draw();
    board->printScore();
    Sleep(100);
  }
  cout<<"YOU DIE!!!!!!"<<endl;
  cout<<"score: "<<board->getScore();
  return 0;
}