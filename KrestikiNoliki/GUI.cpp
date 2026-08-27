#define TX_SAFE_MODE
#include "TXLib.h"
#include "GUI.h"

int СreateWindow()
{
    txCreateWindow(800, 600); 

    txRectangle(0, 0, 800, 600);

    return 0;
}
void CloseWindows() 
{
    txDestroyWindow();
}

void DrawRectangle(int x, int y, int xx, int yy)
{
    txSetColor(TX_BLACK);
    txFillColor(255, 255, 255);
    txRectangle(x, y, xx, yy);
}

void DrawRectangle(int x, int y, int xx, int yy, int R, int G, int B)
{
    txSetColor(TX_BLACK);
    txFillColor(R, G, B);
    txRectangle(x, y, xx, yy);
}
void DrawTexts(int x, int y, int size, char text[30])
{
    txSetColor(TX_BLACK);
    txSelectFont("Times New Roman", size);    
    txTextOut(x, y, text);
}
void DrawButton(int x, int y, int xx, int yy, int size, char text[30])
{
    DrawRectangle(x, y, xx, yy);
    xx = x + (int)((xx - x) / 10);
    yy = y + (yy-y)/2 - size/2 ;
    DrawTexts(xx, yy, size, text);
}
void DrawButton(int x, int y, int xx, int yy, int R, int G, int B, int size, char text[30])
{
    DrawRectangle(x, y, xx, yy, R, G, B);
    xx = x + (int)((xx - x) / 10);
    yy = y + (yy - y) / 2 - size / 2;
    DrawTexts(xx, yy, size, text);
}
int GetSizeText(int s, int h, char text[30])
{
    int size = 1;
    while (true) 
    {
        size++;
        txSelectFont("Times New Roman", size);
        if (s < txGetTextExtentX(text) || txGetTextExtentY(text) > h) 
        {
            break;
        }
    }
    return(size);
}
int GetMouse(int* x, int* y) 
{
    *x = txMouseX();
    *y = txMouseY();
    return(txMouseButtons());
}
void DebugFunc(const char text[10]) {
    cout << text << "\n";
}
void WindowClear() 
{
    txSetColor(TX_WHITE);
    txFillColor(255, 255, 255);
    txRectangle(0, 0, 800, 600);
}

void DrawX(int left, int top, int right, int bottom)
{
    txSetColor(TX_BLACK, 3);
    txLine(left, top, right, bottom);
    txLine(right, top, left, bottom);
}

void DrawO(int left, int top, int right, int bottom)
{
    int centerX = (left + right) / 2;
    int centerY = (top + bottom) / 2;
    int radius = (right - left) / 3;   

    txSetColor(TX_BLACK, 3);
    txSetFillColor(TX_WHITE);    
    txCircle(centerX, centerY, radius);
}
