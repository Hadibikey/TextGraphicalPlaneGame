#include <ncurses.h> //adding  ncurses //
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define Min_X_GamePlay 0 // gameplay window //
#define Min_Y_GamePlay 0
#define GamePlay_COLS 150
#define GamePlay_LINES 57
#define Min_X_Detail 150 // detail window //
#define Min_Y_Detail 0
#define Detail_COLS 36
#define Detail_LINES 28
#define Min_X_Pause 150 // pause window //
#define Min_Y_Pause 28
#define Pause_COLS 36
#define Pause_LINES 29
#define Min_X_Info 70 // pause window //
#define Min_Y_Info 18
#define Info_COLS 45
#define Info_LINES 20
typedef struct Player player;
typedef struct Plane plane;
typedef struct Ship ship;
typedef struct Bullet bullet;
typedef struct Helicopter helicopter;
typedef struct LeftBody leftBody ;
typedef struct RightBody rightBody;
typedef struct Fuel fuel;
WINDOW *detail;
WINDOW *gameplay;
WINDOW *Pause;
int PLANE = 1;
int SPEED = 11;
struct Player
{
    char name[50];                 // name string                             //
    int age;                      // player age                               //
    int score;                   // player score                            //
    char datetime[50];          // time & date of the game start            //
    int GameLevel;
    int PlaneType;
};
struct Plane
{
    int fuel;                   //   plane fuel     //
    int bodyX[8];              //  body x's       //
    int bodyY[8];             //   body y's       //
    int life;                //   plane life    //
    char body[8][14];       //  plane body      //
    char lastBody[8][14];
};
struct Ship
{
    int bodyX[4];            // body x's      //
    int bodyY[4];           // body y's       //
    char body[4][11];
    char lastBody[4][11];
};
struct Bullet
{
    int x;                 //     bullet x's      //
    int y;                //      bullet y's      //
    int lastX;
    int lastY;
};
struct LeftBody
{
    int leftSideBodyX[10];             //    gameBody x's     //
    int leftSideBodyY[10];           //     gameBody y's    //
    char body[10][12];
    char lastBody[10][12];
};
struct RightBody
{
    int rightSideBodyX[10];          //      gameBody x's        //
    int rightSideBodyY[10];        //       gameBody y's       //
    char body[10][12];
    char lastBody[10][12];
};
struct Fuel
{
    int bodyX[2];               //      fuel body x's       //
    int bodyY[2];             //       fuel body y's      //
    char body[2][5];
    char lastBody[2][5];
};
struct Helicopter
{
    int bodyX[6];          //   helicopter body x's    //
    int bodyY[6];          //  helicopter body y's      //
    char body[6][10];
    char lastBody[6][10];
};
void MainMenu() ;
void StartGame () ;
void HowToPlay() ;
void About();
int  ExitGame();
void ScoresTable();
void PrintMainMenu(int choice);
void SaveScore(player guest);
void PrintSettingsMenu ( int choice );
void SettingsMenu();
void ChooseYourPlane();
void ChangeGameSpeed();
void StartSound();
void ShipDestroySound();
void HelicopterDestroySound();
void GameOverSound();
void PlaneHitSound();
void ThanksSound();
void FuelFillSound();
void removeShip(ship *titanic);
void initialization (plane *f35, player *guest, ship titanic[], leftBody LB[], rightBody RB[], fuel gasoline[] , helicopter MQ8[] , int PLANE ) ;
int main() /* Main */
{
    initscr();                     // enter ncurses                   //
    curs_set(0);                  // disable curser show on terminal //
    start_color();               // starting colour                 //
    keypad(stdscr,TRUE);        // enable kayboard                 //
    init_pair(99, COLOR_WHITE, COLOR_BLACK);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    wbkgd(stdscr, COLOR_PAIR(99));
    echo();                     // enable echo on the screen //
    cbreak();                  //  cbreak mode ( ctrl + c ) //
    raw();                    // disable line buffering    //
    MainMenu();
    endwin();                // end ncurses mode          //
    return 0;
}
void MainMenu()/* Game Manu  */
{
    int key;
    int choice = 0;
    StartSound();
    PrintMainMenu(choice);
    noecho();
    do
    {
        key = getch();
        switch(key)
        {
        case KEY_DOWN:
            choice++;
            if(choice > 5 ) choice = 0;
            break;
        case KEY_UP:
            choice--;
            if(choice < 0)  choice = 5 ;
            break;
        default:
            break;
        }
        PrintMainMenu(choice);
    }
    while(key != '\n');
    echo();
    wclear(stdscr);
    refresh();
    if (choice == 0) StartGame(); // Starting Game //
    if (choice == 1) ScoresTable(); // show scores //
    if (choice == 2) HowToPlay(); // tuturial for play //
    if (choice == 3) SettingsMenu(); // Game Settings //
    if (choice == 4) About(); // about writer //
    if (choice == 5) ExitGame(); // exit game //
    refresh();
}
void PrintSettingsMenu ( int choice )/*Settings Menu */
{
char SettingsItems[3][20];
int i;
strcpy(SettingsItems[0] , "GAME SPEED");
strcpy(SettingsItems[1] , "PLANE SHAPE");
strcpy(SettingsItems[2] , "BACK");
    for(i=0; i<3; i++)
    {
        if( i == choice )
            attron( A_REVERSE |  A_BOLD);
        mvaddstr ( 10+(i*3) , COLS/2-55 , SettingsItems[i] );
        attroff( A_REVERSE | A_BOLD);
    }
    refresh();
}
void SettingsMenu()/*Settings Menu */
{
wclear(stdscr);
wrefresh(stdscr);
refresh();
nodelay(stdscr , 0 );
int PLANE;
int input;
int choice = 0 ;
PrintSettingsMenu(choice);
noecho();
do{
   input = getch();
   switch(input)
   {
	case KEY_DOWN :
            choice++;
            if(choice > 2 ) choice = 0;
            break;
        case KEY_UP   :
            choice--;
            if(choice < 0)  choice = 2 ;
            break;
        default:
            break;
   }
PrintSettingsMenu(choice);
}
while (input != '\n');
echo();
wclear(stdscr);
if(choice == 0 )
{
ChangeGameSpeed();
}
if(choice == 1 )
{
  ChooseYourPlane();
}
if(choice == 2 )
{
  MainMenu();
}
}
void ChangeGameSpeed()
{
	wclear(stdscr);
	refresh();
	int input;
	int S=0 , L=1;
        do
	{
	nodelay(stdscr , 1 );
	input = getch();
  attron(A_REVERSE);
	mvprintw(20 , 30 , "<");
	mvprintw(20 , 40 , ">");
  attroff(A_REVERSE);
	if(S==0)
	{
	mvprintw( 20 , 31 , "         ");
	mvprintw( 20 , 34 , "EASY");
	SPEED = 13;
	}
	if(S==1)
	{
	mvprintw( 20 , 31 , "         ");
	mvprintw( 20 , 32 , "MEDIUM");
	SPEED = 9;
	}
	if(S==2)
	{
	mvprintw( 20 , 31 , "         ");
	mvprintw( 20 , 34 , "HARD");
	SPEED = 4;
	}
	switch(input)
	{
		case KEY_RIGHT :
		    S++;
		    if (S>2) S=0;
		    break;
		case KEY_LEFT :
		    S--;
		    if (S<0) S=2;
		    break;
		default:
		    break;
	}
	}
	while (input != '\n');
		refresh();
	if(S==0 || S==1 || S==2) SettingsMenu();

}
void PrintMainMenu(int choice)/* Print Menu */
{
    init_pair(101,COLOR_GREEN,COLOR_BLACK);
    init_pair(130,COLOR_CYAN,COLOR_BLACK);
    int i ;
    char titles[6][25];
    strcpy(titles[0],"S T A R T  G A M E") ;
    strcpy(titles[1],"S C O R E S") ;
    strcpy(titles[2],"H O W  T O  P L A Y") ;
    strcpy(titles[3],"S E T T I N G S");
    strcpy(titles[4],"A B O U T");
    strcpy(titles[5],"E X I T");
    wclear(stdscr);
    char wlc[6][57];
    char jt[21][71];
    FILE *w = fopen("welcome.txt" , "r"); /* welcome shape in welcome */
    for(i=0 ; i<6 ; i++)
    {
        fgets(wlc[i] , sizeof(wlc[i]) , w );
        attron(COLOR_PAIR(101));
        mvprintw(2+i , 65 , "%s" , wlc[i]);
        refresh();
        attroff(COLOR_PAIR(101));
    }
    fclose(w);
    FILE *jet = fopen("jet_21_70.txt" , "r"); /* plane shape in welcome */
    for(i=0 ; i<21 ; i++)
    {
        fgets(jt[i] , sizeof(jt[i]) , jet );
        attron(COLOR_PAIR(130) | A_BOLD );
        mvprintw(10+i , 100 ,"%s" , jt[i]);
        attroff(COLOR_PAIR(130) | A_BOLD );
    }
    fclose(jet);
    mvprintw(4 , 87 , "W E L C O M E");
    mvprintw(5 , 83, "Written By : H4D1B1K3Y");
    for(i=0; i<6; i++)
    {
        if( i == choice )
            attron( A_REVERSE |  A_BOLD);
             if(i==0) mvaddstr(  20  , 35 ,  titles[0]);
             if(i==1) mvaddstr(  22 , 38 ,  titles[1]);
             if(i==2) mvaddstr(  24 , 35 ,  titles[2]);
             if(i==3) mvaddstr(  26 , 37 ,  titles[3]);
             if(i==4) mvaddstr(  28 , 39 ,  titles[4]);
             if(i==5) mvaddstr(  30 , 40 ,  titles[5]);
            attroff( A_REVERSE | A_BOLD);
    }
    refresh();
}
int RandomFunction()/* Generate Random Numbers between 127 & 14 */
{
    napms(70);
    int t;
    srand(time(0)); // srand function for randomize by time //
    do t = ((rand()%1000)*5*9/8)%185/2 ;
    while (	t > 127  ||   t< 14 );
    return t ;
}
void MyTypeFunction (WINDOW *win, int y, int x, char text[])/*Fantasy Type function with insch tool */
{
    char *t;
    int len = strlen(text);
    t = text;
    while (len)
    {
        wmove ( win, y, x );
        winsch(win, *(t+len-1));
        wrefresh(win);
        napms(30);
        len--;
    }
}
void HowToPlay()/* How To Play For Players */
{
    wclear(stdscr); // clear the standard  screen //
    refresh();
    attron(A_BOLD); // bold attribute //
    MyTypeFunction(stdscr, 10,10,"-Welcome To RiverRaid Game");
    MyTypeFunction(stdscr, 11,10,"-You Have To Move The Plane & Save It From Crashes By \"W\" - \"A\" - \"S\" - \"D\" Buttons");
    MyTypeFunction(stdscr, 12,10,"-If U Collapse The Jungle You Will Loose One Life");
    MyTypeFunction(stdscr, 13,10,"-If U Collapse The Ships & Helicopters You Will Loose One Life");
    MyTypeFunction(stdscr, 14,10,"-You Can Shoot & Destroy The Ships & Helicopters By Tapping \"H\" Button");
    MyTypeFunction(stdscr, 15,10,"-The Plane's Fuel Increasing By The Time & If That Became Empty You Will Die");
    MyTypeFunction(stdscr, 16,10,"-You Can  Use Fuels Which Puted In the Way To Fill The Plane's Fuel");
    MyTypeFunction(stdscr, 20 , 10 , "press any key to continue...");
    attroff(A_BOLD); // turn off attribute //
    nodelay ( stdscr, 0); // wait for input //
    getch(); // get input //
    MainMenu(); // goto main menu //
}
void About()/*About Writer */
{
    wclear(stdscr);
    attron(A_BOLD);
    MyTypeFunction(stdscr, 10, 10, "Written By : H4D1B1K3Y");
    MyTypeFunction(stdscr, 11, 10, "Computer Engeeniring Student At Zanjan University");
    MyTypeFunction(stdscr, 12, 10, "Midterm Project - Dec 2017");
    MyTypeFunction(stdscr, 13, 10, "Many Thanks To My Teacher : Mr. Hossein Mohammadi & Our Class Assistant : Mr. Amir Ghasemi");
    MyTypeFunction(stdscr, 14, 10, "Send Feedback");
    MyTypeFunction(stdscr, 15, 10, "hadibikey@gmail.com");
    MyTypeFunction(stdscr, 16, 10, "www.telegram.me/hadibikey");
    MyTypeFunction(stdscr, 20, 10, "Press Any Key to continue ...");
    attroff(A_BOLD);
    refresh();
    nodelay ( stdscr, 0 );
    getch();
    MainMenu();
}
void ScoresTable()/*Show Scores */
{
    int i;
    wclear(stdscr);
    wrefresh(stdscr); // refresh standard screen //
    init_pair(129 , COLOR_YELLOW  , COLOR_BLACK );
    init_pair(130 , COLOR_CYAN  , COLOR_BLACK );
    init_pair(131 , COLOR_GREEN  , COLOR_BLACK );
    init_pair(132 , COLOR_WHITE  , COLOR_BLACK );
    attron(A_BOLD);
    attrset(COLOR_PAIR(129));
    mvprintw(10,20,"NAME");
    attrset(COLOR_PAIR(130));
    mvprintw(10,40,"AGE");
    attrset(COLOR_PAIR(131));
    mvprintw(10,60,"SCORE");
    attrset(COLOR_PAIR(132));
    mvprintw(10,80,"DATE");
    FILE *ScoresTable = fopen("scores.hd" , "r+"); // opening scores file //
    player pl[30];
    fread(pl, sizeof(player) , 30 , ScoresTable ); // reading structures from file and putting into array //
    fclose(ScoresTable);
    for(i=0 ; i<30 ; i++)
    {
        attrset(COLOR_PAIR(129));
        mvprintw(12+i , 20 , "%s" ,  pl[i].name ); // print names //
        attrset(COLOR_PAIR(130));
        mvprintw(12+i , 40 , "%d" ,  pl[i].age ); // print ages //
        attrset(COLOR_PAIR(131));
        mvprintw(12+i , 60 , "%d" ,  pl[i].score ); // print scores //
        attrset(COLOR_PAIR(132));
        mvprintw(12+i , 80 , "%s" ,  pl[i].datetime ); // print date of play //
    }
    MyTypeFunction(stdscr , 50 , 20 , "press any key to continue ... ");
    attroff(A_BOLD);
    nodelay(stdscr , 0 );
    char ch = getch() ;
    MainMenu();
    refresh();
}
void RecievePlayerInformation(player *guest)/*recieve player information */
{
    wclear(gameplay);
    wclear(detail);
    wclear(Pause);
    wbkgd(gameplay, COLOR_BLACK ) ; // make      //
    wbkgd(detail, COLOR_BLACK) ;   // window    //
    wbkgd(Pause,COLOR_BLACK) ;    // colorful  //
    wrefresh(detail) ;
    wrefresh(gameplay) ;
    wrefresh(Pause) ;
    WINDOW *info; // make window pointer //
    info=newwin(Info_LINES , Info_COLS , Min_Y_Info , Min_X_Info ); // creating information window //
    init_pair(100,COLOR_WHITE,COLOR_GREEN) ;
    wbkgd(info,COLOR_PAIR(100)) ;
    wattron(info, A_BOLD ) ;
    MyTypeFunction(info,2,2,"PLAYER NAME :") ;
    mvwscanw(info,2,16,"%s",&guest->name); // recieve name //
    MyTypeFunction(info,4,2,"PLAYER AGE :") ;
    mvwscanw(info,4,15,"%d",&guest->age) ; // recieve age //
    napms(35);
    wrefresh(info);
    mvwprintw(info,12,2,"WELL DONE!") ;
    napms(35) ;
    wrefresh(info);
    mvwprintw(info,14,2,"LET'S PLAY  %s . . . ", guest->name);
    napms(35);
    wrefresh(info);
    napms(35);
    wattroff(info, A_BOLD );
    delwin(info); // deleting information window //
    wrefresh(gameplay);
}
void UpdateDetails(player guest, plane f35)/*Updating info on detail window */
{
    init_pair(103,COLOR_BLACK,COLOR_YELLOW);
    init_pair(105,COLOR_RED, COLOR_YELLOW);
    wbkgd(detail,COLOR_PAIR(103));
    wattron(detail, A_BOLD);
    mvwprintw(detail,2,2,"%s",guest.datetime);
    mvwprintw(detail,3,2,"PLAYER NAME : %s",guest.name);
    mvwprintw(detail,4,2,"PLAYER AGE : %d",guest.age);
    mvwprintw(detail,5,2,"PLAYER SCORE : %d",guest.score);
    mvwprintw(detail,19,1,"FUEL:");
    mvwprintw(detail,16,1,"LIFE:");
    wattron(detail, COLOR_PAIR(105));
    mvwprintw(detail,25,2,"Press P to Pause The Game");
    mvwprintw(detail,26,2,"Press Q to Quit The Game");
    wattroff(detail, COLOR_PAIR(105) | A_BOLD );
    wrefresh(detail);
}
int ExitGame()/*exit game */
{
    char c;
    wclear(stdscr);
    refresh();
    attron(A_BOLD);
    MyTypeFunction(stdscr,28 , 77, "ARE YOU SURE WANT TO EXIT ??   Y/N");
    attroff(A_BOLD);
    while (1)
    {
        nodelay ( stdscr, 1 );
        noecho();
        c = getch();
        echo();
        if ( c == 'y' || c == 'Y')
        {
            wclear(stdscr);
            ThanksSound();
            attron(A_BOLD);
            MyTypeFunction(stdscr, 28, 82 , "Thanks For Playing ..." );
            attroff(A_BOLD);
            refresh();
            napms(1000);
            endwin();
            exit(0);
        }
        if ( c == 'n' || c == 'N') MainMenu();
        else continue;
    }
}
void ExtraGame(plane *f35 , player *guest ,int *ask )
{
if(guest->score>=15)
{
  int temp = guest->score;
  mvwprintw(detail,5,18,"  ");
  wrefresh(detail);
  guest->score = temp - 15 ;
  f35->life +=2 ;
  *ask = 1;
}
else if(guest->score < 14)
{
  wattron(Pause , A_BOLD);
  MyTypeFunction(Pause , 11 , 2 , "YOU DONT HAVE ENOUGH SCORE!");
  wattroff(Pause , A_BOLD);
  wrefresh(Pause);
}
}
void Crashed(player *guest , plane *f35)/*game over */
{
    SaveScore(*guest); /* save scores */
    GameOverSound(); /* game over sound */
    int k ;
    int ask=0;
    char c;
    init_pair(110, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(111, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(112, COLOR_CYAN, COLOR_MAGENTA );
    Pause = newwin (Pause_LINES , Pause_COLS , Min_Y_Pause , Min_X_Pause );
    wclear(Pause);
    wbkgd(Pause, COLOR_PAIR(110));
    wattron(Pause, A_BOLD);
    MyTypeFunction(Pause,7,2,"1-PLAY AGAIN");
    MyTypeFunction(Pause,8,2,"2-EXIT");
    MyTypeFunction(Pause,9,2,"3-EXTRA GAME");
    wattroff(Pause, A_BOLD);
    wrefresh(Pause);
    while(1)
    {
        napms(6);
        k++;
        if ( k %150 == 0 )
        {
            wattron (Pause, A_BOLD | COLOR_PAIR(111));
            mvwprintw(Pause,6,2,"YOU LOOSE !!");
            wattroff (Pause, A_BOLD | COLOR_PAIR(111));
            wrefresh(Pause);
            napms(500);
            wattron ( Pause, A_BOLD | COLOR_PAIR(112));
            mvwprintw(Pause,6,2,"YOU LOOSE !!");
            wattroff (Pause, A_BOLD | COLOR_PAIR(112));
        }
        nodelay ( Pause, 1 );
        noecho();
        c = wgetch(Pause);
        echo();
        if (c == '1')
        {
            StartGame();
        }
        if (c == '2')
            ExitGame();
        if (c == '3'){
        ExtraGame(&(*f35) , &(*guest) , &ask );
        if (ask == 1 ){
          wclear(Pause);
          wrefresh(Pause);
          break;
        }
      }
        else continue ;
        wrefresh(Pause);
    }
}
void GamePaused()/*- pause game */
{
    char c;
    int k;
    Pause = newwin (Pause_LINES , Pause_COLS , Min_Y_Pause , Min_X_Pause );
    init_pair(110, COLOR_WHITE, COLOR_MAGENTA);
    wbkgd(Pause, COLOR_PAIR(110));
    while (1)
    {
        napms(6);
        k++;
        wattron(Pause, A_BOLD);
        if (k % 220 == 0)
        {
            MyTypeFunction(Pause, 2 , 2 , "            PAUSED!!         ");
        }
        mvwprintw(Pause, 4, 2, "press R to resume");
        mvwprintw(Pause, 6, 2, "press Q to Quit");
        wattroff(Pause, A_BOLD);
        nodelay(Pause, 1);
        noecho();
        c = wgetch(Pause);
        echo();
        if ( c == 'R' || c == 'r' )
        {
            wclear(Pause);
            wrefresh(Pause);
            break;
        }
        if ( c == 'q' || c == 'Q')
        {
            wattron(Pause, A_BOLD);
            MyTypeFunction(Pause,8,2,"Quiting...");
            wattron(Pause, A_BOLD);
            wrefresh(Pause);
            ThanksSound();
            napms(2000);
            endwin();
            exit(0);
        }
        else continue;
    }
}
void PrintJungle(leftBody *LB, rightBody *RB , int n )/* print jungles */
{
    int i ;
    init_pair(113, COLOR_BLACK, COLOR_GREEN);
    init_pair(120 , COLOR_BLACK , COLOR_YELLOW);
    for ( i = 0 ; i < 10 ; i++)/* clear jungles first */
    {
        mvwprintw(gameplay, LB->leftSideBodyY[i],  LB->leftSideBodyX[i], LB->lastBody[i]);
        mvwprintw(gameplay, RB->rightSideBodyY[i], RB->rightSideBodyX[i], RB->lastBody[i]);
    }
    for ( i = 0 ; i < 10 ; i++)/* Y's + 1 */
    {
        if(RB->rightSideBodyY[i]>=56 || LB->leftSideBodyY[i]>=56)
        {
            RB->rightSideBodyY[i] = 0 ;
            LB->leftSideBodyY[i] = 0 ;
        }
        RB->rightSideBodyY[i]++;
        LB->leftSideBodyY[i]++;
    }
    for ( i = 0 ; i <10 ; i++)/* finally print them */
    {
        if (n%2==0)wattron ( gameplay , COLOR_PAIR(113) | A_BOLD);
        if (n%2!=0)wattron ( gameplay , COLOR_PAIR(120) | A_BOLD);
        mvwprintw(gameplay , RB->rightSideBodyY[i], RB->rightSideBodyX[i] , "%s", RB->body[i]) ;
        mvwprintw(gameplay , LB->leftSideBodyY[i],  LB->leftSideBodyX[i] , "%s", LB->body[i]) ;
        wrefresh(gameplay);
        wattroff (gameplay, COLOR_PAIR(113) | COLOR_PAIR(120) | A_BOLD );
    }
    wrefresh( gameplay );
}
void SortScores(player pl[])/*sort scores in the array */
{
    int h , t ;
    player temp;
    for(h=0; h<30; h++)
        for(t=0; t<30; t++)
            if (pl[h].score > pl[t].score)/* insertion sort */
            {
                temp = pl[h];
                pl[h] = pl[t];
                pl[t] = temp;
            }
}
void SaveScore(player guest)/* save the score */
{
    int i ;
    player pl[30];/* first , we have a array of player structures */
    for(i=0 ; i<30 ; i++)/* free them */
    {
        strcpy(pl[i].name , "" ) ;
        pl[i].score = 0 ;
        pl[i].age = 0 ;
        strcpy(pl[i].datetime , "") ;
    }
    FILE *s2 = fopen("scores.hd" , "r+");/*open scores file */
    fread(pl, sizeof(player) , 30 , s2 );/*read them to array */
    fclose(s2);
    if(guest.score >= pl[29].score)/* adding the score to array */
        pl[29]=guest;
    SortScores(pl);/* sorting them */
    FILE *s3 = fopen("scores.hd" , "w");
    fwrite(pl , sizeof(player) , 30 , s3 ) ; /* writing to the file  , sorted */
    fclose(s3);
}
void UpdatePrintPlane(plane *f35 , char c , int PLANE  )/* update & print plane */
{
    int pl;
    if (PLANE == 1 ) pl = 5 ;
    if (PLANE == 2 ) pl = 8 ;
    init_pair(109, COLOR_RED, COLOR_BLUE) ;
    init_pair(108, COLOR_YELLOW, COLOR_BLUE) ;
    init_pair(117 , COLOR_WHITE , COLOR_BLUE) ;
    int i;
    for ( i = 0 ; i < pl ; i++)
        mvwprintw(gameplay, f35->bodyY[i], f35->bodyX[i],f35->lastBody[i]);
    if (c == 'A' || c == 'a' )
        for ( i = 0 ; i<pl ; i++)
            (f35->bodyX[i])--;
    if (c == 'D' || c == 'd')
        for (i = 0 ; i<pl ; i++)
            (f35->bodyX[i])++;
    if ( c == 'W' || c == 'w')
        for ( i = 0 ; i<pl ; i++)
            (f35->bodyY[i])--;
    if ( c == 'S' || c == 's')
        for (i = 0 ; i<pl ; i++ )
            (f35->bodyY[i])++;
    if (PLANE == 1 )
    {
        wattron(gameplay , A_BOLD |  COLOR_PAIR(117));
        mvwprintw(gameplay, f35->bodyY[0], f35->bodyX[0],f35->body[0]);
        wattroff(gameplay , COLOR_PAIR(117));
        wattron(gameplay, COLOR_PAIR(108));
        mvwprintw(gameplay, f35->bodyY[1], f35->bodyX[1],f35->body[1]);
        mvwprintw(gameplay, f35->bodyY[2], f35->bodyX[2],f35->body[2]);
        mvwprintw(gameplay, f35->bodyY[3], f35->bodyX[3],f35->body[3]);
        wattrset(gameplay, COLOR_PAIR(109));
        mvwprintw(gameplay, f35->bodyY[4], f35->bodyX[4],f35->body[4]);
        wattroff(gameplay, A_BOLD| COLOR_PAIR(108) | COLOR_PAIR(109));
    }
    if (PLANE == 2 )
    {
        wattron(gameplay , A_BOLD | COLOR_PAIR(108));
        for(i=0 ; i<pl ; i++)
            mvwprintw(gameplay, f35->bodyY[i], f35->bodyX[i],f35->body[i]);
        wattroff(gameplay , A_BOLD | COLOR_PAIR(108));
    }
}
void UpdatePrintShips ( ship *titanic)
{
    int i ;
    for ( i = 0 ; i < 4 ; i++ )
    {
        mvwprintw ( gameplay, titanic->bodyY[i], titanic->bodyX[i], "%s", titanic->lastBody[i]);
        wrefresh(gameplay);
    }
    init_pair(104, COLOR_RED, COLOR_BLUE );
    for ( i = 0 ; i < 4 ; i++ )
    {
        titanic->bodyY[i]++ ;
    }
    for ( i = 0 ; i < 4 ; i++ )
    {
        wattron(gameplay, COLOR_PAIR(104) | A_BOLD );
        mvwprintw(gameplay, titanic->bodyY[i], titanic->bodyX[i],"%s", titanic->body[i]);
        wattroff(gameplay, COLOR_PAIR(104) | A_BOLD);
        wrefresh(gameplay);
    }
}
void UpdatePrintFuels(fuel *gasoline)
{
    init_pair(115, COLOR_WHITE, COLOR_BLACK );
    int i;
    for(i = 0 ; i < 2 ; i++)
    {
        mvwprintw(gameplay, gasoline->bodyY[i], gasoline->bodyX[i],gasoline->lastBody[i]);
        wrefresh(gameplay);
    }
    for(i=0 ; i<2 ; i++)
    {
        gasoline->bodyY[i]++;
    }
    for(i = 0 ; i < 2 ; i++)
    {
        wattron(gameplay, A_BOLD | COLOR_PAIR(115));
        mvwprintw(gameplay, gasoline->bodyY[i], gasoline->bodyX[i], gasoline->body[i]);
        wattroff(gameplay, A_BOLD | COLOR_PAIR(115));
        wrefresh(gameplay);
    }
}
void HelicopterShoot( bullet a500[] , bullet b500[] , int bullA , int bullB)
{
    int o ;
    for(o=0 ; o<=bullA ; o++ )
    {
        mvwprintw( gameplay , a500[o].y , a500[o].x , " ");

        a500[o].y++;

        wattron(gameplay , COLOR_PAIR(123) | A_BOLD );
        mvwprintw( gameplay , a500[o].y , a500[o].x , "*");
        wattroff(gameplay , COLOR_PAIR(123) | A_BOLD );
    }
    for(o=0 ; o<=bullB ; o++ )
    {

        mvwprintw( gameplay , b500[o].y , b500[o].x , " ");

        b500[o].y++;
        wattron(gameplay , COLOR_PAIR(123) | A_BOLD );

        mvwprintw( gameplay , b500[o].y , b500[o].x , "*");
        wattroff(gameplay , COLOR_PAIR(123) | A_BOLD );
    }
}
void PlaneShoot(bullet c500[] , int b)
{
    int i ;
    for(i=0 ; i <= b ; i++)
    {

        mvwprintw(gameplay, c500[i].y , c500[i].x ," ");
        c500[i].y--;
        wattron(gameplay, A_BOLD | COLOR_PAIR(107));
        mvwprintw(gameplay, c500[i].y, c500[i].x, "|" );
        wattroff(gameplay, A_BOLD | COLOR_PAIR(107));
    }
}
void ShipAndHelicopterDestroyCheck(bullet c500[] , ship titanic[] , player *guest , plane *f35 ,int b  , helicopter MQ8[])
{
    int k , i , t ;
    for(k=0 ; k<4 ; k++)
        for(i = 0 ; i<=b ; i++)
            for(t = 0 ; t < 7 ; t++)
            {
                if (( ( c500[i].y == titanic[k].bodyY[3] && c500[i].x == titanic[k].bodyX[3]+t) || (c500[i].y == titanic[k].bodyY[2] && (c500[i].x == titanic[k].bodyX[2] || c500[i].x == titanic[k].bodyX[2]+8 )) ) || ((c500[i].y == titanic[k].bodyY[3]-1 && c500[i].x == titanic[k].bodyX[3]+t) || (c500[i].y == titanic[k].bodyY[2]-1 && (c500[i].x == titanic[k].bodyX[2]-1 || c500[i].x == titanic[k].bodyX[2]+8 ))))
                {
                    ShipDestroySound();
                    mvwprintw(gameplay , c500[i].y , c500[i].x , " ");
                    c500[i].y = -60;
                    guest->score++;
                    UpdateDetails(*guest,*f35);
                    wrefresh(detail);
                    for ( i = 0 ; i<4 ; i++)
                    {
                        mvwprintw(gameplay, titanic[k].bodyY[i], titanic[k].bodyX[i],"%s", titanic[k].lastBody[i]);
                    }
                    wrefresh(gameplay);
                    titanic[k].bodyX[0] = RandomFunction();
                    titanic[k].bodyX[1] = titanic[k].bodyX[0] - 3 ;
                    titanic[k].bodyX[2] = titanic[k].bodyX[0] - 3 ;
                    titanic[k].bodyX[3] = titanic[k].bodyX[0] - 2 ;
                    titanic[k].bodyY[3] = -5 ;
                    titanic[k].bodyY[2] = -6 ;
                    titanic[k].bodyY[1] = -7 ;
                    titanic[k].bodyY[0] = -8 ;
                    UpdatePrintShips(&titanic[k]);
                }
            }

    for ( k = 0 ; k<2 ; k++ )
        for(i = 0 ; i<=b ; i++)
            for(t = 0 ; t < 7 ; t++ )
            {
                if ((c500[i].y == MQ8[k].bodyY[5] && c500[i].x == MQ8[k].bodyX[5]) || (c500[i].y == MQ8[k].bodyY[5] && c500[i].x == MQ8[k].bodyX[5]+1) || (c500[i].y == MQ8[k].bodyY[5] && c500[i].x == MQ8[k].bodyX[5]+2) || (c500[i].y == MQ8[k].bodyY[4] && c500[i].x == MQ8[k].bodyX[4]+t) )
                {
                    HelicopterDestroySound();
                    mvwprintw(gameplay , c500[i].y , c500[i].x , " ");
                    c500[i].y = -60;
                    guest->score+=3;
                    UpdateDetails(*guest,*f35);
                    wrefresh(detail);
                    for ( i = 0 ; i<6 ; i++)
                    {
                        mvwprintw(gameplay, MQ8[k].bodyY[i], MQ8[k].bodyX[i],"         ");
                    }
                    wrefresh(gameplay);
                    MQ8[k].bodyX[0] = RandomFunction();
                    MQ8[k].bodyX[1] = MQ8[k].bodyX[0] ;
                    MQ8[k].bodyX[2] = MQ8[k].bodyX[0] - 1 ;
                    MQ8[k].bodyX[3] = MQ8[k].bodyX[0] - 2 ;
                    MQ8[k].bodyX[4] = MQ8[k].bodyX[0] - 2 ;
                    MQ8[k].bodyX[5] = MQ8[k].bodyX[0] ;
                    MQ8[k].bodyY[0] = -5+1;
                    MQ8[k].bodyY[1] = -4+1;
                    MQ8[k].bodyY[2] = -3+1;
                    MQ8[k].bodyY[3] = -2+1;
                    MQ8[k].bodyY[4] = -1+1;
                    MQ8[k].bodyY[5] = -0+1;
                    wrefresh(gameplay);

                }
            }
}
void removeShip(ship *titanic)
{
  int i;
  for ( i = 0 ; i<4 ; i++)
  {
      mvwprintw(gameplay, titanic->bodyY[i], titanic->bodyX[i],"%s", titanic->lastBody[i]);
  }
  titanic->bodyY[0]=-10;
  titanic->bodyY[1]=-9;
  titanic->bodyY[2]=-8;
  titanic->bodyY[3]=-7;

  wrefresh(gameplay);
}
void PlaneCrashCheck(ship titanic[], plane *f35  , player *guest , helicopter MQ8[] )
{
    int k, p ;
    for( k = 0 ; k<4 ; k++)
        for( p = 0 ; p < 7 ; p++)
        {
            if ((f35->bodyY[0] == titanic[k].bodyY[3] && f35->bodyX[0] == titanic[k].bodyX[3]+p)){
                f35->life--;
                PlaneHitSound();
                removeShip(&titanic[k]);
              }
            else if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1] == titanic[k].bodyX[3]+p)){
              removeShip(&titanic[k]);
              PlaneHitSound();
                f35->life--;
              }
            else if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1]+1 == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1]+2== titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1]+4 == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1]+5 == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else  if ((f35->bodyY[1] == titanic[k].bodyY[3] && f35->bodyX[1]+6 == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else   if ((f35->bodyY[2] == titanic[k].bodyY[3] && f35->bodyX[2] == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            else   if ((f35->bodyY[2] == titanic[k].bodyY[3] && f35->bodyX[2]+8 == titanic[k].bodyX[3]+p))
                {f35->life--;
                  PlaneHitSound();
                removeShip(&titanic[k]);}
            UpdateDetails( *guest , *f35 );
            wrefresh(detail);
            if (f35->life == 0 ) Crashed(&(*guest) , &(*f35));
        }
}
void FuelCheck(plane *f35, fuel *gasoline)
{
    int i, j, p ;
    if (gasoline->bodyY[0] >= 58 )
    {
        gasoline->bodyY[0] =-35;
        gasoline->bodyY[1] =-34;
        gasoline->bodyX[0] = RandomFunction();
        gasoline->bodyX[1] = gasoline->bodyX[0] -1 ;
    }
    for( i = 0 ; i < 4 ; i++)
    {
        if ( (gasoline->bodyY[1] == f35->bodyY[0] && gasoline->bodyX[1]+i == f35->bodyX[0]+1 ) || (gasoline->bodyY[1] == f35->bodyY[1] && gasoline->bodyX[1]+i == f35->bodyX[0]+2) )
        {
            FuelFillSound();
            f35->fuel = 100 ;

            for(p = 0 ; p < 2 ; p++)
                mvwprintw(gameplay, gasoline->bodyY[p], gasoline->bodyX[p], gasoline->lastBody[p]);
            gasoline->bodyY[0] =-50;
            gasoline->bodyY[1] =-49;
            gasoline->bodyX[0] = RandomFunction();
            gasoline->bodyX[1] = gasoline->bodyX[0] -1 ;
        }

        for( j = 0 ; j < 7 ; j++)
        {
            if (gasoline->bodyY[1] == f35->bodyY[1] && gasoline->bodyX[1]+i == f35->bodyX[1]+j )
            {
                f35->fuel = 100 ;

                for(p = 0 ; p < 2 ; p++)
                    mvwprintw(gameplay, gasoline->bodyY[p], gasoline->bodyX[p], gasoline->lastBody[p]);
                gasoline->bodyY[0] =-50;
                gasoline->bodyY[1] =-49;
                gasoline->bodyX[0] = RandomFunction();
                gasoline->bodyX[1] = gasoline->bodyX[0] -1 ;
            }
        }
    }
}
void MyTimer(int *ds , int *s , int *m , player *guest)
{

    init_pair(119 , COLOR_RED, COLOR_YELLOW);
    wattron(detail , A_BOLD | COLOR_PAIR(119));
    (*ds)++;
    if ((*ds)>9)
    {
        (*ds)=0;
        (*s)++;
        if ((*s)>59)
        {
            (*s)=0;
            (*m)++;
        }
    }
    mvwprintw(detail , 6 , 2 , "%d : %d : %d",*m,*s,*ds);
    wattroff(detail , A_BOLD | COLOR_PAIR(119));
    wrefresh(detail);
}
void ChooseYourPlane()/* Choose Plane */
{
    init_pair(125 , COLOR_RED , COLOR_BLACK );
    init_pair(126 , COLOR_YELLOW , COLOR_BLACK );
    init_pair(127 , COLOR_WHITE , COLOR_BLACK );
    wclear(stdscr);
    attron(A_BOLD);
    char pl1[5][11];
    char pl2[8][14];
    int j;
    char input;
    attron(COLOR_PAIR(127));
    MyTypeFunction(stdscr , 4 , 4 , "Please Choose Your Plane ");
    MyTypeFunction(stdscr ,  24 , 4 , "Press B to go to MainMenu");
    attroff(COLOR_PAIR(127));
    FILE *f1=fopen("pl1.txt" , "r" );
    for(j=0 ; j<5 ; j++)
    {
        fgets(pl1[j] , sizeof(pl1[j]) , f1 );
    }
    fclose(f1);
    FILE *f2=fopen("pl2.txt" , "r" );
    for(j=0 ; j<8 ; j++)
    {
        fgets(pl2[j] , sizeof(pl2[j]) , f2 );
    }
    fclose(f2);

    mvprintw(9 , 24 , "1-" );
    mvprintw(9 , 54 , "2-" );

    attron(COLOR_PAIR(125));
    for(j=0 ; j < 5 ; j++ )
        mvprintw(10+j , 30 , pl1[j] );
    attroff(COLOR_PAIR(125));
    attron(COLOR_PAIR(126));
    for(j=0 ; j < 8 ; j++ )
        mvprintw(10+j , 64 , pl2[j] );
    attroff(COLOR_PAIR(126));
    wrefresh(stdscr);
    noecho();
    nodelay(stdscr , 0);
input :
    input = getch();
    if (input != '1' && input != '2' && input != 'b' && input !='B')
        goto input;
    echo();
    switch(input)
    {
    case '1':
        PLANE=1;
        break;
    case '2':
        PLANE=2;
        break;
    case 'b' :
        SettingsMenu();
        break;
    }
    attron(COLOR_PAIR(127));
    MyTypeFunction(stdscr , 40 , 20 , "DONE !!" );
    attroff(COLOR_PAIR(127) | A_BOLD);
    wrefresh(stdscr);
    napms(2000);
    wclear(stdscr);
    wrefresh(stdscr);
    SettingsMenu();
}
void UpdatePrintHelicopters2( helicopter *MQ8 , int o , int *h1 , int *h2 )
{
    int i;
    init_pair(122 , COLOR_WHITE  , COLOR_BLUE );

    for( i = 0 ; i < 6 ; i++)
        mvwprintw(gameplay , MQ8->bodyY[i] , MQ8->bodyX[i]  , "%s" , MQ8->lastBody[i]);
    for( i = 0 ; i < 6 ; i++ )
    {
        if (o == 0 )
        {
            if ((*h1)==0) MQ8->bodyX[i]++;
            if ((*h1)==1) MQ8->bodyX[i]--;
        }
        if (o == 1 )
        {
            if ((*h2)==0) MQ8->bodyX[i]++;
            if ((*h2)==1) MQ8->bodyX[i]--;
        }
    }
    if (MQ8->bodyX[4]+6 == 131 )
    {
        if (o == 0 )
            (*h1) = 1 ;
        if (o == 1 )
            (*h2) = 1 ;
    }
    if (MQ8->bodyX[4] == 11)
    {
        if (o == 0 )
            (*h1)=0;
        if (o == 1 )
            (*h2) = 0 ;
    }
    wattron(gameplay , A_BOLD | COLOR_PAIR(122) | A_PROTECT );
    for( i = 0 ; i < 6 ; i++ )
        mvwprintw(gameplay , MQ8->bodyY[i] , MQ8->bodyX[i] , "%s" , MQ8->body[i]);
    wattroff(gameplay , A_BOLD | COLOR_PAIR(122) | A_PROTECT );
    wrefresh(gameplay);
}
void PrintLifes(int life )/* show lifes fantasy */
{
  int i;
  init_pair(131 , COLOR_MAGENTA , COLOR_RED);
  for(i=0 ; i<life ; i++)
  {
    wattron(detail , COLOR_PAIR(131) );
    mvwprintw(detail , 17 , 1+(i*4) , "   ");
    mvwprintw(detail , 18 , 1+(i*4) , "   ");
    wattroff(detail , COLOR_PAIR(131) );
  }
  if (life <=2 )
  {
    wattron(detail , A_BOLD );
    mvwprintw(detail , 16 , 6 , "LOW LIFE !!!");
    wattroff(detail ,A_BOLD );
  }
  wrefresh(detail);
}
void PrintFuel(int fuel)/* show fuel fantasy */
{
    int i , j  , c ;
    init_pair(128 , COLOR_MAGENTA , COLOR_BLACK);
    c = fuel/3;
    for(i=0 ; i<=c ; i++)
    {
        wattron(detail , COLOR_PAIR(128) );
        mvwprintw(detail , 20 , 1+i , " ");
        mvwprintw(detail , 21 , 1+i , " ");
        wattroff(detail , COLOR_PAIR(128) );
        wrefresh(detail);
    }
}
void UpdatePrintHelicopters1( helicopter *MQ8 )
{
    int i;
    init_pair(121 , COLOR_WHITE  , COLOR_BLUE );
    for( i = 0 ; i < 6 ; i++)
        mvwprintw(gameplay , MQ8->bodyY[i] , MQ8->bodyX[i] , "%s" , MQ8->lastBody[i]);
    for( i = 0 ; i < 6 ; i++ )
    {
        MQ8->bodyY[i]++;
    }
    if (MQ8->bodyY[0] >= 57 )
    {
        MQ8->bodyY[0] = -38 ;
        MQ8->bodyY[1] = -37 ;
        MQ8->bodyY[2] = -36 ;
        MQ8->bodyY[3] = -35 ;
        MQ8->bodyY[4] = -34 ;
        MQ8->bodyY[5] = -33 ;
    }
    wattron(gameplay , A_BOLD | COLOR_PAIR(121));
    for( i = 0 ; i < 6 ; i++ )
        mvwprintw(gameplay , MQ8->bodyY[i] , MQ8->bodyX[i] , "%s" , MQ8->body[i]);
    wattroff(gameplay , A_BOLD | COLOR_PAIR(121));
    wrefresh(gameplay);
}
void ShipRebuild( ship titanic[] )
{
    int o , i , l ;
    for (o = 0 ; o < 4 ; o++)
    {
        if ( titanic[o].bodyY[0] >= 57 )
        {
            titanic[o].bodyX[0] = RandomFunction();
            titanic[o].bodyX[1] = titanic[o].bodyX[0] - 3 ;
            titanic[o].bodyX[2] = titanic[o].bodyX[0] - 3 ;
            titanic[o].bodyX[3] = titanic[o].bodyX[0] - 2 ;
            for ( i = 0 ; i<4 ; i++)
            {
                if ( titanic[o].bodyX[0] == titanic[i].bodyX[0] || titanic[o].bodyX[0] == titanic[i].bodyX[0]+1)
                {
                    titanic[o].bodyY[3] = -i * 4 -2 ;
                    titanic[o].bodyY[2] = -i * 4 -3 ;
                    titanic[o].bodyY[1] = -i * 4 -4 ;
                    titanic[o].bodyY[0] = -i * 4 -5 ;
                }
                for ( l=0 ; l<=8 ; l++)
                    if (titanic[o].bodyX[0] == titanic[i].bodyX[1]+l || titanic[o].bodyX[0] == titanic[i].bodyX[2]+l || titanic[o].bodyX[0] == titanic[i].bodyX[3]+l )
                    {
                        titanic[o].bodyY[3] = -i * 4 -2 ;
                        titanic[o].bodyY[2] = -i * 4 -3 ;
                        titanic[o].bodyY[1] = -i * 4 -4 ;
                        titanic[o].bodyY[0] = -i * 4 -5 ;
                    }
                    else
                    {
                        titanic[o].bodyY[3] = 0;
                        titanic[o].bodyY[2] = -1;
                        titanic[o].bodyY[1] = -2;
                        titanic[o].bodyY[0] = -3;
                    }
            }
            UpdatePrintShips(&titanic[o]);
        }
    }
}
void PlaneDestroyCheck ( player *guest , bullet a500[] , bullet b500[] , plane *f35 , int bullA , int bullB )
{
    int i , j ;
    for( i=0 ; i<bullA ; i++)
    {
        if(( a500[i].x == f35->bodyX[0] && a500[i].y == f35->bodyY[0]) || (a500[i].x == f35->bodyX[0]+1 && a500[i].y == f35->bodyY[0]) )
        {
            f35->life--;
            wrefresh(detail);
            wrefresh(gameplay);

        }
        for(j=0 ; j<7 ; j++)
        {
            if(a500[i].x == f35->bodyX[1]+j && a500[i].y == f35->bodyY[1] )
            {
                PlaneHitSound();
                f35->life--;
                wrefresh(detail);
                wrefresh(gameplay);
            }
        }
    }
    for( i=0 ; i<bullB ; i++)
    {
        if(( b500[i].x == f35->bodyX[0] && b500[i].y == f35->bodyY[0]) || (b500[i].x == f35->bodyX[0]+1 && b500[i].y == f35->bodyY[0]) )
        {
          PlaneHitSound();
            f35->life--;
            wrefresh(detail);
        }
        for(j=0 ; j<7 ; j++)
        {
            if(b500[i].x == f35->bodyX[1]+j && b500[i].y == f35->bodyY[1])
            {
              PlaneHitSound();
                f35->life--;
                wrefresh(detail);
            }
        }
    }
    wrefresh(detail);
    if (f35->life == 0 ) Crashed(&(*guest) , &(*f35) );
}
void StartSound()
{
  Mix_Music *start;
  start = Mix_LoadMUS("background.wav");
  Mix_PlayMusic(start, 4);
}
void ShootSound()
{
  Mix_Music *shoot;
  shoot = Mix_LoadMUS("shoot.wav");
  Mix_PlayMusic(shoot, 1);

}
void ShipDestroySound()
{
  Mix_Music *SDestroy;
  SDestroy = Mix_LoadMUS("shipDestroy.wav");
  Mix_PlayMusic(SDestroy, 1);
}
void HelicopterDestroySound()
{
  Mix_Music *HDestroy;
  HDestroy = Mix_LoadMUS("helicopterDestroy.wav");
  Mix_PlayMusic(HDestroy, 1);
}
void GameOverSound()
{
  Mix_Music *Gover;
  Gover = Mix_LoadMUS("gameover.wav");
  Mix_PlayMusic(Gover, 1);
}
void PlaneHitSound()
{
  Mix_Music *Hit;
  Hit = Mix_LoadMUS("hit.wav");
  Mix_PlayMusic(Hit, 1);
}
void ThanksSound()
{
  Mix_Music *thanks;
  thanks = Mix_LoadMUS("thanks.wav");
  Mix_PlayMusic(thanks, 1);
}
void FuelFillSound()
{
  Mix_Music *fill;
  fill = Mix_LoadMUS("fill.wav");
  Mix_PlayMusic(fill, 1);
}
void StartGame ()
{
    keypad(gameplay,TRUE);
    int  i=0 , j , k , o , p , BulletNumbers=-1 , t , z;
    detail = newwin( Detail_LINES , Detail_COLS , Min_Y_Detail , Min_X_Detail);
    int h1 = 0 , h2 = 1 ;
    int bullA=0 , bullB=0;
    int s , ds , m ;
    ds=s=m=0;
    int situation = 0;
    char c;
    plane f35 ;
    player guest ;
    bullet c500[1000];
    bullet a500[1000];
    bullet b500[1000];
    ship titanic[4];
    helicopter MQ8[2];
    leftBody LB[4];
    rightBody RB[4];
    fuel gasoline[2];
    f35.fuel=100;
    f35.life = 5;
    RecievePlayerInformation(&guest);
    gameplay = newwin(GamePlay_LINES , GamePlay_COLS , Min_Y_GamePlay , Min_X_GamePlay );
    init_pair(102,COLOR_WHITE,COLOR_BLUE);
    init_pair(107, COLOR_WHITE, COLOR_BLACK);
    wbkgd(gameplay,COLOR_PAIR(102));
    initialization( &f35, &guest, titanic, LB, RB, gasoline , MQ8 , PLANE  );
    Mix_PauseMusic();
    UpdateDetails(guest,f35);
    PrintFuel(f35.fuel);
    PrintLifes(f35.life);
    UpdatePrintPlane( &f35 , c  , PLANE ) ;
    wrefresh(gameplay);
    while (1)
    {
        j++ ;
        if ( j == 100000000)
            j = 0 ;
        napms(SPEED);
        if ( j%2 == 0 )
        {
            PlaneShoot( c500 , BulletNumbers );
            ShipAndHelicopterDestroyCheck( c500 , titanic , &guest , &f35 , BulletNumbers , MQ8 );
        }
        if (j%90==0)
        {
            if (MQ8[0].bodyY[5]>0)
            {
                bullA++;
                a500[bullA].x = MQ8[0].bodyX[5]+1;
                a500[bullA].y = MQ8[0].bodyY[5]+2;
            }
            if (MQ8[1].bodyY[5]>0)
            {
                bullB++;
                b500[bullB].x = MQ8[1].bodyX[5]+1;
                b500[bullB].y = MQ8[1].bodyY[5]+2;
            }
        }
        init_pair(123 , COLOR_YELLOW ,COLOR_BLUE );
        if (j % 4 == 0 )
        {
          PlaneDestroyCheck ( &guest , a500 , b500 , &f35 , bullA , bullB );
            HelicopterShoot(a500 , b500 , bullA , bullB );
        }
        if (j % 5 == 0)
        {
            for(p = 0 ; p < 4 ; p++)
                PrintJungle(&LB[p], &RB[p] , p );
        }
        if ( j%10 == 0)
        {
            MyTimer(&ds,&s,&m,&guest);
        }
        PrintFuel(f35.fuel);
        PrintLifes(f35.life);
        wrefresh(detail);
        if ( j%47 == 0 )
        {
            f35.fuel--;
            UpdateDetails(guest, f35 );
            wrefresh(detail);
            if (f35.fuel == 0 ) Crashed(&guest , &f35 );
        }
        if ( j % 42 == 0 )
        {
            for (o = 0 ; o < 2 ; o++ )
                UpdatePrintFuels(&gasoline[o]);
            for( o = 0 ; o < 2 ; o++ )
                FuelCheck(&f35, &gasoline[o]);
        }
        if ( j % 290 == 0 )
        {
            for(o = 0 ; o<2 ; o++ )
                UpdatePrintHelicopters1 ( &MQ8[o] );
        }
        if ( j % 24 == 0 )
        {
            for(o = 0 ; o<2 ; o++ )
                UpdatePrintHelicopters2 ( &MQ8[o] , o , &h1 , &h2);
        }
        if ( j % 21 == 0 )
        {
            for(o = 0 ; o<4 ; o++)
                UpdatePrintShips(&titanic[o]) ;
            PlaneCrashCheck(titanic, &f35 , &guest , MQ8 ) ;
            ShipRebuild(titanic);
        }
        nodelay(gameplay,1) ;
        noecho() ;
        c = wgetch(gameplay) ;
        echo();
        if (c == 'w' || c == 'a'  || c == 'd' || c == 's' || c == 'W' || c == 'A' || c == 'D' || c == 'S')
        {
            UpdatePrintPlane(&f35 , c , PLANE);
            if (f35.bodyX[2] <= 10 || f35.bodyX[2] >= 131 )
            {
                f35.life--;
                mvwprintw(detail,6,2,"PLANE LIFE : %d",f35.life);
                wrefresh(detail);
                if(f35.life == 0 )
                    Crashed(&guest , &f35);
            }
            wrefresh(detail);
            wrefresh(gameplay);
        }
        if ( c == 'Q' || c == 'q')
        {
            ExitGame();
        }
        if (c == 'H' || c== 'h')
        {
            ShootSound();
            BulletNumbers++;
            c500[BulletNumbers].x = f35.bodyX[0];
            c500[BulletNumbers].y = f35.bodyY[0]-1;
            if (BulletNumbers >= 1000)
            {
                BulletNumbers=0;
            }
        }
        if ( c == 'p' || c == 'P' )
        {
            GamePaused();
        }
        if ( c == '0' )
            break;
        else continue;
    }
}
void initialization (plane *f35, player *guest, ship titanic[], leftBody LB[], rightBody RB[], fuel gasoline[] , helicopter MQ8[] , int PLANE )
{
    int i, j;
    if (PLANE == 1 )
    {
        // Plane 1 First Location //
        f35->bodyX[0]=95;
        f35->bodyX[1]=92;
        f35->bodyX[2]=91;
        f35->bodyX[3]=93;
        f35->bodyX[4]=94;
        f35->bodyY[0]=51;
        f35->bodyY[1]=52;
        f35->bodyY[2]=53;
        f35->bodyY[3]=54;
        f35->bodyY[4]=55;
        // Plane 1 Shape //
        strcpy(f35->body[0] , ".");
        strcpy(f35->body[1] , "__/ \\__");
        strcpy(f35->body[2] , "/__   __\\");
        strcpy(f35->body[3] , "/_ _\\");
        strcpy(f35->body[4] , ":::");
        strcpy(f35->lastBody[0] , " ");
        strcpy(f35->lastBody[1] , "       ");
        strcpy(f35->lastBody[2] , "         ");
        strcpy(f35->lastBody[3] , "     ");
        strcpy(f35->lastBody[4] , "   ");
    }
    else if ( PLANE == 2 )
    {
        // Plane 2 First Location //

        f35->bodyX[0]=95;
        f35->bodyX[1]=94;
        f35->bodyX[2]=93;
        f35->bodyX[3]=92;
        f35->bodyX[4]=91;
        f35->bodyX[5]=90;
        f35->bodyX[6]=90;
        f35->bodyX[7]=95;
        f35->bodyY[0]=46;
        f35->bodyY[1]=47;
        f35->bodyY[2]=48;
        f35->bodyY[3]=49;
        f35->bodyY[4]=50;
        f35->bodyY[5]=51;
        f35->bodyY[6]=52;
        f35->bodyY[7]=53;

        // Plane 2 Shape //
        strcpy(f35->body[0] , "/\\");
        strcpy(f35->body[1] , "/<>\\");
        strcpy(f35->body[2] , "/=  =\\");
        strcpy(f35->body[3] , "/      \\");
        strcpy(f35->body[4] , "/ /\\  /\\ \\");
        strcpy(f35->body[5] , "/ /  \\/  \\ \\");
        strcpy(f35->body[6] , "\\/   /\\   \\/");
        strcpy(f35->body[7] , "::");
        strcpy(f35->lastBody[0] , "  ");
        strcpy(f35->lastBody[1] , "    ");
        strcpy(f35->lastBody[2] , "      ");
        strcpy(f35->lastBody[3] , "        ");
        strcpy(f35->lastBody[4] , "          ");
        strcpy(f35->lastBody[5] , "            ");
        strcpy(f35->lastBody[6] , "            ");
        strcpy(f35->lastBody[7] , "   ");
    }
    // Game's Start Time //
    time_t StartTime;
    StartTime=time(NULL);
    strcpy(guest->datetime , ctime(&StartTime) );
    // Player's Score //
    guest->score=0;
    // helicopters location //
    MQ8[0].bodyY[0]=-50;
    MQ8[0].bodyY[1]=-49;
    MQ8[0].bodyY[2]=-48;
    MQ8[0].bodyY[3]=-47;
    MQ8[0].bodyY[4]=-46;
    MQ8[0].bodyY[5]=-45;
    MQ8[1].bodyY[0]= 20;
    MQ8[1].bodyY[1]= 21;
    MQ8[1].bodyY[2]= 22;
    MQ8[1].bodyY[3]= 23;
    MQ8[1].bodyY[4]= 24;
    MQ8[1].bodyY[5]= 25;
    // helicopters location //
    for( i = 0 ; i < 2 ; i++)
    {
        napms(1000);
        MQ8[i].bodyX[0] = RandomFunction() ;
        MQ8[i].bodyX[1] = MQ8[i].bodyX[0] ;
        MQ8[i].bodyX[2] = MQ8[i].bodyX[0] -1 ;
        MQ8[i].bodyX[3] = MQ8[i].bodyX[0] -2 ;
        MQ8[i].bodyX[4] = MQ8[i].bodyX[0] -2 ;
        MQ8[i].bodyX[5] = MQ8[i].bodyX[0] ;
    }
    // helicopters body //
    for( i = 0 ; i<2 ; i++)
    {
        strcpy( MQ8[i].body[0] , " \\ /");
        strcpy( MQ8[i].body[1] , " /|\\");
        strcpy( MQ8[i].body[2] , " __|__");
        strcpy( MQ8[i].body[3] , " /+++++\\");
        strcpy( MQ8[i].body[4] , " \\_____/ ");
        strcpy( MQ8[i].body[5] , " / \\ ");
        strcpy( MQ8[i].lastBody[0] , "    " );
        strcpy( MQ8[i].lastBody[1] , "    " );
        strcpy( MQ8[i].lastBody[2] , "        " );
        strcpy( MQ8[i].lastBody[3] , "        " );
        strcpy( MQ8[i].lastBody[4] , "        " );
        strcpy( MQ8[i].lastBody[4] , "    " );
    }
    // gasolines locations //
    gasoline[0].bodyY[0]= -10 ;
    gasoline[0].bodyY[1]= -9 ;
    gasoline[0].bodyX[0]= 154 ;
    gasoline[0].bodyX[1]= 153 ;
    gasoline[1].bodyY[0]= -40 ;
    gasoline[1].bodyY[1]=-39 ;
    gasoline[1].bodyX[0]=32 ;
    gasoline[1].bodyX[1]=31 ;
    // gasolines Shapes //
    for ( j = 0 ; j < 2 ; j++)
    {
        strcpy(gasoline[j].body[0], "__") ;
        strcpy(gasoline[j].body[1], "|__|") ;
        strcpy(gasoline[j].lastBody[0], "  ") ;
        strcpy(gasoline[j].lastBody[1], "    ") ;
    }
    // The Ships First Y ( X's is random ) //
    titanic[0].bodyY[0]=1;
    titanic[0].bodyY[1]=2;
    titanic[0].bodyY[2]=3;
    titanic[0].bodyY[3]=4;
    titanic[1].bodyY[0]=10;
    titanic[1].bodyY[1]=11;
    titanic[1].bodyY[2]=12;
    titanic[1].bodyY[3]=13;
    titanic[2].bodyY[0]=21;
    titanic[2].bodyY[1]=22;
    titanic[2].bodyY[2]=23;
    titanic[2].bodyY[3]=24;
    titanic[3].bodyY[0]=34;
    titanic[3].bodyY[1]=35;
    titanic[3].bodyY[2]=36;
    titanic[3].bodyY[3]=37;
    // The Ship's Body //
    for ( j = 0 ; j < 4 ; j++)
    {
        strcpy(titanic[j].body[0], "|\\");
        strcpy(titanic[j].body[1], "___|_\\___");
        strcpy(titanic[j].body[2], "\\H-A-D-I/");
        strcpy(titanic[j].body[3], "\\_____/");
        strcpy(titanic[j].lastBody[0], "   ");
        strcpy(titanic[j].lastBody[1], "         ");
        strcpy(titanic[j].lastBody[2], "          ");
        strcpy(titanic[j].lastBody[3], "        ");
    }
    // The Jungle's X Locaions & Body Shape //
    for ( j =0; j<4 ; j++)
    {
        RB[j].rightSideBodyX[0]=149;
        RB[j].rightSideBodyX[1]=147;
        RB[j].rightSideBodyX[2]=145;
        RB[j].rightSideBodyX[3]=143;
        RB[j].rightSideBodyX[4]=139;
        RB[j].rightSideBodyX[5]=139;
        RB[j].rightSideBodyX[6]=143;
        RB[j].rightSideBodyX[7]=145;
        RB[j].rightSideBodyX[8]=147;
        RB[j].rightSideBodyX[9]=149;
        strcpy(LB[j].body[0],"\\");
        strcpy(LB[j].body[1],"\\\\\\");
        strcpy(LB[j].body[2],"\\\\\\\\\\");
        strcpy(LB[j].body[3],"\\\\\\\\\\\\\\");
        strcpy(LB[j].body[4],")))))))))))");
        strcpy(LB[j].body[5],")))))))))))");
        strcpy(LB[j].body[6],"///////");
        strcpy(LB[j].body[7],"/////");
        strcpy(LB[j].body[8],"///");
        strcpy(LB[j].body[9],"/");
        strcpy(RB[j].body[0],"/");
        strcpy(RB[j].body[1],"///");
        strcpy(RB[j].body[2],"/////");
        strcpy(RB[j].body[3],"///////");
        strcpy(RB[j].body[4],"(((((((((((");
        strcpy(RB[j].body[5],"(((((((((((");
        strcpy(RB[j].body[6],"\\\\\\\\\\\\\\");
        strcpy(RB[j].body[7],"\\\\\\\\\\");
        strcpy(RB[j].body[8],"\\\\\\");
        strcpy(RB[j].body[9],"\\");
        strcpy(LB[j].lastBody[0]," ");
        strcpy(LB[j].lastBody[1],"   ");
        strcpy(LB[j].lastBody[2],"     ");
        strcpy(LB[j].lastBody[3],"       ");
        strcpy(LB[j].lastBody[4],"           ");
        strcpy(LB[j].lastBody[5],"           ");
        strcpy(LB[j].lastBody[6],"       ");
        strcpy(LB[j].lastBody[7],"     ");
        strcpy(LB[j].lastBody[8],"   ");
        strcpy(LB[j].lastBody[9]," ");
        strcpy(RB[j].lastBody[0]," ");
        strcpy(RB[j].lastBody[1],"   ");
        strcpy(RB[j].lastBody[2],"     ");
        strcpy(RB[j].lastBody[3],"       ");
        strcpy(RB[j].lastBody[4],"           ");
        strcpy(RB[j].lastBody[5],"           ");
        strcpy(RB[j].lastBody[6],"       ");
        strcpy(RB[j].lastBody[7],"     ");
        strcpy(RB[j].lastBody[8],"   ");
        strcpy(RB[j].lastBody[9]," ");
    }
    // The Jungle 1 Y Locations //
    for( j = 0 ; j< 10 ; j++ )
    {
        LB[0].leftSideBodyY[j] = j;
        RB[0].rightSideBodyY[j]= j;
    }
    // The Jungle 2 Y Locations //
    j = 4;
    for(i = 0 ; i < 10 ; i++ )
    {
        LB[1].leftSideBodyY[i] =10+j;
        RB[1].rightSideBodyY[i] =10+j;
        j++;
    }
    // The Jungle 3 Y Locations //
    j = 8;
    for(i = 0 ; i < 10 ; i++)
    {
        LB[2].leftSideBodyY[i] =20+j;
        RB[2].rightSideBodyY[i]=20+j;
        j++;
    }
    // The Jungle 4 Y Locations //
    j =  12;
    for(i = 0 ; i < 10 ; i++)
    {
        LB[3].leftSideBodyY[i] =30+j;
        RB[3].rightSideBodyY[i]=30+j;
        j++;
    }
    // Setting All Left Side Jungle's X = 0 //
    for( j = 0 ; j < 4 ;  j++ )
        for( i = 0 ; i < 10 ;  i++ )
            LB[j].leftSideBodyX[i] =0;
    // Setting Ships X s Randomly //
    for ( i = 0 ; i < 4 ; i++ )
    {
        napms(400);
        titanic[i].bodyX[0] = RandomFunction()+5;
        titanic[i].bodyX[1] = titanic[i].bodyX[0] - 3 ;
        titanic[i].bodyX[2] = titanic[i].bodyX[0] - 3 ;
        titanic[i].bodyX[3] = titanic[i].bodyX[0] - 2 ;
    }
}
