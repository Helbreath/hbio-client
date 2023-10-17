//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#pragma once

#include <windows.h>

#define DEF_MAXITEMEQUIPPOS		15
#define DEF_EQUIPPOS_NONE		0
#define DEF_EQUIPPOS_HEAD		1
#define DEF_EQUIPPOS_BODY		2
#define DEF_EQUIPPOS_ARMS		3
#define DEF_EQUIPPOS_PANTS		4
#define DEF_EQUIPPOS_BOOTS		5
#define DEF_EQUIPPOS_NECK		6
#define DEF_EQUIPPOS_LHAND		7
#define DEF_EQUIPPOS_RHAND		8
#define DEF_EQUIPPOS_TWOHAND	9
#define DEF_EQUIPPOS_RFINGER	10
#define DEF_EQUIPPOS_LFINGER	11
#define DEF_EQUIPPOS_BACK		12
#define DEF_EQUIPPOS_FULLBODY	13

#define DEF_ITEMTYPE_NONE			0
#define DEF_ITEMTYPE_EQUIP			1
#define DEF_ITEMTYPE_APPLY			2
#define DEF_ITEMTYPE_USE_DEPLETE	3
#define DEF_ITEMTYPE_INSTALL		4
#define DEF_ITEMTYPE_CONSUME		5
#define DEF_ITEMTYPE_ARROW			6
#define DEF_ITEMTYPE_EAT			7
#define DEF_ITEMTYPE_USE_SKILL		8
#define DEF_ITEMTYPE_USE_PERM		9
#define DEF_ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define DEF_ITEMTYPE_USE_DEPLETE_DEST			11
#define DEF_ITEMTYPE_MATERIAL					12

class CItem  
{
public:
	CItem();
	~CItem();
	char  m_cName[21];
	char  m_cItemType;
	char  m_cEquipPos;
	char  m_cItemColor;
	char  m_cSpeed;
	char  m_cGenderLimit;
//	short m_sItemEffectType;
	short m_sLevelLimit;
	short m_sSprite;
	short m_sSpriteFrame;
	short m_sX, m_sY;
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3, m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6; 
	WORD  m_wCurLifeSpan;
	WORD  m_wMaxLifeSpan;
	WORD  m_wPrice, m_wWeight;
	DWORD m_dwCount;
	DWORD m_dwAttribute;
};

/*
¾ÆÀÌÅÛ Á¤ÀÇÆÄÀÏ¿¡ ÇÊ¿äÇÑ ³»¿ë - °ÔÀÓ ¼­¹ö¿ë

char cItemType
 - ¾ÆÀÌÅÛÀÇ Á¾·ù. ÀåÂøµÇ´Â °ÍÀÎÁö ¼Ò¸ðµÇ´Â °ÍÀÎÁö¿¡ ´ëÇÑ Á¤ÀÇ

char cEquipPos
 - ÀåÂøµÇ´Â ¾ÆÀÌÅÛÀÌ¶ó¸é ±× À§Ä¡ 

short sItemEffectType     
  - ¾ÆÀÌÅÛÀÇ È¿°ú Á¾·ù.  

short sItemEffectValue1, 2, 3 ... 
 - ¾ÆÀÌÅÛ È¿°úÀÇ ¼öÄ¡ 1, 2, 3...

WORD wMaxLifeSpan
 - ¾ÆÀÌÅÛÀÇ ÃÖ´ë ¼ö¸í 

short sMaxFixCount
 - ÃÖ´ë °íÄ¥¼ö ÀÖ´Â È½¼ö




ÇÃ·¹ÀÌ¾î µ¥ÀÌÅÍ ÆÄÀÏ¿¡ ÀúÀåµÇ¾î¾ß ÇÒ Á¤º¸ - ·Î±× ¼­¹ö¿ë 

DWORD dwCount
 - ¾ÆÀÌÅÛÀÇ °¹¼ö. (°°Àº Á¾·ùÀÇ È­»ìÀÌ³ª Æ÷¼Ç, Goldµî¸¸ÀÌ ÀÇ¹Ì¸¦ °®´Â´Ù)

WORD wCurLifeSpan
 - ÇöÀç ³²Àº ¼ö¸í 

short sCurFixCount
 - ÇöÀç±îÁö ¼ö¸®ÇÑ È½¼ö 

short sTouchEffectType
 - ¾ÆÀÌÅÛÀ» Ã³À½ °Çµå·ÈÀ»¶§ ÀÛµ¿µÇ´Â È¿°ú Á¾·ù 

short sTouchEffectValue1, 2, 3...
 - ¾ÆÀÌÅÛ Á¢ÃË È¿°úÀÇ ¼öÄ¡ 

short sItemSpecEffectType
 - ¾ÆÀÌÅÛÀÇ Æ¯¼öÈ¿°ú Á¾·ù (ex:¸¶¹ýÀÌ °É¸° ¹«±â¶ó¸é °ø°Ý·ÂÀÌ³ª ¹æ¾î·Â¿¡ ¿µÇâÀÌ ÀÖÀ»¼öµµ)

short sItemSpecEffectValue1, 2, 3...
 - ¾ÆÀÌÅÛ Æ¯¼öÈ¿°úÀÇ ¼öÄ¡ 1, 2, 3...


*/
