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

#ifndef _uterus_H
#define _uterus_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <libk/stddef.h>
#include <libk/stdint.h>
#include <libk/stdbool.h>

#define uterus_MAX_ESC_VALUES 16

#define UTERUS_CB_BELL 20
#define UTERUS_CB_KBD_LEDS 60

#define UTERUS_OOB_OUTPUT_OCRNL (1 << 0)
#define UTERUS_OOB_OUTPUT_OFDEL (1 << 1)
#define UTERUS_OOB_OUTPUT_OFILL (1 << 2)
#define UTERUS_OOB_OUTPUT_OLCUC (1 << 3)
#define UTERUS_OOB_OUTPUT_ONLCR (1 << 4)
#define UTERUS_OOB_OUTPUT_ONLRET (1 << 5)
#define UTERUS_OOB_OUTPUT_ONOCR (1 << 6)
#define UTERUS_OOB_OUTPUT_OPOST (1 << 7)

struct uterus_context {
    /* internal use */

    usize tab_size;
    _bool autoflush;
    _bool cursor_enabled;
    _bool scroll_enabled;
    _bool control_sequence;
    _bool csi;
    _bool escape;
    _bool osc;
    _bool osc_escape;
    _bool rrr;
    _bool discard_next;
    _bool bold;
    _bool bg_bold;
    _bool reverse_video;
    _bool dec_private;
    _bool insert_mode;
    u64 code_point;
    usize unicode_remaining;
    u8 g_select;
    u8 charsets[2];
    usize current_charset;
    usize escape_offset;
    usize esc_values_i;
    usize saved_cursor_x;
    usize saved_cursor_y;
    usize current_primary;
    usize current_bg;
    usize scroll_top_margin;
    usize scroll_bottom_margin;
    u32 esc_values[uterus_MAX_ESC_VALUES];
    u64 oob_output;
    _bool saved_state_bold;
    _bool saved_state_bg_bold;
    _bool saved_state_reverse_video;
    usize saved_state_current_charset;
    usize saved_state_current_primary;
    usize saved_state_current_bg;

    /* to be set by backend */

    usize rows, cols;

    void (*raw_putchar)(struct uterus_context *, u8 c);
    void (*clear)(struct uterus_context *, _bool move);
    void (*set_cursor_pos)(struct uterus_context *, usize x, usize y);
    void (*get_cursor_pos)(struct uterus_context *, usize *x, usize *y);
    void (*set_text_fg)(struct uterus_context *, usize fg);
    void (*set_text_bg)(struct uterus_context *, usize bg);
    void (*set_text_fg_bright)(struct uterus_context *, usize fg);
    void (*set_text_bg_bright)(struct uterus_context *, usize bg);
    void (*set_text_fg_rgb)(struct uterus_context *, u32 fg);
    void (*set_text_bg_rgb)(struct uterus_context *, u32 bg);
    void (*set_text_fg_default)(struct uterus_context *);
    void (*set_text_bg_default)(struct uterus_context *);
    void (*set_text_fg_default_bright)(struct uterus_context *);
    void (*set_text_bg_default_bright)(struct uterus_context *);
    void (*move_character)(struct uterus_context *, usize new_x, usize new_y, usize old_x, usize old_y);
    void (*scroll)(struct uterus_context *);
    void (*revscroll)(struct uterus_context *);
    void (*swap_palette)(struct uterus_context *);
    void (*save_state)(struct uterus_context *);
    void (*restore_state)(struct uterus_context *);
    void (*double_buffer_flush)(struct uterus_context *);
    void (*full_refresh)(struct uterus_context *);

    /* to be set by client */

    void (*callback)(struct uterus_context *, u64, u64, u64, u64);
};

void uterus_context_reinit(struct uterus_context *ctx);
void uterus_write(struct uterus_context *ctx, const char *buf, usize count);

#ifdef __cplusplus
}
#endif

#endif
