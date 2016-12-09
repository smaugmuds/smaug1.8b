/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			                   *
*	ROM has been brought to you by the ROM consortium		               *
*	    Russ Taylor (rtaylor@efn.org)				                       *
*	    Gabrielle Taylor						    				       *
*	    Brian Moore (zump@rom.org)					    				   *
*	By using this code, you have agreed to follow the terms of the         *
*	ROM license, in the file Rom24/doc/rom.license			               *
***************************************************************************/
/***************************************************************************
*	EotN is copyright (C) 1997 by Marcus Speight                           *
*	EotN has been brought to you by the EotN team                          *
*	    Marcus Speight (zortax@usa.net)                                    *
*	    Jonathan White (aldarion@mindless.com)                             *
*	    Martin Dibble  (maetar@earthling.net)                              *
*	    Paul Seward    (voltec@cyberdude.com)                              *
*	By using this code, you have agreed to follow the terms of the         *
*	EotN license, in the file eotn/doc/eotn.license                        *
***************************************************************************/
/***************************************************************************
*                       Magical Tattoo's v1.0                              *
*               Written By: Mirrodan of Techno-Magick                      *
*                 and Harlaquin of Age of the Ancients                     *
* Designed for my mud to balance out the cybernetics availible to the      *
* technology based side. The tattoo's function in much the same way as my  *
* cybernetics with the exception that the players can tattoo other players.*
* The idea will be that as they progress in level more tattoo's will become*
* availible to them. If you would like to implament this code you'll need  *
* to make changes in the following files and include this on in you source *
* directory. You'll also need to make the modifications to mud.h or        *
* simuliar file. This is intended for smaug based muds but i'm sure can be *
* adopted to others.													   *
*   Please leave this and all other credit include in this package.        *
*	Email questions/comments to mirrodan@dreamsphere.org                   *
*                          or											   *
*                   Harlaquin@ancients.org                                 *
***************************************************************************/

/*
build.c
handler.c
mud.h
misc.c
tables.c
save.c

-----------------------------------------------------------------------------------------------

build.c

Find,

char *	const	act_flags [] =

Add "tattooartist"

Find,

do_mset
and edit it to show that you've added the act_flags to the mob


-------------------------------------------------------------------------------------------------

tables.c

Find,

	if ( !str_cmp( name, "do_tamp" ))		return do_tamp;

Under it add,

	if ( !str_cmp( name, "do_tattoo" ))		return do_tattoo;

Find,
    if ( skill == do_cset )		return "do_cset";

Under it add,

    if ( skill == do_tattoo )		return "do_tattoo";

-------------------------------------------------------------------------------------------------

save.c

Find,

    fprintf( fp, "Damroll      %d\n",	ch->damroll		);

Under it add,

    fprintf( fp, "Tattoo        %ld\n",  ch->tattoo               );

Find,

	    ch->pcdata->title = fread_string( fp );
		if ( isalpha(ch->pcdata->title[0])
		||   isdigit(ch->pcdata->title[0]) )
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    if ( ch->pcdata->title )
		      STRFREE( ch->pcdata->title );
		    ch->pcdata->title = STRALLOC( buf );
		}
		fMatch = TRUE;
		break;
	    }

Under it add,

	    KEY( "Tattoo",       ch->tattoo,               fread_number( fp ) );

-------------------------------------------------------------------------------------------------
 In mud.h you'll need to add the following in....
 
 find under the handler.c section 
   OD *	get_objtype	args( ( CHAR_DATA *ch, sh_int type ) );
   
and add this 
    char *  tattoo_bit_name  args( ( int tatto_flags ) );
   
find this     
    DECLARE_DO_FUN(	do_tamp		);

and add under it this
    DECLARE_DO_FUN( do_tattoo   );
    
find 
  
in 

find ACT bits for mobs.
add in where ever there's an availible slot for it replace the ?? with the appropriate number.

#define ACT_IS_TATTOOARTIST           BV??

Then find the struct     char_data section and go to the bottom of it and add,

    long		tattoo;

--------------------------------------------------------------------------------------------------------------
 
Lastly once you're all done you'll have to install the command online as follows....
   
    cedit tattoo create do_tattoo
    cedit tattoo level ? (whatever level you would like the players to have access to this function)
    cedit save cmdtable
    
***************************************************************************************************************/


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mud.h"

/* Defining the bit vectors for tattoos */
#define TATTOO_BULL				BV00
#define TATTOO_PANTHER			BV01
#define TATTOO_WOLF				BV02
#define TATTOO_BEAR				BV03
#define TATTOO_RABBIT			BV04
#define TATTOO_DRAGON			BV05
#define TATTOO_MOON				BV06

char *tattoo_bit_name(int tattoo_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if ( (tattoo_flags & TATTOO_BULL)    == TATTOO_BULL		                           ) strcat(buf, " A raging bull on your right arm.  \n\r");
    if ( (tattoo_flags & TATTOO_PANTHER) == TATTOO_PANTHER	                           ) strcat(buf, " A sleek black panther on your left arm. \n\r");
    if ( (tattoo_flags & TATTOO_WOLF)    == TATTOO_WOLF		                           ) strcat(buf, " A wolk stalking it's prey on your right leg. \n\r");
    if ( (tattoo_flags & TATTOO_BEAR)    == TATTOO_BEAR		                           ) strcat(buf, " A giant sleeping bear on your left leg. \n\r");
    if ( (tattoo_flags & TATTOO_RABBIT)  == TATTOO_RABBIT	                           ) strcat(buf, " A little white rabbit on your ankle. \n\r");
    if ( (tattoo_flags & TATTOO_DRAGON)  == TATTOO_DRAGON	                           ) strcat(buf, " A great flying flaming dragon on your chest.\n\r");
    if ( (tattoo_flags & TATTOO_MOON)    == TATTOO_MOON		                           ) strcat(buf, " A crescent moon across your back. \n\r");
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

//---------------------------------------------------------------------------------------------


void do_tattoo(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int cost;

    /* check for artist */
    for ( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && xIS_SET(mob->act, ACT_IS_TATTOOARTIST) )
            break;
    }

    /* if there are none , display the characters tattoos*/ 
    if ( mob == NULL )
    {
    sprintf(buf, "&C+----------------- Magical Tattoos -----------------------+&W\n\r %s\n\r", 
	tattoo_bit_name(ch->tattoo));
    send_to_char(buf,ch);
	return;
    }

    one_argument(argument,arg);

	/* if there are a surgeon, give a list*/
    if (arg[0] == '\0')
    {
        /* display price list */
	act(AT_CYAN,"&z&C$N says 'I have these parts in stock:'",ch,NULL,mob,TO_CHAR);
	send_to_char("  &z&W+---Tattoos---+                             +--------Cost-------- \n\r",ch);
	send_to_char("  &CRaging_Bull                                   &Y350,000 &CGold\n\r",ch);
	send_to_char("  &CSleek_Panther                                 &Y350,000 &CGold\n\r",ch);
	send_to_char("  &CStalking_Wolf                                 &Y350,000 &CGold\n\r",ch);
	send_to_char("  &CSleeping_Bear                                 &Y450,000 &CGold\n\r",ch);
	send_to_char("  &CRabbit                                        &Y150,000 &CGold\n\r",ch);
	send_to_char("  &CFlying_Dragon                               &Y1,000,000 &CGold\n\r",ch);
	send_to_char("  &CRising_Moon                                   &Y750,000 &CGold\n\r",ch);
    send_to_char(" &CType &Wtattoo &B<&Wtype&B>&C to buy one, or help tattoo to get more info.\n\r",ch);
	return;
    }

    /* Lets see what the character wants to have */
    
// Raging Bull
    
    if (!str_prefix(arg,"raging_bull"))
    {
    cost  = 350000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_BULL))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_BULL );	
    ch->perm_str += 3;
    ch->perm_con += 3;
    }
// Sleek Panther
    else if (!str_prefix(arg,"sleek_panther"))
    {
    cost  = 350000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_PANTHER))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_PANTHER );	
   xSET_BIT (ch->affected_by, AFF_HIDE );
    ch->perm_dex += 2;
    ch->armor -= 10;
    }
//  Stalking Wolf
    else if (!str_prefix(arg,"stalking_wolf"))
    {
    cost  = 350000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_WOLF))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_WOLF );	
    xSET_BIT (ch->affected_by, AFF_SNEAK );
    ch->perm_dex += 2;
    ch->armor -= 10;
    }
   
// Sleeping Bear
    else if (!str_prefix(arg,"slalking_wolf"))
    {
    cost  = 450000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_BEAR))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_BEAR );	
    SET_BIT (ch->immune, RIS_SLEEP );
    SET_BIT (ch->resistant, RIS_NONMAGIC );
    ch->perm_str += 2;
    ch->perm_con += 4;
    ch->damroll += 20;
    }
// Rabbit
    else if (!str_prefix(arg,"Rabbit"))
    {
    cost  = 150000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_RABBIT))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_RABBIT );	
    ch->max_move += 50;
    ch->perm_dex += 2;
    ch->armor -= 10;
    }

// Flying Dragon
    else if (!str_prefix(arg,"Flying_Dragon"))
    {
    cost  = 1000000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_DRAGON))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_DRAGON );	
    ch->max_hit += 100;
    ch->hitroll += 25;
    ch->damroll += 25;
    ch->armor   -= 50;
    ch->perm_str += 2;
    ch->perm_dex += 2;
    ch->perm_con += 2;
    ch->perm_int += 2;
    ch->perm_wis += 2;
    ch->perm_cha += 2;
    ch->perm_lck += 2;
    xSET_BIT (ch->affected_by, AFF_FLYING );
    SET_BIT (ch->resistant, RIS_MAGIC);
    }

// Rising Moon 
    else if (!str_prefix(arg,"Rising_Moon"))
    {
    cost  = 750000;
    if (cost > (ch->gold))
    {
	act(AT_CYAN,"$N says 'I'm sorry but please return to me when you have more gold.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }
	if (IS_SET(ch->tattoo,TATTOO_MOON))
		{
		send_to_char( "You already have that tattoo.\n\r", ch );
		return;
		}
    SET_BIT (ch->tattoo, TATTOO_MOON );	
    xSET_BIT (ch->affected_by, AFF_TRUESIGHT );
    SET_BIT (ch->resistant, RIS_DRAIN );
    ch->max_mana += 200;
    ch->perm_int += 2;
    }        
    else 
    {
	act(AT_CYAN,"$N says 'Type 'tattoo' for a list of tattoos.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    ch->gold -= cost;
    act(AT_CYAN,"$n says 'There we go, all finished'.",mob,NULL,NULL,TO_ROOM); 
}



