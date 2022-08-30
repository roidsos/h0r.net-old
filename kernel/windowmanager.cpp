#include <io.h>
#include <render/renderer.h>
#include <stdint.h>
#include <memory/Heap.h>
#include <memory/memory.h>

struct window_struct
{
    int WindowID;
    uint_16 Width;
    uint_8 Height;
    uint_16 XLoc;
    uint_8 YLoc;
    char *title;
    uint_8 border_color;
    uint_8 inside_color;
};

int WindowCount;
int WindowBudget;
window_struct *WinArr;

void Init()
{
    WindowBudget = 8;
    WindowCount = 0;
    WinArr = malloc(8 * sizeof(window_struct));
}

void UpdateAndRender()
{
    // Render the Windows
    for (int i = 0; i < WindowCount; i++)
    {
        window_struct win = WinArr[i];
        window(win.XLoc, win.YLoc, win.Width, win.Height, win.title, win.border_color, win.inside_color);
    }
}

void CreateWin(int WinID, uint_16 xTo, uint_8 yTo, uint_16 width, uint_8 height, char *title, uint_8 border_color, uint_8 inside_color)
{
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
    if (WindowCount <= WindowBudget)
    {
        WinArr[WindowCount] = win;
        WindowCount++;
    }
    
}
void DeleteWin(int WinID)
{
    // Itt vagy?
    //  ja

    char *stuff = (char *)WinArr + (WinID * sizeof(window_struct));
    for (int i = 0; i < sizeof(window_struct); i++)
    {
        stuff[i] = 0; // deleting the data
    }
    // windowmanager.cpp:62:26: error: cannot convert 'window_struct*' to 'char*' in initialization
    memcpy(stuff, stuff + sizeof(window_struct), (WindowBudget - WinID) * sizeof(window_struct)); // making the data concistant pt 1

    for (int i = 0; i < WindowBudget - WinID; i++)
    {
        WinArr[WinID].WindowID -= 1; // making the data concistant pt 1
        // I think we are done with delete win
        // I think we need discord screenshare
        // createWin isnt working
        // ?
        //
    }
    Clearscr(0x0);
    UpdateAndRender();
}
void MoveWin(int WinID, int xTo, int yTo)
{
    // Move the window with the id "WinID" to xTo and yTo on the screeen
}