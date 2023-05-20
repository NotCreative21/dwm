/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"JetBrains Mono:pixelsize=12:antialiasing=true",
	"FiraCode-Regular:pixelsize=9:antialias=true:autohint=true",
	"JoyPixels:size=9"
};
static const char dmenufont[]       = "JetBrains Mono:pixelsize=12:antialiasing=true";
static const char col_gray1[]       = "#27282d";               /* bar color */
static const char col_gray2[]       = "#6b7b88";               /* window border */
static const char col_gray3[]       = "#c9d2e4";               /* bar font  */
static const char col_gray4[]       = "#9abada";               /* selected tag number color*/
static const char col_cyan[]        = "#27282d";               /* selected bar color*/
static const char col_borderbar[]   = "#6b7b88";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1, col_gray2 },
};

/* bar underline */
static const unsigned int ulinepad		= 5;
static const unsigned int ulinestroke 	= 2;
static const unsigned int ulinecoffset  = 0;
static const int ulineall 				= 0;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Firefox",  NULL,       NULL,       0,            0,           -1 },
  	{ "Steam",    NULL,       NULL,       0,            1,           -1 },
	{ "Discord",  NULL,       NULL,       0,            1,           -1 },
	{ "multimc",  NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]",      tile },    /* first entry is default */
	{ "<>",      NULL },    /* no layout function means floating behavior */
	{ "{}",      monocle },
};

/* key definitions, 4 is the super key for modmask */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "tabbed", "-r", "2", "st", "-w", "''", "-e", "zsh", NULL };

static const char *playerplaypause[] = { "playerctl",  "--player=spotify,%any", "play-pause", NULL };
static const char *playernext[] = { "playerctl",  "--player=spotify,%any", "next", NULL };
static const char *playerprevious[] = { "playerctl",  "--player=spotify,%any", "previous", NULL };

static const char *incvol[] = { "incrementvol", NULL };
static const char *decvol[] = { "decrementvol", NULL };

static const char *screenshot[] = { "screenshot", NULL };
static const char *screenshot2[] = { "screenshot2", NULL };

static const char *browser[] = { "browser", NULL };
static const char *discord[] = { "discord", NULL };
static const char *slock[] = { "slock", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = discord } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = slock } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshot } },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = screenshot2 } },
  	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
 	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ 0,		        XF86XK_AudioPlay,      spawn,  		   {.v = playerplaypause } },
	{ 0,		        XF86XK_AudioNext,      spawn,  		   {.v = playernext } },
	{ 0,		        XF86XK_AudioPrev,      spawn,  		   {.v = playerprevious } },
	{ 0,      	 XF86XK_AudioLowerVolume,      spawn, 		   {.v = decvol } },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn, 		   {.v = incvol } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

