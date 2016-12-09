/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops, Fireblade, Edmond, Conran                         |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 * 		Commands for personal player settings/statictics	    *
 ****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "kallaikia.h"
#include "klkinc_sha256.h"


/*
 *  Locals
 */
char *tiny_affect_loc_name(int location);

void do_gold(CHAR_DATA * ch, char *argument)
{
   set_char_color( AT_GOLD, ch );
   ch_printf( ch,  "Tes %s pezas de ouro.\n\r", num_punct(ch->gold) );
   return;
}

    /*
     * Returns a string from the honour_rank matrix for display purpose
     * -- Raltaris
     */
char * get_honour ( CHAR_DATA *ch )
{
    int honour = ch->pcdata->honour;
	int class = ch->class > 11 ? 3 : ch->class;

    if (honour > 99)		return honour_rank[class][6];
    else if (honour > 64)	return honour_rank[class][5];
    else if (honour > 39)	return honour_rank[class][4];
    else if (honour > 19)	return honour_rank[class][3];
    else if (honour > 9)	return honour_rank[class][2];
    else if (honour > 4)	return honour_rank[class][1];

	return honour_rank[class][0];
}

void do_worth(CHAR_DATA *ch, char *argument)
{
    char            buf[MAX_STRING_LENGTH];
    char            buf2[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
      return;

    set_pager_color(AT_GREEN, ch);
    pager_printf_color(ch, "\n\r&GValor de &w%s&G%s.\n\r", ch->name, ch->pcdata->title);
    send_to_pager_color(" &g----------------------------------------------------------------------------\n\r", ch);
    if (!ch->pcdata->deity)		 sprintf( buf, "N/A" );
    else if (ch->pcdata->favor > 2250)	 sprintf( buf, "amado" );
    else if (ch->pcdata->favor > 2000)	 sprintf( buf, "querido" );
    else if (ch->pcdata->favor > 1750) 	 sprintf( buf, "honorado" );
    else if (ch->pcdata->favor > 1500)	 sprintf( buf, "orado" );
    else if (ch->pcdata->favor > 1250)	 sprintf( buf, "favorecido" );
    else if (ch->pcdata->favor > 1000)	 sprintf( buf, "respetado" );
    else if (ch->pcdata->favor > 750)	 sprintf( buf, "gustado" );
    else if (ch->pcdata->favor > 250)	 sprintf( buf, "tolerado" );
    else if (ch->pcdata->favor > -250)	 sprintf( buf, "ignorado" );
    else if (ch->pcdata->favor > -750)	 sprintf( buf, "shunned" );
    else if (ch->pcdata->favor > -1000)	 sprintf( buf, "disliked" );
    else if (ch->pcdata->favor > -1250)	 sprintf( buf, "dishonored" );
    else if (ch->pcdata->favor > -1500)	 sprintf( buf, "disowned" );
    else if (ch->pcdata->favor > -1750)	 sprintf( buf, "abandoned" );
    else if (ch->pcdata->favor > -2000)	 sprintf( buf, "despised" );
    else if (ch->pcdata->favor > -2250)	 sprintf( buf, "hated" );
    else				 sprintf( buf, "damned" );

    if ( ch->level < 10 )
    {
       if (ch->alignment > 900)		 sprintf(buf2, "devout");
       else if (ch->alignment > 700)	 sprintf(buf2, "noble");
       else if (ch->alignment > 350)	 sprintf(buf2, "honorable");
       else if (ch->alignment > 100)	 sprintf(buf2, "worthy");
       else if (ch->alignment > -100)	 sprintf(buf2, "neutral");
       else if (ch->alignment > -350)	 sprintf(buf2, "base");
       else if (ch->alignment > -700)	 sprintf(buf2, "evil");
       else if (ch->alignment > -900)	 sprintf(buf2, "ignoble");
       else				 sprintf(buf2, "fiendish");      
    }
    else
	sprintf(buf2, "%d", ch->alignment );
    pager_printf_color(ch, "&g|&GNivel: &w%-4d &g|&GFavor: &w%-10s &g|&GAlineamento: &w%-9s &g|&GExperiencia: &w%-9d&g|\n\r", 
                     ch->level, buf, buf2, ch->exp );
    send_to_pager_color("&g ----------------------------------------------------------------------------\n\r", ch);
        switch (ch->style) {
        case STYLE_EVASIVE:
                sprintf(buf, "evasivo");
                break;
        case STYLE_DEFENSIVE:
                sprintf(buf, "defensivo");
                break;
        case STYLE_AGGRESSIVE:
                sprintf(buf, "agresivo");
                break;
        case STYLE_BERSERK:
                sprintf(buf, "salvaxe");
                break;
        default:
                sprintf(buf, "normal");
                break;
        }
    pager_printf_color
(ch, "&g|&GGloria: &w%-4d &g|&GPeso: &w%-9d &g|&GEstilo: &w%-13s &g|&GOuro: &w%-14s &g|\n\r",
                 ch->pcdata->quest_curr, ch->carry_weight, buf, num_punct(ch->gold) );
    send_to_pager_color("&g ----------------------------------------------------------------------------\n\r", ch);
    if ( ch->level < 15 && !IS_PKILL(ch) )
    	pager_printf_color(ch, "&g|            |&GHitroll: &g-------- |&GDamroll: &g----------- |                     |\n\r" );
	else if ( ch -> level >= 50 )
		pager_printf_color(ch, "&g|&GHonor: &w%-3d &g|&GRango: &w%-11s &g|&GHitroll: &w%-11d &g|&GDamroll: &w%-11d &g|\n\r", 
		ch->pcdata->honour, 
		get_honour ( ch ), 
		GET_HITROLL(ch), GET_DAMROLL(ch) );
    else
    	pager_printf_color(ch, "&g|            |&GHitroll: &w%-8d &g|&GDamroll: &w%-11d &g|                     |\n\r", GET_HITROLL(ch), GET_DAMROLL(ch) );
    send_to_pager_color("&g ----------------------------------------------------------------------------\n\r", ch);
    return;
}


/*
 * New score command by Haus
 * This version is left for the people who have lost sight of reality so badly as to
 * become violently problematic over the colors added to do_score
 */
void do_score(CHAR_DATA * ch, char *argument)
{
    char            buf[MAX_STRING_LENGTH];
    AFFECT_DATA    *paf;
//    int iLang;

    if (IS_NPC(ch))
    {
	do_oldscore(ch, argument);
	return;
    }
    set_pager_color(AT_SCORE, ch);

    pager_printf(ch, "\n\rPuntuación para %s%s.\n\r", ch->name, ch->pcdata->title);
    if ( get_trust( ch ) != ch->level )
	pager_printf( ch, "Eres creíble no nivel %d.\n\r", get_trust( ch ) );
    
    send_to_pager("----------------------------------------------------------------------------\n\r", ch);

    pager_printf(ch, "Nivel: %-3d         Raza : %-10.10s        Xogado:   %d horas\n\r",
	ch->level, capitalize(get_race(ch)), (get_age(ch) - 17) * 2);

    pager_printf(ch, "Idade: %-6d      Clase: %-11.11s       Entrada:  %s\r",
		get_age(ch), capitalize(get_class(ch)), ctime(&(ch->logon)) );

    if (ch->level >= 15
    ||  IS_PKILL( ch ) )
    {
	pager_printf(ch, "FOR  : %2.2d(%2.2d)      HitRoll: %-4d            Gardado:  %s\r",
		get_curr_str(ch), ch->perm_str, GET_HITROLL(ch), ch->save_time ? ctime(&(ch->save_time)) : "sin gardar a sesión\n" );

	pager_printf(ch, "INT  : %2.2d(%2.2d)      DamRoll: %-4d            Tempo:    %s\r",
		get_curr_int(ch), ch->perm_int, GET_DAMROLL(ch), ctime(&current_time) );
    }
    else
    {
	pager_printf(ch, "FOR  : %2.2d(%2.2d)                               Gardado:  %s\r",
		get_curr_str(ch), ch->perm_str, ch->save_time ? ctime(&(ch->save_time)) : "non\n" );

	pager_printf(ch, "INT  : %2.2d(%2.2d)                               Tempo:    %s\r",
		get_curr_int(ch), ch->perm_int, ctime(&current_time) );
    }

    if (GET_AC(ch) >= 101)
	sprintf(buf, "the rags of a beggar");
    else if (GET_AC(ch) >= 80)
	sprintf(buf, "improper for adventure");
    else if (GET_AC(ch) >= 55)
	sprintf(buf, "shabby and threadbare");
    else if (GET_AC(ch) >= 40)
	sprintf(buf, "of poor quality");
    else if (GET_AC(ch) >= 20)
	sprintf(buf, "scant protection");
    else if (GET_AC(ch) >= 10)
	sprintf(buf, "that of a knave");
    else if (GET_AC(ch) >= 0)
	sprintf(buf, "moderately crafted");
    else if (GET_AC(ch) >= -10)
	sprintf(buf, "well crafted");
    else if (GET_AC(ch) >= -20)
	sprintf(buf, "the envy of squires");
    else if (GET_AC(ch) >= -40)
	sprintf(buf, "excellently crafted");
    else if (GET_AC(ch) >= -60)
	sprintf(buf, "the envy of knights");
    else if (GET_AC(ch) >= -80)
	sprintf(buf, "the envy of barons");
    else if (GET_AC(ch) >= -100)
	sprintf(buf, "the envy of dukes");
    else if (GET_AC(ch) >= -200)
	sprintf(buf, "the envy of emperors");
    else
	sprintf(buf, "that of an avatar");
    if (ch->level > 24)
	pager_printf(ch, "WIS  : %2.2d(%2.2d)      Armadura: %4.4d, %s\n\r",
		get_curr_wis(ch), ch->perm_wis, GET_AC(ch), buf);
    else
	pager_printf(ch, "WIS  : %2.2d(%2.2d)      Armadura: %s \n\r",
		get_curr_wis(ch), ch->perm_wis, buf);

    if (ch->alignment > 900)
	sprintf(buf, "devout");
    else if (ch->alignment > 700)
	sprintf(buf, "noble");
    else if (ch->alignment > 350)
	sprintf(buf, "honorable");
    else if (ch->alignment > 100)
	sprintf(buf, "worthy");
    else if (ch->alignment > -100)
	sprintf(buf, "neutral");
    else if (ch->alignment > -350)
	sprintf(buf, "base");
    else if (ch->alignment > -700)
	sprintf(buf, "evil");
    else if (ch->alignment > -900)
	sprintf(buf, "ignoble");
    else
	sprintf(buf, "fiendish");
    if (ch->level < 10)
	pager_printf(ch, "DEX  : %2.2d(%2.2d)      Aliñado: %-20.20s  Obxetos: %5.5d (max %5.5d)\n\r",
		get_curr_dex(ch), ch->perm_dex, buf, ch->carry_number, can_carry_n(ch));
    else
	pager_printf(ch, "DEX  : %2.2d(%2.2d)      Aliñado: %+4.4d, %-14.14s  Obxetos: %5.5d (max %5.5d)\n\r",
		get_curr_dex(ch), ch->perm_dex, ch->alignment, buf, ch->carry_number, can_carry_n(ch));

    switch (ch->position)
    {
	case POS_DEAD:
		sprintf(buf, "slowly decomposing");
		break;
	case POS_MORTAL:
		sprintf(buf, "mortally wounded");
		break;
	case POS_INCAP:
		sprintf(buf, "incapacitated");
		break;
	case POS_STUNNED:
		sprintf(buf, "stunned");
		break;
	case POS_SLEEPING:
		sprintf(buf, "sleeping");
		break;
	case POS_RESTING:
		sprintf(buf, "resting");
		break;
	case POS_STANDING:
		sprintf(buf, "standing");
		break;
	case POS_FIGHTING:
		sprintf(buf, "fighting");
		break;
        case POS_EVASIVE:
                sprintf(buf, "fighting (evasive)");   /* Fighting style support -haus */
                break;
        case POS_DEFENSIVE:
                sprintf(buf, "fighting (defensive)");
                break;
        case POS_AGGRESSIVE:
                sprintf(buf, "fighting (aggressive)");
                break;
        case POS_BERSERK:
                sprintf(buf, "fighting (salvaxe)");
                break;
	case POS_MOUNTED:
		sprintf(buf, "mounted");
		break;
        case POS_SITTING:
		sprintf(buf, "sitting");
		break;
    }
    pager_printf(ch, "CON  : %2.2d(%2.2d)      Pos'n: %-21.21s    Peso: %5.5d (max %7.7d)\n\r",
	get_curr_con(ch), ch->perm_con, buf, ch->carry_weight, can_carry_w(ch));


    /*
     * Fighting style support -haus
     */
    pager_printf(ch, "CHA  : %2.2d(%2.2d)      Cobardia: %-5d     \n\r",
	get_curr_cha(ch), ch->perm_cha, ch->wimpy);
 
    pager_printf (ch, "LCK  : %2.2d(%2.2d)      ",
             get_curr_lck (ch), ch->perm_lck);

        switch (ch->style) {
        case STYLE_EVASIVE:
                sprintf(buf, "evasivo");
                break;
        case STYLE_DEFENSIVE:
                sprintf(buf, "defensivo");
                break;
        case STYLE_AGGRESSIVE:
                sprintf(buf, "agresivo");
                break;
        case STYLE_BERSERK:
                sprintf(buf, "salvaxe");
                break;
        default:
                sprintf(buf, "normal");
                break;
        }
    pager_printf(ch, "Estilo: %-10.10s\n\r", buf);
    if ( ch->level >= 50 )
	pager_printf(ch, "Honor: %3.3d    Rango: %s\n\r", ch->pcdata->honour, get_honour ( ch ));

#ifdef SHADDAI
    pager_printf(ch, "Gloria: %4.4d(%4.4d)  Estancia: %s\n\r",
	ch->pcdata->quest_curr, ch->pcdata->quest_accum,
	get_stance_name (ch->stance));
#else
    pager_printf(ch, "Gloria: %4.4d(%4.4d)\n\r",
	ch->pcdata->quest_curr, ch->pcdata->quest_accum);
#endif

    pager_printf(ch, "PRACT: %3.3d         Hitpoints: %-5d of %5d   Pager: (%c) %3d    AutoExit(%c)\n\r",
	ch->practice, ch->hit, ch->max_hit,
	IS_SET(ch->pcdata->flags, PCFLAG_PAGERON) ? 'X' : ' ',
	ch->pcdata->pagerlen, xIS_SET(ch->act, PLR_AUTOEXIT) ? 'X' : ' ');

    if (IS_VAMPIRE(ch) || IS_DEMON(ch))
	pager_printf(ch, "XP   : %-9d       Sangue: %-5d of %5d   MKills:  %-5.5d    AutoLoot(%c)\n\r",
		ch->exp, ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level, ch->pcdata->mkills,
		xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else if (ch->class == CLASS_WARRIOR)
	pager_printf(ch, "XP   : %-9d                               MKills:  %-5.5d    AutoLoot(%c)\n\r",
		ch->exp, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else
	pager_printf(ch, "XP   : %-9d        Mana: %-5d of %5d   MKills:  %-5.5d    AutoLoot(%c)\n\r",
		ch->exp, ch->mana, ch->max_mana, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');

    pager_printf(ch, "OURO : %-13s     Move: %-5d of %5d   Mdeaths: %-5.5d    AutoSac (%c)\n\r",
	num_punct(ch->gold), ch->move, ch->max_move, ch->pcdata->mdeaths, xIS_SET(ch->act, PLR_AUTOSAC) ? 'X' : ' ');
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	send_to_pager("Estás bébedo.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] == 0)
	send_to_pager("Estás en perigo de deshidratación.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_FULL] == 0)
	send_to_pager("Estás rozando a morte.\n\r", ch);
    if ( ch->position != POS_SLEEPING )
	switch( ch->mental_state / 10 )
	{
        default:   send_to_pager( "Estás feito unha faldrapo!\n\r", ch ); break;
        case -10:  send_to_pager( "Estás case consciente.\n\r", ch ); break;
        case  -9:  send_to_pager( "Case non podes manter os ollos abertos.\n\r", ch ); break;
        case  -8:  send_to_pager( "Estás extremadamente sedento.\n\r", ch ); break;
        case  -7:  send_to_pager( "Estás moi desmotivado.\n\r", ch ); break;
        case  -6:  send_to_pager( "Sínteste sedado.\n\r", ch ); break;
        case  -5:  send_to_pager( "Estás soñolento.\n\r", ch ); break;
        case  -4:  send_to_pager( "Estás cansado.\n\r", ch ); break;
        case  -3:  send_to_pager( "Deberías tomarte un descanso.\n\r", ch ); break;
        case  -2:  send_to_pager( "Estás un pouco baixo da auga.\n\r", ch ); break;
        case  -1:  send_to_pager( "Sínteste ben.\n\r", ch ); break;
        case   0:  send_to_pager( "Sínteste de maravilla.\n\r", ch ); break;
        case   1:  send_to_pager( "Sínteste enerxético.\n\r", ch ); break;
        case   2:  send_to_pager( "A tua mente vai en lapas.\n\r", ch ); break;
        case   3:  send_to_pager( "Non podes pensar correctamente.\n\r", ch ); break;
        case   4:  send_to_pager( "A tua mente vai a 1000 kilómetros por hora.\n\r", ch ); break;
        case   5:  send_to_pager( "Sínteste espléndidamente.\n\r", ch ); break;
        case   6:  send_to_pager( "A túa mente e corpo van por separado.\n\r", ch ); break;
        case   7:  send_to_pager( "A Realidade estáse distorsinoando.\n\r", ch ); break;
        case   8:  send_to_pager( "Xa non tes nin idea do que é real, e do que non.\n\r", ch ); break;
        case   9:  send_to_pager( "Sínteste inmortal.\n\r", ch ); break;
        case  10:  send_to_pager( "Eres unha Entidade Suprema.\n\r", ch ); break;
	}
    else
    if ( ch->mental_state >45 )
	send_to_pager( "Your sleep is filled with strange and vivid dreams.\n\r", ch );
    else
    if ( ch->mental_state >25 )
	send_to_pager( "Your sleep is uneasy.\n\r", ch );
    else
    if ( ch->mental_state <-35 )
	send_to_pager( "Estás deep in a much needed sleep.\n\r", ch );
    else
    if ( ch->mental_state <-25 )
	send_to_pager( "Estás in deep slumber.\n\r", ch );
/*
    send_to_pager("Languages: ", ch );
    for ( iLang = 0; lang_array[iLang] != LANG_UNKNOWN; iLang++ )
	if ( knows_language( ch, lang_array[iLang], ch )
	||  (IS_NPC(ch) && ch->speaks == 0) )
	{
	    if ( lang_array[iLang] & ch->speaking
	    ||  (IS_NPC(ch) && !ch->speaking) )
		set_pager_color( AT_RED, ch );
	    send_to_pager( lang_names[iLang], ch );
	    send_to_pager( " ", ch );
	    set_pager_color( AT_SCORE, ch );
	}
    send_to_pager( "\n\r", ch );
*/
    if ( ch->pcdata->bestowments && ch->pcdata->bestowments[0] != '\0' )
	pager_printf( ch, "Estás bestowed with the command(s): %s.\n\r", 
		ch->pcdata->bestowments );

    if ( ch->morph && ch->morph->morph )
    {
      send_to_pager ("----------------------------------------------------------------------------\n\r", ch);
      if ( IS_IMMORTAL( ch ) )
         pager_printf (ch, "Morphed as (%d) %s with a timer of %d.\n\r",
                ch->morph->morph->vnum, ch->morph->morph->short_desc, 
		ch->morph->timer
                );
      else
        pager_printf (ch, "Estás morphed into a %s.\n\r",
                ch->morph->morph->short_desc );
      send_to_pager ("----------------------------------------------------------------------------\n\r", ch);
    } 
    if ( CAN_PKILL( ch ) )
    {
	send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
	pager_printf(ch, "PKILL DATA:  Pkills (%3.3d)     Ilegal Pkills (%3.3d)     Pdeaths (%3.3d)\n\r",
		ch->pcdata->pkills, ch->pcdata->illegal_pk, ch->pcdata->pdeaths );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type != CLAN_ORDER  && ch->pcdata->clan->clan_type != CLAN_GUILD )
    {
	send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
	pager_printf(ch, "CLAN STATS:  %-14.14s  Clan AvPkills : %-5d  Clan NonAvpkills : %-5d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->pkills[6],
		(ch->pcdata->clan->pkills[1]+ch->pcdata->clan->pkills[2]+
		 ch->pcdata->clan->pkills[3]+ch->pcdata->clan->pkills[4]+
		 ch->pcdata->clan->pkills[5]) );
        pager_printf(ch, "                             Clan AvPdeaths: %-5d  Clan NonAvpdeaths: %-5d\n\r",
		ch->pcdata->clan->pdeaths[6],
		( ch->pcdata->clan->pdeaths[1] + ch->pcdata->clan->pdeaths[2] +
		  ch->pcdata->clan->pdeaths[3] + ch->pcdata->clan->pdeaths[4] +
		  ch->pcdata->clan->pdeaths[5] ) );
    }
    if (ch->pcdata->deity)
    {
	send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
	if (ch->pcdata->favor > 2250)
	  sprintf( buf, "loved" );
	else if (ch->pcdata->favor > 2000)
	  sprintf( buf, "cherished" );
	else if (ch->pcdata->favor > 1750)
	  sprintf( buf, "honored" );
	else if (ch->pcdata->favor > 1500)
	  sprintf( buf, "praised" );
	else if (ch->pcdata->favor > 1250)
	  sprintf( buf, "favored" );
	else if (ch->pcdata->favor > 1000)
	  sprintf( buf, "respected" );
	else if (ch->pcdata->favor > 750)
	  sprintf( buf, "liked" );
	else if (ch->pcdata->favor > 250)
	  sprintf( buf, "tolerated" );
	else if (ch->pcdata->favor > -250)
	  sprintf( buf, "ignored" );
	else if (ch->pcdata->favor > -750)
	  sprintf( buf, "shunned" );
	else if (ch->pcdata->favor > -1000)
	  sprintf( buf, "disliked" );
	else if (ch->pcdata->favor > -1250)
	  sprintf( buf, "dishonored" );
	else if (ch->pcdata->favor > -1500)
	  sprintf( buf, "disowned" );
	else if (ch->pcdata->favor > -1750)
	  sprintf( buf, "abandoned" );
	else if (ch->pcdata->favor > -2000)
	  sprintf( buf, "despised" );
	else if (ch->pcdata->favor > -2250)
	  sprintf( buf, "hated" );
	else
	  sprintf( buf, "damned" );
	pager_printf(ch, "Deidade:  %-20s  Favor: %s\n\r", ch->pcdata->deity->name, buf );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_ORDER )
    {
        send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
	pager_printf(ch, "Orden:  %-20s  Orden.Mkills:  %-6d   Orden.MDeaths:  %-6d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_GUILD )
    {
        send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
        pager_printf(ch, "Irmandade:  %-20s  Irmand.Mkills:  %-6d   Irmand.MDeaths:  %-6d\n\r",
                ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    if (IS_IMMORTAL(ch))
    {
	send_to_pager( "----------------------------------------------------------------------------\n\r", ch);

	pager_printf(ch, "INORTAL:  Wizinvis [%s]  Wizlevel (%d)\n\r",
		xIS_SET(ch->act, PLR_WIZINVIS) ? "X" : " ", ch->pcdata->wizinvis );

	pager_printf(ch, "Bamfin:  %s %s\n\r", ch->name, (ch->pcdata->bamfin[0] != '\0')
		? ch->pcdata->bamfin : "aparece nunha brétema misteriosa.");
	pager_printf(ch, "Bamfout: %s %s\n\r", ch->name, (ch->pcdata->bamfout[0] != '\0')
		? ch->pcdata->bamfout : "desaparece nunha brétema misteriosa.");


	/* Area Loaded info - Scryn 8/11*/
	if (ch->pcdata->area)
	{
	    pager_printf(ch, "Vnums: Estancia (%-5.5d - %-5.5d) Obxeto (%-5.5d - %-5.5d) Criat (%-5.5d - %-5.5d)\n\r",
		ch->pcdata->area->low_r_vnum, ch->pcdata->area->hi_r_vnum,
		ch->pcdata->area->low_o_vnum, ch->pcdata->area->hi_o_vnum,
		ch->pcdata->area->low_m_vnum, ch->pcdata->area->hi_m_vnum);
	    pager_printf(ch, "Zona Cargada [%s]\n\r", (IS_SET (ch->pcdata->area->status, AREA_LOADED)) ? "si" : "non");
	}
    }
    if (ch->first_affect)
    {
	int i;
	SKILLTYPE *sktmp;

	i = 0;
	send_to_pager( "----------------------------------------------------------------------------\n\r", ch);
	send_to_pager("AFFECT DATA:                            ", ch);
	for (paf = ch->first_affect; paf; paf = paf->next)
	{
	    if ( (sktmp=get_skilltype(paf->type)) == NULL )
		continue;
	    if (ch->level < 20)
	    {
		pager_printf(ch, "[%-34.34s]    ", sktmp->name);
		if (i == 0)
		   i = 2;
		if ((++i % 3) == 0)
		   send_to_pager("\n\r", ch);
	     }
	     if (ch->level >= 20)
	     {
		if (paf->modifier == 0)
		    pager_printf(ch, "[%-24.24s;%5d rds]    ",
			sktmp->name,
			paf->duration);
		else
		if (paf->modifier > 999)
		    pager_printf(ch, "[%-15.15s; %7.7s;%5d rds]    ",
			sktmp->name,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		else
		    pager_printf(ch, "[%-11.11s;%+-3.3d %7.7s;%5d rds]    ",
			sktmp->name,
			paf->modifier,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		if (i == 0)
		    i = 1;
		if ((++i % 2) == 0)
		    send_to_pager("\n\r", ch);
	    }
	}
    }
    send_to_pager("\n\r", ch);
    return;
}

void do_colorscheme( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( IS_NPC( ch ) )
	return;

    if ( !strcmp( arg, "1" ) )
    {
        ch->pcdata->colorize[24] = 2;
        ch->pcdata->colorize[25] = 3;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "2" ) )
    {
        ch->pcdata->colorize[24] = 2;
        ch->pcdata->colorize[25] = 15;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "3" ) )
    {
        ch->pcdata->colorize[24] = 2;
        ch->pcdata->colorize[25] = 7;
        ch->pcdata->colorize[38] = 15;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "4" ) )
    {
        ch->pcdata->colorize[24] = 3;
        ch->pcdata->colorize[25] = 2;
        ch->pcdata->colorize[38] = 15;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "5" ) )
    {
        ch->pcdata->colorize[24] = 3;
        ch->pcdata->colorize[25] = 15;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "6" ) )
    {
        ch->pcdata->colorize[24] = 3;
        ch->pcdata->colorize[25] = 7;
        ch->pcdata->colorize[38] = 15;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "7" ) )
    {
        ch->pcdata->colorize[24] = 15;
        ch->pcdata->colorize[25] = 2;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "8" ) )
    {
        ch->pcdata->colorize[24] = 15;
        ch->pcdata->colorize[25] = 10;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "9" ) )
    {
        ch->pcdata->colorize[24] = 15;
        ch->pcdata->colorize[25] = 3;
        ch->pcdata->colorize[38] = 7;
	send_to_char_color( "&GEsquema de cores configurado.\n\r", ch );
    }
    else if ( !strcmp( arg, "default" ) )
    {
	ch->pcdata->colorize[25] = -1;
	ch->pcdata->colorize[24] = -1;
	ch->pcdata->colorize[38] = -1;
	send_to_char_color( "&GEsquema de cores configurado ao defecto.\n\r", ch );
    }
    else
	do_help( ch, "colorscheme" );
    return;
}

void do_compass( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        if ( xIS_SET( ch->act, PLR_COMPASS ) )
        {
          xREMOVE_BIT( ch->act, PLR_COMPASS );
          send_to_char( "Compass is now off.\n\r", ch );
          return;
        }
        else
        {
          xSET_BIT( ch->act, PLR_COMPASS );
	  do_look( ch, "auto" );
          return;
        }
    }
    if ( ( strcmp( arg, "on" ) == 0 ) || ( strcmp( arg, "ON" ) == 0 ) )
    {
        xSET_BIT( ch->act, PLR_COMPASS );
	do_look( ch, "auto" );
        return;
    }
    if ( ( strcmp( arg, "off" ) == 0 ) || ( strcmp( arg, "OFF" ) == 0 ) )
    {
        xREMOVE_BIT( ch->act, PLR_COMPASS );
        send_to_char( "Compass is now off.\n\r", ch );
        return;
    }
}


/*
 * New score command by Haus
 * Colorized in 2001
 */
void do_cscore(CHAR_DATA * ch, char *argument)
{
    char            buf[MAX_STRING_LENGTH];
    AFFECT_DATA    *paf;
//    int iLang;

    if (IS_NPC(ch))
    {
	do_oldscore(ch, argument);
	return;
    }
    set_pager_color(AT_GREEN, ch);

    pager_printf_color(ch, "\n\r&GPuntuación de %s%s.\n\r", ch->name, ch->pcdata->title);
    if ( get_trust( ch ) != ch->level )
	pager_printf_color( ch, "&GConfíase en tí no nivel &Y%d.\n\r", get_trust( ch ) );
    
    send_to_pager_color("&g----------------------------------------------------------------------------\n\r", ch);

    pager_printf_color(ch, "&GNivel: &w%-3d         &GRaza : &w%-10.10s        &gXogado:   &w%d horas\n\r",
	ch->level, capitalize(get_race(ch)), (get_age(ch) - 17) * 2);

    pager_printf_color(ch, "&GIdade: &w%-6d      &GClase: &w%-11.11s       &gEntrada:  &w%s\r",
		get_age(ch), capitalize(get_class(ch)), ctime(&(ch->logon)) );

    if (ch->level >= 15
    ||  IS_PKILL( ch ) )
    {
	pager_printf_color(ch, "&GFOR  : &w%2.2d&g(&W%2.2d&g)      &GHitRoll: &R%-4d            &gGardado:  &w%s\r",
		get_curr_str(ch), ch->perm_str, GET_HITROLL(ch), ch->save_time ? ctime(&(ch->save_time)) : "sin gardar a sesión\n" );

	pager_printf_color(ch, "&GINT  : &w%2.2d&g(&W%2.2d&g)      &GDamRoll: &R%-4d            &gTempo:    &w%s\r",
		get_curr_int(ch), ch->perm_int, GET_DAMROLL(ch), ctime(&current_time) );
    }
    else
    {
	pager_printf_color(ch, "&GFOR  : &w%2.2d&g(&W%2.2d&g)                               &gGardado:  &w%s\r",
		get_curr_str(ch), ch->perm_str, ch->save_time ? ctime(&(ch->save_time)) : "non\n" );

	pager_printf_color(ch, "&GINT  : &w%2.2d&g(&W%2.2d&g)                               &gTempo:    &w%s\r",
		get_curr_int(ch), ch->perm_int, ctime(&current_time) );
    }

    if (GET_AC(ch) >= 101)
	sprintf(buf, "the rags of a beggar");
    else if (GET_AC(ch) >= 80)
	sprintf(buf, "improper for adventure");
    else if (GET_AC(ch) >= 55)
	sprintf(buf, "shabby and threadbare");
    else if (GET_AC(ch) >= 40)
	sprintf(buf, "of poor quality");
    else if (GET_AC(ch) >= 20)
	sprintf(buf, "scant protection");
    else if (GET_AC(ch) >= 10)
	sprintf(buf, "that of a knave");
    else if (GET_AC(ch) >= 0)
	sprintf(buf, "moderately crafted");
    else if (GET_AC(ch) >= -10)
	sprintf(buf, "well crafted");
    else if (GET_AC(ch) >= -20)
	sprintf(buf, "the envy of squires");
    else if (GET_AC(ch) >= -40)
	sprintf(buf, "excellently crafted");
    else if (GET_AC(ch) >= -60)
	sprintf(buf, "the envy of knights");
    else if (GET_AC(ch) >= -80)
	sprintf(buf, "the envy of barons");
    else if (GET_AC(ch) >= -100)
	sprintf(buf, "the envy of dukes");
    else if (GET_AC(ch) >= -200)
	sprintf(buf, "the envy of emperors");
    else
	sprintf(buf, "that of an avatar");
    if (ch->level > 24)
	pager_printf_color(ch, "&GWIS  : &w%2.2d&g(&W%2.2d&g)      &GArmadura: &w%4.4d, %s\n\r",
		get_curr_wis(ch), ch->perm_wis, GET_AC(ch), buf);
    else
	pager_printf_color(ch, "&GWIS  : &w%2.2d&g(&W%2.2d&g)      &WArmadura: &w%s \n\r",
		get_curr_wis(ch), ch->perm_wis, buf);

    if (ch->alignment > 900)
	sprintf(buf, "devout");
    else if (ch->alignment > 700)
	sprintf(buf, "noble");
    else if (ch->alignment > 350)
	sprintf(buf, "honorable");
    else if (ch->alignment > 100)
	sprintf(buf, "worthy");
    else if (ch->alignment > -100)
	sprintf(buf, "neutral");
    else if (ch->alignment > -350)
	sprintf(buf, "base");
    else if (ch->alignment > -700)
	sprintf(buf, "evil");
    else if (ch->alignment > -900)
	sprintf(buf, "ignoble");
    else
	sprintf(buf, "fiendish");
    if (ch->level < 10)
	pager_printf_color(ch, "&GDEX  : &w%2.2d&g(&W%2.2d&g)      &GAliñado: &w%-20.20s  &GObxetos: &W%5.5d &g(&wmax %5.5d&g)\n\r",
		get_curr_dex(ch), ch->perm_dex, buf, ch->carry_number, can_carry_n(ch));
    else
	pager_printf_color(ch, "&GDEX  : &w%2.2d&g(&W%2.2d&g)      &GAliñado: &w%+4.4d, %-14.14s  &GObxetos: &W%5.5d &g(&wmax %5.5d&g)\n\r",
		get_curr_dex(ch), ch->perm_dex, ch->alignment, buf, ch->carry_number, can_carry_n(ch));

    switch (ch->position)
    {
	case POS_DEAD:
		sprintf(buf, "slowly decomposing");
		break;
	case POS_MORTAL:
		sprintf(buf, "mortally wounded");
		break;
	case POS_INCAP:
		sprintf(buf, "incapacitated");
		break;
	case POS_STUNNED:
		sprintf(buf, "stunned");
		break;
	case POS_SLEEPING:
		sprintf(buf, "sleeping");
		break;
	case POS_RESTING:
		sprintf(buf, "resting");
		break;
	case POS_STANDING:
		sprintf(buf, "standing");
		break;
	case POS_FIGHTING:
		sprintf(buf, "fighting");
		break;
        case POS_EVASIVE:
                sprintf(buf, "fighting (evasive)");   /* Fighting style support -haus */
                break;
        case POS_DEFENSIVE:
                sprintf(buf, "fighting (defensive)");
                break;
        case POS_AGGRESSIVE:
                sprintf(buf, "fighting (aggressive)");
                break;
        case POS_BERSERK:
                sprintf(buf, "fighting (salvaxe)");
                break;
	case POS_MOUNTED:
		sprintf(buf, "mounted");
		break;
        case POS_SITTING:
		sprintf(buf, "sitting");
		break;
    }
    pager_printf_color(ch, "&GCON  : &w%2.2d&g(&W%2.2d&g)      &GPos'n: &w%-21.21s    &GPeso: &W%5.5d &g(&wmax %7.7d&g)\n\r",
	get_curr_con(ch), ch->perm_con, buf, ch->carry_weight, can_carry_w(ch));


    /*
     * Fighting style support -haus
     */
    pager_printf_color(ch, "&GCHA  : &w%2.2d&g(&W%2.2d&g)      &GCobardia: &Y%-5d     \n\r",
	get_curr_cha(ch), ch->perm_cha, ch->wimpy);
 
    pager_printf_color(ch, "&GLCK  : &w%2.2d&g(&W%2.2d&g)      ",
             get_curr_lck (ch), ch->perm_lck);

        switch (ch->style) {
        case STYLE_EVASIVE:
                sprintf(buf, "evasivo");
                break;
        case STYLE_DEFENSIVE:
                sprintf(buf, "defensivo");
                break;
        case STYLE_AGGRESSIVE:
                sprintf(buf, "agresivo");
                break;
        case STYLE_BERSERK:
                sprintf(buf, "salvaxe");
                break;
        default:
                sprintf(buf, "normal");
                break;
        }
    pager_printf_color(ch, "&GEstilo: &W%-10.10s\n\r", buf);
    if ( ch->level >= 50 )
	pager_printf_color(ch, "&GHonor: &w%3.3d    &GRango: &w%s\n\r", ch->pcdata->honour, get_honour ( ch ));

#ifdef SHADDAI
    pager_printf_color(ch, "&GGloria: &w%4.4d&g(&W%4.4d&g)  &GStance: &W%s\n\r",
	ch->pcdata->quest_curr, ch->pcdata->quest_accum,
	get_stance_name (ch->stance));
#else
    pager_printf_color(ch, "&GGloria: &w%4.4d&g(&W%4.4d&g)\n\r",
	ch->pcdata->quest_curr, ch->pcdata->quest_accum);
#endif

    pager_printf_color(ch, "&GPRACT: &w%3.3d         &GHitpoints: &W%-5d &gof &W%5d   &GPager: &g(&w%c&g) &w%3d    &GAutoExit&g(&w%c&g)\n\r",
	ch->practice, ch->hit, ch->max_hit,
	IS_SET(ch->pcdata->flags, PCFLAG_PAGERON) ? 'X' : ' ',
	ch->pcdata->pagerlen, xIS_SET(ch->act, PLR_AUTOEXIT) ? 'X' : ' ');

    if (IS_VAMPIRE(ch) || IS_DEMON(ch))
	pager_printf_color(ch, "&GXP   : &w%-9d       &GSangue: &R%-5d &Gof &R%5d   &GMKills:  &w%-5.5d    &GAutoLoot&g(&w%c&g)\n\r",
		ch->exp, ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level, ch->pcdata->mkills,
		xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else if (ch->class == CLASS_WARRIOR)
	pager_printf_color(ch, "&GXP   : &w%-9d                               &GMKills:  &w%-5.5d    &GAutoLoot&g(&w%c&g)\n\r",
		ch->exp, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else
	pager_printf_color(ch, "&GXP   : &w%-9d        &GMana: &C%-5d &Gof &C%5d   &GMKills:  &w%-5.5d    &GAutoLoot&g(&w%c&g)\n\r",
		ch->exp, ch->mana, ch->max_mana, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');

    pager_printf_color(ch, "&GOURO : &Y%-13s     &GMove: &w%-5d &Gof &w%5d   &GMdeaths: &w%-5.5d    &GAutoSac &g(&w%c&g)\n\r",
	num_punct(ch->gold), ch->move, ch->max_move, ch->pcdata->mdeaths, xIS_SET(ch->act, PLR_AUTOSAC) ? 'X' : ' ');

    set_char_color( AT_GREEN, ch );
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	send_to_pager("Estás bébedo.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] == 0)
	send_to_pager("Estás en perigo de deshidratación.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_FULL] == 0)
	send_to_pager("Estás rozando a morte.\n\r", ch);
    if ( ch->position != POS_SLEEPING )
	switch( ch->mental_state / 10 )
	{
        default:   send_to_pager( "Estás feito unha faldrapo!\n\r", ch ); break;
        case -10:  send_to_pager( "Estás case consciente.\n\r", ch ); break;
        case  -9:  send_to_pager( "Case non podes manter os ollos abertos.\n\r", ch ); break;
        case  -8:  send_to_pager( "Estás extremadamente sedento.\n\r", ch ); break;
        case  -7:  send_to_pager( "Estás moi desmotivado.\n\r", ch ); break;
        case  -6:  send_to_pager( "Sínteste sedado.\n\r", ch ); break;
        case  -5:  send_to_pager( "Estás soñolento.\n\r", ch ); break;
        case  -4:  send_to_pager( "Estás cansado.\n\r", ch ); break;
        case  -3:  send_to_pager( "Deberías tomarte un descanso.\n\r", ch ); break;
        case  -2:  send_to_pager( "Estás un pouco baixo da auga.\n\r", ch ); break;
        case  -1:  send_to_pager( "Sínteste ben.\n\r", ch ); break;
        case   0:  send_to_pager( "Sínteste de maravilla.\n\r", ch ); break;
        case   1:  send_to_pager( "Sínteste enerxético.\n\r", ch ); break;
        case   2:  send_to_pager( "A tua mente vai en lapas.\n\r", ch ); break;
        case   3:  send_to_pager( "Non podes pensar correctamente.\n\r", ch ); break;
        case   4:  send_to_pager( "A tua mente vai a 1000 kilómetros por hora.\n\r", ch ); break;
        case   5:  send_to_pager( "Sínteste espléndidamente.\n\r", ch ); break;
        case   6:  send_to_pager( "A túa mente e corpo van por separado.\n\r", ch ); break;
        case   7:  send_to_pager( "A Realidade estáse distorsinoando.\n\r", ch ); break;
        case   8:  send_to_pager( "Xa non tes nin idea do que é real, e do que non.\n\r", ch ); break;
        case   9:  send_to_pager( "Sínteste inmortal.\n\r", ch ); break;
        case  10:  send_to_pager( "Eres unha Entidade Suprema.\n\r", ch ); break;
	}
    else
    if ( ch->mental_state >45 )
	send_to_pager( "Your sleep is filled with strange and vivid dreams.\n\r", ch );
    else
    if ( ch->mental_state >25 )
	send_to_pager( "Os teus sonos non son tranquilos.\n\r", ch );
    else
    if ( ch->mental_state <-35 )
	send_to_pager( "Estás deep in a much needed sleep.\n\r", ch );
    else
    if ( ch->mental_state <-25 )
	send_to_pager( "Estás in deep slumber.\n\r", ch );
/*
    send_to_pager("Languages: ", ch );
    for ( iLang = 0; lang_array[iLang] != LANG_UNKNOWN; iLang++ )
	if ( knows_language( ch, lang_array[iLang], ch )
	||  (IS_NPC(ch) && ch->speaks == 0) )
	{
	    if ( lang_array[iLang] & ch->speaking
	    ||  (IS_NPC(ch) && !ch->speaking) )
		set_pager_color( AT_RED, ch );
	    send_to_pager( lang_names[iLang], ch );
	    send_to_pager( " ", ch );
	    set_pager_color( AT_SCORE, ch );
	}
    send_to_pager( "\n\r", ch );
*/
    if ( ch->pcdata->bestowments && ch->pcdata->bestowments[0] != '\0' )
	pager_printf( ch, "Tes en propiedade os comando(s): %s.\n\r", 
		ch->pcdata->bestowments );

    if ( ch->morph && ch->morph->morph )
    {
      send_to_pager_color("&g----------------------------------------------------------------------------\n\r", ch);
      if ( IS_IMMORTAL( ch ) )
         pager_printf (ch, "Transformado como (%d) %s con un tempo de %d.\n\r",
                ch->morph->morph->vnum, ch->morph->morph->short_desc, 
		ch->morph->timer
                );
      else
        pager_printf (ch, "Estás transformado en un %s.\n\r",
                ch->morph->morph->short_desc );
      send_to_pager_color("&g----------------------------------------------------------------------------\n\r", ch);
    } 
    if ( CAN_PKILL( ch ) )
    {
	send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
	pager_printf_color(ch, "&GPKILL DATA:  Pkills &g(&w%3.3d&g)     &GIlegal Pkills &g(&w%3.3d&g)     &GPdeaths &g(&w%3.3d&g)\n\r",
		ch->pcdata->pkills, ch->pcdata->illegal_pk, ch->pcdata->pdeaths );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type != CLAN_ORDER  && ch->pcdata->clan->clan_type != CLAN_GUILD )
    {
	send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
	pager_printf_color(ch, "&GCLAN STATS:  &w%-14.14s  &GClan AvPkills : &w%-5d  &GClan NonAvpkills : &w%-5d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->pkills[6],
		(ch->pcdata->clan->pkills[1]+ch->pcdata->clan->pkills[2]+
		 ch->pcdata->clan->pkills[3]+ch->pcdata->clan->pkills[4]+
		 ch->pcdata->clan->pkills[5]) );
        pager_printf_color(ch, "                             &GClan AvPdeaths: &w%-5d  &GClan NonAvpdeaths: &w%-5d\n\r",
		ch->pcdata->clan->pdeaths[6],
		( ch->pcdata->clan->pdeaths[1] + ch->pcdata->clan->pdeaths[2] +
		  ch->pcdata->clan->pdeaths[3] + ch->pcdata->clan->pdeaths[4] +
		  ch->pcdata->clan->pdeaths[5] ) );
    }
    if (ch->pcdata->deity)
    {
	send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
	if (ch->pcdata->favor > 2250)
	  sprintf( buf, "loved" );
	else if (ch->pcdata->favor > 2000)
	  sprintf( buf, "cherished" );
	else if (ch->pcdata->favor > 1750)
	  sprintf( buf, "honored" );
	else if (ch->pcdata->favor > 1500)
	  sprintf( buf, "praised" );
	else if (ch->pcdata->favor > 1250)
	  sprintf( buf, "favored" );
	else if (ch->pcdata->favor > 1000)
	  sprintf( buf, "respected" );
	else if (ch->pcdata->favor > 750)
	  sprintf( buf, "liked" );
	else if (ch->pcdata->favor > 250)
	  sprintf( buf, "tolerated" );
	else if (ch->pcdata->favor > -250)
	  sprintf( buf, "ignored" );
	else if (ch->pcdata->favor > -750)
	  sprintf( buf, "shunned" );
	else if (ch->pcdata->favor > -1000)
	  sprintf( buf, "disliked" );
	else if (ch->pcdata->favor > -1250)
	  sprintf( buf, "dishonored" );
	else if (ch->pcdata->favor > -1500)
	  sprintf( buf, "disowned" );
	else if (ch->pcdata->favor > -1750)
	  sprintf( buf, "abandoned" );
	else if (ch->pcdata->favor > -2000)
	  sprintf( buf, "despised" );
	else if (ch->pcdata->favor > -2250)
	  sprintf( buf, "hated" );
	else
	  sprintf( buf, "damned" );
	pager_printf_color(ch, "&GDeidade:  &w%-20s  &GFavor: &w%s\n\r", ch->pcdata->deity->name, buf );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_ORDER )
    {
        send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
	pager_printf_color(ch, "&GOrden:  &w%-20s  &GOrden Mkills:  &w%-6d   &GOrden MDeaths:  &w%-6d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_GUILD )
    {
        send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
        pager_printf_color(ch, "&GIrmandade:  &w%-20s  &GIrmandade.MKs:  &w%-6d   &GIrmandade.MDs:  &w%-6d\n\r",
                ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    if (IS_IMMORTAL(ch))
    {
	send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);

	pager_printf_color(ch, "&GINMORTAL:  Wizinvis &g[&w%s&g]  &GWizlevel &g(&w%d&g)\n\r",
		xIS_SET(ch->act, PLR_WIZINVIS) ? "X" : " ", ch->pcdata->wizinvis );

	pager_printf_color(ch, "&GBamfin:  &W%s %s\n\r", ch->name, (ch->pcdata->bamfin[0] != '\0')
		? ch->pcdata->bamfin : "aparece nunha brétema misteriosa.");
	pager_printf_color(ch, "&GBamfout: &W%s %s\n\r", ch->name, (ch->pcdata->bamfout[0] != '\0')
		? ch->pcdata->bamfout : "desaparece nunha brétema misteriosa.");


	/* Area Loaded info - Scryn 8/11*/
	if (ch->pcdata->area)
	{
	    pager_printf_color(ch, "&GVnums: Estancia &g(&w%-5.5d - %-5.5d&g) &GObxecto &g(&w%-5.5d - %-5.5d&g) &GCriat &g(&w%-5.5d - %-5.5d&g)\n\r",
		ch->pcdata->area->low_r_vnum, ch->pcdata->area->hi_r_vnum,
		ch->pcdata->area->low_o_vnum, ch->pcdata->area->hi_o_vnum,
		ch->pcdata->area->low_m_vnum, ch->pcdata->area->hi_m_vnum);
	    pager_printf_color(ch, "&GZona Cargada &g[&w%s&g]\n\r", (IS_SET (ch->pcdata->area->status, AREA_LOADED)) ? "si" : "non");
	}
    }
    if (ch->first_affect)
    {
	int i;
	SKILLTYPE *sktmp;

	i = 0;
	send_to_pager_color( "&g----------------------------------------------------------------------------\n\r", ch);
	send_to_pager_color("&GAFFECT DATA:                            ", ch);
	set_char_color( AT_WHITE, ch );
	for (paf = ch->first_affect; paf; paf = paf->next)
	{
	    if ( (sktmp=get_skilltype(paf->type)) == NULL )
		continue;
	    if (ch->level < 20)
	    {
		pager_printf(ch, "[%-34.34s]    ", sktmp->name);
		if (i == 0)
		   i = 2;
		if ((++i % 3) == 0)
		   send_to_pager("\n\r", ch);
	     }
	     if (ch->level >= 20)
	     {
		if (paf->modifier == 0)
		    pager_printf(ch, "[%-24.24s;%5d rds]    ",
			sktmp->name,
			paf->duration);
		else
		if (paf->modifier > 999)
		    pager_printf(ch, "[%-15.15s; %7.7s;%5d rds]    ",
			sktmp->name,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		else
		    pager_printf(ch, "[%-11.11s;%+-3.3d %7.7s;%5d rds]    ",
			sktmp->name,
			paf->modifier,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		if (i == 0)
		    i = 1;
		if ((++i % 2) == 0)
		    send_to_pager("\n\r", ch);
	    }
	}
    }
    send_to_pager("\n\r", ch);
    return;
}

void do_newscore(CHAR_DATA * ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA    *paf;

    if (IS_NPC(ch))
    {
	do_oldscore(ch, argument);
	return;
    }
    set_pager_color(AT_GREEN, ch);

    pager_printf_color(ch, "\n\r&G%s%s.\n\r", ch->name, ch->pcdata->title);
    if ( get_trust( ch ) != ch->level )
	pager_printf( ch, "Confíase en tí no nivel %d.\n\r", get_trust( ch ) );
    
    send_to_pager_color("&g----------------------------------------------------------------------------\n\r", ch);

    pager_printf_color(ch, "&gNivel: &W%-3d         &gRaza : &W%-10.10s        &gXogado:   &W%d &ghoras\n\r",
	ch->level, capitalize(get_race(ch)), (get_age(ch) - 17) * 2);
    pager_printf_color(ch, "&gIdade: &W%-6d      &gClase: &W%-11.11s       &gEntrada:  %s\r",
		get_age(ch), capitalize(get_class(ch)), ctime(&(ch->logon)) );

    if (ch->level >= 15
    ||  IS_PKILL( ch ) )
    {
	pager_printf_color(ch, "&GFOR  : &W%2.2d&g(&w%2.2d&g)&G    HitRoll: &R%-4d               &gGardado: %s\r",
		get_curr_str(ch), ch->perm_str, GET_HITROLL(ch), ch->save_time ? ctime(&(ch->save_time)) : "sin gardar a sesión\n" );

	pager_printf_color(ch, "&GINT  : &W%2.2d&g(&w%2.2d&g)&G    DamRoll: &R%-4d                &gTempo:  %s\r",
		get_curr_int(ch), ch->perm_int, GET_DAMROLL(ch), ctime(&current_time) );
    }
    else
    {
	pager_printf_color(ch, "&GFOR  : &W%2.2d&g(&w%2.2d&g)&G                               Gardado:  %s\r",
		get_curr_str(ch), ch->perm_str, ch->save_time ? ctime(&(ch->save_time)) : "non\n" );

	pager_printf_color(ch, "&GINT  : &W%2.2d&g(&w%2.2d&g)&G                               Tempo:   %s\r",
		get_curr_int(ch), ch->perm_int, ctime(&current_time) );
    }

    if (GET_AC(ch) >= 101)
	sprintf(buf, "the rags of a beggar");
    else if (GET_AC(ch) >= 80)
	sprintf(buf, "improper for adventure");
    else if (GET_AC(ch) >= 55)
	sprintf(buf, "shabby and threadbare");
    else if (GET_AC(ch) >= 40)
	sprintf(buf, "of poor quality");
    else if (GET_AC(ch) >= 20)
	sprintf(buf, "scant protection");
    else if (GET_AC(ch) >= 10)
	sprintf(buf, "that of a knave");
    else if (GET_AC(ch) >= 0)
	sprintf(buf, "moderately crafted");
    else if (GET_AC(ch) >= -10)
	sprintf(buf, "well crafted");
    else if (GET_AC(ch) >= -20)
	sprintf(buf, "the envy of squires");
    else if (GET_AC(ch) >= -40)
	sprintf(buf, "excellently crafted");
    else if (GET_AC(ch) >= -60)
	sprintf(buf, "the envy of knights");
    else if (GET_AC(ch) >= -80)
	sprintf(buf, "the envy of barons");
    else if (GET_AC(ch) >= -100)
	sprintf(buf, "the envy of dukes");
    else if (GET_AC(ch) >= -200)
	sprintf(buf, "the envy of emperors");
    else
	sprintf(buf, "that of an avatar");
    if (ch->level > 24)
	pager_printf_color(ch, "&GWIS  : &W%2.2d&g(&w%2.2d&g)&G      Armadura: &O%-d; %s\n\r",
		get_curr_wis(ch), ch->perm_wis, GET_AC(ch), buf);
    else
	pager_printf_color(ch, "&GWIS  : &W%2.2d&g(&w%2.2d&g)&G      Armadura: &O%s \n\r",
		get_curr_wis(ch), ch->perm_wis, buf);

    if (ch->alignment > 900)
	sprintf(buf, "devoto");
    else if (ch->alignment > 700)
	sprintf(buf, "noble");
    else if (ch->alignment > 350)
	sprintf(buf, "honorable");
    else if (ch->alignment > 100)
	sprintf(buf, "malo");
    else if (ch->alignment > -100)
	sprintf(buf, "neutral");
    else if (ch->alignment > -350)
	sprintf(buf, "base");
    else if (ch->alignment > -700)
	sprintf(buf, "satanico");
    else if (ch->alignment > -900)
	sprintf(buf, "ignoble");
    else
	sprintf(buf, "fiendish");
    if (ch->level < 10)
	pager_printf_color(ch, "&GDEX  : &W%2.2d&g(&w%2.2d&g)&G      Aliñado: &W%-20.20s  &GObxetos:  &W%d (max %d)\n\r",
		get_curr_dex(ch), ch->perm_dex, buf, ch->carry_number, can_carry_n(ch));
    else
	pager_printf_color(ch, "&GDEX  : &W%2.2d&g(&w%2.2d&g)&G      Aliñado: &W%4d; %-14.14s  &GObxetos:  &W%d &g(&wmax %d&g)\n\r",
		get_curr_dex(ch), ch->perm_dex, ch->alignment, buf, ch->carry_number, can_carry_n(ch));

    switch (ch->position)
    {
	case POS_DEAD:
		sprintf(buf, "descompoñendose lentamente");
		break;
	case POS_MORTAL:
		sprintf(buf, "mortalmente ferido");
		break;
	case POS_INCAP:
		sprintf(buf, "incapacitado");
		break;
	case POS_STUNNED:
		sprintf(buf, "stunned");
		break;
	case POS_SLEEPING:
		sprintf(buf, "durmindo");
		break;
	case POS_RESTING:
		sprintf(buf, "descansando");
		break;
	case POS_STANDING:
		sprintf(buf, "en pé");
		break;
	case POS_FIGHTING:
		sprintf(buf, "loitando");
		break;
        case POS_EVASIVE:
                sprintf(buf, "loitando (evasivo)");   /* Fighting style support -haus */
                break;
        case POS_DEFENSIVE:
                sprintf(buf, "loitando (defensivo)");
                break;
        case POS_AGGRESSIVE:
                sprintf(buf, "loitando (agresivo)");
                break;
        case POS_BERSERK:
                sprintf(buf, "loitando (salaxe)");
                break;
	case POS_MOUNTED:
		sprintf(buf, "montando");
		break;
        case POS_SITTING:
		sprintf(buf, "sentado");
		break;
    }
    pager_printf_color(ch, "&GCON  : &W%2.2d&g(&w%2.2d&g)&G      Pos'n: &W%-21.21s    &GPeso: &W%d &g(&wmax %d&g)\n\r",
	get_curr_con(ch), ch->perm_con, buf, ch->carry_weight, can_carry_w(ch));


    /*
     * Fighting style support -haus
     */
    pager_printf_color(ch, "&GCHA  : &W%2.2d&g(&w%2.2d&g)&G      Cobardia: &Y%-5d      ",
	get_curr_cha(ch), ch->perm_cha, ch->wimpy);
 
        switch (ch->style) {
        case STYLE_EVASIVE:
                sprintf(buf, "evasivo");
                break;
        case STYLE_DEFENSIVE:
                sprintf(buf, "defensivo");
                break;
        case STYLE_AGGRESSIVE:
                sprintf(buf, "agresivo");
                break;
        case STYLE_BERSERK:
                sprintf(buf, "salvaxe");
                break;
        default:
                sprintf(buf, "normal");
                break;
        }
    pager_printf_color(ch, "\n\r&GLCK  : &W%2.2d&g(&w%2.2d&g)&G      Estilo: &W%-10.10s\n\r",
	get_curr_lck(ch), ch->perm_lck, buf );

    pager_printf_color(ch, "&GGloria: &W%d&g/&w%d\n\r",
	ch->pcdata->quest_curr, ch->pcdata->quest_accum );

    pager_printf_color(ch, "&GPRACT: &W%3d         &GHitpoints: &Y%-5d &Gof &Y%5d   &GPager: &G(&W%c&g) &W%3d    &GAutoExit&g(&W%c&g)\n\r",
	ch->practice, ch->hit, ch->max_hit,
	IS_SET(ch->pcdata->flags, PCFLAG_PAGERON) ? 'X' : ' ',
	ch->pcdata->pagerlen, xIS_SET(ch->act, PLR_AUTOEXIT) ? 'X' : ' ');

    if (IS_VAMPIRE(ch) || IS_DEMON(ch))
	pager_printf_color(ch, "&GEXP  : &P%-9d       &GSangue: &R%-5d &Gof &R%5d   &GMKills:  &W%5d    &GAutoLoot&g(&W%c&g)\n\r",
		ch->exp, ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level, ch->pcdata->mkills,
		xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else if (ch->class == CLASS_WARRIOR)
	pager_printf_color(ch, "&GEXP  : &P%-9d                               &GMKills:  &W%5d    &GAutoLoot&g(&W%c&g)\n\r",
		ch->exp, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');
    else
	pager_printf_color(ch, "&GEXP  : &P%-9d        &GMana: &B%-5d &Gof &B%5d   &GMKills:  &W%5d    &GAutoLoot&g(&W%c&g)\n\r",
		ch->exp, ch->mana, ch->max_mana, ch->pcdata->mkills, xIS_SET(ch->act, PLR_AUTOLOOT) ? 'X' : ' ');

    pager_printf_color(ch, "&GOURO : &Y%-13s     &GMove: &O%-5d &Gof &O%5d   &GMdeaths: &W%5d    &GAutoSac &g(&W%c&g)\n\r",
	num_punct(ch->gold), ch->move, ch->max_move, ch->pcdata->mdeaths, xIS_SET(ch->act, PLR_AUTOSAC) ? 'X' : ' ');

    if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	send_to_pager("Estás bébedo.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] == 0)
	send_to_pager("Estás en risco de deshidratación.\n\r", ch);
    if (!IS_NPC(ch) && ch->pcdata->condition[COND_FULL] == 0)
	send_to_pager("Estás rozando a morte.\n\r", ch);
    if ( ch->position != POS_SLEEPING )
	switch( ch->mental_state / 10 )
	{
	    default:   send_to_pager( "You're completely messed up!\n\r", ch );	break;
	    case -10:  send_to_pager( "You're barely conscious.\n\r", ch );	break;
	    case  -9:  send_to_pager( "You can barely keep your eyes open.\n\r", ch );	break;
	    case  -8:  send_to_pager( "You're extremely drowsy.\n\r", ch );	break;
	    case  -7:  send_to_pager( "You feel very unmotivated.\n\r", ch );	break;
	    case  -6:  send_to_pager( "You feel sedated.\n\r", ch );		break;
	    case  -5:  send_to_pager( "You feel sleepy.\n\r", ch );		break;
	    case  -4:  send_to_pager( "You feel tired.\n\r", ch );		break;
	    case  -3:  send_to_pager( "You could use a rest.\n\r", ch );		break;
	    case  -2:  send_to_pager( "You feel a little under the weather.\n\r", ch );	break;
	    case  -1:  send_to_pager( "You feel fine.\n\r", ch );		break;
	    case   0:  send_to_pager( "You feel great.\n\r", ch );		break;
	    case   1:  send_to_pager( "You feel energetic.\n\r", ch );	break;
	    case   2:  send_to_pager( "Your mind is racing.\n\r", ch );	break;
	    case   3:  send_to_pager( "You can't think straight.\n\r", ch );	break;
	    case   4:  send_to_pager( "Your mind is going 100 miles an hour.\n\r", ch );	break;
	    case   5:  send_to_pager( "You're high as a kite.\n\r", ch );	break;
	    case   6:  send_to_pager( "Your mind and body are slipping apart.\n\r", ch );	break;
	    case   7:  send_to_pager( "Reality is slipping away.\n\r", ch );	break;
	    case   8:  send_to_pager( "You have no idea what is real, and what is not.\n\r", ch );	break;
	    case   9:  send_to_pager( "You feel immortal.\n\r", ch );	break;
	    case  10:  send_to_pager( "Estás a Supreme Entity.\n\r", ch );	break;
	}
    else
    if ( ch->mental_state >45 )
	send_to_pager( "Your sleep is filled with strange and vivid dreams.\n\r", ch );
    else
    if ( ch->mental_state >25 )
	send_to_pager( "Your sleep is uneasy.\n\r", ch );
    else
    if ( ch->mental_state <-35 )
	send_to_pager( "Estás deep in a much needed sleep.\n\r", ch );
    else
    if ( ch->mental_state <-25 )
	send_to_pager( "Estás in deep slumber.\n\r", ch );
/*  send_to_pager("Languages: ", ch );
    for ( iLang = 0; lang_array[iLang] != LANG_UNKNOWN; iLang++ )
	if ( knows_language( ch, lang_array[iLang], ch )
	||  (IS_NPC(ch) && ch->speaks == 0) )
	{
	    if ( lang_array[iLang] & ch->speaking
	    ||  (IS_NPC(ch) && !ch->speaking) )
		set_pager_color( AT_RED, ch );
	    send_to_pager( lang_names[iLang], ch );
	    send_to_pager( " ", ch );
	    set_pager_color( AT_SCORE, ch );
	}
    send_to_pager( "\n\r", ch );
*/
    if ( ch->pcdata->bestowments && ch->pcdata->bestowments[0] != '\0' )
	pager_printf_color(ch, "&GYou are bestowed with the command(s): &Y%s\n\r", 
		ch->pcdata->bestowments );

    if ( ch->morph && ch->morph->morph )
    {
      send_to_pager_color("&g----------------------------------------------------------------------------&G\n\r", ch);
      if ( IS_IMMORTAL( ch ) )
         pager_printf (ch, "Transformado como (%d) %s con un tempo de %d.\n\r",
                ch->morph->morph->vnum, ch->morph->morph->short_desc, 
		ch->morph->timer
                );
      else
        pager_printf (ch, "Estás transformado en un %s.\n\r",
                ch->morph->morph->short_desc );
      send_to_pager_color("&g----------------------------------------------------------------------------&G\n\r", ch);
    } 
    if ( CAN_PKILL( ch ) )
    {
	send_to_pager_color("&g----------------------------------------------------------------------------&G\n\r", ch);
	pager_printf_color(ch, "&GPKILL DATA:  Pkills (&W%d&G)     Ilegal Pkills (&W%d&G)     Pdeaths (&W%d&G)\n\r",
		ch->pcdata->pkills, ch->pcdata->illegal_pk, ch->pcdata->pdeaths );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type != CLAN_ORDER  && ch->pcdata->clan->clan_type != CLAN_GUILD )
    {
/*
	send_to_pager_color( "&W----------------------------------------------------------------------------&G\n\r", ch);
*/
	pager_printf_color(ch, "&GCLAN STATS:  &W%-14.14s  &GClan AvPkills : &W%-5d  &GClan NonAvpkills : &W%-5d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->pkills[5],
		(ch->pcdata->clan->pkills[0]+ch->pcdata->clan->pkills[1]+
		 ch->pcdata->clan->pkills[2]+ch->pcdata->clan->pkills[3]+
		 ch->pcdata->clan->pkills[4]) );
        pager_printf_color(ch, "&G                             Clan AvPdeaths: &W%-5d  &GClan NonAvpdeaths: &W%-5d\n\r",
		ch->pcdata->clan->pdeaths[5],
		( ch->pcdata->clan->pdeaths[0] + ch->pcdata->clan->pdeaths[1] +
		  ch->pcdata->clan->pdeaths[2] + ch->pcdata->clan->pdeaths[3] +
		  ch->pcdata->clan->pdeaths[4] ) );
    }
    if (ch->pcdata->deity)
    {
	send_to_pager_color( "&g----------------------------------------------------------------------------&G\n\r", ch);
	if (ch->pcdata->favor > 2250)
	  sprintf( buf, "loved" );
	else if (ch->pcdata->favor > 2000)
	  sprintf( buf, "cherished" );
	else if (ch->pcdata->favor > 1750)
	  sprintf( buf, "honored" );
	else if (ch->pcdata->favor > 1500)
	  sprintf( buf, "praised" );
	else if (ch->pcdata->favor > 1250)
	  sprintf( buf, "favored" );
	else if (ch->pcdata->favor > 1000)
	  sprintf( buf, "respected" );
	else if (ch->pcdata->favor > 750)
	  sprintf( buf, "liked" );
	else if (ch->pcdata->favor > 250)
	  sprintf( buf, "tolerated" );
	else if (ch->pcdata->favor > -250)
	  sprintf( buf, "ignored" );
	else if (ch->pcdata->favor > -750)
	  sprintf( buf, "shunned" );
	else if (ch->pcdata->favor > -1000)
	  sprintf( buf, "disliked" );
	else if (ch->pcdata->favor > -1250)
	  sprintf( buf, "dishonored" );
	else if (ch->pcdata->favor > -1500)
	  sprintf( buf, "disowned" );
	else if (ch->pcdata->favor > -1750)
	  sprintf( buf, "abandoned" );
	else if (ch->pcdata->favor > -2000)
	  sprintf( buf, "despised" );
	else if (ch->pcdata->favor > -2250)
	  sprintf( buf, "hated" );
	else
	  sprintf( buf, "damned" );
	pager_printf_color(ch, "&GDeidade:  &W%-20s &GFavor:  &W%s&G\n\r", ch->pcdata->deity->name, buf );
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_ORDER )
    {
        send_to_pager_color( "&g----------------------------------------------------------------------------&G\n\r", ch);
	pager_printf_color(ch, "&GOrden:  &W%-20s  &GOrden.MKs:  &W%-6d   &GOrden.MDs:  &W%-6d\n\r",
		ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    if (ch->pcdata->clan && ch->pcdata->clan->clan_type == CLAN_GUILD )
    {
        send_to_pager_color( "&g----------------------------------------------------------------------------&G\n\r", ch);
        pager_printf_color(ch, "&GIrmandade:  &W%-20s  &GIrmandade.MKs:  &W%-6d   &GIrmandade.MDs:  &W%-6d\n\r",
                ch->pcdata->clan->name, ch->pcdata->clan->mkills, ch->pcdata->clan->mdeaths);
    }
    argument = one_argument( argument, arg );
    if (ch->first_affect && !str_cmp( arg, "affects" ) )
    {
	int i;
	SKILLTYPE *sktmp;

	i = 0;
	send_to_pager_color( "&g----------------------------------------------------------------------------&G\n\r", ch);
	send_to_pager_color("AFFECT DATA:                            ", ch);
	for (paf = ch->first_affect; paf; paf = paf->next)
	{
	    if ( (sktmp=get_skilltype(paf->type)) == NULL )
		continue;
	    if (ch->level < 20)
	    {
		pager_printf_color(ch, "&G[&W%-34.34s&G]    ", sktmp->name);
		if (i == 0)
		   i = 2;
		if ((++i % 3) == 0)
		   send_to_pager("\n\r", ch);
	     }
	     if (ch->level >= 20)
	     {
		if (paf->modifier == 0)
		    pager_printf_color(ch, "&G[&W%-24.24s;%5d &Grds]    ",
			sktmp->name,
			paf->duration);
		else
		if (paf->modifier > 999)
		    pager_printf_color(ch, "&G[&W%-15.15s; %7.7s;%5d &Grds]    ",
			sktmp->name,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		else
		    pager_printf_color(ch, "&G[&W%-11.11s;%+-3.3d %7.7s;%5d &Grds]    ",
			sktmp->name,
			paf->modifier,
			tiny_affect_loc_name(paf->location),
			paf->duration);
		if (i == 0)
		    i = 1;
		if ((++i % 2) == 0)
		    send_to_pager("\n\r", ch);
	    }
	}
    }
    send_to_pager("\n\r", ch);
    return;
}


/*
 * Return ascii name of an affect location.
 */
char           *
tiny_affect_loc_name(int location)
{
	switch (location) {
	case APPLY_NONE:		return "NIL";
	case APPLY_STR:			return " FOR  ";
	case APPLY_DEX:			return " DEX  ";
	case APPLY_INT:			return " INT  ";
	case APPLY_WIS:			return " WIS  ";
	case APPLY_CON:			return " CON  ";
	case APPLY_CHA:			return " CHA  ";
	case APPLY_LCK:			return " LCK  ";
	case APPLY_SEX:			return " SEX  ";
	case APPLY_CLASS:		return " CLASE";
	case APPLY_LEVEL:		return " NVL  ";
	case APPLY_AGE:			return " IDADE";
	case APPLY_MANA:		return " MANA ";
	case APPLY_HIT:			return " HV   ";
	case APPLY_MOVE:		return " MOVE ";
	case APPLY_GOLD:		return " OURO ";
	case APPLY_EXP:			return " EXP  ";
	case APPLY_AC:			return " AC   ";
	case APPLY_HITROLL:		return " HITRL";
	case APPLY_DAMROLL:		return " DAMRL";
	case APPLY_SAVING_POISON:	return "SV POI";
	case APPLY_SAVING_ROD:		return "SV ROD";
	case APPLY_SAVING_PARA:		return "SV PARA";
	case APPLY_SAVING_BREATH:	return "SV BRTH";
	case APPLY_SAVING_SPELL:	return "SV SPLL";
	case APPLY_HEIGHT:		return "PESO";
	case APPLY_WEIGHT:		return "ALTURA";
	case APPLY_AFFECT:		return "AFE POR";
	case APPLY_RESISTANT:		return "RESISTE";
	case APPLY_IMMUNE:		return "INMUNE";
	case APPLY_SUSCEPTIBLE:		return "SUSCEPT";
	case APPLY_WEAPONSPELL:		return " ARMA  ";
	case APPLY_BACKSTAB:		return "BACKSTB";
	case APPLY_PICK:		return " PICK  ";
	case APPLY_TRACK:		return " TRACK ";
	case APPLY_STEAL:		return " STEAL ";
	case APPLY_SNEAK:		return " SNEAK ";
	case APPLY_HIDE:		return " OCULTA";
	case APPLY_PALM:		return " PALM  ";
	case APPLY_DETRAP:		return " DETRAP";
	case APPLY_DODGE:		return " DODGE ";
	case APPLY_PEEK:		return " PEEK  ";
	case APPLY_SCAN:		return " SCAN  ";
	case APPLY_GOUGE:		return " GOUGE ";
	case APPLY_SEARCH:		return " BUSCA ";
	case APPLY_MOUNT:		return " MONTA ";
	case APPLY_DISARM:		return " DISARM";
	case APPLY_KICK:		return " KICK  ";
	case APPLY_PARRY:		return " PARRY ";
	case APPLY_BASH:		return " BASH  ";
	case APPLY_STUN:		return " STUN  ";
	case APPLY_PUNCH:		return " PUNCH ";
	case APPLY_CLIMB:		return " CLIMB ";
	case APPLY_GRIP:		return " GRIP  ";
	case APPLY_SCRIBE:		return " SCRIBE";
	case APPLY_BREW:		return " BREW  ";
	case APPLY_WEARSPELL:		return " WEAR  ";
	case APPLY_REMOVESPELL:		return " REMOVE";
	case APPLY_EMOTION:		return "EMOTION";
	case APPLY_MENTALSTATE:		return " MENTAL";
	case APPLY_STRIPSN:		return " DISPEL";
	case APPLY_REMOVE:		return " REMOVE";
	case APPLY_DIG:			return " DIG   ";
	case APPLY_FULL:		return " FAMENTO";
	case APPLY_THIRST:		return " SEDENTO";
	case APPLY_DRUNK:		return " BÉBEDO";
	case APPLY_BLOOD:		return " BLOOD ";
	case APPLY_COOK:		return " COCIÑA";
	case APPLY_RECURRINGSPELL:	return " RECURR";
	case APPLY_CONTAGIOUS:		return "CONTGUS";
	case APPLY_ODOR:		return " ODOR  ";
	case APPLY_ROOMFLAG:		return " RMFLG ";
	case APPLY_SECTORTYPE:		return " SECTOR";
	case APPLY_ROOMLIGHT:		return " LIGHT ";
	case APPLY_TELEVNUM:		return " TELEVN";
	case APPLY_TELEDELAY:		return " TELEDY";
	};

	bug("Affect_location_name: unknown location %d.", location);
	return "Descoñecido";
}

char *
get_class(CHAR_DATA *ch)
{
    if ( IS_NPC(ch) && ch->class < MAX_NPC_CLASS && ch->class >= 0)
    	return ( npc_class[ch->class] );
    else if ( !IS_NPC(ch) && ch->class < MAX_PC_CLASS && ch->class >= 0 )
        return class_table[ch->class]->who_name;
    return ("Descoñecido");
}


char *
get_race( CHAR_DATA *ch)
{
    if(  ch->race < MAX_PC_RACE  && ch->race >= 0)
        return (race_table[ch->race]->race_name);
    if ( ch->race < MAX_NPC_RACE && ch->race >= 0)
	return ( npc_race[ch->race] );
    return ("Descoñecido");
}

void do_oldscore( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf;
    SKILLTYPE   *skill;

    set_pager_color( AT_SCORE, ch );
    pager_printf( ch,
	"Eres %s%s.\n\rNivel %d, idade %d anos (%d horas).\n\r",
	ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	ch->level,
	get_age(ch),
	(get_age(ch) - 17) * 2 );

    if ( get_trust( ch ) != ch->level )
	pager_printf( ch, "Confíase en tí no nivel %d.\n\r",
	    get_trust( ch ) );

    if (  IS_NPC(ch) && xIS_SET(ch->act, ACT_MOBINVIS) )
      pager_printf( ch, "Estás invisible á criaturas no nivel %d.\n\r",
            ch->mobinvis);

    if ( !IS_NPC(ch) && IS_VAMPIRE(ch) || IS_DEMON(ch) )
      pager_printf( ch,
	"Tes %d/%d hit, %d/%d nivel sangue, %d/%d moves, %d practicas.\n\r",
	ch->hit,  ch->max_hit,
	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
	ch->move, ch->max_move,
	ch->practice );
    else
      pager_printf( ch,
	"Tes %d/%d hit, %d/%d mana, %d/%d moves, %d practicas.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->practice );

    pager_printf( ch,
	"Estás carretando %d/%d obxetos con peso %d/%d kg.\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );

    pager_printf( ch,
	"For: %d  Int: %d  Wis: %d  Dex: %d  Con: %d  Cha: %d  Lck: %d.\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch),
	get_curr_cha(ch),
	get_curr_lck(ch) );

    /* Can no longer have 2 occurences of num_punct on the same line -Shaddai */
    pager_printf( ch, "Aforrache %s experiencia, e tes ",
        num_punct(ch->exp));
    pager_printf( ch, "%s moedas de ouro.\n\r",
        num_punct(ch->gold) );

    if ( !IS_NPC(ch) )
    pager_printf( ch,
	"Alcanzache %d de gloria durante a túa vida, e actualmente tes %d.\n\r",
	ch->pcdata->quest_accum, ch->pcdata->quest_curr );

    pager_printf( ch,
	"Autosaida: %s   Autoloot: %s   Autosac: %s   Autoouro: %s\n\r",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOEXIT)) ? "si" : "non",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOLOOT)) ? "si" : "non",
	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOSAC) ) ? "si" : "non",
  	(!IS_NPC(ch) && xIS_SET(ch->act, PLR_AUTOGOLD)) ? "si" : "non" );

    pager_printf( ch, "Cobardia configurada a %d puntos de dano.\n\r", ch->wimpy );

    if ( !IS_NPC(ch) ) {
       if ( ch->pcdata->condition[COND_DRUNK]   > 10 )
	   send_to_pager( "Estas bebedo.\n\r",   ch );
       if ( ch->pcdata->condition[COND_THIRST] ==  0 )
	   send_to_pager( "Estas sedento.\n\r", ch );
       if ( ch->pcdata->condition[COND_FULL]   ==  0 )
	   send_to_pager( "Estas esfameado.\n\r",  ch );
    }

    switch( ch->mental_state / 10 )
    {
        default:   send_to_pager( "Estás feito unha faldrapo!\n\r", ch ); break;
        case -10:  send_to_pager( "Estás case consciente.\n\r", ch ); break;
        case  -9:  send_to_pager( "Case non podes manter os ollos abertos.\n\r", ch ); break;
        case  -8:  send_to_pager( "Estás extremadamente sedento.\n\r", ch ); break;
        case  -7:  send_to_pager( "Estás moi desmotivado.\n\r", ch ); break;
        case  -6:  send_to_pager( "Sínteste sedado.\n\r", ch ); break;
        case  -5:  send_to_pager( "Estás soñolento.\n\r", ch ); break;
        case  -4:  send_to_pager( "Estás cansado.\n\r", ch ); break;
        case  -3:  send_to_pager( "Deberías tomarte un descanso.\n\r", ch ); break;
        case  -2:  send_to_pager( "Estás un pouco baixo da auga.\n\r", ch ); break;
        case  -1:  send_to_pager( "Sínteste ben.\n\r", ch ); break;
        case   0:  send_to_pager( "Sínteste de maravilla.\n\r", ch ); break;
        case   1:  send_to_pager( "Sínteste enerxético.\n\r", ch ); break;
        case   2:  send_to_pager( "A tua mente vai en lapas.\n\r", ch ); break;
        case   3:  send_to_pager( "Non podes pensar correctamente.\n\r", ch ); break;
        case   4:  send_to_pager( "A tua mente vai a 1000 kilómetros por hora.\n\r", ch ); break;
        case   5:  send_to_pager( "Sínteste espléndidamente.\n\r", ch ); break;
        case   6:  send_to_pager( "A túa mente e corpo van por separado.\n\r", ch ); break;
        case   7:  send_to_pager( "A Realidade estáse distorsinoando.\n\r", ch ); break;
        case   8:  send_to_pager( "Xa non tes nin idea do que é real, e do que non.\n\r", ch ); break;
        case   9:  send_to_pager( "Sínteste inmortal.\n\r", ch ); break;
        case  10:  send_to_pager( "Eres unha Entidade Suprema.\n\r", ch ); break;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_pager( "Estás MORTO!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_pager( "Estás mortalmente ferido.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_pager( "Estás incapacitado.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_pager( "Estás stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_pager( "Estás dormindo.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_pager( "Estás descansando.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_pager( "Estás en pé.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_pager( "Estás loitando.\n\r",		ch );
	break;
    case POS_MOUNTED:
	send_to_pager( "Montado.\n\r",			ch );
	break;
    case POS_SHOVE:
	send_to_pager( "Being shoved.\n\r",		ch );
	break;
    case POS_DRAG:
	send_to_pager( "Sendo arrastrado.\n\r",		ch );
	break;
    }

    if ( ch->level >= 25 )
	pager_printf( ch, "AC: %d.  ", GET_AC(ch) );

    send_to_pager( "Estás ", ch );
	 if ( GET_AC(ch) >=  101 ) send_to_pager( "PEOR que espido!\n\r", ch );
    else if ( GET_AC(ch) >=   80 ) send_to_pager( "espido.\n\r",            ch );
    else if ( GET_AC(ch) >=   60 ) send_to_pager( "vestindo algo.\n\r",  ch );
    else if ( GET_AC(ch) >=   40 ) send_to_pager( "lixeiramente armado.\n\r", ch );
    else if ( GET_AC(ch) >=   20 ) send_to_pager( "ben protexido.\n\r", ch );
    else if ( GET_AC(ch) >=    0 ) send_to_pager( "armado.\n\r",          ch );
    else if ( GET_AC(ch) >= - 20 ) send_to_pager( "ben armado.\n\r",     ch );
    else if ( GET_AC(ch) >= - 40 ) send_to_pager( "fortemente armado.\n\r", ch );
    else if ( GET_AC(ch) >= - 60 ) send_to_pager( "pesadamente armado.\n\r",  ch );
    else if ( GET_AC(ch) >= - 80 ) send_to_pager( "brutalmente armado.\n\r", ch );
    else if ( GET_AC(ch) >= -100 ) send_to_pager( "divinamente armado.\n\r", 
ch );
    else                           send_to_pager( "totalmente invencible!\n\r",       ch );

    if ( ch->level >= 15
    ||   IS_PKILL( ch ) )
	pager_printf( ch, "Hitroll: %d  Damroll: %d.\n\r",
	    GET_HITROLL(ch), GET_DAMROLL(ch) );

    if ( ch->level >= 10 )
	pager_printf( ch, "Alineado: %d.  ", ch->alignment );

    send_to_pager( "Eres ", ch );
	 if ( ch->alignment >  900 ) send_to_pager( "anxelical.\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_pager( "santo.\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_pager( "bo.\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_pager( "amable.\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_pager( "neutral.\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_pager( "malo.\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_pager( "perverso.\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_pager( "demoniaco.\n\r", ch );
    else                             send_to_pager( "satánico.\n\r", ch );
#ifdef SHADDAI
    pager_printf (ch, "Estancia:    %s \n\r", get_stance_name (ch->stance));
#endif

    if ( ch->first_affect )
    {
	send_to_pager( "Estás afectado por:\n\r", ch );
	for ( paf = ch->first_affect; paf; paf = paf->next )
	    if ( (skill=get_skilltype(paf->type)) != NULL )
	{
	    pager_printf( ch, "Feitizo: '%s'", skill->name );

	    if ( ch->level >= 20 )
		pager_printf( ch,
		    " modifica %s en %d por %d rondas",
		    affect_loc_name( paf->location ),
		    paf->modifier,
		    paf->duration );

	    send_to_pager( ".\n\r", ch );
	}
    }

    if ( !IS_NPC( ch ) && IS_IMMORTAL( ch ) )
    {
	pager_printf( ch, "\n\rWizInvis nivel: %d   WizInvis é %s\n\r",
			ch->pcdata->wizinvis,
			xIS_SET(ch->act, PLR_WIZINVIS) ? "ON" : "OFF" );
	if ( ch->pcdata->r_range_lo && ch->pcdata->r_range_hi )
	  pager_printf( ch, "Est Rango: %d - %d\n\r", ch->pcdata->r_range_lo,
					 	   ch->pcdata->r_range_hi	);
	if ( ch->pcdata->o_range_lo && ch->pcdata->o_range_hi )
	  pager_printf( ch, "Obx Rango: %d - %d\n\r", ch->pcdata->o_range_lo,
	  					   ch->pcdata->o_range_hi	);
	if ( ch->pcdata->m_range_lo && ch->pcdata->m_range_hi )
	  pager_printf( ch, "Crt Rango: %d - %d\n\r", ch->pcdata->m_range_lo,
	  					   ch->pcdata->m_range_hi	);
    }

    return;
}

/*								-Thoric
 * Display your current exp, level, and surrounding level exp requirements
 */
void do_level( CHAR_DATA *ch, char *argument )
{
    char buf [MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int x, lowlvl, hilvl;

    if ( ch->level == 1 )
      lowlvl = 1;
    else
      lowlvl = UMAX( 2, ch->level - 5 );
    hilvl = URANGE( ch->level, ch->level + 5, MAX_LEVEL );
    set_char_color( AT_GREEN, ch );
    ch_printf_color( ch, "\n\r&GExperiencia requerida, niveis &W%d &Ga &W%d&G:\n\r&g______________________________________________\n\r\n\r", lowlvl, hilvl );
    sprintf( buf, "&W exp  (Actual : %12s)", num_punct(ch->exp) );
    sprintf( buf2,"&W exp  (Precisa: &G%12s&W)", num_punct( exp_level(ch, ch->level+1) - ch->exp) );
    for ( x = lowlvl; x <= hilvl; x++ )
	ch_printf_color( ch, "&G (&W%2d&G) &W%12s%s\n\r", x, num_punct( exp_level( ch, x ) ),
		(x == ch->level) ? buf : (x == ch->level+1) ? buf2 : " exp" );
    send_to_char_color( "&g______________________________________________\n\r", ch );
}

/* 1997, Blodkai */
void do_remains( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    bool found = FALSE;
 
    if ( IS_NPC( ch ) )
      return;
    set_char_color( AT_MAGIC, ch );
    if ( !ch->pcdata->deity ) {
      send_to_pager( "You have no deity from which to seek such assistance...\n\r", ch );
      return;
    }
    if ( ch->pcdata->favor < ch->level*2 ) {
      send_to_pager( "Your favor is insufficient for such assistance...\n\r", ch );
      return;
    }
    pager_printf( ch, "%s appears in a vision, revealing that your remains... ", ch->pcdata->deity->name );
    sprintf( buf, "the corpse of %s", ch->name );
    for ( obj = first_object; obj; obj = obj->next ) {
      if ( obj->in_room && !str_cmp( buf, obj->short_descr )
      && ( obj->pIndexData->vnum == 11 ) ) {
        found = TRUE;
        pager_printf( ch, "\n\r  - at %s will endure for %d ticks",
          obj->in_room->name,
          obj->timer );
      }
    }
    if ( !found )
      send_to_pager( " no longer exist.\n\r", ch );
    else
    {
      send_to_pager( "\n\r", ch );
      ch->pcdata->favor -= ch->level*2;
    }
    return;
}

/* Affects-at-a-glance, Blodkai */
void do_affected ( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    SKILLTYPE *skill;
 
    if ( IS_NPC(ch) )
	return;

    set_char_color( AT_GREEN, ch );

    argument = one_argument( argument, arg );
    if ( !str_cmp( arg, "por" ) )
    {
        send_to_char_color( "\n\r&BImbuído con:\n\r", ch );
	ch_printf_color( ch, "&C%s\n\r",
	  !xIS_EMPTY(ch->affected_by) ? affect_bit_name( &ch->affected_by ) : "nada" );
        if ( ch->level >= 20 )
        {
            
            send_to_char( "\n\r", ch );
            if ( ch->resistant > 0 || ch->stance_resistant>0)
	    {
                send_to_char_color( "&BResistencias:  ", ch );
#ifdef SHADDAI
                ch_printf_color( ch, "&C%s\n\r", flag_string(ch->resistant || ch->stance_resistant, ris_flags) );
#endif
                ch_printf_color( ch, "&C%s\n\r", flag_string(ch->resistant, ris_flags) );
	    }
            if ( ch->immune > 0 || ch->stance_immune > 0)
	    {
                send_to_char_color( "&BInmunidades:   ", ch);
#ifdef SHADDAI
                ch_printf_color( ch, "&C%s\n\r",  flag_string(ch->immune||ch->stance_immune, ris_flags) );
#endif
                ch_printf_color( ch, "&C%s\n\r",  flag_string(ch->immune, ris_flags) );
	    }
            if ( ch->susceptible > 0 || ch->stance_susceptible>0)
	    {
                send_to_char_color( "&BSusceptible:     ", ch );
#ifdef SHADDAI
                ch_printf_color( ch, "&C%s\n\r", flag_string(ch->susceptible||ch->stance_susceptible, ris_flags) );
#endif
                ch_printf_color( ch, "&C%s\n\r", flag_string(ch->susceptible, ris_flags) );
	    }
        }
	return;      
    }

    if ( !ch->first_affect )
    {
        send_to_char_color( "\n\r&CNingún feitizo está a afectarche.\n\r", ch );
    }
    else
    {
	send_to_char( "\n\r", ch );
        for (paf = ch->first_affect; paf; paf = paf->next)
	    if ( (skill=get_skilltype(paf->type)) != NULL )
        {
	    set_char_color( AT_BLUE, ch );
            send_to_char( "Afectado:  ", ch );
            set_char_color( AT_SCORE, ch );
            if ( ch->level >= 20
	    ||   IS_PKILL( ch ) )
            {
                if (paf->duration < 25 ) set_char_color( AT_WHITE, ch );
                ch_printf( ch, "(%5d)   ", paf->duration );
	    }
            ch_printf( ch, "%-18s\n\r", skill->name );
        }
    }
    return;
}

/*
 * Support for GOD+ to view inventory of others	-Thoric
 */
void do_inventory( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = ch;
    bool invtarget = FALSE;

    if ( get_trust(ch) >= LEVEL_GOD && argument[0] != '\0' )
    {
	if ( ( victim=get_char_world( ch, argument ) ) == NULL )
	{
	    send_to_char( "Non están aquí.\n\r", ch );
	    return;
	}
	invtarget = TRUE;
    }

    ch_printf_color( ch, "&R%s %s carretar:\n\r",
	invtarget ? capitalize( victim->name) : "Estás",
	invtarget ? "a" : "a" );
    show_list_to_char( victim->first_carrying, ch, TRUE, TRUE );

}
void do_condition( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = ch;
    OBJ_DATA *obj;
    int iWear;
    bool found;
    int cond;  

    if ( get_trust(ch) > LEVEL_GOD && argument[0] != '\0' )
    {
	if ( (victim=get_char_world(ch, argument)) == NULL )
	{
	    send_to_char( "Non están aquí.\n\r", ch );
	    return;
	}
	act( AT_RED, "$n está usando:", victim, NULL, ch, TO_VICT );
    }
    else
    {
	set_char_color( AT_RED, ch );
	send_to_char( "Estás usando:\n\r", ch );
    }
    found = FALSE;
    set_char_color( AT_OBJECT, ch );
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	for ( obj = victim->first_carrying; obj; obj = obj->next_content )
	   if ( obj->wear_loc == iWear )
	   {
                if( (!IS_NPC(ch)) && (ch->race>0) && (ch->race<MAX_PC_RACE))
                    send_to_char(race_table[ch->race]->where_name[iWear], ch);
                else
                    send_to_char( where_name[iWear], ch );

		

		if ( can_see_obj( ch, obj ) )
		{
		    switch(obj->item_type)
                    {
                     case ITEM_ARMOR:
                       cond = (int) ((10 * obj->value[0] / obj->value[1]) );
                     break;
                     case ITEM_WEAPON:
                       cond = (int) ((10* obj->value[0] / 12) );
                     break;
                     default:
                       cond = -1;
                     break;
                    }
/*
                    send_to_char_color("&g<&Y",ch);
*/
                    if (cond >= 0)
		    {
			if ( cond > 8 )
			  ch_printf_color( ch, "&w[&G+&w]&G " );
			else if ( cond > 5 )
			  ch_printf_color( ch, "&w[&Y-&w]&G " );
			else
			  ch_printf_color( ch, "&w[&R!&w]&G " );
                    }
		    else
			send_to_char("    ", ch );
/*
                    send_to_char_color("&g>&G  ",ch);
*/
		    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		    send_to_char( "\n\r", ch );
		}
		else
		    send_to_char( "algo.\n\r", ch );
		found = TRUE;
	   }
    }

    if ( !found )
	send_to_char( "Nada.\n\r", ch );

    return;
}


void do_equipment( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim = ch;
    OBJ_DATA *obj;
    int iWear;
    bool found;

    if ( get_trust(ch) > LEVEL_GOD && argument[0] != '\0' )
    {
	if ( (victim=get_char_world(ch, argument)) == NULL )
	{
	    send_to_char( "Non están aquí.\n\r", ch );
	    return;
	}
	act( AT_RED, "$n está usando:", victim, NULL, ch, TO_VICT );
    }
    else
    {
	set_char_color( AT_RED, ch );
	send_to_char( "Estás usando:\n\r", ch );
    }
    found = FALSE;
    set_char_color( AT_OBJECT, ch );
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	for ( obj = victim->first_carrying; obj; obj = obj->next_content )
	   if ( obj->wear_loc == iWear )
	   {
                if( (!IS_NPC(ch)) && (ch->race>0) && (ch->race<MAX_PC_RACE))
                    send_to_char(race_table[ch->race]->where_name[iWear], ch);
                else
                    send_to_char( where_name[iWear], ch );

		if ( can_see_obj( ch, obj ) )
		{
		    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		    send_to_char( "\n\r", ch );
		}
		else
		    send_to_char( "algo.\n\r", ch );
		found = TRUE;
	   }
    }

    if ( !found )
	send_to_char( "Nada.\n\r", ch );

    send_to_char_color( "&g(teclea 'equipado' para listar os obxetos equipados)\n\r", ch );
    return;
}


void do_equipment_full( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool ankler = FALSE, anklel = FALSE;
    bool candual, hasloc;
    candual = FALSE;
    set_char_color( AT_RED, ch );
    send_to_char( "Estás usando:\n\r", ch );

    set_char_color( AT_OBJECT, ch );
    if ( LEARNED(ch, gsn_dual_wield) )
    {
	candual = TRUE;
    }
    if ( get_eq_char(ch, WEAR_HOLD) )
	candual = FALSE;
    if ( get_eq_char(ch,WEAR_MISSILE_WIELD) )
	candual = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	hasloc = FALSE;
	for ( obj = ch->first_carrying; obj; obj = obj->next_content )
	{
	   if ( obj->wear_loc == iWear )
	   {
                if( (!IS_NPC(ch)) && (ch->race>0) && (ch->race<MAX_PC_RACE))
                    send_to_char(race_table[ch->race]->where_name[iWear], ch);
                else
                    send_to_char( where_name[iWear], ch );
		if ( iWear == WEAR_SHIELD )
		   candual = FALSE;
		if ( iWear == WEAR_ANKLE_L )
			anklel = TRUE;
		if (iWear == WEAR_ANKLE_R )
			ankler = TRUE;
		if ( can_see_obj( ch, obj ) )
		{
		    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		    send_to_char( "\n\r", ch );
		}
		else
		    send_to_char( "algo.\n\r", ch );
		hasloc = TRUE;
	   }
	}
	if (!hasloc)
	{
	   if ( iWear <= WEAR_EYES)
	   {
	   	if ((candual) && (iWear == WEAR_SHIELD))
		   continue;
	   	if ((candual) && (iWear == WEAR_HOLD))
		   continue;
	   	if ((!candual) && (iWear == WEAR_DUAL_WIELD))
		   continue;
           	send_to_char(where_name[iWear],ch);
	   	send_to_char("[nada] \n\r",ch);
	   }
	}
    }
	if ( !anklel ) { 
	send_to_char( where_name[WEAR_ANKLE_L], ch ); 
	send_to_char("[nada] \n\r", ch); }
	if ( !ankler ) {
	send_to_char( where_name[WEAR_ANKLE_R], ch );
	send_to_char("[nada] \n\r", ch);}
    return;
}


void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
	strcpy( buf, title );

    STRFREE( ch->pcdata->title );
    ch->pcdata->title = STRALLOC( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    set_char_color( AT_SCORE, ch );

    if ( IS_SET( ch->pcdata->flags, PCFLAG_NOTITLE ))
    {
	set_char_color( AT_IMMORT, ch );
        send_to_char( "Os Deuses prohíbenche cambiar o teu título.\n\r", ch );
        return;
    }
 
    if ( argument[0] == '\0' )
    {
	send_to_char( "Cambiar o teu título por cal?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 50 )
	argument[50] = '\0';

    smash_tilde( argument );
    smash_color_token( argument );
    set_title( ch, argument );
    send_to_char( "O teu novo título foi configurado.\n\r", ch );
}

void do_email( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( ch->level < 5 )
    {
	send_to_char( "Sentímolo... debes ser de nivel 5 para facer iso.\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    smash_tilde( arg );
    if ( arg[0] == '\0' )
    {
	if ( !ch->pcdata->email )
	  ch->pcdata->email = str_dup( "" );
	ch_printf( ch, "O teu email é: %s\n\r",
		show_tilde( ch->pcdata->email ) );
	return;
    }

    if ( !str_cmp( arg, "eliminar" ) )
    {
	if ( ch->pcdata->email )
	  DISPOSE(ch->pcdata->email);
	ch->pcdata->email = str_dup("");
	send_to_char( "Email eliminado.\n\r", ch );
	return;
    }

    if ( ch->pcdata->email )
      DISPOSE(ch->pcdata->email);
    ch->pcdata->email = str_dup(arg);
    send_to_char( "Email configurado.\n\r", ch );
}

void do_icq( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( ch->level < 5 )
    {
	send_to_char( "Sentímolo... debes ser polo menos nivel 5 para facer iso.\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    smash_tilde( arg );
    if ( arg[0] == '\0' )
    {
	if ( !ch->pcdata->icq )
	  ch->pcdata->icq = str_dup( "" );
	ch_printf( ch, "Your icq is: %s\n\r",
		show_tilde( ch->pcdata->icq ) );
	return;
    }

    if ( !str_cmp( arg, "clear" ) )
    {
	if ( ch->pcdata->icq )
	  DISPOSE(ch->pcdata->icq);
	ch->pcdata->icq = str_dup("");
	send_to_char( "ICQ cleared.\n\r", ch );
	return;
    }

    hide_tilde( arg  );
    if ( ch->pcdata->icq )
      DISPOSE(ch->pcdata->icq);
    ch->pcdata->icq = str_dup(arg );
    send_to_char( "ICQ set.\n\r", ch );
}

void do_homepage( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;

    if ( ch->level < 5 )
    {
	send_to_char( "Sentímolo... debes ser polo menos nivel 5 para facer iso.\n\r", ch );
	return;
    }

    if ( xIS_SET( ch->act, PLR_NOHOMEPAGE ) )
    {
	send_to_char( "The Gods prohibit you from changing your homepage.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	if ( !ch->pcdata->homepage )
	  ch->pcdata->homepage = str_dup( "" );
	ch_printf( ch, "Your homepage is: %s\n\r",
		show_tilde( ch->pcdata->homepage ) );
	return;
    }

    if ( !str_cmp( argument, "clear" ) )
    {
	if ( ch->pcdata->homepage )
	  DISPOSE(ch->pcdata->homepage);
	ch->pcdata->homepage = str_dup("");
	send_to_char( "Homepage cleared.\n\r", ch );
	return;
    }

    if ( strstr( argument, "://" ) )
	strcpy( buf, argument );
    else
	sprintf( buf, "http://%s", argument );
    if ( strlen(buf) > 70 )
	buf[70] = '\0';

    hide_tilde( buf );
    if ( ch->pcdata->homepage )
      DISPOSE(ch->pcdata->homepage);
    ch->pcdata->homepage = str_dup(buf);
    send_to_char( "Homepage set.\n\r", ch );
}


void do_delete_char(CHAR_DATA * ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char arg1[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];

  argument = case_argument( argument, arg );
  argument = one_argument( argument, arg1 );

  if ( IS_NPC(ch) )
	return;

  if ( get_trust(ch) > 5 )
  {
        send_to_char ( "A character must be between levels 2 and 5, and 18 years of age, to delete itself.\n\r", ch );
	return;
  }

  if ( get_age(ch) < 18 )
  {
        send_to_char ( "A character must be between levels 2 and 5, and 18 years of age, to delete itself.\n\r", ch );
	return;
  }

  if ( arg[0] == '\0' )
  {
	send_to_char ( "Syntax:  chardelete <password> yes\n\r", ch );
	return;
  }

  if ( strcmp( sha256_crypt( arg ), ch->pcdata->pwd ) )
  {
	send_to_char ( "Incorrect password.\n\r", ch );
	return;
  }

  if ( arg1[0] == '\0' || ( str_cmp( arg1, "yes" ) ) )
  {
	send_to_char ( "Syntax:  chardelete <password> yes\n\r", ch );
	return;
  }
  else
  {
	sprintf( buf, "%s deleted pfile from site %s", ch->name, ch->desc->host );
	send_to_char_color( "&RYour character has been deleted.\n\r", ch );
	do_destroy( ch, ch->name );
	log_string_plus( log_buf, LOG_NORMAL, 65 );
  }
  return;
}


/*
 * Set your personal description				-Thoric
 */
void do_description( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
    {
	send_to_char( "Monsters are too dumb para facer iso!\n\r", ch );
	return;	  
    }

    if ( IS_SET( ch->pcdata->flags, PCFLAG_NODESC ) )
    {
	send_to_char( "You cannot set your description.\n\r", ch );
	return;
    }
 
    if ( !ch->desc )
    {
	bug( "do_description: no descriptor", 0 );
	return;
    }

    switch( ch->substate )
    {
	default:
	   bug( "do_description: illegal substate", 0 );
	   return;

	case SUB_RESTRICTED:
	   send_to_char( "You cannot use this command from within another command.\n\r", ch );
	   return;

	case SUB_NONE:
	   ch->substate = SUB_PERSONAL_DESC;
	   ch->dest_buf = ch;
	   start_editing( ch, ch->description );
	   return;

	case SUB_PERSONAL_DESC:
	   STRFREE( ch->description );
	   ch->description = copy_buffer( ch );
	   stop_editing( ch );
	   return;	
    }
}

/* Ripped off do_description for whois bio's -- Scryn*/
void do_bio( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
    {
	send_to_char( "Mobs cannot set a bio.\n\r", ch );
	return;	  
    }
    if ( ch->level < 5 )
    {
	set_char_color( AT_SCORE, ch );
	send_to_char( "You must be at least level five to write your bio...\n\r", ch );
	return;
    }

    if ( IS_SET(ch->pcdata->flags, PCFLAG_NOBIO) )  
    {
        set_char_color( AT_RED, ch );
        send_to_char( "Os Deuses non te autorizan para facer iso!\n\r", ch);
        return;
    }

    if ( !ch->desc )
    {
	bug( "do_bio: no descriptor", 0 );
	return;
    }

    switch( ch->substate )
    {
	default:
	   bug( "do_bio: illegal substate", 0 );
	   return;
	  	   
	case SUB_RESTRICTED:
	   send_to_char( "You cannot use this command from within another command.\n\r", ch );
	   return;

	case SUB_NONE:
	   ch->substate = SUB_PERSONAL_BIO;
	   ch->dest_buf = ch;
	   start_editing( ch, ch->pcdata->bio );
	   return;

	case SUB_PERSONAL_BIO:
	   STRFREE( ch->pcdata->bio );
	   ch->pcdata->bio = copy_buffer( ch );
	   stop_editing( ch );
	   return;	
    }
}



/*
 * New stat and statreport command coded by Morphina
 * Bug fixes by Shaddai
 */

void do_statreport( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
    {
	send_to_char("Que dis?\n\r", ch );
	return;
    }

    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
    {
      ch_printf( ch, "Reportas: %d/%d hp %d/%d sangue %d/%d mv %d xp.\n\r",
 		ch->hit,  ch->max_hit, ch->pcdata->condition[COND_BLOODTHIRST],
		 10 + ch->level, ch->move, ch->max_move, ch->exp   );
      sprintf( buf, "$n reporta: %d/%d hp %d/%d sangue %d/%d mv %d xp.",
 		ch->hit,  ch->max_hit, ch->pcdata->condition[COND_BLOODTHIRST],
		 10 + ch->level, ch->move, ch->max_move, ch->exp   );
      act( AT_REPORT, buf, ch, NULL, NULL, TO_ROOM );
    }
    else
    {
      ch_printf( ch, "Reportas: %d/%d hp %d/%d mana %d/%d mv %d xp.\n\r",
 		ch->hit,  ch->max_hit, ch->mana, ch->max_mana,
 		ch->move, ch->max_move, ch->exp   );
      sprintf( buf, "$n reporta: %d/%d hp %d/%d mana %d/%d mv %d xp.",
 		ch->hit,  ch->max_hit, ch->mana, ch->max_mana,
 		ch->move, ch->max_move, ch->exp   );
      act( AT_REPORT, buf, ch, NULL, NULL, TO_ROOM );
    }

    ch_printf( ch, "Estadísticas:    %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.\n\r",
      		ch->perm_str, ch->perm_wis, ch->perm_int, ch->perm_dex, 
		ch->perm_con, ch->perm_cha, ch->perm_lck );
    sprintf( buf, "Estadísticas de $n:    %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.",
      		ch->perm_str, ch->perm_wis, ch->perm_int, ch->perm_dex, 
		ch->perm_con, ch->perm_cha, ch->perm_lck );
    act( AT_REPORT, buf, ch, NULL, NULL, TO_ROOM );

    ch_printf( ch, "Estadísticas actuais: %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.\n\r",
		get_curr_str(ch), get_curr_wis(ch), get_curr_int(ch),
		get_curr_dex(ch), get_curr_con(ch), get_curr_cha(ch),
		get_curr_lck(ch) );
    sprintf( buf, "Estadísticas de $n: %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.",
		get_curr_str(ch), get_curr_wis(ch), get_curr_int(ch),
		get_curr_dex(ch), get_curr_con(ch), get_curr_cha(ch),
		get_curr_lck(ch) );
    act( AT_REPORT, buf, ch, NULL, NULL, TO_ROOM );
    return;
}

void do_stat( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
    {
	send_to_char("Que dis?\n\r", ch );
	return;
    }

    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
      ch_printf( ch, "Reportas: %d/%d hp %d/%d blood %d/%d mv %d xp.\n\r",
 		ch->hit,  ch->max_hit, ch->pcdata->condition[COND_BLOODTHIRST],
		 10 + ch->level, ch->move, ch->max_move, ch->exp   );
    else
      ch_printf( ch, "Reportas: %d/%d hp %d/%d mana %d/%d mv %d xp.\n\r",
 		ch->hit,  ch->max_hit, ch->mana, ch->max_mana,
 		ch->move, ch->max_move, ch->exp   );

      ch_printf( ch, "Estadísticas:    %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.\n\r",
      		ch->perm_str, ch->perm_wis, ch->perm_int, ch->perm_dex, 
		ch->perm_con, ch->perm_cha, ch->perm_lck );

      ch_printf( ch, "Estadísticas actuais: %-2d for %-2d wis %-2d int %-2d dex %-2d con %-2d cha %-2d lck.\n\r",
		get_curr_str(ch), get_curr_wis(ch), get_curr_int(ch),
		get_curr_dex(ch), get_curr_con(ch), get_curr_cha(ch),
		get_curr_lck(ch) );
    return;
}


void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) && ch->fighting )
	return;

    if ( IS_AFFECTED(ch, AFF_POSSESS) )
    {   
       send_to_char("You can't do that in your current state of mind!\n\r", ch);
       return;
    }

    
    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
      ch_printf( ch,
	"Reportas: %d/%d hp %d/%d sangue %d/%d mv %d xp.\n\r",
	ch->hit,  ch->max_hit,
	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
	ch->move, ch->max_move,
	ch->exp   );
    else
      ch_printf( ch,
	"Reportas: %d/%d hp %d/%d mana %d/%d mv %d xp.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->exp   );

    if ( IS_VAMPIRE(ch) || IS_DEMON(ch) )
      sprintf( buf, "Reporte de $n: %d/%d hp %d/%d sangue %d/%d mv %d xp.\n\r",
	ch->hit,  ch->max_hit,
	ch->pcdata->condition[COND_BLOODTHIRST], 10 + ch->level,
	ch->move, ch->max_move,
	ch->exp   );
    else
      sprintf( buf, "$n reporta: %d/%d hp %d/%d mana %d/%d mv %d xp.",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->exp   );

    act( AT_REPORT, buf, ch, NULL, NULL, TO_ROOM );

    return;
}

void do_fprompt( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  
  set_char_color( AT_GREY, ch );

  if ( IS_NPC(ch) )
  {
    send_to_char( "NPC's can't change their prompt..\n\r", ch );
    return;
  }
  smash_tilde( argument );
  one_argument( argument, arg );
  if ( !*arg || !str_cmp( arg, "mostrar" ) )
  {
    send_to_char( "Your current fighting prompt string:\n\r", ch );
    set_char_color( AT_WHITE, ch );
    ch_printf( ch, "%s\n\r", !str_cmp( ch->pcdata->fprompt, "" ) ? "(default prompt)"
				 				: ch->pcdata->fprompt );
    set_char_color( AT_GREY, ch );
    send_to_char( "Type 'help prompt' for information on changing your prompt.\n\r", ch );
    return;
  }
  send_to_char( "Replacing old prompt of:\n\r", ch );
  set_char_color( AT_WHITE, ch );
  ch_printf( ch, "%s\n\r", !str_cmp( ch->pcdata->fprompt, "" ) ? "(default prompt)"
							      : ch->pcdata->fprompt );
  if (ch->pcdata->fprompt)
    STRFREE(ch->pcdata->fprompt);
  if ( strlen(argument) > 128 )
    argument[128] = '\0';

  /* Can add a list of pre-set prompts here if wanted.. perhaps
     'prompt 1' brings up a different, pre-set prompt */
  if ( !str_cmp(arg, "default") )
    ch->pcdata->fprompt = STRALLOC("");
  else if ( !str_cmp(arg, "none") )
    ch->pcdata->fprompt = STRALLOC( ch->pcdata->prompt);
  else
    ch->pcdata->fprompt = STRALLOC(argument);
  return;
}

void do_prompt( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  
  set_char_color( AT_GREY, ch );

  if ( IS_NPC(ch) )
  {
    send_to_char( "NPC's can't change their prompt..\n\r", ch );
    return;
  }
  smash_tilde( argument );
  one_argument( argument, arg );
  if ( !*arg || !str_cmp( arg, "display" ) )
  {
    send_to_char( "Your current prompt string:\n\r", ch );
    set_char_color( AT_WHITE, ch );
    ch_printf( ch, "%s\n\r", !str_cmp( ch->pcdata->prompt, "" ) ? "(default prompt)"
				 				: ch->pcdata->prompt );
    set_char_color( AT_GREY, ch );
    send_to_char( "Type 'help prompt' for information on changing your prompt.\n\r", ch );
    return;
  }
  send_to_char( "Replacing old prompt of:\n\r", ch );
  set_char_color( AT_WHITE, ch );
  ch_printf( ch, "%s\n\r", !str_cmp( ch->pcdata->prompt, "" ) ? "(default prompt)"
							      : ch->pcdata->prompt );
  if (ch->pcdata->prompt)
    STRFREE(ch->pcdata->prompt);
  if ( strlen(argument) > 128 )
    argument[128] = '\0';

  /* Can add a list of pre-set prompts here if wanted.. perhaps
     'prompt 1' brings up a different, pre-set prompt */
  if ( !str_cmp(arg, "default") )
    ch->pcdata->prompt = STRALLOC("");
  else if ( !str_cmp(arg, "fprompt") )
    ch->pcdata->prompt = STRALLOC( ch->pcdata->fprompt );
  else
    ch->pcdata->prompt = STRALLOC(argument);
  return;
}

/*
 * Figured this belonged here seeing it involves players... 
 * really simple little function to tax players with a large
 * amount of gold to help reduce the overall gold pool...
 *  --TRI
 */
void tax_player( CHAR_DATA *ch )
{
/*
  int gold = ch->gold;
  int mgold = (ch->level * 2000000);
  int tax = (ch->gold * .05);

  if ( gold > mgold )
  {
    set_char_color( AT_WHITE, ch );
    ch_printf( ch, "Estás level %d and carry more than %d coins.\n\r",
	ch->level, mgold );
    ch_printf( ch, "Estás being taxed \%5 percent (%d coins) of your %d coins,\n\r",
	tax, ch->gold );
    ch_printf( ch, "and that leaves you with %d coins.\n\r",
	(ch->gold - tax));
    ch->gold -= tax;

  }
  return;
*/
}

void do_die ( CHAR_DATA *ch, char *argument ) {
  if ( ch->position > POS_STUNNED ){
     send_to_char("You don't feel like throwing your life away just yet....\n\r", ch);
     return;
  }
  ch->hit = -11;
  update_pos( ch );
  act( AT_DEAD, "$n está MORTO!!", ch, 0, 0, TO_ROOM );
  act( AT_DEAD, "Acabas de MORRER!!\n\r", ch, 0, 0, TO_CHAR );
  raw_kill( ch, ch );
  return;
}

/*
 *  Command to show current favor by Edmond
 */
void do_favor(CHAR_DATA * ch, char *argument)
{
    char            buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) {
	send_to_char("Que dis?\n\r", ch );
	return;
    }
    set_char_color( AT_GREEN, ch );
    if (!ch->pcdata->deity)              sprintf( buf, "N/A" );
    else if (ch->pcdata->favor > 2250)   sprintf( buf, "amado" );
    else if (ch->pcdata->favor > 2000)   sprintf( buf, "querido" );
    else if (ch->pcdata->favor > 1750)   sprintf( buf, "honorado" );
    else if (ch->pcdata->favor > 1500)   sprintf( buf, "rezado" );
    else if (ch->pcdata->favor > 1250)   sprintf( buf, "favorecido" );
    else if (ch->pcdata->favor > 1000)   sprintf( buf, "respetado" );
    else if (ch->pcdata->favor > 750)    sprintf( buf, "gustado" );
    else if (ch->pcdata->favor > 250)    sprintf( buf, "tolerado" );
    else if (ch->pcdata->favor > -250)   sprintf( buf, "ignorado" );
    else if (ch->pcdata->favor > -750)   sprintf( buf, "shunned" );
    else if (ch->pcdata->favor > -1000)  sprintf( buf, "desgustado" );
    else if (ch->pcdata->favor > -1250)  sprintf( buf, "deshonorable" );
    else if (ch->pcdata->favor > -1500)  sprintf( buf, "desowned" );
    else if (ch->pcdata->favor > -1750)  sprintf( buf, "abandoado" );
    else if (ch->pcdata->favor > -2000)  sprintf( buf, "despised" );
    else if (ch->pcdata->favor > -2250)  sprintf( buf, "odiado" );
    else                                 sprintf( buf, "maldito" );

   ch_printf( ch,  "%s considerate para ser %s.\n\r", ch->pcdata->deity->name, buf );
   return;
}

