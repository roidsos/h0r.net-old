#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

void CreateWin(int WinID,uint_16 xTo,uint_8 yTo, uint_16 width,uint_8 height, char* title,uint_8 border_color, uint_8 inside_color);
void MoveWin(int WinID, int xTo,int yTo);
void DeleteWin(int WinID);
void UpdateAndRender();
#endif // __WINDOWMANAGER_H__