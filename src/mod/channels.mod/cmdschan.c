/* 
 * cmdsirc.c -- handles:
 * commands from a user via dcc that cause server interaction.
 */
/*
 * This file is part of the eggdrop source code
 * copyright (c) 1997 Robey Pointer
 * and is distributed according to the GNU general public license.
 * For full details, read the top of 'main.c' or the file called
 * COPYING that was distributed with this code.
 */

#include <ctype.h>

static struct flag_record user = {
   FR_GLOBAL|FR_CHAN, 0,0,0,0,0
};
static struct flag_record victim = {
   FR_GLOBAL|FR_CHAN, 0,0,0,0,0
};

static void cmd_pls_ban (struct userrec * u, int idx, char * par)
{
   char * chname, * who, s[UHOSTLEN + 1], s1[UHOSTLEN + 1], *p;
   struct chanset_t *chan = 0;
   int bogus = 0;
   module_entry * me;   
   /* The two lines below added for bantime */
   unsigned long int expire_time = 0, expire_foo;
   char * p_expire;

   if (!par[0]) {
      dprintf(idx, "Usage: +ban <hostmask> [channel] [%%bantime<XdXhXm>] [reason]\n");
   } else {
      who = newsplit(&par);
      for (p = who; *p; p++)
	if (((*p < 32) || (*p == 127)) &&
	    (*p != 2) && (*p != 22) && (*p != 31))
	  bogus = 1;
      if (bogus) { 
	 dprintf(idx, "That is a bogus ban!\n");
	 return;
      }
      remove_gunk(who);
      if ((par[0] == '#') || (par[0] == '&') || (par[0] == '+')) 
	chname = newsplit(&par);
      else
	chname = 0;
      if (chname || !(u->flags & USER_MASTER)) {
	 if (!chname)
	   chname = dcc[idx].u.chat->con_chan;
	 get_user_flagrec(u,&user,chname);
	 chan = findchan(chname);
	 if (!chan || !((glob_op(user) && !chan_deop(user)) || chan_op(user)))
	   return;
      } else 
	chan = 0;
      /* Added by Q and Solal  - Requested by Arty2, special thanx :) */
      if (par[0] == '%') {
         p = newsplit (&par);
	 p_expire = p + 1;
	 while (*(++p) != 0) {
	    switch (tolower(*p)) {
	      case 'd':
	       *p = 0;
	       expire_foo = strtol (p_expire, NULL, 10);
	       if (expire_foo > 365) 
		 expire_foo = 365;
	       expire_time += 86400 * expire_foo;
	       p_expire = p + 1;
	       break;
	      case 'h':
	       *p = 0;
	       expire_foo = strtol (p_expire, NULL, 10);
	       if (expire_foo > 8760)
		 expire_foo = 8760;
	       expire_time += 3600 * expire_foo;
	       p_expire = p + 1;
	       break;
	      case 'm':
	       *p = 0;
	       expire_foo = strtol (p_expire, NULL, 10);
	       if (expire_foo > 525600) 
		 expire_foo = 525600;
	       expire_time += 60 * expire_foo;
	       p_expire = p + 1;
	    }
	 }
      }
      if (!par[0])
	par = "requested";
      else if (strlen(par) > 65)
	par[65] = 0;
      if (strlen(who) > UHOSTLEN - 4)
	who[UHOSTLEN - 4] = 0;
      /* fix missing ! or @ BEFORE checking against myself */
      if (!strchr(who, '!')) {
	 if (!strchr(who, '@')) 
	   simple_sprintf(s, "%s!*@*", who);	/* lame nick ban */
	 else
	   simple_sprintf(s, "*!%s",who);
      } else if (!strchr(who, '@'))
	   simple_sprintf(s, "%s@*", who);		/* brain-dead? */
      else
	strcpy(s, who);
      if ((me = module_find("server",0,0)) && me->funcs)
	simple_sprintf(s1, "%s!%s", me->funcs[4], me->funcs[5]);
      else
	simple_sprintf(s1, "%s!%s@%s", origbotname, botuser, hostname);
      if (wild_match(s, s1)) {
	 dprintf(idx, "Duh...  I think I'll ban myself today, Marge!\n");
	 putlog(LOG_CMDS, "*", "#%s# attempted +ban %s", dcc[idx].nick, s);
      } else { 
	 if (strlen(s) > 70) {
	    s[69] = '*';
	    s[70] = 0;
	 }
	 /* irc can't understand bans longer than that */
	 if (chan) {
	    u_addban(chan, s, dcc[idx].nick, par,
		     expire_time ? now + expire_time : 0, 0);
	    if (par[0] == '*') {
	       par++;
	       putlog(LOG_CMDS, "*", "#%s# (%s) +ban %s %s (%s) (sticky)", dcc[idx].nick,
		      dcc[idx].u.chat->con_chan, s, chan->name, par);
	       dprintf(idx, "New %s sticky ban: %s (%s)\n", chan->name, s, par);
	    } else {
	       putlog(LOG_CMDS, "*", "#%s# (%s) +ban %s %s (%s)", dcc[idx].nick,
		      dcc[idx].u.chat->con_chan, s, chan->name, par);
	       dprintf(idx, "New %s ban: %s (%s)\n", chan->name, s, par);
	    }
  	    add_mode(chan, '+', 'b', s);
  	 } else {
	    u_addban(NULL, s, dcc[idx].nick, par, 
		     expire_time ? now + expire_time : 0, 0);
	    if (par[0] == '*') {
	       par++;
	       putlog(LOG_CMDS, "*", "#%s# (GLOBAL) +ban %s (%s) (sticky)", dcc[idx].nick,
		      s, par);
	       dprintf(idx, "New sticky ban: %s (%s)\n", s, par);
	    } else {
	       putlog(LOG_CMDS, "*", "#%s# (GLOBAL) +ban %s (%s)", dcc[idx].nick,
		      s, par);
	       dprintf(idx, "New ban: %s (%s)\n", s, par);
	    }
	    chan = chanset;
	    while (chan != NULL) {
	       add_mode(chan, '+', 'b', s);
	       chan = chan->next;
	    }
	 }
      }
   }
}

static void cmd_mns_ban (struct userrec * u, int idx, char * par)
{
   int i = 0, j;
   struct chanset_t *chan = 0;
   char s[UHOSTLEN + 1], *ban, *chname;
   banlist *b;
   
   if (!par[0]) {
      dprintf(idx, "Usage: -ban <hostmask|ban #> [channel]\n");
      return;
   }
   ban = newsplit(&par);
   if ((par[0] == '#') || (par[0] == '&') || (par[0] == '+')) 
     chname = newsplit(&par);
   else
     chname = dcc[idx].u.chat->con_chan;
   if (chname || !(u->flags & USER_MASTER)) {
      if (!chname)
	chname = dcc[idx].u.chat->con_chan;
      get_user_flagrec(u,&user,chname);
      if (!((glob_op(user) && !chan_deop(user)) || chan_op(user)))
	return;
   }
   strncpy(s,ban, UHOSTLEN);
   s[UHOSTLEN] = 0;
   i = u_delban(NULL,s,(u->flags & USER_MASTER));
   if (i > 0) {
      putlog(LOG_CMDS, "*", "#%s# -ban %s", dcc[idx].nick, s);
      dprintf(idx, "%s: %s\n", IRC_REMOVEDBAN, s);
      chan = chanset;
      while (chan) {
	 add_mode(chan, '-', 'b', s);
	 chan = chan->next;
      }
      return;
   }
   /* channel-specific ban? */
   if (chname)
     chan = findchan(chname);
   if (chan) {
      if (atoi(ban) > 0) {
	 simple_sprintf(s, "%d", -i);
	 j = u_delban(chan, s, 1);
	 if (j > 0) {
	    putlog(LOG_CMDS, "*", "#%s# (%s) -ban %s", dcc[idx].nick,
		   chan->name, s);
	    dprintf(idx, "Removed %s channel ban: %s\n", chan->name, s);
	    add_mode(chan, '-', 'b', s);
	    return;
	 }	 
	 i = 0;
	 for (b = chan->channel.ban;b->ban[0];b=b->next) {
	    if ((!u_equals_ban(global_bans,b->ban)) 
		&& (!u_equals_ban(chan->bans, b->ban))) {
	       i++;
	       if (i == -j) {
		  add_mode(chan, '-', 'b', b->ban);
		  dprintf(idx, "%s '%s' on %s.\n", IRC_REMOVEDBAN,
			  b->ban, chan->name);
		  putlog(LOG_CMDS, "*", "#%s# (%s) -ban %s [on channel]",
			 dcc[idx].nick, dcc[idx].u.chat->con_chan, ban);
		  return;
	       }
	    }
	 }
      } else {
	 j = u_delban(chan, ban, 1);
	 if (j > 0) {
	    putlog(LOG_CMDS, "*", "#%s# (%s) -ban %s", dcc[idx].nick,
		   dcc[idx].u.chat->con_chan, ban);
	    dprintf(idx, "Removed %s channel ban: %s\n", chname, ban);
	    add_mode(chan, '-', 'b', ban);
	    return;
	 }
	 for (b = chan->channel.ban;b->ban[0];b=b->next) {
	    if (!strcasecmp(b->ban, ban)) {
	       add_mode(chan, '-', 'b', b->ban);
	       dprintf(idx, "%s '%s' on %s.\n", 
			IRC_REMOVEDBAN, b->ban, chan->name);
	       putlog(LOG_CMDS, "*", "#%s# (%s) -ban %s [on channel]",
		      dcc[idx].nick, dcc[idx].u.chat->con_chan, ban);
	       return;
	    }
	 }
      }
   }
   dprintf(idx, "No such ban.\n");
}

static void cmd_bans (struct userrec * u, int idx, char * par)
{
   if (!strcasecmp(par, "all")) {
      putlog(LOG_CMDS, "*", "#%s# bans all", dcc[idx].nick);
      tell_bans(idx, 1, "");
   } else {
      putlog(LOG_CMDS, "*", "#%s# bans %s", dcc[idx].nick, par);
      tell_bans(idx, 0, par);
   }
}

static void cmd_info (struct userrec * u, int idx, char * par)
{
   char s[512], *chname, * s1;
   int locked = 0;
   
   if (!use_info) {
      dprintf(idx, "Info storage is turned off.\n");
      return;
   }
   s1 = get_user(&USERENTRY_INFO, u);
   if (s1 && s1[0] == '@')
     locked = 1;
   if ((par[0] == '#') || (par[0] == '+') || (par[0] == '&')) {
      chname = newsplit(&par);
      if (!findchan(chname)) {
	 dprintf(idx, "No such channel.\n");
	 return;
      }
      get_handle_chaninfo(dcc[idx].nick, chname, s);
      if (s[0] == '@')
	locked = 1;
      s1 = s;
   } else
     chname = 0;
   if (!par[0]) {
      if (s1 && s1[0] == '@')
	s1++;
      if (s1 && s1[0]) {
	 if (chname) {
	    dprintf(idx, "Info on %s: %s\n", chname, s1);
	    dprintf(idx, "Use '.info %s none' to remove it.\n", chname);
	 } else {
	    dprintf(idx, "Default info: %s\n", s1);
	    dprintf(idx, "Use '.info none' to remove it.\n");
	 }
      } else
	dprintf(idx, "No info has been set for you.\n");
      putlog(LOG_CMDS, "*", "#%s# info %s", dcc[idx].nick, chname?chname:"");
      return;
   }
   if (locked && !(u && (u->flags & USER_MASTER))) {
      dprintf(idx, "Your info line is locked.  Sorry.\n");
      return;
   }
   if (strcasecmp(par, "none") == 0) {
      if (chname) {
	 par[0] = 0;
	 set_handle_chaninfo(userlist, dcc[idx].nick, chname, par);
	 dprintf(idx, "Removed your info line on %s.\n", chname);
	 putlog(LOG_CMDS, "*", "#%s# info %s none", dcc[idx].nick, chname);
      } else {
	 set_user(&USERENTRY_INFO,u,NULL);
	 dprintf(idx, "Removed your default info line.\n");
	 putlog(LOG_CMDS, "*", "#%s# info none", dcc[idx].nick);
      }
      return;
   }
   if (par[0] == '@')
     par++;
   if (chname) {
      set_handle_chaninfo(userlist, dcc[idx].nick, chname, par);
      dprintf(idx, "Your info on %s is now: %s\n", chname, par);
      putlog(LOG_CMDS, "*", "#%s# info %s ...", dcc[idx].nick, chname);
   } else {
      set_user(&USERENTRY_INFO,u,par);
      dprintf(idx, "Your default info is now: %s\n", par);
      putlog(LOG_CMDS, "*", "#%s# info ...", dcc[idx].nick);
   }
}

static void cmd_chinfo (struct userrec * u, int idx, char * par)
{
   char * handle, * chname;
   struct userrec * u1;

   if (!use_info) {
      dprintf(idx, "Info storage is turned off.\n");
      return;
   }
   handle = newsplit(&par);
   if (!handle[0]) {
      dprintf(idx, "Usage: chinfo <handle> [channel] <new-info>\n");
      return;
   }
   u1 = get_user_by_handle(userlist,handle);
   if (!u1) {
      dprintf(idx, "No such user.\n");
      return;
   }
   if ((par[0] == '#') || (par[0] == '+') || (par[0] == '&')) {
      chname = newsplit(&par);
      if (!findchan(chname)) {
         dprintf(idx, "No such channel.\n");
         return;
      }
   } else
      chname = 0;
   if (u1->flags & USER_BOT) {
      dprintf(idx, "Useful only for users.\n");
      return;
   }
   if ((u1->flags & USER_OWNER) && !(u->flags & USER_OWNER)) {
      dprintf(idx, "Can't change info for the bot owner.\n");
      return;
   }
   if (chname) {
      get_user_flagrec(u,&user,chname);
      get_user_flagrec(u1,&victim,chname);
      if ((chan_owner(victim) || glob_owner(victim)) 
	  && !(glob_owner(user) || chan_owner(user))) {
	 dprintf (idx, "Can't change info for the channel owner.\n");
	 return;
      }
   }
   putlog(LOG_CMDS, "*", "#%s# chinfo %s %s %s", dcc[idx].nick, handle, 
	  chname?chname:par, chname?par:"");
   if (!strcasecmp(par, "none"))
      par[0] = 0;
   if (chname) {
      set_handle_chaninfo(userlist, handle, chname, par);
      if (par[0] == '@')
	dprintf(idx, "New info (LOCKED) for %s on %s: %s\n", handle, chname, &par[1]);
      else if (par[0])
	dprintf(idx, "New info for %s on %s: %s\n", handle, chname, par);
      else
	dprintf(idx, "Wiped info for %s on %s\n", handle, chname);
   } else {
      set_user(&USERENTRY_INFO,u1,par[0]?par:NULL);
      if (par[0] == '@')
	dprintf(idx, "New default info (LOCKED) for %s: %s\n", handle, &par[1]);
      else if (par[0])
	dprintf(idx, "New default info for %s: %s\n", handle, par);
      else
	dprintf(idx, "Wiped default info for %s\n", handle);
   }
}

static void cmd_stick_yn(int idx, char *par, int yn)
{
   int i, j;
   struct chanset_t *chan;
   char s[UHOSTLEN+1];
   
   if (!par[0]) {
      dprintf(idx, "Usage: %sstick <ban>\n", yn ? "" : "un");
      return;
   }
   strncpy(s, par, UHOSTLEN);
   s[UHOSTLEN] = 0;
   i = u_setsticky_ban(NULL, s,
		       (dcc[idx].user->flags & USER_MASTER) ? yn : -1);
   if (i > 0) {
      putlog(LOG_CMDS, "*", "#%s# %sstick %s", 
	     dcc[idx].nick, yn ? "" : "un", s);
      dprintf(idx, "%stuck: %s\n", yn ? "S" : "Uns", s);
      return;
   }
   /* channel-specific ban? */
   chan = findchan(dcc[idx].u.chat->con_chan);
   if (!chan) {
      dprintf(idx, "Invalid console channel.\n");
      return;
   }
   if (i)
     simple_sprintf(s, "%d", -i);
   j = u_setsticky_ban(chan, s, yn);
   if (j > 0) {
      putlog(LOG_CMDS, "*", "#%s# %sstick %s", dcc[idx].nick,
	     yn ? "" : "un", s);
      dprintf(idx, "%stuck: %s\n", yn ? "S" : "Uns", s);
      return;
   }
   dprintf(idx, "No such ban.\n");
}

static void cmd_stick (struct userrec * u, int idx, char * par)
{
   cmd_stick_yn(idx, par, 1);
}

static void cmd_unstick (struct userrec * u, int idx, char * par)
{
   cmd_stick_yn(idx, par, 0);
}

static void cmd_pls_chrec (struct userrec * u, int idx, char * par)
{
   char *nick, *chn;
   struct chanset_t *chan;
   struct userrec *u1;
   struct chanuserrec *chanrec;
   
   context;
   if (!par[0]) {
      dprintf(idx, "Usage: +chrec <User> [channel]\n");
      return;
   }
   nick = newsplit(&par);
   u1 = get_user_by_handle(userlist, nick);
   if (!u1) {
      dprintf(idx, "No such user.\n");
      return;
   }
   if (!par[0])
      chan = findchan(dcc[idx].u.chat->con_chan);
   else {
      chn = newsplit(&par);
      chan = findchan(chn);
   }
   if (!chan) {
      dprintf(idx, "No such channel.\n");
      return;
   }
   get_user_flagrec(u,&user,chan->name);
   if (!glob_master(user) && !chan_master(user)) {
      dprintf(idx, "You have no permission to do that on %s.\n",
	      chan->name);
      return;
   }
   chanrec = get_chanrec(u1, chan->name);
   if (chanrec) {
      dprintf(idx, "User %s already has a channel record for %s.\n", 
	      nick, chan->name);
      return;
   }
   putlog(LOG_CMDS, "*", "#%s# +chrec %s %s", dcc[idx].nick,
	  nick, chan->name);
   add_chanrec(u1,chan->name);
   dprintf(idx, "Added %s channel record for %s.\n", chan->name, nick);
}

static void cmd_mns_chrec (struct userrec * u, int idx, char * par)
{
   char *nick, *chn;
   struct chanset_t *chan;
   struct userrec *u1;
   struct chanuserrec *chanrec;
   
   context;
   if (!par[0]) {
      dprintf(idx, "Usage: -chrec <User> [channel]\n");
      return;
   }
   nick = newsplit(&par);
   u1 = get_user_by_handle(userlist, nick);
   if (!u1) {
      dprintf(idx, "No such user.\n");
      return;
   }
   if (!par[0])
      chan = findchan(dcc[idx].u.chat->con_chan);
   else {
      chn = newsplit(&par);
      chan = findchan(chn);
   }
   if (!chan) {
      dprintf(idx, "No such channel.\n");
      return;
   }
   get_user_flagrec(u,&user,chan->name);
   if (!glob_master(user) && !chan_master(user)) {
      dprintf(idx, "You have no permission to do that on %s.\n",
	      chan->name);
      return;
   }
   chanrec = get_chanrec(u1, chan->name);
   if (!chanrec) {
      dprintf(idx, "User %s doesn't have a channel record for %s.\n", 
	      nick, chan->name);
      return;
   }
   putlog(LOG_CMDS, "*", "#%s# -chrec %s %s", dcc[idx].nick, nick, chan->name);
   del_chanrec(u1, chan->name);
   dprintf(idx, "Removed %s channel record for %s.\n", chan->name, nick);
}

static void cmd_pls_chan (struct userrec * u, int idx, char * par)
{
   char *chname;
   
   if (!par[0]) {
      dprintf(idx, "Usage: +chan <#channel>\n");
      return;
   }
   chname = newsplit(& par);
   if (findchan(chname)) {
      dprintf(idx, "That channel already exists!\n");
      return;
   }
   if (tcl_channel_add(0, chname, "") == TCL_ERROR)
     dprintf(idx, "Invalid channel.\n");
   else
     putlog(LOG_CMDS, "*", "#%s# +chan %s", dcc[idx].nick, chname);
}

static void cmd_mns_chan (struct userrec * u, int idx, char * par)
{
   char *chname;
   struct chanset_t *chan;
   int i;
   
   if (!par[0]) {
      dprintf(idx, "Usage: -chan <#channel>\n");
      return;
   }
   chname = newsplit(&par);
   chan = findchan(chname);
   if (!chan) {
      dprintf(idx, "That channel doesnt exist!\n");
      return;
   }
   if (channel_static(chan)) {
      dprintf(idx, "Cannot remove %s, it is not a dynamic channel!.\n",
	      chname);
      return;
   }
   clear_channel(chan, 0);
   while (chan->bans)
     u_delban(chan,chan->bans->banmask,1);
   killchanset(chan);
   dprintf(DP_SERVER, "PART %s\n", chname);
   dprintf(idx, "Channel %s removed from the bot.\n", chname);
   dprintf(idx, "This includes any channel specific bans you set.\n");
   putlog(LOG_CMDS, "*", "#%s# -chan %s", dcc[idx].nick, chname);
   for (i = 0; i < dcc_total; i++)
     if ((dcc[i].type->flags & DCT_CHAT)
	 && !strcasecmp(dcc[i].u.chat->con_chan, chname)) {
	dprintf(i, "%s is no longer a valid channel, changing your console to '*'\n",
		chname);
	strcpy(dcc[i].u.chat->con_chan, "*");
     }
}

static void cmd_chaninfo (struct userrec * u, int idx, char * par) {
   char *chname, work[512];
   struct chanset_t *chan;
   
   if (!par[0]) {
      chname = dcc[idx].u.chat->con_chan;
      if (chname[0] == '*') {
	 dprintf(idx, "You console channel is invalid\n");
	 return;
      }
   } else {
      chname = newsplit(&par);
      get_user_flagrec(u,&user,chname);
      if (!glob_master(user) && !chan_master(user)) {
	 dprintf(idx,"You dont have access to %s.",chname);
	 return;
      }
   }
   if (!(chan = findchan(chname))) 
      dprintf(idx, "No such channel defined.\n");
   else {
      dprintf(idx, "Settings for %s channel %s\n", 
	      channel_static(chan) ? "static" : "dynamic", chname);
      get_mode_protect(chan, work);
      dprintf(idx, "Protect modes (chanmode): %s\n", work[0] ? work : "None");
      if (chan->idle_kick)
	dprintf(idx, "Idle Kick after (idle-kick): %d\n", chan->idle_kick);
      else
	dprintf(idx, "Idle Kick after (idle-kick): DONT!\n");
      /* only bot owners can see/change these (they're TCL commands) */
      if (u->flags & USER_OWNER) {
	 if (chan->need_op[0])
	   dprintf(idx, "To regain op's (need-op):\n%s\n", chan->need_op);
	 if (chan->need_invite[0])
	   dprintf(idx, "To get invite (need-invite):\n%s\n", chan->need_invite);
	 if (chan->need_key[0])
	   dprintf(idx, "To get key (need-key):\n%s\n", chan->need_key);
	 if (chan->need_unban[0])
	   dprintf(idx, "If Im banned (need-unban):\n%s\n", chan->need_unban);
	 if (chan->need_limit[0])
	   dprintf(idx, "When channel full (need-limit):\n%s\n", chan->need_limit);
      }
      dprintf(idx, "Other modes:\n");
      dprintf(idx, "     %cclearbans  %cenforcebans  %cdynamicbans  %cuserbans\n",
	      (chan->status & CHAN_CLEARBANS) ? '+' : '-',
	      (chan->status & CHAN_ENFORCEBANS) ? '+' : '-',
	      (chan->status & CHAN_DYNAMICBANS) ? '+' : '-',
	      (chan->status & CHAN_NOUSERBANS) ? '-' : '+');
      dprintf(idx, "     %cautoop     %cbitch        %cgreet        %cprotectops\n",
	      (chan->status & CHAN_OPONJOIN) ? '+' : '-',
	      (chan->status & CHAN_BITCH) ? '+' : '-',
	      (chan->status & CHAN_GREET) ? '+' : '-',
	      (chan->status & CHAN_PROTECTOPS) ? '+' : '-');
      dprintf(idx, "     %cstatuslog  %cstopnethack  %crevenge      %csecret\n",
	      (chan->status & CHAN_LOGSTATUS) ? '+' : '-',
	      (chan->status & CHAN_STOPNETHACK) ? '+' : '-',
	      (chan->status & CHAN_REVENGE) ? '+' : '-',
	      (chan->status & CHAN_SECRET) ? '+' : '-');
      dprintf(idx, "     %cshared     %cautovoice    %ccycle        %cseen\n", 
	      (chan->status & CHAN_SHARED) ? '+' : '-',
	      channel_autovoice(chan) ? '+' : '-',
	      channel_cycle(chan) ? '+' : '-',
              (chan->status & CHAN_SEEN) ? '+' : '-');
      dprintf(idx, "     %cdontkickops\n",
              (chan->status & CHAN_DONTKICKOPS) ? '+' : '-');
      dprintf(idx, "flood settings: chan ctcp join kick deop\n");
      dprintf(idx, "number:          %3d  %3d  %3d  %3d  %3d\n",
	      chan->flood_pub_thr, chan->flood_ctcp_thr, chan->flood_join_thr,
	      chan->flood_kick_thr, chan->flood_deop_thr);
      dprintf(idx, "time  :          %3d  %3d  %3d  %3d  %3d\n",
	      chan->flood_pub_time, chan->flood_ctcp_time, chan->flood_join_time,
	      chan->flood_kick_time, chan->flood_deop_time);
      putlog(LOG_CMDS, "*", "#%s# chaninfo %s", dcc[idx].nick, chname);
   }
}

static void cmd_chanset (struct userrec * u, int idx, char * par) {
   char *chname = NULL, answers[512];
   char *list[2];
   struct chanset_t *chan = NULL;
   
   if (!par[0]) 
     dprintf(idx, "Usage: chanset [#channel] <settings>\n");
   else {
      if ((par[0] == '&') || (par[0] == '#') || (par[0] == '+')) {
	 chname = newsplit(&par);
	 get_user_flagrec(u,&user,chname);
	 if (!glob_master(user) && !chan_master(user)) {
	    dprintf(idx,"You dont have access to %s.",chname);
	    return;
	 }  else if (!(chan = findchan(chname)) && (chname[0] != '+')) {
	    dprintf(idx, "That channel doesnt exist!\n");
	    return;
	 } 
	 if (!chan) {
	    if (par[0])
	      *--par = ' ';
	    par = chname;
	 }
      } 
      if (!chan && !(chan = findchan(chname = dcc[idx].u.chat->con_chan))) 
	dprintf(idx, "Invalid console channel.\n");
      else {
	 list[0] = newsplit(&par);
	 answers[0] = 0;
	 while (list[0][0]) {
	    if (list[0][0] == '+' || list[0][0] == '-' ||
		(strcmp(list[0], "dont-idle-kick") == 0)) {
	       if (strcmp(list[0]+1, "shared") == 0) {
		  dprintf(idx, "You can't change shared settings on the fly\n");
	       } else if (tcl_channel_modify(0, chan, 1, list) == TCL_OK) {
		  strcat(answers, list[0]);
		  strcat(answers, " ");
	       } else
		 dprintf(idx, "Error trying to set %s for %s, invalid mode\n",
			 list[0], chname);
	       list[0] = newsplit(&par);
	       continue;
	    }
	    /* the rest have an unknow amount of args, so assume the rest of the *
	     * line is args. Woops nearly made a nasty little hole here :) we'll *
	     * just ignore any non global +n's trying to set the need-commands   */
	    if (strncmp(list[0], "need-", 5) || (u->flags & USER_OWNER)) {
	       list[1] = par;
	       if (tcl_channel_modify(0, chan, 2, list) == TCL_OK) {
		  strcat(answers, list[0]);
		  strcat(answers, " { ");
		  strcat(answers, par);
		  strcat(answers, " }");
	       } else
		 dprintf(idx, "Error trying to set %s for %s, invalid option\n",
			 list[0], chname);
	    }
	    break;
	 }
	 if (answers[0]) {
	    dprintf(idx, "Successfully set modes { %s } on %s.\n",
		    answers, chname);
	    putlog(LOG_CMDS, "*", "#%s# chanset %s %s", dcc[idx].nick, chname,
		   answers);
	 }
      }
   }
}

static void cmd_chansave (struct userrec * u, int idx, char * par) {
   if (!chanfile[0])
      dprintf(idx, "No channel saving file defined.\n");
   else {
      dprintf(idx, "Saving all dynamic channel settings.\n");
      putlog(LOG_MISC, "*", "#%s# chansave", dcc[idx].nick);
      write_channels();
   }
}

static void cmd_chanload (struct userrec * u, int idx, char * par) {
   if (!chanfile[0])
     dprintf(idx, "No channel saving file defined.\n");
   else {
      dprintf(idx, "Reloading all dynamic channel settings.\n");
      putlog(LOG_MISC, "*", "#%s# chanload", dcc[idx].nick);
      setstatic = 0;
      read_channels(1);
   }
}

/* DCC CHAT COMMANDS */
/* function call should be:
   int cmd_whatever(idx,"parameters");
   as with msg commands, function is responsible for any logging
*/
static cmd_t C_dcc_irc[15]={
  { "+ban", "o|o", (Function)cmd_pls_ban, NULL },
  { "+chan", "n", (Function)cmd_pls_chan, NULL },
  { "+chrec", "m|m", (Function)cmd_pls_chrec, NULL },
  { "-ban", "o|o", (Function)cmd_mns_ban, NULL },
  { "-chan", "n", (Function)cmd_mns_chan, NULL },
  { "-chrec", "m|m", (Function)cmd_mns_chrec, NULL },
  { "bans", "o|o", (Function)cmd_bans, NULL },
  { "chaninfo", "m|m", (Function)cmd_chaninfo, NULL },
  { "chanload", "n|n", (Function)cmd_chanload, NULL },
  { "chanset", "n|n", (Function)cmd_chanset, NULL },
  { "chansave", "n|n", (Function)cmd_chansave, NULL },
  { "chinfo", "m|m", (Function)cmd_chinfo, NULL },
  { "info", "", (Function)cmd_info, NULL },
  { "stick", "o|o", (Function)cmd_stick, NULL },
  { "unstick", "o|o", (Function)cmd_unstick, NULL },
};
