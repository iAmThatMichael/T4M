enum XAssetType
{
	ASSET_TYPE_XMODELPIECES = 0, // 64
	ASSET_TYPE_PHYSPRESET = 1, // 64
	ASSET_TYPE_PHYSCONSTRAINTS = 2, // 64
	ASSET_TYPE_DESTRUCTIBLEDEF = 3, // 64
	ASSET_TYPE_XANIM = 4, // 4096
	ASSET_TYPE_XMODEL = 5, // 1000
	ASSET_TYPE_MATERIAL = 6, // 2048
	ASSET_TYPE_TECHSET = 7, // 512
	ASSET_TYPE_IMAGE = 8, // 2400
	ASSET_TYPE_SOUND = 9, // 16000
	ASSET_TYPE_LOADED_SOUND = 10, // 1600
	ASSET_TYPE_COL_MAP_SP = 11, // 1 
	ASSET_TYPE_COL_MAP_MP = 12, // 1
	ASSET_TYPE_COM_MAP = 13, // 1
	ASSET_TYPE_GAME_MAP_SP = 14, // 1
	ASSET_TYPE_GAME_MAP_MP = 15, // 1
	ASSET_TYPE_MAP_ENTS = 16, // 2
	ASSET_TYPE_GFX_MAP = 17, // 1
	ASSET_TYPE_LIGHTDEF = 18, // 32
	ASSET_TYPE_UI_MAP = 19, // 0
	ASSET_TYPE_FONT = 20, // 16
	ASSET_TYPE_MENUFILE = 21, // 128
	ASSET_TYPE_MENU = 22, // 600
	ASSET_TYPE_LOCALIZE = 23, // 8342
	ASSET_TYPE_WEAPON = 24, // 256 (actually 128)
	ASSET_TYPE_SNDDRIVERGLOBALS = 25, // 1
	ASSET_TYPE_FX = 26, // 400
	ASSET_TYPE_IMPACTFX = 27, // 4
	ASSET_TYPE_AITYPE = 28, // 0
	ASSET_TYPE_MPTYPE = 29, // 0
	ASSET_TYPE_CHARACTER = 30, // 0
	ASSET_TYPE_XMODELALIAS = 31, // 0
	ASSET_TYPE_RAWFILE = 32, // 1024
	ASSET_TYPE_STRINGTABLE = 33, // 50
	ASSET_TYPE_PACKINDEX = 34, // 16
	ASSET_TYPE_MAX = 35, // empty
};

#pragma pack(push, 1)
union XAssetHeader
{
	// removed xassets -- have to find yourself
  //XModelPieces *xmodelPieces;
  //PhysPreset *physPreset;
  //XAnimParts *parts;
  //XModel *model;
  //Material *material;
  //MaterialPixelShader *pixelShader;
  //MaterialVertexShader *vertexShader;
  //MaterialTechniqueSet *techniqueSet;
  //GfxImage *image;
  //snd_alias_list_t *sound;
  //SndCurve *sndCurve;
  //clipMap_t *clipMap;
  //ComWorld *comWorld;
  //GameWorldSp *gameWorldSp;
  //GameWorldMp *gameWorldMp;
  //MapEnts *mapEnts;
  //GfxWorld *gfxWorld;
  //GfxLightDef *lightDef;
  //Font_s *font;
  //MenuList *menuList;
  //menuDef_t *menu;
  //LocalizeEntry *localize;
  //WeaponDef *weapon;
  //SndDriverGlobals *sndDriverGlobals;
  //FxEffectDef *fx;
  //FxImpactTable *impactFx;
  //RawFile *rawfile;
  //StringTable *stringTable;
  void *data;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct XAsset
{
  XAssetType type;
  XAssetHeader header;
};

#pragma pack(push, 1)
struct XAssetEntry
{
	XAsset asset;		// 0
	char zoneIndex;		// 8
	bool inuse;			// 9
	unsigned __int16 nextHash;	// 10
	unsigned __int16 nextOverride;
	unsigned __int16 usageFrame;
};
#pragma pack(pop)

#pragma pack(push, 1)
union XAssetEntryPoolEntry
{
	XAssetEntry entry;
	XAssetEntryPoolEntry *next;
};
#pragma pack(pop)