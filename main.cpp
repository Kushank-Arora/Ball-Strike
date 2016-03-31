#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include<stdio.h>
#include "resource.h"


#define n_row 5 //2
#define n_col 10  //6
#define n_boxes (n_row*n_col)
#define BORDER 10
#define W_BOX 100  //200
#define H_BOX 50  //100
#define GAP 25  //50
#define S_WIDTH 1300 //800
#define S_HEIGHT 675 //600
#define R_BALL 9 //7
#define W_BOARD 20
#define L_BOARD 100
float STEP=R_BALL*0.5;//(R_BALL/2)


int tellside(int i,int x,int y);
void OnCreate ( HWND hWnd );
void OnDestroy ( HWND hWnd );
void On_Timer(HWND hWnd);
void MouseMove(HWND hWnd,LPARAM lParam);
int Box_P(int,int);
int Win_Check(void);

/* Global Variables */

//HBITMAP hbmp ;
int x, y ;
int move_m;
//float speed=20;
int finish=0;
//HDC hmemdc ;
//HGDIOBJ holdbmp ;
HINSTANCE hInit;
HBITMAP hbmp,hbmp2,hbmp_box[3];
HDC hmemdc,hmemdc2,hmemdc_box[3];
HGDIOBJ holdbmp,holdbmp2,holdbmp_box[3];



struct bo{
    int x,y,present;
} box[n_boxes];

typedef struct d{
    int x1,x2,y1,y2;
}disk;

int Board_P(int x,int y,disk* b);

disk board;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

 //   printf("%d\n",nCmdShow);
    hInit=hThisInstance;
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground =(HBRUSH)0; //CreatePatternBrush ( LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BRUSH ) ) ) ; ////COLOR_BACKGROUND;








    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Brixxx by Kushank Arora"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
       //     WS_TILEDWINDOW,
      //  WS_VISIBLE,
           0,       /* Windows decides the position */
           0,       /* where the window ends up on the screen */
           S_WIDTH+2*BORDER+20+20,                 /* The programs width */
           S_HEIGHT+2*BORDER+40,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
   // printf("%d",nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 //
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY :
            OnDestroy ( hWnd ) ;
            break ;
        case WM_CREATE :
            OnCreate ( hWnd ) ;
            break ;
        case WM_TIMER :
            if(finish==0)
                On_Timer ( hWnd ) ;
            else
                //MessageBox(hWnd,"Game Over","Game Over",0);
                KillTimer ( hWnd, 1 );//Sleep(2000),//OnDestroy(hWnd);
            break ;
        case WM_MOUSEMOVE:
            MouseMove(hWnd,lParam);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hWnd, message, wParam, lParam);
    }

    return 0;
}

void MouseMove(HWND hWnd,LPARAM lParam){
    int x=LOWORD(lParam);
    HDC hdc=GetDC(hWnd);
   // HGDIOBJ holdbmp3;
   // move_m=1;
//holdbmp3=SelectObject(hdc,CreatePatternBrush ( LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BRUSH ) ) ));
    BitBlt ( hdc, board.x1, board.y1, board.x2-board.x1+10, board.y2-board.y1+10, hmemdc, 0, 0, WHITENESS );
   // Rectangle(hdc,board.x1, board.y1, board.x2+10, board.y2+10);
    if(x>BORDER+L_BOARD/2&&x<BORDER+S_WIDTH-L_BOARD/2)
    {
        board.x1=x-L_BOARD/2;
        board.x2=board.x1+L_BOARD;
    }

    BitBlt ( hdc, board.x1, board.y1, board.x2-board.x1+10, board.y2-board.y1+10, hmemdc, 0, 0, SRCCOPY );
    MoveToEx(hdc,BORDER-2,BORDER+S_HEIGHT+2,NULL);
    LineTo(hdc,BORDER+S_WIDTH+2,BORDER+S_HEIGHT+2);
 //   SelectObject(hdc,holdbmp3);

}

void OnCreate ( HWND hWnd )
{
 HDC hdc=GetDC(hWnd);

 srand ( time ( NULL ) *time(NULL)*time(NULL)*time(NULL)*time(NULL)) ;

 x = rand( ) % S_WIDTH +BORDER;
 //y = rand( ) % r.bottom - 22 ;
 y=S_HEIGHT+BORDER-W_BOARD-R_BALL-10 ;

 if(x>BORDER+S_WIDTH-L_BOARD)
    board.x1=x;
 else
    board.x1=BORDER+S_WIDTH-L_BOARD;
 //board.x1=x-10;
 board.x2=board.x1+L_BOARD;
 board.y1=BORDER+S_HEIGHT-W_BOARD;
 board.y2=BORDER+S_HEIGHT;

int x,y;

    for(int i=0;i<n_boxes;i++){
        x=(i)%n_col; y=(i)/n_col;
        box[i].present=1;
        box[i].x=GAP*(x+1)+x*W_BOX;
        box[i].y=GAP*(y+1)+y*H_BOX;
    }





    hbmp = LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BITMAP1 ) ) ;  hbmp2= LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BITMAP2 ) ) ;
    hmemdc = CreateCompatibleDC ( hdc ) ;                           hmemdc2 = CreateCompatibleDC ( hdc ) ;
    holdbmp = SelectObject ( hmemdc, hbmp ) ;                       holdbmp2 = SelectObject ( hmemdc2, hbmp2 ) ;

    hbmp_box[0]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX1 ) ) ;
    hbmp_box[1]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX2 ) ) ;
    hbmp_box[2]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX3  ) ) ;
//    hbmp_box[3]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX4 ) ) ;

    for(int i=0;i<3;i++)
    {
        hmemdc_box[i] = CreateCompatibleDC ( hdc ) ;
        holdbmp_box[i] = SelectObject ( hmemdc_box[i], hbmp_box[i] ) ;
    }





    ReleaseDC ( hWnd, hdc ) ;




 SetTimer ( hWnd, 1, 30, NULL ) ;
}

void OnDestroy ( HWND hWnd )
{
 KillTimer ( hWnd, 1 ) ;

 SelectObject ( hmemdc, holdbmp ) ;                 SelectObject ( hmemdc2, holdbmp2 ) ;
 DeleteDC ( hmemdc ) ;                              DeleteDC ( hmemdc2 ) ;
 DeleteObject ( hbmp ) ;                            DeleteObject ( hbmp2 ) ;

    for(int i=0;i<3;i++)
    {
        SelectObject ( hmemdc_box[i], holdbmp_box[i] ) ;
        DeleteDC ( hmemdc_box[i] ) ;
        DeleteObject ( hbmp_box[i] ) ;
    }


 PostQuitMessage ( 0 ) ;
}

void On_Timer(HWND hWnd)
{
    HDC hdc ;
    //const int wd = 22, ht = 22 ;

 //   PAINTSTRUCT ps;




  HBRUSH hbr;
  HBITMAP hbmp;
  HGDIOBJ holdbr;
    HPEN hpen ;
 HGDIOBJ holdpen;


//UpdateWindow(hWnd);





    static int dx = STEP, dy = -STEP ;
    static int first=0;//,speed=1;
    int i,temp;
    if(finish!=0)
        return;
    hdc = GetDC ( hWnd );


hpen = CreatePen ( PS_SOLID, 1, RGB ( 255, 255, 255 ) ) ;
 holdpen = SelectObject ( hdc, hpen ) ;








  //  BitBlt ( hdc, 10, 20, 190, 220, hmemdc, 0, 0, SRCCOPY ) ;





  //  hdc=BeginPaint(hWnd,&ps);
   // Rectangle(hdc,BORDER,BORDER,BORDER+S_WIDTH,BORDER+S_HEIGHT);
   MoveToEx(hdc,BORDER-2,BORDER-2,NULL);
   LineTo(hdc,BORDER-2,BORDER+S_HEIGHT+2);
   LineTo(hdc,BORDER+S_WIDTH+2,BORDER+S_HEIGHT+2);
   LineTo(hdc,BORDER+S_WIDTH+2,BORDER-2);
   LineTo(hdc,BORDER-2,BORDER-2);


  //  BitBlt ( hdc, x-R_BALL, y-R_BALL, 2*R_BALL, 2*R_BALL, hmemdc2, 0, 0, WHITENESS ) ;



    hbr = CreateSolidBrush ( RGB (255, 255, 255 ) ) ;
    holdbr = SelectObject ( hdc, hbr ) ;

    Ellipse(hdc,x-R_BALL,y-R_BALL,x+R_BALL,y+R_BALL);

    SelectObject ( hdc, holdbr ) ;
    DeleteObject ( hbr ) ;















   // Rectangle(hdc,board.x1,board.y1,board.x2-10,board.y2-10);

  //  BitBlt ( hdc, board.x1, board.y1, board.x2-board.x1+10, board.y2-board.y1+10, hmemdc, 0, 0, SRCCOPY );


  //  if(Win_Check()==1)
  //  finish=1,MessageBox(0,"You Win","You Win",0),OnDestroy(hWnd);

    if(finish!=0)
        return;


    if(first++==0)
    for(i=0;i<n_boxes;i++){
   //     if(box[i].present==1)
   //       Rectangle(hdc,BORDER+box[i].x,BORDER+box[i].y,BORDER+box[i].x+W_BOX,BORDER+box[i].y+H_BOX);
        if(i%2==0)
         BitBlt ( hdc, BORDER+box[i].x,BORDER+box[i].y, W_BOX, H_BOX, hmemdc_box[0], 0, 0, SRCCOPY ) ;
        else if(i%3==0)
            BitBlt ( hdc, BORDER+box[i].x,BORDER+box[i].y, W_BOX, H_BOX, hmemdc_box[1], 0, 0, SRCCOPY ) ;
        else
            BitBlt ( hdc, BORDER+box[i].x,BORDER+box[i].y, W_BOX, H_BOX, hmemdc_box[2], 0, 0, SRCCOPY ) ;
    }
    else
        for(i=0;i<n_boxes;i++)
        if(box[i].present!=1)
          BitBlt ( hdc, BORDER+box[i].x,BORDER+box[i].y, W_BOX, H_BOX, hmemdc_box[0], 0, 0, WHITENESS ) ;


 if(Win_Check()==1)
    finish=1,MessageBox(0,"You Win","You Win",0),OnDestroy(hWnd);

    if(finish!=0)
        return;

    x += dx ;
    if ( x < BORDER+R_BALL )
    {
        x = BORDER+R_BALL ;
        dx = STEP ;
    }
    else if ( x > ( S_WIDTH+BORDER - R_BALL ) )
    {
        x = S_WIDTH+BORDER - R_BALL ;
        dx = -STEP ;
    }
    y += dy ;
    if ( y < BORDER+R_BALL)
    {
        y = BORDER+R_BALL;
        dy = STEP ;
    }
    else if ( y > ( S_HEIGHT+BORDER - R_BALL-W_BOARD ) &&Board_P(x,y,&board)==1)
    {
        y = S_HEIGHT+BORDER - R_BALL -W_BOARD;
        dy = -STEP ;
    }
    else if( y > ( S_HEIGHT+BORDER - R_BALL-W_BOARD ) &&Board_P(x,y,&board)==0){
        if(first!=1){
        finish=1;
        MessageBox(0,"Game Over","Game Over",0),OnDestroy(hWnd) ;
        }
        else
        first=0;
        y = S_HEIGHT+BORDER - R_BALL -W_BOARD;
        dy = -STEP ;
    }
 //   BitBlt ( hdc, x, y, wd, ht, hmemdc, 0, 0, SRCCOPY ) ;
    temp=tellside(Box_P(x,y),x,y);
    if(temp!=-1){

    //    printf("%d\n",temp);
        if(temp==1||temp==2)
            dx*=-1;
        else if(temp==3||temp==4)
            dy*=-1;

            STEP+=0.9;//*=(speed);
            //speed*=1.8;
       //    if(speed>3) speed=3;
  //      speed-=0.7;
  //      KillTimer(hWnd,1);                      //increase the speed
  //      SetTimer ( hWnd, 1, speed+10, NULL ) ;
    }
 //   Ellipse(hdc,x-R_BALL,y-R_BALL,x+R_BALL,y+R_BALL);
 //   BitBlt ( hdc, x-R_BALL, y-R_BALL, 2*R_BALL, 2*R_BALL, hmemdc2, 0, 0, SRCCOPY ) ;





hbmp = LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BRUSH ) ) ;

 hbr = CreatePatternBrush ( hbmp ) ;
 holdbr = SelectObject ( hdc, hbr ) ;

 //Rectangle ( hdc, 0, 0, 345, 100 ) ;
  Ellipse(hdc,x-R_BALL,y-R_BALL,x+R_BALL,y+R_BALL);


 SelectObject ( hdc, holdbr ) ;
 DeleteObject ( hbr ) ;
 DeleteObject ( hbmp ) ;



SelectObject ( hdc, holdpen ) ;
 DeleteObject ( hpen ) ;













    ReleaseDC ( hWnd, hdc ) ;
 //   EndPaint(hWnd,&ps);
}
int Board_P(int x,int y,disk* b)
{
    if(x>=b->x1-(STEP)&&x<=b->x2+(STEP))
        return 1;
    else
        return 0;
}
int Box_P(int x,int y)
{
    int i,a,b,k,l,m,n;
    int ans=-1;
    for(i=0;i<n_boxes;i++)
    {
        if(box[i].present==0)
            continue;
        a=i%n_col; b=i/n_col;
        k=(x-R_BALL<=GAP*(a+1)+(a+1)*W_BOX+BORDER);
        l=(x+R_BALL>=GAP*(a+1)+(a)*W_BOX+BORDER);
        m=(y-R_BALL<=GAP*(b+1)+(b+1)*H_BOX+BORDER);
        n=(y+R_BALL>=GAP*(b+1)+(b)*H_BOX+BORDER);
       // if((()^())^(()^()))
      /*  if(k==0&&l*m*n!=0)
            ans=1;
        else if (l==0&&k*m*n!=0)
            and=2;
        else if (m==0&&k*l*n!=0)
            ans=3;
        else if(n==0&&k*l*m!=0)
            ans=4;
        //else ans=0;
     */
        if(k&&l&&m&&n)
            box[i].present=0,ans=i;;
   //     printf("%d: k=%d l=%d m=%d n=%d\n",i+1,k,l,m,n);
    }
  //  printf("\n");
    return ans;
}
int Win_Check(void)
{
    for(int i=0;i<n_boxes;i++)
        if(box[i].present==1)
            return 0;
    return 1;
}
int tellside(int i,int x,int y)
{
    if(i==-1)
        return -1;
    int a,b,k,l,m,n,ans=-1;
    a=i%n_col; b=i/n_col;
    k=((x-R_BALL>=GAP*(a+1)+(a+1)*W_BOX+BORDER-R_BALL)&&(x-R_BALL<=GAP*(a+1)+(a+1)*W_BOX+BORDER+R_BALL));
    l=((x+R_BALL>=GAP*(a+1)+(a)*W_BOX+BORDER-R_BALL)&&(x+R_BALL<=GAP*(a+1)+(a)*W_BOX+BORDER+R_BALL));
    m=((y-R_BALL>=GAP*(b+1)+(b+1)*H_BOX+BORDER-R_BALL)&&(y-R_BALL<=GAP*(b+1)+(b+1)*H_BOX+BORDER+R_BALL));
    n=((y+R_BALL>=GAP*(b+1)+(b)*H_BOX+BORDER-R_BALL)&&(y+R_BALL<=GAP*(b+1)+(b)*H_BOX+BORDER+R_BALL));

      /*  if(k==0&&l*m*n!=0)
            ans=1;
        else if (l==0&&k*m*n!=0)
            ans=2;
        else if (m==0&&k*l*n!=0)
            ans=3;
        else if(n==0&&k*l*m!=0)
            ans=4;*/
        if(k==1)
            ans=1;
        else if(l==1)
            ans=2;
        else if(m==1)
            ans=3;
        else if(n==1)
            ans=4;
    return ans;
}
