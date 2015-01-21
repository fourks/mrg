/*
 * Copyright (c) 2014 Øyvind Kolås <pippin@hodefoting.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "mrg.h"

static void ui (Mrg *mrg, void *data)
{
  mrg_stylesheet_clear (mrg);
  mrg_stylesheet_add (mrg, "p { color:blue; }", NULL, 1000, NULL);
  mrg_printf (mrg, "hello %s\n", data);
  mrg_start (mrg, "p", NULL);
  mrg_printf (mrg, "hello %s\n", data);
  mrg_set_style (mrg, "color:red");
  mrg_printf (mrg, "hello %s\n", data);
  mrg_end (mrg);
  mrg_printf (mrg, "hello %s\n", data);
  mrg_add_binding (mrg, "control-q", NULL, NULL, mrg_quit_cb, NULL);
}

int main (int argc, char **argv)
{
  Mrg *mrg = mrg_new (400, 300, NULL);
  mrg_set_ui (mrg, ui, argv[1]?argv[1]:"world");
  mrg_main (mrg);
  return 0;
}
