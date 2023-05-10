#ifndef SDK_PLAYERINFO
#define SDK_PLAYERINFO

#define MAX_NAME_LEN 128

struct PlayerInfo {
	unsigned long long version;
	unsigned long long xuid;
	char name[MAX_NAME_LEN];
	int userID;
	char guid[32 + 1];
	unsigned int friendsID;
	char friendsName[MAX_NAME_LEN];
	bool fakeplayer;
	bool ishltv;
	unsigned int customFiles[4];
	unsigned char filesDownloaded;
};

#endif