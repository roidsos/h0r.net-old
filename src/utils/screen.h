#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "../limine/limine.h"

void InitScreen(struct limine_framebuffer* fb);
void RenderChar(char chr,int color);
void SetCursorpos(int pos);
void Backspace();
#endif // __SCREEN_H__