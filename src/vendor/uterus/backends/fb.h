/* Copyright (C) 2022-2023 mintsuki and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _uterus_FR_H
#define _UTERUS_FB_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <libk/stdint.h>
#include <libk/stddef.h>
#include <libk/stdbool.h>

#include "../uterus.h"

#define UTERUS_FB_FONT_GLYPHS 256

struct uterus_fb_char {
    u32 c;
    u32 fg;
    u32 bg;
};

struct uterus_fb_queue_item {
    usize x, y;
    struct uterus_fb_char c;
};

struct uterus_fb_context {
    struct uterus_context term;

    usize font_width;
    usize font_height;
    usize glyph_width;
    usize glyph_height;

    usize font_scale_x;
    usize font_scale_y;

    usize offset_x, offset_y;

    volatile u32 *framebuffer;
    usize pitch;
    usize width;
    usize height;
    usize bpp;

    usize font_bits_size;
    u8 *font_bits;

    u32 ansi_colours[8];
    u32 ansi_bright_colours[8];
    u32 default_fg, default_bg;
    u32 default_fg_bright, default_bg_bright;

#ifndef UTERUS_FB_DISABLE_CANVAS
    usize canvas_size;
    u32 *canvas;
#endif

    usize grid_size;
    usize queue_size;
    usize map_size;

    struct uterus_fb_char *grid;

    struct uterus_fb_queue_item *queue;
    usize queue_i;

    struct uterus_fb_queue_item **map;

    u32 text_fg;
    u32 text_bg;
    usize cursor_x;
    usize cursor_y;

    u32 saved_state_text_fg;
    u32 saved_state_text_bg;
    usize saved_state_cursor_x;
    usize saved_state_cursor_y;

    usize old_cursor_x;
    usize old_cursor_y;
};

struct uterus_context *uterus_fb_init(
    u32 *framebuffer, usize width, usize height, usize pitch,
#ifndef UTERUS_FB_DISABLE_CANVAS
    u32 *canvas,
#endif
    u32 *ansi_colours, u32 *ansi_bright_colours,
    u32 *default_bg, u32 *default_fg,
    u32 *default_bg_bright, u32 *default_fg_bright,
    void *font, usize font_width, usize font_height
);

#ifndef UTERUS_FB_DISABLE_BUMP_ALLOC
static inline struct uterus_context *uterus_fb_simple_init(
    u32 *framebuffer, usize width, usize height, usize pitch
) {
    return uterus_fb_init(
        framebuffer, width, height, pitch,
#ifndef UTERUS_FB_DISABLE_CANVAS
        NULL,
#endif
        NULL, NULL,
        NULL, NULL,
        NULL, NULL,
        NULL, 0, 0
    );
}
#endif

#ifdef __cplusplus
}
#endif

#endif
