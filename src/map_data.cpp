//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#include "map_data.h"
#include "dynamic_object_id.h"
#include "msg.h"
#include "tile.h"
#include "game.h"
#include "tile_spr.h"

CMapData::CMapData(CGame * pGame)
{
    int i;
    m_pGame = pGame;
    memset(m_iObjectIDcacheLocX, 0, sizeof(m_iObjectIDcacheLocX));
    memset(m_iObjectIDcacheLocY, 0, sizeof(m_iObjectIDcacheLocY));
    m_dwDOframeTime = m_dwFrameTime = unixtime();

    m_pData = new CTile * [MAPDATASIZEX];
    m_pTmpData = new CTile * [MAPDATASIZEX];
    m_tile = new CTileSpr * [MAPDATASIZEX];

    for (i = 0; i < MAPDATASIZEX; i++)
    {
        m_pData[i] = new CTile[MAPDATASIZEY];
        m_pTmpData[i] = new CTile[MAPDATASIZEY];
        m_tile[i] = new CTileSpr[MAPDATASIZEY];
    }

    for (i = 0; i < DEF_TOTALCHARACTERS; i++)
    {
        m_stFrame[i][DEF_OBJECTMOVE].m_sMaxFrame = 7;
    }

    for (i = 1; i <= 6; i++)
    {
        m_stFrame[i][DEF_OBJECTSTOP].m_sMaxFrame = 14;
        m_stFrame[i][DEF_OBJECTSTOP].m_sFrameTime = 60;
        m_stFrame[i][DEF_OBJECTMOVE].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTMOVE].m_sFrameTime = 70;
        m_stFrame[i][DEF_OBJECTDAMAGEMOVE].m_sMaxFrame = 3;
        m_stFrame[i][DEF_OBJECTDAMAGEMOVE].m_sFrameTime = 50;
        m_stFrame[i][DEF_OBJECTRUN].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTRUN].m_sFrameTime = 42;
        m_stFrame[i][DEF_OBJECTATTACK].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTATTACK].m_sFrameTime = 78;
        m_stFrame[i][DEF_OBJECTATTACKMOVE].m_sMaxFrame = 12;
        m_stFrame[i][DEF_OBJECTATTACKMOVE].m_sFrameTime = 78;
        m_stFrame[i][DEF_OBJECTMAGIC].m_sMaxFrame = 15;
        m_stFrame[i][DEF_OBJECTMAGIC].m_sFrameTime = 88;
        m_stFrame[i][DEF_OBJECTGETITEM].m_sMaxFrame = 3;
        m_stFrame[i][DEF_OBJECTGETITEM].m_sFrameTime = 150;
        m_stFrame[i][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
        m_stFrame[i][DEF_OBJECTDAMAGE].m_sFrameTime = 70;
        m_stFrame[i][DEF_OBJECTDYING].m_sMaxFrame = 12;
        m_stFrame[i][DEF_OBJECTDYING].m_sFrameTime = 80;
    }

    // Slime
    m_stFrame[10][DEF_OBJECTSTOP].m_sFrameTime = 240;
    m_stFrame[10][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[10][DEF_OBJECTMOVE].m_sFrameTime = 120;//240;
    m_stFrame[10][DEF_OBJECTATTACK].m_sFrameTime = 90;
    m_stFrame[10][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[10][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[10][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[10][DEF_OBJECTDYING].m_sFrameTime = 240;
    m_stFrame[10][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Skeleton
    m_stFrame[11][DEF_OBJECTSTOP].m_sFrameTime = 150;
    m_stFrame[11][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[11][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[11][DEF_OBJECTATTACK].m_sFrameTime = 90;
    m_stFrame[11][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[11][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[11][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[11][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[11][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Stone-Golem
    m_stFrame[12][DEF_OBJECTSTOP].m_sFrameTime = 210;
    m_stFrame[12][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[12][DEF_OBJECTMOVE].m_sFrameTime = 100;//210;
    m_stFrame[12][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[12][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[12][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[12][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[12][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[12][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Cyclops
    m_stFrame[13][DEF_OBJECTSTOP].m_sFrameTime = 210;
    m_stFrame[13][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[13][DEF_OBJECTMOVE].m_sFrameTime = 80;//210;
    m_stFrame[13][DEF_OBJECTATTACK].m_sFrameTime = 90;
    m_stFrame[13][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[13][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[13][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[13][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[13][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Orc
    m_stFrame[14][DEF_OBJECTSTOP].m_sFrameTime = 180;
    m_stFrame[14][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[14][DEF_OBJECTMOVE].m_sFrameTime = 80;//150;
    m_stFrame[14][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[14][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[14][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[14][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[14][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[14][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // ShopKeeper-W
    m_stFrame[15][DEF_OBJECTSTOP].m_sFrameTime = 180;
    m_stFrame[15][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[15][DEF_OBJECTMOVE].m_sFrameTime = 100;//150;
    m_stFrame[15][DEF_OBJECTATTACK].m_sFrameTime = 150;
    m_stFrame[15][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[15][DEF_OBJECTDAMAGE].m_sFrameTime = 180;
    m_stFrame[15][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[15][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[15][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Giant Ant
    m_stFrame[16][DEF_OBJECTSTOP].m_sFrameTime = 120;
    m_stFrame[16][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[16][DEF_OBJECTMOVE].m_sFrameTime = 60;//120;
    m_stFrame[16][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[16][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[16][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[16][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[16][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[16][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Scorpion
    m_stFrame[17][DEF_OBJECTSTOP].m_sFrameTime = 120;
    m_stFrame[17][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[17][DEF_OBJECTMOVE].m_sFrameTime = 45;//120;
    m_stFrame[17][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[17][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[17][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[17][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[17][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[17][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Zombie
    m_stFrame[18][DEF_OBJECTSTOP].m_sFrameTime = 210;
    m_stFrame[18][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[18][DEF_OBJECTMOVE].m_sFrameTime = 130;//270;
    m_stFrame[18][DEF_OBJECTATTACK].m_sFrameTime = 150;
    m_stFrame[18][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[18][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[18][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[18][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[18][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Gandlf
    m_stFrame[19][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[19][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[19][DEF_OBJECTMOVE].m_sFrameTime = 100;//210;
    m_stFrame[19][DEF_OBJECTATTACK].m_sFrameTime = 150;
    m_stFrame[19][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[19][DEF_OBJECTDAMAGE].m_sFrameTime = 180;
    m_stFrame[19][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[19][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[19][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Howard
    m_stFrame[20][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[20][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[20][DEF_OBJECTMOVE].m_sFrameTime = 100;//210;
    m_stFrame[20][DEF_OBJECTATTACK].m_sFrameTime = 150;
    m_stFrame[20][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[20][DEF_OBJECTDAMAGE].m_sFrameTime = 180;
    m_stFrame[20][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[20][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[20][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Guard
    m_stFrame[21][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[21][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[21][DEF_OBJECTMOVE].m_sFrameTime = 80;//150;
    m_stFrame[21][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[21][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[21][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[21][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[21][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[21][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Amphisbena
    m_stFrame[22][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[22][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[22][DEF_OBJECTMOVE].m_sFrameTime = 80;//150;
    m_stFrame[22][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[22][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[22][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[22][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[22][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[22][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Clay-Golem
    m_stFrame[23][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[23][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[23][DEF_OBJECTMOVE].m_sFrameTime = 80;//150;
    m_stFrame[23][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[23][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[23][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[23][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[23][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[23][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Tom 
    m_stFrame[24][DEF_OBJECTSTOP].m_sFrameTime = 150;
    m_stFrame[24][DEF_OBJECTSTOP].m_sMaxFrame = 7;

    // William 
    m_stFrame[25][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[25][DEF_OBJECTSTOP].m_sMaxFrame = 7;

    // Kenedy 
    m_stFrame[26][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[26][DEF_OBJECTSTOP].m_sMaxFrame = 7;

    // Hellbound
    m_stFrame[27][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[27][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[27][DEF_OBJECTMOVE].m_sFrameTime = 50;
    m_stFrame[27][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[27][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[27][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[27][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[27][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[27][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Troll
    m_stFrame[28][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[28][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[28][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[28][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[28][DEF_OBJECTATTACK].m_sMaxFrame = 5;
    m_stFrame[28][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[28][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[28][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[28][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Orge
    m_stFrame[29][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[29][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[29][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[29][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[29][DEF_OBJECTATTACK].m_sMaxFrame = 5;
    m_stFrame[29][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[29][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[29][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[29][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Liche
    m_stFrame[30][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[30][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[30][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[30][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[30][DEF_OBJECTATTACK].m_sMaxFrame = 5;
    m_stFrame[30][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[30][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[30][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[30][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Demon
    m_stFrame[31][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[31][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[31][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[31][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[31][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[31][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[31][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[31][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[31][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Unicorn
    m_stFrame[32][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[32][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[32][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[32][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[32][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[32][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[32][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[32][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[32][DEF_OBJECTDYING].m_sMaxFrame = 11;

    // WereWolf
    m_stFrame[33][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[33][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[33][DEF_OBJECTMOVE].m_sFrameTime = 120;
    m_stFrame[33][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[33][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[33][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[33][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[33][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[33][DEF_OBJECTDYING].m_sMaxFrame = 11;

    // Dummy
    m_stFrame[34][DEF_OBJECTSTOP].m_sFrameTime = 240;
    m_stFrame[34][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[34][DEF_OBJECTMOVE].m_sFrameTime = 120;
    m_stFrame[34][DEF_OBJECTATTACK].m_sFrameTime = 90;
    m_stFrame[34][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[34][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[34][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[34][DEF_OBJECTDYING].m_sFrameTime = 240;
    m_stFrame[34][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Energy-Ball
    m_stFrame[35][DEF_OBJECTSTOP].m_sFrameTime = 80;
    m_stFrame[35][DEF_OBJECTSTOP].m_sMaxFrame = 9;
    m_stFrame[35][DEF_OBJECTMOVE].m_sFrameTime = 20;
    m_stFrame[35][DEF_OBJECTMOVE].m_sMaxFrame = 3;
    m_stFrame[35][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[35][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[35][DEF_OBJECTDAMAGE].m_sFrameTime = 80;
    m_stFrame[35][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
    m_stFrame[35][DEF_OBJECTDYING].m_sFrameTime = 80;
    m_stFrame[35][DEF_OBJECTDYING].m_sMaxFrame = 7;

    // Crossbow Guard Tower
    m_stFrame[36][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[36][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[36][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[36][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[36][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[36][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[36][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[36][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[36][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[36][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Cannon Guard Tower
    m_stFrame[37][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[37][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[37][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[37][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[37][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[37][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[37][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[37][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[37][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[37][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Mana Collector
    m_stFrame[38][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[38][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[38][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[38][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[38][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[38][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[38][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[38][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[38][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[38][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Detector
    m_stFrame[39][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[39][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[39][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[39][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[39][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[39][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[39][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[39][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[39][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[39][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Energy Shield Generator
    m_stFrame[40][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[40][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[40][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[40][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[40][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[40][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[40][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[40][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[40][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[40][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Grand Magic Generator
    m_stFrame[41][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[41][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[41][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[41][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[41][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[41][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[41][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[41][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[41][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[41][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Mana Stone
    m_stFrame[42][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[42][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[42][DEF_OBJECTMOVE].m_sFrameTime = 80;
    m_stFrame[42][DEF_OBJECTMOVE].m_sMaxFrame = 0;
    m_stFrame[42][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[42][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[42][DEF_OBJECTDAMAGE].m_sFrameTime = 150;
    m_stFrame[42][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[42][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[42][DEF_OBJECTDYING].m_sMaxFrame = 0;

    // Light War Beetle
    m_stFrame[43][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[43][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[43][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[43][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[43][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[43][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[43][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[43][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[43][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // God's Hand Knight
    m_stFrame[44][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[44][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[44][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[44][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[44][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[44][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[44][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[44][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[44][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // God's Hand Knight with Armored Battle Steed
    m_stFrame[45][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[45][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[45][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[45][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[45][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[45][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[45][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[45][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[45][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Temple Knight
    m_stFrame[46][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[46][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[46][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[46][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[46][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[46][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[46][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[46][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[46][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Battle Golem
    m_stFrame[47][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[47][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[47][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[47][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[47][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[47][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[47][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[47][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[47][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Stalker
    m_stFrame[48][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[48][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[48][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[48][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[48][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[48][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[48][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[48][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[48][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Hellclaw
    m_stFrame[49][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[49][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[49][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[49][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[49][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[49][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[49][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[49][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[49][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Tigerworm
    m_stFrame[50][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[50][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[50][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[50][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[50][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[50][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[50][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 7;
    m_stFrame[50][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[50][DEF_OBJECTDYING].m_sMaxFrame = 9;

    // Catapult
    m_stFrame[51][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[51][DEF_OBJECTSTOP].m_sMaxFrame = 0;
    m_stFrame[51][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[51][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[51][DEF_OBJECTATTACK].m_sMaxFrame = 4;
    m_stFrame[51][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[51][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[51][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[51][DEF_OBJECTDYING].m_sMaxFrame = 6;

    // Gargoyle
    m_stFrame[52][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[52][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[52][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[52][DEF_OBJECTATTACK].m_sFrameTime = 70;
    m_stFrame[52][DEF_OBJECTATTACK].m_sMaxFrame = 9;
    m_stFrame[52][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[52][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[52][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[52][DEF_OBJECTDYING].m_sMaxFrame = 11 + 3;

    // Beholder
    m_stFrame[53][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[53][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[53][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[53][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[53][DEF_OBJECTATTACK].m_sMaxFrame = 12;
    m_stFrame[53][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[53][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[53][DEF_OBJECTDYING].m_sFrameTime = 70;
    m_stFrame[53][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // DarkElf
    m_stFrame[54][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[54][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[54][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[54][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[54][DEF_OBJECTATTACK].m_sMaxFrame = 9;
    m_stFrame[54][DEF_OBJECTDAMAGE].m_sFrameTime = 120;
    m_stFrame[54][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[54][DEF_OBJECTDYING].m_sFrameTime = 100;
    m_stFrame[54][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Bunny
    m_stFrame[55][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[55][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[55][DEF_OBJECTMOVE].m_sFrameTime = 70;
    m_stFrame[55][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[55][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[55][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[55][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[55][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[55][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Cat
    m_stFrame[56][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[56][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[56][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[56][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[56][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[56][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[56][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[56][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[56][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Giant-Frog
    m_stFrame[57][DEF_OBJECTSTOP].m_sFrameTime = 300;
    m_stFrame[57][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[57][DEF_OBJECTMOVE].m_sFrameTime = 100;
    m_stFrame[57][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[57][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[57][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[57][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[57][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[57][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Mountain Giant
    m_stFrame[58][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[58][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[58][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[58][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[58][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[58][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[58][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[58][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[58][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Ettin
    m_stFrame[59][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[59][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[59][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[59][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[59][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[59][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[59][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[59][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[59][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Cannibal-Plant
    m_stFrame[60][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[60][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[60][DEF_OBJECTMOVE].m_sFrameTime = 120;
    m_stFrame[60][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[60][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[60][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[60][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[60][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[60][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Rudolph
    m_stFrame[61][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[61][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[61][DEF_OBJECTMOVE].m_sFrameTime = 90;//60;
    m_stFrame[61][DEF_OBJECTATTACK].m_sFrameTime = 120;
    m_stFrame[61][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[61][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[61][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[61][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[61][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;


    m_stFrame[62][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[62][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[62][DEF_OBJECTMOVE].m_sFrameTime = 60;
    m_stFrame[62][DEF_OBJECTATTACK].m_sFrameTime = 60;
    m_stFrame[62][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[62][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[62][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[62][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[62][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;
    m_stFrame[63][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[63][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[63][DEF_OBJECTMOVE].m_sFrameTime = 60;
    m_stFrame[63][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[63][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[63][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[63][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[63][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[63][DEF_OBJECTDYING].m_sMaxFrame = 5 + 3;//7 +3;

    // Crops
    m_stFrame[64][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[64][DEF_OBJECTSTOP].m_sMaxFrame = 40;
    m_stFrame[64][DEF_OBJECTMOVE].m_sFrameTime = 200;
    m_stFrame[64][DEF_OBJECTATTACK].m_sFrameTime = 200;
    m_stFrame[64][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[64][DEF_OBJECTDAMAGE].m_sFrameTime = 200;
    m_stFrame[64][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[64][DEF_OBJECTDYING].m_sFrameTime = 200;
    m_stFrame[64][DEF_OBJECTDYING].m_sMaxFrame = 3;


    m_stFrame[65][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[65][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[65][DEF_OBJECTMOVE].m_sFrameTime = 140;
    m_stFrame[65][DEF_OBJECTATTACK].m_sFrameTime = 105;
    m_stFrame[65][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[65][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[65][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[65][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[65][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Wyvern
    m_stFrame[66][DEF_OBJECTSTOP].m_sFrameTime = 100;
    m_stFrame[66][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[66][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[66][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[66][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[66][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[66][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[66][DEF_OBJECTDYING].m_sFrameTime = 65;
    m_stFrame[66][DEF_OBJECTDYING].m_sMaxFrame = 15 + 3;

    // McGaffin
    m_stFrame[67][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[67][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[67][DEF_OBJECTMOVE].m_sFrameTime = 120;
    m_stFrame[67][DEF_OBJECTMOVE].m_sMaxFrame = 3;
    m_stFrame[67][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[67][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[67][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[67][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[67][DEF_OBJECTDYING].m_sFrameTime = 65;
    m_stFrame[67][DEF_OBJECTDYING].m_sMaxFrame = 3 + 3;

    // Perry
    m_stFrame[68][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[68][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[68][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[68][DEF_OBJECTMOVE].m_sMaxFrame = 3;
    m_stFrame[68][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[68][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[68][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[68][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[68][DEF_OBJECTDYING].m_sFrameTime = 65;
    m_stFrame[68][DEF_OBJECTDYING].m_sMaxFrame = 3 + 3;

    // Devlin
    m_stFrame[69][DEF_OBJECTSTOP].m_sFrameTime = 200;
    m_stFrame[69][DEF_OBJECTSTOP].m_sMaxFrame = 3;
    m_stFrame[69][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[68][DEF_OBJECTMOVE].m_sMaxFrame = 3;
    m_stFrame[69][DEF_OBJECTATTACK].m_sFrameTime = 80;
    m_stFrame[69][DEF_OBJECTATTACK].m_sMaxFrame = 3;
    m_stFrame[69][DEF_OBJECTDAMAGE].m_sFrameTime = 60;
    m_stFrame[69][DEF_OBJECTDAMAGE].m_sMaxFrame = 3;
    m_stFrame[69][DEF_OBJECTDYING].m_sFrameTime = 65;
    m_stFrame[69][DEF_OBJECTDYING].m_sMaxFrame = 3 + 3;

    for (i = 70; i <= 91; i++)
    {
        m_stFrame[i][DEF_OBJECTSTOP].m_sMaxFrame = 14;
        m_stFrame[i][DEF_OBJECTSTOP].m_sFrameTime = 60;
        m_stFrame[i][DEF_OBJECTMOVE].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTMOVE].m_sFrameTime = 70;
        m_stFrame[i][DEF_OBJECTDAMAGEMOVE].m_sMaxFrame = 3;
        m_stFrame[i][DEF_OBJECTDAMAGEMOVE].m_sFrameTime = 50;
        m_stFrame[i][DEF_OBJECTRUN].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTRUN].m_sFrameTime = 42;
        m_stFrame[i][DEF_OBJECTATTACK].m_sMaxFrame = 7;
        m_stFrame[i][DEF_OBJECTATTACK].m_sFrameTime = 78;
        m_stFrame[i][DEF_OBJECTATTACKMOVE].m_sMaxFrame = 12;
        m_stFrame[i][DEF_OBJECTATTACKMOVE].m_sFrameTime = 78;
        m_stFrame[i][DEF_OBJECTMAGIC].m_sMaxFrame = 15;
        m_stFrame[i][DEF_OBJECTMAGIC].m_sFrameTime = 88;
        m_stFrame[i][DEF_OBJECTGETITEM].m_sMaxFrame = 3;
        m_stFrame[i][DEF_OBJECTGETITEM].m_sFrameTime = 150;
        m_stFrame[i][DEF_OBJECTDAMAGE].m_sMaxFrame = 3 + 4;
        m_stFrame[i][DEF_OBJECTDAMAGE].m_sFrameTime = 70;
        m_stFrame[i][DEF_OBJECTDYING].m_sMaxFrame = 12;
        m_stFrame[i][DEF_OBJECTDYING].m_sFrameTime = 80;
    }

    // Barlog
    m_stFrame[70][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[70][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[70][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[70][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[70][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[70][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[70][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[70][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[70][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Centaurus
    m_stFrame[71][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[71][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[71][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[71][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[71][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[71][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[71][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[71][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[71][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Claw-Turtle
    m_stFrame[72][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[72][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[72][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[72][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[72][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[72][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[72][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[72][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[72][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    // Fire Wyvern
    m_stFrame[73][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[73][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[73][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[73][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[73][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[73][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[73][DEF_OBJECTDAMAGE].m_sMaxFrame = 0;
    m_stFrame[73][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[73][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Giant-Crayfish
    m_stFrame[74][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[74][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[74][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[74][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[74][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[74][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[74][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[74][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[74][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Giant-Lizard
    m_stFrame[75][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[75][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[75][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[75][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[75][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[75][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[75][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[75][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[75][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Giant-Plant
    m_stFrame[76][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[76][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[76][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[76][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[76][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[76][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[76][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[76][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[76][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //MasterMage-Orc
    m_stFrame[77][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[77][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[77][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[77][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[77][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[77][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[77][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[77][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[77][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Minotaurs
    m_stFrame[78][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[78][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[78][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[78][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[78][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[78][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[78][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[78][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[78][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Nizie
    m_stFrame[79][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[79][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[79][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[79][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[79][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[79][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[79][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[79][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[79][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Tentocle
    m_stFrame[80][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[80][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[80][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[80][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[80][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[80][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[80][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[80][DEF_OBJECTDYING].m_sFrameTime = 150;
    m_stFrame[80][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Abaddon
    m_stFrame[81][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[81][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[81][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[81][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[81][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[81][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[81][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[81][DEF_OBJECTDYING].m_sFrameTime = 90;
    m_stFrame[81][DEF_OBJECTDYING].m_sMaxFrame = 15 + 3;

    //Sor-Aresden
    m_stFrame[82][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[82][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[82][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[82][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[82][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[82][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[82][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[82][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[82][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //ATK-Aresden
    m_stFrame[83][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[83][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[83][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[83][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[83][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[83][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[83][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[83][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[83][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Elf-Aresden
    m_stFrame[84][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[84][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[84][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[84][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[84][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[84][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[84][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[84][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[84][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //DSK-Aresden
    m_stFrame[85][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[85][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[85][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[85][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[85][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[85][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[85][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[85][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[85][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //HBT-Aresden
    m_stFrame[86][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[86][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[86][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[86][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[86][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[86][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[86][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[86][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[86][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //CT-Aresden
    m_stFrame[87][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[87][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[87][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[87][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[87][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[87][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[87][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[87][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[87][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Bar-Aresden
    m_stFrame[88][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[88][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[88][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[88][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[88][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[88][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[88][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[88][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[88][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //AGC-Aresden
    m_stFrame[89][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[89][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[89][DEF_OBJECTMOVE].m_sFrameTime = 90;
    m_stFrame[89][DEF_OBJECTATTACK].m_sFrameTime = 100;
    m_stFrame[89][DEF_OBJECTATTACK].m_sMaxFrame = 7;
    m_stFrame[89][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[89][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[89][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[89][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;

    //Gail
    m_stFrame[90][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[90][DEF_OBJECTSTOP].m_sMaxFrame = 7;

    //Heldenian Gates
    m_stFrame[91][DEF_OBJECTSTOP].m_sFrameTime = 250;
    m_stFrame[91][DEF_OBJECTSTOP].m_sMaxFrame = 7;
    m_stFrame[91][DEF_OBJECTDAMAGE].m_sFrameTime = 100;
    m_stFrame[91][DEF_OBJECTDAMAGE].m_sMaxFrame = 7;
    m_stFrame[91][DEF_OBJECTDYING].m_sFrameTime = 180;
    m_stFrame[91][DEF_OBJECTDYING].m_sMaxFrame = 7 + 3;
}

void CMapData::Init()
{
    int x, y;
    m_dwFrameCheckTime = unixtime();
    m_dwFrameAdjustTime = 0;
    for (y = 0; y < MAPDATASIZEY; y++)
        for (x = 0; x < MAPDATASIZEX; x++)
            m_pData[x][y].Clear();

    for (x = 0; x < 30000; x++)
    {
        m_iObjectIDcacheLocX[x] = 0;
        m_iObjectIDcacheLocY[x] = 0;
    }
}

CMapData::~CMapData()
{
}

void CMapData::OpenMapDataFile(char * cFn)
{
    // todo: use stdlib
    HANDLE hFileRead;
    DWORD nCount;
    char cHeader[260];
    char * cp, * cpMapData;
    memset(cHeader, 0, sizeof(cHeader));
    hFileRead = CreateFileA(cFn, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (hFileRead == INVALID_HANDLE_VALUE) return;
    SetFilePointer(hFileRead, 0, 0, FILE_BEGIN);
    ReadFile(hFileRead, cHeader, 256, &nCount, 0);
    _bDecodeMapInfo(cHeader);
    cpMapData = new char[m_sMapSizeX * m_sMapSizeY * 10];
    ReadFile(hFileRead, cpMapData, m_sMapSizeX * m_sMapSizeY * 10, &nCount, 0);
    CloseHandle(hFileRead);
    cp = cpMapData;
    short * sp;

    for (int y = 0; y < m_sMapSizeY; y++)
    {
        for (int x = 0; x < m_sMapSizeX; x++)
        {
            sp = (short *)cp;
            m_tile[x][y].m_sTileSprite = *sp;
            cp += 2;
            sp = (short *)cp;
            m_tile[x][y].m_sTileSpriteFrame = *sp;
            cp += 2;
            sp = (short *)cp;
            m_tile[x][y].m_sObjectSprite = *sp;
            cp += 2;
            sp = (short *)cp;
            m_tile[x][y].m_sObjectSpriteFrame = *sp;
            cp += 2;
            if (((*cp) & 0x80) != 0)
                m_tile[x][y].m_bIsMoveAllowed = false;
            else m_tile[x][y].m_bIsMoveAllowed = true;
            if (((*cp) & 0x40) != 0)
                m_tile[x][y].m_bIsTeleport = true;
            else m_tile[x][y].m_bIsTeleport = false;
            cp += 2;
        }
    }
    delete[] cpMapData;
}

void CMapData::_bDecodeMapInfo(char * pHeader)
{
    int i;
    char * token, cReadMode;
    char seps[] = "= ,\t\n";
    for (i = 0; i < 256; i++)
        if (pHeader[i] == 0) pHeader[i] = ' ';

    cReadMode = 0;

    token = strtok(pHeader, seps);
    while (token != nullptr)
    {
        if (cReadMode != 0)
        {
            switch (cReadMode)
            {
                case 1:
                    m_sMapSizeX = atoi(token);
                    cReadMode = 0;
                    break;
                case 2:
                    m_sMapSizeY = atoi(token);
                    cReadMode = 0;
                    break;
            }
        }
        else
        {
            if (memcmp(token, "MAPSIZEX", 8) == 0) cReadMode = 1;
            if (memcmp(token, "MAPSIZEY", 8) == 0) cReadMode = 2;
        }
        token = strtok(nullptr, seps);
    }
}

bool CMapData::bGetIsLocateable(short sX, short sY) const
{
    int dX, dY;

    if ((sX < 0) || (sX > MAPDATASIZEX) ||
        (sY < 0) || (sY > MAPDATASIZEY)) return false;

    dX = sX;
    dY = sY;

    //Change 3.51 3x3 mob check
    //0x0101
    {
        //Wyvern
        if (m_pData[dX - 1][dY - 1].m_sOwnerType == 66) return false;
        if (m_pData[dX - 1][dY].m_sOwnerType == 66) return false;
        if (m_pData[dX - 1][dY + 1].m_sOwnerType == 66) return false;
        if (m_pData[dX][dY - 1].m_sOwnerType == 66) return false;
        if (m_pData[dX][dY + 1].m_sOwnerType == 66) return false;
        if (m_pData[dX + 1][dY - 1].m_sOwnerType == 66) return false;
        if (m_pData[dX + 1][dY].m_sOwnerType == 66) return false;
        if (m_pData[dX + 1][dY + 1].m_sOwnerType == 66) return false;
        //Fire-Wyvern
        if (m_pData[dX - 1][dY - 1].m_sOwnerType == 73) return false;
        if (m_pData[dX - 1][dY].m_sOwnerType == 73) return false;
        if (m_pData[dX - 1][dY + 1].m_sOwnerType == 73) return false;
        if (m_pData[dX][dY - 1].m_sOwnerType == 73) return false;
        if (m_pData[dX][dY + 1].m_sOwnerType == 73) return false;
        if (m_pData[dX + 1][dY - 1].m_sOwnerType == 73) return false;
        if (m_pData[dX + 1][dY].m_sOwnerType == 73) return false;
        if (m_pData[dX + 1][dY + 1].m_sOwnerType == 73) return false;
        //Abaddon
        if (m_pData[dX - 1][dY - 1].m_sOwnerType == 81) return false;
        if (m_pData[dX - 1][dY].m_sOwnerType == 81) return false;
        if (m_pData[dX - 1][dY + 1].m_sOwnerType == 81) return false;
        if (m_pData[dX][dY - 1].m_sOwnerType == 81) return false;
        if (m_pData[dX][dY + 1].m_sOwnerType == 81) return false;
        if (m_pData[dX + 1][dY - 1].m_sOwnerType == 81) return false;
        if (m_pData[dX + 1][dY].m_sOwnerType == 81) return false;
        if (m_pData[dX + 1][dY + 1].m_sOwnerType == 81) return false;
    }

    if (m_pData[dX][dY].m_sOwnerType != 0) return false;
    if (m_tile[sX][sY].m_bIsMoveAllowed == false) return false;
    if (m_pData[dX][dY].m_sDynamicObjectType == DEF_DYNAMICOBJECT_MINERAL1) return false;
    if (m_pData[dX][dY].m_sDynamicObjectType == DEF_DYNAMICOBJECT_MINERAL2) return false;
    return true;
}

bool CMapData::bIsTeleportLoc(short sX, short sY) const
{
    if ((sX < 0) || (sX > MAPDATASIZEX) ||
        (sY < 0) || (sY > MAPDATASIZEY)) return false;

    if (m_tile[sX][sY].m_bIsTeleport == false) return false;

    return true;
}

bool __fastcall CMapData::bSetOwner(uint16_t wObjectID, int sX, int sY, int sType, int cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int iStatus, char * pName, short sAction, short sV1, short sV2, short sV3, int iPreLoc, int iFrame)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int iX{}, iY{}, dX{}, dY{};
    int iChatIndex{}, iAdd{};
    char cTmpName[12]{};
    int64_t dwTime{};
    int iEffectType{}, iEffectFrame{}, iEffectTotalFrame{};

    memset(cTmpName, 0, sizeof(cTmpName));
    strcpy(cTmpName, pName);
    dwTime = m_dwFrameTime;

    iEffectType = iEffectFrame = iEffectTotalFrame = 0;

    if ((wObjectID < 30000) && ((sX < 0) || (sX >= MAPDATASIZEX) || (sY < 0) || (sY >= MAPDATASIZEY)))
    {
        if (m_iObjectIDcacheLocX[wObjectID] > 0)
        {
            iX = m_iObjectIDcacheLocX[wObjectID];
            iY = m_iObjectIDcacheLocY[wObjectID];

            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {
                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if (m_pData[iX][iY].m_wObjectID == wObjectID)
            {
                m_pData[iX][iY].m_sOwnerType = owner_type::ot_none;
                memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));
                memset(pName, 0, strlen(pName));

                if (m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg] != nullptr)
                {
                    delete m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg];
                    m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg] = nullptr;
                }

                m_pData[iX][iY].m_iChatMsg = 0;
                m_pData[iX][iY].m_iEffectType = 0;

                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;

                return false;
            }
        }
        else if (m_iObjectIDcacheLocX[wObjectID] < 0)
        {
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);


            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {
                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
            {
                m_pData[iX][iY].m_cDeadOwnerFrame = 0;
                memset(pName, 0, strlen(pName));

                if (m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg] != nullptr)
                {
                    delete m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg];
                    m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg] = nullptr;
                }

                m_pData[iX][iY].m_iDeadChatMsg = 0;

                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;

                return false;
            }
        }

        int32_t x_width = (m_pGame->get_virtual_width() / 32) / 2;
        int32_t y_height = (m_pGame->get_virtual_height() / 32) / 2;

        for (iX = m_pGame->m_sPlayerX - x_width; iX < m_pGame->m_sPlayerX + x_width; iX++)
            for (iY = m_pGame->m_sPlayerY - y_height; iY < m_pGame->m_sPlayerY + y_height; iY++)
            {
                // todo: optimize - we shouldn't need to check every single tile
                if (m_pData[iX][iY].m_wObjectID == wObjectID)
                {
                    m_pData[iX][iY].m_sOwnerType = owner_type::ot_none;
                    memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));
                    memset(pName, 0, strlen(pName));

                    if (m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg] != nullptr)
                    {
                        delete m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg];
                        m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iChatMsg] = nullptr;
                    }
                    m_pData[iX][iY].m_iChatMsg = 0;

                    m_iObjectIDcacheLocX[wObjectID] = 0;
                    m_iObjectIDcacheLocY[wObjectID] = 0;

                    m_pData[iX][iY].m_iEffectType = 0;

                    return false;
                }

                if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
                {
                    m_pData[iX][iY].m_cDeadOwnerFrame = owner_type::ot_none;
                    memset(pName, 0, strlen(pName));

                    if (m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg] != nullptr)
                    {
                        delete m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg];
                        m_pGame->m_pChatMsgList[m_pData[iX][iY].m_iDeadChatMsg] = nullptr;
                    }

                    m_pData[iX][iY].m_iDeadChatMsg = 0;

                    m_iObjectIDcacheLocX[wObjectID] = 0;
                    m_iObjectIDcacheLocY[wObjectID] = 0;

                    return false;
                }
            }

        memset(pName, 0, strlen(pName));
        return false;
    }

    iChatIndex = 0;

    if ((wObjectID < 30000) && (sAction != DEF_OBJECTNULLACTION))
    {
        memset(cTmpName, 0, sizeof(cTmpName));
        strcpy(cTmpName, pName);

        dX = sX;
        dY = sY;

        if (m_pData[dX][dY].m_wDeadObjectID > 30000) return false;

        if (m_iObjectIDcacheLocX[wObjectID] > 0)
        {
            iX = m_iObjectIDcacheLocX[wObjectID];
            iY = m_iObjectIDcacheLocY[wObjectID];

            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {
                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if (m_pData[iX][iY].m_wObjectID == wObjectID)
            {
                iChatIndex = m_pData[iX][iY].m_iChatMsg;
                iEffectType = m_pData[iX][iY].m_iEffectType;
                iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;

                m_pData[iX][iY].m_wObjectID = 0;
                m_pData[iX][iY].m_iChatMsg = 0;
                m_pData[iX][iY].m_sOwnerType = 0;
                memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));

                m_iObjectIDcacheLocX[wObjectID] = sX;
                m_iObjectIDcacheLocY[wObjectID] = sY;

                goto EXIT_SEARCH_LOOP;
            }
        }
        else if (m_iObjectIDcacheLocX[wObjectID] < 0)
        {
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);

            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {

                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
            {
                iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;

                iEffectType = m_pData[iX][iY].m_iEffectType;
                iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;

                m_pData[iX][iY].m_wDeadObjectID = 0;
                m_pData[iX][iY].m_iDeadChatMsg = 0;
                m_pData[iX][iY].m_sDeadOwnerType = 0;

                m_iObjectIDcacheLocX[wObjectID] = -1 * sX;
                m_iObjectIDcacheLocY[wObjectID] = -1 * sY;

                goto EXIT_SEARCH_LOOP;
            }
        }

        iAdd = 20;
        for (iX = sX - iAdd; iX <= sX + iAdd; iX++)
            for (iY = sY - iAdd; iY <= sY + iAdd; iY++)
            {
                if (iX < 0) break;
                if (iX >= MAPDATASIZEX) break;
                if (iY < 0) break;
                if (iY >= MAPDATASIZEY) break;

                if (m_pData[iX][iY].m_wObjectID == wObjectID)
                {
                    iChatIndex = m_pData[iX][iY].m_iChatMsg;

                    iEffectType = m_pData[iX][iY].m_iEffectType;
                    iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                    iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;

                    m_pData[iX][iY].m_wObjectID = 0;
                    m_pData[iX][iY].m_iChatMsg = 0;
                    m_pData[iX][iY].m_sOwnerType = 0;
                    m_pData[iX][iY].m_iEffectType = 0;

                    memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));

                    m_iObjectIDcacheLocX[wObjectID] = sX;
                    m_iObjectIDcacheLocY[wObjectID] = sY;

                    goto EXIT_SEARCH_LOOP;
                }

                if (m_pData[iX][iY].m_wDeadObjectID == wObjectID)
                {
                    iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;

                    iEffectType = m_pData[iX][iY].m_iEffectType;
                    iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                    iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;

                    m_pData[iX][iY].m_wDeadObjectID = 0;
                    m_pData[iX][iY].m_iDeadChatMsg = 0;
                    m_pData[iX][iY].m_sDeadOwnerType = 0;

                    memset(m_pData[iX][iY].m_cDeadOwnerName, 0, sizeof(m_pData[iX][iY].m_cDeadOwnerName));

                    m_iObjectIDcacheLocX[wObjectID] = -1 * sX;
                    m_iObjectIDcacheLocY[wObjectID] = -1 * sY;

                    goto EXIT_SEARCH_LOOP;
                }
            }

        m_iObjectIDcacheLocX[wObjectID] = sX;
        m_iObjectIDcacheLocY[wObjectID] = sY;
    }
    else
    {
        if (sAction != DEF_OBJECTNULLACTION)
            wObjectID -= 30000;

        if (wObjectID >= 30000) return false;

        if (m_iObjectIDcacheLocX[wObjectID] > 0)
        {
            iX = m_iObjectIDcacheLocX[wObjectID];
            iY = m_iObjectIDcacheLocY[wObjectID];


            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {
                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if (m_pData[iX][iY].m_wObjectID == wObjectID)
            {

                dX = iX;
                dY = iY;
                switch (sAction)
                {
                    case DEF_OBJECTRUN:
                    case DEF_OBJECTMOVE:
                    case DEF_OBJECTDAMAGEMOVE:
                    case DEF_OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;

                    default:
                        break;
                }

                if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
                    (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
                {
                    m_pGame->RequestFullObjectData(wObjectID);
                    memset(pName, 0, strlen(pName));
                    return false;
                }

                iChatIndex = m_pData[iX][iY].m_iChatMsg;
                if (sAction != DEF_OBJECTNULLACTION)
                {
                    sType = m_pData[iX][iY].m_sOwnerType;
                    sAppr1 = m_pData[iX][iY].m_sAppr1;
                    sAppr2 = m_pData[iX][iY].m_sAppr2;
                    sAppr3 = m_pData[iX][iY].m_sAppr3;
                    sAppr4 = m_pData[iX][iY].m_sAppr4;
                    iApprColor = m_pData[iX][iY].m_iApprColor;
                    iStatus = m_pData[iX][iY].m_iStatus;

                    iEffectType = m_pData[iX][iY].m_iEffectType;
                    iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                    iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;
                }
                memset(cTmpName, 0, sizeof(cTmpName));
                memcpy(cTmpName, m_pData[iX][iY].m_cOwnerName, 10);

                memset(pName, 0, sizeof(pName));
                memcpy(pName, m_pData[iX][iY].m_cOwnerName, 10);

                m_pData[iX][iY].m_wObjectID = 0;
                m_pData[iX][iY].m_iChatMsg = 0;
                m_pData[iX][iY].m_sOwnerType = 0;
                m_pData[iX][iY].m_iEffectType = 0;
                memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));

                m_iObjectIDcacheLocX[wObjectID] = dX;
                m_iObjectIDcacheLocY[wObjectID] = dY;

                goto EXIT_SEARCH_LOOP;
            }
        }
        else if (m_iObjectIDcacheLocX[wObjectID] < 0)
        {
            iX = abs(m_iObjectIDcacheLocX[wObjectID]);
            iY = abs(m_iObjectIDcacheLocY[wObjectID]);

            if ((iX < 0) || (iX >= MAPDATASIZEX) || (iY < 0) || (iY >= MAPDATASIZEY))
            {
                m_iObjectIDcacheLocX[wObjectID] = 0;
                m_iObjectIDcacheLocY[wObjectID] = 0;
                return false;
            }

            if ((m_pData[iX][iY].m_cDeadOwnerFrame == -1) && (m_pData[iX][iY].m_wDeadObjectID == wObjectID))
            {
                dX = iX;
                dY = iY;
                switch (sAction)
                {
                    case DEF_OBJECTMOVE:
                    case DEF_OBJECTRUN:
                    case DEF_OBJECTDAMAGEMOVE:
                    case DEF_OBJECTATTACKMOVE:
                        switch (cDir)
                        {
                            case 1: dY--; break;
                            case 2: dY--; dX++; break;
                            case 3: dX++; break;
                            case 4: dX++; dY++; break;
                            case 5: dY++; break;
                            case 6: dX--; dY++; break;
                            case 7: dX--; break;
                            case 8: dX--; dY--; break;
                        }
                        break;

                    default:
                        break;
                }

                if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
                    (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
                {
                    m_pGame->RequestFullObjectData(wObjectID);
                    memset(pName, 0, strlen(pName));
                    return false;
                }


                iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;
                if (sAction != DEF_OBJECTNULLACTION)
                {
                    sType = m_pData[iX][iY].m_sDeadOwnerType;
                    sAppr1 = m_pData[iX][iY].m_sDeadAppr1;
                    sAppr2 = m_pData[iX][iY].m_sDeadAppr2;
                    sAppr3 = m_pData[iX][iY].m_sDeadAppr3;
                    sAppr4 = m_pData[iX][iY].m_sDeadAppr4;
                    iApprColor = m_pData[iX][iY].m_iDeadApprColor;
                    iStatus = m_pData[iX][iY].m_iDeadStatus;
                }
                memset(cTmpName, 0, sizeof(cTmpName));
                memcpy(cTmpName, m_pData[iX][iY].m_cDeadOwnerName, 10);
                memset(pName, 0, sizeof(pName));
                memcpy(pName, m_pData[iX][iY].m_cDeadOwnerName, 10);
                m_pData[iX][iY].m_wDeadObjectID = 0;
                m_pData[iX][iY].m_iDeadChatMsg = 0;
                m_pData[iX][iY].m_sDeadOwnerType = 0;
                memset(m_pData[iX][iY].m_cDeadOwnerName, 0, sizeof(m_pData[iX][iY].m_cDeadOwnerName));
                m_iObjectIDcacheLocX[wObjectID] = -1 * (dX);
                m_iObjectIDcacheLocY[wObjectID] = -1 * (dY);
                goto EXIT_SEARCH_LOOP;
            }
        }

        int32_t x_width = (m_pGame->get_virtual_width() / 32) / 2;
        int32_t y_height = (m_pGame->get_virtual_height() / 32) / 2;

        for (iX = m_pGame->m_sPlayerX - x_width; iX < m_pGame->m_sPlayerX + x_width; iX++)
            for (iY = m_pGame->m_sPlayerY - y_height; iY < m_pGame->m_sPlayerY + y_height; iY++)
            {
                if (iX < 0 || iY < 0 || iY >= MAPDATASIZEY || iX >= MAPDATASIZEX) continue;
                if (m_pData[iX][iY].m_wObjectID == wObjectID)
                {
                    dX = iX;
                    dY = iY;
                    switch (sAction)
                    {
                        case DEF_OBJECTRUN:
                        case DEF_OBJECTMOVE:
                        case DEF_OBJECTDAMAGEMOVE:
                        case DEF_OBJECTATTACKMOVE:
                            switch (cDir)
                            {
                                case 1: dY--; break;
                                case 2: dY--; dX++; break;
                                case 3: dX++; break;
                                case 4: dX++; dY++; break;
                                case 5: dY++; break;
                                case 6: dX--; dY++; break;
                                case 7: dX--; break;
                                case 8: dX--; dY--; break;
                            }
                            break;

                        default:
                            break;
                    }

                    if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
                        (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
                    {
                        m_pGame->RequestFullObjectData(wObjectID);
                        memset(pName, 0, strlen(pName));
                        return false;
                    }


                    iChatIndex = m_pData[iX][iY].m_iChatMsg;
                    if (sAction != DEF_OBJECTNULLACTION)
                    {
                        sType = m_pData[iX][iY].m_sOwnerType;
                        sAppr1 = m_pData[iX][iY].m_sAppr1;
                        sAppr2 = m_pData[iX][iY].m_sAppr2;
                        sAppr3 = m_pData[iX][iY].m_sAppr3;
                        sAppr4 = m_pData[iX][iY].m_sAppr4;
                        iApprColor = m_pData[iX][iY].m_iApprColor;
                        iStatus = m_pData[iX][iY].m_iStatus;
                        iEffectType = m_pData[iX][iY].m_iEffectType;
                        iEffectFrame = m_pData[iX][iY].m_iEffectFrame;
                        iEffectTotalFrame = m_pData[iX][iY].m_iEffectTotalFrame;
                    }
                    memset(cTmpName, 0, sizeof(cTmpName));
                    memcpy(cTmpName, m_pData[iX][iY].m_cOwnerName, 10);
                    memset(pName, 0, sizeof(pName));
                    memcpy(pName, m_pData[iX][iY].m_cOwnerName, 10);
                    m_pData[iX][iY].m_wObjectID = 0;
                    m_pData[iX][iY].m_iChatMsg = 0;
                    m_pData[iX][iY].m_sOwnerType = 0;
                    m_pData[iX][iY].m_iEffectType = 0;
                    memset(m_pData[iX][iY].m_cOwnerName, 0, sizeof(m_pData[iX][iY].m_cOwnerName));
                    m_iObjectIDcacheLocX[wObjectID] = dX;
                    m_iObjectIDcacheLocY[wObjectID] = dY;
                    goto EXIT_SEARCH_LOOP;
                }

                if (m_pData[iX][iY].m_wDeadObjectID == wObjectID)
                {
                    dX = iX;
                    dY = iY;
                    switch (sAction)
                    {
                        case DEF_OBJECTMOVE:
                        case DEF_OBJECTRUN:
                        case DEF_OBJECTDAMAGEMOVE:
                        case DEF_OBJECTATTACKMOVE:
                            switch (cDir)
                            {
                                case 1: dY--; break;
                                case 2: dY--; dX++; break;
                                case 3: dX++; break;
                                case 4: dX++; dY++; break;
                                case 5: dY++; break;
                                case 6: dX--; dY++; break;
                                case 7: dX--; break;
                                case 8: dX--; dY--; break;
                            }
                            break;

                        default:
                            break;
                    }

                    if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
                        (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_wObjectID != wObjectID))
                    {
                        m_pGame->RequestFullObjectData(wObjectID);
                        memset(pName, 0, strlen(pName));
                        return false;
                    }


                    iChatIndex = m_pData[iX][iY].m_iDeadChatMsg;
                    if (sAction != DEF_OBJECTNULLACTION)
                    {
                        sType = m_pData[iX][iY].m_sDeadOwnerType;
                        sAppr1 = m_pData[iX][iY].m_sDeadAppr1;
                        sAppr2 = m_pData[iX][iY].m_sDeadAppr2;
                        sAppr3 = m_pData[iX][iY].m_sDeadAppr3;
                        sAppr4 = m_pData[iX][iY].m_sDeadAppr4;
                        iApprColor = m_pData[iX][iY].m_iDeadApprColor;
                        iStatus = m_pData[iX][iY].m_iDeadStatus;
                    }
                    memset(cTmpName, 0, sizeof(cTmpName));
                    memcpy(cTmpName, m_pData[iX][iY].m_cDeadOwnerName, 10);
                    memset(pName, 0, sizeof(pName));
                    memcpy(pName, m_pData[iX][iY].m_cDeadOwnerName, 10);
                    m_pData[iX][iY].m_wDeadObjectID = 0;
                    m_pData[iX][iY].m_iDeadChatMsg = 0;
                    m_pData[iX][iY].m_sDeadOwnerType = 0;
                    m_pData[iX][iY].m_iEffectType = 0;
                    memset(m_pData[iX][iY].m_cDeadOwnerName, 0, sizeof(m_pData[iX][iY].m_cDeadOwnerName));
                    m_iObjectIDcacheLocX[wObjectID] = -1 * (dX);
                    m_iObjectIDcacheLocY[wObjectID] = -1 * (dY);
                    goto EXIT_SEARCH_LOOP;
                }
            }
        m_pGame->RequestFullObjectData(wObjectID);
        memset(pName, 0, strlen(pName));
        return false;
    }

    EXIT_SEARCH_LOOP:;

    if (sAction == DEF_OBJECTDYING)
    {
        dX = sX;
        dY = sY;
    }

    if ((iPreLoc == 0) && (m_pData[dX][dY].m_sOwnerType != 0) && (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTDYING))
    {
        m_pData[dX][dY].m_wDeadObjectID = m_pData[dX][dY].m_wObjectID;
        m_pData[dX][dY].m_sDeadOwnerType = m_pData[dX][dY].m_sOwnerType;
        m_pData[dX][dY].m_cDeadDir = m_pData[dX][dY].m_cDir;
        m_pData[dX][dY].m_sDeadAppr1 = m_pData[dX][dY].m_sAppr1;
        m_pData[dX][dY].m_sDeadAppr2 = m_pData[dX][dY].m_sAppr2;
        m_pData[dX][dY].m_sDeadAppr3 = m_pData[dX][dY].m_sAppr3;
        m_pData[dX][dY].m_sDeadAppr4 = m_pData[dX][dY].m_sAppr4;
        m_pData[dX][dY].m_iDeadApprColor = m_pData[dX][dY].m_iApprColor;
        m_pData[dX][dY].m_iDeadStatus = m_pData[dX][dY].m_iStatus;
        m_pData[dX][dY].m_cDeadOwnerFrame = -1;
        m_pData[dX][dY].m_dwDeadOwnerTime = dwTime;

        memcpy(m_pData[dX][dY].m_cDeadOwnerName, m_pData[dX][dY].m_cOwnerName, 11);
        m_pData[dX][dY].m_iDeadChatMsg = m_pData[dX][dY].m_iChatMsg;
        m_pData[dX][dY].m_wObjectID = 0;
        m_pData[dX][dY].m_sOwnerType = 0;
        m_pData[dX][dY].m_iChatMsg = 0;
        memset(m_pData[dX][dY].m_cOwnerName, 0, sizeof(m_pData[dX][dY].m_cOwnerName));

        if (m_pData[dX][dY].m_wDeadObjectID < 0 || m_pData[dX][dY].m_wDeadObjectID > 30000) return false;

        m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID] = -1 * m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID];
        m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID] = -1 * m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID];

        if (m_pData[dX][dY].m_iEffectType != 0)
        {
            m_pData[dX][dY].m_iEffectType = 0;
            m_pData[dX][dY].m_iEffectFrame = 0;
            m_pData[dX][dY].m_iEffectTotalFrame = 0;
            m_pData[dX][dY].m_dwEffectTime = 0;
        }
    }
    if (m_pData[dX][dY].m_sOwnerType != 0)
    {
        if ((wObjectID != (uint16_t)m_pGame->m_sPlayerObjectID) &&
            (m_pData[dX][dY].m_wObjectID == (uint16_t)m_pGame->m_sPlayerObjectID))
        {
            return false;
        }
        else
        {
            return false;
        }
    }
    if (iPreLoc == 0)
    {
        m_pData[dX][dY].m_wObjectID = wObjectID;
        m_pData[dX][dY].m_sOwnerType = sType;
        m_pData[dX][dY].m_cDir = cDir;
        m_pData[dX][dY].m_sAppr1 = sAppr1;
        m_pData[dX][dY].m_sAppr2 = sAppr2;
        m_pData[dX][dY].m_sAppr3 = sAppr3;
        m_pData[dX][dY].m_sAppr4 = sAppr4;
        m_pData[dX][dY].m_iApprColor = iApprColor;
        m_pData[dX][dY].m_iStatus = iStatus;
        m_pData[dX][dY].m_sV1 = sV1;
        m_pData[dX][dY].m_sV2 = sV2;
        m_pData[dX][dY].m_sV3 = sV3;

        m_pData[dX][dY].m_iEffectType = iEffectType;
        m_pData[dX][dY].m_iEffectFrame = iEffectFrame;
        m_pData[dX][dY].m_iEffectTotalFrame = iEffectTotalFrame;

        memset(m_pData[dX][dY].m_cOwnerName, 0, sizeof(m_pData[dX][dY].m_cOwnerName));
        strcpy(m_pData[dX][dY].m_cOwnerName, cTmpName);

        if ((sAction != DEF_OBJECTNULLACTION) && (sAction != DEF_MSGTYPE_CONFIRM) && (sAction != DEF_MSGTYPE_REJECT))
        {
            m_pData[dX][dY].m_cOwnerFrame = iFrame;
            m_pData[dX][dY].max_frames = m_stFrame[sType][sAction].m_sMaxFrame + 1;
            m_pData[dX][dY].frame_time = m_stFrame[sType][sAction].m_sFrameTime;
            m_pData[dX][dY].start_time = dwTime;
            m_pData[dX][dY].m_cOwnerAction = (char)sAction;
        }

        m_pData[dX][dY].m_dwOwnerTime = dwTime;
        m_pData[dX][dY].m_iChatMsg = iChatIndex;

        if ((sAppr4 & 0x00F0) != 0)
        {
            m_pData[dX][dY].m_iEffectType = (sAppr4 & 0x00F0) >> 4;
            if (sAction == DEF_OBJECTNULLACTION)
            {
                m_pData[dX][dY].m_iEffectFrame = 0;
                m_pData[dX][dY].m_dwEffectTime = dwTime;
            }

            switch (m_pData[dX][dY].m_iEffectType)
            {
                case 1: m_pData[dX][dY].m_iEffectTotalFrame = 13; break;
                case 2: m_pData[dX][dY].m_iEffectTotalFrame = 11; break;
            }
        }
        else
        {
            m_pData[dX][dY].m_iEffectType = 0;
        }
    }
    else
    {
        // iPreLoc == 1
        m_pData[dX][dY].m_wDeadObjectID = wObjectID;
        m_pData[dX][dY].m_sDeadOwnerType = sType;
        m_pData[dX][dY].m_cDeadDir = cDir;
        m_pData[dX][dY].m_sDeadAppr1 = sAppr1;
        m_pData[dX][dY].m_sDeadAppr2 = sAppr2;
        m_pData[dX][dY].m_sDeadAppr3 = sAppr3;
        m_pData[dX][dY].m_sDeadAppr4 = sAppr4;
        m_pData[dX][dY].m_iDeadApprColor = iApprColor;
        m_pData[dX][dY].m_iDeadStatus = iStatus;

        memset(m_pData[dX][dY].m_cDeadOwnerName, 0, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
        strcpy(m_pData[dX][dY].m_cDeadOwnerName, cTmpName);

        m_pData[dX][dY].m_dwDeadOwnerTime = dwTime;
        m_pData[dX][dY].m_iDeadChatMsg = iChatIndex;

        if ((sAppr4 & 0x00F0) != 0)
        {
            m_pData[dX][dY].m_iEffectType = (sAppr4 & 0x00F0) >> 4;
            if (sAction == DEF_OBJECTNULLACTION)
            {
                m_pData[dX][dY].m_iEffectFrame = 0;
                m_pData[dX][dY].m_dwEffectTime = dwTime;
            }

            switch (m_pData[dX][dY].m_iEffectType)
            {
                case 1: m_pData[dX][dY].m_iEffectTotalFrame = 13; break;
                case 2: m_pData[dX][dY].m_iEffectTotalFrame = 11; break;
            }
        }
        else
        {
            m_pData[dX][dY].m_iEffectType = 0;
        }
    }

    return true;
}

bool __fastcall CMapData::bGetOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, int * pStatus, char * pName, char * pAction, char * pFrame, int * pChatIndex, short * pV1, short * pV2)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY;

    if ((sX < 0) || (sX > MAPDATASIZEX) ||
        (sY < 0) || (sY > MAPDATASIZEY))
    {
        memset(pName, 0, sizeof(pName));
        return false;
    }

    dX = sX;
    dY = sY;

    *pOwnerType = m_pData[dX][dY].m_sOwnerType;
    *pAction = m_pData[dX][dY].m_cOwnerAction;
    *pDir = m_pData[dX][dY].m_cDir;
    *pFrame = m_pData[dX][dY].m_cOwnerFrame;
    *pAppr1 = m_pData[dX][dY].m_sAppr1;
    *pAppr2 = m_pData[dX][dY].m_sAppr2;
    *pAppr3 = m_pData[dX][dY].m_sAppr3;
    *pAppr4 = m_pData[dX][dY].m_sAppr4;
    *pApprColor = m_pData[dX][dY].m_iApprColor;
    *pStatus = m_pData[dX][dY].m_iStatus;
    *pChatIndex = m_pData[dX][dY].m_iChatMsg;
    *pV1 = m_pData[dX][dY].m_sV1;
    *pV2 = m_pData[dX][dY].m_sV2;

    strcpy(pName, m_pData[dX][dY].m_cOwnerName);

    return true;
}

bool __fastcall CMapData::bGetDeadOwner(short sX, short sY, short * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pApprColor, char * pFrame, char * pName, short * pItemSprite, short * pItemSpriteFrame, int * pChatIndex)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY;

    if ((sX < 0) || (sX > MAPDATASIZEX) ||
        (sY < 0) || (sY > MAPDATASIZEY))
    {

        memset(pName, 0, sizeof(pName));
        *pItemSprite = 0;
        return false;
    }

    dX = sX;
    dY = sY;

    *pOwnerType = m_pData[dX][dY].m_sDeadOwnerType;
    *pDir = m_pData[dX][dY].m_cDeadDir;
    *pAppr1 = m_pData[dX][dY].m_sDeadAppr1;
    *pAppr2 = m_pData[dX][dY].m_sDeadAppr2;
    *pAppr3 = m_pData[dX][dY].m_sDeadAppr3;
    *pAppr4 = m_pData[dX][dY].m_sDeadAppr4;
    *pApprColor = m_pData[dX][dY].m_iDeadApprColor;
    *pFrame = m_pData[dX][dY].m_cDeadOwnerFrame;
    *pChatIndex = m_pData[dX][dY].m_iDeadChatMsg;

    strcpy(pName, m_pData[dX][dY].m_cDeadOwnerName);

    *pItemSprite = m_pData[dX][dY].m_sItemSprite;
    *pItemSpriteFrame = m_pData[dX][dY].m_sItemSpriteFrame;

    return true;
}

void CMapData::object_frame_counter(short sViewPointX, short sViewPointY, bool self_only)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX{}, dY{}, sVal{};
    int64_t dwTime{}, dwRealTime{}, dwFrameTime{};
    int  iDelay{};
    int  iRet{}, iSoundIndex{}, iSkipFrame{};
    int  cDir{}, cTotalFrame{}, cFrameMoveDots{};
    static uint32_t S_dwUpdateTime = unixtime();
    int   sWeaponType{}, sCenterX{}, sCenterY{}, sDist{};//, sAbsX, sAbsY;
    bool  bAutoUpdate = false, dynObjsNeedUpdate = false;
    short dx{}, dy{};
    long  lPan{};

    iRet = 0;

    dwTime = dwRealTime = unixtime();

    if ((dwTime - m_dwFrameTime) >= 10)
        m_dwFrameTime = dwTime;

    // todo: adjust "center"
    sVal = sViewPointX;
    sCenterX = (sVal / 32) + (m_pGame->get_virtual_width() / 32) / 2;
    sVal = sViewPointY;
    sCenterY = (sVal / 32) + ((m_pGame->get_virtual_height()) / 32) / 2;

    m_sRectX = m_pGame->m_sVDL_X;
    m_sRectY = m_pGame->m_sVDL_Y;

    dynObjsNeedUpdate = (dwTime - m_dwDOframeTime) > 100;

//#pragma omp parallel for firstprivate(sViewPointX, sViewPointY, dX, dY, sVal, iDelay, iSoundIndex, iSkipFrame, cDir, cTotalFrame, cFrameMoveDots, sWeaponType, sCenterX, sCenterY, sDist, dx, dy, lPan, self_only) num_threads(4)
    for (dX = sViewPointX / 32 - 5; dX < (sViewPointX / 32) + (m_pGame->get_virtual_width() / 32) + 5; dX++)
    {
        for (dY = sViewPointY / 32 - 5; dY < (sViewPointY / 32) + ((m_pGame->get_virtual_height()) / 32) + 5; dY++)
        {
            if ((dX <= 0) || (dY <= 0))
                continue;

            // fix: update to handle dead players
            //m_pGame->player_action = m_pData[dX][dY].m_cOwnerAction;

            if ((!self_only && m_pData[dX][dY].m_wObjectID == m_pGame->m_sPlayerObjectID) || (self_only && !m_pData[dX][dY].m_wObjectID == m_pGame->m_sPlayerObjectID))
                continue;

            lPan = sDist = m_pGame->get_distance_from_player(dX, dY).first;

            if ((dwTime - m_dwDOframeTime) > 100)
            {
                m_pGame->update_objects = true;
                m_pData[dX][dY].m_iEffectFrame++;
                switch (m_pData[dX][dY].m_iEffectType)
                {
                    case 1:
                        if (m_pData[dX][dY].m_iEffectTotalFrame < m_pData[dX][dY].m_iEffectFrame)
                            m_pData[dX][dY].m_iEffectFrame = 4;
                        break;
                    case 2:
                        if (m_pData[dX][dY].m_iEffectTotalFrame < m_pData[dX][dY].m_iEffectFrame)
                            m_pData[dX][dY].m_iEffectFrame = 3;
                        break;
                }
                if ((m_pData[dX][dY].m_sDynamicObjectType != 0))
                {
                    m_pData[dX][dY].m_cDynamicObjectFrame++;
                    switch (m_pData[dX][dY].m_sDynamicObjectType)
                    {
                        case DEF_DYNAMICOBJECT_SPIKE:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 13)
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            break;

                        case DEF_DYNAMICOBJECT_ICESTORM:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 10)
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            //if (m_pData[dX][dY].m_cDynamicObjectFrame == 1) 
                            //	m_pGame->PlaySound('E', 16, sDist);
                            break;

                        case DEF_DYNAMICOBJECT_FIRE://Firewall
                        case DEF_DYNAMICOBJECT_FIRE3: //FireBow				
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 24)
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            if (m_pData[dX][dY].m_cDynamicObjectFrame == 1)
                            {
                                m_pGame->PlaySound('E', 9, sDist);
                            }
                            break;

                        case DEF_DYNAMICOBJECT_FIRE2:	//Crusade buildings burning.			
                            if (m_pData[dX][dY].m_cDynamicObjectFrame > 27)
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            if (m_pData[dX][dY].m_cDynamicObjectFrame == 1)
                                m_pGame->PlaySound('E', 9, sDist);
                            if ((m_pData[dX][dY].m_cDynamicObjectFrame % 4) == 0)//Change from 6 -- makes fire consistent
                            {
                                m_pGame->bAddNewEffect(65, (dX) * 32 + (rand() % 10 - 5) + 5, (dY) * 32, 0, 0, 0, 0);
                                m_pGame->bAddNewEffect(67, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                            }
                            break;

                        case DEF_DYNAMICOBJECT_FISHOBJECT:
                            if ((rand() % 12) == 1)
                                m_pGame->bAddNewEffect(13, (dX) * 32 + m_pData[dX][dY].m_cDynamicObjectData1, (dY) * 32 + m_pData[dX][dY].m_cDynamicObjectData2, 0, 0, 0);
                            break;

                        case DEF_DYNAMICOBJECT_FISH:
                            if ((dwTime - m_pData[dX][dY].m_dwDynamicObjectTime) < 100) break;
                            m_pData[dX][dY].m_dwDynamicObjectTime = dwTime;
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 15) m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            if ((rand() % 15) == 1) m_pGame->bAddNewEffect(13, (dX) * 32 + m_pData[dX][dY].m_cDynamicObjectData1, (dY) * 32 + m_pData[dX][dY].m_cDynamicObjectData2, 0, 0, 0);
                            cDir = m_pGame->cGetNextMoveDir(m_pData[dX][dY].m_cDynamicObjectData1, m_pData[dX][dY].m_cDynamicObjectData2, 0, 0);
                            switch (cDir)
                            {
                                case 1:
                                    m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
                                    break;
                                case 2:
                                    m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
                                    m_pData[dX][dY].m_cDynamicObjectData3 += 2;
                                    break;
                                case 3:
                                    m_pData[dX][dY].m_cDynamicObjectData3 += 2;
                                    break;
                                case 4:
                                    m_pData[dX][dY].m_cDynamicObjectData3 += 2;
                                    m_pData[dX][dY].m_cDynamicObjectData4 += 2;
                                    break;
                                case 5:
                                    m_pData[dX][dY].m_cDynamicObjectData4 += 2;
                                    break;
                                case 6:
                                    m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
                                    m_pData[dX][dY].m_cDynamicObjectData4 += 2;
                                    break;
                                case 7:
                                    m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
                                    break;
                                case 8:
                                    m_pData[dX][dY].m_cDynamicObjectData3 -= 2;
                                    m_pData[dX][dY].m_cDynamicObjectData4 -= 2;
                                    break;
                            }

                            if (m_pData[dX][dY].m_cDynamicObjectData3 < -12) m_pData[dX][dY].m_cDynamicObjectData3 = -12;
                            if (m_pData[dX][dY].m_cDynamicObjectData3 > 12) m_pData[dX][dY].m_cDynamicObjectData3 = 12;
                            if (m_pData[dX][dY].m_cDynamicObjectData4 < -12) m_pData[dX][dY].m_cDynamicObjectData4 = -12;
                            if (m_pData[dX][dY].m_cDynamicObjectData4 > 12) m_pData[dX][dY].m_cDynamicObjectData4 = 12;

                            m_pData[dX][dY].m_cDynamicObjectData1 += m_pData[dX][dY].m_cDynamicObjectData3;
                            m_pData[dX][dY].m_cDynamicObjectData2 += m_pData[dX][dY].m_cDynamicObjectData4;
                            break;

                        case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8)
                            {
                                m_pData[dX][dY].m_sDynamicObjectType = DEF_DYNAMICOBJECT_PCLOUD_LOOP;
                                m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 8;
                            }
                            break;

                        case DEF_DYNAMICOBJECT_PCLOUD_LOOP:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8)
                            {
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            }
                            break;

                        case DEF_DYNAMICOBJECT_PCLOUD_END:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8)
                            {
                                m_pData[dX][dY].m_sDynamicObjectType = 0;
                            }
                            break;

                        case DEF_DYNAMICOBJECT_ARESDENFLAG1:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 4)
                            {
                                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                            }
                            break;

                        case DEF_DYNAMICOBJECT_ELVINEFLAG1:
                            if (m_pData[dX][dY].m_cDynamicObjectFrame >= 8)
                            {
                                m_pData[dX][dY].m_cDynamicObjectFrame = 4;
                            }
                            break;

                    }
                }
            }

            // Dead object
            if (m_pData[dX][dY].m_sDeadOwnerType != 0)
                if ((m_pData[dX][dY].m_cDeadOwnerFrame >= 0) && ((dwTime - m_pData[dX][dY].m_dwDeadOwnerTime) > 150))
                {
                    m_pData[dX][dY].m_dwDeadOwnerTime = dwTime;
                    m_pData[dX][dY].m_cDeadOwnerFrame++;

                    if (m_pData[dX][dY].m_cDeadOwnerFrame > 10)
                    {
                        m_pData[dX][dY].m_wDeadObjectID = 0;
                        m_pData[dX][dY].m_sDeadOwnerType = 0;
                        memset(m_pData[dX][dY].m_cDeadOwnerName, 0, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
                    }
                }

            // Alive object
            if (m_pData[dX][dY].m_sOwnerType != 0)
            {
                switch (m_pData[dX][dY].m_cOwnerAction)
                {
                    case DEF_OBJECTATTACK:
                    case DEF_OBJECTATTACKMOVE:
                        iDelay = (m_pData[dX][dY].m_iStatus & 0x000F) * 12;
                        break;

                    case DEF_OBJECTMAGIC:
                        if (m_pGame->m_cSkillMastery[4] == 100) iDelay = -13;
                        else iDelay = 0;
                        break;

                    default:
                        iDelay = 0;
                        break;
                }
                //Change - speed changing
//					if ( m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTMOVE || DEF_OBJECTRUN )
//					{
//						iDelay += G_iStuff;
//					}
                    // v1.42 Frozen
                if ((m_pData[dX][dY].m_iStatus & 0x40) != 0)
                    iDelay += (m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime) >> 2;

                dwFrameTime = m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime + iDelay;

                //if (dwFrameTime == 0) dwFrameTime = 1;
                //if (dwFrameTime > 200) dwFrameTime = m_stFrame[m_pData[dX][dY].m_sOwnerType][m_pData[dX][dY].m_cOwnerAction].m_sFrameTime;

                CTile * tile = &m_pData[dX][dY];

                auto frames_elapsed = ((dwTime - tile->start_time) / dwFrameTime);
                bool loop = false;
                auto frames_advanced = frames_elapsed - (int)tile->m_cOwnerFrame;

                if (frames_elapsed >= tile->max_frames)
                    loop = true;

                tile->m_cOwnerFrame = frames_elapsed % tile->max_frames;

                tile->m_dwOwnerTime = tile->start_time + (frames_elapsed * dwFrameTime);

                if (frames_advanced != 0)
                {
                    if (self_only && m_pData[dX][dY].m_wObjectID == m_pGame->m_sPlayerObjectID)
                    {
                        m_pGame->current_map_action = m_pData[dX][dY].m_cOwnerAction;
                        if (loop)
                        {
                            if (tile->m_cOwnerAction == DEF_OBJECTATTACKMOVE && tile->m_cOwnerFrame >= tile->max_frames)
                                m_pGame->dashing = false;
                        }
                    }

                    if (memcmp(m_pData[dX][dY].m_cOwnerName, m_pGame->m_cPlayerName, 10) == 0)
                    {
                        m_pGame->update_camera_shake = true;
                        //m_pGame->update_effects = true;
                        m_pGame->update_objects = true;
                        m_pGame->update_dialogs = true;

                        switch (m_pData[dX][dY].m_cOwnerAction)
                        {
                            case DEF_OBJECTMOVE:
                            case DEF_OBJECTATTACKMOVE:
                            case DEF_OBJECTDAMAGEMOVE:
                                m_pGame->update_background = true;
                                break;

                            case DEF_OBJECTATTACK:
                            case DEF_OBJECTSTOP:
                            case DEF_OBJECTMAGIC:
                            case DEF_OBJECTDAMAGE:
                            case DEF_OBJECTDYING:
                            case DEF_OBJECTDEAD:
                            case DEF_OBJECTNULLACTION:
                                break;
                        }
                        if ((dwRealTime - m_dwFrameCheckTime) > dwFrameTime)
                            m_dwFrameAdjustTime = ((dwRealTime - m_dwFrameCheckTime) - dwFrameTime);

                        m_dwFrameCheckTime = dwRealTime;
                    }

                    if (loop)
                    {
                        if ((sViewPointX / 32 - 15 <= dX) && (((sViewPointX / 32) + (m_pGame->get_virtual_width() / 32) + 15) >= dX) &&
                            (sViewPointY / 32 - 15 <= dY) && (((sViewPointY / 32) + ((m_pGame->get_virtual_height()) / 32) + 15) >= dY))
                        {
                            if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTDYING)
                            {
                                m_pData[dX][dY].m_wDeadObjectID = m_pData[dX][dY].m_wObjectID;
                                m_pData[dX][dY].m_sDeadOwnerType = m_pData[dX][dY].m_sOwnerType;
                                m_pData[dX][dY].m_cDeadDir = m_pData[dX][dY].m_cDir;
                                m_pData[dX][dY].m_sDeadAppr1 = m_pData[dX][dY].m_sAppr1;
                                m_pData[dX][dY].m_sDeadAppr2 = m_pData[dX][dY].m_sAppr2;
                                m_pData[dX][dY].m_sDeadAppr3 = m_pData[dX][dY].m_sAppr3;
                                m_pData[dX][dY].m_sDeadAppr4 = m_pData[dX][dY].m_sAppr4;
                                m_pData[dX][dY].m_iDeadApprColor = m_pData[dX][dY].m_iApprColor;
                                m_pData[dX][dY].m_iDeadStatus = m_pData[dX][dY].m_iStatus;
                                m_pData[dX][dY].m_iDeadChatMsg = m_pData[dX][dY].m_iChatMsg;
                                m_pData[dX][dY].m_cDeadOwnerFrame = -1;
                                memcpy(m_pData[dX][dY].m_cDeadOwnerName, m_pData[dX][dY].m_cOwnerName, 11);
                                m_pData[dX][dY].m_wObjectID = 0;
                                m_pData[dX][dY].m_sOwnerType = 0;
                                memset(m_pData[dX][dY].m_cOwnerName, 0, sizeof(m_pData[dX][dY].m_cOwnerName));

                                m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID] = -1 * m_iObjectIDcacheLocX[m_pData[dX][dY].m_wDeadObjectID];
                                m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID] = -1 * m_iObjectIDcacheLocY[m_pData[dX][dY].m_wDeadObjectID];
                            }
                            else
                            {
                                m_pData[dX][dY].m_cOwnerAction = DEF_OBJECTSTOP;
                                m_pData[dX][dY].m_dwOwnerTime = dwTime;
                                m_pData[dX][dY].start_time = dwTime;
                                m_pData[dX][dY].m_cOwnerFrame = 0;
                                m_pData[dX][dY].max_frames = m_stFrame[m_pData[dX][dY].m_sOwnerType][DEF_OBJECTSTOP].m_sMaxFrame + 1;
                                m_pData[dX][dY].frame_time = m_stFrame[m_pData[dX][dY].m_sOwnerType][DEF_OBJECTSTOP].m_sFrameTime;
                            }

                            if (memcmp(m_pData[dX][dY].m_cOwnerName, m_pGame->m_cPlayerName, 10) == 0)
                            {
                                m_pGame->can_take_action(true);
                                m_pGame->current_map_action = m_pData[dX][dY].m_cOwnerAction;
                                m_pGame->finished_animation_cycle = true;
                            }
                        }
                        else
                        {
                            m_pData[dX][dY].m_wObjectID = 0;
                            m_pData[dX][dY].m_sOwnerType = 0;

                            memset(m_pData[dX][dY].m_cOwnerName, 0, sizeof(m_pData[dX][dY].m_cOwnerName));

                            if (m_pGame->m_pChatMsgList[m_pData[dX][dY].m_iChatMsg] != nullptr)
                            {
                                delete m_pGame->m_pChatMsgList[m_pData[dX][dY].m_iChatMsg];
                                m_pGame->m_pChatMsgList[m_pData[dX][dY].m_iChatMsg] = nullptr;
                            }
                        }
                    }
                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTSTOP)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6: //Glowing armor/effect
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 5))
                                {
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 20 - 10), (dY) * 32 - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }
                                }
                                break;
                            case 40: // ESG
                            case 41: // GMG
                            case 42: // ManaStone
                                if ((rand() % 40) == 25)
                                {
                                    m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 60 - 30), (dY) * 32 - (rand() % 100) - 5, 0, 0, -(rand() % 12), 0);
                                    m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 60 - 30), (dY) * 32 - (rand() % 100) - 5, 0, 0, -(rand() % 12), 0);
                                    m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 60 - 30), (dY) * 32 - (rand() % 100) - 5, 0, 0, -(rand() % 12), 0);
                                    m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 60 - 30), (dY) * 32 - (rand() % 100) - 5, 0, 0, -(rand() % 12), 0);
                                }
                                break;
                            case 65: // IceGolem
                                if (m_pData[dX][dY].m_cOwnerFrame == 3)
                                {
                                    m_pGame->bAddNewEffect(75, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                    m_pGame->bAddNewEffect(75, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->bAddNewEffect(76, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                    m_pGame->bAddNewEffect(76, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                {
                                    m_pGame->bAddNewEffect(77, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                    m_pGame->bAddNewEffect(77, (dX) * 32 + (rand() % 40 - 20), (dY) * 32 + (rand() % 40 - 20), 0, 0, 0);
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTMOVE)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                            case 46: // TK
                            case 53: // Beholder
                            case 54: // Dark-Elf
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 5))
                                {
                                    m_pGame->PlaySound('C', 8, sDist, lPan);
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        cTotalFrame = 8;
                                        cFrameMoveDots = 32 / cTotalFrame;
                                        dx = dy = 0;
                                        switch (m_pData[dX][dY].m_cDir)
                                        {
                                            case 1: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 2: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 3: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 4: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 5: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 6: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 7: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                            case 8: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        }

                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }
                                }
                                break;

                            case 82: // Sorceress			
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 149, sDist, lPan);
                                break;

                            case 83: // ATK				
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 142, sDist, lPan);
                                break;

                            case 84: // MasterElf		
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('C', 10, sDist, lPan);
                                break;

                            case 85: // DSK			
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 147, sDist, lPan);
                                break;


                            case 10: // Slime
                            case 50: // TW		
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 1, sDist, lPan);
                                break;

                            case 11: // Skeleton
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 13, sDist, lPan);
                                break;

                            case 13: // Cyclops
                            case 49: // HC
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 41, sDist, lPan);
                                break;

                            case 14: // Orc
                            case 48: // SK
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 9, sDist, lPan);
                                break;

                            case 16: // Ant
                            case 43: // LWBeetle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 29, sDist, lPan);
                                break;

                            case 17: // Scorpion
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 21, sDist, lPan);
                                break;

                            case 18: // Zombie
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 17, sDist, lPan);
                                break;

                            case 22: // Snake
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 25, sDist, lPan);
                                break;

                            case 23: // Clay-Golem
                            case 52: // Gargoyle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 37, sDist, lPan);
                                break;

                            case 27: // HH
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 5, sDist, lPan);
                                break;

                            case 28: // Troll
                            case 78: // Minotaur
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 46, sDist, lPan);
                                break;

                            case 29: // Ogre
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 51, sDist, lPan);
                                break;

                            case 30: // Liche
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 55, sDist, lPan);
                                break;

                            case 31: // DD
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 59, sDist, lPan);
                                break;

                            case 32: // Uni
                            case 45: // GHKABS
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 63, sDist, lPan);
                                break;

                            case 33: // WW
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 67, sDist, lPan);
                                break;

                            case 55://Rabbit
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 71, sDist, lPan);
                                break;

                            case 56://Cat
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 72, sDist, lPan);
                                break;

                            case 57://Giant-Frog
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 73, sDist, lPan);
                                break;

                            case 58://Mountain Giant
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 87, sDist, lPan);
                                break;

                            case 59://Ettin
                            case 77: //  MasterMageOrc
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 91, sDist, lPan);
                                break;

                            case 60://Cannibal Plant
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 95, sDist, lPan);
                                break;
                            case 61://Rudolph
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('C', 11, sDist, lPan);
                                break;

                            case 62://DireBoar
                            case 74: // GiantCrayFish
                            case 88: // Barbarian
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 87, sDist, lPan);
                                break;

                            case 63://Frost
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 25, sDist, lPan);
                                break;

                            case 12: // Stone-Golem
                            case 47: // BG			
                            case 65: // IceGolem
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 33, sDist, lPan);
                                break;
                                /*	case 65: // IceGolem effect in 2.20 -- removed in 3.51
                                if ( (m_pData[dX][dY].m_cOwnerFrame == 1) )
                                {	m_pGame->PlaySound('M', 33, sDist, lPan);
                                switch (m_pData[dX][dY].m_cDir) {
                                case 1 : dx = 0; dy = -1; break;
                                case 2 : dy = -1; dx = 1; break;
                                case 3 : dx = 1; dy = 0; break;
                                case 4 : dx = 1; dy = 1; break;
                                case 5 : dx = 0; dy = 1; break;
                                case 6 : dy = 1; dx = -1; break;
                                case 7 : dx = -1; dy = 0; break;
                                case 8 : dx = -1; dy = -1; break;
                                }
                                //m_pGame->bAddNewEffect( 75, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
                                //m_pGame->bAddNewEffect( 76, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
                                //m_pGame->bAddNewEffect( 77, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, dx, dy, 0 );
                                }
                                break;*/

                            case 73: // Fite-Wyvern
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 106, sDist, lPan);
                                break;

                            case 80: // Tentocle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 110, sDist, lPan);
                                break;

                            case 72: // Claw Turtle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 114, sDist, lPan);
                                break;

                            case 71: // Centaurus
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 117, sDist, lPan);
                                break;

                            case 76: // Giant Tree
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 122, sDist, lPan);
                                break;

                            case 75: // Giant Lizard
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 126, sDist, lPan);
                                break;

                            case 70: // Balrog
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 130, sDist, lPan);
                                break;

                            case 79: // Nizie
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 134, sDist, lPan);
                                break;

                            case 81: // Abaddon
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 136, sDist, lPan);
                                break;

                            default:
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 3))
                                    m_pGame->PlaySound('C', 8, sDist, lPan);
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTRUN)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                            case 44: // GHK
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1) || (m_pData[dX][dY].m_cOwnerFrame == 5))
                                {
                                    cTotalFrame = 8;
                                    cFrameMoveDots = 32 / cTotalFrame;

                                    dx = dy = 0;
                                    switch (m_pData[dX][dY].m_cDir)
                                    {
                                        case 1: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 2: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 3: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 4: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 5: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 6: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 7: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 8: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                    }

                                    if ((m_pGame->m_cWhetherEffectType >= 1) && (m_pGame->m_cWhetherEffectType <= 3))
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + dx, (dY) * 32 + dy, 0, 0, 0, 0);
                                    else m_pGame->bAddNewEffect(14, (dX) * 32 + dx, (dY) * 32 + dy, 0, 0, 0, 0);
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }

                                    m_pGame->PlaySound('C', 10, sDist, lPan);
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTATTACKMOVE)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->PlaySound('C', 4, sDist);

                                    cTotalFrame = 8;
                                    cFrameMoveDots = 32 / cTotalFrame;

                                    dx = dy = 0;
                                    switch (m_pData[dX][dY].m_cDir)
                                    {
                                        case 1: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 2: dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 3: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 4: dx = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 5: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 6: dy = -cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 7: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                        case 8: dx = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); dy = cFrameMoveDots * (cTotalFrame - m_pData[dX][dY].m_cOwnerFrame); break;
                                    }
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + dx + (rand() % 20 - 10), (dY) * 32 + dy - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }
                                }
                                else if (m_pData[dX][dY].m_cOwnerFrame == 4)
                                {
                                    if ((m_pGame->m_cWhetherEffectType >= 1) && (m_pGame->m_cWhetherEffectType <= 3))
                                    {
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(32, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                    }
                                    else
                                    {
                                        m_pGame->bAddNewEffect(14, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(14, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(14, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(14, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                        m_pGame->bAddNewEffect(14, (dX) * 32 + ((rand() % 20) - 10), (dY) * 32 + ((rand() % 20) - 10), 0, 0, 0, 0);
                                    }
                                    m_pGame->PlaySound('C', 11, sDist, lPan);
                                }
                                else if (m_pData[dX][dY].m_cOwnerFrame == 5) // vu comme case 5
                                {
                                    sWeaponType = ((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4);
                                    if ((sWeaponType >= 1) && (sWeaponType <= 2))
                                    {
                                        m_pGame->PlaySound('C', 1, sDist, lPan);
                                    }
                                    else if ((sWeaponType >= 3) && (sWeaponType <= 19))
                                    {
                                        m_pGame->PlaySound('C', 2, sDist, lPan);
                                    }
                                    else if ((sWeaponType >= 20) && (sWeaponType <= 39))
                                    {
                                        m_pGame->PlaySound('C', 18, sDist, lPan);
                                    }
                                    else if ((sWeaponType >= 40) && (sWeaponType <= 59))
                                    {
                                        m_pGame->PlaySound('C', 3, sDist, lPan);

                                    }
                                }
                                break;
                        }
                    }

                    if ((m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTATTACK))
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 65: // IceGolem
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->bAddNewEffect(73, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                }
                                break;
                            case 87: // Crossbow Turret (Heldenian)	
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->bAddNewEffect(250, m_pData[dX][dY].m_sV1, m_pData[dX][dY].m_sV2
                                        , m_pData[dX][dY].m_sV1 + dX, m_pData[dX][dY].m_sV2 + dY, 0, 87);
                                    //m_pGame->PlaySound('E', 43, sDist, lPan);
                                }
                                break;
                            case 89: // AGT (Heldenian)						
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->bAddNewEffect(2, m_pData[dX][dY].m_sV1, m_pData[dX][dY].m_sV2
                                        , m_pData[dX][dY].m_sV1 + dX, m_pData[dX][dY].m_sV2 + dY, 0, 89);
                                    //m_pGame->PlaySound('E', 43, sDist, lPan);
                                }
                                break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6: // Human
                                if ((m_pData[dX][dY].m_sV3 >= 20) && (m_pData[dX][dY].m_cOwnerFrame == 2))
                                {
                                    if (m_pGame->bHasHeroSet(m_pData[dX][dY].m_sAppr3, m_pData[dX][dY].m_sAppr4, m_pData[dX][dY].m_sOwnerType) == 1) // Warr hero set
                                    {
                                        m_pGame->bAddNewEffect(243, dX, dY
                                            , dX, dY, 0, 1);
                                    }
                                    switch (m_pData[dX][dY].m_sOwnerType)
                                    {
                                        case 1:
                                        case 2:
                                        case 3:
                                            m_pGame->PlaySound('C', 23, sDist, lPan); // Critical sound	
                                            break;
                                        case 4:
                                        case 5:
                                        case 6:
                                            m_pGame->PlaySound('C', 24, sDist, lPan); // Critical sound		
                                            break;
                                    }
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                {
                                    if ((m_pData[dX][dY].m_sAppr2 & 0xF000) != 0) // not Peace mode								
                                    {
                                        if (m_pData[dX][dY].m_sV3 != 1)
                                        {
                                            m_pGame->bAddNewEffect(m_pData[dX][dY].m_sV3, dX, dY
                                                , dX + m_pData[dX][dY].m_sV1, dY + m_pData[dX][dY].m_sV2
                                                , 0, m_pData[dX][dY].m_sOwnerType);
                                            if (m_pData[dX][dY].m_sV3 >= 20) m_pGame->PlaySound('E', 43, sDist, lPan);
                                        }
                                        if (((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4) == 15) // StormBlade
                                        {
                                            m_pGame->bAddNewEffect(81, dX, dY
                                                , dX + m_pData[dX][dY].m_sV1, dY + m_pData[dX][dY].m_sV2
                                                , 0, m_pData[dX][dY].m_sOwnerType);
                                        }
                                        else
                                        {
                                            m_pGame->bAddNewEffect(250, dX, dY
                                                , dX + m_pData[dX][dY].m_sV1, dY + m_pData[dX][dY].m_sV2
                                                , 0, m_pData[dX][dY].m_sOwnerType);
                                        }
                                    }
                                    // Weapon Glare = m_sAppr4 & 0x000F
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 20 - 10), (dY) * 32 - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 20 - 10), (dY) * 32 - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }
                                }
                                break;

                            default:
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    if (m_pData[dX][dY].m_sV3 == 2) // Arrows...
                                    {
                                        m_pGame->bAddNewEffect(m_pData[dX][dY].m_sV3, dX, dY, dX + m_pData[dX][dY].m_sV1, dY + m_pData[dX][dY].m_sV2, 0, m_pData[dX][dY].m_sOwnerType);
                                    }
                                }
                                break;
                        }

                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                if ((m_pData[dX][dY].m_sAppr2 & 0xF000) != 0)
                                {
                                    sWeaponType = ((m_pData[dX][dY].m_sAppr2 & 0x0FF0) >> 4);
                                    if ((sWeaponType >= 1) && (sWeaponType <= 2))
                                    {
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                        {
                                            m_pGame->PlaySound('C', 1, sDist, lPan);
                                        }
                                    }
                                    else if ((sWeaponType >= 3) && (sWeaponType <= 19))
                                    {
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                        {
                                            m_pGame->PlaySound('C', 2, sDist, lPan);
                                        }
                                    }
                                    else if ((sWeaponType >= 20) && (sWeaponType <= 39))
                                    {
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                        {
                                            m_pGame->PlaySound('C', 18, sDist, lPan);
                                        }
                                    }
                                    else if ((sWeaponType >= 40) && (sWeaponType <= 59))
                                    {
                                        if (m_pData[dX][dY].m_cOwnerFrame == 3)
                                        {
                                            m_pGame->PlaySound('C', 3, sDist, lPan);
                                        }
                                    }
                                }
                                break;

                            case 83: // ATK
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 140, sDist, lPan);
                                break;

                            case 84: // MasterElf
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('C', 8, sDist, lPan);
                                break;

                            case 85: // DSK
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 145, sDist, lPan);
                                break;

                            case 53: // Beholder
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('E', 46, sDist, lPan);
                                break;

                            case 54: // DE
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('C', 3, sDist, lPan);
                                break;

                            case 50: // TW
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('C', 1, sDist, lPan);
                                break;

                            case 10: // Slime
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 2, sDist, lPan);
                                break;

                            case 11: // Skell
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 14, sDist, lPan);
                                break;

                            case 12: // Stone-Golem
                            case 65: // ICeGolem
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 34, sDist, lPan);
                                break;

                            case 13: // Cyclops
                            case 49: // HC
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 42, sDist, lPan);
                                break;

                            case 44: // GHK
                            case 45: // GHKABS
                            case 46: // TK
                            case 52: // GG
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('C', 2, sDist, lPan);
                                break;

                            case 14: // Orc
                            case 48: // SK
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 10, sDist, lPan);
                                break;

                            case 16: // Ant
                            case 43: // LWB
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 30, sDist, lPan);
                                break;

                            case 17: // Scorpion
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 22, sDist, lPan);
                                break;

                            case 18: // Zombie
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 18, sDist, lPan);
                                break;

                            case 22: // Snake
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 26, sDist, lPan);
                                break;

                            case 23: // Clay-Golem
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 38, sDist, lPan);
                                break;

                            case 27: // HH
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 6, sDist, lPan);
                                break;

                            case 28: // Troll
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 47, sDist, lPan);
                                break;

                            case 29: // Ogre
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 52, sDist, lPan);
                                break;

                            case 30: // Liche
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 56, sDist, lPan);
                                break;

                            case 31: // DD
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 60, sDist, lPan);
                                break;

                            case 32: // Uni
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 64, sDist, lPan);
                                break;

                            case 33: // WW
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                    m_pGame->PlaySound('M', 68, sDist, lPan);
                                break;

                            case 55://  Rabbit
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 75, sDist, lPan);
                                break;

                            case 56://  Cat
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 76, sDist, lPan);
                                break;

                            case 57://  Giant-Frog
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 77, sDist, lPan);
                                break;

                            case 58://  Mountain Giant
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 88, sDist, lPan);
                                break;

                            case 59://  Ettin
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 92, sDist, lPan);
                                break;

                            case 60://  Cannibal Plant
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 96, sDist, lPan);
                                break;

                            case 61://  Rudolph
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 38, sDist, lPan);
                                break;

                            case 62://  DireBoar
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 68, sDist, lPan);
                                break;

                            case 63://  Frost
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('C', 4, sDist, lPan);
                                break;

                            case 77: // Master MageOrc
                            case 88: // Barbarian
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 78, sDist, lPan);
                                break;

                            case 74: // GiantCrayFish
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 100, sDist, lPan);
                                break;

                            case 73: // Fire Wyvern
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 107, sDist, lPan);
                                break;

                            case 80: // Tentocle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 111, sDist, lPan);
                                break;

                            case 81: // Abaddon
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 137, sDist, lPan);
                                break;

                            case 72: // Claw-Turtle
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 115, sDist, lPan);
                                break;

                            case 71: // Centaurus
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 119, sDist, lPan);
                                break;

                            case 76: // Giant-Tree
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 123, sDist, lPan);
                                break;

                            case 75: // GiantLizard
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 127, sDist, lPan);
                                break;

                            case 70: // Balrog
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 131, sDist, lPan);
                                break;

                            case 79: //  Nizie
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 135, sDist, lPan);
                                break;

                            case 78: // Minaus
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 104, sDist, lPan);
                                break;

                            case 86: // Heavy BattleTank
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 151, sDist, lPan);
                                break;

                            case 87: // Crosbow Turret
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 153, sDist, lPan);
                                break;

                            case 89: // Cannon Turret
                                if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                    m_pGame->PlaySound('M', 155, sDist, lPan);
                                break;

                            case 34: // Dummy
                            case 35: // EnergySphere
                            default:
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                {
                                    m_pGame->PlaySound('C', 2, sDist, lPan);
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTDAMAGE)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:  // Men
                            case 44: // GHK
                            case 45: // GHKABS
                            case 46: // TK
                                if (m_pData[dX][dY].m_cOwnerFrame == 4)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                    m_pGame->PlaySound('C', 12, sDist, lPan);
                                break;
                            case 4:
                            case 5:
                            case 6: // Women
                                if (m_pData[dX][dY].m_cOwnerFrame == 4)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                    m_pGame->PlaySound('C', 13, sDist, lPan);
                                break;

                            default:
                                if (m_pData[dX][dY].m_cOwnerFrame == 4)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;

                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }

                                switch (m_pData[dX][dY].m_sOwnerType)
                                {
                                    case 88: // Barbarian
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 144, sDist, lPan);
                                        break;

                                    case 83: // ATK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 143, sDist, lPan);
                                        break;

                                    case 84: // MasterElf
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('C', 7, sDist, lPan);
                                        break;

                                    case 85: // DSK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1) m_pGame->PlaySound('M', 148, sDist, lPan);
                                        break;

                                    case 54: // DE
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('C', 13, sDist, lPan);
                                        break;

                                    case 10: // Slime
                                    case 53: // BB
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 3, sDist, lPan);
                                        break;

                                    case 11: // Skell
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 15, sDist, lPan);
                                        break;

                                    case 12: // Stone-Golem
                                    case 65: // IceGolem
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 35, sDist, lPan);
                                        break;

                                    case 13: // Cyclops
                                    case 49: // HC
                                    case 52: // GG
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 43, sDist, lPan);
                                        break;

                                    case 14: // Orc
                                    case 48: // SK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 11, sDist, lPan);
                                        break;

                                    case 16: // Ant
                                    case 43: // LWB
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 31, sDist, lPan);
                                        break;

                                    case 17: // Scorp
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 23, sDist, lPan);
                                        break;

                                    case 18: // Zombie
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 19, sDist, lPan);
                                        break;

                                    case 22: // Snake
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 27, sDist, lPan);
                                        break;

                                    case 23: // Clay-Golem
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 39, sDist, lPan);
                                        break;

                                    case 27: // HH
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 7, sDist, lPan);
                                        break;

                                    case 28: // Troll
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 48, sDist, lPan);
                                        break;

                                    case 29: // Ogre
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 53, sDist, lPan);
                                        break;

                                    case 30: // Liche
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 57, sDist, lPan);
                                        break;

                                    case 31: // DD
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 61, sDist, lPan);
                                        break;

                                    case 32: // Uni
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 65, sDist, lPan);
                                        break;

                                    case 33: // WW
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 69, sDist, lPan);
                                        break;

                                    case 34: // dummy
                                    case 35: // EnergyBall
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5) m_pGame->PlaySound('M', 2, sDist, lPan);
                                        break;

                                    case 55://  Rabbit
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 79, sDist, lPan);
                                        break;

                                    case 56://  Cat
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 80, sDist, lPan);
                                        break;

                                    case 57://  Giant-Frog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 81, sDist, lPan);
                                        break;

                                    case 58: // Mountain Giant
                                    case 77: // MasterMageOrc
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 89, sDist, lPan);
                                        break;

                                    case 59://Ettin
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 93, sDist, lPan);
                                        break;
                                    case 60://Cannabl Plant
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 97, sDist, lPan);
                                        break;
                                    case 61://Rudolph
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 69, sDist, lPan);
                                        break;
                                    case 62://DireBoar
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 78, sDist, lPan);
                                        break;
                                    case 63://Frost
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('C', 13, sDist, lPan);
                                        break;

                                    case 74: // Giant CrayFish
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 101, sDist, lPan);
                                        break;

                                    case 78: // Minotaur
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 102, sDist, lPan);
                                        break;

                                    case 80: // Tentocle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 108, sDist, lPan);
                                        break;

                                    case 81: // Abaddon
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 138, sDist, lPan);
                                        break;

                                    case 72: // ClawTurtle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 112, sDist, lPan);
                                        break;

                                    case 71: // Centaurus
                                    case 82: // Sorceress
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 116, sDist, lPan);
                                        break;

                                    case 76: // GiantTree
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 120, sDist, lPan);
                                        break;

                                    case 75: // GiantLizard
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 124, sDist, lPan);
                                        break;

                                    case 70: // Balrog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 128, sDist, lPan);
                                        break;

                                    case 79: // Nizie
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1)) m_pGame->PlaySound('M', 132, sDist, lPan);
                                        break;
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTDAMAGEMOVE)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;

                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                    m_pGame->PlaySound('C', 12, sDist, lPan);
                                break;

                            case 4:
                            case 5:
                            case 6:
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                    m_pGame->PlaySound('C', 13, sDist, lPan);
                                break;

                            default:
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 4);
                                }

                                switch (m_pData[dX][dY].m_sOwnerType)
                                {
                                    case 83: //ATK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('M', 143, sDist, lPan);
                                        break;
                                    case 84: // MasterElf
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('C', 7, sDist, lPan);
                                        break;
                                    case 88: // Barbarian
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('M', 144, sDist, lPan);
                                        break;
                                    case 85: // DSK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('M', 148, sDist, lPan);
                                        break;

                                    case 10: // Slime
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 3, sDist, lPan);
                                        break;

                                    case 11: // Skell
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 15, sDist, lPan);
                                        break;

                                    case 12: // Stone Golem
                                    case 65: // IceGolem
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 35, sDist, lPan);
                                        break;

                                    case 13: // Cyclops
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 43, sDist, lPan);
                                        break;

                                    case 14: // Orc
                                    case 48: // SK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 11, sDist, lPan);
                                        break;

                                    case 16: // Ant
                                    case 43: // LWB
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 31, sDist, lPan);
                                        break;

                                    case 17: // Scorpion
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 23, sDist, lPan);
                                        break;

                                    case 18: // Zombie
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 19, sDist, lPan);
                                        break;

                                    case 22: // Snake
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 27, sDist, lPan);
                                        break;

                                    case 23: // Clay-Golem
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 39, sDist, lPan);
                                        break;

                                    case 27: // HH
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 7, sDist, lPan);
                                        break;

                                    case 28: // Troll
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 48, sDist, lPan);
                                        break;

                                    case 29: // Ogre
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 53, sDist, lPan);
                                        break;

                                    case 30: // Liche
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 57, sDist, lPan);
                                        break;

                                    case 31: // DD
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 61, sDist, lPan);
                                        break;

                                    case 32: // Uni
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 65, sDist, lPan);
                                        break;

                                    case 33: // WW
                                        if (m_pData[dX][dY].m_cOwnerFrame == 2)
                                            m_pGame->PlaySound('M', 69, sDist, lPan);
                                        break;
                                    case 55://Rabbit
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 79, sDist, lPan);
                                        break;

                                    case 56://Cat
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 80, sDist, lPan);
                                        break;

                                    case 57://Giant-Frog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 81, sDist, lPan);
                                        break;

                                    case 58://Mountain Giant
                                    case 77: // MasterMageOrc
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 89, sDist, lPan);
                                        break;

                                    case 59://Ettin
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 93, sDist, lPan);
                                        break;

                                    case 60://Cannibal Plant
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 97, sDist, lPan);
                                        break;

                                    case 61://Rudolph
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 69, sDist, lPan);
                                        break;
                                    case 62://DireBoar
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 78, sDist, lPan);
                                        break;

                                    case 74: //GiantCrayFish
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 101, sDist, lPan);
                                        break;

                                    case 78: // Minotaur
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 101, sDist, lPan);
                                        break;

                                    case 80: // Tentocle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 108, sDist, lPan);
                                        break;

                                    case 81: // Abaddon
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 138, sDist, lPan);
                                        break;

                                    case 72: // ClawTurtle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 112, sDist, lPan);
                                        break;

                                    case 71: // Centaurus
                                    case 82: // Sorceress
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 116, sDist, lPan);
                                        break;

                                    case 76: // GiantTree
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 120, sDist, lPan);
                                        break;

                                    case 75: // GiantLizard
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 124, sDist, lPan);
                                        break;

                                    case 70: // Balrog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 128, sDist, lPan);
                                        break;

                                    case 79: // Nizie
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 132, sDist, lPan);
                                        break;

                                    default:
                                        break;
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTMAGIC)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                {
                                    m_pGame->PlaySound('C', 16, sDist, lPan);
                                    if (((m_pData[dX][dY].m_sAppr4 & 0x000F) != 0) && ((m_pData[dX][dY].m_iStatus & 0x10) == 0))
                                    {
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 20 - 10), (dY) * 32 - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                        m_pGame->bAddNewEffect(54, (dX) * 32 + (rand() % 20 - 10), (dY) * 32 - (rand() % 50) - 5, 0, 0, -(rand() % 8), 0);
                                    }
                                    if (m_pGame->bHasHeroSet(m_pData[dX][dY].m_sAppr3, m_pData[dX][dY].m_sAppr4, m_pData[dX][dY].m_sOwnerType) == 2) // Mage hero set
                                    {
                                        m_pGame->bAddNewEffect(242, dX, dY
                                            , dX, dY, 0, 1);
                                    }
                                    if (m_pData[dX][dY].m_sV1 >= 70)
                                        m_pGame->bAddNewEffect(57, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                    if (m_pData[dX][dY].m_sV1 == 82)
                                    {
                                        m_pGame->bAddNewEffect(244, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                    }
                                }
                                break;
                        }
                    }

                    if (m_pData[dX][dY].m_cOwnerAction == DEF_OBJECTDYING)
                    {
                        switch (m_pData[dX][dY].m_sOwnerType)
                        {
                            case 1:
                            case 2:
                            case 3:
                            case 44: // GHK
                            case 45: // GHKABS
                            case 46: // TK				
                                if (m_pData[dX][dY].m_cOwnerFrame == 6)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 12);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 7)
                                    m_pGame->PlaySound('C', 14, sDist, lPan);
                                break;

                            case 4:
                            case 5:
                            case 6:
                            case 54: // DE	
                                if (m_pData[dX][dY].m_cOwnerFrame == 6)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 12);
                                }
                                if (m_pData[dX][dY].m_cOwnerFrame == 7)
                                    m_pGame->PlaySound('C', 15, sDist, lPan);
                                break;

                            default:
                                if (m_pData[dX][dY].m_cOwnerFrame == 4)
                                {
                                    if (m_pData[dX][dY].m_sV2 == -1)
                                        iSoundIndex = 5;
                                    else if (m_pData[dX][dY].m_sV2 == 0)
                                        iSoundIndex = 5;
                                    else if ((m_pData[dX][dY].m_sV2 >= 1) && (m_pData[dX][dY].m_sV2 <= 19))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 20) && (m_pData[dX][dY].m_sV2 <= 39))
                                        iSoundIndex = 6;
                                    else if ((m_pData[dX][dY].m_sV2 >= 40) && (m_pData[dX][dY].m_sV2 <= 59))
                                        iSoundIndex = 7;
                                    else iSoundIndex = 5;
                                    m_pGame->PlaySound('C', iSoundIndex, sDist, lPan);
                                    m_pGame->bAddNewEffect(1, dX, dY, 0, 0, 0, 12);
                                }

                                switch (m_pData[dX][dY].m_sOwnerType)
                                {
                                    case 53: // BB
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 39, sDist, lPan);
                                        break;

                                    case 10: // Slime
                                    case 34: // Dummy
                                    case 35: // EnergyBall
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 4, sDist, lPan);
                                        break;

                                    case 11: // Skell
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 16, sDist, lPan);
                                        break;

                                    case 12: // Stone-Golem
                                    case 47: // BG
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 36, sDist, lPan);
                                        break;

                                    case 65: // IceGolem
                                        //							if (m_pData[dX][dY].m_cOwnerFrame == 1) 
                                        //								m_pGame->bAddNewEffect(74, (m_sPivotX+dX)*32, (m_sPivotY+dY)*32, 0, 0, 0 );
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                        {
                                            m_pGame->bAddNewEffect(74, (dX) * 32, (dY) * 32, 0, 0, 0);
                                            m_pGame->PlaySound('M', 36, sDist, lPan);
                                        }
                                        break;

                                    case 13: // Cyclops
                                    case 49: // HC
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 44, sDist, lPan);
                                        break;

                                    case 14: // Orc
                                    case 48: // SK
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 12, sDist, lPan);
                                        break;

                                    case 16: // Ant
                                    case 43: // LWB
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 32, sDist, lPan);
                                        break;

                                    case 17: // Scorp
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 24, sDist, lPan);
                                        break;

                                    case 18: // Zombie
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 20, sDist, lPan);
                                        break;

                                    case 22: // Snake
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 28, sDist, lPan);
                                        break;

                                    case 23: // Clay-Golem
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 40, sDist, lPan);
                                        break;

                                    case 27: // HH
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 8, sDist, lPan);
                                        break;

                                    case 28: // Troll
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 49, sDist, lPan);
                                        break;

                                    case 29: // Ogre
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 54, sDist, lPan);
                                        break;

                                    case 30: // Liche
                                    case 50: // TW
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 58, sDist, lPan);
                                        break;

                                    case 31: // DD
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 62, sDist, lPan);
                                        break;

                                    case 32: // Uni
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 66, sDist, lPan);
                                        break;

                                    case 33: // WW
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('M', 70, sDist, lPan);
                                        break;

                                    case 36: // AGT
                                    case 37: // CGT
                                    case 38: // MS
                                    case 39: // DT
                                    case 40: // ESG
                                    case 41: // GMG
                                    case 42: // ManaStone						
                                        if (m_pData[dX][dY].m_cOwnerFrame == 3)
                                        {
                                            m_pGame->bAddNewEffect(66, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                        }
                                        break;

                                    case 87: // CrossBowTurret
                                        if (m_pData[dX][dY].m_cOwnerFrame == 3)
                                        {
                                            m_pGame->bAddNewEffect(66, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                        }
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('M', 154, sDist, lPan);
                                        break;

                                    case 89: // CannonTurret
                                        if (m_pData[dX][dY].m_cOwnerFrame == 3)
                                        {
                                            m_pGame->bAddNewEffect(66, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60), 0, 0, -1 * (rand() % 2));
                                        }
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                            m_pGame->PlaySound('M', 156, sDist, lPan);
                                        break;

                                    case 51: // CP
                                        if (m_pData[dX][dY].m_cOwnerFrame == 1)
                                        {
                                            m_pGame->bAddNewEffect(66, (dX) * 32, (dY) * 32 - 30, 0, 0, 0, 0);
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                        }
                                        break;

                                    case 52: // GG
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                        {
                                            m_pGame->PlaySound('M', 44, sDist, lPan);
                                        }
                                        if (m_pData[dX][dY].m_cOwnerFrame == 11)
                                        {
                                            m_pGame->bAddNewEffect(66, (dX) * 32, (dY) * 32 - 30, 0, 0, 0, 0);
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(12, (dX) * 32 + 5 - (rand() % 10), (dY) * 32 + 5 - (rand() % 10) - 30, 0, 0, -1 * (rand() % 2));

                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                            m_pGame->bAddNewEffect(65, (dX) * 32 + 30 - (rand() % 60), (dY) * 32 + 30 - (rand() % 60) - 30, 0, 0, -1 * (rand() % 2));
                                        }
                                        break;

                                    case 55:// Rabbit
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 83, sDist, lPan);
                                        break;

                                    case 56: // Cat
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 84, sDist, lPan);
                                        break;

                                    case 57://Giant-Frog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 85, sDist, lPan);
                                        break;

                                    case 58://Mountain Giant
                                    case 77: // MasterMageOrc
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 90, sDist, lPan);
                                        break;

                                    case 59://Ettin
                                    case 88: // Barbarian
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 94, sDist, lPan);
                                        break;

                                    case 83: // ATK
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 141, sDist, lPan);
                                        break;

                                    case 85: // DSK
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 146, sDist, lPan);
                                        break;

                                    case 61://Rudolph
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 65, sDist, lPan);
                                        break;

                                    case 62://DireBoar
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 94, sDist, lPan);
                                        break;

                                    case 66: // Wyvern
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('E', 7, sDist, lPan);
                                        break;

                                    case 70: // Balrog
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 129, sDist, lPan);
                                        break;

                                    case 71: // Centaur
                                    case 82: // Sorceress
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 129, sDist, lPan);
                                        break;

                                    case 72: // ClawTurtle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 113, sDist, lPan);
                                        break;

                                    case 73: // FireWyvern
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 105, sDist, lPan);
                                        break;


                                    case 60: // Cannibal Plant
                                    case 74: // GiantGrayFish
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 98, sDist, lPan);
                                        break;

                                    case 75: //
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 125, sDist, lPan);
                                        break;

                                    case 76: // 
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 121, sDist, lPan);
                                        break;

                                    case 78: // 
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 103, sDist, lPan);
                                        break;

                                    case 79: // 
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 133, sDist, lPan);
                                        break;

                                    case 80: //Tentocle
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 109, sDist, lPan);
                                        break;

                                    case 81: // Abaddon
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 139, sDist, lPan);
                                        break;

                                    case 84: // MasterElf
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 150, sDist, lPan);
                                        break;

                                    case 86: // HBT
                                        if ((m_pData[dX][dY].m_cOwnerFrame == 1))
                                            m_pGame->PlaySound('M', 152, sDist, lPan);
                                        break;

                                    default:
                                        if (m_pData[dX][dY].m_cOwnerFrame == 5)
                                            m_pGame->PlaySound('C', 15, sDist, lPan);
                                        break;

                                    case 63: // Frost
                                    case 91: // Gate
                                        break;
                                }
                                break;
                        }
                    }
                }
            }
        }
    }

    if ((dwTime - m_dwDOframeTime) > 100) m_dwDOframeTime = dwTime;
}

bool CMapData::bSetItem(short sX, short sY, short sItemSpr, short sItemSprFrame, char cItemColor, bool bDropEffect)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY;
    int sAbsX{}, sAbsY{}, sDist;

    if ((sX < 0) || (sX >= MAPDATASIZEX) ||
        (sY < 0) || (sY >= MAPDATASIZEY))
    {
        return false;
    }

    dX = sX;
    dY = sY;

    m_pData[dX][dY].m_sItemSprite = sItemSpr;
    m_pData[dX][dY].m_sItemSpriteFrame = sItemSprFrame;
    m_pData[dX][dY].m_cItemColor = cItemColor;

    auto result = m_pGame->get_distance_from_player(sX, sY);

    // only care about the X distance
    sDist = result.first;

    if (sItemSpr != 0)
    {
        if (bDropEffect == true)
        {
            m_pGame->PlaySound('E', 11, sDist);

            m_pGame->bAddNewEffect(14, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
            m_pGame->bAddNewEffect(14, (dX) * 32 + (10 - (rand() % 20)), (dY) * 32 + (10 - (rand() % 20)), 0, 0, (rand() % 2), 0);
            m_pGame->bAddNewEffect(14, (dX) * 32 + (10 - (rand() % 20)), (dY) * 32 + (10 - (rand() % 20)), 0, 0, (rand() % 2), 0);
        }
    }

    return true;
}

bool CMapData::bSetDeadOwner(uint16_t wObjectID, short sX, short sY, short sType, char cDir, short sAppr1, short sAppr2, short sAppr3, short sAppr4, int iApprColor, int iStatus, char * pName)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int  dX{}, dY{};
    char pTmpName[12]{};
    bool bEraseFlag = false;

    memset(pTmpName, 0, sizeof(pTmpName));
    if (pName != nullptr) strcpy(pTmpName, pName);

    if ((sX < 0) || (sX >= MAPDATASIZEX) ||
        (sY < 0) || (sY >= MAPDATASIZEY))
    {

        for (dX = 0; dX < MAPDATASIZEX; dX++)
            for (dY = 0; dY < MAPDATASIZEY; dY++)
            {
                if (memcmp(m_pData[dX][dY].m_cDeadOwnerName, pTmpName, 10) == 0)
                {
                    m_pData[dX][dY].m_sDeadOwnerType = 0;
                    memset(m_pData[dX][dY].m_cDeadOwnerName, 0, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
                }
            }

        return false;
    }


    for (dX = sX - 2; dX <= sX + 2; dX++)
        for (dY = sY - 2; dY <= sY + 2; dY++)
        {
            if (dX < 0) break;
            else
                if (dX > MAPDATASIZEX) break;
            if (dY < 0) break;
            else
                if (dY > MAPDATASIZEY) break;

            if (memcmp(m_pData[dX][dY].m_cDeadOwnerName, pTmpName, 10) == 0)
            {
                m_pData[dX][dY].m_sDeadOwnerType = 0;
                memset(m_pData[dX][dY].m_cDeadOwnerName, 0, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
                bEraseFlag = true;
            }
        }
    if (bEraseFlag != true)
    {
        for (dX = 0; dX < MAPDATASIZEX; dX++)
            for (dY = 0; dY < MAPDATASIZEY; dY++)
            {
                if (memcmp(m_pData[dX][dY].m_cDeadOwnerName, pTmpName, 10) == 0)
                {
                    m_pData[dX][dY].m_sDeadOwnerType = 0;
                    memset(m_pData[dX][dY].m_cDeadOwnerName, 0, sizeof(m_pData[dX][dY].m_cDeadOwnerName));
                }
            }
    }

    dX = sX;
    dY = sY;

    m_pData[dX][dY].m_wDeadObjectID = wObjectID;
    m_pData[dX][dY].m_sDeadOwnerType = sType;
    m_pData[dX][dY].m_cDeadDir = cDir;
    m_pData[dX][dY].m_sDeadAppr1 = sAppr1;
    m_pData[dX][dY].m_sDeadAppr2 = sAppr2;
    m_pData[dX][dY].m_sDeadAppr3 = sAppr3;
    m_pData[dX][dY].m_sDeadAppr4 = sAppr4;
    m_pData[dX][dY].m_iDeadApprColor = iApprColor;
    m_pData[dX][dY].m_iDeadStatus = iStatus;
    m_pData[dX][dY].m_cDeadOwnerFrame = -1;
    strcpy(m_pData[dX][dY].m_cDeadOwnerName, pTmpName);

    m_iObjectIDcacheLocX[wObjectID] = -1 * sX; //dX; 
    m_iObjectIDcacheLocY[wObjectID] = -1 * sY; //dY;



    return true;
}

bool CMapData::bSetChatMsgOwner(uint16_t wObjectID, short sX, short sY, int iIndex)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY;

    if ((sX == -10) && (sY == -10)) goto SCMO_FULL_SEARCH;

    if ((sX < 0) || (sX >= MAPDATASIZEX) ||
        (sY < 0) || (sY >= MAPDATASIZEY))
    {
        return false;
    }

    for (dX = sX - 4; dX <= sX + 4; dX++)
        for (dY = sY - 4; dY <= sY + 4; dY++)
        {
            if (dX < 0) break;
            else
                if (dX > MAPDATASIZEX) break;
            if (dY < 0) break;
            else
                if (dY > MAPDATASIZEY) break;

            if (m_pData[dX][dY].m_wObjectID == wObjectID)
            {
                m_pData[dX][dY].m_iChatMsg = iIndex;
                return true;
            }
            if (m_pData[dX][dY].m_wDeadObjectID == wObjectID)
            {
                m_pData[dX][dY].m_iDeadChatMsg = iIndex;
                return true;
            }
        }

    SCMO_FULL_SEARCH:;
    for (dX = 0; dX < MAPDATASIZEX; dX++)
        for (dY = 0; dY < MAPDATASIZEY; dY++)
        {

            if (m_pData[dX][dY].m_wObjectID == wObjectID)
            {
                m_pData[dX][dY].m_iChatMsg = iIndex;
                return true;
            }
            if (m_pData[dX][dY].m_wDeadObjectID == wObjectID)
            {
                m_pData[dX][dY].m_iDeadChatMsg = iIndex;
                return true;
            }
        }

    return false;
}

void CMapData::ClearChatMsg(short sX, short sY)
{
    std::unique_lock<std::mutex> lock(map_mut);
    if (m_pGame->m_pChatMsgList[m_pData[sX][sY].m_iChatMsg] != nullptr)
    {
        delete m_pGame->m_pChatMsgList[m_pData[sX][sY].m_iChatMsg];
        m_pGame->m_pChatMsgList[m_pData[sX][sY].m_iChatMsg] = nullptr;
    }

    m_pData[sX][sY].m_iChatMsg = 0;
}

void CMapData::ClearDeadChatMsg(short sX, short sY)
{
    std::unique_lock<std::mutex> lock(map_mut);
    m_pData[sX][sY].m_iDeadChatMsg = 0;
}

bool __fastcall CMapData::bGetOwner(short sX, short sY, char * pName, short * pOwnerType, int * pOwnerStatus, uint16_t * pObjectID) const
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY;

    if ((sX < 0) || (sX > MAPDATASIZEX) ||
        (sY < 0) || (sY > MAPDATASIZEY))
    {
        memset(pName, 0, sizeof(pName));
        return false;
    }

    dX = sX;
    dY = sY;

    *pOwnerType = m_pData[dX][dY].m_sOwnerType;
    strcpy(pName, m_pData[dX][dY].m_cOwnerName);
    *pOwnerStatus = m_pData[dX][dY].m_iStatus;
    *pObjectID = m_pData[dX][dY].m_wObjectID;

    return true;
}

bool CMapData::bSetDynamicObject(short sX, short sY, uint16_t wID, short sType, bool bIsEvent)
{
    std::unique_lock<std::mutex> lock(map_mut);
    int dX, dY, sPrevType;

    if ((sX < 0) || (sX >= MAPDATASIZEX) ||
        (sY < 0) || (sY >= MAPDATASIZEY))
    {
        return false;
    }

    dX = sX;
    dY = sY;

    sPrevType = m_pData[dX][dY].m_sDynamicObjectType;

    m_pData[dX][dY].m_sDynamicObjectType = sType;
    m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 5;
    m_pData[dX][dY].m_dwDynamicObjectTime = unixtime();

    m_pData[dX][dY].m_cDynamicObjectData1 = 0;
    m_pData[dX][dY].m_cDynamicObjectData2 = 0;
    m_pData[dX][dY].m_cDynamicObjectData3 = 0;
    m_pData[dX][dY].m_cDynamicObjectData4 = 0;

    switch (sType)
    {
        case 0:
            if (sPrevType == DEF_DYNAMICOBJECT_FIRE)
            {
                m_pGame->bAddNewEffect(15, (dX) * 32, (dY) * 32, 0, 0, 0, 0);
                m_pGame->bAddNewEffect(15, (dX) * 32 + (10 - (rand() % 20)), (dY) * 32 + (20 - (rand() % 40)), 0, 0, 0, 0);
                m_pGame->bAddNewEffect(15, (dX) * 32 + (10 - (rand() % 20)), (dY) * 32 + (20 - (rand() % 40)), 0, 0, 0, 0);
                m_pGame->bAddNewEffect(15, (dX) * 32 + (10 - (rand() % 20)), (dY) * 32 + (20 - (rand() % 40)), 0, 0, 0, 0);
            }
            else if ((sPrevType == DEF_DYNAMICOBJECT_PCLOUD_BEGIN) || (sPrevType == DEF_DYNAMICOBJECT_PCLOUD_LOOP))
            {
                m_pData[dX][dY].m_sDynamicObjectType = DEF_DYNAMICOBJECT_PCLOUD_END;
                m_pData[dX][dY].m_cDynamicObjectFrame = 0;
                m_pData[dX][dY].m_dwDynamicObjectTime = unixtime();
            }
            break;

        case DEF_DYNAMICOBJECT_FISH:
            m_pData[dX][dY].m_cDynamicObjectData1 = (rand() % 40) - 20;
            m_pData[dX][dY].m_cDynamicObjectData2 = (rand() % 40) - 20;
            m_pData[dX][dY].m_cDynamicObjectData3 = (rand() % 10) - 5;
            m_pData[dX][dY].m_cDynamicObjectData4 = (rand() % 10) - 5;
            break;

        case DEF_DYNAMICOBJECT_PCLOUD_BEGIN:
            if (bIsEvent == false)
            {
                m_pData[dX][dY].m_sDynamicObjectType = DEF_DYNAMICOBJECT_PCLOUD_LOOP;
                m_pData[dX][dY].m_cDynamicObjectFrame = rand() % 8;
            }
            else m_pData[dX][dY].m_cDynamicObjectFrame = -1 * (rand() % 8);
            break;
    }

    return true;
}

void CMapData::GetOwnerStatusByObjectID(uint16_t wObjectID, char * pOwnerType, char * pDir, short * pAppr1, short * pAppr2, short * pAppr3, short * pAppr4, int * pStatus, int * pColor, char * pName) const
{
    std::unique_lock<std::mutex> lock(map_mut);
    int iX, iY;

    for (iX = 0; iX < MAPDATASIZEX; iX++)
        for (iY = 0; iY < MAPDATASIZEY; iY++)
            if (m_pData[iX][iY].m_wObjectID == wObjectID)
            {

                *pOwnerType = (char)m_pData[iX][iY].m_sOwnerType;
                *pDir = m_pData[iX][iY].m_cDir;
                *pAppr1 = m_pData[iX][iY].m_sAppr1;
                *pAppr2 = m_pData[iX][iY].m_sAppr2;
                *pAppr3 = m_pData[iX][iY].m_sAppr3;
                *pAppr4 = m_pData[iX][iY].m_sAppr4;
                *pStatus = m_pData[iX][iY].m_iStatus;
                *pColor = m_pData[iX][iY].m_iApprColor;
                strcpy(pName, m_pData[iX][iY].m_cOwnerName);
                return;
            }
}
