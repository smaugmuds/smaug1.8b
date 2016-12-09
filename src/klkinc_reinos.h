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



typedef struct  kingdom_data            KINGDOM_DATA;
typedef struct  kgdmwar_data		KGDMWAR_DATA;
typedef struct  unit_data		UNIT_DATA;
typedef struct  troop_data		TROOP_DATA;
typedef struct  hero_data		HERO_DATA;
typedef struct  kingdom_area_data	KGDM_AREA;
typedef struct  kingdom_news_data	KGDM_NEWS;
typedef struct  subunit_data		SUBUNIT_DATA;
typedef struct	kingdom_system		KINGDOM_SYSTEM;



extern	KINGDOM_DATA    *	first_kingdom;
extern  KINGDOM_DATA    *       last_kingdom;
extern	KGDMWAR_DATA    *	first_kgdmwar;
extern	KGDMWAR_DATA	*	last_kgdmwar;
extern  UNIT_DATA       *     	first_unit;
extern  UNIT_DATA       *     	last_unit;
extern  HERO_DATA       *     	first_hero;
extern  HERO_DATA       *     	last_hero;
extern  TROOP_DATA      *     	first_troop;
extern  TROOP_DATA      *     	last_troop;
extern	KGDM_AREA	*	first_karea;
extern	KGDM_AREA	*	last_karea;

extern	char *	const	troop_attributes   [];
extern  char *  const   troop_abilities    [];
extern  char *  const   hero_abiltites     [];



typedef enum
{
  TROOP_ATTRIB_MERCENARY, TROOP_ATTRIB_SUMMON, TROOP_ATTRIB_SPECIAL, TROOP_ATTRIB_UNDISBANDABLE,
  TROOP_ATTRIB_EVENT, TROOP_ATTRIB_NOLEAVE, TROOP_ATTRIB_NOBATTLE, TROOP_ATTRIB_SINGLE,
  TROOP_ATTRIB_LIMIT_TURN, TROOP_ATTRIB_NORECRUIT, TROOP_ATTRIB_BASICT,
  MAX_TROOP_ATTRIB
} troop_attribute;


typedef enum
{
  TROOP_ABILITY_FLYING, TROOP_ABILITY_ADDITIONAL_STRIKE, TROOP_ABILITY_SWIFT, TROOP_ABILITY_LARGE_SHIELD,
  TROOP_ABILITY_PIKE, TROOP_ABILITY_REGENERATION, TROOP_ABILITY_ENDURANCE, TROOP_ABILITY_SIEGE,
  TROOP_ABILITY_BEAUTY, TROOP_ABILITY_STEAL_LIFE, TROOP_ABILITY_MARKSMANSHIP, TROOP_ABILITY_SCALES,
  TROOP_ABILITY_FEAR, TROOP_ABILITY_HEALING, TROOP_ABILITY_CHARM, TROOP_ABILITY_BURSTING_FLAME,
  TROOP_ABILITY_CLUMSINESS, TROOP_ABILITY_ACCURACY, TROOP_ABILITY_RECRUIT_SPEED, TROOP_ABILITY_WEAKNESS,
  TROOP_ABILITY_INITIATIVE, TROOP_ABILITY_ATTACK, TROOP_ABILITY_COUNTERATTACK, TROOP_ABILITY_HITPOINT,
  TROOP_ABILITY_ATTACKTYPE, TROOP_ABILITY_MANA, TROOP_ABILITY_MOVE, TROOP_ABILITY_PREVENT_DAMAGE,
  TROOP_ABILITY_DESTROY, TROOP_ABILITY_RESURRECT, TROOP_ABILITY_SPELL_IMMUNITY, TROOP_ABILITY_SLEEP,
  TROOP_ABILITY_ATTDEF, TROOP_ABILITY_ATTACK_RESISTANCE, TROOP_ABILITY_SPELL_RESISTANCE, TROOP_ABILITY_DAMAGE_SELF,
  MAX_TROOP_ABILITY
} troop_ability;


typedef enum
{
  HERO_ABILITY_ACID_DISSOLVE, HERO_ABILITY_ALCHEMY, HERO_ABILITY_ANIMAL_TAMING, HERO_ABILITY_ANIMATE_DEAD,
  HERO_ABILITY_ARCFIRE, HERO_ABILITY_BACKSTABBING, HERO_ABILITY_BEASTMASTER, HERO_ABILITY_BERSERK,
  HERO_ABILITY_BLESSING, HERO_ABILITY_BLOODTHIRSTY, HERO_ABILITY_BREWING, HERO_ABILITY_BUILDING,
  HERO_ABILITY_CHANNELING, HERO_ABILITY_CHARISMA, HERO_ABILITY_CHILL_TOUCH, HERO_ABILITY_CONFUSE,
  HERO_ABILITY_CONTRACT, HERO_ABILITY_COUNTERSPELL, HERO_ABILITY_CURSING, HERO_ABILITY_DEMON_HUNTER,
  HERO_ABILITY_DRAGON_RIDER, HERO_ABILITY_DRAGONSLAYER, HERO_ABILITY_DWARFBANE, HERO_ABILITY_EARTHQUAKE,
  HERO_ABILITY_ENCHANT_ITEM, HERO_ABILITY_ENCHANT_WEAPON, HERO_ABILITY_FIRESTORM, HERO_ABILITY_FIREWALL,
  HERO_ABILITY_FORCEFIELD, HERO_ABILITY_GIANTSLAYER, HERO_ABILITY_HEALING, HERO_ABILITY_HONOR,
  HERO_ABILITY_ICESHIELD, HERO_ABILITY_ICESTORM, HERO_ABILITY_ICICLE, HERO_ABILITY_LEADERSHIP,
  HERO_ABILITY_LIGHTNING_FLASH, HERO_ABILITY_LIGHTNINGSTORM, HERO_ABILITY_MAGIC_SHIELD, HERO_ABILITY_MINDBLAST,
  HERO_ABILITY_MIRROR, HERO_ABILITY_NECROMANCY, HERO_ABILITY_ORCSLAYER, HERO_ABILITY_PACIFIST,
  HERO_ABILITY_PEACE_WITH_NATURE, HERO_ABILITY_PLANT_CONTROL, HERO_ABILITY_POLYMORPH, HERO_ABILITY_QUICKSAND,
  HERO_ABILITY_RAISE_DEAD, HERO_ABILITY_RALLY, HERO_ABILITY_RESEARCHING, HERO_ABILITY_SHEPHERDING,
  HERO_ABILITY_SONG, HERO_ABILITY_SOUL_DEVOUR, HERO_ABILITY_SPELL_LORE, HERO_ABILITY_SPRING_OF_LIFE,
  HERO_ABILITY_SQUIRREL_DANCE, HERO_ABILITY_SUMMONING, HERO_ABILITY_TACTICS, HERO_ABILITY_TELEPORTATION,
  HERO_ABILITY_TERROR, HERO_ABILITY_TRADING, HERO_ABILITY_TURN_UNDEAD, HERO_ABILITY_UNHOLY_AURA,
  HERO_ABILITY_VALOR, HERO_ABILITY_VAMPIRE_HUNTER, HERO_ABILITY_WALL_OF_FORCE, HERO_ABILITY_WALL_OF_THORN,
  MAX_HERO_ABILITY
} hero_ability;



struct kingdom_data
{
  KINGDOM_DATA *next;        /* next Kingdom in list                  */
  KINGDOM_DATA *prev;        /* previous Kingdom in list              */
  UNIT_DATA * 	first_unit;  	/* First unit in list                    */
  UNIT_DATA * 	last_unit;   	/* Last unit in list                     */
  HERO_DATA * 	first_hero;  	/* First hero in list                    */
  HERO_DATA * 	last_hero;   	/* Last hero in list                     */
  KGDM_AREA * 	first_karea;
  KGDM_AREA * 	last_karea;
  char *      	filename;       /* Kingdom filename                      */
  char *	dirname;	/* Kingdom directory			 */
  char *      	name;           /* Kingdom name                          */
  int         	race;           /* Kingdom's Race                        */
  char *      	whoname;        /* name displayed                        */
  char *      	motto;          /* Kingdom motto                         */
  char *      	description;    /* A brief description                   */
  char *      	deity;          /* kingdoms deity                        */
  char *      	ruler;          /* kingdom leader                        */
  char *      	number1;        /* First officer                         */
  char *      	number2;        /* Second officer                        */
  char *      	number3;        /* Third officer                         */
  char *      	number4;	/* Fourth officer			 */
  char *	number5;	/* Fifth officer			 */
  char *      	general;	/* Millitary Advisor			 */
  char *      	badge;          /* Kingdoms badge on who/where/to_room   */
  char *      	leadrank;       /* Ruler's rank                          */
  char *      	onerank;        /* Number One's rank                     */
  char *      	tworank;        /* Number Two's rank                     */
  char *      	threerank;      /* Number Three's rank                   */
  char *	fourrank;	/* Number Four's rank			 */
  char *	fiverank;	/* Number Five's rank			 */
  char *      	genrank;	/* General's rank			 */
  short      	members;        /* Number of kingdom members             */
  char *      	url;            /* Kingdoms Webpage (Aurora)             */
  char *      	warwith;        /* At War with  	                 */
  char *      	allywith;       /* Allied with	        	         */
  short      	warkills;       /* Current KingdomWar Kills              */
  short      	wkills;         /* Total KingdomWar Kills                */
  int		population;	/* Current population of kingdom         */
  int		max_population; /* Maximum population allowed		 */
  int		gold;		/* Current amount of gold in possession  */
  int		wood;		/* Current amount of wood in possession  */
  int		stone;		/* Current amount of stone in possession */
  int		food;		/* Current amount of food in possession  */
  int		mana;
  int		max_mana;
  int		curr_glory;	/* Current amount of glory in possession */
  int		max_glory;	/* Lifetime amount of glory achieved     */
  short      	gminers;        /* Number of peons assigned to mine gold */
  short      	sminers;        /* Number of peons assigned to mine stone */
  short      	farmers;        /* Number of peons assigned to farm      */
  short      	woodcutters;    /* Number of peons assigned to cut wood  */
  short		timer_hero;	/* Timer left until hero is spawned	 */
  KGDM_NEWS	*news;		/* News					 */
};


struct kgmdwar_data
{
    KGDMWAR_DATA * next;        /* next War in list                  	*/
    KGDMWAR_DATA * prev;        /* previous War in list              	*/
    KINGDOM_DATA * fighting;
};


struct unit_data
{
  UNIT_DATA *	next;
  UNIT_DATA *	prev;
  SUBUNIT_DATA *first_troop;
  SUBUNIT_DATA *last_troop;
  char *	commander;
  short		commander_level;
  short		commander_class;
};


struct subunit_data
{
  SUBUNIT_DATA *next;
  SUBUNIT_DATA *prev;
  char *	name;
  int		number;
};


struct troop_data
{
  TROOP_DATA *	next;
  TROOP_DATA *	prev;
  char *      	filename;
  short		magic_specialty;
  char *	name;
  short		race;
  EXT_BV	attribute;
  int		power_rank;
  int		attack_power;
  int		extra_attack_power;
  short		attack_type;
  short		extra_attack_type;
  short      	damtype;
  short      	extra_damtype;
  short		attack_initiative;
  short		extra_attack_initiative;
  short		counter_attack;
  short		hit;
  short		mana;
  short		move;
  short		recruit_cost;
  int		upkeep[5];
  short		spell_resistance[5];
  short		attack_resistance[11];
  EXT_BV	ability;
  int		recruit_speed;
};


struct hero_data
{
  HERO_DATA *	next;
  HERO_DATA *	prev;
  char *	filename;
  char *	name;
  short		Class;
  short		level;
  short		magic_specialty;
  EXT_BV	attribute;
  short		race;
  short		gender;
  int		attack_power;
  int		attack_level;
  int		attack_type;
  int		counter_attack;
  int		counter_attack_level;
  int		hit;
  int		hit_level;
  int		upkeep[5];
  int		upkeep_level[5];
  int		mana;
  int		mana_level;
  EXT_BV	ability;
};


struct kingdom_system
{
  int		build_town    [5];  // Gold, Food, Wood, Stone, Mana, Days
  int   	build_farm    [5];
  int   	build_guild   [5];
  int   	build_node    [5];
  int   	build_barrier [5];
  int   	build_workshop[5];
  int   	build_barracks[5];
  int   	build_fortress[5];
  short		max_command;
};


struct kingdom_area_data
{
  KGDM_AREA *	next;
  KGDM_AREA *	prev;
  char *	filename;
  char *	name;
  char *	owner;
  int		gold[2];
  int		food[2];
  int		wood[2];
  int		stone[2];
  int		land;
  short		towns;
  short		farms;
  short		barracks;
  short		guilds;
  short		barriers;
  short		fortresses;
  short		nodes;
  short		workshops;
  int		population;
  short		mood;
  char *	troop1;
  char *	troop2;
  char *	troop3;
  short		rec_timer1;
  short		rec_timer2;
  short		rec_timer3;
  short		rec_spd1;
  short		rec_spd2;
  short		rec_spd3;
  short		rec_num1;
  short		rec_num2;
  short		rec_num3;
  short		num_troops1;
  short		num_troops2;
  short		num_troops3;
  UNIT_DATA *	first_unit;
  UNIT_DATA *	last_unit;
};


struct kingdom_news_data
{
  char *         news;
  char *         date;
  time_t         mudtime;
};



// Eradication
#define HCLASS_BERSERKER		0
#define HCLASS_DRAGON_KNIGHT		1
#define HCLASS_FIRE_ELEMENTALIST	2
#define HCLASS_WARLORD			3

// Verdant
#define HCLASS_AMAZON			4
#define HCLASS_ENCHANTRESS		5
#define HCLASS_SHAMAN			6
#define HCLASS_SHEPHERDESS		7
#define HCLASS_SUMMONER			8

// Phantasm
#define HCLASS_BARD			9
#define HCLASS_ILLUSIONIST		10
#define HCLASS_SAGE			11
#define HCLASS_VALKYRIE			12

// Ascendant
#define HCLASS_PRIESTESS		13
#define HCLASS_SHIELDMAIDEN		14
#define HCLASS_VAMPIRE_HUNTER		15
#define HCLASS_WHITE_KNIGHT		16

// Nether
#define HCLASS_CRYPT_KEEPER		17
#define HCLASS_DEVIL_PRINCE		18
#define HCLASS_DREAD_KNIGHT		19
#define HCLASS_NECROMANCER		20
#define HCLASS_SOUL_REAPER		21
#define HCLASS_WITCH			22

// General
#define HCLASS_ALCHEMIST		23
#define HCLASS_ENGINEER			24
#define HCLASS_MERCHANT			25
#define HCLASS_VETERAN			26


#define MAX_HCLASS		26

#define MSPEC_PLAIN		0
#define MSPEC_ERADICATION	1
#define MSPEC_VERDANCY		2
#define MSPEC_PHANTASM		3
#define MSPEC_ASCENDANT		4
#define MSPEC_NETHER		5


#define KINGDOM_DIR     "../reinos/"          /* Nation data dir              */
#define KINGDOM_LIST    "reinos.lista"           /* List of nations              */
#define TROOP_DIR       "../reinos/tropas/"   /* Army data dir                */
#define TROOP_LIST      "tropas.lista"            /* List of army troops          */
#define HERO_DIR	"../reinos/heroes/"	/* Hero data dir		*/
#define HERO_LIST	"heroes.lista"		/* List of heroes		*/
#define KGDM_AREA_DIR	"../reinos/zonas/"	/* Dir of conquered areas	*/
#define KGDM_AREA_LIST  "zonasreinos.lista"	/* List of conquered areas	*/


#define MAX_KINGDOM            20
#define MAX_UNITS              20
#define MAX_TROOPS(hero)       hero->level * 5;

#define KGDM_GPD		3
#define KGDM_SPD		1
#define KGDM_WPD		2
#define KGDM_FPD		4

#define ATWAR            	1   /* If a kingdom is at war */
#define ALLIED           	1   /* If a kingdom is allied */


#define UPKEEP_GOLD             0
#define UPKEEP_FOOD             1
#define UPKEEP_WOOD             2
#define UPKEEP_STONE            3
#define UPKEEP_MANA             4
#define UPKEEP_POPULATION       5
#define UPKEEP_MAX		UPKEEP_POPULATION


#define GATHERING		0	// How many villagers are gathering resource?
#define REMAINING		1	// How much of the resource remains?
#define STOCKPILE		2	// How much is in the areas stockpile?

#define KCOST_GOLD		0
#define KCOST_FOOD		1
#define KCOST_WOOD		2
#define KCOST_STONE		3
#define KCOST_MANA		4
#define KCOST_DAYS		5

#define BUILDING_TOWN		0
#define BUILDING_FARM		1
#define BUILDING_GUILD		2
#define BUILDING_NODE		3
#define BUILDING_BARRIER	4
#define BUILDING_BARRACKS	5
#define BUILDING_WORKSHOP	6
#define BUILDING_FORTRESS	7



#define TROOP_ATTKTYPE_CLOSE    	0
#define TROOP_ATTKTYPE_RANGED   	1
#define TROOP_ATTKTYPE_MAGIC    	3
#define TROOP_ATTKTYPE_MAGIC_RANGED     4
#define TROOP_ATTKTYPE_PARALYZE 	5
#define TROOP_ATTKTYPE_BREATH   	6
#define TROOP_ATTKTYPE_MAX		TROOP_ATTKTYPE_BREATH


#define TROOP_DAMTYPE_MELEE     	0
#define TROOP_DAMTYPE_MISSILE   	1
#define TROOP_DAMTYPE_MAGIC     	2
#define TROOP_DAMTYPE_HOLY      	3
#define TROOP_DAMTYPE_POISON    	4
#define TROOP_DAMTYPE_LIGHTNING 	5
#define TROOP_DAMTYPE_PSYCHIC   	6
#define TROOP_DAMTYPE_FIRE      	7
#define TROOP_DAMTYPE_MAX		TROOP_DAMTYPE_FIRE


#define TIMER_HERO		7


// Command validation --Exo
#define KRANK_LEADER	8
#define KRANK_DEITY	7
#define KRANK_NUMBER1	6
#define KRANK_NUMBER2	5
#define KRANK_NUMBER3	4
#define KRANK_NUMBER4	3
#define KRANK_NUMBER5	2
#define KRANK_GENERAL	1


#ifndef NULLSTR
  #define NULLSTR(str)                    ((!str) || ((str[0]) == '\0'))
#endif

#ifndef FCLOSE
  #define FCLOSE(fp)                      fclose(fp); fp=NULL;
#endif


#define KD	KINGDOM_DATA
KD *    get_kingdom	args( ( char *name ) );
void 	remove_kmember	args( ( CHAR_DATA *ch ) );
void 	save_kingdom	args( ( KINGDOM_DATA *kingdom ) );
#undef KD
