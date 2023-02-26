#ifndef SDK_PLAYERINFO
#define SDK_PLAYERINFO

struct PlayerInfo {
	unsigned long long	version;
	unsigned long long	xuid;
	char				name[128];
	int					userID;
	char				guid[32 + 1];
	unsigned int		friendsID;
	char				friendsName[128];
	bool				fakeplayer;
	bool				ishltv;
	unsigned int		customFiles[4];
	unsigned char		filesDownloaded;
};

#endif