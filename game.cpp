#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;

#define Max_Length 1000

const char UP = 'U';
const char DOWN = 'D';
const char LEFT = 'L';
const char RIGHT = 'R';

int consoleWidth, consoleHeight;

// Methods from windows.h
void screen_init()
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hConsole, &csbi);
  consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;
  consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left;
}

struct point
{
  int xCoordinate, yCoordinate;
  point(){}
  point(int x, int y)
  {
    xCoordinate = x;
    yCoordinate = y;
  }
};

class Snake
{
  private:
    int length;
    char direction;
  public:
    point body[Max_Length]; 
    Snake(int x, int y)
    {
      length = 1;
      body[0] = point(x, y);
      direction = RIGHT;
    }
    int getLength()
    {
      return length;
    }
    void changeDirection(char newDirection)
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
      for(int i = length-1; i>0; i--)
      {
        body[i] = body[i-1];
      }
      switch (direction)
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
      for(int i=1; i<length; i++)
      {
        if(body[0].xCoordinate == body[i].xCoordinate && body[0].yCoordinate == body[i].yCoordinate)
        {
          return false;
        }
      }
      //eat and grow
      if(food.xCoordinate == body[0].xCoordinate && food.yCoordinate == body[0].yCoordinate)      
      {
        body[length] = point(body[length-1].xCoordinate, body[length-1].yCoordinate);
        length++;
      }
      return true;
    }
};

class Board
{
  private:
    Snake *snake;
    const char snakeBody = 'o';
    point food;
    int score;
    const char FOOD = 'o';
  public:
    Board()
    {
      spawnFood();
      snake = new Snake(2,2);
      score = 0;
    }
    ~Board()
    {
      delete snake;
    }
    int getScore()
    {
      return score;
    }
    void spawnFood()
    {
      int x = rand() % consoleWidth;
      int y = rand() % consoleHeight;
      food = point(x, y);
    }
    void gotoXY(int x, int y)
    {
      COORD coord;
      coord.X = x;
      coord.Y = y;
      SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    void draw()
    {
      system("cls");
      for(int i=0; i<snake->getLength(); i++)
      {
        gotoXY(snake->body[i].xCoordinate, snake->body[i].yCoordinate);
        cout<<snakeBody;
      }
      gotoXY(food.xCoordinate, food.yCoordinate);
      cout<<FOOD;
    }
    bool update()
    {
      bool valid = snake->move(food);
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
  
      void getInput()
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
    Sleep(100);
  }
  cout<<"YOU DIE!!!!!!";
  return 0;
}