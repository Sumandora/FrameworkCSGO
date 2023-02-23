#ifndef SDK_RAYTRACE_DEFINITONS_CONTENTFLAGS
#define SDK_RAYTRACE_DEFINITONS_CONTENTFLAGS

#define CONTENTS_EMPTY					0

#define CONTENTS_SOLID					0x1
#define CONTENTS_WINDOW					0x2
#define CONTENTS_AUX					0x4
#define CONTENTS_GRATE					0x8
#define CONTENTS_SLIME					0x10
#define CONTENTS_WATER					0x20
#define CONTENTS_BLOCKLOS				0x40
#define CONTENTS_OPAQUE					0x80
#define CONTENTS_TESTFOGVOLUME			0x100
#define CONTENTS_UNUSED                 0x200

#define CONTENTS_BLOCKLIGHT				0x400

#define CONTENTS_TEAM1					0x800
#define CONTENTS_TEAM2					0x1000

#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

#define CONTENTS_MOVEABLE				0x4000

#define CONTENTS_AREAPORTAL				0x8000

#define CONTENTS_PLAYERCLIP				0x10000
#define CONTENTS_MONSTERCLIP			0x20000

#define CONTENTS_BRUSH_PAINT			0x40000
#define CONTENTS_GRENADECLIP			0x80000
#define CONTENTS_UNUSED2				0x100000
#define CONTENTS_UNUSED3				0x200000
#define CONTENTS_UNUSED4				0x400000
#define CONTENTS_UNUSED5				0x800000

#define CONTENTS_ORIGIN					0x1000000

#define CONTENTS_MONSTER				0x2000000
#define CONTENTS_DEBRIS					0x4000000
#define CONTENTS_DETAIL					0x8000000
#define CONTENTS_TRANSLUCENT			0x10000000
#define CONTENTS_LADDER					0x20000000
#define CONTENTS_HITBOX					0x40000000

#endif
