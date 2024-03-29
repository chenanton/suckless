/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int user_bh 	= 16;
static const unsigned int horizpadbar = 0;
static const unsigned int vertpadbar = 20;
static const unsigned int snap      = 5;       /* snap pixel */
static const unsigned int cornerrad = 0;
static const unsigned int gappih    = 18;
static const unsigned int gappiv    = 18;
static const unsigned int gappoh    = 18;
static const unsigned int gappov    = 18;
static const int smartgaps          = 0;
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* DESKTOP */
/* static const char *fonts[]          = { "PragmataPro Mono Liga:style=regular:pixelsize=14:antialias=true:autohint=true" }; */
/* static const char dmenufont[]       = "PragmataPro Mono Liga:style=regular:pixelsize=14:antialias=true:autohint=true"; */
/* LAPTOP */
static const char *fonts[]          = { "PragmataPro Mono Liga:style=regular:pixelsize=16:antialias=true:autohint=true" };
static const char dmenufont[]       = "PragmataPro Mono Liga:style=regular:pixelsize=16:antialias=true:autohint=true";
static const char col_gray1[]       = "#101620";
static const char col_gray2[]       = "#2c323c";
static const char col_gray3[]       = "#838495";
static const char col_gray4[]       = "#d3d4e5";
static const char col_cyan[]        = "#2c323c";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray2, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray2, col_gray2 },
};

/* tagging */
static const char *tags[] = { 
    /* "Home", "", "", "Media", "Games", "Misc" */
    "甲", "乙", "丙", "丁", "戊", "己"
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "feh",     NULL,       NULL,       0,            1,           -1 },
	{ "mpv",     NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	// { "==",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "",      bstackhoriz },	
};

/* key definitions */
#define MODKEY    Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *smalltermcmd[]  = { "/home/anton/.local/bin/sst", NULL };

/* media keys */
static const char *upbright[] = { "/bin/zsh", "/home/anton/.local/bin/backlight_incr", NULL };
static const char *downbright[] = { "/bin/zsh", "/home/anton/.local/bin/backlight_decr", NULL };

static const char *upvol[]   = { "/usr/bin/amixer", "set", "Master", "5%+",     NULL };
static const char *downvol[] = { "/usr/bin/amixer", "set", "Master", "5%-",     NULL };
static const char *mutevol[] = { "/usr/bin/amixer", "set",   "Master", "toggle",  NULL };
static const char *toggleplay[] = { "/home/anton/.cargo/bin/spt", "pb", "-t", NULL };
static const char *playprev[] = { "/home/anton/.cargo/bin/spt", "pb", "-p", NULL };
static const char *playnext[] = { "/home/anton/.cargo/bin/spt", "pb", "-n", NULL };

/* misc commands */
static const char *lockscreen[] = { "/usr/bin/physlock", NULL };
static const char *st_smallfont[] = { "/home/anton/.local/bin/st-small", NULL };
static const char *exitdwm[] = { "/usr/bin/killall", "xinit", NULL };

static Key keys[] = {
	/* modifier                     key			    function        argument */
	{ MODKEY,                       XK_d,      		    spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_t,			    spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask|ShiftMask, XK_Return, 		    spawn,          {.v = smalltermcmd } },
	{ MODKEY,                       XK_b,      		    togglebar,      {0} },
	{ MODKEY,                       XK_j,      		    focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      		    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_n,      		    incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_m,      		    incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      		    setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      		    setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, 		    zoom,           {0} },
	{ MODKEY,                       XK_Tab,    		    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      		    killclient,     {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,			    spawn,	    {.v = exitdwm   } },
	{ 0,				XF86XK_AudioPlay,	    spawn,	    {.v = toggleplay   } },
	{ MODKEY,                       XK_space,		    spawn,	    {.v = toggleplay   } },
	{ MODKEY,                       XK_Left,		    spawn, 	    {.v = playprev   } },
	{ MODKEY,                       XK_Right, 	    	    spawn, 	    {.v = playnext   } },
	{ MODKEY|ShiftMask,             XK_l, 	    		    spawn, 	    {.v = lockscreen   } },
	{ 0,				XF86XK_AudioLowerVolume,    spawn,	    {.v = downvol } },
	{ 0,                       	XF86XK_AudioMute,	    spawn,	    {.v = mutevol } },
	{ 0,                       	XF86XK_AudioRaiseVolume,    spawn,	    {.v = upvol   } },
	{ 0,				XF86XK_MonBrightnessUp,	    spawn,	    {.v = upbright} },
	{ 0,				XF86XK_MonBrightnessDown,   spawn,	    {.v = downbright} },
	{ MODKEY,                       XK_t,			    setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_f,   		       setlayout,      {.v = &layouts[1]} },
	// { MODKEY,                       XK_m,   		       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      		    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_f,      		    fullscreen,     {0} },
	// { MODKEY|ShiftMask,             XK_space,			setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_i,			    setlayout,      {0} },
	// { MODKEY,                       XK_space,		    togglefloating, {0} },
	{ MODKEY,                       XK_0,			    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      		    tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  		    focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 		    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  		    tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 		    tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
    // gaps
    { MODKEY|ShiftMask,             XK_g,      togglegaps,     {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

