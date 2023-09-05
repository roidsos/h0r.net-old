#include "screen.h"
#include "vbe/font_renderer.h"

static struct limine_framebuffer *fb;

static int Cursorpos;

static int width;
static int height;

int get_offset(int col, int row) { return 2 * (row * width + col); }
int get_offset_row(int offset) { return offset / (2 * width); }
int get_offset_col(int offset) {
  return (offset - (get_offset_row(offset) * 2 * width)) / 2;
}

void InitScreen(struct limine_framebuffer *_fb) {
  Cursorpos = 0;
  fb = _fb;
  width = fb->width / CHAR_WIDTH;
  height = fb->height / CHAR_HEIGHT;

  InitFB(_fb);
}

void RenderChar(char chr, int color) {
  switch (chr) {
  case '\n':
    Cursorpos += width - (Cursorpos % width) - 1;
    break;

  default:;
    int x = (Cursorpos % width) * CHAR_WIDTH;
    int y = (Cursorpos / width) * CHAR_HEIGHT;
    renderChar(chr, x, y, 1, color);
    Cursorpos++;
  }
}

void SetCursorpos(int pos) { Cursorpos = pos; }

void Backspace() {
  Cursorpos--;
  int x = (Cursorpos % width) * CHAR_WIDTH;
  int y = (Cursorpos / width) * CHAR_HEIGHT;
  for (int _y = 0; _y < CHAR_HEIGHT; _y++) {
    for (int _x = 0; _x < CHAR_WIDTH; _x++) {
      int *fb_ptr = fb->address;
      fb_ptr[(y + _y) * (fb->pitch / 4) + (x + _x)] = 0;
    }
  }
}
