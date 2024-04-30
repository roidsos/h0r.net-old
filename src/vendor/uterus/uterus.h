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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define UTERUS_MAX_ESC_VALUES 16

#define UTERUS_CB_BELL 20
#define UTERUS_CB_KBD_LEDS 60

struct uterus_context {
    /* internal use */

    size_t tab_size;
    bool cursor_enabled;
    bool scroll_enabled;
    bool control_sequence;
    bool csi;
    bool escape;
    bool osc;
    bool osc_escape;
    bool rrr;
    bool discard_next;
    bool dec_private;
    bool insert_mode;
    uint64_t code_point;
    size_t unicode_remaining;
    uint8_t g_select;
    uint8_t charsets[2];
    size_t current_charset;
    size_t escape_offset;
    size_t esc_values_i;
    size_t current_primary;
    size_t current_bg;
    size_t scroll_top_margin;
    size_t scroll_bottom_margin;
    uint32_t esc_values[UTERUS_MAX_ESC_VALUES];

    /* to be set by backend */

    size_t rows, cols;

    void (*raw_putchar)(struct uterus_context *, uint8_t c);
    void (*clear)(struct uterus_context *, bool move);
    void (*set_cursor_pos)(struct uterus_context *, size_t x, size_t y);
    void (*get_cursor_pos)(struct uterus_context *, size_t *x, size_t *y);
    void (*set_text_fg)(struct uterus_context *, size_t fg);
    void (*set_text_bg)(struct uterus_context *, size_t bg);
    void (*set_text_fg_rgb)(struct uterus_context *, uint32_t fg);
    void (*set_text_bg_rgb)(struct uterus_context *, uint32_t bg);
    void (*set_text_fg_default)(struct uterus_context *);
    void (*set_text_bg_default)(struct uterus_context *);
    void (*scroll)(struct uterus_context *);
    void (*double_buffer_flush)(struct uterus_context *);
    void (*full_refresh)(struct uterus_context *);

    /* to be set by client */

    void (*callback)(struct uterus_context *, uint64_t, uint64_t, uint64_t, uint64_t);
};

void uterus_context_reinit(struct uterus_context *ctx);
void uterus_write(struct uterus_context *ctx, const char *buf, size_t count);

#ifdef __cplusplus
}
#endif

#endif
