/*
      Conversion definitions for language support
*/

#ifndef _H_LANG
#define _H_LANG

#define USAGE            get_language(0x001)
#define FAILED           get_language(0x002)

/* file area */
#define FILES_CONVERT    get_language(0x300)
#define FILES_NOUPDATE   get_language(0x301)
#define FILES_NOCONVERT  get_language(0x302)
#define FILES_LSHEAD1    get_language(0x303)
#define FILES_LSHEAD2    get_language(0x304)
#define FILES_NOFILES    get_language(0x305)
#define FILES_NOMATCH    get_language(0x306)
#define FILES_DIRDNE     get_language(0x307)
#define FILES_FILEDNE    get_language(0x308)
#define FILES_NOSHARE    get_language(0x309)
#define FILES_REMOTE     get_language(0x30a)
#define FILES_SENDERR    get_language(0x30b)
#define FILES_SENDING    get_language(0x30c)
#define FILES_REMOTEREQ  get_language(0x30d)
#define FILES_BROKEN     get_language(0x30e)
#define FILES_INVPATH    get_language(0x30f)
#define FILES_CURDIR     get_language(0x310)
#define FILES_NEWCURDIR  get_language(0x311)
#define FILES_NOSUCHDIR  get_language(0x312)
#define FILES_ILLDIR     get_language(0x313)
#define FILES_BADNICK    get_language(0x314)
#define FILES_NOTAVAIL   get_language(0x315)
#define FILES_REQUESTED  get_language(0x316)
#define FILES_NORMAL     get_language(0x317)
#define FILES_CHGLINK    get_language(0x318)
#define FILES_NOTOWNER   get_language(0x319)
#define FILES_CREADIR    get_language(0x31a)
#define FILES_REQACCESS  get_language(0x31b)
#define FILES_CHGACCESS  get_language(0x31c)
#define FILES_CHGNACCESS get_language(0x31d)
#define FILES_REMDIR     get_language(0x31e)
#define FILES_ILLSOURCE  get_language(0x31f)
#define FILES_ILLDEST    get_language(0x320)
#define FILES_STUPID     get_language(0x321)
#define FILES_EXISTDIR   get_language(0x322)
#define FILES_SKIPSTUPID get_language(0x323)
#define FILES_DEST       get_language(0x324)
#define FILES_COPY       get_language(0x325)
#define FILES_COPIED     get_language(0x326)
#define FILES_MOVE       get_language(0x327)
#define FILES_MOVED      get_language(0x328)
#define FILES_CANTWRITE  get_language(0x329)
#define FILES_REQUIRES   get_language(0x32a)
#define FILES_HID        get_language(0x32b)
#define FILES_UNHID      get_language(0x32c)
#define FILES_SHARED     get_language(0x32d)
#define FILES_UNSHARED   get_language(0x32e)
#define FILES_ADDLINK    get_language(0x32f)
#define FILES_CHANGED    get_language(0x330)
#define FILES_BLANKED    get_language(0x331)
#define FILES_ERASED     get_language(0x332)
#define FILES_WELCOME    get_language(0x33a)
#define FILES_WELCOME1   get_language(0x33b)


/* Userfile messages */
#define USERF_XFERDONE	get_language(0x400)
#define USERF_BADREREAD	get_language(0x401)
#define USERF_CANTREAD	get_language(0x402)
#define USERF_CANTSEND	get_language(0x403)
#define USERF_NOMATCH	get_language(0x404)
#define USERF_OLDFMT	get_language(0x405)
#define USERF_INVALID	get_language(0x406)
#define USERF_CORRUPT	get_language(0x407)
#define USERF_DUPE	get_language(0x408)
#define USERF_BROKEPASS	get_language(0x409)
#define USERF_IGNBANS	get_language(0x40a)
#define USERF_WRITING	get_language(0x40b)
#define USERF_ERRWRITE	get_language(0x40c)
#define USERF_ERRWRITE2	get_language(0x40d)
#define USERF_NONEEDNEW	get_language(0x40e)
#define USERF_REHASHING	get_language(0x40f)
#define USERF_UNKNOWN	get_language(0x410)
#define USERF_NOUSERREC	get_language(0x411)
#define USERF_BACKUP	get_language(0x412)
#define USERF_FAILEDXFER get_language(0x413)
#define USERF_OLDSHARE	get_language(0x414)
#define USERF_ANTIQUESHARE get_language(0x415)
#define USERF_REJECTED	get_language(0x416)

/* Misc messages */
#define MISC_EXPIRED	get_language(0x500)
#define MISC_TOTAL	get_language(0x501)
#define MISC_ERASED	get_language(0x502)
#define MISC_LEFT	get_language(0x503)
#define MISC_ONLOCALE	get_language(0x504)
#define MISC_MATCHING	get_language(0x505)
#define MISC_SKIPPING 	get_language(0x506)
#define MISC_TRUNCATED	get_language(0x507)
#define MISC_FOUNDMATCH	get_language(0x508)
#define MISC_AMBIGUOUS	get_language(0x509)
#define MISC_NOSUCHCMD	get_language(0x50a)
#define MISC_CMDBINDS	get_language(0x50b)
#define MISC_RESTARTING	get_language(0x50c)
#define MISC_NOMODULES	get_language(0x50d)
#define MISC_LOGSWITCH	get_language(0x50e)
#define MISC_OWNER	get_language(0x50f)
#define MISC_MASTER	get_language(0x510)
#define MISC_OP		get_language(0x511)
#define MISC_IDLE	get_language(0x512)
#define MISC_AWAY	get_language(0x513)
#define MISC_IGNORING	get_language(0x514)
#define MISC_UNLINKED	get_language(0x515)
#define MISC_DISCONNECTED get_language(0x516)
#define MISC_INVALIDBOT get_language(0x517)
#define MISC_LOOP	get_language(0x518)
#define MISC_MUTUAL	get_language(0x519)
#define MISC_FROM	get_language(0x51a)
#define MISC_OUTDATED   get_language(0x51b)
#define MISC_REJECTED	get_language(0x51c)
#define MISC_IMPOSTER	get_language(0x51d)
#define MISC_TRYING	get_language(0x51e)
#define MISC_MOTDFILE	get_language(0x51f)
#define MISC_NOMOTDFILE get_language(0x520)
#define MISC_USEFORMAT	get_language(0x521)
#define MISC_CHADDRFORMAT get_language(0x522)
#define MISC_UNKNOWN	get_language(0x523)
#define MISC_CHANNELS	get_language(0x524)
#define MISC_TRYINGMISTAKE	get_language(0x525)
#define MISC_PENDING	get_language(0x526)
#define MISC_WANTOPS	get_language(0x527)
#define MISC_LURKING	get_language(0x528)
#define MISC_BACKGROUND	get_language(0x529)
#define MISC_TERMMODE	get_language(0x52a)
#define MISC_STATMODE	get_language(0x52b)
#define MISC_LOGMODE	get_language(0x52c)
#define MISC_ONLINEFOR	get_language(0x52d)
#define MISC_CACHEHIT	get_language(0x52e)
#define MISC_TCLLIBVER	get_language(0x52f)
#define MISC_NEWUSERFLAGS get_language(0x530)
#define MISC_NOTIFY	get_language(0x531)
#define MISC_PERMOWNER	get_language(0x532)
#define MISC_ROOTWARN	get_language(0x533)
#define MISC_NOCONFIGFILE get_language(0x534)
#define MISC_NOUSERFILE	get_language(0x535)
#define MISC_NOUSERFILE2 get_language(0x536)
#define MISC_USERFCREATE1 get_language(0x537)
#define MISC_USERFCREATE2 get_language(0x538)
#define MISC_USERFEXISTS get_language(0x539)
#define MISC_CANTWRITETEMP get_language(0x53a)
#define MISC_CANTRELOADUSER get_language(0x53b)
#define MISC_MISSINGUSERF get_language(0x53c)
#define MISC_BOTSCONNECTED get_language(0x53d)
#define MISC_BANNER get_language(0x53e)

/* IRC */
#define IRC_BANNED	get_language(0x600)
#define IRC_YOUREBANNED	get_language(0x601)
/* BOT log messages when attempting to place a ban which matches me */
#define IRC_IBANNEDME	get_language(0x602)
#define IRC_FUNKICK	get_language(0x603)
#define IRC_HI		get_language(0x604)
#define IRC_GOODBYE	get_language(0x605)
#define IRC_BANNED2	get_language(0x606)
#define IRC_NICKTOOLONG get_language(0x607)
#define IRC_INTRODUCED	get_language(0x608)
#define IRC_COMMONSITE	get_language(0x609)
#define IRC_SALUT1	get_language(0x60a)
#define IRC_SALUT1_ARGS	nick, nick, botname
#define IRC_SALUT2	get_language(0x60b)
#define IRC_SALUT2_ARGS	nick, host
#define IRC_SALUT2A	get_language(0x60c)
#define IRC_SALUT2B	get_language(0x60d)
#define IRC_INITOWNER1	get_language(0x60e)
#define IRC_INIT1	get_language(0x60f)
#define IRC_INIT1_ARGS	nick
#define IRC_INITNOTE	get_language(0x610)
#define IRC_INITINTRO	get_language(0x611)
#define IRC_PASS	get_language(0x612)
#define IRC_NOPASS	get_language(0x613)
#define IRC_NOPASS2	get_language(0x614)
#define IRC_EXISTPASS	get_language(0x615)
#define IRC_PASSFORMAT	get_language(0x616)
#define IRC_SETPASS	get_language(0x617)
#define IRC_FAILPASS	get_language(0x618)
#define IRC_CHANGEPASS	get_language(0x619)
#define IRC_FAILCOMMON	get_language(0x61a)
#define IRC_MISIDENT	get_language(0x61b)
#define IRC_MISIDENT_ARGS	nick, nick, u->handle
#define IRC_MISIDENT_ARGS2	nick, who, u->handle
#define IRC_DENYACCESS	get_language(0x61c)
#define IRC_RECOGNIZED	get_language(0x61d)
#define IRC_ADDHOSTMASK	get_language(0x61e)
#define IRC_DELMAILADDR	get_language(0x61f)
#define IRC_FIELDCURRENT	get_language(0x620)
#define IRC_FIELDCHANGED	get_language(0x621)
#define IRC_FIELDTOREMOVE	get_language(0x622)
#define IRC_NOEMAIL	get_language(0x623)
#define IRC_INFOLOCKED	get_language(0x624)
#define IRC_REMINFOON	get_language(0x625)
#define IRC_REMINFO	get_language(0x626)
#define IRC_NOINFOON	get_language(0x627)
#define IRC_NOINFO	get_language(0x628)
#define IRC_NOMONITOR	get_language(0x629)
#define IRC_RESETCHAN	get_language(0x62a)
#define IRC_JUMP	get_language(0x62b)
#define IRC_CHANHIDDEN	get_language(0x62c)
#define IRC_ONCHANNOW	get_language(0x62d)
#define IRC_NEVERJOINED	get_language(0x62e)
#define IRC_LASTSEENAT	get_language(0x62f)
#define IRC_DONTKNOWYOU	get_language(0x630)
#define IRC_NOHELP	get_language(0x631)
#define IRC_NOHELP2	get_language(0x632)
#define IRC_NOTNORMFILE	get_language(0x633)
#define IRC_NOTONCHAN	get_language(0x634)
#define IRC_GETORIGNICK	get_language(0x635)
#define IRC_BADBOTNICK	get_language(0x636)
#define IRC_BOTNICKINUSE	get_language(0x637)
#define IRC_CANTCHANGENICK	get_language(0x638)
#define IRC_BOTNICKJUPED	get_language(0x639)
#define IRC_CHANNELJUPED	get_language(0x63a)
#define IRC_NOTREGISTERED1	get_language(0x63b)
#define	IRC_NOTREGISTERED2	get_language(0x63c)
#define IRC_FLOODIGNORE1	get_language(0x63d)
#define IRC_FLOODIGNORE2	get_language(0x63e)
#define IRC_FLOODIGNORE3	get_language(0x63f)
#define IRC_FLOODKICK		get_language(0x640)
#define IRC_SERVERTRY		get_language(0x641)
#define IRC_DNSFAILED		get_language(0x642)
#define IRC_FAILEDCONNECT	get_language(0x643)
#define IRC_SERVERSTONED	get_language(0x644)
#define IRC_DISCONNECTED	get_language(0x645)
#define IRC_NOSERVER		get_language(0x646)
#define IRC_MODEQUEUE		get_language(0x647)
#define IRC_SERVERQUEUE		get_language(0x648)
#define IRC_HELPQUEUE		get_language(0x649)
#define IRC_BOTNOTONIRC		get_language(0x64a)
#define IRC_NOTACTIVECHAN	get_language(0x64b)
#define IRC_PROCESSINGCHAN	get_language(0x64c)
#define IRC_CHANNEL		get_language(0x64d)
#define IRC_DESIRINGCHAN	get_language(0x64e)
#define IRC_CHANNELTOPIC	get_language(0x64f)
#define IRC_PENDINGOP		get_language(0x650)
#define IRC_PENDINGDEOP		get_language(0x651)
#define IRC_PENDINGKICK		get_language(0x652)
#define IRC_FAKECHANOP		get_language(0x653)
#define IRC_ENDCHANINFO		get_language(0x654)
#define IRC_MASSKICK		get_language(0x655)
#define IRC_REMOVEDBAN		get_language(0x656)
#define IRC_UNEXPECTEDMODE	get_language(0x657)
#define IRC_POLITEKICK		get_language(0x658)
#define IRC_AUTOJUMP		get_language(0x659)
#define IRC_CHANGINGSERV	get_language(0x65a)
#define IRC_TOOMANYCHANS	get_language(0x65b)
#define IRC_CHANFULL		get_language(0x65c)
#define IRC_CHANINVITEONLY	get_language(0x65d)
#define IRC_BANNEDFROMCHAN	get_language(0x65e)
#define IRC_SERVNOTONCHAN	get_language(0x65f)
#define IRC_BADCHANKEY		get_language(0x660)
#define IRC_INTRO1              get_language(0x661)
#define IRC_INTRO1_ARGS nick, botname
#define IRC_BADHOST1            get_language(0x662)
#define IRC_BADHOST1_ARGS nick
#define IRC_BADHOST2            get_language(0x663)     
#define IRC_BADHOST2_ARGS nick, botname
#define IRC_NEWBOT1             get_language(0x664)
#define IRC_NEWBOT1_ARGS nick, botname
#define IRC_NEWBOT2             get_language(0x665)
#define IRC_NEWBOT2_ARGS nick
#define IRC_TELNET              get_language(0x666)
#define IRC_TELNET_ARGS botnetnick
#define IRC_TELNET1             get_language(0x667)
#define IRC_LIMBO               get_language(0x668)
#define IRC_TELNETFLOOD         get_language(0x669)
#define IRC_PREBANNED           get_language(0x66a)
#define IRC_LEMMINGBOT          get_language(0x66b)
#define IRC_PROTECT             get_language(0x66c)		
#define IRC_COMMENTKICK         get_language(0x66d)

/* Eggdrop command line usage */

#define EGG_USAGE	get_language(0x700)
#define EGG_RUNNING1	get_language(0x701)
#define EGG_RUNNING2	get_language(0x702)

#define USER_ISGLOBALOP	get_language(0x800)
#define USER_ISBOT	get_language(0x801)
#define USER_ISMASTER	get_language(0x802)

/* Messages used when listing with `.bans' */
#define BANS_CREATED	get_language(0x100)
#define BANS_LASTUSED	get_language(0x101)
#define BANS_INACTIVE	get_language(0x102)
#define BANS_PLACEDBY	get_language(0x103)
#define BANS_GLOBAL	get_language(0x104)
#define BANS_NOTACTIVE	get_language(0x105)
#define BANS_BYCHANNEL	get_language(0x106)
#define BANS_NOTACTIVE2	get_language(0x107)
#define BANS_NOTBYBOT	get_language(0x108)
#define BANS_USEBANSALL	get_language(0x109)
#define BANS_NOLONGER	get_language(0x10a)

/* Messages referring to channels */
#define CHAN_NOSUCH	get_language(0x900)
#define CHAN_BADCHANKEY	get_language(0x901)
#define CHAN_BADCHANMODE	get_language(0x902)
#define CHAN_BADCHANMODE_ARGS	chan->name, who
#define CHAN_BADCHANMODE_ARGS2	chan->name, op
#define CHAN_MASSDEOP	get_language(0x903)
#define CHAN_MASSDEOP_ARGS	chan->name, from
#define CHAN_MASSDEOP_KICK	get_language(0x904)
#define CHAN_BADBAN	get_language(0x905)
#define CHAN_PERMBANNED	get_language(0x906)
#define CHAN_FORCEJOIN	get_language(0x907)
#define CHAN_FAKEMODE	get_language(0x908)
#define CHAN_FAKEMODE_KICK	get_language(0x909)
#define CHAN_DESYNCMODE	get_language(0x90a)
#define CHAN_DESYNCMODE_KICK	get_language(0x90b)
#define CHAN_FLOOD get_language(0x90c)
#define CHAN_BOGUSUSERNAME get_language(0x90d)
#define CHAN_BOGUSBAN get_language(0x90e)

/* Messages referring to ignores */
#define IGN_NONE	get_language(0xa00)
#define IGN_CURRENT	get_language(0xa01)
#define IGN_NOLONGER	get_language(0xa02)

/* Messages referring to bots */
#define BOT_NOTHERE	get_language(0xb00)
#define BOT_NONOTES	get_language(0xb01)
#define BOT_USERAWAY	get_language(0xb02)
#define BOT_NOTEUNSUPP	get_language(0xb03)
#define BOT_NOTES2MANY	get_language(0xb04)
#define BOT_NOTESERROR1	get_language(0xb05)
#define BOT_NOTESERROR2	get_language(0xb06)
#define BOT_NOTEARRIVED	get_language(0xb07)
#define BOT_NOTESTORED	get_language(0xb08)
#define BOT_NOTEDELIV	get_language(0xb09)
#define BOT_NOTEOUTSIDE	get_language(0xb0a)
#define BOT_NOTESUSAGE	get_language(0xb0b)
#define BOT_NOMESSAGES	get_language(0xb0c)
#define BOT_NOTEEXP1	get_language(0xb0d)
#define BOT_NOTEEXP2	get_language(0xb0e)
#define BOT_NOTEWAIT	get_language(0xb0f)
#define BOT_NOTTHATMANY	get_language(0xb10)
#define BOT_NOTEUSAGE	get_language(0xb11)
#define BOT_CANTMODNOTE	get_language(0xb12)
#define BOT_NOTESERASED	get_language(0xb13)
#define BOT_NOTESWAIT1	get_language(0xb14)
#define BOT_NOTESWAIT1_ARGS	m->nick, k, k == 1 ? "" : "s", origbotname
#define BOT_NOTESWAIT1_ARGS2	nick, k, k == 1 ? "" : "s", origbotname
#define BOT_NOTESWAIT2	get_language(0xb15)
#define BOT_NOTESWAIT3	get_language(0xb16)
#define BOT_NOTESWAIT3_ARGS k, k == 1 ? "" : "s"
#define BOT_NOTESWAIT4	get_language(0xb17)
#define BOT_MSGDIE	get_language(0xb18)
#define BOT_NOSUCHUSER  get_language(0xb19)
#define BOT_NOCHANNELS  get_language(0xb1a)
#define BOT_PARTYMEMBS  get_language(0xb1b)
#define BOT_BOTSCONNECTED get_language(0xb1c)
#define BOT_OTHERPEOPLE	get_language(0xb1d)
#define BOT_OUTDATEDWHOM get_language(0xb1e)
#define BOT_LINKATTEMPT get_language(0xb1f)
#define BOT_NOTESTORED2	get_language(0xb20)
#define BOT_NOTEBOXFULL get_language(0xb21)
#define BOT_NOTEISAWAY  get_language(0xb22)
#define BOT_NOTESENTTO  get_language(0xb23)
#define BOT_DISCONNECTED get_language(0xb24)
#define BOT_PEOPLEONCHAN get_language(0xb25)
#define BOT_CANTLINKTHERE get_language(0xb26)
#define BOT_CANTUNLINK	get_language(0xb27)
#define BOT_LOOPDETECT get_language(0xb28)
#define BOT_BOGUSLINK	get_language(0xb29)
#define BOT_BOGUSLINK2	get_language(0xb2a)
#define BOT_DISCONNLEAF	get_language(0xb2b)
#define BOT_LINKEDTO	get_language(0xb2c)
#define BOT_ILLEGALLINK get_language(0xb2d)
#define BOT_YOUREALEAF  get_language(0xb2e)
#define BOT_REJECTING	get_language(0xb2f)
#define BOT_OLDBOT	get_language(0xb30)
#define BOT_TRACERESULT	get_language(0xb31)
#define BOT_DOESNTEXIST	get_language(0xb32)
#define BOT_NOREMOTEBOOT get_language(0xb33)
#define BOT_NOOWNERBOOT	get_language(0xb34)
#define BOT_XFERREJECTED get_language(0xb35)
#define BOT_NOFILESYS	get_language(0xb36)
#define BOT_BOTNETUSERS	get_language(0xb37)
#define BOT_PARTYLINE	get_language(0xb38)
#define BOT_LOCALCHAN	get_language(0xb39)
#define BOT_USERSONCHAN	get_language(0xb3a)
#define BOT_NOBOTSLINKED get_language(0xb3b)
#define BOT_NOTRACEINFO	get_language(0xb3c)
#define BOT_COMPLEXTREE	get_language(0xb3d)
#define BOT_UNLINKALL	get_language(0xb3e)
#define BOT_KILLLINKATTEMPT get_language(0xb3f)
#define BOT_ENDLINKATTEMPT get_language(0xb40)
#define BOT_BREAKLINK	get_language(0xb41)
#define BOT_UNLINKEDFROM get_language(0xb42)
#define BOT_NOTCONNECTED get_language(0xb43)
#define BOT_WIPEBOTTABLE get_language(0xb44)
#define BOT_BOTUNKNOWN	get_language(0xb45)
#define BOT_CANTLINKMYSELF get_language(0xb46)
#define BOT_ALREADYLINKED get_language(0xb47)
#define BOT_NOTELNETADDY get_language(0xb48)
#define BOT_LINKING	get_language(0xb49)
#define BOT_CANTFINDRELAYUSER get_language(0xb4a)
#define BOT_CANTLINKTO	get_language(0xb4b)
#define BOT_CANTRELAYMYSELF get_language(0xb4c)
#define BOT_CONNECTINGTO get_language(0xb4d)
#define BOT_BYEINFO1	get_language(0xb4e)
#define BOT_ABORTRELAY1 get_language(0xb4f)
#define BOT_ABORTRELAY2 get_language(0xb50)
#define BOT_ABORTRELAY3 get_language(0xb51)
#define BOT_PARTYJOINED	get_language(0xb52)
#define BOT_LOSTDCCUSER	get_language(0xb53)
#define BOT_DROPPINGRELAY get_language(0xb54)
#define BOT_RELAYSUCCESS get_language(0xb55)
#define BOT_BYEINFO2	get_language(0xb56)
#define BOT_RELAYLINK	get_language(0xb57)
#define BOT_PARTYLEFT	get_language(0xb58)
#define BOT_ENDRELAY1	get_language(0xb59)
#define BOT_ENDRELAY2	get_language(0xb5a)
#define BOT_PARTYREJOINED get_language(0xb5b)
#define BOT_DROPPEDRELAY get_language(0xb5c)
#define BOT_BREAKRELAY	get_language(0xb5d)
#define BOT_RELAYBROKEN	get_language(0xb5e)
#define BOT_PINGTIMEOUT	get_language(0xb5f)
#define BOT_BOTNOTLEAFLIKE get_language(0xb60)
#define BOT_BOTDROPPED	get_language(0xb61)
#define BOT_ALREADYLINKING get_language(0xb62)

/* Messages pertaining to MODULES */
#define MOD_ALREADYLOAD	get_language(0x200)
#define MOD_BADCWD	get_language(0x201)
#define MOD_NOSTARTDEF	get_language(0x202)
#define MOD_LOADED	get_language(0x203)
#define MOD_NEEDED	get_language(0x204)
#define MOD_NOCLOSEDEF	get_language(0x205)
#define MOD_UNLOADED	get_language(0x206)
#define MOD_NOSUCH	get_language(0x207)
#define MOD_NOINFO	get_language(0x208)
#define MOD_LOADERROR	get_language(0x209)
#define MOD_UNLOADERROR	get_language(0x20a)
#define MOD_CANTLOADMOD	get_language(0x20b)
#define MOD_STAGNANT	get_language(0x20c)
#define MOD_NOCRYPT	get_language(0x20d)
#define MOD_NOFILESYSMOD get_language(0x20e)


#define DCC_NOSTRANGERS	get_language(0xc00)
#define DCC_REFUSED	get_language(0xc01)
#define DCC_REFUSED2	get_language(0xc02)
#define DCC_REFUSED3	get_language(0xc03)
#define DCC_REFUSED4	get_language(0xc04)
#define DCC_REFUSED5	get_language(0xc05)
#define DCC_REFUSED6	get_language(0xc06)
#define DCC_REFUSED7    get_language(0xc21)
#define DCC_TOOMANY	get_language(0xc07)
#define DCC_TRYLATER	get_language(0xc08)
#define DCC_REFUSEDTAND	get_language(0xc09)
#define DCC_NOSTRANGERFILES1	get_language(0xc0a)
#define DCC_NOSTRANGERFILES2	get_language(0xc0b)
#define DCC_TOOMANYDCCS1	get_language(0xc0c)
#define DCC_TOOMANYDCCS2	get_language(0xc0d)
#define DCC_DCCNOTSUPPORTED	get_language(0xc0e)
#define DCC_REFUSEDNODCC	get_language(0xc0f)
#define DCC_REFUSEDNODCC_ARGS	param, nick, from
#define DCC_FILENAMEBADSLASH	get_language(0xc10)
#define DCC_MISSINGFILESIZE	get_language(0xc11)
#define DCC_FILEEXISTS		get_language(0xc12)
#define DCC_CREATEERROR		get_language(0xc13)
#define DCC_FILEBEINGSENT	get_language(0xc14)
#define DCC_REFUSEDNODCC2	get_language(0xc15)
#define DCC_REFUSEDNODCC3	get_language(0xc16)
#define DCC_FILETOOLARGE	get_language(0xc17)
#define DCC_FILETOOLARGE2	get_language(0xc18)
#define DCC_CONNECTFAILED1	get_language(0xc19)
#define DCC_CONNECTFAILED2	get_language(0xc1a)
#define DCC_CONNECTFAILED3  get_language(0xc22)
#define DCC_FILESYSBROKEN	get_language(0xc1b)
#define DCC_ENTERPASS		get_language(0xc1c)
#define DCC_FLOODBOOT		get_language(0xc1d)
#define DCC_BOOTED1	get_language(0xc1e)
#define DCC_BOOTED2	get_language(0xc1f)
#define DCC_BOOTED2_ARGS		files ? "file section" : "bot", \
					by, reason[0] ? ": " : ".", reason
#define DCC_BOOTED3	get_language(0xc20)
#define DCC_BOOTED3_ARGS	by, dcc[idx].nick, \
                                reason[0] ? ": " : ".", reason


/* Stuff from chan.c */

#define CHAN_LIMBOBOT	get_language(0xd00)

/* BOTNET messages */

#define NET_FAKEREJECT	get_language(0xe00)
#define NET_LINKEDTO	get_language(0xe01)
#define NET_WRONGBOT	get_language(0xe02)
#define NET_LEFTTHE	get_language(0xe03)
#define NET_JOINEDTHE	get_language(0xe04)
#define NET_AWAY	get_language(0xe05)
#define NET_UNAWAY	get_language(0xe06)
#define NET_NICKCHANGE	get_language(0xe07)
#endif
