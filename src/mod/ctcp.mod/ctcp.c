/*
 * ctcp.c - all the ctcp handling (except DCC, it's special ;)
 *
 */

#define MAKING_WOOBIE
#define MODULE_NAME "ctcp"
#include "ctcp.h"
#include "../module.h"
#include "../server.mod/server.h"
#include <netinet/in.h>
#include <arpa/inet.h>


static Function * global = NULL, * server_funcs = NULL;

static char ctcp_version[256];
static char ctcp_finger[256];
static char ctcp_userinfo[256];

static int ctcp_FINGER(char * nick, char * uhost, char * handle, 
		 char * object, char * keyword, char * text) {
   context;
   if (ctcp_finger[0]) 
     simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],
		    "\001FINGER %s\001", ctcp_finger);
   return 1;
}

static int ctcp_ECHOPINGERR (char * nick, char * uhost, char * handle,
		       char * object, char * keyword, char * text) {
   context;
   if (strlen(text) <= 80) /* bitch ignores > 80 */
     simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],"\001%s %s\001",
		    keyword,text);
   return 1;
}

static int ctcp_VERSION(char * nick, char * uhost, char * handle, 
		 char * object, char * keyword, char * text) {
   context;
   if (ctcp_version[0]) 
     simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],
		    "\001VERSION %s\001", ctcp_version);
   return 1;
}

static int ctcp_USERINFO(char * nick, char * uhost, char * handle, 
		 char * object, char * keyword, char * text) {
   context;
   if (ctcp_userinfo[0]) 
     simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],
		    "\001USERINFO %s\001", ctcp_userinfo);
   return 1;
}

static int ctcp_CLIENTINFO(char * nick, char * uhosr, char * handle,
		     char * object, char * keyword, char * msg) {
   char * p = NULL;
   
   context;
   if (!msg[0])
     p = CLIENTINFO;
   else if (strcasecmp(msg, "sed") == 0)
     p = CLIENTINFO_SED;
   else if (strcasecmp(msg, "version") == 0)
     p = CLIENTINFO_VERSION;
   else if (strcasecmp(msg, "clientinfo") == 0)
     p = CLIENTINFO_CLIENTINFO;
   else if (strcasecmp(msg, "userinfo") == 0)
     p = CLIENTINFO_USERINFO;
   else if (strcasecmp(msg, "errmsg") == 0)
     p = CLIENTINFO_ERRMSG;
   else if (strcasecmp(msg, "finger") == 0)
     p = CLIENTINFO_FINGER;
   else if (strcasecmp(msg, "time") == 0)
     p = CLIENTINFO_TIME;
   else if (strcasecmp(msg, "action") == 0)
     p = CLIENTINFO_ACTION;
   else if (strcasecmp(msg, "dcc") == 0)
     p = CLIENTINFO_DCC;
   else if (strcasecmp(msg, "utc") == 0)
     p = CLIENTINFO_UTC;
   else if (strcasecmp(msg, "ping") == 0)
     p = CLIENTINFO_PING;
   else if (strcasecmp(msg, "echo") == 0)
     p = CLIENTINFO_ECHO;
   if (p == NULL) {
      simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],
		     "\001ERRMSG CLIENTINFO: %s is not a valid function\001",
		     msg);
      } else
	 simple_sprintf(&ctcp_reply[strlen(ctcp_reply)],
			"\001CLIENTINFO %s\001", p);
   return 1;
}

static int ctcp_TIME (char * nick, char * uhost, char * handle, char * object,
		char * keyword, char * text) {
      char tms[81];
   
   context;
   strcpy(tms, ctime(&now));
   tms[strlen(tms) - 1] = 0;
   simple_sprintf(&ctcp_reply[strlen(ctcp_reply)], "\001TIME %s\001", tms);
   return 1;
}

static int ctcp_CHAT (char * nick, char * uhost, char * handle, char * object,
		char * keyword, char * text) {
   struct userrec * u = get_user_by_handle(userlist,handle);
   int atr = u ? u->flags : 0, i, ix = (-1);
   
   context;
   if ((atr & (USER_PARTY | USER_XFER)) ||
       ((atr & USER_OP) && !require_p)) {
      for (i = 0; i < dcc_total; i++) {
	 if ((dcc[i].type->flags & DCT_LISTEN) &&
	     ((strcmp(dcc[i].nick, "(telnet)") == 0) ||
	      (strcmp(dcc[i].nick, "(users)") == 0))) {
	    ix = i;
	    /* do me a favour and don't change this back to a CTCP reply, */
	    /* CTCP replies are NOTICE's this has to be a PRIVMSG */
	    /* -poptix 5/1/97 */
	    dprintf(DP_SERVER, "PRIVMSG %s :\001DCC CHAT chat %lu %u\001\n",
		    nick, 
		    iptolong(natip[0]?(IP) inet_addr(natip):getmyip()),
		    dcc[ix].port);
	 }
      }
      if (ix < 0)
	simple_sprintf(&ctcp_reply[strlen(ctcp_reply)], 
		       "\001ERROR no telnet port\001");
   }
   return 1;
}

static cmd_t myctcp[9] =
{
     { "FINGER", "", ctcp_FINGER, NULL },
     { "ECHO", "", ctcp_ECHOPINGERR, NULL },
     { "PING", "", ctcp_ECHOPINGERR, NULL },
     { "ERRMSG", "", ctcp_ECHOPINGERR, NULL },
     { "VERSION", "", ctcp_VERSION, NULL },
     { "USERINFO", "", ctcp_USERINFO, NULL },
     { "CLIENTINFO", "", ctcp_CLIENTINFO, NULL },
     { "TIME", "", ctcp_TIME, NULL },
     { "CHAT", "", ctcp_CHAT, NULL },
};

static tcl_strings mystrings [] =
{
     {"ctcp-version", ctcp_version, 120, 0},
     {"ctcp-finger", ctcp_finger, 120, 0},
     {"ctcp-userinfo", ctcp_userinfo, 120, 0},
     {0, 0, 0, 0}
};

static char *ctcp_close()
{
   rem_tcl_strings(mystrings);
   rem_builtins(H_ctcp, myctcp,9);
   rem_help_reference("ctcp.help");
   module_undepend(MODULE_NAME);
   return NULL;
}

char *ctcp_start ();

static Function ctcp_table[] =
{
   (Function) ctcp_start,
   (Function) ctcp_close,
   (Function) 0,
   (Function) 0,
};

char *ctcp_start (Function * global_funcs)
{
   global = global_funcs;
   context;
   module_register(MODULE_NAME, ctcp_table, 1, 0);
   if (!(server_funcs = module_depend(MODULE_NAME, "server", 1, 0)))
      return "You need the server module to user the ctcp module.";
   add_tcl_strings(mystrings);
   add_builtins(H_ctcp, myctcp,9);
   add_help_reference("ctcp.help");
   if (!ctcp_version[0]) {
      strncpy(ctcp_version, ver, 160);
      ctcp_version[160] = 0;
   }
   if (!ctcp_finger[0]) {
      strncpy(ctcp_finger, ver, 160);
      ctcp_finger[160] = 0;
   }
   if (!ctcp_userinfo[0]) {
      strncpy(ctcp_userinfo, ver, 160);
      ctcp_userinfo[160] = 0;
   }
   return NULL;
}
