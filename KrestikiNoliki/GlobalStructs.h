#pragma once
#include "GUI.h"
#include <string>
struct Button
{
	int* Func;
	int value;
	char text[30];
	int coords[4];//Левый верхний(x, y), правый нижний
	int sizetext;
	bool States[2] = { false, true }; 
	bool IsActive;
	void CreateButton(int x, int y, int xx, int yy, const char texts[30], int* Fun, int val)
	{
		coords[0] = x;
		coords[1] = y;
		coords[2] = xx;
		coords[3] = yy;
		Func = Fun;
		value = val;
		strcpy_s(text, texts);
		sizetext = SetTextSize();
		IsActive = true;
		States[0] = false;
		States[1] = true;
	
	}

	void PaintButton()
	{
		if (!States[0] && States[1])
		{
			DrawButton(coords[0], coords[1], coords[2], coords[3], sizetext, text);
			States[1] = false;
		
		}
		if (States[0] && !States[1]) 
		{
			DrawButton(coords[0], coords[1], coords[2], coords[3], 250, 0, 0, sizetext, text);
			States[1] = true;
		}
	
	}
	int SetTextSize()
	{
		int s = coords[2] - coords[0];
		int h = coords[3] - coords[1];
		s = s - 2 * s / 10;
		h = s - 2 * s / 10;
		return(GetSizeText(s, h, text));
	}
	void Click() {
		*Func = value;
	}
	void OnCursor() 
	{
		States[0] = true;
		PaintButton();
	}
	void OutCursor() 
	{
		States[0] = false;
		PaintButton();
	}
	void Clear() 
	{
		IsActive = false;
	}

};
struct Pole
{
	int index;
	int coords[4];
	char Symvol = '*';
	bool IsActive = false;
	void CreatePole(int x, int y, int xx, int yy, int ind)
	{
		coords[0] = x;
		coords[1] = y;
		coords[2] = xx;
		coords[3] = yy;
		index = ind;
		Symvol = '*';
		IsActive = true;
	}
	void PaintBorder() 
	{
		DrawRectangle(coords[0], coords[1], coords[2], coords[3]);
	}

	void PaintSymvol() 
	{
		if (Symvol == '0') 
		{
			DrawO(coords[0] - 4, coords[1] - 4, coords[2] - 4, coords[3] - 4);
			return;
		}
		if (Symvol == 'X')
		{
			DrawX(coords[0] + 4, coords[1] + 4, coords[2] - 4, coords[3] - 4);
			return;
		}
	}
	int SwapSymvol(char Sym) 
	{
		if (Symvol != '*') return 0;
		Symvol = Sym; PaintSymvol();
		return(1);
	}
	void OnCursor(char Sym) {
		if (Symvol != '*') return;

		if (Sym == '0')
		{
			DrawO(coords[0] - 4, coords[1] - 4, coords[2] - 4, coords[3] - 4);
			return;
		}
		if (Sym == 'X')
		{
			DrawX(coords[0] + 4, coords[1] + 4, coords[2] - 4, coords[3] - 4);
			return;
		}
	}
	void OutCursor() 
	{
		if (Symvol == '*') DrawRectangle(coords[0], coords[1], coords[2], coords[3]);
	}

	void ClearPole() 
	{
		Symvol = '*';
		IsActive = false;

	}
	


};
struct Page
{

};
