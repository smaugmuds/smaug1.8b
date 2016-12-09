/*--------------------------------------------------------------------------
              .88b  d88. db    db d8888b.   .d888b. db   dD
              88'YbdP`88 88    88 88  `8D   VP  `8D 88 ,8P'
              88  88  88 88    88 88   88      odD' 88,8P
              88  88  88 88    88 88   88    .88'   88`8b
              88  88  88 88b  d88 88  .8D   j88.    88 `88.
              YP  YP  YP ~Y8888P' Y8888D'   888888D YP   YD
This material is copyrighted (c) 1999 - 2002 by Thomas J Whiting 
(twhiting@xanth.2y.net). Usage of this material  means that you have read
and agree to all of the licenses in the ../licenses directory. None of these
licenses may ever be removed.
----------------------------------------------------------------------------
A LOT of time has gone into this code by a LOT of people. Not just on
this individual code, but on all of the codebases this even takes a piece
of. I hope that you find this code in some way useful and you decide to
contribute a small bit to it. There's still a lot of work yet to do.
---------------------------------------------------------------------------*/

char    membuf[MSL];
void pager_printf args((CHAR_DATA *ch, char *fmt, ...));
void write_memlog_line args(( char *log ));
char *       is_hiscore_obj  args( ( OBJ_DATA *obj ) );
void         show_hiscore    args( ( char *keyword, CHAR_DATA*ch ));
void         adjust_hiscore  args( ( char *keyword, CHAR_DATA *ch, int score));
void         save_hiscores   args( ( void ) );
void         load_hiscores   args( ( void ) );

#define MEMLOG_FILE   DATA_DIR"/memory.txt"
#define LINK(link, first, last, next, prev)                     \
do                                                              \
{                                                               \
    if ( !(first) )                                             \
      (first)                   = (link);                       \
    else                                                        \
      (last)->next              = (link);                       \
    (link)->next                = NULL;                         \
    (link)->prev                = (last);                       \
    (last)                      = (link);                       \
} while(0)
#define INSERT(link, insert, first, next, prev)                 \
do                                                              \
{                                                               \
    (link)->prev                = (insert)->prev;               \
    if ( !(insert)->prev )                                      \
      (first)                   = (link);                       \
    else                                                        \
      (insert)->prev->next      = (link);                       \
    (insert)->prev              = (link);                       \
    (link)->next                = (insert);                     \
} while(0)
#define UNLINK(link, first, last, next, prev)                   \
do                                                              \
{                                                               \
    if ( !(link)->prev )                                        \
      (first)                   = (link)->next;                 \
    else                                                        \
      (link)->prev->next        = (link)->next;                 \
    if ( !(link)->next )                                        \
      (last)                    = (link)->prev;                 \
    else                                                        \
      (link)->next->prev        = (link)->prev;                 \
} while(0)

#define DISPOSE(point)                                          \
do                                                              \
{                                                               \
  sprintf(membuf,"DISPOSING: %s: %d\n",__FILE__,__LINE__);      \
  write_memlog_line(membuf);                                    \
  if (!(point))                                                 \
  {                                                             \
        bug( "Freeing null pointer",0 ); \
fprintf( stderr, "DISPOSEing NULL in %s, line %d\n", __FILE__, __LINE__); \
  }                                                             \
  else free(point);                                             \
  point = NULL;                                         \
} while(0)

#define CREATE(result, type, number)                            \
do                                                              \
{                                                               \
   if (!((result) = (type *) calloc ((number), sizeof(type))))  \
        { perror("malloc failure"); abort(); }                  \
} while(0)


typedef struct hiscore_entry HISCORE_ENTRY;
typedef struct hiscore HISCORE;
struct hiscore_entry
{
        char *name;
        int score;

        HISCORE_ENTRY *next;
        HISCORE_ENTRY *prev;
};
struct hiscore
{
        char *keyword;
        char *desc;
        int vnum;
        sh_int max_length;
        sh_int length;
        HISCORE_ENTRY *first_entry;
        HISCORE_ENTRY *last_entry;

        HISCORE           *next;
        HISCORE           *prev;
};
HISCORE *first_table;
HISCORE *last_table;


