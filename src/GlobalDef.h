//
// Copyright (c) Sharon Fox (sharon at sharonfox dot dev)
//
// Distributed under the MIT License. (See accompanying file LICENSE)
//

#define DEF_ENGLISHITEM
#define DEF_GIZON
#define DEF_LANGUAGE 4
//#define DEF_TAIWAN	1
//#define DEF_CHINESE	2
//#define DEF_KOREAN	3
//#define DEF_ENGLISH	4 
//#define DEF_JAPANESE	5
//#define DEF_GIZON

#define NAME_WORLDNAME1			"xtreme" 
#define MSG_WORLDNAME1			"Helbreath Xtreme" 

#define NAME_WORLDNAME2			"WS2"
#define MSG_WORLDNAME2			"Helbreath Xtreme Beta"

#define MSG_HOMEPAGE			"http://www.helbreathx.net"
#define MSG_HOMEPAGE2			"http://forum.helbreathx.net"

#ifdef _DEBUG
#define SERVER_IP			"127.0.0.1"
#else
#define SERVER_IP			"127.0.0.1"
#endif
#define SERVER_PORT			8443

#define ENGLISH 0
#define SPANISH 1

#define DS_VISIBLE 0
#define DS_BG 1
#define DS_CS 2
#define DS_WIN 3
#define DS_DYNBG 4
#define DS_DIALOGS 5

#define DEF_SNOW_POSITION_Y		430

#if DEF_LANGUAGE == 1	// Taiwan
#define DEF_UPPERVERSION		2
#define DEF_LOWERVERSION		19
#elif DEF_LANGUAGE == 2	// China
#define DEF_UPPERVERSION		2
#define DEF_LOWERVERSION		19
#elif DEF_LANGUAGE == 3	// Korea
#define DEF_UPPERVERSION		2
#define DEF_LOWERVERSION		20
#elif DEF_LANGUAGE == 4	// English
#define DEF_UPPERVERSION		0
#define DEF_LOWERVERSION		0
#define DEF_PATCHVERSION		1
// Fuck USA : 2.20     International : 2.20
#elif DEF_LANGUAGE == 5	// Japan
#define DEF_UPPERVERSION		2
#define DEF_LOWERVERSION		20
// Terra : 2.185     Inter Japan : 2.20
#endif



//#define DEF_TESTSERVER
//#define DEF_SHOWCURSORPOS
//
