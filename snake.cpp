//---------------------------------------------------------------------------

#include <iostream>
#include <ctime>
#include <unistd.h>
#define field 225

using namespace std;
//---------------------------------------------------------------------------


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}


struct point
{
  int x;
  int y;
};

void showGame(point snake[],int snake_length, point& meal )
{
 cout<<" ";
  for( int i =0; i<15; i++)
    cout<<"_";
  cout<<"\n";
  for (int i = 0; i<15; i++)
  {
  cout<<"|";
    for (int j = 0; j<15; j++)
    {
      bool isSnake = false;
      bool isMeal = false;
      if (meal.y==i && meal.x==j)
        isMeal = true;
      else
        for (int k = 0; k < snake_length; k++)
          if (snake[k].y==i && snake[k].x==j)
          {
            isSnake = true;
            break;
          }
      if(isSnake==true)
        cout<<"O";
      else
        if (isMeal == true) cout<< "X";
        else cout<<"_";

    }
    cout<<"|";
    cout<<"\n";
  }
}

void add(point snake[],int &snake_length, int x, int y)
{
  snake[snake_length].x=x;
  snake[snake_length].y=y;
  snake_length++;
}

bool gameover=false;

int move(point snake[], bool Dir[],int& snake_length, point& meal)
{
  int dx=0;
  int dy=0;

  if (Dir[0]==true)
    dy=-1;
  if (Dir[1]==true)
    dy=1;
  if (Dir[2]==true)
    dx=-1;
  if (Dir[3]==true)
    dx=1;

  int cur_x=snake[snake_length-1].x + dx;
  int cur_y=snake[snake_length-1].y + dy;

  if(cur_x>=15 || cur_x<0 || cur_y>=15 || cur_y<0)
    return 0;
  else
  {
    for (int i =0; i <snake_length; i++)
    {
      if (cur_x == snake[i].x && cur_y==snake[i].y)
        return 0;
      if (cur_x == meal.x && cur_y==meal.y)
      {
        meal.x=-1; meal.y=-1;
        add(snake,snake_length,cur_x,cur_y);
        return 1;
      }
    }
    for (int i =0; i<snake_length-1;i++)
      snake[i]=snake[i+1]; //подтягивание к голове
    snake[snake_length-1].x=cur_x;
    snake[snake_length-1].y=cur_y;
    return 1;
  }
}
point newMeal(point snake[], int snake_length)
{
  point nMeal;
  point freeSpace[225];
  int freeSpaceCounter=0;
  for(int i = 0; i < 15; i++)
  {
    for (int j=0; j<15; j++)
    {
      bool isFree = true;
      for (int k = 0; k < snake_length; k++)
      {
         if (snake[k].x==j && snake[k].y==i) { isFree = false; break; }
      }
      if (isFree)
      {
        freeSpace[freeSpaceCounter].x = j;
        freeSpace[freeSpaceCounter].y = i;
        freeSpaceCounter++;
        //cout<<"\n"<<freeSpaceCounter-1<<" :"<<freeSpace[freeSpaceCounter-1].x<<" "<<freeSpace[freeSpaceCounter-1].y;
        //getchar();
      }
    }
  }
  int randInd = rand()%(field-snake_length);
  nMeal = freeSpace[randInd];
  //cout<<"randIND"<<randInd<<"\n";
  //cout<<nMeal.x<<" "<<nMeal.y;
  //getchar();
  return nMeal;
}
#pragma argsused
int main(int argc, char* argv[])
{
  srand(time(NULL));
  //еда
  point meal;
  meal.x=-1; meal.y=-1;
  //координаты каждой точки тела змейки
  point snake[field];
  //изначальная длинна змейки
  int snake_length = 0;
  //заполнение тела змейки
  while(snake_length<4)
  {
    add(snake,snake_length,snake_length,0);
  }
  bool Dir[4];
  Dir[0] = false; // вверх
  Dir[1] = false; // вниз
  Dir[2] = false; // влево
  Dir[3] = true;  // вправо
  while(gameover==false)
  {
    //ОБНОВЛЕНИЕ ЭКРАНА
    system("clear");
    showGame(snake, snake_length,meal);
    usleep(200000);
    //ИЗМЕНЕНИЕ НАПРАВЛЕНИЯ
    if (kbhit())
    {
      int key = getchar();
      if(key==0)
      {
        key = getchar();
        if (key==75 && Dir[3]!=true)        // лево
        {
          Dir[0] = false;
          Dir[1] = false;
          Dir[2] = true;
          Dir[3] = false;
        }

        if (key==77 && Dir[2]!=true)        //право
        {
          Dir[0] = false;
          Dir[1] = false;
          Dir[2] = false;
          Dir[3] = true;
        }

        if (key==72 && Dir[1]!=true)        //вверх
        {
          Dir[0] = true;
          Dir[1] = false;
          Dir[2] = false;
          Dir[3] = false;
        }

        if (key==80 && Dir[0]!=true)        //вниз
        {
          Dir[0] = false;
          Dir[1] = true;
          Dir[2] = false;
          Dir[3] = false;
        }
      }
    }
    if(meal.x==-1)
    {
      meal = newMeal(snake,snake_length);
    }
    if(move(snake,Dir,snake_length,meal)==0)
    {
      cout<<"GAME OVER!\nYOUR SCORE: "<<snake_length-4;
      gameover=true;
    }
  }
  getchar();
  return 0;
}
//---------------------------------------------------------------------------
