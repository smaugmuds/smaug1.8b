Area Editor version 1.30
========================

Release notes
-------------

Monday, 29 January 2001


Author: Nick Gammon <nick@gammon.com.au>

-----------------------------------------
If you are reading this file with NotePad,
enable "Word Wrap" under the Edit menu for
proper viewing of it.
-----------------------------------------

Area Editor is produced by Gammon Software Solutions:  http://www.gammon.com.au


Introduction
------------

This program is designed to facilitate editing of the "area" files used by the SMAUG and ROM MUD program.


Installation
------------

1. Create a directory on your disk called "AreaEditor".

2. Use WinZip to unzip all of the files into the AreaEditor directory. Make sure that you have "Use folder names" checked. It is important that the following files are placed in a subdirectory called "spell", otherwise the spell checker will not work properly:

 accent.tlx
 correct.tlx
 Ssce.hlp
 ssceam.tlx
 ssceam2.clx
 userdic.tlx

3. Double-click on AreaEditor.exe to start the Area Editor.

4. Press <F1> for help on using the editor.


What is SMAUG and ROM?
----------------------

SMAUG stands for [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame. 
SMAUG itself is copyright: SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider
You can find the source code for SMAUG, and more about it at: http://www.game.org


ROM 2.4 beta is copyright 1993 - 1996 by Russ Taylor <rtaylor@efn.org>.
You can find the source code for ROM, and more about it at: http://www.cmc.net/~rtaylor/


Program features
----------------

Area Editor lets you edit area files "offline" using a GUI (graphical) interface.

You can:

1. Set attributes for rooms, mobiles etc. using checkboxes, rather than having to add up large numbers.
2. Add, delete or duplicate things in the area. Deleting an item (eg. a room) optionally deletes references to that item (eg. exits leading to the room). Duplicating an item (eg. an object) also duplicates sub-items (eg. programs, affects etc.)
3. Syntax-check MUD programs against your current commands/skills/socials files
4. Do an "area check" to check for missing, or unused, items
5. Renumber the vnums in an area, including references to vnums in MUD programs
6. Cross-reference most things against each other (eg. which mobs are loaded into a particular room)
7. Find things by summary, detail, program contents, or vnum.
8. View, edit, save commands/skills/socials in easy-to-view windows.
9. Print an area to give a hard-copy summary of what all rooms/mobs/objects are.
10. Specify flag names as used in your particular version of the MUD code-base.
11. Do an "area walkthrough" which simulates what you would see as you enter each room in the area.
12. Export an area as MUSH code.
13. Read SMAUG or ROM areas, and convert from one to the other if desired.
14. Keep lists of things (eg. classes, races, skills, gender) in a configuration file, so that they configured to suit a particular MUD implementation.
15. Spell check descriptions as they are entered, or check a whole area as a batch.
16. Work with multiple areas open at once. You can make exits from one area to another, find a string over all open areas, and do an Area Check over all open areas.
17. Open a batch of areas at once (by reading the area.lst file).

Instructions
------------

Press F1 in the program for an explanation of the various screens.


Enhancements in this version  
----------------------------

Changed the program registration system so it is no longer "keyed" to a particular PC. This should greatly simplify using Area Editor on more than one PC, or to re-install it if your hard disk crashes.

If you lose your registration code you can retrieve it by visiting:

http://www.gammon.com.au/mushclient/regcode.htm



Configuration file
------------------

Details about using the Area Editor configuration file are in the separate document: configuration.txt.


Warning regarding making backups
--------------------------------

Although this program has been tested and saved area files seem to work OK, you are strongly advised to back up (ie. make a copy under a different name) any existing area files you have before editing them with this program. This particularly applies if you are about to renumber your area, or convert from SMAUG to ROM, or ROM to SMAUG.


Example area
------------

An example area - manor.are - has been included, so you can play with the Area Editor immediately after downloading it (in case you don't have any area files of your own). This file is from the SMAUG distribution, from www.game.org.


Comments welcome
----------------

You are most welcome to make comments or suggestions. Send comments to:

	Nick Gammon <nick@gammon.com.au>


Registration
------------

This version incorporates a small delay at program start-up for unregistered copies. If you find this annoying, my apologies! The delay screen is eliminated if you register. You may register by fax or email if you wish to register quickly. 

Unregistered versions have the restriction that they do not process the Area Editor configuration file, using inbuilt default values instead. These default values should be fine for evaluating the Area Editor, particularly for SMAUG areas. If you need to change flags, races, classes and other things provided for in the configuration file, then you will need to register the program. Unregistered versions are also limited to 10 spelling errors per session, after which the spell checker is disabled.

Email registration incorporates an "encrypted" credit card number, so that your credit card details are secure, even if the email message is intercepted by third parties. You should receive a registration number within 24 hours of submitting a fax or email registration request. The (snail) mail is another alternative, allow about a week for registration (a week for the mail to arrive, your registration number will be emailed to you).


Comments welcome
----------------

Please let me know if you have any problems. Check out the web pages mentioned above for details about later versions or known bugs.

Send mail to:

Nick Gammon <nick@gammon.com.au>


ShareWare Fee
-------------

There will be a $US 30 ShareWare fee for using Area Editor or $US 40 if paying by check or postal order. 

You can register "online" by visiting:

http://www.gammon.com.au/smaugeditor/fee.htm

This service lets you register your copy and receive a registration code immediately that the order is successfully processed.

Alternatively, the program includes a built-in registration form producer. This will encrypt your credit card details, if you wish to pay by credit card. 

The registration number which you will receive removes the start-up delay, and will continue to work for future versions of Area Editor.

For Australian residents the ShareWare fee is $AUD 45, which includes the GST (Goods and Service Tax).


Spell checker
-------------

The Sentry Spelling-Checker Engine Copyright © 1999 Wintertree Software Inc.


Copyright notice and credits
----------------------------

The world of MUDs is a co-operative effort by many people. For a very interesting history of MUDs (and the Internet in general) see the paper "Cultural Formations in Text-Based Virtual Realities" by Elizabeth M. Reid at: 

http://www.ee.mu.oz.au/papers/emr/cult-form.html 

The sequence of programs that preceded the creation of Area Editor was: 

1. Diku Mud 
2. Merc 
3. ROM
4. SMAUG 
5. Area Editor 

Credits for these programs are given below ... 

* Diku Mud is copyright (C) 1990, 1991 by Sebastian Hammer, Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe. 

* Merc Diku Mud is a derivative of the original Diku Mud. Merc Diku Mud contains substantial enhancements to Diku Mud. These enhancements are copyright 1992, 1993 by Michael Chastain, Michael Quan, and Mitchell Tse. 

* ROM 2.4 is copyright 1993-1998 Russ Taylor. ROM has been brought to you by the ROM consortium. Russ Taylor (rtaylor@hypercube.org), Gabrielle Taylor (gtaylor@hypercube.org) Brian Moore (zump@rom.org).
The full ROM licence agreement is in the file "license.rom" which is supplied as part of the Area Editor archive.

* SMAUG is a derivative of the version 2.1 distribution of Merc Diku Mud. SMAUG contains substantial enhancements to Merc Diku Mud, including some features of the original Diku Mud that were removed by the Merc team. 

* Area Editor does not use code from any of the above programs, however it is designed to edit area files used by the current version of the SMAUG server (1.04) and ROM Server (2.4). SMAUG Editor is copyright (C) 1998 by Nick Gammon of Gammon Software Solutions. 


Thanks!
-------

Thanks to everyone who has supported Area Editor, either by registering or by sending in comments. Your support is appreciated. If you have not yet registered, I will certainly appreciate receiving your ShareWare fee. I have spent many, many hours working on Area Editor. A registration by you is a token of appreciation of that work.

Thanks again. I hope you enjoy using Area Editor, as much as I have writing it.

- Nick Gammon
