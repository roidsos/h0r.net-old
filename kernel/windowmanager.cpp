#include <io.h>
#include <render/renderer.h>
#include <stdint.h>
#include <memory/Heap.h>

struct window_struct {
    int WindowID;
    uint_16 Width;
    uint_8 Height;
    uint_16 XLoc;
    uint_8 YLoc;
    char* title;
    uint_8 border_color;
    uint_8 inside_color;
};

int WindowCount;
int WindowBudget;
window_struct* WinArr;

void Init(){
    WindowBudget = 8;
    WindowCount = 0;
    WinArr = malloc(8 * sizeof(window_struct));
}

void CreateWin(int WinID,uint_16 xTo,uint_8 yTo, uint_16 width,uint_8 height, char* title,uint_8 border_color, uint_8 inside_color){
    // Create a window with the id "WinID" to yTo anf xTo on the screen with "width" width and "height" height and with the title "title"    
    window_struct win;
    win.WindowID = WinID;
    win.Width = width;
    win.Height = height;
    win.XLoc = xTo;
    win.YLoc = yTo;
    win.title = title;
    win.border_color = border_color;
    win.inside_color = inside_color;
    if (WindowCount <= WindowBudget){
        WinArr[WindowCount] = win;
        WindowCount++;
    }
    window(win.XLoc, win.YLoc, win.Width, win.Height, win.title,win.border_color,win.inside_color);
}

void MoveWin(int WinID, int xTo,int yTo){
    // Move the window with the id "WinID" to xTo and yTo on the screeen
    
}