//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include <windows.h>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <vector>
#include <functional>
#include <queue>
#include <process.h>
#include <direct.h>
#include <set>
#include <deque>
#include <chrono>
#include <queue>
#include <atomic>
#include <future>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>

#include "Game.h"
#include "lan_eng.h"
#include "sprite.h"
#include "sprite_id.h"
#include "str_tok.h"
#include "Msg.h"
#include "Effect.h"
#include "MapData.h"
#include "action_id.h"
#include "NetMessages.h"
#include "MouseInterface.h"
#include "CharInfo.h"
#include "Item.h"
#include "Magic.h"
#include "Skill.h"
#include "dynamic_object_id.h"
#include "BuildItem.h"
#include "ItemName.h"

int G_iStuff = 0;
int64_t G_iBytesOut, G_iBytesIn;//Change 0x0107
int64_t G_iBytesOutLast = 0, G_iBytesInLast = 0;

extern char G_cSpriteAlphaDegree;

char G_cFile[100];

char _cDrawingOrder[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1 };
char _cMantleDrawingOrder[] = { 0, 1, 1, 1, 0, 0, 0, 2, 2 };
char _cMantleDrawingOrderOnRun[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };


short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
int _tmp_iStatus;
char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
uint16_t  _tmp_wObjectID;
char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
uint16_t  wFocusObjectID;
short sFocus_dX, sFocus_dY;
char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
int iFocusStatus;
int   iFocusApprColor;

void CGame::ReadSettings()
{

}

void CGame::WriteSettings()
{

}

CGame::CGame()
{
#if defined(WIN32)
    ix::initNetSystem();
#endif

    socket_mode(0);
    oldmode = 0;

    int i;
    srand((unsigned)time(0));
    ReadSettings();

    spdlog::init_thread_pool(8192, 2);

    std::vector<spdlog::sink_ptr> sinks;

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    sinks.push_back(stdout_sink);

    // daily rotation max 30 days
    auto rotating = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/helbreath.log", 0, 0, false, 30);
    sinks.push_back(rotating);

    log = std::make_shared<spdlog::async_logger>("helbreath", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);

    log->set_pattern(log_formatting);
    log->set_level(loglevel);

    m_stMCursor.sCursorFrame = 0;

    loggedin = false;

#ifdef _DEBUG
    m_bToggleScreen = true;
#else
    m_bToggleScreen = false;
#endif
    m_bShowFPS = true;
    m_cDetailLevel = 2;
    m_cLoading = 0;
    m_bZoomMap = true;
    m_bIsFirstConn = true;


    m_iItemDropCnt = 0;
    m_bItemDrop = false;

    m_bIsSpecial = false;

    m_cGameMode = DEF_GAMEMODE_ONLOADING;
    m_cWhisperIndex = DEF_MAXWHISPERMSG;
    m_cGameModeCount = 0;

    memset(m_cMapName, 0, sizeof(m_cMapName));

    m_pMapData = 0;

    m_cCommandCount = 0;
    m_dwCommandTime = 0;

    m_iFeedBackCardIndex = -1;

    m_sPlayerX = 0;
    m_sPlayerY = 0;

    m_sViewDX = 0;
    m_sViewDY = 0;

    m_cCommand = DEF_OBJECTSTOP;
    m_bIsObserverMode = false;

    for (i = 0; i < DEF_MAXSPRITES; i++) m_pSprite[i] = 0;
    for (i = 0; i < DEF_MAXTILES; i++) m_pTileSpr[i] = 0;
    for (i = 0; i < DEF_MAXEFFECTSPR; i++) m_pEffectSpr[i] = 0;

    for (i = 0; i < DEF_MAXCHATMSGS; i++) m_pChatMsgList[i] = 0;

    for (i = 0; i < DEF_MAXEFFECTS; i++) m_pEffectList[i] = 0;

    for (i = 0; i < DEF_MAXITEMS; i++) m_pItemList[i] = 0;

    for (i = 0; i < DEF_MAXBANKITEMS; i++) m_pBankList[i] = 0;

    for (i = 0; i < 4; i++) m_pCharList[i] = 0;

    for (i = 0; i < 101; i++) m_cDialogBoxOrder[i] = 0;

    for (i = 0; i < DEF_MAXMAGICTYPE; i++) m_pMagicCfgList[i] = 0;

    for (i = 0; i < DEF_MAXSKILLTYPE; i++) m_pSkillCfgList[i] = 0;

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        m_pMsgTextList[i] = 0;
        m_pMsgTextList2[i] = 0;
    }

    for (i = 0; i < DEF_MAXBUILDITEMS; i++) m_pBuildItemList[i] = 0;

    for (i = 0; i < DEF_MAXBUILDITEMS; i++) m_pDispBuildItemList[i] = 0;

    for (i = 0; i < DEF_MAXGAMEMSGS; i++) m_pGameMsgList[i] = 0;

    m_pExID = 0;

    for (i = 0; i < DEF_MAXITEMNAMES; i++) m_pItemNameList[i] = 0;

    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
    m_stMCursor.dwSelectClickTime = 0;

    memset(m_cLogServerAddr, 0, sizeof(m_cLogServerAddr));

    for (i = 0; i < DEF_MAXMENUITEMS; i++)
        m_pItemForSaleList[i] = 0;

    m_bCtrlPressed = false;
    m_bShiftPressed = false;
    m_bEnterPressed = false;
    m_bEscPressed = false;
    m_altPressed = false;

    m_bSoundFlag = false;
    m_dwDialogCloseTime = 0;

    m_iTimeLeftSecAccount = 0;
    m_iTimeLeftSecIP = 0;

    m_bWhisper = true;
    m_bShout = true;

    //Change Added \/\/
    m_stConfigList.bDebugNpc = false;
    m_stConfigList.bDebugStatus = false;
    m_stConfigList.bFastCast = true;
    m_stConfigList.bCameraShake = true;
    m_stConfigList.dwFastCast = 0;
    m_stConfigList.bShowNames = false;
    m_stConfigList.bMouseTileDebug = true;
    m_stConfigList.bGMChat = true;
    m_iStatLimit = 330;//200;
    m_dwPotCheckTime = unixtime();
    m_iTotalUsers = 0;
    m_iPing = 0;

#ifdef DEF_ADMINCLIENT
    memset(m_cAdminPassword, 0, sizeof(m_cAdminPassword));
    memcpy(m_cRefreshRate, "3000", 4);
#endif

    //Heldenian
    m_iAresdenDeathToll = m_iElvineDeathToll = m_iAresdenLeftBuildings = m_iElvineLeftBuildings = 0;
    m_bIsHeldenianMode = 0;

    m_iGateFrame = m_iGatePositX = m_iGatePositY = 0;
    memset(m_cGateMapName, 0, 12);

    memset(m_cEdit, 0, sizeof(m_cEdit));
}

CGame::~CGame()
{

}

void CGame::init_dialogs()
{
    //Character-Info Dialog(F5)
    m_stDialogBoxInfo[1].sX = 30;
    m_stDialogBoxInfo[1].sY = 30;
    m_stDialogBoxInfo[1].sSizeX = 270;
    m_stDialogBoxInfo[1].sSizeY = 376;

    //Inventory Dialog(F6)
    m_stDialogBoxInfo[2].sX = 380;
    m_stDialogBoxInfo[2].sY = 210;
    m_stDialogBoxInfo[2].sSizeX = 225;
    m_stDialogBoxInfo[2].sSizeY = 185;

    //Magic Circle Dialog(F7)
    m_stDialogBoxInfo[3].sX = 337;
    m_stDialogBoxInfo[3].sY = 57;
    m_stDialogBoxInfo[3].sSizeX = 258;//280;
    m_stDialogBoxInfo[3].sSizeY = 328;//346;


    m_stDialogBoxInfo[4].sX = 0;
    m_stDialogBoxInfo[4].sY = 0;
    m_stDialogBoxInfo[4].sSizeX = 270;
    m_stDialogBoxInfo[4].sSizeY = 105;
#ifdef DEF_FEEDBACKCARD
    m_stDialogBoxInfo[5].sX = 0;
    m_stDialogBoxInfo[5].sY = 0;
    m_stDialogBoxInfo[5].sSizeX = 320;
    m_stDialogBoxInfo[5].sSizeY = 320;
#else
    m_stDialogBoxInfo[5].sX = 0;
    m_stDialogBoxInfo[5].sY = 0;
    m_stDialogBoxInfo[5].sSizeX = 310;
    m_stDialogBoxInfo[5].sSizeY = 170;
#endif
    m_stDialogBoxInfo[6].sX = 0;
    m_stDialogBoxInfo[6].sY = 0;
    m_stDialogBoxInfo[6].sSizeX = 310;
    m_stDialogBoxInfo[6].sSizeY = 170;

    //Guild Menu Dialog
    m_stDialogBoxInfo[7].sX = 337;
    m_stDialogBoxInfo[7].sY = 57;
    m_stDialogBoxInfo[7].sSizeX = 258;
    m_stDialogBoxInfo[7].sSizeY = 339;

    //Guild Operation Dialog
    m_stDialogBoxInfo[8].sX = 337;
    m_stDialogBoxInfo[8].sY = 57;
    m_stDialogBoxInfo[8].sSizeX = 295;
    m_stDialogBoxInfo[8].sSizeY = 346;

    //Guide Map Dialog
    m_stDialogBoxInfo[9].sX = get_virtual_width() - 128;
    m_stDialogBoxInfo[9].sY = 0;
    m_stDialogBoxInfo[9].sSizeX = 128;
    m_stDialogBoxInfo[9].sSizeY = 128;

    //Chatting History Dialog(F9)
    m_stDialogBoxInfo[10].sX = 135;
    m_stDialogBoxInfo[10].sY = 273;
    m_stDialogBoxInfo[10].sSizeX = 364;
    m_stDialogBoxInfo[10].sSizeY = 162;

    //Sale Menu Dialog
    m_stDialogBoxInfo[11].sX = 70;
    m_stDialogBoxInfo[11].sY = 50;
    m_stDialogBoxInfo[11].sSizeX = 258;
    m_stDialogBoxInfo[11].sSizeY = 339;

    //Level-Up Setting Dialog
    m_stDialogBoxInfo[12].sX = 0;
    m_stDialogBoxInfo[12].sY = 0;
    m_stDialogBoxInfo[12].sSizeX = 258;
    m_stDialogBoxInfo[12].sSizeY = 339;

    //City Hall Menu Dialog
    m_stDialogBoxInfo[13].sX = 337;
    m_stDialogBoxInfo[13].sY = 57;
    m_stDialogBoxInfo[13].sSizeX = 258;
    m_stDialogBoxInfo[13].sSizeY = 339;
    m_stDialogBoxInfo[14].sX = 60; //337
    m_stDialogBoxInfo[14].sY = 50;
    m_stDialogBoxInfo[14].sSizeX = 258;
    m_stDialogBoxInfo[14].sSizeY = 339;
    m_stDialogBoxInfo[14].sV1 = 13;

    //Skill Menu(F8)
    m_stDialogBoxInfo[15].sX = 337;
    m_stDialogBoxInfo[15].sY = 57;
    m_stDialogBoxInfo[15].sSizeX = 258;
    m_stDialogBoxInfo[15].sSizeY = 339;

    //Magic Shop Menu
    m_stDialogBoxInfo[16].sX = 30;
    m_stDialogBoxInfo[16].sY = 30;
    m_stDialogBoxInfo[16].sSizeX = 304;
    m_stDialogBoxInfo[16].sSizeY = 328;


    m_stDialogBoxInfo[17].sX = 0;
    m_stDialogBoxInfo[17].sY = 0;
    m_stDialogBoxInfo[17].sSizeX = 215;
    m_stDialogBoxInfo[17].sSizeY = 87;


    m_stDialogBoxInfo[18].sX = 5;
    m_stDialogBoxInfo[18].sY = 65;
    m_stDialogBoxInfo[18].sSizeX = 258;
    m_stDialogBoxInfo[18].sSizeY = 339;

    //System Menu Dialog(F12)
    m_stDialogBoxInfo[19].sX = 337;
    m_stDialogBoxInfo[19].sY = 107;
    m_stDialogBoxInfo[19].sSizeX = 258;//270; //v2.18
    m_stDialogBoxInfo[19].sSizeY = 268;//346;

    //NpcActionQuery Dialog
    m_stDialogBoxInfo[20].sX = 237;
    m_stDialogBoxInfo[20].sY = 57;
    m_stDialogBoxInfo[20].sSizeX = 252;
    m_stDialogBoxInfo[20].sSizeY = 87;

    //NpcTalk Dialog
    m_stDialogBoxInfo[21].sX = 337;
    m_stDialogBoxInfo[21].sY = 57;
    m_stDialogBoxInfo[21].sSizeX = 258;
    m_stDialogBoxInfo[21].sSizeY = 339;

    //Map
    m_stDialogBoxInfo[22].sX = 336;
    m_stDialogBoxInfo[22].sY = 88;
    m_stDialogBoxInfo[22].sSizeX = 270;
    m_stDialogBoxInfo[22].sSizeY = 346;

    //ItemSellorRepair Dialog
    m_stDialogBoxInfo[23].sX = 337;
    m_stDialogBoxInfo[23].sY = 57;
    m_stDialogBoxInfo[23].sSizeX = 258;
    m_stDialogBoxInfo[23].sSizeY = 339;

    //Fishing Dialog
    m_stDialogBoxInfo[24].sX = 193;
    m_stDialogBoxInfo[24].sY = 241;
    m_stDialogBoxInfo[24].sSizeX = 263;
    m_stDialogBoxInfo[24].sSizeY = 100;

    //Noticement Dialog
    m_stDialogBoxInfo[25].sX = 162;
    m_stDialogBoxInfo[25].sY = 40;
    m_stDialogBoxInfo[25].sSizeX = 315;
    m_stDialogBoxInfo[25].sSizeY = 171;

    //Manufacture Dialog
    m_stDialogBoxInfo[26].sX = 100;
    m_stDialogBoxInfo[26].sY = 60;
    m_stDialogBoxInfo[26].sSizeX = 258;
    m_stDialogBoxInfo[26].sSizeY = 339;

    //Exchange Dialog
    m_stDialogBoxInfo[27].sX = 100;
    m_stDialogBoxInfo[27].sY = 30;
    m_stDialogBoxInfo[27].sSizeX = 520;
    m_stDialogBoxInfo[27].sSizeY = 357;

    //Quest Dialog
    m_stDialogBoxInfo[28].sX = 0;
    m_stDialogBoxInfo[28].sY = 0;
    m_stDialogBoxInfo[28].sSizeX = 258;
    m_stDialogBoxInfo[28].sSizeY = 339;

    //Gauge Panel
    m_stDialogBoxInfo[29].sX = 0;
    m_stDialogBoxInfo[29].sY = 434;
    m_stDialogBoxInfo[29].sSizeX = 157;
    m_stDialogBoxInfo[29].sSizeY = 53;

    //Icon Panel
    m_stDialogBoxInfo[30].sX = (get_virtual_width() - 640) / 2;
    m_stDialogBoxInfo[30].sY = get_virtual_height() - 53;
    m_stDialogBoxInfo[30].sSizeX = 640;
    m_stDialogBoxInfo[30].sSizeY = 53;

    //Sell List Dialog
    m_stDialogBoxInfo[31].sX = 170;
    m_stDialogBoxInfo[31].sY = 70;
    m_stDialogBoxInfo[31].sSizeX = 258;
    m_stDialogBoxInfo[31].sSizeY = 339;

    //Party Dialog
    m_stDialogBoxInfo[32].sX = 0;
    m_stDialogBoxInfo[32].sY = 0;
    m_stDialogBoxInfo[32].sSizeX = 258;
    m_stDialogBoxInfo[32].sSizeY = 339;

    //Crusade Job Dialog
    m_stDialogBoxInfo[33].sX = 360;
    m_stDialogBoxInfo[33].sY = 65;
    m_stDialogBoxInfo[33].sSizeX = 258;
    m_stDialogBoxInfo[33].sSizeY = 339;

    //Item Upgrade Dialog
    m_stDialogBoxInfo[34].sX = 60;
    m_stDialogBoxInfo[34].sY = 50;
    m_stDialogBoxInfo[34].sSizeX = 258;
    m_stDialogBoxInfo[34].sSizeY = 339;

    //Help Menu Dialog(F1)
    m_stDialogBoxInfo[35].sX = 358;
    m_stDialogBoxInfo[35].sY = 65;
    m_stDialogBoxInfo[35].sSizeX = 258;
    m_stDialogBoxInfo[35].sSizeY = 339;

    //Crusade Commander Dialog
    m_stDialogBoxInfo[36].sX = 20;
    m_stDialogBoxInfo[36].sY = 20;
    m_stDialogBoxInfo[36].sSizeX = 310;
    m_stDialogBoxInfo[36].sSizeY = 386;

    //Crusade Constructor Dialog
    m_stDialogBoxInfo[37].sX = 20;
    m_stDialogBoxInfo[37].sY = 20;
    m_stDialogBoxInfo[37].sSizeX = 310;
    m_stDialogBoxInfo[37].sSizeY = 386;

    //Crusade Soldier Dialog
    m_stDialogBoxInfo[38].sX = 20;
    m_stDialogBoxInfo[38].sY = 20;
    m_stDialogBoxInfo[38].sSizeX = 310;
    m_stDialogBoxInfo[38].sSizeY = 386;
    m_stDialogBoxInfo[39].sX = 0;
    m_stDialogBoxInfo[39].sY = 0;
    m_stDialogBoxInfo[39].sSizeX = 291;
    m_stDialogBoxInfo[39].sSizeY = 413;

    // 3.51 Slates Dialog
    m_stDialogBoxInfo[40].sX = 100;
    m_stDialogBoxInfo[40].sY = 60;
    m_stDialogBoxInfo[40].sSizeX = 258;
    m_stDialogBoxInfo[40].sSizeY = 339;

    // Item exchange confirmation
    m_stDialogBoxInfo[41].sX = 285;
    m_stDialogBoxInfo[41].sY = 200;
    m_stDialogBoxInfo[41].sSizeX = 270;
    m_stDialogBoxInfo[41].sSizeY = 105;

    // MJ Stats Change DialogBox
    m_stDialogBoxInfo[42].sX = 0;
    m_stDialogBoxInfo[42].sY = 0;
    m_stDialogBoxInfo[42].sSizeX = 258;
    m_stDialogBoxInfo[42].sSizeY = 339;

    // Resurection
    m_stDialogBoxInfo[50].sX = 185;
    m_stDialogBoxInfo[50].sY = 100;
    m_stDialogBoxInfo[50].sSizeX = 270;
    m_stDialogBoxInfo[50].sSizeY = 105;

    //Guild Hall Menu Dialog
    m_stDialogBoxInfo[51].sX = 337;
    m_stDialogBoxInfo[51].sY = 57;
    m_stDialogBoxInfo[51].sSizeX = 258;
    m_stDialogBoxInfo[51].sSizeY = 339;


    //Dialog
    //Friends List
    m_stDialogBoxInfo[70].sX = 280;
    m_stDialogBoxInfo[70].sY = 25;
    m_stDialogBoxInfo[70].sSizeX = 258;
    m_stDialogBoxInfo[70].sSizeY = 339;

#ifdef DEF_ADMINCLIENT
    //Admin Dialog
    m_stDialogBoxInfo[71].sX = 280;
    m_stDialogBoxInfo[71].sY = 25;
    m_stDialogBoxInfo[71].sSizeX = 258;
    m_stDialogBoxInfo[71].sSizeY = 339;

    //Char List Dialog
    m_stDialogBoxInfo[72].sX = 70;
    m_stDialogBoxInfo[72].sY = 50;
    m_stDialogBoxInfo[72].sSizeX = 258;
    m_stDialogBoxInfo[72].sSizeY = 339;
#endif

    //Configs Dialog
    m_stDialogBoxInfo[73].sX = 70;
    m_stDialogBoxInfo[73].sY = 50;
    m_stDialogBoxInfo[73].sSizeX = 258;
    m_stDialogBoxInfo[73].sSizeY = 339;
}

void CGame::send_screen_settings_to_server()
{
    stream_write sw{};
    sw.write<uint32_t>(MSGID_SCREEN_SETTINGS);
    sw.write<uint16_t>(get_virtual_width());
    sw.write<uint16_t>(get_virtual_height());
    sw.write<uint16_t>(get_width());
    sw.write<uint16_t>(get_height());
    write(sw);
}

bool CGame::bInit()
{
    int iIndex{};
    CStrTok * pStrTok{};
    char seps[] = "&= ,\t\n";
    char * token{};

    m_bCommandAvailable = true;

    m_dwTime = G_dwGlobalTime;

    m_bSoundFlag = true;
    m_bMusicStat = m_bSoundStat = m_bSoundFlag;
    m_bIsHideLocalCursor = false;

    m_cEnterCheck = m_cTabCheck = m_cLeftArrowCheck = 0;

    //	if (bCheckImportantFile() == false) {//Change
    //		MessageBoxA(m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
    //		return false;	
    //	}


    if (_bDecodeBuildItemContents() == false)
    {
        MessageBoxA(m_hWnd, "File checksum error! Get Update again please!", "ERROR2", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    if (bReadItemNameConfigFile() == false)
    {
        MessageBoxA(m_hWnd, "ItemName.cfg file contains wrong information.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    if (bInitMagicCfgList() == false)
    {
        MessageBoxA(m_hWnd, "MAGICCFG.TXT file contains wrong information.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    if (bInitSkillCfgList() == false)
    {
        MessageBoxA(m_hWnd, "SKILLCFG.TXT file contains wrong information.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    m_pSprite[DEF_SPRID_INTERFACE_ND_LOADING] = sprite::create_sprite("New-Dialog", 0, false);
    m_pSprite[DEF_SPRID_INTERFACE_ADDINTERFACE] = sprite::create_sprite("interface2", 0, false);
    //m_pSprite[DEF_SPRID_INTERFACE_CRAFTING] = sprite::CreateSprite("interface2", 3, false);
    m_pSprite[DEF_SPRID_INTERFACE_SPRFONTS2] = sprite::create_sprite("interface2", 1, false);
    m_pSprite[DEF_SPRID_INTERFACE_F1HELPWINDOWS] = sprite::create_sprite("interface2", 2, false);
    m_pSprite[DEF_SPRID_INTERFACE_FONT1] = sprite::create_sprite("sprfonts", 0, false);
    m_pSprite[DEF_SPRID_INTERFACE_FONT2] = sprite::create_sprite("sprfonts", 1, false);

    m_stMCursor.sX = 0;
    m_stMCursor.sY = 0;


    m_pMapData = new CMapData(this);

    memset(m_cPlayerName, 0, sizeof(m_cPlayerName));
    memset(m_cAccountName, 0, sizeof(m_cAccountName));
    memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));

    m_sPlayerType = 2;
    m_cPlayerTurn = 0;

    m_cDialogBoxOrder[100] = 29;
    m_cDialogBoxOrder[99] = 30;

    m_cMenuDir = 4;
    m_cMenuDirCnt = 0;
    m_cMenuFrame = 0;

    m_cSoundVolume = 100;
    m_cMusicVolume = 100;

    m_wWR[0] = 0; m_wWG[0] = 0; m_wWB[0] = 0; // Black
    m_wWR[1] = 70; m_wWG[1] = 70; m_wWB[1] = 80; // Light-blue
    m_wWR[2] = 70; m_wWG[2] = 70; m_wWB[2] = 80; // light-blue
    m_wWR[3] = 70; m_wWG[3] = 70; m_wWB[3] = 80; // light-blue
    m_wWR[4] = 70; m_wWG[4] = 100; m_wWB[4] = 70; // Green
    m_wWR[5] = 130; m_wWG[5] = 90; m_wWB[5] = 10; // ?
    m_wWR[6] = 42; m_wWG[6] = 53; m_wWB[6] = 111; // Heavy-blue
    m_wWR[7] = 145; m_wWG[7] = 145; m_wWB[7] = 145; // White
    m_wWR[8] = 120; m_wWG[8] = 100; m_wWB[8] = 120; // Violet
    m_wWR[9] = 75; m_wWG[9] = 10; m_wWB[9] = 10; // Heavy-Red
    m_wWR[10] = 135; m_wWG[10] = 104; m_wWB[10] = 30; // Gold

    m_wR[0] = 200 / 2; m_wG[0] = 200 / 2; m_wB[0] = 200 / 2;
    m_wR[1] = 0x50 / 2; m_wG[1] = 0x50 / 2; m_wB[1] = 0xC0 / 2; // Blue
    m_wR[2] = 79; m_wG[2] = 79; m_wB[2] = 62; // Custom-Weapon Color
    m_wR[3] = 135; m_wG[3] = 104; m_wB[3] = 30; // Gold
    m_wR[4] = 255 / 2; m_wG[4] = 36 / 2; m_wB[4] = 0; // Orange
    m_wR[5] = 10; m_wG[5] = 60; m_wB[5] = 10; // Green
    m_wR[6] = 0x50 / 2; m_wG[6] = 0x50 / 2; m_wB[6] = 0x50 / 2; // Gray
    m_wR[7] = 0x5F / 2; m_wG[7] = 0x9E / 2; m_wB[7] = 0xA0 / 2;
    m_wR[8] = 0xFF / 2; m_wG[8] = 0x69 / 2; m_wB[8] = 0xB4 / 2;
    m_wR[9] = 90; m_wG[9] = 60; m_wB[9] = 90; // Violet

    m_wR[10] = 0; m_wG[10] = 35; m_wB[10] = 60;
    m_wR[11] = 0xD2 / 2; m_wG[11] = 0xB4 / 2; m_wB[11] = 0x8C / 2; // Tan
    m_wR[12] = 0xBD / 2; m_wG[12] = 0xB7 / 2; m_wB[12] = 0x6B / 2; // Khaki
    m_wR[13] = 85; m_wG[13] = 85; m_wB[13] = 8; // Yellow
    m_wR[14] = 75; m_wG[14] = 10; m_wB[14] = 10; // Red
    m_wR[15] = 0x30; m_wG[15] = 0x30; m_wB[15] = 0x30; // Black

    _LoadGameMsgTextContents();

    ReadDataConfig();

    memset(m_cWorldServerName, 0, sizeof(m_cWorldServerName));

    return true;
}
void CGame::ReadDataConfig()
{
    FILE * pFile;
    pFile = fopen("config.dat", "rb");
    if (pFile == 0)
    {
        m_stPartyConfig.bEnabled = true;
        m_stPartyConfig.cType = 1;
        m_stPartyConfig.wColor[0][0] = m_wR[3];
        m_stPartyConfig.wColor[0][0] = m_wG[3];
        m_stPartyConfig.wColor[0][0] = m_wB[3];
        m_stPartyConfig.wPartyVersion = 2214;
        return;
    }
    else
    {
        fread(&m_stGameConfig, sizeof(GAMECONFIG), 1, pFile);
        fseek(pFile, m_stGameConfig.Party.wOffset, SEEK_SET);

        memset(&m_stPartyConfig, 0, sizeof(PARTYCONFIG));
        fread(&m_stPartyConfig, m_stGameConfig.Party.wSize, 1, pFile);
        if (m_stPartyConfig.wPartyVersion < 2214)
        {
            m_stPartyConfig.bEnabled = true;
            m_stPartyConfig.cType = 1;
            m_stPartyConfig.wColor[0][0] = m_wR[3];
            m_stPartyConfig.wColor[0][0] = m_wG[3];
            m_stPartyConfig.wColor[0][0] = m_wB[3];
            m_stPartyConfig.wPartyVersion = 2214;
        }
    }
    fclose(pFile);
    return;
}
void CGame::SaveDataConfig()
{
    FILE * pFile;
    pFile = fopen("config.dat", "wb+");
    m_stGameConfig.Party.wOffset = 200;
    m_stGameConfig.Party.wSize = sizeof(PARTYCONFIG);

    fwrite(&m_stGameConfig, sizeof(GAMECONFIG), 1, pFile);
    fseek(pFile, m_stGameConfig.Party.wOffset, SEEK_SET);

    fwrite(&m_stPartyConfig, sizeof(PARTYCONFIG), 1, pFile);
    fclose(pFile);
}

void CGame::Quit()
{
    int i;

    m_bIsProgramActive = false;

    SaveDataConfig();
    WriteSettings();
    ChangeGameMode(DEF_GAMEMODE_NULL);

    for (i = 0; i < DEF_MAXSPRITES; i++)
        if (m_pSprite[i] != 0) delete m_pSprite[i];
    for (i = 0; i < DEF_MAXTILES; i++)
        if (m_pTileSpr[i] != 0) delete m_pTileSpr[i];
    for (i = 0; i < DEF_MAXEFFECTSPR; i++)
        if (m_pEffectSpr[i] != 0) delete m_pEffectSpr[i];

    for (i = 0; i < 4; i++)
        if (m_pCharList[i] != 0) delete m_pCharList[i];

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pItemList[i] != 0)	delete m_pItemList[i];

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0)	delete m_pBankList[i];

    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if (m_pEffectList[i] != 0) delete m_pEffectList[i];

    for (i = 0; i < DEF_MAXCHATMSGS; i++)
        if (m_pChatMsgList[i] != 0) delete m_pChatMsgList[i];

    for (i = 0; i < DEF_MAXCHATSCROLLMSGS; i++)
        if (m_pChatScrollList[i] != 0) delete m_pChatScrollList[i];

    for (i = 0; i < DEF_MAXWHISPERMSG; i++)
        if (m_pWhisperMsg[i] != 0) delete m_pWhisperMsg[i];

    for (i = 0; i < DEF_MAXMENUITEMS; i++)
        if (m_pItemForSaleList[i] != 0) delete m_pItemForSaleList[i];

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        if (m_pMagicCfgList[i] != 0) delete m_pMagicCfgList[i];

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        if (m_pSkillCfgList[i] != 0) delete m_pSkillCfgList[i];

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)  delete m_pMsgTextList[i];
        if (m_pMsgTextList2[i] != 0) delete m_pMsgTextList2[i];
    }

    if (m_pExID != 0) delete m_pExID;

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != 0) delete m_pBuildItemList[i];

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pDispBuildItemList[i] != 0) delete m_pDispBuildItemList[i];

    for (i = 0; i < DEF_MAXGAMEMSGS; i++)
        if (m_pGameMsgList[i] != 0) delete m_pGameMsgList[i];

	for (i = 0; i < DEF_MAXITEMNAMES; i++)
		if (m_pItemNameList[i] != 0) delete m_pItemNameList[i];

    delete m_pMapData;

    close(ix::WebSocketCloseConstants::kNormalClosureCode, ix::WebSocketCloseConstants::kNormalClosureMessage);

    ChangeGameMode(DEF_GAMEMODE_ONQUIT);
    {
        std::unique_lock<std::mutex> lock(thread_mut);
        if (mapdata_object_counter)
            mapdata_object_counter->join();
    }
}

void CGame::CalcViewPoint()
{
    short dX, dY;

    dX = m_sViewPointX - m_sViewDstX;
    dY = m_sViewPointY - m_sViewDstY;

    if (abs(dX) < abs(m_sViewDX))
    {
        m_sViewPointX = m_sViewDstX;
        m_sViewDX = 0;
    }
    else
    {
        if (dX > 0) m_sViewDX--;
        if (dX < 0) m_sViewDX++;
        if (dX == 0) m_sViewDX = 0;
        if (abs(dX) < 60)
        {//40
            if (m_sViewDX > 4)  m_sViewDX = 4;
            else if (m_sViewDX < -4) m_sViewDX = -4;
        }
        m_sViewPointX += m_sViewDX;
        update_background = true;
    }

    if (abs(dY) < abs(m_sViewDY))
    {
        m_sViewPointY = m_sViewDstY;
        m_sViewDY = 0;
    }
    else
    {
        if (dY > 0) m_sViewDY--;
        if (dY < 0) m_sViewDY++;
        if (dY == 0) m_sViewDY = 0;
        if (abs(dY) < 60)
        {//40
            if (m_sViewDY > 4)  m_sViewDY = 4;
            else if (m_sViewDY < -4) m_sViewDY = -4;
        }
        m_sViewPointY += m_sViewDY;
        update_background = true;
    }
}

bool CGame::_bCheckMoveable(short sx, short sy) const
{
    if (m_bIsCrusadeMode) return true;
    if (m_bHunter == false) return true;
    if (m_bCitizen == false) return true;
    if (m_bAresden == true && m_cMapIndex == 11)
    {
        if (sy == 20)
        {
            if ((sx >= 39) && (sx <= 47)) return false;
            if ((sx >= 360) && (sx <= 368)) return false;
        }
    }
    if (m_bAresden == false && m_cMapIndex == 3)
    {
        if (sy == 373)
        {
            if ((sx >= 27) && (sx <= 32)) return false;
        }
        if (sy == 374)
        {
            if ((sx >= 298) && (sx <= 305)) return false;
        }
    }
    return true;
}

bool CGame::bSendCommand(uint32_t dwMsgID, uint16_t wCommand, char cDir, int iV1, int iV2, int iV3, char * pString, int iV4)
{

    char * cp, cMsg[300]{}, cTxt[256]{}, cKey{};
    uint16_t * wp;
    uint32_t * dwp, dwTime{};
    short * sp;
    int * ip;
    int iRet{}, i{};
    int * fightzonenum{};

    stream_write sw;

    if (!is_connected()) return false;

    dwTime = unixtime();

    memset(cMsg, 0, sizeof(cMsg));
    cKey = (char)(rand() % 255) + 1;// todo: remove this garbage

    switch (dwMsgID)
    {
        case MSGID_REQUEST_TELEPORT_LIST:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;
            cp = (char *)(cMsg + 6);
            memset(cp, 0, 20);
            memcpy(cp, "William", 7);
            iRet = send_message(cMsg, 26, cKey);
            break;

        case MSGID_REQUEST_CHARGED_TELEPORT:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;
            ip = (int *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            *ip = iV1;
            iRet = send_message(cMsg, 10, cKey);
            break;

        case MSGID_REQUEST_SELLITEMLIST:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            for (i = 0; i < DEF_MAXSELLLIST; i++)
            {
                *cp = m_stSellItemList[i].iIndex;
                cp++;
                ip = (int *)cp;
                *ip = m_stSellItemList[i].iAmount;
                cp += 4;
            }

            iRet = send_message(cMsg, 70, cKey);
            break;

        case MSGID_REQUEST_RESTART:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            iRet = send_message(cMsg, 6, cKey);
            break;

        case MSGID_REQUEST_PANNING:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            *cp = cDir;
            cp++;

            iRet = send_message(cMsg, 7, cKey);
            break;

        case MSGID_REQUEST_LOGIN:
            // to Log Server
            sw.write_uint32(MSGID_REQUEST_LOGIN);
            sw.write_uint16(0);
            sw.write_string(account_name);
            sw.write_string(password);
            sw.write_string(NAME_WORLDNAME1);

            write(sw);
            break;

        case MSGID_REQUEST_CREATENEWCHARACTER:
            // to Log Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = (uint16_t)0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cp, m_cPlayerName, 10);
            cp += 10;

            memcpy(cp, m_cAccountName, 10);
            cp += 10;

            memcpy(cp, m_cAccountPassword, 10);
            cp += 10;

            memcpy(cp, m_cWorldServerName, 30);
            cp += 30;

            *cp = m_cGender;
            cp++;

            *cp = m_cSkinCol;
            cp++;

            *cp = m_cHairStyle;
            cp++;

            *cp = m_cHairCol;
            cp++;

            *cp = m_cUnderCol;
            cp++;

            *cp = m_ccStr;
            cp++;

            *cp = m_ccVit;
            cp++;

            *cp = m_ccDex;
            cp++;

            *cp = m_ccInt;
            cp++;

            *cp = m_ccMag;
            cp++;

            *cp = m_ccChr;
            cp++;

            iRet = send_message(cMsg, 77, cKey);
            break;

        case MSGID_REQUEST_ENTERGAME:
            // to Log Server

            sw.write_uint32(MSGID_REQUEST_ENTERGAME);
            sw.write_string(m_cPlayerName);

            write(sw);
            break;

        case MSGID_REQUEST_DELETECHARACTER:
            // to Log Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = (uint16_t)m_wEnterGameType;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            memcpy(cp, m_pCharList[m_wEnterGameType - 1]->m_cName, 10);
            cp += 10;

            memcpy(cp, m_cAccountName, 10);
            cp += 10;

            memcpy(cp, m_cAccountPassword, 10);
            cp += 10;

            memcpy(cp, m_cWorldServerName, 30);
            cp += 30;

            iRet = send_message(cMsg, 66, cKey);
            break;

        case MSGID_REQUEST_SETITEMPOS:
            // to Game Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            *cp = cDir;

            cp++;

            sp = (short *)cp;
            *sp = (short)iV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)iV2;
            cp += 2;

            iRet = send_message(cMsg, 11);
            break;

        case MSGID_COMMAND_CHECKCONNECTION:
            sw.write_uint32(dwMsgID);
            sw.write_uint16(0);
            sw.write_uint32(unixtime());
            write(sw);
            break;

        case MSGID_REQUEST_PING:
            sw.write_uint32(dwMsgID);
            sw.write_uint16(0);
            sw.write_uint64(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            write(sw);
            break;

        case MSGID_REQUEST_PKLIST:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            iRet = send_message(cMsg, 6, cKey);
            break;

        case MSGID_REQUEST_INITDATA:
            // to Game Server
            sw.write_uint32(dwMsgID);
            sw.write_uint16(0);
            write(sw);
            break;

#ifdef DEF_ADMINCLIENT
        case MSGID_REQUEST_CHARLIST:
            sw.write_uint32(dwMsgID);
            sw.write_uint16(0);
            write(sw);
            break;
#endif

        case MSGID_STATECHANGEPOINT:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;
            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            *cp = m_cStateChange1;
            cp++;
            *cp = m_cStateChange2;
            cp++;
            *cp = m_cStateChange3;
            cp++;
            iRet = send_message(cMsg, 12);
            AddEventList(G_cTxt, 10);
            break;

        case MSGID_REQUEST_REGISTER:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            ip = (int *)cp;
            *ip = DEF_UPPERVERSION;
            cp += 4;
            ip = (int *)(cp);
            *ip = DEF_LOWERVERSION;
            cp += 4;
            ip = (int *)(cp);
            *ip = DEF_PATCHVERSION;
            cp += 4;

            iRet = send_message(cMsg, 18, cKey);
            break;

        case MSGID_REQUEST_FRIENDSLIST:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + 6);
            *cp = cDir;

            iRet = send_message(cMsg, 10, cKey);
            break;

        case MSGID_REQUEST_FLOORSTATS:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            dwp = (uint32_t *)cp;
            *dwp = dwTime;
            cp += 4;

            iRet = send_message(cMsg, 10, cKey);
            break;

        case MSGID_LEVELUPSETTINGS:
            //if ((m_cLU_Str + m_cLU_Vit + m_cLU_Dex + m_cLU_Int + m_cLU_Mag + m_cLU_Char) > 3) return false;//Change blahblah > 3 Uncommented

            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            *cp = m_cLU_Str;
            cp++;

            *cp = m_cLU_Vit;
            cp++;

            *cp = m_cLU_Dex;
            cp++;

            *cp = m_cLU_Int;
            cp++;

            *cp = m_cLU_Mag;
            cp++;

            *cp = m_cLU_Char;
            cp++;

            iRet = send_message(cMsg, 12);
            break;

        case MSGID_COMMAND_CHATMSG:
            if (m_bIsTeleportRequested == true) return false;

            if (bCheckLocalChatCommand(pString) == true) return false;

            {
                stream_write sw;
                sw.write_uint32(dwMsgID);
                sw.write_uint16(0);
                sw.write_string(pString);
                write(sw);
            }
            break;

        case MSGID_COMMAND_COMMON:
            if (m_bIsTeleportRequested == true) return false;

            // to Game Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = wCommand;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            //		ip = (int *)cp;
            //		*ip = unixtime();
            //		cp += 4;

            sp = (short *)cp;
            *sp = m_sPlayerX;
            cp += 2;

            sp = (short *)cp;
            *sp = m_sPlayerY;
            cp += 2;

            *cp = cDir;
            cp++;

            switch (wCommand)
            {
                case DEF_COMMONTYPE_BUILDITEM:
                    memcpy(cp, pString, 20);
                    cp += 20;

                    *cp = (char)m_stDialogBoxInfo[26].sV1;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV2;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV3;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV4;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV5;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV6;
                    cp++;

                    iRet = send_message(cMsg, 37 + 4);
                    break;

                case DEF_COMMONTYPE_REQ_CREATEPOTION:
                    *cp = (char)m_stDialogBoxInfo[26].sV1;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV2;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV3;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV4;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV5;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV6;
                    cp++;

                    iRet = send_message(cMsg, 18 + 4);
                    break;

                    //Crafting
                case DEF_COMMONTYPE_CRAFTITEM:
                    memcpy(cp, "                    ", 20);
                    cp += 20;
                    *cp = (char)m_stDialogBoxInfo[26].sV1;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV2;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV3;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV4;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV5;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[26].sV6;
                    cp++;
                    iRet = send_message(cMsg, 37 + 4);
                    break;

                case DEF_COMMONTYPE_REQ_CREATESLATE:
                    *cp = (char)m_stDialogBoxInfo[40].sV1;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[40].sV2;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[40].sV3;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[40].sV4;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[40].sV5;
                    cp++;
                    *cp = (char)m_stDialogBoxInfo[40].sV6;
                    cp++;
                    iRet = send_message(cMsg, 18 + 4);
                    break;

                default:
                    if (pString == 0)
                    {
                        ip = (int *)cp;
                        *ip = iV1;
                        cp += 4;

                        ip = (int *)cp;
                        *ip = iV2;
                        cp += 4;

                        ip = (int *)cp;
                        *ip = iV3;
                        cp += 4;

                        cp += 30;

                        dwp = (uint32_t *)cp;
                        *dwp = dwTime;
                        cp += 4;

                        iRet = send_message(cMsg, 23 + 34 + 4);
                    }
                    else
                    {
                        ip = (int *)cp;
                        *ip = iV1;
                        cp += 4;

                        ip = (int *)cp;
                        *ip = iV2;
                        cp += 4;

                        ip = (int *)cp;
                        *ip = iV3;
                        cp += 4;

                        memcpy(cp, pString, 30);
                        cp += 30;

                        ip = (int *)cp;
                        *ip = iV4;
                        cp += 4;

                        iRet = send_message(cMsg, 23 + 34 + 4);
                    }
                    break;
            }

            break;

        case MSGID_REQUEST_CREATENEWGUILD:
        case MSGID_REQUEST_DISBANDGUILD:
            // to Game Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            memset(cTxt, 0, sizeof(cTxt));
            memcpy(cTxt, m_cPlayerName, 10);
            memcpy(cp, cTxt, 10);
            cp += 10;

            memset(cTxt, 0, sizeof(cTxt));
            memcpy(cTxt, m_cAccountName, 10);
            memcpy(cp, cTxt, 10);
            cp += 10;

            memset(cTxt, 0, sizeof(cTxt));
            memcpy(cTxt, m_cAccountPassword, 10);
            memcpy(cp, cTxt, 10);
            cp += 10;

            char cTemp[21];
            memset(cTemp, 0, sizeof(cTemp));
            memcpy(cTemp, m_cGuildName, 20);
            m_Misc.ReplaceString(cTemp, ' ', '_');
            memcpy(cp, cTemp, 20);
            cp += 20;

            iRet = send_message(cMsg, 56, cKey);
            break;

        case MSGID_REQUEST_TELEPORT:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            iRet = send_message(cMsg, 6);

            m_bIsTeleportRequested = true;
            break;

        case MSGID_REQUEST_CIVILRIGHT:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            iRet = send_message(cMsg, 6);
            break;

        case MSGID_REQUEST_RETRIEVEITEM:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = DEF_MSGTYPE_CONFIRM;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            *cp = (char)iV1;

            iRet = send_message(cMsg, 7);
            break;

        case MSGID_REQUEST_NOTICEMENT:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            ip = (int *)cp;
            *ip = iV1;
            cp += 4;

            iRet = send_message(cMsg, 10, cKey);
            break;
        case  MSGID_REQUEST_FIGHTZONE_RESERVE:
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);
            fightzonenum = (int *)cp;
            *fightzonenum = iV1;
            cp += 4;

            iRet = send_message(cMsg, 10);
            break;

        default:
            if (m_bIsTeleportRequested == true) return false;

            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = wCommand;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            sp = (short *)cp;
            *sp = m_sPlayerX;
            cp += 2;

            sp = (short *)cp;
            *sp = m_sPlayerY;
            cp += 2;

            *cp = cDir;
            cp++;

            sp = (short *)cp;
            *sp = (short)iV1;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)iV2;
            cp += 2;

            sp = (short *)cp;
            *sp = (short)iV3;
            cp += 2;
            if ((wCommand == DEF_OBJECTATTACK) || (wCommand == DEF_OBJECTATTACKMOVE))
            { // v1.4  v2.171
                sp = (short *)cp;
                *sp = (short)iV4;
                cp += 2;

                //v2.171
                dwp = (uint32_t *)cp;
                *dwp = dwTime;
                cp += 4;

                iRet = send_message(cMsg, 19 + 4); //v2.171
            }
            else
            {
                //v2.171
                dwp = (uint32_t *)cp;
                *dwp = dwTime;
                cp += 4;

                iRet = send_message(cMsg, 17 + 4); //v2.171
            }
            m_cCommandCount++;
            break;

    }

    switch (iRet)
    {
        case DEF_XSOCKEVENT_SOCKETCLOSED:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_QUENEFULL:

            ChangeGameMode(DEF_GAMEMODE_ONCONNECTIONLOST);

            close(ix::WebSocketCloseConstants::kAbnormalCloseCode, ix::WebSocketCloseConstants::kAbnormalCloseMessage);
            break;
    }

    return true;
}


void CGame::make_sprite(char * FileName, int iStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
        m_pSprite[i + iStart] = sprite::create_sprite(FileName, i, bAlphaEffect);
}
void CGame::make_tile_sprite(char * FileName, short sStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
        m_pTileSpr[i + sStart] = sprite::create_sprite(FileName, i, bAlphaEffect);
}
void CGame::make_effect_sprite(char * FileName, short sStart, short sCount, bool bAlphaEffect)
{
    for (short i = 0; i < sCount; i++)
        m_pEffectSpr[i + sStart] = sprite::create_sprite(FileName, i, bAlphaEffect);
}

void CGame::OnTimer()
{
    if (m_cGameMode < 0) return;
    uint32_t dwTime = unixtime();

    {
        std::lock_guard<std::mutex> lock(socket_mut);
        while (!loginpipe.empty())
        {
            std::shared_ptr<CGame::msg_queue_entry> entry = get_login_msg_queue();
            LogResponseHandler(entry->data, entry->size);
        }
    }

    if (newconnection && ws)
    {
        m_dwConnectMode = MSGID_REQUEST_LOGIN;
        ConnectionEstablishHandler(DEF_SERVERTYPE_LOG);
        newconnection = false;
    }

    if (m_cGameMode != DEF_GAMEMODE_ONLOADING)
    {
        if ((dwTime - m_dwCheckSprTime) > 8000)//3000
        {
            m_dwCheckSprTime = dwTime;
            if (m_bIsProgramActive) ReleaseUnusedSprites();
            if (is_connected() && loggedin)
                bSendCommand(MSGID_COMMAND_CHECKCONNECTION, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
        }
    }

    if (m_cGameMode == DEF_GAMEMODE_ONMAINGAME)
    {
//         if ((dwTime - m_dwPartyListTime) > 1000)
//         {
//             m_dwPartyListTime = dwTime;
//             bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, 0, 2, 0, 0, m_cMCName);
//         }
        if ((dwTime - m_dwBWTime) > 1000)
        {
            G_iBytesInLast = G_iBytesIn;
            G_iBytesOutLast = G_iBytesOut;
            G_iBytesIn = G_iBytesOut = 0;
            m_dwBWTime = dwTime;
        }
        if ((dwTime - m_dwCheckWhoTime) > 5000)
        {
            m_dwCheckWhoTime = dwTime;
            bSendCommand(MSGID_COMMAND_CHATMSG, 0, 0, 0, 0, 0, "/who");
        }
        if ((dwTime - m_dwCheckPingTime) > 5000)
        {
            m_dwCheckPingTime = dwTime;
            bSendCommand(MSGID_REQUEST_PING, 0, 0, 0, 0, 0, 0);
        }
        if ((dwTime - m_dwCheckPkTime) > 30000)
        {
            m_dwCheckPkTime = dwTime;
            ClearPKNameList();
            //	bSendCommand(MSGID_REQUEST_PKLIST, 0, 0, 0, 0, 0, 0);
        }
#ifdef DEF_ADMINCLIENT
        if ((dwTime - m_dwCheckRefreshTime) > atoi(m_cRefreshRate))
        {
            m_dwCheckRefreshTime = dwTime;
            if (bAdminListUpdate == true)
                bSendCommand(MSGID_REQUEST_CHARLIST, 0, 0, 0, 0, 0, 0);
        }
#endif
        if ((dwTime - m_dwCheckConnTime) > 5000)
        {
            bSendCommand(MSGID_REQUEST_FRIENDSLIST, 0, 7, 0, 0, 0, 0);
            m_dwCheckConnTime = dwTime;
            if ((_iCheckLUS() != 0)) EnableDialogBox(12, 0, 0, 0);
            if ((m_bIsCrusadeMode) && (m_iCrusadeDuty == 0)) EnableDialogBox(33, 1, 0, 0);
        }

        if ((dwTime - m_dwCheckChatTime) > 2000)
        {
            m_dwCheckChatTime = m_dwTime;
            ReleaseTimeoverChatMsg();
//             if (m_cCommandCount >= 6)
//             {
//                 m_iNetLagCount++;
//                 if (m_iNetLagCount >= 7)
//                 {
//                     ChangeGameMode(DEF_GAMEMODE_ONCONNECTIONLOST);
//                     close(ix::WebSocketCloseConstants::kAbnormalCloseCode, "Net lag count too high");
//                     return;
//                 }
//             }
//             else m_iNetLagCount = 0;
        }
    }
}

void CGame::_SetItemOrder(char cWhere, char cItemID)
{
    int i;

    switch (cWhere)
    {
        case 0:
            for (i = 0; i < DEF_MAXITEMS; i++)
                if (m_cItemOrder[i] == cItemID)
                    m_cItemOrder[i] = -1;

            for (i = 1; i < DEF_MAXITEMS; i++)
                if ((m_cItemOrder[i - 1] == -1) && (m_cItemOrder[i] != -1))
                {
                    m_cItemOrder[i - 1] = m_cItemOrder[i];
                    m_cItemOrder[i] = -1;
                }

            for (i = 0; i < DEF_MAXITEMS; i++)
                if (m_cItemOrder[i] == -1)
                {
                    m_cItemOrder[i] = cItemID;
                    return;
                }
            break;
    }
}

void CGame::ClearGuildNameList()
{
    for (int i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        m_stGuildName[i].dwRefTime = 0;
        m_stGuildName[i].iGuildRank = -1;
        memset(m_stGuildName[i].cCharName, 0, sizeof(m_stGuildName[i].cCharName));
        memset(m_stGuildName[i].cGuildName, 0, sizeof(m_stGuildName[i].cGuildName));
    }
}
void CGame::ClearPKNameList()
{
    for (int i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        m_stPKList[i].m_dwRefTime = 0;
        memset(m_stPKList[i].m_cCharName, 0, sizeof(m_stPKList[i].m_cCharName));
        memset(m_stPKList[i].m_cDisplayInfo, 0, sizeof(m_stPKList[i].m_cDisplayInfo));
        m_stPKList[i].m_bIsEnabled = false;
        m_stPKList[i].m_iGM = 0;
        m_stPKList[i].m_iPKs = 0;
    }
}

void CGame::InitGameSettings()
{
    int i;

    m_bForceAttack = true;//Change false
    m_dwCommandTime = 0;
    m_bInputStatus = false;
    m_pInputBuffer = 0;

    m_iPDBGSdivX = 0;
    m_iPDBGSdivY = 0;
    m_bIsRedrawPDBGS = true;

    m_iCameraShakingDegree = 0;

    m_cCommand = DEF_OBJECTSTOP;
    m_cCommandCount = 0;

    m_bIsGetPointingMode = false;
    m_iPointCommandType = -1; //v2.15 0 -> -1
    ClearCoords();

    m_bIsCombatMode = false;
    m_bRunningMode = false;

    m_stMCursor.cPrevStatus = DEF_CURSORSTATUS_NULL;
    m_stMCursor.dwSelectClickTime = 0;

    m_bSkillUsingStatus = false;
    m_bItemUsingStatus = false;

    m_bIsWhetherEffect = false;
    m_cWhetherEffectType = 0;

    m_iDownSkillIndex = -1;
    m_stDialogBoxInfo[15].bFlag = false;

    m_bIsConfusion = false;

    m_iIlusionOwnerH = 0;
    m_cIlusionOwnerType = 0;
    m_bIllusionMVT = false;

    m_iDrawFlag = 0;
    m_bDrawFlagDir = false;

    m_bIsCrusadeMode = false;
    m_iCrusadeDuty = 0;

    m_iNetLagCount = 0;

    m_dwEnvEffectTime = unixtime();

    for (i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        m_stGuildName[i].dwRefTime = 0;
        m_stGuildName[i].iGuildRank = -1;
        memset(m_stGuildName[i].cCharName, 0, sizeof(m_stGuildName[i].cCharName));
        memset(m_stGuildName[i].cGuildName, 0, sizeof(m_stGuildName[i].cGuildName));
    }

    for (i = 0; i < 101; i++)
        m_bIsDialogEnabled[i] = false;

    for (i = 0; i < 98; i++)
        m_cDialogBoxOrder[i] = 0;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
    {
        if (m_pEffectList[i] != 0) delete m_pEffectList[i];
        m_pEffectList[i] = 0;
    }

    for (i = 0; i < DEF_MAXCHATMSGS; i++)
    {
        if (m_pChatMsgList[i] != 0) delete m_pChatMsgList[i];
        m_pChatMsgList[i] = 0;
    }

    for (i = 0; i < DEF_MAXCHATSCROLLMSGS; i++)
    {
        if (m_pChatScrollList[i] != 0) delete m_pChatScrollList[i];
        m_pChatScrollList[i] = 0;
    }

    for (i = 0; i < DEF_MAXWHISPERMSG; i++)
    {
        if (m_pWhisperMsg[i] != 0) delete m_pWhisperMsg[i];
        m_pWhisperMsg[i] = 0;
    }

    memset(m_cLocation, 0, sizeof(m_cLocation));

    memset(m_cGuildName, 0, sizeof(m_cGuildName));
    m_iGuildRank = -1;
    m_iTotalGuildsMan = 0;

    for (i = 0; i < 100; i++)
    {
        m_stGuildOpList[i].cOpMode = 0;
        memset(m_stGuildOpList[i].cName, 0, sizeof(m_stGuildOpList[i].cName));
    }

    for (i = 0; i < 6; i++)
    {
        memset(m_stEventHistory[i].cTxt, 0, sizeof(m_stEventHistory[i].cTxt));
        m_stEventHistory[i].dwTime = G_dwGlobalTime;

        memset(m_stEventHistory2[i].cTxt, 0, sizeof(m_stEventHistory2[i].cTxt));
        m_stEventHistory2[i].dwTime = G_dwGlobalTime;
    }

    for (i = 0; i < DEF_MAXMENUITEMS; i++)
    {
        if (m_pItemForSaleList[i] != 0) delete m_pItemForSaleList[i];
        m_pItemForSaleList[i] = 0;
    }

    for (i = 0; i < 21; i++)
    {
        m_stDialogBoxInfo[i].bFlag = false;
        m_stDialogBoxInfo[i].sView = 0;
        m_stDialogBoxInfo[i].bIsScrollSelected = false;
    }

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pItemList[i] != 0)
        {
            delete m_pItemList[i];
            m_pItemList[i] = 0;
        }

    for (i = 0; i < DEF_MAXSELLLIST; i++)
    {
        m_stSellItemList[i].iIndex = -1;
        m_stSellItemList[i].iAmount = 0;
    }

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0)
        {
            delete m_pBankList[i];
            m_pBankList[i] = 0;
        }

    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
        m_cMagicMastery[i] = 0;

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
        m_cSkillMastery[i] = 0;

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;

        if (m_pMsgTextList2[i] != 0)
            delete m_pMsgTextList2[i];
        m_pMsgTextList2[i] = 0;
    }

    for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
    {
        m_stPartyMember[i].cStatus = 0;
        memset(m_stPartyMember[i].cName, 0, sizeof(m_stPartyMember[i].cName));
    }

    m_iLU_Point = 0;//Change = 3
    m_cLU_Str = m_cLU_Vit = m_cLU_Dex = m_cLU_Int = m_cLU_Mag = m_cLU_Char = 0;
    m_bDialogTrans = false;
    m_cWhetherStatus = 0;

    m_cLogOutCount = -1;
    m_dwLogOutCountTime = 0;
    m_iSuperAttackLeft = 0;
    m_bSuperAttackMode = false;
    m_iFightzoneNumber = 0;


    memset(m_cBGMmapName, 0, sizeof(m_cBGMmapName));

    m_dwWOFtime = 0;

    m_stQuest.sWho = 0;
    m_stQuest.sQuestType = 0;
    m_stQuest.sContribution = 0;
    m_stQuest.sTargetType = 0;
    m_stQuest.sTargetCount = 0;
    m_stQuest.sX = 0;
    m_stQuest.sY = 0;
    m_stQuest.sRange = 0;
    m_stQuest.bIsQuestCompleted = false;

    memset(m_stQuest.cTargetName, 0, sizeof(m_stQuest.cTargetName));

    m_bIsObserverMode = false;
    m_bIsObserverCommanded = false;
    m_iRating = 0;

    m_bIsPoisoned = false;
    m_bIsPrevMoveBlocked = false;
    m_iPrevMoveX = m_iPrevMoveY = -1;

    m_sDamageMove = 0;
    m_sDamageMoveAmount = 0;


    m_bForceDisconn = false;

    m_bIsSpecialAbilityEnabled = false;
    m_iSpecialAbilityType = 0;

    m_dwSpecialAbilitySettingTime = 0;
    m_iSpecialAbilityTimeLeftSec = 0;

    m_stMCursor.cSelectedObjectType = 0;

    m_bIsF1HelpWindowEnabled = false;
    m_bIsTeleportRequested = false;

    for (i = 0; i < DEF_MAXCRUSADESTRUCTURES; i++)
    {
        m_stCrusadeStructureInfo[i].cType = 0;
        m_stCrusadeStructureInfo[i].cSide = 0;
        m_stCrusadeStructureInfo[i].sX = 0;
        m_stCrusadeStructureInfo[i].sY = 0;
    }
    memset(m_cStatusMapName, 0, sizeof(m_cStatusMapName));

    m_dwCommanderCommandRequestedTime = 0;

    memset(m_cTopMsg, 0, sizeof(m_cTopMsg));
    m_iTopMsgLastSec = 0;
    m_dwTopMsgTime = 0;

    m_iConstructionPoint = 0;
    m_iWarContribution = 0;

    memset(m_cTeleportMapName, 0, sizeof(m_cTeleportMapName));
    m_iTeleportLocX = m_iTeleportLocY = -1;

    memset(m_cConstructMapName, 0, sizeof(m_cConstructMapName));
    m_iConstructLocX = m_iConstructLocY = -1;

    m_iTotalPartyMember = 0;
    m_iPartyStatus = 0;
    //for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) memset(m_stPartyMemberNameList[i].cName, 0, sizeof(m_stPartyMemberNameList[i].cName));
    memset(m_stPartyMemberNameList, 0, sizeof(m_stPartyMemberNameList));

    m_iGizonItemUpgradeLeft = 0;

    EnableDialogBox(9, 0, 0, 0);
}

void CGame::_GetHairColorRGB(int iColorType, int * pR, int * pG, int * pB)
{
    switch (iColorType)
    {
        case 0:
            *pR = 14; *pG = -5; *pB = -5; break;
        case 1:
            *pR = 20; *pG = 0; *pB = 0; break;
        case 2:
            *pR = 22; *pG = 13; *pB = -10; break;
        case 3:
            *pR = 0; *pG = 10; *pB = 0; break;
        case 4:
            *pR = 0; *pG = 0; *pB = 22; break;
        case 5:
            *pR = -5; *pG = -5; *pB = 15; break;
        case 6:
            *pR = 15; *pG = -5; *pB = 16; break;
        case 7:
            *pR = -6; *pG = -6; *pB = -6; break;
        case 8:
            *pR = 10; *pG = 3; *pB = 10; break;
        case 9:
            *pR = 10; *pG = 3; *pB = -10; break;
        case 10:
            *pR = -10; *pG = 3; *pB = 10; break;
        case 11:
            *pR = 10; *pG = 3; *pB = 20; break;
        case 12:
            *pR = 21; *pG = 3; *pB = 3; break;
        case 13:
            *pR = 3; *pG = 3; *pB = 25; break;
        case 14:
            *pR = 3; *pG = 11; *pB = 3; break;
        case 15:
            *pR = 6; *pG = 8; *pB = 0; break;
    }
}


void CGame::_PutGuildOperationList(char * pName, char cOpMode)
{
    int i;

    for (i = 0; i < 100; i++)
        if (m_stGuildOpList[i].cOpMode == 0)
        {
            m_stGuildOpList[i].cOpMode = cOpMode;
            memset(m_stGuildOpList[i].cName, 0, sizeof(m_stGuildOpList[i].cName));
            memcpy(m_stGuildOpList[i].cName, pName, 20);
            return;
        }
}

void CGame::_ShiftGuildOperationList()
{
    int i;

    memset(m_stGuildOpList[0].cName, 0, sizeof(m_stGuildOpList[0].cName));
    m_stGuildOpList[0].cOpMode = 0;

    for (i = 1; i < 100; i++)
        if ((m_stGuildOpList[i - 1].cOpMode == 0) && (m_stGuildOpList[i].cOpMode != 0))
        {
            m_stGuildOpList[i - 1].cOpMode = m_stGuildOpList[i].cOpMode;
            memset(m_stGuildOpList[i - 1].cName, 0, sizeof(m_stGuildOpList[i - 1].cName));
            memcpy(m_stGuildOpList[i - 1].cName, m_stGuildOpList[i].cName, 20);

            memset(m_stGuildOpList[i].cName, 0, sizeof(m_stGuildOpList[i].cName));
            m_stGuildOpList[i].cOpMode = 0;
        }
}


void CGame::SetItemCount(char * pItemName, uint32_t dwCount)
{
    int i;
    char cTmpName[21];

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pItemName);

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_pItemList[i] != 0) && (memcmp(m_pItemList[i]->m_cName, cTmpName, 20) == 0))
        {

            m_pItemList[i]->m_dwCount = dwCount;
            return;
        }
}


void CGame::AddEventList(const char * pTxt, char cColor, bool bDupAllow, int iGM)
{
    int i;

    if ((bDupAllow == false) && (strcmp(m_stEventHistory[5].cTxt, pTxt) == 0)) return;

    // v2.12
    if (cColor == 10)
    {
        for (i = 1; i < 6; i++)
        {
            strcpy(m_stEventHistory2[i - 1].cTxt, m_stEventHistory2[i].cTxt);
            m_stEventHistory2[i - 1].cColor = m_stEventHistory2[i].cColor;
            m_stEventHistory2[i - 1].dwTime = m_stEventHistory2[i].dwTime;
        }

        memset(m_stEventHistory2[5].cTxt, 0, sizeof(m_stEventHistory2[5].cTxt));
        strcpy(m_stEventHistory2[5].cTxt, pTxt);
        m_stEventHistory2[5].cColor = cColor;
        m_stEventHistory2[5].dwTime = m_dwCurTime;

    }
    else
    {
        for (i = 1; i < 6; i++)
        {
            strcpy(m_stEventHistory[i - 1].cTxt, m_stEventHistory[i].cTxt);
            m_stEventHistory[i - 1].cColor = m_stEventHistory[i].cColor;
            m_stEventHistory[i - 1].dwTime = m_stEventHistory[i].dwTime;
        }

        memset(m_stEventHistory[5].cTxt, 0, sizeof(m_stEventHistory[5].cTxt));
        strcpy(m_stEventHistory[5].cTxt, pTxt);
        m_stEventHistory[5].cColor = cColor;
        m_stEventHistory[5].dwTime = m_dwCurTime;
    }
}


int _iAttackerHeight[] = { 0, 35, 35,35,35,35,35, 0,0,0,
5,  // Slime
35, // Skeleton
40, // Stone-Golem
45, // Cyclops
35,// OrcMage
35,// ShopKeeper
5,// GiantAnt
8,// Scorpion
35,// Zombie
35,// Gandalf
35,// Howard
35,// Guard
10,// Amphis
38,// Clay-Golem
35,// Tom
35,// William
35,// Kennedy
35,// Hellhound
50,// Troll
45,// Orge
55,// Liche
65,// Demon
46,// Unicorn
49,// WereWolf
55,// Dummy
35,// Energysphere
75,// Arrow Guard Tower
75,// Cannon Guard Tower
50,// Mana Collector
50,// Detector
50,// Energy Shield Generator
50,// Grand Magic Generator
50,// ManaStone 42
40,// Light War Beetle
35,// GHK
40,// GHKABS
35,// TK
60,// BG
40,// Stalker
70,// HellClaw
85,// Tigerworm
50,// Catapult
85,// Gargoyle
70,// Beholder
40,// Dark-Elf
20,// Bunny
20,// Cat
40,// Giant-Frog
80,// Mountain-Giant
85,// Ettin
50,// Cannibal-Plant
50, // Rudolph //Change \/
80, // Direboar 62
90, // Frost 63
40, // Crops 64 
80, // IceGolem 65
190, // Wyvern 66
35, // npc 67
35, // npc 68
35, // npc 69
100, // Balrog 70
90, // Centaur 71
75, // ClawTurtle 72
200, // FireWyvern 73
80, // GiantCrayfish 74
120, // Gi Lizard 75
100, // Gi Tree 76
100, // Master Orc 77
80, // Minotaur 78
100, // Nizie 79
25,  // Tentocle 80
200, // Abaddon	 81
60, // Sorceress 82
60, // ATK 83
70, // MasterElf 84
60, // DSK 85
50, // HBT 86
60, // CT 87
60, // Barbarian 88
60, // AGC 89
35, // ncp 90 Gail
35  // Gate 91
};

void CGame::bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1)
{
    int i;
    int16_t sAbsX, sAbsY, sDist, lPan;
    int  iV2 = 0;

    auto result = get_distance_from_player(dX, dY);

    lPan = result.first;

    if (m_cDetailLevel == 0)
    {
        switch (sType)
        {
            case 8:
            case 9:
            case 11:
            case 12:
            case 14:
            case 15:
                return;
        }
    }

    if (m_bIsProgramActive == false) return;

    sAbsX = abs(((m_sViewPointX / 32) + (get_virtual_width() / 32) / 2) - dX);
    sAbsY = abs(((m_sViewPointY / 32) + (get_virtual_height() / 32) / 2) - dY);

    if (sAbsX > sAbsY) sDist = sAbsX;
    else sDist = sAbsY;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if (m_pEffectList[i] == 0)
        {
            m_pEffectList[i] = new CEffect;

            m_pEffectList[i]->m_sType = sType;
            m_pEffectList[i]->m_sX = sX;
            m_pEffectList[i]->m_sY = sY;
            m_pEffectList[i]->m_dX = dX;
            m_pEffectList[i]->m_dY = dY;
            m_pEffectList[i]->m_iV1 = iV1;

            m_pEffectList[i]->m_cFrame = cStartFrame;
            m_pEffectList[i]->m_dwTime = m_dwCurTime;

            switch (sType)
            {
                case 1:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - _iAttackerHeight[iV1];
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 2:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - _iAttackerHeight[iV1];
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);

                    PlaySound('C', 4, sDist);
                    break;

                case 4:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32;
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 100;

                    sAbsX = abs(((m_sViewPointX / 32) + (get_virtual_width() / 32) / 2) - sX);
                    sAbsY = abs(((m_sViewPointY / 32) + (get_virtual_height() / 32) / 2) - sY);

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    PlaySound('E', 12, sDist, lPan);
                    break;

                case 5:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 6:	 // Energy Bolt
                case 10: // Lightning Arrow
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 2, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 7:
                    // Magic Missile Exp
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 5;
                    m_pEffectList[i]->m_dwFrameTime = 50;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 3, sDist, lPan);
                    break;

                case 8:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 4;
                    m_pEffectList[i]->m_dwFrameTime = 30;
                    break;

                case 9:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_rX = 6 - (rand() % 12);
                    m_pEffectList[i]->m_rY = -8 - (rand() % 6);

                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 30;
                    break;

                case 11:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_rX = 6 - (rand() % 12);

                    if (iV2 == 0)
                        m_pEffectList[i]->m_rY = -2 - (rand() % 4);
                    else m_pEffectList[i]->m_rY = -2 - (rand() % 10);

                    m_pEffectList[i]->m_cMaxFrame = 8;
                    m_pEffectList[i]->m_dwFrameTime = 30;
                    break;

                case 12:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_rX = 8 - (rand() % 16);
                    m_pEffectList[i]->m_rY = 4 - (rand() % 12);

                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 30;
                    break;

                case 13:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 18;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;

                case 14:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 4;
                    m_pEffectList[i]->m_dwFrameTime = 100;
                    break;

                case 15:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 16:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;

                case 17:
                    m_pEffectList[i]->m_mX = sX + (rand() % 20) - 40;
                    m_pEffectList[i]->m_mY = sY + (rand() % 20) - 40;
                    m_pEffectList[i]->m_rX = 8 - (rand() % 16);
                    m_pEffectList[i]->m_rY = 4 - (rand() % 12);
                    m_pEffectList[i]->m_mX3 = sX;
                    m_pEffectList[i]->m_mY3 = sY;
                    m_pEffectList[i]->m_iV1 = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;

                case 18:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 50;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    SetCameraShakingEffect(sDist);
                    break;

                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
                    break;

                case 30:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 9;
                    m_pEffectList[i]->m_dwFrameTime = 40;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist * 2);
                    break;

                case 31:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 8;
                    m_pEffectList[i]->m_dwFrameTime = 40;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 32:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 4;
                    m_pEffectList[i]->m_dwFrameTime = 100;
                    break;

                case 33:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 34:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    SetCameraShakingEffect(sDist);
                    break;

                case 35: //Change
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 18;
                    m_pEffectList[i]->m_dwFrameTime = 40;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;
                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist * 2);
                    break;

                case 36: //Change
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 40;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;
                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 40:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 45, sDist, lPan);
                    break;

                case 41:
                case 42:
                case 43:
                case 44:

                case 45:
                case 46:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY - 220;
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    m_pEffectList[i]->m_iV1 = 20;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 46, sDist, lPan);
                    break;

                case 47:
                case 48:
                case 49:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY - 220;
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    m_pEffectList[i]->m_iV1 = 20;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 46, sDist, lPan);
                    break;

                case 50:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 50;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    if ((rand() % 4) == 1) SetCameraShakingEffect(sDist);
                    PlaySound('E', 47, sDist, lPan);
                    break;

                case 51:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 9; //15;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 52: // Protect ring
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 80;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 53: // Hold twist
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 80;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 54: // star twinkling
                case 55:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 15;
                    break;

                case 56:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 45, sDist, lPan);
                    break;

                case 57:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 60:
                    m_pEffectList[i]->m_mX = sX + 300;
                    m_pEffectList[i]->m_mY = sY - 460;

                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 50;
                    break;

                case 61:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist, 2);
                    break;

                case 62:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 6;
                    m_pEffectList[i]->m_dwFrameTime = 100;
                    break;

                case 63:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;

                case 64:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;

                case 65:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 66:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist, 2);
                    break;

                case 67:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 27;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 68: // worm-bite 
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 17;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 4, sDist, lPan);

                    m_pEffectList[i]->m_iV1 = sDist;
                    //SetCameraShakingEffect(sDist, 2);
                    break;

                case 69:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 42, sDist, lPan);
                    break;

                case 70:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    PlaySound('E', 42, sDist, lPan);
                    break;

                case 71:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    SetCameraShakingEffect(sDist);
                    break;

                case 72:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    if ((rand() % 4) == 1) SetCameraShakingEffect(sDist);
                    PlaySound('E', 47, sDist, lPan);
                    break;

                case 73:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 60;
                    break;

                case 74:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 19;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 75:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_dX = dX;
                    m_pEffectList[i]->m_dY = dY;
                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 76:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_dX = dX;
                    m_pEffectList[i]->m_dY = dY;
                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 77:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_dX = dX;
                    m_pEffectList[i]->m_dY = dY;
                    m_pEffectList[i]->m_cMaxFrame = 16;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 80: //Change
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_iV1 = 20;
                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 25;
                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));
                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;
                    SetCameraShakingEffect(sDist);
                    break;

                case 81: //Change Storm Bringer
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 27;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 82: //Change Apocalypse Gate
                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                    //
                    //		case 90:// Earth Shock Wave //Change
                    //			m_pEffectList[i]->m_mX     = sX;
                    //			m_pEffectList[i]->m_mY     = sY;
                    //			m_pEffectList[i]->m_cMaxFrame   = 29;
                    //			m_pEffectList[i]->m_dwFrameTime = 20;
                    //
                    ///*			sAbsX = abs(320 - (sX - m_sViewPointX));
                    //			sAbsY = abs(240 - (sY - m_sViewPointY));
                    //
                    //			if (sAbsX > sAbsY) sDist = sAbsX;
                    //			else sDist = sAbsY;
                    //			sDist = sDist / 32;
                    //
                    //			lPan = ((sX - m_sViewPointX)-320)*30;
                    //			if ((rand()%4) == 1) SetCameraShakingEffect(sDist);*/
                    //			//PlaySound('E', 47, sDist, lPan);
                    //			break;

                            /*case 79:
                                m_pEffectList[i]->m_mX     = sX + rand()%32;
                                m_pEffectList[i]->m_mY     = sY;
                                m_pEffectList[i]->m_cMaxFrame   = 29;
                                m_pEffectList[i]->m_dwFrameTime = 20;

                                sAbsX = abs(320 - (sX - m_sViewPointX));
                                sAbsY = abs(240 - (sY - m_sViewPointY));

                                if (sAbsX > sAbsY) sDist = sAbsX;
                                else sDist = sAbsY;
                                sDist = sDist / 32;

                                lPan = ((sX - m_sViewPointX)-320)*30;
                                if ((rand()%4) == 1) SetCameraShakingEffect(sDist);
                                PlaySound('E', 47, sDist, lPan);
                                break;*/
                                /*
                                        case 91: // Earth Piece
                                        case 92: // Earth Cloud
                                        case 93:
                                            m_pEffectList[i]->m_mX     = sX;
                                            m_pEffectList[i]->m_mY     = sY;
                                            m_pEffectList[i]->m_cMaxFrame   = 29;
                                            m_pEffectList[i]->m_dwFrameTime = 20;
                                            m_pEffectList[i]->m_iV1 = 20;

                                            sAbsX = abs(320 - (sX - m_sViewPointX));
                                            sAbsY = abs(240 - (sY - m_sViewPointY));

                                            if (sAbsX > sAbsY) sDist = sAbsX;
                                            else sDist = sAbsY;
                                            sDist = sDist / 32;

                                            lPan = ((sX - m_sViewPointX)-320)*30;
                                            //PlaySound('E', 46, sDist, lPan);
                                            break;*/

                case 100:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 124: // Protect form N.M
                case 133: // Protection from Magic
                    bAddNewEffect(52, dX * 32, dY * 32, 0, 0, 0, 0);
                    delete m_pEffectList[i];
                    m_pEffectList[i] = 0;
                    break;

                case 125: // Hold Person
                case 135: // Paralyze
                    bAddNewEffect(53, dX * 32, dY * 32, 0, 0, 0, 0);
                    delete m_pEffectList[i];
                    m_pEffectList[i] = 0;
                    break;

                case 102: //
                case 122: // Recall
                case 126:
                case 167: // Scan
                case 134:
                case 142:
                case 152:
                case 127: // Poison
                case 153: // Mass-Poison
                case 162: // Confusion
                case 171: // Mass-Confusion
                case 136: // Cure
                    m_pEffectList[i]->m_cMaxFrame = 13;
                    m_pEffectList[i]->m_dwFrameTime = 120;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 150: // Berserk : Circle 6 magic 
                case 184: // Ultra Berserk
                case 180: // Illusion
                case 190: // Mass-Illusion
                    //Change
                case 177: // Illusion-Movement
                case 183: // Inhibition-Casting
                case 195: // Mass-Illusion-Movement
                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 100;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 113: // Defense-Shield
                case 144: // Great-Defense-Shield
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 120;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 111: // Stamina-Drain
                case 101: //Heal
                case 121: // Great Heal
                case 123: // Stamina-Recovery
                case 128: // Great-Stamina-Recovery
                case 178: // Super Heal
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 132: // Invisibility
                case 112: // Recall
                case 131: // Summon
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 110:
                    // Energy-Bolt
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 114: // Celebrating Light
                    bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), 0, 0, -12);
                    bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), 0, 0, -9);
                    bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), 0, 0, -6);
                    bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), 0, 0, -3);
                    bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), 0, 0, 0);
                    delete m_pEffectList[i];
                    m_pEffectList[i] = 0;
                    break;

                case 120:
                    // Fire Ball
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 130: // Fire Strike
                case 137: // Lightning Arrow
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 138:
                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist, 2);

                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);

                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);

                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 164: // worm-bite
                    PlaySound('E', 4, sDist, lPan);

                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);

                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);
                    bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, 0, 0, 0, 0);

                    m_pEffectList[i]->m_cMaxFrame = 1;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 166: // Armor Break
                    m_pEffectList[i]->m_cMaxFrame = 13;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 165: // Absolute-Magic-Protection
                    m_pEffectList[i]->m_cMaxFrame = 21;
                    m_pEffectList[i]->m_dwFrameTime = 70;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 143:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 50;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_rX = 5 - (rand() % 10);
                    m_pEffectList[i]->m_rY = 5 - (rand() % 10);
                    m_pEffectList[i]->m_cMaxFrame = 7;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    PlaySound('E', 40, sDist, lPan);
                    break;

                case 145:
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 147: // v1.4334 Triple-Energy-Bolt 
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    break;


                case 151:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 50;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_rX = 5 - (rand() % 10);
                    m_pEffectList[i]->m_rY = 5 - (rand() % 10);
                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    PlaySound('E', 40, sDist, lPan);
                    break;

                case 156: // Mass-Lightning-Arrow
                    m_pEffectList[i]->m_cMaxFrame = 3;
                    m_pEffectList[i]->m_dwFrameTime = 130;
                    break;

                case 157: // Ice-Strike
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 160: // Energy-Strike
                    m_pEffectList[i]->m_cMaxFrame = 7;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 161: // Mass-Fire-Strike
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 163: // Mass-Chill-Wind
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 170: // Bloody-Shock-Wave
                    m_pEffectList[i]->m_cMaxFrame = 7;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                case 172: // Mass-Ice-Strike
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 174: // Lightning-Strike
                    m_pEffectList[i]->m_cMaxFrame = 5;
                    m_pEffectList[i]->m_dwFrameTime = 120;
                    break;

                case 176: //Change - Cancellation 
                    m_pEffectList[i]->m_cMaxFrame = 23;
                    m_pEffectList[i]->m_dwFrameTime = 60;
                    sDist = sDist / 32;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 181:

                    m_pEffectList[i]->m_mX = dX * 32 + 300;
                    m_pEffectList[i]->m_mY = dY * 32 - 460;

                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 25;
                    break;

                case 182: //Change Mass-Magic-Missile
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;
                    PlaySound('E', 1, sDist, lPan);
                    break;

                    //hmm? \/
                case 244: //Change Mass MagicMissile
                    //case 184: //MassMM
                    m_pEffectList[i]->m_cMaxFrame = 29;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    break;

                case 191:
                    m_pEffectList[i]->m_cMaxFrame = 7;
                    m_pEffectList[i]->m_dwFrameTime = 80;
                    break;

                    //case 192: // Hero set Effect
                case 242: // Hero set Effect
                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                    //case 193: // Hero set Effect
                case 243: // Hero set Effect
                    m_pEffectList[i]->m_cMaxFrame = 19;
                    m_pEffectList[i]->m_dwFrameTime = 18;
                    break;

                case 194: // Resurrection
                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 40;
                    break;

                case 196: // Earth-Shock-Wave
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 30;
                    m_pEffectList[i]->m_dwFrameTime = 25;
                    sAbsX = abs((get_virtual_width() / 2) - (sX * 32 - m_sViewPointX));
                    sAbsY = abs((get_virtual_height() / 2) - (sY * 32 - m_sViewPointY));
                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;
                    SetCameraShakingEffect(sDist);
                    break;
                case 200: // 
                case 201: // 
                case 202: // 
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 25;
                    break;

                case 203: // 
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 18;
                    m_pEffectList[i]->m_dwFrameTime = 70;
                    break;

                case 204: // 
                case 205: // 
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 70;
                    break;

                case 206: // 
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 20;
                    m_pEffectList[i]->m_dwFrameTime = 70;
                    break;

                case 250: //
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                default:
                    delete m_pEffectList[i];
                    m_pEffectList[i] = 0;
                    break;
            }

            if (m_pEffectList[i] != 0)
            {
                m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
                m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;
            }

            return;
        }
}

bool CGame::_bGetIsStringIsNumber(char * pStr)
{
    int i;
    for (i = 0; i < (int)strlen(pStr); i++)
        if ((pStr[i] != '-') && ((pStr[i] < (char)'0') || (pStr[i] > (char)'9'))) return false;

    return true;
}

void CGame::RequestFullObjectData(uint16_t wObjectID)
{
    char    cMsg[256];
    int     iRet;
    uint32_t * dwp;
    uint16_t * wp;

    memset(cMsg, 0, sizeof(cMsg));

    dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
    *dwp = MSGID_REQUEST_FULLOBJECTDATA;
    wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
    *wp = wObjectID;

    iRet = send_message((char *)cMsg, 6);

    switch (iRet)
    {
        case DEF_XSOCKEVENT_SOCKETCLOSED:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_QUENEFULL:

            ChangeGameMode(DEF_GAMEMODE_ONCONNECTIONLOST);

            close(ix::WebSocketCloseConstants::kAbnormalCloseCode, ix::WebSocketCloseConstants::kAbnormalCloseMessage);
            break;
    }
}

void CGame::_ReadMapData(short sPivotX, short sPivotY, char * pData)
{
    int i{};
    char * cp, ucHeader{}, cDir{}, cName[12]{}, cItemColor{};
    short * sp, sTotal{}, sX{}, sY{}, sType{}, sAppr1{}, sAppr2{}, sAppr3{}, sAppr4{}, sItemSpr{}, sItemSprFrame{}, sDynamicObjectType{};
    int iStatus{};
    int * ip, iApprColor{};
    uint16_t wObjectID{};
    uint16_t * wp, wDynamicObjectID{};

    cp = pData;
    m_sVDL_X = sPivotX; // Valid Data Loc-X
    m_sVDL_Y = sPivotY;
    sp = (short *)cp;
    sTotal = *sp;
    cp += 2;

    for (i = 1; i <= sTotal; i++)
    {
        sp = (short *)cp;
        sX = *sp;
        cp += 2;
        sp = (short *)cp;
        sY = *sp;
        cp += 2;
        ucHeader = *cp;
        cp++;

        if (ucHeader & 0x01)
        {
            // object ID
            wp = (uint16_t *)cp;
            wObjectID = *wp;
            cp += 2;
            // object type
            sp = (short *)cp;
            sType = *sp;
            cp += 2;
            // dir
            cDir = *cp;
            cp++;

            if (wObjectID < 10000)
            {
                // Appearance1
                sp = (short *)cp;
                sAppr1 = *sp;
                cp += 2;
                // Appearance2
                sp = (short *)cp;
                sAppr2 = *sp;
                cp += 2;
                // Appearance3
                sp = (short *)cp;
                sAppr3 = *sp;
                cp += 2;
                // Appearance4
                sp = (short *)cp;
                sAppr4 = *sp;
                cp += 2;
                // v1.4 ApprColor
                ip = (int *)cp;
                iApprColor = *ip;
                cp += 4;
                // Status
                ip = (int *)cp;
                iStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 10);
                cp += 10;

                //ip  = (int *)cp;
                //m_stNPCList[wObjectID].m_iMaxHP = *ip;
                //cp += 4;

                //if (m_stNPCList[wObjectID].m_iMaxHP > 10000)
                //{
                //	m_stNPCList[wObjectID].m_iMaxHP = 0;
                //	//return;
                //}

                //ip  = (int *)cp;
                //m_stNPCList[wObjectID].m_iHP = *ip;
                //cp += 4;

                //if (m_stNPCList[wObjectID].m_iHP > 10000)
                //{
                //	m_stNPCList[wObjectID].m_iHP = 0;
                //	//return;
                //}

                //iMaxPoint = m_iVit*3 + m_iLevel*2 + m_iStr/2;
                //Change HP Bar
            }
            else
            {
                sAppr1 = sAppr3 = sAppr4 = 0;
                // Appearance2
                sp = (short *)cp;
                sAppr2 = *sp;
                cp += 2;
                // Status
                ip = (int *)cp;
                iStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 5);
                cp += 5;

                ip = (int *)cp;
                m_stNPCList[wObjectID].m_iMaxHP = *ip;
                cp += 4;

                if (m_stNPCList[wObjectID].m_iMaxHP > 10000)
                {
                    m_stNPCList[wObjectID].m_iMaxHP = 0;
                    //return;
                }

                ip = (int *)cp;
                m_stNPCList[wObjectID].m_iHP = *ip;
                cp += 4;

                if (m_stNPCList[wObjectID].m_iHP > 10000)
                {
                    m_stNPCList[wObjectID].m_iHP = 0;
                    //return;
                }

                //iMaxPoint = m_iVit*3 + m_iLevel*2 + m_iStr/2;
                //Change HP Bar
            }

            m_pMapData->bSetOwner(wObjectID, sPivotX + sX, sPivotY + sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, DEF_OBJECTSTOP, 0, 0, 0);
        }

        if (ucHeader & 0x02)
        {
            // object ID
            wp = (uint16_t *)cp;
            wObjectID = *wp;
            cp += 2;
            // object type
            sp = (short *)cp;
            sType = *sp;
            cp += 2;
            // dir
            cDir = *cp;
            cp++;
            if (wObjectID < 10000)
            {
                // Appearance1
                sp = (short *)cp;
                sAppr1 = *sp;
                cp += 2;
                // Appearance2
                sp = (short *)cp;
                sAppr2 = *sp;
                cp += 2;
                // Appearance3
                sp = (short *)cp;
                sAppr3 = *sp;
                cp += 2;
                // Appearance4
                sp = (short *)cp;
                sAppr4 = *sp;
                cp += 2;
                // v1.4 ApprColor
                ip = (int *)cp;
                iApprColor = *ip;
                cp += 4;
                // Status
                ip = (int *)cp;
                iStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 10);
                cp += 10;
            }
            else
            {
                sAppr1 = sAppr3 = sAppr4 = 0;
                // Appearance2
                sp = (short *)cp;
                sAppr2 = *sp;
                cp += 2;
                // Status
                ip = (int *)cp;
                iStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 5);
                cp += 5;
            }

            m_pMapData->bSetDeadOwner(wObjectID, sPivotX + sX, sPivotY + sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName);
        }

        if (ucHeader & 0x04)
        {
            sp = (short *)cp;
            sItemSpr = *sp;
            cp += 2;
            sp = (short *)cp;
            sItemSprFrame = *sp;
            cp += 2;
            cItemColor = *cp;
            cp++;

            m_pMapData->bSetItem(sPivotX + sX, sPivotY + sY, sItemSpr, sItemSprFrame, cItemColor, false);
        }

        if (ucHeader & 0x08)
        {
            wp = (uint16_t *)cp;
            wDynamicObjectID = *wp;
            cp += 2;

            sp = (short *)cp;
            sDynamicObjectType = *sp;
            cp += 2;

            m_pMapData->bSetDynamicObject(sPivotX + sX, sPivotY + sY, wDynamicObjectID, sDynamicObjectType, false);
        }
    }
}


int CGame::_iCheckLUS()
{
    //	if (m_iLU_Point != 0) return 1;//Change
    if ((m_iStr + m_cLU_Str) > m_iStatLimit) return 2;
    if ((m_iDex + m_cLU_Dex) > m_iStatLimit) return 2;
    if ((m_iInt + m_cLU_Int) > m_iStatLimit) return 2;
    if ((m_iVit + m_cLU_Vit) > m_iStatLimit) return 2;
    if ((m_iMag + m_cLU_Mag) > m_iStatLimit) return 2;
    if ((m_iCharisma + m_cLU_Char) > m_iStatLimit) return 2;//Change 200

    return 0;

}

void CGame::_InitOnCreateNewCharacter()
{
    m_cGender = rand() % 2 + 1;
    m_cSkinCol = rand() % 3 + 1;
    m_cHairStyle = rand() % 8;
    m_cHairCol = rand() % 16;
    m_cUnderCol = rand() % 8;
    m_ccStr = 10;
    m_ccVit = 10;
    m_ccDex = 10;
    m_ccInt = 10;
    m_ccMag = 10;
    m_ccChr = 10;
}

void CGame::ClearContents_OnCreateNewAccount()
{
    memset(m_cAccountPassword, 0, sizeof(m_cAccountPassword));
    memset(m_cAccountAge, 0, sizeof(m_cAccountAge));
    memset(m_cAccountCountry, 0, sizeof(m_cAccountCountry));
    memset(m_cAccountSSN, 0, sizeof(m_cAccountSSN));
    memset(m_cEmailAddr, 0, sizeof(m_cEmailAddr));
    //v1.4334 
    memset(m_cAccountQuiz, 0, sizeof(m_cAccountQuiz));
    memset(m_cAccountAnswer, 0, sizeof(m_cAccountAnswer));
}

void CGame::ChangeGameMode(char cMode)
{
    if (m_cGameMode == DEF_GAMEMODE_ONLOADING)
        window.setFramerateLimit(frame_limit);

    if (m_cGameMode == cMode) return;
    m_cGameMode = cMode;
    m_cGameModeCount = 0;
    m_dwTime = G_dwGlobalTime;

    if (cMode <= DEF_GAMEMODE_ONMAINMENU && !disconnecting)
    {
        switch (ws->getReadyState())
        {
            case ix::ReadyState::Open:
            case ix::ReadyState::Connecting:
                close(1000, "gamemodechange");
                break;
            case ix::ReadyState::Closing:
            case ix::ReadyState::Closed:
            default:
                break;
        }
    }
    disconnecting = false;

    {
        std::unique_lock<std::mutex> lock(thread_mut);
        if (mapdata_object_counter)
        {
            mapdata_object_counter->join();
            mapdata_object_counter.reset();
        }

        if (cMode == DEF_GAMEMODE_ONMAINGAME && !mapdata_object_counter)
        {
            mapdata_object_counter = std::make_unique<std::thread>([&]()
                {
                    try
                    {
                        while (get_game_mode() == DEF_GAMEMODE_ONMAINGAME)
                        {
                            {
                                std::unique_lock<std::mutex> l(screen_mtx);
                                iUpdateRet = m_pMapData->object_frame_counter(m_cPlayerName, m_sViewPointX, m_sViewPointY);
                                if (iUpdateRet == 0)
                                    iUpdateRet = -1;
                                if ((effect_frame_counter() == true) && (iUpdateRet == 0)) iUpdateRet = -1;
                                if (iUpdateRet == 2)
                                {
                                    m_bCommandAvailable = true;
                                    m_dwCommandTime = 0;
                                }

                                weather_object_frame_counter();
                            }
                            std::this_thread::sleep_for(10ms);
                        }
                    }
                    catch (std::exception & ex)
                    {
                        std::cout << ex.what() << '\n';
                    }
                });
        }
    }

    if (cMode == DEF_GAMEMODE_ONSELECTCHARACTER)
    {
        G_cSpriteAlphaDegree = 1;
        InitGameSettings();
    }
}

bool CGame::bReadLoginConfigFile(char * cFn)
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadMode;
    char seps[] = "= ,\t\n";

    cReadMode = 0;

    hFile = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0) return false;
    cp = new char[dwFileSize + 2];
    memset(cp, 0, dwFileSize + 2);
    fread(cp, dwFileSize, 1, pFile);

    token = strtok(cp, seps);
    while (token != 0)
    {
        if (cReadMode != 0)
        {
            switch (cReadMode)
            {
                case 1:
                    if (strlen(token) > 15)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    memset(m_cLogServerAddr, 0, sizeof(m_cLogServerAddr));
                    strcpy(m_cLogServerAddr, token);
                    cReadMode = 0;
                    break;
                case 2:
                    m_iLogServerPort = atoi(token);
                    if (m_iLogServerPort == 0)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    cReadMode = 0;
                    break;
                case 3:
                    if (strlen(token) > 31)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    memset(m_cWorldServerName, 0, sizeof(m_cWorldServerName));
                    strcpy(m_cWorldServerName, token);
                    cReadMode = 0;
                    break;
                case 4:
                    m_iStatLimit = atoi(token);
                    if ((m_iStatLimit <= 0) || (m_iStatLimit > 2000))
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    cReadMode = 0;
                    break;
#ifdef DEF_ADMINCLIENT
                case 5:
                    if (strlen(token) > 31)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    memset(m_cAdminPassword, 0, sizeof(m_cAdminPassword));
                    strcpy(m_cAdminPassword, token);
                    cReadMode = 0;
                    break;
#endif
                case 6:
                    if (strlen(token) > 10)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    memset(m_cAutoName, 0, sizeof(m_cAutoName));
                    memset(m_cAutoPass, 0, sizeof(m_cAutoPass));
                    strcpy(m_cAutoName, token);
                    token = strtok(0, seps);
                    if (!token)
                    {
                        delete[] cp;
                        if (pFile != 0) fclose(pFile);
                        return false;
                    }
                    strcpy(m_cAutoPass, token);
                    cReadMode = 0;
                    break;
            }
        }
        else
        {
            //if (memcmp(token, "log-server-address",18) == 0) cReadMode = 1;
            //if (memcmp(token, "log-server-port",15) == 0)    cReadMode = 2;
            //if (memcmp(token, "log-server-name",15) == 0)    cReadMode = 3;
            //if (memcmp(token, "max-stat-limit", 14) == 0)    cReadMode = 4;
#ifdef DEF_ADMINCLIENT
            if (memcmp(token, "admin-password", 14) == 0)    cReadMode = 5;
#endif
            if (memcmp(token, "auto-login", 10) == 0)    cReadMode = 6;
        }
        token = strtok(0, seps);
    }
    delete[] cp;
    if (pFile != 0) fclose(pFile);
    return true;
}

void CGame::ReleaseUnusedSprites()
{
    for (int i = 0; i < DEF_MAXSPRITES; i++)
        if (m_pSprite[i] != nullptr)
            if (m_pSprite[i]->m_bIsSurfaceEmpty == false)
                if ((G_dwGlobalTime - m_pSprite[i]->m_dwRefTime) > 60000)
                    m_pSprite[i]->close_sprite_();
    for (int i = 0; i < DEF_MAXTILES; i++)
        if (m_pTileSpr[i] != nullptr)
            if (m_pTileSpr[i]->m_bIsSurfaceEmpty == false)
                if ((G_dwGlobalTime - m_pTileSpr[i]->m_dwRefTime) > 60000)
                    m_pTileSpr[i]->close_sprite_();
    for (int i = 0; i < DEF_MAXEFFECTSPR; i++)
        if (m_pEffectSpr[i] != nullptr)
            if (m_pEffectSpr[i]->m_bIsSurfaceEmpty == false)
                if ((G_dwGlobalTime - m_pEffectSpr[i]->m_dwRefTime) > 60000)
                    m_pEffectSpr[i]->close_sprite_();
}

void CGame::PutChatScrollList(char * pMsg, char cType, int iGM)
{
    int i;

    if (m_pChatScrollList[DEF_MAXCHATSCROLLMSGS - 1] != 0)
    {
        delete m_pChatScrollList[DEF_MAXCHATSCROLLMSGS - 1];
        m_pChatScrollList[DEF_MAXCHATSCROLLMSGS - 1] = 0;
    }
    for (i = DEF_MAXCHATSCROLLMSGS - 2; i >= 0; i--)
    {
        m_pChatScrollList[i + 1] = m_pChatScrollList[i];
        m_pChatScrollList[i] = 0;
    }
    m_pChatScrollList[0] = new CMsg(1, pMsg, cType);

    m_pChatScrollList[0]->m_iGM = iGM;
}

void CGame::ReleaseTimeoverChatMsg()
{
    int i;
    uint32_t dwTime;

    dwTime = G_dwGlobalTime;

    for (i = 1; i < DEF_MAXCHATMSGS; i++)
        if (m_pChatMsgList[i] != 0)
        {

            if ((m_pChatMsgList[i]->m_cType >= 1) && (m_pChatMsgList[i]->m_cType <= 20))
            {
                if ((dwTime - m_pChatMsgList[i]->m_dwTime) > DEF_CHATTIMEOUT_A)
                {
                    delete m_pChatMsgList[i];
                    m_pChatMsgList[i] = 0;
                }
            }
            else
                if ((m_pChatMsgList[i]->m_cType >= 21) && (m_pChatMsgList[i]->m_cType <= 40))
                {
                    if ((dwTime - m_pChatMsgList[i]->m_dwTime) > DEF_CHATTIMEOUT_B)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = 0;
                    }
                }
                else
                    if ((m_pChatMsgList[i]->m_cType >= 41) && (m_pChatMsgList[i]->m_cType <= 60))
                    {
                        if ((dwTime - m_pChatMsgList[i]->m_dwTime) > DEF_CHATTIMEOUT_C)
                        {
                            delete m_pChatMsgList[i];
                            m_pChatMsgList[i] = 0;
                        }
                    }
                    else if ((dwTime - m_pChatMsgList[i]->m_dwTime) > DEF_CHATTIMEOUT_A)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = 0;
                    }
        }
}

bool CGame::effect_frame_counter()
{
    int i, x;
    uint32_t dwTime;
    bool bRet = false;
    short sAbsX, sAbsY, sDist{};
    char  cDir;
    long lPan;

    dwTime = m_dwCurTime;
    dwTime += m_pMapData->m_dwFrameAdjustTime;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if (m_pEffectList[i] != 0)
        {
            if ((dwTime - m_pEffectList[i]->m_dwTime) > m_pEffectList[i]->m_dwFrameTime)
            {
                m_pEffectList[i]->m_dwTime = dwTime;
                m_pEffectList[i]->m_cFrame++;
                bRet = true;

                m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
                m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;

                switch (m_pEffectList[i]->m_sType)
                {
                    case 1:
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {
                            for (int j = 1; j <= m_pEffectList[i]->m_iV1; j++) bAddNewEffect(11, m_pEffectList[i]->m_mX + 15 - (rand() % 30), m_pEffectList[i]->m_mY + 15 - (rand() % 30), 0, 0, -1 * (rand() % 2));
                        }
                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 2:
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32 - 40,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 70);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
                        {
                            bAddNewEffect(14, m_pEffectList[i]->m_mX + (rand() % 5) - 2, m_pEffectList[i]->m_mY + (rand() % 5) - 2, 0, 0, 0, 0);

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32 - 40,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);

                        bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + 10 - (rand() % 20), m_pEffectList[i]->m_mY + 10 - (rand() % 20), 0, 0, 0);//-1*(rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
                        {

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 4:
                        // Gold Drop 

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            //m_pMapData->bSetItem(m_pEffectList[i]->m_mX/32, m_pEffectList[i]->m_mY/32, 6, 0, 0); //v1.41
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }

                        break;

                    case 5:
                    case 30:
                    case 31:
                        // Fire Explosion

                        if (m_pEffectList[i]->m_cFrame == 1)
                        {
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                        }

                        if (m_pEffectList[i]->m_cFrame == 7)
                        {
                            bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
                            bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
                            bAddNewEffect(15, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, 0);
                        }

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 6:
                        // Lightning Bolt Burst
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                        }

                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }

                        break;

                    case 7:
                        // Magic Missile Burst
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                        }

                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }

                        break;

                    case 9:
                        // Burst Type 2
                        m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
                        m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;

                        m_pEffectList[i]->m_rY++;

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 10:
                        // Lightning Arrow Burst
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                        }

                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 11:
                        m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
                        m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;

                        m_pEffectList[i]->m_rY++;

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 12:
                        m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
                        m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 13:

                        if (m_pEffectList[i]->m_cFrame < 15)
                        {
                            if ((rand() % 2) == 0)
                                m_pEffectList[i]->m_mX++;
                            else m_pEffectList[i]->m_mX--;
                            m_pEffectList[i]->m_mY--;
                        }

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 16:

                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 40);

                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
                        {

                            bAddNewEffect(18, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0); // testcode 0111 18
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 20 - (rand() % 40), m_pEffectList[i]->m_mY + 20 - (rand() % 40), 0, 0, -1 * (rand() % 2));

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 34:

                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);

                        bAddNewEffect(33, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
                        {

                            bAddNewEffect(33, m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0, 0, 0); //7

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 17:
                        cDir = cGetNextMoveDir(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, m_pEffectList[i]->m_mX3, m_pEffectList[i]->m_mY3);

                        switch (cDir)
                        {
                            case 1:
                                m_pEffectList[i]->m_rY -= 2;
                                break;
                            case 2:
                                m_pEffectList[i]->m_rY -= 2;
                                m_pEffectList[i]->m_rX += 2;
                                break;
                            case 3:
                                m_pEffectList[i]->m_rX += 2;
                                break;
                            case 4:
                                m_pEffectList[i]->m_rX += 2;
                                m_pEffectList[i]->m_rY += 2;
                                break;
                            case 5:
                                m_pEffectList[i]->m_rY += 2;
                                break;
                            case 6:
                                m_pEffectList[i]->m_rX -= 2;
                                m_pEffectList[i]->m_rY += 2;
                                break;
                            case 7:
                                m_pEffectList[i]->m_rX -= 2;
                                break;
                            case 8:
                                m_pEffectList[i]->m_rX -= 2;
                                m_pEffectList[i]->m_rY -= 2;
                                break;
                        }

                        if (m_pEffectList[i]->m_rX < -10) m_pEffectList[i]->m_rX = -10;
                        if (m_pEffectList[i]->m_rX > 10) m_pEffectList[i]->m_rX = 10;
                        if (m_pEffectList[i]->m_rY < -10) m_pEffectList[i]->m_rY = -10;
                        if (m_pEffectList[i]->m_rY > 10) m_pEffectList[i]->m_rY = 10;

                        m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
                        m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;

                        m_pEffectList[i]->m_mY3--;

                        if (m_pEffectList[i]->m_cFrame > 10)
                        {
                            m_pEffectList[i]->m_cFrame = 0;
                            if (abs(m_pEffectList[i]->m_sY - m_pEffectList[i]->m_mY3) > 100)
                            {
                                delete m_pEffectList[i];
                                m_pEffectList[i] = 0;
                            }
                        }
                        break;

                    case 40:
                    case 56:
                        if (m_pEffectList[i]->m_cFrame == 9)
                        {
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 100) - 50), m_pEffectList[i]->m_mY + ((rand() % 70) - 35), 0, 0, 0, 0);
                        }

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 41:
                    case 42:
                    case 43:
                    case 44:

                    case 45:
                    case 46:

                        if (m_pEffectList[i]->m_cFrame >= 7)
                        {
                            m_pEffectList[i]->m_mX--;
                            m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
                            m_pEffectList[i]->m_iV1++;
                        }

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            if ((m_pEffectList[i]->m_sType != 45) && (m_pEffectList[i]->m_sType != 46))
                            {
                                bAddNewEffect(50, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);

                                bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                                bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                                bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);

                                bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                                bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                            }
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;
                    case 47:
                    case 48:
                    case 49:

                        if (m_pEffectList[i]->m_cFrame >= 7)
                        {
                            m_pEffectList[i]->m_mX--;
                            m_pEffectList[i]->m_mY += m_pEffectList[i]->m_iV1;
                            m_pEffectList[i]->m_iV1 += 4;
                        }

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            if (m_pEffectList[i]->m_sType == 49)
                                bAddNewEffect(72, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);
                            else bAddNewEffect(50, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0);

                            bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                            bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                            bAddNewEffect(14, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);

                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                            bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 181:
                    case 60:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {

                            bAddNewEffect(61, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
                            bAddNewEffect(63, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);

                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(12, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else if (m_pEffectList[i]->m_cFrame >= 0)
                        {
                            m_pEffectList[i]->m_mX -= 30;
                            m_pEffectList[i]->m_mY += 46;

                            // ¿¬±â ±¸¸§ »ý¼º 
                            bAddNewEffect(62, m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, 0, 0, 0, 0);
                        }
                        break;

                    case 62:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else if (m_pEffectList[i]->m_cFrame >= 0)
                        {
                            m_pEffectList[i]->m_mX += (rand() % 3) - 1;
                            m_pEffectList[i]->m_mY += (rand() % 3) - 1;
                        }
                        break;

                    case 65:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else if (m_pEffectList[i]->m_cFrame >= 0)
                        {
                            m_pEffectList[i]->m_mX += (rand() % 3) - 1;
                            m_pEffectList[i]->m_mY -= 4 + (rand() % 2);
                        }
                        break;

                    case 66:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 68:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else if (m_pEffectList[i]->m_cFrame == 11)
                        {
                            SetCameraShakingEffect(m_pEffectList[i]->m_iV1, 2);
                        }
                        break;
                    case 69:
                    case 70:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;
                    case 71:

                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);


                        bAddNewEffect(48, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0);
                        bAddNewEffect(51, m_pEffectList[i]->m_mX + ((rand() % 20) - 10), m_pEffectList[i]->m_mY + ((rand() % 20) - 10), 0, 0, 0, 0);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY)) <= 2))
                        {

                            bAddNewEffect(49, m_pEffectList[i]->m_mX/* + (rand() % 30) - 15*/, m_pEffectList[i]->m_mY/* + (rand() % 30) - 15*/, 0, 0, 0);

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 81: // StromBlade
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX
                            , m_pEffectList[i]->m_mY
                            , m_pEffectList[i]->m_dX * 32
                            , m_pEffectList[i]->m_dY * 32
                            , &m_pEffectList[i]->m_mX
                            , &m_pEffectList[i]->m_mY
                            , &m_pEffectList[i]->m_iErr
                            , 10);
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;


                    case 100:
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);

                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32/* - 40*/)) <= 2))
                        {

                            bAddNewEffect(7, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 110:
                        // Enegy-Bolt
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);

                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {

                            bAddNewEffect(6, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0); // 6 testcode 0111

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 120:
                        // Fire Ball
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {

                            bAddNewEffect(5, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 130:
                        // Fire Strike
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {

                            bAddNewEffect(5, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                            bAddNewEffect(5, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7);
                            bAddNewEffect(5, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5);
                            bAddNewEffect(5, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 137:
                        // Lightning Arrow
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);

                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {

                            bAddNewEffect(10, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 145:
                        bAddNewEffect(40, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                        bAddNewEffect(40, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -10);
                        bAddNewEffect(40, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -6);
                        bAddNewEffect(40, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3);
                        delete m_pEffectList[i];
                        m_pEffectList[i] = 0;
                        break;

                    case 147:  // v1.4334 Triple-Energy-Bolt 
                        bAddNewEffect(110, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                            m_pEffectList[i]->m_dX - 1, m_pEffectList[i]->m_dY - 1, 0);

                        bAddNewEffect(110, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                            m_pEffectList[i]->m_dX + 1, m_pEffectList[i]->m_dY - 1, 0);

                        bAddNewEffect(110, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                            m_pEffectList[i]->m_dX + 1, m_pEffectList[i]->m_dY + 1, 0);


                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));


                        lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX) * 1000;
                        PlaySound('E', 1, sDist, lPan);

                        bAddNewEffect(7, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);


                        delete m_pEffectList[i];
                        m_pEffectList[i] = 0;

                        break;


                    case 143: // Lightning
                    case 151: // Lightning-Bolt
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            bAddNewEffect(10, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            m_pEffectList[i]->m_rX = 5 - (rand() % 10);
                            m_pEffectList[i]->m_rY = 5 - (rand() % 10);
                        }
                        break;

                    case 156: // Mass-Lightnig-Arrow
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(137, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                                m_pEffectList[i]->m_dX, m_pEffectList[i]->m_dY, 0);

                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);

                            if (sAbsX > sAbsY) sDist = sAbsX;
                            else sDist = sAbsY;

                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX) * 1000;
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 157:
                        bAddNewEffect(41, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                        for (x = 0; x < 14; x++)
                        {
                            bAddNewEffect(41 + (rand() % 3), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1);
                        }

                        for (x = 0; x < 6; x++)
                        {
                            bAddNewEffect(45 + (rand() % 2), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1 - 10);
                        }

                        delete m_pEffectList[i];
                        m_pEffectList[i] = 0;
                        break;

                    case 160: // Energy-Strike
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(16, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                                m_pEffectList[i]->m_dX * 32 + 50 - (rand() % 100), m_pEffectList[i]->m_dY * 32 + 50 - (rand() % 100), 0);

                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);

                            if (sAbsX > sAbsY) sDist = sAbsX;
                            else sDist = sAbsY;

                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 161:
                        // Mass-Fire-Strike
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY,
                            m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32/* - 40*/,
                            &m_pEffectList[i]->m_mX, &m_pEffectList[i]->m_mY,
                            &m_pEffectList[i]->m_iErr, 50);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2) &&
                            (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {

                            bAddNewEffect(30, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                            bAddNewEffect(31, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7);
                            bAddNewEffect(31, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5);
                            bAddNewEffect(31, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 163: // Mass-Chill-Wind
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -10);
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -6);
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3);

                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));
                        bAddNewEffect(56, m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50, m_pEffectList[i]->m_dY * 32 + (rand() % 70) - 35, 0, 0, -1 * (rand() % 10));

                        delete m_pEffectList[i];
                        m_pEffectList[i] = 0;
                        break;

                    case 164: // worm-bite
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            bAddNewEffect(68, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0); // testcode 0111 18
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 170:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else if ((m_pEffectList[i]->m_cFrame % 2) == 0)
                        {

                            bAddNewEffect(34, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                                m_pEffectList[i]->m_dX * 32 + 30 - (rand() % 60), m_pEffectList[i]->m_dY * 32 + 30 - (rand() % 60), 0);

                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);

                            if (sAbsX > sAbsY) sDist = sAbsX;
                            else sDist = sAbsY;

                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 172:
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0);

                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));
                        bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * (rand() % 3));

                        for (x = 0; x < 16; x++)
                        {
                            bAddNewEffect(44, m_pEffectList[i]->m_dX * 32 + (rand() % 110) - 55 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 100) - 50, 0, 0, -1 * x - 1);
                        }

                        for (x = 0; x < 8; x++)
                        {
                            bAddNewEffect(45 + (rand() % 2), m_pEffectList[i]->m_dX * 32 + (rand() % 100) - 50 + 10, m_pEffectList[i]->m_dY * 32 + (rand() % 90) - 45, 0, 0, -1 * x - 1 - 10);
                        }

                        delete m_pEffectList[i];
                        m_pEffectList[i] = 0;
                        break;


                    case 174: // Lightning-Strike
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(151, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                                m_pEffectList[i]->m_dX + (rand() % 3) - 1, m_pEffectList[i]->m_dY + (rand() % 3) - 1, 0);

                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);

                            if (sAbsX > sAbsY) sDist = sAbsX;
                            else sDist = sAbsY;

                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 182: // Mass-Magic-Missile
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX
                            , m_pEffectList[i]->m_mY
                            , m_pEffectList[i]->m_dX * 32
                            , m_pEffectList[i]->m_dY * 32
                            , &m_pEffectList[i]->m_mX
                            , &m_pEffectList[i]->m_mY
                            , &m_pEffectList[i]->m_iErr
                            , 50);
                        bAddNewEffect(8, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, -1 * (rand() % 4));
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
                            && (abs(m_pEffectList[i]->m_mY - m_pEffectList[i]->m_dY * 32) <= 2))
                        {	// JLE 0043132A				
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(35, m_pEffectList[i]->m_dX * 32 + 22, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 1);
                            bAddNewEffect(36, m_pEffectList[i]->m_dX * 32 - 22, m_pEffectList[i]->m_dY * 32 - 7, 0, 0, -7, 1);
                            bAddNewEffect(36, m_pEffectList[i]->m_dX * 32 + 30, m_pEffectList[i]->m_dY * 32 - 22, 0, 0, -5, 1);
                            bAddNewEffect(36, m_pEffectList[i]->m_dX * 32 + 12, m_pEffectList[i]->m_dY * 32 + 22, 0, 0, -3, 1);
                        }
                        break;
                    case 191:

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else /*if (m_pEffectList[i]->m_cFrame == 1)*/
                        {

                            bAddNewEffect(71, m_pEffectList[i]->m_sX, m_pEffectList[i]->m_sY,
                                m_pEffectList[i]->m_dX * 32 + (rand() % 120) - 60, m_pEffectList[i]->m_dY * 32 + (rand() % 120) - 60, 0);

                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);

                            if (sAbsX > sAbsY) sDist = sAbsX;
                            else sDist = sAbsY;

                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 196: // Earth-Shock-Wave
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX
                            , m_pEffectList[i]->m_mY
                            , m_pEffectList[i]->m_dX * 32
                            , m_pEffectList[i]->m_dY * 32
                            , &m_pEffectList[i]->m_mX
                            , &m_pEffectList[i]->m_mY
                            , &m_pEffectList[i]->m_iErr
                            , 40);
                        bAddNewEffect(80, m_pEffectList[i]->m_mX + (rand() % 30) - 15, m_pEffectList[i]->m_mY + (rand() % 30) - 15, 0, 0, 0, 1);
                        bAddNewEffect(80, m_pEffectList[i]->m_mX + (rand() % 20) - 10, m_pEffectList[i]->m_mY + (rand() % 20) - 10, 0, 0, 0, 0);
                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            sAbsX = abs(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            sAbsY = abs(((m_sViewPointY / 32) + 7) - m_pEffectList[i]->m_dY);
                            if (sAbsX > sAbsY) sDist = sAbsX - 10;
                            else sDist = sAbsY - 10;
                            lPan = -(((m_sViewPointX / 32) + 10) - m_pEffectList[i]->m_dX);
                            PlaySound('E', 1, sDist, lPan);
                        }
                        break;

                    case 200:
                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(203, m_pEffectList[i]->m_sX + 40, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
                            bAddNewEffect(204, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
                            bAddNewEffect(205, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
                            bAddNewEffect(206, m_pEffectList[i]->m_sX - 7, m_pEffectList[i]->m_sY + 27, 0, 0, 0, 0);
                            bAddNewEffect(201, (rand() % 160) + get_virtual_width() / 2, (rand() % 120) + get_virtual_height() / 2, 0, 0, 0, 1);
                            bAddNewEffect(202, (rand() % 160) + get_virtual_width() / 2, (rand() % 120) + get_virtual_height() / 2, 0, 0, 0, 1);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 201:
                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(203, m_pEffectList[i]->m_sX + 110, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
                            bAddNewEffect(204, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
                            bAddNewEffect(205, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
                            bAddNewEffect(202, (rand() % 160) + get_virtual_width() / 2, (rand() % 120) + get_virtual_height() / 2, 0, 0, 0, 1);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 202:
                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(203, m_pEffectList[i]->m_sX + 65, m_pEffectList[i]->m_sY + 120, 0, 0, 0, 0);
                            bAddNewEffect(204, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 70, 0, 0, 0, 0);
                            bAddNewEffect(205, m_pEffectList[i]->m_sX - 10, m_pEffectList[i]->m_sY + 75, 0, 0, 0, 0);
                            bAddNewEffect(206, m_pEffectList[i]->m_sX - 7, m_pEffectList[i]->m_sY + 27, 0, 0, 0, 0);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 250: // Gate round
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX
                            , m_pEffectList[i]->m_mY
                            , m_pEffectList[i]->m_dX * 32
                            , m_pEffectList[i]->m_dY * 32 - 40
                            , &m_pEffectList[i]->m_mX
                            , &m_pEffectList[i]->m_mY
                            , &m_pEffectList[i]->m_iErr
                            , 10);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
                            && (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 251: // Salmon burst (effect11s)
                        m_Misc.GetPoint(m_pEffectList[i]->m_mX
                            , m_pEffectList[i]->m_mY
                            , m_pEffectList[i]->m_dX * 32
                            , m_pEffectList[i]->m_dY * 32
                            , &m_pEffectList[i]->m_mX
                            , &m_pEffectList[i]->m_mY
                            , &m_pEffectList[i]->m_iErr
                            , 50);
                        if ((abs(m_pEffectList[i]->m_mX - m_pEffectList[i]->m_dX * 32) <= 2)
                            && (abs(m_pEffectList[i]->m_mY - (m_pEffectList[i]->m_dY * 32 - 40)) <= 2))
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        else
                        {
                            bAddNewEffect(252, m_pEffectList[i]->m_dX * 32, m_pEffectList[i]->m_dY * 32, 0, 0, 0, 1);
                            bAddNewEffect(252, m_pEffectList[i]->m_dX * 32 - 30, m_pEffectList[i]->m_dY * 32 - 15, 0, 0, -7, 1);
                            bAddNewEffect(252, m_pEffectList[i]->m_dX * 32 - 35, m_pEffectList[i]->m_dY * 32 - 30, 0, 0, -5, 1);
                            bAddNewEffect(252, m_pEffectList[i]->m_dX * 32 + 20, m_pEffectList[i]->m_dY * 32 + 30, 0, 0, -3, 1);
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 33:
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;

                    case 8:
                    case 14:
                    case 15:
                    case 18:
                    case 32:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 57:
                    case 61:
                    case 63:
                    case 64:
                    case 67:
                    case 72:
                    case 73:
                    case 74:
                    case 75:
                    case 76:
                    case 77:
                    case 79:

                    case 101:
                    case 102:
                    case 111:
                    case 112:
                    case 113:
                    case 121:
                    case 122:
                    case 123:
                    case 124:
                    case 125:
                    case 126:
                    case 127:
                    case 128:
                    case 131:
                    case 132:
                    case 133:
                    case 134:
                    case 135:
                    case 136:
                    case 142:
                    case 144:
                    case 150: // Berserk : Circle 6 magic 
                    case 152:
                    case 153:
                    case 162:
                    case 165:
                    case 166:
                    case 167: // Scan
                    case 171:
                    case 178: // Super Heal
                    case 180:
                    case 184: // Ultra Berserk
                    case 190:

                    case 35: //
                    case 36: //
                    case 80: //
                    case 82: // 
                    case 176: // 
                    case 177: // 
                    case 183: // 
                    case 192:
                    case 193:
                    case 194:
                    case 195:
                    case 242: // Mage hero effect
                    case 243: // War hero effect
                    case 244: // Mass MagicMissile
                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {
                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
                        break;
                }
            }
        }

    return bRet;
}


void CGame::GetPlayerTurn()
{
    char cDir;
    short sX, sY, sCnt1, sCnt2;
    int   iError;

    sX = m_sPlayerX;
    sY = m_sPlayerY;
    sCnt1 = 0;
    m_cPlayerTurn = 0;
    iError = 0;
    while (1)
    {
        cDir = cGetNextMoveDir(sX, sY, m_sCommX, m_sCommY);
        if (cDir == 0) break;
        switch (cDir)
        {
            case 1: sY--;       break;
            case 2: sX++; sY--; break;
            case 3: sX++;       break;
            case 4: sX++; sY++; break;
            case 5: sY++;       break;
            case 6: sX--; sY++; break;
            case 7: sX--;       break;
            case 8: sX--; sY--; break;
        }
        sCnt1++;
        if (sCnt1 > 30) break;
    }

    sX = m_sPlayerX;
    sY = m_sPlayerY;
    sCnt2 = 0;
    m_cPlayerTurn = 1;
    iError = 0;
    while (1)
    {
        cDir = cGetNextMoveDir(sX, sY, m_sCommX, m_sCommY);
        if (cDir == 0) break;
        switch (cDir)
        {
            case 1: sY--;       break;
            case 2: sX++; sY--; break;
            case 3: sX++;       break;
            case 4: sX++; sY++; break;
            case 5: sY++;       break;
            case 6: sX--; sY++; break;
            case 7: sX--;       break;
            case 8: sX--; sY--; break;
        }
        sCnt2++;
        if (sCnt2 > 30) break;
    }

    if (sCnt1 > sCnt2)
        m_cPlayerTurn = 0;
    else m_cPlayerTurn = 1;
}

void CGame::EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, char * pString)
{
    int i;
    short sX, sY;
    // char cTemp[80];

        //memset(cTemp, 0, sizeof(cTemp));
        //format_to_local(cTemp, "EnableDialogBox(iBoxID:{}, cType:{}, sV1:{}, sV2:{}, pString:{})", iBoxID, cType, sV1, sV2, pString);
        //AddEventList(cTemp);
    switch (iBoxID)
    {
        case 11:
            if (m_bIsDialogEnabled[11] == false)
            {
                switch (cType)
                {
                    case 0:
                        break;
                    default:
                        _LoadShopMenuContents(cType);
                        m_stDialogBoxInfo[11].sV1 = cType;
                        m_stDialogBoxInfo[11].cMode = 0;
                        m_stDialogBoxInfo[11].sView = 0;
                        m_stDialogBoxInfo[11].bFlag = true;
                        m_stDialogBoxInfo[11].sV3 = 1;
                        break;
                }
            }
            break;

        case 12:
            if (m_bIsDialogEnabled[12] == false)
            {
                m_stDialogBoxInfo[12].sX = m_stDialogBoxInfo[1].sX + 20;
                m_stDialogBoxInfo[12].sY = m_stDialogBoxInfo[1].sY + 20;
            }
            break;

        case 3:
            break;

        case 4:
            if (m_bIsDialogEnabled[4] == false)
            {
                m_stDialogBoxInfo[4].sView = cType;
            }
            break;

        case 5:
            if (m_bIsDialogEnabled[5] == false)
            {
                m_stDialogBoxInfo[5].sView = cType;
            }
            break;

        case 6:
            if (m_bIsDialogEnabled[6] == false)
            {
                m_stDialogBoxInfo[6].sView = cType;
            }
            break;

        case 7:
            if (m_stDialogBoxInfo[7].cMode == 1)
            {
                sX = m_stDialogBoxInfo[7].sX;
                sY = m_stDialogBoxInfo[7].sY;
                EndInputString();
                StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
            }
            break;

        case 17:
            if (m_bIsDialogEnabled[17] == false)
            {
                m_stDialogBoxInfo[iBoxID].cMode = 1;
                m_stDialogBoxInfo[17].sView = cType;
                EndInputString();
                memset(m_cAmountString, 0, sizeof(m_cAmountString));
                format_to_local(m_cAmountString, "{}", sV1);

                sX = m_stDialogBoxInfo[17].sX;
                sY = m_stDialogBoxInfo[17].sY;
                StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
            }
            else
            {
                if (m_stDialogBoxInfo[17].cMode == 1)
                {
                    sX = m_stDialogBoxInfo[17].sX;
                    sY = m_stDialogBoxInfo[17].sY;
                    EndInputString();
                    StartInputString(sX + 40, sY + 57, 11, m_cAmountString, false);
                }
            }
            break;

        case 18:
            if (m_bIsDialogEnabled[18] == false)
            {
                switch (cType)
                {
                    case 0:
                        m_stDialogBoxInfo[18].cMode = 0;
                        m_stDialogBoxInfo[18].sView = 0;
                        break;
                    default:
                        _LoadTextDlgContents(cType);
                        m_stDialogBoxInfo[18].cMode = 0;
                        m_stDialogBoxInfo[18].sView = 0;
                        break;
                }
            }
            break;

        case 19:
            break;

        case 20:
            // v1.41
            m_bIsItemDisabled[m_stDialogBoxInfo[20].sV1] = false;

            if (m_bIsDialogEnabled[20] == false)
            {
                m_stDialogBoxInfo[11].sV1 = m_stDialogBoxInfo[11].sV2 = m_stDialogBoxInfo[11].sV3 =
                    m_stDialogBoxInfo[11].sV4 = m_stDialogBoxInfo[11].sV5 = m_stDialogBoxInfo[11].sV6 = 0;

                m_stDialogBoxInfo[20].cMode = cType;
                m_stDialogBoxInfo[20].sView = 0;
                m_stDialogBoxInfo[20].sV1 = sV1;
                m_stDialogBoxInfo[20].sV2 = sV2;
            }
            break;

        case 21:
            if (m_bIsDialogEnabled[21] == false)
            {
                m_stDialogBoxInfo[21].cMode = cType;
                m_stDialogBoxInfo[21].sView = 0;
                m_stDialogBoxInfo[21].sV1 = _iLoadTextDlgContents2(sV1 + 20);
                m_stDialogBoxInfo[21].sV2 = sV1 + 20;
            }
            break;

        case 22:
            if (m_bIsDialogEnabled[22] == false)
            {
                m_stDialogBoxInfo[22].sV1 = sV1;
                m_stDialogBoxInfo[22].sV2 = sV2;

                m_stDialogBoxInfo[22].sSizeX = 290;
                m_stDialogBoxInfo[22].sSizeY = 290;
            }
            break;

        case 23:
            if (m_bIsDialogEnabled[23] == false)
            {
                m_stDialogBoxInfo[23].cMode = cType;
                m_stDialogBoxInfo[23].sV1 = sV1;		// ItemID
                m_stDialogBoxInfo[23].sV2 = sV2;
                if (cType == 2)
                {
                    m_stDialogBoxInfo[23].sX = m_stDialogBoxInfo[11].sX;
                    m_stDialogBoxInfo[23].sY = m_stDialogBoxInfo[11].sY;
                }
            }
            break;

        case 15:
            break;

        case 24:
            if (m_bIsDialogEnabled[24] == false)
            {
                m_stDialogBoxInfo[24].cMode = cType;
                m_stDialogBoxInfo[24].sV1 = sV1;
                m_stDialogBoxInfo[24].sV2 = sV2;
                m_bSkillUsingStatus = true;
            }
            break;

        case 25:
            if (m_bIsDialogEnabled[25] == false)
            {
                m_stDialogBoxInfo[25].cMode = cType;
                m_stDialogBoxInfo[25].sV1 = sV1;
                m_stDialogBoxInfo[25].sV2 = sV2;
            }
            break;

        case 26:
            switch (cType)
            {
                case 1:
                case 2:
                    if (m_bIsDialogEnabled[26] == false)
                    {
                        m_stDialogBoxInfo[26].cMode = cType;
                        m_stDialogBoxInfo[26].sV1 = -1;
                        m_stDialogBoxInfo[26].sV2 = -1;
                        m_stDialogBoxInfo[26].sV3 = -1;
                        m_stDialogBoxInfo[26].sV4 = -1;
                        m_stDialogBoxInfo[26].sV5 = -1;
                        m_stDialogBoxInfo[26].sV6 = -1;
                        m_stDialogBoxInfo[26].cStr[0] = 0;
                        m_bSkillUsingStatus = true;
                        m_stDialogBoxInfo[26].sSizeX = 195;
                        m_stDialogBoxInfo[26].sSizeY = 215;
                        DisableDialogBox(17);
                        DisableDialogBox(20);
                        DisableDialogBox(23);
                    }
                    break;

                case 3:
                    if (m_bIsDialogEnabled[26] == false)
                    {
                        m_stDialogBoxInfo[26].sView = 0;
                        m_stDialogBoxInfo[26].cMode = cType;
                        m_stDialogBoxInfo[26].sV1 = -1;
                        m_stDialogBoxInfo[26].sV2 = -1;
                        m_stDialogBoxInfo[26].sV3 = -1;
                        m_stDialogBoxInfo[26].sV4 = -1;
                        m_stDialogBoxInfo[26].sV5 = -1;
                        m_stDialogBoxInfo[26].sV6 = -1;
                        m_stDialogBoxInfo[26].cStr[0] = 0;
                        m_stDialogBoxInfo[26].cStr[1] = 0;
                        m_stDialogBoxInfo[26].cStr[4] = 0;
                        m_bSkillUsingStatus = true;
                        _bCheckBuildItemStatus();
                        //m_stDialogBoxInfo[26].sX = 0;
                        //m_stDialogBoxInfo[26].sY = 0;

                        m_stDialogBoxInfo[26].sSizeX = 270;
                        m_stDialogBoxInfo[26].sSizeY = 381;

                        DisableDialogBox(17);
                        DisableDialogBox(20);
                        DisableDialogBox(23);
                    }
                    break;

                case 6:
                    if (m_bIsDialogEnabled[26] == false)
                    {

                        m_stDialogBoxInfo[26].cMode = cType;

                        m_stDialogBoxInfo[26].cStr[2] = sV1;
                        m_stDialogBoxInfo[26].cStr[3] = sV2;

                        m_stDialogBoxInfo[26].sSizeX = 270;
                        m_stDialogBoxInfo[26].sSizeY = 381;

                        m_bSkillUsingStatus = true;
                        _bCheckBuildItemStatus();

                        DisableDialogBox(17);
                        DisableDialogBox(20);
                        DisableDialogBox(23);
                    }
                    break;
                    //Change Crafting
                case 7:
                case 8:
                    if (m_bIsDialogEnabled[26] == false)
                    {
                        m_stDialogBoxInfo[26].cMode = cType;
                        m_stDialogBoxInfo[26].sV1 = -1;
                        m_stDialogBoxInfo[26].sV2 = -1;
                        m_stDialogBoxInfo[26].sV3 = -1;
                        m_stDialogBoxInfo[26].sV4 = -1;
                        m_stDialogBoxInfo[26].sV5 = -1;
                        m_stDialogBoxInfo[26].sV6 = -1;
                        m_stDialogBoxInfo[26].cStr[0] = 0;
                        m_stDialogBoxInfo[26].cStr[1] = 0;
                        m_bSkillUsingStatus = true;
                        //_bCheckCraftItemStatus();
                        m_stDialogBoxInfo[26].sSizeX = 195;
                        m_stDialogBoxInfo[26].sSizeY = 215;
                        DisableDialogBox(17);
                        DisableDialogBox(20);
                        DisableDialogBox(23);
                    }
                    break;
            }
            break;

        case 27: //Change multitrade
            if (m_bIsDialogEnabled[27] == false)
            {
                m_stDialogBoxInfo[27].cMode = cType;
                for (i = 0; i < 8; i++)
                {
                    memset(m_stDialogBoxExchangeInfo[i].cStr1, 0, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
                    memset(m_stDialogBoxExchangeInfo[i].cStr2, 0, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
                    m_stDialogBoxExchangeInfo[i].sV1 = -1;
                    m_stDialogBoxExchangeInfo[i].sV2 = -1;
                    m_stDialogBoxExchangeInfo[i].sV3 = -1;
                    m_stDialogBoxExchangeInfo[i].sV4 = -1;
                    m_stDialogBoxExchangeInfo[i].sV5 = -1;
                    m_stDialogBoxExchangeInfo[i].sV6 = -1;
                    m_stDialogBoxExchangeInfo[i].sV7 = -1;
                    m_stDialogBoxExchangeInfo[i].dwV1 = 0;
                }
                DisableDialogBox(17);
                DisableDialogBox(20);
                DisableDialogBox(23);
                DisableDialogBox(26);
            }
            break;

        case 41: // multitrade confirm
            break;

        case 28:
            if (m_bIsDialogEnabled[28] == false)
            {
                m_stDialogBoxInfo[28].cMode = cType;
                m_stDialogBoxInfo[28].sX = m_stDialogBoxInfo[1].sX + 20;
                m_stDialogBoxInfo[28].sY = m_stDialogBoxInfo[1].sY + 20;
            }
            break;

        case 32:
            if (m_bIsDialogEnabled[32] == false)
            {
                m_stDialogBoxInfo[32].cMode = cType;
                m_stDialogBoxInfo[32].sX = m_stDialogBoxInfo[1].sX + 20;
                m_stDialogBoxInfo[32].sY = m_stDialogBoxInfo[1].sY + 20;
            }
            break;

        case 33:
            if ((m_iHP <= 0) || (m_bCitizen == false)) return;

            if (m_bIsDialogEnabled[33] == false)
            {
                m_stDialogBoxInfo[33].cMode = cType;
                m_stDialogBoxInfo[33].sX = 360;
                m_stDialogBoxInfo[33].sY = 65;
                m_stDialogBoxInfo[33].sV1 = sV1;
            }
            break;

        case 34:
            if (m_bIsDialogEnabled[34] == false)
            {
                m_stDialogBoxInfo[34].cMode = 5;
                m_stDialogBoxInfo[34].sV2 = -1;
                m_stDialogBoxInfo[34].sV3 = -1;
                m_stDialogBoxInfo[34].sV1 = -1;
                m_stDialogBoxInfo[34].dwV1 = 0;

                /*int iSoX, iSoM;
                iSoX = iSoM = 0;
                for (i = 0; i < DEF_MAXITEMS; i++)
                    if (m_pItemList[i] != 0)
                    {	if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 128)) iSoX++;
                if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 129)) iSoM++;
                    }*/
                    //if ((iSoX > 0) || (iSoM > 0))
                    //{
                    //	m_stDialogBoxInfo[34].cMode = 6; // Stone upgrade
                    //	m_stDialogBoxInfo[34].sV2 = iSoX;
                    //	m_stDialogBoxInfo[34].sV3 = iSoM;
                    //	m_stDialogBoxInfo[34].sV1  = -1;
                    //	m_stDialogBoxInfo[34].dwV1 = 0;
                    //}
                    //else if (m_iGizonItemUpgradeLeft >0)
                    //{
                    //	m_stDialogBoxInfo[34].cMode = 1;
                    //	m_stDialogBoxInfo[34].sV2 = -1;
                    //	m_stDialogBoxInfo[34].sV3 = -1;
                    //	m_stDialogBoxInfo[34].sV1  = -1;
                    //	m_stDialogBoxInfo[34].dwV1 = 0;
                    //}
                    //else
                    //{
                    //	AddEventList(DRAW_DIALOGBOX_ITEMUPGRADE30, 10); // "Stone of Xelima or Merien is not present."
                    //	return;
                    //}
            }
            break;

        case 16:

            if (m_bIsDialogEnabled[iBoxID] == false)
            {
                if (m_cSkillMastery[4] == 0)
                {
                    DisableDialogBox(16);
                    EnableDialogBox(21, 0, 480, 0);
                    return;
                }
                else
                {
                    m_stDialogBoxInfo[iBoxID].cMode = 0;
                    m_stDialogBoxInfo[iBoxID].sView = 0;
                }
            }
            break;

        case 14:
            EndInputString();
            if (m_bIsDialogEnabled[iBoxID] == false)
            {
                m_stDialogBoxInfo[iBoxID].cMode = 0;
                m_stDialogBoxInfo[iBoxID].sView = 0;
                EnableDialogBox(2, 0, 0, 0);
            }
            break;

        case 40: // Slates
            if (m_bIsDialogEnabled[40] == false)
            {
                m_stDialogBoxInfo[40].sView = 0;
                m_stDialogBoxInfo[40].cMode = cType;
                m_stDialogBoxInfo[40].sV1 = -1;
                m_stDialogBoxInfo[40].sV2 = -1;
                m_stDialogBoxInfo[40].sV3 = -1;
                m_stDialogBoxInfo[40].sV4 = -1;
                m_stDialogBoxInfo[40].sV5 = -1;
                m_stDialogBoxInfo[40].sV6 = -1;
                m_stDialogBoxInfo[40].cStr[0] = 0;
                m_stDialogBoxInfo[40].cStr[1] = 0;
                m_stDialogBoxInfo[40].cStr[4] = 0;

                m_stDialogBoxInfo[40].sSizeX = 180;
                m_stDialogBoxInfo[40].sSizeY = 183;

                DisableDialogBox(17);
                DisableDialogBox(20);
                DisableDialogBox(23);
                DisableDialogBox(26);
            }
            break;
        case 42: // Change stats window
            if (m_bIsDialogEnabled[42] == false)
            {
                m_stDialogBoxInfo[42].sX = m_stDialogBoxInfo[12].sX + 10;
                m_stDialogBoxInfo[42].sY = m_stDialogBoxInfo[12].sY + 10;
                m_stDialogBoxInfo[42].cMode = 0;
                m_stDialogBoxInfo[42].sView = 0;
                m_bSkillUsingStatus = false;
            }
            break;
        case 50: // Resurrection
            if (m_bIsDialogEnabled[50] == false)
            {
                m_stDialogBoxInfo[50].sX = 185;
                m_stDialogBoxInfo[50].sY = 100;
                m_stDialogBoxInfo[50].cMode = 0;
                m_stDialogBoxInfo[50].sView = 0;
                m_bSkillUsingStatus = false;
            }
            break;

            //Dialog
#ifdef DEF_ADMINCLIENT
        case 72:
            switch (cType)
            {
                case 0:
                    break;
                default:
                    m_stDialogBoxInfo[72].cMode = 0;
                    m_stDialogBoxInfo[72].sView = 0;
                    m_stDialogBoxInfo[72].sV1 = cType;
            }
            break;
        case 71:
            switch (cType)
            {
                case 0:

                    if (m_stDialogBoxInfo[71].sV1 == 1)
                    {
                        sX = m_stDialogBoxInfo[71].sX;
                        sY = m_stDialogBoxInfo[71].sY;
                        StartInputString(sX + 105, sY + 50 + 15 * 1, 11, m_cRefreshRate, false);
                        m_bInputStatus = false;
                    }

                    break;
                default:
                    m_stDialogBoxInfo[71].cMode = sV1;
                    m_stDialogBoxInfo[71].sView = 0;
                    m_stDialogBoxInfo[71].sV1 = cType;
            }
            break;
#endif
            /*case 41:
                if (m_stDialogBoxInfo[41].cMode == 1) {
                    sX = m_stDialogBoxInfo[41].sX;
                    sY = m_stDialogBoxInfo[41].sY;
                    EndInputString();
                    StartInputString(sX + 75, sY + 140, 21, "lol");
                }
                break;*/

        default:
            EndInputString();
            if (m_bIsDialogEnabled[iBoxID] == false)
            {
                m_stDialogBoxInfo[iBoxID].cMode = 0;
                m_stDialogBoxInfo[iBoxID].sView = 0;
            }
            break;
    }

    if (iBoxID != 30)
    {
        if (m_bIsDialogEnabled[iBoxID] == false)
        {
            if (m_stDialogBoxInfo[iBoxID].sY > 400) m_stDialogBoxInfo[iBoxID].sY = 410;
            if (m_stDialogBoxInfo[iBoxID].sX > 620) m_stDialogBoxInfo[iBoxID].sX = 620;
            if ((m_stDialogBoxInfo[iBoxID].sX + m_stDialogBoxInfo[iBoxID].sSizeX) < 10) m_stDialogBoxInfo[iBoxID].sX += 20;
            if ((m_stDialogBoxInfo[iBoxID].sY + m_stDialogBoxInfo[iBoxID].sSizeY) < 10) m_stDialogBoxInfo[iBoxID].sY += 20;
        }
    }
    m_bIsDialogEnabled[iBoxID] = true;
    if (pString != 0) strcpy(m_stDialogBoxInfo[iBoxID].cStr, pString);
    for (i = 0; i < 99; i++)
        if (m_cDialogBoxOrder[i] == iBoxID) m_cDialogBoxOrder[i] = 0;

    for (i = 1; i < 99; i++)
        if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
        {
            m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
            m_cDialogBoxOrder[i] = 0;
        }

    for (i = 0; i < 99; i++)
        if (m_cDialogBoxOrder[i] == 0)
        {
            m_cDialogBoxOrder[i] = iBoxID;
            return;
        }
}

void CGame::DisableDialogBox(int iBoxID)
{
    int i;

    switch (iBoxID)
    {
        case 4:
            m_bIsItemDisabled[m_stDialogBoxInfo[4].sView] = false;
            break;

        case 5:
            m_bIsItemDisabled[m_stDialogBoxInfo[5].sView] = false;
            break;

        case 6:
            m_bIsItemDisabled[m_stDialogBoxInfo[6].sView] = false;
            break;

        case 7:
            if (m_stDialogBoxInfo[7].cMode == 1)
                EndInputString();
            m_stDialogBoxInfo[7].cMode = 0;
            break;

        case 11:
            for (i = 0; i < DEF_MAXMENUITEMS; i++)
                if (m_pItemForSaleList[i] != 0)
                {
                    delete m_pItemForSaleList[i];
                    m_pItemForSaleList[i] = 0;
                }
            m_stDialogBoxInfo[39].sV3 = 0;
            m_stDialogBoxInfo[39].sV4 = 0; // v1.4
            m_stDialogBoxInfo[39].sV5 = 0;
            m_stDialogBoxInfo[39].sV6 = 0;
            break;

        case 14:
            if (m_stDialogBoxInfo[14].cMode < 0) return;
            break;

        case 17:
            if (m_stDialogBoxInfo[17].cMode == 1)
            {
                EndInputString();
                m_bIsItemDisabled[m_stDialogBoxInfo[17].sView] = false;
            }
            break;

        case 20: // v1.4
            m_bIsItemDisabled[m_stDialogBoxInfo[20].sV1] = false;
            break;

        case 21:
            if (m_stDialogBoxInfo[21].sV2 == 500)
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GETMAGICABILITY, 0, 0, 0, 0, 0);
            }
            break;

        case 24:
            m_bSkillUsingStatus = false;
            break;

        case 26:
            if (m_stDialogBoxInfo[26].sV1 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = false;
            if (m_stDialogBoxInfo[26].sV2 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = false;
            if (m_stDialogBoxInfo[26].sV3 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = false;
            if (m_stDialogBoxInfo[26].sV4 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = false;
            if (m_stDialogBoxInfo[26].sV5 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = false;
            if (m_stDialogBoxInfo[26].sV6 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = false;
            m_bSkillUsingStatus = false;
            break;

        case 27: // multitrade
            for (i = 0; i < 8; i++)
            {
                memset(m_stDialogBoxExchangeInfo[i].cStr1, 0, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
                memset(m_stDialogBoxExchangeInfo[i].cStr2, 0, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
                m_stDialogBoxExchangeInfo[i].sV1 = -1;
                m_stDialogBoxExchangeInfo[i].sV2 = -1;
                m_stDialogBoxExchangeInfo[i].sV3 = -1;
                m_stDialogBoxExchangeInfo[i].sV4 = -1;
                m_stDialogBoxExchangeInfo[i].sV5 = -1;
                m_stDialogBoxExchangeInfo[i].sV6 = -1;
                m_stDialogBoxExchangeInfo[i].sV7 = -1;
                m_stDialogBoxExchangeInfo[i].dwV1 = 0;
                if (m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] == true)
                    m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] = false;
            }
            break;


        case 31:
            for (i = 0; i < DEF_MAXSELLLIST; i++)
            {
                if (m_stSellItemList[i].iIndex != -1) m_bIsItemDisabled[m_stSellItemList[i].iIndex] = false;
                m_stSellItemList[i].iIndex = -1;
                m_stSellItemList[i].iAmount = 0;
            }
            break;

        case 34:
            if (m_stDialogBoxInfo[34].sV1 != -1)
                m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = false;
            break;

        case 40:
            m_bIsItemDisabled[m_stDialogBoxInfo[40].sV1] = false;
            m_bIsItemDisabled[m_stDialogBoxInfo[40].sV2] = false;
            m_bIsItemDisabled[m_stDialogBoxInfo[40].sV3] = false;
            m_bIsItemDisabled[m_stDialogBoxInfo[40].sV4] = false;

            memset(m_stDialogBoxInfo[40].cStr, 0, sizeof(m_stDialogBoxInfo[40].cStr));
            memset(m_stDialogBoxInfo[40].cStr2, 0, sizeof(m_stDialogBoxInfo[40].cStr2));
            memset(m_stDialogBoxInfo[40].cStr3, 0, sizeof(m_stDialogBoxInfo[40].cStr3));
            memset(m_stDialogBoxInfo[40].cStr4, 0, sizeof(m_stDialogBoxInfo[40].cStr4));
            m_stDialogBoxInfo[40].sV1 = -1;
            m_stDialogBoxInfo[40].sV2 = -1;
            m_stDialogBoxInfo[40].sV3 = -1;
            m_stDialogBoxInfo[40].sV4 = -1;
            m_stDialogBoxInfo[40].sV5 = -1;
            m_stDialogBoxInfo[40].sV6 = -1;
            m_stDialogBoxInfo[40].sV9 = -1;
            m_stDialogBoxInfo[40].sV10 = -1;
            m_stDialogBoxInfo[40].sV11 = -1;
            m_stDialogBoxInfo[40].sV12 = -1;
            m_stDialogBoxInfo[40].sV13 = -1;
            m_stDialogBoxInfo[40].sV14 = -1;
            m_stDialogBoxInfo[40].dwV1 = 0;
            m_stDialogBoxInfo[40].dwV2 = 0;
            break;

        case 42:
            m_cStateChange1 = 0;
            m_cStateChange2 = 0;
            m_cStateChange3 = 0;
            /*	m_cLU_Str = 0;
            m_cLU_Vit = 0;
            m_cLU_Dex = 0;
            m_cLU_Int = 0;
            m_cLU_Mag = 0;
            m_cLU_Char = 0;*/
            break;

            //Dialog
        case 71:
            if (m_stDialogBoxInfo[71].sV1 == 1)
                EndInputString();
            m_stDialogBoxInfo[71].sV1 = 0;
            break;


    }

    m_bIsDialogEnabled[iBoxID] = false;
    for (i = 0; i < 99; i++)
        if (m_cDialogBoxOrder[i] == iBoxID)
            m_cDialogBoxOrder[i] = 0;

    for (i = 1; i < 99; i++)
        if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
        {
            m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
            m_cDialogBoxOrder[i] = 0;
        }
}
int CGame::iGetTopDialogBoxIndex()
{
    int i;

    for (i = 98; i >= 0; i--)
        if (m_cDialogBoxOrder[i] != 0)
            return m_cDialogBoxOrder[i];

    return 0;
}

void CGame::DrawChatMsgs(short sX, short sY, short dX, short dY)
{
    int i;

    for (i = 0; i < DEF_MAXCHATMSGS; i++)
    {
        if (m_pChatMsgList[i] != 0)
        {
            if ((m_pChatMsgList[i]->m_sX >= sX) && (m_pChatMsgList[i]->m_sX <= dX) &&
                (m_pChatMsgList[i]->m_sY >= sY) && (m_pChatMsgList[i]->m_sY <= dY))
            {

                switch (m_pChatMsgList[i]->m_cType)
                {
                    case 41:
                    case 42:
                    case 21:
                    case 22:
                    case 23:
                        DrawChatMsgBox(m_pChatMsgList[i]->m_sX, m_pChatMsgList[i]->m_sY, i, false);
                        break;
                }
            }
        }
    }

    for (i = 0; i < DEF_MAXCHATMSGS; i++)
    {
        if (m_pChatMsgList[i] != 0)
        {
            if ((m_pChatMsgList[i]->m_sX >= sX) && (m_pChatMsgList[i]->m_sX <= dX) &&
                (m_pChatMsgList[i]->m_sY >= sY) && (m_pChatMsgList[i]->m_sY <= dY))
            {

                switch (m_pChatMsgList[i]->m_cType)
                {
                    case 41:
                    case 42:
                    case 21:
                    case 22:
                    case 23:
                        break;

                    case 20:
                    default:
                        DrawChatMsgBox(m_pChatMsgList[i]->m_sX, m_pChatMsgList[i]->m_sY, i, true);
                        break;
                }
            }
        }
    }
}

void CGame::_RequestMapStatus(char * pMapName, int iMode)
{
    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_MAPSTATUS, 0, iMode, 0, 0, pMapName);
}

void CGame::AddMapStatusInfo(char * pData, bool bIsLastData)
{
    char * cp, cTotal;
    short * sp, sIndex;
    int i;

    memset(m_cStatusMapName, 0, sizeof(m_cStatusMapName));

    cp = (char *)(pData + 6);
    memcpy(m_cStatusMapName, cp, 10);
    cp += 10;

    sp = (short *)cp;
    sIndex = *sp;
    cp += 2;

    cTotal = *cp;
    cp++;

    for (i = 1; i <= cTotal; i++)
    {
        m_stCrusadeStructureInfo[sIndex].cType = *cp;
        cp++;
        sp = (short *)cp;
        m_stCrusadeStructureInfo[sIndex].sX = *sp;
        cp += 2;
        sp = (short *)cp;
        m_stCrusadeStructureInfo[sIndex].sY = *sp;
        cp += 2;
        m_stCrusadeStructureInfo[sIndex].cSide = *cp;
        cp++;

        sIndex++;
    }

    if (bIsLastData == true)
    {
        while (sIndex < DEF_MAXCRUSADESTRUCTURES)
        {
            m_stCrusadeStructureInfo[sIndex].cType = 0;
            m_stCrusadeStructureInfo[sIndex].sX = 0;
            m_stCrusadeStructureInfo[sIndex].sY = 0;
            m_stCrusadeStructureInfo[sIndex].cSide = 0;
            sIndex++;
        }
    }
}

int CGame::GetCharKind(char * str, int index)
{
    int kind = 1;

    do
    {
        if (kind == 2) kind = 3;
        else
        {
            if ((unsigned char)*str < 128) kind = 1;
            else kind = 2;
        }
        str++;
        index--;
    } while (index >= 0);
    return kind;
}

void CGame::ShowReceivedString(bool bIsHide)
{
    memset(G_cTxt, 0, sizeof(G_cTxt));

    strcpy(G_cTxt, m_pInputBuffer);
    if ((m_cEdit[0] != 0) && (strlen(m_pInputBuffer) + strlen(m_cEdit) + 1 <= m_cInputMaxLen))
    {
        strcpy(G_cTxt + strlen(m_pInputBuffer), m_cEdit);
    }

    if (bIsHide == true)
    {
        for (unsigned char i = 0; i < strlen(G_cTxt); i++)
            if (G_cTxt[i] != 0) G_cTxt[i] = '*';
    }

    if ((G_dwGlobalTime % 400) < 210) G_cTxt[strlen(G_cTxt)] = '_';

    put_string(m_iInputX + 1, m_iInputY + 1, G_cTxt, Color(0, 0, 0));
    put_string(m_iInputX, m_iInputY + 1, G_cTxt, Color(0, 0, 0));
    put_string(m_iInputX + 1, m_iInputY, G_cTxt, Color(0, 0, 0));
    put_string(m_iInputX, m_iInputY, G_cTxt, Color(255, 255, 255));
}

void CGame::ClearInputString()
{
    if (m_pInputBuffer != 0)	memset(m_pInputBuffer, 0, sizeof(m_pInputBuffer));
    memset(m_cEdit, 0, sizeof(m_cEdit));
}

void CGame::StartInputString(int sX, int sY, unsigned char iLen, char * pBuffer, bool bIsHide)
{
    m_bInputStatus = true;
    m_iInputX = sX;
    m_iInputY = sY;
    m_pInputBuffer = pBuffer;
    memset(m_cEdit, 0, sizeof(m_cEdit));
    m_cInputMaxLen = iLen;
}

void CGame::EndInputString()
{
    m_bInputStatus = false;
    int len = (int)strlen(m_cEdit);

    if (len > 0)
    {
        m_cEdit[len] = 0;
        strcpy(m_pInputBuffer + strlen(m_pInputBuffer), m_cEdit);
        memset(m_cEdit, 0, sizeof(m_cEdit));
    }
}

void CGame::ReceiveString(char * pString)
{
    strcpy(pString, m_pInputBuffer);
}

void CGame::SetCameraShakingEffect(short sDist, int iMul)
{
    int iDegree;

    iDegree = 5 - sDist;
    if (iDegree <= 0) iDegree = 0;
    iDegree *= 2;

    if (iMul != 0) iDegree *= iMul;

    if (iDegree <= 2) return;

    m_iCameraShakingDegree = iDegree;
}

void CGame::MeteorStrikeComing(int iCode)
{
    switch (iCode)
    {
        case 1:
            SetTopMsg(m_pGameMsgList[0]->message, 5);
            break;
        case 2:
            SetTopMsg(m_pGameMsgList[10]->message, 10);
            break;
        case 3:
            SetTopMsg(m_pGameMsgList[91]->message, 5);
            break;
        case 4:
            SetTopMsg(m_pGameMsgList[11]->message, 10);
            break;
    }
}

void CGame::DrawObjectFOE(int ix, int iy, int iFrame)
{
    if (_iGetFOE(_tmp_iStatus) < 0)
    {
        if (iFrame <= 4) m_pEffectSpr[38]->put_trans_sprite(ix, iy, iFrame, G_dwGlobalTime);
    }
}
//72LOL
void CGame::DrawObjectPFM(int ix, int iy, int iFrame)
{
    //	if( _iGetFOE(_tmp_iStatus) < 0 )
    //	{
    m_pEffectSpr[79]->put_trans_sprite70(ix + 100, iy + 135, ((G_dwGlobalTime / 100) % 16), G_dwGlobalTime);
    //	}
}
void CGame::DrawObjectPFA(int ix, int iy, int iFrame)
{
    //	if( _iGetFOE(_tmp_iStatus) < 0 )
    //	{
    m_pEffectSpr[72]->put_trans_sprite70(ix, iy + 35, ((G_dwGlobalTime / 100) % 16), G_dwGlobalTime);
    //	}
}
void CGame::DrawObjectDEF(int ix, int iy, int iFrame)
{
    //	if( _iGetFOE(_tmp_iStatus) < 0 )
    //	{
    m_pEffectSpr[80]->put_trans_sprite70(ix + 75, iy + 110, ((G_dwGlobalTime / 100) % 16), G_dwGlobalTime);
    //	}
}

void CGame::SetTopMsg(std::string pString, unsigned char iLastSec)
{
    memset(m_cTopMsg, 0, sizeof(m_cTopMsg));
    strcpy(m_cTopMsg, pString.c_str());

    m_iTopMsgLastSec = iLastSec;
    m_dwTopMsgTime = G_dwGlobalTime;
}

void CGame::DrawTopMsg()
{
    if (strlen(m_cTopMsg) == 0) return;
    draw_shadow_box(0, 0, get_virtual_width() - 1, 30);

    if ((((G_dwGlobalTime - m_dwTopMsgTime) / 250) % 2) == 0)
        put_aligned_string(0, get_virtual_width() - 1, 10, m_cTopMsg, 255, 255, 255);

    if (G_dwGlobalTime > (m_iTopMsgLastSec * 1000 + m_dwTopMsgTime))
    {
        memset(m_cTopMsg, 0, sizeof(m_cTopMsg));
    }
}

void CGame::CannotConstruct(int iCode)
{
    switch (iCode)
    {
        case 1:
            SetTopMsg(m_pGameMsgList[18]->message, 5);
            break;

        case 2:
            format_to_local(G_cTxt, "{} XY({}, {})", m_pGameMsgList[19]->message, m_iConstructLocX, m_iConstructLocY);
            SetTopMsg(G_cTxt, 5);
            break;

        case 3:
            SetTopMsg(m_pGameMsgList[20]->message, 5);
            break;
        case 4:
            SetTopMsg(m_pGameMsgList[20]->message, 5);
            break;

    }
}

void CGame::DisplayGold(int iGold)
{
    char cGold[20];
    int iStrLen;

    memset(cGold, 0, sizeof(cGold));
    memset(G_cTxt, 0, sizeof(G_cTxt));

    _itoa(iGold, cGold, 10);

    iStrLen = (int)strlen(cGold);
    iStrLen--;

    int cnt = 0;

    for (int i = 0; i < iStrLen + 1; i++)
    {

        if ((cnt != 0) && ((cnt + 1) % 4 == 0))
        {
            G_cTxt[cnt] = ',';
            i--;
        }
        else
            G_cTxt[cnt] = cGold[iStrLen - i];
        cnt++;
    }

    iStrLen = (int)strlen(G_cTxt);
    G_cTxt[iStrLen] = '\0';
    //strcat(G_cTxt, "1337\0");
    _strrev(G_cTxt);

    //	strcpy(G_cTxt, cGold);
}


void CGame::CrusadeContributionResult(int iWarContribution)
{
    int i;
    char cTemp[120];

    DisableDialogBox(18);

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;
    }

    if (iWarContribution > 0)
    {

        PlaySound('E', 23, 0, 0);
        PlaySound('C', 21, 0, 0);
        PlaySound('C', 22, 0, 0);

        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[22]->message, 0);
        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[23]->message, 0);
        m_pMsgTextList[2] = new CMsg(0, " ", 0);
        m_pMsgTextList[3] = new CMsg(0, m_pGameMsgList[24]->message, 0);
        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[25]->message, 0);
        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[26]->message, 0);
        m_pMsgTextList[6] = new CMsg(0, " ", 0);

        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[27]->message, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "+{}Exp Points!", iWarContribution);
        m_pMsgTextList[8] = new CMsg(0, cTemp, 0);

        for (i = 9; i < 18; i++)
            m_pMsgTextList[i] = new CMsg(0, " ", 0);

    }
    else if (iWarContribution < 0)
    {

        PlaySound('E', 24, 0, 0);
        PlaySound('C', 12, 0, 0);
        PlaySound('C', 13, 0, 0);

        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[28]->message, 0);
        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[29]->message, 0);
        m_pMsgTextList[2] = new CMsg(0, " ", 0);
        m_pMsgTextList[3] = new CMsg(0, m_pGameMsgList[30]->message, 0);
        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[31]->message, 0);
        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[32]->message, 0);
        m_pMsgTextList[6] = new CMsg(0, " ", 0);
        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[33]->message, 0);
        m_pMsgTextList[8] = new CMsg(0, m_pGameMsgList[34]->message, 0);

        for (i = 9; i < 18; i++)
            m_pMsgTextList[i] = new CMsg(0, " ", 0);
    }
    else if (iWarContribution == 0)
    {
        PlaySound('E', 25, 0, 0);

        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[50]->message, 0);
        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[51]->message, 0);
        m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[52]->message, 0);
        m_pMsgTextList[3] = new CMsg(0, " ", 0);
        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[53]->message, 0);
        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[54]->message, 0);
        m_pMsgTextList[6] = new CMsg(0, m_pGameMsgList[55]->message, 0);
        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[56]->message, 0);

        for (i = 8; i < 18; i++)
            m_pMsgTextList[i] = new CMsg(0, " ", 0);
    }
    EnableDialogBox(18, 0, 0, 0);
}


void CGame::CrusadeWarResult(int iWinnerSide)
{
    int i, iPlayerSide;

    DisableDialogBox(18);

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;
    }

    if (m_bCitizen == false) iPlayerSide = 0;
    else if (m_bAresden == true) iPlayerSide = 1;
    else if (m_bAresden == false) iPlayerSide = 2;

    if (iPlayerSide == 0)
    {
        switch (iWinnerSide)
        {
            case 0:
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[36]->message, 0);
                m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[37]->message, 0);
                m_pMsgTextList[3] = new CMsg(0, " ", 0);
                break;
            case 1:
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[38]->message, 0);
                m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[39]->message, 0);
                m_pMsgTextList[3] = new CMsg(0, " ", 0);
                break;
            case 2:
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[40]->message, 0);
                m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[41]->message, 0);
                m_pMsgTextList[3] = new CMsg(0, " ", 0);
                break;
        }

        for (i = 4; i < 18; i++)
            m_pMsgTextList[i] = new CMsg(0, " ", 0);
    }
    else
    {
        if (iWinnerSide == 0)
        {
            PlaySound('E', 25, 0, 0);
            m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
            m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[36]->message, 0);
            m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[37]->message, 0);
            m_pMsgTextList[3] = new CMsg(0, " ", 0);

            for (i = 4; i < 18; i++)
                m_pMsgTextList[i] = new CMsg(0, " ", 0);
        }
        else
        {
            if (iWinnerSide == iPlayerSide)
            {
                PlaySound('E', 23, 0, 0);
                PlaySound('C', 21, 0, 0);
                PlaySound('C', 22, 0, 0);

                switch (iWinnerSide)
                {
                    case 1:
                        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[38]->message, 0);
                        m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[39]->message, 0);
                        m_pMsgTextList[3] = new CMsg(0, " ", 0);
                        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[42]->message, 0);
                        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[43]->message, 0);
                        m_pMsgTextList[6] = new CMsg(0, m_pGameMsgList[44]->message, 0);
                        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[45]->message, 0);
                        break;
                    case 2:
                        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[40]->message, 0);
                        m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[41]->message, 0);
                        m_pMsgTextList[3] = new CMsg(0, " ", 0);
                        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[42]->message, 0);
                        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[43]->message, 0);
                        m_pMsgTextList[6] = new CMsg(0, m_pGameMsgList[44]->message, 0);
                        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[45]->message, 0);
                        break;
                }

                for (i = 8; i < 18; i++)
                    m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
            else if (iWinnerSide != iPlayerSide)
            {
                PlaySound('E', 24, 0, 0);
                PlaySound('C', 12, 0, 0);
                PlaySound('C', 13, 0, 0);

                switch (iWinnerSide)
                {
                    case 1:
                        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[38]->message, 0);
                        m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[39]->message, 0);
                        m_pMsgTextList[3] = new CMsg(0, " ", 0);
                        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[46]->message, 0);
                        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[47]->message, 0);
                        m_pMsgTextList[6] = new CMsg(0, m_pGameMsgList[48]->message, 0);
                        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[49]->message, 0);
                        break;
                    case 2:
                        m_pMsgTextList[0] = new CMsg(0, m_pGameMsgList[35]->message, 0);
                        m_pMsgTextList[1] = new CMsg(0, m_pGameMsgList[40]->message, 0);
                        m_pMsgTextList[2] = new CMsg(0, m_pGameMsgList[41]->message, 0);
                        m_pMsgTextList[3] = new CMsg(0, " ", 0);
                        m_pMsgTextList[4] = new CMsg(0, m_pGameMsgList[46]->message, 0);
                        m_pMsgTextList[5] = new CMsg(0, m_pGameMsgList[47]->message, 0);
                        m_pMsgTextList[6] = new CMsg(0, m_pGameMsgList[48]->message, 0);
                        m_pMsgTextList[7] = new CMsg(0, m_pGameMsgList[49]->message, 0);
                        break;
                }

                for (i = 8; i < 18; i++)
                    m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
        }
    }
    EnableDialogBox(18, 0, 0, 0);
    DisableDialogBox(36);
    DisableDialogBox(37);
    DisableDialogBox(38);
}

void CGame::ClearContents_OnSelectCharacter()
{
    m_cCurFocus = 1;
}

void CGame::_RemoveChatMsgListByObjectID(int iObjectID)
{
    int i;

    for (i = 1; i < DEF_MAXCHATMSGS; i++)
        if ((m_pChatMsgList[i] != 0) && (m_pChatMsgList[i]->m_iObjectID == iObjectID))
        {
            delete m_pChatMsgList[i];
            m_pChatMsgList[i] = 0;
        }
}

void CGame::PlaySound(char cType, int iNum, int iDist, long lPan)
{
    float fPan = (float)lPan;

    if (m_bSoundFlag == false) return;
    if (m_bSoundStat == false) return;

    int16_t max_distance = (get_virtual_width() / 32) / 2;

    if (fPan > max_distance) fPan = max_distance;
    if (fPan < -max_distance) fPan = -max_distance;
    if (fPan > -3 && fPan < 3) fPan = 0;

    switch (cType)
    {
        case 'C':
            if (iNum >= MAX_CHARACTER_SOUNDS) return;
            m_pCSound[iNum].setMinDistance(4.f);
            m_pCSound[iNum].setAttenuation(1.f);
            m_pCSound[iNum].setPosition({ (float)fPan, 0.f, 0.f });
            m_pCSound[iNum].setVolume(m_cSoundVolume);
            m_pCSound[iNum].play();
            break;

        case 'M':
            if (iNum >= MAX_MONSTER_SOUNDS) return;
            m_pMSound[iNum].setMinDistance(4.f);
            m_pMSound[iNum].setAttenuation(1.f);
            m_pMSound[iNum].setPosition({ (float)fPan, 0.f, 0.f });
            m_pMSound[iNum].setVolume(m_cSoundVolume);
            m_pMSound[iNum].play();
            break;

        case 'E':
            if (iNum >= MAX_EFFECT_SOUNDS) return;
            m_pESound[iNum].setMinDistance(4.f);
            m_pESound[iNum].setAttenuation(1.f);
            m_pESound[iNum].setPosition({ (float)fPan, 0.f, 0.f });
            m_pESound[iNum].setVolume(m_cSoundVolume);
            m_pESound[iNum].play();
            break;
    }
}

void CGame::_DrawBlackRect(int iSize)
{
    int ix, iy, sx, sy, dcx, dcy;
    uint32_t dwTime;

    dwTime = unixtime();

    dcx = 40 - iSize * 2;
    dcy = 30 - iSize * 2;

    sx = iSize * 16;
    sy = iSize * 16;

    for (ix = 0; ix < dcx; ix++)
    {
        m_pSprite[DEF_SPRID_MOUSECURSOR]->put_sprite_fast(ix * 16 + sx, iSize * 16, 12, dwTime);
        m_pSprite[DEF_SPRID_MOUSECURSOR]->put_sprite_fast(ix * 16 + sx, 464 - iSize * 16, 12, dwTime);
    }

    for (iy = 0; iy < dcy; iy++)
    {
        m_pSprite[DEF_SPRID_MOUSECURSOR]->put_sprite_fast(iSize * 16, iy * 16 + sy, 12, dwTime);
        m_pSprite[DEF_SPRID_MOUSECURSOR]->put_sprite_fast(624 - iSize * 16, iy * 16 + sy, 12, dwTime);
    }
}

bool CGame::_bCheckItemByType(char cType)
{
    int i;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_pItemList[i] != 0) && (m_pItemList[i]->m_cItemType == cType)) return true;

    return false;
}

bool CGame::_bIsItemOnHand()
{
    int i;
    uint16_t wWeaponType;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if ((m_pItemList[i] != 0) && (m_bIsItemEquipped[i] == true))
        {
            if (m_pItemList[i]->m_cEquipPos == DEF_EQUIPPOS_RHAND)
            {
                wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);
                if ((wWeaponType >= 34) && (wWeaponType < 40)) //Change 35 -> 34
                    return false;
                else if (wWeaponType == 27) return false;
                else return true;
            }

            if ((m_pItemList[i]->m_cEquipPos == DEF_EQUIPPOS_LHAND) ||
                (m_pItemList[i]->m_cEquipPos == DEF_EQUIPPOS_TWOHAND))
                return true;
        }

    return false;
}

int CGame::_iCalcTotalWeight()
{
    int i, iWeight, iCnt, iTemp;

    iCnt = 0;
    iWeight = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pItemList[i] != 0)
        {

            if ((m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pItemList[i]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {
                iTemp = m_pItemList[i]->m_wWeight * m_pItemList[i]->m_dwCount;
                if (strcmp(m_pItemList[i]->m_cName, "Gold") == 0) iTemp = iTemp / 20;

                iWeight += iTemp;
            }
            else iWeight += m_pItemList[i]->m_wWeight;

            iCnt++;
        }

    return iWeight;
}


int CGame::iGetLevelExp(int iLevel)
{
    int iRet;


    if (iLevel == 0) return 0;

    iRet = iGetLevelExp(iLevel - 1) + iLevel * (50 + (iLevel * (iLevel / 17) * (iLevel / 17)));

    return iRet;
}

int CGame::_iGetTotalItemNum()
{
    int i, iCnt;

    iCnt = 0;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pItemList[i] != 0) iCnt++;

    return iCnt;
}

bool CGame::bCheckExID(char * pName)
{
    if (m_pExID == 0) return false;
    if (memcmp(m_cPlayerName, pName, 10) == 0) return false;
    char cTxt[12];
    memset(cTxt, 0, sizeof(cTxt));
    memcpy(cTxt, m_pExID->message.c_str(), m_pExID->message.length());
    if (memcmp(cTxt, pName, 10) == 0) return true;
    else return false;
}

void CGame::DrawWhetherEffects()
{
    int i;
    short dX, dY, sCnt;
    char cTempFrame;
    uint32_t dwTime = m_dwCurTime;

    switch (m_cWhetherEffectType)
    {
        case 1:
        case 2:
        case 3:
            switch (m_cWhetherEffectType)
            {
                case 1: sCnt = DEF_MAXWHETHEROBJECTS / 5; break;
                case 2:	sCnt = DEF_MAXWHETHEROBJECTS / 2; break;
                case 3:	sCnt = DEF_MAXWHETHEROBJECTS;     break;
            }

            for (i = 0; i < sCnt; i++)
            {
                if ((m_stWhetherObject[i].cStep >= 0) && (m_stWhetherObject[i].cStep < 20) && (m_stWhetherObject[i].sX != 0))
                {
                    dX = m_stWhetherObject[i].sX - m_sViewPointX;
                    dY = m_stWhetherObject[i].sY - m_sViewPointY;
                    cTempFrame = 16 + (m_stWhetherObject[i].cStep / 6);

                    m_pEffectSpr[11]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                }
                else if ((m_stWhetherObject[i].cStep >= 20) && (m_stWhetherObject[i].cStep < 25) && (m_stWhetherObject[i].sX != 0))
                {
                    dX = m_stWhetherObject[i].sX - m_sViewPointX;
                    dY = m_stWhetherObject[i].sY - m_sViewPointY;
                    m_pEffectSpr[11]->put_trans_sprite(dX, dY, m_stWhetherObject[i].cStep, dwTime);
                }
            }
            break;

        case 4:
        case 5:
        case 6:
            switch (m_cWhetherEffectType)
            {
                case 4: sCnt = DEF_MAXWHETHEROBJECTS / 5; break;
                case 5:	sCnt = DEF_MAXWHETHEROBJECTS / 2; break;
                case 6:	sCnt = DEF_MAXWHETHEROBJECTS;     break;
            }

#ifdef DEF_XMAS
#define MAXNUM 1000
            static int ix1[MAXNUM];
            static int iy2[MAXNUM];
            static int iFrame[MAXNUM];
            static int iNum = 0;
#endif

            for (i = 0; i < sCnt; i++)
            {
                if ((m_stWhetherObject[i].cStep >= 0) && (m_stWhetherObject[i].cStep < 80))
                {
                    dX = m_stWhetherObject[i].sX - m_sViewPointX;
                    dY = m_stWhetherObject[i].sY - m_sViewPointY;
                    cTempFrame = 39 + (m_stWhetherObject[i].cStep / 20) * 3 + (rand() % 3);

                    m_pEffectSpr[11]->put_trans_sprite(dX, dY, cTempFrame, dwTime);

#ifdef DEF_XMAS
                    if (dY == 478 - 53)
                    {
                        ix1[iNum] = dX;
                        iy2[iNum] = dY + (rand() % 5);
                        iFrame[iNum] = cTempFrame;
                        iNum++;
                    }
                    if (iNum >= MAXNUM)					iNum = 0;
#endif
                }
                else
                {

                }
            }

#ifdef DEF_XMAS
            for (i = 0; i <= MAXNUM; i++)
            {
                if (iy2[i] > 10)
                    m_pEffectSpr[11]->put_trans_sprite(ix1[i], iy2[i], iFrame[i], dwTime);
            }
#endif

            break;
    }
}

void CGame::weather_object_frame_counter()
{
    int i;
    short sCnt;
    char  cAdd;
    uint32_t dwTime = m_dwCurTime;
    if ((dwTime - m_dwWOFtime) < 30) return;
    m_dwWOFtime = dwTime;

    switch (m_cWhetherEffectType)
    {
        case 1:
        case 2:
        case 3:
            switch (m_cWhetherEffectType)
            {
                case 1: sCnt = DEF_MAXWHETHEROBJECTS / 5; break;
                case 2:	sCnt = DEF_MAXWHETHEROBJECTS / 2; break;
                case 3:	sCnt = DEF_MAXWHETHEROBJECTS;     break;
            }

            for (i = 0; i < sCnt; i++)
            {
                m_stWhetherObject[i].cStep++;
                if ((m_stWhetherObject[i].cStep >= 0) && (m_stWhetherObject[i].cStep < 20))
                {
                    cAdd = (40 - m_stWhetherObject[i].cStep);
                    if (cAdd < 0) cAdd = 0;
                    m_stWhetherObject[i].sY = m_stWhetherObject[i].sY + cAdd;
                    if (cAdd != 0)
                        m_stWhetherObject[i].sX = m_stWhetherObject[i].sX - 1;
                }
                else if (m_stWhetherObject[i].cStep >= 25)
                {
                    if (m_bIsWhetherEffect == false)
                    {
                        m_stWhetherObject[i].sX = 0;
                        m_stWhetherObject[i].sY = 0;
                        m_stWhetherObject[i].cStep = 30;
                    }
                    else
                    {
                        int pivot_x = m_sPlayerX - get_virtual_width() / 2;
                        int pivot_y = m_sPlayerY - get_virtual_height() / 2;
                        //todo - check 300/240
                        m_stWhetherObject[i].sX = (pivot_x * 32) + ((rand() % 940) - 200) + 300;
                        m_stWhetherObject[i].sY = (pivot_y * 32) + ((rand() % 800) - 600) + 240;
                        m_stWhetherObject[i].cStep = -1 * (rand() % 10);
                    }
                }
            }
            break;

        case 4:
        case 5:
        case 6:
            switch (m_cWhetherEffectType)
            {
                case 4: sCnt = DEF_MAXWHETHEROBJECTS / 5; break;
                case 5:	sCnt = DEF_MAXWHETHEROBJECTS / 2; break;
                case 6:	sCnt = DEF_MAXWHETHEROBJECTS;     break;
            }

            for (i = 0; i < sCnt; i++)
            {
                m_stWhetherObject[i].cStep++;
                if ((m_stWhetherObject[i].cStep >= 0) && (m_stWhetherObject[i].cStep < 80))
                {
                    cAdd = (80 - m_stWhetherObject[i].cStep) / 10;
                    if (cAdd < 0) cAdd = 0;
                    m_stWhetherObject[i].sY = m_stWhetherObject[i].sY + cAdd;
                    //if ((rand() % 3) == 1) 
                    m_stWhetherObject[i].sX += 1 - (rand() % 3);
                }
                else if (m_stWhetherObject[i].cStep >= 80)
                {
                    if (m_bIsWhetherEffect == false)
                    {
                        m_stWhetherObject[i].sX = 0;
                        m_stWhetherObject[i].sY = 0;
                        m_stWhetherObject[i].cStep = 80;
                    }
                    else
                    {
                        int pivot_x = m_sPlayerX - get_virtual_width() / 2;
                        int pivot_y = m_sPlayerY - get_virtual_height() / 2;
                        m_stWhetherObject[i].sX = (pivot_x * 32) + ((rand() % 940) - 200) + 300;
                        m_stWhetherObject[i].sY = (pivot_y * 32) + ((rand() % 800) - 600) + 600;
                        m_stWhetherObject[i].cStep = -1 * (rand() % 10);
                    }
                }
            }
            break;
    }
}

void CGame::SetWhetherStatus(bool bStart, char cType)
{
    if (bStart == true)
    {
        m_bIsWhetherEffect = true;
        m_cWhetherEffectType = cType;
        if ((m_bSoundStat == true) && (m_bSoundFlag) && (cType >= 1) && (cType <= 3)) m_pESound[38].play();

        for (int i = 0; i < DEF_MAXWHETHEROBJECTS; i++)
        {
            m_stWhetherObject[i].sX = 1;
            m_stWhetherObject[i].sY = 1;
            m_stWhetherObject[i].cStep = -1 * (rand() % 40);
        }
        if (cType >= 4 && cType <= 6)
        {
            if (m_bMusicStat) StartBGM();
        }
    }
    else
    {
        m_bIsWhetherEffect = false;
        m_cWhetherEffectType = 0;
        if ((m_bSoundStat == true) && (m_bSoundFlag)) m_pESound[38].stop();
    }
}

void CGame::DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB)
{
    draw_line(x0, y0, x1, y1, iR, iG, iB);
//     int dx, dy, x_inc, y_inc, error, index, dstR, dstG, dstB;
//     int iResultX, iResultY;
//     uint16_t * pDst;
// 
//     if ((x0 == x1) && (y0 == y1))
//         return;
// 
//     error = 0;
// 
//     iResultX = x0;
//     iResultY = y0;
// 
//     dx = x1 - x0;
//     dy = y1 - y0;
// 
//     if (dx >= 0)
//     {
//         x_inc = 1;
//     }
//     else
//     {
//         x_inc = -1;
//         dx = -dx;
//     }
// 
//     if (dy >= 0)
//     {
//         y_inc = 1;
//     }
//     else
//     {
//         y_inc = -1;
//         dy = -dy;
//     }
// 
//     if (dx > dy)
//     {
//         for (index = 0; index <= dx; index++)
//         {
//             error += dy;
//             if (error > dx)
//             {
//                 error -= dx;
//                 iResultY += y_inc;
//             }
//             iResultX += x_inc;
//             if ((iResultX >= 0) && (iResultX < get_virtual_width()) && (iResultY >= 0) && (iResultY < get_virtual_height()))
//             {
//                 pDst = (uint16_t *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
//                 switch (m_DDraw.m_cPixelFormat)
//                 {
//                     case 1:
//                         dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
//                         dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
//                         dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
// 
//                         *pDst = (uint16_t)((dstR << 11) | (dstG << 5) | dstB);
//                         break;
// 
//                     case 2:
//                         dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
//                         dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
//                         dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
// 
//                         *pDst = (uint16_t)((dstR << 10) | (dstG << 5) | dstB);
//                         break;
//                 }
//             }
//             //
//         }
//     }
//     else
//     {
//         for (index = 0; index <= dy; index++)
//         {
//             error += dx;
//             if (error > dy)
//             {
//                 error -= dy;
//                 iResultX += x_inc;
//             }
//             iResultY += y_inc;
//             if ((iResultX >= 0) && (iResultX < get_virtual_width()) && (iResultY >= 0) && (iResultY < get_virtual_height()))
//             {
//                 pDst = (uint16_t *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
//                 switch (m_DDraw.m_cPixelFormat)
//                 {
//                     case 1:
//                         dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
//                         dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
//                         dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
// 
//                         *pDst = (uint16_t)((dstR << 11) | (dstG << 5) | dstB);
//                         break;
// 
//                     case 2:
//                         dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
//                         dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
//                         dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
// 
//                         *pDst = (uint16_t)((dstR << 10) | (dstG << 5) | dstB);
//                         break;
//                 }
//             }
//             //
//         }
//     }
}

void CGame::_DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType)
{
    int j, iErr, pX1, pY1, iX1, iY1, tX, tY;
    char cDir;
    uint32_t dwTime;
    uint16_t  wR1, wG1, wB1, wR2, wG2, wB2, wR3, wG3, wB3, wR4, wG4, wB4;

    dwTime = m_dwCurTime;
    sX = pX1 = iX1 = tX = sX;
    sY = pY1 = iY1 = tY = sY;

    wR1 = 50; wG1 = 50; wB1 = 100;
    wR2 = 30; wG2 = 30; wB2 = 100;
    wR3 = 0; wG3 = 0; wB3 = 30;
    wR4 = 50; wG4 = 50; wB4 = 200;

    for (j = 0; j < 100; j++)
    {

        switch (cType)
        {
            case 1:
                draw_line(pX1, pY1, iX1, iY1, 15, 15, 20);
                draw_line(pX1 - 1, pY1, iX1 - 1, iY1, wR1, wG1, wB1);
                draw_line(pX1 + 1, pY1, iX1 + 1, iY1, wR1, wG1, wB1);
                draw_line(pX1, pY1 - 1, iX1, iY1 - 1, wR1, wG1, wB1);
                draw_line(pX1, pY1 + 1, iX1, iY1 + 1, wR1, wG1, wB1);

                draw_line(pX1 - 2, pY1, iX1 - 2, iY1, wR2, wG2, wB2);
                draw_line(pX1 + 2, pY1, iX1 + 2, iY1, wR2, wG2, wB2);
                draw_line(pX1, pY1 - 2, iX1, iY1 - 2, wR2, wG2, wB2);
                draw_line(pX1, pY1 + 2, iX1, iY1 + 2, wR2, wG2, wB2);

                draw_line(pX1 - 1, pY1 - 1, iX1 - 1, iY1 - 1, wR3, wG3, wB3);
                draw_line(pX1 + 1, pY1 - 1, iX1 + 1, iY1 - 1, wR3, wG3, wB3);
                draw_line(pX1 + 1, pY1 - 1, iX1 + 1, iY1 - 1, wR3, wG3, wB3);
                draw_line(pX1 - 1, pY1 + 1, iX1 - 1, iY1 + 1, wR3, wG3, wB3);
                break;

            case 2:
                DrawLine2(pX1, pY1, iX1, iY1, wR4, wG4, wB4);
                break;
        }

        iErr = 0;
        m_Misc.GetPoint(sX, sY, dX, dY, &tX, &tY, &iErr, j * 10);

        pX1 = iX1;
        pY1 = iY1;

        cDir = cGetNextMoveDir(iX1, iY1, tX, tY);
        switch (cDir)
        {
            case 1:	rY -= 5; break;
            case 2: rY -= 5; rX += 5; break;
            case 3:	rX += 5; break;
            case 4: rX += 5; rY += 5; break;
            case 5: rY += 5; break;
            case 6: rX -= 5; rY += 5; break;
            case 7: rX -= 5; break;
            case 8: rX -= 5; rY -= 5; break;
        }

        if (rX < -20) rX = -20;
        if (rX > 20) rX = 20;
        if (rY < -20) rY = -20;
        if (rY > 20) rY = 20;

        iX1 = iX1 + rX;
        iY1 = iY1 + rY;

        if ((abs(tX - dX) < 5) && (abs(tY - dY) < 5)) break;
    }

    switch (cType)
    {
        case 1:
            m_pEffectSpr[6]->put_trans_sprite(iX1, iY1, (rand() % 2), dwTime);
            break;
    }
}

int CGame::_iGetAttackType()
{
    uint16_t wWeaponType;
    wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);
    if (wWeaponType == 0)
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[5] >= 100)) return 20;
        else return 1;		// Boxe
    }
    else if ((wWeaponType >= 1) && (wWeaponType <= 2))
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[7] >= 100)) return 21;
        else return 1;		//Dag, SS
    }
    else if ((wWeaponType > 2) && (wWeaponType < 20))
    {
        if ((wWeaponType == 7) || (wWeaponType == 18)) // Added Kloness Esterk
        {
            if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[9] >= 100)) return 22;
            else return 1;  // Esterk
        }
        else if (wWeaponType == 15)
        {
            if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[8] >= 100)) return 30;
            else return 5;  // StormBlade
        }
        else
        {
            if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[8] >= 100)) return 23;
            else return 1;	// LongSwords
        }
    }
    else if ((wWeaponType >= 20) && (wWeaponType < 29))
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[10] >= 100)) return 24;
        else return 1;		// Haches
    }
    else if ((wWeaponType >= 30) && (wWeaponType < 33))
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[14] >= 100)) return 26;
        else return 1;		// Hammers
    }
    else if ((wWeaponType >= 34) && (wWeaponType < 40))
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[21] >= 100)) return 27;
        else return 1;		// Wands
    }
    else if (wWeaponType >= 40)
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[6] >= 100)) return 25;
        else return 2;		// Bows
    }
    else if ((wWeaponType == 29) || (wWeaponType == 33))
    {
        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[8] >= 100)) return 23;
        else return 1;		// LS
    }
    return 0;
}

int CGame::_iGetWeaponSkillType()
{
    uint16_t wWeaponType;
    wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);
    if (wWeaponType == 0)
    {
        return 5; // Openhand
    }
    else if ((wWeaponType >= 1) && (wWeaponType < 3))
    {
        return 7; // SS
    }
    else if ((wWeaponType >= 3) && (wWeaponType < 20))
        if ((wWeaponType == 7) || (wWeaponType == 18)) // Esterk or KlonessEsterk
            return 9; // Fencing
        else return 8; // LS
    else if ((wWeaponType >= 20) && (wWeaponType < 29))
        return 10; // Axe (20..28)
    else if ((wWeaponType >= 30) && (wWeaponType < 33))
        return 14; // Hammer (30,31,32)
    else if ((wWeaponType >= 34) && (wWeaponType < 40))
        return 21; // Wand
    else if (wWeaponType >= 40)
        return 6;  // Bow
    else if ((wWeaponType == 29) || (wWeaponType == 33))
        return 8;  // LS LightingBlade || BlackShadow
    return 1; // Fishing !
}

int CGame::_iGetBankItemCount()
{
    int i, iCnt;

    iCnt = 0;
    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0) iCnt++;

    return iCnt;
}

bool CGame::_bCheckBuildItemStatus()
{
    int iIndex, i, j, iMatch, iCount;
    char cTempName[21];
    int  iItemCount[DEF_MAXITEMS]{};

    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pDispBuildItemList[i] != 0)
        {
            delete m_pDispBuildItemList[i];
            m_pDispBuildItemList[i] = 0;
        }

    iIndex = 0;
    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != 0)
        {
            if (m_cSkillMastery[13] >= m_pBuildItemList[i]->m_iSkillLimit)
            {

                iMatch = 0;
                m_pDispBuildItemList[iIndex] = new CBuildItem;
                memcpy(m_pDispBuildItemList[iIndex]->m_cName, m_pBuildItemList[i]->m_cName, 20);

                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName1, m_pBuildItemList[i]->m_cElementName1, 20);
                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName2, m_pBuildItemList[i]->m_cElementName2, 20);
                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName3, m_pBuildItemList[i]->m_cElementName3, 20);
                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName4, m_pBuildItemList[i]->m_cElementName4, 20);
                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName5, m_pBuildItemList[i]->m_cElementName5, 20);
                memcpy(m_pDispBuildItemList[iIndex]->m_cElementName6, m_pBuildItemList[i]->m_cElementName6, 20);

                m_pDispBuildItemList[iIndex]->m_iElementCount[1] = m_pBuildItemList[i]->m_iElementCount[1];
                m_pDispBuildItemList[iIndex]->m_iElementCount[2] = m_pBuildItemList[i]->m_iElementCount[2];
                m_pDispBuildItemList[iIndex]->m_iElementCount[3] = m_pBuildItemList[i]->m_iElementCount[3];
                m_pDispBuildItemList[iIndex]->m_iElementCount[4] = m_pBuildItemList[i]->m_iElementCount[4];
                m_pDispBuildItemList[iIndex]->m_iElementCount[5] = m_pBuildItemList[i]->m_iElementCount[5];
                m_pDispBuildItemList[iIndex]->m_iElementCount[6] = m_pBuildItemList[i]->m_iElementCount[6];

                m_pDispBuildItemList[iIndex]->m_iSprH = m_pBuildItemList[i]->m_iSprH;
                m_pDispBuildItemList[iIndex]->m_iSprFrame = m_pBuildItemList[i]->m_iSprFrame;
                m_pDispBuildItemList[iIndex]->m_iMaxSkill = m_pBuildItemList[i]->m_iMaxSkill;
                m_pDispBuildItemList[iIndex]->m_iSkillLimit = m_pBuildItemList[i]->m_iSkillLimit;
                for (j = 0; j < DEF_MAXITEMS; j++)
                    if (m_pItemList[j] != 0)
                        iItemCount[j] = m_pItemList[j]->m_dwCount;
                    else iItemCount[j] = 0;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName1, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[1];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[1] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP2;
                            }
                        }
                }

                CBIS_STEP2:;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName2, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[2];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[2] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP3;
                            }
                        }
                }

                CBIS_STEP3:;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName3, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[3];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[3] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP4;
                            }
                        }
                }

                CBIS_STEP4:;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName4, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[4];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[4] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP5;
                            }
                        }
                }

                CBIS_STEP5:;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName5, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[5];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[5] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP6;
                            }
                        }
                }

                CBIS_STEP6:;
                memset(cTempName, 0, sizeof(cTempName));
                memcpy(cTempName, m_pBuildItemList[i]->m_cElementName6, 20);
                iCount = m_pBuildItemList[i]->m_iElementCount[6];

                if (iCount == 0) iMatch++;
                else
                {
                    for (j = 0; j < DEF_MAXITEMS; j++)
                        if (m_pItemList[j] != 0)
                        {
                            if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (uint32_t)(iCount)) &&
                                (iItemCount[j] > 0))
                            {
                                iMatch++;
                                m_pDispBuildItemList[iIndex]->m_bElementFlag[6] = true;
                                iItemCount[j] -= iCount;
                                goto CBIS_STEP7;
                            }
                        }
                }

                CBIS_STEP7:;
                if (iMatch == 6) m_pDispBuildItemList[iIndex]->m_bBuildEnabled = true;
                iIndex++;
            }
        }

    return true;
}

bool CGame::_ItemDropHistory(char * ItemName)
{
    bool bFlag = false;

    if (m_iItemDropCnt == 0)
    {
        strcpy(m_cItemDrop[m_iItemDropCnt], ItemName);
        m_iItemDropCnt++;
        return true;
    }

    if ((1 <= m_iItemDropCnt) && (20 >= m_iItemDropCnt))
    {
        for (int i = 0; i < m_iItemDropCnt; i++)
        {
            if (strcmp(m_cItemDrop[i], ItemName) == 0)
            {
                bFlag = true;
                break;
            }
        }

        if (bFlag)
        {
            if (m_bItemDrop)
                return false;
            else
                return true;
        }

        if (20 < m_iItemDropCnt)
        {
            for (int i = 0; i < m_iItemDropCnt; i++)
                strcpy(m_cItemDrop[i - 1], ItemName);
            strcpy(m_cItemDrop[20], ItemName);
            m_iItemDropCnt = 21;
        }
        else
        {
            strcpy(m_cItemDrop[m_iItemDropCnt], ItemName);
            m_iItemDropCnt++;
        }
    }
    return true;
}

bool CGame::_bCheckCurrentBuildItemStatus()
{
    int i, iCount2, iMatch, iIndex, iItemIndex[7]{};
    int iCount;
    int iItemCount[7]{};
    char cTempName[21];
    bool bItemFlag[7]{};

    iIndex = m_stDialogBoxInfo[26].cStr[0];

    if (m_pBuildItemList[iIndex] == 0) return false;

    iItemIndex[1] = m_stDialogBoxInfo[26].sV1;
    iItemIndex[2] = m_stDialogBoxInfo[26].sV2;
    iItemIndex[3] = m_stDialogBoxInfo[26].sV3;
    iItemIndex[4] = m_stDialogBoxInfo[26].sV4;
    iItemIndex[5] = m_stDialogBoxInfo[26].sV5;
    iItemIndex[6] = m_stDialogBoxInfo[26].sV6;

    for (i = 1; i <= 6; i++)
        if (iItemIndex[i] != -1)
            iItemCount[i] = m_pItemList[iItemIndex[i]]->m_dwCount;
        else iItemCount[i] = 0;

    iMatch = 0;

    for (i = 1; i <= 6; i++) bItemFlag[i] = false;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName1, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[1];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP2;
            }
        }
    }

    CCBIS_STEP2:;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName2, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[2];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP3;
            }
        }
    }

    CCBIS_STEP3:;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName3, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[3];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP4;
            }
        }
    }

    CCBIS_STEP4:;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName4, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[4];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP5;
            }
        }
    }

    CCBIS_STEP5:;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName5, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[5];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP6;
            }
        }
    }

    CCBIS_STEP6:;
    memset(cTempName, 0, sizeof(cTempName));
    memcpy(cTempName, m_pDispBuildItemList[iIndex]->m_cElementName6, 20);
    iCount = m_pDispBuildItemList[iIndex]->m_iElementCount[6];

    if (iCount == 0) iMatch++;
    else
    {
        for (i = 1; i <= 6; i++)
        {
            if ((iItemIndex[i] != -1) && (memcmp(m_pItemList[iItemIndex[i]]->m_cName, cTempName, 20) == 0) &&
                (m_pItemList[iItemIndex[i]]->m_dwCount >= (uint32_t)(iCount)) &&
                (iItemCount[i] > 0) && (bItemFlag[i] == false))
            {
                iMatch++;
                iItemCount[i] -= iCount;
                bItemFlag[i] = true;
                goto CCBIS_STEP7;
            }
        }
    }

    CCBIS_STEP7:;

    iCount = 0;
    for (i = 1; i <= 6; i++)
        if (m_pDispBuildItemList[iIndex]->m_iElementCount[i] != 0) iCount++;

    iCount2 = 0;
    for (i = 1; i <= 6; i++)
        if (iItemIndex[i] != -1) iCount2++;

    if ((iMatch == 6) && (iCount == iCount2)) return true;

    return false;
}


int CGame::_iGetFOE(int iStatus)
{
    bool bPK, bCitizen, bAresden, bHunter;

    if (m_iPKCount != 0) return -1;

    //Change Added 3.51 support for possible status 0x8000 != 0x80000000 0x8000 = 0x00008000
    if (iStatus & 0x80000000) bPK = true;
    else bPK = false;
    if (iStatus & 0x40000000) bCitizen = true;
    else bCitizen = false;
    if (iStatus & 0x20000000) bAresden = true;
    else bAresden = false;
    if (iStatus & 0x10000000) bHunter = true;
    else bHunter = false;

    if (bPK == true) return -2;
    if (bCitizen == false) return 0;
    if (m_bCitizen == false) return 0;

    if ((m_bAresden == true) && (bAresden == true)) return 1;
    if ((m_bAresden == false) && (bAresden == false)) return 1;

    if (m_bIsCrusadeMode == true) return -1;
    else
    {
        if ((m_bHunter == false) && (bHunter == false)) return -1;
        else return 0;
    }
}

void CGame::_SetIlusionEffect(int iOwnerH)
{
    char cDir;

    m_iIlusionOwnerH = iOwnerH;

    memset(m_cName_IE, 0, sizeof(m_cName_IE));
    m_pMapData->GetOwnerStatusByObjectID(iOwnerH, &m_cIlusionOwnerType, &cDir, &m_sAppr1_IE, &m_sAppr2_IE, &m_sAppr3_IE, &m_sAppr4_IE, &m_iStatus_IE, &m_iApprColor_IE, m_cName_IE);
}

void CGame::CreateScreenShot()
{
    time_t t = time(0); // get time now
    struct tm * now = localtime(&t);

    _text.setOutlineThickness(0);
    _text.setFont(*default_font);
    _text.setString("Helbreath Xtreme");
    _text.setFillColor(sf::Color::White);
    sf::FloatRect bounds = _text.getLocalBounds();
    _text.setPosition(float(get_width() - 180), 10.f);
    _text.setCharacterSize(12);

    sf::Texture tex;
    sf::RenderTexture rtex;
    tex.create(window.getSize().x, window.getSize().y);
    rtex.create(window.getSize().x, window.getSize().y);
    tex.update(window);

    sf::Sprite sprite;
    sprite.setTexture(tex);

    rtex.draw(sprite);
    rtex.draw(_text);
    _text.setString(std::format("%F %T"));
    _text.setPosition(float(get_width() - 180), 30.f);
    rtex.draw(_text);

    auto ret = _mkdir("screenshots");
    std::string tempstr;

    for (int i = 0; i < 100; ++i)
    {
        if (i == 0)
            tempstr = std::format("screenshots\\HelShot{:4}{:2}{:2}_{:2}{:2}{:2}.jpg",
                (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday,
                now->tm_hour, now->tm_min, now->tm_sec);
        else
            tempstr = std::format("screenshots\\HelShot{:4}{:2}{:2}_{:2}{:2}{:2}_{}.jpg",
                (now->tm_year + 1900), (now->tm_mon + 1), now->tm_mday,
                now->tm_hour, now->tm_min, now->tm_sec, i);

        if (_access(tempstr.c_str(), 0) == -1)
        {
            sf::Image img = rtex.getTexture().copyToImage();
            img.flipVertically();
            img.saveToFile(tempstr);

            AddEventList(std::format(NOTIFYMSG_CREATE_SCREENSHOT1, tempstr.c_str()).c_str(), 10);
            return;
        }
    }
    AddEventList(NOTIFYMSG_CREATE_SCREENSHOT2, 10);
}


void CGame::EraseItem(char cItemID)
{
    int i;
    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    for (i = 0; i < 5; i++)
    {
        if (m_sShortCut[i] == cItemID)
        {
            GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);
            if (i < 3) format_to_local(G_cTxt, ERASE_ITEM, cStr1, cStr2, cStr3, i + 1);
            else format_to_local(G_cTxt, ERASE_ITEM, cStr1, cStr2, cStr3, i + 2);
            AddEventList(G_cTxt, 10);
            m_sShortCut[i] = -1;
        }
    }

    if (cItemID == m_sRecentShortCut)
        m_sRecentShortCut = -1;
    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_cItemOrder[i] == cItemID)
            m_cItemOrder[i] = -1;
    for (i = 1; i < DEF_MAXITEMS; i++)
        if ((m_cItemOrder[i - 1] == -1) && (m_cItemOrder[i] != -1))
        {
            m_cItemOrder[i - 1] = m_cItemOrder[i];
            m_cItemOrder[i] = -1;
        }
    delete m_pItemList[cItemID];
    m_pItemList[cItemID] = 0;
    m_bIsItemEquipped[cItemID] = false;
    m_bIsItemDisabled[cItemID] = false;
}

void CGame::DrawNpcName(short sX, short sY, short sOwnerType, int iStatus)
{
    char cTxt[64], cTxt2[128];
    memset(cTxt, 0, sizeof(cTxt));
    memset(cTxt2, 0, sizeof(cTxt2));

    GetNpcName(sOwnerType, cTxt);
    if ((iStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
    if ((iStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

    put_string2(sX, sY, cTxt, 255, 255, 255);

    if (m_bIsObserverMode == true) put_string2(sX, sY + 14, cTxt, 50, 50, 255);
    else if (m_bIsConfusion || (m_iIlusionOwnerH != 0))
    {
        memset(cTxt, 0, sizeof(cTxt));
        strcpy(cTxt, DRAW_OBJECT_NAME87);
        put_string2(sX, sY + 14, cTxt, 150, 150, 150); // v2.171
    }
    else
    {
        switch (_iGetFOE(iStatus))
        {
            case -2:
                put_string2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case -1:
                put_string2(sX, sY + 14, DRAW_OBJECT_NAME90, 255, 0, 0);
                break;
            case 0:
                put_string2(sX, sY + 14, DRAW_OBJECT_NAME88, 50, 50, 255);
                break;
            case 1:
                put_string2(sX, sY + 14, DRAW_OBJECT_NAME89, 30, 255, 30);
                break;
        }
    }

    switch ((iStatus & 0x0F00) >> 8)
    {
        case 0: break;
        case 1: strcpy(cTxt2, DRAW_OBJECT_NAME52); break;//"Clairvoyant"
        case 2: strcpy(cTxt2, DRAW_OBJECT_NAME53); break;//"Destruction of Magic Protection"
        case 3: strcpy(cTxt2, DRAW_OBJECT_NAME54); break;//"Anti-Physical Damage"
        case 4: strcpy(cTxt2, DRAW_OBJECT_NAME55); break;//"Anti-Magic Damage"
        case 5: strcpy(cTxt2, DRAW_OBJECT_NAME56); break;//"Poisonous"
        case 6: strcpy(cTxt2, DRAW_OBJECT_NAME57); break;//"Critical Poisonous" 
        case 7: strcpy(cTxt2, DRAW_OBJECT_NAME58); break;//"Explosive"  
        case 8: strcpy(cTxt2, DRAW_OBJECT_NAME59); break;//"Critical Explosive"
    }
    if (m_Misc.bCheckIMEString(cTxt2)) put_string_sprite_font3(sX, sY + 28, cTxt2, m_wR[13] * 4, m_wG[13] * 4, m_wB[13] * 4, false, 2);
    else put_string2(sX, sY + 28, cTxt2, 240, 240, 70);

#ifdef DEF_HACKCLIENT
    if (m_stConfigList.bDebugStatus == true)
    {
        if (strlen(cTxt2) > 0)
        {
            format_to_local(cTxt2, "iStatus(0x%X)", iStatus);
            put_string2(sX, sY + 28, cTxt2, 60, 160, 200);//Change Added -- Status
        }
        else
        {
            format_to_local(cTxt2, "iStatus(0x%X)", iStatus);
            put_string2(sX, sY + 38, cTxt2, 60, 160, 200);//Change Added -- Status
        }
    }
#endif

}

void CGame::DrawObjectName(short sX, short sY, char * pName, int iStatus)
{
    char cTxt[64], cTxt2[64];
    short sR, sG, sB;
    int i, iGuildIndex, iFOE, iAddY = 0;
    bool bPK, bCitizen, bAresden, bHunter;

    iFOE = _iGetFOE(iStatus);
    if (iFOE < 0)
    {
        sR = 255; sG = 0; sB = 0;
    }
    else if (iFOE == 0)
    {
        sR = 50; sG = 50; sB = 255;
    }
    else
    {
        sR = 30; sG = 200; sB = 30;
    }

    memset(cTxt, 0, sizeof(cTxt));
    memset(cTxt2, 0, sizeof(cTxt2));

    if (m_iIlusionOwnerH == 0)
    {
        if (m_bIsCrusadeMode == false) format_to_local(cTxt, "{}", pName);
        else
        {
            if (_tmp_wObjectID >= 10000) strcpy(cTxt, NPC_NAME_MERCENARY); //"¿ëº´"
            else
            {
                if (iFOE == -1)
                    //Change Crusade naming convention
                    format_to_local(cTxt, "{}", _tmp_wObjectID);
                //format_to_local(cTxt, "{}", pName);
                else strcpy(cTxt, pName);
            }
        }
        if (m_iPartyStatus != 0)
        {
            for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
            {
                if (strcmp(m_stPartyMemberNameList[i].cName, pName) == 0)
                {
                    strcat(cTxt, BGET_NPC_NAME23);
                    break;
                }
            }
        }
    }
    else strcpy(cTxt, "?????");

    if ((iStatus & 0x20) != 0) strcat(cTxt, DRAW_OBJECT_NAME50);//" Berserk" 
    if ((iStatus & 0x40) != 0) strcat(cTxt, DRAW_OBJECT_NAME51);//" Frozen"

    put_under_entity_string(sX, sY, cTxt, Color(255, 255, 255));

    memset(cTxt, 0, sizeof(cTxt));

    if (memcmp(m_cPlayerName, pName, 10) == 0)
    {
        if (m_iGuildRank == 0)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_cGuildName);
            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            iAddY = 14;
        }
        if (m_iGuildRank == 1)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDCOMASTER, m_cGuildName);
            put_string2(sX, sY + 14, G_cTxt, 180, 180, 180);
            iAddY = 14;
        }
        if (m_iGuildRank > 1)
        {
            format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_cGuildName);
            put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
            iAddY = 14;
        }
        if (m_iPKCount != 0)
        {
            bPK = true;
            sR = 255; sG = 0; sB = 0;
        }
        else
        {
            bPK = false;
            sR = 30; sG = 200; sB = 30;
        }
        bCitizen = m_bCitizen;
        bAresden = m_bAresden;
        bHunter = m_bHunter;
    }
    else
    {
        //Change Added 3.51 support for possible status 0x8000 != 0x80000000 0x8000 = 0x00008000
        if (m_iIlusionOwnerH == 0)
        {
            if (iStatus & 0x80000000) bPK = true;
            else bPK = false;
            if (iStatus & 0x40000000) bCitizen = true;
            else bCitizen = false;
            if (iStatus & 0x20000000) bAresden = true;
            else bAresden = false;
            if (iStatus & 0x10000000) bHunter = true;
            else bHunter = false;

            if (m_bIsCrusadeMode == false || iFOE >= 0)
            {
                if (FindGuildName(pName, &iGuildIndex) == true)
                {
                    if (m_stGuildName[iGuildIndex].cGuildName[0] != 0)
                    {
                        if (strcmp(m_stGuildName[iGuildIndex].cGuildName, "NONE") != 0)
                        {
                            if (m_stGuildName[iGuildIndex].iGuildRank == 0)
                            {
                                format_to_local(G_cTxt, DEF_MSG_GUILDMASTER, m_stGuildName[iGuildIndex].cGuildName);
                                put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                                m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                                iAddY = 14;
                            }
                            else if (m_stGuildName[iGuildIndex].iGuildRank == 1)
                            {
                                format_to_local(G_cTxt, DEF_MSG_GUILDCOMASTER, m_stGuildName[iGuildIndex].cGuildName);
                                put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                                m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                                iAddY = 14;
                            }
                            else if (m_stGuildName[iGuildIndex].iGuildRank > 1)
                            {
                                format_to_local(G_cTxt, DEF_MSG_GUILDSMAN, m_stGuildName[iGuildIndex].cGuildName);
                                put_under_entity_string(sX, sY + 14, G_cTxt, Color(180, 180, 180));
                                m_stGuildName[iGuildIndex].dwRefTime = m_dwCurTime;
                                iAddY = 14;
                            }
                        }
                        else
                        {
                            m_stGuildName[iGuildIndex].dwRefTime = 0;
                        }
                    }
                }
                else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQGUILDNAME, 0, _tmp_wObjectID, iGuildIndex, 0, 0);
            }
        }
        else strcpy(cTxt, "?????");
    }

    if (bCitizen == false)	strcpy(cTxt, DRAW_OBJECT_NAME60);
    else
    {
        if (bAresden)
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == true) strcpy(cTxt, DEF_MSG_ARECIVIL);
            else strcpy(cTxt, DEF_MSG_ARESOLDIER);
            //#else

            //#endif
        }
        else
        {
            //#if DEF_LANGUAGE > 2		// Korea 2.19
            if (bHunter == true) strcpy(cTxt, DEF_MSG_ELVCIVIL);
            else strcpy(cTxt, DEF_MSG_ELVSOLDIER);
            //#else
            //#endif
        }
    }
    if (bPK == true)
    {
        if (bCitizen == false)
        {

            for (i = 0; i < DEF_MAXPKS; i++)
            {
                if (strcmp(m_stPKList[i].m_cCharName, pName) == 0)
                {
                    if (m_stPKList[i].m_iPKs > 0)
                        strcpy(cTxt, DEF_MSG_PK);
//                     if ((m_stPKList[i].m_iPKs > 0) && (m_stPKList[i].m_iPKs < 5))
//                         strcpy(cTxt, DEF_MSG_PK);
//                     else if ((m_stPKList[i].m_iPKs > 4) && (m_stPKList[i].m_iPKs < 10))
//                         strcpy(cTxt, DEF_MSG_PK1);
//                     else if ((m_stPKList[i].m_iPKs > 9) && (m_stPKList[i].m_iPKs < 15))
//                         strcpy(cTxt, DEF_MSG_PK2);
//                     else if (m_stPKList[i].m_iPKs > 14)
//                         strcpy(cTxt, DEF_MSG_PK3);
                }
            }
        }
        else
        {
            if (bAresden)
            {

                for (i = 0; i < DEF_MAXPKS; i++)
                {
                    if (strcmp(m_stPKList[i].m_cCharName, pName) == 0)
                    {
                        if (m_stPKList[i].m_iPKs > 0)
                            strcpy(cTxt, DEF_MSG_AREPK);
//                         if ((m_stPKList[i].m_iPKs > 0) && (m_stPKList[i].m_iPKs < 5))
//                             strcpy(cTxt, DEF_MSG_AREPK);
//                         else if ((m_stPKList[i].m_iPKs > 4) && (m_stPKList[i].m_iPKs < 10))
//                             strcpy(cTxt, DEF_MSG_AREPK1);
//                         else if ((m_stPKList[i].m_iPKs > 9) && (m_stPKList[i].m_iPKs < 15))
//                             strcpy(cTxt, DEF_MSG_AREPK2);
//                         else if (m_stPKList[i].m_iPKs > 14)
//                             strcpy(cTxt, DEF_MSG_AREPK3);
                    }
                }
            }
            else
            {
                for (i = 0; i < DEF_MAXPKS; i++)
                {
                    if (strcmp(m_stPKList[i].m_cCharName, pName) == 0)
                    {
                        if (m_stPKList[i].m_iPKs > 0)
                            strcpy(cTxt, DEF_MSG_ELVPK);
//                         if ((m_stPKList[i].m_iPKs > 0) && (m_stPKList[i].m_iPKs < 5))
//                             strcpy(cTxt, DEF_MSG_ELVPK);
//                         else if ((m_stPKList[i].m_iPKs > 4) && (m_stPKList[i].m_iPKs < 10))
//                             strcpy(cTxt, DEF_MSG_ELVPK1);
//                         else if ((m_stPKList[i].m_iPKs > 9) && (m_stPKList[i].m_iPKs < 15))
//                             strcpy(cTxt, DEF_MSG_ELVPK2);
//                         else if (m_stPKList[i].m_iPKs > 14)
//                             strcpy(cTxt, DEF_MSG_ELVPK3);
                    }
                }
            }
        }
    }
    put_under_entity_string(sX, sY + 14 + iAddY, cTxt, Color(sR, sG, sB));


    if (FindPlayerName(pName, &iGuildIndex) == true)
    {
        if (m_stPKList[iGuildIndex].m_cDisplayInfo[0] != 0)
        {
            if (strcmp(m_stPKList[iGuildIndex].m_cCharName, pName) == 0)
            {
                if ((m_stPKList[iGuildIndex].m_bIsEnabled == true) && (memcmp(m_stPKList[iGuildIndex].m_cDisplayInfo, "     ", 5)))
                {
                    put_under_entity_string(sX, sY + 28 + iAddY, m_stPKList[iGuildIndex].m_cDisplayInfo, Color(255, 128, 0));
                    iAddY += 14;
                }

                if (m_stPKList[iGuildIndex].m_iGM == 1)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Entry level GM", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 2)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Event GM", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 3)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Game Master", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 4)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Senior Game Master", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 5)
                    put_under_entity_string(sX, sY + 28 + iAddY, "null", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 6)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Coder", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 7)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Head Game Master", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 8)
                    put_under_entity_string(sX, sY + 28 + iAddY, "Head Coder", Color(218, 248, 52));
                if (m_stPKList[iGuildIndex].m_iGM == 9)
                    put_under_entity_string(sX, sY + 42 + iAddY, "Admin", Color(218, 248, 52));
            }
            else
            {
                m_stPKList[iGuildIndex].m_dwRefTime = 0;
            }
        }
    }
    else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQPLAYERNAME, 0, _tmp_wObjectID, iGuildIndex, 0, 0);




    //	for (i = 0; i < DEF_MAXPKS; i++)
    //	{
    //		if ((strcmp(m_stPKList[i].m_cCharName, pName) == 0) && (m_stPKList[i].m_iGM > 0))
    //		{
    //			if ((strcmp(pName, "Wretch[GM]") == 0))
    //				put_string2(sX, sY+28 +iAddY, "Zero's Slave", 218, 248, 52);
    //			if ((strcmp(pName, "Zero") == 0))
    //				put_string2(sX, sY+28 +iAddY, "e=mc²", 70, 208, 255);
    //			if ((strcmp(pName, "Slash[GM]") == 0))
    //				put_string2(sX, sY+28 +iAddY, "Slash > Brandon", 73, 27, 174);
    ////			if ((strcmp(pName, "Andrew") == 0))
    ////				put_string2(sX, sY+28 +iAddY, "I'm gay", 183, 128, 234);
    //			//put_string2(sX, sY+28 +iAddY, "Omnipotent Deity", 70, 208, 255);
    //		}
    //		if (strcmp(m_stPKList[i].m_cCharName, pName) == 0)
    //		{
    //			if (m_stPKList[i].m_iGM == 1)
    //				put_string2(sX, sY+42 +iAddY, "Entry level GM", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 2)
    //				put_string2(sX, sY+42 +iAddY, "Event GM", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 3)
    //				put_string2(sX, sY+42 +iAddY, "Spanish Game Master", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 4)
    //				put_string2(sX, sY+42 +iAddY, "Game Master", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 5)
    //				put_string2(sX, sY+42 +iAddY, "null", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 6)
    //				put_string2(sX, sY+42 +iAddY, "null", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 7)
    //				put_string2(sX, sY+42 +iAddY, "Head Game Master", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 8)
    //				put_string2(sX, sY+42 +iAddY, "Head Coder", 218, 248, 52);
    //			if (m_stPKList[i].m_iGM == 9)
    //				put_string2(sX, sY+42 +iAddY, "Admin", 218, 248, 52);
    //		}
    //	}

#ifdef DEF_HACKCLIENT
    if (m_stConfigList.bDebugStatus == true)
    {
        format_to_local(cTxt2, "iStatus(0x%X) - {} - {} - {}", iStatus, ((_tmp_iStatus & 0x01000000) >> 24), ((_tmp_iStatus & 0x02000000) >> 25), ((_tmp_iStatus & 0x04000000) >> 26), ((_tmp_iStatus & 0x08000000) >> 27));
        put_under_entity_string(sX, sY + 28 + iAddY, cTxt2, Color(60, 160, 200));//Change Added -- Status
    }
#endif
}

bool CGame::FindGuildName(char * pName, int * ipIndex)
{
    int i, iRet = 0;
    uint32_t dwTmpTime;
    for (i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        if (memcmp(m_stGuildName[i].cCharName, pName, 10) == 0)
        {
            m_stGuildName[i].dwRefTime = m_dwCurTime;
            *ipIndex = i;
            return true;
        }
    }
    dwTmpTime = m_stGuildName[0].dwRefTime;
    for (i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        if (m_stGuildName[i].dwRefTime < dwTmpTime)
        {
            iRet = i;
            dwTmpTime = m_stGuildName[i].dwRefTime;
        }
    }
    memset(m_stGuildName[iRet].cGuildName, 0, sizeof(m_stGuildName[iRet].cGuildName));
    memcpy(m_stGuildName[iRet].cCharName, pName, 10);
    m_stGuildName[iRet].dwRefTime = m_dwCurTime;
    m_stGuildName[iRet].iGuildRank = -1;
    *ipIndex = iRet;
    return false;
}
bool CGame::FindPlayerName(char * pName, int * ipIndex)
{
    int i, iRet = 0;
    uint32_t dwTmpTime;
    for (i = 0; i < DEF_MAXPKS; i++)
    {
        if (memcmp(m_stPKList[i].m_cCharName, pName, 10) == 0)
        {
            m_stPKList[i].m_dwRefTime = m_dwCurTime;
            *ipIndex = i;
            return true;
        }
    }
    dwTmpTime = m_stPKList[0].m_dwRefTime;
    for (i = 0; i < DEF_MAXPKS; i++)
    {
        if (m_stPKList[i].m_dwRefTime < dwTmpTime)
        {
            iRet = i;
            dwTmpTime = m_stPKList[i].m_dwRefTime;
        }
    }
    memset(m_stPKList[iRet].m_cDisplayInfo, 0, sizeof(m_stPKList[iRet].m_cDisplayInfo));
    memcpy(m_stPKList[iRet].m_cCharName, pName, 10);
    m_stPKList[iRet].m_dwRefTime = m_dwCurTime;
    *ipIndex = iRet;
    return false;
}


//void CGame::DrawVersion()
//{
//	put_string2( 15, 455, "X-Mas Version", 255, 255, 0 );
//}

void CGame::DrawVersion()
{
    format_to_local(G_cTxt, "Version: {}.{}.{}", DEF_UPPERVERSION, DEF_LOWERVERSION, DEF_PATCHVERSION);
    put_string(get_virtual_width() - 100, 10, G_cTxt, Color(128, 128, 128, 128));
}

char CGame::GetOfficialMapName(char * pMapName, char * pName)
{
    if (strcmp(pMapName, "middleland") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME28);
        return 4;
    }
    else if (strcmp(pMapName, "huntzone3") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME31);
        return 0;
    }
    else if (strcmp(pMapName, "huntzone1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME29);
        return 1;
    }
    else if (strcmp(pMapName, "elvuni") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME57);
        return 2;
    }
    else if (strcmp(pMapName, "elvine") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME24);
        return 3;
    }
    else if (strcmp(pMapName, "elvfarm") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME2);
        return 5;
    }
    else if (strcmp(pMapName, "arefarm") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME1);
        return 6;
    }
    else if (strcmp(pMapName, "default") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME3);
        return 7;
    }
    else if (strcmp(pMapName, "huntzone4") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME32);
        return 8;
    }
    else if (strcmp(pMapName, "huntzone2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME30);
        return 9;
    }
    else if (strcmp(pMapName, "areuni") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME56);
        return 10;
    }
    else if (strcmp(pMapName, "aresden") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME22);
        return 11;
    }
    else if (strcmp(pMapName, "dglv2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME25);
        return 12;
    }
    else if (strcmp(pMapName, "dglv3") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME26);
        return 13;
    }
    else if (strcmp(pMapName, "dglv4") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME53);
        return 14;
    }
    else if (strcmp(pMapName, "elvined1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME23);
        return 15;
    }
    else if (strcmp(pMapName, "aresdend1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME21);
        return 16;
    }
    else if (strcmp(pMapName, "bisle") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME27);
        return 17;
    }
    else if (strcmp(pMapName, "toh1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME60);
        return 18;
    }
    else if (strcmp(pMapName, "toh2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME61);
        return 19;
    }
    else if (strcmp(pMapName, "toh3") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME62);
        return 20;
    }
    else if (strcmp(pMapName, "middled1x") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME58);
        return 21;
    }
    else if (strcmp(pMapName, "middled1n") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME59);
        return 22;
    }
    else if (strcmp(pMapName, "2ndmiddle") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME65);
        return 23;
    }
    else if (strcmp(pMapName, "icebound") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME66);
        return 24;
    }
    else if (strcmp(pMapName, "cityhall_1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME35);
        return -1;
    }
    else if (strcmp(pMapName, "cityhall_2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME36);
        return -1;
    }
    else if (strcmp(pMapName, "gldhall_1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME37);
        return -1;
    }
    else if (strcmp(pMapName, "gldhall_2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME38);
        return -1;
    }
    else if (memcmp(pMapName, "bsmith_1", 8) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME33);
        return -1;
    }
    else if (memcmp(pMapName, "bsmith_2", 8) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME34);
        return -1;
    }
    else if (memcmp(pMapName, "gshop_1", 7) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME39);
        return -1;
    }
    else if (memcmp(pMapName, "gshop_2", 7) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME40);
        return -1;
    }
    else if (memcmp(pMapName, "wrhus_1", 7) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME43);
        return -1;
    }
    else if (memcmp(pMapName, "wrhus_2", 7) == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME44);
        return -1;
    }
    else if (strcmp(pMapName, "arewrhus") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME45);
        return -1;
    }
    else if (strcmp(pMapName, "elvwrhus") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME46);
        return -1;
    }
    else if (strcmp(pMapName, "wzdtwr_1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME41);
        return -1;
    }
    else if (strcmp(pMapName, "wzdtwr_2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME42);
        return -1;
    }
    else if (strcmp(pMapName, "cath_1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME47);
        return -1;
    }
    else if (strcmp(pMapName, "cath_2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME48);
        return -1;
    }
    else if (strcmp(pMapName, "resurr1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME54);
        return -1;
    }
    else if (strcmp(pMapName, "resurr2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME55);
        return -1;
    }
    else if (strcmp(pMapName, "arebrk11") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME4);
        return -1;
    }
    else if (strcmp(pMapName, "arebrk12") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME5);
        return -1;
    }
    else if (strcmp(pMapName, "arebrk21") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME6);
        return -1;
    }
    else if (strcmp(pMapName, "arebrk22") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME7);
        return -1;
    }
    else if (strcmp(pMapName, "elvbrk11") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME8);
        return -1;
    }
    else if (strcmp(pMapName, "elvbrk12") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME9);
        return -1;
    }
    else if (strcmp(pMapName, "elvbrk21") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME10);
        return -1;
    }
    else if (strcmp(pMapName, "elvbrk22") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME11);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME12);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME13);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone3") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME14);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone4") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME15);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone5") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME16);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone6") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME17);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone7") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME18);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone8") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME19);
        return -1;
    }
    else if (strcmp(pMapName, "fightzone9") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME20);
        return -1;
    }
    else if (strcmp(pMapName, "arejail") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME63);
        return -1;
    }
    else if (strcmp(pMapName, "elvjail") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME64);
        return -1;
    }
    else if (strcmp(pMapName, "druncncity") == 0)
    {//Change Added new mapname checking
        strcpy(pName, GET_OFFICIAL_MAP_NAME67);
        return -1;
    }
    else if (strcmp(pMapName, "inferniaa") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME68);
        return -1;
    }
    else if (strcmp(pMapName, "inferniab") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME69);
        return -1;
    }
    else if (strcmp(pMapName, "procella") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME70);
        return -1;
    }
    else if (strcmp(pMapName, "maze") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME71);
        return -1;
    }
    else if (strcmp(pMapName, "CmdHall_1") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME72);
        return -1;
    }
    else if (strcmp(pMapName, "CmdHall_2") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME73);
        return -1;
    }
    else if (strcmp(pMapName, "btfield") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME74);
        return -1;
    }
    else if (strcmp(pMapName, "hrampart") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME75);
        return -1;
    }
    else if (strcmp(pMapName, "godh") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME76);
        return -1;
    }
    else if (strcmp(pMapName, "abaddon") == 0)
    {
        strcpy(pName, GET_OFFICIAL_MAP_NAME77);
        return -1;
    }
    else
    {
        strcpy(pName, pMapName);
        return -1;
    }
}
void CGame::LOL(int iType, int iNum)
{
    uint32_t * dwp;
    uint16_t * wp;
    CHAR * cp, cLOL[1000];
    int iRet;
    memset(cLOL, 0, 50);
    dwp = (uint32_t *)(cLOL + DEF_INDEX4_MSGID);
    *dwp = MSGID_LEVELUPSETTINGS;
    wp = (uint16_t *)(cLOL + DEF_INDEX2_MSGTYPE);
    *wp = 0;

    cp = (char *)(cLOL + DEF_INDEX2_MSGTYPE + 2);

    if (iType == 1)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    if (iType == 2)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    if (iType == 3)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    if (iType == 4)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    if (iType == 5)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    if (iType == 6)
        *cp = iNum;
    else
        *cp = 0;
    cp++;

    iRet = send_message(cLOL, 12);
}
bool CGame::bCheckLocalChatCommand(char * pMsg)
{
     CStrTok * pStrTok = 0;
    char * token, cBuff[256], cTxt[120], cName[12], cTemp[120];
    char   seps[] = " \t\n";

    memset(cBuff, 0, sizeof(cBuff));
    memset(cName, 0, sizeof(cName));
    strcpy(cBuff, pMsg);
    if (memcmp(cBuff, "/showframe", 10) == 0)
    {
        if (m_bShowFPS) m_bShowFPS = false;
        else m_bShowFPS = true;
        return true;
    }
    if (memcmp(cBuff, "/enabletogglescreen", 19) == 0)
    {
        m_bToggleScreen = true;
        return true;
    }
    if (memcmp(cBuff, "/flist", 6) == 0)
    {
        EnableDialogBox(70, 0, 0, 0);
        return true;
    }
    if (memcmp(cBuff, "/config", 7) == 0)
    {
        EnableDialogBox(73, 0, 0, 0);
    }
#ifdef DEF_ADMINCLIENT
    if (memcmp(cBuff, "/admin", 6) == 0)
    {
        EnableDialogBox(71, 0, 0, 0);
        return true;
    }
    if (memcmp(cBuff, "/box ", 5) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            int iNum2 = atoi(token);
            token = pStrTok->pGet();
            if (token[0] == 1)
            {
                token = pStrTok->pGet();
                int iNum3 = atoi(token);
                token = pStrTok->pGet();
                int iNum4 = atoi(token);
                token = pStrTok->pGet();
                int iNum5 = atoi(token);
                token = pStrTok->pGet();
                if (token != 0)
                    EnableDialogBox(iNum2, iNum3, iNum4, iNum5, token);
                else
                    EnableDialogBox(iNum2, iNum3, iNum4, iNum5);
            }
            else
                DisableDialogBox(iNum2);
        }
        return true;
    }

    if (memcmp(cBuff, "/enable ", 8) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            if (memcmp(token, "shownames", 9) == 0)
            {
                m_stConfigList.bShowNames = true;
                AddEventList("Names have been set to always show.", 10);
            }
            else if (memcmp(token, "mtdisplay", 9) == 0)
            {
                m_stConfigList.bMouseTileDebug = true;
                AddEventList("Mouse and Tile display has been enabled", 10);
            }
            else if (memcmp(token, "debugnpc", 8) == 0)
            {
                m_stConfigList.bDebugNpc = true;
                AddEventList("NPC Debugging has been enabled", 10);
            }
            else if (memcmp(token, "status", 6) == 0)
            {
                m_stConfigList.bDebugStatus = true;
                AddEventList("Status Debugging has been enabled", 10);
            }
            else if (memcmp(token, "fastcast", 8) == 0)
            {
                m_stConfigList.bFastCast = true;
                AddEventList("Fast casting has been enabled", 10);
            }
            else if (memcmp(token, "camerashake", 6) == 0)
            {
                m_stConfigList.bCameraShake = true;
                AddEventList("Camera shaking has been enabled", 10);
            }
            else if (memcmp(token, "poison", 6) == 0)
            {
                m_bIsPoisoned = true;
                AddEventList("Poison status has been enabled", 10);
            }
            else if (memcmp(token, "gmchat", 6) == 0)
            {
                m_stConfigList.bGMChat = true;
                AddEventList("Player GM chat enabled.. may god have mercy on you", 10);
            }
        }

        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/disable ", 9) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            if (memcmp(token, "shownames", 9) == 0)
            {
                m_stConfigList.bShowNames = false;
                AddEventList("Names have been set to only show on control press.", 10);
            }
            else if (memcmp(token, "mtdisplay", 9) == 0)
            {
                m_stConfigList.bMouseTileDebug = false;
                AddEventList("Mouse and Tile display has been disabled", 10);
            }
            else if (memcmp(token, "debugnpc", 8) == 0)
            {
                m_stConfigList.bDebugNpc = false;
                AddEventList("NPC Debugging has been disabled", 10);
            }
            else if (memcmp(token, "status", 6) == 0)
            {
                m_stConfigList.bDebugStatus = false;
                AddEventList("Status Debugging has been disabled", 10);
            }
            else if (memcmp(token, "fastcast", 8) == 0)
            {
                m_stConfigList.bFastCast = false;
                AddEventList("Fast casting has been disabled", 10);
            }
            else if (memcmp(token, "camerashake", 6) == 0)
            {
                m_stConfigList.bCameraShake = false;
                AddEventList("Camera shaking has been disabled", 10);
            }
            else if (memcmp(token, "poison", 6) == 0)
            {
                m_bIsPoisoned = false;
                AddEventList("Poison status has been disabled", 10);
            }
            else if (memcmp(token, "gmchat", 6) == 0)
            {
                m_stConfigList.bGMChat = true;
                AddEventList("The ever so annoying player GM chat has been disabled", 10);
            }
        }

        delete pStrTok;
        return true;
    }
#endif
    //Change
    if (memcmp(cBuff, "/cversion", 9) == 0)
    {
        format_to_local(G_cTxt, "Helbreath Xtreme ({}.{})", m_iUpperVersion, m_iLowerVersion);
        AddEventList(G_cTxt, 10);
    }
    //if (memcmp(cBuff, "/chgname ", 9) == 0)
    //{
    //	pStrTok = new CStrTok(cBuff, seps);
    //	token = pStrTok->pGet();
    //	token = pStrTok->pGet();
    //	memset(m_cPlayerName, 0, 12);
    //	memcpy(m_cPlayerName, token, 12);
    //	delete pStrTok;
    //	return true;
    //}
    if (memcmp(cBuff, "/str+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(1, 1);
        else
            LOL(1, atoi(token));
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/vit+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(2, 1);
        else
            LOL(2, atoi(token));
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/dex+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(3, 1);
        else
            LOL(3, atoi(token));
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/int+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(4, 1);
        else
            LOL(4, atoi(token));
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/mag+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(5, 1);
        else
            LOL(5, atoi(token));
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/chr+", 4) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token == 0)
            LOL(6, 1);
        else
            LOL(6, atoi(token));
        delete pStrTok;
        return true;
    }
#ifdef DEF_HACKCLIENT
    if (memcmp(cBuff, "/cast ", 6) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            int iSpell = atoi(token), iX, iY;
            token = pStrTok->pGet();
            if (token == 0)
                iX = m_sPlayerX;
            else
                iX = atoi(token);
            token = pStrTok->pGet();
            if (token == 0)
                iY = m_sPlayerY;
            else
                iY = atoi(token);

            m_iPointCommandType = iSpell + 100;
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, iX, iY, m_iPointCommandType, 0);
            //UseMagic(iCheck);
        }
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/set ", 5) == 0)
    {
        char cNpcName[30];
        char cTxtTest[100];
        memset(cNpcName, 0, 30);
        memset(cTxtTest, 0, 100);
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        int iNumNpc = atoi(token);
        token = pStrTok->pGet();
        int iTypeNpc = atoi(token);
        token = pStrTok->pGet();
        int iValueNpc = atoi(token);
        token = pStrTok->pGet();
        int iWhichNpc = atoi(token);
        if (token != 0)
        {
            GetNpcName(iNumNpc, cNpcName);
            switch (iWhichNpc)
            {
                case 1:
                    m_pMapData->m_stFrame[iNumNpc][iTypeNpc].m_sMaxFrame = iValueNpc;
                    format_to_local(cTxtTest, "Number Of Frames for npc {} set to {}", cNpcName, iValueNpc);
                    AddEventList(cTxtTest, 10);
                    break;
                case 2:
                    m_pMapData->m_stFrame[iNumNpc][iTypeNpc].m_sFrameTime = iValueNpc;
                    format_to_local(cTxtTest, "Frame Rate for npc {} set to {}", cNpcName, iValueNpc);
                    AddEventList(cTxtTest, 10);
                    break;
            }
        }
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/crash", 6) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            int iSpell = atoi(token);
            m_iPointCommandType = iSpell + 100;
            bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTMAGIC, m_cPlayerDir, m_iCastingMagicType, 0, 0, 0);
            UseMagic(iSpell);
        }
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/skill", 6) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            int iSpell = atoi(token);
            token = pStrTok->pGet();
            int iSpell2 = atoi(token);
            m_iCastingMagicType = iSpell + 100;
            m_cCommand = DEF_OBJECTMAGIC;
            m_iPointCommandType = iSpell2;
            //bSendCommand(MSGID_COMMAND_MOTION, DEF_OBJECTMAGIC, m_cPlayerDir, m_iCastingMagicType, 0, 0, 0);
            UseMagic(iSpell);
        }
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/makeguild", 10) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            char cTmp2[100];
            memset(cTmp2, 0, 100);
            memcpy(m_cGuildName, token, 20);
            bSendCommand(MSGID_REQUEST_CREATENEWGUILD, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
            format_to_local(cTmp2, "Guild attempting to be made \"{}\"", m_cGuildName);
            AddEventList(cTmp2, 10);
        }
        delete pStrTok;
        return true;
    }
    if (memcmp(cBuff, "/civil", 6) == 0)
    {
        bSendCommand(MSGID_REQUEST_CIVILRIGHT, DEF_MSGTYPE_CONFIRM, 0, 0, 0, 0, 0);
        delete pStrTok;
        return true;
    }
#endif
    if (memcmp(cBuff, "set", 3) == 0)
    {
        G_iStuff = atoi(cBuff + 3);
    }
//     if (memcmp(cBuff, "/res ", 5) == 0)
//     {
//         pStrTok = new CStrTok(cBuff, seps);
//         token = pStrTok->pGet();
//         token = pStrTok->pGet();
//         get_virtual_height() = atoi(token);
//         token = pStrTok->pGet();
//         get_virtual_width() = atoi(token);
//         m_DDraw.bInit(m_hWnd);
//     }
    if (memcmp(cBuff, "/whon", 5) == 0)
    {
        m_bWhisper = true;
        AddEventList(BCHECK_LOCAL_CHAT_COMMAND6, 10);
        return true;
    }

    else if (memcmp(cBuff, "/whoff", 6) == 0)
    {
        m_bWhisper = false;
        AddEventList(BCHECK_LOCAL_CHAT_COMMAND7, 10);
        return true;
    }

    else if (memcmp(cBuff, "/shon", 5) == 0)
    {
        m_bShout = true;
        AddEventList(BCHECK_LOCAL_CHAT_COMMAND8, 10);
        return true;
    }

    else if (memcmp(cBuff, "/shoff", 6) == 0)
    {
        m_bShout = false;
        AddEventList(BCHECK_LOCAL_CHAT_COMMAND9, 10);
        return true;
    }

    if (memcmp(cBuff, "/tooff", 6) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();
        if (token != 0)
        {
            if (strlen(token) <= 10)
            {
                strcpy(cName, token);
                if (memcmp(m_cPlayerName, cName, 10) == 0)
                {
                    AddEventList(BCHECK_LOCAL_CHAT_COMMAND2, 10);
                    if (pStrTok != 0) delete pStrTok;
                    return true;
                }
                if (m_pExID != 0) delete m_pExID;
                format_to_local(cTxt, BCHECK_LOCAL_CHAT_COMMAND3, token);
                AddEventList(cTxt, 10);
                m_pExID = new CMsg(0, token, 0);
                if (pStrTok != 0) delete pStrTok;
                return true;
            }
            else AddEventList(BCHECK_LOCAL_CHAT_COMMAND5, 10);
        }
        if (pStrTok != 0) delete pStrTok;
        return true;
    }

    else if (memcmp(cBuff, "/toon", 5) == 0)
    {
        pStrTok = new CStrTok(cBuff, seps);
        token = pStrTok->pGet();
        token = pStrTok->pGet();

        if (token != 0)
        {
            if (strlen(token) <= 10)
            {
                strcpy(cName, token);
                if (m_pExID != 0)
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    strcpy(cTemp, m_pExID->message.c_str());
                    if (memcmp(cTemp, cName, 10) == 0)
                    {
                        format_to_local(cTxt, BCHECK_LOCAL_CHAT_COMMAND1, token);
                        AddEventList(cTxt, 10);
                        delete m_pExID;
                        m_pExID = 0;
                        if (pStrTok != 0) delete pStrTok;
                        return true;
                    }
                }
            }
            else AddEventList(BCHECK_LOCAL_CHAT_COMMAND5, 10);
        }
        if (pStrTok != 0) delete pStrTok;
        return true;
    }

    if (pStrTok != 0) delete pStrTok;
    return false;
}

bool CGame::bCheckItemOperationEnabled(char cItemID)
{
    if (m_pItemList[cItemID] == 0) return false;
    if (m_cCommand < 0) return false;
    if (m_bIsTeleportRequested == true) return false;
    if (m_bIsItemDisabled[cItemID] == true) return false;
    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    // v1.41
    if (m_bIsDialogEnabled[20] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    if (m_bIsDialogEnabled[26] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    if (m_bIsDialogEnabled[27] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    if (m_bIsDialogEnabled[31] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    if (m_bIsDialogEnabled[4] == true)
    {
        AddEventList(BCHECK_ITEM_OPERATION_ENABLE1, 10);
        return false;
    }

    return true;
}

void CGame::ClearSkillUsingStatus()
{
    if (m_bSkillUsingStatus == true)
    {
        AddEventList(CLEAR_SKILL_USING_STATUS1, 10);
        DisableDialogBox(24);
        DisableDialogBox(26);
        if ((m_sPlayerType >= 1) && (m_sPlayerType <= 6)/* && ((m_sPlayerAppr2 & 0xF000) == 0)*/)
        {
            m_cCommand = DEF_OBJECTSTOP;
            m_sCommX = m_sPlayerX;
            m_sCommY = m_sPlayerY;
        }
    }
    m_bSkillUsingStatus = false;
}



void CGame::GetNpcName(short sType, char * pName)
{
    switch (sType)
    {
        case 10: strcpy(pName, NPC_NAME_SLIME); break;
        case 11: strcpy(pName, NPC_NAME_SKELETON); break;
        case 12: strcpy(pName, NPC_NAME_STONEGOLEM); break;
        case 13: strcpy(pName, NPC_NAME_CYCLOPS); break;
        case 14: strcpy(pName, NPC_NAME_ORC); break;
        case 15: strcpy(pName, NPC_NAME_SHOP_KEEPER); break;
        case 16: strcpy(pName, NPC_NAME_GIANTANT); break;
        case 17: strcpy(pName, NPC_NAME_GIANTSCORPION); break;
        case 18: strcpy(pName, NPC_NAME_ZOMBIE); break;
        case 19: strcpy(pName, NPC_NAME_MAGICIAN); break;
        case 20: strcpy(pName, NPC_NAME_WAREHOUSE_KEEPER); break;
        case 21: strcpy(pName, NPC_NAME_GUARD); break;
        case 22: strcpy(pName, NPC_NAME_SNAKE); break;
        case 23: strcpy(pName, NPC_NAME_CLAYGOLEM); break;
        case 24: strcpy(pName, NPC_NAME_BLACKSMITH_KEEPER); break;
        case 25: strcpy(pName, NPC_NAME_CITYHALL_OFFICER); break;
        case 26: strcpy(pName, NPC_NAME_GUILDHALL_OFFICER); break;
        case 27: strcpy(pName, NPC_NAME_HELHOUND); break;
        case 28: strcpy(pName, NPC_NAME_TROLL); break;
        case 29: strcpy(pName, NPC_NAME_OGRE); break;
        case 30: strcpy(pName, NPC_NAME_LICHE); break;//"¸®Ä¡"
        case 31: strcpy(pName, NPC_NAME_DEMON); break;
        case 32: strcpy(pName, NPC_NAME_UNICORN); break;
        case 33: strcpy(pName, NPC_NAME_WEREWOLF); break;
        case 34: strcpy(pName, NPC_NAME_DUMMY); break;
        case 35: strcpy(pName, NPC_NAME_ENERGYSPHERE); break;
        case 36:
            if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_ARROWGUARDTOWER_CK);
            else strcpy(pName, NPC_NAME_ARROWGUARDTOWER);
            break;
        case 37:
            if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_CANNONGUARDTOWER_CK);
            else strcpy(pName, NPC_NAME_CANNONGUARDTOWER);
            break;
        case 38:
            if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_MANACOLLECTOR_CK);
            else strcpy(pName, NPC_NAME_MANACOLLECTOR);
            break;
        case 39:
            if (_tmp_sAppr2 != 0) strcpy(pName, NPC_NAME_DETECTOR_CK);
            else strcpy(pName, NPC_NAME_DETECTOR);
            break;
        case 40: strcpy(pName, NPC_NAME_ENERGYSHIELD); break;
        case 41: strcpy(pName, NPC_NAME_GRANDMAGICGENERATOR); break;
        case 42: strcpy(pName, NPC_NAME_MANASTONE); break;
        case 43: strcpy(pName, NPC_NAME_LIGHTWARBEETLE); break;
        case 44: strcpy(pName, NPC_NAME_GODSHANDKNIGHT); break;
        case 45: strcpy(pName, NPC_NAME_GODSHANDKNIGHT_CK); break;
        case 46: strcpy(pName, NPC_NAME_TEMPLEKNIGHT); break;
        case 47: strcpy(pName, NPC_NAME_BATTLEGOLEM); break;
        case 48: strcpy(pName, NPC_NAME_STALKER); break;
        case 49: strcpy(pName, NPC_NAME_HELLCLAW); break;
        case 50: strcpy(pName, NPC_NAME_TIGERWORM); break;
        case 51: strcpy(pName, NPC_NAME_CATAPULT); break;
        case 52: strcpy(pName, NPC_NAME_GARGOYLE); break;
        case 53: strcpy(pName, NPC_NAME_BEHOLDER); break;
        case 54: strcpy(pName, NPC_NAME_DARKELF); break;
        case 55: strcpy(pName, NPC_NAME_RABBIT); break;
        case 56: strcpy(pName, NPC_NAME_CAT); break;
        case 57: strcpy(pName, NPC_NAME_FROG); break;
        case 58: strcpy(pName, NPC_NAME_MOUNTAIN_GIANT); break;
        case 59: strcpy(pName, NPC_NAME_ETTIN); break;
        case 60: strcpy(pName, NPC_NAME_CANNIBAL); break;
        case 61: strcpy(pName, NPC_NAME_RUDOLPH); break;
        case 62: strcpy(pName, NPC_NAME_DIREBOAR); break;
        case 63: strcpy(pName, NPC_NAME_FROST); break;
        case 64:
        {
            switch ((_tmp_sAppr2 & 0xFF00) >> 8)
            {
                case 1:
                    strcpy(pName, NPC_NAME_WATERMELON);
                    break;
                case 2:
                    strcpy(pName, NPC_NAME_PUMPKIN);
                    break;
                case 3:
                    strcpy(pName, NPC_NAME_GARLIC);
                    break;
                case 4:
                    strcpy(pName, NPC_NAME_BARLEY);
                    break;
                case 5:
                    strcpy(pName, NPC_NAME_CARROT);
                    break;
                case 6:
                    strcpy(pName, NPC_NAME_RADISH);
                    break;
                case 7:
                    strcpy(pName, NPC_NAME_CHBFLOWER);
                    break;
                case 8:
                    strcpy(pName, NPC_NAME_MELON);
                    break;
                case 9:
                    strcpy(pName, NPC_NAME_TOMATO);
                    break;
                case 10:
                    strcpy(pName, NPC_NAME_GRAPES);
                    break;
                case 11:
                    strcpy(pName, NPC_NAME_BGRAPES);
                    break;
                case 12:
                    strcpy(pName, NPC_NAME_MUSHROOM);
                    break;
                case 13:
                    strcpy(pName, NPC_NAME_GINSENG);
                    break;
                default:
                    strcpy(pName, NPC_NAME_CROP);
                    break;
            }
        }
        break;
        case 65: strcpy(pName, NPC_NAME_ICEGOLEM); break;
        case 66: strcpy(pName, NPC_NAME_WYVERN); break;
        case 67: strcpy(pName, NPC_NAME_MCGAFFIN); break;//"Ã»³â"
        case 68: strcpy(pName, NPC_NAME_PERRY); break;
        case 69: strcpy(pName, NPC_NAME_DEVLIN); break;

            //Change Added NPC Names
        case 70: strcpy(pName, NPC_NAME_BARLOG); break;
        case 71: strcpy(pName, NPC_NAME_CENTAURUS); break;
        case 72: strcpy(pName, NPC_NAME_CLAWTURTLE); break;
        case 73: strcpy(pName, NPC_NAME_FIREWYVERN); break;
        case 74: strcpy(pName, NPC_NAME_GIANTCRAYFISH); break;
        case 75: strcpy(pName, NPC_NAME_GIANTLIZARD); break;
        case 76: strcpy(pName, NPC_NAME_GIANTPLANT); break;
        case 77: strcpy(pName, NPC_NAME_MASTERMAGEORC); break;
        case 78: strcpy(pName, NPC_NAME_MINOTAURS); break;
        case 79: strcpy(pName, NPC_NAME_NIZIE); break;
        case 80: strcpy(pName, NPC_NAME_TENTOCLE); break;
        case 81: strcpy(pName, NPC_NAME_ABADDON); break;

        case 82: strcpy(pName, NPC_NAME_SOR); break;
        case 83: strcpy(pName, NPC_NAME_ATK); break;
        case 84: strcpy(pName, NPC_NAME_ELF); break;
        case 85: strcpy(pName, NPC_NAME_DSK); break;
        case 86: strcpy(pName, NPC_NAME_HBT); break;
        case 87: strcpy(pName, NPC_NAME_CT); break;
        case 88: strcpy(pName, NPC_NAME_BAR); break;
        case 89: strcpy(pName, NPC_NAME_AGC); break;
        case 90: strcpy(pName, NPC_NAME_GAIL); break;
        case 91: strcpy(pName, NPC_NAME_GATE); break;
    }
}

void CGame::GetItemName(CItem * pItem, char * pStr1, char * pStr2, char * pStr3, std::size_t len)
{
    int i;
    char cTxt[256], cTxt2[256], cName[51], cTmp[50];
    uint32_t dwType1, dwType2, dwValue1, dwValue2, dwValue3;

    m_bIsSpecial = false;

    memset(cName, 0, sizeof(cName));
    memset(pStr1, 0, sizeof(pStr1));
    memset(pStr2, 0, sizeof(pStr2));
    memset(pStr3, 0, sizeof(pStr3));
    memset(cTmp, 0, sizeof(cTmp));

    strcpy(cName, pItem->m_cName);
    for (i = 0; i < DEF_MAXITEMNAMES; i++)
        if ((m_pItemNameList[i] != 0) && (strcmp(m_pItemNameList[i]->m_cOriginName, pItem->m_cName) == 0))
        {
            strcpy(cName, m_pItemNameList[i]->m_cName);
            break;
        }

    if ((pItem->m_dwAttribute & 0x00000001) != 0)
    {
        m_bIsSpecial = true;
        strcpy(pStr1, cName);
        if (pItem->m_cItemType == DEF_ITEMTYPE_MATERIAL)
            format_to_local(pStr2, len, GET_ITEM_NAME1, pItem->m_sItemSpecEffectValue2);
        else format_to_local(pStr2, len, GET_ITEM_NAME2, pItem->m_sItemSpecEffectValue2 + 100);
    }
    else
    {
        if (pItem->m_dwCount == 1)
            format_to_local(cTmp, "{}", cName);
        else
        {
            DisplayGold(pItem->m_dwCount);//Change comma gold/arrows
            //format_to_local(cTmp, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, G_cTxt, cName);
            format_to_local(cTmp, "{} {}", G_cTxt, cName);
        }
        strcpy(pStr1, cTmp);
    }

    if ((pItem->m_dwAttribute & 0x00F0F000) != 0)
    {
        m_bIsSpecial = true;
        dwType1 = (pItem->m_dwAttribute & 0x00F00000) >> 20;
        dwValue1 = (pItem->m_dwAttribute & 0x000F0000) >> 16;

        dwType2 = (pItem->m_dwAttribute & 0x0000F000) >> 12;
        dwValue2 = (pItem->m_dwAttribute & 0x00000F00) >> 8;

        if (dwType1 != 0)
        {

            memset(cTxt, 0, sizeof(cTxt));
            switch (dwType1)
            {
                case 1: strcpy(cTxt, GET_ITEM_NAME3);   break;
                case 2: strcpy(cTxt, GET_ITEM_NAME4);   break;
                case 3: strcpy(cTxt, GET_ITEM_NAME5);   break;
                case 4: break;
                case 5: strcpy(cTxt, GET_ITEM_NAME6);   break;
                case 6: strcpy(cTxt, GET_ITEM_NAME7);   break;
                case 7: strcpy(cTxt, GET_ITEM_NAME8);   break;
                case 8: strcpy(cTxt, GET_ITEM_NAME9);   break;
                case 9: strcpy(cTxt, GET_ITEM_NAME10);  break;
                case 10: strcpy(cTxt, GET_ITEM_NAME11); break;
                case 11: strcpy(cTxt, GET_ITEM_NAME12); break;
                case 12: strcpy(cTxt, GET_ITEM_NAME13); break;
            }
            strcat(cTxt, pStr1);
            memset(pStr1, 0, sizeof(pStr1));
            strcpy(pStr1, cTxt);

            memset(cTxt, 0, sizeof(cTxt));
            switch (dwType1)
            {
                case 1: format_to_local(cTxt, GET_ITEM_NAME14, dwValue1); break;
                case 2: format_to_local(cTxt, GET_ITEM_NAME15, dwValue1 * 5); break;
                case 3: break;
                case 4: break;
                case 5: strcpy(cTxt, GET_ITEM_NAME16); break;
                case 6: format_to_local(cTxt, GET_ITEM_NAME17, dwValue1 * 4); break;
                case 7: strcpy(cTxt, GET_ITEM_NAME18); break;
                case 8: format_to_local(cTxt, GET_ITEM_NAME19, dwValue1 * 7); break;
                case 9: strcpy(cTxt, GET_ITEM_NAME20); break;
                case 10: format_to_local(cTxt, GET_ITEM_NAME21, dwValue1 * 3); break;
                case 11: format_to_local(cTxt, GET_ITEM_NAME22, dwValue1); break;
                case 12: format_to_local(cTxt, GET_ITEM_NAME23, dwValue1); break;
            }
            strcat(pStr2, cTxt);

            if (dwType2 != 0)
            {
                memset(cTxt, 0, sizeof(cTxt));
                switch (dwType2)
                {
                    case 1:  format_to_local(cTxt, GET_ITEM_NAME24, dwValue2 * 7); break;
                    case 2:  format_to_local(cTxt, GET_ITEM_NAME25, dwValue2 * 7); break;
                    case 3:  format_to_local(cTxt, GET_ITEM_NAME26, dwValue2 * 7); break;
                    case 4:  format_to_local(cTxt, GET_ITEM_NAME27, dwValue2 * 7); break;
                    case 5:  format_to_local(cTxt, GET_ITEM_NAME28, dwValue2 * 7); break;
                    case 6:  format_to_local(cTxt, GET_ITEM_NAME29, dwValue2 * 7); break;
                    case 7:  format_to_local(cTxt, GET_ITEM_NAME30, dwValue2 * 7); break;
                    case 8:  format_to_local(cTxt, GET_ITEM_NAME31, dwValue2 * 3); break;
                    case 9:  format_to_local(cTxt, GET_ITEM_NAME32, dwValue2 * 3); break;
                    case 10: format_to_local(cTxt, GET_ITEM_NAME33, dwValue2);   break;
                    case 11: format_to_local(cTxt, GET_ITEM_NAME34, dwValue2 * 10); break;
                    case 12: format_to_local(cTxt, GET_ITEM_NAME35, dwValue2 * 10); break;//"Gold +{}%% "
                }
                strcpy(pStr3, cTxt);
            }
        }
    }

    dwValue3 = (pItem->m_dwAttribute & 0xF0000000) >> 28;
    if (dwValue3 > 0)
    {
        if (pStr1[strlen(pStr1) - 2] == '+')
        {
            dwValue3 = atoi((char *)(pStr1 + strlen(pStr1) - 1)) + dwValue3;
            memset(cTxt, 0, sizeof(cTxt));
            memcpy(cTxt, pStr1, strlen(pStr1) - 2);
            memset(cTxt2, 0, sizeof(cTxt2));
            format_to_local(cTxt2, "{}+{}", cTxt, dwValue3);
            memset(pStr1, 0, sizeof(pStr1));
            strcpy(pStr1, cTxt2);
        }
        else
        {
            memset(cTxt, 0, sizeof(cTxt));
            format_to_local(cTxt, "+{}", dwValue3);
            strcat(pStr1, cTxt);
        }
    }
}

void CGame::GetItemName(char * cItemName, uint32_t dwAttribute, char * pStr1, char * pStr2, char * pStr3, std::size_t len)
{
    int i;
    char cTxt[256], cTxt2[256], cName[51];
    uint32_t dwType1, dwType2, dwValue1, dwValue2, dwValue3;

    m_bIsSpecial = false;
    memset(cName, 0, sizeof(cName));
    memset(pStr1, 0, sizeof(pStr1));
    memset(pStr2, 0, sizeof(pStr2));
    memset(pStr3, 0, sizeof(pStr3));

    strcpy(cName, cItemName);
    for (i = 0; i < DEF_MAXITEMNAMES; i++)
        if ((m_pItemNameList[i] != 0) && (strcmp(m_pItemNameList[i]->m_cOriginName, cItemName) == 0))
        {
            strcpy(cName, m_pItemNameList[i]->m_cName);
            break;
        }

    strcpy(pStr1, cName);

    if ((dwAttribute & 0x00F0F000) != 0)
    {
        m_bIsSpecial = true;
        dwType1 = (dwAttribute & 0x00F00000) >> 20;
        dwValue1 = (dwAttribute & 0x000F0000) >> 16;

        dwType2 = (dwAttribute & 0x0000F000) >> 12;
        dwValue2 = (dwAttribute & 0x00000F00) >> 8;

        if (dwType1 != 0)
        {

            memset(cTxt, 0, sizeof(cTxt));
            switch (dwType1)
            {
                case 1: strcpy(cTxt, GET_ITEM_NAME3); break;
                case 2: strcpy(cTxt, GET_ITEM_NAME4); break;
                case 3: strcpy(cTxt, GET_ITEM_NAME5); break;
                case 4: break;
                case 5: strcpy(cTxt, GET_ITEM_NAME6); break;
                case 6: strcpy(cTxt, GET_ITEM_NAME7); break;
                case 7: strcpy(cTxt, GET_ITEM_NAME8); break;
                case 8: strcpy(cTxt, GET_ITEM_NAME9); break;
                case 9: strcpy(cTxt, GET_ITEM_NAME10); break;
                case 10: strcpy(cTxt, GET_ITEM_NAME11); break;
                case 11: strcpy(cTxt, GET_ITEM_NAME12); break;
                case 12: strcpy(cTxt, GET_ITEM_NAME13); break;
            }
            strcat(cTxt, pStr1);
            memset(pStr1, 0, sizeof(pStr1));
            strcpy(pStr1, cTxt);

            memset(cTxt, 0, sizeof(cTxt));
            switch (dwType1)
            {
                case 1: format_to_local(cTxt, GET_ITEM_NAME14, dwValue1); break;
                case 2: format_to_local(cTxt, GET_ITEM_NAME15, dwValue1 * 5); break;
                case 3: break;
                case 4: break;
                case 5: strcpy(cTxt, GET_ITEM_NAME16); break;
                case 6: format_to_local(cTxt, GET_ITEM_NAME17, dwValue1 * 4); break;
                case 7: strcpy(cTxt, GET_ITEM_NAME18); break;
                case 8: format_to_local(cTxt, GET_ITEM_NAME19, dwValue1 * 7); break;
                case 9: strcpy(cTxt, GET_ITEM_NAME20); break;
                case 10: format_to_local(cTxt, GET_ITEM_NAME21, dwValue1 * 3); break;
                case 11: format_to_local(cTxt, GET_ITEM_NAME22, dwValue1); break;
                case 12: format_to_local(cTxt, GET_ITEM_NAME23, dwValue1); break;
            }
            strcat(pStr2, cTxt);

            if (dwType2 != 0)
            {
                memset(cTxt, 0, sizeof(cTxt));
                switch (dwType2)
                {
                    case 1:  format_to_local(cTxt, GET_ITEM_NAME24, dwValue2 * 7);  break;
                    case 2:  format_to_local(cTxt, GET_ITEM_NAME25, dwValue2 * 7);  break;
                    case 3:  format_to_local(cTxt, GET_ITEM_NAME26, dwValue2 * 7);  break;
                    case 4:  format_to_local(cTxt, GET_ITEM_NAME27, dwValue2 * 7);  break;
                    case 5:  format_to_local(cTxt, GET_ITEM_NAME28, dwValue2 * 7);  break;
                    case 6:  format_to_local(cTxt, GET_ITEM_NAME29, dwValue2 * 7);  break;
                    case 7:  format_to_local(cTxt, GET_ITEM_NAME30, dwValue2 * 7);  break;
                    case 8:  format_to_local(cTxt, GET_ITEM_NAME31, dwValue2 * 3);  break;
                    case 9:  format_to_local(cTxt, GET_ITEM_NAME32, dwValue2 * 3);  break;
                    case 10: format_to_local(cTxt, GET_ITEM_NAME33, dwValue2);    break;
                    case 11: format_to_local(cTxt, GET_ITEM_NAME34, dwValue2 * 10); break;
                    case 12: format_to_local(cTxt, GET_ITEM_NAME35, dwValue2 * 10); break;//"Gold +{}%% "
                }
                strcpy(pStr3, cTxt);
            }
        }
    }

    dwValue3 = (dwAttribute & 0xF0000000) >> 28;
    if (dwValue3 > 0)
    {
        if (pStr1[strlen(pStr1) - 2] == '+')
        {
            dwValue3 = atoi((char *)(pStr1 + strlen(pStr1) - 1)) + dwValue3;
            memset(cTxt, 0, sizeof(cTxt));
            memcpy(cTxt, pStr1, strlen(pStr1) - 2);
            memset(cTxt2, 0, sizeof(cTxt2));
            format_to_local(cTxt2, "{}+{}", cTxt, dwValue3);
            memset(pStr1, 0, sizeof(pStr1));
            strcpy(pStr1, cTxt2);
        }
        else
        {
            memset(cTxt, 0, sizeof(cTxt));
            format_to_local(cTxt, "+{}", dwValue3);
            strcat(pStr1, cTxt);
        }
    }
}

void CGame::PointCommandHandler(int indexX, int indexY, char cItemID)
{
    char cTemp[31];
    if ((m_iPointCommandType >= 100) && (m_iPointCommandType < 200))
    {
#ifdef DEF_HACKCLIENT
        if (m_stConfigList.bFastCast == true)
        {
            uint32_t dwTime = unixtime();
            if ((dwTime - m_stConfigList.dwFastCast) > 50)
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, indexX, indexY, m_iPointCommandType, 0);
        }
        else
        {
#endif
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, indexX, indexY, m_iPointCommandType, 0);
#ifdef DEF_HACKCLIENT
        }
#endif
    }
    else if ((m_iPointCommandType >= 0) && (m_iPointCommandType < 50))
    {
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, m_iPointCommandType, indexX, indexY, cTemp, cItemID); // v1.4 

        if (m_pItemList[m_iPointCommandType]->m_cItemType == DEF_ITEMTYPE_USE_SKILL)
            m_bSkillUsingStatus = true;

    }
    else if (m_iPointCommandType == 200)
    {
        if ((strlen(m_cMCName) == 0) || (strcmp(m_cMCName, m_cPlayerName) == 0) || (m_cMCName[0] == '_'))
        {
            m_stDialogBoxInfo[32].cMode = 0;
            PlaySound('E', 14, 5);

            AddEventList(POINT_COMMAND_HANDLER1, 10);
        }
        else
        {
            m_stDialogBoxInfo[32].cMode = 3;
            PlaySound('E', 14, 5);
            memset(m_stDialogBoxInfo[32].cStr, 0, sizeof(m_stDialogBoxInfo[32].cStr));
            strcpy(m_stDialogBoxInfo[32].cStr, m_cMCName);
            bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQUEST_JOINPARTY, 0, 1, 0, 0, m_cMCName);
            return;
        }
    }
}


void CGame::StartBGM()
{
    if (m_bSoundFlag == false)
    {
        m_pBGM.stop();
        return;
    }

    std::string music_name;


    if (bgm_override)
    {
        music_name = bgm_override_name;
    }
    else
    {
        if (m_cWhetherEffectType >= 4 && m_cWhetherEffectType <= 6) music_name = "carol";
        else
        {
            if (memcmp(m_cCurLocation, "aresden", 7) == 0) music_name = "aresden";
            else if (memcmp(m_cCurLocation, "elvine", 6) == 0) music_name = "elvine";
            else if (memcmp(m_cCurLocation, "dglv", 4) == 0) music_name = "dungeon";
            else if (memcmp(m_cCurLocation, "middled1", 8) == 0) music_name = "dungeon";
            else if (memcmp(m_cCurLocation, "middleland", 10) == 0) music_name = "middleland";
            else if (memcmp(m_cCurLocation, "druncncity", 10) == 0) music_name = "druncncity";
            else if (memcmp(m_cCurLocation, "inferniaA", 9) == 0) music_name = "middleland";
            else if (memcmp(m_cCurLocation, "inferniaB", 9) == 0) music_name = "middleland";
            else if (memcmp(m_cCurLocation, "maze", 4) == 0) music_name = "dungeon";
            else if (memcmp(m_cCurLocation, "abaddon", 7) == 0) music_name = "abaddon";
            else music_name = "maintm";
        }
    }

    // track is already current buffer. is it playing?
    if (bgm_current == music_name)
    {
        if (m_pBGM.getStatus() != sf::Sound::Playing) m_pBGM.play();
        return;
    }

    m_pBGM.stop();
    bgmbuffer.loadFromFile(std::format("data\\music\\{}.ogg", music_name));
    m_pBGM.setBuffer(bgmbuffer);
    m_pBGM.setLoop(true);
    m_pBGM.setVolume(m_cMusicVolume);
    m_pBGM.play();
    bgm_current = music_name;
}

void CGame::ShowEventList(uint32_t dwTime)
{
    int i;

    int height = get_virtual_height();

    for (i = 0; i < 6; i++)
        if ((dwTime - m_stEventHistory[i].dwTime) < 5000)
        {
            switch (m_stEventHistory[i].cColor)
            {
                case 0:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(225, 225, 225), false, 1);
                    break;
                case 1:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(130, 255, 130), false, 1);
                    break;
                case 2:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(255, 130, 130), false, 1);
                    break;
                case 3:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(130, 130, 255), false, 1);
                    break;
                case 4:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(230, 230, 130), false, 1);
                    break;
                case 10:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(180, 255, 180), false, 1);
                    break;
                case 20:
                    put_string(10, 10 + i * 15, m_stEventHistory[i].cTxt, Color(150, 150, 170), false, 1);
                    break;
            }
        }

    for (i = 0; i < 6; i++)
        if ((dwTime - m_stEventHistory2[i].dwTime) < 5000)
        {

            switch (m_stEventHistory2[i].cColor)
            {
                case 0:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(225, 225, 225), false, 1);
                    break;
                case 1:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(130, 255, 130), false, 1);
                    break;
                case 2:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(255, 130, 130), false, 1);
                    break;
                case 3:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(130, 130, 255), false, 1);
                    break;
                case 4:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(230, 230, 130), false, 1);
                    break;
                case 10:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(180, 255, 180), false, 1);
                    break;
                case 20:
                    put_string(10, height - 158 + i * 15, m_stEventHistory2[i].cTxt, Color(150, 150, 170), false, 1);
                    break;
            }
        }

    if (m_bSkillUsingStatus == true)
    {
        put_string(height / 2 - 29, height / 2 - 52, SHOW_EVENT_LIST1, Color(235, 235, 235), false, 1);

    }
}

void CGame::RequestTeleportAndWaitData()
{
    if (strcmp(m_cMapName, "aresden") == 0)
    {
        if (((m_sPlayerX == 188) && (m_sPlayerY == 105)) ||
            ((m_sPlayerX == 187) && (m_sPlayerY == 105)) ||
            ((m_sPlayerX == 187) && (m_sPlayerY == 106)) ||
            ((m_sPlayerX == 186) && (m_sPlayerY == 106)) ||
            ((m_sPlayerX == 186) && (m_sPlayerY == 107)))
        {
            if ((m_iLevel < 30) || (m_iLevel > 80))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA1, 10);
                return;
            }
        }
    }
    if (strcmp(m_cMapName, "elvine") == 0)
    {
        if (((m_sPlayerX == 218) && (m_sPlayerY == 109)) ||
            ((m_sPlayerX == 217) && (m_sPlayerY == 109)) ||
            ((m_sPlayerX == 217) && (m_sPlayerY == 110)) ||
            ((m_sPlayerX == 216) && (m_sPlayerY == 110)) ||
            ((m_sPlayerX == 216) && (m_sPlayerY == 111)))
        {
            if ((m_iLevel < 30) || (m_iLevel > 80))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA1, 10);
                return;
            }
        }
    }

    for (int i = 0; i < 50; i++)
    {
        if (m_pItemList[i] != 0)
            bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, m_pItemList[i]->m_sX, m_pItemList[i]->m_sY, 0, 0);
    }

    bSendCommand(MSGID_REQUEST_TELEPORT, 0, 0, 0, 0, 0, 0);
    ChangeGameMode(DEF_GAMEMODE_ONWAITINGINITDATA);
}

void CGame::GrandMagicResult(char * pMapName, int iV1, int iV2, int iV3, int iV4, int iHP1, int iHP2, int iHP3, int iHP4)
{
    int i, iTxtIdx = 0;
    char cTemp[120];

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;
    }
    for (i = 0; i < 92; i++)
        if (m_pGameMsgList[i] == 0) return;

    if (strcmp(pMapName, "aresden") == 0)
    {

        m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[2]->message, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[3]->message, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[4]->message, iV1);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[5]->message, iV2);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[6]->message, iV3);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[58]->message, iV4);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {} {} {} {}", NOTIFY_MSG_STRUCTURE_HP, iHP1, iHP2, iHP3, iHP4);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        if (iV2 == 0)
        {
            if ((m_bCitizen == true) && (m_bAresden == false))
            {
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[59]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[60]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[61]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[62]->message, 0);
                for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
            else if ((m_bCitizen == true) && (m_bAresden == true))
            {
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[69]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[70]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[71]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[72]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[73]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[74]->message, 0);
                for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
            else PlaySound('E', 25, 0, 0);
        }
        else
        {
            if (iV1 != 0)
            {
                if ((m_bCitizen == true) && (m_bAresden == false))
                {
                    PlaySound('E', 23, 0, 0);
                    PlaySound('C', 21, 0, 0);
                    PlaySound('C', 22, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[63]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[64]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[65]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else if ((m_bCitizen == true) && (m_bAresden == true))
                {
                    PlaySound('E', 24, 0, 0);
                    PlaySound('C', 12, 0, 0);
                    PlaySound('C', 13, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[75]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[76]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[77]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[78]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[79]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[80]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[81]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[82]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else PlaySound('E', 25, 0, 0);
            }
            else
            {
                if ((m_bCitizen == true) && (m_bAresden == false))
                {
                    PlaySound('E', 23, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[66]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[67]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[68]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else if ((m_bCitizen == true) && (m_bAresden == true))
                {
                    PlaySound('E', 24, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[83]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[84]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[85]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[86]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[87]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[88]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[89]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[90]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else PlaySound('E', 25, 0, 0);
            }
        }
    }
    else if (strcmp(pMapName, "elvine") == 0)
    {

        m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[7]->message, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[8]->message, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[4]->message, iV1);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[5]->message, iV2);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[6]->message, iV3);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {}", m_pGameMsgList[58]->message, iV4);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        memset(cTemp, 0, sizeof(cTemp));
        format_to_local(cTemp, "{} {} {} {} {}", NOTIFY_MSG_STRUCTURE_HP, iHP1, iHP2, iHP3, iHP4);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, cTemp, 0);
        m_pMsgTextList[iTxtIdx++] = new CMsg(0, " ", 0);

        if (iV2 == 0)
        {
            if ((m_bCitizen == true) && (m_bAresden == true))
            {
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[59]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[60]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[61]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[62]->message, 0);
                for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
            else if ((m_bCitizen == true) && (m_bAresden == false))
            {
                PlaySound('E', 25, 0, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[69]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[70]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[71]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[72]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[73]->message, 0);
                m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[74]->message, 0);
                for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
            else PlaySound('E', 25, 0, 0);
        }
        else
        {
            if (iV1 != 0)
            {
                if ((m_bCitizen == true) && (m_bAresden == true))
                {
                    PlaySound('E', 23, 0, 0);
                    PlaySound('C', 21, 0, 0);
                    PlaySound('C', 22, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[63]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[64]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[65]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else if ((m_bCitizen == true) && (m_bAresden == false))
                {
                    PlaySound('E', 24, 0, 0);
                    PlaySound('C', 12, 0, 0);
                    PlaySound('C', 13, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[75]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[76]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[77]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[78]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[79]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[80]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[81]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[82]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else PlaySound('E', 25, 0, 0);
            }
            else
            {
                if ((m_bCitizen == true) && (m_bAresden == true))
                {
                    PlaySound('E', 23, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[66]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[67]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[68]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else if ((m_bCitizen == true) && (m_bAresden == false))
                {
                    PlaySound('E', 24, 0, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[83]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[84]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[85]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[86]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[87]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[88]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[89]->message, 0);
                    m_pMsgTextList[iTxtIdx++] = new CMsg(0, m_pGameMsgList[90]->message, 0);
                    for (i = iTxtIdx; i < 18; i++) m_pMsgTextList[i] = new CMsg(0, " ", 0);
                }
                else PlaySound('E', 25, 0, 0);
            }
        }
    }
    EnableDialogBox(18, 0, 0, 0);
}

// num : 0 - F1, 1 - F2, 2 - F3, 3 - F5, 4 - F6
void CGame::UseShortCut(int num)
{
    int index;
    if (num < 3) index = num + 1;
    else index = num + 2;
    if (m_cGameMode != DEF_GAMEMODE_ONMAINGAME) return;
    if (m_bCtrlPressed == true)
    {
        if (m_sRecentShortCut == -1)
        {
            AddEventList(MSG_SHORTCUT1, 10);
            format_to_local(G_cTxt, MSG_SHORTCUT2, index);
            AddEventList(G_cTxt, 10);
            format_to_local(G_cTxt, MSG_SHORTCUT3, index);
            AddEventList(G_cTxt, 10);
        }
        else
        {
            m_sShortCut[num] = m_sRecentShortCut;
            if (m_sShortCut[num] < 100)
            {
                if (m_pItemList[m_sShortCut[num]] == 0)
                {
                    m_sShortCut[num] = -1;
                    m_sRecentShortCut = -1;
                    return;
                }
                char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};

                GetItemName(m_pItemList[m_sShortCut[num]], cStr1, cStr2, cStr3, 64);
                format_to_local(G_cTxt, MSG_SHORTCUT4, cStr1, cStr2, cStr3, index);
                AddEventList(G_cTxt, 10);
            }
            else if (m_sShortCut[num] >= 100)
            {
                if (m_pMagicCfgList[m_sShortCut[num] - 100] == 0)
                {
                    m_sShortCut[num] = -1;
                    m_sRecentShortCut = -1;
                    return;
                }
                format_to_local(G_cTxt, MSG_SHORTCUT5, m_pMagicCfgList[m_sShortCut[num] - 100]->m_cName, index);
                AddEventList(G_cTxt, 10);
            }
        }
    }
    else
    {
        if (m_sShortCut[num] == -1)
        {
            AddEventList(MSG_SHORTCUT1, 10);
            format_to_local(G_cTxt, MSG_SHORTCUT2, index);
            AddEventList(G_cTxt, 10);
            format_to_local(G_cTxt, MSG_SHORTCUT3, index);
            AddEventList(G_cTxt, 10);
        }
        else if (m_sShortCut[num] < 100)
        {
            ItemEquipHandler((char)m_sShortCut[num]);
        }
        else if (m_sShortCut[num] >= 100) UseMagic(m_sShortCut[num] - 100);
    }
}

int CGame::iGetManaCost(int iMagicNo)
{
    int i, iManaSave, iManaCost;
    iManaSave = 0;

    if (iMagicNo < 0 || iMagicNo >= 100) return 1;

    for (i = 0; i < DEF_MAXITEMS; i++)
    {
        if (m_pItemList[i] == 0) continue;
        if (m_bIsItemEquipped[i] == true)
        {
#ifdef DEF_ENGLISHITEM
            if (strcmp(m_pItemList[i]->m_cName, "MagicWand(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "MagicWand(MS20)") == 0) iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "MagicWand(MS30-LLF)") == 0) iManaSave += 30;
            else if (strcmp(m_pItemList[i]->m_cName, "WizMagicWand(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "WizMagicWand(MS20)") == 0) iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "MagicNecklace(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "DarkMageMagicStaff") == 0) iManaSave += 25;
            else if (strcmp(m_pItemList[i]->m_cName, "DarkMageMagicStaffW") == 0) iManaSave += 25;
            else if (strcmp(m_pItemList[i]->m_cName, "DarkMageMagicWand") == 0) iManaSave += 28;
            else if (strcmp(m_pItemList[i]->m_cName, "NecklaceOfLiche") == 0) iManaSave += 15;

            //Change
            else if (strcmp(m_pItemList[i]->m_cName, "DarkMageTempleWand") == 0)	iManaSave += 28;
            else if (strcmp(m_pItemList[i]->m_cName, "BerserkWand(MS.20)") == 0)	iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "BerserkWand(MS.10)") == 0)	iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "KlonessWand(MS.20)") == 0)	iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "KlonessWand(MS.10)") == 0)	iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "ResurWand(MS.20)") == 0)	iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "ResurWand(MS.10)") == 0)	iManaSave += 10;
#else
            if (strcmp(m_pItemList[i]->m_cName, "¸ÅÁ÷½ºÅÇ(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "¸ÅÁ÷½ºÅÇ(MS20)") == 0) iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "¸ÅÁ÷¿øµå(MS30-LLF)") == 0) iManaSave += 30;
            else if (strcmp(m_pItemList[i]->m_cName, "À§Àúµå¸ÅÁ÷½ºÅÇ(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "À§Àúµå¸ÅÁ÷½ºÅÇ(MS20)") == 0) iManaSave += 20;
            else if (strcmp(m_pItemList[i]->m_cName, "¸¶¹ý¸ñ°ÉÀÌ(MS10)") == 0) iManaSave += 10;
            else if (strcmp(m_pItemList[i]->m_cName, "Èæ¸¶¹ý»çÀÇ¸ÅÁ÷½ºÅÇ") == 0) iManaSave += 25;
            else if (strcmp(m_pItemList[i]->m_cName, "Èæ¿©¸¶¹ý»ç¸ÅÁ÷½ºÅÇ") == 0) iManaSave += 25;
            else if (strcmp(m_pItemList[i]->m_cName, "Èæ¸¶¹ý»çÀÇ¸ÅÁ÷¿øµå") == 0) iManaSave += 28;
            else if (strcmp(m_pItemList[i]->m_cName, "³ØÅ¬¸®½º¿Àºê¸®Ä¡") == 0) iManaSave += 15;
#endif
        }
    }
    if (m_pMagicCfgList[iMagicNo] == 0)//Change Invalid Spell Mana set to 1
        iManaCost = 1;
    else
        iManaCost = m_pMagicCfgList[iMagicNo]->m_sValue1;
    if (m_bIsSafeAttackMode) iManaCost += (iManaCost / 2) - (iManaCost / 10);
    if (iManaSave > 0)
    {
        double dV1 = (double)iManaSave;
        double dV2 = (double)(dV1 / 100.0f);
        double dV3 = (double)iManaCost;
        dV1 = dV2 * dV3;
        dV2 = dV3 - dV1;
        iManaCost = (int)dV2;
    }
    if (iManaCost < 1) iManaCost = 1; // v2.173
    return iManaCost;
}

void CGame::UseMagic(int iMagicNo)
{
    //if( iMagicNo < 0 || iMagicNo >= 100 ) return;//Change was uncommented - allows skill casts
    //if ((m_cMagicMastery[iMagicNo] == 0) || (m_pMagicCfgList[iMagicNo] == 0)) return;//Change Uncommented
    if (m_iHP <= 0) return;
#ifdef DEF_HACKCLIENT
    if (m_stConfigList.bFastCast == false)//Change << \/
#endif
        if (m_bIsGetPointingMode == true) return;//Change was uncommented
#ifndef DEF_HACKCLIENT
    if (iGetManaCost(iMagicNo) > m_iMP) return;//Change
#endif

#ifndef DEF_HACKCLIENT
    if (_bIsItemOnHand() == true)
    {
        AddEventList(DLGBOX_CLICK_MAGIC1, 10);
        return;
    }//Change
#endif
    if (m_bSkillUsingStatus == true)
    {
        AddEventList(DLGBOX_CLICK_MAGIC2, 10);
        return;
    }
    if ((m_sPlayerAppr2 & 0xF000) == 0) bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_TOGGLECOMBATMODE, 0, 0, 0, 0, 0);

    m_cCommand = DEF_OBJECTMAGIC;

    m_iCastingMagicType = iMagicNo;
    m_sMagicShortCut = iMagicNo;
    m_sRecentShortCut = iMagicNo + 100;
    m_iPointCommandType = iMagicNo + 100;
#ifdef DEF_HACKCLIENT
    m_bIsGetPointingMode = true;
#endif

    DisableDialogBox(3);
}
void CGame::ReleaseEquipHandler(char cEquipPos)
{
    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    if (m_sItemEquipmentStatus[cEquipPos] < 0) return;
    GetItemName(m_pItemList[m_sItemEquipmentStatus[cEquipPos]], cStr1, cStr2, cStr3, 64);
    format_to_local(G_cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
    AddEventList(G_cTxt, 10);
    m_bIsItemEquipped[m_sItemEquipmentStatus[cEquipPos]] = false;
    m_sItemEquipmentStatus[cEquipPos] = -1;
}

void CGame::ItemEquipHandler(char cItemID)
{
    if (bCheckItemOperationEnabled(cItemID) == false) return;
    if (m_bIsItemEquipped[cItemID] == true) return;
    if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE)
    {
        AddEventList(BITEMDROP_CHARACTER3, 10);
        return;
    }
    if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
    {
        AddEventList(BITEMDROP_CHARACTER1, 10);
        return;
    }
    if (m_pItemList[cItemID]->m_wWeight / 100 > m_iStr)
    {
        AddEventList(BITEMDROP_CHARACTER2, 10);
        return;
    }
    if (((m_pItemList[cItemID]->m_dwAttribute & 0x00000001) == 0) && (m_pItemList[cItemID]->m_sLevelLimit > m_iLevel))
    {
        AddEventList(BITEMDROP_CHARACTER4, 10);
        return;
    }
    if (m_bSkillUsingStatus == true)
    {
        AddEventList(BITEMDROP_CHARACTER5, 10);
        return;
    }
    if (m_pItemList[cItemID]->m_cGenderLimit != 0)
    {
        switch (m_sPlayerType)
        {
            case 1:
            case 2:
            case 3:
                if (m_pItemList[cItemID]->m_cGenderLimit != 1)
                {
                    AddEventList(BITEMDROP_CHARACTER6, 10);
                    return;
                }
                break;
            case 4:
            case 5:
            case 6:
                if (m_pItemList[cItemID]->m_cGenderLimit != 2)
                {
                    AddEventList(BITEMDROP_CHARACTER7, 10);
                    return;
                }
                break;
        }
    }
    bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_EQUIPITEM, 0, cItemID, 0, 0, 0);

    m_sRecentShortCut = cItemID;
    ReleaseEquipHandler(m_pItemList[cItemID]->m_cEquipPos);
    switch (m_pItemList[cItemID]->m_cEquipPos)
    {
        case DEF_EQUIPPOS_HEAD:
        case DEF_EQUIPPOS_BODY:
        case DEF_EQUIPPOS_ARMS:
        case DEF_EQUIPPOS_PANTS:
        case DEF_EQUIPPOS_BOOTS:
        case DEF_EQUIPPOS_BACK:
            ReleaseEquipHandler(DEF_EQUIPPOS_FULLBODY);
            break;
        case DEF_EQUIPPOS_FULLBODY:
            ReleaseEquipHandler(DEF_EQUIPPOS_HEAD);
            ReleaseEquipHandler(DEF_EQUIPPOS_BODY);
            ReleaseEquipHandler(DEF_EQUIPPOS_ARMS);
            ReleaseEquipHandler(DEF_EQUIPPOS_PANTS);
            ReleaseEquipHandler(DEF_EQUIPPOS_BOOTS);
            ReleaseEquipHandler(DEF_EQUIPPOS_BACK);
            break;
        case DEF_EQUIPPOS_LHAND:
        case DEF_EQUIPPOS_RHAND:
            ReleaseEquipHandler(DEF_EQUIPPOS_TWOHAND);
            break;
        case DEF_EQUIPPOS_TWOHAND:
            ReleaseEquipHandler(DEF_EQUIPPOS_RHAND);
            ReleaseEquipHandler(DEF_EQUIPPOS_LHAND);
            break;
    }
    m_sItemEquipmentStatus[m_pItemList[cItemID]->m_cEquipPos] = cItemID;
    m_bIsItemEquipped[cItemID] = true;

    char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
    GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);

    format_to_local(G_cTxt, BITEMDROP_CHARACTER9, cStr1);
    AddEventList(G_cTxt, 10);
    PlaySound('E', 28, 0);
}

int CGame::bHasHeroSet(short Appr3, short Appr4, char OwnerType)
{
    char cArmor, cLeg, cBerk, cHat;
    char cTxt[100];

    memset(cTxt, 0, 100);

    cArmor = (Appr3 & 0xF000) >> 12;
    cLeg = (Appr3 & 0x0F00) >> 8;
    cHat = (Appr3 & 0x00F0) >> 4;
    cBerk = Appr3 & 0x000F;

    //	format_to_local(cTxt, "Armor: {} Leg: {} Hat: {} Berk: {}", cArmor, cLeg, cHat, cBerk);

    //	AddEventList(cTxt, 10);
    switch (OwnerType)
    {
        case 1:
        case 2:
        case 3:
            if ((cArmor == 8) && (cLeg == 5) && (cHat == 9) && (cBerk == 3)) return (1); // 1 = Warrior set
            if ((cArmor == 9) && (cLeg == 6) && (cHat == 10) && (cBerk == 4)) return (1); // 2 = mage set
            if ((cArmor == 10) && (cLeg == 5) && (cHat == 11) && (cBerk == 3)) return (2);
            if ((cArmor == 11) && (cLeg == 6) && (cHat == 12) && (cBerk == 4)) return (2);
            break;
        case 4:
        case 5:
        case 6:
            if ((cArmor == 9) && (cLeg == 6) && (cHat == 9) && (cBerk == 4)) return (1);
            if ((cArmor == 10) && (cLeg == 7) && (cHat == 10) && (cBerk == 5)) return (1);
            if ((cArmor == 11) && (cLeg == 6) && (cHat == 11) && (cBerk == 4)) return (2);
            if ((cArmor == 12) && (cLeg == 7) && (cHat == 12) && (cBerk == 5)) return (2);
            break;
    }
    return 0;
}

void CGame::Abaddon_corpse(int sX, int sY)
{
    int ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX + 30, 0, sX + 30, sY - 10, ir, ir, 1);
    _DrawThunderEffect(sX + 30, 0, sX + 30, sY - 10, ir + 2, ir, 2);
    _DrawThunderEffect(sX + 30, 0, sX + 30, sY - 10, ir - 2, ir, 2);
    ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX - 20, 0, sX - 20, sY - 35, ir, ir, 1);
    _DrawThunderEffect(sX - 20, 0, sX - 20, sY - 35, ir + 2, ir, 2);
    _DrawThunderEffect(sX - 20, 0, sX - 20, sY - 35, ir - 2, ir, 2);
    ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX - 10, 0, sX - 10, sY + 30, ir, ir, 1);
    _DrawThunderEffect(sX - 10, 0, sX - 10, sY + 30, ir + 2, ir + 2, 2);
    _DrawThunderEffect(sX - 10, 0, sX - 10, sY + 30, ir - 2, ir + 2, 2);
    ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX + 50, 0, sX + 50, sY + 35, ir, ir, 1);
    _DrawThunderEffect(sX + 50, 0, sX + 50, sY + 35, ir + 2, ir + 2, 2);
    _DrawThunderEffect(sX + 50, 0, sX + 50, sY + 35, ir - 2, ir + 2, 2);
    ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX + 65, 0, sX + 65, sY - 5, ir, ir, 1);
    _DrawThunderEffect(sX + 65, 0, sX + 65, sY - 5, ir + 2, ir + 2, 2);
    _DrawThunderEffect(sX + 65, 0, sX + 65, sY - 5, ir - 2, ir + 2, 2);
    ir = (rand() % 20) - 10;
    _DrawThunderEffect(sX + 45, 0, sX + 45, sY - 50, ir, ir, 1);
    _DrawThunderEffect(sX + 45, 0, sX + 45, sY - 50, ir + 2, ir + 2, 2);
    _DrawThunderEffect(sX + 45, 0, sX + 45, sY - 50, ir - 2, ir + 2, 2);

    for (int x = sX - 50; x <= sX + 100; x += rand() % 35)
        for (int y = sY - 30; y <= sY + 50; y += rand() % 45)
        {
            ir = (rand() % 20) - 10;
            _DrawThunderEffect(x, 0, x, y, ir, ir, 2);
        }
}

void CGame::DKGlare(int iWeaponColor, int iWeaponIndex, int * iWeaponGlare)
{
    if (iWeaponColor != 9) return;
    if (((iWeaponIndex >= DEF_SPRID_WEAPON_M + 64 * 14) && (iWeaponIndex < DEF_SPRID_WEAPON_M + 64 * 14 + 56)) //msw3
        || ((iWeaponIndex >= DEF_SPRID_WEAPON_W + 64 * 14) && (iWeaponIndex < DEF_SPRID_WEAPON_W + 64 * 14 + 56))) //wsw3
        *iWeaponGlare = 3;
    else if (((iWeaponIndex >= DEF_SPRID_WEAPON_M + 64 * 37) && (iWeaponIndex < DEF_SPRID_WEAPON_M + 64 * 37 + 56)) //MStaff3
        || ((iWeaponIndex >= DEF_SPRID_WEAPON_W + 64 * 37) && (iWeaponIndex < DEF_SPRID_WEAPON_W + 64 * 37 + 56)))//WStaff3
        *iWeaponGlare = 2;
}

void CGame::ShowHeldenianVictory(short sSide)
{
    int i, iPlayerSide;

    DisableDialogBox(18);
    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;
    }
    if (m_bCitizen == false) iPlayerSide = 0;
    else if (m_bAresden == true) iPlayerSide = 1;
    else if (m_bAresden == false) iPlayerSide = 2;
    switch (sSide)
    {
        case 0:
            PlaySound('E', 25, 0, 0);
            m_pMsgTextList[0] = new CMsg(0, "Heldenian holy war has been closed!", 0);
            m_pMsgTextList[1] = new CMsg(0, " ", 0);
            m_pMsgTextList[2] = new CMsg(0, "Heldenian Holy war ended", 0);
            m_pMsgTextList[3] = new CMsg(0, "in a tie.", 0);
            break;
        case 1:
            PlaySound('E', 25, 0, 0);
            m_pMsgTextList[0] = new CMsg(0, "Heldenian holy war has been closed!", 0);
            m_pMsgTextList[1] = new CMsg(0, " ", 0);
            m_pMsgTextList[2] = new CMsg(0, "Heldenian Holy war ended", 0);
            m_pMsgTextList[3] = new CMsg(0, "in favor of Aresden.", 0);
            break;
        case 2:
            PlaySound('E', 25, 0, 0);
            m_pMsgTextList[0] = new CMsg(0, "Heldenian holy war has been closed!", 0);
            m_pMsgTextList[1] = new CMsg(0, " ", 0);
            m_pMsgTextList[2] = new CMsg(0, "Heldenian Holy war ended", 0);
            m_pMsgTextList[3] = new CMsg(0, "in favor of Elvine.", 0);
            break;
    }
    m_pMsgTextList[4] = new CMsg(0, " ", 0);

    if (((iPlayerSide != 1) && (iPlayerSide != 2))   // Player not a normal citizen
        || (sSide == 0))								// or no winner
    {
        PlaySound('E', 25, 0, 0);
        m_pMsgTextList[5] = new CMsg(0, " ", 0);
        m_pMsgTextList[6] = new CMsg(0, " ", 0);
        m_pMsgTextList[7] = new CMsg(0, " ", 0);
        m_pMsgTextList[8] = new CMsg(0, " ", 0);
    }
    else
    {
        if (sSide == iPlayerSide)
        {
            PlaySound('E', 23, 0, 0);
            PlaySound('C', 21, 0, 0);
            PlaySound('C', 22, 0, 0);
            m_pMsgTextList[5] = new CMsg(0, "Congratulation.", 0);
            m_pMsgTextList[6] = new CMsg(0, "As citizen of victory,", 0);
            m_pMsgTextList[7] = new CMsg(0, "You will receive a reward.", 0);
            m_pMsgTextList[8] = new CMsg(0, "      ", 0);
        }
        else
        {
            PlaySound('E', 24, 0, 0);
            PlaySound('C', 12, 0, 0);
            PlaySound('C', 13, 0, 0);
            m_pMsgTextList[5] = new CMsg(0, "To our regret", 0);
            m_pMsgTextList[6] = new CMsg(0, "As citizen of defeat,", 0);
            m_pMsgTextList[7] = new CMsg(0, "You cannot receive any reward.", 0);
            m_pMsgTextList[8] = new CMsg(0, "     ", 0);
        }
    }
    for (i = 9; i < 18; i++)
        m_pMsgTextList[i] = new CMsg(0, " ", 0);
    EnableDialogBox(18, 0, 0, 0);
    DisableDialogBox(36);
    DisableDialogBox(37);
    DisableDialogBox(38);
}




void CGame::CheckActiveAura(short sX, short sY, uint32_t dwTime, short sOwnerType)
{
    if ((_tmp_iStatus & 0x80) != 0)
        m_pEffectSpr[81]->put_trans_sprite70(sX + 115, sY + 85, _tmp_iEffectFrame % 21, dwTime);
    // DefenseShield
    if ((_tmp_iStatus & 0x02000000) != 0)
        //m_pEffectSpr[80]->put_trans_sprite(sX+75, sY+107, _tmp_iEffectFrame%17, dwTime);
        m_pEffectSpr[80]->put_trans_sprite50(sX + 75, sY + 107, _tmp_iEffectFrame % 17, dwTime);

    // Protection From Magic
    if ((_tmp_iStatus & 0x04000000) != 0)
        //m_pEffectSpr[79]->put_trans_sprite(sX+101, sY+135, _tmp_iEffectFrame%15, dwTime);
        m_pEffectSpr[79]->put_trans_sprite70(sX + 101, sY + 135, _tmp_iEffectFrame % 15, dwTime);

    // Protection From Arrow
    if ((_tmp_iStatus & 0x08000000) != 0)
        //m_pEffectSpr[72]->put_trans_sprite(sX, sY+35, _tmp_iEffectFrame%30, dwTime);
        m_pEffectSpr[72]->put_trans_sprite70(sX, sY + 35, _tmp_iEffectFrame % 30, dwTime);

    // Illusion
    if ((_tmp_iStatus & 0x01000000) != 0)
        //m_pEffectSpr[73]->put_trans_sprite(sX+125, sY+95, _tmp_iEffectFrame%24, dwTime);
        m_pEffectSpr[73]->put_trans_sprite70(sX + 125, sY + 130 - _iAttackerHeight[sOwnerType], _tmp_iEffectFrame % 24, dwTime);

    // Illusion movement
    if ((_tmp_iStatus & 0x00200000) != 0)
        //m_pEffectSpr[151]->put_trans_sprite(sX+90, sY+55, _tmp_iEffectFrame%24, dwTime);
        m_pEffectSpr[151]->put_trans_sprite70(sX + 90, sY + 90 - _iAttackerHeight[sOwnerType], _tmp_iEffectFrame % 24, dwTime);

    // Slate red  (HP)
    if ((_tmp_iStatus & 0x00400000) != 0)
        //m_pEffectSpr[149]->put_trans_sprite(sX+90, sY+120, _tmp_iEffectFrame%15, dwTime);
        m_pEffectSpr[149]->put_trans_sprite70(sX + 90, sY + 120, _tmp_iEffectFrame % 15, dwTime);

    // Slate Blue (Mana)
    if ((_tmp_iStatus & 0x00800000) != 0)
        //m_pEffectSpr[150]->put_trans_sprite(sX+1, sY+26, _tmp_iEffectFrame%15, dwTime);
        m_pEffectSpr[150]->put_trans_sprite70(sX + 1, sY + 26, _tmp_iEffectFrame % 15, dwTime);

    // Slate Green (XP)
    if ((_tmp_iStatus & 0x00010000) != 0)
        //m_pEffectSpr[148]->put_trans_sprite(sX, sY+32, _tmp_iEffectFrame%23, dwTime);
        m_pEffectSpr[148]->put_trans_sprite70(sX, sY + 32, _tmp_iEffectFrame % 23, dwTime);

    // Hero Flag (Heldenian)
    if ((_tmp_iStatus & 0x00020000) != 0)
        //m_pEffectSpr[87]->put_trans_sprite(sX+53, sY+54, _tmp_iEffectFrame%29, dwTime);
        m_pEffectSpr[87]->put_trans_sprite70(sX + 53, sY + 54, _tmp_iEffectFrame % 29, dwTime);
}
void CGame::DisplayHPBar(int iID, short sX, short sY, uint32_t dwTime, uint16_t wType)
{
    int iBarWidth;
    if ((m_stNPCList[iID].m_iMaxHP != 0) && (m_stNPCList[iID].m_iHP != 0))
    {
        //////////////////////////////////////////////////////////////////////////
        //NPC/Party check
        //////////////////////////////////////////////////////////////////////////

                /*
                31
                52
                49
                50
                66
                73
                81
                */
        if ((_tmp_sOwnerType == 31) || (_tmp_sOwnerType == 52)
            || (_tmp_sOwnerType == 49) || (_tmp_sOwnerType == 50)
            || (_tmp_sOwnerType == 66) || (_tmp_sOwnerType == 73)
            || (_tmp_sOwnerType == 81))
        {
            iBarWidth = 70 - (m_stNPCList[iID].m_iHP * 70) / m_stNPCList[iID].m_iMaxHP;
            if (iBarWidth > 70) iBarWidth = 70;
            if (iBarWidth < 0) iBarWidth = 0;
            m_pSprite[54000]->put_trans_sprite70(sX - 25, sY - 90, 0, dwTime);
            m_pSprite[54000]->put_sprite_fast_width(sX - 25, sY - 90, 1, 70 - iBarWidth, dwTime);
        }


        for (int i = 0; i < DEF_MAXPARTYMEMBERS; i++)
        {
            //if (memcmp(m_stPartyMemberNameList[i].cName, _tmp_cName, 12) == 0)
            //{
            //	if (memcmp(m_stPartyMemberNameList[i].cName, m_cPlayerName, 12) == 0)
            //	{
            //		iBarWidth = 40 - (m_iHP*40)/(m_iVit*3 + m_iLevel*2 + m_iStr/2);
            //		if (iBarWidth > 40) iBarWidth = 40;
            //		if (iBarWidth < 0) iBarWidth = 0;
            //		m_pSprite[54000]->put_trans_sprite70(sX-5, sY, 0, dwTime);
            //		m_pSprite[54000]->put_sprite_fast_width(sX-5, sY, 1, 40-iBarWidth, dwTime);
            //		return;
            //	}
            //	iBarWidth = 40 - (m_stNPCList[iID].m_iHP*40)/m_stNPCList[iID].m_iMaxHP;
            //	if (iBarWidth > 40) iBarWidth = 40;
            //	if (iBarWidth < 0) iBarWidth = 0;
            //	m_pSprite[54000]->put_trans_sprite70(sX-5, sY, 0, dwTime);
            //	m_pSprite[54000]->put_sprite_fast_width(sX-5, sY, 1, 40-iBarWidth, dwTime);
            //	return;
            //}
            if (memcmp(m_stPartyMemberNameList[i].cName, _tmp_cName, 12) == 0)
            {
                if (memcmp(m_stPartyMemberNameList[i].cName, m_cPlayerName, 12) == 0)
                {
                    iBarWidth = 40 - (m_iHP * 40) / (m_iVit * 3 + m_iLevel * 2 + m_iStr / 2);
                    if (iBarWidth > 40) iBarWidth = 40;
                    if (iBarWidth < 0) iBarWidth = 0;
                    m_pSprite[54000]->put_trans_sprite70(sX - 5, sY, 0, dwTime);
                    m_pSprite[54000]->put_sprite_fast_width(sX - 5, sY, 1, 40 - iBarWidth, dwTime);
                    return;
                }
                iBarWidth = 40 - (m_stPartyMemberNameList[i].iHP * 40) / m_stPartyMemberNameList[i].iMaxHP;
                if (iBarWidth > 40) iBarWidth = 40;
                if (iBarWidth < 0) iBarWidth = 0;
                m_pSprite[54000]->put_trans_sprite70(sX - 5, sY, 0, dwTime);
                m_pSprite[54000]->put_sprite_fast_width(sX - 5, sY, 1, 40 - iBarWidth, dwTime);
                return;
            }

            //Debug \/		
            //		format_to_local(G_cTxt, "HP: {} MAXHP: {}", m_stNPCList[iID].m_iHP, m_stNPCList[_tmp_wObjectID].m_iMaxHP);
            //		put_string2(sX, sY+28, G_cTxt, 30,255,30);
        }
    }
}
