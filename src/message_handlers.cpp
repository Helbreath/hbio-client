//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include <algorithm>
#include <iostream>
#include <fmt/format.h>

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

extern char G_cSpriteAlphaDegree;

extern char _cDrawingOrder[];
extern char _cMantleDrawingOrder[];
extern char _cMantleDrawingOrderOnRun[];


extern short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;//, _tmp_sStatus;
extern int _tmp_sStatus;
extern char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
extern int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY;
extern WORD  _tmp_wObjectID;
extern char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
extern WORD  wFocusObjectID;
extern short sFocus_dX, sFocus_dY;
extern char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
extern short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
extern int sFocusStatus;
extern int   iFocusApprColor;


void CGame::ChatMsgHandler(char * pData)
{
    int i, iObjectID, iLoc;
    short * sp, sX, sY;
    char * cp, cMsgType, cName[21], cTemp[100], cMsg[100], cTxt1[100], cTxt2[100];
    DWORD dwTime;
    WORD * wp;
    BOOL bFlag;

    char cHeadMsg[200];

    dwTime = m_dwCurTime;

    ZeroMemory(cTxt1, sizeof(cTxt1));
    ZeroMemory(cTxt2, sizeof(cTxt2));
    ZeroMemory(cMsg, sizeof(cMsg));

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    iObjectID = (int)*wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    sp = (short *)cp;
    sX = *sp;
    cp += 2;

    sp = (short *)cp;
    sY = *sp;
    cp += 2;
    ZeroMemory(cName, sizeof(cName));
    memcpy(cName, (char *)cp, 10);
    cp += 10;

    cMsgType = *cp;
    cp++;

    
    if (bCheckExID(cName) == TRUE) return;

    ZeroMemory(cTemp, sizeof(cTemp));
    strcpy(cTemp, cp);

#if DEF_LANGUAGE == 4
    if ((cMsgType == 0) || (cMsgType == 2) || (cMsgType == 3))
    {
        if (m_Misc.bCheckIMEString(cTemp) == FALSE) return;
    }
#endif
    if (!m_bWhisper)
    {
        if (cMsgType == 20) return;
    }
    if (!m_bShout)
    {
        if (cMsgType == 2 || cMsgType == 3) return;
    }

    ZeroMemory(cMsg, sizeof(cMsg));
    format_to_local(cMsg, "{}: {}", cName, cTemp);

    bFlag = FALSE;
    short sCheckByte = 0;

    while (bFlag == FALSE)
    {
        iLoc = m_Misc.iGetTextLengthLoc(chat_window_text, cMsg, 305);
        for (int i = 0; i < iLoc; i++) if (cMsg[i] < 0) sCheckByte++;
        if (iLoc == 0)
        {
            PutChatScrollList(cMsg, cMsgType);
            bFlag = TRUE;
        }
        else
        {
            if ((sCheckByte % 2) == 0)
            {
                
                ZeroMemory(cTemp, sizeof(cTemp));
                memcpy(cTemp, cMsg, iLoc);
                PutChatScrollList(cTemp, cMsgType);
                
                ZeroMemory(cTemp, sizeof(cTemp));
                strcpy(cTemp, cMsg + iLoc);
                ZeroMemory(cMsg, sizeof(cMsg));
                strcpy(cMsg, " ");
                strcat(cMsg, cTemp);
            }
            else
            {
                
                ZeroMemory(cTemp, sizeof(cTemp));
                memcpy(cTemp, cMsg, iLoc + 1);
                PutChatScrollList(cTemp, cMsgType);
                
                ZeroMemory(cTemp, sizeof(cTemp));
                strcpy(cTemp, cMsg + iLoc + 1);
                ZeroMemory(cMsg, sizeof(cMsg));
                strcpy(cMsg, " ");
                strcat(cMsg, cTemp);
            }
        }
    }

    
    _RemoveChatMsgListByObjectID(iObjectID);

    
    for (i = 1; i < DEF_MAXCHATMSGS; i++)
        if (m_pChatMsgList[i] == NULL)
        {
            m_pChatMsgList[i] = new CMsg(1, (char *)(cp), dwTime);
            m_pChatMsgList[i]->m_iObjectID = iObjectID;

            if (m_pMapData->bSetChatMsgOwner(iObjectID, sX, sY, i) == FALSE)
            {
                delete m_pChatMsgList[i];
                m_pChatMsgList[i] = NULL;
            }

            if ((cMsgType != 0) && (m_bIsDialogEnabled[10] != TRUE))
            {
                ZeroMemory(cHeadMsg, sizeof(cHeadMsg));
                format_to_local(cHeadMsg, "{}:{}", cName, cp);
                AddEventList(cHeadMsg, cMsgType);
            }
            return;
        }
}

void CGame::CivilRightAdmissionHandler(char * pData)
{
    WORD * wp, wResult;
    char * cp;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    wResult = *wp;

    switch (wResult)
    {
    case 0:
        
        m_stDialogBoxInfo[13].cMode = 4;
        break;

    case 1:
        
        m_stDialogBoxInfo[13].cMode = 3;
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
        ZeroMemory(m_cLocation, sizeof(m_cLocation));
        memcpy(m_cLocation, cp, 10);
        if (memcmp(m_cLocation, "aresden", 7) == 0)
        {
            m_bAresden = TRUE;
            m_bCitizen = TRUE;
            m_bHunter = FALSE;
        }
        else if (memcmp(m_cLocation, "arehunter", 9) == 0)
        {
            m_bAresden = TRUE;
            m_bCitizen = TRUE;
            m_bHunter = TRUE;
        }
        else if (memcmp(m_cLocation, "elvine", 6) == 0)
        {
            m_bAresden = FALSE;
            m_bCitizen = TRUE;
            m_bHunter = FALSE;
        }
        else if (memcmp(m_cLocation, "elvhunter", 9) == 0)
        {
            m_bAresden = FALSE;
            m_bCitizen = TRUE;
            m_bHunter = TRUE;
        }
        else
        {
            m_bAresden = TRUE;
            m_bCitizen = FALSE;
            m_bHunter = TRUE;
        }
        break;
    }
}

void CGame::DynamicObjectHandler(char * pData)
{
    WORD * wp;
    char * cp;
    short * sp, sX, sY, sV1, sV2, sV3;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE);
    wp = (WORD *)cp;
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
    sV2 = *sp;		   // Dynamic Object Index
    cp += 2;

    sp = (short *)cp;
    sV3 = *sp;
    cp += 2;

    switch (*wp)
    {
    case DEF_MSGTYPE_CONFIRM:
        
        m_pMapData->bSetDynamicObject(sX, sY, sV2, sV1, TRUE);
        break;

    case DEF_MSGTYPE_REJECT:
        
        m_pMapData->bSetDynamicObject(sX, sY, sV2, NULL, TRUE);
        break;
    }
}

void CGame::NoticementHandler(char * pData)
{
    char * cp;
    FILE * pFile;
    WORD * wp;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);

    switch (*wp)
    {
    case DEF_MSGTYPE_CONFIRM:
        
        break;

    case DEF_MSGTYPE_REJECT:
        
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
        pFile = fopen("contents\\contents1000.txt", "wt");
        if (pFile == NULL) return;
        fwrite(cp, strlen(cp), 1, pFile);
        fclose(pFile);
        break;
    }


    
    EnableDialogBox(18, 1000, NULL, NULL);

    
    if (m_iLevel < 42) EnableDialogBox(35, NULL, NULL, NULL);

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

    m_pMapData->ShiftMapData(cDir);
    _ReadMapData(sX, sY, cp);

    m_bIsRedrawPDBGS = TRUE;

    m_bIsObserverCommanded = FALSE;
}


void CGame::ReserveFightzoneResponseHandler(char * pData)
{
    WORD * wpResult;
    char * cp;
    int * ip;
    wpResult = (WORD *)(pData + DEF_INDEX2_MSGTYPE);

    
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
    WORD * wp;
    int j;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);

    if (*wp != DEF_MSGTYPE_REJECT)
    {
        
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
        cBankItemIndex = *cp;
        cp++;
        cItemIndex = *cp;
        cp++;

        if (m_pBankList[cBankItemIndex] != NULL)
        {
           
            char cStr1[64]{}, cStr2[64]{}, cStr3[64]{};
            GetItemName(m_pBankList[cBankItemIndex], cStr1, cStr2, cStr3, 64);

            ZeroMemory(cTxt, sizeof(cTxt));
            format_to_local(cTxt, RETIEVE_ITEM_HANDLER4, cStr1);
            AddEventList(cTxt, 10);

            if ((m_pBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_CONSUME) ||
                (m_pBankList[cBankItemIndex]->m_cItemType == DEF_ITEMTYPE_ARROW))
            {
                

                
                if (m_pItemList[cItemIndex] == NULL) goto RIH_STEP2;

                
                //m_pItemList[cItemIndex]->m_dwCount += m_pBankList[cBankItemIndex]->m_dwCount;

                delete m_pBankList[cBankItemIndex];
                m_pBankList[cBankItemIndex] = NULL;

                
                for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                {
                    if ((m_pBankList[j + 1] != NULL) && (m_pBankList[j] == NULL))
                    {
                        m_pBankList[j] = m_pBankList[j + 1];

                        m_pBankList[j + 1] = NULL;
                    }
                }
            }
            else
            {
                RIH_STEP2:;
                
                if (m_pItemList[cItemIndex] != NULL) return;
                
                short nX, nY;  
                nX = 40;
                nY = 30;
                for (j = 0; j < DEF_MAXITEMS; j++)
                {
                    if ((m_pItemList[j] != NULL) && (memcmp(m_pItemList[j]->m_cName, cStr1, 20) == 0))
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
                bSendCommand(MSGID_REQUEST_SETITEMPOS, NULL, cItemIndex, nX, nY, NULL, NULL);

                for (j = 0; j < DEF_MAXITEMS; j++)
                    if (m_cItemOrder[j] == -1)
                    {
                        m_cItemOrder[j] = cItemIndex;
                        break;
                    }
                //m_cItemOrder[cItemIndex] = cItemIndex;

                m_bIsItemEquipped[cItemIndex] = FALSE;
                m_bIsItemDisabled[cItemIndex] = FALSE;

                m_pBankList[cBankItemIndex] = NULL;

                
                for (j = 0; j <= DEF_MAXBANKITEMS - 2; j++)
                {
                    if ((m_pBankList[j + 1] != NULL) && (m_pBankList[j] == NULL))
                    {
                        m_pBankList[j] = m_pBankList[j + 1];

                        m_pBankList[j + 1] = NULL;
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

    ZeroMemory(cRewardName, sizeof(cRewardName));
    memcpy(cRewardName, cp, 20);
    cp += 20;

    ZeroMemory(cTargetName, sizeof(cTargetName));
    memcpy(cTargetName, cp, 20);
    cp += 20;

    EnableDialogBox(21, sResponse, sType, 0);

    if ((sType >= 1) && (sType <= 100))
    {
        
        
        iIndex = m_stDialogBoxInfo[21].sV1;
        m_pMsgTextList2[iIndex] = new CMsg(NULL, "  ", NULL);
        iIndex++;

        iQuestionType = NULL;
        switch (sType)
        {
        case 1: //Monster Hunt
            ZeroMemory(cTemp, sizeof(cTemp));
            GetNpcName(iTargetType, cTemp);
            ZeroMemory(cTxt, sizeof(cTxt));
#if DEF_LANGUAGE == 4	//¾ð¾î:English
            format_to_local(cTxt, NPC_TALK_HANDLER16, iTargetCount, cTemp);
#else
            format_to_local(cTxt, NPC_TALK_HANDLER16, cTemp, iTargetCount);
#endif
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            if (memcmp(cTargetName, "NONE", 4) == 0)
            {
                strcpy(cTxt, NPC_TALK_HANDLER17);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            else
            {
                ZeroMemory(cTemp, sizeof(cTemp));
                GetOfficialMapName(cTargetName, cTemp);
                format_to_local(cTxt, NPC_TALK_HANDLER18, cTemp);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;

                if (iX != 0)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER19, iX, iY, iRange);
                    m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                    iIndex++;
                }

                ZeroMemory(cTxt, sizeof(cTxt));
                format_to_local(cTxt, NPC_TALK_HANDLER20, iContribution);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            iQuestionType = 1;
            break;

        case 7: 
            ZeroMemory(cTxt, sizeof(cTxt));
            m_pMsgTextList2[iIndex] = new CMsg(NULL, NPC_TALK_HANDLER21, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            if (memcmp(cTargetName, "NONE", 4) == 0)
            {
                strcpy(cTxt, NPC_TALK_HANDLER22);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            else
            {
                ZeroMemory(cTemp, sizeof(cTemp));
                GetOfficialMapName(cTargetName, cTemp);
                format_to_local(cTxt, NPC_TALK_HANDLER23, cTemp);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;

                if (iX != 0)
                {
                    ZeroMemory(cTxt, sizeof(cTxt));
                    format_to_local(cTxt, NPC_TALK_HANDLER24, iX, iY, iRange);
                    m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                    iIndex++;
                }

                ZeroMemory(cTxt, sizeof(cTxt));
                format_to_local(cTxt, NPC_TALK_HANDLER25, iContribution);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            iQuestionType = 1;
            break;

        case 10: 
            ZeroMemory(cTxt, sizeof(cTxt));
            m_pMsgTextList2[iIndex] = new CMsg(NULL, NPC_TALK_HANDLER26, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER27);
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER28);
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER29);
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, NPC_TALK_HANDLER30);
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            strcpy(cTxt, " ");
            m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
            iIndex++;

            ZeroMemory(cTxt, sizeof(cTxt));
            if (memcmp(cTargetName, "NONE", 4) == 0)
            {
                strcpy(cTxt, NPC_TALK_HANDLER31);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            else
            {
                ZeroMemory(cTemp, sizeof(cTemp));
                GetOfficialMapName(cTargetName, cTemp);
                format_to_local(cTxt, NPC_TALK_HANDLER32, cTemp);
                m_pMsgTextList2[iIndex] = new CMsg(NULL, cTxt, NULL);
                iIndex++;
            }
            iQuestionType = 2;
            break;
        }

        switch (iQuestionType)
        {
        case 1:
            m_pMsgTextList2[iIndex] = new CMsg(NULL, "  ", NULL);
            iIndex++;
            m_pMsgTextList2[iIndex] = new CMsg(NULL, NPC_TALK_HANDLER33, NULL);
            iIndex++;
            m_pMsgTextList2[iIndex] = new CMsg(NULL, NPC_TALK_HANDLER34, NULL);
            iIndex++;
            m_pMsgTextList2[iIndex] = new CMsg(NULL, "  ", NULL);
            iIndex++;
            break;

        case 2:
            m_pMsgTextList2[iIndex] = new CMsg(NULL, "  ", NULL);
            iIndex++;
            m_pMsgTextList2[iIndex] = new CMsg(NULL, NPC_TALK_HANDLER35, NULL);
            iIndex++;
            m_pMsgTextList2[iIndex] = new CMsg(NULL, "  ", NULL);
            iIndex++;
            break;

        default: break;
        }
    }
}

void CGame::MotionResponseHandler(char * pData)
{
    WORD * wp, wResponse;
    short * sp, sX, sY;
    char * cp, cDir;
    int * ip, iPreHP;

    //						          0 3        4 5						 6 7		8 9		   10	    11	
    // Confirm Code(4) | MsgSize(4) | MsgID(4) | DEF_OBJECTMOVE_CONFIRM(2) | Loc-X(2) | Loc-Y(2) | Dir(1) | MapData ...
    // Confirm Code(4) | MsgSize(4) | MsgID(4) | DEF_OBJECTMOVE_REJECT(2)  | Loc-X(2) | Loc-Y(2)  

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
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
            m_sPlayerStatus, m_cPlayerName,
            DEF_OBJECTSTOP, NULL, NULL, NULL);
        m_cCommandCount = 0;
        
        m_bIsGetPointingMode = FALSE;

        
        m_sViewDstX = m_sViewPointX = (m_sPlayerX - 10) * 32;
        m_sViewDstY = m_sViewPointY = (m_sPlayerY - 7) * 32;

        m_bIsRedrawPDBGS = TRUE;
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

                if ((m_cLogOutCount > 0) && (m_bForceDisconn == FALSE))
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
        m_pMapData->ShiftMapData(cDir);
        _ReadMapData(sX, sY, cp);

        m_bIsRedrawPDBGS = TRUE;

        m_cCommandCount--;
        break;

    case DEF_OBJECTMOVE_REJECT:
        
        if (m_iHP <= 0) return;

        wp = (WORD *)cp;
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

        sp = (short *)cp;
        m_sPlayerStatus = *sp;
        cp += 2;

        m_cCommand = DEF_OBJECTSTOP;
        m_sCommX = m_sPlayerX;
        m_sCommY = m_sPlayerY;

        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
            m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
            m_sPlayerStatus, m_cPlayerName,
            DEF_OBJECTSTOP, NULL, NULL, NULL,
            0, 7);
        m_cCommandCount = 0;
        
        m_bIsGetPointingMode = FALSE;

        
        m_sViewDstX = m_sViewPointX = (m_sPlayerX - 10) * 32;
        m_sViewDstY = m_sViewPointY = (m_sPlayerY - 7) * 32;

       
        m_bIsPrevMoveBlocked = TRUE;

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
    BOOL  bIsObserverMode;
    HANDLE hFile;
    DWORD  dwFileSize;

    ZeroMemory(cPreCurLocation, sizeof(cPreCurLocation));
    
    m_bParalyze = FALSE;
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

    //m_bCommandAvailable = TRUE;
    m_cCommandCount = 0;

    m_bIsGetPointingMode = FALSE;
    m_iPointCommandType = -1;

    
    m_iIlusionOwnerH = NULL;
    m_cIlusionOwnerType = NULL;

   
    m_bIsTeleportRequested = FALSE;

    
    m_bIsConfusion = FALSE;
    
    m_bSkillUsingStatus = FALSE;
    
    m_bItemUsingStatus = FALSE;

    m_cRestartCount = -1;
    m_dwRestartCountTime = NULL;

    for (i = 0; i < DEF_MAXEFFECTS; i++)
    {
        if (m_pEffectList[i] != NULL) delete m_pEffectList[i];
        m_pEffectList[i] = NULL;
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
        ZeroMemory(m_stGuildName[i].cCharName, sizeof(m_stGuildName[i].cCharName));
        ZeroMemory(m_stGuildName[i].cGuildName, sizeof(m_stGuildName[i].cGuildName));
    }

    
    for (i = 0; i < DEF_MAXCHATMSGS; i++)
    {
        if (m_pChatMsgList[i] != NULL) delete m_pChatMsgList[i];
        m_pChatMsgList[i] = NULL;
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
    m_sPlayerStatus = *ip;
    cp += 4;

    
    ZeroMemory(m_cMapName, sizeof(m_cMapName));
    ZeroMemory(m_cMapMessage, sizeof(m_cMapMessage));
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
    ZeroMemory(m_cCurLocation, sizeof(m_cCurLocation));
    memcpy(m_cCurLocation, cp, 10);
    cp += 10;

    
    G_cSpriteAlphaDegree = *cp;
    cp++;

    
    m_cWhetherStatus = *cp;
    cp++;

   
    ip = (int *)cp;
    m_iContribution = *ip;
    cp += 4;

   
    bIsObserverMode = (BOOL)*cp;
    cp++;

    ip = (int *)cp;
    //m_iRating = *ip;
    cp += 4;

   
    ip = (int *)cp;
    m_iHP = *ip;
    cp += 4;

    m_cDiscount = (char)*cp;
    cp++;

    if (m_cWhetherStatus != NULL)
        SetWhetherStatus(TRUE, m_cWhetherStatus);
    else SetWhetherStatus(FALSE, m_cWhetherStatus);

    ZeroMemory(cMapFileName, sizeof(cMapFileName));
    strcat(cMapFileName, "data\\mapdata\\");
    strcat(cMapFileName, m_cMapName);
    strcat(cMapFileName, ".amd");
    m_pMapData->OpenMapDataFile(cMapFileName);

    m_pMapData->m_sPivotX = sX;
    m_pMapData->m_sPivotY = sY;
    

    m_sPlayerX = sX + 14 + 5;
    m_sPlayerY = sY + 12 + 5;

    m_cPlayerDir = 5;

    
    if (bIsObserverMode == FALSE)
    {
        m_pMapData->bSetOwner(m_sPlayerObjectID, m_sPlayerX, m_sPlayerY, m_sPlayerType, m_cPlayerDir,
            m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4, m_iPlayerApprColor,
            m_sPlayerStatus, m_cPlayerName,
            DEF_OBJECTSTOP, NULL, NULL, NULL);
    }

    
    m_sViewDstX = m_sViewPointX = (sX + 4 + 5) * 32;
    m_sViewDstY = m_sViewPointY = (sY + 5 + 5) * 32;

    
    _ReadMapData(sX + 4 + 5, sY + 5 + 5, cp);

    m_bIsRedrawPDBGS = TRUE;

    
    format_to_local(cTxt, INITDATA_RESPONSE_HANDLER1, m_cMapMessage);
    AddEventList(cTxt, 10);

#if DEF_LANGUAGE > 2
    m_stDialogBoxInfo[6].sX = 150;
    m_stDialogBoxInfo[6].sY = 130;

    
    if ((memcmp(m_cCurLocation, "middleland", 10) == 0) ||
        (memcmp(m_cCurLocation, "dglv2", 5) == 0) ||
        (memcmp(m_cCurLocation, "middled1n", 9) == 0))
        EnableDialogBox(6, NULL, NULL, NULL);

    BOOL bPrevSafe, bNowSafe;
    if (memcmp(cPreCurLocation, m_cLocation, 3) == 0)
        bPrevSafe = TRUE;
    else bPrevSafe = FALSE;

    if (memcmp(m_cCurLocation, m_cLocation, 3) == 0)
        bNowSafe = TRUE;
    else bNowSafe = FALSE;

    if (memcmp(m_cCurLocation, "2nd", 3) == 0) bNowSafe = TRUE;
    if (m_iPKCount != 0) bNowSafe = FALSE;

    if (bPrevSafe)
    {
        if (bNowSafe == FALSE) SetTopMsg(DEF_MSG_DANGERZONE, 5);
    }
    else
    {
        if (bNowSafe) SetTopMsg(DEF_MSG_SAFEZONE, 5);
    }

#endif

    // ------------------------------------------------------------------------+

    
    ChangeGameMode(DEF_GAMEMODE_ONMAINGAME);

   
    if ((m_sPlayerAppr2 & 0xF000) != 0)
        m_bIsCombatMode = TRUE;
    else m_bIsCombatMode = FALSE;

   
    if (m_bIsFirstConn == TRUE)
    {
        m_bIsFirstConn = FALSE;

        
        hFile = CreateFileA("data\\contents\\contents1000.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            dwFileSize = 0;
        else
        {
            dwFileSize = GetFileSize(hFile, NULL);
            CloseHandle(hFile);
        }

        bSendCommand(MSGID_REQUEST_NOTICEMENT, NULL, NULL, (int)dwFileSize, NULL, NULL, NULL);
    }


    //cp += 2;
}

void CGame::MotionEventHandler(char * pData)
{
    WORD * wp, wEventType{}, wObjectID{};
    short * sp, sX{}, sY{}, sType{}, sAppr1{}, sAppr2{}, sAppr3{}, sAppr4{}, sV1{}, sV2{}, sV3{}, sPrevAppr2{};
    char * cp, cDir{}, cName[12]{};
    int sStatus{};
    int * ip, iApprColor{}, iLoc{};
    char    cTxt[120]{};
    int i;

    ZeroMemory(cName, sizeof(cName));
    sV1 = sV2 = sV3 = NULL;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    wEventType = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    wp = (WORD *)cp;
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
            sStatus = *ip;
            cp += 4;

            iLoc = *cp;
            cp++;
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
            sStatus = *ip;
            cp += 4;

            iLoc = *cp;
            cp++;
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
        m_sPlayerStatus = sStatus;

        if ((sPrevAppr2 & 0xF000) == 0)
        {
            if ((sAppr2 & 0xF000) != 0)
            {
                AddEventList(MOTION_EVENT_HANDLER1, 10); 
                m_bIsCombatMode = TRUE;
            }
        }
        else
        {
            if ((sAppr2 & 0xF000) == 0)
            {
                AddEventList(MOTION_EVENT_HANDLER2, 10); 
                m_bIsCombatMode = FALSE;
            }
        }

        
        if (m_cCommand != DEF_OBJECTRUN) m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName, (char)wEventType, sV1, sV2, sV3, iLoc);
    }
    else m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName, (char)wEventType, sV1, sV2, sV3, iLoc);

    switch (wEventType)
    {
    case DEF_OBJECTMAGIC:
        
        _RemoveChatMsgListByObjectID(wObjectID - 30000);

        for (i = 1; i < DEF_MAXCHATMSGS; i++)
            if (m_pChatMsgList[i] == NULL)
            {
                ZeroMemory(cTxt, sizeof(cTxt));
                format_to_local(cTxt, "{}!", m_pMagicCfgList[sV1]->m_cName);
                m_pChatMsgList[i] = new CMsg(chat_types::magic, cTxt, m_dwCurTime);
                m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;
                if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
                {
                    delete m_pChatMsgList[i];
                    m_pChatMsgList[i] = NULL;
                }
                return;
            }
        break;

    case DEF_OBJECTDYING:
        _RemoveChatMsgListByObjectID(wObjectID - 30000);

        for (i = 1; i < DEF_MAXCHATMSGS; i++)
            if (m_pChatMsgList[i] == NULL)
            {
                ZeroMemory(cTxt, sizeof(cTxt));
                if (sV1 > 0)
                    format_to_local(cTxt, "-{}Pts!", sV1); //pts
                else strcpy(cTxt, "Critical!");

                int iFontType;
                if ((sV1 >= 0) && (sV1 < 12))		iFontType = chat_types::small_damage;
                else if ((sV1 >= 12) && (sV1 < 40)) iFontType = chat_types::medium_damage;
                else if ((sV1 >= 40) || (sV1 < 0))	iFontType = chat_types::critical_damage;

                m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;

                if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
                {
                    delete m_pChatMsgList[i];
                    m_pChatMsgList[i] = NULL;
                }
                return;
            }
        break;

    case DEF_OBJECTDAMAGEMOVE:
    case DEF_OBJECTDAMAGE:
        
        if (memcmp(cName, m_cPlayerName, 10) == 0)
        {
            m_bIsGetPointingMode = FALSE;
            m_iPointCommandType = -1; 
           
            m_stMCursor.sCursorFrame = 0;

            
            ClearSkillUsingStatus();
        }

        
        _RemoveChatMsgListByObjectID(wObjectID - 30000);

        for (i = 1; i < DEF_MAXCHATMSGS; i++)
            if (m_pChatMsgList[i] == NULL)
            {
                ZeroMemory(cTxt, sizeof(cTxt));

                if (sV1 != 0)
                {
                    if (sV1 > 0)
                        format_to_local(cTxt, "-{}Pts", sV1); //pts
                    else strcpy(cTxt, "Critical!");

                    int iFontType;
                    if ((sV1 >= 0) && (sV1 < 12))		iFontType = chat_types::small_damage;
                    else if ((sV1 >= 12) && (sV1 < 40)) iFontType = chat_types::medium_damage;
                    else if ((sV1 >= 40) || (sV1 < 0))	iFontType = chat_types::critical_damage;

                    m_pChatMsgList[i] = new CMsg(iFontType, cTxt, m_dwCurTime);
                }
                else
                {
                    strcpy(cTxt, " * Failed! *");
                    m_pChatMsgList[i] = new CMsg(chat_types::medium_damage, cTxt, m_dwCurTime);
                    PlaySound('C', 17, 0);
                }
                m_pChatMsgList[i]->m_iObjectID = wObjectID - 30000;

                if (m_pMapData->bSetChatMsgOwner(wObjectID - 30000, -10, -10, i) == FALSE)
                {
                    delete m_pChatMsgList[i];
                    m_pChatMsgList[i] = NULL;
                }
                return;
            }
        break;
    }
}

void CGame::CommonEventHandler(char * pData)
{
    WORD * wp, wEventType;
    short * sp, sX, sY, sV1, sV2, sV3, sV4;
    char * cp;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
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
            bAddNewEffect(4, sX, sY, NULL, NULL, 0);
        }
        //else

        m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3); 
        break;

    case DEF_COMMONTYPE_SETITEM:				
        m_pMapData->bSetItem(sX, sY, sV1, sV2, (char)sV3, FALSE);
        break;

    case DEF_COMMONTYPE_MAGIC:
        bAddNewEffect(sV3, sX, sY, sV1, sV2, 0, sV4); 
        break;
    case DEF_COMMONTYPE_CLEARGUILDNAME:
        ClearGuildNameList();
        break;
    }
}

void CGame::CreateNewGuildResponseHandler(char * pData)
{
    WORD * wpResult;

    wpResult = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    
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
    WORD * wpResult;

    wpResult = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    
    switch (*wpResult)
    {
    case DEF_MSGTYPE_CONFIRM:
        
        ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
        m_iGuildRank = -1;
        m_stDialogBoxInfo[7].cMode = 7;
        break;

    case DEF_MSGTYPE_REJECT:
        
        m_stDialogBoxInfo[7].cMode = 8;
        break;
    }
}

bool CGame::GameRecvMsgHandler(char * pData, uint64_t size)
{
    uint32_t * dwpMsgID;
    dwpMsgID = (uint32_t *)(pData + DEF_INDEX4_MSGID);

    std::cout << "Received " << std::hex << *dwpMsgID << " - " << std::dec << size << " bytes of data\n";

    if (*dwpMsgID == MSGID_EVENT_MOTION)
    {
        MotionEventHandler(pData);
        return true;
    }

    uint8_t v = 0;

    switch (*dwpMsgID)
    {
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
        return true;

    case MSGID_RESPONSE_MOTION:
        MotionResponseHandler(pData);
        return true;

    case MSGID_EVENT_COMMON:
        CommonEventHandler(pData);
        return true;

    case MSGID_EVENT_LOG:
        LogEventHandler(pData);
        return true;

    case MSGID_COMMAND_CHATMSG:
        ChatMsgHandler(pData);
        return true;

    case MSGID_PLAYERITEMLISTCONTENTS:
        InitItemList(pData);
        return true;

    case MSGID_NOTIFY:
        NotifyMsgHandler(pData);
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

void CGame::ConnectionEstablishHandler(char cWhere)
{
    if (m_bIsCheckingGateway == FALSE)
        ChangeGameMode(DEF_GAMEMODE_ONWAITINGRESPONSE);

    switch (cWhere)
    {
    case DEF_SERVERTYPE_GAME:
        
        bSendCommand(MSGID_REQUEST_INITDATA, NULL, NULL, NULL, NULL, NULL, NULL);
        ChangeGameMode(DEF_GAMEMODE_ONWAITINGINITDATA);
        break;

    case DEF_SERVERTYPE_LOG:
        if (m_bIsCheckingGateway == TRUE)
        {
            
            bSendCommand(MSGID_GETMINIMUMLOADGATEWAY, NULL, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
           
            switch (m_dwConnectMode)
            {
            case MSGID_REQUEST_LOGIN:
                bSendCommand(MSGID_REQUEST_LOGIN, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_CREATENEWACCOUNT:
                
                bSendCommand(MSGID_REQUEST_CREATENEWACCOUNT, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_CREATENEWCHARACTER:
                bSendCommand(MSGID_REQUEST_CREATENEWCHARACTER, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_ENTERGAME:
                bSendCommand(MSGID_REQUEST_ENTERGAME, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_DELETECHARACTER:
                bSendCommand(MSGID_REQUEST_DELETECHARACTER, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_CHANGEPASSWORD:
                bSendCommand(MSGID_REQUEST_CHANGEPASSWORD, NULL, NULL, NULL, NULL, NULL, NULL);
                break;

            case MSGID_REQUEST_INPUTKEYCODE:
                bSendCommand(MSGID_REQUEST_INPUTKEYCODE, NULL, NULL, NULL, NULL, NULL, NULL);
                break;
            }
        }
        break;
    }
}

void CGame::LogEventHandler(char * pData)
{
    WORD * wp, wEventType, wObjectID;
    short * sp, sX, sY, sType, sAppr1, sAppr2, sAppr3, sAppr4;
    char * cp, cDir, cName[12];
    int * ip, iApprColor, sStatus;

    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    wEventType = *wp;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    wp = (WORD *)cp;
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

    ZeroMemory(cName, sizeof(cName));
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
        sStatus = *ip;
        cp += 4;
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
        sStatus = * ip;
        cp += 4;
    }

    switch (wEventType)
    {
    case DEF_MSGTYPE_CONFIRM:
        
        m_pMapData->bSetOwner(wObjectID, sX, sY, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName, DEF_OBJECTSTOP, NULL, NULL, NULL);
        switch (sType)
        {
        case 43: 
        case 44:
        case 45:
        case 46:
        case 47:
            bAddNewEffect(64, (sX) * 32, (sY) * 32, NULL, NULL, 0);
            break;
        }
        break;

    case DEF_MSGTYPE_REJECT:
        
        m_pMapData->bSetOwner(wObjectID, -1, -1, sType, cDir, sAppr1, sAppr2, sAppr3, sAppr4, iApprColor, sStatus, cName, DEF_OBJECTSTOP, NULL, NULL, NULL);
        break;
    }

    
    _RemoveChatMsgListByObjectID(wObjectID);
}

void CGame::LogResponseHandler(char * pData, uint64_t size)
{
    stream_read sr(pData, size);
    WORD * wp, wResponse;
    WORD wServerUpperVersion = {}, wServerLowerVersion = {}, wServerPatchVersion = {};
    DWORD * dwp;
    char * cp, cCharName[12];
    int * ip, i;

    dwp = (DWORD *)(pData);
    wp = (WORD *)(pData + DEF_INDEX2_MSGTYPE);
    wResponse = *wp;
    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    uint32_t unused = sr.read_int32();
    wResponse = sr.read_uint16();
    if (GameRecvMsgHandler(pData, size)) return;

    switch (wResponse)
    {
    case DEF_LOGRESMSGTYPE_CHARACTERDELETED:
        
        cp = (pData + DEF_INDEX2_MSGTYPE + 2);

        
        cp++;

        m_iTotalChar = (int)*cp;
        cp++;

        for (i = 0; i < 4; i++)
            if (m_pCharList[i] != NULL)
            {
                delete m_pCharList[i];
                m_pCharList[i] = NULL;
            }

        for (i = 0; i < m_iTotalChar; i++)
        {
            m_pCharList[i] = new CCharInfo;
            memcpy(m_pCharList[i]->m_cName, cp, 10);
            cp += 10;
            if (*cp == 0)
            {
                
                m_pCharList[i]->m_sSex = NULL; 
                cp += 40;
            }
            else
            {
                cp++;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr1 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr2 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr3 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr4 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sSex = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sSkinCol = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sLevel = *wp;
                cp += 2;

                dwp = (DWORD *)cp;
                m_pCharList[i]->m_iExp = *dwp;
                cp += 4;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sStr = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sVit = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sDex = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sInt = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sMag = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sChr = *wp;
                cp += 2;

                ip = (int *)cp;
                m_pCharList[i]->m_iApprColor = *ip;
                cp += 4;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iYear = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iMonth = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iDay = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iHour = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iMinute = (int)*wp;
                cp += 2;

                ZeroMemory(m_pCharList[i]->m_cMapName, sizeof(m_pCharList[i]->m_cMapName));
                memcpy(m_pCharList[i]->m_cMapName, cp, 10);
                cp += 10;
            }
        }
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "3A");
        break;

    case DEF_LOGRESMSGTYPE_CONFIRM:
        loggedin = true;
        sr.reset_pos();
        sr.read_int32();
        sr.read_int16();
        wServerUpperVersion = sr.read_uint16();
        wServerLowerVersion = sr.read_uint16();
        wServerPatchVersion = sr.read_uint16();
        /*m_iAccountStatus = */(int)sr.read_byte();
        m_iAccntYear = sr.read_uint16();
        m_iAccntMonth = sr.read_uint16();
        m_iAccntDay = sr.read_uint16();
        m_iIpYear = sr.read_uint16();
        m_iIpMonth = sr.read_uint16();
        m_iIpDay = sr.read_uint16();
        m_iTotalChar = (int)sr.read_byte();
        //m_pCharList.clear();

        if (m_iTotalChar > 10) //error
        {
            m_iTotalChar = 10;
        }

        for (i = 0; i < 4; i++)
            if (m_pCharList[i] != NULL)
            {
                delete m_pCharList[i];
                m_pCharList[i] = NULL;
            }

        for (int i = 0; i < m_iTotalChar; i++)
        {
            //std::shared_ptr<CCharInfo> character = std::make_shared<CCharInfo>();
            m_pCharList[i] = new CCharInfo;
            auto * character = m_pCharList[i];
            ZeroMemory(m_pCharList[i]->m_cName, 20);
            memcpy(m_pCharList[i]->m_cName, sr.read_string(10).c_str(), 10);
            if (sr.read_byte() == 0)
            {
                character->m_sSex = 0;
                sr.position += 39;
            }
            else
            {
                //character->id = sr.read_int64();
                character->m_sAppr1 = sr.read_uint16();
                character->m_sAppr2 = sr.read_uint16();
                character->m_sAppr3 = sr.read_uint16();
                character->m_sAppr4 = sr.read_uint16();
//                 character->m_sHeadApprValue = sr.ReadShort();
//                 character->m_sBodyApprValue = sr.ReadShort();
//                 character->m_sArmApprValue = sr.ReadShort();
//                 character->m_sLegApprValue = sr.ReadShort();
                character->m_sSex = sr.read_uint16();
                character->m_sSkinCol = sr.read_uint16();
                character->m_sLevel = sr.read_uint16();
                character->m_iExp = sr.read_uint32();
                character->m_sStr = sr.read_uint16();
                character->m_sVit = sr.read_uint16();
                character->m_sDex = sr.read_uint16();
                character->m_sInt = sr.read_uint16();
                character->m_sMag = sr.read_uint16();
                character->m_sChr = sr.read_uint16();
                character->m_iApprColor = sr.read_uint32();
                character->m_iYear = sr.read_uint16();
                character->m_iMonth = sr.read_uint16();
                character->m_iDay = sr.read_uint16();
                character->m_iHour = sr.read_uint16();
                character->m_iMinute = sr.read_uint16();
                ZeroMemory(m_pCharList[i]->m_cMapName, 20);
                memcpy(m_pCharList[i]->m_cMapName, sr.read_string(10).c_str(), 10);
            }
            //m_pCharList.push_back(character);
            //m_pCharList[i] = character;
        }
        m_iTimeLeftSecAccount = sr.read_uint32();
        m_iTimeLeftSecIP = sr.read_uint32();
//         for (unsigned char & i : key)
//             i = sr.read_byte();
//         has_key = true;
        ChangeGameMode(DEF_GAMEMODE_ONSELECTCHARACTER);
        ClearContents_OnSelectCharacter();

#ifndef _DEBUG
        if (wServerUpperVersion != DEF_UPPERVERSION || wServerLowerVersion != DEF_LOWERVERSION || wServerPatchVersion != DEF_PATCHVERSION)
            ChangeGameMode(DEF_GAMEMODE_ONVERSIONNOTMATCH);
#endif
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
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "7H");
        break;

        
    case DEF_LOGRESMSGTYPE_NOTENOUGHPOINT:
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "7I");
        break;

        
    case DEF_LOGRESMSGTYPE_ACCOUNTLOCKED:
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "7K");
        break;

        
    case DEF_LOGRESMSGTYPE_SERVICENOTAVAILABLE:
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "7L");
        break;

    case DEF_LOGRESMSGTYPE_PASSWORDCHANGESUCCESS:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "6B");
        break;

    case DEF_LOGRESMSGTYPE_PASSWORDCHANGEFAIL:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "6C");
        break;

    case DEF_LOGRESMSGTYPE_PASSWORDMISMATCH:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "11");
        break;

    case DEF_LOGRESMSGTYPE_NOTEXISTINGACCOUNT:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "12");
        break;

    case DEF_LOGRESMSGTYPE_NEWACCOUNTCREATED:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "54");
        break;

    case DEF_LOGRESMSGTYPE_NEWACCOUNTFAILED:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "05");
        break;

    case DEF_LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "06");
        break;

    case DEF_LOGRESMSGTYPE_NOTEXISTINGCHARACTER:
        
        ChangeGameMode(DEF_GAMEMODE_ONMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "Not existing character!");
        break;

    case DEF_LOGRESMSGTYPE_NEWCHARACTERCREATED:
        
        
        ZeroMemory(cCharName, sizeof(cCharName));
        memcpy(cCharName, cp, 10);
        cp += 10;

        m_iTotalChar = (int)*cp;
        cp++;

        for (i = 0; i < 4; i++)
            if (m_pCharList[i] != NULL) delete m_pCharList[i];
       
        for (i = 0; i < m_iTotalChar; i++)
        {
            m_pCharList[i] = new CCharInfo;
            memcpy(m_pCharList[i]->m_cName, cp, 10);
            cp += 10;
            if (*cp == 0)
            {
                
                m_pCharList[i]->m_sSex = NULL; 
                cp += 40;
            }
            else
            {
                cp++;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr1 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr2 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr3 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sAppr4 = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sSex = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sSkinCol = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sLevel = *wp;
                cp += 2;

                dwp = (DWORD *)cp;
                m_pCharList[i]->m_iExp = *dwp;
                cp += 4;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sStr = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sVit = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sDex = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sInt = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sMag = *wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_sChr = *wp;
                cp += 2;

                ip = (int *)cp;
                m_pCharList[i]->m_iApprColor = *ip;
                cp += 4;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iYear = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iMonth = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iDay = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iHour = (int)*wp;
                cp += 2;

                wp = (WORD *)cp;
                m_pCharList[i]->m_iMinute = (int)*wp;
                cp += 2;

                ZeroMemory(m_pCharList[i]->m_cMapName, sizeof(m_pCharList[i]->m_cMapName));
                memcpy(m_pCharList[i]->m_cMapName, cp, 10);
                cp += 10;
            }
        }
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "47");
        break;

    case DEF_LOGRESMSGTYPE_NEWCHARACTERFAILED:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "28");
        break;

    case DEF_LOGRESMSGTYPE_ALREADYEXISTINGCHARACTER:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "29");
        break;

    case DEF_ENTERGAMERESTYPE_PLAYING:
        
        ChangeGameMode(DEF_GAMEMODE_ONQUERYFORCELOGIN);
        break;

    case DEF_ENTERGAMERESTYPE_CONFIRM:
    {
        socketmode(true);
        printf("ENTERGAMERESTYPE_CONFIRM\n");
        ConnectionEstablishHandler(DEF_SERVERTYPE_GAME);
    }
    break;

    case DEF_ENTERGAMERESTYPE_REJECT:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);

        
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
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
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "3X");
        break;

    case DEF_LOGRESMSGTYPE_NOTEXISTINGWORLDSERVER:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "1Y");
        break;
        
    case DEF_LOGRESMSGTYPE_INPUTKEYCODE:
        
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);
        switch (*cp)
        {
        case 1:	strcpy(m_cMsg, "8U"); break; //MainMenu, Keycode registration success
        case 2:	strcpy(m_cMsg, "82"); break; //MainMenu, Not existing Account
        case 3:	strcpy(m_cMsg, "81"); break; //MainMenu, Password wrong
        case 4: strcpy(m_cMsg, "8V"); break; //MainMenu, Invalid Keycode
        case 5: strcpy(m_cMsg, "8W"); break; //MainMenu, Already Used Keycode
        }
        break;

        
    case DEF_LOGRESMSGTYPE_FORCECHANGEPASSWORD:
        //		ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        //		ZeroMemory(m_cMsg, sizeof(m_cMsg));
        //		strcpy(m_cMsg, "2M");

                // 2002-10-17 #1
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "6M");
        break;

    case DEF_LOGRESMSGTYPE_INVALIDKOREANSSN:	
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "1a");
        break;

    case DEF_LOGRESMSGTYPE_LESSTHENFIFTEEN:		
        ChangeGameMode(DEF_GAMEMODE_ONLOGRESMSG);
        ZeroMemory(m_cMsg, sizeof(m_cMsg));
        strcpy(m_cMsg, "1b");
        break;

    }
}

void CGame::InitPlayerCharacteristics(char * pData)
{
    int * ip;
    char * cp;
    short * sp;

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
    m_iAC = *ip; // defense ratio
    cp += 4;

    ip = (int *)cp;
    m_iTHAC0 = *ip; // hit ratio
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

    sp = (short *)cp;
    m_iLU_Point = *sp;
    cp += 2;

    // ????
    cp += 5;

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
        m_bAresden = TRUE;
        m_bCitizen = TRUE;
        m_bHunter = FALSE;
    }
    else if (memcmp(m_cLocation, "arehunter", 9) == 0)
    {
        m_bAresden = TRUE;
        m_bCitizen = TRUE;
        m_bHunter = TRUE;
    }
    else if (memcmp(m_cLocation, "elvine", 6) == 0)
    {
        m_bAresden = FALSE;
        m_bCitizen = TRUE;
        m_bHunter = FALSE;
    }
    else if (memcmp(m_cLocation, "elvhunter", 9) == 0)
    {
        m_bAresden = FALSE;
        m_bCitizen = TRUE;
        m_bHunter = TRUE;
    }
    else
    {
        m_bAresden = TRUE;
        m_bCitizen = FALSE;
        m_bHunter = TRUE;
    }

    cp = (char *)cp;
    memcpy(m_cGuildName, cp, 20);
    cp += 20;

    if (strcmp(m_cGuildName, "NONE") == 0)
        ZeroMemory(m_cGuildName, sizeof(m_cGuildName));

    m_Misc.ReplaceString(m_cGuildName, '_', ' ');

    ip = (int *)cp;
    m_iGuildRank = *ip;
    cp += 4;

   
    m_iSuperAttackLeft = (int)*cp;
    cp++;

    
    ip = (int *)cp;
    m_iFightzoneNumber = *ip;
    cp += 4;
}

void CGame::AddMapStatusInfo(char * pData, BOOL bIsLastData)
{
    char * cp, cTotal;
    short * sp, sIndex;
    int i;

    ZeroMemory(m_cStatusMapName, sizeof(m_cStatusMapName));

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

    if (bIsLastData == TRUE)
    {
        
        while (sIndex < DEF_MAXCRUSADESTRUCTURES)
        {
            m_stCrusadeStructureInfo[sIndex].cType = NULL;
            m_stCrusadeStructureInfo[sIndex].sX = NULL;
            m_stCrusadeStructureInfo[sIndex].sY = NULL;
            m_stCrusadeStructureInfo[sIndex].cSide = NULL;
            sIndex++;
        }
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
        ZeroMemory(m_stTeleportList[i].mapname, sizeof(m_stTeleportList[i].mapname));
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
