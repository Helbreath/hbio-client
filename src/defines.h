//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <chrono>
#include <iostream>
#include <sstream>
#include <memory>
#include <ctime>
#include <sys/timeb.h>
#include <string>
#include <cstring>
#include "global_def.h"
#include "enums.h"
#include <cstdint>
#include <SFML/Graphics/Color.hpp>
using Color = sf::Color;

// template <typename OutputIt, typename Locale, typename... T,
//     FMT_ENABLE_IF(std::detail::is_output_iterator<OutputIt, char>::value &&
//         std::detail::is_locale<Locale>::value)>
// FMT_INLINE auto format_to(OutputIt out, const Locale & loc,
//     std::format_string<T...> fmt, T&&... args) -> OutputIt
// {
//     return vformat_to(out, loc, fmt, fmt::make_format_args(args...));
// }

constexpr auto MAX_CHARACTER_SOUNDS = 30;
constexpr auto MAX_MONSTER_SOUNDS = 160;
constexpr auto MAX_EFFECT_SOUNDS = 55;

inline std::vector<std::string> split(std::string str)
{
    std::vector<std::string> tokens;
    std::size_t start = str.find_first_not_of(' '), end = start;

    while (start != std::string::npos)
    {
        end = str.find(' ', start);
        tokens.push_back(str.substr(start, end-start));
        start = str.find_first_not_of(' ', end);
    }
    return tokens;
}

// todo: clean these up over time (they exist to be a drop-in for wsprintf)
template <std::size_t N, class... T>
inline const char * format_to_local(char(&_Out)[N], const std::format_string<T...> _Fmt, T&&... _Args)
{
    memset(_Out, 0, N);
    return _STD vformat_to(_STD move(_Out), _Fmt.get(), _STD make_format_args(_Args...));
}

template <std::output_iterator<const char &> OutputIt, class... T>
inline OutputIt format_to_local(OutputIt _Out, std::size_t len, const std::format_string<T...> _Fmt, T&&... _Args)
{
    memset(_Out, 0, len);
    return _STD vformat_to(_STD move(_Out), _Fmt.get(), _STD make_format_args(_Args...));
}

constexpr bool isValidUnit(int64_t id)
{
    if (id > 0 && id < 30000) return true;
    return false;
}

constexpr bool isValidPlayer(int64_t id)
{
    if (id > 0 && id < 10000) return true;
    return false;
}

constexpr bool isValidNpc(int64_t id)
{
    if (id > 10000 && id < 30000) return true;
    return false;
}

enum chat_types
{
    skill_up = 20,
    small_damage = 21,
    medium_damage = 22,
    critical_damage = 23, // TODO: also level up - separate out?
    magic = 41,
    something = 42, // TODO: figure this out
};

static const auto colorW0 = Color(0, 0, 0); // Black
static const auto colorW1 = Color(70, 70, 80); // Light-blue
static const auto colorW2 = Color(70, 70, 80); // Light-blue
static const auto colorW3 = Color(70, 70, 80); // Light-blue
static const auto colorW4 = Color(70, 100, 70); // Green
static const auto colorW5 = Color(130, 90, 10); // ??
static const auto colorW6 = Color(42, 53, 111); // Heavy-blue
static const auto colorW7 = Color(145, 145, 145); // White
static const auto colorW8 = Color(120, 100, 120); // Violet
static const auto colorW9 = Color(75, 10, 10); // Heavy-red
static const auto colorW10 = Color(135, 104, 30); // Gold

static const auto color0 = Color(100, 100, 100); // ??
static const auto color1 = Color(40, 40, 96); // Blue
static const auto color2 = Color(79, 79, 62); // Manufactured Item?
static const auto color3 = Color(135, 104, 30); // Gold
static const auto color4 = Color(128, 13, 0); // Orange
static const auto color5 = Color(10, 60, 10); // Green
static const auto color6 = Color(40, 40, 40); // Gray
static const auto color7 = Color(48, 129, 130); // ??
static const auto color8 = Color(128, 53, 140); // ??
static const auto color9 = Color(90, 90, 90); // Violet

static const auto color10 = Color(0, 35, 60); // ??
static const auto color11 = Color(105, 90, 70); // Tan
static const auto color12 = Color(95, 91, 54); // Khaki
static const auto overhead_damage_color = Color(85, 85, 8); // Yellow
static const auto overhead_magic_color = Color(63, 10, 10); // Red
static const auto color15 = Color(48, 48, 48); // Black



#define MAXSIDES 3

const std::string sideName[MAXSIDES] = { "Traveler", "Aresden", "Elvine" };
const std::string sideMap[MAXSIDES] = { "default", "aresden", "elvine" };
const std::string sideMapJail[MAXSIDES] = { "default", "arejail", "elvjail" };
const std::string sideMapFarm[MAXSIDES] = { "default", "arefarm", "elvfarm" };
const std::string sideMapRes[MAXSIDES] = { "default", "resurr1", "resurr2" };

#define GUILDPARTYBONUS_RANGE_X		FLAGRANGE_X
#define GUILDPARTYBONUS_RANGE_Y		FLAGRANGE_Y

#define MAXCLIENTS			10000
#define MAXNPCS				15000
#define MAXMAPS				120
#define MAXBUILDITEMS		300
#define CLIENTTIMEOUT		18000
#define AUTOSAVETIME		3*60000
#define HPUPTIME			8000
#define MPUPTIME			15000 // Changed 9->15 Sec
#define SPUPTIME			10000

#define HUNGERTIME			1*60000
#define POISONTIME			12000
#define SUMMONTIME			5*60000
#define NOTICETIME			80000
#define PLANTTIME			5*60000

#define EXPSTOCKTIME		10000
#define AUTOEXPTIME			6*60000



#define TOTALLEVELUPPOINT	7

#define MAXDYNAMICOBJECTS	60000
#define MAXDELAYEVENTS		60000

#define SSN_LIMIT_MULTIPLY_VALUE	2

#define MAXNOTIFYMSGS		1000
#define NIGHTTIME			40

#define CHARPOINTLIMIT		700
#define RAGPROTECTIONTIME	7000
#define MAXREWARDGOLD		99999999

#define MAXSUPERATTACK		10000
#define MAXLUCKY			5000
#define MAXREP				10000
#define MAXNEGAREP			-10000

#define ATTACKAI_NORMAL				1
#define ATTACKAI_EXCHANGEATTACK		2
#define ATTACKAI_TWOBYONEATTACK		3

#define MAXFISHS					200
#define MAXMINERALS					200
#define MAXENGAGINGFISH				30
#define MAXPOTIONTYPES				500
#define MAXCRAFTING					120

#define MOBEVENTTIME				5*60000
#define MAXQUESTTYPE				200

#define MAXSUBLOGSOCK				5

#define ITEMLOG_GIVE				1
#define ITEMLOG_DROP				2
#define ITEMLOG_GET					3
#define ITEMLOG_DEPLETE				4
#define ITEMLOG_NEWGENDROP			5
#define ITEMLOG_DUPITEMID			6
#define ITEMLOG_BUY					7
#define ITEMLOG_SELL				8
#define ITEMLOG_RETRIEVE			9
#define ITEMLOG_DEPOSIT				10
#define ITEMLOG_EXCHANGE			11
#define ITEMLOG_MAGICLEARN			12
#define ITEMLOG_MAKE				13
#define ITEMLOG_SUMMONMONSTER		14
#define ITEMLOG_POISONED			15
#define ITEMLOG_SKILLLEARN			16
#define ITEMLOG_REPAIR				17
#define ITEMLOG_JOINGUILD           18
#define ITEMLOG_BANGUILD            19
#define ITEMLOG_RESERVEFIGZONE      20	//  "
#define ITEMLOG_APPLY               21	//  "
#define ITEMLOG_SHUTUP              22	//  "
#define ITEMLOG_CLOSECONN			23	//  "
#define ITEMLOG_SPELLFIELD			24	//  "
#define ITEMLOG_CREATEGUILD			25	//  "
#define ITEMLOG_GUILDDISMISS		26	//  "
#define ITEMLOG_SUMMONPLAYER        27	//  "
#define ITEMLOG_CREATE				28	//  "
#define ITEMLOG_UPGRADEFAIL         29
#define ITEMLOG_UPGRADESUCCESS      30
#define ITEMLOG_MAILRETRIEVE			31
#define ITEMLOG_MAILSEND				32
#define ITEMLOG_GWHDEPOSIT				33
#define ITEMLOG_GWHRETRIEVE			34


#define ITEMSPREAD_RANDOM			1
#define ITEMSPREAD_FIXED			2


#define CRUSADELOG_ENDCRUSADE       1
#define CRUSADELOG_STARTCRUSADE     2
#define CRUSADELOG_SELECTDUTY       3
#define CRUSADELOG_GETEXP           4
#define CRUSADELOG_APOCALYPSE		5

#define PKLOG_BYPLAYER				1
#define PKLOG_BYPK					2
#define PKLOG_BYENERMY				3
#define PKLOG_BYNPC					4
#define PKLOG_BYOTHER				5
#define PKLOG_REDUCECRIMINAL        6


#define MAXDUPITEMID				100

#define MAXGUILDS					1000
#define MAXONESERVERUSERS			800
#define MAXGATESERVERSTOCKMSGSIZE	30000


#define MAXCONSTRUCTNUM			20 //raised from 10 to 20 to in corporate 4mana stones

#define MAXSCHEDULE				15 // Raised from 10 to 15



#define MAXFIGHTZONE 10

//============================
// #define LEVELLIMIT		130
#define LEVELLIMIT		50
//============================

//============================
#define MAGICHITRATIO 85.5f
//#define MAGICHITRATIO 204.5f
//============================

//============================
#define MINIMUMHITRATIO 15
//============================

//============================
#define MAXIMUMHITRATIO	99
//============================

//============================
#define PLAYERMAXLEVEL	205
//============================

//============================
#define GMGMANACONSUMEUNIT	15
//============================

#define MAXCONSTRUCTIONPOINT 30000

#define MAXWARCONTRIBUTION	 500000

#define MAXPARTYNUM			200
//#define MAXARENAPLAYERS     200

#define ALLSIDES			100
#define ATTACKER			101
#define DEFENDER			102

//v2.19 2002-11-19
#define PK					0
#define NONPK				1
#define NEVERNONPK			2

#define MAX_CRUSADESUMMONMOB	100

#define CRUSADELOG_ENDHELDENIAN		11
#define CRUSADELOG_STARTHELDENIAN	12
#define CRUSADELOG_GETHELDENIANEXP	14
#define CRUSADELOG_HELDENIANVICTORY	15
#define CRUSADELOG_HELDENIANWARNOW	16

#define MAX_HELDENIANTOWER			200
#define MAXHELDENIAN				10
#define MAXHELDENIANSUMMONPOINT		12000
#ifdef RestatScroll
#define STR 0x01 //Correct
#define DEX 0x02 //Correct
#define INT	0x03 //Correct
#define VIT 0x04 //Correct
#define MAG	0x05 //Correct
#define CHR 0x06 //Correct
#endif

#define DEF_SOCKETTHREADS 2
#pragma endregion


#define MAXWAYPOINTS			10

#define MOVETYPE_STOP			0
#define MOVETYPE_SEQWAYPOINT	1
#define MOVETYPE_RANDOMWAYPOINT	2
#define MOVETYPE_FOLLOW			3
#define MOVETYPE_RANDOMAREA		4
#define MOVETYPE_RANDOM			5
#define MOVETYPE_GUARD			6

#define BEHAVIOR_STOP			0
#define BEHAVIOR_MOVE			1
#define BEHAVIOR_ATTACK			2
#define BEHAVIOR_FLEE			3
#define BEHAVIOR_DEAD			4

// for Client Data
#define MAXTELEPORTLOC		200
#define MAXWAYPOINTCFG		200
#define MAXMGAR				50
#define MAXNMR				50
#define MAXSPOTMOBGENERATOR	100
#define MAXFISHPOINT		200
#define MAXMINERALPOINT		200
#define MAXOCCUPYFLAG		20001 //3001
#define MAXINITIALPOINT		20

#define MAPTYPE_NORMAL				0
#define MAPTYPE_NOPENALTY_NOREWARD	1
#define MAXENERGYSPHERES			10

#define MAXSTRATEGICPOINTS			200
#define MAXSECTORS					60
#define MAXSTRIKEPOINTS				20

#define MAXITEMEVENTS				50

#define MAXHELDENIANDOOR	10 
#define MAXDYNAMICGATES		10
#define MAXHELDENIANTOWER	200

// for Client Data
#define DEF_MAPDATASIZEX		30
#define DEF_MAPDATASIZEY		25
#define DEF_MAXTELEPORTLOC		200
#define DEF_MAXWAYPOINTCFG		200
#define DEF_MAXMGAR				50
#define DEF_MAXNMR				50
#define DEF_MAXSPOTMOBGENERATOR	100
#define DEF_MAXFISHPOINT		200
#define DEF_MAXMINERALPOINT		200
#define DEF_MAXOCCUPYFLAG		20001 //3001
#define	DEF_MAXINITIALPOINT		20


#define DEF_MAPTYPE_NORMAL				0
#define DEF_MAPTYPE_NOPENALTY_NOREWARD	1
#define DEF_MAXENERGYSPHERES			10

#define DEF_MAXSTRATEGICPOINTS			200
#define DEF_MAXSECTORS					60
#define DEF_MAXSTRIKEPOINTS				20

#define DEF_MAXITEMEVENTS				50

#define DEF_MAXSPRITES			55000
#define DEF_MAXTILES			700
#define DEF_MAXEFFECTSPR		200
#define DEF_MAXSOUNDEFFECTS		300
#define DEF_MAXCHATMSGS			500
#define DEF_MAXWHISPERMSG		5
#define DEF_MAXCHATSCROLLMSGS	80
#define DEF_MAXEFFECTS			300
#define DEF_CHATTIMEOUT_A		4000
#define DEF_CHATTIMEOUT_B		500
#define DEF_CHATTIMEOUT_C		2000
#define DEF_TEXTDLGMAXLINES		300

#define DEF_CLIENTSOCKETBLOCKLIMIT	15

#define DEF_MSGBUFFERSIZE	30000
#define DEF_MAXITEMS		50
#define DEF_MAXBANKITEMS	120
#define DEF_MAXGUILDSMAN	128
#define DEF_MAXMENUITEMS		140

#define	DEF_MAXMAGICTYPE	100
#define DEF_MAXSKILLTYPE	60

#define DEF_MAXTELEPORTLIST	20

#define DEF_MAXPARTYMEMBERS	8

#define DEF_SPECABLTYTIMESEC	1200

#define DEF_BTNSZX				74
#define DEF_BTNSZY				20
#define DEF_LBTNPOSX			30
#define DEF_RBTNPOSX			154
#define DEF_BTNPOSY				292

#define DEF_INDEX4_MSGID		0
#define DEF_INDEX2_MSGTYPE		4

#define DEF_MAXWEATHEROBJECTS	600
#define DEF_MAXBUILDITEMS		100

#define DEF_MAXGAMEMSGS			300
#define DEF_MAXITEMNAMES		1000

#define DEF_MAXGUILDNAMES		100

#define DEF_MAXSELLLIST			12

#define DEF_GAMEMODE_NULL					-2
#define DEF_GAMEMODE_ONQUIT					-1
#define DEF_GAMEMODE_ONMAINMENU				0
#define DEF_GAMEMODE_ONCONNECTING			1
#define DEF_GAMEMODE_ONLOADING				2
#define DEF_GAMEMODE_ONWAITINGINITDATA		3
#define DEF_GAMEMODE_ONMAINGAME				4
#define DEF_GAMEMODE_ONCONNECTIONLOST		5
#define DEF_GAMEMODE_ONMSG					6
#define DEF_GAMEMODE_ONCREATENEWACCOUNT		7
#define DEF_GAMEMODE_ONLOGIN				8
#define DEF_GAMEMODE_ONQUERYFORCELOGIN		9
#define DEF_GAMEMODE_ONSELECTCHARACTER		10
#define DEF_GAMEMODE_ONCREATENEWCHARACTER	11
#define DEF_GAMEMODE_ONWAITINGRESPONSE		12
#define DEF_GAMEMODE_ONQUERYDELETECHARACTER 13
#define DEF_GAMEMODE_ONLOGRESMSG			14
#define DEF_GAMEMODE_ONVERSIONNOTMATCH		17
#define DEF_GAMEMODE_ONINTRODUCTION			18// todo: needed?
#define DEF_GAMEMODE_ONSELECTSERVER			20

#define DEF_SERVERTYPE_GAME			1
#define DEF_SERVERTYPE_LOG			2

#define DEF_CURSORSTATUS_NULL		0
#define DEF_CURSORSTATUS_PRESSED	1
#define DEF_CURSORSTATUS_SELECTED	2
#define DEF_CURSORSTATUS_DRAGGING	3

#define DEF_SELECTEDOBJTYPE_DLGBOX	1
#define DEF_SELECTEDOBJTYPE_ITEM	2

#define DEF_DOUBLECLICKTIME			300
#define DEF_MAXPARTYMEMBERS			8
#define DEF_MAXCRUSADESTRUCTURES	300

#define DEF_MAXMAGICEFFECTS			100



#define MAXMAGICEFFECTS			40




#define DEF_MAXWAYPOINTS			10



#define DEF_ATTRIBUTE_EARTH			1
#define DEF_ATTRIBUTE_AIR			2
#define DEF_ATTRIBUTE_FIRE			3
#define DEF_ATTRIBUTE_WATER			4

#include "net_messages.h"

//#define TOTALCHARACTERS		120
// todo - make sure this is what it should be
#define TOTALCHARACTERS		130
#define TOTALACTION			15

#define OBJECTSTOP			0
#define OBJECTMOVE			1
#define OBJECTRUN				2
#define OBJECTATTACK			3
#define OBJECTMAGIC			4
#define OBJECTGETITEM		5
#define OBJECTDAMAGE			6
#define OBJECTDAMAGEMOVE	7
#define OBJECTATTACKMOVE	8
#define OBJECTDYING			10			   
#define OBJECTNULLACTION	100
#define OBJECTDEAD			101


#define OBJECTMOVE_CONFIRM				1001
#define OBJECTMOVE_REJECT				1010
#define OBJECTMOTION_CONFIRM			1020
#define OBJECTMOTION_ATTACK_CONFIRM		1030
#define OBJECTMOTION_REJECT				1040

const char motion_to_actionid[MOTION_MAX] =
{
    OBJECTSTOP, OBJECTMOVE, OBJECTRUN, OBJECTATTACK, OBJECTMAGIC, OBJECTGETITEM, OBJECTDAMAGE,
    OBJECTDAMAGEMOVE, OBJECTATTACKMOVE, OBJECTDYING, OBJECTNULLACTION, OBJECTNULLACTION, OBJECTNULLACTION
};

#define GET_MOTION_ACTIONID(x)	( motion_to_actionid[x & ~MSGIDTYPE_MOTION] )


#define DEFAULTBAGSIZE	20
#define DEFAULTEXTRABAGSIZE	5
#define DEFAULTBANKSIZE 20
#define MAXITEMTYPES			2000
#define MAXNPCTYPES			200
#define MAXNPCTIERS			10
#define MAXPARTYMEMBERS		40
#define MAXBANKITEMS			120
#define MAXITEMS		75
#define MAXGUILDSMAN	128
#define MAXMAGICTYPE	100
#define MAXSKILLTYPE	25
#define MAXTELEPORTLIST	20

#define DEF_LIMITHUNTERLEVEL 100

#define SPECABLTYTIMESEC	1200



struct GuildUpgrade
{
    std::string name;
    GuildUpgrades type;
    uint8_t maxLvl;
    uint32_t costGold[5];
    uint32_t costMaj[5];
    uint32_t costCont[5];
};

const GuildUpgrade gldUps[int(GuildUpgrades::GU_MAX)] =
{
    {
        "Warehouse", GuildUpgrades::GU_WAREHOUSE, 4,
        {0, 200000, 500000, 1000000, 5000000},
        {0, 1000, 2000, 10000, 50000},
        {0, 1000, 3000, 8000, 20000},
    },
    {
        "Summons", GuildUpgrades::GU_SUMMONS, 4,
        {0, 200000, 500000, 1000000, 5000000},
        {0, 1000, 2000, 10000, 50000},
        {0, 1000, 3000, 8000, 20000},
    },
    {
        "Captaincy", GuildUpgrades::GU_CAPTAINCY, 4,
        {0, 100000, 400000, 1000000, 5000000},
        {0, 500, 1000, 8000, 20000},
        {0, 2000, 5000, 10000, 50000},
    },
    {
        "Raidmasters", GuildUpgrades::GU_RAIDMASTERS, 4,
        {0, 500000, 1000000, 3000000, 6000000},
        {0, 1500, 3000, 12000, 24000},
        {0, 4000, 8000, 24000, 48000},
    },
    {
        "Huntmasters", GuildUpgrades::GU_HUNTMASTERS, 4,
        {0, 500000, 1000000, 3000000, 6000000},
        {0, 1500, 3000, 12000, 24000},
        {0, 4000, 8000, 24000, 48000},
    },
};

const uint32_t maxGWHItems[5] = { 0, 75, 75 * 2, 75 * 3, 75 * 4 };




using UnitStatus = uint32_t;

//TODO: Make this customizable in game via UI?
struct GuildRankPermissions
{
    bool canDisband;
    bool canInvite;
    bool canBan;
    bool canSummon;
    bool canWithdrawWH;
    bool crusadeCommander;
    bool canPromote;
    bool canPing;
};

const GuildRankPermissions gldRankPerm[GUILDRANK_MAX] = {
    {true, true, true, true, true, true, true, true},		// GUILDRANK_MASTER
    {false, true, true, false, true, true, true, true},	// GUILDRANK_CAPTAIN
    {false, false, false, false, true, false, false, true},	// GUILDRANK_HUNTMASTER
    {false, false, false, false, true, false, false, true},	// GUILDRANK_RAIDMASTER
    {false, false, false, false, true, false, false, false},	// GUILDRANK_GUILDSMAN
    {false, false, false, false, false, false, false, false}	// GUILDRANK_RECRUIT
};

#define GUILDSTARTRANK		GUILDRANK_RECRUIT

//-----------------------------------------------------------------------------------------------------------------------


//----------------------------------Events-------------------------------------------------------------------------------------

#define RELICVICTORYTIME		(10 * 1000)


//static const char * eventName[] = { "", "Capture the Relic", "Destroy the Shield", "Crusade", "Apocalypse", "MonsterEvent"};
const std::string eventName[] = { "", "Capture the Relic", "Destroy the Shield", "Crusade", "Apocalypse", "MonsterEvent" };

struct Casualties
{
    uint32_t deaths;
    uint32_t kills;
};

//----------------------------------------------------------------------------------------------------------------------------------------



struct HeroItemCost
{
    uint32_t EK;
    uint32_t contribution;
};

const HeroItemCost heroPrice[HR_MAX] =
{
    {300,0},		// HR_CAPE
    {150,20},	// HR_HELM
    {100,20},	// HR_CAP
    {300,30},	// HR_PLATE
    {200,20},	// HR_ROBE
    {100,10},	// HR_HAUBERK
    {150,15}		// HR_LEGGINGS
};

static const uint32_t HeroItemID[HR_MAX][MAXSIDES - 1][2] =
{
    {	// HR_CAPE
        {400, 400},		// ARESDEN
        {401, 401},		// ELVINE
    },
    {	// HR_HELM
        {403, 404},		// ARESDEN
        {405, 406},		// ELVINE
    },
    {	// HR_CAP
        {407, 408},		// ARESDEN
        {409, 410},		// ELVINE
    },
    {	// HR_PLATE
        {411, 412},		// ARESDEN
        {413, 414},		// ELVINE
    },
    {	// HR_ROBE
        {415, 416},		// ARESDEN
        {417, 418},		// ELVINE
    },
    {	// HR_HAUBERK
        {419, 420},		// ARESDEN
        {421, 422},		// ELVINE
    },
    {	// HR_LEGGINGS
        {423, 424},		// ARESDEN
        {425, 426},		// ELVINE
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------

#define atoul(str)			strtoul(str, 0, 10)
#define atoull(str)			_strtoui64(str, 0, 10)
#define strcpyn(dst, src)	strncpy(dst, src, sizeof(dst))

//#define logb(n, b)	(log(n)/log(b))

struct Point
{
    int32_t x, y;

    bool operator == (Point p)
    {
        return (p.x == x && p.y == y) ? true : false;
    }

    bool operator != (Point p)
    {
        return !(p == *this);
    }

    Point operator ++ (int)
    {
        Point copy = *this;
        x++;
        y++;
        return copy;
    }

    Point operator -- (int)
    {
        Point copy = *this;
        x--;
        y--;
        return copy;
    }

    Point & operator += (const Point & p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point & operator -= (const Point & p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point operator + (const Point & p) const
    {
        Point copy = *this;
        return (copy += p);
    }

    Point operator - (const Point & p) const
    {
        Point copy = *this;
        return (copy -= p);
    }
};


#define OBJECTMOVE_CONFIRM				1001
#define OBJECTMOVE_REJECT				1010
#define OBJECTMOTION_CONFIRM			1020
#define OBJECTMOTION_ATTACK_CONFIRM		1030
#define OBJECTMOTION_REJECT				1040

#define HITRATIOFACTOR		80.0f
#define MAXSKILLPOINTS		3000

struct hbxrect
{
    hbxrect(int32_t a, int32_t b, int32_t c, int32_t d) { left = a; top = b; right = c; bottom = d; }
    hbxrect() { left = top = right = bottom = 0; };
    hbxrect(const hbxrect & r) { left = r.left; top = r.top; right = r.right; bottom = r.bottom; }
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct hbxpoint
{
    hbxpoint(int32_t a, int32_t b) { x = a; y = b; }
    hbxpoint() { x = y = 0; };
    int32_t  x;
    int32_t  y;
};


#define DYNAMICOBJECT_FIRE			1
#define DYNAMICOBJECT_FISH			2
#define DYNAMICOBJECT_FISHOBJECT	3
#define DYNAMICOBJECT_MINERAL1		4
#define DYNAMICOBJECT_MINERAL2		5
#define DYNAMICOBJECT_ARESDENFLAG	6
#define DYNAMICOBJECT_ELVINEFLAG	7
#define DYNAMICOBJECT_ICESTORM		8
#define DYNAMICOBJECT_SPIKE			9
#define DYNAMICOBJECT_PCLOUD_BEGIN	10
#define DYNAMICOBJECT_PCLOUD_LOOP	11
#define DYNAMICOBJECT_PCLOUD_END	12
#define DYNAMICOBJECT_FIRE2			13
#define DYNAMICOBJECT_FIRE3			14 //for FireBow




//#include <cmath>
//#include <math.h>

//TODO: need a new way to detect speed hacking and not "you can speed hack 10 steps then slow down"
#ifdef _DEBUG
#define SPEEDCHECKTURNS 1
#else
#define DOSPEEDCHECK
#define SPEEDCHECKTURNS 10
#endif

#define MAXCRUSADESTRUCTURES		300
#define MAXAGRICULTURE				200

#define PFMABSORBVAL					0.50

//TODO: time you can spend in enemy maps - needs some changing to the system of raiding
#define RAIDTIME				600

//TODO: should not be hard coded
#define GOLDDROPMULTIPLIER			2
#define EXPMULTIPLIER				355

//TODO: what are these and why do they exist hardcoded?
#define BISLEGOLDCOST				100
#define ARENAGOLDCOST				500


#define HITRATIOFACTOR		80.0f
#define MAXSKILLPOINTS		3000


#define SERVERSTATUS_STOPPED 1
#define SERVERSTATUS_STARTING 2
#define SERVERSTATUS_ONLINE 3
#define SERVERSTATUS_SHUTDOWN 4


#define ByteSwap(x) ByteSwap5((unsigned char *) &x, sizeof(x))


// todo - fix old shit str tokenizer
#if !defined(WIN32)
#define strtok_s strtok_r
#define _atoi64 atoll
#define sprintf_s snprintf
#define strcpy_s(a,b,c) strcpy(a,c)
#define SLEEP(a) { struct timespec req={0}; req.tv_sec = 0; req.tv_nsec = 1000000 * a; nanosleep(&req,0); }
#endif

#define DBL "%f"
#define DBL2 "f"
#define XI64 "%lld"


#define CLIENTSOCKETBLOCKLIMIT	15




#define FLAGRANGE_X		11
#define FLAGRANGE_Y		9



#define DELAYEVENTTYPE_DAMAGEOBJECT				1
#define DELAYEVENTTYPE_MAGICRELEASE				2
#define DELAYEVENTTYPE_USEITEM_SKILL			3
#define DELAYEVENTTYPE_METEORSTRIKE				4
#define DELAYEVENTTYPE_DOMETEORSTRIKEDAMAGE		5
#define DELAYEVENTTYPE_CALCMETEORSTRIKEEFFECT	6
#define DELAYEVENTTYPE_ANCIENT_TABLET			7

#define DELAYEVENTTYPE_END_APOCALYPSE			8
#define DELAYEVENTTYPE_KILL_ABADDON				9

#define DEFAULTINVENTORYSIZE 20
#define DEFAULTBANKSIZE 20

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_LEGGINGS	5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13

#define DEF_ITEMTYPE_NOTUSED	-1
#define DEF_ITEMTYPE_NONE		 0
#define DEF_ITEMTYPE_EQUIP		 1
#define DEF_ITEMTYPE_APPLY		 2
#define DEF_ITEMTYPE_USE_DEPLETE 3       
#define DEF_ITEMTYPE_INSTALL	 4
#define DEF_ITEMTYPE_CONSUME	 5
#define DEF_ITEMTYPE_ARROW		 6
#define DEF_ITEMTYPE_EAT		 7
#define DEF_ITEMTYPE_USE_SKILL   8
#define DEF_ITEMTYPE_USE_PERM    9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12


#define DEF_ITEMEFFECTTYPE_NONE				0
#define DEF_ITEMEFFECTTYPE_ATTACK			1
#define DEF_ITEMEFFECTTYPE_DEFENSE			2
#define DEF_ITEMEFFECTTYPE_ATTACK_ARROW		3
#define DEF_ITEMEFFECTTYPE_HP   		    4
#define DEF_ITEMEFFECTTYPE_MP   		    5
#define DEF_ITEMEFFECTTYPE_SP   		    6
#define DEF_ITEMEFFECTTYPE_HPSTOCK 		    7
#define DEF_ITEMEFFECTTYPE_GET			    8
#define DEF_ITEMEFFECTTYPE_STUDYSKILL		9
#define DEF_ITEMEFFECTTYPE_SHOWLOCATION		10
#define DEF_ITEMEFFECTTYPE_MAGIC			11
#define DEF_ITEMEFFECTTYPE_CHANGEATTR		12
#define DEF_ITEMEFFECTTYPE_ATTACK_MANASAVE	13
#define DEF_ITEMEFFECTTYPE_ADDEFFECT	    14
#define DEF_ITEMEFFECTTYPE_MAGICDAMAGESAVE	15 
#define DEF_ITEMEFFECTTYPE_OCCUPYFLAG		16
#define DEF_ITEMEFFECTTYPE_DYE				17
#define DEF_ITEMEFFECTTYPE_STUDYMAGIC		18
#define DEF_ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19
#define DEF_ITEMEFFECTTYPE_ATTACK_DEFENSE	20
#define DEF_ITEMEFFECTTYPE_MATERIAL_ATTR	21
#define DEF_ITEMEFFECTTYPE_FIRMSTAMINA		22
#define DEF_ITEMEFFECTTYPE_LOTTERY			23

#define DEF_ITEMEFFECTTYPE_ATTACK_SPECABLTY		24
#define DEF_ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25
#define DEF_ITEMEFFECTTYPE_ALTERITEMDROP		26

#define DEF_ITEMEFFECTTYPE_CONSTRUCTIONKIT		27
#define DEF_ITEMEFFECTTYPE_WARM					28
#define DEF_ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29
#define DEF_ITEMEFFECTTYPE_ITEMFARM				30

#define DEF_ITET_UNIQUE_OWNER				1
#define DEF_ITET_ID							2
#define DEF_ITET_DATE						3


#define AColor(a,r,g,b)          ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)|(((uint32_t)(uint8_t)(a))<<24)))


struct ItemStats2
{
    std::string desc;
    uint32_t mult;
};

static inline const ItemStats2 itemStats2[] = {
    { "", 0 },
    { "PSN", 7 },
    { "HIT", 7 },
    { "DF", 7 },
    { "HP", 7},
    { "SP", 7 },
    { "MP", 7 },
    { "MR", 7 },
    { "PA", 3 },
    { "MA", 3 },
    { "CAD", 1 },
    { "XP", 10 },
    { "GOLD", 10 },
    { "", 0 }
};

#define MAXITEMEQUIPPOS		20
#define EQUIPPOS_NONE		0
#define EQUIPPOS_HEAD		1
#define EQUIPPOS_BODY		2
#define EQUIPPOS_ARMS		3
#define EQUIPPOS_PANTS		4
#define EQUIPPOS_BOOTS		5
#define EQUIPPOS_NECK		6
#define EQUIPPOS_LHAND		7
#define EQUIPPOS_RHAND		8
#define EQUIPPOS_TWOHAND	9
#define EQUIPPOS_RFINGER	10
#define EQUIPPOS_LFINGER	11
#define EQUIPPOS_BACK		12
#define EQUIPPOS_FULLBODY	13     


#define ITEMEFFECTTYPE_NONE				0
#define ITEMEFFECTTYPE_ATTACK			1
#define ITEMEFFECTTYPE_DEFENSE			2
#define ITEMEFFECTTYPE_ATTACK_ARROW		3
#define ITEMEFFECTTYPE_HP   		    4
#define ITEMEFFECTTYPE_MP   		    5
#define ITEMEFFECTTYPE_SP   		    6
#define ITEMEFFECTTYPE_HPSTOCK 		    7
#define ITEMEFFECTTYPE_GET			    8
#define ITEMEFFECTTYPE_STUDYSKILL		9
#define ITEMEFFECTTYPE_SHOWLOCATION		10
#define ITEMEFFECTTYPE_MAGIC			11
#define ITEMEFFECTTYPE_CHANGEATTR		12
#define ITEMEFFECTTYPE_ATTACK_MANASAVE	13
#define ITEMEFFECTTYPE_ADDEFFECT	    14
#define ITEMEFFECTTYPE_MAGICDAMAGESAVE	15
#define ITEMEFFECTTYPE_OCCUPYFLAG		16
#define ITEMEFFECTTYPE_DYE				17
#define ITEMEFFECTTYPE_STUDYMAGIC		18
#define ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19
#define ITEMEFFECTTYPE_ATTACK_DEFENSE	20
#define ITEMEFFECTTYPE_MATERIAL_ATTR	21
#define ITEMEFFECTTYPE_FIRMSTAMINA		22
#define ITEMEFFECTTYPE_LOTTERY			23

#define ITEMEFFECTTYPE_ATTACK_SPECABLTY		24
#define ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25
#define ITEMEFFECTTYPE_ALTERITEMDROP		26

#define ITEMEFFECTTYPE_CONSTRUCTIONKIT		27
#define ITEMEFFECTTYPE_WARM					28
#define ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29
#define ITEMEFFECTTYPE_ITEMFARM				30  
#define ITEMEFFECTTYPE_SLATES			31
#define ITEMEFFECTTYPE_JEWELRY			32
#define ITEMEFFECTTYPE_SOCKET			33
#define ITEMEFFECTTYPE_VARIABLEADD			34
#define ITEMEFFECTTYPE_ATTACK_MAGICITEM		50 // Battle Mages
#define ITEMEFFECTTYPE_CRIT				51  // Critical Potion
#define ITEMEFFECTTYPE_ARMORDYE			52	// Armor Dye
#define ITEMEFFECTTYPE_WEAPONDYE		53	// Weapon Dye
#define ITEMEFFECTTYPE_ADDSKILLS		54  // New Skill % To Items
#define ITEMEFFECTTYPE_ADDLUCK			55  // Drop System Luck
#define ITEMEFFECTTYPE_ADDREP			56  // Rep Pots
//#define ITEMEFFECTTYPE_ADDBALLPOINTS	57	// Ball Points



#define ITET_UNIQUE_OWNER			1
#define ITET_ID						2
#define ITET_DATE						3
#define ITET_GM_MADE					4


#define SKILLEFFECTTYPE_GET			    1
#define SKILLEFFECTTYPE_PRETEND			2
#define SKILLEFFECTTYPE_TAMING			3

#define MAXMAPNAMES	92

static inline const char * mapNames[MAXMAPNAMES] = {
    /* 0 */  "NONE",
    /* 1 */	"arefarm",
    /* 2 */	"elvfarm",
    /* 3 */	"default",
    /* 4 */	"arebrk11",
    /* 5 */	"arebrk12",
    /* 6 */	"arebrk21",
    /* 7 */	"arebrk22",
    /* 8 */	"elvbrk11",
    /* 9 */	"elvbrk12",
    /* 10 */	"elvbrk21",
    /* 11 */	"elvbrk22",
    /* 12 */	"fightzone1",
    /* 13 */	"fightzone2",
    /* 14 */	"fightzone3",
    /* 15 */	"fightzone4",
    /* 16 */	"fightzone5",
    /* 17 */	"fightzone6",
    /* 18 */	"fightzone7",
    /* 19 */	"fightzone8",
    /* 20 */	"fightzone9",



    /* 21 */	"elvined1",
    /* 22 */	"aresden",
    /* 23 */	"aresdend1",
    /* 24 */	"elvine",
    /* 25 */	"dglv2",
    /* 26 */	"dglv3",
    /* 27 */	"bisle",
    /* 28 */	"middleland",
    /* 29 */	"huntzone1",
    /* 30 */	"huntzone2",
    /* 31 */	"huntzone3",
    /* 32 */	"huntzone4",
    /* 33 */	"bsmith_1",
    /* 34 */	"bsmith_2",
    /* 35 */	"cityhall_1",
    /* 36 */	"cityhall_2",
    /* 37 */	"gldhall_1",
    /* 38 */	"gldhall_2",
    /* 39 */	"gshop_1",
    /* 40 */	"gshop_2",
    /* 41 */	"wzdtwr_1",
    /* 42 */	"wzdtwr_2",
    /* 43 */	"wrhus_1",
    /* 44 */	"wrhus_2",
    /* 45 */	"arewrhus",
    /* 46 */	"elvwrhus",
    /* 47 */	"cath_1",
    /* 48 */	"cath_2",
    /* 49 */	"Aresden-Middle DMZ",
    /* 50 */	"Aresden-Middle DMZ",
    /* 51 */	"Elvine-Middle DMZ",
    /* 52 */	"Elvine-Middle DMZ",
    /* 53 */	"dglv4",
    /* 54 */	"resurr1",
    /* 55 */	"resurr2",
    /* 56 */	"areuni",
    /* 57 */	"elvuni",
    /* 58 */	"middled1x",
    /* 59 */	"middled1n",
    /* 60 */	"toh1",
    /* 61 */	"toh2",
    /* 62 */	"toh3",
    /* 63 */	"arejail",
    /* 64 */	"elvjail",
    /* 65 */	"2ndmiddle",
    /* 66 */	"icebound",

    /* 67 */	"Unknown",
    /* 68 */	"Unknown",
    /* 69 */	"Unknown",

    /* 70 */	"druncncity",
    /* 71 */	"inferniaA",
    /* 72 */	"inferniaB",
    /* 73 */	"maze",
    /* 74 */	"procella",
    /* 75 */	"abaddon",
    /* 76 */	"BtField",
    /* 77 */	"GodH",
    /* 78 */	"HRampart",
    /* 79 */	"CmdHall",
    /* 80 */	"Barracks1",
    /* 81 */	"Barracks2",
    /* 82 */	"Bsmith",
    /* 83 */	"default2",
    /* 84 */ "astoria",
    /* 85 */ "CmdHall_1",
    /* 86 */ "CmdHall_2",
    /* 87 */ "pvpbarrack",
    ///* 88 */ "darkcrypt",
    ///* 89 */ "pvpbar2",
    /* 88 */ "arebar",
    /* 89 */ "elvbar",
    /* 90 */ "catacombs",
    /* 91 */ "outlands"
};

#define GetMapName(i) ( ((uint32_t)i < MAXMAPNAMES) ? mapNames[i] : mapNames[0] )


#define DYNAMICOBJECT_FIRE			1
#define DYNAMICOBJECT_FISH			2
#define DYNAMICOBJECT_FISHOBJECT	3
#define DYNAMICOBJECT_MINERAL1		4
#define DYNAMICOBJECT_MINERAL2		5
#define DYNAMICOBJECT_ARESDENFLAG	6
#define DYNAMICOBJECT_ELVINEFLAG	7
#define DYNAMICOBJECT_ICESTORM		8
#define DYNAMICOBJECT_SPIKE			9
#define DYNAMICOBJECT_PCLOUD_BEGIN	10
#define DYNAMICOBJECT_PCLOUD_LOOP	11
#define DYNAMICOBJECT_PCLOUD_END	12
#define DYNAMICOBJECT_FIRE2			13
#define DYNAMICOBJECT_FIRE3			14 //for FireBow


#define EQUIPPOS_NONE		0
#define EQUIPPOS_HEAD		1
#define EQUIPPOS_BODY		2
#define EQUIPPOS_ARMS		3
#define EQUIPPOS_PANTS		4
#define EQUIPPOS_BOOTS		5
#define EQUIPPOS_NECK		6
#define EQUIPPOS_LHAND		7
#define EQUIPPOS_RHAND		8
#define EQUIPPOS_TWOHAND	9
#define EQUIPPOS_RFINGER	10
#define EQUIPPOS_LFINGER	11
#define EQUIPPOS_BACK		12
#define EQUIPPOS_FULLBODY	13     



#define ITEMEFFECTTYPE_NONE				0
#define ITEMEFFECTTYPE_ATTACK			1
#define ITEMEFFECTTYPE_DEFENSE			2
#define ITEMEFFECTTYPE_ATTACK_ARROW		3
#define ITEMEFFECTTYPE_HP   		    4
#define ITEMEFFECTTYPE_MP   		    5
#define ITEMEFFECTTYPE_SP   		    6
#define ITEMEFFECTTYPE_HPSTOCK 		    7
#define ITEMEFFECTTYPE_GET			    8
#define ITEMEFFECTTYPE_STUDYSKILL		9
#define ITEMEFFECTTYPE_SHOWLOCATION		10
#define ITEMEFFECTTYPE_MAGIC			11
#define ITEMEFFECTTYPE_CHANGEATTR		12
#define ITEMEFFECTTYPE_ATTACK_MANASAVE	13
#define ITEMEFFECTTYPE_ADDEFFECT	    14
#define ITEMEFFECTTYPE_MAGICDAMAGESAVE	15
#define ITEMEFFECTTYPE_OCCUPYFLAG		16
#define ITEMEFFECTTYPE_DYE				17
#define ITEMEFFECTTYPE_STUDYMAGIC		18
#define ITEMEFFECTTYPE_ATTACK_MAXHPDOWN	19
#define ITEMEFFECTTYPE_ATTACK_DEFENSE	20
#define ITEMEFFECTTYPE_MATERIAL_ATTR	21
#define ITEMEFFECTTYPE_FIRMSTAMINA		22
#define ITEMEFFECTTYPE_LOTTERY			23

#define ITEMEFFECTTYPE_ATTACK_SPECABLTY		24
#define ITEMEFFECTTYPE_DEFENSE_SPECABLTY	25
#define ITEMEFFECTTYPE_ALTERITEMDROP		26

#define ITEMEFFECTTYPE_CONSTRUCTIONKIT		27
#define ITEMEFFECTTYPE_WARM					28
#define ITEMEFFECTTYPE_DEFENSE_ANTIMINE		29
#define ITEMEFFECTTYPE_ITEMFARM				30  
#define ITEMEFFECTTYPE_SLATES			31
#define ITEMEFFECTTYPE_JEWELRY			32
#define ITEMEFFECTTYPE_SOCKET			33
#define ITEMEFFECTTYPE_VARIABLEADD			34

#define ITET_UNIQUE_OWNER			1
#define ITET_ID						2
#define ITET_DATE						3
#define ITET_GM_MADE					4




#pragma region movedirection
//#ifndef 800x600
static inline const int _tmp_iMoveLocX[9][44] = {
    // 0
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        // 1 NORTH
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, -1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //2
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 24, 24, 24, 24, 24,
        24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, -1 },//done
        //3 EAST
        { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
        24, 24, 24, 24, 24, 24, 24, 24, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //4
        { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
        24, 24, 24, 24, 24, 24, 24, 24, 24, 23, 22, 21,
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1 },//done
        //5 SOUTH
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, -1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //6
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
        3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1 },//done
        //7 WEST
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //8
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3,
        4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, -1 }//done
};

static inline const int _tmp_iMoveLocY[9][44] = {
    // 0
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //1 NORTH
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //2
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,
        7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 - 1 },//done
        //3 EAST
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //4
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 18,
        18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
        18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, -1 },//done
        //5 SOUTH
        { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
        18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
        18, 18, 18, 18, 18, -1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        //6
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 18,
        18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
        18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, -1 },//done
        //7 WEST
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, -1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//done
        // 8
        { 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
        7, 6, 5, 4, 3, 2, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1 }//done
};

static inline const char _tmp_cTmpDirX[9] = { 0, 0, 1, 1, 1, 0, -1, -1, -1 };
static inline const char _tmp_cTmpDirY[9] = { 0, -1, -1, 0, 1, 1, 1, 0, -1 };

static inline const char _tmp_cEmptyPosX[] = { 0, 1, 1, 0, -1, -1, -1, 0, 1, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2 };
static inline const char _tmp_cEmptyPosY[] = { 0, 0, 1, 1, 1, 0, -1, -1, -1, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2 };
#pragma endregion

extern int64_t unixtime();
extern int32_t unixseconds();

#ifndef WIN32
typedef struct tagRECT
{
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} RECT, * LPRECT;

void SetRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom)
{
    lprc->left = xLeft;
    lprc->top = yTop;
    lprc->right = xRight;
    lprc->bottom = yBottom;
}
#endif



#define ITEMS_PER_TILE	12
#define MAXGROUNDITEMS	5000

// todo - do times better with stdlib
#define TILECLEANTIME		MINUTE(15)
#define TILECLEANTIMEPLAYER	MINUTE(30)

#define SPECABLTYTIMESEC	1200


#define SEND_MAIL_COST			300
#define SEND_MAIL_ITEM_COST	700
#define GUILDBOARD_POST_COST	500
#define MAX_MAIL_MSG_LENGTH	400+1
#define MAX_MAIL_ITEMS			10

#define MAXITEMSOCKETS		4

// ----------------------------------Guild-------------------------------------------------------------------------------------
#define MAXGUILDBANKITEMS		75*4
#define GUILDSUMMONSGOLDCOST	5000
#define GUILDSUMMONSTIME		MINUTE(5)

//-----------------------------------------------------------------------------------------------------------------------

#define GetBoolText(val)	( val ? "true" : "false" )


//----------------------------------------------- TYPEDEFS ----------------------------------------------------------------------------------

//typedef std::map<int16_t, CGuild*> GuildMap;
//typedef GuildMap::iterator GuildMapIter;
//typedef GuildMap::const_iterator GuildMapCIter;
//typedef std::shared_ptr<CAstoria> Astoria;

//typedef std::list<CClient*> ClientList;
//typedef ClientList::iterator ClientListIter;
//typedef ClientList::const_iterator ClientListCIter;

using CharID = uint32_t;

struct owner_data
{
    uint64_t id;
    int32_t x;
    int32_t y;
    owner_type type;
    int8_t direction;
    int16_t appr1;
    int16_t appr2;
    int16_t appr3;
    int16_t appr4;
    uint32_t appr_color; // todo - Color?
    int16_t head_appr;
    int16_t body_appr;
    int16_t arm_appr;
    int16_t leg_appr;
    int32_t status;
    std::string name;
    int16_t action;
    int16_t frame;
    int32_t chat_index;
    int16_t v1;
    int16_t v2;
    int16_t v3;
    int32_t previous_location;
    int16_t sprite;
    int16_t sprite_frame;
    int16_t character_index; // ??
    int16_t dynamic_object_type;
    std::chrono::time_point<std::chrono::steady_clock> last_seen_time;
};

// #define DEF_NPC			46220
// #define DEF_MUNDIES		30000
// #define DEF_MHAIR		31000
// #define DEF_MARMOR		25075
// #define DEF_MCLOTHES	5315
// #define DEF_MLEGS		5555
// #define DEF_MSHOES		5795
// #define DEF_MWEAPONS	6020
// #define DEF_MSHIELDS	11100
// #define DEF_MCAPES		29230
// #define DEF_MHELMS		9300
// 
// #define DEF_WUNDIES		14580
// #define DEF_WHAIR		14820
// #define DEF_WARMOR		15060
// #define DEF_WCLOTHES	15300
// #define DEF_WLEGS		15540
// #define DEF_WSHOES		15780
// #define DEF_WWEAPONS	16020
// #define DEF_WSHIELDS	19100
// #define DEF_WCAPES		19230
// #define DEF_WHELMS		20300

#define DEF_XSOCKEVENT_SOCKETMISMATCH			-121
#define DEF_XSOCKEVENT_CONNECTIONESTABLISH		-122
#define DEF_XSOCKEVENT_RETRYINGCONNECTION		-123
#define DEF_XSOCKEVENT_ONREAD					-124
#define DEF_XSOCKEVENT_READCOMPLETE				-125
#define DEF_XSOCKEVENT_UNKNOWN					-126
#define DEF_XSOCKEVENT_SOCKETCLOSED				-127
#define DEF_XSOCKEVENT_BLOCK					-128
#define DEF_XSOCKEVENT_SOCKETERROR				-129
#define DEF_XSOCKEVENT_CRITICALERROR			-130
#define DEF_XSOCKEVENT_NOTINITIALIZED			-131
#define DEF_XSOCKEVENT_MSGSIZETOOLARGE			-132
#define DEF_XSOCKEVENT_CONFIRMCODENOTMATCH		-133
#define DEF_XSOCKEVENT_QUENEFULL                -134
#define DEF_XSOCKEVENT_UNSENTDATASENDBLOCK		-135
#define DEF_XSOCKEVENT_UNSENTDATASENDCOMPLETE	-136
