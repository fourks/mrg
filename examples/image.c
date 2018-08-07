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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mrg.h"

static void scroll_cb (MrgEvent *event, void *data1, void *data2)
{
  fprintf (stderr, "foo %s\n", event->scroll_direction?"down":"up");
}

static void ui (Mrg *mrg, void *data)
{
  mrg_image (mrg, 0, 10, 32, -1,    "hodefoting.png", NULL, NULL);
  mrg_image (mrg, 40, 10, 64, -1,   "hodefoting.png", NULL, NULL);
  mrg_image (mrg, 128, 10, -1, 64,  "hodefoting.png", NULL, NULL);
  mrg_image (mrg, 196, 10, -1, 128, "hodefoting.png", NULL, NULL);
  mrg_image (mrg, 10, 196, -1, -1, "hodefoting.png", NULL, NULL);

  mrg_text_listen (mrg, MRG_SCROLL, scroll_cb, NULL, NULL);
  mrg_print (mrg, "scroll me");
  mrg_text_listen_done (mrg);

  mrg_add_binding (mrg, "control-q", NULL, NULL, mrg_quit_cb, NULL);
}

int main (int argc, char **argv)
{
  Mrg *mrg = mrg_new (640, 480, NULL);
  //Mrg *mrg = mrg_new (-1, -1, NULL);
  mrg_set_ui (mrg, ui, NULL);
  mrg_main (mrg);
  return 0;
}
