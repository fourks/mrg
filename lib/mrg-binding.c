/* mrg - MicroRaptor Gui
 * Copyright (c) 2014 Øyvind Kolås <pippin@hodefoting.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mrg-internal.h"

void mrg_add_binding (Mrg *mrg,
                      const char *key,
                      const char *action,
                      const char *label,
                      MrgCb cb,
                      void  *cb_data)
{
  mrg_add_binding_full (mrg, key, action, label, cb, cb_data, NULL, NULL);
}

void mrg_add_binding_full (Mrg *mrg,
                           const char *key,
                           const char *action,
                           const char *label,
                           MrgCb       cb,
                           void       *cb_data,
                           MrgDestroyNotify destroy_notify,
                           void       *destroy_data)
{
  if (mrg->n_bindings +1 >= MRG_MAX_BINDINGS)
  {
    fprintf (stderr, "warning: mrg binding overflow\n");
    return;
  }
  mrg->bindings[mrg->n_bindings].nick = strdup (key);
  strcpy (mrg->bindings[mrg->n_bindings].nick, key);

  if (action)
    mrg->bindings[mrg->n_bindings].command = action ? strdup (action) : NULL;
  if (label)
    mrg->bindings[mrg->n_bindings].label = label ? strdup (label) : NULL;
  mrg->bindings[mrg->n_bindings].cb = cb;
  mrg->bindings[mrg->n_bindings].cb_data = cb_data;
  mrg->bindings[mrg->n_bindings].destroy_notify = destroy_notify;
  mrg->bindings[mrg->n_bindings].destroy_data = destroy_data;
  mrg->n_bindings++;
}

void _mrg_bindings_key_down (MrgEvent *event, void *data1, void *data2)
{
  Mrg *mrg = event->mrg;
  int i;
  int handled = 0;

  for (i = mrg->n_bindings-1; i>=0; i--)
    if (!strcmp (mrg->bindings[i].nick, event->string))
    {
      if (mrg->bindings[i].cb)
      {
        mrg->bindings[i].cb (event, mrg->bindings[i].cb_data, NULL);
        if (event->stop_propagate)
          return;
        handled = 1;
      }
    }
  if (!handled)
  for (i = mrg->n_bindings-1; i>=0; i--)
    if (!strcmp (mrg->bindings[i].nick, "unhandled"))
    {
      if (mrg->bindings[i].cb)
      {
        mrg->bindings[i].cb (event, mrg->bindings[i].cb_data, NULL);
        if (event->stop_propagate)
          return;
      }
    }
}

void mrg_clear_bindings (Mrg *mrg)
{
  int i;
  for (i = 0; mrg->bindings[i].nick; i ++)
  {
    if (mrg->bindings[i].destroy_notify)
      mrg->bindings[i].destroy_notify (mrg->bindings[i].destroy_data);
    free (mrg->bindings[i].nick);
    if (mrg->bindings[i].command)
      free (mrg->bindings[i].command);
    if (mrg->bindings[i].label)
      free (mrg->bindings[i].label);
  }
  memset (&mrg->bindings, 0, sizeof (mrg->bindings));
  mrg->n_bindings = 0;
}

MrgBinding *mrg_get_bindings (Mrg *mrg)
{
  return &mrg->bindings[0];
}
