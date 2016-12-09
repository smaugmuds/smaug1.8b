/********************************************************************************
 **    ( .-""-.   )+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+(   .-""-. )   **
 **     / _  _ \ ( | | | | | | |D|A|R|K| |R|E|A|L|M|S| | | | | | ) / _  _ \    **
 **     |(_\/_)|  )+-+-+-+-+-+ +-+-+-+-+-+-+-+-+-+-+-+ +-+-+-+-+(  |/_)(_\|    **
 **     (_ /\ _)           MULTIVERSE Distribution v1.0            (_ /\ _)    **
 **      |v==v| (       Multiverse source created by Exodus       ) |mmmm|     **
 **      '-..-'           with Aeone, Kain, Kyros, and Kast         '-..-'     **
 **----------------------------------------------------------------------------**
 ** SMAUG 1.4 written by : Thoric (Derek Snider) with Altrag, Blodkai,         **
 **                        Haus, Narn, Scryn, Swordbearer, Tricops, Gorog,     **
 **                        Rennard, Grishnakh, Fireblade and Nivek.            **
 **----------------------------------------------------------------------------**
 ** Original MERC 2.1 by : Hatchet, Furey, and Kahn.                           **
 ** Original Diku MUD by : Hans Staerfeldt, Katja Nyboe, Tom Madsen,           **
 **                        Michael Seifert & Sebastian Hammer.                 **
 **----------------------------------------------------------------------------**
 *			   Player Kingdoms header			        *
 ********************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "kallaikia.h"
#include "klkmod_reinos.h"



extern char * const	day_name[];
extern char * const	month_name[];

KINGDOM_SYSTEM	kgdmdata;
KINGDOM_DATA *	first_kingdom;
KINGDOM_DATA * 	last_kingdom;
KGDM_AREA *	first_karea;
KGDM_AREA *	last_karea;
TROOP_DATA *	first_troop;
TROOP_DATA * 	last_troop;
HERO_DATA * 	first_hero;
HERO_DATA * 	last_hero;

int      	maxNews;



/* local routines */
void    write_kingdom_list	args( ( void ) );
void    write_troop_list        args( ( void ) );
char * 	current_date   		args( ( void ) );
int    	num_news       		args( ( KINGDOM_DATA *kingdom ) );
bool    load_troop_file         args( ( const char *troopfile ) );
bool    load_kingdom_file	args( ( const char *kingdomfile ) );
// void 	save_member_list	args( ( MEMBER_LIST *members_list ) );
void    fread_troops            args( ( TROOP_DATA *troop, FILE *fp ) );
void    do_settroop		args( ( CHAR_DATA *ch, char *argument ) );
void    fread_kingdom		args( ( KINGDOM_DATA *kingdom, FILE *fp ) );
void 	kingdom_message		args( ( char *argument, KINGDOM_DATA *kingdom ) );



char *	const   troop_attributes [] = 
{ 
  "mercenary", "summon", "special", "undisbandable", "event", "noleave", "nobattle",
  "single", "limit_turn", "norecruit", "basic"
};


char *  const   troop_abilities [] = 
{ 
  "flying", "additional_strike", "swift", "large_shield", "pike", "regeneration",
  "endurance", "siege", "beauty", "steal_life", "marksmanship", "scales", "fear",
  "healing", "charm", "bursting_flame", "clumsiness", "accuracy", "recruit_speed",
  "weakness", "initiative", "attack", "counterattack", "hitpoint", "attacktype",
  "mana", "move", "prevent_damage", "destroy", "resurrect", "spell_immunity", "sleep",
  "attdef", "attack_resistance", "spell_resistance", "damage_self"
};


char *  const   hero_abilities [] = 
{ 
  "acid_dissolve", "alchemy", "animal_taming", "animate_dead", "arcfire", "backstabbing",
  "beastmaster", "berserk", "blessing", "bloodthirsty", "brewing", "building", "channeling",
  "charisma", "chill_touch", "confuse", "contract", "counterspell", "cursing", "demon_hunter",
  "dragon_rider", "dragonslayer", "dwarfbane", "earthquake", "enchant_item", "enchant_weapon",
  "firestorm", "firewall", "forcefield", "giantslayer", "healing", "honor", "iceshield", "icestorm",
  "icicle", "leadership", "lightning_flash", "lightningstorm", "magic_shield", "mindblast", "mirror",
  "necromancy", "orcslayer", "pacifist", "peace_with_nature", "plant_control", "polymorph", "quicksand",
  "raise_dead", "rally", "researching", "shepherding", "song", "soul_devour", "spell_lore", "spring_of_life",
  "squirrel_dance", "summoning", "tactics", "teleportation", "terror", "trading", "turn_undead",
  "unholy_aura", "valor", "vampire_hunter", "wall_of_force", "wall_of_thorn"
};



int get_troop_attrib_flag( char *flag )
{
  int x;

  for( x = 0; x < MAX_TROOP_ATTRIB; x++ )
  {
    if( !str_cmp(flag, troop_attributes[x]) )
      return x;
  }
  return -1;
}


int get_troop_ability_flag( char *flag )
{
  int x;

  for( x = 0; x < MAX_TROOP_ABILITY; x++ )
  {
    if( !str_cmp(flag, troop_abilities[x]) )
      return x;
  }
  return -1;
}


int get_troop_hero_ability_flag( char *flag )
{
  int x;

  for( x = 0; x < MAX_HERO_ABILITY; x++ )
  {
    if( !str_cmp(flag, hero_abilities[x]) )
      return x;
  }
  return -1;
}


// Helper function, didn't write most of these - needed to compile in stock
void stralloc_printf( char **pointer, char *fmt, ...)
{
  char buf[MAX_STRING_LENGTH*2];
  va_list args;

  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  STRFREE(*pointer);
  *pointer = STRALLOC(buf);
  return;
}

// Helper function, didn't write most of these - needed to compile in stock
void strdup_printf( char **pointer, char *fmt, ...)
{
  char buf[MAX_STRING_LENGTH*2];
  va_list args;

  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  DISPOSE(*pointer);
  *pointer = str_dup(buf);
  return;
}

// Helper function, didn't write most of these - needed to compile in stock
void nstralloc( char **pointer )
{
  if( !*pointer )
    *pointer = STRALLOC("");
  return;
}

// Helper function, didn't write most of these - needed to compile in stock
void nstrdup( char **pointer )
{
  if( !*pointer )
    *pointer = str_dup("");
  return;
}

// Helper function, didn't write most of these - needed to compile in stock
bool exists_file( char *name )
{
  struct stat fst;

  if( NULLSTR(name) || !str_cmp(name, "") )
    return FALSE;

  if( stat(name, &fst) != -1 )
    return TRUE;
  else
    return FALSE;
}

// Helper function, didn't write most of these - needed to compile in stock
void echo_all_printf(short AT_COLOR, short tar, char *Str, ... )
{
  va_list arg;
  char argument[MAX_STRING_LENGTH];

  if( NULLSTR(Str) )
    return;

  va_start(arg, Str);
  vsnprintf(argument, MAX_STRING_LENGTH, Str, arg);
  va_end(arg);
  echo_to_all(AT_COLOR, argument, tar);
  return;
}

// Helper function, didn't write most of these - needed to compile in stock
int get_race_by_name( char *argument)
{
  int x;

  if( is_number(argument) && npc_race[atoi(argument)] )
    return atoi(argument);

  for( x = 0; x < MAX_NPC_RACE; x++ )
  {
    if( npc_race[x] && !str_cmp(argument, npc_race[x]) )
      return x;
  }
  return -1;
}

// Helper function, didn't write most of these - needed to compile in stock
/* Remove suffix (last . in string and beyond) if any. */
void chopSuffix( char *s )
{
  char *e = strrchr(s, '.');

  if( e != NULL )
    *e = 0;
}

// Helper function, didn't write most of these - needed to compile in stock
size_t my_strftime( char *s, size_t max, const char  *fmt,  const struct tm *tme )
{
  return strftime(s, max, fmt, tme);
}

// Helper function, didn't write most of these - needed to compile in stock
char * current_date( void )
{
  static char buf[128];
  struct tm * datetime;

  datetime = localtime(&current_time);
  my_strftime(buf, sizeof(buf), "%x", datetime );
  return buf;
}


/* Get pointer to kingdom structure from kingdom name. */
KINGDOM_DATA *get_kingdom( char *name )
{
  KINGDOM_DATA *kingdom;

  for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
  {
    if( !str_cmp(name, kingdom->name) )
      return kingdom;
  }
  return NULL;
}


void write_kingdom_list( void )
{
  KINGDOM_DATA *tkingdom;
  FILE *fpout;
  char filename[256];

  snprintf(filename, 256, "%s%s", KINGDOM_DIR, KINGDOM_LIST );
  if( !(fpout = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    return;
  }

  for( tkingdom = first_kingdom; tkingdom; tkingdom = tkingdom->next )
    fprintf(fpout, "%s\n", tkingdom->filename );

  fprintf(fpout, "$\n" );
  FCLOSE(fpout);
}


/* Save a kingdoms's data to its data file */
void save_kingdom( KINGDOM_DATA *kingdom )
{
  FILE *fp;
  char filename[256];

  if( !kingdom )
  {
    bug( "%s: NULL kingdom!", __FUNCTION__ );
    return;
  }

  if( NULLSTR(kingdom->filename) )
  {
    bug( "%s: %s has no filename!", __FUNCTION__, kingdom->name );
    return;
  }

  snprintf(filename, 256, "%s%s", KINGDOM_DIR, kingdom->filename );
  if( !(fp = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    perror(filename);
  }
  else
  {
    fprintf(fp, "#KINGDOM\n" );
    fprintf(fp, "Name              %s~\n", kingdom->name           );
    fprintf(fp, "Filename          %s~\n", kingdom->filename       );
    fprintf(fp, "Race              %d\n",  kingdom->race           );
    fprintf(fp, "Whoname           %s~\n", kingdom->whoname        );
    fprintf(fp, "Motto             %s~\n", kingdom->motto          );
    fprintf(fp, "Description       %s~\n", kingdom->description    );
    fprintf(fp, "Deity             %s~\n", kingdom->deity          );
    fprintf(fp, "Ruler             %s~\n", kingdom->ruler          );
    fprintf(fp, "Number1           %s~\n", kingdom->number1        );
    fprintf(fp, "Number2           %s~\n", kingdom->number2        );
    fprintf(fp, "Number3           %s~\n", kingdom->number3        );
    fprintf(fp, "Number4           %s~\n", kingdom->number4        );
    fprintf(fp, "Number5           %s~\n", kingdom->number5        );
    fprintf(fp, "General           %s~\n", kingdom->general        );
    fprintf(fp, "Badge             %s~\n", kingdom->badge          );
    fprintf(fp, "Leadrank          %s~\n", kingdom->leadrank       );
    fprintf(fp, "Onerank           %s~\n", kingdom->onerank        );
    fprintf(fp, "Tworank           %s~\n", kingdom->tworank        );
    fprintf(fp, "Threerank         %s~\n", kingdom->threerank      );
    fprintf(fp, "Fourrank          %s~\n", kingdom->fourrank       );
    fprintf(fp, "Fiverank          %s~\n", kingdom->fiverank       );
    fprintf(fp, "Genrank           %s~\n", kingdom->genrank        );
    fprintf(fp, "Members           %d\n",  kingdom->members        );
    fprintf(fp, "URL               %s~\n", kingdom->url            );
    fprintf(fp, "Warwith           %s~\n", kingdom->warwith        );
    fprintf(fp, "Allywith          %s~\n", kingdom->allywith       );
    fprintf(fp, "Warkills          %d\n",  kingdom->warkills       );
    fprintf(fp, "Wkills            %d\n",  kingdom->wkills         );
    fprintf(fp, "Population        %d\n",  kingdom->population     );
    fprintf(fp, "Max_population    %d\n",  kingdom->max_population );
    fprintf(fp, "Gold              %d\n",  kingdom->gold           );
    fprintf(fp, "Wood              %d\n",  kingdom->wood           );
    fprintf(fp, "Stone             %d\n",  kingdom->stone          );
    fprintf(fp, "Food              %d\n",  kingdom->food           );
    fprintf(fp, "Curr_glory        %d\n",  kingdom->curr_glory     );
    fprintf(fp, "Max_glory         %d\n",  kingdom->max_glory      );
    fprintf(fp, "Gminers           %d\n",  kingdom->gminers        );
    fprintf(fp, "Sminers           %d\n",  kingdom->sminers        );
    fprintf(fp, "Farmers           %d\n",  kingdom->farmers        );
    fprintf(fp, "Woodcutters       %d\n",  kingdom->woodcutters    );
    fprintf(fp, "End\n\n"                                          );
    fprintf(fp, "#END\n"                                           );
  }

  FCLOSE(fp);
  return;
}


/* Read in actual kingdom data. */
void fread_kingdom( KINGDOM_DATA *kingdom, FILE *fp )
{
  const char *word;
  bool fMatch;

  for( ; ; )
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;
    switch(UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'A':
        KEY( "Allywith",    	kingdom->allywith,      fread_string(fp)	);
        break;

      case 'B':
        KEY( "Badge",       	kingdom->badge,         fread_string(fp)	);
        break;

      case 'C':
        KEY( "Curr_glory",  	kingdom->curr_glory,    fread_number(fp)	);
        break;

      case 'D':
        KEY( "Deity",       	kingdom->deity,		fread_string(fp) 	);
        KEY( "Description",	kingdom->description,	fread_string(fp) 	);
        break;

      case 'E':
        if( !str_cmp(word, "End" ) )
        {
          nstralloc(&kingdom->name);
          nstralloc(&kingdom->ruler);
          nstralloc(&kingdom->description);
          nstralloc(&kingdom->motto);
          nstralloc(&kingdom->number1);
          nstralloc(&kingdom->number2);
          nstralloc(&kingdom->number3);
          nstralloc(&kingdom->number4);
          nstralloc(&kingdom->number5);
          nstralloc(&kingdom->general);
          nstralloc(&kingdom->deity);
          nstralloc(&kingdom->leadrank);
          nstralloc(&kingdom->onerank);
          nstralloc(&kingdom->tworank);
          nstralloc(&kingdom->threerank);
          nstralloc(&kingdom->fourrank);
          nstralloc(&kingdom->fiverank);
          nstralloc(&kingdom->genrank);
          nstralloc(&kingdom->warwith);
          nstralloc(&kingdom->allywith);
          nstrdup(&kingdom->url);
          return;
        }
        break;

      case 'F':
        KEY( "Farmers",     	kingdom->farmers,	fread_number(fp) 	);
        KEY( "Filename",    	kingdom->filename,  	fread_string_nohash(fp) );
        KEY( "Fiverank",	kingdom->fiverank,	fread_string(fp) 	);
        KEY( "Food",        	kingdom->food,          fread_number(fp) 	);
        KEY( "Fourrank",	kingdom->fourrank,	fread_string(fp) 	);
        break;

      case 'G':
        KEY( "General",     	kingdom->general,       fread_string(fp) 	);
        KEY( "Genrank",     	kingdom->genrank,       fread_string(fp) 	);
        KEY( "Gminers",     	kingdom->gminers,       fread_number(fp) 	);
        KEY( "Gold",        	kingdom->gold,		fread_number(fp) 	);
        break;

      case 'L':
        KEY( "Leadrank",    	kingdom->leadrank,      fread_string(fp) );
        break;

      case 'M':
        KEY( "Max_glory",   	kingdom->max_glory,     fread_number(fp) 	);
        KEY( "Max_population", 	kingdom->max_population,fread_number(fp) 	);
        KEY( "Members",     	kingdom->members,       fread_number(fp) 	);
        KEY( "Motto",       	kingdom->motto,         fread_string(fp) 	);
        break;

      case 'N':
        KEY( "Name",        	kingdom->name,          fread_string(fp) 	);
        KEY( "Number1",     	kingdom->number1,       fread_string(fp) 	);
        KEY( "Number2",     	kingdom->number2,       fread_string(fp) 	);
        KEY( "Number3",     	kingdom->number3,       fread_string(fp) 	);
        KEY( "Number4",     	kingdom->number4,	fread_string(fp) 	);
        KEY( "Number5",     	kingdom->number5,       fread_string(fp) 	);
        break;

      case 'O':
        KEY( "Onerank",     	kingdom->onerank,       fread_string(fp) 	);
        break;

      case 'P':
        KEY( "Population",  	kingdom->population,    fread_number(fp) 	);
        break;

      case 'R':
        KEY( "Race",        	kingdom->race,          fread_number(fp) 	);
        KEY( "Ruler",       	kingdom->ruler,         fread_string(fp) 	);
        break;

      case 'S':
        KEY( "Sminers",     	kingdom->sminers,       fread_number(fp) 	);
        KEY( "Stone",       	kingdom->stone,         fread_number(fp) 	);
        break;

      case 'T':
        KEY( "Tworank",     	kingdom->tworank,       fread_string(fp) 	);
        KEY( "Threerank",   	kingdom->threerank,     fread_string(fp) 	);
        break;

      case 'U':
        KEY( "URL",         	kingdom->url,           fread_string_nohash(fp) );
        break;

      case 'W':
        KEY( "Warkills",    	kingdom->warkills,      fread_number(fp) 	);
        KEY( "Warwith",     	kingdom->warwith,       fread_string(fp) 	);
        KEY( "Whoname",     	kingdom->whoname,       fread_string(fp) 	);
        KEY( "Wkills",      	kingdom->wkills,        fread_number(fp) 	);
        KEY( "Wood",        	kingdom->wood,          fread_number(fp) 	);
        KEY( "Woodcutters", 	kingdom->woodcutters,   fread_number(fp) 	);
        break;
    }

    if( !fMatch )
      bug( "%s: No match: %s", __FUNCTION__, word );
  }
}


/* Load a kingdom file */
bool load_kingdom_file( const char *kingdomfile )
{
  char filename[256];
  KINGDOM_DATA *kingdom;
  FILE *fp;
  bool found = FALSE;

  CREATE( kingdom, KINGDOM_DATA, 1 );
  snprintf(filename, 256, "%s%s", KINGDOM_DIR, kingdomfile );
  if( (fp = fopen(filename, "r")) != NULL )
  {
    found = TRUE;
    for( ; ; )
    {
      char letter;
      const char *word;

      letter = fread_letter(fp);
      if( letter == '*' )
      {
        fread_to_eol(fp);
        continue;
      }

      if( letter != '#' )
      {
        bug( "%s: # not found.", __FUNCTION__ );
        break;
      }

      word = fread_word(fp);
      if( !str_cmp(word, "KINGDOM") )
      {
        fread_kingdom(kingdom, fp);
        break;
      }
      else if( !str_cmp(word, "END") )
        break;
      else
      {
        bug( "%s: bad section: %s.", __FUNCTION__, word );
        break;
      }
    }
    FCLOSE(fp);
  }

  if( found )
    LINK( kingdom, first_kingdom, last_kingdom, next, prev );
  else
    DISPOSE(kingdom);
  return found;
}


/* Load in all the kingdom files. */
void load_kingdoms( void )
{
  FILE *fpList;
  const char *filename;
  char kingdomlist[256];

  first_kingdom = NULL;
  last_kingdom = NULL;
  log_string( "Loading kingdoms..." );
  snprintf(kingdomlist, 256, "%s%s", KINGDOM_DIR, KINGDOM_LIST );
  if( !(fpList = fopen(kingdomlist, "r")) )
  {
    bug( "%s: Couldn't open %s for reading!", __FUNCTION__, kingdomlist );
    perror(kingdomlist);
    exit(1);
  }

  for( ; ; )
  {
    filename = feof(fpList) ? "$" : fread_word(fpList);
    if( filename[0] == '$' )
      break;

    if( !load_kingdom_file(filename) )
      bug( "Couldn't load kingdom file: %s", filename );
  }

  FCLOSE(fpList);
  return;
}


void do_setkingdom( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  if( arg1[0] == '\0' )
  {
    send_to_char( "Usage: setkingdom <kingdom> <field>\n\r", ch );
    send_to_char( "\n\rField being one of:\n\r", ch );
    send_to_char( " deity leader number1 number2 number3 number4 number5 general\n\r", ch );
    send_to_char( " leadrank onerank tworank threerank fourrank fiverank genrank members url\n\r", ch );
    if( get_trust(ch) >= LEVEL_GOD )
      send_to_char( " name filename motto desc race\n\r", ch );
    return;
  }

  kingdom = get_kingdom(arg1);
  if( !kingdom )
  {
    send_to_char( "No such kingdom.\n\r", ch );
    return;
  }

  if( !str_cmp(arg2, "deity") )
  {
    stralloc_printf(&kingdom->deity, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "leader") )
  {
    stralloc_printf(&kingdom->ruler, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "number1") )
  {
    stralloc_printf(&kingdom->number1, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "number2") )
  {
    stralloc_printf(&kingdom->number2, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "number3") )
  {
    stralloc_printf(&kingdom->number3, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "number4") )
  {
    stralloc_printf(&kingdom->number4, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "number5") )
  {
    stralloc_printf(&kingdom->number5, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "general") )
  {
    stralloc_printf(&kingdom->general, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "leadrank") )
  {
    stralloc_printf(&kingdom->leadrank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "onerank") )
  {
    stralloc_printf(&kingdom->onerank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "tworank") )
  {
    stralloc_printf(&kingdom->tworank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "threerank") )
  {
    stralloc_printf(&kingdom->threerank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "fourrank") )
  {
    stralloc_printf(&kingdom->fourrank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "fiverank") )
  {
    stralloc_printf(&kingdom->fiverank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "genrank") )
  {
    stralloc_printf(&kingdom->genrank, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "url") )
  {
    strdup_printf(&kingdom->url, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "members") )
  {
    kingdom->members = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( get_trust(ch) < LEVEL_GOD )
  {
    do_setkingdom(ch, "");
    return;
  }

  if( !str_cmp(arg2, "race") )
  {
    kingdom->race = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "name") )
  {
    stralloc_printf(&kingdom->name, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "filename") )
  {
    strdup_printf(&kingdom->filename, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    write_kingdom_list();
    return;
  }

  if( !str_cmp(arg2, "motto") )
  {
    stralloc_printf(&kingdom->motto, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "whoname") )
  {
    stralloc_printf(&kingdom->whoname, "%s", argument );
    send_to_char( "Done.\n\r", ch);
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg2, "desc") )
  {
    stralloc_printf(&kingdom->description, "%s", argument );
    send_to_char( "Done.\n\r", ch );
    save_kingdom(kingdom);
    return;
  }

  do_setkingdom(ch, "");
  return;
}


void do_showkingdom( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  if( argument[0] == '\0' )
  {
    send_to_char( "Usage: showkingdom <kingdom>\n\r", ch );
    return;
  }

  kingdom = get_kingdom(argument);
  if( !kingdom )
  {
    send_to_char( "No such kingdom.\n\r", ch );
    return;
  }

  ch_printf(ch, "\n\r&W%s\n\r&wFilename : &W%s\n\r&wMotto    : &W%s\n\r",
    kingdom->name, kingdom->filename, kingdom->motto );
  ch_printf(ch, "&wWhoname  : &R%s&R\n\r", kingdom->whoname );
  ch_printf(ch, "&wWebsite  : &B%s&W\n\r", kingdom->url ? kingdom->url : "(not set)" );
  ch_printf(ch, "&wDesc     : &W%s\n\r&wDeity    : &W%s\n\r", kingdom->description, kingdom->deity );
  ch_printf(ch, "&wLeader   : &W%-19.19s\t&wRank: &W%s\n\r", kingdom->ruler, kingdom->leadrank );
  ch_printf(ch, "&wNumber1  : &W%-19.19s\t&wRank: &W%s\n\r", kingdom->number1, kingdom->onerank );
  ch_printf(ch, "&wNumber2  : &W%-19.19s\t&wRank: &W%s\n\r", kingdom->number2, kingdom->tworank );
  ch_printf(ch, "&wNumber3  : &W%-19.19s\t&wRank: &W%s\n\r", kingdom->number3, kingdom->threerank );
  ch_printf(ch, "&wMembers  : &W%-6d  &wRace: &W%-6d", kingdom->members, kingdom->race);
  ch_printf(ch, "&wAt War With  : &R%s&W\n\r", kingdom->warwith);
  ch_printf(ch, "&wAllied With  : &Y%s&W\n\r", kingdom->allywith);
  send_to_char( "\n\r", ch );
  return;
}


void do_makekingdom( CHAR_DATA *ch, char *argument )
{
  char filename[256];
  char dirname[256];
  KINGDOM_DATA *kingdom;
  bool found = FALSE;

  if( NULLSTR(argument) )
  {
    send_to_char( "Usage: makekingdom <kingdom name>\n\r", ch );
    return;
  }

  if( strstr(argument, " ") )
  {
    send_to_char( "For filename purposes, please use a single keyword when using this command.\n\r", ch );
    send_to_char( "You may alter the kingdom name after it has been created.\n\r", ch );
    return;
  }

  for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
  {
    if( !str_cmp(argument, kingdom->name) )
    {
      found = TRUE;
      break;
    }
  }

  if( found )
  {
    send_to_char( "A kingdom with that name already exists!\n\r", ch );
    return;
  }

  snprintf(filename, 256, "%s/%s.kgdm", strlower(argument), strlower(argument) );
  snprintf(dirname, 256, "%s%s", KINGDOM_DIR, strlower(argument) );
  if( !exists_file(dirname) )
  {
    log_string_plus( "Creating directory...", LOG_NORMAL, LEVEL_IMMORTAL );
    snprintf(dirname, 256, "mkdir %s%s", KINGDOM_DIR, strlower(argument) );
    system(dirname);
  }

  CREATE( kingdom, KINGDOM_DATA, 1 );
  LINK( kingdom, first_kingdom, last_kingdom, next, prev );
  kingdom->name                = STRALLOC(argument);
  kingdom->filename            = str_dup(filename);
  kingdom->whoname             = STRALLOC(argument);
  kingdom->badge               = STRALLOC("");
  kingdom->motto               = STRALLOC("");
  kingdom->description         = STRALLOC("");
  kingdom->deity               = STRALLOC("");
  kingdom->ruler               = STRALLOC("");
  kingdom->number1             = STRALLOC("");
  kingdom->number2             = STRALLOC("");
  kingdom->number3             = STRALLOC("");
  kingdom->number4             = STRALLOC("");
  kingdom->number5             = STRALLOC("");
  kingdom->general             = STRALLOC("");
  kingdom->leadrank            = STRALLOC("");
  kingdom->onerank             = STRALLOC("");
  kingdom->tworank             = STRALLOC("");
  kingdom->threerank           = STRALLOC("");
  kingdom->fourrank            = STRALLOC("");
  kingdom->fiverank            = STRALLOC("");
  kingdom->genrank             = STRALLOC("");
  kingdom->url                 = str_dup("");
  kingdom->warwith             = STRALLOC("");
  kingdom->allywith            = STRALLOC("");
}


void do_kingdoms( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;
  int count = 0;

  if( argument[0] == '\0' )
  {
    send_to_char( "\n\rKingdom                   Deity          Leader         Race         Members\n\r______________________________________________________________________________\n\r\n\r", ch );
    for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
      ch_printf(ch, "%-25s %-14s %-14s %-16s %5d\n\r",
        kingdom->name, kingdom->deity, kingdom->ruler, race_table[kingdom->race]->race_name, kingdom->members );

    count++;
    if( !count )
      send_to_char( "There are no Kingdom currently formed.\n\r", ch );
    else
      send_to_char( "______________________________________________________________________________\n\r\n\rUse 'kingdoms <kingdom>' for more information\n\r", ch );
    return;
  }

  kingdom = get_kingdom(argument);
  if( !kingdom )
  {
    send_to_char( "No such Kingdom.\n\r", ch );
    return;
  }

  ch_printf(ch, "\n\r%s\n\r'%s'\n\r\n\r", kingdom->name, kingdom->motto );
  ch_printf(ch, "URL: %s\n\r", kingdom->url);
  ch_printf(ch, "&WAt War With: &R%s\n\r", kingdom->warwith);
  ch_printf(ch, "Deity:  %s\n\r%s :  %s\n\r%s :  %s\n\r%s :  %s\n\r%s :  %s\n\r%s :  %s\n\r%s :  %s\n\r",
    kingdom->deity, kingdom->leadrank, kingdom->ruler, kingdom->onerank, kingdom->number1,
    kingdom->tworank, kingdom->number2, kingdom->threerank, kingdom->number3, kingdom->fourrank,
    kingdom->number4, kingdom->fiverank, kingdom->number5 );

  if( !str_cmp(ch->name, kingdom->deity)
  ||  !str_cmp(ch->name, kingdom->ruler)
  ||  !str_cmp(ch->name, kingdom->number1)
  ||  !str_cmp(ch->name, kingdom->number2)
  ||  !str_cmp(ch->name, kingdom->number3) )
    ch_printf(ch, "Members    :  %d\n\r", kingdom->members );

  ch_printf(ch, "\n\rDescription:\n\r%s\n\r", kingdom->description );
  return;
}


void do_fealty( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  kingdom = ch->pcdata->kingdom;
  if( !kingdom )
  {
    kingdom = get_kingdom(ch->pcdata->kingdom_name);
    if( !kingdom )
    {
      send_to_char( "Que dis?\n\r", ch );
      return;
    }
  }

  if( (ch->pcdata && ch->pcdata->bestowments && is_name("fealty", ch->pcdata->bestowments))
  ||  !str_cmp(ch->name, kingdom->deity)
  ||  !str_cmp(ch->name, kingdom->ruler)
  ||  !str_cmp(ch->name, kingdom->number1)
  ||  !str_cmp(ch->name, kingdom->number2)
  ||  !str_cmp(ch->name, kingdom->number3) )
    ;
  else
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg);
  if( arg[0] == '\0' )
  {
    send_to_char( "Who will you allow to swear fealty to your kingdom?\n\r", ch );
    return;
  }

  victim = get_char_room(ch, arg);
  if( !victim )
  {
    send_to_char( "That player is not here.\n\r", ch);
    return;
  }

  if( IS_NPC(victim) )
  {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
  }

  if( IS_IMMORTAL(victim) )
  {
    send_to_char( "Umm...no.\n\r", ch );
    return;
  }

  if( victim->level < 10 )
  {
    send_to_char( "This player is not worthy of joining yet.\n\r", ch );
    return;
  }

  if( victim->pcdata->kingdom )
  {
    if( victim->pcdata->kingdom == kingdom )
      send_to_char( "This player already belongs to your kingdom!\n\r", ch );
    else
      send_to_char( "This player belongs to another kingdom!\n\r", ch );
    return;
  }

  kingdom->members++;
  victim->pcdata->kingdom = kingdom;
  if( victim->pcdata->kingdom_name != NULL )
    STRFREE(victim->pcdata->kingdom_name);

  victim->pcdata->kingdom_name = QUICKLINK(kingdom->name);
  act( AT_MAGIC, "&BYou allow &R$N &Bto swear fealty to the kingdom of &W$t", ch, kingdom->name, victim, TO_CHAR );
  act( AT_MAGIC, "&R$N &Bkneels before &R$n &Band swears fealty to the kingdom of &W$t.", ch, kingdom->name, victim, TO_NOTVICT );
  act( AT_MAGIC, "&BYou kneel before &R$n &Band swear fealty to the kingdom of &W$t", ch, kingdom->name, victim, TO_VICT );
  save_char_obj(victim);
  save_kingdom(kingdom);
  return;
}


void do_exile( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  kingdom = ch->pcdata->kingdom;
  if( !kingdom )
  {
    kingdom = get_kingdom(ch->pcdata->kingdom_name);
    if( !kingdom )
    {
      send_to_char( "Que dis?\n\r", ch );
      return;
    }
  }

  if( (ch->pcdata && ch->pcdata->bestowments && is_name("exile", ch->pcdata->bestowments))
  ||  !str_cmp(ch->name, kingdom->deity)
  ||  !str_cmp(ch->name, kingdom->ruler)
  ||  !str_cmp(ch->name, kingdom->number1)
  ||  !str_cmp(ch->name, kingdom->number2)
  ||  !str_cmp(ch->name, kingdom->number3) )
    ;
  else
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg);
  if( arg[0] == '\0' )
  {
    send_to_char( "Exile whom?\n\r", ch );
    return;
  }

  victim = get_char_room(ch, arg);
  if( !victim )
  {
    send_to_char( "That player is not here.\n\r", ch);
    return;
  }

  if( IS_NPC(victim) )
  {
    send_to_char( "Not on NPC's.\n\r", ch );
    return;
  }

  if( victim == ch )
  {
    send_to_char( "Try abdicate instead.\n\r", ch );
    return;
  }

  if( victim->pcdata->kingdom != ch->pcdata->kingdom )
  {
    send_to_char( "You have no dominion over that player!\n\r", ch );
    return;
  }

  --kingdom->members;
  if( !str_cmp(victim->name, ch->pcdata->kingdom->number1) )
    stralloc_printf(&ch->pcdata->kingdom->number1, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->number2) )
    stralloc_printf(&ch->pcdata->kingdom->number2, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->number3) )
    stralloc_printf(&ch->pcdata->kingdom->number3, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->number4) )
    stralloc_printf(&ch->pcdata->kingdom->number4, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->number5) )
    stralloc_printf(&ch->pcdata->kingdom->number5, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->general) )
    stralloc_printf(&ch->pcdata->kingdom->general, "%s", "" );

  if( !str_cmp(victim->name, ch->pcdata->kingdom->deity) )
    stralloc_printf(&ch->pcdata->kingdom->deity, "%s", "" );

  victim->pcdata->kingdom = NULL;
  stralloc_printf(&victim->pcdata->kingdom_name, "%s", "" );
  act( AT_MAGIC, "You exile $N from the kingdom of &R$t", ch, kingdom->name, victim, TO_CHAR );
  act( AT_MAGIC, "$n exiles $N from the kingdom of &R$t", ch, kingdom->name, victim, TO_ROOM );
  act( AT_MAGIC, "$n exiles you from the kingdom of &R$t", ch, kingdom->name, victim, TO_VICT );
  echo_all_printf( AT_MAGIC, ECHOTAR_ALL, "&B[&WKingdom&B] &w%s has been Exiled from the kingdom of %s!", victim->name, kingdom->name );
  save_char_obj(victim);
  save_kingdom(kingdom);
  return;
}


void do_abdicate( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) || !ch->pcdata->kingdom )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  kingdom = ch->pcdata->kingdom;
  --kingdom->members;
  if( !str_cmp(ch->name, ch->pcdata->kingdom->number1) )
    stralloc_printf(&ch->pcdata->kingdom->number1, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->number2) )
    stralloc_printf(&ch->pcdata->kingdom->number2, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->number3) )
    stralloc_printf(&ch->pcdata->kingdom->number3, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->number4) )
    stralloc_printf(&ch->pcdata->kingdom->number4, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->number5) )
    stralloc_printf(&ch->pcdata->kingdom->number5, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->general) )
    stralloc_printf(&ch->pcdata->kingdom->general, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->deity) )
    stralloc_printf(&ch->pcdata->kingdom->deity, "%s", "" );

  if( !str_cmp(ch->name, ch->pcdata->kingdom->ruler) )
    stralloc_printf(&ch->pcdata->kingdom->ruler, "%s", "" );
   
  ch->pcdata->kingdom = NULL;
  stralloc_printf(&ch->pcdata->kingdom_name, "%s", "" );
  act( AT_MAGIC, "&BYou abidcate from from the kingdom of &W$t", ch, kingdom->name, NULL, TO_CHAR );
  act( AT_MAGIC, "&R$n &Babdicates from the kingdom of &W$t", ch, kingdom->name, NULL, TO_ROOM );
  echo_all_printf(AT_MAGIC, ECHOTAR_ALL, "&B[&WKingdom&B] &w%s has abdicated from the kingdom of %s!", ch->name, kingdom->name );
  save_char_obj(ch);
  save_kingdom(kingdom);
  return;
}


void do_kwars( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;
  int count = 0;

  send_to_char( "\n\r&r__________________________________________________________________________________\n\r\n\r", ch );
  send_to_char( "&r|&WKingdom                         At War With               War Kills  Total Kills &r|\n\r", ch );
  send_to_char( "&r__________________________________________________________________________________\n\r\n\r", ch );
  for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
  {
    ch_printf(ch, "&r|&z%-30s &R%-28s &w%-10d %-9d&r|\n\r", kingdom->name, kingdom->warwith, kingdom->warkills, kingdom->wkills );
    count++;
  }

  if( !count )
    send_to_char( "There are no Kingdom currently formed.\n\r", ch );
  else
    send_to_char( "&r__________________________________________________________________________________\n\r\n\r", ch );
  return;
}


void do_kallies( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;
  int count = 0;

  send_to_char( "\n\r&r_____________________________________________________________\n\r\n\r", ch );
  send_to_char( "&r|&WKingdom                         Declared as Ally            &r|\n\r", ch );
  send_to_char( "&r_____________________________________________________________\n\r\n\r", ch );
  for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
  {
    ch_printf(ch, "&r|&Z%-30s &Y%-28s&r|\n\r", kingdom->name, kingdom->allywith );
    count++;
  }

  if( !count )
    send_to_char( "There are no Kingdom currently formed.\n\r", ch );
  else
    send_to_char( "&r_____________________________________________________________\n\r\n\r", ch );
  return;
}


char * get_kingdom_size( int population )
{
  if( population == 0 )    		return "Barren";
  else if( population > 0 )    		return "Outpost";
  else if( population > 100 )    	return "Tribe";
  else if( population > 250 )    	return "Hamlet";
  else if( population > 500 )    	return "Village";
  else if( population > 1000 )    	return "Town";
  else if( population > 2500 )    	return "Small City";
  else if( population > 10000 )    	return "City";
  else if( population > 20000 )    	return "Large City";
  else if( population > 50000 )    	return "Metropolis";
  else if( population > 100000 )    	return "Kingdom";
  return "Uknown";
}


int get_kpower( KINGDOM_DATA *kingdom )
{
  int mod = 0;

  mod += kingdom->max_population;
  mod -= (kingdom->max_population - kingdom->population);
  mod += (kingdom->gold / 8);
  mod += (kingdom->wood / 4);
  mod += (kingdom->food / 6);
  mod += (kingdom->stone / 2);

  return URANGE(0, mod, 2097152000);
}


char * get_krank( KINGDOM_DATA *kingdom )
{
  static char buf[20];
  int x, cnt = 0;
  KINGDOM_DATA *kgdm;
  char prefix[5];

  for( kgdm = first_kingdom; kgdm; kgdm = kgdm->next )
    cnt++;

  if( cnt <= 1 )
    return "1st";

  x = cnt;
  for( kgdm = first_kingdom; kgdm; kgdm = kgdm->next )
  {
    if( get_kpower(kingdom) > get_kpower(kgdm) )
      x -= 1;

    if( x < 1 )
      x = 1;
  }

  switch(x)
  {
    case 1: snprintf(prefix, 5, "st" ); break;
    case 2: snprintf(prefix, 5, "nd" ); break;
    case 3: snprintf(prefix, 5, "rd" ); break;
    case 4: 
    case 5: 
    case 6: 
    case 7: 
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20: snprintf(prefix, 5, "th" ); break;
  }
  snprintf(buf, 20, "%d%s", x, prefix );
  return buf;
}


void show_kingdom_status( CHAR_DATA *ch )
{
  KINGDOM_DATA *kingdom;
  short rank = 0;
  char buf[MAX_STRING_LENGTH];
  int x = 0;
  int a = 0;


  if( IS_NPC(ch) )
  {
    send_to_char( "Nope.\n\r", ch );
    return;
  }

  if( !ch->pcdata->kingdom || ch->pcdata->kingdom_name[0] == '\0' )
  {
    send_to_char( "You don't belong to any kingdom.\n\r", ch );
    return;
  }

  kingdom = get_kingdom(ch->pcdata->kingdom_name);
  if( !kingdom )
  {
    send_to_char( "Somehow you belong to a non-existant kingdom. Alerting Immortals.\n\r", ch );
    bug( "%s %s belongs to a non-existant kingdom! (%s)\n\rFixing...", __FUNCTION__, ch->name, ch->pcdata->kingdom_name );
    stralloc_printf(&ch->pcdata->kingdom_name, "%s", "" );
    ch->pcdata->kingdom = NULL;
    return;
  }

  /* That should be everything we need to validate the existance of the kingdom.
   * Next, we validate who can view what. Anyone in the kingdom can view the
   * general status, but only ruler through number5 can view certain information.
   */
  if( !str_cmp(kingdom->ruler, ch->name) )      rank = 5;
  if( !str_cmp(kingdom->number1, ch->name) )    rank = 4;
  if( !str_cmp(kingdom->number3, ch->name) )    rank = 3;
  if( !str_cmp(kingdom->number4, ch->name) )    rank = 2;
  if( !str_cmp(kingdom->number5, ch->name) )	rank = 1;
    
  ch_printf(ch, "&CControl Model for the Kingdom of %s:\n\r", ch->pcdata->kingdom_name );
  send_to_char( "-------------------------------------------------------------\n\r", ch );
  send_to_char( " &B___________________________________________________________________________________\n\r", ch );
  send_to_char( "&B|                &ROVERALL		  &B|                  &RLEADERS                &B|\n\r", ch );

  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->ruler );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  ch_printf(ch, "&B| &CPopulation: &W%10s &CSize: &W%-10s &B| &CLeader  : &W%s %13s &B|\n\r",
    num_punct(kingdom->population), get_kingdom_size(kingdom->population), buf,
    !NULLSTR(kingdom->leadrank) ? kingdom->leadrank : "" );
  ch_printf(ch, "&B| &CPower: &W%13s   &CRank: &W%4s       &B|                                         |\n\r",
    num_punct(get_kpower(kingdom)), get_krank(kingdom) );
  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->number1 );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  ch_printf(ch, "&B| &CGlory:  &W%7s                         &B| &CNumber 1: &W%s %13s &B|\n\r",
    num_punct(kingdom->curr_glory), buf, !NULLSTR(kingdom->number1) ? kingdom->onerank : "" );
  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->number2 );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  ch_printf(ch, "&B|                                         &B| &CNumber 2: &W%s %13s &B|\n\r",
    buf, !NULLSTR(kingdom->number2) ? kingdom->tworank : "" );
  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->number3 );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  ch_printf(ch, "&B| &RResources                               &B| &CNumber 3: &W%s %13s &B|\n\r",
    buf, !NULLSTR(kingdom->number3) ? kingdom->threerank : "" );
  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->number4 );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  a = (kingdom->gminers/KGDM_GPD);
  ch_printf(ch, "&B| &YGold  &C: &B[&W%13s&B] &C(&W%7d&w/day&C)   &B| &CNumber 4: &W%s %13s &B|\n\r",
    num_punct(kingdom->gold), a, buf, !NULLSTR(kingdom->number4) ? kingdom->fourrank : "" );
  snprintf(buf, MAX_STRING_LENGTH, "%s,", kingdom->number5 );
  if( strlen(buf) < 15 )
  {
    for( x = strlen(buf); x < 15; x++ )
      mudstrlcat(buf, " ", MAX_STRING_LENGTH);
  }

  a = (kingdom->gminers/KGDM_FPD);
  ch_printf(ch, "&B| &OFood  &C: &B[&W%13s&B] &C(&W%7d&w/day&C)   &B| &CNumber 5: &W%s %13s &B|\n\r",
    num_punct(kingdom->food), a,
    buf, !NULLSTR(kingdom->number5) ? kingdom->fiverank : "" );
  a = (kingdom->gminers/KGDM_SPD);
  ch_printf(ch, "&B| &wStone &C: &B[&W%13s&B] &C(&W%7d&w/day&C)   &B|                                         |\n\r",
    num_punct(kingdom->stone), a );
  a = (kingdom->gminers/KGDM_WPD);
  ch_printf(ch, "&B| &GLumber&C: &B[&W%13s&B] &C(&W%7d&w/day&C)   &B| &CNumber of players with Nobility: &W%3d    &B|\n\r",
    num_punct(kingdom->wood), a, 0 );
  send_to_char( "|_________________________________________|_________________________________________|&W\n\r", ch );
  return;
}


/* Get pointer to troop structure from troop name. */
TROOP_DATA *get_troop( char *name )
{
  TROOP_DATA *troop;

  for( troop = first_troop; troop; troop = troop->next )
  {
    if( !str_cmp(name, troop->name) )
      return troop;
  }
  return NULL;
}


void write_troop_list( void )
{
  TROOP_DATA *troop;
  FILE *fpout;
  char filename[256];

  snprintf(filename, 256, "%s%s", TROOP_DIR, TROOP_LIST );
  if( !(fpout = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    return;
  }

  for( troop = first_troop; troop; troop = troop->next )
    fprintf(fpout, "%s\n", troop->filename );

  fprintf(fpout, "$\n" );
  FCLOSE(fpout);
}


/* Save a troop's data to its data file */
void save_troop( TROOP_DATA *troop )
{
  FILE *fp;
  char filename[256];
  int x;

  if( !troop )
  {
    bug( "%s: NULL troop!", __FUNCTION__ );
    return;
  }

  if( NULLSTR(troop->filename) )
  {
    bug( "%s: %s has no filename", __FUNCTION__, troop->name );
    return;
  }

  snprintf(filename, 256, "%s%s", TROOP_DIR, troop->filename );
  if( !(fp = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    perror(filename);
  }
  else
  {
    fprintf(fp, "#TROOP\n" );
    fprintf(fp, "Name              %s~\n", troop->name             );
    fprintf(fp, "Filename          %s~\n", troop->filename         );
    fprintf(fp, "Magic_specialty   %d\n",  troop->magic_specialty  );
    fprintf(fp, "Race              %d\n",  troop->race             );
    if( !xIS_EMPTY( troop->attribute ) )
      fprintf(fp, "Attributes      %s\n",  print_bitvector(&troop->attribute) );
    fprintf(fp, "Power_rank        %d\n",  troop->power_rank       );
    fprintf(fp, "Attack_power      %d\n",  troop->attack_power     );
    fprintf(fp, "Xtra_attack_power %d\n",  troop->extra_attack_power );
    fprintf(fp, "Attack_type       %d\n",  troop->attack_type      );
    fprintf(fp, "Xtra_attack_type  %d\n",  troop->extra_attack_type );
    fprintf(fp, "Damtype           %d\n",  troop->damtype          );
    fprintf(fp, "Xtra_damtype      %d\n",  troop->extra_damtype    );
    fprintf(fp, "Attack_init       %d\n",  troop->attack_initiative );
    fprintf(fp, "Xtra_attack_init  %d\n",  troop->extra_attack_initiative );
    fprintf(fp, "Counter_attack    %d\n",  troop->counter_attack   );
    fprintf(fp, "Hit               %d\n",  troop->hit              );
    fprintf(fp, "Mana              %d\n",  troop->mana             );
    fprintf(fp, "Move              %d\n",  troop->move             );
    fprintf(fp, "Recruit_cost      %d\n",  troop->recruit_cost     );
    fprintf(fp, "Upkeep            " );
    for( x = 0; x < UPKEEP_MAX; x++ )
      fprintf(fp, "%d ", troop->upkeep[x] );
    fprintf(fp, "\n" );
    fprintf(fp, "Spell_resistance  %d %d %d %d %d %d\n", troop->spell_resistance[0],
      troop->spell_resistance[1], troop->spell_resistance[2], troop->spell_resistance[3],
      troop->spell_resistance[4], troop->spell_resistance[5] );
    fprintf(fp, "Attack_resistance %d %d %d %d %d %d %d %d %d %d %d %d\n", troop->attack_resistance[0], 
      troop->attack_resistance[1], troop->attack_resistance[2], troop->attack_resistance[3], 
      troop->attack_resistance[4], troop->attack_resistance[5], troop->attack_resistance[6], 
      troop->attack_resistance[7], troop->attack_resistance[8], troop->attack_resistance[9], 
      troop->attack_resistance[10], troop->attack_resistance[11] );
    fprintf(fp, "Abilities         %s\n",  print_bitvector(&troop->ability) );
    fprintf(fp, "Recruit_speed     %d\n",  troop->recruit_speed    );
    fprintf(fp, "End\n\n"                                          );
    fprintf(fp, "#END\n"                                           );
  }

  FCLOSE(fp);
  return;
}


/* Read in actual troop data. */
void fread_troop( TROOP_DATA *troop, FILE *fp )
{
  const char *word;
  bool fMatch;
  char *line;
  int x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12;

  for( ; ; )
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;
    switch(UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'A':
        KEY( "Abilities",   	troop->ability,         	fread_bitvector(fp)	);
        KEY( "Attack_init",	troop->attack_initiative, 	fread_number(fp) 	);
        KEY( "Attack_power",	troop->attack_power,    	fread_number(fp) 	);
        KEY( "Attack_type", 	troop->attack_type,     	fread_number(fp) 	);
        if( !str_cmp(word, "Attack_resistance") )
        {
          line = fread_line(fp);
          sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d",
            &x1, &x2, &x3, &x4, &x5, &x6, &x7, &x8, &x9, &x10, &x11, &x12 );
          troop->attack_resistance[0] = x1;
          troop->attack_resistance[1] = x2;
          troop->attack_resistance[2] = x3;
          troop->attack_resistance[3] = x4;
          troop->attack_resistance[4] = x5;
          troop->attack_resistance[5] = x6;
          troop->attack_resistance[6] = x7;
          troop->attack_resistance[7] = x8;
          troop->attack_resistance[8] = x9;
          troop->attack_resistance[9] = x10;
          troop->attack_resistance[10] = x11;
          troop->attack_resistance[11] = x12;
          fMatch = TRUE;
          break;
        }

        KEY( "Attributes",  	troop->attribute,       	fread_bitvector(fp) 	);
        break;

      case 'C':
        KEY( "Counter_attack",	troop->counter_attack,		fread_number(fp) 	);
        break;

      case 'D':
        KEY( "Damtype",     	troop->damtype,         	fread_number(fp) 	);
        break;

      case 'E':
        if( !str_cmp(word, "End") )
        {
          nstralloc(&troop->name);
          return;
        }
        break;

      case 'F':
        KEY( "Filename",    	troop->filename,  		fread_string_nohash(fp)	);
        break;

      case 'H':
        KEY( "Hit",         	troop->hit,             	fread_number(fp) 	);
        break;

      case 'M':
        KEY( "Magic_specialty",	troop->magic_specialty, 	fread_number(fp) 	);
        KEY( "Mana",        	troop->mana,            	fread_number(fp) 	);
        KEY( "Move",        	troop->move,            	fread_number(fp) 	);
        break;

      case 'N':
        KEY( "Name",		troop->name,			fread_string(fp) 	);
	break;

      case 'P':
        KEY( "Power_rank",  	troop->power_rank,      	fread_number(fp) 	);
        break;

      case 'R':
        KEY( "Race",        	troop->race,            	fread_number(fp) 	);
        KEY( "Recruit_cost",	troop->recruit_cost,    	fread_number(fp) 	);
        KEY( "Recruit_speed",	troop->recruit_speed,  		fread_number(fp) 	);
        break;

      case 'S':
        if( !str_cmp(word, "Spell_resistance") )
        {
          line = fread_line(fp);
          sscanf(line, "%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6 );
          troop->spell_resistance[0] = x1;
          troop->spell_resistance[1] = x2;
          troop->spell_resistance[2] = x3;
          troop->spell_resistance[3] = x4;
          troop->spell_resistance[4] = x5;
          troop->spell_resistance[5] = x6;
          fMatch = TRUE;
          break;
        }
        break;

      case 'U':
        if( !str_cmp(word, "Upkeep") )
        {
          line = fread_line(fp);
          sscanf(line, "%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6 );
          troop->upkeep[0] = x1;
          troop->upkeep[1] = x2;
          troop->upkeep[2] = x3;
          troop->upkeep[3] = x4;
          troop->upkeep[4] = x5;
          troop->upkeep[5] = x6;
          fMatch = TRUE;
          break;
        }
        break;

      case 'X':
        KEY( "Xtra_attack_init",    troop->extra_attack_initiative, fread_number(fp) 	);
        KEY( "Xtra_attack_power",   troop->extra_attack_power,      fread_number(fp) 	);
        KEY( "Xtra_attack_type",    troop->extra_attack_type,       fread_number(fp) 	);
        KEY( "Xtra_damtype",        troop->extra_damtype,           fread_number(fp) 	);
        break;
    }

    if( !fMatch )
      bug( "%s: No match: %s", __FUNCTION__, word );
  }
}


/* Load a troop file */
bool load_troop_file( const char *troopfile )
{
  char filename[256];
  TROOP_DATA *troop;
  FILE *fp;
  bool found = FALSE;

  CREATE( troop, TROOP_DATA, 1 );
  snprintf(filename, 256, "%s%s", TROOP_DIR, troopfile );
  if( (fp = fopen(filename, "r")) != NULL )
  {
    found = TRUE;
    for( ; ; )
    {
      char letter;
      const char *word;

      letter = fread_letter(fp);
      if( letter == '*' )
      {
        fread_to_eol(fp);
        continue;
      }

      if( letter != '#' )
      {
        bug( "%s: # not found.", __FUNCTION__ );
        break;
      }

      word = fread_word(fp);
      if( !str_cmp(word, "TROOP") )
      {
        fread_troop(troop, fp);
        break;
      }
      else if( !str_cmp(word, "END") )
        break;
      else
      {
        bug( "%s: Bad section: %s", __FUNCTION__, word );
        break;
      }
    }
    FCLOSE(fp);
  }

  if( found )
    LINK( troop, first_troop, last_troop, next, prev );
  else
    DISPOSE(troop);
  return found;
}


/* Load in all the troop files. */
void load_troops( void )
{
  FILE *fpList;
  const char *filename;
  char trooplist[256];

  first_troop = NULL;
  last_troop = NULL;
  log_string( "Loading troops..." );
  snprintf(trooplist, 256, "%s%s", TROOP_DIR, TROOP_LIST );
  if( !(fpList = fopen(trooplist, "r")) )
  {
    bug( "%s: Couldn't open %s for reading!", __FUNCTION__, trooplist );
    perror(trooplist);
    exit(1);
  }

  for( ; ; )
  {
    filename = feof(fpList) ? "$" : fread_word(fpList);
    if( filename[0] == '$' )
      break;

    if( !load_troop_file(filename) )
      bug( "%s: Couldn't load troop file: %s", __FUNCTION__, filename );
  }
  FCLOSE(fpList);
  return;
}


void do_settroop( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  TROOP_DATA *troop;
  int value;
  char arg3[MAX_INPUT_LENGTH];

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if( arg1[0] == '\0' )
  {
    send_to_char( "Usage: settroops <troop> <field> <setting>\n\r", ch );
    send_to_char( "\n\rField being one of:\n\r", ch );
    send_to_char( " magic_speciality race attribute power_rank damtype extra_damtype\n\r", ch );
    send_to_char( " attack_power extra_attack_power attack_type extra_attack_type\n\r", ch );
    send_to_char( " attack_initiative extra_attack_initiative counter_attack\n\r", ch );
    send_to_char( " hit mana move recruit_cost ability recruit_speed\n\r", ch );
    send_to_char( " upkeep1 upkeep2 upkeep3 upkeep4 upkeep5 upkeep6\n\r", ch );
    send_to_char( " spellres1 spellres2 spellres3 spellres4 spellres5 spellres6\n\r", ch );
    send_to_char( " attackres1 attackres2 attackres3 attackres4 attackres5 attackres6\n\r", ch );
    send_to_char( " attackres7 attackres8 attackres9 attackres10 attackres11 attackres12\n\r", ch );
    if( get_trust(ch) >= LEVEL_GOD )
      send_to_char( " name filename\n\r", ch );
    return;
  }

  troop = get_troop(arg1);
  if( !troop )
  {
    send_to_char( "No such troop.\n\r", ch );
    return;
  }

  if( !str_cmp(arg2, "magic_specialty" ) )
  {
    troop->magic_specialty = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "race" ) )
  {
    troop->race = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "attribute" ) )
  {
    if( NULLSTR(argument) )
    {
      send_to_char( "Usage: settroops <troop> attribute <flag>\n\r", ch );
      return;
    }

    while( argument[0] != '\0' )
    {
      argument = one_argument( argument, arg3 );
      value = get_troop_attrib_flag( arg3 );
      if( value < 0 || value > MAX_BITS )
        ch_printf(ch, "Unknown flag: %s\n\r", arg3 );
      else
        xTOGGLE_BIT( troop->attribute, value );
    }
    return;
  }

  if( !str_cmp(arg2, "power_rank" ) )
  {
    troop->power_rank = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "attack_power" ) )
  {
    troop->attack_power = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "extra_attack_power" ) )
  {
    troop->extra_attack_power = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "attack_type" ) )
  {
    troop->attack_type = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "damtype" ) )
  {
    troop->damtype = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "extra_damtype" ) )
  {
    troop->extra_damtype = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "extra_attack_type" ) )
  {
    troop->extra_attack_type = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "attack_initiative" ) )
  {
    troop->attack_initiative = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "extra_attack_initiative" ) )
  {
    troop->extra_attack_initiative = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "counter_attack" ) )
  {
    troop->counter_attack = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "hit" ) )
  {
    troop->hit = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "mana" ) )
  {
    troop->mana = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "move" ) )
  {
    troop->move = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "recruit_cost" ) )
  {
    troop->recruit_cost = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "recruit_speed" ) )
  {
    troop->recruit_speed = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "ability" ) )
  {
    if( NULLSTR(argument) )
    {
      send_to_char( "Usage: settroops <troop> ability <flag>\n\r", ch );
      return;
    }

    while( argument[0] != '\0' )
    {
      argument = one_argument( argument, arg3 );
      value = get_troop_ability_flag( arg3 );
      if( value < 0 || value > MAX_BITS )
        ch_printf(ch, "Unknown flag: %s\n\r", arg3 );
      else
        xTOGGLE_BIT( troop->ability, value );
    }
    return;
  }

  if( !str_cmp(arg2, "name" ) )
  {
    STRFREE(troop->name );
    troop->name = STRALLOC( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    return;
  }

  if( !str_cmp(arg2, "filename" ) )
  {
    DISPOSE(troop->filename );
    troop->filename = str_dup( argument );
    send_to_char( "Done.\n\r", ch );
    save_troop( troop );
    write_troop_list( );
    return;
  }

  if( !str_cmp(arg2, "upkeep1") )
  {
    troop->upkeep[0] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }
 
  if( !str_cmp(arg2, "upkeep2") )
  {
    troop->upkeep[1] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "upkeep3") )
  {
    troop->upkeep[2] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "upkeep4") )
  {
    troop->upkeep[3] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "upkeep5") )
  {
    troop->upkeep[4] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "upkeep6") )
  {
    troop->upkeep[5] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres1") )
  {
    troop->spell_resistance[0] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres2") )
  {
    troop->spell_resistance[1] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres3") )
  {
    troop->spell_resistance[2] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres4") )
  {
    troop->spell_resistance[3] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres5") )
  {
    troop->spell_resistance[4] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "spellres6") )
  {
    troop->spell_resistance[5] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres1") )
  {
    troop->attack_resistance[0] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres2") )
  {
    troop->attack_resistance[1] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres3") )
  {
    troop->attack_resistance[2] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres4") )
  {
    troop->attack_resistance[3] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres5") )
  {
    troop->attack_resistance[4] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres6") )
  {
    troop->attack_resistance[5] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres7") )
  {
    troop->attack_resistance[6] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres8") )
  {
    troop->attack_resistance[7] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres9") )
  {
    troop->attack_resistance[8] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres10") )
  {
    troop->attack_resistance[9] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres11") )
  {
    troop->attack_resistance[10] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }

  if( !str_cmp(arg2, "attackres12") )
  {
    troop->attack_resistance[11] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_troop(troop);
    return;
  }  
  do_settroop(ch, "");
  return;
}


void do_showtroop( CHAR_DATA *ch, char *argument )
{
  TROOP_DATA *troop;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  if( argument[0] == '\0' )
  {
    send_to_char( "Usage: showtroop <troop>\n\r", ch );
    return;
  }

  troop = get_troop(argument);
  if( !troop )
  {
    send_to_char( "No such troop.\n\r", ch );
    return;
  }

  ch_printf(ch, "Name: %s\n\rFilename: %s\n\r", troop->name, troop->filename );
  ch_printf(ch, "Race: %d\n\r", troop->race );
  ch_printf(ch, "Magic Specialty: %d\n\r", troop->magic_specialty );
  ch_printf(ch, "Attributes: %s\n\r", ext_flag_string( &troop->attribute, troop_attributes ) );
  ch_printf(ch, "Attack Power: %d  Extra Attack Power: %d\n\r", troop->attack_power, troop->extra_attack_power );
  ch_printf(ch, "Attack Type: %d  Extra Attack Type: %d\n\r", troop->attack_type, troop->extra_attack_type );
  ch_printf(ch, "Damage Type: %d  Extra Damage Type: %d\n\r", troop->damtype, troop->extra_damtype );
  ch_printf(ch, "Attack Initiative: %d  Extra Attack Initiative: %d\n\r",
    troop->attack_initiative, troop->extra_attack_initiative );
  ch_printf(ch, "Counter Attack: %d\n\r", troop->counter_attack );
  ch_printf(ch, "Hitpoints: %d  Mana: %d  Moves: %d\n\r", troop->hit, troop->mana, troop->move );
  ch_printf(ch, "Recruit Cost: %d  Recruit Speed: %d\n\r", troop->recruit_cost, troop->recruit_speed );
  ch_printf(ch, "\n\rUpkeep\n\rGold: %d  Food: %d  Wood: %d  Stone: %d  Mana: %d  Population: %d\n\r",
    troop->upkeep[0], troop->upkeep[1], troop->upkeep[2], troop->upkeep[3], troop->upkeep[4], troop->upkeep[5] );
/*  ch_printf(ch, "\n\rSpell Resistance\n\rAscendant: %d %\n\rEradication: %d %\n\rNether: %d %\n\r"
                "Phantasm: %d %\n\rVerdancy: %d %\n\rOther: %d\n\r",
    troop->spell_resistance[0], troop->spell_resistance[1], troop->spell_resistance[2],
    troop->spell_resistance[3], troop->spell_resistance[4], troop->spell_resistance[5] ); */
  ch_printf(ch, "Abilities: %s\n\r", ext_flag_string(&troop->ability, troop_abilities) );
  return;
}


void do_maketroop( CHAR_DATA *ch, char *argument )
{
  char filename[256];
  TROOP_DATA *troop;
  bool found = FALSE;

  if( NULLSTR(argument) )
  {
    send_to_char( "Usage: maketroop <troop name>\n\r", ch );
    return;
  }

  for( troop = first_troop; troop; troop = troop->next )
  {
    if( !str_cmp(argument, troop->name) )
    {
      found = TRUE;
      break;
    }
  }

  if( found )
  {
    send_to_char( "A troop with that name already exists!\n\r", ch );
    return;
  }

  snprintf(filename, 256, "%s.troop", strlower(argument) );
  CREATE( troop, TROOP_DATA, 1 );
  LINK( troop, first_troop, last_troop, next, prev );
  troop->name = STRALLOC(argument);
  troop->filename = str_dup(filename);
  ch_printf(ch, "Troop %s created.\n\r", capitalize(argument) );
  write_troop_list();
}


void do_viewtroops( CHAR_DATA *ch, char *argument )
{
  TROOP_DATA *troop;
  int count = 0;

  if( !first_troop )
  {
    send_to_char( "There are currently no troops. Use maketroop <name> to create them.\n\r", ch );
    return;
  }

  send_to_char( "Name\n\r-------------------------------\n\r", ch );
  for( troop = first_troop; troop; troop = troop->next )
  {
    count++;
    ch_printf(ch, "%s\n\r", troop->name );
  }

  ch_printf(ch, "-------------------------------\n\r%d total.\n\r", count );
  return;
}


void do_destroytroop( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_INPUT_LENGTH];
  char arg[MAX_STRING_LENGTH];
  TROOP_DATA *troop;

  if( IS_NPC(ch) )
  {
    send_to_char( "Oh hell no.\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg);
  if( arg[0] == '\0' )
  {
    send_to_char( "Destroy which troop?\n\r", ch );
    return;
  }

  if( !first_troop )
  {
    send_to_char( "There are no troops currently formed.\n\r", ch );
    return;
  }

  for( troop = first_troop; troop; troop = troop->next )
  {
    if( !str_cmp(arg, troop->name) )
      break;
  }

  if( !troop )
  {
    send_to_char( "No such troop.\n\r", ch );
    return;
   }

  UNLINK( troop, first_troop, last_troop, next, prev );
  ch_printf(ch, "Troop \"%s\" has been deleted.\n\r", arg );
  write_troop_list();
  snprintf(buf, MAX_INPUT_LENGTH, "rm %s%s", TROOP_DIR, troop->filename );
  system(buf);
  send_to_char( "Destroyed.\n\r", ch );
  return;
}


/* Get pointer to hero structure from hero name. */
HERO_DATA *get_hero( char *name )
{
  HERO_DATA *hero;

  for( hero = first_hero; hero; hero = hero->next )
  {
    if( !str_cmp(name, hero->name) )
      return hero;
  }
  return NULL;
}


void write_hero_list( void )
{
  HERO_DATA *hero;
  FILE *fpout;
  char filename[256];

  snprintf(filename, 256, "%s%s", HERO_DIR, HERO_LIST );
  if( !(fpout = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    return;
  }

  for( hero = first_hero; hero; hero = hero->next )
    fprintf(fpout, "%s\n", hero->filename );

  fprintf(fpout, "$\n" );
  FCLOSE(fpout);
}


/* Save a hero's data to its data file */
void save_hero( HERO_DATA *hero )
{
  FILE *fp;
  char filename[256];

  if( !hero )
  {
    bug( "%s: NULL hero!", __FUNCTION__ );
    return;
  }

  if( NULLSTR(hero->filename) )
  {
    bug( "%s: Hero %s has no filename", __FUNCTION__, hero->name );
    return;
  }

  snprintf(filename, 256, "%s%s", HERO_DIR, hero->filename );
  if( !(fp = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    perror(filename);
  }
  else
  {
    fprintf(fp, "#HERO\n" );
    fprintf(fp, "Name              %s~\n", hero->name           );
    fprintf(fp, "Filename          %s~\n", hero->filename       );
    fprintf(fp, "Class             %d\n",  hero->Class		);
    fprintf(fp, "Race              %d\n",  hero->race		);
    fprintf(fp, "Gender            %d\n",  hero->gender		);
    fprintf(fp, "Level             %d\n",  hero->level		);
    fprintf(fp, "Magic_specialty   %d\n",  hero->magic_specialty);
    fprintf(fp, "Attributes        %s\n",  print_bitvector(&hero->attribute) );
    fprintf(fp, "Attack_power      %d*%d\n",  hero->attack_power, hero->attack_level );
    fprintf(fp, "Attack_type       %d\n",  hero->attack_type	);
    fprintf(fp, "Counter_attack    %d*%d\n",  hero->counter_attack, hero->counter_attack_level );
    fprintf(fp, "Hit               %d*%d\n", hero->hit, hero->hit_level );
    fprintf(fp, "Mana              %d*%d\n", hero->mana, hero->mana_level );
    fprintf(fp, "Upkeep		%d*%d %d*%d %d*%d %d*%d %d*%d %d*%d\n",
      hero->upkeep[0], hero->upkeep_level[0], hero->upkeep[1], hero->upkeep_level[1],
      hero->upkeep[2], hero->upkeep_level[2], hero->upkeep[3], hero->upkeep_level[3],
      hero->upkeep[4], hero->upkeep_level[4], hero->upkeep[5], hero->upkeep_level[5] );
    fprintf(fp, "Ability           %s\n", print_bitvector(&hero->ability) );
    fprintf(fp, "End\n\n"                                       );
    fprintf(fp, "#END\n"                                        );
  }

  FCLOSE(fp);
  return;
}


/* Read in actual hero data. */
void fread_hero( HERO_DATA *hero, FILE *fp )
{
  const char *word;
  bool fMatch;
  char letter;

  for( ; ; )
  {
    word   = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;
    switch(UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'A':
        KEY( "Ability",     	hero->ability,		fread_bitvector(fp)	);
        if( !str_cmp(word, "Attack_power") )
        {
          hero->attack_power = 	fread_number(fp);
          letter 	     = 	fread_letter(fp);
	  hero->attack_level = 	fread_number(fp);
          fMatch = TRUE;
	  break;
        }

        KEY( "Attack_type", 	hero->attack_type,	fread_number(fp) 	);
        KEY( "Attributes",  	hero->attribute,       	fread_bitvector(fp) 	);
        break;

      case 'C':
        KEY( "Class",		hero->Class,		fread_number(fp) 	);
        if( !str_cmp(word, "Counter_attack") )
        {
          hero->counter_attack       = fread_number(fp);
          letter 	  	     = fread_letter(fp);
          hero->counter_attack_level = fread_number(fp);
          fMatch = TRUE;
          break;
        }
        break;

      case 'E':
        if( !str_cmp(word, "End") )
        {
          nstralloc(&hero->name);
          return;
        }
        break;

      case 'F':
        KEY( "Filename",    	hero->filename,  	fread_string_nohash(fp) );
        break;

      case 'G':
        KEY( "Gender",      	hero->gender,          	fread_number(fp) 	);
        break;

      case 'H':
        if( !str_cmp(word, "Hit") )
        {
          hero->hit       = 	fread_number(fp);
          letter 	  = 	fread_letter(fp);
          hero->hit_level = 	fread_number(fp);
          fMatch = TRUE;
          break;
        }
        break;

      case 'L':
        KEY( "Level",       	hero->level,           	fread_number(fp) 	);
        break;

      case 'M':
        KEY( "Magic_specialty",	hero->magic_specialty, 	fread_number(fp) 	);
        if( !str_cmp(word, "Mana") )
        {
          hero->mana       = 	fread_number(fp);
          letter 	   = 	fread_letter(fp);
          hero->mana_level = 	fread_number(fp);
          fMatch = TRUE;
          break;
        }
        break;

      case 'N':
        KEY( "Name",		hero->name,		fread_string(fp) 	);
        break;

      case 'R':
        KEY( "Race",        	hero->race,            	fread_number(fp) 	);
        break;

      case 'U':
        if( !str_cmp(word, "Upkeep") )
        {
          hero->upkeep[0] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[0] = fread_number(fp);
          hero->upkeep[1] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[1] = fread_number(fp);
          hero->upkeep[2] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[2] = fread_number(fp);
          hero->upkeep[3] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[3] = fread_number(fp);
          hero->upkeep[4] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[4] = fread_number(fp);
          hero->upkeep[5] 	= fread_number(fp);
          letter 	      	= fread_letter(fp);
          hero->upkeep_level[5] = fread_number(fp);
          fMatch = TRUE;
          break;
        }
        break;
    }

    if( !fMatch )
      bug( "%s: No match: %s", __FUNCTION__, word );
  }
}


/* Load a hero file */
bool load_hero_file( const char *herofile )
{
  char filename[256];
  HERO_DATA *hero;
  FILE *fp;
  bool found = FALSE;

  CREATE( hero, HERO_DATA, 1 );
  snprintf(filename, 256, "%s%s", HERO_DIR, herofile );
  if( (fp = fopen(filename, "r")) != NULL )
  {
    found = TRUE;
    for( ; ; )
    {
      char letter;
      const char *word;

      letter = fread_letter(fp);
      if( letter == '*' )
      {
        fread_to_eol(fp);
        continue;
      }

      if( letter != '#' )
      {
        bug( "%s: # not found.", __FUNCTION__ );
        break;
      }

      word = fread_word(fp);
      if( !str_cmp(word, "HERO") )
      {
        fread_hero(hero, fp);
        break;
      }
      else if( !str_cmp(word, "END") )
        break;
      else
      {
        bug( "%s: Bad section: %s", __FUNCTION__, word );
        break;
      }
    }
    FCLOSE(fp);
  }

  if( found )
    LINK( hero, first_hero, last_hero, next, prev );
  else
    DISPOSE(hero);
  return found;
}


/* Load in all the hero files. */
void load_heroes( void )
{
  FILE *fpList;
  const char *filename;
  char herolist[256];

  first_hero = NULL;
  last_hero = NULL;
  log_string( "Loading heroes..." );
  snprintf(herolist, 256, "%s%s", HERO_DIR, HERO_LIST );
  if( !(fpList = fopen(herolist, "r")) )
  {
    bug( "%s: Couldn't open %s for reading!", __FUNCTION__, herolist );
    perror(herolist);
    exit(1);
  }

  for( ; ; )
  {
    filename = feof(fpList) ? "$" : fread_word(fpList);
    if( filename[0] == '$' )
      break;

    if( !load_hero_file(filename) )
      bug( "%s: Couldn't load hero file: %s", __FUNCTION__, filename );
  }

  FCLOSE(fpList);
  return;
}


void do_sethero( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  HERO_DATA *hero;
  int value;
  char arg3[MAX_INPUT_LENGTH];

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  if( arg1[0] == '\0' )
  {
    send_to_char( "Usage: setheros <hero> <field> <setting>\n\r", ch );
    send_to_char( "\n\rField being one of:\n\r", ch );
    send_to_char( " magic_speciality race attribute\n\r", ch );
    send_to_char( " attack_power attack_level attack_type\n\r", ch );
    send_to_char( " counter_attack counter_attack_level\n\r", ch );
    send_to_char( " hit mana ability level gender\n\r", ch );
    send_to_char( " upkeep1 upkeep2 upkeep3 upkeep4 upkeep5 upkeep6\n\r", ch );
    send_to_char( " upkeep_level1 upkeep_level2 upkeep_level3 upkeep_level4 upkeep_level5 upkeep_level6\n\r", ch );
    if( get_trust(ch) >= LEVEL_GOD )
    {
      send_to_char( " name filename\n\r", ch );
    }
    return;
  }

  hero = get_hero( arg1 );
  if( !hero )
  {
    send_to_char( "No such hero.\n\r", ch );
    return;
  }

  if( !str_cmp(arg2, "magic_specialty" ) )
  {
    hero->magic_specialty = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "race" ) )
  {
    hero->race = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "attribute" ) )
  {
    if( NULLSTR(argument) )
    {
      send_to_char( "Usage: sethero <hero> attribute <flag>\n\r", ch );
      return;
    }

    while( argument[0] != '\0' )
    {
      argument = one_argument( argument, arg3 );
      value = get_troop_attrib_flag( arg3 );
      if( value < 0 || value > MAX_BITS )
        ch_printf(ch, "Unknown flag: %s\n\r", arg3 );
      else
        xTOGGLE_BIT( hero->attribute, value );
    }
    return;
  }

  if( !str_cmp(arg2, "attack_power" ) )
  {
    hero->attack_power = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "attack_level" ) )
  {
    hero->attack_level = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "attack_type" ) )
  {
    hero->attack_type = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "counter_attack" ) )
  {
    hero->counter_attack = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }
 
  if( !str_cmp(arg2, "counter_attack_level" ) )
  {
    hero->counter_attack_level = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }
 
  if( !str_cmp(arg2, "hit" ) )
  {
    hero->hit = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }
 
  if( !str_cmp(arg2, "hit_level" ) )
  {
    hero->hit_level = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }
 
  if( !str_cmp(arg2, "mana" ) )
  {
    hero->mana = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "mana_level" ) )
  {
    hero->mana_level = atoi( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "ability" ) )
  {
    if( NULLSTR(argument) )
    {
      send_to_char( "Usage: sethero <hero> ability <flag>\n\r", ch );
      return;
    }

    while( argument[0] != '\0' )
    {
      argument = one_argument( argument, arg3 );
      value = get_troop_hero_ability_flag( arg3 );
      if( value < 0 || value > MAX_BITS )
        ch_printf(ch, "Unknown flag: %s\n\r", arg3 );
      else
        xTOGGLE_BIT( hero->ability, value );
    }
    return;
  }

  if( !str_cmp(arg2, "name" ) )
  {
    STRFREE(hero->name );
    hero->name = STRALLOC( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    return;
  }

  if( !str_cmp(arg2, "filename" ) )
  {
    DISPOSE(hero->filename );
    hero->filename = str_dup( argument );
    send_to_char( "Done.\n\r", ch );
    save_hero( hero );
    write_hero_list( );
    return;
  }

  if( !str_cmp(arg2, "upkeep1") )
  {
    hero->upkeep[0] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep2") )
  {
    hero->upkeep[1] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep3") )
  {
    hero->upkeep[2] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep4") )
  {
    hero->upkeep[3] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep5") )
  {
    hero->upkeep[4] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep6") )
  {
    hero->upkeep[5] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level1") )
  {
    hero->upkeep_level[0] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level2") )
  {
    hero->upkeep_level[1] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level3") )
  {
    hero->upkeep_level[2] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level4") )
  {
    hero->upkeep_level[3] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level5") )
  {
    hero->upkeep_level[4] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }

  if( !str_cmp(arg2, "upkeep_level6") )
  {
    hero->upkeep_level[5] = atoi(argument);
    send_to_char( "Done.\n\r", ch );
    save_hero(hero);
    return;
  }  
  do_sethero(ch, "");
  return;
}


void do_showhero( CHAR_DATA *ch, char *argument )
{
  HERO_DATA *hero;

  if( IS_NPC(ch) )
  {
    send_to_char( "Que dis?\n\r", ch );
    return;
  }

  if( argument[0] == '\0' )
  {
    send_to_char( "Usage: showhero <hero>\n\r", ch );
    return;
  }

  hero = get_hero(argument);
  if( !hero )
  {
    send_to_char( "No such hero.\n\r", ch );
    return;
  }

  ch_printf(ch, "Name: %s\n\rFilename: %s\n\r", hero->name, hero->filename );
  ch_printf(ch, "Class: %d\n\r", hero->Class );
  ch_printf(ch, "Level: %d\n\r", hero->level );
  ch_printf(ch, "Race: %d\n\r", hero->race );
  ch_printf(ch, "Gender: %d\n\r", hero->gender );
  ch_printf(ch, "Magic Specialty: %d\n\r", hero->magic_specialty );
  ch_printf(ch, "Attributes: %s\n\r", ext_flag_string(&hero->attribute, troop_attributes) );
  ch_printf(ch, "Attack Power: %d + %d*level\n\r", hero->attack_power, hero->attack_level );
  ch_printf(ch, "Attack Type: %d\n\r", hero->attack_type );
  ch_printf(ch, "Counter Attack: %d + %d*level\n\r", hero->counter_attack, hero->counter_attack_level );
  ch_printf(ch, "Hitpoints: %d + %d*level  Mana: %d + %d*level\n\r", hero->hit, hero->hit_level,
    hero->mana, hero->mana_level );
  ch_printf(ch, "\n\rUpkeep\n\rGold: %d + %d*level  Food: %d + %d*level  Wood: %d + %d*level  Stone: %d + %d*level\n\r"
              "  Mana: %d + %d*level  Population: %d + %d*level\n\r",
    hero->upkeep[0], hero->upkeep_level[0], hero->upkeep[1], hero->upkeep_level[1],
    hero->upkeep[2], hero->upkeep_level[2], hero->upkeep[3], hero->upkeep_level[3], 
    hero->upkeep[4], hero->upkeep_level[4], hero->upkeep[5], hero->upkeep_level[0] );
  ch_printf(ch, "Abilities: %s\n\r", ext_flag_string( &hero->ability, hero_abilities ) );
  return;
}


void do_makehero( CHAR_DATA *ch, char *argument )
{
  char filename[256];
  HERO_DATA *hero;
  bool found = FALSE;

  if( NULLSTR(argument) )
  {
    send_to_char( "Usage: makehero <hero name>\n\r", ch );
    return;
  }

  for( hero = first_hero; hero; hero = hero->next )
  {
    if( !str_cmp(argument, hero->name) )
    {
      found = TRUE;
      break;
    }
  }

  if( found )
  {
    send_to_char( "A hero with that name already exists!", ch );
    return;
  }

  snprintf(filename, 256, "%s.hero", strlower(argument) );
  CREATE( hero, HERO_DATA, 1 );
  LINK( hero, first_hero, last_hero, next, prev );
  hero->name = STRALLOC(argument);
  hero->filename = str_dup(filename);
  ch_printf(ch, "Hero %s created.\n\r", capitalize(argument) );
  write_hero_list();
}


void do_viewheroes( CHAR_DATA *ch, char *argument )
{
  HERO_DATA *hero;
  int count = 0;

  if( !first_hero )
  {
    send_to_char( "There are currently no heroes. Use makehero <name> to create them.\n\r", ch );
    return;
  }

  send_to_char( "Name\n\r-------------------------------\n\r", ch );
  for( hero = first_hero; hero; hero = hero->next )
  {
    count++;
    ch_printf(ch, "%s\n\r", hero->name );
  }
  ch_printf(ch, "-------------------------------\n\r%d total.\n\r", count );

  return;
}


void do_destroyhero( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_INPUT_LENGTH];
  char arg[MAX_STRING_LENGTH];
  HERO_DATA *hero;

  if( IS_NPC(ch) )
  {
    send_to_char( "Oh hell no.\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg);
  if( arg[0] == '\0' )
  {
    send_to_char( "Destroy which hero?\n\r", ch );
    return;
  }

  if( !first_hero )
  {
    send_to_char( "No heroes have been created yet.\n\r", ch );
    return;
  }

  for( hero = first_hero; hero; hero = hero->next )
  {
    if( !str_cmp(arg, hero->name) )
      break;
  }

  if( !hero )
  {
    send_to_char( "No such hero.\n\r", ch );
    return;
  }

  UNLINK( hero, first_hero, last_hero, next, prev );
  ch_printf(ch,"Hero \"%s\" has been deleted.\n\r", arg );
  write_hero_list();
  snprintf(buf, MAX_INPUT_LENGTH, "rm %s%s", HERO_DIR, hero->filename );
  system(buf);
  send_to_char( "Destroyed.\n\r", ch );
  return;
}


int kingdom_area_power( KGDM_AREA *area )
{
  return 0;
}


char *kingdom_area_status( KGDM_AREA *area )
{
  return "Happy";
}


void show_kareas( CHAR_DATA *ch, KINGDOM_DATA *kingdom, bool fAll )
{
  KGDM_AREA *kArea;
  short cnt = 0, total = 0;

  if( fAll )
  {
    for( kArea = first_karea; kArea; kArea = kArea->next )
    {
      if( !NULLSTR(kArea->owner) )
        cnt++;
      total++;
    }
    send_to_char( "Owner                Name                      Population     Power     Status\n\r", ch );
    send_to_char( "------------------------------------------------------------------------------\n\r", ch );
    for( kArea = first_karea; kArea; kArea = kArea->next )
    {
      ch_printf(ch, "%20s %25s %13s   %3d%%     %s\n\r", kArea->owner, kArea->name, num_punct(kArea->population),
        kingdom_area_power(kArea) / cnt, kingdom_area_status(kArea) );
    }
    send_to_char( "------------------------------------------------------------------------------\n\r", ch );
    ch_printf(ch, "%d of %d areas conquered.\n\r", cnt, total );
    return;
  }

  if( !kingdom )
  {
    bug( "%s: NULL kingdom!", __FUNCTION__ );
    return;
  }

  if( !kingdom->first_karea )
  {
    send_to_char( "No areas conquered.\n\r", ch );
    return;
  }

  for( kArea = kingdom->first_karea; kArea; kArea = kArea->next )
    cnt++;

  send_to_char( "Name                      Population     Power     Status\n\r", ch );
  send_to_char( "-----------------------------------------------------------\n\r", ch );
  for( kArea = kingdom->first_karea; kArea; kArea = kArea->next )
  {
    ch_printf(ch, "%25s %13s   %3d%%     %s\n\r", kArea->name, num_punct(kArea->population),
      kingdom_area_power(kArea) / cnt, kingdom_area_status(kArea) );
  }

  send_to_char( "-----------------------------------------------------------\n\r", ch );
  ch_printf(ch, "%d areas conquered.\n\r", cnt );
  return;
}


void spawn_troops( KINGDOM_DATA *kingdom )
{
  KGDM_AREA *area;

  if( kingdom )
    return;

  if( !kingdom->first_karea )
    return;

  for( area = kingdom->first_karea; area; area = area->next )
  {
    if( !NULLSTR(area->troop1) )
    {
      if( --area->rec_timer1 <= 0 )
      {
        area->rec_timer1 = area->rec_spd1;   // Reset timer
        area->num_troops1 += area->rec_num1; // Increase number of troops available to recruit
      }
    }

    if( !NULLSTR(area->troop2) )
    {
      if( --area->rec_timer2 <= 0 )
      {
        area->rec_timer2 = area->rec_spd2;
        area->num_troops2 += area->rec_num2;
      }
    }

    if( !NULLSTR(area->troop3) )
    {
      if( --area->rec_timer3 <= 0 )
      {
        area->rec_timer3 = area->rec_spd3;
        area->num_troops3 += area->rec_num3;
      }
    }
  }
  return;
}


void spawn_hero( KINGDOM_DATA *kingdom )
{

  if( !kingdom )
    return;

  if( --kingdom->timer_hero <= 0 )
  {
    if( number_percent() <= 35 )
      kingdom_message( "A new hero has wandered into the realm and seeks employment.\n\r", kingdom );
  }
  return;
}


void harvest_resources( KINGDOM_DATA *kingdom )
{
  char buf[MAX_STRING_LENGTH];
  KGDM_AREA *area;
  int gold = 0;
  int food = 0;
  int wood = 0;
  int stone = 0;

  for( area = kingdom->first_karea; area; area = area->next )
  {
    if( area->gold[REMAINING] > 0 && area->gold[GATHERING] > 0 )
    {
      gold = UMAX( area->gold[GATHERING], area->gold[REMAINING] );
      area->gold[STOCKPILE] += gold;
      area->gold[REMAINING] -= gold;
    }

    if( area->food[REMAINING] > 0 && area->food[GATHERING] > 0 )
    {
      food = UMAX( area->food[GATHERING] * 2, area->food[REMAINING] );
      area->food[STOCKPILE] += food;
      area->food[REMAINING] -= food;
    }

    if( area->wood[REMAINING] > 0 && area->wood[GATHERING] > 0 )
    {
      wood = UMAX( (int) (area->wood[GATHERING] * .50), area->wood[REMAINING] );
      area->wood[STOCKPILE] += wood;
      area->wood[REMAINING] -= wood;
    }

    if( area->stone[REMAINING] > 0 && area->stone[GATHERING] > 0 )
    {
      stone = UMAX( (int) (area->stone[GATHERING] * .30), area->stone[REMAINING] );
      area->stone[STOCKPILE] += stone;
      area->stone[REMAINING] -= stone;
    }
  }
  snprintf(buf, MAX_STRING_LENGTH, "%d gold, %d food, %d wood and %d stone have been harvested from %s and added to the stockpile.\n\r",
    gold, food, wood, stone, area->name );
  kingdom_message(buf, kingdom);
  return;
}    


void increase_population( KINGDOM_DATA *kingdom )
{
  KGDM_AREA *area;
  char buf[MAX_STRING_LENGTH];
  int increase;
  int mod = 1;
  int change = 0;
  int diff;

  if( number_percent() > 5 )
    return;

  for( area = kingdom->first_karea; area; area = area->next )
  {
    increase = (int) (area->population * .01);
    mod = ((increase * area->mood) / 100);
    change = increase + mod;
    diff = area->population - change;
    area->population += change;
  }
  snprintf(buf, MAX_STRING_LENGTH, "The population of %s has been %s by %d!\n\r",
    area->name, change >= 0 ? "increased" : "decreased", change );
  kingdom_message(buf, kingdom);
  return;   
}


void check_kingdom_status( KINGDOM_DATA *kingdom )
{
}


void kingdom_update( void )
{
  KINGDOM_DATA *kingdom;

  /* Figured it should only do one loop and call the functions opposed to each function
   * running the same loop --Exo
   */
  for( kingdom = first_kingdom; kingdom; kingdom = kingdom->next )
  {
    spawn_hero(kingdom);
    spawn_troops(kingdom);
    harvest_resources(kingdom);
    increase_population(kingdom);
    check_kingdom_status(kingdom);
  }
  return;
}

// Copy of changes system, no sense in re-writing what's already done
void load_kingdom_news( KINGDOM_DATA *kingdom )
{
  char filename[256];
  FILE *fp;
  int i;

  snprintf(filename, 256, "%s%s.news", KINGDOM_DIR, kingdom->filename );
  if( !(fp = fopen(filename, "r")) )
  {
    bug( "%s: Couldn't open %s for reading!", __FUNCTION__, filename );
    return;
  }

  fscanf(fp, "%d\n", &maxNews );
  /* Use malloc so we can realloc later on */
  kingdom->news = (KGDM_NEWS *) malloc(sizeof(KGDM_NEWS) * (maxNews + 1) );
  for( i = 0; i < maxNews; i++ )
  {
    kingdom->news[i].news = fread_string(fp);
    kingdom->news[i].date = fread_string(fp);
    kingdom->news[i].mudtime = fread_number(fp);
  }
  FCLOSE(fp);
  return; /* just return */
}


void save_kingdom_news( KINGDOM_DATA *kingdom )
{
  char filename[256];
  FILE *fp;
  int i;

  snprintf(filename, 256, "%s%s.news", KINGDOM_DIR, kingdom->filename );
  if( !(fp = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    perror(filename);
    return;
  }

  fprintf(fp, "%d\n", maxNews );
  for( i = 0; i < maxNews; i++ )
  {
    fprintf(fp, "%s~\n", kingdom->news[i].news);
    fprintf(fp, "%s~\n", kingdom->news[i].date);
    fprintf(fp, "%ld\n", kingdom->news[i].mudtime );
    fprintf(fp, "\n" );
  }
 
  FCLOSE(fp);
  return;
}


void delete_kingdom_news( KINGDOM_DATA *kingdom, int iNews )
{
  int i, j;
  KGDM_NEWS *new_table;

  new_table = (KGDM_NEWS *) malloc(sizeof(KGDM_NEWS) * maxNews);
  if( !new_table )
    return;
    
  for( i = 0, j = 0; i < (maxNews + 1); i++)
  {
    if( i != iNews )
    {
      new_table[j] = kingdom->news[i];
      j++;
    }
  }
    
  free(kingdom->news);
  kingdom->news = new_table;
  maxNews--;
  return;
}


void free_kingdom_news( KINGDOM_DATA *kingdom )
{
  int i;

  for( i = 0; i < maxNews; i++ )
  {
    DISPOSE(kingdom->news[i].news);
    DISPOSE(kingdom->news[i].date);
  }
  DISPOSE(kingdom->news);
}


void kingdom_message( char *argument, KINGDOM_DATA *kingdom )
{
  KGDM_NEWS * new_table;
    
  if( argument[0] == '\0' )
    return;

  maxNews++;
  new_table = (KGDM_NEWS *) realloc(kingdom->news, sizeof(KGDM_NEWS) * (maxNews + 1));
  if( !new_table )
  {
    bug( "%s: Memory allocation for %s's news failed. Brace for impact.\n\r", __FUNCTION__, kingdom->name );
    return;
  }

  kingdom->news = new_table;
  kingdom->news[maxNews-1].news = str_dup(argument);
  kingdom->news[maxNews-1].date = str_dup(current_date());
  kingdom->news[maxNews-1].mudtime = current_time;
  save_kingdom_news( kingdom );
  return;
}
 

int num_news( KINGDOM_DATA *kingdom )
{
  char *test = current_date();
  int today = 0;
  int i = 0;
    
  for( i = 0; i < maxNews; i++)
  {
    if( !str_cmp(test, kingdom->news[i].date) )
      today++;
  }
  return today;
}
    

void do_kingdom_news( CHAR_DATA *ch, char *argument ) 
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  char *test;
  int today = 0;
  int i = 0;
  bool fAll;
  KINGDOM_DATA *kingdom;
    
  one_argument(argument, arg);
  if( IS_NPC(ch) )
    return;

  kingdom = ch->pcdata->kingdom;
  if( !kingdom )
    return;

  if( maxNews < 1 )
    return;

  test = current_date();
  for( i = 0; i < maxNews; i++)
  {
    if( !str_cmp(test, kingdom->news[i].date) )
      today++;
  }

  if( NULLSTR(arg) )
    fAll = FALSE;
  else
    fAll = !str_cmp(arg, "all");

  send_to_char("&wNo.  Date        Change&d\n\r",ch );
  send_to_char("&R----------------------------------------\n\r", ch );
  for(i = 0; i < maxNews; i++)
  {
    if( !fAll && kingdom->news[i].mudtime + (7 * 24L * 3600L) < current_time )
      continue;

    snprintf(buf, MAX_STRING_LENGTH, "&z[&R%2d&z] &c*%-6s &W%-55s&d\n\r", (i + 1), kingdom->news[i].date, kingdom->news[i].news );
    send_to_char( buf, ch );
  }

  send_to_char( "&R----------------------------------------\n\r", ch );
  snprintf(buf, MAX_STRING_LENGTH, "&wThere is a total of &z[ &Y%3d &z] &wmessages.&d\n\r", maxNews );
  send_to_char( buf, ch );
  send_to_char( "&zAlso see: '&Ckingdomnews all&z' for a list of all the messages.&z\n\r", ch );
  send_to_char( "&R----------------------------------------\n\r", ch );
  snprintf(buf, MAX_STRING_LENGTH, "&wThere is a total of &z[ &Y%2d &z] &wnew messages that have been added today.&d\n\r", today );
  send_to_char( buf, ch );
  send_to_char( "&R----------------------------------------\n\r", ch );
  send_to_char( "&R----------------------------------------\n\r", ch );
  return;
}


char *show_army( SUBUNIT_DATA *unit )
{
  int num = unit->number;

  if( num > 0 && num <= 5 )		return "A few";
  if( num >= 5 && num <= 9 )    	return "Several";
  if( num >= 10 && num <= 19 ) 		return "Pack";
  if( num >= 20 && num <= 49 )  	return "Lots";
  if( num >= 50 && num <= 99 )  	return "Horde";
  if( num >= 100 && num <= 249 )	return "Throng";
  if( num >= 250 && num <= 499 )    	return "Swarm";
  if( num >= 500 && num <= 999 )    	return "Zounds";
  if( num >= 1000 )    			return "Legion";
  return "Unknown";
}


int used_land( KGDM_AREA *area )
{
  if( !area )
  {
    bug( "%s: NULL area!", __FUNCTION__ );
    return 0;
  }
  return (area->towns + area->farms + area->guilds + area->nodes +
          area->barracks + area->workshops + area->fortresses + area->barriers);
}


int availible_land( KGDM_AREA *area )
{
  if( !area )
  {
    bug( "%s: NULL area!", __FUNCTION__ );
    return 0;
  }
  return (area->land - used_land(area));
}


int build_cost( short type, short cost, int num )
{
  int total = 0;

  switch(type)
  {
    case BUILDING_TOWN:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_town[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_town[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_town[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_town[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_town[KCOST_STONE];
      break;
    case BUILDING_FARM:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_farm[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_farm[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_farm[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_farm[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_farm[KCOST_STONE];
      break;
    case BUILDING_GUILD:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_guild[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_guild[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_guild[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_guild[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_guild[KCOST_STONE];
      break;
    case BUILDING_NODE:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_node[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_node[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_node[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_node[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_node[KCOST_STONE];
      break;
    case BUILDING_BARRIER:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_barrier[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_barrier[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_barrier[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_barrier[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_barrier[KCOST_STONE];
      break;
    case BUILDING_BARRACKS:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_barracks[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_barracks[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_barracks[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_barracks[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_barracks[KCOST_STONE];
      break;
    case BUILDING_WORKSHOP:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_workshop[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_workshop[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_workshop[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_workshop[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_workshop[KCOST_STONE];
      break;
    case BUILDING_FORTRESS:
      if( cost == KCOST_GOLD )	total = kgdmdata.build_fortress[KCOST_GOLD];
      if( cost == KCOST_FOOD )	total = kgdmdata.build_fortress[KCOST_FOOD];
      if( cost == KCOST_WOOD )	total = kgdmdata.build_fortress[KCOST_WOOD];
      if( cost == KCOST_MANA )	total = kgdmdata.build_fortress[KCOST_MANA];
      if( cost == KCOST_STONE )	total = kgdmdata.build_fortress[KCOST_STONE];
      break;
    default: return 0;
  }
  return total * num;
}


bool can_build( CHAR_DATA *ch, KINGDOM_DATA *kingdom, short type, int num )
{
  bool found = TRUE;

  switch(type)
  {
    case BUILDING_TOWN:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_FARM:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_GUILD:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_NODE:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_BARRIER:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_BARRACKS:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_WORKSHOP:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    case BUILDING_FORTRESS:
      if( kingdom->gold < build_cost(type, KCOST_GOLD, num) )
      {
        send_to_char( "Your kingdom does not have enough gold to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->food < build_cost(type, KCOST_FOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough food to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->wood < build_cost(type, KCOST_WOOD, num) )
      {
        send_to_char( "Your kingdom does not have enough wood to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->mana < build_cost(type, KCOST_MANA, num) )
      {
        send_to_char( "Your kingdom does not have enough mana to build that many.\n\r", ch );
        found = TRUE;
      }
      if( kingdom->stone < build_cost(type, KCOST_STONE,num) )
      {
        send_to_char( "Your kingdom does not have enough stone to build that many.\n\r", ch );
        found = TRUE;
      }
      break;

    default: return FALSE;
  }
  if( found )
    return FALSE;

  return TRUE;
}


void build_kingdom( KINGDOM_DATA *kingdom, KGDM_AREA *area, short type, int value )
{
  switch(type)
  {
    case BUILDING_TOWN:
      kingdom->gold -= build_cost(BUILDING_TOWN, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_TOWN, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_TOWN, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_TOWN, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_TOWN, KCOST_STONE, value);
      area->towns += value;
      break;

    case BUILDING_FARM:
      kingdom->gold -= build_cost(BUILDING_FARM, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_FARM, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_FARM, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_FARM, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_FARM, KCOST_STONE, value);
      area->farms += value;
      break;

    case BUILDING_GUILD:
      kingdom->gold -= build_cost(BUILDING_GUILD, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_GUILD, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_GUILD, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_GUILD, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_GUILD, KCOST_STONE, value);
      area->guilds += value;
      break;

    case BUILDING_NODE:
      kingdom->gold -= build_cost(BUILDING_NODE, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_NODE, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_NODE, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_NODE, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_NODE, KCOST_STONE, value);
      area->nodes += value;
      break;

    case BUILDING_BARRIER:
      kingdom->gold -= build_cost(BUILDING_BARRIER, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_BARRIER, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_BARRIER, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_BARRIER, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_BARRIER, KCOST_STONE, value);
      area->barriers += value;
      break;

    case BUILDING_BARRACKS:
      kingdom->gold -= build_cost(BUILDING_BARRACKS, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_BARRACKS, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_BARRACKS, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_BARRACKS, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_BARRACKS, KCOST_STONE, value);
      area->barracks += value;
      break;

    case BUILDING_WORKSHOP:
      kingdom->gold -= build_cost(BUILDING_WORKSHOP, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_WORKSHOP, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_WORKSHOP, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_WORKSHOP, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_WORKSHOP, KCOST_STONE, value);
      area->workshops += value;
      break;

    case BUILDING_FORTRESS:
      kingdom->gold -= build_cost(BUILDING_FORTRESS, KCOST_GOLD, value);
      kingdom->food -= build_cost(BUILDING_FORTRESS, KCOST_FOOD, value);
      kingdom->wood -= build_cost(BUILDING_FORTRESS, KCOST_WOOD, value);
      kingdom->mana -= build_cost(BUILDING_FORTRESS, KCOST_MANA, value);
      kingdom->stone-= build_cost(BUILDING_FORTRESS, KCOST_STONE, value);
      area->fortresses += value;
      break;

    default: return;
  }
  return;
}


int daily_production( KGDM_AREA *area, short type )
{
  return 0;
}


char *get_area_status( KGDM_AREA *area )
{
  return "Happy";
}


int build_days( short type, KGDM_AREA *area, short num )
{
  return 1;
}


char * get_hero_class( short Class )
{
  return "Unknown";
}


void add_karea( KGDM_AREA *area, KINGDOM_DATA *kingdom )
{
}


void remove_karea( KGDM_AREA *area, KINGDOM_DATA *kingdom )
{
}


void do_kingdom( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  short rank = 0;
  KINGDOM_DATA *kingdom;

  if( IS_NPC(ch) )
    return;

  switch(ch->substate)
  {
    default:
      break;

    case SUB_RESTRICTED:
      send_to_char( "You cannot do this while in another command.\n\r", ch );
      return;

    case SUB_KINGDOM_DESC:
      kingdom = (KINGDOM_DATA *) ch->dest_buf;
      STRFREE(kingdom->description );
      kingdom->description = copy_buffer(ch);
      stop_editing(ch);
      save_kingdom(kingdom);
      ch->substate = ch->tempnum;
      return;
  }

  kingdom = ch->pcdata->kingdom;
  if( !kingdom )
  {
    send_to_char( "You don't belong to a kingdom.\n\r", ch );
    return;
  }

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);

       if( !str_cmp(kingdom->ruler, ch->name) )   rank = KRANK_LEADER;
  else if( !str_cmp(kingdom->deity, ch->name) )   rank = KRANK_DEITY;
  else if( !str_cmp(kingdom->number1, ch->name) ) rank = KRANK_NUMBER1;
  else if( !str_cmp(kingdom->number2, ch->name) ) rank = KRANK_NUMBER2;
  else if( !str_cmp(kingdom->number3, ch->name) ) rank = KRANK_NUMBER3;
  else if( !str_cmp(kingdom->number4, ch->name) ) rank = KRANK_NUMBER4;
  else if( !str_cmp(kingdom->number5, ch->name) ) rank = KRANK_NUMBER5;
  else if( !str_cmp(kingdom->general, ch->name) ) rank = KRANK_GENERAL;

  if( rank <= 0 )
  {
    send_to_char( "You are not worthy.\n\r", ch );
    return;
  }

  if( NULLSTR(arg1) )
  {
    send_to_char( "&WKingdom Management                      Description\n\r", ch );
    send_to_char( "&B========================================================================\n\r", ch );
    send_to_char( "&CGeneral\n\r", ch );
    send_to_char( "        &WSyntax: kingdom status          &wView the overall kingdom status.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom settings        &wModify kingdom settings.\n\r", ch );
    send_to_char( "\n\r", ch );
    send_to_char( "&CWar\n\r", ch );
    send_to_char( "        &WSyntax: kingdom ally            &wDeclare or end an alliance with another kingdom.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom war             &wDeclare or end a war with another kingdom.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom areas           &wView areas under kindom control.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom conquer         &wTake control of a defeated area.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom relinquish      &wRelinquish control over an area.\n\r", ch );
    send_to_char( "\n\r", ch );
    send_to_char( "&CExpansion\n\r", ch );
    send_to_char( "        &WSyntax: kingdom build           &wCreate buildings within the kingdom.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom destroy         &wDestroy buildings within the kingdom.\n\r", ch );
    send_to_char( "\n\r", ch );
    send_to_char( "&CArmies\n\r", ch );
    send_to_char( "        &WSyntax: kingdom recruit         &wRecruit available troops in an area.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom disband         &wDisband a unit and release the troops back into the area pool.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom assign          &wCreate units and assign them to areas.\n\r", ch );
    send_to_char( "\n\r", ch );
    send_to_char( "&CMagic\n\r", ch );
    send_to_char( "        &WSyntax: kingdom research        &wOrder scholars to research new spells and technologies.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom learn           &wLearn a spell or technology that has been researched.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom cast            &wOrder wizards to cast a spell.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom use             &wUse an item in the treasury.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom mpcharge        &wRestore kingdom mana.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom dispel          &wAttempt to dispel magical effects on the kingdom.\n\r", ch );
    send_to_char( "\n\r", ch );
    send_to_char( "&CMisc\n\r", ch );
    send_to_char( "        &WSyntax: kingdom audience        &wRequest an audience with kingdom heroes.\n\r", ch );
    send_to_char( "        &WSyntax: kingdom tax             &wTax the kingdom's citizens.\n\r\n\r", ch );
    return;
  }

  if( !str_cmp(arg1, "status") )
  {
    show_kingdom_status(ch);
    return;
  }

  if( !str_cmp(arg1, "settings") )
  {
    if(  NULLSTR(arg2) )
    {
      send_to_char( "Syntax: kingdom settings <command> [setting]\n\r", ch );
      send_to_char( "Setting      Description\n\r------------------------\n\r", ch );
      if( rank >= KRANK_LEADER )
      {
        send_to_char( "name              Set the kingdom's name.\n\r"
                      "race              Set the kingdom's primary race.\n\r"
                      "whoname           Set the kingdom's name as shown on the who-list.\n\r"
                      "motto             Set the kingdom's motto.\n\r"
                      "description       Set the kingdom's description.\n\r"
	              "badge             Set the kingdom's badge.\n\r"
	              "url               Set the kingdom's homepage.\n\r"
                      "ruler             Set the kingdom's ruler.\n\r"
	              "leadrank          Set the ruler's rank.\n\r", ch );
      }

      if( rank >= KRANK_DEITY )
        send_to_char( "deity             Set the kingdom's deity.\n\r", ch );

      if( rank >= KRANK_NUMBER1 )
        send_to_char( "number1           Set the kingdom's number one.\n\r"
	              "onerank           Set the number one's rank.\n\r", ch );

      if( rank >= KRANK_NUMBER2 )
        send_to_char( "number2           Set the kingdom's number two.\n\r"
	              "tworank           Set the number two's rank.\n\r", ch );

      if( rank >= KRANK_NUMBER3 )
        send_to_char( "number3           Set the kingdom's number three.\n\r"
	              "threerank         Set the number three's rank.\n\r", ch );

      if( rank >= KRANK_NUMBER4 )
        send_to_char( "number4           Set the kingdom's number four.\n\r"
	              "fourrank          Set the number four's rank.\n\r", ch );

      if( rank >= KRANK_NUMBER5 )
        send_to_char( "number5           Set the kingdom's number five.\n\r"
	              "fiverank          Set the number five's rank.\n\r", ch );

      if( rank == KRANK_GENERAL || rank >= KRANK_NUMBER2 )
        send_to_char( "general           Set the kingdom's general.\n\r"
	              "genrank           Set the general's rank.\n\r", ch );
      return;
    }

    smash_tilde(arg2);
    if( !str_cmp(arg2, "name") )
    {
      send_to_char( "Please contact all current members before doing this. If you change the\n\r"
	            "Kingdom name, everyone, including yourself will no longer belong to a kingdom.\n\r"
		    "If you still wish to change the name after letting everyone know, please contact an Immortal.\n\r", ch );
      return;
    }

    if( !str_cmp(arg2, "race") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the kingdom's primary race to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      if( get_race_by_name(argument) == -1 )
      {
        send_to_char( "That is an invalid race.\n\r", ch );
        return;
      }

      ch_printf(ch, "Your kingdom's primary race is now: %s\n\r", (char *) race_table[get_race_by_name(argument)] );
      kingdom->race = get_race_by_name(argument);
      save_kingdom(kingdom);
      return;
    }
      
    if( !str_cmp(arg2, "whoname") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the kingdom whoname to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "Your kingdom's who name is now: %s\n\r", argument );
      stralloc_printf(&kingdom->whoname, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "motto") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the kingdom motto to what?\n\r", ch );
        return;

      }

      smash_tilde(argument);
      ch_printf(ch, "Your kingdom's motto is now: %s\n\r", argument );
      stralloc_printf(&kingdom->motto, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "description") || !str_cmp(arg2, "desc") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      smash_tilde(argument);
      if( ch->substate == SUB_REPEATCMD )
        ch->tempnum = SUB_REPEATCMD;
      else
        ch->tempnum = SUB_NONE;
      ch->substate = SUB_KINGDOM_DESC;
      ch->dest_buf = kingdom;
      start_editing(ch, kingdom->description);
      return;
    }

    if( !str_cmp(arg2, "badge") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the kingdom badge to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "Your kingdom's badge is now: %s\n\r", argument );
      stralloc_printf(&kingdom->badge, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "url") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the kingdom homepage to what?\n\r", ch );
        return;
      }
      smash_tilde(argument);
      ch_printf(ch, "Your kingdom's homepage is now: http://%s\n\r", argument );
      strdup_printf(&kingdom->url, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "ruler") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new ruler?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      send_to_char( "Ok...\n\r", ch );
      echo_all_printf( AT_WHITE, ECHOTAR_ALL,
        "&B[&WKINGDOM&B] &W%s has stepped down and appointed %s as the new ruler of the Kingdom of %s!\n\r",
        ch->name, argument, kingdom->name );
      stralloc_printf(&kingdom->ruler, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "leadrank") )
    {
      if( rank < KRANK_LEADER )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the ruler's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The ruler's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->leadrank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "deity") )
    {
      if( rank < KRANK_DEITY )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_DEITY )
          send_to_char( "&RWARNING: This will remove you from deityship!\n\r", ch );
        send_to_char( "&WWho will be the new deity?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new deity is now: %s\n\r", argument );
      stralloc_printf(&kingdom->deity, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "number1") )
    {
      if( rank < KRANK_NUMBER1 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_NUMBER1 )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new number one?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new number one is now: %s\n\r", argument );
      stralloc_printf(&kingdom->number1, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "onerank") )
    {
      if( rank < KRANK_NUMBER1 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number one's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number one's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->onerank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "number2") )
    {
      if( rank < KRANK_NUMBER2 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_NUMBER2 )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new number two?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new number two is now: %s\n\r", argument );
      stralloc_printf(&kingdom->number2, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "tworank") )
    {
      if( rank < KRANK_NUMBER2 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number two's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number two's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->tworank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "number3") )
    {
      if( rank < KRANK_NUMBER3 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_NUMBER3 )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new number three?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new number three is now: %s\n\r", argument );
      stralloc_printf(&kingdom->number3, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "threerank") )
    {
      if( rank < KRANK_NUMBER3 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number three's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number three's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->threerank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "number4") )
    {
      if( rank < KRANK_NUMBER4 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_NUMBER4 )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new number four?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new number four is now: %s\n\r", argument );
      stralloc_printf(&kingdom->number4, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "fourrank") )
    {
      if( rank < KRANK_NUMBER1 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number four's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number four's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->fourrank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "number5") )
    {
      if( rank < KRANK_NUMBER5 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_NUMBER5 )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new number five?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new number five is now: %s\n\r", argument );
      stralloc_printf(&kingdom->number5, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "fiverank") )
    {
      if( rank < KRANK_NUMBER5 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number five's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number five's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->fiverank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "general") )
    {
      if( rank != KRANK_GENERAL && rank < KRANK_NUMBER2 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        if( rank == KRANK_GENERAL )
          send_to_char( "&RWARNING: This will remove you from leadership!\n\r", ch );
        send_to_char( "&WWho will be the new general\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The new general is now: %s\n\r", argument );
      stralloc_printf(&kingdom->general, "%s", argument );
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg2, "genrank") )
    {
      if( rank != KRANK_GENERAL &&  rank < KRANK_NUMBER2 )
      {
        send_to_char( "You do not have permission to do that!\n\r", ch );
        return;
      }

      if( NULLSTR(argument) )
      {
        send_to_char( "Set the number general's rank to what?\n\r", ch );
        return;
      }

      smash_tilde(argument);
      ch_printf(ch, "The number general's rank is now: %s\n\r", argument );
      stralloc_printf(&kingdom->genrank, "%s", argument );
      save_kingdom(kingdom);
      return;
    }
  }

  if( !str_cmp(arg1, "ally") )
  {
    KINGDOM_DATA *target;

    if( rank != KRANK_GENERAL && rank < KRANK_LEADER )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }
    if( NULLSTR(arg2) )
    {
      send_to_char( "Ally with which kingdom?\n\r", ch );
      if( !NULLSTR(kingdom->allywith) )
        ch_printf(ch, "Your kingdom is allied with the kingdom of %s.\n\r"
                      "To end the alliance, type: &Rkingdom ally end\n\r", kingdom->allywith );
      return;
    }

    smash_tilde(arg2);
    if( !str_cmp(arg2, "end") )
    {
      if( NULLSTR(kingdom->allywith) )
      {
        send_to_char( "Your kingdom has no allies.\n\r", ch );
        return;
      }

      send_to_char( "Ok.\n\r", ch );
      echo_all_printf( AT_WHITE, ECHOTAR_ALL,
        "&B[&WKINGDOM&B] &WThe kingdom of %s has ended the alliance with the kingdom of %s!\n\r",
        kingdom->name, kingdom->allywith );        
      stralloc_printf(&kingdom->allywith, "%s", "" );
      save_kingdom(kingdom);
      return;
    }

    target = get_kingdom(arg2);
    if( !target )
    {
      send_to_char( "No such kingdom.\n\r", ch );
      return;
    }

    if( !str_cmp(kingdom->allywith, target->name) )
    {
      send_to_char( "Your kingdom already has an alliance with them.\n\r", ch );
      return;
    }

    // End war and declare ally
    if( !str_cmp(kingdom->warwith, target->name) )
    {
      if( !str_cmp(target->warwith, kingdom->name) )
      {
        send_to_char( "Try more peace talks. Declaring them as an ally while they are still"
                      " at war with you will leave you open to attack!\n\r", ch );
        return;
      }

      send_to_char( "Ok.\n\r", ch );
      echo_all_printf( AT_WHITE, ECHOTAR_ALL,
        "&B[&WKINGDOM&B] &WThe kingdom of %s has ended the war with the kingdom of %s and declared them as allies!\n\r",
        kingdom->name, target->name ); 
      stralloc_printf(&kingdom->warwith, "%s", "" );
      stralloc_printf(&kingdom->allywith, "%s", target->name );
      save_kingdom(kingdom);
      return;
    }

    echo_all_printf( AT_WHITE, ECHOTAR_ALL,
      "&B[&WKINGDOM&B] &WThe kingdom of %s has declared the kingdom of %s an ally!\n\r", kingdom->name, target->name );
    send_to_char( "Ok.\n\r", ch );
    stralloc_printf(&kingdom->allywith, "%s", target->name );
    save_kingdom(kingdom);
    return;
  }
  
  if( !str_cmp(arg1, "war") )
  {
    KINGDOM_DATA *target;

    if( rank != KRANK_GENERAL && rank < KRANK_LEADER )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }
    if( NULLSTR(arg2) )
    {
      send_to_char( "Declare war on which kingdom?\n\r", ch );
      if( !NULLSTR(kingdom->warwith) )
        ch_printf(ch, "Your kingdom is at war with the kingdom of %s.\n\r"
                      "To end the war, type: &Rkingdom war end\n\r", kingdom->warwith );
      return;
    }

    smash_tilde(arg2);
    if( !str_cmp(arg2, "end") )
    {
      if( NULLSTR(kingdom->warwith) )
      {
        send_to_char( "Your kingdom is not at war.\n\r", ch );
        return;
      }

      echo_all_printf( AT_WHITE, ECHOTAR_ALL,
        "&B[&WKINGDOM&B] &WThe kingdom of %s has ended the war with the kingdom of %s!\n\r",
        kingdom->name, kingdom->warwith );
      send_to_char( "Ok.\n\r", ch );
      stralloc_printf(&kingdom->warwith, "%s", "" );
      save_kingdom(kingdom);
      return;
    }

    target = get_kingdom(arg2);
    if( !target )
    {
      send_to_char( "No such kingdom.\n\r", ch );
      return;
    }

    if( !str_cmp(kingdom->warwith, target->name) )
    {
      send_to_char( "Your kingdom is already at war with them.\n\r", ch );
      return;
    }

    // End alliance and declare war
    if( !str_cmp(kingdom->allywith, target->name) )
    {
      // No check to prevent declaring war with someone allied with your kingdom. Encourages backstabbing
      echo_all_printf( AT_WHITE, ECHOTAR_ALL,
        "&B[&WKINGDOM&B] &WThe kingdom of %s has broken the alliance with the kingdom of %s and declared war on them!\n\r",
        kingdom->name, target->name );
      send_to_char( "Ok.\n\r", ch );
      stralloc_printf(&kingdom->allywith, "%s", "" );
      stralloc_printf(&kingdom->warwith, "%s", target->name );
      // We do however, remove the alliance the target may have had since declarations of war are unfriendly
      stralloc_printf(&target->allywith, "%s", kingdom->name );
      save_kingdom(kingdom);
      return;
    }

    echo_all_printf( AT_WHITE, ECHOTAR_ALL,
      "&B[&WKINGDOM&B] &WThe kingdom of %s has declared war on the kingdom of %s!\n\r", kingdom->name, target->name );
    send_to_char( "Ok.\n\r", ch );
    stralloc_printf(&kingdom->warwith, "%s", target->name );
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg1, "areas") )
  {
    KGDM_AREA *area;
    char buf2[MAX_STRING_LENGTH];

    if( rank < KRANK_NUMBER5 )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }
    
    if( NULLSTR(arg2) )
    {
      show_kareas(ch, kingdom, FALSE);
      return;
    }

    if( !str_cmp(arg2, "all") )
    {
      show_kareas(ch, kingdom, TRUE);
      return;
    }

    for( area = first_karea; area; area = area->next )
    {
      if( !str_cmp(arg2, area->name) )
        break;
    }

    if( !area )
    {
      send_to_char( "No such area exists in your kingdom.\n\r", ch );
      return;
    }

    send_to_char( "              Area Information\n\r", ch );
    send_to_char( "|--------------------------------------------|\n\r", ch );
    ch_printf(ch, "| Name: %37s |\n\r", !NULLSTR(area->name) ? area->name : "**BUG**" );
    send_to_char( "|--------------------------------------------|\n\r", ch );
    ch_printf(ch, "|Population: %13s  Mood: %6s (%d%%)|\n\r",
      num_punct(area->population), get_area_status(area), area->mood );
    send_to_char( "|                                            |\n\r", ch );
    snprintf(buf, MAX_STRING_LENGTH, "Land: %s/", num_punct(used_land(area)) ); // <--| stupid num_punct prints garbage
    snprintf(buf2, MAX_STRING_LENGTH, "%s", num_punct(area->land) );            // <--| when called twice from the same buffer
    mudstrlcat(buf, buf2, MAX_STRING_LENGTH);
    ch_printf(ch, "|%44s|\n\r", color_align(buf, 44, ALIGN_CENTER) );
    send_to_char( "|Stockpiles               Production         |\n\r", ch );
    ch_printf(ch, "|Gold:  %11s       ", num_punct(area->gold[2]) );
    ch_printf(ch, "%7s/day        |\n\r", num_punct(daily_production(area, KCOST_GOLD)) );
    ch_printf(ch, "|Food:  %11s       ", num_punct(area->food[2]) );
    ch_printf(ch, "%7s/day        |\n\r", num_punct(daily_production(area, KCOST_FOOD)) );
    ch_printf(ch, "|Wood:  %11s       ", num_punct(area->wood[2]) );
    ch_printf(ch, "%7s/day        |\n\r", num_punct(daily_production(area, KCOST_WOOD)) );
    ch_printf(ch, "|Stone: %11s       ", num_punct(area->stone[2]) );
    ch_printf(ch, "%7s/day        |\n\r", num_punct(daily_production(area, KCOST_STONE)));
    send_to_char( "|                                            |\n\r", ch );
    ch_printf(ch, "|Towns    : %7s     ", num_punct(area->towns) );
    ch_printf(ch, "Guilds  : %7s    |\n\r", num_punct(area->guilds) );
    ch_printf(ch, "|Farms    : %7s     ", num_punct(area->farms) );
    ch_printf(ch, "Nodes   : %7s    |\n\r", num_punct(area->nodes) );
    ch_printf(ch, "|Workshops: %7s     ", num_punct(area->workshops) );
    ch_printf(ch, "Barriers: %7s    |\n\r", num_punct(area->barriers) );
    ch_printf(ch, "|Barracks : %7s     ", num_punct(area->barracks) );
    ch_printf(ch, "Fortress: %7s    |\n\r", num_punct(area->fortresses) );
    send_to_char( "|--------------------------------------------|\n\r", ch );

    if( rank == KRANK_GENERAL || rank >= KRANK_NUMBER2 )
    {
      send_to_char( "           Millitary Information\n\r", ch );
      send_to_char( "|------------------|--------------------------|\n\r", ch );
      send_to_char( "|Recruitable Troops| Recruit speed   Availible|\n\r", ch );
      send_to_char( "|------------------|--------------------------|\n\r", ch );
      if( !NULLSTR(area->troop1) )
        ch_printf(ch, "| %17s | %3d/day         %7s  |\n\r", area->troop1, area->rec_spd1, num_punct(area->num_troops1) );
      if( !NULLSTR(area->troop2) )
        ch_printf(ch, "| %17s | %3d/day         %7s  |\n\r", area->troop2, area->rec_spd2, num_punct(area->num_troops2) );
      if( !NULLSTR(area->troop3) )
        ch_printf(ch, "| %17s | %3d/day         %7s  |\n\r", area->troop3, area->rec_spd3, num_punct(area->num_troops3) );
      send_to_char( "|------------------|--------------------------|\n\r", ch );
      send_to_char( "         Units assigned to this area\n\r", ch );
      send_to_char( "|---------------------------------------------|\n\r", ch );

      if( area->first_unit )
      {
        UNIT_DATA *unit;
        SUBUNIT_DATA *troop;
        short cnt = 0;

        for( unit = area->first_unit; unit; unit = unit->next )
        {
          ch_printf(ch, "| Commander: %14s (Lv. %2d %9s)|\n\r", !NULLSTR(unit->commander) ? unit->commander : "**BUG**",
            unit->commander_level, get_hero_class(unit->commander_class) );
          if( unit->first_troop )
          {
            for( troop = unit->first_troop; troop; troop = troop->next )
            {
              cnt++;
              ch_printf(ch, "| %d> %6s %34s|\n\r", cnt, num_punct(troop->number), !NULLSTR(troop->name) ? troop->name : "**BUG**" );
            }
          }
          send_to_char( "|                                             |\n\r", ch );
        }
        send_to_char( "|---------------------------------------------|\n\r", ch );
      }
    }
  }

  if( !str_cmp(arg1, "conquer") )
  {
    KGDM_AREA *area;

    if( rank != KRANK_GENERAL && rank < KRANK_LEADER )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }

    if( NULLSTR(arg2) )
    {
      send_to_char( "Which area do you wish to conquer?\n\r", ch );
      return;
    }

    for( area = first_karea; area; area = area->next )
    {
      if( !str_cmp(arg2, area->name) )
        break;
    }

    if( !area )
    {
      send_to_char( "No such area.\n\r", ch );
      return;
    }

    if( !NULLSTR(area->owner) )
    {
      send_to_char( "You cannot conquer an area owned by another kingdom. Attack them and take it!\n\r", ch );
      return;
    }

    if( !str_cmp(area->owner, kingdom->name) )
    {
      send_to_char( "Your kingdom already has dominion over that area.\n\r", ch );
      return;
    }

    echo_all_printf( AT_WHITE, ECHOTAR_ALL,
      "&B[&WKINGDOM&B] &WThe kingdom of %s has conquered %s and claimed dominion over its land!\n\r",
      kingdom->name, area->name );
    send_to_char( "Ok.\n\r", ch );
    add_karea(area, kingdom);
    save_kingdom(kingdom);
    return;
  }
  
  if( !str_cmp(arg1, "relinquish") )
  {
    KGDM_AREA *area;

    if( rank != KRANK_GENERAL && rank < KRANK_LEADER )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }

    if( NULLSTR(arg2) )
    {
      send_to_char( "Which area do you wish to relinquish control of?\n\r", ch );
      return;
    }

    for( area = kingdom->first_karea; area; area = area->next )
    {
      if( !str_cmp(arg2, area->name) )
        break;
    }

    if( !area )
    {
      send_to_char( "No such area.\n\r", ch );
      return;
    }

    echo_all_printf( AT_WHITE, ECHOTAR_ALL,
      "&B[&WKINGDOM&B] &WThe kingdom of %s has relinquished control of %s!\n\r", kingdom->name, area->name );
    send_to_char( "Ok.\n\r", ch );
    remove_karea(area, kingdom);
    save_kingdom(kingdom);
    return;
  }

  if( !str_cmp(arg1, "build") )
  {
    KGDM_AREA *area;
    int value;

    if( rank != KRANK_GENERAL && rank < KRANK_NUMBER2 )
    {
      send_to_char( "You do not have permission to do that!\n\r", ch );
      return;
    }

    if( NULLSTR(arg2) )
    {
      send_to_char( "Which area do you wish to build in?\n\r", ch );
      return;
    }

    for( area = kingdom->first_karea; area; area = area->next )
    {
      if( !str_cmp(arg2, area->name) )
        break;
    }

    if( !area )
    {
      send_to_char( "No such area.\n\r", ch );
      return;
    }

    if( NULLSTR(arg3) )
    {
      send_to_char( "What do you wish to build?\n\r", ch );
      send_to_char( "You may build the following: town farm guild node barrier fortress workshop", ch );
      if( rank >= KRANK_GENERAL )
        send_to_char( "barracks", ch );
      send_to_char( "\n\r", ch );
      return;
    }

    if( !str_cmp(arg3, "town") || !str_cmp(arg3, "towns") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many towns do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s towns and %s available land.\n\r",
          area->name, num_punct(area->towns), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_TOWN, value) )
        return;

      ch_printf(ch, "You oversee the building of %s towns in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_TOWN, area, value)) );
      if( build_cost(BUILDING_TOWN, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_TOWN, KCOST_GOLD, value)) );
 
      if( build_cost(BUILDING_TOWN, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_TOWN, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_TOWN, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_TOWN, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_TOWN, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_TOWN, KCOST_MANA, value)) );

      if( build_cost(BUILDING_TOWN, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_TOWN, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_TOWN, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "farm") || !str_cmp(arg3, "farms") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many farms do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s farms and %s available land.\n\r",
          area->name, num_punct(area->farms), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_FARM, value) )
        return;

      ch_printf(ch, "You oversee the building of %s farms in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_FARM, area, value)) );
      if( build_cost(BUILDING_FARM, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_FARM, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_FARM, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_FARM, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_FARM, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_FARM, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_FARM, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_FARM, KCOST_MANA, value)) );

      if( build_cost(BUILDING_FARM, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_FARM, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_FARM, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "guild") || !str_cmp(arg3, "guilds") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many guilds do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s guilds and %s available land.\n\r",
          area->name, num_punct(area->guilds), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_GUILD, value) )
        return;

      ch_printf(ch, "You oversee the building of %s guilds in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_GUILD, area, value)) );

      if( build_cost(BUILDING_GUILD, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_GUILD, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_GUILD, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_GUILD, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_GUILD, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_GUILD, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_GUILD, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_GUILD, KCOST_MANA, value)) );

      if( build_cost(BUILDING_GUILD, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_GUILD, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_GUILD, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "node") || !str_cmp(arg3, "nodes") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many nodes do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s nodes and %s available land.\n\r",
          area->name, num_punct(area->nodes), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_NODE, value) )
        return;

      ch_printf(ch, "You oversee the building of %s nodes in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_NODE, area, value)) );

      if( build_cost(BUILDING_NODE, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_NODE, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_NODE, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_NODE, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_NODE, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_NODE, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_NODE, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_NODE, KCOST_MANA, value)) );

      if( build_cost(BUILDING_NODE, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_NODE, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_NODE, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "barrier") || !str_cmp(arg3, "barriers") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many barriers do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s barriers and %s available land.\n\r",
          area->name, num_punct(area->barriers), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build( ch, kingdom, BUILDING_BARRIER, value ) )
        return;

      ch_printf(ch, "You oversee the building of %s barriers in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_BARRIER, area, value)) );

      if( build_cost(BUILDING_BARRIER, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_BARRIER, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_BARRIER, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_BARRIER, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_BARRIER, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_BARRIER, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_BARRIER, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_BARRIER, KCOST_MANA, value)) );

      if( build_cost(BUILDING_BARRIER, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_BARRIER, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_BARRIER, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "barrack") || !str_cmp(arg3, "barracks") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many barracks do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s barracks and %s available land.\n\r",
          area->name, num_punct(area->barracks), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_BARRACKS, value) )
        return;

      ch_printf(ch, "You oversee the building of %s barracks in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_BARRACKS, area, value)) );

      if( build_cost(BUILDING_BARRACKS, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_BARRACKS, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_BARRACKS, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_BARRACKS, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_BARRACKS, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_BARRACKS, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_BARRACKS, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_BARRACKS, KCOST_MANA, value)) );

      if( build_cost(BUILDING_BARRACKS, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_BARRACKS, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_BARRACKS, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "workshop") || !str_cmp(arg3, "workshops") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many workshops do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s workshops and %s available land.\n\r",
          area->name, num_punct(area->workshops), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_WORKSHOP, value) )
        return;

      ch_printf(ch, "You oversee the building of %s workshops in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_WORKSHOP, area, value)) );

      if( build_cost(BUILDING_WORKSHOP, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_WORKSHOP, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_WORKSHOP, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_WORKSHOP, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_WORKSHOP, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_WORKSHOP, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_WORKSHOP, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_WORKSHOP, KCOST_MANA, value)) );

      if( build_cost(BUILDING_WORKSHOP, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_WORKSHOP, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_WORKSHOP, value);
      save_kingdom(kingdom);
      return;
    }

    if( !str_cmp(arg3, "fortress") || !str_cmp(arg3, "fortresses") )
    {
      if( NULLSTR(argument) )
      {
        send_to_char( "How many fortresses do you wish to build?\n\r", ch );
        ch_printf(ch, "%s has %s fortresses and %s available land.\n\r",
          area->name, num_punct(area->fortresses), num_punct(availible_land(area)) );
        return;
      }

      value = atoi(argument);
      if( value > availible_land(area) )
      {
        send_to_char( "There isn't enough availble land to build that many.\n\r", ch );
        return;
      }

      if( !can_build(ch, kingdom, BUILDING_FORTRESS, value) )
        return;

      ch_printf(ch, "You oversee the building of %s fortresses in %s.\n\r", num_punct(value), area->name );
      ch_printf(ch, "It will take %s days to finish construction at a total cost of:\n\r",
        num_punct(build_days(BUILDING_FORTRESS, area, value)) );

      if( build_cost(BUILDING_FORTRESS, KCOST_GOLD, value) > 0 )
        ch_printf(ch, "%s gold", num_punct(build_cost(BUILDING_FORTRESS, KCOST_GOLD, value)) );

      if( build_cost(BUILDING_FORTRESS, KCOST_FOOD, value) > 0 )
        ch_printf(ch, "%s food", num_punct(build_cost(BUILDING_FORTRESS, KCOST_FOOD, value)) );

      if( build_cost(BUILDING_FORTRESS, KCOST_WOOD, value) > 0 )
        ch_printf(ch, "%s wood", num_punct(build_cost(BUILDING_FORTRESS, KCOST_WOOD, value)) );

      if( build_cost(BUILDING_FORTRESS, KCOST_MANA, value) > 0 )
        ch_printf(ch, "%s mana", num_punct(build_cost(BUILDING_FORTRESS, KCOST_MANA, value)) );

      if( build_cost(BUILDING_FORTRESS, KCOST_STONE, value) > 0 )
        ch_printf(ch, "%s stone", num_punct(build_cost(BUILDING_FORTRESS, KCOST_STONE, value)) );

      send_to_char( "\n\r", ch );
      build_kingdom(kingdom, area, BUILDING_FORTRESS, value);
      save_kingdom(kingdom);
      return;
    }
  }
  
  if( !str_cmp(arg1, "destroy") )
  {
  }
  
  if( !str_cmp(arg1, "recruit") )
  {
  }
  
  if( !str_cmp(arg1, "disband") )
  {
  }
  
  if( !str_cmp(arg1, "audience") )
  {
  }
  
  if( !str_cmp(arg1, "research") )
  {
  }
  
  if( !str_cmp(arg1, "learn") )
  {
  }
  
  if( !str_cmp(arg1, "cast") )
  {
  }
  
  if( !str_cmp(arg1, "use") )
  {
  }
  
  if( !str_cmp(arg1, "mpcharge") )
  {
  }
  
  if( !str_cmp(arg1, "assign") )
  {
  }
  
  if( !str_cmp(arg1, "dispel") )
  {
  }
  
  if( !str_cmp(arg1, "tax") )
  {
  }
  return;
}


KGDM_AREA *get_kingdom_area( char *name )
{
  KGDM_AREA *area;

  for( area = first_karea; area; area = area->next )
  {
    if( !str_cmp(name, area->name) )
      return area;
  }
  return NULL;
}


void write_karea_list( void )
{
  KGDM_AREA *tarea;
  FILE *fpout;
  char filename[256];

  snprintf(filename, 256, "%s%s", KGDM_AREA_DIR, KGDM_AREA_LIST );
  if( !(fpout = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    return;
  }

  for( tarea = first_karea; tarea; tarea = tarea->next )
    fprintf(fpout, "%s\n", tarea->filename );

  fprintf(fpout, "$\n" );
  FCLOSE(fpout);
}


void save_unit( UNIT_DATA *unit, FILE *fp )
{
  if( !fp )
    return;

  fprintf(fp, "%s", "#UNIT\n" );
  fprintf(fp, "Commander      %s~\n",	unit->commander );
  fprintf(fp, "Level          %d\n",	unit->commander_level );
  fprintf(fp, "Class          %d\n",	unit->commander_class );
  if( unit->first_troop )
  {
    SUBUNIT_DATA *troop;

    for( troop = unit->first_troop; troop; troop = troop->next )
    {
      fprintf(fp, "#SUBUNIT\n" );
      fprintf(fp, "Name           %s~\n", troop->name 	);
      fprintf(fp, "Number         %d\n",  troop->number	);
      fprintf(fp, "EndSubUnit\n" );
    }
  }

  fprintf(fp, "EndUnit\n" );
  return;
}


/* Save an areas's data to its data file */
void save_karea( KGDM_AREA *area )
{
  FILE *fp;
  char filename[256];

  if( !area )
  {
    bug( "%s: NULL area!", __FUNCTION__ );
    return;
  }

  if( NULLSTR(area->filename) )
  {
    bug( "%s: %s has no filename!", __FUNCTION__, area->name );
    return;
  }

  snprintf(filename, 256, "%s%s", KGDM_AREA_DIR, area->filename );
  if( !(fp = fopen(filename, "w")) )
  {
    bug( "%s: Couldn't open %s for writing!", __FUNCTION__, filename );
    perror(filename);
  }
  else
  {
    fprintf(fp, "#KGDMAREA\n" );
    fprintf(fp, "Name              %s~\n", area->name		);
    fprintf(fp, "Filename          %s~\n", area->filename	);
    fprintf(fp, "Owner             %s~\n", area->owner		);
    fprintf(fp, "Gold0             %d\n", area->gold[0] 	);
    fprintf(fp, "Gold1             %d\n", area->gold[1] 	);
    fprintf(fp, "Gold2             %d\n", area->gold[2] 	);
    fprintf(fp, "Food0             %d\n", area->food[0] 	);
    fprintf(fp, "Food1             %d\n", area->food[1] 	);
    fprintf(fp, "Food2             %d\n", area->food[2] 	);
    fprintf(fp, "Wood0             %d\n", area->wood[0] 	);
    fprintf(fp, "Wood1             %d\n", area->wood[1] 	);
    fprintf(fp, "Wood2             %d\n", area->wood[2] 	);
    fprintf(fp, "Stone0            %d\n", area->stone[0] 	);
    fprintf(fp, "Stone1            %d\n", area->stone[0] 	);
    fprintf(fp, "Stone2            %d\n", area->stone[0] 	);
    fprintf(fp, "Land              %d\n", area->land    	);
    fprintf(fp, "Building1         %d\n", area->towns		);
    fprintf(fp, "Building2         %d\n", area->farms		);
    fprintf(fp, "Building3         %d\n", area->barracks	);
    fprintf(fp, "Building4         %d\n", area->guilds		);
    fprintf(fp, "Building5         %d\n", area->barriers	);
    fprintf(fp, "Building6         %d\n", area->fortresses	);
    fprintf(fp, "Building7         %d\n", area->nodes		);
    fprintf(fp, "Building8         %d\n", area->workshops	);
    fprintf(fp, "Population        %d\n", area->population	);
    fprintf(fp, "Mood              %d\n", area->mood		);
    fprintf(fp, "Troop1            %s~\n", area->troop1		);
    fprintf(fp, "Troop2            %s~\n", area->troop2		);
    fprintf(fp, "Troop3            %s~\n", area->troop3		);
    fprintf(fp, "RecTimer1         %d\n", area->rec_timer1 	);
    fprintf(fp, "RecTimer2         %d\n", area->rec_timer2 	);
    fprintf(fp, "RecTimer3         %d\n", area->rec_timer3 	);
    fprintf(fp, "RecSpeed1         %d\n", area->rec_spd1  	);
    fprintf(fp, "RecSpeed2         %d\n", area->rec_spd2  	);
    fprintf(fp, "RecSpeed3         %d\n", area->rec_spd3  	);
    fprintf(fp, "RecNumber1        %d\n", area->rec_num1   	);
    fprintf(fp, "RecNumber2        %d\n", area->rec_num2   	);
    fprintf(fp, "RecNumber3        %d\n", area->rec_num3   	);
    fprintf(fp, "NumTroops1        %d\n", area->num_troops1 	);
    fprintf(fp, "NumTroops2        %d\n", area->num_troops1 	);
    fprintf(fp, "NumTroops3        %d\n", area->num_troops1 	);
    if( area->first_unit )
    {
      UNIT_DATA *unit;

      for( unit = area->first_unit; unit; unit = unit->next )
        save_unit(unit, fp);
    }
    fprintf(fp, "End\n\n"                                  	);
    fprintf(fp, "#END\n"                                   	);
  }

  FCLOSE(fp);
  return;
}


/* Read in actual area data. */
void fread_karea( KGDM_AREA *area, FILE *fp )
{
  const char *word;
  bool fMatch;

  for( ; ; )
  {
    word = feof(fp) ? "End" : fread_word(fp);
    fMatch = FALSE;
    switch(UPPER(word[0]))
    {
      case '*':
        fMatch = TRUE;
        fread_to_eol(fp);
        break;

      case 'B':
        KEY( "Building1",	area->towns,		fread_number(fp) 	);
        KEY( "Building2",	area->farms,		fread_number(fp) 	);
	KEY( "Building3",	area->barracks,		fread_number(fp)	);
	KEY( "Building4",	area->guilds,		fread_number(fp) 	);
	KEY( "Building5",	area->barriers,		fread_number(fp) 	);
	KEY( "Building6",	area->fortresses,	fread_number(fp) 	);
	KEY( "Building7",	area->nodes,		fread_number(fp) 	);
	KEY( "Building8",	area->workshops,	fread_number(fp) 	);
  	break;

      case 'E':
        if( !str_cmp(word, "End" ) )
        {
          nstrdup(&area->name);
          nstralloc(&area->troop1);
          nstralloc(&area->troop2);
          nstralloc(&area->troop3);
          return;
        }
        break;

      case 'F':
        KEY( "Filename",    	area->filename,  	fread_string_nohash(fp) );
        KEY( "Food0",		area->food[0],		fread_number(fp) 	);
        KEY( "Food1",		area->food[1],		fread_number(fp) 	);
        KEY( "Food2",		area->food[2],		fread_number(fp) 	);
        break;

      case 'G':
        KEY( "Gold0",		area->gold[0],		fread_number(fp) 	);
        KEY( "Gold1",		area->gold[1],		fread_number(fp) 	);
        KEY( "Gold2",		area->gold[2],		fread_number(fp) 	);
        break;

      case 'L':
        KEY( "Land",		area->land,		fread_number(fp) 	);
        break;

      case 'M':
        KEY( "Mood",		area->mood,		fread_number(fp) 	);
        break;

      case 'N':
        KEY( "Name",        	area->name,           	fread_string_nohash(fp) );
        KEY( "NumTroops1",	area->num_troops1,	fread_number(fp) 	);
        KEY( "NumTroops2",	area->num_troops2,	fread_number(fp) 	);
        KEY( "NumTroops3",	area->num_troops3,	fread_number(fp) 	);
        break;

      case 'O':
        KEY( "Owner",		area->owner,		fread_string(fp) 	);
        break;

      case 'P':
        KEY( "Population",  	area->population,     	fread_number(fp) 	);
        break;

      case 'R':
        KEY( "RecTimer1",	area->rec_timer1,	fread_number(fp) 	);
        KEY( "RecTimer2",	area->rec_timer2,	fread_number(fp) 	);
        KEY( "RecTimer3",	area->rec_timer3,	fread_number(fp) 	);
        KEY( "RecSpeed1",	area->rec_spd1,		fread_number(fp) 	);
        KEY( "RecSpeed2",	area->rec_spd2,		fread_number(fp) 	);
        KEY( "RecSpeed3",	area->rec_spd3,		fread_number(fp) 	);
        KEY( "RecNumber1",	area->rec_num1,		fread_number(fp) 	);
        KEY( "RecNumber2",	area->rec_num2,		fread_number(fp) 	);
        KEY( "RecNumber3",	area->rec_num3,		fread_number(fp) 	);
        break;

      case 'S':
        KEY( "Stone0",		area->stone[0],		fread_number(fp) 	);
        KEY( "Stone1",		area->stone[1],		fread_number(fp) 	);
        KEY( "Stone2",		area->stone[2],		fread_number(fp) 	);
        break;

      case 'T':
        KEY( "Troop1",		area->troop1,		fread_string(fp) 	);
        KEY( "Troop2",		area->troop2,		fread_string(fp) 	);
        KEY( "Troop3",		area->troop3,		fread_string(fp) 	);
        break;

      case 'W':
        KEY( "Wood0",		area->wood[0],		fread_number(fp) 	);
        KEY( "Wood1",		area->wood[1],		fread_number(fp) 	);
        KEY( "Wood2",		area->wood[2],		fread_number(fp) 	);
        break;
    }

    if( !fMatch )
      bug( "%s: No match: %s", __FUNCTION__, word );
  }
}


/* Load a area file */
bool load_karea_file( const char *areafile )
{
  char filename[256];
  KGDM_AREA *area;
  FILE *fp;
  bool found = FALSE;

  CREATE( area, KGDM_AREA, 1 );
  snprintf(filename, 256, "%s%s", KGDM_AREA_DIR, areafile );
  if( (fp = fopen(filename, "r")) != NULL )
  {
    found = TRUE;
    for( ; ; )
    {
      char letter;
      const char *word;

      letter = fread_letter(fp);
      if( letter == '*' )
      {
        fread_to_eol(fp);
        continue;
      }

      if( letter != '#' )
      {
        bug( "%s: # not found.", __FUNCTION__ );
        break;
      }

      word = fread_word(fp);
      if( !str_cmp(word, "KGDMAREA") )
      {
        fread_karea(area, fp);
        break;
      }
      else if( !str_cmp(word, "END") )
        break;
      else
      {
        bug( "%s: Bad section: %s.", __FUNCTION__, word );
        break;
      }
    }
    FCLOSE(fp);
  }

  if( found )
    LINK( area, first_karea, last_karea, next, prev );
  else
    DISPOSE(area );
  return found;
}


/* Load in all the area files. */
void load_kareas( void )
{
  FILE *fpList;
  const char *filename;
  char arealist[256];

  first_karea = NULL;
  last_karea = NULL;
  log_string( "Loading Kingdom Areas..." );
  snprintf(arealist, 256, "%s%s", KGDM_AREA_DIR, KGDM_AREA_LIST );
  if( !(fpList = fopen(arealist, "r")) )
  {
    bug( "%s: Couldn't open %s for reading!", __FUNCTION__, arealist );
    perror(arealist);
    exit(1);
  }

  for( ; ; )
  {
    filename = feof(fpList) ? "$" : fread_word(fpList);
    if( filename[0] == '$' )
      break;

    if( !load_karea_file(filename) )
      bug( "%s: Couldn't load kingdom area file: %s", __FUNCTION__, filename );
  }

  FCLOSE(fpList);
  return;
}


void create_kingdom_area( AREA_DATA *area )
{
  KGDM_AREA *kArea, *new_area;
  char fname[256];
  int x;

  for( kArea = first_karea; kArea; kArea = kArea->next )
  {
    if( !str_cmp(area->name, kArea->name) )
    {
      bug( "%s: Duplicate kingdom area: %s. Ignoring.", __FUNCTION__, kArea->name );
      return;
    }
  }

  log_printf_plus( LOG_NORMAL, LEVEL_GOD, "Creating kingdom area for %s...", area->name );
  snprintf(fname, 256, "%s", area->filename );
  chopSuffix(fname);
  mudstrlcat(fname, ".kare", 256);
  CREATE( new_area, KGDM_AREA, 1 );
  LINK( new_area, first_karea, last_karea, next, prev );
  new_area->filename = str_dup(fname);
  new_area->name = str_dup(area->name);
  new_area->owner = STRALLOC("");
  for( x = 1; x < 2; x++ )
  {
    new_area->gold[x] = ((area->low_soft_range + area->hi_soft_range + area->low_hard_range + area->hi_hard_range) / 4);
    new_area->food[x] = ((area->low_r_vnum + area->hi_r_vnum) * 1000);
    new_area->wood[x] = ((area->low_r_vnum + area->hi_r_vnum) * 500);
    new_area->stone[x]= ((area->low_r_vnum + area->hi_r_vnum) * 250);
  }
  new_area->land	= (area->low_m_vnum + area->hi_m_vnum);
  new_area->population  = (area->low_m_vnum + area->hi_m_vnum);
  new_area->mood	= number_range(25, 75);
  new_area->troop1	= STRALLOC("");
  new_area->troop2	= STRALLOC("");
  new_area->troop3	= STRALLOC("");
  new_area->first_unit	= NULL;
  new_area->last_unit	= NULL;
  save_karea(new_area);
  return;
}


void destroy_kingdom_area( KGDM_AREA *area )
{
  log_printf_plus( LOG_NORMAL, LEVEL_GOD, "Destroying kingdom area %s...", area->name );
  UNLINK( area, first_karea, last_karea, next, prev );
  DISPOSE(area->filename);
  DISPOSE(area->name);
  STRFREE(area->owner);
  STRFREE(area->troop1);
  STRFREE(area->troop2);
  STRFREE(area->troop3);
  DISPOSE(area);
  return;
}


void check_kingdom_areas( void )
{
  AREA_DATA *area;
  KGDM_AREA *kArea;

  log_string( "Checking kingdom areas..." );
  // Loop through loaded areas and create kingdom area if one doesn't exist for it --Exo
  for( area = first_area; area; area = area->next )
  {
    kArea = get_kingdom_area(area->name);
    if( !kArea )
      create_kingdom_area(area);
  }

  // Loop through loaded kingdom areas and remove if it doesn't have a corresponding area --Exo
  for( kArea = first_karea; kArea; kArea = kArea->next )
  {
    area = get_area(kArea->name);
    if( !area )
      destroy_kingdom_area(kArea);
  }
  write_karea_list();
  return;
}

/*
void show_kmembers( CHAR_DATA *ch, char *argument, char *format )
{
  MEMBER_LIST *members_list;
  MEMBER_DATA *member;
  KINGDOM_DATA *kingdom;

  for( members_list = first_member_list; members_list; members_list = members_list->next )
  {
    if( !str_cmp(members_list->name, argument) )
      break;
  }

  if( !members_list )
    return;

  kingdom = get_kingdom(argument);
  if( !kingdom )
    return;

  pager_printf(ch, "\n\r&WMembers of &W%s\n\r", kingdom->whoname );
  pager_printf(ch, "&B------------------------------------------------------------\n\r" );
  pager_printf(ch, "&WDeity  : &w%s\n\r", kingdom->deity );
  pager_printf(ch, "&WRuler  : &w%s, %s\n\r", kingdom->ruler, kingdom->leadrank );
  pager_printf(ch, "&WNumber1: &w%s, %s\n\r", kingdom->number1, kingdom->onerank );
  pager_printf(ch, "&WNumber2: &w%s, %s\n\r", kingdom->number2, kingdom->tworank );
  pager_printf(ch, "&WNumber3: &w%s, %s\n\r", kingdom->number3, kingdom->threerank );
  pager_printf(ch, "&WNumber4: &w%s, %s\n\r", kingdom->number4, kingdom->fourrank );
  pager_printf(ch, "&WNumber5: &w%s, %s\n\r", kingdom->number5, kingdom->fiverank );
  pager_printf(ch, "&WGeneral: &w%s, %s\n\r", kingdom->general, kingdom->genrank );
  pager_printf(ch, "&B------------------------------------------------------------\n\r" );
  pager_printf(ch, "&CLvl  Name                 Class  Kills   Deaths        Since  Sworn in by\n\r\n\r" );

  if( format[0] != '\0' )
  {
    if( !str_cmp(format, "kills") || !str_cmp(format, "deaths") || !str_cmp(format, "alpha"))
    {
      MS_DATA *insert = NULL;
      MS_DATA *sort;
      MS_DATA *first_member = NULL;
      MS_DATA *last_member = NULL;

      CREATE( sort, MS_DATA, 1 );
      sort->member = members_list->first_member;
      LINK( sort, first_member, last_member, next, prev );
      for( member = members_list->first_member->next; member; member = member->next )
      {
        insert = NULL;
        for( sort = first_member; sort; sort = sort->next )
        {
          if( !str_cmp(format, "kills") )
          {
            if( member->kills > sort->member->kills )
            {
              CREATE( insert, MS_DATA, 1 );
              insert->member = member;
              INSERT( insert, sort, first_member, next, prev );
              break;
            }
          }
          else if( !str_cmp(format, "deaths") )
          {
            if( member->deaths > sort->member->deaths )
            {
              CREATE( insert, MS_DATA, 1 );
              insert->member = member;
              INSERT( insert, sort, first_member, next, prev );
              break;
            }
          }
          else if( !str_cmp(format, "alpha") )
          {
            if( strcmp(member->name, sort->member->name) < 0 )
            {
              CREATE( insert, MS_DATA, 1 );
              insert->member = member;
              INSERT( insert, sort, first_member, next, prev );
              break;
            }
          }
        }

        if( insert == NULL )
        {
          CREATE( insert, MS_DATA, 1 );
          insert->member = member;
          LINK( insert, first_member, last_member, next, prev );
        }
      }

      for( sort = first_member; sort; sort = sort->next )
      {
        if( str_cmp(sort->member->name, kingdom->ruler)
        && str_cmp(sort->member->name, kingdom->number1)
        && str_cmp(sort->member->name, kingdom->number2)
        && str_cmp(sort->member->name, kingdom->number3)
        && str_cmp(sort->member->name, kingdom->number4)
        && str_cmp(sort->member->name, kingdom->number5)
        && str_cmp(sort->member->name, kingdom->general)
        && str_cmp(sort->member->name, kingdom->deity) )
          pager_printf(ch, "&B[&W%2d&B] &W%-12s %13s %6d &R%8d &W%10s  &W%s\n\r",
            sort->member->level, capitalize(sort->member->name), class_table[sort->member->Class]->who_name,
            sort->member->kills, sort->member->deaths, sort->member->since,
            !NULLSTR(sort->member->inducted_by) ? sort->member->inducted_by : "Unknown" );
      }
    }

    for( member = members_list->first_member; member; member = member->next )
    {
      if( !str_prefix(format, member->name) )
        pager_printf(ch, "&B[&W%2d&B] &W%-12s %13s %6d &R%8d &W%10s  &W%s\n\r",
          member->level, capitalize(member->name), class_table[member->Class]->who_name,
          member->kills, member->deaths, member->since,
          !NULLSTR(member->inducted_by) ? member->inducted_by : "Unknown" );
    }
  }
  else
  {
    for( member = members_list->first_member; member; member = member->next )
    {
      if( str_cmp(member->name, kingdom->ruler)
      && str_cmp(member->name, kingdom->number1)
      && str_cmp(member->name, kingdom->number2)
      && str_cmp(member->name, kingdom->number3)
      && str_cmp(member->name, kingdom->number4)
      && str_cmp(member->name, kingdom->number5)
      && str_cmp(member->name, kingdom->general)
      && str_cmp(member->name, kingdom->deity) )
        pager_printf(ch, "&B[&W%2d&B] &W%-12s %13s %6d  &R%7d &W%10s  &W%s\n\r", member->level,
          capitalize(member->name), class_table[member->Class]->who_name,
          member->kills, member->deaths, member->since,
          !NULLSTR(member->inducted_by) ? member->inducted_by : "Unknown" );
    }
  }
  pager_printf( ch, "&B------------------------------------------------------------\n\r" );
}


void remove_kmember( CHAR_DATA *ch )
{
  MEMBER_LIST *members_list;
  MEMBER_DATA *member;

  if( !ch->pcdata )
    return;

  for( members_list = first_member_list; members_list; members_list = members_list->next )
  {
    if( !str_cmp(members_list->name, ch->pcdata->kingdom_name) )
      break;
  }

  if( !members_list )
    return;

  for( member = members_list->first_member; member; member = member->next )
  {
    if( !str_cmp(member->name, ch->name) )
    {
      UNLINK( member, members_list->first_member, members_list->last_member, next, prev );
      STRFREE(member->name);
      STRFREE(member->since);
      STRFREE(member->inducted_by);
      DISPOSE(member);
      save_member_list(members_list);
      break;
    }
  }
}
*/

void free_kareas( KINGDOM_DATA *kingdom )
{
  KGDM_AREA *area, *area_next;
  UNIT_DATA *unit, *unit_next;

  for( area = kingdom->first_karea; area; area = area_next )
  {
    area_next = area->next;

    UNLINK( area, kingdom->first_karea, kingdom->last_karea, next, prev );
    DISPOSE(area->filename);
    STRFREE(area->name);
    STRFREE(area->owner);
    STRFREE(area->troop1);
    STRFREE(area->troop2);
    STRFREE(area->troop3);
    for( unit = area->first_unit; unit; unit = unit_next )
    {
      unit_next = unit->next;

      UNLINK( unit, area->first_unit, area->last_unit, next, prev );
      STRFREE(unit->commander);
      DISPOSE(unit);
    }
  }
}


void free_one_kingdom( KINGDOM_DATA *kingdom )
{
//  MEMBER_LIST *members_list, *list_next;
//  MEMBER_DATA *member;
  UNIT_DATA *unit, *unit_next;
  SUBUNIT_DATA *sub, *sub_next;
  HERO_DATA *hero, *hero_next;

/*
  for( members_list = first_member_list; members_list; members_list = list_next )
  {
    list_next = members_list->next;
    if( !str_cmp(kingdom->name, members_list->name) )
    {
      while( members_list->first_member )
      {
        member = members_list->first_member;
        UNLINK( member, members_list->first_member, members_list->last_member, next, prev );
        STRFREE(member->name);
        STRFREE(member->since);
        STRFREE(member->inducted_by);
        DISPOSE(member);
      }
      UNLINK( members_list, first_member_list, last_member_list, next, prev );
      STRFREE(members_list->name);
      DISPOSE(members_list);
    }
  }
*/
  // Get rid of units
  for( unit = kingdom->first_unit; unit; unit = unit_next )
  {
    unit_next = unit->next;

    UNLINK( unit, kingdom->first_unit, kingdom->last_unit, next, prev );
    STRFREE(unit->commander);

    for( sub = unit->first_troop; sub; sub = sub_next )
    {
      sub_next = sub->next;

      UNLINK( sub, unit->first_troop, unit->last_troop, next, prev ); // Get rid of their troops
      STRFREE(sub->name);
      DISPOSE(sub);
    }
    DISPOSE(unit);
  }

  for( hero = kingdom->first_hero; hero; hero = hero_next ) // axe the heroes
  {
    hero_next = hero->next;

    UNLINK( hero, kingdom->first_hero, kingdom->last_hero, next, prev );
    DISPOSE(hero->filename);
    STRFREE(hero->name);
    xCLEAR_BITS(hero->ability);
    DISPOSE(hero);
  }

  free_kareas(kingdom);
  free_kingdom_news(kingdom);

  UNLINK( kingdom, first_kingdom, last_kingdom, next, prev );
  DISPOSE(kingdom->filename);
  DISPOSE(kingdom->dirname);
  STRFREE(kingdom->name);
  STRFREE(kingdom->whoname);
  STRFREE(kingdom->motto);
  STRFREE(kingdom->description);
  STRFREE(kingdom->deity);
  STRFREE(kingdom->ruler);
  STRFREE(kingdom->number1);
  STRFREE(kingdom->number2);
  STRFREE(kingdom->number3);
  STRFREE(kingdom->number4);
  STRFREE(kingdom->number5);
  STRFREE(kingdom->general);
  STRFREE(kingdom->leadrank);
  STRFREE(kingdom->onerank);
  STRFREE(kingdom->tworank);
  STRFREE(kingdom->threerank);
  STRFREE(kingdom->fourrank);
  STRFREE(kingdom->fiverank);
  STRFREE(kingdom->genrank);
  DISPOSE(kingdom->url);
  STRFREE(kingdom->warwith);
  STRFREE(kingdom->allywith);
  STRFREE(kingdom->badge);
  DISPOSE(kingdom);
  return;
}


void free_kingdoms( void )
{
  KINGDOM_DATA *kingdom, *kingdom_next;

  for( kingdom = first_kingdom; kingdom; kingdom = kingdom_next )
  {
    kingdom_next = kingdom->next;
    free_one_kingdom( kingdom );
  }
  return;
}

void delete_kingdom( CHAR_DATA *ch, KINGDOM_DATA *kingdom )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *vch;
  char filename[256], dirname[256], roster[256];
  char kingdomname[MAX_INPUT_LENGTH];

  mudstrlcpy(filename, kingdom->filename, 256);
  mudstrlcpy(kingdomname, kingdom->name, 256);
  mudstrlcpy(dirname, kingdom->dirname, 256);
  snprintf(roster, 256, "%s.mem", filename);

  for( d = first_descriptor; d; d = d->next )
  {
    vch = d->original ? d->original : d->character;

    if( !vch->pcdata->kingdom )
      continue;

    if( vch->pcdata->kingdom == kingdom )
    {
      STRFREE(vch->pcdata->kingdom_name );
      vch->pcdata->kingdom = NULL;
      ch_printf(ch, "The kingdom of &W%s&D has been destroyed by the gods!\n\r", kingdom->name );
    }
  }

  if( !remove(roster) )
  {
    if( !ch )
      log_string( "Clan roster file destroyed." );
    else
      send_to_char( "Clan roster file destroyed.\n\r", ch );
  }
     
  if( !remove(dirname) )
  {
    if( !ch )
      log_string( "Kingdom directory destroyed." );
    else
      send_to_char( "Kingdom directory destroyed.\n\r", ch );
  }

  free_one_kingdom(kingdom);
  if( !ch )
  {
    if( !remove(filename) )
      log_printf_plus( LOG_NORMAL, LEVEL_IMMORTAL, "Kingdom data for %s destroyed - no members left.", kingdomname );
    return;
  }

  if( !remove(filename) )
  {
    ch_printf(ch, "&RKingdom data for %s has been destroyed.\n\r", kingdomname );
    log_printf_plus( LOG_NORMAL, LEVEL_IMMORTAL, "Kingdom data for %s has been destroyed by %s.", kingdomname, ch->name );
  }
  return;
}


void do_destroykingdom( CHAR_DATA *ch, char *argument )
{
  KINGDOM_DATA *kingdom;

  if( NULLSTR(argument) )
  {
    send_to_char( "Destroy which kingdom?\n\r", ch );
    return;
  }

  kingdom = get_kingdom(argument);
  if( !kingdom )
  {
    send_to_char( "No such kingdom.\n\r", ch );
    return;
  }

  delete_kingdom(ch, kingdom);
  return;
}

