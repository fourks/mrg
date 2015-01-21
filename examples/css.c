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

typedef struct State {
  float edge_left;
  float edge_right;
  float edge_top;
  float edge_bottom;
  char *string;
} State;


static void update_string (Mrg *mrg, char **string_loc, const char *new_string,
                           void *user_data)
{
  /* using user_data; different type of API hookups for updating the currently
   * edited string can be used here
   */
  free (*string_loc);
  *string_loc = strdup (new_string);
}

static int editable_press (MrgEvent *e, void *data1, void *data2)
{
  char **string = data1;
  mrg_queue_draw (e->mrg, NULL);
  mrg_edit_string (e->mrg, string, update_string, NULL);
  return 1;
}

static int drag_edge_left (MrgEvent *e, void *data1, void *data2)
{
  State *state = data1;
  state->edge_left += e->delta_x;
  mrg_queue_draw (e->mrg, NULL);
  return 1;
}

static int drag_edge_right (MrgEvent *e, void *data1, void *data2)
{
  State *state = data1;
  state->edge_right += e->delta_x;
  mrg_queue_draw (e->mrg, NULL);
  return 1;
}

static int drag_edge_top (MrgEvent *e, void *data1, void *data2)
{
  State *state = data1;
  state->edge_top += e->delta_y;
  mrg_queue_draw (e->mrg, NULL);
  return 1;
}

static int drag_edge_bottom (MrgEvent *e, void *data1, void *data2)
{
  State *state = data1;
  state->edge_bottom += e->delta_y;
  mrg_queue_draw (e->mrg, NULL);
  return 1;
}

static void ui (Mrg *mrg, void *data)
{
  State *state = data;
  float em = mrg_em (mrg);
  char *error = NULL;

  mrg_stylesheet_clear (mrg);
  mrg_stylesheet_add (mrg, state->string, NULL, 1000, &error);
  if (error)
  {
    fprintf (stderr, "%s\n", error);
    free (error);
  }

  mrg_set_edge_left   (mrg, state->edge_left);
  mrg_set_edge_right  (mrg, state->edge_right);
  mrg_set_edge_top    (mrg, state->edge_top);
  mrg_set_edge_bottom (mrg, state->edge_bottom);

  mrg_listen (mrg, MRG_DRAG_MOTION,
              state->edge_left - em, 0, 2 * em, mrg_height (mrg),
              drag_edge_left, state, NULL);

  mrg_listen (mrg, MRG_DRAG_MOTION,
              state->edge_right - em, 0, 2 * em, mrg_height (mrg),
              drag_edge_right, state, NULL);

  mrg_listen (mrg, MRG_DRAG_MOTION,
              0, state->edge_top - em, mrg_width(mrg), 2 * em,
              drag_edge_top, state, NULL);

  mrg_listen (mrg, MRG_DRAG_MOTION,
              0, state->edge_bottom - em, mrg_width(mrg), 2 * em,
              drag_edge_bottom, state, NULL);

  {
    mrg_text_listen (mrg, MRG_PRESS, editable_press, &state->string, NULL);
    mrg_print (mrg, state->string);
    mrg_text_listen_done (mrg);
  }

  {
    static char *string = NULL;
    if (!string)
      string = strdup ("foo");
    mrg_set_style (mrg, "font-size: 10");
    mrg_text_listen (mrg, MRG_PRESS, editable_press, &string, NULL);
    mrg_print (mrg, string);
    mrg_text_listen_done (mrg);
  }

#if MRG_CAIRO
  cairo_t *cr = mrg_cr (mrg);
    cairo_set_source_rgb (cr, 0, 0, 1);

    cairo_move_to (cr, state->edge_left, 0);
    cairo_line_to (cr, state->edge_left, mrg_height (mrg));
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    cairo_move_to (cr, state->edge_right, 0);
    cairo_line_to (cr, state->edge_right, mrg_height (mrg));
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    cairo_move_to (cr, 0, state->edge_top);
    cairo_line_to (cr, mrg_width (mrg), state->edge_top);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);

    cairo_move_to (cr, 0, state->edge_bottom);
    cairo_line_to (cr, mrg_width (mrg), state->edge_bottom);
    cairo_set_line_width (cr, 2.0);
    cairo_stroke (cr);
#endif

  mrg_start (mrg, "foo", NULL);
  mrg_print (mrg, "foo asdc asdmlkc amsdlkc asdmkcl asdkmc laskdmc alskdmc lsakd mcalskd mcalskd mcalsdkc masldkc masld kcmasldkc msaldkc msaldkc msaldkc msaldkc masldkc malsdkc masldkc masldkc masdlkc");
  mrg_end (mrg);

  mrg_start (mrg, "bar", NULL);
  mrg_print (mrg, "bar");
  mrg_end (mrg);

  mrg_add_binding (mrg, "control-q", NULL, NULL, mrg_quit_cb, NULL);
}

int main (int argc, char **argv)
{
  static State static_state = {-1,-1,-1,-1, NULL};
  State *state = &static_state;
  Mrg *mrg = mrg_new (640, 480, NULL);
  float em = mrg_em (mrg);
  state->edge_left = 2 * em;
  state->edge_top = 2 * em;
  state->edge_right = mrg_width (mrg) - 2 * em;
  state->edge_bottom = mrg_height (mrg) - 2 * em;
  state->string = strdup (
      
" foo , bar{\n"
"   stroke-color:white;\n"
"   border-width:2;\n"
"   display:block;\n"
"   top:40;\n"
"   width:60;\n"
"   height:70;\n"
"}\n"
" foo {left:300; } bar {left:50; } \n"
" bold * { font-weight:bold; } \n"
" dim * { opacity:0.5; } \n"
" underline *     { text-decoration:underline; }\n"
" reverse *,selected * { text-decoration:reverse; }\n"
" unhandled       { color:cyan; }\n"
" binding:key     { background-color:white; color:black; }\n"
" binding:label   { color:cyan; }\n"
" title           { color:red; font-size:13 }\n"
      );
  mrg_set_ui (mrg, ui, state);
  mrg_main (mrg);
  return 0;
}

#if 0
http://www.w3.org/TR/CSS2/sample.html

html, address,
blockquote,
body, dd, div,
dl, dt, fieldset, form,
frame, frameset,
h1, h2, h3, h4,
h5, h6, noframes,
ol, p, ul, center,
dir, hr, menu, pre   { display: block; unicode-bidi: embed }
li              { display: list-item }
head            { display: none }
table           { display: table }
tr              { display: table-row }
thead           { display: table-header-group }
tbody           { display: table-row-group }
tfoot           { display: table-footer-group }
col             { display: table-column }
colgroup        { display: table-column-group }
td, th          { display: table-cell }
caption         { display: table-caption }
th              { font-weight: bolder; text-align: center }
caption         { text-align: center }
body            { margin: 8px }
h1              { font-size: 2em; margin: .67em 0 }
h2              { font-size: 1.5em; margin: .75em 0 }
h3              { font-size: 1.17em; margin: .83em 0 }
h4, p,
blockquote, ul,
fieldset, form,
ol, dl, dir,
menu            { margin: 1.12em 0 }
h5              { font-size: .83em; margin: 1.5em 0 }
h6              { font-size: .75em; margin: 1.67em 0 }
h1, h2, h3, h4,
h5, h6, b,
strong          { font-weight: bolder }
blockquote      { margin-left: 40px; margin-right: 40px }
i, cite, em,
var, address    { font-style: italic }
pre, tt, code,
kbd, samp       { font-family: monospace }
pre             { white-space: pre }
button, textarea,
input, select   { display: inline-block }
big             { font-size: 1.17em }
small, sub, sup { font-size: .83em }
sub             { vertical-align: sub }
sup             { vertical-align: super }
table           { border-spacing: 2px; }
thead, tbody,
tfoot           { vertical-align: middle }
td, th, tr      { vertical-align: inherit }
s, strike, del  { text-decoration: line-through }
hr              { border: 1px inset }
ol, ul, dir,
menu, dd        { margin-left: 40px }
ol              { list-style-type: decimal }
ol ul, ul ol,
ul ul, ol ol    { margin-top: 0; margin-bottom: 0 }
u, ins          { text-decoration: underline }
br:before       { content: "\A"; white-space: pre-line }
center          { text-align: center }
:link, :visited { text-decoration: underline }
:focus          { outline: thin dotted invert }

#endif
