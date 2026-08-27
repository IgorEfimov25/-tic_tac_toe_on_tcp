#pragma once
int СreateWindow();
void DrawRectangle(int x, int y, int xx, int yy);
void DrawRectangle(int x, int y, int xx, int yy, int R, int G, int B);
void DrawTexts(int x, int y, int size, char text[30]);
void DrawButton(int x, int y, int xx, int yy, int size, char text[30]);
void DrawButton(int x, int y, int xx, int yy, int R, int G, int B, int size, char text[30]);
int GetSizeText(int s, int h, char text[30]);
int GetMouse(int* x, int* y);
void DrawX(int left, int top, int right, int bottom);
void DrawO(int left, int top, int right, int bottom);
void DebugFunc(const char text[10]);
void CloseWindows();
void SleepTimer();
void WindowClear();