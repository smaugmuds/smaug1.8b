--- act_comm.c	Mon Jun  2 22:46:07 1997
+++ new/act_comm.c	Tue Jun  3 23:46:46 1997
@@ -1663,7 +1663,7 @@
 		    "????",
 		    "????",
 		    "????",
-		    IS_VAMPIRE(gch) ? "bp" : "mana",
+		    IS_VAMPIRE(gch) || IS_DEMON(gch) ? "bp" : "mana",
 		    "????",  
 		    "????",  
 		    "?????"    );
@@ -1676,10 +1676,10 @@
 		    capitalize( PERS(gch, ch) ),
 		    gch->hit,   
 		    gch->max_hit,
-		    IS_VAMPIRE(gch) ? gch->pcdata->condition[COND_BLOODTHIRST]
+		    IS_VAMPIRE(gch) || IS_DEMON(gch) ? gch->pcdata->condition[COND_BLOODTHIRST]
 				    : gch->mana,
-		    IS_VAMPIRE(gch) ? 10 + gch->level : gch->max_mana,
-		    IS_VAMPIRE(gch) ? "bp" : "mana",
+		    IS_VAMPIRE(gch) || IS_DEMON(gch) ? 10 + gch->level : gch->max_mana,
+		    IS_VAMPIRE(gch) || IS_DEMON(gch) ? "bp" : "mana",
 		    gch->move,  
 		    gch->max_move,  
 		    gch->exp    );
