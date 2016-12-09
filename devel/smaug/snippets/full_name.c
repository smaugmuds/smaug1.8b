==========================================
"FULL_NAME" Command flag snippet for SMAUG
==========================================

===========
It's origin
===========
This snippet was found lingering in my hard drive. The original author of
this snippet is unknown, but this snippet includes some modifications
I made. If you happen to know who the original author is leave a comment
and I'll update this file.

============
It's purpose
============
This snippet adds a new flag that can be added to commands. Commands enabled
with the flag will force the player/typer/user to type the entire command
instead of just typing the first few letters of the command. (For example,
typing "hot" instead of "hotboot".) It prevents accidents from happening,
well almost. :p So this removes the needs for commands such as 'reboo'
to nanny things you dont want accidentally triggered.

=================
It's distribution
=================
Please do not distribute this snippet. This snippet was intended to be
distributed only at MudBytes. And as always please do not claim this snippet
as your own.

==================
It's compatibility
==================
This snippet was installed on SMAUGFUSS 1.4 and should work for any SMAUG MUD.
Though adjustments can be made to the code and make it compatible with other
codebases. (ROM, Circle, etc.)

=====
Misc.
=====
If updates/fixes are made to the code you can submit them at MudBytes (leave a comment)
and this file will be updated to include your fix/update and you will be given credit.
If you find a bug with the snippet, post it at MudBytes and I will try to assist you.
If for some reason you need to contact me email me at "gundam00692000@hotmail.com".

++++++++++++++++++++++
"INSTALLATION PROCESS"
++++++++++++++++++++++

===============
In file "mud.h"
===============
After: (or after the last CMD define)
	#define CMD_WATCH		BV02	/* FB */

Add:
	#define CMD_FULLNAME		BV03	/* Commands flagged with this must be typed entirely. */

=================
In file "build.c"
=================
Locate:
	char *const cmd_flags[] =
	{
		"possessed", "polymorphed", "watch", "r2", "r3", "r4", "r5", "r6", "r7",

Change:
	char *const cmd_flags[] =
	{
		"possessed", "polymorphed", "watch", "fullname", "r3", "r4", "r5", "r6", "r7",

==================
In file "interp.c"
==================
Locate:
	void interpret( CHAR_DATA * ch, char *argument )
	{
		char command[MAX_INPUT_LENGTH];
		char logline[MAX_INPUT_LENGTH];
		char logname[MAX_INPUT_LENGTH];
		char *buf;
		TIMER *timer = NULL;
		CMDTYPE *cmd = NULL;
		int trust;
		int loglvl;
		struct timeval time_used;
		long tmptime;
		bool found;

Add: (under 'bool found')
		bool full_name = FALSE;
		char buffer[MAX_STRING_LENGTH];

Change: (in 'void interp' func.)
	for ( cmd = command_hash[LOWER( command[0] ) % 126]; cmd; cmd = cmd->next )
		if ( !str_prefix( command, cmd->name ) && ( cmd->level <= trust
		|| ( !IS_NPC( ch ) && ch->pcdata->council && is_name( cmd->name, ch->pcdata->council->powers )
		&& cmd->level <= ( trust + MAX_CPD ) ) || ( !IS_NPC( ch ) && ch->pcdata->bestowments
		&& ch->pcdata->bestowments[0] != '\0' && is_name( cmd->name, ch->pcdata->bestowments )
		&& cmd->level <= ( trust + sysdata.bestow_dif ) ) ) )
		{
			found = TRUE;
			break;
		}

To:
	for ( cmd = command_hash[LOWER( command[0] ) % 126]; cmd; cmd = cmd->next )
	{
		if ( ( IS_SET( cmd->flags, CMD_FULLNAME ) && str_cmp( command, cmd->name )
		&& !str_prefix( command, cmd->name ) ) )
		{
			full_name = TRUE;
			break;
		}

		if ( ( ( !IS_SET( cmd->flags, CMD_FULLNAME ) && !str_prefix( command, cmd->name ) )
		|| ( IS_SET( cmd->flags, CMD_FULLNAME ) && !str_cmp( command, cmd->name ) ) )
		&& ( cmd->level <= trust || ( !IS_NPC( ch ) && ch->pcdata->council
		&& is_name( cmd->name, ch->pcdata->council->powers ) && cmd->level <= ( trust + MAX_CPD ) )
		|| ( !IS_NPC( ch ) && ch->pcdata->bestowments && ch->pcdata->bestowments[0] != '\0'
		&& is_name( cmd->name, ch->pcdata->bestowments )
		&& cmd->level <= ( trust + sysdata.bestow_dif ) ) ) )
		{
			found = TRUE;
			break;
		}
	}

Locate:
	/*
	 * Look for command in skill and socials table.
	 */
	if ( !found )
	{
		if ( !check_skill( ch, command, argument ) && !check_social( ch, command, argument ) )
		{
			EXIT_DATA *pexit;

Add: (above the 'if ( !found )')

	/*
	 * Tell the typer to type the entire command. -Anavel
	 */
	if ( full_name )
	{
		sprintf( buffer, "&RIf you want to '%s', then type the entire command!&D\r\n", strupper( cmd->name ) );
		send_to_char( buffer, ch );
		return;
	}

==========================================
NOTE: Remeber to 'make clean' and compile.
==========================================
Once your done you can remove commands such as 'do_reboo', do_sla', do_qui', etc. from
the code. Then when online use the cedit command to add the flag to commands that you
want to be typed entirely. (IE: do_slay, do_quit, do_destroy, do_delete, do_shutdown,
do_reboot, do_freeze, do_hotboot, do_authorize, do_dump, do_silence, do_hlist, etc.)
Enjoy!
