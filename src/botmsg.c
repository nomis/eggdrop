/*
 * botmsg.c - formatting of messages to be sent on the botnet ...
 * by beldin<beldin@light.iinet.net.au>
 * 
 * it allows us to send different messages to different versioned bots
 */
/*
   This file is part of the eggdrop source code
   copyright (c) 1997 Robey Pointer
   and is distributed according to the GNU general public license.
   For full details, read the top of 'main.c' or the file called
   COPYING that was distributed with this code.
 */

#include "main.h"
#include "tandem.h"
#include <varargs.h>

extern struct dcc_t * dcc;
extern int dcc_total,tands;
extern char botnetnick[];
extern party_t * party;
extern Tcl_Interp * interp;
extern struct userrec * userlist;

static char OBUF[1024];

#ifndef NO_OLD_BOTNET
/* ditto for tandem bots */
void tandout_but(va_alist)
va_dcl
{
   int i, x, l;
   va_list va;
   char *format;
   char s[601];
   va_start(va);
   x = va_arg(va, int);
   format = va_arg(va, char *);
#ifdef HAVE_VSNPRINTF
   if ((l = vsnprintf(s, 511, format, va)) < 0) 
     s[l = 511] = 0;
#else
   l = vsprintf(s, format, va);
#endif
   va_end(va);
   for (i = 0; i < dcc_total; i++) 
      if ((dcc[i].type == &DCC_BOT) && (i != x)
	  && (b_numver(i) < NEAT_BOTNET)) {
	 tputs(dcc[i].sock, s, l);
      }
}
#endif

/* thank you ircu :) */
static char tobase64array[64] = {
     'A','B','C','D','E','F','G','H','I','J','K','L','M',
       'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
       'a','b','c','d','e','f','g','h','i','j','k','l','m',
       'n','o','p','q','r','s','t','u','v','w','x','y','z',
       '0','1','2','3','4','5','6','7','8','9',
       '[',']'
};

char * int_to_base64 (unsigned int val) {
   static char buf_base64[12];
   int i = 11;
   
   buf_base64[11] = 0;
   if (!val) {
      buf_base64[10] = 'A';
      return buf_base64+10;
   }
   while (val) {
      i--;
      buf_base64[i] = tobase64array[val & 0x3f];
      val = val >> 6;
   }
   return buf_base64+i;
}

char * int_to_base10 (unsigned int val) {
   static char buf_base10[16];
   int i = 15;
   
   buf_base10[15] = 0;
   if (!val) {
      buf_base10[14] = '0';
      return buf_base10+14;
   }
   while (val) {
      i--;
      buf_base10[i] = '0' + (val % 10);
      val /= 10;
   }
   return buf_base10+i;
}

int simple_sprintf (va_alist)
va_dcl
{
   char *buf, *format, *s;
   int c = 0,i;
   
   va_list va;
   va_start(va);
   buf = va_arg(va, char *);
   format = va_arg(va, char *);
   while (*format && (c<1023)) {
      if (*format == '%') {
	 format++;
	 
	 switch (*format) {
	  case 's':
	    s = va_arg(va,char *);
	    break;
	  case 'd':
	    i = va_arg(va,unsigned int);
	    s = int_to_base10(i);
	    break;
	  case 'D':
	    i = va_arg(va,unsigned int);
	    s = int_to_base64(i);
	    break;
	  case '%':
	    buf[c++] = *format++;
	    continue;
	  case 'c':
	    buf[c++] = (char)va_arg(va,int);
	    format++;
	    continue;
	  default:
	    continue;
	 }
	 if (s)
	   while (*s)
	     buf[c++] = *s++;
	 format++;
      } else
	buf[c++] = *format++;
   }
   va_end(va);
   buf[c] = 0;
   return c;
}

/* ditto for tandem bots */
void send_tand_but(int x, char * buf, int len)
{
   int i;

   for (i = 0; i < dcc_total; i++) 
     if ((dcc[i].type == &DCC_BOT) && (i != x)
	 && (b_numver(i) >= NEAT_BOTNET)) {
       tputs(dcc[i].sock, buf, len);
     }
}

void botnet_send_bye () {
   if (tands > 0) {
      send_tand_but(-1,"bye\n",4);
#ifndef NO_OLD_BOTNET
      tandout_but(-1,"bye\n");
#endif
   }
}

void botnet_send_chan (int idx, char * botnick, char * user,
			int chan, char * data) {
   int i;
   
   if ((tands > 0) && (chan < GLOBAL_CHANS)) {
      if (user) {
	 i = simple_sprintf(OBUF,"c %s@%s %D %s\n",user,botnick,chan,data);
      } else {
	 i = simple_sprintf(OBUF,"c %s %D %s\n",botnick,chan,data);
      }
      send_tand_but(idx,OBUF,i);
#ifndef NO_OLD_BOTNET	    
      tandout_but(idx,"chan %s%s%s %d %s\n", user?user:"", 
		  user?"@":"",botnick, chan, data);
#endif
   }
}

void botnet_send_act (int idx, char * botnick, char * user,
			int chan, char * data) {
   int i;
   
   if ((tands > 0) && (chan < GLOBAL_CHANS)) {
       if (user) {
	 i = simple_sprintf(OBUF,"a %s@%s %D %s\n",user,botnick,chan,data);
      } else {
	 i = simple_sprintf(OBUF,"a %s %D %s\n",botnick,chan,data);
      }
      send_tand_but(idx,OBUF,i);
#ifndef NO_OLD_BOTNET	    
      tandout_but(idx,"actchan %s%s%s %d %s\n", user?user:"",
		  user?"@":"",botnick, chan, data);
#endif
   }
}

void botnet_send_chat (int idx, char * botnick, char * data) {
   int i;
   
   if (tands > 0) {
      i = simple_sprintf(OBUF,"ct %s %s\n",botnick,data);
      send_tand_but(idx,OBUF,i);
#ifndef NO_OLD_BOTNET
      tandout_but(idx,"chat %s %s\n",botnick,data);
#endif
   }
}

void botnet_send_ping (int idx) {
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET) 
     tputs(dcc[idx].sock,"ping\n",5);
   else
#endif
     tputs(dcc[idx].sock,"pi\n",3);
}

void botnet_send_pong (int idx) {
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET) 
     tputs(dcc[idx].sock,"pong\n",5);
   else
#endif
     tputs(dcc[idx].sock,"po\n",3);
}

void botnet_send_priv (va_alist)
va_dcl
{
   int idx, l;
   char * from, * to, * tobot, * format;
   char tbuf[1024];
   
   va_list va;
   va_start(va);
   idx = va_arg(va, int);
   from = va_arg(va, char *);
   to = va_arg(va, char *);
   tobot = va_arg(va, char *);
   format = va_arg(va, char *);
#ifdef HAVE_VSNPRINTF 
   if (vsnprintf(tbuf, 450, format,va) < 0)
     tbuf[450] = 0;
#else
   vsprintf(tbuf,format,va);
#endif
   va_end(va);
   if (tobot) {
#ifndef NO_OLD_BOTNET
      if (b_numver(idx) < NEAT_BOTNET) 
	l = simple_sprintf(OBUF,"priv %s %s@%s %s\n",from,to,tobot,tbuf);
      else
#endif
	l = simple_sprintf(OBUF,"p %s %s@%s %s\n",from,to,tobot,tbuf);
   } else {
#ifndef NO_OLD_BOTNET
      if (b_numver(idx) < NEAT_BOTNET) 
	l = simple_sprintf(OBUF,"priv %s %s %s\n",from,to,tbuf);
      else
#endif
	l = simple_sprintf(OBUF,"p %s %s %s\n",from,to,tbuf);
   }
   tputs(dcc[idx].sock,OBUF,l);
}

void botnet_send_who (int idx, char * from, char * to, int chan) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"who %s %s %d\n",from,to,chan);
   else
#endif
     l = simple_sprintf(OBUF,"w %s %s %D\n",from,to,chan);
   tputs(dcc[idx].sock,OBUF,l);
}

void botnet_send_infoq (int idx, char * par) {
   int i = simple_sprintf(OBUF,"i? %s\n",par);
   send_tand_but(idx,OBUF,i);
#ifndef NO_OLD_BOTNET
   tandout_but(idx,"info? %s\n", par);
#endif
}

void botnet_send_unlink (int idx, char * who, char * via,
 			 char * bot, char * reason) {
   int l;
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"unlink %s %s %s %s\n", who, via, bot, reason);
   else
#endif
     l = simple_sprintf(OBUF,"ul %s %s %s %s\n", who, via, bot, reason);
   tputs(dcc[idx].sock,OBUF,l);
}   

void botnet_send_link (int idx, char * who, char * via, char * bot) {
   int l;
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"link %s %s %s\n", who, via, bot);
   else
#endif
     l = simple_sprintf(OBUF,"l %s %s %s\n", who, via, bot);
   tputs(dcc[idx].sock,OBUF,l);
}   

void botnet_send_unlinked (int idx, char * bot, char * args) {
   int l;
   
   context;

   if (tands > 0) {
      l = simple_sprintf(OBUF,"un %s %s\n",bot,args?args:"");
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      if ((b_numver(idx) >= NEAT_BOTNET) && args && args[0])
	tandout_but(idx, "chat %s %s\n", lastbot(bot), args);
      tandout_but(idx, "unlinked %s\n", bot);
#endif
   }
}   

void botnet_send_nlinked (int idx, char * bot, char * next, char flag,
			  int vernum) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"n %s %s %c%D\n",bot,next,flag,vernum);
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      if (flag == '!') {
	 flag = '-';
	 tandout_but(idx, "chat %s %s %s\n", next, NET_LINKEDTO, bot);
      }
      tandout_but(idx, "nlinked %s %s %c%d\n", bot, next, flag, vernum);
#endif
   }
}   

void botnet_send_traced (int idx, char * bot, char * buf) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"traced %s %s\n",bot,buf);
   else
#endif
     l = simple_sprintf(OBUF,"td %s %s\n", bot, buf);
   tputs(dcc[idx].sock, OBUF, l);
}

void botnet_send_trace (int idx, char * to, char * from, char * buf) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"trace %s %s %s:%s\n", to, from, buf, botnetnick);
   else
#endif
     l = simple_sprintf(OBUF,"t %s %s %s:%s\n", to, from, buf, botnetnick);
   tputs(dcc[idx].sock, OBUF, l);
}

void botnet_send_update (int idx, tand_t * ptr) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"u %s %c%D\n", ptr->bot, ptr->share, ptr->ver);
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(idx,"update %s %c%d\n", ptr->bot, ptr->share, ptr->ver);
#endif
   }
}

void botnet_send_reject (int idx, char * fromp, char * frombot, char * top,
			 char * tobot, char * reason) {
   int l;
   char to[NOTENAMELEN+1], from[NOTENAMELEN+1];
   
   if (tobot) {
      simple_sprintf(to,"%s@%s",top,tobot);
      top = to;
   }
   if (frombot) {
      simple_sprintf(from,"%s@%s",fromp,frombot);
      fromp = from;
   }
   if (!reason)
     reason = "";
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"reject %s %s %s\n", fromp, top, reason);
   else
#endif
     l = simple_sprintf(OBUF,"r %s %s %s\n", fromp, top, reason);
   tputs(dcc[idx].sock, OBUF, l);
}
			 
void botnet_send_zapf (int idx, char * a, char * b, char * c) {
   int l;
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"zapf %s %s %s\n", a, b, c);
   else
#endif
     l = simple_sprintf(OBUF,"z %s %s %s\n",a,b,c);
   tputs(dcc[idx].sock,OBUF,l);
}

void botnet_send_zapf_broad (int idx, char * a, char * b, char * c) {
   int l;

   if (tands > 0) {
      l = simple_sprintf(OBUF,"zb %s %s%s%s\n",a,b?b:"",b?" ":"",c);
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(idx,"zapf-broad %s\n",OBUF+3);
#endif
   }
}

void botnet_send_motd (int idx, char * from, char * to) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"motd %s %s\n", from, to);
   else
#endif
     l = simple_sprintf(OBUF,"m %s %s\n", from, to);
   tputs(dcc[idx].sock, OBUF, l);
}

void botnet_send_filereject(int idx, char * path, char * from, char * reason) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"filereject %s %s %s\n", path,from,reason);
   else
#endif
     l = simple_sprintf(OBUF,"f! %s %s %s\n", path,from,reason);
   tputs(dcc[idx].sock, OBUF, l);
}
   
void botnet_send_filesend (int idx, char * path, char * from, char * data) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"filesend %s %s %s\n", path,from,data);
   else
#endif
     l = simple_sprintf(OBUF,"fs %s %s %s\n", path,from,data);
   tputs(dcc[idx].sock, OBUF, l);
}

void botnet_send_filereq (int idx, char * from, char * bot, char * path) {
   int l;
   
#ifndef NO_OLD_BOTNET
   if (b_numver(idx) < NEAT_BOTNET)
     l = simple_sprintf(OBUF,"filereq %s %s:%s\n", from,bot,path);
   else
#endif
     l = simple_sprintf(OBUF,"fr %s %s:%s\n", from,bot,path);
   tputs(dcc[idx].sock, OBUF, l);
}

void botnet_send_idle (int idx, char * bot, int sock, int idle, char * away) {
   int l;
   
   context;
   if (tands > 0) {
      l = simple_sprintf(OBUF,"i %s %D %D %s\n", bot, sock, idle, 
			 away ? away : "");
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      if (away && away[0]) 
	tandout_but(idx,"away %s %d %s\n", bot, sock, away);
      tandout_but(idx,"idle %s %d %d\n", bot, sock, idle); 
#endif
   }
}
   
void botnet_send_away (int idx, char * bot, int sock,
		       char * msg, int linking) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"aw %s%s %D %s\n", 
			 ((idx >=0) && linking) ? "!":"",
			 bot, sock, msg ? msg : "");
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      if (msg) {
	 
	 if (idx < 0) {
	    tandout_but(idx,"chat %s %s is now away: %s.\n",
			bot, dcc[linking].nick, msg);
	 } else if ((b_numver(idx) >= NEAT_BOTNET))  {
	    int partyidx = getparty(bot, sock);
	    if (partyidx >= 0)
	      tandout_but(idx,"chat %s %s %s: %s.\n",
			  bot, party[partyidx].nick, NET_AWAY, msg);
	 }
	 tandout_but(idx,"away %s %d %s\n", bot, sock, msg);
      } else {
	 if (idx < 0) {
	    tandout_but(idx,"chat %s %s %s.\n",
			bot, dcc[linking].nick, NET_UNAWAY);
	 } else if (b_numver(idx) >= NEAT_BOTNET) {
	    int partyidx = getparty(bot, sock);
	    if (partyidx >= 0)
	      tandout_but(idx,"chat %s %s %s.\n",
			  bot, party[partyidx].nick, NET_UNAWAY);
	 }
	 tandout_but(idx,"unaway %s %d\n", bot, sock);
      }
#endif
   }
}

void botnet_send_join_idx (int useridx, int oldchan) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"j %s %s %D %c%D %s\n", 
			 botnetnick, dcc[useridx].nick,
			 dcc[useridx].u.chat->channel, geticon(useridx),
			 dcc[useridx].sock, dcc[useridx].host);
      send_tand_but(-1,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(-1,"join %s %s %d %c%d %s\n", botnetnick, 
		  dcc[useridx].nick, dcc[useridx].u.chat->channel,
		  geticon(useridx), dcc[useridx].sock, dcc[useridx].host);
      tandout_but(-1,"chan %s %d %s %s %s.\n",
		  botnetnick, dcc[useridx].u.chat->channel,
		  dcc[useridx].nick, NET_JOINEDTHE,
		  dcc[useridx].u.chat->channel ? "channel" : "party line");
      if ((oldchan >= 0) && (oldchan < GLOBAL_CHANS)) {
	 tandout_but(-1,"chan %s %d %s %s %s.\n",
		     botnetnick, oldchan,
		     dcc[useridx].nick, NET_LEFTTHE,
		     oldchan ? "channel" : "party line");
      }
#endif
   }
}

void botnet_send_join_party (int idx, int linking, int useridx, int oldchan) {
   int l;
   
   context;
   if (tands > 0) {
      l = simple_sprintf(OBUF,"j %s%s %s %D %c%D %s\n", linking ? "!" : "",
			 party[useridx].bot, party[useridx].nick,
			 party[useridx].chan, party[useridx].flag,
			 party[useridx].sock, safe_str(party[useridx].from));
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(idx,"join %s %s %d %c%d %s\n", party[useridx].bot,
		  party[useridx].nick, party[useridx].chan,
		  party[useridx].flag, party[useridx].sock,
		  safe_str(party[useridx].from));
      if ((idx < 0) || (!linking && (b_numver(idx) >= NEAT_BOTNET))) {
	 tandout_but(idx,"chan %s %d %s %s %s.\n",
		  party[useridx].bot, party[useridx].chan,
		     party[useridx].nick, NET_JOINEDTHE,
		     party[useridx].chan ? "channel" : "party line");
      }
      if ((oldchan >= 0) && (oldchan < GLOBAL_CHANS) 
	  && ((idx < 0) || (b_numver(idx) >= NEAT_BOTNET))) {
	 tandout_but(idx,"chan %s %d %s %s %s.\n",
		     party[useridx].bot, oldchan, party[useridx].nick, 
		     NET_LEFTTHE,
		     party[useridx].chan ? "channel" : "party line");
      }
#endif
   }
}

void botnet_send_part_idx (int useridx,char * reason) {
   int l = simple_sprintf(OBUF,"pt %s %s %D %s\n",botnetnick,
		      dcc[useridx].nick,dcc[useridx].sock,
		      reason?reason:"");
   if (tands > 0) {
      send_tand_but(-1,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(-1,"part %s %s %d\n", botnetnick,
		  dcc[useridx].nick, dcc[useridx].sock);
      tandout_but(-1,"chan %s %d %s has left the %s%s%s.\n",
		  botnetnick, dcc[useridx].u.chat->channel,
		  dcc[useridx].nick, 
		  dcc[useridx].u.chat->channel ? "channel" : "party line",
		  reason?": ":"",reason?reason:"");
#endif
   }
}

void botnet_send_part_party (int idx, int partyidx, char * reason,
			     int silent) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"pt %s%s %s %D %s\n", 
			 silent ? "!" : "", party[partyidx].bot,
			 party[partyidx].nick, party[partyidx].sock,
			 reason?reason:"");
      send_tand_but(idx,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(idx,"part %s %s %d\n", party[partyidx].bot,
		  party[partyidx].nick, party[partyidx].sock);
      if (((idx < 0) || (b_numver(idx) >= NEAT_BOTNET)) && !silent) {
	 tandout_but(idx,"chan %s %d %s has left the %s%s%s.\n",
		     party[partyidx].bot, party[partyidx].chan,
		     party[partyidx].nick, 
		     party[partyidx].chan ? "channel" : "party line",
		     reason?": ":"",reason?reason:"");
      }
#endif
   }  
}

void botnet_send_nkch(int useridx, char * oldnick) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"nc %s %D %s\n", botnetnick,
			 dcc[useridx].sock, dcc[useridx].nick);
      send_tand_but(-1,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(-1,"part %s %s %d\n", botnetnick,
		  dcc[useridx].nick, dcc[useridx].sock);
      tandout_but(-1,"join %s %s %d %c%d %s\n", botnetnick, 
		  dcc[useridx].nick, dcc[useridx].u.chat->channel,
		  geticon(useridx), dcc[useridx].sock, dcc[useridx].host);
      tandout_but(-1,"chan %s %d %s: %s -> %s.\n",
		  botnetnick, dcc[useridx].u.chat->channel,
		  oldnick, NET_NICKCHANGE, dcc[useridx].nick);
#endif
   }
}

void botnet_send_nkch_part(int butidx, int useridx, char * oldnick) {
   int l;
   
   if (tands > 0) {
      l = simple_sprintf(OBUF,"nc %s %D %s\n", party[useridx].bot,
			 party[useridx].sock, party[useridx].nick);
      send_tand_but(butidx,OBUF,l);
#ifndef NO_OLD_BOTNET
      tandout_but(butidx,"part %s %s %d\n", party[useridx].bot,
		  party[useridx].nick, party[useridx].sock);
      tandout_but(butidx,"join %s %s %d %c%d %s\n", party[useridx].bot, 
		  party[useridx].nick, party[useridx].chan,
		  party[useridx].flag, party[useridx].sock, 
		  safe_str(party[useridx].from));
      tandout_but(butidx,"chan %s %d %s : %s -> %s.\n",
		  party[useridx].bot, party[useridx].chan,
		  NET_NICKCHANGE,
		  oldnick, party[useridx].nick);
#endif
   }
}

/* this part of add_note is more relevant to the botnet than
 * to the notes file */
int add_note (char * to, char * from, char * msg, int idx, int echo)
{
   int status, i, iaway, sock;
   char *p, botf[81], ss[81], ssf[81];
   struct userrec * u;
   
   if (strlen(msg) > 450)
      msg[450] = 0;		/* notes have a limit */
   /* note length + PRIVMSG header + nickname + date  must be <512  */
   p = strchr(to, '@');
   if (p != NULL) {		/* cross-bot note */
      char x[20];
      *p = 0;
      strcpy(x,to);
      *p = '@';
      p++;
      if (strcasecmp(p, botnetnick) == 0)	/* to me?? */
	return add_note(x, from, msg, idx, echo);	/* start over, dimwit. */
      if (strcasecmp(from, botnetnick)) {
	 if (strchr(from,'@')) {
	    strcpy(botf, from);
	 } else
	   sprintf(botf, "%s@%s", from, botnetnick);
      } else
	 strcpy(botf, botnetnick);
      i = nextbot(p);
      if (i < 0) {
	 if (idx >= 0)
	    dprintf(idx, BOT_NOTHERE);
	 return NOTE_ERROR;
      }
      if ((idx >= 0) && (echo))
	 dprintf(idx, "-> %s@%s: %s\n", x, p, msg);
      if (idx >= 0) {
	 sprintf(ssf,"%lu:%s",dcc[idx].sock,botf);
	 botnet_send_priv(i,ssf,x,p,"%s",msg);
      } else
	 botnet_send_priv(i,botf,x,p,"%s",msg);
      return NOTE_OK;		/* forwarded to the right bot */
   }
   /* might be form "sock:nick" */
   splitc(ssf, from, ':');
   rmspace(ssf);
   splitc(ss, to, ':');
   rmspace(ss);
   if (!ss[0])
      sock = (-1);
   else
      sock = atoi(ss);
   /* don't process if there's a note binding for it */
   if (idx != (-2)) {		/* notes from bots don't trigger it */
      if (check_tcl_note(from, to, msg)) {
	 if ((idx >= 0) && (echo))
	    dprintf(idx, "-> %s: %s\n", to, msg);
	 return NOTE_TCL;
      }
   }
   if (!(u = get_user_by_handle(userlist,to))) {
      if (idx >= 0)
	 dprintf(idx, USERF_UNKNOWN);
      return NOTE_ERROR;
   }
   if (is_bot(u)) {
      if (idx >= 0)
	 dprintf(idx, BOT_NONOTES);
      return NOTE_ERROR;
   }
   status = NOTE_STORED;
   iaway = 0;
   /* online right now? */
   for (i = 0; i < dcc_total; i++) {
      if ((dcc[i].type->flags & DCT_GETNOTES) &&
	  ((sock == (-1)) || (sock == dcc[i].sock)) &&
	  (strcasecmp(dcc[i].nick, to) == 0)) {
	 int aok = 1;
	 if (dcc[i].type == &DCC_CHAT)
	   if ((dcc[i].u.chat->away != NULL) &&
	       (idx != (-2))) {
	      /* only check away if it's not from a bot */
	      aok = 0;
	      if (idx >= 0)
		dprintf(idx, "%s %s: %s\n", dcc[i].nick, BOT_USERAWAY,
			dcc[i].u.chat->away);
	      if (!iaway)
		iaway = i;
	      status = NOTE_AWAY;
	   }
	 if (aok) {
	    char * p, *fr = from;
	    int l = 0;
	    char work[1024];
	    
	    while ((*msg == '<') || (*msg == '>')) {
	       p = newsplit(&msg);
	       if (*p == '<')
		 l += simple_sprintf(work+l, "via %s, ", p + 1);
	       else if (*from == '@')
		 fr = p + 1;
	    }  
	    if ((idx == (-2)) || (strcasecmp(from, botnetnick) == 0))
	      dprintf(i, "*** [%s] %s%s\n", fr, l ? work : "", msg);
	    else
	      dprintf(i, "%cNote [%s]: %s%s\n", 7, fr, l ? work : "", msg);
	    if ((idx >= 0) && (echo))
	       dprintf(idx, "-> %s: %s\n", to, msg);
	    return NOTE_OK;
	 }
      }
   }
   if (idx == (-2))
      return NOTE_OK;		/* error msg from a tandembot: don't store */
/* call store note here */
   Tcl_SetVar(interp,"_from", from, 0);
   Tcl_SetVar(interp,"_to", to, 0);
   Tcl_SetVar(interp,"_data", msg, 0);
   simple_sprintf(ss,"%d",dcc[idx].sock);
   Tcl_SetVar(interp,"_idx",ss,0);
   set_tcl_vars();
   if (Tcl_VarEval(interp,"storenote"," $_from $_to $_data $_idx",NULL) == TCL_OK) {
      if (interp->result && interp->result[0]) {
	 strncpy(to, interp->result,120);
	 to[120] = 0;
	 status = NOTE_FWD;
      }
      if (status == NOTE_AWAY) {
	 /* user is away in all sessions -- just notify the user that a */
	 /* message arrived and was stored.  (only oldest session is notified.) */
	 dprintf(iaway, "*** %s.\n", BOT_NOTEARRIVED);
      }
      return status;
   }
   return NOTE_ERROR;
}
