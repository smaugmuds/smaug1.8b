/*
 * Change.c   (last update 3rd may, 2001)
 *
 * Allows adding and removing of changes while online,
 * also handles displaying the changes to the player.
 *
 * Would be a good thing to add to a players login,
 * but keep the MAX_CHANGE low in that case, so the player
 * doesn't get spammed to much.
 *
 * Code by Brian Graversen aka Jobo
 */

/*
 * Note, the code uses memory recycling, even though it's
 * probably a waste of time, since most muds won't add that
 * many changes in between each copyover. But I figured why
 * not make it right the first time around.
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#define MAX_CHANGE  15

CHANGE_DATA *change_list = NULL;
CHANGE_DATA *change_last = NULL;
CHANGE_DATA *change_free = NULL;

bool remove_change   args((int i));

/*
 * load_changes simply loads the list of changes,
 * should only be called at boot time. Add this
 * call to db.c's boot function.
 */
void load_changes()
{
  CHANGE_DATA *change;
  FILE *fp;
  char *name;

  if ((fp = fopen("../txt/changes.txt", "r")) == NULL)
  {
    log_string("Non-fatal error: changes.txt not found!");
    return;
  }
  name = fread_word(fp);
  while (str_cmp(name, END_MARKER))
  {
    change = alloc_perm(sizeof(CHANGE_DATA));
    change->imm  = str_dup(name);
    change->date = fread_string(fp);
    change->text = fread_string(fp);

    if (change_list)
      change_list->prev = change;
    change->next = change_list;
    change_list = change;

    if (!change_last) change_last = change;
    name = fread_word(fp);
  }

  fclose(fp);
}

/*
 * save_changes handles the storage of the change_list into
 * a file. It is called each time the list is changed.
 */
void save_changes()
{
  FILE *fp;
  CHANGE_DATA *change;
  int i = 0;

  if ((fp = fopen("../txt/changes.txt","w")) == NULL)
  {
    log_string("Error writing to changes.txt");
    return;
  }
  for (change = change_last; change; change = change->prev)
  {
    if (++i > MAX_CHANGE) break;
    fprintf(fp, "%s\n", change->imm);
    fprintf(fp, "%s~\n", change->date);
    fprintf(fp, "%s~\n", change->text);
  }
  fprintf(fp, "%s\n", END_MARKER);
  fclose(fp);
}

/*
 * This is the immortal function, which allows any
 * immortal with access to the function to add new
 * changes to the changes list. The immortals name
 * will also be added to the list, as well as the
 * date the change was added.
 */
void do_addchange( CHAR_DATA *ch, char *argument )
{
  CHANGE_DATA *change;
  CHANGE_DATA *cchange;
  char *strtime;
  char buf[50];
  int i;

  if (IS_NPC(ch)) return;

  /* we need something to add to the list */
  if (argument[0] == '\0' || strlen(argument) < 5)
  {
    send_to_char("What did you change?\n\r", ch);
    return;
  }
  /* Mainly to avoid that the list looks ugly */
  if (strlen(argument) > 60)
  {
    send_to_char("Keep it on 60 chars please.\n\r", ch);
    return;
  }

  /* Set the current time */
  strtime = ctime(&current_time);
  for (i = 0; i < 6; i++)
  {
    buf[i] = strtime[i + 4];
  }
  buf[6] = '\0';

  /* If we have a free change, we reuse it */
  if (change_free)
  {
    change = change_free;
    change_free = change_free->next;
    change->next = NULL;
    if (change_free) change_free->prev = NULL;
  }
  else
    change = alloc_perm(sizeof(*change));

  /* set the strings for the change */
  change->imm = str_dup(ch->name);
  change->text = str_dup(argument);
  change->date = str_dup(buf);

  /* If theres already a change, just add to the list */
  if (change_last)
  {
    change_last->next = change;
    change->prev = change_last;
    change->next = NULL;
    change_last = change;
  }
  else // there are no changes.
  {
    change->next = change_list;
    change_list = change;
    change_last = change;
  }

  /* Removing the oldest change if the list has gone beyond the max */
  cchange = change_list; i = 0;
  while ((cchange = cchange->next) != NULL) i++;
  if (i >= MAX_CHANGE)
    remove_change(1);

  send_to_char("Change added.\n\r", ch );  
  save_changes();
  return;
}

/*
 * The player function.
 * simply lists the last MAX_CHANGE changes
 */
void do_changes(CHAR_DATA *ch, char *argument)
{
  CHANGE_DATA *change;
  char buf[MAX_STRING_LENGTH];
  char tempbuf[MAX_STRING_LENGTH];
  bool found = FALSE;
  int i = 0;

  if (IS_NPC(ch)) return;

  sprintf(buf, " #R[#0***#R] #yMinor Code Changes and Quick Messages #R[#0***#R]#n\n\r\n\r");
  for (change = change_list; change; change = change->next)
  {
    found = TRUE;
    ++i;
    sprintf(tempbuf, " #R[#0%3d#R] #G%-6s  #L%-9s #C%s#n\n\r",
      i, change->date, change->imm, change->text);
    strcat(buf, tempbuf);
  }
  if (found) send_to_char(buf, ch);
  else send_to_char("No changes.\n\r", ch);
  return;
}

/*
 * delchange removes a given change, and adds the
 * change to a free_list so it can be recycled later
 */
void do_delchange(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  bool found = FALSE;
  int i;

  if (IS_NPC(ch)) return;

  one_argument(argument, arg);

  if ((i = atoi(arg)) < 1)
  {
    send_to_char("Which number change did you want to remove ?\n\r", ch);
    return;
  }
  found = remove_change(i);
  if (!found) send_to_char("No such change.\n\r", ch);
  else send_to_char("Change removed.\n\r", ch);
  save_changes();
  return;
}

/*
 * This function handles the actual removing of the change
 */
bool remove_change(int i)
{
  CHANGE_DATA *change;
  bool found = FALSE;

  for (change = change_list; change; change = change->next)
  {
    if (--i > 0) continue;
    found = TRUE;

    /* clearing out the strings */
    free_string(change->imm);
    free_string(change->text);
    free_string(change->date);

    /* update the pointer to the last change if needed */
    if (change == change_last) change_last = change->prev;

    /* handle the special case of the first change */
    if (change == change_list)
    {
      change_list = change->next;
      if (change->next) change->next->prev = NULL;
    }
    else
    {
      change->prev->next = change->next;
      if (change->next)
        change->next->prev = change->prev;
    }

    /* Handle the free list */
    change->next = change_free;
    change->prev = NULL;
    if (change_free) change_free->prev = change;
    change_free = change;

    /* terminate the loop */
    break;
  }

  /* did we remove anything ? */
  return found;
}
