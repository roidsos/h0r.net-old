#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "../limine/limine.h"

void InitScreen(struct limine_framebuffer* fb);
void RenderChar(char chr,int color);
void SetCursorpos(int pos);
void Backspace();

int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
#endif // __SCREEN_H__