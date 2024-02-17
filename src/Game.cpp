//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <windows.h>

#if DEF_LANGUAGE == 1
#include "lan_tai.h"
#elif DEF_LANGUAGE == 2
#include "lan_chi.h"
#elif DEF_LANGUAGE == 3
#include "lan_kor.h"
#elif DEF_LANGUAGE == 4
#include "lan_eng.h"
#elif DEF_LANGUAGE == 5
#include "lan_jap.h"
#endif

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
#include <fmt/core.h>
#include <fmt/format.h>

#include "sprite.h"
#include "SpriteID.h"
#include "StrTok.h"
#include "Msg.h"
#include "Effect.h"
#include "MapData.h"
#include "ActionID.h"
#include "NetMessages.h"
#include "MouseInterface.h"
#include "CharInfo.h"
#include "Item.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObjectID.h"
#include "BuildItem.h"
#include "ItemName.h"

#ifdef _DEBUG
int		G_iColor = 0;
int		G_iColorMax = 15;
#endif

extern char G_cSpriteAlphaDegree;

char _cDrawingOrder[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1 };
char _cMantleDrawingOrder[] = { 0, 1, 1, 1, 0, 0, 0, 2, 2 };
char _cMantleDrawingOrderOnRun[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };


short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
int _tmp_sStatus;
char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
uint16_t  _tmp_wObjectID;
char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
uint16_t  wFocusObjectID;
short sFocus_dX, sFocus_dY;
char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
int sFocusStatus;
int   iFocusApprColor;

CGame::CGame()
{
#if defined(WIN32)
    ix::initNetSystem();
#endif

    socket_mode(0);
    oldmode = 0;

    int i;
    srand((unsigned)time(0));

    m_stMCursor.sCursorFrame = 0;

    loggedin = false;

    if (!autoresolution)
    {
        //SetResolution(640, 480);
        //SetResolution(800, 600);
        //SetResolution(1024, 768);
        //SetResolution(1280, 1024);
        //SetResolution(1920, 1200);

        //SetResolution(1280, 720);
        //SetResolution(1366, 768);
        //SetResolution(1600, 900);
        //SetResolution(1920, 1080);

        //SetVirtualResolution(5120, 2160);
        //SetResolution(800, 600);
        set_resolution(1024, 768);
    }
    if (!autovresolution)
    {
        //SetVirtualResolution(800, 600);
        set_virtual_resolution(1024, 768);
    }

#ifdef _DEBUG
    m_bToggleScreen = true;
#else
    m_bToggleScreen = false;
#endif
    m_bShowFPS = false;
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

    for (i = 0; i < 41; i++) m_cDialogBoxOrder[i] = 0;

    for (i = 0; i < DEF_MAXMAGICTYPE; i++) m_pMagicCfgList[i] = 0;

    for (i = 0; i < DEF_MAXSKILLTYPE; i++) m_pSkillCfgList[i] = 0;

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        m_pMsgTextList[i] = 0;
        m_pMsgTextList2[i] = 0;
        m_pAgreeMsgTextList[i] = 0;
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



    m_stDialogBoxInfo[5].sX = 0;
    m_stDialogBoxInfo[5].sY = 0;
    m_stDialogBoxInfo[5].sSizeX = 310;
    m_stDialogBoxInfo[5].sSizeY = 170;


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
    m_stDialogBoxInfo[9].sX = 512;
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
    m_stDialogBoxInfo[19].sSizeX = 258;//270;
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
    m_stDialogBoxInfo[30].sX = 0;
    m_stDialogBoxInfo[30].sY = 427;
    m_stDialogBoxInfo[30].sSizeX = 640;
    m_stDialogBoxInfo[30].sSizeY = 53;//47;

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

    m_bCtrlPressed = false;
    m_bShiftPressed = false;
    m_bEnterPressed = false;
    m_bEscPressed = false;

    m_bSoundFlag = false;
    m_dwDialogCloseTime = 0;

    m_iTimeLeftSecAccount = 0;
    m_iTimeLeftSecIP = 0;



    m_bWhisper = true;
    m_bShout = true;
}


CGame::~CGame()
{


}


bool CGame::bInit()
{
    int iIndex = {};
    CStrTok * pStrTok = {};
    char seps[] = "&= ,\t\n";
    char * token = {};

    m_bCommandAvailable = true;


    m_dwTime = G_dwGlobalTime;

    m_bSoundFlag = true;
    m_bMusicStat = m_bSoundStat = m_bSoundFlag;
    m_bIsHideLocalCursor = false;

    m_cEnterCheck = m_cTabCheck = m_cLeftArrowCheck = 0;

    // todo: multi platform
//     SYSTEMTIME SysTime;
//     GetLocalTime(&SysTime);
//     if ((SysTime.wMonth == 12) || (SysTime.wMonth == 1))
//         m_bIsXmas = true;
//     else
//         m_bIsXmas = false;

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


    m_cDialogBoxOrder[40] = 29;
    m_cDialogBoxOrder[39] = 30;

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


    memset(m_cWorldServerName, 0, sizeof(m_cWorldServerName));

    return true;
}


void CGame::Quit()
{
    int i;

    change_game_mode(DEF_GAMEMODE_NULL);

    for (i = 0; i < DEF_MAXSPRITES; i++)
        if (m_pSprite[i] != nullptr) delete m_pSprite[i];
    for (i = 0; i < DEF_MAXTILES; i++)
        if (m_pTileSpr[i] != nullptr) delete m_pTileSpr[i];
    for (i = 0; i < DEF_MAXEFFECTSPR; i++)
        if (m_pEffectSpr[i] != nullptr) delete m_pEffectSpr[i];

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
        if (m_pAgreeMsgTextList[i] != 0) delete m_pAgreeMsgTextList[i];
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

    change_game_mode(DEF_GAMEMODE_ONQUIT);
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
        if (abs(dX) < 40)
        {
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
        if (abs(dY) < 40)
        {
            if (m_sViewDY > 4)  m_sViewDY = 4;
            else if (m_sViewDY < -4) m_sViewDY = -4;
        }
        m_sViewPointY += m_sViewDY;
        update_background = true;
    }
}


void CGame::RestoreSprites()
{
    //     for (int i = 0; i < DEF_MAXSPRITES; i++)
    //         if (m_pSprite[i] != 0) m_pSprite[i]->iRestore();
}

char CGame::cGetNextMoveDir(short sX, short sY, short dstX, short dstY, bool bMoveCheck)
{
    char  cDir, cTmpDir;
    //int   aX, aY, aX2, aY2, dX, dY;
    int   aX, aY, dX, dY;
    int   i;

    if ((sX == dstX) && (sY == dstY)) return 0;

    dX = sX;
    dY = sY;

    cDir = m_Misc.cGetNextMoveDir(dX, dY, dstX, dstY);

    if (m_cPlayerTurn == 0)
        for (i = cDir; i <= cDir + 2; i++)
        {
            cTmpDir = i;
            if (cTmpDir > 8) cTmpDir -= 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
            {

                m_bIsPrevMoveBlocked = false;
            }
            else if (m_pMapData->is_locateable(dX + aX, dY + aY) == true)
            {
                if (m_pMapData->is_teleport(dX + aX, dY + aY) == true)
                {
                    if (_bCheckMoveable(dX + aX, dY + aY) == true) return cTmpDir;
                    else
                    {
                        SetTopMsg(DEF_MSG_GETNEXTMOVEDIR, 5);
                    }
                }
                else return cTmpDir;
            }
        }

    if (m_cPlayerTurn == 1)
        for (i = cDir; i >= cDir - 2; i--)
        {
            cTmpDir = i;
            if (cTmpDir < 1) cTmpDir += 8;
            aX = _tmp_cTmpDirX[cTmpDir];
            aY = _tmp_cTmpDirY[cTmpDir];
            if (((dX + aX) == m_iPrevMoveX) && ((dY + aY) == m_iPrevMoveY) && (m_bIsPrevMoveBlocked == true) && (bMoveCheck == true))
            {

                m_bIsPrevMoveBlocked = false;
            }
            else if (m_pMapData->is_locateable(dX + aX, dY + aY) == true)
            {
                if (m_pMapData->is_teleport(dX + aX, dY + aY) == true)
                {
                    if (_bCheckMoveable(dX + aX, dY + aY) == true) return cTmpDir;
                    else
                    {
                        SetTopMsg(DEF_MSG_GETNEXTMOVEDIR, 5);
                    }
                }
                else return cTmpDir;
            }
        }
    return 0;
}

bool CGame::_bCheckMoveable(short sx, short sy)
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
    cKey = (char)(rand() % 255) + 1;

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

        case MSGID_REQUEST_INITDATA:
            // to Game Server
            sw.write_uint32(dwMsgID);
            sw.write_uint16(0);
            write(sw);
            break;

        case MSGID_LEVELUPSETTINGS:

            //if ((m_cLU_Str + m_cLU_Vit + m_cLU_Dex + m_cLU_Int + m_cLU_Mag + m_cLU_Char) > 3) return false;

            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            sp = (short *)cp;
            *sp = m_cLU_Str;
            cp += 2;

            sp = (short *)cp;
            *sp = m_cLU_Vit;
            cp += 2;

            sp = (short *)cp;
            *sp = m_cLU_Dex;
            cp += 2;

            sp = (short *)cp;
            *sp = m_cLU_Int;
            cp += 2;

            sp = (short *)cp;
            *sp = m_cLU_Mag;
            cp += 2;

            sp = (short *)cp;
            *sp = m_cLU_Char;
            cp += 2;

            iRet = send_message(cMsg, 18);
            break;

        case MSGID_COMMAND_CHATMSG:

            if (m_bIsTeleportRequested == true) return false;

            // to Game Server
            dwp = (uint32_t *)(cMsg + DEF_INDEX4_MSGID);
            *dwp = dwMsgID;
            wp = (uint16_t *)(cMsg + DEF_INDEX2_MSGTYPE);
            *wp = 0;

            cp = (char *)(cMsg + DEF_INDEX2_MSGTYPE + 2);

            sp = (short *)cp;
            *sp = m_sPlayerX;
            cp += 2;

            sp = (short *)cp;
            *sp = m_sPlayerY;
            cp += 2;

            memcpy(cp, m_cPlayerName, 10);
            cp += 10;

            *cp = (char)iV1;
            cp++;


            if (bCheckLocalChatCommand(pString) == true) return false;
            memcpy((char *)cp, pString, strlen(pString) + 1);

            iRet = send_message(cMsg, 22 + strlen(pString));
            break;

        case MSGID_COMMAND_COMMON:

            if (m_bIsTeleportRequested == true) return false;

            // to Game Server

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

                    iRet = send_message(cMsg, 37);
                    break;

                case DEF_COMMONTYPE_REQ_CREATEPORTION:

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

                    iRet = send_message(cMsg, 18);
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


                        dwp = (uint32_t *)cp;
                        *dwp = dwTime;
                        cp += 4;

                        iRet = send_message(cMsg, 23 + 4);
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

                        iRet = send_message(cMsg, 23 + 34);
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
            {
                sp = (short *)cp;
                *sp = (short)iV4;
                cp += 2;


                dwp = (uint32_t *)cp;
                *dwp = dwTime;
                cp += 4;

                iRet = send_message(cMsg, 19 + 4);
            }
            else
            {

                dwp = (uint32_t *)cp;
                *dwp = dwTime;
                cp += 4;

                iRet = send_message(cMsg, 17 + 4);
            }
            m_cCommandCount++;
            break;

    }

    switch (iRet)
    {
        case DEF_XSOCKEVENT_SOCKETCLOSED:
        case DEF_XSOCKEVENT_SOCKETERROR:
        case DEF_XSOCKEVENT_QUENEFULL:

            change_game_mode(DEF_GAMEMODE_ONCONNECTIONLOST);

            close(ix::WebSocketCloseConstants::kAbnormalCloseCode, ix::WebSocketCloseConstants::kAbnormalCloseMessage);
            break;
    }

    return true;
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

        if ((dwTime - m_dwCheckConnTime) > 5000)
        {
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
            //                     close(ix::WebSocketCloseConstants::kAbnormalCloseCode, ix::WebSocketCloseConstants::kAbnormalCloseMessage);
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

bool CGame::_bCheckDraggingItemRelease(short msX, short msY)
{
    int i;
    char         cDlgID;

    for (i = 0; i < 41; i++)
        if (m_cDialogBoxOrder[40 - i] != 0)
        {
            cDlgID = m_cDialogBoxOrder[40 - i];
            if ((m_stDialogBoxInfo[cDlgID].sX < msX) && ((m_stDialogBoxInfo[cDlgID].sX + m_stDialogBoxInfo[cDlgID].sSizeX) > msX) &&
                (m_stDialogBoxInfo[cDlgID].sY < msY) && ((m_stDialogBoxInfo[cDlgID].sY + m_stDialogBoxInfo[cDlgID].sSizeY) > msY))
            {

                EnableDialogBox(cDlgID, 0, 0, 0);

                switch (cDlgID)
                {
                    case 1:
                        bItemDrop_Character();
                        break;

                    case 2:
                        bItemDrop_Inventory(msX, msY);
                        break;

                    case 14:
                        bItemDrop_Bank(msX, msY);
                        break;

                    case 26:
                        bItemDrop_SkillDialog();
                        break;

                    case 27:
                        bItemDrop_ExchangeDialog(msX, msY);
                        break;

                    case 30:
                        bItemDrop_IconPannel(msX, msY);
                        break;

                    case 31:
                        bItemDrop_SellList(msX, msY);
                        break;

                    case 34:
                        bItemDrop_ItemUpgrade();
                        break;
                }

                return true;
            }
        }


    bItemDrop_ExternalScreen((char)m_stMCursor.sSelectedObjectID, msX, msY);

    return false;
}

void CGame::bItemDrop_ExternalScreen(char cItemID, short msX, short msY)
{

    char  cName[21];
    short sType, tX, tY;
    int sStatus;

    if (bCheckItemOperationEnabled(cItemID) == false) return;




    if ((m_sMCX != 0) && (m_sMCY != 0) && (abs(m_sPlayerX - m_sMCX) <= 8) && (abs(m_sPlayerY - m_sMCY) <= 8))
    {

        memset(cName, 0, sizeof(cName));
        m_pMapData->get_owner(m_sMCX, m_sMCY, cName, &sType, &sStatus, &m_wCommObjectID);

        if (memcmp(m_cPlayerName, cName, 10) == 0)
        {


        }
        else
        {
            if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
                (m_pItemList[cItemID]->m_dwCount > 1))
            {

                m_stDialogBoxInfo[17].sX = msX - 140;
                m_stDialogBoxInfo[17].sY = msY - 70;
                if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

                m_stDialogBoxInfo[17].sV1 = m_sMCX;
                m_stDialogBoxInfo[17].sV2 = m_sMCY;

                m_stDialogBoxInfo[17].sV3 = sType;

                m_stDialogBoxInfo[17].sV4 = m_wCommObjectID;

                memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
                if (sType < 10)
                    memcpy(m_stDialogBoxInfo[17].cStr, cName, 10);
                else
                {

                    GetNpcName(sType, m_stDialogBoxInfo[17].cStr);
                }
                EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {


                switch (sType)
                {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        EnableDialogBox(20, 1, cItemID, sType);
                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > 639) tX = 639 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > 479) tY = 479 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        strcpy(m_stDialogBoxInfo[20].cStr, cName);

                        //bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName);
                        break;

                    case 20:

                        EnableDialogBox(20, 3, cItemID, sType);

                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > 639) tX = 639 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > 479) tY = 479 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        GetNpcName(sType, m_stDialogBoxInfo[20].cStr);
                        //bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName); 
                        break;

                    case 15:

                    case 24:

                        EnableDialogBox(20, 2, cItemID, sType);

                        m_stDialogBoxInfo[20].sV3 = 1;
                        m_stDialogBoxInfo[20].sV4 = m_wCommObjectID;
                        m_stDialogBoxInfo[20].sV5 = m_sMCX;
                        m_stDialogBoxInfo[20].sV6 = m_sMCY;

                        tX = msX - 117;
                        tY = msY - 50;
                        if (tX < 0) tX = 0;
                        if ((tX + 235) > 639) tX = 639 - 235;
                        if (tY < 0) tY = 0;
                        if ((tY + 100) > 479) tY = 479 - 100;
                        m_stDialogBoxInfo[20].sX = tX;
                        m_stDialogBoxInfo[20].sY = tY;

                        memset(m_stDialogBoxInfo[20].cStr, 0, sizeof(m_stDialogBoxInfo[20].cStr));
                        GetNpcName(sType, m_stDialogBoxInfo[20].cStr);
                        break;

                    default:
                        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, cItemID, 1, m_sMCX, m_sMCY, m_pItemList[cItemID]->m_cName);
                        break;
                }


                //m_bIsItemDisabled[cItemID] = true;
            }
            m_bIsItemDisabled[cItemID] = true;
        }
    }
    else
    {

        if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
            (m_pItemList[cItemID]->m_dwCount > 1))
        {

            m_stDialogBoxInfo[17].sX = msX - 140;
            m_stDialogBoxInfo[17].sY = msY - 70;
            if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

            m_stDialogBoxInfo[17].sV1 = 0;
            m_stDialogBoxInfo[17].sV2 = 0;

            m_stDialogBoxInfo[17].sV3 = 0;
            m_stDialogBoxInfo[17].sV4 = 0;
            memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
            EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
        }
        else
        {

            if (_ItemDropHistory(m_pItemList[cItemID]->m_cName))
            {
                m_stDialogBoxInfo[4].sX = msX - 140;
                m_stDialogBoxInfo[4].sY = msY - 70;

                if (m_stDialogBoxInfo[4].sY < 0)
                    m_stDialogBoxInfo[4].sY = 0;
                m_stDialogBoxInfo[4].sV1 = 0;
                m_stDialogBoxInfo[4].sV2 = 0;

                m_stDialogBoxInfo[4].sV3 = 1;
                m_stDialogBoxInfo[4].sV4 = 0;
                m_stDialogBoxInfo[4].sV5 = cItemID;

                memset(m_stDialogBoxInfo[4].cStr, 0, sizeof(m_stDialogBoxInfo[4].cStr));
                EnableDialogBox(4, cItemID, m_pItemList[cItemID]->m_dwCount, 0);
            }
            else
            {
                bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_ITEMDROP, 0, cItemID, 1, 0, m_pItemList[cItemID]->m_cName);
            }
        }
        m_bIsItemDisabled[cItemID] = true;
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

void CGame::InitGameSettings()
{
    int i;

#if DEF_LANGUAGE == 2
    m_bForceAttack = true;
#else
    m_bForceAttack = false;
#endif
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
    m_iPointCommandType = -1;

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

    for (i = 0; i < 41; i++)
        m_bIsDialogEnabled[i] = false;

    for (i = 0; i < 38; i++)
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

        if (m_pAgreeMsgTextList[i] != 0)
            delete m_pAgreeMsgTextList[i];
        m_pAgreeMsgTextList[i] = 0;
    }

    for (i = 0; i < DEF_MAXPARTYMEMBERS; i++)
    {
        m_stPartyMember[i].cStatus = 0;
        memset(m_stPartyMember[i].cName, 0, sizeof(m_stPartyMember[i].cName));
    }

    m_iLU_Point = 3;
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
    //	m_iRating = 0;

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
    for (i = 0; i < DEF_MAXPARTYMEMBERS; i++) memset(m_stPartyMemberNameList[i].cName, 0, sizeof(m_stPartyMemberNameList[i].cName));

    m_iGizonItemUpgradeLeft = 0;

    EnableDialogBox(9, 0, 0, 0);
}

void CGame::_GetHairColorColor(int iColorType, int * pR, int * pG, int * pB)
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

void CGame::AddEventList(const char * pTxt, char cColor, bool bDupAllow)
{
    int i;

    if ((bDupAllow == false) && (strcmp(m_stEventHistory[5].cTxt, pTxt) == 0)) return;


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
50 // Rudolph
};

void CGame::bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1)
{
    int i;
    short sAbsX, sAbsY, sDist;
    long lPan;
    int  iV2 = 0;


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

    sAbsX = abs(((m_sViewPointX / 32) + 10) - dX);
    sAbsY = abs(((m_sViewPointY / 32) + 7) - dY);

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

                    sAbsX = abs(((m_sViewPointX / 32) + 10) - sX);
                    sAbsY = abs(((m_sViewPointY / 32) + 7) - sY);

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
                    PlaySound('E', 12, sDist, lPan);
                    break;

                case 5:

                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 6:	 // Energy Bolt
                case 10: // Lightning Arrow

                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 10;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(320 - (sX - m_sViewPointX)) * 1000;
                    PlaySound('E', 2, sDist, lPan);
                    SetCameraShakingEffect(sDist);
                    break;

                case 7:
                    // Magic Missile Exp
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 5;
                    m_pEffectList[i]->m_dwFrameTime = 50;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(320 - (sX - m_sViewPointX)) * 1000;
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
#if DEF_LANGUAGE != 3

                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_rX = 6 - (rand() % 12);

                    if (iV2 == 0)
                        m_pEffectList[i]->m_rY = -2 - (rand() % 4);
                    else m_pEffectList[i]->m_rY = -2 - (rand() % 10);

                    m_pEffectList[i]->m_cMaxFrame = 8;
                    m_pEffectList[i]->m_dwFrameTime = 30;
#endif
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(320 - (sX - m_sViewPointX)) * 1000;
                    PlaySound('E', 4, sDist, lPan);
                    SetCameraShakingEffect(sDist * 2);
                    break;

                case 31:

                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 8;
                    m_pEffectList[i]->m_dwFrameTime = 40;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = -(320 - (sX - m_sViewPointX)) * 1000;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    SetCameraShakingEffect(sDist);
                    break;

                case 40:

                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
                    PlaySound('E', 46, sDist, lPan);
                    break;

                case 50:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 50;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 53: // Hold twist
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;
                    m_pEffectList[i]->m_cMaxFrame = 15;
                    m_pEffectList[i]->m_dwFrameTime = 80;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
                    PlaySound('E', 4, sDist, lPan);

                    m_pEffectList[i]->m_iV1 = sDist;
                    //SetCameraShakingEffect(sDist, 2);
                    break;

                case 69:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - (sX / 32)) * 1000;
                    PlaySound('E', 42, sDist, lPan);
                    break;

                case 70:
                    m_pEffectList[i]->m_mX = sX;
                    m_pEffectList[i]->m_mY = sY;

                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 30;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;

                    sDist = sDist / 32;

                    lPan = -(((m_sViewPointX / 32) + 10) - (sX / 32)) * 1000;
                    PlaySound('E', 42, sDist, lPan);
                    break;


                case 71:
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

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

                    sAbsX = abs(320 - (sX - m_sViewPointX));
                    sAbsY = abs(240 - (sY - m_sViewPointY));

                    if (sAbsX > sAbsY) sDist = sAbsX;
                    else sDist = sAbsY;
                    sDist = sDist / 32;

                    lPan = ((sX - m_sViewPointX) - 320) * 30;
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

                case 100:

                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;
                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;

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
                case 102:

                case 122: // Recall
                case 126:
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 150: // Berserk : Cirlcle 6 magic 
                case 180: // Illusion
                case 190: // Mass-Illusion

                    m_pEffectList[i]->m_cMaxFrame = 11;
                    m_pEffectList[i]->m_dwFrameTime = 100;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 113: // Defense-Shield
                case 144: // Great-Defense-Shield

                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 120;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;


                case 111: // Stamina-Drain
                case 101: //Heal
                case 121: // Great Heal
                case 123: // Stamina-Recovery
                case 128: // Great-Stamina-Recovery
                    m_pEffectList[i]->m_cMaxFrame = 14;
                    m_pEffectList[i]->m_dwFrameTime = 80;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 132: // Invi
                case 112: // Recall
                case 131: // Summon
                    m_pEffectList[i]->m_cMaxFrame = 12;
                    m_pEffectList[i]->m_dwFrameTime = 80;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;


                case 110:
                    // Energy-Bolt
                    m_pEffectList[i]->m_mX = sX * 32;
                    m_pEffectList[i]->m_mY = sY * 32 - 40;

                    m_pEffectList[i]->m_iErr = 0;
                    m_pEffectList[i]->m_cMaxFrame = 0;
                    m_pEffectList[i]->m_dwFrameTime = 20;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 1, sDist, lPan);
                    break;

                case 138:
                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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
                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 5, sDist, lPan);
                    break;

                case 165: // Absolute-Magic-Protection
                    m_pEffectList[i]->m_cMaxFrame = 21;
                    m_pEffectList[i]->m_dwFrameTime = 70;

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 40, sDist, lPan);
                    break;

                case 145:
                    m_pEffectList[i]->m_cMaxFrame = 2;
                    m_pEffectList[i]->m_dwFrameTime = 10;
                    break;

                case 147:
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
                    PlaySound('E', 40, sDist, lPan);
                    break;

                case 156: // Mass-Ligtning-Arrow
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

                    lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
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

                case 181:

                    m_pEffectList[i]->m_mX = dX * 32 + 300;
                    m_pEffectList[i]->m_mY = dY * 32 - 460;

                    m_pEffectList[i]->m_cMaxFrame = 10;
                    m_pEffectList[i]->m_dwFrameTime = 25;
                    break;


                case 191:

                    m_pEffectList[i]->m_cMaxFrame = 7;
                    m_pEffectList[i]->m_dwFrameTime = 80;
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

void CGame::DrawEffects()
{
    int i, dX, dY, iDvalue, tX, tY, rX, rY, rX2, rY2, rX3, rY3, rX4, rY4, rX5, rY5, iErr;
    char  cTempFrame;
    uint32_t dwTime = m_dwCurTime;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if ((m_pEffectList[i] != 0) && (m_pEffectList[i]->m_cFrame >= 0))
        {

            switch (m_pEffectList[i]->m_sType)
            {
                case 1:
                    if (m_pEffectList[i]->m_cFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[8]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 2:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 2;
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[7]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                    break;

                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[8]->put_trans_sprite_no_color_key(dX, dY, 1, dwTime);
                    break;

                case 4:
                    /*
                    if (m_pEffectList[i]->m_cFrame < 9) break;
                    cTempFrame = m_pEffectList[i]->m_cFrame - 9;
                    dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                    dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
                    m_pEffectSpr[1]->PutSpriteFast(dX, dY-40, cTempFrame, dwTime);
                    */
                    break;

                case 5:
                    // Fire Explosion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 8) * (-5);

                    if (cTempFrame < 7)
                        m_pEffectSpr[3]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[3]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
                    break;

                case 6:	 // Energy Bolt 
                case 10: // Lightning Arrow
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 7) * (-6);

                    if (cTempFrame < 6)
                        m_pEffectSpr[6]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[6]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 7:
                    // Magic Missile Explosion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    iDvalue = (cTempFrame - 4) * (-3);

                    if (cTempFrame < 4)
                        m_pEffectSpr[6]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[6]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 8:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    cTempFrame = 4 - cTempFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 9:
                    cTempFrame = (rand() % 5);
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 11:
#if DEF_LANGUAGE != 3
                    cTempFrame = (rand() % 5) + 5;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite2(dX, dY, cTempFrame, dwTime);
#endif
                    break;

                case 12:
                    cTempFrame = (rand() % 6) + 10;
                    if (cTempFrame < 0) break;

                    iDvalue = (m_pEffectList[i]->m_cFrame - 4) * (-3);

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame < 4)
                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    else m_pEffectSpr[11]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    //m_pEffectSpr[11]->PutTransSprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 13:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame < 13)
                    {

                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, 25 + (cTempFrame / 5), dwTime);
                    }
                    else
                    {

                        m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, (8 + cTempFrame), dwTime);
                    }
                    break;

                case 14:
                    if (m_pEffectList[i]->m_cFrame < 0) break;

                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, (28 + m_pEffectList[i]->m_cFrame), dwTime);
                    break;

                case 15:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = m_pEffectList[i]->m_mX - m_sViewPointX;
                    dY = m_pEffectList[i]->m_mY - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, (33 + cTempFrame), dwTime);
                    break;

                case 16:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 0, dwTime);
                    break;

                case 17:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    cTempFrame = 39 + (rand() % 3) * 3 + (rand() % 3);
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);

                    dX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;

                    m_pEffectSpr[11]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 18:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[18]->put_trans_sprite70_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 30:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[14]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 31:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[15]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 32:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame + 20;
                    if (cTempFrame < 0) break;

                    m_pEffectSpr[11]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 33:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    iDvalue = 0;
                    m_pEffectSpr[19]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 40:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[20]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime); // 20
                    break;

                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:


                    dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
                    m_pEffectSpr[21]->put_fade_sprite(dX, dY, 48, dwTime);

                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if ((8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame) < (8 * (m_pEffectList[i]->m_sType - 41) + 7))
                    {
                        iDvalue = -8 * (6 - cTempFrame);
                        m_pEffectSpr[21]->put_trans_sprite_color(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    }
                    else
                    {
                        if ((cTempFrame - 5) >= 8) cTempFrame = ((cTempFrame - 5) - 8) + 5;
                        m_pEffectSpr[21]->put_sprite_fast(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + (cTempFrame - 5), dwTime);
                    }
                    break;


                case 47:
                case 48:
                case 49:

                    dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
                    m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_reverse_trans_sprite(dX, dY, 0, dwTime);

                    cTempFrame = m_pEffectList[i]->m_cFrame;

                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    //PutString(dX, dY, "*", Color(255,255,255));
                    if (cTempFrame < 7)
                    {
                        iDvalue = -8 * (6 - cTempFrame);
                        m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_trans_sprite_color(dX, dY, cTempFrame + 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    else
                    {
                        if (cTempFrame >= 8) cTempFrame = cTempFrame % 8;
                        m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->put_sprite_fast(dX, dY, cTempFrame + 1, dwTime);
                    }
                    break;

                case 50:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame <= 6)
                    {
                        iDvalue = 0;
                        m_pEffectSpr[22]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2			
                    }
                    else
                    {
                        iDvalue = -5 * (cTempFrame - 6);
                        m_pEffectSpr[22]->put_trans_sprite_color(dX, dY, 6, iDvalue, iDvalue, iDvalue, dwTime); // RGB2				
                    }
                    break;

                case 51:
                    cTempFrame = m_pEffectList[i]->m_cFrame + 11; //15
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[28]->put_trans_sprite25(dX, dY, cTempFrame, dwTime); //20
                    break;

                case 52:
                    break;
                    /*
                    case 52: // Protection Ring
                        cTempFrame = m_pEffectList[i]->m_cFrame;
                        dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                        dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
                        m_pEffectSpr[24]->PutTransSprite(dX, dY, cTempFrame, dwTime);
                        break;
                    */

                case 53: // Hold Twist
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[25]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime); //25
                    break;

                case 54:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[28]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 55:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX);
                    dY = (m_pEffectList[i]->m_mY);
                    m_pEffectSpr[28]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 56:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) cTempFrame = 0;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[29]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 181:
                case 60:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame > 4)
                    {
                        cTempFrame = cTempFrame / 4;
                    }

                    if (cTempFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;


                        m_pEffectSpr[31]->put_sprite_fast(dX, dY, 15 + cTempFrame, dwTime);

                        m_pEffectSpr[31]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    }
                    break;

                case 61:

                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[32]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 62:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    if (cTempFrame > 0)
                    {
                        cTempFrame = cTempFrame - 1;
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        m_pEffectSpr[31]->put_reverse_trans_sprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame / 3);
                    }
                    break;

                case 63:

                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[33]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 64:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[34]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 65:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    cTempFrame = cTempFrame / 6;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[31]->put_reverse_trans_sprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame >> 2);
                    break;

                case 66:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[39]->put_reverse_trans_sprite(dX, dY, cTempFrame, dwTime);
                    m_pEffectSpr[39]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 67:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    switch (rand() % 3)
                    {
                        case 0: m_pEffectSpr[0]->put_trans_sprite25_no_color_key(dX, dY + 20, 1, dwTime); break;
                        case 1: m_pEffectSpr[0]->put_trans_sprite50_no_color_key(dX, dY + 20, 1, dwTime); break;
                        case 2: m_pEffectSpr[0]->put_trans_sprite70_no_color_key(dX, dY + 20, 1, dwTime); break;
                    }

                    m_pEffectSpr[35]->put_trans_sprite70_no_color_key(dX, dY, cTempFrame / 3, dwTime);
                    break;

                case 68:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    if (cTempFrame <= 11)
                    {
                        m_pEffectSpr[40]->put_sprite_fast(dX, dY, cTempFrame, dwTime);
                        m_pEffectSpr[41]->put_trans_sprite50_no_color_key(dX, dY, cTempFrame, dwTime);
                        m_pEffectSpr[44]->put_reverse_trans_sprite(dX - 2, dY - 3, cTempFrame, dwTime);
                        m_pEffectSpr[44]->put_trans_sprite_no_color_key(dX - 4, dY - 3, cTempFrame, dwTime);
                    }
                    else
                    {
                        switch (cTempFrame)
                        {
                            case 12:
                            case 13:
                            case 14: m_pEffectSpr[40]->put_sprite_fast(dX, dY, 11, dwTime); break;
                            case 15: m_pEffectSpr[40]->put_trans_sprite70_no_color_key(dX, dY, 11, dwTime); break;
                            case 16: m_pEffectSpr[40]->put_trans_sprite50_no_color_key(dX, dY, 11, dwTime); break;
                            case 17: m_pEffectSpr[40]->put_trans_sprite25_no_color_key(dX, dY, 11, dwTime); break;
                        }
                    }
                    break;

                case 69:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[42]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 70:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[43]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;


                case 72:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    if (cTempFrame <= 8)
                    {
                        iDvalue = 0;
                        m_pEffectSpr[51]->put_trans_sprite_color(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    else
                    {
                        iDvalue = -1 * (cTempFrame - 8);
                        m_pEffectSpr[51]->put_trans_sprite_color(dX, dY, 8, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2			
                    }
                    break;

                case 100: // Magic Missile
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 0, dwTime);
                    break;

                case 110: // Energy-Bolt
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY, 2 + (rand() % 4), dwTime);
                    break;


                case 102: // Create Food

                case 124: // Protection from N.M
                case 125: // Hold-Person
                case 126: // Possession
                case 127: // Poison
                case 133: // Protect-From-Magic
                case 134: // Detect-Invisibility
                case 135: // Paralyze
                case 136: // Cure
                case 142: // Confuse Language
                case 152:
                case 153: // Mass-Poison
                case 162: // Confusion
                case 171: // Mass-Confusion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 5)
                        m_pEffectSpr[4]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[4]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 180: // Illusion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-3);

                    if (cTempFrame < 9)	m_pEffectSpr[60]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[60]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 190: // Mass-Illusion
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-3);

                    if (cTempFrame < 9) m_pEffectSpr[61]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[61]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 113: // Defense Shield
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 6)
                        m_pEffectSpr[62]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[62]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;

                case 144: // Great-Defense-Shield
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);

                    if (cTempFrame < 9)
                        m_pEffectSpr[63]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    else m_pEffectSpr[63]->put_trans_sprite_color(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
                    break;


                case 111:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[49]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 101: // Heal
                case 121:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[50]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 123: // Stamina-Recovery
                case 128:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[56]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 132: // Invisibility
                case 112: // Recall
                case 131: // Summon-Creature
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;

                    iDvalue = (cTempFrame - 5) * (-5);
                    m_pEffectSpr[52]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;


                case 165: // Absolute-Magic-Protect
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[53]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;


                case 166: // Armor-Break
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    if (cTempFrame < 0) break;

                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[55]->put_reverse_trans_sprite(dX, dY + 35, m_pEffectList[i]->m_cFrame, dwTime);
                    m_pEffectSpr[54]->put_trans_sprite50(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 120: // Fire Ball
                case 130: // Fire Strike
                case 161: // Mass-Fire-Strike
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[5]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    break;

                case 137: // Lightning Arrow

                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                    tX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
                    tY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;

                    iErr = 0;
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX, &rY, &iErr, 15);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX2, &rY2, &iErr, 30);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX3, &rY3, &iErr, 45);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX4, &rY4, &iErr, 60);
                    m_Misc.GetPoint(dX, dY, tX, tY, &rX5, &rY5, &iErr, 75);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite25_no_color_key(rX5, rY5, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite25_no_color_key(rX4, rY4, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite50_no_color_key(rX3, rY3, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite50_no_color_key(rX2, rY2, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite70_no_color_key(rX, rY, cTempFrame, dwTime);

                    cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
                    if (cTempFrame < 0) break;
                    m_pEffectSpr[10]->put_trans_sprite(dX, dY, cTempFrame, dwTime);
                    break;

                case 143: // Lightning
                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX + 4, m_pEffectList[i]->m_rY + 2, 2);

                    _DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
                    break;

                case 151: // Lightning Bolt
                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX + 2, m_pEffectList[i]->m_rY - 2, 2);

                    _DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
                        m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
                        m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
                    break;
            }
        }
}

void CGame::bItemDrop_IconPannel(short msX, short msY)
{

    short sX, sY, sItemIndex;

    sX = m_stDialogBoxInfo[30].sX;
    sY = m_stDialogBoxInfo[30].sY;

    sItemIndex = m_stMCursor.sSelectedObjectID;
    if (m_bIsItemDisabled[sItemIndex] == true) return;


    if (m_cCommand < 0) return;


    if ((453 < msX) && (486 > msX) && (440 < msY) && (475 > msY))
    {
        bItemDrop_Inventory(m_stDialogBoxInfo[2].sX + (rand() % 148), m_stDialogBoxInfo[2].sY + (rand() % 55));
        return;
    }

    if ((425 < msX) && (448 > msX) && (440 < msY) && (475 > msY))
    {
        bItemDrop_Character();
        return;
    }
}

void CGame::DrawEffectLights()
{
    int i, dX, dY, iDvalue;
    uint32_t dwTime = m_dwCurTime;
    char  cTempFrame;


    for (i = 0; i < DEF_MAXEFFECTS; i++)
        if (m_pEffectList[i] != 0)
        {
            switch (m_pEffectList[i]->m_sType)
            {
                case 1:
                    break;

                case 2:
                    break;

                case 4:
                    break;

                case 5:
                    // Fire Explosion
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 7) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 6)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 7:
                    // Magic Missile Explosion
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 2) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 2)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 69:
                case 70:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[0]->put_trans_sprite25(dX, dY + 30, 1, dwTime);
                    break;

                case 16:
                case 61:
                case 66:
                case 100:
                case 110:
                case 120:
                case 130:
                case 137:
                case 165:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    iDvalue = -5;
                    m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    break;

                case 6:	 // Energy Bolt
                case 10: // Lightning Arrow
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 9) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 8)
                            m_pEffectSpr[0]->put_trans_sprite_no_color_key(dX, dY + 30, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY + 30, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 33:
                    /*
                    if (m_pEffectList[i]->m_cFrame >= 0) {
                        dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
                        dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 9)*(-1);

                        if (m_pEffectList[i]->m_cFrame < 8)
                             m_pEffectSpr[0]->PutTransSprite(dX, dY, 1, dwTime);
                        else m_pEffectSpr[0]->PutTransSpriteColor(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    */
                    break;

                case 40:
                case 56:
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

                        iDvalue = (m_pEffectList[i]->m_cFrame - 7) * (-1);

                        if (m_pEffectList[i]->m_cFrame < 6)
                            m_pEffectSpr[0]->put_trans_sprite(dX, dY, 1, dwTime);
                        else m_pEffectSpr[0]->put_trans_sprite_color(dX, dY, 1, iDvalue, iDvalue, iDvalue, dwTime);
                    }
                    break;

                case 52: // Protection Ring
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[24]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    }
                    break;

                case 57:
                    if (m_pEffectList[i]->m_cFrame >= 0)
                    {
                        dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                        dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                        m_pEffectSpr[30]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    }
                    break;

                case 73:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[74]->put_trans_sprite(dX, dY - 34, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 74:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[75]->put_trans_sprite(dX, dY + 35, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 75:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[76]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 76:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[77]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 77:
                    dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
                    m_pEffectSpr[78]->put_trans_sprite25(dX + m_pEffectList[i]->m_dX * m_pEffectList[i]->m_cFrame, dY + m_pEffectList[i]->m_dY * m_pEffectList[i]->m_cFrame, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 150: // Berserk : Circle 6 magic 
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[58]->put_trans_sprite_no_color_key(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
                    break;

                case 180:
                case 190:
                    cTempFrame = m_pEffectList[i]->m_cFrame;
                    dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
                    dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
                    m_pEffectSpr[59]->put_trans_sprite_no_color_key(dX, dY, cTempFrame, dwTime);
                    //if( cTempFrame < 9 ) m_pEffectSpr[59]->PutTransSprite_NoColorKey( dX, dY, cTempFrame, dwTime );
                    //else m_pEffectSpr[59]->PutSpriteFast( dX, dY, cTempFrame, dwTime );
                    break;
            }
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

            change_game_mode(DEF_GAMEMODE_ONCONNECTIONLOST);

            close(ix::WebSocketCloseConstants::kAbnormalCloseCode, ix::WebSocketCloseConstants::kAbnormalCloseMessage);
            break;
    }
}

void CGame::_ReadMapData(short sPivotX, short sPivotY, char * pData)
{
    int i{};
    char * cp{}, ucHeader{}, cDir{}, cName[12]{}, cItemColor{};
    short * sp{};
    short sTotal{}, sX{}, sY{}, sType{}, sAppr1{}, sAppr2{}, sAppr3{}, sAppr4{}, sItemSpr{}, sItemSprFrame{}, sDynamicObjectType{};
    int  sStatus{}, iApprColor{};
    int * ip{};
    uint16_t    wObjectID{};
    uint16_t * wp{}, wDynamicObjectID{};

    cp = pData;

    m_sVDL_X = sPivotX; // Valid Data Loc-X
    m_sVDL_Y = sPivotY;

    return;
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

                ip = (int *)cp;
                iApprColor = *ip;
                cp += 4;
                // Status
                ip = (int *)cp;
                sStatus = *ip;
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
                sStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 5);
                cp += 5;
            }

            m_pMapData->set_owner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName, DEF_OBJECTSTOP, 0, 0, 0);
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

                ip = (int *)cp;
                iApprColor = *ip;
                cp += 4;
                // Status
                ip = (int *)cp;
                sStatus = *ip;
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
                sStatus = *ip;
                cp += 4;
                // Name
                memset(cName, 0, sizeof(cName));
                memcpy(cName, cp, 5);
                cp += 5;
            }

            m_pMapData->bSetDeadOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName);
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

            m_pMapData->bSetItem(sX, sY, sItemSpr, sItemSprFrame, cItemColor, false);
        }

        if (ucHeader & 0x08)
        {

            wp = (uint16_t *)cp;
            wDynamicObjectID = *wp;
            cp += 2;

            sp = (short *)cp;
            sDynamicObjectType = *sp;
            cp += 2;

            m_pMapData->bSetDynamicObject(sX, sY, wDynamicObjectID, sDynamicObjectType, false);
        }
    }
}

int CGame::_iCheckLUS()
{
    if (m_iLU_Point != 0) return 1;
    if ((m_iStr + m_cLU_Str) > 200) return 2;
    if ((m_iDex + m_cLU_Dex) > 200) return 2;
    if ((m_iInt + m_cLU_Int) > 200) return 2;
    if ((m_iVit + m_cLU_Vit) > 200) return 2;
    if ((m_iMag + m_cLU_Mag) > 200) return 2;
    if ((m_iCharisma + m_cLU_Char) > 200) return 2;

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

    memset(m_cAccountQuiz, 0, sizeof(m_cAccountQuiz));
    memset(m_cAccountAnswer, 0, sizeof(m_cAccountAnswer));
}

void CGame::change_game_mode(char cMode)
{
    if (m_cGameMode == DEF_GAMEMODE_ONLOADING)
        window.setFramerateLimit(frame_limit);
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
                            std::this_thread::sleep_for(1ms);
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
            }
        }
        else
        {
            if (memcmp(token, "log-server-address", 18) == 0) cReadMode = 1;
            if (memcmp(token, "log-server-port", 15) == 0)    cReadMode = 2;
        }
        token = strtok(0, seps);
    }
    delete[] cp;
    if (pFile != 0) fclose(pFile);
    return true;
}

void CGame::ReleaseUnusedSprites()
{
    int i;


    for (i = 0; i < DEF_MAXSPRITES; i++)
        if ((m_pSprite[i] != nullptr))
            if ((m_pSprite[i]->m_bIsSurfaceEmpty == false) && (m_pSprite[i]->m_bOnCriticalSection == false))
                if ((G_dwGlobalTime - m_pSprite[i]->m_dwRefTime) > 60000)
                    m_pSprite[i]->close_sprite_();

    for (i = 0; i < DEF_MAXTILES; i++)
        if ((m_pTileSpr[i] != nullptr))
            if ((m_pTileSpr[i]->m_bIsSurfaceEmpty == false) && (m_pTileSpr[i]->m_bOnCriticalSection == false))
                if ((G_dwGlobalTime - m_pTileSpr[i]->m_dwRefTime) > 60000)
                    m_pTileSpr[i]->close_sprite_();

    for (i = 0; i < DEF_MAXEFFECTSPR; i++)
        if ((m_pEffectSpr[i] != nullptr))
            if ((m_pEffectSpr[i]->m_bIsSurfaceEmpty == false) && (m_pEffectSpr[i]->m_bOnCriticalSection == false))
                if ((G_dwGlobalTime - m_pEffectSpr[i]->m_dwRefTime) > 60000)
                    m_pEffectSpr[i]->close_sprite_();
}

void CGame::PutChatScrollList(const char * pMsg, char cType)
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
}

void CGame::ReleaseTimeoverChatMsg()
{
    int i;
    uint32_t dwTime;

    // TODO: why is this not as updated anymore? Magic is created with a time in the "future" compared to this
    //dwTime = G_dwGlobalTime;
    dwTime = unixtime();

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
#if DEF_LANGUAGE != 3

                        if (m_pEffectList[i]->m_cFrame == 1)
                        {

                            for (int j = 1; j <= m_pEffectList[i]->m_iV1; j++) bAddNewEffect(11, m_pEffectList[i]->m_mX + 15 - (rand() % 30), m_pEffectList[i]->m_mY + 15 - (rand() % 30), 0, 0, -1 * (rand() % 2));
                        }
#endif
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

                            //m_pMapData->bSetItem(m_pEffectList[i]->m_mX/32, m_pEffectList[i]->m_mY/32, 6, 0, 0);
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
#if DEF_LANGUAGE != 3
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {

                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                        }
#endif

                        if (m_pEffectList[i]->m_cFrame >= m_pEffectList[i]->m_cMaxFrame)
                        {

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }

                        break;

                    case 7:
#if DEF_LANGUAGE != 3
                        // Magic Missile Burst
                        if (m_pEffectList[i]->m_cFrame == 1)
                        {

                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                            bAddNewEffect(9, m_pEffectList[i]->m_mX + 5 - (rand() % 10), m_pEffectList[i]->m_mY + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                        }
#endif

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
#if DEF_LANGUAGE != 3

                        m_pEffectList[i]->m_mX += m_pEffectList[i]->m_rX;
                        m_pEffectList[i]->m_mY += m_pEffectList[i]->m_rY;

                        m_pEffectList[i]->m_rY++;

                        if (m_pEffectList[i]->m_cFrame > m_pEffectList[i]->m_cMaxFrame)
                        {

                            delete m_pEffectList[i];
                            m_pEffectList[i] = 0;
                        }
#endif				
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

                        cDir = m_Misc.cGetNextMoveDir(m_pEffectList[i]->m_mX, m_pEffectList[i]->m_mY, m_pEffectList[i]->m_mX3, m_pEffectList[i]->m_mY3);

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

                    case 147:
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
                    case 150: // Berserk : Cirlcle 6 magic 
                    case 152:
                    case 153:
                    case 162:
                    case 165:
                    case 166:
                    case 171:
                    case 180:
                    case 190:

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
                start_input_string(sX + 75, sY + 140, 21, m_cGuildName);
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
                start_input_string(sX + 40, sY + 57, 11, m_cAmountString, false);
            }
            else
            {

                if (m_stDialogBoxInfo[17].cMode == 1)
                {

                    sX = m_stDialogBoxInfo[17].sX;
                    sY = m_stDialogBoxInfo[17].sY;
                    EndInputString();
                    start_input_string(sX + 40, sY + 57, 11, m_cAmountString, false);
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
            }
            break;

        case 27:
            if (m_bIsDialogEnabled[27] == false)
            {
                m_stDialogBoxInfo[27].cMode = cType;
                memset(m_stDialogBoxInfo[27].cStr, 0, sizeof(m_stDialogBoxInfo[27].cStr));
                memset(m_stDialogBoxInfo[27].cStr2, 0, sizeof(m_stDialogBoxInfo[27].cStr2));
                memset(m_stDialogBoxInfo[27].cStr3, 0, sizeof(m_stDialogBoxInfo[27].cStr3));
                memset(m_stDialogBoxInfo[27].cStr4, 0, sizeof(m_stDialogBoxInfo[27].cStr4));
                m_stDialogBoxInfo[27].sV1 = -1;
                m_stDialogBoxInfo[27].sV2 = -1;
                m_stDialogBoxInfo[27].sV3 = -1;
                m_stDialogBoxInfo[27].sV4 = -1;
                m_stDialogBoxInfo[27].sV5 = -1;
                m_stDialogBoxInfo[27].sV6 = -1;
                m_stDialogBoxInfo[27].sV7 = -1;
                m_stDialogBoxInfo[27].sV8 = -1;
                m_stDialogBoxInfo[27].sV9 = -1;
                m_stDialogBoxInfo[27].sV10 = -1;
                m_stDialogBoxInfo[27].sV11 = -1;
                m_stDialogBoxInfo[27].sV12 = -1;
                m_stDialogBoxInfo[27].sV13 = -1;
                m_stDialogBoxInfo[27].sV14 = -1;
                m_stDialogBoxInfo[27].dwV1 = 0;
                m_stDialogBoxInfo[27].dwV2 = 0;


                DisableDialogBox(17);
                DisableDialogBox(20);
                DisableDialogBox(23);
                DisableDialogBox(26);
            }
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
#ifdef DEF_GIZON
            if (m_bIsDialogEnabled[34] == false)
            {
                m_stDialogBoxInfo[34].cMode = cType;
                m_stDialogBoxInfo[34].sV1 = -1;
                m_stDialogBoxInfo[34].dwV1 = 0;
            }
#else
            if (m_bIsDialogEnabled[34] == false)
            {
                int iSoX, iSoM;
                iSoX = iSoM = 0;
                for (i = 0; i < DEF_MAXITEMS; i++)
                    if (m_pItemList[i] != 0)
                    {
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 128)) iSoX++;
                        if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 129)) iSoM++;
                    }

                if ((iSoX > 0) || (iSoM > 0))
                {

                    m_stDialogBoxInfo[34].cMode = 6;
                    m_stDialogBoxInfo[34].sV2 = iSoX;
                    m_stDialogBoxInfo[34].sV3 = iSoM;
                    m_stDialogBoxInfo[34].sV1 = -1;
                    m_stDialogBoxInfo[34].dwV1 = 0;
                }
                else
                {
                    AddEventList(DRAW_DIALOGBOX_ITEMUPGRADE30, 10);
                    return;
                }
            }
#endif
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
    for (i = 0; i < 39; i++)
        if (m_cDialogBoxOrder[i] == iBoxID) m_cDialogBoxOrder[i] = 0;

    for (i = 1; i < 39; i++)
        if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
        {
            m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
            m_cDialogBoxOrder[i] = 0;
        }

    for (i = 0; i < 39; i++)
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
            m_stDialogBoxInfo[39].sV4 = 0;
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

        case 20:
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

        case 27:
            memset(m_stDialogBoxInfo[27].cStr, 0, sizeof(m_stDialogBoxInfo[27].cStr));
            memset(m_stDialogBoxInfo[27].cStr2, 0, sizeof(m_stDialogBoxInfo[27].cStr2));
            memset(m_stDialogBoxInfo[27].cStr3, 0, sizeof(m_stDialogBoxInfo[27].cStr3));
            memset(m_stDialogBoxInfo[27].cStr4, 0, sizeof(m_stDialogBoxInfo[27].cStr4));
            m_stDialogBoxInfo[27].sV1 = -1;
            m_stDialogBoxInfo[27].sV2 = -1;
            m_stDialogBoxInfo[27].sV3 = -1;
            m_stDialogBoxInfo[27].sV4 = -1;
            m_stDialogBoxInfo[27].sV5 = -1;
            m_stDialogBoxInfo[27].sV6 = -1;
            m_stDialogBoxInfo[27].sV9 = -1;
            m_stDialogBoxInfo[27].sV10 = -1;
            m_stDialogBoxInfo[27].sV11 = -1;
            m_stDialogBoxInfo[27].sV12 = -1;
            m_stDialogBoxInfo[27].sV13 = -1;
            m_stDialogBoxInfo[27].sV14 = -1;
            m_stDialogBoxInfo[27].dwV1 = 0;
            m_stDialogBoxInfo[27].dwV2 = 0;

            m_bIsItemDisabled[m_stDialogBoxInfo[27].sView] = false;
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

    }

    m_bIsDialogEnabled[iBoxID] = false;

    for (i = 0; i < 39; i++)
        if (m_cDialogBoxOrder[i] == iBoxID)
            m_cDialogBoxOrder[i] = 0;

    for (i = 1; i < 39; i++)
        if ((m_cDialogBoxOrder[i - 1] == 0) && (m_cDialogBoxOrder[i] != 0))
        {
            m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
            m_cDialogBoxOrder[i] = 0;
        }
}

int CGame::iGetTopDialogBoxIndex()
{
    int i;

    for (i = 38; i >= 0; i--)
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

    if (m_pInputBuffer == nullptr) return;

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

    if (m_iInputX2 == 0)
    {
        put_string(m_iInputX + 1, m_iInputY + 1, G_cTxt, Color(0, 0, 0));
        put_string(m_iInputX, m_iInputY + 1, G_cTxt, Color(0, 0, 0));
        put_string(m_iInputX + 1, m_iInputY, G_cTxt, Color(0, 0, 0));
        put_string(m_iInputX, m_iInputY, G_cTxt, Color(255, 255, 255));
    }
    else
    {
        put_aligned_string(m_iInputX + 1, m_iInputX2 + 1, m_iInputY + 1, G_cTxt, Color(255, 0, 0, 0));
        put_aligned_string(m_iInputX, m_iInputX2, m_iInputY + 1, G_cTxt, Color(255, 0, 0, 0));
        put_aligned_string(m_iInputX + 1, m_iInputX2 + 1, m_iInputY, G_cTxt, Color(255, 0, 0, 0));
        put_aligned_string(m_iInputX, m_iInputX2, m_iInputY, G_cTxt, Color(255, 255, 255, 255));
    }
}

void CGame::ClearInputString()
{
    if (m_pInputBuffer != 0)	memset(m_pInputBuffer, 0, sizeof(m_pInputBuffer));
    memset(m_cEdit, 0, sizeof(m_cEdit));
#ifdef DEF_USING_WIN_IME
    if (G_hEditWnd != 0)	SetWindowText(G_hEditWnd, "");
#endif
}

void CGame::start_input_string(int left, int top, uint32_t len, char * pBuffer, bool bIsHide, int right)
{
    m_bInputStatus = true;
    m_iInputX = left;
    m_iInputY = top;
    m_iInputX2 = right;
    m_pInputBuffer = pBuffer;
    memset(m_cEdit, 0, sizeof(m_cEdit));
    m_cInputMaxLen = len;
#ifdef DEF_USING_WIN_IME 
    if (bIsHide == false) G_hEditWnd = CreateWindow(RICHEDIT_CLASS, 0, WS_POPUP | ES_SELFIME, sX - 5, sY - 1, iLen * 12, 16, G_hWnd, (HMENU)0, G_hInstance, 0);
    else G_hEditWnd = CreateWindow(RICHEDIT_CLASS, 0, WS_POPUP | ES_PASSWORD | ES_SELFIME, sX - 5, sY - 1, iLen * 12, 16, G_hWnd, (HMENU)0, G_hInstance, 0);
    SetWindowText(G_hEditWnd, m_pInputBuffer);
    SendMessage(G_hEditWnd, EM_EXLIMITTEXT, 0, iLen - 1);
    SendMessage(G_hEditWnd, EM_SETLANGOPTIONS, 0, ~IMF_AUTOFONT);
    COMPOSITIONFORM composform;
    composform.dwStyle = CFS_POINT;
    composform.ptCurrentPos.x = sX;
    composform.ptCurrentPos.y = sY;
    HIMC hImc = ImmGetContext(G_hWnd);
    ImmSetCompositionWindow(hImc, &composform);
    int StrLen = strlen(m_pInputBuffer);
    SendMessage(G_hEditWnd, EM_SETSEL, StrLen, StrLen);
#endif
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

void CGame::DrawNewDialogBox(char cType, int sX, int sY, int iFrame, bool bIsNoColorKey, bool bIsTrans)
{
    uint32_t dwTime = G_dwGlobalTime;

    if (m_pSprite[cType] == 0) return;
    if (bIsNoColorKey == false)
    {
        if (bIsTrans == true)
            m_pSprite[cType]->put_trans_sprite2(sX, sY, iFrame, dwTime);
        else m_pSprite[cType]->put_sprite_fast(sX, sY, iFrame, dwTime);
    }
    else m_pSprite[cType]->put_sprite_fast_no_color_key(sX, sY, iFrame, dwTime);
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
    draw_shadow_box(0, 0, 639, 30);

    if ((((G_dwGlobalTime - m_dwTopMsgTime) / 250) % 2) == 0)
        put_aligned_string(0, 639, 10, m_cTopMsg, 255, 255, 255);

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
        {
            std::string str = std::format("{} XY({}, {})", m_pGameMsgList[19]->message, m_iConstructLocX, m_iConstructLocY);
            SetTopMsg(str, 5);
            break;
        }

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
    int iPlayerSide = 0;

    DisableDialogBox(18);

    for (int i = 0; i < DEF_TEXTDLGMAXLINES; i++)
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

        for (int i = 4; i < 18; i++)
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

            for (int i = 4; i < 18; i++)
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

                for (int i = 8; i < 18; i++)
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

                for (int i = 8; i < 18; i++)
                    m_pMsgTextList[i] = new CMsg(0, " ", 0);
            }
        }
    }

    EnableDialogBox(18, 0, 0, 0);

    DisableDialogBox(36);
    DisableDialogBox(37);
    DisableDialogBox(38);
}

void CGame::DrawChatMsgBox(short sX, short sY, int iChatIndex, bool bIsPreDC)
{
    char cMsgA[22]{}, cMsgB[22]{}, cMsgC[22]{};
    int iLines{}, i{}, iSize{}, iSize2{}, iLoc{}, iFontSize{};
    uint32_t dwTime{};
    Color rgb{};
    bool bIsTrans{};
    std::string & str = m_pChatMsgList[iChatIndex]->message;

    memset(cMsgA, 0, sizeof(cMsgA));
    memset(cMsgB, 0, sizeof(cMsgB));
    memset(cMsgC, 0, sizeof(cMsgC));

    dwTime = m_pChatMsgList[iChatIndex]->m_dwTime;
    iLines = 0;

    rgb = Color(255, 255, 255);
    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 1:
            rgb = Color(255, 255, 255);
            break;
        case 20:
            rgb = Color(255, 255, 20);
            if ((m_dwCurTime - dwTime) < 650) return;
            dwTime += 650;
            break;
        case 41:
            rgb = Color(255, 80, 80);
            break;

        case 42:
            rgb = Color(255, 80, 80);
            if ((m_dwCurTime - dwTime) < 650) return;
            dwTime += 650;
            break;
    }

    if (str.length() < 21)
    {
        memcpy(cMsgA, str.c_str(), 20);
        iLines = 1;
    }
    else if (str.length() < 41)
    {
        memcpy(cMsgA, str.c_str(), 20);
        memcpy(cMsgB, str.c_str() + 20, 20);
        iLines = 2;
    }
    else
    {
        memcpy(cMsgA, str.c_str(), 20);
        memcpy(cMsgB, str.c_str() + 20, 20);
        memcpy(cMsgC, str.c_str() + 40, 20);
        iLines = 3;
    }

    iSize = 0;
    for (i = 0; i < 20; i++)
    {
        if (cMsgA[i] != 0)
        {
            if ((unsigned char)cMsgA[i] >= 128)
            {
                iSize += 5;	//6
                i++;
            }
            else iSize += 4;
        }
    }

    iLoc = m_dwCurTime - dwTime;
    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 21:
        case 22:
        case 23:
            if (iLoc > 80) iLoc = 10;
            else iLoc = iLoc >> 3;
            break;
        default:
            if (iLoc > 352) iLoc = 9;
            else if (iLoc > 320) iLoc = 10;
            else iLoc = iLoc >> 5;
            break;
    }

    if (m_cDetailLevel == 0)
        bIsTrans = false;
    else bIsTrans = true;

    switch (m_pChatMsgList[iChatIndex]->m_cType)
    {
        case 41:
        case 42:
            iSize2 = 0;
            for (i = 0; i < str.length(); i++)
                if (str[i] != 0)
                    if ((unsigned char)str[i] >= 128)
                    {
                        iSize2 += 5; //6
                        i++;
                    }
                    else iSize2 += 4;
            if (m_Misc.bCheckIMEString(str.c_str()) == false)
            {
                //                 PutString(sX - iSize2, sY - 65 - iLoc, cMsg, Color(180, 30, 30));
                //                 PutString(sX - iSize2 + 1, sY - 65 - iLoc, cMsg, Color(180, 30, 30));
                put_string3(sX - iSize2, sY - 65 - iLoc, str, Color(180, 30, 30));
            }
            else
            {
                put_overhead_string(sX - iSize2, sY - 65 - iLoc, str, overhead_magic_color, 4, false, 16);
                //PutString_SprFont3(sX - iSize2, sY - 65 - iLoc, str, m_wR[14] * 4, m_wG[14] * 4, m_wB[14] * 4, false, 0);
            }
            break;

        case 21:
        case 22:
        case 23:
            switch (m_pChatMsgList[iChatIndex]->m_cType)
            {
                case 21:
                    iFontSize = 12; break;
                case 22:
                    iFontSize = 16; break;
                case 23:
                    iFontSize = 20; break;
            }
            switch (iLines)
            {
                case 1:
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    //PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    break;
                case 2:
                    put_overhead_string(sX - iSize, sY - 81 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgB, overhead_damage_color, 2, bIsTrans, iFontSize);
                    //                     PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    //                     PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgB, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    break;
                case 3:
                    put_overhead_string(sX - iSize, sY - 97 - iLoc, cMsgA, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 81 - iLoc, cMsgB, overhead_damage_color, 2, bIsTrans, iFontSize);
                    put_overhead_string(sX - iSize, sY - 65 - iLoc, cMsgC, overhead_damage_color, 2, bIsTrans, iFontSize);
                    //                     PutString_SprFont3(sX - iSize, sY - 97 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    //                     PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgB, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    //                     PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgC, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
                    break;
            }
            break;

        case 20:
        default:
            int len = (int)str.length() - 1;

            sf::Text chat_text;

            chat_text.setPosition(0.f, 0.f);
            chat_text.setFont(*arya_font);
            chat_text.setCharacterSize(12);
            chat_text.setString(str);

            sf::FloatRect fr = chat_text.getGlobalBounds();

            switch (int(fr.width / 160))
            {
                case 0:
                default:
                    chat_text.setPosition(float(sX - 80 + 1), float(sY - 65 - iLoc));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 65 - iLoc + 1));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 65 - iLoc));
                    chat_text.setFillColor(rgb);
                    visible.draw(chat_text);
                    break;

                case 1:
                    chat_text.setPosition(float(sX - 80 + 1), float(sY - 83 - iLoc));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 83 - iLoc + 1));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 83 - iLoc));
                    chat_text.setFillColor(rgb);
                    visible.draw(chat_text);
                    break;

                case 2:
                    chat_text.setPosition(float(sX - 80 + 1), float(sY - 101 - iLoc));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 101 - iLoc + 1));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 101 - iLoc));
                    chat_text.setFillColor(rgb);
                    visible.draw(chat_text);
                    break;

                case 3:
                    chat_text.setPosition(float(sX - 80 + 1), float(sY - 119 - iLoc));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 119 - iLoc + 1));
                    chat_text.setFillColor(Color(0, 0, 0));
                    visible.draw(chat_text);
                    chat_text.setPosition(float(sX - 80), float(sY - 119 - iLoc));
                    chat_text.setFillColor(rgb);
                    visible.draw(chat_text);
                    break;
            }

            break;
    }
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
    int iVol;
    float fPan = (float)lPan;
    float forward = 0;

    if (m_bSoundFlag == false) return;
    if (m_bSoundStat == false) return;

    if (iDist > 10) iDist = 10;

    iVol = (m_cSoundVolume - 100) * 20;
    iVol += -200 * iDist;

    if (iVol > 0) iVol = 0;
    if (iVol < -10000) iVol = -10000;

    if (fPan != 0)
    {
        fPan -= 0.5f;
        forward = -4;
    }
    if (fPan > 0) fPan -= 3.f;
    else if (fPan < 0) fPan += 3.f;

    if (fPan >= -3 && fPan <= 3)
        fPan = 0;

    if (iVol > -2000)
    {
        switch (cType)
        {
            case 'C':
                if (iNum >= MAX_CHARACTER_SOUNDS) return;
                m_pCSound[iNum].setMinDistance(4.f);
                m_pCSound[iNum].setAttenuation(1.f);
                m_pCSound[iNum].setPosition({ (float)fPan, 0.f, forward });
                m_pCSound[iNum].play();
                break;

            case 'M':
                if (iNum >= MAX_MONSTER_SOUNDS) return;
                m_pMSound[iNum].setMinDistance(4.f);
                m_pMSound[iNum].setAttenuation(1.f);
                m_pMSound[iNum].setPosition({ (float)fPan, 0.f, forward });
                m_pMSound[iNum].play();
                break;

            case 'E':
                if (iNum >= MAX_EFFECT_SOUNDS) return;
                m_pESound[iNum].setMinDistance(4.f);
                m_pESound[iNum].setAttenuation(1.f);
                m_pESound[iNum].setPosition({ (float)fPan, 0.f, forward });
                m_pESound[iNum].play();
                break;
        }
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
                if ((wWeaponType >= 35) && (wWeaponType < 40))
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

void CGame::draw_weather_effects()
{
    int i{};
    short dX{}, dY{}, sCnt{};
    char cTempFrame{};
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
    int i{};
    short sCnt{};
    char  cAdd{};
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
                        m_stWhetherObject[i].sX = (32) + ((rand() % 940) - 200) + 300;
                        m_stWhetherObject[i].sY = (32) + ((rand() % 800) - 600) + 240;
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
                        m_stWhetherObject[i].sX = (m_pMapData->m_sPivotX * 32) + ((rand() % 940) - 200) + 300;
                        m_stWhetherObject[i].sY = (m_pMapData->m_sPivotY * 32) + ((rand() % 800) - 600) + 600;
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

    //     if ((x0 == x1) && (y0 == y1))
    //         return;

    //     error = 0;

    //     iResultX = x0;
    //     iResultY = y0;

    //     dx = x1 - x0;
    //     dy = y1 - y0;

    //     if (dx >= 0)
    //     {
    //         x_inc = 1;
    //     }
    //     else
    //     {
    //         x_inc = -1;
    //         dx = -dx;
    //     }

    //     if (dy >= 0)
    //     {
    //         y_inc = 1;
    //     }
    //     else
    //     {
    //         y_inc = -1;
    //         dy = -dy;
    //     }

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
    //             if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480))
    //             {
    //                 pDst = (uint16_t *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
    //                 switch (m_DDraw.m_cPixelFormat)
    //                 {
    //                 case 1:
    //                     dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
    //                     dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
    //                     dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];

    //                     *pDst = (uint16_t)((dstR << 11) | (dstG << 5) | dstB);
    //                     break;

    //                 case 2:
    //                     dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
    //                     dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
    //                     dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];

    //                     *pDst = (uint16_t)((dstR << 10) | (dstG << 5) | dstB);
    //                     break;
    //                 }
    //             }
    //            
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
    //             if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480))
    //             {
    //                 pDst = (uint16_t *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
    //                 switch (m_DDraw.m_cPixelFormat)
    //                 {
    //                 case 1:
    //                     dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
    //                     dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
    //                     dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];

    //                     *pDst = (uint16_t)((dstR << 11) | (dstG << 5) | dstB);
    //                     break;

    //                 case 2:
    //                     dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
    //                     dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
    //                     dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];

    //                     *pDst = (uint16_t)((dstR << 10) | (dstG << 5) | dstB);
    //                     break;
    //                 }
    //             }
    //            
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

        cDir = m_Misc.cGetNextMoveDir(iX1, iY1, tX, tY);
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
        else return 1;
    }
    else if ((wWeaponType >= 1) && (wWeaponType <= 2))
    {

        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[7] >= 100)) return 21;
        else return 1;
    }
    else if ((wWeaponType > 2) && (wWeaponType < 20))
    {
        if (wWeaponType == 7)
        {

            if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[9] >= 100)) return 22;
            else return 1;
        }
        else
        {

            if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[8] >= 100)) return 23;
            else return 1;
        }
    }
    else if ((wWeaponType >= 20) && (wWeaponType < 30))
    {

        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[10] >= 100)) return 24;
        else return 1;
    }
    else if ((wWeaponType >= 30) && (wWeaponType < 35))
    {

        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[14] >= 100)) return 26;
        else return 1;
    }
    else if ((wWeaponType >= 35) && (wWeaponType < 40))
    {

        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[21] >= 100)) return 27;
        else return 1;
    }
    else if (wWeaponType >= 40)
    {

        if ((m_iSuperAttackLeft > 0) && (m_bSuperAttackMode == true) && (m_cSkillMastery[6] >= 100)) return 25;
        else return 2;
    }

    return 0;
}

int CGame::_iGetWeaponSkillType()
{
    uint16_t wWeaponType;


    wWeaponType = ((m_sPlayerAppr2 & 0x0FF0) >> 4);

    if (wWeaponType == 0)
    {

        return 5;
    }
    else if ((wWeaponType >= 1) && (wWeaponType <= 2))
    {

        return 7;
    }
    else if ((wWeaponType > 2) && (wWeaponType < 20))
    {
        if (wWeaponType == 7)
            return 9;
        else return 8;
    }
    else if ((wWeaponType >= 20) && (wWeaponType < 30))
    {

        return 10;
    }
    else if ((wWeaponType >= 30) && (wWeaponType < 35))
    {

        return 14;
    }
    else if ((wWeaponType >= 35) && (wWeaponType < 40))
    {

        return 21;
    }
    else if (wWeaponType >= 40)
    {

        return 6;
    }

    return 1;
}

void CGame::bItemDrop_ExchangeDialog(short msX, short msY)
{
    char cItemID;


    if (m_cCommand < 0) return;


    if (m_stDialogBoxInfo[27].sV1 != -1) return;

    cItemID = (char)m_stMCursor.sSelectedObjectID;


    if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (m_pItemList[cItemID]->m_dwCount > 1))
    {

        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;

        m_stDialogBoxInfo[17].sV3 = 1000;
        m_stDialogBoxInfo[17].sV4 = cItemID;

        m_stDialogBoxInfo[27].sView = cItemID;

        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);

        return;
    }
    else
    {

        m_stDialogBoxInfo[27].sV1 = m_pItemList[cItemID]->m_sSprite;
        m_stDialogBoxInfo[27].sV2 = m_pItemList[cItemID]->m_sSpriteFrame;
        m_stDialogBoxInfo[27].sV3 = 1;
        m_stDialogBoxInfo[27].sV4 = m_pItemList[cItemID]->m_cItemColor;
        m_stDialogBoxInfo[27].sView = cItemID;

        m_bIsItemDisabled[cItemID] = true;
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_SETEXCHANGEITEM, 0, cItemID, 1, 0, 0);
        return;
    }
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
            for (int i = 1; i < m_iItemDropCnt; i++)
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
    int i, iCount2, iMatch, iIndex, iItemIndex[7] = {};
    int iCount;
    int iItemCount[7] = {};
    char cTempName[21];
    bool bItemFlag[7] = {};

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

int CGame::_iGetFOE(int sStatus)
{
    bool bPK, bCitizen, bAresden, bHunter;

    if (m_iPKCount != 0) return -1;

    if (sStatus & 0x80000000) bPK = true;
    else bPK = false;
    if (sStatus & 0x40000000) bCitizen = true;
    else bCitizen = false;
    if (sStatus & 0x20000000) bAresden = true;
    else bAresden = false;
    if (sStatus & 0x10000000) bHunter = true;
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
    m_pMapData->GetOwnerStatusByObjectID(iOwnerH, &m_cIlusionOwnerType, &cDir, &m_sAppr1_IE, &m_sAppr2_IE, &m_sAppr3_IE, &m_sAppr4_IE, &m_sStatus_IE, &m_iApprColor_IE, m_cName_IE);
}

void CGame::CreateScreenShot()
{
    time_t t = time(0); // get time now
    struct tm * now = localtime(&t);

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

//void CGame::DrawVersion()
//{
//	PutString2( 15, 455, "X-Mas Version", 255, 255, 0 );
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
    else
    {
        strcpy(pName, pMapName);
        return -1;
    }
}

bool CGame::bCheckLocalChatCommand(char * pMsg)
{
    class  CStrTok * pStrTok = 0;
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
        case 30: strcpy(pName, NPC_NAME_LICHE); break;
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
                default:
                    strcpy(pName, NPC_NAME_CROP);
                    break;
            }
        }
        break;
        case 65: strcpy(pName, NPC_NAME_ICEGOLEM); break;
        case 66: strcpy(pName, NPC_NAME_WYVERN); break;
        case 67: strcpy(pName, NPC_NAME_MCGAFFIN); break;
        case 68: strcpy(pName, NPC_NAME_PERRY); break;
        case 69: strcpy(pName, NPC_NAME_DEVLIN); break;
    }
}

void CGame::GetItemName(CItem * pItem, char * pStr1, char * pStr2, char * pStr3, std::size_t len)
{
    int i{};
    char cTxt[256]{}, cTxt2[256]{}, cName[51]{};
    uint32_t dwType1{}, dwType2{}, dwValue1{}, dwValue2{}, dwValue3{};

    if (pItem == nullptr)
    {
        std::cout << "Item non-existant\n";
        return;
    }

    m_bIsSpecial = false;

    memset(cName, 0, sizeof(cName));
    memset(pStr1, 0, sizeof(pStr1));
    memset(pStr2, 0, sizeof(pStr2));
    memset(pStr3, 0, sizeof(pStr3));

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
            format_to_local(pStr1, len, "{}", cName);
#if DEF_LANGUAGE == 4	//¾ð¾î:English
        else format_to_local(pStr1, len, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, pItem->m_dwCount, cName);
#else
        else format_to_local(pStr1, len, DRAW_DIALOGBOX_SELLOR_REPAIR_ITEM1, cName, pItem->m_dwCount);
#endif
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
    int i{};
    char cTxt[256]{}, cTxt2[256]{}, cName[51]{};
    uint32_t dwType1{}, dwType2{}, dwValue1{}, dwValue2{}, dwValue3{};

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
        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_MAGIC, 0, indexX, indexY, m_iPointCommandType, 0);
    }
    else if ((m_iPointCommandType >= 0) && (m_iPointCommandType < 50))
    {

        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_REQ_USEITEM, 0, m_iPointCommandType, indexX, indexY, cTemp, cItemID);

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

void CGame::bItemDrop_Character()
{
    ItemEquipHandler((char)m_stMCursor.sSelectedObjectID);
}

void CGame::bItemDrop_Inventory(short msX, short msY)
{
    short sX, sY, dX, dY;
    char  cTxt[120];


    if (m_cCommand < 0) return;
    if (m_pItemList[m_stMCursor.sSelectedObjectID] == 0) return;

    if ((m_bSkillUsingStatus == true) && (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true))
    {
        AddEventList(BITEMDROP_INVENTORY1, 10);
        return;
    }


    if (m_bIsItemDisabled[m_stMCursor.sSelectedObjectID] == true) return;



    sY = m_stDialogBoxInfo[2].sY;
    sX = m_stDialogBoxInfo[2].sX;

    dX = msX - sX - 32 - m_stMCursor.sDistX;
    dY = msY - sY - 44 - m_stMCursor.sDistY;


    if (dY < -10) dY = -10;
    if (dX < 0)   dX = 0;
    if (dX > 170) dX = 170;
    if (dY > 95) dY = 95;

    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sX = dX;
    m_pItemList[m_stMCursor.sSelectedObjectID]->m_sY = dY;

    short sTmpSpr, sTmpSprFrm;
    sTmpSpr = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSprite;
    sTmpSprFrm = m_pItemList[m_stMCursor.sSelectedObjectID]->m_sSpriteFrame;
    /*

        if( m_bShiftPressed && (sTmpSpr == 6) && (sTmpSprFrm > 0) && (sTmpSprFrm <= 6) && (sTmpSprFrm != 5) )
        {
            for( int i=0 ; i<DEF_MAXITEMS ; i++ )
            {
                if( m_pItemList[i] == 0 ) continue;
                if( (m_pItemList[i]->m_sSprite == sTmpSpr) && (m_pItemList[i]->m_sSpriteFrame == sTmpSprFrm) )
                {
                    m_pItemList[i]->m_sX = dX;
                    m_pItemList[i]->m_sY = dY;
                    bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, i, dX, dY, 0, 0);
                }
            }
        }
        else bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, m_stMCursor.sSelectedObjectID, dX, dY, 0, 0);
    */


    char cItemID;
    if (m_bShiftPressed)
    {
        for (int i = 0; i < DEF_MAXITEMS; i++)
        {
            if (m_cItemOrder[DEF_MAXITEMS - 1 - i] != -1)
            {
                cItemID = m_cItemOrder[DEF_MAXITEMS - 1 - i];
                if (m_pItemList[cItemID] != 0 && memcmp(m_pItemList[cItemID]->m_cName, m_pItemList[m_stMCursor.sSelectedObjectID]->m_cName, 20) == 0)
                {
                    m_pItemList[cItemID]->m_sX = dX;
                    m_pItemList[cItemID]->m_sY = dY;
                    bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, cItemID, dX, dY, 0, 0);
                }
            }
        }
    }
    else
        bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, (char)(m_stMCursor.sSelectedObjectID), dX, dY, 0, 0);



    if (m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] == true)
    {

        char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
        GetItemName(m_pItemList[m_stMCursor.sSelectedObjectID], cStr1, cStr2, cStr3, 64);

        format_to_local(cTxt, ITEM_EQUIPMENT_RELEASED, cStr1);
        AddEventList(cTxt, 10);

        PlaySound('E', 29, 0);


        bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_RELEASEITEM, 0, m_stMCursor.sSelectedObjectID, 0, 0, 0);

        m_bIsItemEquipped[m_stMCursor.sSelectedObjectID] = false;
        m_sItemEquipmentStatus[m_pItemList[m_stMCursor.sSelectedObjectID]->m_cEquipPos] = -1;
    }
}

void CGame::bItemDrop_SellList(short msX, short msY)
{
    int i;
    char cItemID;

    cItemID = (char)m_stMCursor.sSelectedObjectID;

    if (m_pItemList[cItemID] == 0) return;
    if (m_bIsItemDisabled[cItemID] == true) return;

    if (m_cCommand < 0) return;


    for (i = 0; i < DEF_MAXSELLLIST; i++)
        if (m_stSellItemList[i].iIndex == cItemID)
        {
            AddEventList(BITEMDROP_SELLLIST1, 10);
            return;
        }
    if (strcmp(m_pItemList[cItemID]->m_cName, "Gold") == 0)
    {
        AddEventList(BITEMDROP_SELLLIST2, 10);
        return;
    }
    if (m_pItemList[cItemID]->m_wCurLifeSpan == 0)
    {
        memset(G_cTxt, 0, sizeof(G_cTxt));
        char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
        GetItemName(m_pItemList[cItemID], cStr1, cStr2, cStr3, 64);
        format_to_local(G_cTxt, NOTIFYMSG_CANNOT_SELL_ITEM2, cStr1);
        AddEventList(G_cTxt, 10);
        return;
    }


    if (((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_ARROW)) &&
        (m_pItemList[cItemID]->m_dwCount > 1))
    {

        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;

        m_stDialogBoxInfo[17].sV3 = 1001;
        m_stDialogBoxInfo[17].sV4 = cItemID;

        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, cItemID, m_pItemList[cItemID]->m_dwCount, 0);


        m_bIsItemDisabled[cItemID] = true;
    }
    else
    {

        for (i = 0; i < DEF_MAXSELLLIST; i++)
            if (m_stSellItemList[i].iIndex == -1)
            {
                m_stSellItemList[i].iIndex = cItemID;
                m_stSellItemList[i].iAmount = 1;
                m_bIsItemDisabled[cItemID] = true;
                return;
            }


        AddEventList(BITEMDROP_SELLLIST3, 10);
    }
}

void CGame::bItemDrop_ItemUpgrade()
{
    char cItemID;
    cItemID = (char)m_stMCursor.sSelectedObjectID;
    if (m_bIsItemDisabled[cItemID] == true) return;

    if (m_cCommand < 0) return;
    if (m_pItemList[cItemID]->m_cEquipPos == DEF_EQUIPPOS_NONE) return;

    switch (m_stDialogBoxInfo[34].cMode)
    {
        case 1:
            m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = false;
            m_stDialogBoxInfo[34].sV1 = cItemID;
            m_bIsItemDisabled[cItemID] = true;
            PlaySound('E', 29, 0);
            break;

        case 6:
            m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = false;
            m_stDialogBoxInfo[34].sV1 = cItemID;
            m_bIsItemDisabled[cItemID] = true;
            PlaySound('E', 29, 0);
            break;
    }
}

void CGame::bItemDrop_Bank(short msX, short msY)
{
    m_stDialogBoxInfo[39].sV1 = m_stMCursor.sSelectedObjectID;

    if (m_cCommand < 0) return;

    if (m_pItemList[m_stDialogBoxInfo[39].sV1] == 0) return;

    if (m_bIsItemDisabled[m_stDialogBoxInfo[39].sV1] == true) return;
    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }
    if ((m_bIsDialogEnabled[20] == true) && ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }
    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if (m_bIsDialogEnabled[4] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }


    if (((m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cItemType == DEF_ITEMTYPE_CONSUME) || (m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cItemType == DEF_ITEMTYPE_ARROW)) && (m_pItemList[m_stDialogBoxInfo[39].sV1]->m_dwCount > 1))
    {

        m_stDialogBoxInfo[17].sX = msX - 140;
        m_stDialogBoxInfo[17].sY = msY - 70;
        if (m_stDialogBoxInfo[17].sY < 0) m_stDialogBoxInfo[17].sY = 0;

        m_stDialogBoxInfo[17].sV1 = m_sPlayerX + 1;
        m_stDialogBoxInfo[17].sV2 = m_sPlayerY + 1;
        m_stDialogBoxInfo[17].sV3 = 1002;
        m_stDialogBoxInfo[17].sV4 = m_stDialogBoxInfo[39].sV1;

        memset(m_stDialogBoxInfo[17].cStr, 0, sizeof(m_stDialogBoxInfo[17].cStr));
        EnableDialogBox(17, m_stDialogBoxInfo[39].sV1, m_pItemList[m_stDialogBoxInfo[39].sV1]->m_dwCount, 0);
    }
    else
    {
        if (_iGetBankItemCount() >= (DEF_MAXBANKITEMS - 1)) AddEventList(DLGBOX_CLICK_NPCACTION_QUERY9, 10);
        else bSendCommand(MSGID_COMMAND_COMMON, DEF_COMMONTYPE_GIVEITEMTOCHAR, m_stDialogBoxInfo[39].sV1, 1, m_stDialogBoxInfo[39].sV5, m_stDialogBoxInfo[39].sV6, m_pItemList[m_stDialogBoxInfo[39].sV1]->m_cName, m_stDialogBoxInfo[39].sV4);
    }
}

void CGame::bItemDrop_SkillDialog()
{
    int iConsumeNum;
    char cItemID;


    if (m_cCommand < 0) return;


    cItemID = (char)m_stMCursor.sSelectedObjectID;

    if (m_pItemList[cItemID] == 0) return;


    if (m_bIsItemDisabled[cItemID] == true) return;


    if (m_bIsDialogEnabled[17] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if ((m_bIsDialogEnabled[20] == true) &&
        ((m_stDialogBoxInfo[20].cMode == 1) || (m_stDialogBoxInfo[20].cMode == 2)))
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    if (m_bIsDialogEnabled[23] == true)
    {
        AddEventList(BITEMDROP_SKILLDIALOG1, 10);
        return;
    }

    switch (m_stDialogBoxInfo[26].cMode)
    {
        case 1:
            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME)
            {
                iConsumeNum = 0;
                if (m_stDialogBoxInfo[26].sV1 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV2 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV3 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV4 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV5 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV6 == cItemID) iConsumeNum++;
                if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
            }

            if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_EAT) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_NONE))
            {

            }
            else return;


            if (m_stDialogBoxInfo[26].sV1 == -1)
            {
                m_stDialogBoxInfo[26].sV1 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV2 == -1)
            {
                m_stDialogBoxInfo[26].sV2 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV3 == -1)
            {
                m_stDialogBoxInfo[26].sV3 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV4 == -1)
            {
                m_stDialogBoxInfo[26].sV4 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV5 == -1)
            {
                m_stDialogBoxInfo[26].sV5 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV6 == -1)
            {
                m_stDialogBoxInfo[26].sV6 = cItemID;
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }

            AddEventList(BITEMDROP_SKILLDIALOG4, 10);
            break;

        case 4:

            if (m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME)
            {
                iConsumeNum = 0;
                if (m_stDialogBoxInfo[26].sV1 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV2 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV3 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV4 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV5 == cItemID) iConsumeNum++;
                if (m_stDialogBoxInfo[26].sV6 == cItemID) iConsumeNum++;
                if (iConsumeNum >= (int)(m_pItemList[cItemID]->m_dwCount)) return;
            }


            if (m_stDialogBoxInfo[26].sV1 == -1)
            {
                m_stDialogBoxInfo[26].sV1 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV2 == -1)
            {
                m_stDialogBoxInfo[26].sV2 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV3 == -1)
            {
                m_stDialogBoxInfo[26].sV3 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV4 == -1)
            {
                m_stDialogBoxInfo[26].sV4 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV5 == -1)
            {
                m_stDialogBoxInfo[26].sV5 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }
            else if (m_stDialogBoxInfo[26].sV6 == -1)
            {
                m_stDialogBoxInfo[26].sV6 = cItemID;

                m_stDialogBoxInfo[26].cStr[4] = (char)_bCheckCurrentBuildItemStatus();
                if ((m_pItemList[cItemID]->m_cItemType == DEF_ITEMTYPE_CONSUME) && (m_pItemList[cItemID]->m_dwCount > 1))
                {

                }
                else m_bIsItemDisabled[cItemID] = true;
                return;
            }

            AddEventList(BITEMDROP_SKILLDIALOG4, 10);
            break;

        default:
            break;
    }
}

void CGame::ShowEventList(uint32_t dwTime)
{
    int i;

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
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(225, 225, 225), false, 1);
                    break;
                case 1:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(130, 255, 130), false, 1);
                    break;
                case 2:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(255, 130, 130), false, 1);
                    break;
                case 3:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(130, 130, 255), false, 1);
                    break;
                case 4:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(230, 230, 130), false, 1);
                    break;
                case 10:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(180, 255, 180), false, 1);
                    break;
                case 20:
                    put_string(10, 322 + i * 15, m_stEventHistory2[i].cTxt, Color(150, 150, 170), false, 1);
                    break;
            }
        }

    if (m_bSkillUsingStatus == true)
    {
        put_string(280 - 29, 280 - 52, SHOW_EVENT_LIST1, Color(235, 235, 235), false, 1);

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



#if DEF_LANGUAGE == 3
    if (!m_bIsCrusadeMode)
    {
        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 180) && (m_sPlayerY == 206) ||
                (m_sPlayerX == 180) && (m_sPlayerY == 207) ||
                (m_sPlayerX == 181) && (m_sPlayerY == 207) ||
                (m_sPlayerX == 184) && (m_sPlayerY == 207) ||
                (m_sPlayerX == 185) && (m_sPlayerY == 206))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 167) && (m_sPlayerY == 136) ||
                (m_sPlayerX == 166) && (m_sPlayerY == 137) ||
                (m_sPlayerX == 167) && (m_sPlayerY == 137) ||
                (m_sPlayerX == 168) && (m_sPlayerY == 137))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 136) && (m_sPlayerY == 210) ||
                (m_sPlayerX == 137) && (m_sPlayerY == 211) ||
                (m_sPlayerX == 138) && (m_sPlayerY == 212) ||
                (m_sPlayerX == 142) && (m_sPlayerY == 212))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 271) && (m_sPlayerY == 172) ||
                (m_sPlayerX == 272) && (m_sPlayerY == 173) ||
                (m_sPlayerX == 273) && (m_sPlayerY == 174) ||
                (m_sPlayerX == 277) && (m_sPlayerY == 174))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 239) && (m_sPlayerY == 133) ||
                (m_sPlayerX == 240) && (m_sPlayerY == 133) ||
                (m_sPlayerX == 240) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 241) && (m_sPlayerY == 132))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 200) && (m_sPlayerY == 162) ||
                (m_sPlayerX == 200) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 199) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 198) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 189) && (m_sPlayerY == 169) ||
                (m_sPlayerX == 190) && (m_sPlayerY == 169) ||
                (m_sPlayerX == 191) && (m_sPlayerY == 169) ||
                (m_sPlayerX == 191) && (m_sPlayerY == 168))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 67) && (m_sPlayerY == 160) ||
                (m_sPlayerX == 68) && (m_sPlayerY == 160) ||
                (m_sPlayerX == 69) && (m_sPlayerY == 159))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "elvine") == 0 && strcmp(m_cMapName, "aresden") == 0)
        {
            if ((m_sPlayerX == 211) && (m_sPlayerY == 240) ||
                (m_sPlayerX == 211) && (m_sPlayerY == 241) ||
                (m_sPlayerX == 212) && (m_sPlayerY == 241) ||
                (m_sPlayerX == 223) && (m_sPlayerY == 234) ||
                (m_sPlayerX == 223) && (m_sPlayerY == 235) ||
                (m_sPlayerX == 222) && (m_sPlayerY == 235) ||
                (m_sPlayerX == 221) && (m_sPlayerY == 235))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }


        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 273) && (m_sPlayerY == 131) ||
                (m_sPlayerX == 273) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 274) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 277) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 278) && (m_sPlayerY == 131))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 101) && (m_sPlayerY == 176) ||
                (m_sPlayerX == 100) && (m_sPlayerY == 177) ||
                (m_sPlayerX == 101) && (m_sPlayerY == 177) ||
                (m_sPlayerX == 102) && (m_sPlayerY == 177))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 199) && (m_sPlayerY == 104) ||
                (m_sPlayerX == 198) && (m_sPlayerY == 104) ||
                (m_sPlayerX == 198) && (m_sPlayerY == 105) ||
                (m_sPlayerX == 197) && (m_sPlayerY == 105))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 257) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 258) && (m_sPlayerY == 164) ||
                (m_sPlayerX == 259) && (m_sPlayerY == 165) ||
                (m_sPlayerX == 263) && (m_sPlayerY == 165))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 249) && (m_sPlayerY == 250) ||
                (m_sPlayerX == 250) && (m_sPlayerY == 251) ||
                (m_sPlayerX == 251) && (m_sPlayerY == 252) ||
                (m_sPlayerX == 255) && (m_sPlayerY == 252))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 163) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 162) && (m_sPlayerY == 132) ||
                (m_sPlayerX == 162) && (m_sPlayerY == 133) ||
                (m_sPlayerX == 161) && (m_sPlayerY == 133))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 182) && (m_sPlayerY == 178) ||
                (m_sPlayerX == 181) && (m_sPlayerY == 178) ||
                (m_sPlayerX == 181) && (m_sPlayerY == 179) ||
                (m_sPlayerX == 180) && (m_sPlayerY == 179) ||
                (m_sPlayerX == 173) && (m_sPlayerY == 184) ||
                (m_sPlayerX == 173) && (m_sPlayerY == 185) ||
                (m_sPlayerX == 172) && (m_sPlayerY == 185) ||
                (m_sPlayerX == 171) && (m_sPlayerY == 185))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 98) && (m_sPlayerY == 108) ||
                (m_sPlayerX == 99) && (m_sPlayerY == 108) ||
                (m_sPlayerX == 100) && (m_sPlayerY == 107))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }

        if (strcmp(m_cLocation, "aresden") == 0 && strcmp(m_cMapName, "elvine") == 0)
        {
            if ((m_sPlayerX == 313) && (m_sPlayerY == 168) ||
                (m_sPlayerX == 313) && (m_sPlayerY == 169) ||
                (m_sPlayerX == 314) && (m_sPlayerY == 169) ||
                (m_sPlayerX == 325) && (m_sPlayerY == 162) ||
                (m_sPlayerX == 325) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 324) && (m_sPlayerY == 163) ||
                (m_sPlayerX == 323) && (m_sPlayerY == 163))
            {
                AddEventList(REQUEST_TELEPORT_AND_WAIT_DATA2, 10);
                return;
            }
        }
    }
#endif	


    bSendCommand(MSGID_REQUEST_TELEPORT, 0, 0, 0, 0, 0, 0);
    change_game_mode(DEF_GAMEMODE_ONWAITINGINITDATA);
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
            if (m_sShortCut[num] < 50/*100*/)
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
    if (iManaCost < 1) iManaCost = 1;
    return iManaCost;
}

void CGame::UseMagic(int iMagicNo)
{
    if (iMagicNo < 0 || iMagicNo >= 100) return;
    if ((m_cMagicMastery[iMagicNo] == 0) || (m_pMagicCfgList[iMagicNo] == 0)) return;


    if (m_iHP <= 0) return;

    if (m_bIsGetPointingMode == true) return;

    if (iGetManaCost(iMagicNo) > m_iMP) return;

    if (_bIsItemOnHand() == true)
    {
        AddEventList(DLGBOX_CLICK_MAGIC1, 10);
        return;
    }

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
    //m_bIsGetPointingMode = true;

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
