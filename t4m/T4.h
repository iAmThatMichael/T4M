#pragma once
typedef enum assetType_e
{
	ASSET_TYPE_XMODELPIECES = 0,
	ASSET_TYPE_PHYSPRESET = 1,
	ASSET_TYPE_PHYSCONSTRAINTS = 2,
	ASSET_TYPE_DESTRUCTIBLEDEF = 3,
	ASSET_TYPE_XANIM = 4,
	ASSET_TYPE_XMODEL = 5,
	ASSET_TYPE_MATERIAL = 6,
	ASSET_TYPE_TECHSET = 7,
	ASSET_TYPE_IMAGE = 8,
	ASSET_TYPE_SOUND = 9,
	ASSET_TYPE_LOADED_SOUND = 10,
	ASSET_TYPE_COL_MAP_SP = 11,
	ASSET_TYPE_COL_MAP_MP = 12,
	ASSET_TYPE_COM_MAP = 13,
	ASSET_TYPE_GAME_MAP_SP = 14,
	ASSET_TYPE_GAME_MAP_MP = 15,
	ASSET_TYPE_MAP_ENTS = 16,
	ASSET_TYPE_GFX_MAP = 17,
	ASSET_TYPE_LIGHTDEF = 18,
	ASSET_TYPE_UI_MAP = 19,
	ASSET_TYPE_FONT = 20,
	ASSET_TYPE_MENUFILE = 21,
	ASSET_TYPE_MENU = 22,
	ASSET_TYPE_LOCALIZE = 23,
	ASSET_TYPE_WEAPON = 24,
	ASSET_TYPE_SNDDRIVERGLOBALS = 25,
	ASSET_TYPE_FX = 26,
	ASSET_TYPE_IMPACTFX = 27,
	ASSET_TYPE_AITYPE = 28,
	ASSET_TYPE_MPTYPE = 29,
	ASSET_TYPE_CHARACTER = 30,
	ASSET_TYPE_XMODELALIAS = 31,
	ASSET_TYPE_RAWFILE = 32,
	ASSET_TYPE_STRINGTABLE = 33,
	ASSET_TYPE_PACKINDEX = 34
} assetType_t;

typedef enum
{
	DVAR_FLAG_ARCHIVE = 1 << 0,				// 0x0001
	DVAR_FLAG_USERINFO = 1 << 1,			// 0x0002
	DVAR_FLAG_SERVERINFO = 1 << 2,			// 0x0004
	DVAR_FLAG_SYSTEMINFO = 1 << 3,			// 0x0008
	DVAR_FLAG_INIT = 1 << 4,				// 0x0010
	DVAR_FLAG_LATCH = 1 << 5,				// 0x0020
	DVAR_FLAG_ROM = 1 << 6,					// 0x0040
	DVAR_FLAG_CHEAT = 1 << 7,				// 0x0080
	DVAR_FLAG_DEVELOPER = 1 << 8,			// 0x0100
	DVAR_FLAG_SAVED = 1 << 9,				// 0x0200
	DVAR_FLAG_NORESTART = 1 << 10,			// 0x0400
	DVAR_FLAG_CHANGEABLE_RESET = 1 << 12,	// 0x1000
	DVAR_FLAG_EXTERNAL = 1 << 14,			// 0x4000
	DVAR_FLAG_AUTOEXEC = 1 << 15,			// 0x8000
} dvar_flag;

typedef enum
{
	DVAR_TYPE_BOOL = 0,
	DVAR_TYPE_FLOAT = 1,
	DVAR_TYPE_FLOAT_2 = 2,
	DVAR_TYPE_FLOAT_3 = 3,
	DVAR_TYPE_FLOAT_4 = 4,
	DVAR_TYPE_INT = 5,
	DVAR_TYPE_ENUM = 6,
	DVAR_TYPE_STRING = 7,
	DVAR_TYPE_COLOR = 8,
	DVAR_TYPE_INT64	= 9 // only in Tx
} dvar_type;

union dvar_value_t {
	char*	string;
	int		integer;
	float	value;
	bool	boolean;
	float	vec2[2];
	float	vec3[3];
	float	vec4[4];
	BYTE	color[4]; //to get float: multiply by 0.003921568859368563 - BaberZz
	__int64 integer64; // only in Tx
};
union dvar_maxmin_t {
	int i;
	float f;
};
typedef struct dvar_t
{
	//startbyte:endbyte
	const char*		name; //0:3
	const char*		description; //4:7
	unsigned int	flags; //8:11
	char			type; //12:12
	char			pad2[3]; //13:15
	dvar_value_t	current; //16:31
	dvar_value_t	latched; //32:47
	dvar_value_t	default; //48:64
	dvar_maxmin_t min; //65:67
	dvar_maxmin_t max; //68:72 woooo
} dvar_t;

/*
	Engine Functions
*/
extern "C"
{
	typedef void(__cdecl * Cbuf_AddText_t)(int a1, const char* cmd);
	extern Cbuf_AddText_t Cbuf_AddText;

	//typedef void(__cdecl * Com_Printf_t)(int channel, const char *fmt, ...);
	//extern Com_Printf_t Com_Printf;

	typedef void(__cdecl * Com_PrintMessage_t)(int channel, const char *fmt, int error);
	extern Com_PrintMessage_t Com_PrintMessage;

	extern dvar_t* Dvar_RegisterBool(bool value, const char *dvarName, int flags, const char *description);
	extern void Com_Printf(int channel, const char* format, ...);

	extern void Sys_ShowConsole();
}

/*
	Source Functions
*/
void* ReallocateAssetPool(assetType_t type, unsigned int newSize);