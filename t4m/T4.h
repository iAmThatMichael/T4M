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
	DVAR_FLAG_NONE = 0x0,			//no flags
	DVAR_FLAG_SAVED = 0x1,			//saves in config_mp.cfg for clients
	DVAR_FLAG_LATCHED = 0x2,			//no changing apart from initial value (although it might apply on a map reload, I think)
	DVAR_FLAG_CHEAT = 0x4,			//cheat
	DVAR_FLAG_REPLICATED = 0x8,			//on change, this is sent to all clients (if you are host)
	DVAR_FLAG_UNKNOWN10 = 0x10,			//unknown
	DVAR_FLAG_UNKNOWN20 = 0x20,			//unknown
	DVAR_FLAG_UNKNOWN40 = 0x40,			//unknown
	DVAR_FLAG_UNKNOWN80 = 0x80,			//unknown
	DVAR_FLAG_USERCREATED = 0x100,		//a 'set' type command created it
	DVAR_FLAG_USERINFO = 0x200,		//userinfo?
	DVAR_FLAG_SERVERINFO = 0x400,		//in the getstatus oob
	DVAR_FLAG_WRITEPROTECTED = 0x800,		//write protected
	DVAR_FLAG_UNKNOWN1000 = 0x1000,		//unknown
	DVAR_FLAG_READONLY = 0x2000,		//read only (same as 0x800?)
	DVAR_FLAG_UNKNOWN4000 = 0x4000,		//unknown
	DVAR_FLAG_UNKNOWN8000 = 0x8000,		//unknown
	DVAR_FLAG_UNKNOWN10000 = 0x10000,		//unknown
	DVAR_FLAG_DEDISAVED = 0x1000000,		//unknown
	DVAR_FLAG_NONEXISTENT = 0xFFFFFFFF	//no such dvar
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
// 67/72 bytes figured out
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
typedef void(__cdecl * Com_Printf_t)(int channel, const char *fmt, ...);
extern Com_Printf_t Com_Printf;

typedef void(__cdecl * Com_PrintMessage_t)(int channel, const char *fmt, int error, ...);
extern Com_PrintMessage_t Com_PrintMessage;

typedef dvar_t* (__cdecl * Dvar_RegisterBool_t)(int value, const char *dvarName, int flags, const char *description);
extern Dvar_RegisterBool_t Dvar_RegisterBool;

/*
	Source Functions
*/
void* ReallocateAssetPool(assetType_t type, unsigned int newSize);