//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <fmt/format.h>
#include "sprite.h"
#include "sprite_id.h"
#include "CharInfo.h"
#include "MouseInterface.h"
#include "action_id.h"
#include "Item.h"
#include "MapData.h"
#include "lan_eng.h"
#include "msg.h"
#include "Magic.h"
#include "Effect.h"

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_iStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern uint16_t  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern uint16_t  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int iFocusStatus;
extern int   iFocusApprColor;

bool CGame::GameRecvMsgHandler(char * pData, uint32_t dwMsgSize)
{
    uint32_t * dwpMsgID;
    uint32_t dwTimeSent, dwTimeRcv, * dwp;
    uint16_t * wp;
    char * cp, cName[11];
    int i, * ip, itmp, itmp2, itmp3;//, iTotalFriends;

    dwpMsgID = (uint32_t *)(pData + DEF_INDEX4_MSGID);

    if (*dwpMsgID == MSGID_EVENT_MOTION)
    {
        MotionEventHandler(pData);
        return true;
    }

    uint8_t v = 0;

    stream_read sr(pData, dwMsgSize);
    int32_t message_id = sr.read_int32();
    int16_t command_id = sr.read_int16();

    switch (*dwpMsgID)
    {
        case MSGID_COMMAND_CHECKCONNECTION:
            return true;

        case MSGID_RESPONSE_PING:
            // round trip ping
            m_iPing = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - sr.read_int64()) / 2;
            // server->client ping
            //sr.read_int64();
            return true;

        case MSGID_RESPONSE_FRIENDSLIST:

            memset(cName, 0, 11);
            wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);

            switch (*wp)
            {
                case 1://Add
                    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
                    //cp++;

                    memcpy(m_stFriendsList[m_iTotalFriends].cCharName, cp, 10);
                    cp += 10;

                    memcpy(m_stFriendsList[m_iTotalFriends].cMapName, cp, 10);
                    cp += 10;

                    m_stFriendsList[m_iTotalFriends].cSide = *cp;
                    cp++;

                    ip = (int *)cp;
                    m_stFriendsList[m_iTotalFriends].iLevel = *ip;
                    cp += 4;

                    ip = (int *)cp;
                    m_stFriendsList[m_iTotalFriends].iPKs = *ip;
                    cp += 4;

                    //				StringCbPrintf(G_cTxt, 128, "Friend {} has been added to your list.", m_stFriendsList[m_iTotalFriends].cCharName);
                    format_to_local(G_cTxt, "Friend {} has been added to your list.", m_stFriendsList[m_iTotalFriends].cCharName);
                    AddEventList(G_cTxt, 10);

                    m_stFriendsList[m_iTotalFriends].bIsOnline = true;
                    m_iTotalFriends++;

                    break;
                case 2://Delete
                    break;
                case 3://Update

                    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

                    m_iTotalFriends = *cp;
                    cp++;

                    itmp = itmp2 = itmp3 = 0;

                    memset(m_stFriendsList, 0, sizeof(m_stFriendsList));

                    for (i = 0; i < m_iTotalFriends; i++)
                    {
                        if (*cp == 1)
                        {
                            cp++;
                            memcpy(m_stFriendsList[i].cCharName, cp, 10);
                            cp += 10;
                            memcpy(m_stFriendsList[i].cMapName, cp, 10);
                            cp += 10;

                            m_stFriendsList[i].cSide = *cp;
                            cp++;

                            ip = (int *)cp;
                            m_stFriendsList[i].iLevel = *ip;
                            cp += 4;

                            ip = (int *)cp;
                            m_stFriendsList[i].iPKs = *ip;
                            cp += 4;

                            m_stFriendsList[i].bIsOnline = true;
                        }
                        else
                        {
                            cp++;
                            memcpy(m_stFriendsList[i].cCharName, cp, 10);
                            cp += 10;
                            m_stFriendsList[i].bIsOnline = false;
                        }
                    }

                    break;
                case 4://Come Online

                    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

                    memcpy(cName, cp, 10);
                    cp += 10;
                    for (i = 0; i < 50; i++)
                    {
                        if (memcmp(m_stFriendsList[i].cCharName, cName, 10) == 0)
                        {
                            memcpy(m_stFriendsList[i].cMapName, cp, 10);
                            cp += 10;

                            m_stFriendsList[i].cSide = *cp;
                            cp++;

                            ip = (int *)cp;
                            m_stFriendsList[i].iLevel = *ip;
                            cp += 4;

                            ip = (int *)cp;
                            m_stFriendsList[i].iPKs = *ip;
                            cp += 4;

                            m_stFriendsList[i].bIsOnline = true;
                            format_to_local(G_cTxt, "Friend {} has come online.", cName);
                            //						StringCbPrintf(G_cTxt, 128, "Friend {} has come online.", cName);
                            AddEventList(G_cTxt, 10);
                            break;
                        }
                    }
                    break;
                case 5://Go Offline
                    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

                    for (i = 0; i < m_iTotalFriends; i++)
                    {
                        if (memcmp(m_stFriendsList[i].cCharName, cp, 10) == 0)
                        {
                            m_stFriendsList[i].bIsOnline = false;
                            format_to_local(G_cTxt, "Friend {} is now offline.", cp);
                            //						StringCbPrintf(G_cTxt, 128, "Friend {} is now offline.", cp);
                            AddEventList(G_cTxt, 10);
                        }
                    }
                    break;

                case 6://Single Update

                    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

                    for (i = 0; i < m_iTotalFriends; i++)
                    {
                        if (memcmp(m_stFriendsList[i].cCharName, cp, 10) == 0)
                        {
                            cp += 10;

                            memcpy(m_stFriendsList[i].cMapName, cp, 10);
                            cp += 10;
                            if (strlen(m_stFriendsList[i].cMapName) > 0)
                                m_stFriendsList[i].bIsOnline = true;
                            else
                                m_stFriendsList[i].bIsOnline = false;

                            m_stFriendsList[i].cSide = *cp;
                            cp++;

                            ip = (int *)cp;
                            m_stFriendsList[i].iLevel = *ip;
                            cp += 4;

                            ip = (int *)cp;
                            m_stFriendsList[i].iPKs = *ip;
                            cp += 4;
                        }
                    }

                    return true;
            }



            //struct {
            //	char  cCharName[11];
            //	char  cMapName[10];
            //	int   iLevel, iPKs;
            //	bool  bIsOnline;
            //} m_stFriendsList[50];


            return true;

            //case MSGID_RESPONSE_PKLIST:

            //	cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

            //	ip = (int *)cp;
            //	itmp = *ip;

            //	cp+=4;

            //	memset(m_stPKList, 0, sizeof(m_stPKList));

            //	for (i = 0; i < itmp; i++)
            //	{
            //		memcpy(m_stPKList[i].m_cCharName, cp, 10);
            //		cp+=10;

            //		ip = (int *)cp;
            //		m_stPKList[i].m_iPKs = *ip;
            //		cp+=4;

            //		ip = (int *)cp;
            //		m_stPKList[i].m_iGM = *ip;
            //		cp+=4;
            //	}
                //format_to_local(G_cTxt, "PKers found: {} - name {} pks {}", itmp, m_stPKList[0].m_cCharName, m_stPKList[0].m_iPKs);
                //AddEventList(G_cTxt, 10);

            break;

        case MSGID_RESPONSE_REGISTER:

            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

            itmp = *cp;
            cp++;

            if (itmp != 1)
                ChangeGameMode(DEF_GAMEMODE_ONVERSIONNOTMATCH);

            return true;

#ifdef DEF_ADMINCLIENT
        case MSGID_RESPONSE_CHARLIST:

            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

            ip = (int *)cp;
            m_iTotalCharList = *ip;

            cp += 4;

            memset(m_stCharList, 0, sizeof(m_stCharList));

            for (i = 0; i < m_iTotalCharList; i++)
            {
                memcpy(m_stCharList[i].cCharName, cp, 10);
                cp += 10;
                memcpy(m_stCharList[i].cTown, cp, 10);
                cp += 10;
                memcpy(m_stCharList[i].cMapName, cp, 10);
                cp += 10;
                memcpy(m_stCharList[i].cAddress, cp, 20);
                cp += 20;

                ip = (int *)cp;
                m_stCharList[i].iX = *ip;
                cp += 4;

                ip = (int *)cp;
                m_stCharList[i].iY = *ip;
                cp += 4;

                ip = (int *)cp;
                m_stCharList[i].iLevel = *ip;
                cp += 4;

                ip = (int *)cp;
                m_stCharList[i].iPKs = *ip;
                cp += 4;

                m_stCharList[i].bIsGM = *cp;
                cp++;
            }

            ip = (int *)cp;
            m_iTotalMapList = *ip;

            cp += 4;

            memset(m_stMapList, 0, sizeof(m_stMapList));

            for (i = 0; i < m_iTotalMapList; i++)
            {
                memcpy(m_stMapList[i].cMapName, cp, 10);
                cp += 10;

                ip = (int *)cp;
                m_stMapList[i].iClients = *ip;
                cp += 4;

                ip = (int *)cp;
                m_stMapList[i].iMobs = *ip;
                cp += 4;

                ip = (int *)cp;
                m_stMapList[i].iItems = *ip;
                cp += 4;
            }

            return true;
#endif
        case MSGID_RESPONSE_FLOORSTATS:
            return true;

        case MSGID_RESPONSE_CHARGED_TELEPORT:
            ResponseChargedTeleport(pData);
            return true;

        case MSGID_RESPONSE_TELEPORT_LIST:
            ResponseTeleportList(pData);
            return true;

        case MSGID_RESPONSE_NOTICEMENT:
            NoticementHandler(pData);
            return true;

        case MSGID_DYNAMICOBJECT:
            DynamicObjectHandler(pData);
            return true;

        case MSGID_RESPONSE_INITDATA:
            InitDataResponseHandler(pData);
            bSendCommand(MSGID_REQUEST_REGISTER, 0, 0, 0, 0, 0, 0);
            return true;

        case MSGID_RESPONSE_MOTION:
            MotionResponseHandler(pData);
            return true;

        case MSGID_EVENT_COMMON:
            CommonEventHandler(pData);
            return true;

        case MSGID_EVENT_MOTION:
            MotionEventHandler(pData);
            return true;

        case MSGID_EVENT_LOG:
            LogEventHandler(pData);
            return true;

        case MSGID_COMMAND_CHATMSG:
            ChatMsgHandler(pData, dwMsgSize);
            return true;

        case MSGID_PLAYERITEMLISTCONTENTS:
            InitItemList(pData);
            return true;

        case MSGID_NOTIFY:
            NotifyMsgHandler(pData);
            return true;

        case MSGID_GMLIST:
            memset(m_cGMList, 0, 200);
            memcpy(m_cGMList, pData + 6, dwMsgSize - 6);
            return true;

        case MSGID_RESPONSE_CREATENEWGUILD:
            CreateNewGuildResponseHandler(pData);
            return true;

        case MSGID_RESPONSE_DISBANDGUILD:
            DisbandGuildResponseHandler(pData);
            return true;

        case MSGID_PLAYERCHARACTERCONTENTS:
            InitPlayerCharacteristics(pData);
            return true;

        case MSGID_RESPONSE_CIVILRIGHT:
            CivilRightAdmissionHandler(pData);
            return true;

        case MSGID_RESPONSE_RETRIEVEITEM:
            RetrieveItemHandler(pData);
            return true;

        case MSGID_RESPONSE_PANNING:
            ResponsePanningHandler(pData);
            return true;

        case MSGID_RESPONSE_FIGHTZONE_RESERVE:
            ReserveFightzoneResponseHandler(pData);
            return true;
    }
    return false;
}


void CGame::CommonEventHandler(char * pData)
{
    uint16_t * wp, wEventType;
    short * sp, sX, sY, sV1, sV2, sV3, sV4;
    char * cp;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wEventType = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    sp = (short *)cp;
    sV1 = *sp;
    cp += 2;

    sp = (short *)cp;
    sV2 = *sp;
    cp += 2;

    sp = (short *)cp;
    sV3 = *sp;
    cp += 2;

    sp = (short *)cp;
    sV4 = *sp;
    cp += 2;

    switch (wEventType)
    {
        case DEF_COMMONTYPE_ITEMDROP:
            if ((sV1 == 6) && (sV2 == 0))
            {
                bAddNewEffect(4, sX, sY, 0, 0, 0);
            }
            //else

            m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3);
            break;

        case DEF_COMMONTYPE_SETITEM:
            m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3, false);
            break;

        case DEF_COMMONTYPE_MAGIC:
            bAddNewEffect(sV3, sX, sY, sV1, sV2, 0, sV4);
            break;
        case DEF_COMMONTYPE_CLEARGUILDNAME:
            ClearGuildNameList();
            break;
    }
}

void CGame::InitPlayerCharacteristics(char * pData)
{
    int * ip;
    char * cp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    ip = (int *)cp;
    m_iHP = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iMP = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iSP = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iAC = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iTHAC0 = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iLevel = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iStr = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iInt = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iVit = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iDex = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iMag = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iCharisma = *ip;
    cp += 4;

    /*m_cLU_Str = *cp;
    cp++;
    m_cLU_Vit = *cp;
    cp++;
    m_cLU_Dex = *cp;
    cp++;
    m_cLU_Int = *cp;
    cp++;
    m_cLU_Mag = *cp;
    cp++;
    m_cLU_Char = *cp;
    cp++;*/

    //m_iLU_Point = 3 - (m_cLU_Str + m_cLU_Vit + m_cLU_Dex + m_cLU_Int + m_cLU_Mag + m_cLU_Char);
    m_iLU_Point = *cp;
    cp += (5 + 2);

    ip = (int *)cp;
    m_iExp = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iEnemyKillCount = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iPKCount = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iRewardGold = *ip;
    cp += 4;

    memcpy(m_cLocation, cp, 10);
    cp += 10;
    if (memcmp(m_cLocation, "aresden", 7) == 0)
    {
        m_bAresden = true;
        m_bCitizen = true;
        m_bHunter = false;
    }
    else if (memcmp(m_cLocation, "arehunter", 9) == 0)
    {
        m_bAresden = true;
        m_bCitizen = true;
        m_bHunter = true;
    }
    else if (memcmp(m_cLocation, "elvine", 6) == 0)
    {
        m_bAresden = false;
        m_bCitizen = true;
        m_bHunter = false;
    }
    else if (memcmp(m_cLocation, "elvhunter", 9) == 0)
    {
        m_bAresden = false;
        m_bCitizen = true;
        m_bHunter = true;
    }
    else
    {
        m_bAresden = true;
        m_bCitizen = false;
        m_bHunter = true;
    }

    cp = (char *)cp;
    memcpy(m_cGuildName, cp, 20);
    cp += 20;

    if (strcmp(m_cGuildName, "NONE") == 0)
        memset(m_cGuildName, 0, sizeof(m_cGuildName));

    m_Misc.ReplaceString(m_cGuildName, '_', ' ');

    ip = (int *)cp;
    m_iGuildRank = *ip;
    cp += 4;

    m_iSuperAttackLeft = (int)*cp;
    cp++;

    ip = (int *)cp;
    m_iFightzoneNumber = *ip;
    cp += 4;

    bSendCommand(MSGID_REQUEST_FRIENDSLIST, 0, 3, 0, 0, 0, 0);
}

void CGame::CreateNewGuildResponseHandler(char * pData)
{
    uint16_t * wpResult;

    wpResult = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    switch (*wpResult)
    {
        case DEF_MSGTYPE_CONFIRM:
            m_iGuildRank = 0;
            m_stDialogBoxInfo[7].cMode = 3;
            break;

        case DEF_MSGTYPE_REJECT:
            m_iGuildRank = -1;
            m_stDialogBoxInfo[7].cMode = 4;
            break;
    }
}

void CGame::DisbandGuildResponseHandler(char * pData)
{
    uint16_t * wpResult;

    wpResult = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    switch (*wpResult)
    {
        case DEF_MSGTYPE_CONFIRM:
            memset(m_cGuildName, 0, sizeof(m_cGuildName));
            m_iGuildRank = -1;
            m_stDialogBoxInfo[7].cMode = 7;
            break;

        case DEF_MSGTYPE_REJECT:
            m_stDialogBoxInfo[7].cMode = 8;
            break;
    }
}
void CGame::LogEventHandler(char * pData)
{
    uint16_t * wp, wEventType, wObjectID;
    short * sp, sX, sY, sType, sAppr1 = 0, sAppr2 = 0, sAppr3 = 0, sAppr4 = 0;
    int iStatus;
    char * cp, cDir, cName[12];
    int * ip, iApprColor = 0;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wEventType = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    wp = (uint16_t *)cp;
    wObjectID = *wp;
    cp += 2;

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    sp = (short *)cp;
    sType = *sp;
    cp += 2;

    cDir = *cp;
    cp++;

    memset(cName, 0, sizeof(cName));
    if (wObjectID < 10000)
    {
        memcpy(cName, cp, 10);
        cp += 10;

        sp = (short *)cp;
        sAppr1 = *sp;
        cp += 2;

        sp = (short *)cp;
        sAppr2 = *sp;
        cp += 2;

        sp = (short *)cp;
        sAppr3 = *sp;
        cp += 2;

        sp = (short *)cp;
        sAppr4 = *sp;
        cp += 2;

        ip = (int *)cp;
        iApprColor = *ip;
        cp += 4;

        ip = (int *)cp;
        iStatus = *ip;
        cp += 4;

        m_stNPCList[wObjectID].m_iMaxHP = 0;

        m_stNPCList[wObjectID].m_iHP = 0;
        //Change HP Bar
    }
    else
    {
        memcpy(cName, cp, 5);
        cp += 5;

        sAppr1 = sAppr3 = sAppr4 = 0;

        sp = (short *)cp;
        sAppr2 = *sp;
        cp += 2;

        ip = (int *)cp;
        iStatus = *ip;
        cp += 4;

        m_stNPCList[wObjectID].m_iMaxHP = 0;

        m_stNPCList[wObjectID].m_iHP = 0;
        //Change HP Bar
    }

    switch (wEventType)
    {
        case DEF_MSGTYPE_CONFIRM:
            m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, DEF_OBJECTSTOP, 0, 0, 0);
            switch (sType)
            {
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                    bAddNewEffect(64, (sX) * 32, (sY) * 32, 0, 0, 0);
                    break;
            }
            break;

        case DEF_MSGTYPE_REJECT:
            m_pMapData->bSetOwner(wObjectID, -1, -1, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, DEF_OBJECTSTOP, 0, 0, 0);
            break;
    }
    _RemoveChatMsgListByObjectID(wObjectID);
}
void CGame::LogResponseHandler(char * pData, uint64_t size)
{
    stream_read sr(pData, size);
    uint16_t * wp, wResponse;
    uint16_t wServerUpperVersion, wServerLowerVersion, wServerPatchVersion;
    uint32_t * dwp;
    char * cp, cCharName[12];
    int * ip, i;

    dwp = (uint32_t *)(pData);
    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wResponse = *wp;
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    uint32_t unused = sr.read_int32();
    wResponse = sr.read_uint16();
    if (GameRecvMsgHandler(pData, size)) return;

    switch (wResponse)
    {
        case DEF_LOGRESMSGTYPE_CHARACTERDELETED:
            cp = (pData + DEF_INDEX2_MSGTYPE + 2);

            //m_iAccountStatus = (int)*cp;
            cp++;

            m_iTotalChar = (int)*cp;
            cp++;

            for (i = 0; i < 4; i++)
                if (m_pCharList[i] != 0)
                {
                    delete m_pCharList[i];
                    m_pCharList[i] = 0;
                }

            for (i = 0; i < m_iTotalChar; i++)
            {
                m_pCharList[i] = new CCharInfo;
                memcpy(m_pCharList[i]->m_cName, cp, 10);
                cp += 10;
                if (*cp == 0)
                {
                    m_pCharList[i]->m_sSex = 0;
                    cp += 40;
                }
                else
                {
                    cp++;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr1 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr2 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr3 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr4 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSkinCol = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sLevel = *wp;
                    cp += 2;

                    dwp = (uint32_t *)cp;
                    m_pCharList[i]->m_iExp = *dwp;
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sStr = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sVit = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sDex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sInt = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sMag = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sChr = *wp;
                    cp += 2;

                    ip = (int *)cp; // v1.4
                    m_pCharList[i]->m_iApprColor = *ip;
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iYear = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMonth = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iDay = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iHour = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMinute = (int)*wp;
                    cp += 2;

                    memset(m_pCharList[i]->m_cMapName, 0, sizeof(m_pCharList[i]->m_cMapName));
                    memcpy(m_pCharList[i]->m_cMapName, cp, 10);
                    cp += 10;
                }
            }
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "3A");
            break;

        case DEF_LOGRESMSGTYPE_CONFIRM:
            loggedin = true;
            cp = (pData + DEF_INDEX2_MSGTYPE + 2);
            wp = (uint16_t *)cp;
            wServerUpperVersion = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            wServerLowerVersion = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            wServerPatchVersion = *wp;
            cp += 2;
            ////////////////////////////

    //		m_iAccountStatus = (int)*cp;
            cp++;
            wp = (uint16_t *)cp;
            m_iAccntYear = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            m_iAccntMonth = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            m_iAccntDay = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            m_iIpYear = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            m_iIpMonth = *wp;
            cp += 2;

            wp = (uint16_t *)cp;
            m_iIpDay = *wp;
            cp += 2;

            m_iTotalChar = (int)*cp;
            cp++;

            for (i = 0; i < 4; i++)
                if (m_pCharList[i] != 0)
                {
                    delete m_pCharList[i];
                    m_pCharList[i] = 0;
                }

            for (i = 0; i < m_iTotalChar; i++)
            {
                m_pCharList[i] = new CCharInfo;
                memcpy(m_pCharList[i]->m_cName, cp, 10);
                cp += 10;
                if (*cp == 0)
                {
                    m_pCharList[i]->m_sSex = 0;
                    cp += 40;
                }
                else
                {
                    cp++;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr1 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr2 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr3 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr4 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSkinCol = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sLevel = *wp;
                    cp += 2;

                    dwp = (uint32_t *)cp;
                    m_pCharList[i]->m_iExp = *dwp;
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sStr = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sVit = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sDex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sInt = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sMag = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sChr = *wp;
                    cp += 2;

                    ip = (int *)cp;
                    m_pCharList[i]->m_iApprColor = *ip; // v1.4
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iYear = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMonth = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iDay = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iHour = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMinute = (int)*wp;
                    cp += 2;

                    memset(m_pCharList[i]->m_cMapName, 0, sizeof(m_pCharList[i]->m_cMapName));
                    memcpy(m_pCharList[i]->m_cMapName, cp, 10);
                    cp += 10;
                }
            }
            ip = (int *)cp;
            m_iTimeLeftSecAccount = *ip;
            cp += 4;

            ip = (int *)cp;
            m_iTimeLeftSecIP = *ip;
            cp += 4;

            ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
            ClearContents_OnSelectCharacter();

            //m_iUpperVersion = wServerUpperVersion;//Changed added -- Always will connect no matter the version
            //m_iLowerVersion = wServerLowerVersion;

            //if (m_iUpperVersion != (3 || 2)) m_iUpperVersion = 2;
            if ((wServerUpperVersion != DEF_UPPERVERSION) || (wServerLowerVersion != DEF_LOWERVERSION) || (wServerPatchVersion != DEF_PATCHVERSION)) ChangeGameMode(DEF_GAMEMODE_ONVERSIONNOTMATCH);

            break;

        case DEF_LOGRESMSGTYPE_REJECT:
            cp = (pData + DEF_INDEX2_MSGTYPE + 2);
            ip = (int *)cp;
            m_iBlockYear = *ip;
            cp += 4;

            ip = (int *)cp;
            m_iBlockMonth = *ip;
            cp += 4;

            ip = (int *)cp;
            m_iBlockDay = *ip;
            cp += 4;

            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "7H");
            break;
        case DEF_LOGRESMSGTYPE_ACCOUNTLOCKED:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "7K");
            break;
        case DEF_LOGRESMSGTYPE_SERVICENOTAVAILABLE:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "7L");
            break;

        case DEF_LOGRESMSGTYPE_PASSWORDCHANGESUCCESS:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "6B");
            break;

        case DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "6C");
            break;

        case DEF_LOGRESMSGTYPE_PASSWORDMISMATCH:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "11");
            break;

        case DEF_LOGRESMSGTYPE_NOTEXISTINGACCOUNT:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "12");
            break;

        case DEF_LOGRESMSGTYPE_NEWACCOUNTCREATED:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "54");
            break;

        case DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "05");
            break;

        case DEF_LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "06");
            break;

        case DEF_LOGRESMSGTYPE_NOTEXISTINGCHARACTER:
            ChangeGameMode(DEF_GAMEMODE_ONMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "Not existing character!");
            break;

        case DEF_LOGRESMSGTYPE_NEWCHARACTERCREATED:
            memset(cCharName, 0, sizeof(cCharName));
            memcpy(cCharName, cp, 10);
            cp += 10;

            m_iTotalChar = (int)*cp;
            cp++;

            for (i = 0; i < 4; i++)
                if (m_pCharList[i] != 0) delete m_pCharList[i];
            //
            for (i = 0; i < m_iTotalChar; i++)
            {
                m_pCharList[i] = new CCharInfo;
                memcpy(m_pCharList[i]->m_cName, cp, 10);
                cp += 10;
                if (*cp == 0)
                {
                    m_pCharList[i]->m_sSex = 0;
                    cp += 40;
                }
                else
                {
                    cp++;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr1 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr2 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr3 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sAppr4 = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sSkinCol = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sLevel = *wp;
                    cp += 2;

                    dwp = (uint32_t *)cp;
                    m_pCharList[i]->m_iExp = *dwp;
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sStr = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sVit = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sDex = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sInt = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sMag = *wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_sChr = *wp;
                    cp += 2;

                    ip = (int *)cp; // v1.4
                    m_pCharList[i]->m_iApprColor = *ip;
                    cp += 4;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iYear = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMonth = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iDay = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iHour = (int)*wp;
                    cp += 2;

                    wp = (uint16_t *)cp;
                    m_pCharList[i]->m_iMinute = (int)*wp;
                    cp += 2;

                    memset(m_pCharList[i]->m_cMapName, 0, sizeof(m_pCharList[i]->m_cMapName));
                    memcpy(m_pCharList[i]->m_cMapName, cp, 10);
                    cp += 10;
                }
            }
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "47");
            break;

        case DEF_LOGRESMSGTYPE_NEWCHARACTERFAILED:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "28");
            break;

        case DEF_LOGRESMSGTYPE_ALREADYEXISTINGCHARACTER:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "29");
            break;

        case DEF_ENTERGAMERESTYPE_PLAYING:
            ChangeGameMode(DEF_GAMEMODE_ONQUERYFORCELOGIN);
            break;

        case DEF_ENTERGAMERESTYPE_CONFIRM:
        {
            socket_mode(true);
            ConnectionEstablishHandler(DEF_SERVERTYPE_GAME);

            m_bIllusionMVT = false;
            m_iIlusionOwnerH = 0;
            m_cIlusionOwnerType = 0;
        }
        break;

        case DEF_ENTERGAMERESTYPE_REJECT:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
            switch (*cp)
            {
                case 1:	strcpy(m_cMsg, "3E"); break;
                case 2:	strcpy(m_cMsg, "3F"); break;
                case 3:	strcpy(m_cMsg, "33"); break;
                case 4: strcpy(m_cMsg, "3D"); break;
                case 5: strcpy(m_cMsg, "3G"); break;
                case 6: strcpy(m_cMsg, "3Z"); break;
                case 7: strcpy(m_cMsg, "3J"); break;
            }
            break;

        case DEF_ENTERGAMERESTYPE_FORCEDISCONN:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "3X");
            break;

        case DEF_LOGRESMSGTYPE_NOTEXISTINGWORLDSERVER:
            ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
            memset(m_cMsg, 0, sizeof(m_cMsg));
            strcpy(m_cMsg, "1Y");
            break;
    }
}

void CGame::ChatMsgHandler(char * pData, uint32_t size)
{
    int i, iObjectID, iLoc, iGM = 0;
    short sX, sY;
    char * cp, cMsgType, cName[21], cTemp[100], cMsg[100];
    uint32_t dwTime;
    bool bFlag;

    char cHeadMsg[200];

    dwTime = m_dwCurTime;

    stream_read sr(pData, size);

    sr.read_uint32();
    sr.read_uint16();
    iObjectID = sr.read_uint16();
    sX = sr.read_int16();
    sY = sr.read_int16();
    memset(cName, 0, sizeof(cName));
    sr.read_bytes(cName, 10);
    cMsgType = sr.read_byte();
    std::string msg = sr.read_string();

    if (bCheckExID(cName) == true) return;

    if ((cMsgType == 0) || (cMsgType == 2) || (cMsgType == 3))
    {
        //if (m_Misc.bCheckIMEString(cTemp) == false) return;
    }
    if (!m_bWhisper)
    {
        if (cMsgType == 20) return;
    }
    if (!m_bShout)
    {
        if (cMsgType == 2 || cMsgType == 3) return;
    }
    if (!m_stConfigList.bGMChat)
        if (cMsgType == 4) return;

    memset(cMsg, 0, sizeof(cMsg));
    format_to_local(cMsg, "{}: {}", cName, msg);

    bFlag = false;
    short sCheckByte = 0;

    while (bFlag == false)
    {
        iLoc = m_Misc.iGetTextLengthLoc(chat_window_text, cMsg, 305);
        for (int i = 0; i < iLoc; i++) if (cMsg[i] < 0) sCheckByte++;
        if (iLoc == 0)
        {
            PutChatScrollList(cMsg, cMsgType, iGM);
            bFlag = true;
        }
        else
        {
            if ((sCheckByte % 2) == 0)
            {
                memset(cTemp, 0, sizeof(cTemp));
                memcpy(cTemp, cMsg, iLoc);
                PutChatScrollList(cTemp, cMsgType, iGM);

                memset(cTemp, 0, sizeof(cTemp));
                strcpy(cTemp, cMsg + iLoc);
                memset(cMsg, 0, sizeof(cMsg));
                strcpy(cMsg, " ");
                strcat(cMsg, cTemp);
            }
            else
            {
                memset(cTemp, 0, sizeof(cTemp));
                memcpy(cTemp, cMsg, iLoc + 1);
                PutChatScrollList(cTemp, cMsgType, iGM);

                memset(cTemp, 0, sizeof(cTemp));
                strcpy(cTemp, cMsg + iLoc + 1);
                memset(cMsg, 0, sizeof(cMsg));
                strcpy(cMsg, " ");
                strcat(cMsg, cTemp);
            }
        }
    }

    _RemoveChatMsgListByObjectID(iObjectID);

    for (i = 1; i < DEF_MAXCHATMSGS; i++)
        if (m_pChatMsgList[i] == 0)
        {
            m_pChatMsgList[i] = new CMsg(1, msg, dwTime);
            m_pChatMsgList[i]->m_iObjectID = iObjectID;
            m_pChatMsgList[i]->m_iGM = iGM;

            if (m_pMapData->bSetChatMsgOwner(iObjectID, sX, sY, i) == false)
            {
                delete m_pChatMsgList[i];
                m_pChatMsgList[i] = 0;
            }

            if ((cMsgType != 0) && (m_bIsDialogEnabled[10] != true))
            {
                memset(cHeadMsg, 0, sizeof(cHeadMsg));
                format_to_local(cHeadMsg, "{}:{}", cName, cp);
                AddEventList(cHeadMsg, cMsgType, true, iGM);
            }
            return;
        }
}

void CGame::CivilRightAdmissionHandler(char * pData)
{
    uint16_t * wp, wResult;
    char * cp;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wResult = *wp;

    switch (wResult)
    {
        case 0:
            m_stDialogBoxInfo[13].cMode = 4;
            break;

        case 1:
            m_stDialogBoxInfo[13].cMode = 3;
            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
            memset(m_cLocation, 0, sizeof(m_cLocation));
            memcpy(m_cLocation, cp, 10);
            if (memcmp(m_cLocation, "aresden", 7) == 0)
            {
                m_bAresden = true;
                m_bCitizen = true;
                m_bHunter = false;
            }
            else if (memcmp(m_cLocation, "arehunter", 9) == 0)
            {
                m_bAresden = true;
                m_bCitizen = true;
                m_bHunter = true;
            }
            else if (memcmp(m_cLocation, "elvine", 6) == 0)
            {
                m_bAresden = false;
                m_bCitizen = true;
                m_bHunter = false;
            }
            else if (memcmp(m_cLocation, "elvhunter", 9) == 0)
            {
                m_bAresden = false;
                m_bCitizen = true;
                m_bHunter = true;
            }
            else
            {
                m_bAresden = true;
                m_bCitizen = false;
                m_bHunter = true;
            }
            break;
    }
}
void CGame::DynamicObjectHandler(char * pData)
{
    uint16_t * wp;
    char * cp;
    short * sp, sX, sY, sV1, sV2, sV3;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE);
    wp = (uint16_t *)cp;
    cp += 2;

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    sp = (short *)cp;
    sV1 = *sp;
    cp += 2;

    sp = (short *)cp;
    sV2 = *sp;		   // Dyamic Object Index
    cp += 2;

    sp = (short *)cp;
    sV3 = *sp;
    cp += 2;

    switch (*wp)
    {
        case DEF_MSGTYPE_CONFIRM:
            m_pMapData->bSetDynamicObject(sX, sY, sV2, sV1, true);
            break;

        case DEF_MSGTYPE_REJECT:
            m_pMapData->bSetDynamicObject(sX, sY, sV2, 0, true);
            break;
    }
}
void CGame::NoticementHandler(char * pData)
{
    char * cp;
    FILE * pFile;
    uint16_t * wp;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);

    switch (*wp)
    {
        case DEF_MSGTYPE_CONFIRM:
            break;

        case DEF_MSGTYPE_REJECT:
            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2 + 110);
            pFile = fopen("contents\\contents1000.txt", "wt");
            if (pFile == 0) return;
            fwrite(cp, strlen(cp), 1, pFile);
            fclose(pFile);
            break;
    }
    EnableDialogBox(18, 1000, 0, 0);


    if (m_iLevel < 42) EnableDialogBox(35, 0, 0, 0);

}
void CGame::ResponsePanningHandler(char * pData)
{
    char * cp, cDir;
    short * sp, sX, sY;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    cDir = *cp;
    cp++;

    switch (cDir)
    {
        case 1: m_sViewDstY -= 32; m_sPlayerY--; break;
        case 2: m_sViewDstY -= 32; m_sPlayerY--; m_sViewDstX += 32; m_sPlayerX++; break;
        case 3: m_sViewDstX += 32; m_sPlayerX++; break;
        case 4: m_sViewDstY += 32; m_sPlayerY++; m_sViewDstX += 32; m_sPlayerX++; break;
        case 5: m_sViewDstY += 32; m_sPlayerY++; break;
        case 6: m_sViewDstY += 32; m_sPlayerY++; m_sViewDstX -= 32; m_sPlayerX--; break;
        case 7: m_sViewDstX -= 32; m_sPlayerX--; break;
        case 8: m_sViewDstY -= 32; m_sPlayerY--; m_sViewDstX -= 32; m_sPlayerX--; break;
    }

    m_sVDL_X = sX - (get_virtual_width() / 32) / 2;
    m_sVDL_Y = sY - ((get_virtual_height() - 60) / 32) / 2;
    _ReadMapData(sX, sY, cp);

    m_bIsRedrawPDBGS = true;

    m_bIsObserverCommanded = false;
}
void CGame::ReserveFightzoneResponseHandler(char * pData)
{
    uint16_t * wpResult;
    char * cp;
    int * ip;
    wpResult = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    switch (*wpResult)
    {
        case DEF_MSGTYPE_CONFIRM:
            AddEventList(RESERVE_FIGHTZONE_RESPONSE_HANDLER1, 10);
            m_stDialogBoxInfo[7].cMode = 14;
            m_iFightzoneNumber = m_iFightzoneNumberTemp;
            break;

        case DEF_MSGTYPE_REJECT:
            cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
            ip = (int *)cp;
            cp += 4;
            AddEventList(RESERVE_FIGHTZONE_RESPONSE_HANDLER2, 10);
            m_iFightzoneNumberTemp = 0;

            if (*ip == 0)
            {
                m_stDialogBoxInfo[7].cMode = 15;
            }
            else if (*ip == -1)
            {
                m_stDialogBoxInfo[7].cMode = 16;
            }
            else if (*ip == -2)
            {
                m_stDialogBoxInfo[7].cMode = 17;
            }
            else if (*ip == -3)
            {
                m_stDialogBoxInfo[7].cMode = 21;
            }
            else if (*ip == -4)
            {
                m_stDialogBoxInfo[7].cMode = 22;
            }
            break;
    }
}

void CGame::RetrieveItemHandler(char * pData)
{
    char * cp, cBankItemIndex, cItemIndex, cTxt[120];
    uint16_t * wp;
    int j;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);

    if (*wp != DEF_MSGTYPE_REJECT)
    {
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
        cBankItemIndex = *cp;
        cp++;
        cItemIndex = *cp;
        cp++;

        if (m_pBankList[cBankItemIndex] != 0)
        {
            char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
            GetItemName(m_pBankList[cBankItemIndex], cStr1, cStr2, cStr3, 64);

            memset(cTxt, 0, sizeof(cTxt));
            format_to_local(cTxt, RETIEVE_ITEM_HANDLER4, cStr1);
            AddEventList(cTxt, 10);

            if ((m_pBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {

                if (m_pItemList[cItemIndex] == 0) goto RIH_STEP2;
                //m_pItemList[cItemIndex]->m_dwCount += m_pBankList[cBankItemIndex]->m_dwCount;

                delete m_pBankList[cBankItemIndex];
                m_pBankList[cBankItemIndex] = 0;
                for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                {
                    if ((m_pBankList[j + 1] != 0) && (m_pBankList[j] == 0))
                    {
                        m_pBankList[j] = m_pBankList[j + 1];

                        m_pBankList[j + 1] = 0;
                    }
                }
            }
            else
            {
                RIH_STEP2:;
                if (m_pItemList[cItemIndex] != 0) return;
                short nX, nY;
                nX = 40;
                nY = 30;
                for (j = 0; j < DEF_MAXITEMS; j++)
                {
                    if ((m_pItemList[j] != 0) && (memcmp(m_pItemList[j]->m_cName, cStr1, 20) == 0))
                    {
                        nX = m_pItemList[j]->m_sX + 1;
                        nY = m_pItemList[j]->m_sY + 1;
                        break;
                    }
                }
                m_pItemList[cItemIndex] = m_pBankList[cBankItemIndex];
                //m_pItemList[cItemIndex]->m_sX =	40;
                //m_pItemList[cItemIndex]->m_sY =	30;
                m_pItemList[cItemIndex]->m_sX = nX;
                m_pItemList[cItemIndex]->m_sY = nY;
                bSendCommand(MSGID_REQUEST_SETITEMPOS, 0, cItemIndex, nX, nY, 0, 0);

                for (j = 0; j < DEF_MAXITEMS; j++)
                    if (m_cItemOrder[j] == -1)
                    {
                        m_cItemOrder[j] = cItemIndex;
                        break;
                    }
                //m_cItemOrder[cItemIndex] = cItemIndex;

                m_bIsItemEquipped[cItemIndex] = false;
                m_bIsItemDisabled[cItemIndex] = false;

                m_pBankList[cBankItemIndex] = 0;
                for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                {
                    if ((m_pBankList[j + 1] != 0) && (m_pBankList[j] == 0))
                    {
                        m_pBankList[j] = m_pBankList[j + 1];

                        m_pBankList[j + 1] = 0;
                    }
                }
            }

        }
    }
    m_stDialogBoxInfo[14].cMode = 0;
}


void CGame::NpcTalkHandler(char * pData)
{
    char * cp, cRewardName[21], cTargetName[21], cTemp[21], cTxt[250];
    short * sp, sType, sResponse;
    int     iAmount, iIndex, iContribution, iX, iY, iRange;
    int     iTargetType, iTargetCount, iQuestionType;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sType = *sp;
    cp += 2;

    sp = (short *)cp;
    sResponse = *sp;
    cp += 2;

    sp = (short *)cp;
    iAmount = *sp;
    cp += 2;

    sp = (short *)cp;
    iContribution = *sp;
    cp += 2;

    sp = (short *)cp;
    iTargetType = *sp;
    cp += 2;

    sp = (short *)cp;
    iTargetCount = *sp;
    cp += 2;

    sp = (short *)cp;
    iX = *sp;
    cp += 2;

    sp = (short *)cp;
    iY = *sp;
    cp += 2;

    sp = (short *)cp;
    iRange = *sp;
    cp += 2;

    memset(cRewardName, 0, sizeof(cRewardName));
    memcpy(cRewardName, cp, 20);
    cp += 20;

    memset(cTargetName, 0, sizeof(cTargetName));
    memcpy(cTargetName, cp, 20);
    cp += 20;

    EnableDialogBox(21, sResponse, sType, 0);

    if ((sType >= 1) && (sType <= 100))
    {
        iIndex = m_stDialogBoxInfo[21].sV1;
        m_pMsgTextList2[iIndex] = new CMsg(0, "  ", 0);
        iIndex++;

        iQuestionType = 0;
        switch (sType)
        {
            case 1: //Monster Hunt
                memset(cTemp, 0, sizeof(cTemp));
                GetNpcName(iTargetType, cTemp);
                memset(cTxt, 0, sizeof(cTxt));
                format_to_local(cTxt, NPC_TALK_HANDLER16, iTargetCount, cTemp);
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                if (memcmp(cTargetName, "NONE", 4) == 0)
                {
                    strcpy(cTxt, NPC_TALK_HANDLER17);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                else
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    GetOfficialMapName(cTargetName, cTemp);
                    format_to_local(cTxt, NPC_TALK_HANDLER18, cTemp);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;

                    if (iX != 0)
                    {
                        memset(cTxt, 0, sizeof(cTxt));
                        format_to_local(cTxt, NPC_TALK_HANDLER19, iX, iY, iRange);
                        m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                        iIndex++;
                    }

                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER20, iContribution);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                iQuestionType = 1;
                break;

            case 7:
                memset(cTxt, 0, sizeof(cTxt));
                m_pMsgTextList2[iIndex] = new CMsg(0, NPC_TALK_HANDLER21, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                if (memcmp(cTargetName, "NONE", 4) == 0)
                {
                    strcpy(cTxt, NPC_TALK_HANDLER22);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                else
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    GetOfficialMapName(cTargetName, cTemp);
                    format_to_local(cTxt, NPC_TALK_HANDLER23, cTemp);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;

                    if (iX != 0)
                    {
                        memset(cTxt, 0, sizeof(cTxt));
                        format_to_local(cTxt, NPC_TALK_HANDLER24, iX, iY, iRange);
                        m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                        iIndex++;
                    }

                    memset(cTxt, 0, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER25, iContribution);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                iQuestionType = 1;
                break;

            case 10:
                memset(cTxt, 0, sizeof(cTxt));
                m_pMsgTextList2[iIndex] = new CMsg(0, NPC_TALK_HANDLER26, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                strcpy(cTxt, NPC_TALK_HANDLER27);
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                strcpy(cTxt, NPC_TALK_HANDLER28);
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                strcpy(cTxt, NPC_TALK_HANDLER29);
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                strcpy(cTxt, NPC_TALK_HANDLER30);
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                strcpy(cTxt, " ");
                m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                iIndex++;

                memset(cTxt, 0, sizeof(cTxt));
                if (memcmp(cTargetName, "NONE", 4) == 0)
                {
                    strcpy(cTxt, NPC_TALK_HANDLER31);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                else
                {
                    memset(cTemp, 0, sizeof(cTemp));
                    GetOfficialMapName(cTargetName, cTemp);
                    format_to_local(cTxt, NPC_TALK_HANDLER32, cTemp);
                    m_pMsgTextList2[iIndex] = new CMsg(0, cTxt, 0);
                    iIndex++;
                }
                iQuestionType = 2;
                break;
        }

        switch (iQuestionType)
        {
            case 1:
                m_pMsgTextList2[iIndex] = new CMsg(0, "  ", 0);
                iIndex++;
                m_pMsgTextList2[iIndex] = new CMsg(0, NPC_TALK_HANDLER33, 0);
                iIndex++;
                m_pMsgTextList2[iIndex] = new CMsg(0, NPC_TALK_HANDLER34, 0);
                iIndex++;
                m_pMsgTextList2[iIndex] = new CMsg(0, "  ", 0);
                iIndex++;
                break;

            case 2:
                m_pMsgTextList2[iIndex] = new CMsg(0, "  ", 0);
                iIndex++;
                m_pMsgTextList2[iIndex] = new CMsg(0, NPC_TALK_HANDLER35, 0);
                iIndex++;
                m_pMsgTextList2[iIndex] = new CMsg(0, "  ", 0);
                iIndex++;
                break;

            default: break;
        }
    }
}
void CGame::MotionResponseHandler(char * pData)
{
    uint16_t * wp, wResponse;
    short * sp, sX, sY;
    char * cp, cDir;
    int * ip, iPreHP;

    //						          0 3        4 5						 6 7		8 9		   10	    11	
    // Confirm Code(4) | MsgSize(4) | MsgID(4) | DEF_OBJECTMOVE_CONFIRM(2) | Loc-X(2) | Loc-Y(2) | Dir(1) | MapData ...
    // Confirm Code(4) | MsgSize(4) | MsgID(4) | DEF_OBJECTMOVE_REJECT(2)  | Loc-X(2) | Loc-Y(2)  

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wResponse = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    switch (wResponse)
    {
        case DEF_OBJECTMOTION_CONFIRM:
            m_cCommandCount--;
            break;

        case DEF_OBJECTMOTION_ATTACK_CONFIRM:
            m_cCommandCount--;
            break;

        case DEF_OBJECTMOTION_REJECT:

            if (m_iHP <= 0) return;

            sp = (short *)cp;
            m_sPlayerX = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerY = *sp;
            cp += 2;

            m_cCommand = DEF_OBJECTSTOP;
            m_sCommX = m_sPlayerX;
            m_sCommY = m_sPlayerY;

            m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                m_iPlayerStatus, m_cPlayerName,
                DEF_OBJECTSTOP, 0, 0, 0);
            m_cCommandCount = 0;
            m_bIsGetPointingMode = false;
            ClearCoords();

            m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32 - 32;
            m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32 - 32;

            m_bIsRedrawPDBGS = true;
            break;

        case DEF_OBJECTMOVE_CONFIRM:
            sp = (short *)cp;
            sX = *sp;
            cp += 2;

            sp = (short *)cp;
            sY = *sp;
            cp += 2;

            cDir = *cp;
            cp++;

            m_iSP = m_iSP - *cp;
            cp++;
            if (m_iSP < 0) m_iSP = 0;

            // v1.3
            //m_iOccupyStatus = (int)*cp;
            cp++;

            // v1.4
            iPreHP = m_iHP;
            ip = (int *)cp;
            m_iHP = *ip;
            cp += 4;

            if (m_iHP != iPreHP)
            {
                if (m_iHP < iPreHP)
                {
                    format_to_local(G_cTxt, NOTIFYMSG_HP_DOWN, iPreHP - m_iHP);
                    AddEventList(G_cTxt, 10);
                    m_dwDamagedTime = unixtime();

                    if ((m_cLogOutCount > 0) && (m_bForceDisconn == false))
                    {
                        m_cLogOutCount = -1;
                        AddEventList(MOTION_RESPONSE_HANDLER2, 10);
                    }
                }
                else
                {
                    format_to_local(G_cTxt, NOTIFYMSG_HP_UP, m_iHP - iPreHP);
                    AddEventList(G_cTxt, 10);
                }
            }

            m_sVDL_X = sX;
            m_sVDL_Y = sY;
            _ReadMapData(sX, sY, cp);

            m_bIsRedrawPDBGS = true;

            m_cCommandCount--;
            break;

        case DEF_OBJECTMOVE_REJECT:
            if (m_iHP <= 0) return;

            wp = (uint16_t *)cp;
            if (m_sPlayerObjectID != *wp) return;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerX = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerY = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerType = *sp;
            cp += 2;

            m_cPlayerDir = *cp;
            cp++;

            //memcpy(cName, cp, 10);
            cp += 10;

            sp = (short *)cp;
            m_sPlayerAppr1 = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerAppr2 = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerAppr3 = *sp;
            cp += 2;

            sp = (short *)cp;
            m_sPlayerAppr4 = *sp;
            cp += 2;

            ip = (int *)cp;
            m_iPlayerApprColor = *ip;
            cp += 4;

            ip = (int *)cp;
            m_iPlayerStatus = *ip;
            cp += 4;

            m_cCommand = DEF_OBJECTSTOP;
            m_sCommX = m_sPlayerX;
            m_sCommY = m_sPlayerY;

            m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
                m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
                m_iPlayerStatus, m_cPlayerName,
                DEF_OBJECTSTOP, 0, 0, 0,
                0, 7);
            m_cCommandCount = 0;
            m_bIsGetPointingMode = false;
            ClearCoords();

            m_sViewDstX = m_sViewPointX = (m_sPlayerX - ((get_virtual_width() / 32) / 2)) * 32 - 32;
            m_sViewDstY = m_sViewPointY = (m_sPlayerY - ((get_virtual_height() / 32) / 2)) * 32 - 32;

            m_bIsPrevMoveBlocked = true;

            switch (m_sPlayerType)
            {
                case 1:
                case 2:
                case 3:
                    PlaySound('C', 12, 0);
                    break;

                case 4:
                case 5:
                case 6:
                    PlaySound('C', 13, 0);
                    break;
            }
            break;
    }
}
void CGame::InitDataResponseHandler(char * pData)
{
    int * ip, i;
    short * sp, sX, sY;
    char * cp, cMapFileName[32], cTxt[120]{}, cPreCurLocation[12];
    bool bIsObserverMode;
    HANDLE hFile;
    uint32_t dwFileSize;

    memset(cPreCurLocation, 0, sizeof(cPreCurLocation));

    m_bParalyze = false;
    m_pMapData->Init();

    m_sMonsterID = 0;
    m_dwMonsterEventTime = 0;

    DisableDialogBox(7);
    DisableDialogBox(11);
    DisableDialogBox(13);
    DisableDialogBox(14);
    DisableDialogBox(16);
    DisableDialogBox(22);
    DisableDialogBox(20);
    DisableDialogBox(21);
    DisableDialogBox(23);

    m_cCommand = DEF_OBJECTSTOP;

    //m_bCommandAvailable = true;
    m_cCommandCount = 0;

    m_bIsGetPointingMode = false;
    m_iPointCommandType = -1;
    ClearCoords();

    m_iIlusionOwnerH = 0;
    m_cIlusionOwnerType = 0;

    m_bIsTeleportRequested = false;
    m_bIsConfusion = false;

    if (m_bIllusionMVT != true) m_bIllusionMVT = false;

    m_bSkillUsingStatus = false;
    m_bItemUsingStatus = false;

    m_cRestartCount = -1;
    m_dwRestartCountTime = 0;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
    {
        if (m_pEffectList[i] != 0) delete m_pEffectList[i];
        m_pEffectList[i] = 0;
    }

    for (i = 0; i < DEF_MAXWHETHEROBJECTS; i++)
    {
        m_stWhetherObject[i].sX = 0;
        m_stWhetherObject[i].sY = 0;
        m_stWhetherObject[i].cStep = 0;
    }

    for (i = 0; i < DEF_MAXGUILDNAMES; i++)
    {
        m_stGuildName[i].dwRefTime = 0;
        m_stGuildName[i].iGuildRank = -1;
        memset(m_stGuildName[i].cCharName, 0, sizeof(m_stGuildName[i].cCharName));
        memset(m_stGuildName[i].cGuildName, 0, sizeof(m_stGuildName[i].cGuildName));
    }
    for (i = 0; i < DEF_MAXCHATMSGS; i++)
    {
        if (m_pChatMsgList[i] != 0) delete m_pChatMsgList[i];
        m_pChatMsgList[i] = 0;
    }

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
    sp = (short *)cp;
    m_sPlayerObjectID = *sp;
    cp += 2;

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;

    sp = (short *)cp;
    m_sPlayerType = *sp;
    if (m_sPlayerType > 0 && m_sPlayerType <= 3)
        m_cGender = 0;
    if (m_sPlayerType > 3 && m_sPlayerType <= 6)
        m_cGender = 1;
    cp += 2;

    sp = (short *)cp;
    m_sPlayerAppr1 = *sp;
    cp += 2;

    sp = (short *)cp;
    m_sPlayerAppr2 = *sp;
    cp += 2;

    sp = (short *)cp;
    m_sPlayerAppr3 = *sp;
    cp += 2;

    sp = (short *)cp;
    m_sPlayerAppr4 = *sp;
    cp += 2;

    ip = (int *)cp;
    m_iPlayerApprColor = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iPlayerStatus = *ip;
    cp += 4;

    memset(m_cMapName, 0, sizeof(m_cMapName));
    memset(m_cMapMessage, 0, sizeof(m_cMapMessage));
    memcpy(m_cMapName, cp, 10);
    m_cMapIndex = GetOfficialMapName(m_cMapName, m_cMapMessage);
    if (m_cMapIndex < 0)
    {
        m_stDialogBoxInfo[9].sSizeX = -1;
        m_stDialogBoxInfo[9].sSizeY = -1;
    }
    else
    {
        m_stDialogBoxInfo[9].sSizeX = 128;
        m_stDialogBoxInfo[9].sSizeY = 128;
    }
    cp += 10;

    strcpy(cPreCurLocation, m_cCurLocation);
    memset(m_cCurLocation, 0, sizeof(m_cCurLocation));
    memcpy(m_cCurLocation, cp, 10);
    cp += 10;
    G_cSpriteAlphaDegree = *cp;
    cp++;
    m_cWhetherStatus = *cp;
    cp++;

    ip = (int *)cp;
    m_iContribution = *ip;
    cp += 4;

    bIsObserverMode = (bool)*cp;
    cp++;

    ip = (int *)cp;
    m_iRating = *ip;
    cp += 4;

    ip = (int *)cp;
    m_iHP = *ip;
    cp += 4;

    m_cDiscount = (char)*cp;
    cp++;

    if (m_cWhetherStatus != 0)
        SetWhetherStatus(true, m_cWhetherStatus);
    else SetWhetherStatus(false, m_cWhetherStatus);

    memset(cMapFileName, 0, sizeof(cMapFileName));
    strcat(cMapFileName, "data\\mapdata\\");
    strcat(cMapFileName, m_cMapName);
    strcat(cMapFileName, ".amd");
    m_pMapData->OpenMapDataFile(cMapFileName);

    m_sPlayerX = sX;
    m_sPlayerY = sY;

    m_cPlayerDir = 5;
    if (bIsObserverMode == false)
    {
        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
            m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor, // v1.4
            m_iPlayerStatus, m_cPlayerName,
            DEF_OBJECTSTOP, 0, 0, 0);
    }

    m_sViewDstX = m_sViewPointX = sX * 32 - (get_virtual_width() / 2);
    m_sViewDstY = m_sViewPointY = sY * 32 - ((get_virtual_height() - 60) / 2) - 16;

    m_sVDL_X = sX - (get_virtual_width() / 32) / 2;
    m_sVDL_Y = sY - ((get_virtual_height() - 60) / 32) / 2;

    _ReadMapData(sX, sY, cp);

    m_bIsRedrawPDBGS = true;

    format_to_local(cTxt, INITDATA_RESPONSE_HANDLER1, m_cMapMessage);
    AddEventList(cTxt, 10);

    m_stDialogBoxInfo[6].sX = 150;
    m_stDialogBoxInfo[6].sY = 130;
    if ((memcmp(m_cCurLocation, "middleland", 10) == 0) ||
        (memcmp(m_cCurLocation, "dglv2", 5) == 0) ||
        (memcmp(m_cCurLocation, "middled1n", 9) == 0))
        EnableDialogBox(6, 0, 0, 0);

    bool bPrevSafe, bNowSafe;
    if (memcmp(cPreCurLocation, m_cLocation, 3) == 0)
        bPrevSafe = true;
    else bPrevSafe = false;

    if (memcmp(m_cCurLocation, m_cLocation, 3) == 0)
        bNowSafe = true;
    else bNowSafe = false;

    if (memcmp(m_cCurLocation, "2nd", 3) == 0) bNowSafe = true;
    if (m_iPKCount != 0) bNowSafe = false;

    if (bPrevSafe)
    {
        if (bNowSafe == false) SetTopMsg(DEF_MSG_DANGERZONE, 5);
    }
    else
    {
        if (bNowSafe) SetTopMsg(DEF_MSG_SAFEZONE, 5);
    }

    ChangeGameMode(DEF_GAMEMODE_ONMAINGAME);

    if ((m_sPlayerAppr2 & 0xF000) != 0)
        m_bIsCombatMode = true;
    else m_bIsCombatMode = false;

    if (m_bIsFirstConn == true)
    {
        m_bIsFirstConn = false;
        hFile = CreateFileA("contents\\contents1000.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
        if (hFile == INVALID_HANDLE_VALUE)
            dwFileSize = 0;
        else
        {
            dwFileSize = GetFileSize(hFile, 0);
            CloseHandle(hFile);
        }

        bSendCommand(MSGID_REQUEST_NOTICEMENT, 0, 0, (int)dwFileSize, 0, 0, 0);
    }
}

void CGame::MotionEventHandler(char * pData)
{
    uint16_t * wp, wEventType{}, wObjectID{};
    short * sp, sX{}, sY{}, sType{}, sAppr1{}, sAppr2{}, sAppr3{}, sAppr4{}, sV1{}, sV2{}, sV3{}, sPrevAppr2{};
    char * cp, cDir{}, cName[12]{};
    int iStatus{};
    int * ip, iApprColor{}, iLoc{};
    char cTxt[120]{};
    int i;

    sX = sY = sType = sAppr1 = sAppr2 = sAppr3 = sAppr4 = sV1 = sV2 = sV3 = sPrevAppr2 = iApprColor = iLoc = iStatus = wEventType = wObjectID = -1;

    memset(cName, 0, sizeof(cName));
    sV1 = sV2 = sV3 = 0;

    wp = (uint16_t *)(pData + DEF_INDEX2_MSGTYPE);
    wEventType = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    wp = (uint16_t *)cp;
    wObjectID = *wp;
    cp += 2;

    iLoc = 0;

    if (wObjectID < 30000)
    {
        if (wObjectID < 10000)
        {

            sp = (short *)cp;
            sX = *sp;
            cp += 2;

            sp = (short *)cp;
            sY = *sp;
            cp += 2;

            sp = (short *)cp;
            sType = *sp;
            cp += 2;

            cDir = *cp;
            cp++;

            memcpy(cName, cp, 10);
            cp += 10;

            sp = (short *)cp;
            sAppr1 = *sp;
            cp += 2;

            sp = (short *)cp;
            sAppr2 = *sp;
            cp += 2;

            sp = (short *)cp;
            sAppr3 = *sp;
            cp += 2;

            sp = (short *)cp;
            sAppr4 = *sp;
            cp += 2;

            ip = (int *)cp;
            iApprColor = *ip;
            cp += 4;

            ip = (int *)cp;
            iStatus = *ip;
            cp += 4;

            iLoc = *cp;
            cp++;

            // todo: hp bars for npcs
            //			ip  = (int *)cp;
            //			m_stNPCList[wObjectID].m_iMaxHP = *ip;
            //			cp += 4;

            //			ip  = (int *)cp;
            //			m_stNPCList[wObjectID].m_iHP = *ip;
            //			cp += 4;
        }
        else
        {
            sp = (short *)cp;
            sX = *sp;
            cp += 2;

            sp = (short *)cp;
            sY = *sp;
            cp += 2;

            sp = (short *)cp;
            sType = *sp;
            cp += 2;

            cDir = *cp;
            cp++;

            memcpy(cName, cp, 5);
            cp += 5;

            sAppr1 = sAppr3 = sAppr4 = 0;

            sp = (short *)cp;
            sAppr2 = *sp;
            cp += 2;

            ip = (int *)cp;
            iStatus = *ip;
            cp += 4;

            iLoc = *cp;
            cp++;

            ip = (int *)cp;
            m_stNPCList[wObjectID].m_iMaxHP = *ip;
            cp += 4;

            ip = (int *)cp;
            m_stNPCList[wObjectID].m_iHP = *ip;
            cp += 4;
        }
    }
    else
    {
        switch (wEventType)
        {
            case DEF_OBJECTMAGIC:
            case DEF_OBJECTDAMAGEMOVE:
            case DEF_OBJECTDAMAGE:
                cDir = *cp;
                cp++;

                sV1 = (short)*cp;
                cp++;
                sV2 = (short)*cp;
                cp++;
                break;

            case DEF_OBJECTDYING:
                cDir = *cp;
                cp++;

                sV1 = (short)*cp;
                cp++;
                sV2 = (short)*cp;
                cp++;

                sp = (short *)cp;
                sX = *sp;
                cp += 2;

                sp = (short *)cp;
                sY = *sp;
                cp += 2;
                break;

            case DEF_OBJECTATTACK:
                cDir = *cp;
                cp++;

                sV1 = *cp;
                cp++;

                sV2 = *cp;
                cp++;

                sp = (short *)cp;
                sV3 = *sp;
                cp += 2;
                break;

            default:
                cDir = *cp;
                cp++;
                break;
        }
    }

    if ((wEventType == DEF_OBJECTNULLACTION) && (memcmp(cName, m_cPlayerName, 10) == 0))
    {
        m_sPlayerType = sType;
        m_sPlayerAppr1 = sAppr1;
        sPrevAppr2 = m_sPlayerAppr2;
        m_sPlayerAppr2 = sAppr2;
        m_sPlayerAppr3 = sAppr3;
        m_sPlayerAppr4 = sAppr4;
        m_iPlayerApprColor = iApprColor;
        m_iPlayerStatus = iStatus;

        if ((sPrevAppr2 & 0xF000) == 0)
        {
            if ((sAppr2 & 0xF000) != 0)
            {
                AddEventList(MOTION_EVENT_HANDLER1, 10);
                m_bIsCombatMode = true;
            }
        }
        else
        {
            if ((sAppr2 & 0xF000) == 0)
            {
                AddEventList(MOTION_EVENT_HANDLER2, 10);
                m_bIsCombatMode = false;
            }
        }
        if (m_cCommand != DEF_OBJECTRUN) m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, (char)wEventType, sV1, sV2, sV3, iLoc);
    }
    else
    {
#ifdef DEF_HACKCLIENT
//		if (m_stConfigList.bDebugNpc == true) {
//			format_to_local(G_cTxt, "wObjectID({})sX(%hd)sY(%hd)iStatus(0x%X)cName({})iLoc({})", wObjectID, sX, sY, iStatus, cName, iLoc);
//			PutChatScrollList(G_cTxt, 1);
//		}
#endif
#ifndef DEF_HACKCLIENT
        m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, (char)wEventType, sV1, sV2, sV3, iLoc);
#else
        if (wEventType != DEF_OBJECTDAMAGE)
            m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, iStatus, cName, (char)wEventType, sV1, sV2, sV3, iLoc);
#endif
    }

    switch (wEventType)
    {
        case DEF_OBJECTMAGIC:
            _RemoveChatMsgListByObjectID(wObjectID - 30000);

            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == 0)
                {
                    memset(cTxt, 0, sizeof(cTxt));

                    if (m_pMagicCfgList[sV1] == 0)//Change Invalid spell
                        format_to_local(cTxt, "Invalid Spell!");
                    else
                        format_to_local(cTxt, "{}!", m_pMagicCfgList[sV1]->m_cName);

                    m_pChatMsgList[i] = new CMsg(41, cTxt, m_dwCurTime);
                    m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
                    if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = 0;
                    }
                    return;
                }
            break;

        case DEF_OBJECTDYING:
            _RemoveChatMsgListByObjectID(wObjectID - 30000);

            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == 0)
                {
                    memset(cTxt, 0, sizeof(cTxt));
                    if (sV1 > 0)
                        format_to_local(cTxt, "-{}Pts!", sV1); //pts
                    else strcpy(cTxt, "Critical!");

                    int iFontType;
                    if ((sV1 >= 0) && (sV1 < 12))		iFontType = 21;
                    else if ((sV1 >= 12) && (sV1 < 40)) iFontType = 22;
                    else if ((sV1 >= 40) || (sV1 < 0))	iFontType = 23;

                    m_stNPCList[wObjectID - 30000].m_iMaxHP = m_stNPCList[wObjectID - 30000].m_iHP = 0;//Change HP Bar

                    m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                    m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;

                    if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = 0;
                    }
                    return;
                }
            break;

        case DEF_OBJECTDAMAGEMOVE:
        case DEF_OBJECTDAMAGE:
            if (memcmp(cName, m_cPlayerName, 10) == 0)
            {
#ifndef DEF_HACKCLIENT
                m_bIsGetPointingMode = false;
                ClearCoords();
                m_iPointCommandType = -1;  // v2.15 0 -> -1
                m_stMCursor.sCursorFrame = 0;
                ClearSkillUsingStatus();
#endif
            }
            _RemoveChatMsgListByObjectID(wObjectID - 30000);

            for (i = 1; i < DEF_MAXCHATMSGS; i++)
                if (m_pChatMsgList[i] == 0)
                {
                    memset(cTxt, 0, sizeof(cTxt));

                    if (sV1 != 0)
                    {
                        if (sV1 > 0)
                        {
                            format_to_local(cTxt, "-{}Pts", sV1); //pts
                            m_stNPCList[wObjectID - 30000].m_iHP -= sV1;//Change HP Bar
                        }
                        else strcpy(cTxt, "Critical!");

                        int iFontType;
                        if ((sV1 >= 0) && (sV1 < 12))		iFontType = 21;
                        else if ((sV1 >= 12) && (sV1 < 40)) iFontType = 22;
                        else if ((sV1 >= 40) || (sV1 < 0))	iFontType = 23;

                        m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                    }
                    else
                    {
                        strcpy(cTxt, " * Failed! *");
                        m_pChatMsgList[i] = new CMsg(22, cTxt, m_dwCurTime);
                        PlaySound('C', 17, 0);
                    }
                    m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;

                    if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == false)
                    {
                        delete m_pChatMsgList[i];
                        m_pChatMsgList[i] = 0;
                    }
                    return;
                }
            break;
    }
}
void CGame::ResponseTeleportList(char * pData)
{
    char * cp;
    int * ip, i;

#ifdef _DEBUG
    AddEventList("ÅÚ·¹Æ÷Æ® ÇÏ·Á±¸ ÇÏ³Ä..??", 10);
#endif

    cp = pData + 6;
    ip = (int *)cp;
    m_iTeleportMapCount = *ip;
    cp += 4;

    for (i = 0; i < m_iTeleportMapCount; i++)
    {
        ip = (int *)cp;
        m_stTeleportList[i].iIndex = *ip;
        cp += 4;
        memset(m_stTeleportList[i].mapname, 0, sizeof(m_stTeleportList[i].mapname));
        memcpy(m_stTeleportList[i].mapname, cp, 10);
        cp += 10;
        ip = (int *)cp;
        m_stTeleportList[i].iX = *ip;
        cp += 4;
        ip = (int *)cp;
        m_stTeleportList[i].iY = *ip;
        cp += 4;
        ip = (int *)cp;
        m_stTeleportList[i].iCost = *ip;
        cp += 4;
    }
}

void CGame::ResponseChargedTeleport(char * pData)
{
    short * sp;
    char * cp;
    short sRejectReason = 0;

    cp = (char *)pData + DEF_INDEX2_MSGTYPE + 2;
    sp = (short *)cp;
    sRejectReason = *sp;

#ifdef _DEBUG
    AddEventList("¾ÈµÈµ¥", 10);
#endif

    switch (sRejectReason)
    {
        case 1:
            AddEventList(RESPONSE_CHARGED_TELEPORT1, 10);
            break;
        case 2:
            AddEventList(RESPONSE_CHARGED_TELEPORT2, 10);
            break;
        case 3:
            AddEventList(RESPONSE_CHARGED_TELEPORT3, 10);
            break;
        case 4:
            AddEventList(RESPONSE_CHARGED_TELEPORT4, 10);
            break;
        case 5:
            AddEventList(RESPONSE_CHARGED_TELEPORT5, 10);
            break;
        case 6:
            AddEventList(RESPONSE_CHARGED_TELEPORT6, 10);
            break;
        default:
            AddEventList(RESPONSE_CHARGED_TELEPORT7, 10);
    }
}
void CGame::ResponseHeldenianTeleportList(char * pData)
{
    char * cp;
    int * ip, i;
    cp = pData + 6;
    ip = (int *)cp;
    m_iTeleportMapCount = *ip;
    cp += 4;
    for (i = 0; i < m_iTeleportMapCount; i++)
    {
        ip = (int *)cp;
        m_stTeleportList[i].iIndex = *ip;
        cp += 4;
        memset(m_stTeleportList[i].mapname, 0, sizeof(m_stTeleportList[i].mapname));
        memcpy(m_stTeleportList[i].mapname, cp, 10);
        cp += 10;
        ip = (int *)cp;
        m_stTeleportList[i].iX = *ip;
        cp += 4;
        ip = (int *)cp;
        m_stTeleportList[i].iY = *ip;
        cp += 4;
        ip = (int *)cp;
        m_stTeleportList[i].iCost = *ip;
        cp += 4;
    }
}



void CGame::ConnectionEstablishHandler(char cWhere)
{
    ChangeGameMode(DEF_GAMEMODE_ONWAITINGRESPONSE);

    switch (cWhere)
    {
        case DEF_SERVERTYPE_GAME:
            bSendCommand(MSGID_REQUEST_INITDATA, 0, 0, 0, 0, 0, 0);
            ChangeGameMode(DEF_GAMEMODE_ONWAITINGINITDATA);
            break;

        case DEF_SERVERTYPE_LOG:
            switch (m_dwConnectMode)
            {
                case MSGID_REQUEST_LOGIN:
                    bSendCommand(MSGID_REQUEST_LOGIN, 0, 0, 0, 0, 0, 0);
                    break;

                case MSGID_REQUEST_CREATENEWACCOUNT:
                    bSendCommand(MSGID_REQUEST_CREATENEWACCOUNT, 0, 0, 0, 0, 0, 0);
                    break;

                case MSGID_REQUEST_CREATENEWCHARACTER:
                    bSendCommand(MSGID_REQUEST_CREATENEWCHARACTER, 0, 0, 0, 0, 0, 0);
                    break;

                case MSGID_REQUEST_ENTERGAME:
                    send_screen_settings_to_server();
                    bSendCommand(MSGID_REQUEST_ENTERGAME, 0, 0, 0, 0, 0, 0);
                    break;

                case MSGID_REQUEST_DELETECHARACTER:
                    bSendCommand(MSGID_REQUEST_DELETECHARACTER, 0, 0, 0, 0, 0, 0);
                    break;

                case MSGID_REQUEST_CHANGEPASSWORD:
                    bSendCommand(MSGID_REQUEST_CHANGEPASSWORD, 0, 0, 0, 0, 0, 0);
                    break;

            }
            break;
    }
}
