//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "Game.h"
#include "StrTok.h"
#include "Item.h"
#include "ItemNAme.h"
#include "Msg.h"
#include "BuildItem.h"
#include "Magic.h"
#include "Skill.h"

void CGame::_LoadShopMenuContents(char cType)
{
    char cFileName[255]{}, cTemp[255]{};
    HANDLE hFile{};
    FILE * pFile{};
    uint32_t  dwFileSize{};
    char * pBuffer{};

    memset(cTemp, 0, sizeof(cTemp));
    memset(cFileName, 0, sizeof(cFileName));
    format_to_local(cFileName, "data\\shops\\contents{}.txt", cType);

    hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFileName, "rt");
    if (pFile == 0) return;
    else
    {
        pBuffer = new char[dwFileSize + 1];
        memset(pBuffer, 0, dwFileSize + 1);
        fread(pBuffer, dwFileSize, 1, pFile);

        __bDecodeContentsAndBuildItemForSaleList(pBuffer);
        delete[] pBuffer;
    }
    fclose(pFile);
}

bool CGame::_bDecodeBuildItemContents()
{
    char cFileName[255]{}, cTemp[255]{};
    HANDLE hFile{};
    FILE * pFile{};
    uint32_t  dwFileSize{};
    char * pBuffer{};
    bool   bRet{};
    int    i{};


    for (i = 0; i < DEF_MAXBUILDITEMS; i++)
        if (m_pBuildItemList[i] != 0)
        {
            delete m_pBuildItemList[i];
            m_pBuildItemList[i] = 0;
        }

    memset(cTemp, 0, sizeof(cTemp));
    memset(cFileName, 0, sizeof(cFileName));


    format_to_local(cFileName, "data\\shops\\BItemcfg.txt");

    hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFileName, "rt");
    if (pFile == 0) return false;
    else
    {
        pBuffer = new char[dwFileSize + 1];
        memset(pBuffer, 0, dwFileSize + 1);
        fread(pBuffer, dwFileSize, 1, pFile);

        bRet = __bDecodeBuildItemContents(pBuffer);
        delete[] pBuffer;
    }

    fclose(pFile);

    return bRet;
}

bool CGame::__bDecodeContentsAndBuildItemForSaleList(char * pBuffer)
{
    char * pContents, * token;
    char seps[] = "= ,\t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iItemForSaleListIndex = 0;
    class CStrTok * pStrTok;

    pContents = pBuffer;

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != 0)
    {
        if (cReadModeA != 0)
        {

            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:

                            memset(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, 0, sizeof(m_pItemForSaleList[iItemForSaleListIndex]->m_cName));
                            memcpy(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 2;
                            break;
                        case 2:
                            // m_cItemType
                            m_pItemForSaleList[iItemForSaleListIndex]->m_cItemType = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            // m_cEquipPos
                            m_pItemForSaleList[iItemForSaleListIndex]->m_cEquipPos = atoi(token);
                            cReadModeB = 4;
                            break;
                        case 4:
                            // m_sItemEffectType

                            cReadModeB = 5;
                            break;
                        case 5:
                            // m_sItemEffectValue1
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue1 = atoi(token);
                            cReadModeB = 6;
                            break;
                        case 6:
                            // m_sItemEffectValue2
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue2 = atoi(token);
                            cReadModeB = 7;
                            break;
                        case 7:
                            // m_sItemEffectValue3
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue3 = atoi(token);
                            cReadModeB = 8;
                            break;
                        case 8:
                            // m_sItemEffectValue4
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue4 = atoi(token);
                            cReadModeB = 9;
                            break;
                        case 9:
                            // m_sItemEffectValue5
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue5 = atoi(token);
                            cReadModeB = 10;
                            break;
                        case 10:
                            // m_sItemEffectValue6
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue6 = atoi(token);
                            cReadModeB = 11;
                            break;
                        case 11:
                            // m_wMaxLifeSpan
                            m_pItemForSaleList[iItemForSaleListIndex]->m_wMaxLifeSpan = (uint16_t)atoi(token);
                            cReadModeB = 12;
                            break;
                        case 12:
                            // m_sMaxFixCount
                            //m_pItemForSaleList[iItemForSaleListIndex]->m_sMaxFixCount = atoi(token);
                            cReadModeB = 13;
                            break;
                        case 13:
                            // m_sSprite
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sSprite = atoi(token);
                            cReadModeB = 14;
                            break;
                        case 14:
                            // m_sSpriteFrame
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sSpriteFrame = atoi(token);
                            cReadModeB = 15;
                            break;
                        case 15:
                            // m_wPrice
                            m_pItemForSaleList[iItemForSaleListIndex]->m_wPrice = atoi(token);
                            cReadModeB = 16;
                            break;
                        case 16:
                            // m_wWeight
                            m_pItemForSaleList[iItemForSaleListIndex]->m_wWeight = atoi(token);
                            cReadModeB = 17;
                            break;
                        case 17:
                            // Appr Value
                            //m_pItemForSaleList[iItemForSaleListIndex]->m_cApprValue = atoi(token);
                            cReadModeB = 18;
                            break;
                        case 18:
                            // m_cSpeed
                            m_pItemForSaleList[iItemForSaleListIndex]->m_cSpeed = atoi(token);
                            cReadModeB = 19;
                            break;

                        case 19:
                            // Level Limit
                            m_pItemForSaleList[iItemForSaleListIndex]->m_sLevelLimit = atoi(token);
                            m_pItemForSaleList[iItemForSaleListIndex]->m_dwCount = 1;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            iItemForSaleListIndex++;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "ItemForSale", 4) == 0)
            {

                if (iItemForSaleListIndex >= DEF_MAXMENUITEMS)
                {
                    delete pStrTok;
                    return false;
                }
                cReadModeA = 1;
                cReadModeB = 1;
                m_pItemForSaleList[iItemForSaleListIndex] = new CItem;
            }

        }
        token = pStrTok->pGet();
    }
    delete pStrTok;

    if ((cReadModeA != 0) || (cReadModeB != 0)) return false;
    return true;
}

void CGame::_LoadTextDlgContents(int cType)
{
    char * pContents, * token, cTemp[120]{}, cFileName[120]{};
    char   seps[] = "\n";
    int    iIndex = 0, i{};
    CStrTok * pStrTok{};
    uint32_t  dwFileSize{};
    HANDLE hFile{};
    FILE * pFile{};

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList[i] != 0)
            delete m_pMsgTextList[i];
        m_pMsgTextList[i] = 0;
    }


    memset(cTemp, 0, sizeof(cTemp));
    memset(cFileName, 0, sizeof(cFileName));

    format_to_local(cFileName, "data\\shops\\contents{}.txt", cType);

    hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFileName, "rt");
    if (pFile == 0) return;
    else
    {
        pContents = new char[dwFileSize + 1];
        memset(pContents, 0, dwFileSize + 1);
        fread(pContents, dwFileSize, 1, pFile);
    }

    fclose(pFile);
    ////

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != 0)
    {
        m_pMsgTextList[iIndex] = new CMsg(0, token, 0);
        token = pStrTok->pGet();
        iIndex++;
    }

    delete pStrTok;
    delete[] pContents;
}

int CGame::_iLoadTextDlgContents2(int iType)
{
    char * pContents, * token, cTemp[120]{}, cFileName[120]{};
    char   seps[] = "\n";
    int    iIndex = 0, i{};
    CStrTok * pStrTok{};
    uint32_t  dwFileSize{};
    HANDLE hFile{};
    FILE * pFile{};

    for (i = 0; i < DEF_TEXTDLGMAXLINES; i++)
    {
        if (m_pMsgTextList2[i] != 0)
            delete m_pMsgTextList2[i];
        m_pMsgTextList2[i] = 0;
    }


    memset(cTemp, 0, sizeof(cTemp));
    memset(cFileName, 0, sizeof(cFileName));

    format_to_local(cFileName, "data\\shops\\contents{}.txt", iType);

    hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFileName, "rt");
    if (pFile == 0) return -1;
    else
    {
        pContents = new char[dwFileSize + 1];
        if (pContents == 0) return -1;

        memset(pContents, 0, dwFileSize + 1);
        fread(pContents, dwFileSize, 1, pFile);
    }
    fclose(pFile);

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != 0)
    {
        m_pMsgTextList2[iIndex] = new CMsg(0, token, 0);
        token = pStrTok->pGet();
        iIndex++;
    }

    delete pStrTok;
    delete[] pContents;

    return iIndex;
}

void CGame::_LoadGameMsgTextContents()
{
    char * pContents, * token, cTemp[120]{}, cFileName[120]{};
    char   seps[] = ";\n";
    int    iIndex = 0, i{};
    CStrTok * pStrTok{};
    uint32_t  dwFileSize{};
    HANDLE hFile{};
    FILE * pFile{};

    for (i = 0; i < DEF_MAXGAMEMSGS; i++)
    {
        if (m_pGameMsgList[i] != 0)
            delete m_pGameMsgList[i];
        m_pGameMsgList[i] = 0;
    }

    memset(cTemp, 0, sizeof(cTemp));
    memset(cFileName, 0, sizeof(cFileName));

    format_to_local(cFileName, "data\\shops\\GameMsgList.txt");

    hFile = CreateFileA(cFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFileName, "rt");
    if (pFile == 0) return;
    else
    {
        pContents = new char[dwFileSize + 1];
        memset(pContents, 0, dwFileSize + 1);
        fread(pContents, dwFileSize, 1, pFile);
    }

    fclose(pFile);

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != 0)
    {
        m_pGameMsgList[iIndex] = new CMsg(0, token, 0);
        token = pStrTok->pGet();
        iIndex++;
    }

    delete pStrTok;
    delete[] pContents;
}

bool CGame::__bDecodeBuildItemContents(char * pBuffer)
{
    char * pContents, * token;
    char seps[] = "= ,\t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iIndex = 0;
    class CStrTok * pStrTok;

    pContents = pBuffer;

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();

    while (token != 0)
    {
        if (cReadModeA != 0)
        {

            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:

                            memset(m_pBuildItemList[iIndex]->m_cName, 0, sizeof(m_pBuildItemList[iIndex]->m_cName));
                            memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));
                            cReadModeB = 2;
                            break;
                        case 2:

                            m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
                            cReadModeB = 3;
                            break;
                        case 3:
                            // m_cElementName1
                            memset(m_pBuildItemList[iIndex]->m_cElementName1, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName1));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName1, token, strlen(token));
                            cReadModeB = 4;
                            break;
                        case 4:
                            // m_iElementCount1
                            m_pBuildItemList[iIndex]->m_iElementCount[1] = atoi(token);
                            cReadModeB = 5;
                            break;
                        case 5:
                            // m_cElementName2
                            memset(m_pBuildItemList[iIndex]->m_cElementName2, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName2));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName2, token, strlen(token));
                            cReadModeB = 6;
                            break;
                        case 6:
                            // m_iElementCount2
                            m_pBuildItemList[iIndex]->m_iElementCount[2] = atoi(token);
                            cReadModeB = 7;
                            break;
                        case 7:
                            // m_cElementName3
                            memset(m_pBuildItemList[iIndex]->m_cElementName3, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName3));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName3, token, strlen(token));
                            cReadModeB = 8;
                            break;
                        case 8:
                            // m_iElementCount3
                            m_pBuildItemList[iIndex]->m_iElementCount[3] = atoi(token);
                            cReadModeB = 9;
                            break;
                        case 9:
                            // m_cElementName4
                            memset(m_pBuildItemList[iIndex]->m_cElementName4, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName4));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName4, token, strlen(token));
                            cReadModeB = 10;
                            break;
                        case 10:
                            // m_iElementCount4
                            m_pBuildItemList[iIndex]->m_iElementCount[4] = atoi(token);
                            cReadModeB = 11;
                            break;
                        case 11:
                            // m_cElementName5
                            memset(m_pBuildItemList[iIndex]->m_cElementName5, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName5));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName5, token, strlen(token));
                            cReadModeB = 12;
                            break;
                        case 12:
                            // m_iElementCount5
                            m_pBuildItemList[iIndex]->m_iElementCount[5] = atoi(token);
                            cReadModeB = 13;
                            break;
                        case 13:
                            // m_cElementName6
                            memset(m_pBuildItemList[iIndex]->m_cElementName6, 0, sizeof(m_pBuildItemList[iIndex]->m_cElementName6));
                            memcpy(m_pBuildItemList[iIndex]->m_cElementName6, token, strlen(token));
                            cReadModeB = 14;
                            break;
                        case 14:
                            // m_iElementCount6
                            m_pBuildItemList[iIndex]->m_iElementCount[6] = atoi(token);
                            cReadModeB = 15;
                            break;

                        case 15:
                            m_pBuildItemList[iIndex]->m_iSprH = atoi(token);
                            cReadModeB = 16;
                            break;

                        case 16:
                            m_pBuildItemList[iIndex]->m_iSprFrame = atoi(token);
                            cReadModeB = 17;
                            break;

                        case 17:
                            m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

                            cReadModeA = 0;
                            cReadModeB = 0;
                            iIndex++;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "BuildItem", 9) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
                m_pBuildItemList[iIndex] = new CBuildItem;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;

    if ((cReadModeA != 0) || (cReadModeB != 0)) return false;
    return true;
}

bool CGame::bReadItemNameConfigFile()
{
    FILE * pFile;
    HANDLE hFile;
    uint32_t  dwFileSize;
    char * cp, * token, cReadModeA, cReadModeB;
    char seps[] = "=\n";
    int iIndex;

    cReadModeA = 0;
    cReadModeB = 0;
    iIndex = 0;

    hFile = CreateFileA("data\\shops\\ItemName.cfg", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
    pFile = fopen("data\\shops\\ItemName.cfg", "rt");
    if (pFile == 0) return false;
    else
    {
        cp = new char[dwFileSize + 2];
        memset(cp, 0, dwFileSize + 2);
        fread(cp, dwFileSize, 1, pFile);

        token = strtok(cp, seps);
        while (token != 0)
        {

            if (cReadModeA != 0)
            {
                switch (cReadModeA)
                {
                    case 1:
                        switch (cReadModeB)
                        {
                            case 1:
                                m_pItemNameList[iIndex] = new CItemName;
                                strcpy(m_pItemNameList[iIndex]->m_cOriginName, token);
                                cReadModeB = 2;
                                break;

                            case 2:
                                strcpy(m_pItemNameList[iIndex]->m_cName, token);
                                cReadModeA = 0;
                                cReadModeB = 0;
                                iIndex++;
                                break;
                        }
                }
            }
            else
            {
                if (memcmp(token, "Item", 4) == 0)
                {
                    cReadModeA = 1;
                    cReadModeB = 1;
                }
            }
            token = strtok(0, seps);
        }
        delete[] cp;
    }
    if (pFile != 0) fclose(pFile);

    return true;
}

bool CGame::bInitMagicCfgList()
{
    char cFn[255]{};
    char * pContents, * token;
    char seps[] = "= ,\t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iMagicCfgListIndex = 0;
    CStrTok * pStrTok{};
    HANDLE hFile{};
    FILE * pFile{};
    uint32_t  dwFileSize{};

    memset(cFn, 0, sizeof(cFn));

    format_to_local(cFn, "data\\shops\\magiccfg.txt");

    hFile = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0) return false;
    else
    {
        pContents = new char[dwFileSize + 1];
        memset(pContents, 0, dwFileSize + 1);
        fread(pContents, dwFileSize, 1, pFile);
        fclose(pFile);
    }

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != 0)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pMagicCfgList[atoi(token)] != 0)
                            {

                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicCfgList[atoi(token)] = new CMagic;
                            iMagicCfgListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:

                            memset(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, 0, sizeof(m_pMagicCfgList[iMagicCfgListIndex]->m_cName));
                            memcpy(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;

                        case 3:
                            // m_sValue1
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicCfgList[iMagicCfgListIndex]->m_sValue1 = atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_sValue2
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicCfgList[iMagicCfgListIndex]->m_sValue2 = atoi(token);
                            cReadModeB = 5;
                            break;

                        case 5:
                            // m_sValue3
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pMagicCfgList[iMagicCfgListIndex]->m_sValue3 = atoi(token);
                            cReadModeB = 6;
                            break;

                        case 6:
                            // m_sValue4
                            //if (_bGetIsStringIsNumber(token) == false) {
                            //	delete[] pContents;
                            //	delete pStrTok;
                            //	return false;
                            //}
                            //m_pMagicCfgList[iMagicCfgListIndex]->m_sValue4 = atoi(token);
                            cReadModeB = 7;
                            break;

                        case 7:
                            // m_sValue5
                            //if (_bGetIsStringIsNumber(token) == false) {
                            //	delete[] pContents;
                            //	delete pStrTok;
                            //	return false;
                            //}
                            //m_pMagicCfgList[iMagicCfgListIndex]->m_sValue5 = atoi(token);
                            cReadModeB = 8;
                            break;

                        case 8:
                            // m_sValue6
                            //if (_bGetIsStringIsNumber(token) == false) {
                            //	delete[] pContents;
                            //	delete pStrTok;
                            //	return false;
                            //}
                            //m_pMagicCfgList[iMagicCfgListIndex]->m_sValue6 = atoi(token);
                            cReadModeB = 9;
                            break;

                        case 9:
                            // m_bIsVisible
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            if (atoi(token) == 0) m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = false;
                            else m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = true;
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "magic", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete[] pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        return false;
    }

    return true;
}

bool CGame::bCheckImportantFile()
{
    HANDLE hFile;
    hFile = CreateFileA("CONTENTS\\badword.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    CloseHandle(hFile);

    //	hFile = CreateFileA("SPRITES\\TREES1.PAK", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    //	if( hFile == INVALID_HANDLE_VALUE ) return false;



    //	if( GetFileSize( hFile, 0 ) != 2945524 )
    //	{
    //		CloseHandle( hFile );
    //		return false;
    //	}
    //	CloseHandle( hFile );
    return true;
}

bool CGame::bInitSkillCfgList()
{
    char cFn[255]{};
    char * pContents, * token;
    char seps[] = "= ,\t\n";
    char cReadModeA = 0;
    char cReadModeB = 0;
    int  iSkillCfgListIndex = 0;
    CStrTok * pStrTok{};
    HANDLE hFile{};
    FILE * pFile{};
    uint32_t  dwFileSize{};

    memset(cFn, 0, sizeof(cFn));

    format_to_local(cFn, "data\\shops\\skillcfg.txt");

    hFile = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    dwFileSize = GetFileSize(hFile, 0);
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    pFile = fopen(cFn, "rt");
    if (pFile == 0) return false;
    else
    {
        pContents = new char[dwFileSize + 1];
        memset(pContents, 0, dwFileSize + 1);
        fread(pContents, dwFileSize, 1, pFile);
        fclose(pFile);
    }

    pStrTok = new CStrTok(pContents, seps);
    token = pStrTok->pGet();
    while (token != 0)
    {
        if (cReadModeA != 0)
        {
            switch (cReadModeA)
            {
                case 1:
                    switch (cReadModeB)
                    {
                        case 1:

                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }

                            if (m_pSkillCfgList[atoi(token)] != 0)
                            {

                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillCfgList[atoi(token)] = new CSkill;
                            iSkillCfgListIndex = atoi(token);

                            cReadModeB = 2;
                            break;

                        case 2:

                            memset(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, 0, sizeof(m_pSkillCfgList[iSkillCfgListIndex]->m_cName));
                            memcpy(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, token, strlen(token));
                            cReadModeB = 3;
                            break;

                        case 3:
                            // m_bIsUseable
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillCfgList[iSkillCfgListIndex]->m_bIsUseable = (bool)atoi(token);
                            cReadModeB = 4;
                            break;

                        case 4:
                            // m_cUseMethod
                            if (_bGetIsStringIsNumber(token) == false)
                            {
                                delete[] pContents;
                                delete pStrTok;
                                return false;
                            }
                            m_pSkillCfgList[iSkillCfgListIndex]->m_cUseMethod = atoi(token);
                            cReadModeA = 0;
                            cReadModeB = 0;
                            break;

                    }
                    break;

                default:
                    break;
            }
        }
        else
        {
            if (memcmp(token, "skill", 5) == 0)
            {
                cReadModeA = 1;
                cReadModeB = 1;
            }
        }
        token = pStrTok->pGet();
    }

    delete pStrTok;
    delete[] pContents;

    if ((cReadModeA != 0) || (cReadModeB != 0))
    {
        return false;
    }

    return true;
}

void CGame::InitItemList(char * pData)
{
    char    cTotalItems;
    int     i;
    short * sp;
    uint32_t * dwp;
    uint16_t * wp;
    char * cp;


    for (i = 0; i < DEF_MAXITEMS; i++)
        m_cItemOrder[i] = -1;


    for (i = 0; i < DEF_MAXITEMEQUIPPOS; i++)
        m_sItemEquipmentStatus[i] = -1;

    for (i = 0; i < DEF_MAXITEMS; i++)
        m_bIsItemDisabled[i] = false;

    cp = (char *)(pData + DEF_INDEX2_MSGTYPE + 2);

    cTotalItems = *cp;
    cp++;

    for (i = 0; i < DEF_MAXITEMS; i++)
        if (m_pItemList[i] != 0)
        {
            delete m_pItemList[i];
            m_pItemList[i] = 0;
        }

    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0)
        {
            delete m_pBankList[i];
            m_pBankList[i] = 0;
        }

    for (i = 0; i < cTotalItems; i++)
    {
        m_pItemList[i] = new CItem;
        memcpy(m_pItemList[i]->m_cName, cp, 20);
        cp += 20;

        dwp = (uint32_t *)cp;
        m_pItemList[i]->m_dwCount = *dwp;
        m_pItemList[i]->m_sX = 40;
        m_pItemList[i]->m_sY = 30;
        cp += 4;

        m_pItemList[i]->m_cItemType = *cp;
        cp++;

        m_pItemList[i]->m_cEquipPos = *cp;
        cp++;

        if (*cp == 0) m_bIsItemEquipped[i] = false;
        else m_bIsItemEquipped[i] = true;
        cp++;

        if (m_bIsItemEquipped[i] == true)
        {

            m_sItemEquipmentStatus[m_pItemList[i]->m_cEquipPos] = i;
        }
        sp = (short *)cp;
        m_pItemList[i]->m_sLevelLimit = *sp;
        cp += 2;

        m_pItemList[i]->m_cGenderLimit = *cp;
        cp++;

        wp = (uint16_t *)cp;
        m_pItemList[i]->m_wCurLifeSpan = *wp;
        cp += 2;

        wp = (uint16_t *)cp;
        m_pItemList[i]->m_wWeight = *wp;
        cp += 2;

        sp = (short *)cp;
        m_pItemList[i]->m_sSprite = *sp;
        cp += 2;

        sp = (short *)cp;
        m_pItemList[i]->m_sSpriteFrame = *sp;
        cp += 2;

        m_pItemList[i]->m_cItemColor = *cp;
        cp++;

        m_pItemList[i]->m_sItemSpecEffectValue2 = (short)*cp;
        cp++;

        dwp = (uint32_t *)cp;
        m_pItemList[i]->m_dwAttribute = *dwp;
        cp += 4;
        /*
        m_pItemList[i]->m_bIsCustomMade = (bool)*cp;
        cp++;
        */

        m_cItemOrder[i] = i;
    }

    cTotalItems = *cp;
    cp++;


    for (i = 0; i < DEF_MAXBANKITEMS; i++)
        if (m_pBankList[i] != 0)
        {
            delete m_pBankList[i];
            m_pBankList[i] = 0;
        }

    for (i = 0; i < cTotalItems; i++)
    {
        m_pBankList[i] = new CItem;
        memcpy(m_pBankList[i]->m_cName, cp, 20);
        cp += 20;

        dwp = (uint32_t *)cp;
        m_pBankList[i]->m_dwCount = *dwp;
        cp += 4;

        m_pBankList[i]->m_sX = 40;
        m_pBankList[i]->m_sY = 30;

        m_pBankList[i]->m_cItemType = *cp;
        cp++;

        m_pBankList[i]->m_cEquipPos = *cp;
        cp++;

        sp = (short *)cp;
        m_pBankList[i]->m_sLevelLimit = *sp;
        cp += 2;

        m_pBankList[i]->m_cGenderLimit = *cp;
        cp++;

        wp = (uint16_t *)cp;
        m_pBankList[i]->m_wCurLifeSpan = *wp;
        cp += 2;

        wp = (uint16_t *)cp;
        m_pBankList[i]->m_wWeight = *wp;
        cp += 2;

        sp = (short *)cp;
        m_pBankList[i]->m_sSprite = *sp;
        cp += 2;

        sp = (short *)cp;
        m_pBankList[i]->m_sSpriteFrame = *sp;
        cp += 2;

        m_pBankList[i]->m_cItemColor = *cp;
        cp++;

        m_pBankList[i]->m_sItemSpecEffectValue2 = (short)*cp;
        cp++;

        dwp = (uint32_t *)cp;
        m_pBankList[i]->m_dwAttribute = *dwp;
        cp += 4;
        /*
        m_pBankList[i]->m_bIsCustomMade = (bool)*cp;
        cp++;
        */
    }


    for (i = 0; i < DEF_MAXMAGICTYPE; i++)
    {
        m_cMagicMastery[i] = *cp;
        cp++;
    }

    for (i = 0; i < DEF_MAXSKILLTYPE; i++)
    {
        m_cSkillMastery[i] = (unsigned char)*cp;
        if (m_pSkillCfgList[i] != 0)
            m_pSkillCfgList[i]->m_iLevel = (int)*cp;
        cp++;
    }
}
