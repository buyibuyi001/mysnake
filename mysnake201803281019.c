#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define MapLength 20
#define MapHeight 20
#define FreshTime 100
#define Up 72
#define Down 80
#define Left 75
#define Right 77
// return value from keyboard
//typedef struct {int x, y; } COO;    // from main() to write to avoid restruct the data type
typedef struct node {int x, y; char drct; struct node *pre,*next; } NODE;
typedef struct {int x, y, counter; } FOOD;  // for structure language, use structure embed structurure,for c ,no need for cannot compare direct
typedef struct {int gameover, score; }STATE;   //use short name as type name

void InitBackground(int length, int height);
void DrawSnake(NODE *psnakehead);
void DrawSnakeHead( NODE *psnakehead);
char GetKeyBoard(char drct);
NODE *GetSnakeHead(NODE *psnakehead, NODE * tpsnakehead, char drct);
FOOD ProduceFood(FOOD food);
STATE HitWallOrBody( NODE *psnakehead, FOOD food,  STATE gamestate);
int FoodEatOrNot(NODE *psnakehead, FOOD food);
NODE *UpdateSnakeTail( NODE *psnaketail, FOOD food);

int main()
{
    int maplength= MapLength, mapheight= MapHeight;
    char drct= 0;
    FOOD food= { -1, -1, 0};
    STATE gamestate= {0, 0};

    NODE snakehead= {  maplength/2,     mapheight/2,  Left, NULL, NULL};
    NODE *psnakehead= &snakehead;
    NODE snaketail= {  maplength/2+2,   mapheight/2,  0,    NULL, NULL}; //fr col one node occupy two position
    NODE *psnaketail= &snaketail;
    psnakehead->next= psnaketail;
    psnaketail->pre=  psnakehead;

    //my style, first write a big func,if part useful, apart it to another func
    InitBackground(maplength, mapheight);
    food= ProduceFood( food);
    DrawSnake( psnakehead);

    while ( !gamestate.gameover)
    {
        drct= GetKeyBoard( drct);
        while (!_kbhit())
        {
        NODE *tpsnakehead = (NODE *)malloc(sizeof(NODE));  // address cannot be lower such a numgroup adress 
        psnakehead= GetSnakeHead( psnakehead,tpsnakehead, drct);
        gamestate= HitWallOrBody( psnakehead, food, gamestate);
        if (gamestate.gameover) break;
        DrawSnakeHead( psnakehead);
        food.counter= FoodEatOrNot( psnakehead, food);
        psnaketail= UpdateSnakeTail( psnaketail, food); //better to transfer pointer when structure is big

        // first divide then converge
        food= ProduceFood( food);
        Sleep( FreshTime);   
        }
    }
    EndGame(maplength, mapheight);
    getchar();
    return 0;
}

void GotoXY(int x, int y)
{
  COORD coord={x,y};
  SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE),coord );
}
void InitBackground(int maplength, int mapheight)
{
  CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };   // hide cursor
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
  //system("mode con:cols=43 lines=20");  //set console long 43, high 20
  system("cls");
  srand((unsigned)time(NULL));

  for (int i=0;i<mapheight;i++)
    for (int j=0;j<2*maplength; j++)
    {
      if (i==0 || i==mapheight-1 )
      {
        printf("¡ö");
        if (j==maplength-1) {printf("\n");break;}
      }
      else
      {
        if ( j==0) printf("¡ö");
        else if ( j==2*maplength-3) {printf("¡ö\n");break;}
        else printf(" ");
      }
    }
}
void DrawSnake(NODE *psnakehead)
{
  NODE *cur= psnakehead;
  while (cur!= NULL)
  {
    GotoXY(cur->x,cur->y);
    printf("¡ñ");   // note use ascii code ,or
    cur= cur->next;

  }
}
void DrawSnakeHead( NODE *psnakehead)
{
   GotoXY(psnakehead->x,psnakehead->y);
   printf("¡ñ");
}

char GetKeyBoard(char drct)
{
    // up down left right input 2 bytes into stdout but getch() only get one once
    int counter =0;
    char newdrct=0;
    /*while (newdrct!=72 && newdrct!=75 &&newdrct!=77 && newdrct!=80)
    {
      if (  _kbhit() &&  (getch()== -32) )
          newdrct=getch();  // if drct is define int, -32 will be convert into a plus number
      if (++counter==1000)
          break;
    }
    if (counter==1000)  newdrct=drct;*/
    //getch() get one charter and a back charter

    while ( (newdrct=_getch()) != -32 && newdrct != 80 && newdrct != 72 && newdrct != 77 && newdrct != 75);
    newdrct = _getch();

    return newdrct;
}
NODE  *GetSnakeHead( NODE *psnakehead, NODE *tpsnakehead, char drct)
{
    //NODE tmp=snakehead;
    *tpsnakehead= *psnakehead;
    tpsnakehead->next= psnakehead;
    psnakehead-> pre = tpsnakehead;
    switch (drct)
    {
        case Up:      {tpsnakehead->y-=1; tpsnakehead->drct=Up;    }    break;
        case Down:    {tpsnakehead->y+=1; tpsnakehead->drct=Down;  }    break;
        case Left:    {tpsnakehead->x-=2; tpsnakehead->drct=Left;  }    break;
        case Right:   {tpsnakehead->x+=2; tpsnakehead->drct=Right; }    break;
        default :     break;
    }
    return tpsnakehead;
}
STATE HitWallOrBody( NODE * psnakehead,  FOOD food, STATE gamestate)
{

  gamestate.score += (psnakehead->x== food.x) &&(psnakehead->y==food.y);

  if ( (psnakehead->x <2) || (psnakehead->x >=2*MapLength-2) || \
       (psnakehead->y <1) || (psnakehead->y >=MapHeight-1)  )
  {
    gamestate.gameover= 1;
  }

  return gamestate;
}
FOOD ProduceFood(FOOD food)
{
  if ( !food.counter)
  {
    food.x= rand()% (MapLength-2)*2+2;
    food.y= rand()% (MapHeight-2)+1;
    food.counter++;
    GotoXY(food.x,food.y);
    printf("¡ï");
  }
  return food;
}
int FoodEatOrNot(NODE *psnakehead, FOOD food)
{
  if ( (psnakehead->x== food.x) && (psnakehead->y== food.y) ) food.counter=food.counter-1;;
  return food.counter;
}
NODE * UpdateSnakeTail(NODE *psnaketail, FOOD food)
{
  //NODE *ptmp= psnaketail;
  //GotoXY(2,2);printf("%d %d  %d %d %d",psnaketail->pre->x,psnaketail->pre->y,psnaketail->x,psnaketail->y,food.counter);
  if ( food.counter )
  {
    // free(psnaketail);
    GotoXY(psnaketail->x,psnaketail->y);printf(" ");
    GotoXY(psnaketail->x+1,psnaketail->y);printf(" ");
    psnaketail= psnaketail->pre;

    free(psnaketail->next);
    psnaketail->next=NULL;
  }
  //GotoXY(2,3);printf("%d %d  %d %d",psnaketail->pre->x,psnaketail->pre->y,psnaketail->x,psnaketail->y);

  return psnaketail;
}
void EndGame(int maplength, int mapheight)
{
    GotoXY(0,mapheight);
}
