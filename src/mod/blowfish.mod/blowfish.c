/*
   blowfish.c -- handles:
   encryption and decryption of passwords

   The first half of this is very lightly edited from public domain
   sourcecode.  For simplicity, this entire module will remain public
   domain.
 */

#ifdef MODULES
#define MODULE_NAME "blowfish"
#endif
#include "../module.h"
#include "blowfish.h"
#include "bf_tab.h"		/* P-box P-array, S-box  */

/* each box takes up 4k so be very careful here */
#define BOXES  3

/* #define S(x,i) (bf_S[i][x.w.byte##i])  */
#define S0(x) (bf_S[0][x.w.byte0])
#define S1(x) (bf_S[1][x.w.byte1])
#define S2(x) (bf_S[2][x.w.byte2])
#define S3(x) (bf_S[3][x.w.byte3])
#define bf_F(x) (((S0(x) + S1(x)) ^ S2(x)) + S3(x))
#define ROUND(a,b,n) (a.word ^= bf_F(b) ^ bf_P[n])

/* keep a set of rotating P & S boxes */
static struct box_t {
   UWORD_32bits *P;
   UWORD_32bits **S;
   char key[81];
   char keybytes;
   time_t lastuse;
} box[BOXES];

/*
   static UWORD_32bits bf_P[bf_N+2];
   static UWORD_32bits bf_S[4][256];
 */
static UWORD_32bits *bf_P;
static UWORD_32bits **bf_S;


#ifdef MODULES
static int blowfish_expmem()
#else
int expmem_blowfish()
#endif
{
   int i, tot = 0;
   modcontext;
   for (i = 0; i < BOXES; i++)
      if (box[i].P != NULL) {
	 tot += ((bf_N + 2) * sizeof(UWORD_32bits));
	 tot += (4 * sizeof(UWORD_32bits *));
	 tot += (4 * 256 * sizeof(UWORD_32bits));
      }
   return tot;
}

#ifdef MODULES
static
#endif
void blowfish_encipher PROTO2(UWORD_32bits *, xl, UWORD_32bits *, xr)
{
   union aword Xl;
   union aword Xr;

   Xl.word = *xl;
   Xr.word = *xr;

   Xl.word ^= bf_P[0];
   ROUND(Xr, Xl, 1);
   ROUND(Xl, Xr, 2);
   ROUND(Xr, Xl, 3);
   ROUND(Xl, Xr, 4);
   ROUND(Xr, Xl, 5);
   ROUND(Xl, Xr, 6);
   ROUND(Xr, Xl, 7);
   ROUND(Xl, Xr, 8);
   ROUND(Xr, Xl, 9);
   ROUND(Xl, Xr, 10);
   ROUND(Xr, Xl, 11);
   ROUND(Xl, Xr, 12);
   ROUND(Xr, Xl, 13);
   ROUND(Xl, Xr, 14);
   ROUND(Xr, Xl, 15);
   ROUND(Xl, Xr, 16);
   Xr.word ^= bf_P[17];

   *xr = Xl.word;
   *xl = Xr.word;
}

#ifdef MODULES
static
#endif
void blowfish_decipher PROTO2(UWORD_32bits *, xl, UWORD_32bits *, xr)
{
   union aword Xl;
   union aword Xr;

   Xl.word = *xl;
   Xr.word = *xr;

   Xl.word ^= bf_P[17];
   ROUND(Xr, Xl, 16);
   ROUND(Xl, Xr, 15);
   ROUND(Xr, Xl, 14);
   ROUND(Xl, Xr, 13);
   ROUND(Xr, Xl, 12);
   ROUND(Xl, Xr, 11);
   ROUND(Xr, Xl, 10);
   ROUND(Xl, Xr, 9);
   ROUND(Xr, Xl, 8);
   ROUND(Xl, Xr, 7);
   ROUND(Xr, Xl, 6);
   ROUND(Xl, Xr, 5);
   ROUND(Xr, Xl, 4);
   ROUND(Xl, Xr, 3);
   ROUND(Xr, Xl, 2);
   ROUND(Xl, Xr, 1);
   Xr.word ^= bf_P[0];

   *xl = Xr.word;
   *xr = Xl.word;
}

#ifdef MODULES
static void blowfish_report PROTO1(int, idx)
#else
void debug_blowfish PROTO1(int, idx)
#endif
{
   int i, tot = 0;
   for (i = 0; i < BOXES; i++)
      if (box[i].P != NULL)
	 tot++;
   modprintf(idx, "%d of %d boxes in use: ", tot, BOXES);
   for (i = 0; i < BOXES; i++)
      if (box[i].P != NULL) {
	 modprintf(idx, "(age: %d) ", time(NULL) - box[i].lastuse);
      }
   modprintf(idx, "\n");
}

#ifdef MODULES
static
#endif
void blowfish_init PROTO2(UBYTE_08bits *, key, short, keybytes)
{
   int i, j, bx;
   time_t lowest;
   UWORD_32bits data;
   UWORD_32bits datal;
   UWORD_32bits datar;
   union aword temp;

   /* is buffer already allocated for this? */
   for (i = 0; i < BOXES; i++)
      if (box[i].P != NULL) {
	 if ((box[i].keybytes == keybytes) &&
	 (strncmp((char *) (box[i].key), (char *) key, keybytes) == 0)) {
	    /* match! */
	    box[i].lastuse = time(NULL);
	    bf_P = box[i].P;
	    bf_S = box[i].S;
	    return;
	 }
      }
   /* no pre-allocated buffer: make new one */
   /* set 'bx' to empty buffer */
   bx = (-1);
   for (i = 0; i < BOXES; i++) {
      if (box[i].P == NULL) {
	 bx = i;
	 i = BOXES + 1;
      }
   }
   if (bx < 0) {
      /* find oldest */
      lowest = time(NULL);
      for (i = 0; i < BOXES; i++)
	 if (box[i].lastuse <= lowest) {
	    lowest = box[i].lastuse;
	    bx = i;
	 }
      modfree(box[bx].P);
      for (i = 0; i < 4; i++)
	 modfree(box[bx].S[i]);
      modfree(box[bx].S);
   }
   /* initialize new buffer */
   /* uh... this is over 4k */
   box[bx].P = (UWORD_32bits *) modmalloc((bf_N + 2) * sizeof(UWORD_32bits));
   box[bx].S = (UWORD_32bits **) modmalloc(4 * sizeof(UWORD_32bits *));
   for (i = 0; i < 4; i++)
      box[bx].S[i] = (UWORD_32bits *) modmalloc(256 * sizeof(UWORD_32bits));
   bf_P = box[bx].P;
   bf_S = box[bx].S;
   box[bx].keybytes = keybytes;
   strncpy(box[bx].key, key, keybytes);
   box[bx].lastuse = time(NULL);

   /* robey: reset blowfish boxes to initial state */
   /* (i guess normally it just keeps scrambling them, but here it's */
   /* important to get the same encrypted result each time) */
   for (i = 0; i < bf_N + 2; i++)
      bf_P[i] = initbf_P[i];
   for (i = 0; i < 4; i++)
      for (j = 0; j < 256; j++)
	 bf_S[i][j] = initbf_S[i][j];

   j = 0;
   for (i = 0; i < bf_N + 2; ++i) {
      temp.word = 0;
      temp.w.byte0 = key[j];
      temp.w.byte1 = key[(j + 1) % keybytes];
      temp.w.byte2 = key[(j + 2) % keybytes];
      temp.w.byte3 = key[(j + 3) % keybytes];
      data = temp.word;
      bf_P[i] = bf_P[i] ^ data;
      j = (j + 4) % keybytes;
   }

   datal = 0x00000000;
   datar = 0x00000000;

   for (i = 0; i < bf_N + 2; i += 2) {
      blowfish_encipher(&datal, &datar);

      bf_P[i] = datal;
      bf_P[i + 1] = datar;
   }

   for (i = 0; i < 4; ++i) {
      for (j = 0; j < 256; j += 2) {

	 blowfish_encipher(&datal, &datar);

	 bf_S[i][j] = datal;
	 bf_S[i][j + 1] = datar;
      }
   }
}


/* stuff below this line was written by robey for eggdrop use */

/* of course, if you change either of these, then your userfile will */
/* no longer be able to be shared. :)  */
#define SALT1  0xdeadd061
#define SALT2  0x23f6b095

/* convert 64-bit encrypted password to text for userfile */
static char *base64 = "./0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

#ifdef MODULES
static
#endif
int base64dec PROTO1(char, c)
{
   int i;
   for (i = 0; i < 64; i++)
      if (base64[i] == c)
	 return i;
   return 0;
}

#ifdef MODULES
static
#endif
void encrypt_pass PROTO2(char *, text, char *, new)
{
   UWORD_32bits left, right;
   int n;
   char *p;
   blowfish_init(text, strlen(text));
   left = SALT1;
   right = SALT2;
   blowfish_encipher(&left, &right);
   p = new;
   *p++ = '+';			/* + means encrypted pass */
   n = 32;
   while (n > 0) {
      *p++ = base64[right & 0x3f];
      right = (right >> 6);
      n -= 6;
   }
   n = 32;
   while (n > 0) {
      *p++ = base64[left & 0x3f];
      left = (left >> 6);
      n -= 6;
   }
   *p = 0;
}

/* returned string must be freed when done with it! */
#ifdef MODULES
static
#endif
char *encrypt_string PROTO2(char *, key, char *, str)
{
   UWORD_32bits left, right;
   char *p, *s, *dest, *d;
   int i;
   dest = (char *) modmalloc((strlen(str) + 9) * 2);
   /* pad fake string with 8 bytes to make sure there's enough */
   s = (char *) modmalloc(strlen(str) + 9);
   strcpy(s, str);
   p = s;
   while (*p)
      p++;
   for (i = 0; i < 8; i++)
      *p++ = 0;
   blowfish_init(key, strlen(key));
   p = s;
   d = dest;
   while (*p) {
      left = ((*p++) << 24);
      left += ((*p++) << 16);
      left += ((*p++) << 8);
      left += (*p++);
      right = ((*p++) << 24);
      right += ((*p++) << 16);
      right += ((*p++) << 8);
      right += (*p++);
      blowfish_encipher(&left, &right);
      for (i = 0; i < 6; i++) {
	 *d++ = base64[right & 0x3f];
	 right = (right >> 6);
      }
      for (i = 0; i < 6; i++) {
	 *d++ = base64[left & 0x3f];
	 left = (left >> 6);
      }
   }
   *d = 0;
   modfree(s);
   return dest;
}

/* returned string must be freed when done with it! */
#ifdef MODULES
static
#endif
char *decrypt_string PROTO2(char *, key, char *, str)
{
   UWORD_32bits left, right;
   char *p, *s, *dest, *d;
   int i;
   dest = (char *) modmalloc(strlen(str) + 12);
   /* pad encoded string with 0 bits in case it's bogus */
   s = (char *) modmalloc(strlen(str) + 12);
   strcpy(s, str);
   p = s;
   while (*p)
      p++;
   for (i = 0; i < 12; i++)
      *p++ = 0;
   blowfish_init(key, strlen(key));
   p = s;
   d = dest;
   while (*p) {
      right = 0L;
      left = 0L;
      for (i = 0; i < 6; i++)
	 right |= (base64dec(*p++)) << (i * 6);
      for (i = 0; i < 6; i++)
	 left |= (base64dec(*p++)) << (i * 6);
      blowfish_decipher(&left, &right);
      for (i = 0; i < 4; i++)
	 *d++ = (left & (0xff << ((3 - i) * 8))) >> ((3 - i) * 8);
      for (i = 0; i < 4; i++)
	 *d++ = (right & (0xff << ((3 - i) * 8))) >> ((3 - i) * 8);
   }
   *d = 0;
   modfree(s);
   return dest;
}

#ifdef MODULES
static
#endif
int tcl_encrypt STDVAR
{
   char *p;
    BADARGS(3, 3, " key string");
    p = encrypt_string(argv[1], argv[2]);
    Tcl_AppendResult(irp, p, NULL);
    modfree(p);
    return TCL_OK;
}

#ifdef MODULES 
static
#endif
int tcl_decrypt STDVAR
{
   char *p;
    BADARGS(3, 3, " key string");
    p = decrypt_string(argv[1], argv[2]);
    Tcl_AppendResult(irp, p, NULL);
    modfree(p);
    return TCL_OK;
}
#ifdef MODULES 
static tcl_cmds mytcls[] =
{
   {"encrypt", tcl_encrypt},
   {"decrypt", tcl_decrypt},
   {0, 0}
};

Function *global;

/* you CANT -module an encryption module , so -module just resets it */
static char *blowfish_close()
{
   return "You can't unload an encryption module";
}

char *blowfish_start PROTO((Function *));

static Function blowfish_table[] =
{
   (Function) blowfish_start,
   (Function) blowfish_close,
   (Function) blowfish_expmem,
   (Function) blowfish_report,
};

/* initialize buffered boxes */
char *blowfish_start PROTO1(Function *, globs)
#else 
void init_blowfish()
#endif
{
   int i;
#ifdef MODULES
   if (globs != NULL) {
      global = globs;
#endif
      for (i = 0; i < BOXES; i++) {
	 box[i].P = NULL;
	 box[i].S = NULL;
	 box[i].key[0] = 0;
	 box[i].lastuse = 0L;
      }
#ifdef MODULES
      modcontext;
      add_hook(HOOK_ENCRYPT_PASS, encrypt_pass);
   }
   module_register("blowfish", blowfish_table, 1, 0);
   add_tcl_commands(mytcls);
   return NULL;
#endif
}
