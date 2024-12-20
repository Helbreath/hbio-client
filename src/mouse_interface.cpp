//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "mouse_interface.h"
#include "defines.h"

CMouseInterface::CMouseInterface()
{
    m_cPrevPress = 0;
    for (int i = 0; i < DEF_MAXRECTS; i++)
        m_pRect[i] = nullptr;
    m_dwTime = unixtime();
}

CMouseInterface::~CMouseInterface()
{
    for (int i = 1; i < DEF_MAXRECTS; i++)
        if (m_pRect[i] != nullptr) delete m_pRect[i];
}

void CMouseInterface::AddRect(long sx, long sy, long dx, long dy)
{
    for (int i = 1; i < DEF_MAXRECTS; i++)
        if (m_pRect[i] == nullptr)
        {
            m_pRect[i] = (hbxrect *)new hbxrect(sx, sy, dx, dy);
            return;
        }
}

int CMouseInterface::iGetStatus(int msX, int msY, char cLB, char * pResult)
{
    // @bug
    if (m_cPrevPress < 0) m_cPrevPress = 0;
    int i, iRet;
    if (cLB != 0)
    {

        for (i = 1; i < DEF_MAXRECTS; i++)
            if (m_pRect[i] != nullptr)
            {
                if ((m_pRect[i]->left < msX) && (m_pRect[i]->right > msX) &&
                    (m_pRect[i]->top < msY) && (m_pRect[i]->bottom > msY))
                {
                    m_cPrevPress = i;
                    *pResult = DEF_MIRESULT_PRESS;
                    return i;
                }
            }
    }

    if ((m_cPrevPress != 0) && (cLB == 0))
    {
        //if (m_cPrevPress <= 0 ) return 0 ;

        if ((m_pRect[m_cPrevPress]->left < msX) && (m_pRect[m_cPrevPress]->right > msX) &&
            (m_pRect[m_cPrevPress]->top < msY) && (m_pRect[m_cPrevPress]->bottom > msY))
        {
            iRet = m_cPrevPress;
            m_cPrevPress = 0;
            *pResult = DEF_MIRESULT_CLICK;
            return iRet;
        }
    }

    *pResult = DEF_MIRESULT_NONE;
    return 0;
}

