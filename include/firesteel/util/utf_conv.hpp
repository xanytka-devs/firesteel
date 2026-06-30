/*******************************************************************************************

Function set for UTF StrLwr, StrUpr and case insensitive StrCmp and  StrStr

by Jan BergstrÃ¶m, Alphabet AB, 2021 Open source code

2021-10-21

*******************************************************************************************/
//#define UTF32_TEST 1
#include <stdlib.h> 
#include <string.h>

#include <stdint.h>
#define Utf32Char uint32_t		// min 4 bytes (int32_t), uint32_t suggested
#define Utf16Char wchar_t		// min 2 bytes (int16_t), wchar_t suggested 
#define Utf8Char unsigned char	// must be 1 byte, 8 bits, can be char, the UTF consortium specify unsigned 

// Utf 32
size_t StrLenUtf32(const Utf32Char* str);
Utf32Char* StrCpyUtf32(Utf32Char* dest, const Utf32Char* src);
Utf32Char* StrCatUtf32(Utf32Char* dest, const Utf32Char* src);
int StrnCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2, size_t ztCount);
int StrCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2);
Utf32Char* StrToUprUtf32(Utf32Char* pUtf32);
Utf32Char* StrToLwrUtf32(Utf32Char* pUtf32);
int StrnCiCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2, size_t ztCount);
int StrCiCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2);
Utf32Char* StrCiStrUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2);

// Utf 16
size_t StrLenUtf16(const Utf16Char* str);
Utf16Char* StrCpyUtf16(Utf16Char* dest, const Utf16Char* src);
Utf16Char* StrCatUtf16(Utf16Char* dest, const Utf16Char* src);
int StrnCmpUtf16(const Utf16Char* Utf16s1, const Utf16Char* Utf16s2, size_t ztCount);
int StrCmpUtf16(const Utf16Char* Utf16s1, const Utf16Char* Utf16s2);
size_t CharLenUtf16(const Utf16Char* pUtf16);
Utf16Char* ForwardUtf16Chars(const Utf16Char* pUtf16, size_t ztForwardUtf16Chars);
size_t StrLenUtf32AsUtf16(const Utf32Char* pUtf32);
Utf16Char* Utf32ToUtf16(const Utf32Char* pUtf32);
Utf32Char* Utf16ToUtf32(const Utf16Char* pUtf16);
Utf8Char* Utf16ToUtf8(const Utf16Char* pUtf16);
Utf16Char* Utf16StrMakeUprUtf16Str(const Utf16Char* pUtf16);
Utf16Char* Utf16StrMakeLwrUtf16Str(const Utf16Char* pUtf16);
int StrnCiCmpUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2, size_t ztCount);
int StrCiCmpUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2);
Utf16Char* StrCiStrUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2);

// Utf 8
size_t StrLenUtf8(const Utf8Char* str);
int StrnCmpUtf8(const Utf8Char* Utf8s1, const Utf8Char* Utf8s2, size_t ztCount);
int StrCmpUtf8(const Utf8Char* Utf8s1, const Utf8Char* Utf8s2);
size_t CharLenUtf8(const Utf8Char* pUtf8);
Utf8Char* ForwardUtf8Chars(const Utf8Char* pUtf8, size_t ztForwardUtf8Chars);
size_t StrLenUtf32AsUtf8(const Utf32Char* pUtf32);
Utf8Char* Utf32ToUtf8(const Utf32Char* pUtf32);
Utf32Char* Utf8ToUtf32(const Utf8Char* pUtf8);
Utf16Char* Utf8ToUtf16(const Utf8Char* pUtf8);
Utf8Char* Utf8StrMakeUprUtf8Str(const Utf8Char* pUtf8);
Utf8Char* Utf8StrMakeLwrUtf8Str(const Utf8Char* pUtf8);
int StrnCiCmpUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2, size_t ztCount);
int StrCiCmpUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2);
Utf8Char* StrCiStrUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2);

/*******************************************************************************************

UTF32 Upr/Lwr conversions

*******************************************************************************************/
Utf32Char* StrToUprUtf32(Utf32Char* pUtf32)
{
	Utf32Char* p = pUtf32;
	if (pUtf32 && *pUtf32) {
		while (*p) {
			if ((*p >= 0x61) && (*p <= 0x7a)) /* US ASCII */
				(*p) -= 0x20;
			else {
				switch (*p) {
				case 0x00DF:		//	ÃŸ	0xc3 0x9f	LATIN SMALL LETTER SHARP S
					*p = 0x1E9E;	//	áºž	0xe1 0xba 0x9e	LATIN CAPITAL LETTER SHARP S
					break;
				case 0x00E0:		//	Ã 	0xc3 0xa0	LATIN SMALL LETTER A WITH GRAVE
					*p = 0x00C0;	//	Ã€	0xc3 0x80	LATIN CAPITAL LETTER A WITH GRAVE
					break;
				case 0x00E1:		//	Ã¡	0xc3 0xa1	LATIN SMALL LETTER A WITH ACUTE
					*p = 0x00C1;	//	Ã	0xc3 0x81	LATIN CAPITAL LETTER A WITH ACUTE
					break;
				case 0x00E2:		//	Ã¢	0xc3 0xa2	LATIN SMALL LETTER A WITH CIRCUMFLEX
					*p = 0x00C2;	//	Ã‚	0xc3 0x82	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
					break;
				case 0x00E3:		//	Ã£	0xc3 0xa3	LATIN SMALL LETTER A WITH TILDE
					*p = 0x00C3;	//	Ãƒ	0xc3 0x83	LATIN CAPITAL LETTER A WITH TILDE
					break;
				case 0x00E4:		//	Ã¤	0xc3 0xa4	LATIN SMALL LETTER A WITH DIAERESIS
					*p = 0x00C4;	//	Ã„	0xc3 0x84	LATIN CAPITAL LETTER A WITH DIAERESIS
					break;
				case 0x00E5:		//	Ã¥	0xc3 0xa5	LATIN SMALL LETTER A WITH RING ABOVE
					*p = 0x00C5;	//	Ã…	0xc3 0x85	LATIN CAPITAL LETTER A WITH RING ABOVE
					break;
				case 0x00E6:		//	Ã¦	0xc3 0xa6	LATIN SMALL LETTER AE
					*p = 0x00C6;	//	Ã†	0xc3 0x86	LATIN CAPITAL LETTER AE
					break;
				case 0x00E7:		//	Ã§	0xc3 0xa7	LATIN SMALL LETTER C WITH CEDILLA
					*p = 0x00C7;	//	Ã‡	0xc3 0x87	LATIN CAPITAL LETTER C WITH CEDILLA
					break;
				case 0x00E8:		//	Ã¨	0xc3 0xa8	LATIN SMALL LETTER E WITH GRAVE
					*p = 0x00C8;	//	Ãˆ	0xc3 0x88	LATIN CAPITAL LETTER E WITH GRAVE
					break;
				case 0x00E9:		//	Ã©	0xc3 0xa9	LATIN SMALL LETTER E WITH ACUTE
					*p = 0x00C9;	//	Ã‰	0xc3 0x89	LATIN CAPITAL LETTER E WITH ACUTE
					break;
				case 0x00EA:		//	Ãª	0xc3 0xaa	LATIN SMALL LETTER E WITH CIRCUMFLEX
					*p = 0x00CA;	//	ÃŠ	0xc3 0x8a	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
					break;
				case 0x00EB:		//	Ã«	0xc3 0xab	LATIN SMALL LETTER E WITH DIAERESIS
					*p = 0x00CB;	//	Ã‹	0xc3 0x8b	LATIN CAPITAL LETTER E WITH DIAERESIS
					break;
				case 0x00EC:		//	Ã¬	0xc3 0xac	LATIN SMALL LETTER I WITH GRAVE
					*p = 0x00CC;	//	ÃŒ	0xc3 0x8c	LATIN CAPITAL LETTER I WITH GRAVE
					break;
				case 0x00ED:		//	Ã­	0xc3 0xad	LATIN SMALL LETTER I WITH ACUTE
					*p = 0x00CD;	//	Ã	0xc3 0x8d	LATIN CAPITAL LETTER I WITH ACUTE
					break;
				case 0x00EE:		//	Ã®	0xc3 0xae	LATIN SMALL LETTER I WITH CIRCUMFLEX
					*p = 0x00CE;	//	ÃŽ	0xc3 0x8e	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
					break;
				case 0x00EF:		//	Ã¯	0xc3 0xaf	LATIN SMALL LETTER I WITH DIAERESIS
					*p = 0x00CF;	//	Ã	0xc3 0x8f	LATIN CAPITAL LETTER I WITH DIAERESIS
					break;
				case 0x00F0:		//	Ã°	0xc3 0xb0	LATIN SMALL LETTER ETH
					*p = 0x00D0;	//	Ã	0xc3 0x90	LATIN CAPITAL LETTER ETH
					break;
				case 0x00F1:		//	Ã±	0xc3 0xb1	LATIN SMALL LETTER N WITH TILDE
					*p = 0x00D1;	//	Ã‘	0xc3 0x91	LATIN CAPITAL LETTER N WITH TILDE
					break;
				case 0x00F2:		//	Ã²	0xc3 0xb2	LATIN SMALL LETTER O WITH GRAVE
					*p = 0x00D2;	//	Ã’	0xc3 0x92	LATIN CAPITAL LETTER O WITH GRAVE
					break;
				case 0x00F3:		//	Ã³	0xc3 0xb3	LATIN SMALL LETTER O WITH ACUTE
					*p = 0x00D3;	//	Ã“	0xc3 0x93	LATIN CAPITAL LETTER O WITH ACUTE
					break;
				case 0x00F4:		//	Ã´	0xc3 0xb4	LATIN SMALL LETTER O WITH CIRCUMFLEX
					*p = 0x00D4;	//	Ã”	0xc3 0x94	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
					break;
				case 0x00F5:		//	Ãµ	0xc3 0xb5	LATIN SMALL LETTER O WITH TILDE
					*p = 0x00D5;	//	Ã•	0xc3 0x95	LATIN CAPITAL LETTER O WITH TILDE
					break;
				case 0x00F6:		//	Ã¶	0xc3 0xb6	LATIN SMALL LETTER O WITH DIAERESIS
					*p = 0x00D6;	//	Ã–	0xc3 0x96	LATIN CAPITAL LETTER O WITH DIAERESIS
					break;
				case 0x00F8:		//	Ã¸	0xc3 0xb8	LATIN SMALL LETTER O WITH STROKE
					*p = 0x00D8;	//	Ã˜	0xc3 0x98	LATIN CAPITAL LETTER O WITH STROKE
					break;
				case 0x00F9:		//	Ã¹	0xc3 0xb9	LATIN SMALL LETTER U WITH GRAVE
					*p = 0x00D9;	//	Ã™	0xc3 0x99	LATIN CAPITAL LETTER U WITH GRAVE
					break;
				case 0x00FA:		//	Ãº	0xc3 0xba	LATIN SMALL LETTER U WITH ACUTE
					*p = 0x00DA;	//	Ãš	0xc3 0x9a	LATIN CAPITAL LETTER U WITH ACUTE
					break;
				case 0x00FB:		//	Ã»	0xc3 0xbb	LATIN SMALL LETTER U WITH CIRCUMFLEX
					*p = 0x00DB;	//	Ã›	0xc3 0x9b	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
					break;
				case 0x00FC:		//	Ã¼	0xc3 0xbc	LATIN SMALL LETTER U WITH DIAERESIS
					*p = 0x00DC;	//	Ãœ	0xc3 0x9c	LATIN CAPITAL LETTER U WITH DIAERESIS
					break;
				case 0x00FD:		//	Ã½	0xc3 0xbd	LATIN SMALL LETTER Y WITH ACUTE
					*p = 0x00DD;	//	Ã	0xc3 0x9d	LATIN CAPITAL LETTER Y WITH ACUTE
					break;
				case 0x00FE:		//	Ã¾	0xc3 0xbe	LATIN SMALL LETTER THORN
					*p = 0x00DE;	//	Ãž	0xc3 0x9e	LATIN CAPITAL LETTER THORN
					break;
				case 0x00FF:		//	Ã¿	0xc3 0xbf	LATIN SMALL LETTER Y WITH DIAERESIS
					*p = 0x0178;	//	Å¸	0xc5 0xb8	LATIN CAPITAL LETTER Y WITH DIAERESIS
					break;
				case 0x0101:		//	Ä	0xc4 0x81	LATIN SMALL LETTER A WITH MACRON
					*p = 0x0100;	//	Ä€	0xc4 0x80	LATIN CAPITAL LETTER A WITH MACRON
					break;
				case 0x0103:		//	Äƒ	0xc4 0x83	LATIN SMALL LETTER A WITH BREVE
					*p = 0x0102;	//	Ä‚	0xc4 0x82	LATIN CAPITAL LETTER A WITH BREVE
					break;
				case 0x0105:		//	Ä…	0xc4 0x85	LATIN SMALL LETTER A WITH OGONEK
					*p = 0x0104;	//	Ä„	0xc4 0x84	LATIN CAPITAL LETTER A WITH OGONEK
					break;
				case 0x0107:		//	Ä‡	0xc4 0x87	LATIN SMALL LETTER C WITH ACUTE
					*p = 0x0106;	//	Ä†	0xc4 0x86	LATIN CAPITAL LETTER C WITH ACUTE
					break;
				case 0x0109:		//	Ä‰	0xc4 0x89	LATIN SMALL LETTER C WITH CIRCUMFLEX
					*p = 0x0108;	//	Äˆ	0xc4 0x88	LATIN CAPITAL LETTER C WITH CIRCUMFLEX
					break;
				case 0x010B:		//	Ä‹	0xc4 0x8b	LATIN SMALL LETTER C WITH DOT ABOVE
					*p = 0x010A;	//	ÄŠ	0xc4 0x8a	LATIN CAPITAL LETTER C WITH DOT ABOVE
					break;
				case 0x010D:		//	Ä	0xc4 0x8d	LATIN SMALL LETTER C WITH CARON
					*p = 0x010C;	//	ÄŒ	0xc4 0x8c	LATIN CAPITAL LETTER C WITH CARON
					break;
				case 0x010F:		//	Ä	0xc4 0x8f	LATIN SMALL LETTER D WITH CARON
					*p = 0x010E;	//	ÄŽ	0xc4 0x8e	LATIN CAPITAL LETTER D WITH CARON
					break;
				case 0x0111:		//	Ä‘	0xc4 0x91	LATIN SMALL LETTER D WITH STROKE
					*p = 0x0110;	//	Ä	0xc4 0x90	LATIN CAPITAL LETTER D WITH STROKE
					break;
				case 0x0113:		//	Ä“	0xc4 0x93	LATIN SMALL LETTER E WITH MACRON
					*p = 0x0112;	//	Ä’	0xc4 0x92	LATIN CAPITAL LETTER E WITH MACRON
					break;
				case 0x0115:		//	Ä•	0xc4 0x95	LATIN SMALL LETTER E WITH BREVE
					*p = 0x0114;	//	Ä”	0xc4 0x94	LATIN CAPITAL LETTER E WITH BREVE
					break;
				case 0x0117:		//	Ä—	0xc4 0x97	LATIN SMALL LETTER E WITH DOT ABOVE
					*p = 0x0116;	//	Ä–	0xc4 0x96	LATIN CAPITAL LETTER E WITH DOT ABOVE
					break;
				case 0x0119:		//	Ä™	0xc4 0x99	LATIN SMALL LETTER E WITH OGONEK
					*p = 0x0118;	//	Ä˜	0xc4 0x98	LATIN CAPITAL LETTER E WITH OGONEK
					break;
				case 0x011B:		//	Ä›	0xc4 0x9b	LATIN SMALL LETTER E WITH CARON
					*p = 0x011A;	//	Äš	0xc4 0x9a	LATIN CAPITAL LETTER E WITH CARON
					break;
				case 0x011D:		//	Ä	0xc4 0x9d	LATIN SMALL LETTER G WITH CIRCUMFLEX
					*p = 0x011C;	//	Äœ	0xc4 0x9c	LATIN CAPITAL LETTER G WITH CIRCUMFLEX
					break;
				case 0x011F:		//	ÄŸ	0xc4 0x9f	LATIN SMALL LETTER G WITH BREVE
					*p = 0x011E;	//	Äž	0xc4 0x9e	LATIN CAPITAL LETTER G WITH BREVE
					break;
				case 0x0121:		//	Ä¡	0xc4 0xa1	LATIN SMALL LETTER G WITH DOT ABOVE
					*p = 0x0120;	//	Ä 	0xc4 0xa0	LATIN CAPITAL LETTER G WITH DOT ABOVE
					break;
				case 0x0123:		//	Ä£	0xc4 0xa3	LATIN SMALL LETTER G WITH CEDILLA
					*p = 0x0122;	//	Ä¢	0xc4 0xa2	LATIN CAPITAL LETTER G WITH CEDILLA
					break;
				case 0x0125:		//	Ä¥	0xc4 0xa5	LATIN SMALL LETTER H WITH CIRCUMFLEX
					*p = 0x0124;	//	Ä¤	0xc4 0xa4	LATIN CAPITAL LETTER H WITH CIRCUMFLEX
					break;
				case 0x0127:		//	Ä§	0xc4 0xa7	LATIN SMALL LETTER H WITH STROKE
					*p = 0x0126;	//	Ä¦	0xc4 0xa6	LATIN CAPITAL LETTER H WITH STROKE
					break;
				case 0x0129:		//	Ä©	0xc4 0xa9	LATIN SMALL LETTER I WITH TILDE
					*p = 0x0128;	//	Ä¨	0xc4 0xa8	LATIN CAPITAL LETTER I WITH TILDE
					break;
				case 0x012B:		//	Ä«	0xc4 0xab	LATIN SMALL LETTER I WITH MACRON
					*p = 0x012A;	//	Äª	0xc4 0xaa	LATIN CAPITAL LETTER I WITH MACRON
					break;
				case 0x012D:		//	Ä­	0xc4 0xad	LATIN SMALL LETTER I WITH BREVE
					*p = 0x012C;	//	Ä¬	0xc4 0xac	LATIN CAPITAL LETTER I WITH BREVE
					break;
				case 0x012F:		//	Ä¯	0xc4 0xaf	LATIN SMALL LETTER I WITH OGONEK
					*p = 0x012E;	//	Ä®	0xc4 0xae	LATIN CAPITAL LETTER I WITH OGONEK
					break;
				case 0x0131:		//	Ä±	0xc4 0xb1	LATIN SMALL LETTER DOTLESS I
					*p = 0x0049;	//	I	0x49 LATIN CAPITAL LETTER I
					break;
				case 0x0133:		//	Ä³	0xc4 0xb3	LATIN SMALL LETTER LIGATURE IJ
					*p = 0x0132;	//	Ä²	0xc4 0xb2	LATIN CAPITAL LETTER LIGATURE IJ
					break;
				case 0x0135:		//	Äµ	0xc4 0xb5	LATIN SMALL LETTER J WITH CIRCUMFLEX
					*p = 0x0134;	//	Ä´	0xc4 0xb4	LATIN CAPITAL LETTER J WITH CIRCUMFLEX
					break;
				case 0x0137:		//	Ä·	0xc4 0xb7	LATIN SMALL LETTER K WITH CEDILLA
					*p = 0x0136;	//	Ä¶	0xc4 0xb6	LATIN CAPITAL LETTER K WITH CEDILLA
					break;
				case 0x013A:		//	Äº	0xc4 0xba	LATIN SMALL LETTER L WITH ACUTE
					*p = 0x0139;	//	Ä¹	0xc4 0xb9	LATIN CAPITAL LETTER L WITH ACUTE
					break;
				case 0x013C:		//	Ä¼	0xc4 0xbc	LATIN SMALL LETTER L WITH CEDILLA
					*p = 0x013B;	//	Ä»	0xc4 0xbb	LATIN CAPITAL LETTER L WITH CEDILLA
					break;
				case 0x013E:		//	Ä¾	0xc4 0xbe	LATIN SMALL LETTER L WITH CARON
					*p = 0x013D;	//	Ä½	0xc4 0xbd	LATIN CAPITAL LETTER L WITH CARON
					break;
				case 0x0140:		//	Å€	0xc5 0x80	LATIN SMALL LETTER L WITH MIDDLE DOT
					*p = 0x013F;	//	Ä¿	0xc4 0xbf	LATIN CAPITAL LETTER L WITH MIDDLE DOT
					break;
				case 0x0142:		//	Å‚	0xc5 0x82	LATIN SMALL LETTER L WITH STROKE
					*p = 0x0141;	//	Å	0xc5 0x81	LATIN CAPITAL LETTER L WITH STROKE
					break;
				case 0x0144:		//	Å„	0xc5 0x84	LATIN SMALL LETTER N WITH ACUTE
					*p = 0x0143;	//	Åƒ	0xc5 0x83	LATIN CAPITAL LETTER N WITH ACUTE
					break;
				case 0x0146:		//	Å†	0xc5 0x86	LATIN SMALL LETTER N WITH CEDILLA
					*p = 0x0145;	//	Å…	0xc5 0x85	LATIN CAPITAL LETTER N WITH CEDILLA
					break;
				case 0x0148:		//	Åˆ	0xc5 0x88	LATIN SMALL LETTER N WITH CARON
					*p = 0x0147;	//	Å‡	0xc5 0x87	LATIN CAPITAL LETTER N WITH CARON
					break;
				case 0x014B:		//	Å‹	0xc5 0x8b	LATIN SMALL LETTER ENG
					*p = 0x014A;	//	ÅŠ	0xc5 0x8a	LATIN CAPITAL LETTER ENG
					break;
				case 0x014D:		//	Å	0xc5 0x8d	LATIN SMALL LETTER O WITH MACRON
					*p = 0x014C;	//	ÅŒ	0xc5 0x8c	LATIN CAPITAL LETTER O WITH MACRON
					break;
				case 0x014F:		//	Å	0xc5 0x8f	LATIN SMALL LETTER O WITH BREVE
					*p = 0x014E;	//	ÅŽ	0xc5 0x8e	LATIN CAPITAL LETTER O WITH BREVE
					break;
				case 0x0151:		//	Å‘	0xc5 0x91	LATIN SMALL LETTER O WITH DOUBLE ACUTE
					*p = 0x0150;	//	Å	0xc5 0x90	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
					break;
				case 0x0153:		//	Å“	0xc5 0x93	LATIN SMALL LETTER LIGATURE OE
					*p = 0x0152;	//	Å’	0xc5 0x92	LATIN CAPITAL LETTER LIGATURE OE
					break;
				case 0x0155:		//	Å•	0xc5 0x95	LATIN SMALL LETTER R WITH ACUTE
					*p = 0x0154;	//	Å”	0xc5 0x94	LATIN CAPITAL LETTER R WITH ACUTE
					break;
				case 0x0157:		//	Å—	0xc5 0x97	LATIN SMALL LETTER R WITH CEDILLA
					*p = 0x0156;	//	Å–	0xc5 0x96	LATIN CAPITAL LETTER R WITH CEDILLA
					break;
				case 0x0159:		//	Å™	0xc5 0x99	LATIN SMALL LETTER R WITH CARON
					*p = 0x0158;	//	Å˜	0xc5 0x98	LATIN CAPITAL LETTER R WITH CARON
					break;
				case 0x015B:		//	Å›	0xc5 0x9b	LATIN SMALL LETTER S WITH ACUTE
					*p = 0x015A;	//	Åš	0xc5 0x9a	LATIN CAPITAL LETTER S WITH ACUTE
					break;
				case 0x015D:		//	Å	0xc5 0x9d	LATIN SMALL LETTER S WITH CIRCUMFLEX
					*p = 0x015C;	//	Åœ	0xc5 0x9c	LATIN CAPITAL LETTER S WITH CIRCUMFLEX
					break;
				case 0x015F:		//	ÅŸ	0xc5 0x9f	LATIN SMALL LETTER S WITH CEDILLA
					*p = 0x015E;	//	Åž	0xc5 0x9e	LATIN CAPITAL LETTER S WITH CEDILLA
					break;
				case 0x0161:		//	Å¡	0xc5 0xa1	LATIN SMALL LETTER S WITH CARON
					*p = 0x0160;	//	Å 	0xc5 0xa0	LATIN CAPITAL LETTER S WITH CARON
					break;
				case 0x0163:		//	Å£	0xc5 0xa3	LATIN SMALL LETTER T WITH CEDILLA
					*p = 0x0162;	//	Å¢	0xc5 0xa2	LATIN CAPITAL LETTER T WITH CEDILLA
					break;
				case 0x0165:		//	Å¥	0xc5 0xa5	LATIN SMALL LETTER T WITH CARON
					*p = 0x0164;	//	Å¤	0xc5 0xa4	LATIN CAPITAL LETTER T WITH CARON
					break;
				case 0x0167:		//	Å§	0xc5 0xa7	LATIN SMALL LETTER T WITH STROKE
					*p = 0x0166;	//	Å¦	0xc5 0xa6	LATIN CAPITAL LETTER T WITH STROKE
					break;
				case 0x0169:		//	Å©	0xc5 0xa9	LATIN SMALL LETTER U WITH TILDE
					*p = 0x0168;	//	Å¨	0xc5 0xa8	LATIN CAPITAL LETTER U WITH TILDE
					break;
				case 0x016B:		//	Å«	0xc5 0xab	LATIN SMALL LETTER U WITH MACRON
					*p = 0x016A;	//	Åª	0xc5 0xaa	LATIN CAPITAL LETTER U WITH MACRON
					break;
				case 0x016D:		//	Å­	0xc5 0xad	LATIN SMALL LETTER U WITH BREVE
					*p = 0x016C;	//	Å¬	0xc5 0xac	LATIN CAPITAL LETTER U WITH BREVE
					break;
				case 0x016F:		//	Å¯	0xc5 0xaf	LATIN SMALL LETTER U WITH RING ABOVE
					*p = 0x016E;	//	Å®	0xc5 0xae	LATIN CAPITAL LETTER U WITH RING ABOVE
					break;
				case 0x0171:		//	Å±	0xc5 0xb1	LATIN SMALL LETTER U WITH DOUBLE ACUTE
					*p = 0x0170;	//	Å°	0xc5 0xb0	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
					break;
				case 0x0173:		//	Å³	0xc5 0xb3	LATIN SMALL LETTER U WITH OGONEK
					*p = 0x0172;	//	Å²	0xc5 0xb2	LATIN CAPITAL LETTER U WITH OGONEK
					break;
				case 0x0175:		//	Åµ	0xc5 0xb5	LATIN SMALL LETTER W WITH CIRCUMFLEX
					*p = 0x0174;	//	Å´	0xc5 0xb4	LATIN CAPITAL LETTER W WITH CIRCUMFLEX
					break;
				case 0x0177:		//	Å·	0xc5 0xb7	LATIN SMALL LETTER Y WITH CIRCUMFLEX
					*p = 0x0176;	//	Å¶	0xc5 0xb6	LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
					break;
				case 0x017A:		//	Åº	0xc5 0xba	LATIN SMALL LETTER Z WITH ACUTE
					*p = 0x0179;	//	Å¹	0xc5 0xb9	LATIN CAPITAL LETTER Z WITH ACUTE
					break;
				case 0x017C:		//	Å¼	0xc5 0xbc	LATIN SMALL LETTER Z WITH DOT ABOVE
					*p = 0x017B;	//	Å»	0xc5 0xbb	LATIN CAPITAL LETTER Z WITH DOT ABOVE
					break;
				case 0x017E:		//	Å¾	0xc5 0xbe	LATIN SMALL LETTER Z WITH CARON
					*p = 0x017D;	//	Å½	0xc5 0xbd	LATIN CAPITAL LETTER Z WITH CARON
					break;
				case 0x0180:		//	Æ€	0xc6 0x80	LATIN SMALL LETTER B WITH STROKE
					*p = 0x0243;	//	Éƒ	0xc9 0x83	LATIN CAPITAL LETTER B WITH STROKE
					break;
				case 0x0183:		//	Æƒ	0xc6 0x83	LATIN SMALL LETTER B WITH TOPBAR
					*p = 0x0182;	//	Æ‚	0xc6 0x82	LATIN CAPITAL LETTER B WITH TOPBAR
					break;
				case 0x0185:		//	Æ…	0xc6 0x85	LATIN SMALL LETTER TONE SIX
					*p = 0x0184;	//	Æ„	0xc6 0x84	LATIN CAPITAL LETTER TONE SIX
					break;
				case 0x0188:		//	Æˆ	0xc6 0x88	LATIN SMALL LETTER C WITH HOOK
					*p = 0x0187;	//	Æ‡	0xc6 0x87	LATIN CAPITAL LETTER C WITH HOOK
					break;
				case 0x018C:		//	ÆŒ	0xc6 0x8c	LATIN SMALL LETTER D WITH TOPBAR
					*p = 0x018B;	//	Æ‹	0xc6 0x8b	LATIN CAPITAL LETTER D WITH TOPBAR
					break;
				case 0x0192:		//	Æ’	0xc6 0x92	LATIN SMALL LETTER F WITH HOOK
					*p = 0x0191;	//	Æ‘	0xc6 0x91	LATIN CAPITAL LETTER F WITH HOOK
					break;
				case 0x0195:		//	Æ•	0xc6 0x95	LATIN SMALL LETTER HWAIR
					*p = 0x01F6;	//	Ç¶	0xc7 0xb6	LATIN CAPITAL LETTER HWAIR
					break;
				case 0x0199:		//	Æ™	0xc6 0x99	LATIN SMALL LETTER K WITH HOOK
					*p = 0x0198;	//	Æ˜	0xc6 0x98	LATIN CAPITAL LETTER K WITH HOOK
					break;
				case 0x019A:		//	Æš	0xc6 0x9a	LATIN SMALL LETTER L WITH BAR
					*p = 0x023D;	//	È½	0xc8 0xbd	LATIN CAPITAL LETTER L WITH BAR
					break;
				case 0x019E:		//	Æž	0xc6 0x9e	LATIN SMALL LETTER N WITH LONG RIGHT LEG
					*p = 0x0220;	//	È 	0xc8 0xa0	LATIN CAPITAL LETTER N WITH LONG RIGHT LEG
					break;
				case 0x01A1:		//	Æ¡	0xc6 0xa1	LATIN SMALL LETTER O WITH HORN
					*p = 0x01A0;	//	Æ 	0xc6 0xa0	LATIN CAPITAL LETTER O WITH HORN
					break;
				case 0x01A3:		//	Æ£	0xc6 0xa3	LATIN SMALL LETTER OI
					*p = 0x01A2;	//	Æ¢	0xc6 0xa2	LATIN CAPITAL LETTER OI
					break;
				case 0x01A5:		//	Æ¥	0xc6 0xa5	LATIN SMALL LETTER P WITH HOOK
					*p = 0x01A4;	//	Æ¤	0xc6 0xa4	LATIN CAPITAL LETTER P WITH HOOK
					break;
				case 0x01A8:		//	Æ¨	0xc6 0xa8	LATIN SMALL LETTER TONE TWO
					*p = 0x01A7;	//	Æ§	0xc6 0xa7	LATIN CAPITAL LETTER TONE TWO
					break;
				case 0x01AD:		//	Æ­	0xc6 0xad	LATIN SMALL LETTER T WITH HOOK
					*p = 0x01AC;	//	Æ¬	0xc6 0xac	LATIN CAPITAL LETTER T WITH HOOK
					break;
				case 0x01B0:		//	Æ°	0xc6 0xb0	LATIN SMALL LETTER U WITH HORN
					*p = 0x01AF;	//	Æ¯	0xc6 0xaf	LATIN CAPITAL LETTER U WITH HORN
					break;
				case 0x01B4:		//	Æ´	0xc6 0xb4	LATIN SMALL LETTER Y WITH HOOK
					*p = 0x01B3;	//	Æ³	0xc6 0xb3	LATIN CAPITAL LETTER Y WITH HOOK
					break;
				case 0x01B6:		//	Æ¶	0xc6 0xb6	LATIN SMALL LETTER Z WITH STROKE
					*p = 0x01B5;	//	Æµ	0xc6 0xb5	LATIN CAPITAL LETTER Z WITH STROKE
					break;
				case 0x01B9:		//	Æ¹	0xc6 0xb9	LATIN SMALL LETTER EZH REVERSED
					*p = 0x01B8;	//	Æ¸	0xc6 0xb8	LATIN CAPITAL LETTER EZH REVERSED
					break;
				case 0x01BD:		//	Æ½	0xc6 0xbd	LATIN SMALL LETTER TONE FIVE
					*p = 0x01BC;	//	Æ¼	0xc6 0xbc	LATIN CAPITAL LETTER TONE FIVE
					break;
				case 0x01BF:		//	Æ¿	0xc6 0xbf	LATIN SMALL LETTER WYNN
					*p = 0x01F7;	//	Ç·	0xc7 0xb7	LATIN CAPITAL LETTER WYNN
					break;
				case 0x01C5:		//	Ç…	0xc7 0x85	LATIN CAPITAL LETTER D WITH SMALL Z WITH CARON
				case 0x01C6:		//	Ç†	0xc7 0x86	LATIN SMALL LETTER DZ WITH CARON
					*p = 0x01C4;	//	Ç„	0xc7 0x84	LATIN CAPITAL LETTER DZ WITH CARON
					break;
				case 0x01C8:		//	Çˆ	0xc7 0x88	LATIN CAPITAL LETTER L WITH SMALL J
				case 0x01C9:		//	Ç‰	0xc7 0x89	LATIN SMALL LETTER LJ
					*p = 0x01C7;	//	Ç‡	0xc7 0x87	LATIN CAPITAL LETTER LJ
					break;
				case 0x01CB:		//	Ç‹	0xc7 0x8b	LATIN CAPITAL LETTER N WITH SMALL J
				case 0x01CC:		//	ÇŒ	0xc7 0x8c	LATIN SMALL LETTER NJ
					*p = 0x01CA;	//	ÇŠ	0xc7 0x8a	LATIN CAPITAL LETTER NJ
					break;
				case 0x01CE:		//	ÇŽ	0xc7 0x8e	LATIN SMALL LETTER A WITH CARON
					*p = 0x01CD;	//	Ç	0xc7 0x8d	LATIN CAPITAL LETTER A WITH CARON
					break;
				case 0x01D0:		//	Ç	0xc7 0x90	LATIN SMALL LETTER I WITH CARON
					*p = 0x01CF;	//	Ç	0xc7 0x8f	LATIN CAPITAL LETTER I WITH CARON
					break;
				case 0x01D2:		//	Ç’	0xc7 0x92	LATIN SMALL LETTER O WITH CARON
					*p = 0x01D1;	//	Ç‘	0xc7 0x91	LATIN CAPITAL LETTER O WITH CARON
					break;
				case 0x01D4:		//	Ç”	0xc7 0x94	LATIN SMALL LETTER U WITH CARON
					*p = 0x01D3;	//	Ç“	0xc7 0x93	LATIN CAPITAL LETTER U WITH CARON
					break;
				case 0x01D6:		//	Ç–	0xc7 0x96	LATIN SMALL LETTER U WITH DIAERESIS AND MACRON
					*p = 0x01D5;	//	Ç•	0xc7 0x95	LATIN CAPITAL LETTER U WITH DIAERESIS AND MACRON
					break;
				case 0x01D8:		//	Ç˜	0xc7 0x98	LATIN SMALL LETTER U WITH DIAERESIS AND ACUTE
					*p = 0x01D7;	//	Ç—	0xc7 0x97	LATIN CAPITAL LETTER U WITH DIAERESIS AND ACUTE
					break;
				case 0x01DA:		//	Çš	0xc7 0x9a	LATIN SMALL LETTER U WITH DIAERESIS AND CARON
					*p = 0x01D9;	//	Ç™	0xc7 0x99	LATIN CAPITAL LETTER U WITH DIAERESIS AND CARON
					break;
				case 0x01DC:		//	Çœ	0xc7 0x9c	LATIN SMALL LETTER U WITH DIAERESIS AND GRAVE
					*p = 0x01DB;	//	Ç›	0xc7 0x9b	LATIN CAPITAL LETTER U WITH DIAERESIS AND GRAVE
					break;
				case 0x01DD:		//	Ç	0xC7 0x9D	LATIN SMALL LETTER TURNED E (look mid page 297 https://www.unicode.org/versions/Unicode14.0.0/ch07.pdf)
					*p = 0x018E;	//	ÆŽ	0xc6 0x8e	LATIN CAPITAL LETTER REVERSED E
					break;
				case 0x01DF:		//	ÇŸ	0xc7 0x9f	LATIN SMALL LETTER A WITH DIAERESIS AND MACRON
					*p = 0x01DE;	//	Çž	0xc7 0x9e	LATIN CAPITAL LETTER A WITH DIAERESIS AND MACRON
					break;
				case 0x01E1:		//	Ç¡	0xc7 0xa1	LATIN SMALL LETTER A WITH DOT ABOVE AND MACRON
					*p = 0x01E0;	//	Ç 	0xc7 0xa0	LATIN CAPITAL LETTER A WITH DOT ABOVE AND MACRON
					break;
				case 0x01E3:		//	Ç£	0xc7 0xa3	LATIN SMALL LETTER AE WITH MACRON
					*p = 0x01E2;	//	Ç¢	0xc7 0xa2	LATIN CAPITAL LETTER AE WITH MACRON
					break;
				case 0x01E5:		//	Ç¥	0xc7 0xa5	LATIN SMALL LETTER G WITH STROKE
					*p = 0x01E4;	//	Ç¤	0xc7 0xa4	LATIN CAPITAL LETTER G WITH STROKE
					break;
				case 0x01E7:		//	Ç§	0xc7 0xa7	LATIN SMALL LETTER G WITH CARON
					*p = 0x01E6;	//	Ç¦	0xc7 0xa6	LATIN CAPITAL LETTER G WITH CARON
					break;
				case 0x01E9:		//	Ç©	0xc7 0xa9	LATIN SMALL LETTER K WITH CARON
					*p = 0x01E8;	//	Ç¨	0xc7 0xa8	LATIN CAPITAL LETTER K WITH CARON
					break;
				case 0x01EB:		//	Ç«	0xc7 0xab	LATIN SMALL LETTER O WITH OGONEK
					*p = 0x01EA;	//	Çª	0xc7 0xaa	LATIN CAPITAL LETTER O WITH OGONEK
					break;
				case 0x01ED:		//	Ç­	0xc7 0xad	LATIN SMALL LETTER O WITH OGONEK AND MACRON
					*p = 0x01EC;	//	Ç¬	0xc7 0xac	LATIN CAPITAL LETTER O WITH OGONEK AND MACRON
					break;
				case 0x01EF:		//	Ç¯	0xc7 0xaf	LATIN SMALL LETTER EZH WITH CARON
					*p = 0x01EE;	//	Ç®	0xc7 0xae	LATIN CAPITAL LETTER EZH WITH CARON
					break;
				case 0x01F2:		//	Ç²	0xc7 0xb2	LATIN CAPITAL LETTER D WITH SMALL Z
				case 0x01F3:		//	Ç³	0xc7 0xb3	LATIN SMALL LETTER DZ
					*p = 0x01F1;	//	Ç±	0xc7 0xb1	LATIN CAPITAL LETTER DZ
					break;
				case 0x01F5:		//	Çµ	0xc7 0xb5	LATIN SMALL LETTER G WITH ACUTE
					*p = 0x01F4;	//	Ç´	0xc7 0xb4	LATIN CAPITAL LETTER G WITH ACUTE
					break;
				case 0x01F9:		//	Ç¹	0xc7 0xb9	LATIN SMALL LETTER N WITH GRAVE
					*p = 0x01F8;	//	Ç¸	0xc7 0xb8	LATIN CAPITAL LETTER N WITH GRAVE
					break;
				case 0x01FB:		//	Ç»	0xc7 0xbb	LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE
					*p = 0x01FA;	//	Çº	0xc7 0xba	LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE
					break;
				case 0x01FD:		//	Ç½	0xc7 0xbd	LATIN SMALL LETTER AE WITH ACUTE
					*p = 0x01FC;	//	Ç¼	0xc7 0xbc	LATIN CAPITAL LETTER AE WITH ACUTE
					break;
				case 0x01FF:		//	Ç¿	0xc7 0xbf	LATIN SMALL LETTER O WITH STROKE AND ACUTE
					*p = 0x01FE;	//	Ç¾	0xc7 0xbe	LATIN CAPITAL LETTER O WITH STROKE AND ACUTE
					break;
				case 0x0201:		//	È	0xc8 0x81	LATIN SMALL LETTER A WITH DOUBLE GRAVE
					*p = 0x0200;	//	È€	0xc8 0x80	LATIN CAPITAL LETTER A WITH DOUBLE GRAVE
					break;
				case 0x0203:		//	Èƒ	0xc8 0x83	LATIN SMALL LETTER A WITH INVERTED BREVE
					*p = 0x0202;	//	È‚	0xc8 0x82	LATIN CAPITAL LETTER A WITH INVERTED BREVE
					break;
				case 0x0205:		//	È…	0xc8 0x85	LATIN SMALL LETTER E WITH DOUBLE GRAVE
					*p = 0x0204;	//	È„	0xc8 0x84	LATIN CAPITAL LETTER E WITH DOUBLE GRAVE
					break;
				case 0x0207:		//	È‡	0xc8 0x87	LATIN SMALL LETTER E WITH INVERTED BREVE
					*p = 0x0206;	//	È†	0xc8 0x86	LATIN CAPITAL LETTER E WITH INVERTED BREVE
					break;
				case 0x0209:		//	È‰	0xc8 0x89	LATIN SMALL LETTER I WITH DOUBLE GRAVE
					*p = 0x0208;	//	Èˆ	0xc8 0x88	LATIN CAPITAL LETTER I WITH DOUBLE GRAVE
					break;
				case 0x020B:		//	È‹	0xc8 0x8b	LATIN SMALL LETTER I WITH INVERTED BREVE
					*p = 0x020A;	//	ÈŠ	0xc8 0x8a	LATIN CAPITAL LETTER I WITH INVERTED BREVE
					break;
				case 0x020D:		//	È	0xc8 0x8d	LATIN SMALL LETTER O WITH DOUBLE GRAVE
					*p = 0x020C;	//	ÈŒ	0xc8 0x8c	LATIN CAPITAL LETTER O WITH DOUBLE GRAVE
					break;
				case 0x020F:		//	È	0xc8 0x8f	LATIN SMALL LETTER O WITH INVERTED BREVE
					*p = 0x020E;	//	ÈŽ	0xc8 0x8e	LATIN CAPITAL LETTER O WITH INVERTED BREVE
					break;
				case 0x0211:		//	È‘	0xc8 0x91	LATIN SMALL LETTER R WITH DOUBLE GRAVE
					*p = 0x0210;	//	È	0xc8 0x90	LATIN CAPITAL LETTER R WITH DOUBLE GRAVE
					break;
				case 0x0213:		//	È“	0xc8 0x93	LATIN SMALL LETTER R WITH INVERTED BREVE
					*p = 0x0212;	//	È’	0xc8 0x92	LATIN CAPITAL LETTER R WITH INVERTED BREVE
					break;
				case 0x0215:		//	È•	0xc8 0x95	LATIN SMALL LETTER U WITH DOUBLE GRAVE
					*p = 0x0214;	//	È”	0xc8 0x94	LATIN CAPITAL LETTER U WITH DOUBLE GRAVE
					break;
				case 0x0217:		//	È—	0xc8 0x97	LATIN SMALL LETTER U WITH INVERTED BREVE
					*p = 0x0216;	//	È–	0xc8 0x96	LATIN CAPITAL LETTER U WITH INVERTED BREVE
					break;
				case 0x0219:		//	È™	0xc8 0x99	LATIN SMALL LETTER S WITH COMMA BELOW
					*p = 0x0218;	//	È˜	0xc8 0x98	LATIN CAPITAL LETTER S WITH COMMA BELOW
					break;
				case 0x021B:		//	È›	0xc8 0x9b	LATIN SMALL LETTER T WITH COMMA BELOW
					*p = 0x021A;	//	Èš	0xc8 0x9a	LATIN CAPITAL LETTER T WITH COMMA BELOW
					break;
				case 0x021D:		//	È	0xc8 0x9d	LATIN SMALL LETTER YOGH
					*p = 0x021C;	//	Èœ	0xc8 0x9c	LATIN CAPITAL LETTER YOGH
					break;
				case 0x021F:		//	ÈŸ	0xc8 0x9f	LATIN SMALL LETTER H WITH CARON
					*p = 0x021E;	//	Èž	0xc8 0x9e	LATIN CAPITAL LETTER H WITH CARON
					break;
				case 0x0223:		//	È£	0xc8 0xa3	LATIN SMALL LETTER OU
					*p = 0x0222;	//	È¢	0xc8 0xa2	LATIN CAPITAL LETTER OU
					break;
				case 0x0225:		//	È¥	0xc8 0xa5	LATIN SMALL LETTER Z WITH HOOK
					*p = 0x0224;	//	È¤	0xc8 0xa4	LATIN CAPITAL LETTER Z WITH HOOK
					break;
				case 0x0227:		//	È§	0xc8 0xa7	LATIN SMALL LETTER A WITH DOT ABOVE
					*p = 0x0226;	//	È¦	0xc8 0xa6	LATIN CAPITAL LETTER A WITH DOT ABOVE
					break;
				case 0x0229:		//	È©	0xc8 0xa9	LATIN SMALL LETTER E WITH CEDILLA
					*p = 0x0228;	//	È¨	0xc8 0xa8	LATIN CAPITAL LETTER E WITH CEDILLA
					break;
				case 0x022B:		//	È«	0xc8 0xab	LATIN SMALL LETTER O WITH DIAERESIS AND MACRON
					*p = 0x022A;	//	Èª	0xc8 0xaa	LATIN CAPITAL LETTER O WITH DIAERESIS AND MACRON
					break;
				case 0x022D:		//	È­	0xc8 0xad	LATIN SMALL LETTER O WITH TILDE AND MACRON
					*p = 0x022C;	//	È¬	0xc8 0xac	LATIN CAPITAL LETTER O WITH TILDE AND MACRON
					break;
				case 0x022F:		//	È¯	0xc8 0xaf	LATIN SMALL LETTER O WITH DOT ABOVE
					*p = 0x022E;	//	È®	0xc8 0xae	LATIN CAPITAL LETTER O WITH DOT ABOVE
					break;
				case 0x0231:		//	È±	0xc8 0xb1	LATIN SMALL LETTER O WITH DOT ABOVE AND MACRON
					*p = 0x0230;	//	È°	0xc8 0xb0	LATIN CAPITAL LETTER O WITH DOT ABOVE AND MACRON
					break;
				case 0x0233:		//	È³	0xc8 0xb3	LATIN SMALL LETTER Y WITH MACRON
					*p = 0x0232;	//	È²	0xc8 0xb2	LATIN CAPITAL LETTER Y WITH MACRON
					break;
				case 0x023C:		//	È¼	0xc8 0xbc	LATIN SMALL LETTER C WITH STROKE
					*p = 0x023B;	//	È»	0xc8 0xbb	LATIN CAPITAL LETTER C WITH STROKE
					break;
				case 0x023F:		//	È¿	0xc8 0xbf	LATIN SMALL LETTER S WITH SWASH TAIL
					*p = 0x2C7E;	//	â±¾	0xe2 0xb1 0xbe	LATIN CAPITAL LETTER S WITH SWASH TAIL
					break;
				case 0x0240:		//	É€	0xc9 0x80	LATIN SMALL LETTER Z WITH SWASH TAIL
					*p = 0x2C7F;	//	â±¿	0xe2 0xb1 0xbf	LATIN CAPITAL LETTER Z WITH SWASH TAIL
					break;
				case 0x0242:		//	É‚	0xc9 0x82	LATIN SMALL LETTER GLOTTAL STOP
					*p = 0x0241;	//	É	0xc9 0x81	LATIN CAPITAL LETTER GLOTTAL STOP
					break;
				case 0x0247:		//	É‡	0xc9 0x87	LATIN SMALL LETTER E WITH STROKE
					*p = 0x0246;	//	É†	0xc9 0x86	LATIN CAPITAL LETTER E WITH STROKE
					break;
				case 0x0249:		//	É‰	0xc9 0x89	LATIN SMALL LETTER J WITH STROKE
					*p = 0x0248;	//	Éˆ	0xc9 0x88	LATIN CAPITAL LETTER J WITH STROKE
					break;
				case 0x024B:		//	É‹	0xc9 0x8b	LATIN SMALL LETTER SMALL Q WITH HOOK TAIL
					*p = 0x024A;	//	ÉŠ	0xc9 0x8a	LATIN CAPITAL LETTER SMALL Q WITH HOOK TAIL
					break;
				case 0x024D:		//	É	0xc9 0x8d	LATIN SMALL LETTER R WITH STROKE
					*p = 0x024C;	//	ÉŒ	0xc9 0x8c	LATIN CAPITAL LETTER R WITH STROKE
					break;
				case 0x024F:		//	É	0xc9 0x8f	LATIN SMALL LETTER Y WITH STROKE
					*p = 0x024E;	//	ÉŽ	0xc9 0x8e	LATIN CAPITAL LETTER Y WITH STROKE
					break;
				case 0x0250:		//	É	0xc9 0x90	LATIN SMALL LETTER TURNED A
					*p = 0x2C6F;	//	â±¯	0xe2 0xb1 0xaf	LATIN CAPITAL LETTER TURNED A
					break;
				case 0x0251:		//	É‘	0xc9 0x91	LATIN SMALL LETTER ALPHA
					*p = 0x2C6D;	//	â±­	0xe2 0xb1 0xad	LATIN CAPITAL LETTER ALPHA
					break;
				case 0x0252:		//	É’	0xc9 0x92	LATIN SMALL LETTER TURNED ALPHA
					*p = 0x2C70;	//	â±°	0xe2 0xb1 0xb0	LATIN CAPITAL LETTER TURNED ALPHA
					break;
				case 0x0253:		//	É“	0xc9 0x93	LATIN SMALL LETTER B WITH HOOK
					*p = 0x0181;	//	Æ	0xc6 0x81	LATIN CAPITAL LETTER B WITH HOOK
					break;
				case 0x0254:		//	É”	0xc9 0x94	LATIN SMALL LETTER OPEN O
					*p = 0x0186;	//	Æ†	0xc6 0x86	LATIN CAPITAL LETTER OPEN O
					break;
				case 0x0256:		//	É–	0xc9 0x96	LATIN SMALL LETTER AFRICAN D
					*p = 0x0189;	//	Æ‰	0xc6 0x89	LATIN CAPITAL LETTER AFRICAN D
					break;
				case 0x0257:		//	É—	0xc9 0x97	LATIN SMALL LETTER D WITH HOOK
					*p = 0x018A;	//	ÆŠ	0xc6 0x8a	LATIN CAPITAL LETTER D WITH HOOK
					break;
				case 0x0259:		//	É™	0xc9 0x99	LATIN SMALL LETTER SCHWA
					*p = 0x018F;	//	Æ	0xc6 0x8f	LATIN CAPITAL LETTER SCHWA
					break;
				case 0x025B:		//	É›	0xc9 0x9b	LATIN SMALL LETTER OPEN E
					*p = 0x0190;	//	Æ	0xc6 0x90	LATIN CAPITAL LETTER OPEN E
					break;
				case 0x025C:		//	Éœ	0xc9 0x9c	LATIN SMALL LETTER REVERSED OPEN E
					*p = 0xA7AB;	//	êž«	0xea 0x9e 0xab	LATIN CAPITAL LETTER REVERSED OPEN E
					break;
				case 0x0260:		//	É 	0xc9 0xa0	LATIN SMALL LETTER G WITH HOOK
					*p = 0x0193;	//	Æ“	0xc6 0x93	LATIN CAPITAL LETTER G WITH HOOK
					break;
				case 0x0261:		//	É¡	0xc9 0xa1	LATIN SMALL LETTER SCRIPT G
					*p = 0xA7AC;	//	êž¬	0xea 0x9e 0xac	LATIN CAPITAL LETTER SCRIPT G
					break;
				case 0x0263:		//	É£	0xc9 0xa3	LATIN SMALL LETTER GAMMA
					*p = 0x0194;	//	Æ”	0xc6 0x94	LATIN CAPITAL LETTER GAMMA
					break;
				case 0x0265:		//	É¥	0xc9 0xa5	LATIN SMALL LETTER TURNED H
					*p = 0xA78D;	//	êž	0xea 0x9e 0x8d	LATIN CAPITAL LETTER TURNED H
					break;
				case 0x0266:		//	É¦	0xc9 0xa6	LATIN SMALL LETTER H WITH HOOK
					*p = 0xA7AA;	//	êžª	0xea 0x9e 0xaa	LATIN CAPITAL LETTER H WITH HOOK
					break;
				case 0x0268:		//	É¨	0xc9 0xa8	LATIN SMALL LETTER I WITH STROKE
					*p = 0x0197;	//	Æ—	0xc6 0x97	LATIN CAPITAL LETTER I WITH STROKE
					break;
				case 0x0269:		//	É©	0xc9 0xa9	LATIN SMALL LETTER IOTA
					*p = 0x0196;	//	Æ–	0xc6 0x96	LATIN CAPITAL LETTER IOTA
					break;
				case 0x026A:		//	Éª	0xc9 0xaa	LATIN SMALL LETTER SMALL CAPITAL I
					*p = 0xA7AE;	//	êž®	0xea 0x9e 0xae	LATIN CAPITAL LETTER SMALL CAPITAL I
					break;
				case 0x026B:		//	É«	0xc9 0xab	LATIN SMALL LETTER L WITH MIDDLE TILDE
					*p = 0x2C62;	//	â±¢	0xe2 0xb1 0xa2	LATIN CAPITAL LETTER L WITH MIDDLE TILDE
					break;
				case 0x026C:		//	É¬	0xc9 0xac	LATIN SMALL LETTER L WITH BELT
					*p = 0xA7AD;	//	êž­	0xea 0x9e 0xad	LATIN CAPITAL LETTER L WITH BELT
					break;
				case 0x026F:		//	É¯	0xc9 0xaf	LATIN SMALL LETTER TURNED M
					*p = 0x019C;	//	Æœ	0xc6 0x9c	LATIN CAPITAL LETTER TURNED M
					break;
				case 0x0271:		//	É±	0xc9 0xb1	LATIN SMALL LETTER M WITH HOOK
					*p = 0x2C6E;	//	â±®	0xe2 0xb1 0xae	LATIN CAPITAL LETTER M WITH HOOK
					break;
				case 0x0272:		//	É²	0xc9 0xb2	LATIN SMALL LETTER N WITH LEFT HOOK
					*p = 0x019D;	//	Æ	0xc6 0x9d	LATIN CAPITAL LETTER N WITH LEFT HOOK
					break;
				case 0x0275:		//	Éµ	0xc9 0xb5	LATIN SMALL LETTER O WITH MIDDLE TILDE
					*p = 0x019F;	//	ÆŸ	0xc6 0x9f	LATIN CAPITAL LETTER O WITH MIDDLE TILDE
					break;
				case 0x027D:		//	É½	0xc9 0xbd	LATIN SMALL LETTER R WITH TAIL
					*p = 0x2C64;	//	â±¤	0xe2 0xb1 0xa4	LATIN CAPITAL LETTER R WITH TAIL
					break;
				case 0x0282:		//	Ê‚	0xca 0x82	LATIN SMALL LETTER S WITH HOOK
					*p = 0xA7C5;	//	êŸ…	0xea 0x9f 0x85	LATIN CAPITAL LETTER S WITH HOOK
					break;
				case 0x0283:		//	Êƒ	0xca 0x83	LATIN SMALL LETTER ESH
					*p = 0x01A9;	//	Æ©	0xc6 0xa9	LATIN CAPITAL LETTER ESH
					break;
				case 0x0287:		//	Ê‡	0xca 0x87	LATIN SMALL LETTER TURNED T
					*p = 0xA7B1;	//	êž±	0xea 0x9e 0xb1	LATIN CAPITAL LETTER TURNED T
					break;
				case 0x0288:		//	Êˆ	0xca 0x88	LATIN SMALL LETTER T WITH RETROFLEX HOOK
					*p = 0x01AE;	//	Æ®	0xc6 0xae	LATIN CAPITAL LETTER T WITH RETROFLEX HOOK
					break;
				case 0x0289:		//	Ê‰	0xca 0x89	LATIN SMALL LETTER U BAR
					*p = 0x0244;	//	É„	0xc9 0x84	LATIN CAPITAL LETTER U BAR
					break;
				case 0x028A:		//	ÊŠ	0xca 0x8a	LATIN SMALL LETTER UPSILON
					*p = 0x01B1;	//	Æ±	0xc6 0xb1	LATIN CAPITAL LETTER UPSILON
					break;
				case 0x028B:		//	Ê‹	0xca 0x8b	LATIN SMALL LETTER V WITH HOOK
					*p = 0x01B2;	//	Æ²	0xc6 0xb2	LATIN CAPITAL LETTER V WITH HOOK
					break;
				case 0x028C:		//	ÊŒ	0xca 0x8c	LATIN SMALL LETTER TURNED V
					*p = 0x0245;	//	É…	0xc9 0x85	LATIN CAPITAL LETTER TURNED V
					break;
				case 0x0292:		//	Ê’	0xca 0x92	LATIN SMALL LETTER EZH
					*p = 0x01B7;	//	Æ·	0xc6 0xb7	LATIN CAPITAL LETTER EZH
					break;
				case 0x029D:		//	Ê	0xca 0x9d	LATIN SMALL LETTER J WITH CROSSED-TAIL
					*p = 0xA7B2;	//	êž²	0xea 0x9e 0xb2	LATIN CAPITAL LETTER J WITH CROSSED-TAIL
					break;
				case 0x029E:		//	Êž	0xca 0x9e	LATIN SMALL LETTER TURNED K
					*p = 0xA7B0;	//	êž°	0xea 0x9e 0xb0	LATIN CAPITAL LETTER TURNED K
					break;
				case 0x0371:		//	Í±	0xcd 0xb1	GREEK SMALL LETTER HETA
					*p = 0x0370;	//	Í°	0xcd 0xb0	GREEK CAPITAL LETTER HETA
					break;
				case 0x0373:		//	Í³	0xcd 0xb3	GREEK SMALL LETTER ARCHAIC SAMPI
					*p = 0x0372;	//	Í²	0xcd 0xb2	GREEK CAPITAL LETTER ARCHAIC SAMPI
					break;
				case 0x0377:		//	Í·	0xcd 0xb7	GREEK SMALL LETTER PAMPHYLIAN DIGAMMA
					*p = 0x0376;	//	Í¶	0xcd 0xb6	GREEK CAPITAL LETTER PAMPHYLIAN DIGAMMA
					break;
				case 0x037B:		//	Í»	0xcd 0xbb	GREEK SMALL LETTER REVERSED LUNATE SIGMA SYMBOL
					*p = 0x03FD;	//	Ï½	0xcf 0xbd	GREEK CAPITAL LETTER REVERSED LUNATE SIGMA SYMBOL
					break;
				case 0x037C:		//	Í¼	0xcd 0xbc	GREEK SMALL LETTER DOTTED LUNATE SIGMA SYMBOL
					*p = 0x03FE;	//	Ï¾	0xcf 0xbe	GREEK CAPITAL LETTER DOTTED LUNATE SIGMA SYMBOL
					break;
				case 0x037D:		//	Í½	0xcd 0xbd	GREEK SMALL LETTER REVERSED DOTTED LUNATE SIGMA SYMBOL
					*p = 0x03FF;	//	Ï¿	0xcf 0xbf	GREEK CAPITAL LETTER REVERSED DOTTED LUNATE SIGMA SYMBOL
					break;
				case 0x03AC:		//	Î¬	0xce 0xac	GREEK SMALL LETTER ALPHA WITH TONOS
					*p = 0x0386;	//	Î†	0xce 0x86	GREEK CAPITAL LETTER ALPHA WITH TONOS
					break;
				case 0x03AD:		//	Î­	0xce 0xad	GREEK SMALL LETTER EPSILON WITH TONOS
					*p = 0x0388;	//	Îˆ	0xce 0x88	GREEK CAPITAL LETTER EPSILON WITH TONOS
					break;
				case 0x03AE:		//	Î®	0xce 0xae	GREEK SMALL LETTER ETA WITH TONOS
					*p = 0x0389;	//	Î‰	0xce 0x89	GREEK CAPITAL LETTER ETA WITH TONOS
					break;
				case 0x03AF:		//	Î¯	0xce 0xaf	GREEK SMALL LETTER IOTA WITH TONOS
					*p = 0x038A;	//	ÎŠ	0xce 0x8a	GREEK CAPITAL LETTER IOTA WITH TONOS
					break;
				case 0x03B1:		//	Î±	0xce 0xb1	GREEK SMALL LETTER ALPHA
					*p = 0x0391;	//	Î‘	0xce 0x91	GREEK CAPITAL LETTER ALPHA
					break;
				case 0x03B2:		//	Î²	0xce 0xb2	GREEK SMALL LETTER BETA
					*p = 0x0392;	//	Î’	0xce 0x92	GREEK CAPITAL LETTER BETA
					break;
				case 0x03B3:		//	Î³	0xce 0xb3	GREEK SMALL LETTER GAMMA
					*p = 0x0393;	//	Î“	0xce 0x93	GREEK CAPITAL LETTER GAMMA
					break;
				case 0x03B4:		//	Î´	0xce 0xb4	GREEK SMALL LETTER DELTA
					*p = 0x0394;	//	Î”	0xce 0x94	GREEK CAPITAL LETTER DELTA
					break;
				case 0x03B5:		//	Îµ	0xce 0xb5	GREEK SMALL LETTER EPSILON
					*p = 0x0395;	//	Î•	0xce 0x95	GREEK CAPITAL LETTER EPSILON
					break;
				case 0x03B6:		//	Î¶	0xce 0xb6	GREEK SMALL LETTER ZETA
					*p = 0x0396;	//	Î–	0xce 0x96	GREEK CAPITAL LETTER ZETA
					break;
				case 0x03B7:		//	Î·	0xce 0xb7	GREEK SMALL LETTER ETA
					*p = 0x0397;	//	Î—	0xce 0x97	GREEK CAPITAL LETTER ETA
					break;
				case 0x03B8:		//	Î¸	0xce 0xb8	GREEK SMALL LETTER THETA
					*p = 0x0398;	//	Î˜	0xce 0x98	GREEK CAPITAL LETTER THETA
					break;
				case 0x03B9:		//	Î¹	0xce 0xb9	GREEK SMALL LETTER IOTA
					*p = 0x0399;	//	Î™	0xce 0x99	GREEK CAPITAL LETTER IOTA
					break;
				case 0x03BA:		//	Îº	0xce 0xba	GREEK SMALL LETTER KAPPA
					*p = 0x039A;	//	Îš	0xce 0x9a	GREEK CAPITAL LETTER KAPPA
					break;
				case 0x03BB:		//	Î»	0xce 0xbb	GREEK SMALL LETTER LAMDA
					*p = 0x039B;	//	Î›	0xce 0x9b	GREEK CAPITAL LETTER LAMDA
					break;
				case 0x03BC:		//	Î¼	0xce 0xbc	GREEK SMALL LETTER MU
					*p = 0x039C;	//	Îœ	0xce 0x9c	GREEK CAPITAL LETTER MU
					break;
				case 0x03BD:		//	Î½	0xce 0xbd	GREEK SMALL LETTER NU
					*p = 0x039D;	//	Î	0xce 0x9d	GREEK CAPITAL LETTER NU
					break;
				case 0x03BE:		//	Î¾	0xce 0xbe	GREEK SMALL LETTER XI
					*p = 0x039E;	//	Îž	0xce 0x9e	GREEK CAPITAL LETTER XI
					break;
				case 0x03BF:		//	Î¿	0xce 0xbf	GREEK SMALL LETTER OMICRON
					*p = 0x039F;	//	ÎŸ	0xce 0x9f	GREEK CAPITAL LETTER OMICRON
					break;
				case 0x03C0:		//	Ï€	0xcf 0x80	GREEK SMALL LETTER PI
					*p = 0x03A0;	//	Î 	0xce 0xa0	GREEK CAPITAL LETTER PI
					break;
				case 0x03C1:		//	Ï	0xcf 0x81	GREEK SMALL LETTER RHO
					*p = 0x03A1;	//	Î¡	0xce 0xa1	GREEK CAPITAL LETTER RHO
					break;
				case 0x03C2:		//	Ï‚	0xcf 0x82	GREEK SMALL FINAL SIGMA
				case 0x03C3:		//	Ïƒ	0xcf 0x83	GREEK SMALL LETTER SIGMA
					*p = 0x03A3;	//	Î£	0xce 0xa3	GREEK CAPITAL LETTER SIGMA
					break;
				case 0x03C4:		//	Ï„	0xcf 0x84	GREEK SMALL LETTER TAU
					*p = 0x03A4;	//	Î¤	0xce 0xa4	GREEK CAPITAL LETTER TAU
					break;
				case 0x03C5:		//	Ï…	0xcf 0x85	GREEK SMALL LETTER UPSILON
					*p = 0x03A5;	//	Î¥	0xce 0xa5	GREEK CAPITAL LETTER UPSILON
					break;
				case 0x03C6:		//	Ï†	0xcf 0x86	GREEK SMALL LETTER PHI
					*p = 0x03A6;	//	Î¦	0xce 0xa6	GREEK CAPITAL LETTER PHI
					break;
				case 0x03C7:		//	Ï‡	0xcf 0x87	GREEK SMALL LETTER CHI
					*p = 0x03A7;	//	Î§	0xce 0xa7	GREEK CAPITAL LETTER CHI
					break;
				case 0x03C8:		//	Ïˆ	0xcf 0x88	GREEK SMALL LETTER PSI
					*p = 0x03A8;	//	Î¨	0xce 0xa8	GREEK CAPITAL LETTER PSI
					break;
				case 0x03C9:		//	Ï‰	0xcf 0x89	GREEK SMALL LETTER OMEGA
					*p = 0x03A9;	//	Î©	0xce 0xa9	GREEK CAPITAL LETTER OMEGA
					break;
				case 0x03CA:		//	ÏŠ	0xcf 0x8a	GREEK SMALL LETTER IOTA WITH DIALYTIKA
					*p = 0x03AA;	//	Îª	0xce 0xaa	GREEK CAPITAL LETTER IOTA WITH DIALYTIKA
					break;
				case 0x03CB:		//	Ï‹	0xcf 0x8b	GREEK SMALL LETTER UPSILON WITH DIALYTIKA
					*p = 0x03AB;	//	Î«	0xce 0xab	GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA
					break;
				case 0x03CC:		//	ÏŒ	0xcf 0x8c	GREEK SMALL LETTER OMICRON WITH TONOS
					*p = 0x038C;	//	ÎŒ	0xce 0x8c	GREEK CAPITAL LETTER OMICRON WITH TONOS
					break;
				case 0x03CD:		//	Ï	0xcf 0x8d	GREEK SMALL LETTER UPSILON WITH TONOS
					*p = 0x038E;	//	ÎŽ	0xce 0x8e	GREEK CAPITAL LETTER UPSILON WITH TONOS
					break;
				case 0x03CE:		//	ÏŽ	0xcf 0x8e	GREEK SMALL LETTER OMEGA WITH TONOS
					*p = 0x038F;	//	Î	0xce 0x8f	GREEK CAPITAL LETTER OMEGA WITH TONOS
					break;
				case 0x03D1:		//	Ï‘	0xcf 0x91	GREEK SMALL LETTER THETA SYMBOL
					*p = 0x03F4;	//	Ï´	0xcf 0xb4	GREEK CAPITAL LETTER THETA SYMBOL
					break;
				case 0x03D7:		//	Ï—	0xcf 0x97	GREEK SMALL LETTER KAI SYMBOL
					*p = 0x03CF;	//	Ï	0xcf 0x8f	GREEK CAPITAL LETTER KAI SYMBOL
					break;
				case 0x03D9:		//	Ï™	0xcf 0x99	GREEK SMALL LETTER ARCHAIC KOPPA
					*p = 0x03D8;	//	Ï˜	0xcf 0x98	GREEK CAPITAL LETTER ARCHAIC KOPPA
					break;
				case 0x03DB:		//	Ï›	0xcf 0x9b	GREEK SMALL LETTER STIGMA
					*p = 0x03DA;	//	Ïš	0xcf 0x9a	GREEK CAPITAL LETTER STIGMA
					break;
				case 0x03DD:		//	Ï	0xcf 0x9d	GREEK SMALL LETTER DIGAMMA
					*p = 0x03DC;	//	Ïœ	0xcf 0x9c	GREEK CAPITAL LETTER DIGAMMA
					break;
				case 0x03DF:		//	ÏŸ	0xcf 0x9f	GREEK SMALL LETTER KOPPA
					*p = 0x03DE;	//	Ïž	0xcf 0x9e	GREEK CAPITAL LETTER KOPPA
					break;
				case 0x03E1:		//	Ï¡	0xcf 0xa1	GREEK SMALL LETTER SAMPI
					*p = 0x03E0;	//	Ï 	0xcf 0xa0	GREEK CAPITAL LETTER SAMPI
					break;
				case 0x03E3:		//	Ï£	0xcf 0xa3	COPTIC SMALL LETTER SHEI
					*p = 0x03E2;	//	Ï¢	0xcf 0xa2	COPTIC CAPITAL LETTER SHEI
					break;
				case 0x03E5:		//	Ï¥	0xcf 0xa5	COPTIC SMALL LETTER FEI
					*p = 0x03E4;	//	Ï¤	0xcf 0xa4	COPTIC CAPITAL LETTER FEI
					break;
				case 0x03E7:		//	Ï§	0xcf 0xa7	COPTIC SMALL LETTER KHEI
					*p = 0x03E6;	//	Ï¦	0xcf 0xa6	COPTIC CAPITAL LETTER KHEI
					break;
				case 0x03E9:		//	Ï©	0xcf 0xa9	COPTIC SMALL LETTER HORI
					*p = 0x03E8;	//	Ï¨	0xcf 0xa8	COPTIC CAPITAL LETTER HORI
					break;
				case 0x03EB:		//	Ï«	0xcf 0xab	COPTIC SMALL LETTER GANGIA
					*p = 0x03EA;	//	Ïª	0xcf 0xaa	COPTIC CAPITAL LETTER GANGIA
					break;
				case 0x03ED:		//	Ï­	0xcf 0xad	COPTIC SMALL LETTER SHIMA
					*p = 0x03EC;	//	Ï¬	0xcf 0xac	COPTIC CAPITAL LETTER SHIMA
					break;
				case 0x03EF:		//	Ï¯	0xcf 0xaf	COPTIC SMALL LETTER DEI
					*p = 0x03EE;	//	Ï®	0xcf 0xae	COPTIC CAPITAL LETTER DEI
					break;
				case 0x03F2:		//	Ï²	0xcf 0xb2	GREEK SMALL LETTER LUNATE SIGMA SYMBOL
					*p = 0x03F9;	//	Ï¹	0xcf 0xb9	GREEK CAPITAL LETTER LUNATE SIGMA SYMBOL
					break;
				case 0x03F3:		//	Ï³	0xcf 0xb3	GREEK SMALL LETTER YOT
					*p = 0x037F;	//	Í¿	0xcd 0xbf	GREEK CAPITAL LETTER YOT
					break;
				case 0x03F8:		//	Ï¸	0xcf 0xb8	GREEK SMALL LETTER SHO
					*p = 0x03F7;	//	Ï·	0xcf 0xb7	GREEK CAPITAL LETTER SHO
					break;
				case 0x03FB:		//	Ï»	0xcf 0xbb	GREEK SMALL LETTER SAN
					*p = 0x03FA;	//	Ïº	0xcf 0xba	GREEK CAPITAL LETTER SAN
					break;
				case 0x0430:		//	Ð°	0xd0 0xb0	CYRILLIC SMALL LETTER A
					*p = 0x0410;	//	Ð	0xd0 0x90	CYRILLIC CAPITAL LETTER A
					break;
				case 0x0431:		//	Ð±	0xd0 0xb1	CYRILLIC SMALL LETTER BE
					*p = 0x0411;	//	Ð‘	0xd0 0x91	CYRILLIC CAPITAL LETTER BE
					break;
				case 0x0432:		//	Ð²	0xd0 0xb2	CYRILLIC SMALL LETTER VE
					*p = 0x0412;	//	Ð’	0xd0 0x92	CYRILLIC CAPITAL LETTER VE
					break;
				case 0x0433:		//	Ð³	0xd0 0xb3	CYRILLIC SMALL LETTER GHE
					*p = 0x0413;	//	Ð“	0xd0 0x93	CYRILLIC CAPITAL LETTER GHE
					break;
				case 0x0434:		//	Ð´	0xd0 0xb4	CYRILLIC SMALL LETTER DE
					*p = 0x0414;	//	Ð”	0xd0 0x94	CYRILLIC CAPITAL LETTER DE
					break;
				case 0x0435:		//	Ðµ	0xd0 0xb5	CYRILLIC SMALL LETTER IE
					*p = 0x0415;	//	Ð•	0xd0 0x95	CYRILLIC CAPITAL LETTER IE
					break;
				case 0x0436:		//	Ð¶	0xd0 0xb6	CYRILLIC SMALL LETTER ZHE
					*p = 0x0416;	//	Ð–	0xd0 0x96	CYRILLIC CAPITAL LETTER ZHE
					break;
				case 0x0437:		//	Ð·	0xd0 0xb7	CYRILLIC SMALL LETTER ZE
					*p = 0x0417;	//	Ð—	0xd0 0x97	CYRILLIC CAPITAL LETTER ZE
					break;
				case 0x0438:		//	Ð¸	0xd0 0xb8	CYRILLIC SMALL LETTER I
					*p = 0x0418;	//	Ð˜	0xd0 0x98	CYRILLIC CAPITAL LETTER I
					break;
				case 0x0439:		//	Ð¹	0xd0 0xb9	CYRILLIC SMALL LETTER SHORT I
					*p = 0x0419;	//	Ð™	0xd0 0x99	CYRILLIC CAPITAL LETTER SHORT I
					break;
				case 0x043A:		//	Ðº	0xd0 0xba	CYRILLIC SMALL LETTER KA
					*p = 0x041A;	//	Ðš	0xd0 0x9a	CYRILLIC CAPITAL LETTER KA
					break;
				case 0x043B:		//	Ð»	0xd0 0xbb	CYRILLIC SMALL LETTER EL
					*p = 0x041B;	//	Ð›	0xd0 0x9b	CYRILLIC CAPITAL LETTER EL
					break;
				case 0x043C:		//	Ð¼	0xd0 0xbc	CYRILLIC SMALL LETTER EM
					*p = 0x041C;	//	Ðœ	0xd0 0x9c	CYRILLIC CAPITAL LETTER EM
					break;
				case 0x043D:		//	Ð½	0xd0 0xbd	CYRILLIC SMALL LETTER EN
					*p = 0x041D;	//	Ð	0xd0 0x9d	CYRILLIC CAPITAL LETTER EN
					break;
				case 0x043E:		//	Ð¾	0xd0 0xbe	CYRILLIC SMALL LETTER O
					*p = 0x041E;	//	Ðž	0xd0 0x9e	CYRILLIC CAPITAL LETTER O
					break;
				case 0x043F:		//	Ð¿	0xd0 0xbf	CYRILLIC SMALL LETTER PE
					*p = 0x041F;	//	ÐŸ	0xd0 0x9f	CYRILLIC CAPITAL LETTER PE
					break;
				case 0x0440:		//	Ñ€	0xd1 0x80	CYRILLIC SMALL LETTER ER
					*p = 0x0420;	//	Ð 	0xd0 0xa0	CYRILLIC CAPITAL LETTER ER
					break;
				case 0x0441:		//	Ñ	0xd1 0x81	CYRILLIC SMALL LETTER ES
					*p = 0x0421;	//	Ð¡	0xd0 0xa1	CYRILLIC CAPITAL LETTER ES
					break;
				case 0x0442:		//	Ñ‚	0xd1 0x82	CYRILLIC SMALL LETTER TE
					*p = 0x0422;	//	Ð¢	0xd0 0xa2	CYRILLIC CAPITAL LETTER TE
					break;
				case 0x0443:		//	Ñƒ	0xd1 0x83	CYRILLIC SMALL LETTER U
					*p = 0x0423;	//	Ð£	0xd0 0xa3	CYRILLIC CAPITAL LETTER U
					break;
				case 0x0444:		//	Ñ„	0xd1 0x84	CYRILLIC SMALL LETTER EF
					*p = 0x0424;	//	Ð¤	0xd0 0xa4	CYRILLIC CAPITAL LETTER EF
					break;
				case 0x0445:		//	Ñ…	0xd1 0x85	CYRILLIC SMALL LETTER HA
					*p = 0x0425;	//	Ð¥	0xd0 0xa5	CYRILLIC CAPITAL LETTER HA
					break;
				case 0x0446:		//	Ñ†	0xd1 0x86	CYRILLIC SMALL LETTER TSE
					*p = 0x0426;	//	Ð¦	0xd0 0xa6	CYRILLIC CAPITAL LETTER TSE
					break;
				case 0x0447:		//	Ñ‡	0xd1 0x87	CYRILLIC SMALL LETTER CHE
					*p = 0x0427;	//	Ð§	0xd0 0xa7	CYRILLIC CAPITAL LETTER CHE
					break;
				case 0x0448:		//	Ñˆ	0xd1 0x88	CYRILLIC SMALL LETTER SHA
					*p = 0x0428;	//	Ð¨	0xd0 0xa8	CYRILLIC CAPITAL LETTER SHA
					break;
				case 0x0449:		//	Ñ‰	0xd1 0x89	CYRILLIC SMALL LETTER SHCHA
					*p = 0x0429;	//	Ð©	0xd0 0xa9	CYRILLIC CAPITAL LETTER SHCHA
					break;
				case 0x044A:		//	ÑŠ	0xd1 0x8a	CYRILLIC SMALL LETTER HARD SIGN
					*p = 0x042A;	//	Ðª	0xd0 0xaa	CYRILLIC CAPITAL LETTER HARD SIGN
					break;
				case 0x044B:		//	Ñ‹	0xd1 0x8b	CYRILLIC SMALL LETTER YERU
					*p = 0x042B;	//	Ð«	0xd0 0xab	CYRILLIC CAPITAL LETTER YERU
					break;
				case 0x044C:		//	ÑŒ	0xd1 0x8c	CYRILLIC SMALL LETTER SOFT SIGN
					*p = 0x042C;	//	Ð¬	0xd0 0xac	CYRILLIC CAPITAL LETTER SOFT SIGN
					break;
				case 0x044D:		//	Ñ	0xd1 0x8d	CYRILLIC SMALL LETTER E
					*p = 0x042D;	//	Ð­	0xd0 0xad	CYRILLIC CAPITAL LETTER E
					break;
				case 0x044E:		//	ÑŽ	0xd1 0x8e	CYRILLIC SMALL LETTER YU
					*p = 0x042E;	//	Ð®	0xd0 0xae	CYRILLIC CAPITAL LETTER YU
					break;
				case 0x044F:		//	Ñ	0xd1 0x8f	CYRILLIC SMALL LETTER YA
					*p = 0x042F;	//	Ð¯	0xd0 0xaf	CYRILLIC CAPITAL LETTER YA
					break;
				case 0x0450:		//	Ñ	0xd1 0x90	CYRILLIC SMALL LETTER IE WITH GRAVE
					*p = 0x0400;	//	Ð€	0xd0 0x80	CYRILLIC CAPITAL LETTER IE WITH GRAVE
					break;
				case 0x0451:		//	Ñ‘	0xd1 0x91	CYRILLIC SMALL LETTER IO
					*p = 0x0401;	//	Ð	0xd0 0x81	CYRILLIC CAPITAL LETTER IO
					break;
				case 0x0452:		//	Ñ’	0xd1 0x92	CYRILLIC SMALL LETTER DJE
					*p = 0x0402;	//	Ð‚	0xd0 0x82	CYRILLIC CAPITAL LETTER DJE
					break;
				case 0x0453:		//	Ñ“	0xd1 0x93	CYRILLIC SMALL LETTER GJE
					*p = 0x0403;	//	Ðƒ	0xd0 0x83	CYRILLIC CAPITAL LETTER GJE
					break;
				case 0x0454:		//	Ñ”	0xd1 0x94	CYRILLIC SMALL LETTER UKRAINIAN IE
					*p = 0x0404;	//	Ð„	0xd0 0x84	CYRILLIC CAPITAL LETTER UKRAINIAN IE
					break;
				case 0x0455:		//	Ñ•	0xd1 0x95	CYRILLIC SMALL LETTER DZE
					*p = 0x0405;	//	Ð…	0xd0 0x85	CYRILLIC CAPITAL LETTER DZE
					break;
				case 0x0456:		//	Ñ–	0xd1 0x96	CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
					*p = 0x0406;	//	Ð†	0xd0 0x86	CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
					break;
				case 0x0457:		//	Ñ—	0xd1 0x97	CYRILLIC SMALL LETTER YI
					*p = 0x0407;	//	Ð‡	0xd0 0x87	CYRILLIC CAPITAL LETTER YI
					break;
				case 0x0458:		//	Ñ˜	0xd1 0x98	CYRILLIC SMALL LETTER JE
					*p = 0x0408;	//	Ðˆ	0xd0 0x88	CYRILLIC CAPITAL LETTER JE
					break;
				case 0x0459:		//	Ñ™	0xd1 0x99	CYRILLIC SMALL LETTER LJE
					*p = 0x0409;	//	Ð‰	0xd0 0x89	CYRILLIC CAPITAL LETTER LJE
					break;
				case 0x045A:		//	Ñš	0xd1 0x9a	CYRILLIC SMALL LETTER NJE
					*p = 0x040A;	//	ÐŠ	0xd0 0x8a	CYRILLIC CAPITAL LETTER NJE
					break;
				case 0x045B:		//	Ñ›	0xd1 0x9b	CYRILLIC SMALL LETTER TSHE
					*p = 0x040B;	//	Ð‹	0xd0 0x8b	CYRILLIC CAPITAL LETTER TSHE
					break;
				case 0x045C:		//	Ñœ	0xd1 0x9c	CYRILLIC SMALL LETTER KJE
					*p = 0x040C;	//	ÐŒ	0xd0 0x8c	CYRILLIC CAPITAL LETTER KJE
					break;
				case 0x045D:		//	Ñ	0xd1 0x9d	CYRILLIC SMALL LETTER I WITH GRAVE
					*p = 0x040D;	//	Ð	0xd0 0x8d	CYRILLIC CAPITAL LETTER I WITH GRAVE
					break;
				case 0x045E:		//	Ñž	0xd1 0x9e	CYRILLIC SMALL LETTER SHORT U
					*p = 0x040E;	//	ÐŽ	0xd0 0x8e	CYRILLIC CAPITAL LETTER SHORT U
					break;
				case 0x045F:		//	ÑŸ	0xd1 0x9f	CYRILLIC SMALL LETTER DZHE
					*p = 0x040F;	//	Ð	0xd0 0x8f	CYRILLIC CAPITAL LETTER DZHE
					break;
				case 0x0461:		//	Ñ¡	0xd1 0xa1	CYRILLIC SMALL LETTER OMEGA
					*p = 0x0460;	//	Ñ 	0xd1 0xa0	CYRILLIC CAPITAL LETTER OMEGA
					break;
				case 0x0463:		//	Ñ£	0xd1 0xa3	CYRILLIC SMALL LETTER YAT
					*p = 0x0462;	//	Ñ¢	0xd1 0xa2	CYRILLIC CAPITAL LETTER YAT
					break;
				case 0x0465:		//	Ñ¥	0xd1 0xa5	CYRILLIC SMALL LETTER IOTIFIED E
					*p = 0x0464;	//	Ñ¤	0xd1 0xa4	CYRILLIC CAPITAL LETTER IOTIFIED E
					break;
				case 0x0467:		//	Ñ§	0xd1 0xa7	CYRILLIC SMALL LETTER LITTLE YUS
					*p = 0x0466;	//	Ñ¦	0xd1 0xa6	CYRILLIC CAPITAL LETTER LITTLE YUS
					break;
				case 0x0469:		//	Ñ©	0xd1 0xa9	CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS
					*p = 0x0468;	//	Ñ¨	0xd1 0xa8	CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS
					break;
				case 0x046B:		//	Ñ«	0xd1 0xab	CYRILLIC SMALL LETTER BIG YUS
					*p = 0x046A;	//	Ñª	0xd1 0xaa	CYRILLIC CAPITAL LETTER BIG YUS
					break;
				case 0x046D:		//	Ñ­	0xd1 0xad	CYRILLIC SMALL LETTER IOTIFIED BIG YUS
					*p = 0x046C;	//	Ñ¬	0xd1 0xac	CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS
					break;
				case 0x046F:		//	Ñ¯	0xd1 0xaf	CYRILLIC SMALL LETTER KSI
					*p = 0x046E;	//	Ñ®	0xd1 0xae	CYRILLIC CAPITAL LETTER KSI
					break;
				case 0x0471:		//	Ñ±	0xd1 0xb1	CYRILLIC SMALL LETTER PSI
					*p = 0x0470;	//	Ñ°	0xd1 0xb0	CYRILLIC CAPITAL LETTER PSI
					break;
				case 0x0473:		//	Ñ³	0xd1 0xb3	CYRILLIC SMALL LETTER FITA
					*p = 0x0472;	//	Ñ²	0xd1 0xb2	CYRILLIC CAPITAL LETTER FITA
					break;
				case 0x0475:		//	Ñµ	0xd1 0xb5	CYRILLIC SMALL LETTER IZHITSA
					*p = 0x0474;	//	Ñ´	0xd1 0xb4	CYRILLIC CAPITAL LETTER IZHITSA
					break;
				case 0x0477:		//	Ñ·	0xd1 0xb7	CYRILLIC SMALL LETTER IZHITSA WITH DOUBLE GRAVE ACCENT
					*p = 0x0476;	//	Ñ¶	0xd1 0xb6	CYRILLIC CAPITAL LETTER IZHITSA WITH DOUBLE GRAVE ACCENT
					break;
				case 0x0479:		//	Ñ¹	0xd1 0xb9	CYRILLIC SMALL LETTER UK
					*p = 0x0478;	//	Ñ¸	0xd1 0xb8	CYRILLIC CAPITAL LETTER UK
					break;
				case 0x047B:		//	Ñ»	0xd1 0xbb	CYRILLIC SMALL LETTER ROUND OMEGA
					*p = 0x047A;	//	Ñº	0xd1 0xba	CYRILLIC CAPITAL LETTER ROUND OMEGA
					break;
				case 0x047D:		//	Ñ½	0xd1 0xbd	CYRILLIC SMALL LETTER OMEGA WITH TITLO
					*p = 0x047C;	//	Ñ¼	0xd1 0xbc	CYRILLIC CAPITAL LETTER OMEGA WITH TITLO
					break;
				case 0x047F:		//	Ñ¿	0xd1 0xbf	CYRILLIC SMALL LETTER OT
					*p = 0x047E;	//	Ñ¾	0xd1 0xbe	CYRILLIC CAPITAL LETTER OT
					break;
				case 0x0481:		//	Ò	0xd2 0x81	CYRILLIC SMALL LETTER KOPPA
					*p = 0x0480;	//	Ò€	0xd2 0x80	CYRILLIC CAPITAL LETTER KOPPA
					break;
				case 0x048B:		//	Ò‹	0xd2 0x8b	CYRILLIC SMALL LETTER SHORT I WITH TAIL
					*p = 0x048A;	//	ÒŠ	0xd2 0x8a	CYRILLIC CAPITAL LETTER SHORT I WITH TAIL
					break;
				case 0x048D:		//	Ò	0xd2 0x8d	CYRILLIC SMALL LETTER SEMISOFT SIGN
					*p = 0x048C;	//	ÒŒ	0xd2 0x8c	CYRILLIC CAPITAL LETTER SEMISOFT SIGN
					break;
				case 0x048F:		//	Ò	0xd2 0x8f	CYRILLIC SMALL LETTER ER WITH TICK
					*p = 0x048E;	//	ÒŽ	0xd2 0x8e	CYRILLIC CAPITAL LETTER ER WITH TICK
					break;
				case 0x0491:		//	Ò‘	0xd2 0x91	CYRILLIC SMALL LETTER GHE WITH UPTURN
					*p = 0x0490;	//	Ò	0xd2 0x90	CYRILLIC CAPITAL LETTER GHE WITH UPTURN
					break;
				case 0x0493:		//	Ò“	0xd2 0x93	CYRILLIC SMALL LETTER GHE WITH STROKE
					*p = 0x0492;	//	Ò’	0xd2 0x92	CYRILLIC CAPITAL LETTER GHE WITH STROKE
					break;
				case 0x0495:		//	Ò•	0xd2 0x95	CYRILLIC SMALL LETTER GHE WITH MIDDLE HOOK
					*p = 0x0494;	//	Ò”	0xd2 0x94	CYRILLIC CAPITAL LETTER GHE WITH MIDDLE HOOK
					break;
				case 0x0497:		//	Ò—	0xd2 0x97	CYRILLIC SMALL LETTER ZHE WITH DESCENDER
					*p = 0x0496;	//	Ò–	0xd2 0x96	CYRILLIC CAPITAL LETTER ZHE WITH DESCENDER
					break;
				case 0x0499:		//	Ò™	0xd2 0x99	CYRILLIC SMALL LETTER ZE WITH DESCENDER
					*p = 0x0498;	//	Ò˜	0xd2 0x98	CYRILLIC CAPITAL LETTER ZE WITH DESCENDER
					break;
				case 0x049B:		//	Ò›	0xd2 0x9b	CYRILLIC SMALL LETTER KA WITH DESCENDER
					*p = 0x049A;	//	Òš	0xd2 0x9a	CYRILLIC CAPITAL LETTER KA WITH DESCENDER
					break;
				case 0x049D:		//	Ò	0xd2 0x9d	CYRILLIC SMALL LETTER KA WITH VERTICAL STROKE
					*p = 0x049C;	//	Òœ	0xd2 0x9c	CYRILLIC CAPITAL LETTER KA WITH VERTICAL STROKE
					break;
				case 0x049F:		//	ÒŸ	0xd2 0x9f	CYRILLIC SMALL LETTER KA WITH STROKE
					*p = 0x049E;	//	Òž	0xd2 0x9e	CYRILLIC CAPITAL LETTER KA WITH STROKE
					break;
				case 0x04A1:		//	Ò¡	0xd2 0xa1	CYRILLIC SMALL LETTER BASHKIR KA
					*p = 0x04A0;	//	Ò 	0xd2 0xa0	CYRILLIC CAPITAL LETTER BASHKIR KA
					break;
				case 0x04A3:		//	Ò£	0xd2 0xa3	CYRILLIC SMALL LETTER EN WITH DESCENDER
					*p = 0x04A2;	//	Ò¢	0xd2 0xa2	CYRILLIC CAPITAL LETTER EN WITH DESCENDER
					break;
				case 0x04A5:		//	Ò¥	0xd2 0xa5	CYRILLIC SMALL LETTER LIGATURE EN GHE
					*p = 0x04A4;	//	Ò¤	0xd2 0xa4	CYRILLIC CAPITAL LETTER LIGATURE EN GHE
					break;
				case 0x04A7:		//	Ò§	0xd2 0xa7	CYRILLIC SMALL LETTER PE WITH MIDDLE HOOK
					*p = 0x04A6;	//	Ò¦	0xd2 0xa6	CYRILLIC CAPITAL LETTER PE WITH MIDDLE HOOK
					break;
				case 0x04A9:		//	Ò©	0xd2 0xa9	CYRILLIC SMALL LETTER ABKHASIAN HA
					*p = 0x04A8;	//	Ò¨	0xd2 0xa8	CYRILLIC CAPITAL LETTER ABKHASIAN HA
					break;
				case 0x04AB:		//	Ò«	0xd2 0xab	CYRILLIC SMALL LETTER ES WITH DESCENDER
					*p = 0x04AA;	//	Òª	0xd2 0xaa	CYRILLIC CAPITAL LETTER ES WITH DESCENDER
					break;
				case 0x04AD:		//	Ò­	0xd2 0xad	CYRILLIC SMALL LETTER TE WITH DESCENDER
					*p = 0x04AC;	//	Ò¬	0xd2 0xac	CYRILLIC CAPITAL LETTER TE WITH DESCENDER
					break;
				case 0x04AF:		//	Ò¯	0xd2 0xaf	CYRILLIC SMALL LETTER STRAIGHT U
					*p = 0x04AE;	//	Ò®	0xd2 0xae	CYRILLIC CAPITAL LETTER STRAIGHT U
					break;
				case 0x04B1:		//	Ò±	0xd2 0xb1	CYRILLIC SMALL LETTER STRAIGHT U WITH STROKE
					*p = 0x04B0;	//	Ò°	0xd2 0xb0	CYRILLIC CAPITAL LETTER STRAIGHT U WITH STROKE
					break;
				case 0x04B3:		//	Ò³	0xd2 0xb3	CYRILLIC SMALL LETTER HA WITH DESCENDER
					*p = 0x04B2;	//	Ò²	0xd2 0xb2	CYRILLIC CAPITAL LETTER HA WITH DESCENDER
					break;
				case 0x04B5:		//	Òµ	0xd2 0xb5	CYRILLIC SMALL LETTER LIGATURE TE TSE
					*p = 0x04B4;	//	Ò´	0xd2 0xb4	CYRILLIC CAPITAL LETTER LIGATURE TE TSE
					break;
				case 0x04B7:		//	Ò·	0xd2 0xb7	CYRILLIC SMALL LETTER CHE WITH DESCENDER
					*p = 0x04B6;	//	Ò¶	0xd2 0xb6	CYRILLIC CAPITAL LETTER CHE WITH DESCENDER
					break;
				case 0x04B9:		//	Ò¹	0xd2 0xb9	CYRILLIC SMALL LETTER CHE WITH VERTICAL STROKE
					*p = 0x04B8;	//	Ò¸	0xd2 0xb8	CYRILLIC CAPITAL LETTER CHE WITH VERTICAL STROKE
					break;
				case 0x04BB:		//	Ò»	0xd2 0xbb	CYRILLIC SMALL LETTER SHHA
					*p = 0x04BA;	//	Òº	0xd2 0xba	CYRILLIC CAPITAL LETTER SHHA
					break;
				case 0x04BD:		//	Ò½	0xd2 0xbd	CYRILLIC SMALL LETTER ABKHASIAN CHE
					*p = 0x04BC;	//	Ò¼	0xd2 0xbc	CYRILLIC CAPITAL LETTER ABKHASIAN CHE
					break;
				case 0x04BF:		//	Ò¿	0xd2 0xbf	CYRILLIC SMALL LETTER ABKHASIAN CHE WITH DESCENDER
					*p = 0x04BE;	//	Ò¾	0xd2 0xbe	CYRILLIC CAPITAL LETTER ABKHASIAN CHE WITH DESCENDER
					break;
				case 0x04C2:		//	Ó‚	0xd3 0x82	CYRILLIC SMALL LETTER ZHE WITH BREVE
					*p = 0x04C1;	//	Ó	0xd3 0x81	CYRILLIC CAPITAL LETTER ZHE WITH BREVE
					break;
				case 0x04C4:		//	Ó„	0xd3 0x84	CYRILLIC SMALL LETTER KA WITH HOOK
					*p = 0x04C3;	//	Óƒ	0xd3 0x83	CYRILLIC CAPITAL LETTER KA WITH HOOK
					break;
				case 0x04C6:		//	Ó†	0xd3 0x86	CYRILLIC SMALL LETTER EL WITH TAIL
					*p = 0x04C5;	//	Ó…	0xd3 0x85	CYRILLIC CAPITAL LETTER EL WITH TAIL
					break;
				case 0x04C8:		//	Óˆ	0xd3 0x88	CYRILLIC SMALL LETTER EN WITH HOOK
					*p = 0x04C7;	//	Ó‡	0xd3 0x87	CYRILLIC CAPITAL LETTER EN WITH HOOK
					break;
				case 0x04CA:		//	ÓŠ	0xd3 0x8a	CYRILLIC SMALL LETTER EN WITH TAIL
					*p = 0x04C9;	//	Ó‰	0xd3 0x89	CYRILLIC CAPITAL LETTER EN WITH TAIL
					break;
				case 0x04CC:		//	ÓŒ	0xd3 0x8c	CYRILLIC SMALL LETTER KHAKASSIAN CHE
					*p = 0x04CB;	//	Ó‹	0xd3 0x8b	CYRILLIC CAPITAL LETTER KHAKASSIAN CHE
					break;
				case 0x04CE:		//	ÓŽ	0xd3 0x8e	CYRILLIC SMALL LETTER EM WITH TAIL
					*p = 0x04CD;	//	Ó	0xd3 0x8d	CYRILLIC CAPITAL LETTER EM WITH TAIL
					break;
				case 0x04CF:		//	Ó	0xd3 0x8f	CYRILLIC SMALL LETTER PALOCHKA
					*p = 0x04C0;	//	Ó€	0xd3 0x80	CYRILLIC CAPITAL LETTER PALOCHKA
					break;
				case 0x04D1:		//	Ó‘	0xd3 0x91	CYRILLIC SMALL LETTER A WITH BREVE
					*p = 0x04D0;	//	Ó	0xd3 0x90	CYRILLIC CAPITAL LETTER A WITH BREVE
					break;
				case 0x04D3:		//	Ó“	0xd3 0x93	CYRILLIC SMALL LETTER A WITH DIAERESIS
					*p = 0x04D2;	//	Ó’	0xd3 0x92	CYRILLIC CAPITAL LETTER A WITH DIAERESIS
					break;
				case 0x04D5:		//	Ó•	0xd3 0x95	CYRILLIC SMALL LETTER LIGATURE A IE
					*p = 0x04D4;	//	Ó”	0xd3 0x94	CYRILLIC CAPITAL LETTER LIGATURE A IE
					break;
				case 0x04D7:		//	Ó—	0xd3 0x97	CYRILLIC SMALL LETTER IE WITH BREVE
					*p = 0x04D6;	//	Ó–	0xd3 0x96	CYRILLIC CAPITAL LETTER IE WITH BREVE
					break;
				case 0x04D9:		//	Ó™	0xd3 0x99	CYRILLIC SMALL LETTER SCHWA
					*p = 0x04D8;	//	Ó˜	0xd3 0x98	CYRILLIC CAPITAL LETTER SCHWA
					break;
				case 0x04DB:		//	Ó›	0xd3 0x9b	CYRILLIC SMALL LETTER SCHWA WITH DIAERESIS
					*p = 0x04DA;	//	Óš	0xd3 0x9a	CYRILLIC CAPITAL LETTER SCHWA WITH DIAERESIS
					break;
				case 0x04DD:		//	Ó	0xd3 0x9d	CYRILLIC SMALL LETTER ZHE WITH DIAERESIS
					*p = 0x04DC;	//	Óœ	0xd3 0x9c	CYRILLIC CAPITAL LETTER ZHE WITH DIAERESIS
					break;
				case 0x04DF:		//	ÓŸ	0xd3 0x9f	CYRILLIC SMALL LETTER ZE WITH DIAERESIS
					*p = 0x04DE;	//	Óž	0xd3 0x9e	CYRILLIC CAPITAL LETTER ZE WITH DIAERESIS
					break;
				case 0x04E1:		//	Ó¡	0xd3 0xa1	CYRILLIC SMALL LETTER ABKHASIAN DZE
					*p = 0x04E0;	//	Ó 	0xd3 0xa0	CYRILLIC CAPITAL LETTER ABKHASIAN DZE
					break;
				case 0x04E3:		//	Ó£	0xd3 0xa3	CYRILLIC SMALL LETTER I WITH MACRON
					*p = 0x04E2;	//	Ó¢	0xd3 0xa2	CYRILLIC CAPITAL LETTER I WITH MACRON
					break;
				case 0x04E5:		//	Ó¥	0xd3 0xa5	CYRILLIC SMALL LETTER I WITH DIAERESIS
					*p = 0x04E4;	//	Ó¤	0xd3 0xa4	CYRILLIC CAPITAL LETTER I WITH DIAERESIS
					break;
				case 0x04E7:		//	Ó§	0xd3 0xa7	CYRILLIC SMALL LETTER O WITH DIAERESIS
					*p = 0x04E6;	//	Ó¦	0xd3 0xa6	CYRILLIC CAPITAL LETTER O WITH DIAERESIS
					break;
				case 0x04E9:		//	Ó©	0xd3 0xa9	CYRILLIC SMALL LETTER BARRED O
					*p = 0x04E8;	//	Ó¨	0xd3 0xa8	CYRILLIC CAPITAL LETTER BARRED O
					break;
				case 0x04EB:		//	Ó«	0xd3 0xab	CYRILLIC SMALL LETTER BARRED O WITH DIAERESIS
					*p = 0x04EA;	//	Óª	0xd3 0xaa	CYRILLIC CAPITAL LETTER BARRED O WITH DIAERESIS
					break;
				case 0x04ED:		//	Ó­	0xd3 0xad	CYRILLIC SMALL LETTER E WITH DIAERESIS
					*p = 0x04EC;	//	Ó¬	0xd3 0xac	CYRILLIC CAPITAL LETTER E WITH DIAERESIS
					break;
				case 0x04EF:		//	Ó¯	0xd3 0xaf	CYRILLIC SMALL LETTER U WITH MACRON
					*p = 0x04EE;	//	Ó®	0xd3 0xae	CYRILLIC CAPITAL LETTER U WITH MACRON
					break;
				case 0x04F1:		//	Ó±	0xd3 0xb1	CYRILLIC SMALL LETTER U WITH DIAERESIS
					*p = 0x04F0;	//	Ó°	0xd3 0xb0	CYRILLIC CAPITAL LETTER U WITH DIAERESIS
					break;
				case 0x04F3:		//	Ó³	0xd3 0xb3	CYRILLIC SMALL LETTER U WITH DOUBLE ACUTE
					*p = 0x04F2;	//	Ó²	0xd3 0xb2	CYRILLIC CAPITAL LETTER U WITH DOUBLE ACUTE
					break;
				case 0x04F5:		//	Óµ	0xd3 0xb5	CYRILLIC SMALL LETTER CHE WITH DIAERESIS
					*p = 0x04F4;	//	Ó´	0xd3 0xb4	CYRILLIC CAPITAL LETTER CHE WITH DIAERESIS
					break;
				case 0x04F7:		//	Ó·	0xd3 0xb7	CYRILLIC SMALL LETTER GHE WITH DESCENDER
					*p = 0x04F6;	//	Ó¶	0xd3 0xb6	CYRILLIC CAPITAL LETTER GHE WITH DESCENDER
					break;
				case 0x04F9:		//	Ó¹	0xd3 0xb9	CYRILLIC SMALL LETTER YERU WITH DIAERESIS
					*p = 0x04F8;	//	Ó¸	0xd3 0xb8	CYRILLIC CAPITAL LETTER YERU WITH DIAERESIS
					break;
				case 0x04FB:		//	Ó»	0xd3 0xbb	CYRILLIC SMALL LETTER GHE WITH STROKE AND HOOK
					*p = 0x04FA;	//	Óº	0xd3 0xba	CYRILLIC CAPITAL LETTER GHE WITH STROKE AND HOOK
					break;
				case 0x04FD:		//	Ó½	0xd3 0xbd	CYRILLIC SMALL LETTER HA WITH HOOK
					*p = 0x04FC;	//	Ó¼	0xd3 0xbc	CYRILLIC CAPITAL LETTER HA WITH HOOK
					break;
				case 0x04FF:		//	Ó¿	0xd3 0xbf	CYRILLIC SMALL LETTER HA WITH STROKE
					*p = 0x04FE;	//	Ó¾	0xd3 0xbe	CYRILLIC CAPITAL LETTER HA WITH STROKE
					break;
				case 0x0501:		//	Ô	0xd4 0x81	CYRILLIC SMALL LETTER KOMI DE
					*p = 0x0500;	//	Ô€	0xd4 0x80	CYRILLIC CAPITAL LETTER KOMI DE
					break;
				case 0x0503:		//	Ôƒ	0xd4 0x83	CYRILLIC SMALL LETTER KOMI DJE
					*p = 0x0502;	//	Ô‚	0xd4 0x82	CYRILLIC CAPITAL LETTER KOMI DJE
					break;
				case 0x0505:		//	Ô…	0xd4 0x85	CYRILLIC SMALL LETTER KOMI ZJE
					*p = 0x0504;	//	Ô„	0xd4 0x84	CYRILLIC CAPITAL LETTER KOMI ZJE
					break;
				case 0x0507:		//	Ô‡	0xd4 0x87	CYRILLIC SMALL LETTER KOMI DZJE
					*p = 0x0506;	//	Ô†	0xd4 0x86	CYRILLIC CAPITAL LETTER KOMI DZJE
					break;
				case 0x0509:		//	Ô‰	0xd4 0x89	CYRILLIC SMALL LETTER KOMI LJE
					*p = 0x0508;	//	Ôˆ	0xd4 0x88	CYRILLIC CAPITAL LETTER KOMI LJE
					break;
				case 0x050B:		//	Ô‹	0xd4 0x8b	CYRILLIC SMALL LETTER KOMI NJE
					*p = 0x050A;	//	ÔŠ	0xd4 0x8a	CYRILLIC CAPITAL LETTER KOMI NJE
					break;
				case 0x050D:		//	Ô	0xd4 0x8d	CYRILLIC SMALL LETTER KOMI SJE
					*p = 0x050C;	//	ÔŒ	0xd4 0x8c	CYRILLIC CAPITAL LETTER KOMI SJE
					break;
				case 0x050F:		//	Ô	0xd4 0x8f	CYRILLIC SMALL LETTER KOMI TJE
					*p = 0x050E;	//	ÔŽ	0xd4 0x8e	CYRILLIC CAPITAL LETTER KOMI TJE
					break;
				case 0x0511:		//	Ô‘	0xd4 0x91	CYRILLIC SMALL LETTER REVERSED ZE
					*p = 0x0510;	//	Ô	0xd4 0x90	CYRILLIC CAPITAL LETTER REVERSED ZE
					break;
				case 0x0513:		//	Ô“	0xd4 0x93	CYRILLIC SMALL LETTER EL WITH HOOK
					*p = 0x0512;	//	Ô’	0xd4 0x92	CYRILLIC CAPITAL LETTER EL WITH HOOK
					break;
				case 0x0515:		//	Ô•	0xd4 0x95	CYRILLIC SMALL LETTER LHA
					*p = 0x0514;	//	Ô”	0xd4 0x94	CYRILLIC CAPITAL LETTER LHA
					break;
				case 0x0517:		//	Ô—	0xd4 0x97	CYRILLIC SMALL LETTER RHA
					*p = 0x0516;	//	Ô–	0xd4 0x96	CYRILLIC CAPITAL LETTER RHA
					break;
				case 0x0519:		//	Ô™	0xd4 0x99	CYRILLIC SMALL LETTER YAE
					*p = 0x0518;	//	Ô˜	0xd4 0x98	CYRILLIC CAPITAL LETTER YAE
					break;
				case 0x051B:		//	Ô›	0xd4 0x9b	CYRILLIC SMALL LETTER QA
					*p = 0x051A;	//	Ôš	0xd4 0x9a	CYRILLIC CAPITAL LETTER QA
					break;
				case 0x051D:		//	Ô	0xd4 0x9d	CYRILLIC SMALL LETTER WE
					*p = 0x051C;	//	Ôœ	0xd4 0x9c	CYRILLIC CAPITAL LETTER WE
					break;
				case 0x051F:		//	ÔŸ	0xd4 0x9f	CYRILLIC SMALL LETTER ALEUT KA
					*p = 0x051E;	//	Ôž	0xd4 0x9e	CYRILLIC CAPITAL LETTER ALEUT KA
					break;
				case 0x0521:		//	Ô¡	0xd4 0xa1	CYRILLIC SMALL LETTER EL WITH MIDDLE HOOK
					*p = 0x0520;	//	Ô 	0xd4 0xa0	CYRILLIC CAPITAL LETTER EL WITH MIDDLE HOOK
					break;
				case 0x0523:		//	Ô£	0xd4 0xa3	CYRILLIC SMALL LETTER EN WITH MIDDLE HOOK
					*p = 0x0522;	//	Ô¢	0xd4 0xa2	CYRILLIC CAPITAL LETTER EN WITH MIDDLE HOOK
					break;
				case 0x0525:		//	Ô¥	0xd4 0xa5	CYRILLIC SMALL LETTER PE WITH DESCENDER
					*p = 0x0524;	//	Ô¤	0xd4 0xa4	CYRILLIC CAPITAL LETTER PE WITH DESCENDER
					break;
				case 0x0527:		//	Ô§	0xd4 0xa7	CYRILLIC SMALL LETTER SHHA WITH DESCENDER
					*p = 0x0526;	//	Ô¦	0xd4 0xa6	CYRILLIC CAPITAL LETTER SHHA WITH DESCENDER
					break;
				case 0x0529:		//	Ô©	0xd4 0xa9	CYRILLIC SMALL LETTER EN WITH LEFT HOOK
					*p = 0x0528;	//	Ô¨	0xd4 0xa8	CYRILLIC CAPITAL LETTER EN WITH LEFT HOOK
					break;
				case 0x052B:		//	Ô«	0xd4 0xab	CYRILLIC SMALL LETTER DZZHE
					*p = 0x052A;	//	Ôª	0xd4 0xaa	CYRILLIC CAPITAL LETTER DZZHE
					break;
				case 0x052D:		//	Ô­	0xd4 0xad	CYRILLIC SMALL LETTER DCHE
					*p = 0x052C;	//	Ô¬	0xd4 0xac	CYRILLIC CAPITAL LETTER DCHE
					break;
				case 0x052F:		//	Ô¯	0xd4 0xaf	CYRILLIC SMALL LETTER EL WITH DESCENDER
					*p = 0x052E;	//	Ô®	0xd4 0xae	CYRILLIC CAPITAL LETTER EL WITH DESCENDER
					break;
				case 0x0561:		//	Õ¡	0xd5 0xa1	ARMENIAN SMALL LETTER AYB
					*p = 0x0531;	//	Ô±	0xd4 0xb1	ARMENIAN CAPITAL LETTER AYB
					break;
				case 0x0562:		//	Õ¢	0xd5 0xa2	ARMENIAN SMALL LETTER BEN
					*p = 0x0532;	//	Ô²	0xd4 0xb2	ARMENIAN CAPITAL LETTER BEN
					break;
				case 0x0563:		//	Õ£	0xd5 0xa3	ARMENIAN SMALL LETTER GIM
					*p = 0x0533;	//	Ô³	0xd4 0xb3	ARMENIAN CAPITAL LETTER GIM
					break;
				case 0x0564:		//	Õ¤	0xd5 0xa4	ARMENIAN SMALL LETTER DA
					*p = 0x0534;	//	Ô´	0xd4 0xb4	ARMENIAN CAPITAL LETTER DA
					break;
				case 0x0565:		//	Õ¥	0xd5 0xa5	ARMENIAN SMALL LETTER ECH
					*p = 0x0535;	//	Ôµ	0xd4 0xb5	ARMENIAN CAPITAL LETTER ECH
					break;
				case 0x0566:		//	Õ¦	0xd5 0xa6	ARMENIAN SMALL LETTER ZA
					*p = 0x0536;	//	Ô¶	0xd4 0xb6	ARMENIAN CAPITAL LETTER ZA
					break;
				case 0x0567:		//	Õ§	0xd5 0xa7	ARMENIAN SMALL LETTER EH
					*p = 0x0537;	//	Ô·	0xd4 0xb7	ARMENIAN CAPITAL LETTER EH
					break;
				case 0x0568:		//	Õ¨	0xd5 0xa8	ARMENIAN SMALL LETTER ET
					*p = 0x0538;	//	Ô¸	0xd4 0xb8	ARMENIAN CAPITAL LETTER ET
					break;
				case 0x0569:		//	Õ©	0xd5 0xa9	ARMENIAN SMALL LETTER TO
					*p = 0x0539;	//	Ô¹	0xd4 0xb9	ARMENIAN CAPITAL LETTER TO
					break;
				case 0x056A:		//	Õª	0xd5 0xaa	ARMENIAN SMALL LETTER ZHE
					*p = 0x053A;	//	Ôº	0xd4 0xba	ARMENIAN CAPITAL LETTER ZHE
					break;
				case 0x056B:		//	Õ«	0xd5 0xab	ARMENIAN SMALL LETTER INI
					*p = 0x053B;	//	Ô»	0xd4 0xbb	ARMENIAN CAPITAL LETTER INI
					break;
				case 0x056C:		//	Õ¬	0xd5 0xac	ARMENIAN SMALL LETTER LIWN
					*p = 0x053C;	//	Ô¼	0xd4 0xbc	ARMENIAN CAPITAL LETTER LIWN
					break;
				case 0x056D:		//	Õ­	0xd5 0xad	ARMENIAN SMALL LETTER XEH
					*p = 0x053D;	//	Ô½	0xd4 0xbd	ARMENIAN CAPITAL LETTER XEH
					break;
				case 0x056E:		//	Õ®	0xd5 0xae	ARMENIAN SMALL LETTER CA
					*p = 0x053E;	//	Ô¾	0xd4 0xbe	ARMENIAN CAPITAL LETTER CA
					break;
				case 0x056F:		//	Õ¯	0xd5 0xaf	ARMENIAN SMALL LETTER KEN
					*p = 0x053F;	//	Ô¿	0xd4 0xbf	ARMENIAN CAPITAL LETTER KEN
					break;
				case 0x0570:		//	Õ°	0xd5 0xb0	ARMENIAN SMALL LETTER HO
					*p = 0x0540;	//	Õ€	0xd5 0x80	ARMENIAN CAPITAL LETTER HO
					break;
				case 0x0571:		//	Õ±	0xd5 0xb1	ARMENIAN SMALL LETTER JA
					*p = 0x0541;	//	Õ	0xd5 0x81	ARMENIAN CAPITAL LETTER JA
					break;
				case 0x0572:		//	Õ²	0xd5 0xb2	ARMENIAN SMALL LETTER GHAD
					*p = 0x0542;	//	Õ‚	0xd5 0x82	ARMENIAN CAPITAL LETTER GHAD
					break;
				case 0x0573:		//	Õ³	0xd5 0xb3	ARMENIAN SMALL LETTER CHEH
					*p = 0x0543;	//	Õƒ	0xd5 0x83	ARMENIAN CAPITAL LETTER CHEH
					break;
				case 0x0574:		//	Õ´	0xd5 0xb4	ARMENIAN SMALL LETTER MEN
					*p = 0x0544;	//	Õ„	0xd5 0x84	ARMENIAN CAPITAL LETTER MEN
					break;
				case 0x0575:		//	Õµ	0xd5 0xb5	ARMENIAN SMALL LETTER YI
					*p = 0x0545;	//	Õ…	0xd5 0x85	ARMENIAN CAPITAL LETTER YI
					break;
				case 0x0576:		//	Õ¶	0xd5 0xb6	ARMENIAN SMALL LETTER NOW
					*p = 0x0546;	//	Õ†	0xd5 0x86	ARMENIAN CAPITAL LETTER NOW
					break;
				case 0x0577:		//	Õ·	0xd5 0xb7	ARMENIAN SMALL LETTER SHA
					*p = 0x0547;	//	Õ‡	0xd5 0x87	ARMENIAN CAPITAL LETTER SHA
					break;
				case 0x0578:		//	Õ¸	0xd5 0xb8	ARMENIAN SMALL LETTER VO
					*p = 0x0548;	//	Õˆ	0xd5 0x88	ARMENIAN CAPITAL LETTER VO
					break;
				case 0x0579:		//	Õ¹	0xd5 0xb9	ARMENIAN SMALL LETTER CHA
					*p = 0x0549;	//	Õ‰	0xd5 0x89	ARMENIAN CAPITAL LETTER CHA
					break;
				case 0x057A:		//	Õº	0xd5 0xba	ARMENIAN SMALL LETTER PEH
					*p = 0x054A;	//	ÕŠ	0xd5 0x8a	ARMENIAN CAPITAL LETTER PEH
					break;
				case 0x057B:		//	Õ»	0xd5 0xbb	ARMENIAN SMALL LETTER JHEH
					*p = 0x054B;	//	Õ‹	0xd5 0x8b	ARMENIAN CAPITAL LETTER JHEH
					break;
				case 0x057C:		//	Õ¼	0xd5 0xbc	ARMENIAN SMALL LETTER RA
					*p = 0x054C;	//	ÕŒ	0xd5 0x8c	ARMENIAN CAPITAL LETTER RA
					break;
				case 0x057D:		//	Õ½	0xd5 0xbd	ARMENIAN SMALL LETTER SEH
					*p = 0x054D;	//	Õ	0xd5 0x8d	ARMENIAN CAPITAL LETTER SEH
					break;
				case 0x057E:		//	Õ¾	0xd5 0xbe	ARMENIAN SMALL LETTER VEW
					*p = 0x054E;	//	ÕŽ	0xd5 0x8e	ARMENIAN CAPITAL LETTER VEW
					break;
				case 0x057F:		//	Õ¿	0xd5 0xbf	ARMENIAN SMALL LETTER TIWN
					*p = 0x054F;	//	Õ	0xd5 0x8f	ARMENIAN CAPITAL LETTER TIWN
					break;
				case 0x0580:		//	Ö€	0xd6 0x80	ARMENIAN SMALL LETTER REH
					*p = 0x0550;	//	Õ	0xd5 0x90	ARMENIAN CAPITAL LETTER REH
					break;
				case 0x0581:		//	Ö	0xd6 0x81	ARMENIAN SMALL LETTER CO
					*p = 0x0551;	//	Õ‘	0xd5 0x91	ARMENIAN CAPITAL LETTER CO
					break;
				case 0x0582:		//	Ö‚	0xd6 0x82	ARMENIAN SMALL LETTER YIWN
					*p = 0x0552;	//	Õ’	0xd5 0x92	ARMENIAN CAPITAL LETTER YIWN
					break;
				case 0x0583:		//	Öƒ	0xd6 0x83	ARMENIAN SMALL LETTER PIWR
					*p = 0x0553;	//	Õ“	0xd5 0x93	ARMENIAN CAPITAL LETTER PIWR
					break;
				case 0x0584:		//	Ö„	0xd6 0x84	ARMENIAN SMALL LETTER KEH
					*p = 0x0554;	//	Õ”	0xd5 0x94	ARMENIAN CAPITAL LETTER KEH
					break;
				case 0x0585:		//	Ö…	0xd6 0x85	ARMENIAN SMALL LETTER OH
					*p = 0x0555;	//	Õ•	0xd5 0x95	ARMENIAN CAPITAL LETTER OH
					break;
				case 0x0586:		//	Ö†	0xd6 0x86	ARMENIAN SMALL LETTER FEH
					*p = 0x0556;	//	Õ–	0xd5 0x96	ARMENIAN CAPITAL LETTER FEH
					break;
				case 0x10A0:		//	á‚ 	0xe1 0x82 0xa0	GEORGIAN LETTER AN
					*p = 0x1C90;	//	á²	0xe1 0xb2 0x90	GEORGIAN CAPITAL LETTER AN
					break;
				case 0x10A1:		//	á‚¡	0xe1 0x82 0xa1	GEORGIAN LETTER BAN
					*p = 0x1C91;	//	á²‘	0xe1 0xb2 0x91	GEORGIAN CAPITAL LETTER BAN
					break;
				case 0x10A2:		//	á‚¢	0xe1 0x82 0xa2	GEORGIAN LETTER GAN
					*p = 0x1C92;	//	á²’	0xe1 0xb2 0x92	GEORGIAN CAPITAL LETTER GAN
					break;
				case 0x10A3:		//	á‚£	0xe1 0x82 0xa3	GEORGIAN LETTER DON
					*p = 0x1C93;	//	á²“	0xe1 0xb2 0x93	GEORGIAN CAPITAL LETTER DON
					break;
				case 0x10A4:		//	á‚¤	0xe1 0x82 0xa4	GEORGIAN LETTER EN
					*p = 0x1C94;	//	á²”	0xe1 0xb2 0x94	GEORGIAN CAPITAL LETTER EN
					break;
				case 0x10A5:		//	á‚¥	0xe1 0x82 0xa5	GEORGIAN LETTER VIN
					*p = 0x1C95;	//	á²•	0xe1 0xb2 0x95	GEORGIAN CAPITAL LETTER VIN
					break;
				case 0x10A6:		//	á‚¦	0xe1 0x82 0xa6	GEORGIAN LETTER ZEN
					*p = 0x1C96;	//	á²–	0xe1 0xb2 0x96	GEORGIAN CAPITAL LETTER ZEN
					break;
				case 0x10A7:		//	á‚§	0xe1 0x82 0xa7	GEORGIAN LETTER TAN
					*p = 0x1C97;	//	á²—	0xe1 0xb2 0x97	GEORGIAN CAPITAL LETTER TAN
					break;
				case 0x10A8:		//	á‚¨	0xe1 0x82 0xa8	GEORGIAN LETTER IN
					*p = 0x1C98;	//	á²˜	0xe1 0xb2 0x98	GEORGIAN CAPITAL LETTER IN
					break;
				case 0x10A9:		//	á‚©	0xe1 0x82 0xa9	GEORGIAN LETTER KAN
					*p = 0x1C99;	//	á²™	0xe1 0xb2 0x99	GEORGIAN CAPITAL LETTER KAN
					break;
				case 0x10AA:		//	á‚ª	0xe1 0x82 0xaa	GEORGIAN LETTER LAS
					*p = 0x1C9A;	//	á²š	0xe1 0xb2 0x9a	GEORGIAN CAPITAL LETTER LAS
					break;
				case 0x10AB:		//	á‚«	0xe1 0x82 0xab	GEORGIAN LETTER MAN
					*p = 0x1C9B;	//	á²›	0xe1 0xb2 0x9b	GEORGIAN CAPITAL LETTER MAN
					break;
				case 0x10AC:		//	á‚¬	0xe1 0x82 0xac	GEORGIAN LETTER NAR
					*p = 0x1C9C;	//	á²œ	0xe1 0xb2 0x9c	GEORGIAN CAPITAL LETTER NAR
					break;
				case 0x10AD:		//	á‚­	0xe1 0x82 0xad	GEORGIAN LETTER ON
					*p = 0x1C9D;	//	á²	0xe1 0xb2 0x9d	GEORGIAN CAPITAL LETTER ON
					break;
				case 0x10AE:		//	á‚®	0xe1 0x82 0xae	GEORGIAN LETTER PAR
					*p = 0x1C9E;	//	á²ž	0xe1 0xb2 0x9e	GEORGIAN CAPITAL LETTER PAR
					break;
				case 0x10AF:		//	á‚¯	0xe1 0x82 0xaf	GEORGIAN LETTER ZHAR
					*p = 0x1C9F;	//	á²Ÿ	0xe1 0xb2 0x9f	GEORGIAN CAPITAL LETTER ZHAR
					break;
				case 0x10B0:		//	á‚°	0xe1 0x82 0xb0	GEORGIAN LETTER RAE
					*p = 0x1CA0;	//	á² 	0xe1 0xb2 0xa0	GEORGIAN CAPITAL LETTER RAE
					break;
				case 0x10B1:		//	á‚±	0xe1 0x82 0xb1	GEORGIAN LETTER SAN
					*p = 0x1CA1;	//	á²¡	0xe1 0xb2 0xa1	GEORGIAN CAPITAL LETTER SAN
					break;
				case 0x10B2:		//	á‚²	0xe1 0x82 0xb2	GEORGIAN LETTER TAR
					*p = 0x1CA2;	//	á²¢	0xe1 0xb2 0xa2	GEORGIAN CAPITAL LETTER TAR
					break;
				case 0x10B3:		//	á‚³	0xe1 0x82 0xb3	GEORGIAN LETTER UN
					*p = 0x1CA3;	//	á²£	0xe1 0xb2 0xa3	GEORGIAN CAPITAL LETTER UN
					break;
				case 0x10B4:		//	á‚´	0xe1 0x82 0xb4	GEORGIAN LETTER PHAR
					*p = 0x1CA4;	//	á²¤	0xe1 0xb2 0xa4	GEORGIAN CAPITAL LETTER PHAR
					break;
				case 0x10B5:		//	á‚µ	0xe1 0x82 0xb5	GEORGIAN LETTER KHAR
					*p = 0x1CA5;	//	á²¥	0xe1 0xb2 0xa5	GEORGIAN CAPITAL LETTER KHAR
					break;
				case 0x10B6:		//	á‚¶	0xe1 0x82 0xb6	GEORGIAN LETTER GHAN
					*p = 0x1CA6;	//	á²¦	0xe1 0xb2 0xa6	GEORGIAN CAPITAL LETTER GHAN
					break;
				case 0x10B7:		//	á‚·	0xe1 0x82 0xb7	GEORGIAN LETTER QAR
					*p = 0x1CA7;	//	á²§	0xe1 0xb2 0xa7	GEORGIAN CAPITAL LETTER QAR
					break;
				case 0x10B8:		//	á‚¸	0xe1 0x82 0xb8	GEORGIAN LETTER SHIN
					*p = 0x1CA8;	//	á²¨	0xe1 0xb2 0xa8	GEORGIAN CAPITAL LETTER SHIN
					break;
				case 0x10B9:		//	á‚¹	0xe1 0x82 0xb9	GEORGIAN LETTER CHIN
					*p = 0x1CA9;	//	á²©	0xe1 0xb2 0xa9	GEORGIAN CAPITAL LETTER CHIN
					break;
				case 0x10BA:		//	á‚º	0xe1 0x82 0xba	GEORGIAN LETTER CAN
					*p = 0x1CAA;	//	á²ª	0xe1 0xb2 0xaa	GEORGIAN CAPITAL LETTER CAN
					break;
				case 0x10BB:		//	á‚»	0xe1 0x82 0xbb	GEORGIAN LETTER JIL
					*p = 0x1CAB;	//	á²«	0xe1 0xb2 0xab	GEORGIAN CAPITAL LETTER JIL
					break;
				case 0x10BC:		//	á‚¼	0xe1 0x82 0xbc	GEORGIAN LETTER CIL
					*p = 0x1CAC;	//	á²¬	0xe1 0xb2 0xac	GEORGIAN CAPITAL LETTER CIL
					break;
				case 0x10BD:		//	á‚½	0xe1 0x82 0xbd	GEORGIAN LETTER CHAR
					*p = 0x1CAD;	//	á²­	0xe1 0xb2 0xad	GEORGIAN CAPITAL LETTER CHAR
					break;
				case 0x10BE:		//	á‚¾	0xe1 0x82 0xbe	GEORGIAN LETTER XAN
					*p = 0x1CAE;	//	á²®	0xe1 0xb2 0xae	GEORGIAN CAPITAL LETTER XAN
					break;
				case 0x10BF:		//	á‚¿	0xe1 0x82 0xbf	GEORGIAN LETTER JHAN
					*p = 0x1CAF;	//	á²¯	0xe1 0xb2 0xaf	GEORGIAN CAPITAL LETTER JHAN
					break;
				case 0x10C0:		//	áƒ€	0xe1 0x83 0x80	GEORGIAN LETTER HAE
					*p = 0x1CB0;	//	á²°	0xe1 0xb2 0xb0	GEORGIAN CAPITAL LETTER HAE
					break;
				case 0x10C1:		//	áƒ	0xe1 0x83 0x81	GEORGIAN LETTER HE
					*p = 0x1CB1;	//	á²±	0xe1 0xb2 0xb1	GEORGIAN CAPITAL LETTER HE
					break;
				case 0x10C2:		//	áƒ‚	0xe1 0x83 0x82	GEORGIAN LETTER HIE
					*p = 0x1CB2;	//	á²²	0xe1 0xb2 0xb2	GEORGIAN CAPITAL LETTER HIE
					break;
				case 0x10C3:		//	áƒƒ	0xe1 0x83 0x83	GEORGIAN LETTER WE
					*p = 0x1CB3;	//	á²³	0xe1 0xb2 0xb3	GEORGIAN CAPITAL LETTER WE
					break;
				case 0x10C4:		//	áƒ„	0xe1 0x83 0x84	GEORGIAN LETTER HAR
					*p = 0x1CB4;	//	á²´	0xe1 0xb2 0xb4	GEORGIAN CAPITAL LETTER HAR
					break;
				case 0x10C5:		//	áƒ…	0xe1 0x83 0x85	GEORGIAN LETTER HOE
					*p = 0x1CB5;	//	á²µ	0xe1 0xb2 0xb5	GEORGIAN CAPITAL LETTER HOE
					break;
				case 0x10C7:		//	áƒ‡	0xe1 0x83 0x87	GEORGIAN LETTER YN
					*p = 0x1CB7;	//	á²·	0xe1 0xb2 0xb7	GEORGIAN CAPITAL LETTER YN
					break;
				case 0x10CD:		//	áƒ	0xe1 0x83 0x8d	GEORGIAN LETTER AEN
					*p = 0x1CBD;	//	á²½	0xe1 0xb2 0xbd	GEORGIAN CAPITAL LETTER AEN
					break;
				case 0x10D0:		//	áƒ	0xe1 0x83 0x90	GEORGIAN SMALL LETTER AN
					*p = 0x1C90;	//	á²	0xe1 0xb2 0x90	GEORGIAN CAPITAL LETTER AN
					break;
				case 0x10D1:		//	áƒ‘	0xe1 0x83 0x91	GEORGIAN SMALL LETTER BAN
					*p = 0x1C91;	//	á²‘	0xe1 0xb2 0x91	GEORGIAN CAPITAL LETTER BAN
					break;
				case 0x10D2:		//	áƒ’	0xe1 0x83 0x92	GEORGIAN SMALL LETTER GAN
					*p = 0x1C92;	//	á²’	0xe1 0xb2 0x92	GEORGIAN CAPITAL LETTER GAN
					break;
				case 0x10D3:		//	áƒ“	0xe1 0x83 0x93	GEORGIAN SMALL LETTER DON
					*p = 0x1C93;	//	á²“	0xe1 0xb2 0x93	GEORGIAN CAPITAL LETTER DON
					break;
				case 0x10D4:		//	áƒ”	0xe1 0x83 0x94	GEORGIAN SMALL LETTER EN
					*p = 0x1C94;	//	á²”	0xe1 0xb2 0x94	GEORGIAN CAPITAL LETTER EN
					break;
				case 0x10D5:		//	áƒ•	0xe1 0x83 0x95	GEORGIAN SMALL LETTER VIN
					*p = 0x1C95;	//	á²•	0xe1 0xb2 0x95	GEORGIAN CAPITAL LETTER VIN
					break;
				case 0x10D6:		//	áƒ–	0xe1 0x83 0x96	GEORGIAN SMALL LETTER ZEN
					*p = 0x1C96;	//	á²–	0xe1 0xb2 0x96	GEORGIAN CAPITAL LETTER ZEN
					break;
				case 0x10D7:		//	áƒ—	0xe1 0x83 0x97	GEORGIAN SMALL LETTER TAN
					*p = 0x1C97;	//	á²—	0xe1 0xb2 0x97	GEORGIAN CAPITAL LETTER TAN
					break;
				case 0x10D8:		//	áƒ˜	0xe1 0x83 0x98	GEORGIAN SMALL LETTER IN
					*p = 0x1C98;	//	á²˜	0xe1 0xb2 0x98	GEORGIAN CAPITAL LETTER IN
					break;
				case 0x10D9:		//	áƒ™	0xe1 0x83 0x99	GEORGIAN SMALL LETTER KAN
					*p = 0x1C99;	//	á²™	0xe1 0xb2 0x99	GEORGIAN CAPITAL LETTER KAN
					break;
				case 0x10DA:		//	áƒš	0xe1 0x83 0x9a	GEORGIAN SMALL LETTER LAS
					*p = 0x1C9A;	//	á²š	0xe1 0xb2 0x9a	GEORGIAN CAPITAL LETTER LAS
					break;
				case 0x10DB:		//	áƒ›	0xe1 0x83 0x9b	GEORGIAN SMALL LETTER MAN
					*p = 0x1C9B;	//	á²›	0xe1 0xb2 0x9b	GEORGIAN CAPITAL LETTER MAN
					break;
				case 0x10DC:		//	áƒœ	0xe1 0x83 0x9c	GEORGIAN SMALL LETTER NAR
					*p = 0x1C9C;	//	á²œ	0xe1 0xb2 0x9c	GEORGIAN CAPITAL LETTER NAR
					break;
				case 0x10DD:		//	áƒ	0xe1 0x83 0x9d	GEORGIAN SMALL LETTER ON
					*p = 0x1C9D;	//	á²	0xe1 0xb2 0x9d	GEORGIAN CAPITAL LETTER ON
					break;
				case 0x10DE:		//	áƒž	0xe1 0x83 0x9e	GEORGIAN SMALL LETTER PAR
					*p = 0x1C9E;	//	á²ž	0xe1 0xb2 0x9e	GEORGIAN CAPITAL LETTER PAR
					break;
				case 0x10DF:		//	áƒŸ	0xe1 0x83 0x9f	GEORGIAN SMALL LETTER ZHAR
					*p = 0x1C9F;	//	á²Ÿ	0xe1 0xb2 0x9f	GEORGIAN CAPITAL LETTER ZHAR
					break;
				case 0x10E0:		//	áƒ 	0xe1 0x83 0xa0	GEORGIAN SMALL LETTER RAE
					*p = 0x1CA0;	//	á² 	0xe1 0xb2 0xa0	GEORGIAN CAPITAL LETTER RAE
					break;
				case 0x10E1:		//	áƒ¡	0xe1 0x83 0xa1	GEORGIAN SMALL LETTER SAN
					*p = 0x1CA1;	//	á²¡	0xe1 0xb2 0xa1	GEORGIAN CAPITAL LETTER SAN
					break;
				case 0x10E2:		//	áƒ¢	0xe1 0x83 0xa2	GEORGIAN SMALL LETTER TAR
					*p = 0x1CA2;	//	á²¢	0xe1 0xb2 0xa2	GEORGIAN CAPITAL LETTER TAR
					break;
				case 0x10E3:		//	áƒ£	0xe1 0x83 0xa3	GEORGIAN SMALL LETTER UN
					*p = 0x1CA3;	//	á²£	0xe1 0xb2 0xa3	GEORGIAN CAPITAL LETTER UN
					break;
				case 0x10E4:		//	áƒ¤	0xe1 0x83 0xa4	GEORGIAN SMALL LETTER PHAR
					*p = 0x1CA4;	//	á²¤	0xe1 0xb2 0xa4	GEORGIAN CAPITAL LETTER PHAR
					break;
				case 0x10E5:		//	áƒ¥	0xe1 0x83 0xa5	GEORGIAN SMALL LETTER KHAR
					*p = 0x1CA5;	//	á²¥	0xe1 0xb2 0xa5	GEORGIAN CAPITAL LETTER KHAR
					break;
				case 0x10E6:		//	áƒ¦	0xe1 0x83 0xa6	GEORGIAN SMALL LETTER GHAN
					*p = 0x1CA6;	//	á²¦	0xe1 0xb2 0xa6	GEORGIAN CAPITAL LETTER GHAN
					break;
				case 0x10E7:		//	áƒ§	0xe1 0x83 0xa7	GEORGIAN SMALL LETTER QAR
					*p = 0x1CA7;	//	á²§	0xe1 0xb2 0xa7	GEORGIAN CAPITAL LETTER QAR
					break;
				case 0x10E8:		//	áƒ¨	0xe1 0x83 0xa8	GEORGIAN SMALL LETTER SHIN
					*p = 0x1CA8;	//	á²¨	0xe1 0xb2 0xa8	GEORGIAN CAPITAL LETTER SHIN
					break;
				case 0x10E9:		//	áƒ©	0xe1 0x83 0xa9	GEORGIAN SMALL LETTER CHIN
					*p = 0x1CA9;	//	á²©	0xe1 0xb2 0xa9	GEORGIAN CAPITAL LETTER CHIN
					break;
				case 0x10EA:		//	áƒª	0xe1 0x83 0xaa	GEORGIAN SMALL LETTER CAN
					*p = 0x1CAA;	//	á²ª	0xe1 0xb2 0xaa	GEORGIAN CAPITAL LETTER CAN
					break;
				case 0x10EB:		//	áƒ«	0xe1 0x83 0xab	GEORGIAN SMALL LETTER JIL
					*p = 0x1CAB;	//	á²«	0xe1 0xb2 0xab	GEORGIAN CAPITAL LETTER JIL
					break;
				case 0x10EC:		//	áƒ¬	0xe1 0x83 0xac	GEORGIAN SMALL LETTER CIL
					*p = 0x1CAC;	//	á²¬	0xe1 0xb2 0xac	GEORGIAN CAPITAL LETTER CIL
					break;
				case 0x10ED:		//	áƒ­	0xe1 0x83 0xad	GEORGIAN SMALL LETTER CHAR
					*p = 0x1CAD;	//	á²­	0xe1 0xb2 0xad	GEORGIAN CAPITAL LETTER CHAR
					break;
				case 0x10EE:		//	áƒ®	0xe1 0x83 0xae	GEORGIAN SMALL LETTER XAN
					*p = 0x1CAE;	//	á²®	0xe1 0xb2 0xae	GEORGIAN CAPITAL LETTER XAN
					break;
				case 0x10EF:		//	áƒ¯	0xe1 0x83 0xaf	GEORGIAN SMALL LETTER JHAN
					*p = 0x1CAF;	//	á²¯	0xe1 0xb2 0xaf	GEORGIAN CAPITAL LETTER JHAN
					break;
				case 0x10F0:		//	áƒ°	0xe1 0x83 0xb0	GEORGIAN SMALL LETTER HAE
					*p = 0x1CB0;	//	á²°	0xe1 0xb2 0xb0	GEORGIAN CAPITAL LETTER HAE
					break;
				case 0x10F1:		//	áƒ±	0xe1 0x83 0xb1	GEORGIAN SMALL LETTER HE
					*p = 0x1CB1;	//	á²±	0xe1 0xb2 0xb1	GEORGIAN CAPITAL LETTER HE
					break;
				case 0x10F2:		//	áƒ²	0xe1 0x83 0xb2	GEORGIAN SMALL LETTER HIE
					*p = 0x1CB2;	//	á²²	0xe1 0xb2 0xb2	GEORGIAN CAPITAL LETTER HIE
					break;
				case 0x10F3:		//	áƒ³	0xe1 0x83 0xb3	GEORGIAN SMALL LETTER WE
					*p = 0x1CB3;	//	á²³	0xe1 0xb2 0xb3	GEORGIAN CAPITAL LETTER WE
					break;
				case 0x10F4:		//	áƒ´	0xe1 0x83 0xb4	GEORGIAN SMALL LETTER HAR
					*p = 0x1CB4;	//	á²´	0xe1 0xb2 0xb4	GEORGIAN CAPITAL LETTER HAR
					break;
				case 0x10F5:		//	áƒµ	0xe1 0x83 0xb5	GEORGIAN SMALL LETTER HOE
					*p = 0x1CB5;	//	á²µ	0xe1 0xb2 0xb5	GEORGIAN CAPITAL LETTER HOE
					break;
				case 0x10F6:		//	áƒ¶	0xe1 0x83 0xb6	GEORGIAN SMALL LETTER FI
					*p = 0x1CB6;	//	á²¶	0xe1 0xb2 0xb6	GEORGIAN CAPITAL LETTER FI
					break;
				case 0x10F7:		//	áƒ·	0xe1 0x83 0xb7	GEORGIAN SMALL LETTER YN
					*p = 0x1CB7;	//	á²·	0xe1 0xb2 0xb7	GEORGIAN CAPITAL LETTER YN
					break;
				case 0x10F8:		//	áƒ¸	0xe1 0x83 0xb8	GEORGIAN SMALL LETTER ELIFI
					*p = 0x1CB8;	//	á²¸	0xe1 0xb2 0xb8	GEORGIAN CAPITAL LETTER ELIFI
					break;
				case 0x10F9:		//	áƒ¹	0xe1 0x83 0xb9	GEORGIAN SMALL LETTER TURNED GAN
					*p = 0x1CB9;	//	á²¹	0xe1 0xb2 0xb9	GEORGIAN CAPITAL LETTER TURNED GAN
					break;
				case 0x10FA:		//	áƒº	0xe1 0x83 0xba	GEORGIAN SMALL LETTER AIN
					*p = 0x1CBA;	//	á²º	0xe1 0xb2 0xba	GEORGIAN CAPITAL LETTER AIN
					break;
				case 0x10FD:		//	áƒ½	0xe1 0x83 0xbd	GEORGIAN SMALL LETTER AEN
					*p = 0x1CBD;	//	á²½	0xe1 0xb2 0xbd	GEORGIAN CAPITAL LETTER AEN
					break;
				case 0x10FE:		//	áƒ¾	0xe1 0x83 0xbe	GEORGIAN SMALL LETTER HARD SIGN
					*p = 0x1CBE;	//	á²¾	0xe1 0xb2 0xbe	GEORGIAN CAPITAL LETTER HARD SIGN
					break;
				case 0x10FF:		//	áƒ¿	0xe1 0x83 0xbf	GEORGIAN SMALL LETTER LABIAL SIGN
					*p = 0x1CBF;	//	á²¿	0xe1 0xb2 0xbf	GEORGIAN CAPITAL LETTER LABIAL SIGN
					break;
				case 0x13F8:		//	á¸	0xe1 0x8f 0xb8	CHEROKEE SMALL LETTER YE
					*p = 0x13F0;	//	á°	0xe1 0x8f 0xb0	CHEROKEE CAPITAL LETTER YE
					break;
				case 0x13F9:		//	á¹	0xe1 0x8f 0xb9	CHEROKEE SMALL LETTER YI
					*p = 0x13F1;	//	á±	0xe1 0x8f 0xb1	CHEROKEE CAPITAL LETTER YI
					break;
				case 0x13FA:		//	áº	0xe1 0x8f 0xba	CHEROKEE SMALL LETTER YO
					*p = 0x13F2;	//	á²	0xe1 0x8f 0xb2	CHEROKEE CAPITAL LETTER YO
					break;
				case 0x13FB:		//	á»	0xe1 0x8f 0xbb	CHEROKEE SMALL LETTER YU
					*p = 0x13F3;	//	á³	0xe1 0x8f 0xb3	CHEROKEE CAPITAL LETTER YU
					break;
				case 0x13FC:		//	á¼	0xe1 0x8f 0xbc	CHEROKEE SMALL LETTER YV
					*p = 0x13F4;	//	á´	0xe1 0x8f 0xb4	CHEROKEE CAPITAL LETTER YV
					break;
				case 0x13FD:		//	á½	0xe1 0x8f 0xbd	CHEROKEE SMALL LETTER MV
					*p = 0x13F5;	//	áµ	0xe1 0x8f 0xb5	CHEROKEE CAPITAL LETTER MV
					break;
				case 0x1D79:		//	áµ¹	0xe1 0xb5 0xb9	LATIN SMALL LETTER INSULAR G
					*p = 0xA77D;	//	ê½	0xea 0x9d 0xbd	LATIN CAPITAL LETTER INSULAR G
					break;
				case 0x1D7D:		//	áµ½	0xe1 0xb5 0xbd	LATIN SMALL LETTER P WITH STROKE
					*p = 0x2C63;	//	â±£	0xe2 0xb1 0xa3	LATIN CAPITAL LETTER P WITH STROKE
					break;
				case 0x1D8E:		//	á¶Ž	0xe1 0xb6 0x8e	LATIN SMALL LETTER Z WITH PALATAL HOOK
					*p = 0xA7C6;	//	êŸ†	0xea 0x9f 0x86	LATIN CAPITAL LETTER Z WITH PALATAL HOOK
					break;
				case 0x1E01:		//	á¸	0xe1 0xb8 0x81	LATIN SMALL LETTER A WITH RING BELOW
					*p = 0x1E00;	//	á¸€	0xe1 0xb8 0x80	LATIN CAPITAL LETTER A WITH RING BELOW
					break;
				case 0x1E03:		//	á¸ƒ	0xe1 0xb8 0x83	LATIN SMALL LETTER B WITH DOT ABOVE
					*p = 0x1E02;	//	á¸‚	0xe1 0xb8 0x82	LATIN CAPITAL LETTER B WITH DOT ABOVE
					break;
				case 0x1E05:		//	á¸…	0xe1 0xb8 0x85	LATIN SMALL LETTER B WITH DOT BELOW
					*p = 0x1E04;	//	á¸„	0xe1 0xb8 0x84	LATIN CAPITAL LETTER B WITH DOT BELOW
					break;
				case 0x1E07:		//	á¸‡	0xe1 0xb8 0x87	LATIN SMALL LETTER B WITH LINE BELOW
					*p = 0x1E06;	//	á¸†	0xe1 0xb8 0x86	LATIN CAPITAL LETTER B WITH LINE BELOW
					break;
				case 0x1E09:		//	á¸‰	0xe1 0xb8 0x89	LATIN SMALL LETTER C WITH CEDILLA AND ACUTE
					*p = 0x1E08;	//	á¸ˆ	0xe1 0xb8 0x88	LATIN CAPITAL LETTER C WITH CEDILLA AND ACUTE
					break;
				case 0x1E0B:		//	á¸‹	0xe1 0xb8 0x8b	LATIN SMALL LETTER D WITH DOT ABOVE
					*p = 0x1E0A;	//	á¸Š	0xe1 0xb8 0x8a	LATIN CAPITAL LETTER D WITH DOT ABOVE
					break;
				case 0x1E0D:		//	á¸	0xe1 0xb8 0x8d	LATIN SMALL LETTER D WITH DOT BELOW
					*p = 0x1E0C;	//	á¸Œ	0xe1 0xb8 0x8c	LATIN CAPITAL LETTER D WITH DOT BELOW
					break;
				case 0x1E0F:		//	á¸	0xe1 0xb8 0x8f	LATIN SMALL LETTER D WITH LINE BELOW
					*p = 0x1E0E;	//	á¸Ž	0xe1 0xb8 0x8e	LATIN CAPITAL LETTER D WITH LINE BELOW
					break;
				case 0x1E11:		//	á¸‘	0xe1 0xb8 0x91	LATIN SMALL LETTER D WITH CEDILLA
					*p = 0x1E10;	//	á¸	0xe1 0xb8 0x90	LATIN CAPITAL LETTER D WITH CEDILLA
					break;
				case 0x1E13:		//	á¸“	0xe1 0xb8 0x93	LATIN SMALL LETTER D WITH CIRCUMFLEX BELOW
					*p = 0x1E12;	//	á¸’	0xe1 0xb8 0x92	LATIN CAPITAL LETTER D WITH CIRCUMFLEX BELOW
					break;
				case 0x1E15:		//	á¸•	0xe1 0xb8 0x95	LATIN SMALL LETTER E WITH MACRON AND GRAVE
					*p = 0x1E14;	//	á¸”	0xe1 0xb8 0x94	LATIN CAPITAL LETTER E WITH MACRON AND GRAVE
					break;
				case 0x1E17:		//	á¸—	0xe1 0xb8 0x97	LATIN SMALL LETTER E WITH MACRON AND ACUTE
					*p = 0x1E16;	//	á¸–	0xe1 0xb8 0x96	LATIN CAPITAL LETTER E WITH MACRON AND ACUTE
					break;
				case 0x1E19:		//	á¸™	0xe1 0xb8 0x99	LATIN SMALL LETTER E WITH CIRCUMFLEX BELOW
					*p = 0x1E18;	//	á¸˜	0xe1 0xb8 0x98	LATIN CAPITAL LETTER E WITH CIRCUMFLEX BELOW
					break;
				case 0x1E1B:		//	á¸›	0xe1 0xb8 0x9b	LATIN SMALL LETTER E WITH TILDE BELOW
					*p = 0x1E1A;	//	á¸š	0xe1 0xb8 0x9a	LATIN CAPITAL LETTER E WITH TILDE BELOW
					break;
				case 0x1E1D:		//	á¸	0xe1 0xb8 0x9d	LATIN SMALL LETTER E WITH CEDILLA AND BREVE
					*p = 0x1E1C;	//	á¸œ	0xe1 0xb8 0x9c	LATIN CAPITAL LETTER E WITH CEDILLA AND BREVE
					break;
				case 0x1E1F:		//	á¸Ÿ	0xe1 0xb8 0x9f	LATIN SMALL LETTER F WITH DOT ABOVE
					*p = 0x1E1E;	//	á¸ž	0xe1 0xb8 0x9e	LATIN CAPITAL LETTER F WITH DOT ABOVE
					break;
				case 0x1E21:		//	á¸¡	0xe1 0xb8 0xa1	LATIN SMALL LETTER G WITH MACRON
					*p = 0x1E20;	//	á¸ 	0xe1 0xb8 0xa0	LATIN CAPITAL LETTER G WITH MACRON
					break;
				case 0x1E23:		//	á¸£	0xe1 0xb8 0xa3	LATIN SMALL LETTER H WITH DOT ABOVE
					*p = 0x1E22;	//	á¸¢	0xe1 0xb8 0xa2	LATIN CAPITAL LETTER H WITH DOT ABOVE
					break;
				case 0x1E25:		//	á¸¥	0xe1 0xb8 0xa5	LATIN SMALL LETTER H WITH DOT BELOW
					*p = 0x1E24;	//	á¸¤	0xe1 0xb8 0xa4	LATIN CAPITAL LETTER H WITH DOT BELOW
					break;
				case 0x1E27:		//	á¸§	0xe1 0xb8 0xa7	LATIN SMALL LETTER H WITH DIAERESIS
					*p = 0x1E26;	//	á¸¦	0xe1 0xb8 0xa6	LATIN CAPITAL LETTER H WITH DIAERESIS
					break;
				case 0x1E29:		//	á¸©	0xe1 0xb8 0xa9	LATIN SMALL LETTER H WITH CEDILLA
					*p = 0x1E28;	//	á¸¨	0xe1 0xb8 0xa8	LATIN CAPITAL LETTER H WITH CEDILLA
					break;
				case 0x1E2B:		//	á¸«	0xe1 0xb8 0xab	LATIN SMALL LETTER H WITH BREVE BELOW
					*p = 0x1E2A;	//	á¸ª	0xe1 0xb8 0xaa	LATIN CAPITAL LETTER H WITH BREVE BELOW
					break;
				case 0x1E2D:		//	á¸­	0xe1 0xb8 0xad	LATIN SMALL LETTER I WITH TILDE BELOW
					*p = 0x1E2C;	//	á¸¬	0xe1 0xb8 0xac	LATIN CAPITAL LETTER I WITH TILDE BELOW
					break;
				case 0x1E2F:		//	á¸¯	0xe1 0xb8 0xaf	LATIN SMALL LETTER I WITH DIAERESIS AND ACUTE
					*p = 0x1E2E;	//	á¸®	0xe1 0xb8 0xae	LATIN CAPITAL LETTER I WITH DIAERESIS AND ACUTE
					break;
				case 0x1E31:		//	á¸±	0xe1 0xb8 0xb1	LATIN SMALL LETTER K WITH ACUTE
					*p = 0x1E30;	//	á¸°	0xe1 0xb8 0xb0	LATIN CAPITAL LETTER K WITH ACUTE
					break;
				case 0x1E33:		//	á¸³	0xe1 0xb8 0xb3	LATIN SMALL LETTER K WITH DOT BELOW
					*p = 0x1E32;	//	á¸²	0xe1 0xb8 0xb2	LATIN CAPITAL LETTER K WITH DOT BELOW
					break;
				case 0x1E35:		//	á¸µ	0xe1 0xb8 0xb5	LATIN SMALL LETTER K WITH LINE BELOW
					*p = 0x1E34;	//	á¸´	0xe1 0xb8 0xb4	LATIN CAPITAL LETTER K WITH LINE BELOW
					break;
				case 0x1E37:		//	á¸·	0xe1 0xb8 0xb7	LATIN SMALL LETTER L WITH DOT BELOW
					*p = 0x1E36;	//	á¸¶	0xe1 0xb8 0xb6	LATIN CAPITAL LETTER L WITH DOT BELOW
					break;
				case 0x1E39:		//	á¸¹	0xe1 0xb8 0xb9	LATIN SMALL LETTER L WITH DOT BELOW AND MACRON
					*p = 0x1E38;	//	á¸¸	0xe1 0xb8 0xb8	LATIN CAPITAL LETTER L WITH DOT BELOW AND MACRON
					break;
				case 0x1E3B:		//	á¸»	0xe1 0xb8 0xbb	LATIN SMALL LETTER L WITH LINE BELOW
					*p = 0x1E3A;	//	á¸º	0xe1 0xb8 0xba	LATIN CAPITAL LETTER L WITH LINE BELOW
					break;
				case 0x1E3D:		//	á¸½	0xe1 0xb8 0xbd	LATIN SMALL LETTER L WITH CIRCUMFLEX BELOW
					*p = 0x1E3C;	//	á¸¼	0xe1 0xb8 0xbc	LATIN CAPITAL LETTER L WITH CIRCUMFLEX BELOW
					break;
				case 0x1E3F:		//	á¸¿	0xe1 0xb8 0xbf	LATIN SMALL LETTER M WITH ACUTE
					*p = 0x1E3E;	//	á¸¾	0xe1 0xb8 0xbe	LATIN CAPITAL LETTER M WITH ACUTE
					break;
				case 0x1E41:		//	á¹	0xe1 0xb9 0x81	LATIN SMALL LETTER M WITH DOT ABOVE
					*p = 0x1E40;	//	á¹€	0xe1 0xb9 0x80	LATIN CAPITAL LETTER M WITH DOT ABOVE
					break;
				case 0x1E43:		//	á¹ƒ	0xe1 0xb9 0x83	LATIN SMALL LETTER M WITH DOT BELOW
					*p = 0x1E42;	//	á¹‚	0xe1 0xb9 0x82	LATIN CAPITAL LETTER M WITH DOT BELOW
					break;
				case 0x1E45:		//	á¹…	0xe1 0xb9 0x85	LATIN SMALL LETTER N WITH DOT ABOVE
					*p = 0x1E44;	//	á¹„	0xe1 0xb9 0x84	LATIN CAPITAL LETTER N WITH DOT ABOVE
					break;
				case 0x1E47:		//	á¹‡	0xe1 0xb9 0x87	LATIN SMALL LETTER N WITH DOT BELOW
					*p = 0x1E46;	//	á¹†	0xe1 0xb9 0x86	LATIN CAPITAL LETTER N WITH DOT BELOW
					break;
				case 0x1E49:		//	á¹‰	0xe1 0xb9 0x89	LATIN SMALL LETTER N WITH LINE BELOW
					*p = 0x1E48;	//	á¹ˆ	0xe1 0xb9 0x88	LATIN CAPITAL LETTER N WITH LINE BELOW
					break;
				case 0x1E4B:		//	á¹‹	0xe1 0xb9 0x8b	LATIN SMALL LETTER N WITH CIRCUMFLEX BELOW
					*p = 0x1E4A;	//	á¹Š	0xe1 0xb9 0x8a	LATIN CAPITAL LETTER N WITH CIRCUMFLEX BELOW
					break;
				case 0x1E4D:		//	á¹	0xe1 0xb9 0x8d	LATIN SMALL LETTER O WITH TILDE AND ACUTE
					*p = 0x1E4C;	//	á¹Œ	0xe1 0xb9 0x8c	LATIN CAPITAL LETTER O WITH TILDE AND ACUTE
					break;
				case 0x1E4F:		//	á¹	0xe1 0xb9 0x8f	LATIN SMALL LETTER O WITH TILDE AND DIAERESIS
					*p = 0x1E4E;	//	á¹Ž	0xe1 0xb9 0x8e	LATIN CAPITAL LETTER O WITH TILDE AND DIAERESIS
					break;
				case 0x1E51:		//	á¹‘	0xe1 0xb9 0x91	LATIN SMALL LETTER O WITH MACRON AND GRAVE
					*p = 0x1E50;	//	á¹	0xe1 0xb9 0x90	LATIN CAPITAL LETTER O WITH MACRON AND GRAVE
					break;
				case 0x1E53:		//	á¹“	0xe1 0xb9 0x93	LATIN SMALL LETTER O WITH MACRON AND ACUTE
					*p = 0x1E52;	//	á¹’	0xe1 0xb9 0x92	LATIN CAPITAL LETTER O WITH MACRON AND ACUTE
					break;
				case 0x1E55:		//	á¹•	0xe1 0xb9 0x95	LATIN SMALL LETTER P WITH ACUTE
					*p = 0x1E54;	//	á¹”	0xe1 0xb9 0x94	LATIN CAPITAL LETTER P WITH ACUTE
					break;
				case 0x1E57:		//	á¹—	0xe1 0xb9 0x97	LATIN SMALL LETTER P WITH DOT ABOVE
					*p = 0x1E56;	//	á¹–	0xe1 0xb9 0x96	LATIN CAPITAL LETTER P WITH DOT ABOVE
					break;
				case 0x1E59:		//	á¹™	0xe1 0xb9 0x99	LATIN SMALL LETTER R WITH DOT ABOVE
					*p = 0x1E58;	//	á¹˜	0xe1 0xb9 0x98	LATIN CAPITAL LETTER R WITH DOT ABOVE
					break;
				case 0x1E5B:		//	á¹›	0xe1 0xb9 0x9b	LATIN SMALL LETTER R WITH DOT BELOW
					*p = 0x1E5A;	//	á¹š	0xe1 0xb9 0x9a	LATIN CAPITAL LETTER R WITH DOT BELOW
					break;
				case 0x1E5D:		//	á¹	0xe1 0xb9 0x9d	LATIN SMALL LETTER R WITH DOT BELOW AND MACRON
					*p = 0x1E5C;	//	á¹œ	0xe1 0xb9 0x9c	LATIN CAPITAL LETTER R WITH DOT BELOW AND MACRON
					break;
				case 0x1E5F:		//	á¹Ÿ	0xe1 0xb9 0x9f	LATIN SMALL LETTER R WITH LINE BELOW
					*p = 0x1E5E;	//	á¹ž	0xe1 0xb9 0x9e	LATIN CAPITAL LETTER R WITH LINE BELOW
					break;
				case 0x1E61:		//	á¹¡	0xe1 0xb9 0xa1	LATIN SMALL LETTER S WITH DOT ABOVE
					*p = 0x1E60;	//	á¹ 	0xe1 0xb9 0xa0	LATIN CAPITAL LETTER S WITH DOT ABOVE
					break;
				case 0x1E63:		//	á¹£	0xe1 0xb9 0xa3	LATIN SMALL LETTER S WITH DOT BELOW
					*p = 0x1E62;	//	á¹¢	0xe1 0xb9 0xa2	LATIN CAPITAL LETTER S WITH DOT BELOW
					break;
				case 0x1E65:		//	á¹¥	0xe1 0xb9 0xa5	LATIN SMALL LETTER S WITH ACUTE AND DOT ABOVE
					*p = 0x1E64;	//	á¹¤	0xe1 0xb9 0xa4	LATIN CAPITAL LETTER S WITH ACUTE AND DOT ABOVE
					break;
				case 0x1E67:		//	á¹§	0xe1 0xb9 0xa7	LATIN SMALL LETTER S WITH CARON AND DOT ABOVE
					*p = 0x1E66;	//	á¹¦	0xe1 0xb9 0xa6	LATIN CAPITAL LETTER S WITH CARON AND DOT ABOVE
					break;
				case 0x1E69:		//	á¹©	0xe1 0xb9 0xa9	LATIN SMALL LETTER S WITH DOT BELOW AND DOT ABOVE
					*p = 0x1E68;	//	á¹¨	0xe1 0xb9 0xa8	LATIN CAPITAL LETTER S WITH DOT BELOW AND DOT ABOVE
					break;
				case 0x1E6B:		//	á¹«	0xe1 0xb9 0xab	LATIN SMALL LETTER T WITH DOT ABOVE
					*p = 0x1E6A;	//	á¹ª	0xe1 0xb9 0xaa	LATIN CAPITAL LETTER T WITH DOT ABOVE
					break;
				case 0x1E6D:		//	á¹­	0xe1 0xb9 0xad	LATIN SMALL LETTER T WITH DOT BELOW
					*p = 0x1E6C;	//	á¹¬	0xe1 0xb9 0xac	LATIN CAPITAL LETTER T WITH DOT BELOW
					break;
				case 0x1E6F:		//	á¹¯	0xe1 0xb9 0xaf	LATIN SMALL LETTER T WITH LINE BELOW
					*p = 0x1E6E;	//	á¹®	0xe1 0xb9 0xae	LATIN CAPITAL LETTER T WITH LINE BELOW
					break;
				case 0x1E71:		//	á¹±	0xe1 0xb9 0xb1	LATIN SMALL LETTER T WITH CIRCUMFLEX BELOW
					*p = 0x1E70;	//	á¹°	0xe1 0xb9 0xb0	LATIN CAPITAL LETTER T WITH CIRCUMFLEX BELOW
					break;
				case 0x1E73:		//	á¹³	0xe1 0xb9 0xb3	LATIN SMALL LETTER U WITH DIAERESIS BELOW
					*p = 0x1E72;	//	á¹²	0xe1 0xb9 0xb2	LATIN CAPITAL LETTER U WITH DIAERESIS BELOW
					break;
				case 0x1E75:		//	á¹µ	0xe1 0xb9 0xb5	LATIN SMALL LETTER U WITH TILDE BELOW
					*p = 0x1E74;	//	á¹´	0xe1 0xb9 0xb4	LATIN CAPITAL LETTER U WITH TILDE BELOW
					break;
				case 0x1E77:		//	á¹·	0xe1 0xb9 0xb7	LATIN SMALL LETTER U WITH CIRCUMFLEX BELOW
					*p = 0x1E76;	//	á¹¶	0xe1 0xb9 0xb6	LATIN CAPITAL LETTER U WITH CIRCUMFLEX BELOW
					break;
				case 0x1E79:		//	á¹¹	0xe1 0xb9 0xb9	LATIN SMALL LETTER U WITH TILDE AND ACUTE
					*p = 0x1E78;	//	á¹¸	0xe1 0xb9 0xb8	LATIN CAPITAL LETTER U WITH TILDE AND ACUTE
					break;
				case 0x1E7B:		//	á¹»	0xe1 0xb9 0xbb	LATIN SMALL LETTER U WITH MACRON AND DIAERESIS
					*p = 0x1E7A;	//	á¹º	0xe1 0xb9 0xba	LATIN CAPITAL LETTER U WITH MACRON AND DIAERESIS
					break;
				case 0x1E7D:		//	á¹½	0xe1 0xb9 0xbd	LATIN SMALL LETTER V WITH TILDE
					*p = 0x1E7C;	//	á¹¼	0xe1 0xb9 0xbc	LATIN CAPITAL LETTER V WITH TILDE
					break;
				case 0x1E7F:		//	á¹¿	0xe1 0xb9 0xbf	LATIN SMALL LETTER V WITH DOT BELOW
					*p = 0x1E7E;	//	á¹¾	0xe1 0xb9 0xbe	LATIN CAPITAL LETTER V WITH DOT BELOW
					break;
				case 0x1E81:		//	áº	0xe1 0xba 0x81	LATIN SMALL LETTER W WITH GRAVE
					*p = 0x1E80;	//	áº€	0xe1 0xba 0x80	LATIN CAPITAL LETTER W WITH GRAVE
					break;
				case 0x1E83:		//	áºƒ	0xe1 0xba 0x83	LATIN SMALL LETTER W WITH ACUTE
					*p = 0x1E82;	//	áº‚	0xe1 0xba 0x82	LATIN CAPITAL LETTER W WITH ACUTE
					break;
				case 0x1E85:		//	áº…	0xe1 0xba 0x85	LATIN SMALL LETTER W WITH DIAERESIS
					*p = 0x1E84;	//	áº„	0xe1 0xba 0x84	LATIN CAPITAL LETTER W WITH DIAERESIS
					break;
				case 0x1E87:		//	áº‡	0xe1 0xba 0x87	LATIN SMALL LETTER W WITH DOT ABOVE
					*p = 0x1E86;	//	áº†	0xe1 0xba 0x86	LATIN CAPITAL LETTER W WITH DOT ABOVE
					break;
				case 0x1E89:		//	áº‰	0xe1 0xba 0x89	LATIN SMALL LETTER W WITH DOT BELOW
					*p = 0x1E88;	//	áºˆ	0xe1 0xba 0x88	LATIN CAPITAL LETTER W WITH DOT BELOW
					break;
				case 0x1E8B:		//	áº‹	0xe1 0xba 0x8b	LATIN SMALL LETTER X WITH DOT ABOVE
					*p = 0x1E8A;	//	áºŠ	0xe1 0xba 0x8a	LATIN CAPITAL LETTER X WITH DOT ABOVE
					break;
				case 0x1E8D:		//	áº	0xe1 0xba 0x8d	LATIN SMALL LETTER X WITH DIAERESIS
					*p = 0x1E8C;	//	áºŒ	0xe1 0xba 0x8c	LATIN CAPITAL LETTER X WITH DIAERESIS
					break;
				case 0x1E8F:		//	áº	0xe1 0xba 0x8f	LATIN SMALL LETTER Y WITH DOT ABOVE
					*p = 0x1E8E;	//	áºŽ	0xe1 0xba 0x8e	LATIN CAPITAL LETTER Y WITH DOT ABOVE
					break;
				case 0x1E91:		//	áº‘	0xe1 0xba 0x91	LATIN SMALL LETTER Z WITH CIRCUMFLEX
					*p = 0x1E90;	//	áº	0xe1 0xba 0x90	LATIN CAPITAL LETTER Z WITH CIRCUMFLEX
					break;
				case 0x1E93:		//	áº“	0xe1 0xba 0x93	LATIN SMALL LETTER Z WITH DOT BELOW
					*p = 0x1E92;	//	áº’	0xe1 0xba 0x92	LATIN CAPITAL LETTER Z WITH DOT BELOW
					break;
				case 0x1E95:		//	áº•	0xe1 0xba 0x95	LATIN SMALL LETTER Z WITH LINE BELOW
					*p = 0x1E94;	//	áº”	0xe1 0xba 0x94	LATIN CAPITAL LETTER Z WITH LINE BELOW
					break;
				case 0x1EA1:		//	áº¡	0xe1 0xba 0xa1	LATIN SMALL LETTER A WITH DOT BELOW
					*p = 0x1EA0;	//	áº 	0xe1 0xba 0xa0	LATIN CAPITAL LETTER A WITH DOT BELOW
					break;
				case 0x1EA3:		//	áº£	0xe1 0xba 0xa3	LATIN SMALL LETTER A WITH HOOK ABOVE
					*p = 0x1EA2;	//	áº¢	0xe1 0xba 0xa2	LATIN CAPITAL LETTER A WITH HOOK ABOVE
					break;
				case 0x1EA5:		//	áº¥	0xe1 0xba 0xa5	LATIN SMALL LETTER A WITH CIRCUMFLEX AND ACUTE
					*p = 0x1EA4;	//	áº¤	0xe1 0xba 0xa4	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1EA7:		//	áº§	0xe1 0xba 0xa7	LATIN SMALL LETTER A WITH CIRCUMFLEX AND GRAVE
					*p = 0x1EA6;	//	áº¦	0xe1 0xba 0xa6	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1EA9:		//	áº©	0xe1 0xba 0xa9	LATIN SMALL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1EA8;	//	áº¨	0xe1 0xba 0xa8	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1EAB:		//	áº«	0xe1 0xba 0xab	LATIN SMALL LETTER A WITH CIRCUMFLEX AND TILDE
					*p = 0x1EAA;	//	áºª	0xe1 0xba 0xaa	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1EAD:		//	áº­	0xe1 0xba 0xad	LATIN SMALL LETTER A WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1EAC;	//	áº¬	0xe1 0xba 0xac	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EAF:		//	áº¯	0xe1 0xba 0xaf	LATIN SMALL LETTER A WITH BREVE AND ACUTE
					*p = 0x1EAE;	//	áº®	0xe1 0xba 0xae	LATIN CAPITAL LETTER A WITH BREVE AND ACUTE
					break;
				case 0x1EB1:		//	áº±	0xe1 0xba 0xb1	LATIN SMALL LETTER A WITH BREVE AND GRAVE
					*p = 0x1EB0;	//	áº°	0xe1 0xba 0xb0	LATIN CAPITAL LETTER A WITH BREVE AND GRAVE
					break;
				case 0x1EB3:		//	áº³	0xe1 0xba 0xb3	LATIN SMALL LETTER A WITH BREVE AND HOOK ABOVE
					*p = 0x1EB2;	//	áº²	0xe1 0xba 0xb2	LATIN CAPITAL LETTER A WITH BREVE AND HOOK ABOVE
					break;
				case 0x1EB5:		//	áºµ	0xe1 0xba 0xb5	LATIN SMALL LETTER A WITH BREVE AND TILDE
					*p = 0x1EB4;	//	áº´	0xe1 0xba 0xb4	LATIN CAPITAL LETTER A WITH BREVE AND TILDE
					break;
				case 0x1EB7:		//	áº·	0xe1 0xba 0xb7	LATIN SMALL LETTER A WITH BREVE AND DOT BELOW
					*p = 0x1EB6;	//	áº¶	0xe1 0xba 0xb6	LATIN CAPITAL LETTER A WITH BREVE AND DOT BELOW
					break;
				case 0x1EB9:		//	áº¹	0xe1 0xba 0xb9	LATIN SMALL LETTER E WITH DOT BELOW
					*p = 0x1EB8;	//	áº¸	0xe1 0xba 0xb8	LATIN CAPITAL LETTER E WITH DOT BELOW
					break;
				case 0x1EBB:		//	áº»	0xe1 0xba 0xbb	LATIN SMALL LETTER E WITH HOOK ABOVE
					*p = 0x1EBA;	//	áºº	0xe1 0xba 0xba	LATIN CAPITAL LETTER E WITH HOOK ABOVE
					break;
				case 0x1EBD:		//	áº½	0xe1 0xba 0xbd	LATIN SMALL LETTER E WITH TILDE
					*p = 0x1EBC;	//	áº¼	0xe1 0xba 0xbc	LATIN CAPITAL LETTER E WITH TILDE
					break;
				case 0x1EBF:		//	áº¿	0xe1 0xba 0xbf	LATIN SMALL LETTER E WITH CIRCUMFLEX AND ACUTE
					*p = 0x1EBE;	//	áº¾	0xe1 0xba 0xbe	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1EC1:		//	á»	0xe1 0xbb 0x81	LATIN SMALL LETTER E WITH CIRCUMFLEX AND GRAVE
					*p = 0x1EC0;	//	á»€	0xe1 0xbb 0x80	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1EC3:		//	á»ƒ	0xe1 0xbb 0x83	LATIN SMALL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1EC2;	//	á»‚	0xe1 0xbb 0x82	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1EC5:		//	á»…	0xe1 0xbb 0x85	LATIN SMALL LETTER E WITH CIRCUMFLEX AND TILDE
					*p = 0x1EC4;	//	á»„	0xe1 0xbb 0x84	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1EC7:		//	á»‡	0xe1 0xbb 0x87	LATIN SMALL LETTER E WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1EC6;	//	á»†	0xe1 0xbb 0x86	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EC9:		//	á»‰	0xe1 0xbb 0x89	LATIN SMALL LETTER I WITH HOOK ABOVE
					*p = 0x1EC8;	//	á»ˆ	0xe1 0xbb 0x88	LATIN CAPITAL LETTER I WITH HOOK ABOVE
					break;
				case 0x1ECB:		//	á»‹	0xe1 0xbb 0x8b	LATIN SMALL LETTER I WITH DOT BELOW
					*p = 0x1ECA;	//	á»Š	0xe1 0xbb 0x8a	LATIN CAPITAL LETTER I WITH DOT BELOW
					break;
				case 0x1ECD:		//	á»	0xe1 0xbb 0x8d	LATIN SMALL LETTER O WITH DOT BELOW
					*p = 0x1ECC;	//	á»Œ	0xe1 0xbb 0x8c	LATIN CAPITAL LETTER O WITH DOT BELOW
					break;
				case 0x1ECF:		//	á»	0xe1 0xbb 0x8f	LATIN SMALL LETTER O WITH HOOK ABOVE
					*p = 0x1ECE;	//	á»Ž	0xe1 0xbb 0x8e	LATIN CAPITAL LETTER O WITH HOOK ABOVE
					break;
				case 0x1ED1:		//	á»‘	0xe1 0xbb 0x91	LATIN SMALL LETTER O WITH CIRCUMFLEX AND ACUTE
					*p = 0x1ED0;	//	á»	0xe1 0xbb 0x90	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1ED3:		//	á»“	0xe1 0xbb 0x93	LATIN SMALL LETTER O WITH CIRCUMFLEX AND GRAVE
					*p = 0x1ED2;	//	á»’	0xe1 0xbb 0x92	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1ED5:		//	á»•	0xe1 0xbb 0x95	LATIN SMALL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1ED4;	//	á»”	0xe1 0xbb 0x94	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1ED7:		//	á»—	0xe1 0xbb 0x97	LATIN SMALL LETTER O WITH CIRCUMFLEX AND TILDE
					*p = 0x1ED6;	//	á»–	0xe1 0xbb 0x96	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1ED9:		//	á»™	0xe1 0xbb 0x99	LATIN SMALL LETTER O WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1ED8;	//	á»˜	0xe1 0xbb 0x98	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EDB:		//	á»›	0xe1 0xbb 0x9b	LATIN SMALL LETTER O WITH HORN AND ACUTE
					*p = 0x1EDA;	//	á»š	0xe1 0xbb 0x9a	LATIN CAPITAL LETTER O WITH HORN AND ACUTE
					break;
				case 0x1EDD:		//	á»	0xe1 0xbb 0x9d	LATIN SMALL LETTER O WITH HORN AND GRAVE
					*p = 0x1EDC;	//	á»œ	0xe1 0xbb 0x9c	LATIN CAPITAL LETTER O WITH HORN AND GRAVE
					break;
				case 0x1EDF:		//	á»Ÿ	0xe1 0xbb 0x9f	LATIN SMALL LETTER O WITH HORN AND HOOK ABOVE
					*p = 0x1EDE;	//	á»ž	0xe1 0xbb 0x9e	LATIN CAPITAL LETTER O WITH HORN AND HOOK ABOVE
					break;
				case 0x1EE1:		//	á»¡	0xe1 0xbb 0xa1	LATIN SMALL LETTER O WITH HORN AND TILDE
					*p = 0x1EE0;	//	á» 	0xe1 0xbb 0xa0	LATIN CAPITAL LETTER O WITH HORN AND TILDE
					break;
				case 0x1EE3:		//	á»£	0xe1 0xbb 0xa3	LATIN SMALL LETTER O WITH HORN AND DOT BELOW
					*p = 0x1EE2;	//	á»¢	0xe1 0xbb 0xa2	LATIN CAPITAL LETTER O WITH HORN AND DOT BELOW
					break;
				case 0x1EE5:		//	á»¥	0xe1 0xbb 0xa5	LATIN SMALL LETTER U WITH DOT BELOW
					*p = 0x1EE4;	//	á»¤	0xe1 0xbb 0xa4	LATIN CAPITAL LETTER U WITH DOT BELOW
					break;
				case 0x1EE7:		//	á»§	0xe1 0xbb 0xa7	LATIN SMALL LETTER U WITH HOOK ABOVE
					*p = 0x1EE6;	//	á»¦	0xe1 0xbb 0xa6	LATIN CAPITAL LETTER U WITH HOOK ABOVE
					break;
				case 0x1EE9:		//	á»©	0xe1 0xbb 0xa9	LATIN SMALL LETTER U WITH HORN AND ACUTE
					*p = 0x1EE8;	//	á»¨	0xe1 0xbb 0xa8	LATIN CAPITAL LETTER U WITH HORN AND ACUTE
					break;
				case 0x1EEB:		//	á»«	0xe1 0xbb 0xab	LATIN SMALL LETTER U WITH HORN AND GRAVE
					*p = 0x1EEA;	//	á»ª	0xe1 0xbb 0xaa	LATIN CAPITAL LETTER U WITH HORN AND GRAVE
					break;
				case 0x1EED:		//	á»­	0xe1 0xbb 0xad	LATIN SMALL LETTER U WITH HORN AND HOOK ABOVE
					*p = 0x1EEC;	//	á»¬	0xe1 0xbb 0xac	LATIN CAPITAL LETTER U WITH HORN AND HOOK ABOVE
					break;
				case 0x1EEF:		//	á»¯	0xe1 0xbb 0xaf	LATIN SMALL LETTER U WITH HORN AND TILDE
					*p = 0x1EEE;	//	á»®	0xe1 0xbb 0xae	LATIN CAPITAL LETTER U WITH HORN AND TILDE
					break;
				case 0x1EF1:		//	á»±	0xe1 0xbb 0xb1	LATIN SMALL LETTER U WITH HORN AND DOT BELOW
					*p = 0x1EF0;	//	á»°	0xe1 0xbb 0xb0	LATIN CAPITAL LETTER U WITH HORN AND DOT BELOW
					break;
				case 0x1EF3:		//	á»³	0xe1 0xbb 0xb3	LATIN SMALL LETTER Y WITH GRAVE
					*p = 0x1EF2;	//	á»²	0xe1 0xbb 0xb2	LATIN CAPITAL LETTER Y WITH GRAVE
					break;
				case 0x1EF5:		//	á»µ	0xe1 0xbb 0xb5	LATIN SMALL LETTER Y WITH DOT BELOW
					*p = 0x1EF4;	//	á»´	0xe1 0xbb 0xb4	LATIN CAPITAL LETTER Y WITH DOT BELOW
					break;
				case 0x1EF7:		//	á»·	0xe1 0xbb 0xb7	LATIN SMALL LETTER Y WITH HOOK ABOVE
					*p = 0x1EF6;	//	á»¶	0xe1 0xbb 0xb6	LATIN CAPITAL LETTER Y WITH HOOK ABOVE
					break;
				case 0x1EF9:		//	á»¹	0xe1 0xbb 0xb9	LATIN SMALL LETTER Y WITH TILDE
					*p = 0x1EF8;	//	á»¸	0xe1 0xbb 0xb8	LATIN CAPITAL LETTER Y WITH TILDE
					break;
				case 0x1EFB:		//	á»»	0xe1 0xbb 0xbb	LATIN SMALL LETTER MIDDLE-WELSH LL
					*p = 0x1EFA;	//	á»º	0xe1 0xbb 0xba	LATIN CAPITAL LETTER MIDDLE-WELSH LL
					break;
				case 0x1EFD:		//	á»½	0xe1 0xbb 0xbd	LATIN SMALL LETTER MIDDLE-WELSH V
					*p = 0x1EFC;	//	á»¼	0xe1 0xbb 0xbc	LATIN CAPITAL LETTER MIDDLE-WELSH V
					break;
				case 0x1EFF:		//	á»¿	0xe1 0xbb 0xbf	LATIN SMALL LETTER Y WITH LOOP
					*p = 0x1EFE;	//	á»¾	0xe1 0xbb 0xbe	LATIN CAPITAL LETTER Y WITH LOOP
					break;
				case 0x1F00:		//	á¼€	0xe1 0xbc 0x80	GREEK SMALL LETTER ALPHA WITH PSILI
					*p = 0x1F08;	//	á¼ˆ	0xe1 0xbc 0x88	GREEK CAPITAL LETTER ALPHA WITH PSILI
					break;
				case 0x1F01:		//	á¼	0xe1 0xbc 0x81	GREEK SMALL LETTER ALPHA WITH DASIA
					*p = 0x1F09;	//	á¼‰	0xe1 0xbc 0x89	GREEK CAPITAL LETTER ALPHA WITH DASIA
					break;
				case 0x1F02:		//	á¼‚	0xe1 0xbc 0x82	GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA
					*p = 0x1F0A;	//	á¼Š	0xe1 0xbc 0x8a	GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA
					break;
				case 0x1F03:		//	á¼ƒ	0xe1 0xbc 0x83	GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA
					*p = 0x1F0B;	//	á¼‹	0xe1 0xbc 0x8b	GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA
					break;
				case 0x1F04:		//	á¼„	0xe1 0xbc 0x84	GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA
					*p = 0x1F0C;	//	á¼Œ	0xe1 0xbc 0x8c	GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA
					break;
				case 0x1F05:		//	á¼…	0xe1 0xbc 0x85	GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA
					*p = 0x1F0D;	//	á¼	0xe1 0xbc 0x8d	GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA
					break;
				case 0x1F06:		//	á¼†	0xe1 0xbc 0x86	GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI
					*p = 0x1F0E;	//	á¼Ž	0xe1 0xbc 0x8e	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F07:		//	á¼‡	0xe1 0xbc 0x87	GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI
					*p = 0x1F0F;	//	á¼	0xe1 0xbc 0x8f	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F10:		//	á¼	0xe1 0xbc 0x90	GREEK SMALL LETTER EPSILON WITH PSILI
					*p = 0x1F18;	//	á¼˜	0xe1 0xbc 0x98	GREEK CAPITAL LETTER EPSILON WITH PSILI
					break;
				case 0x1F11:		//	á¼‘	0xe1 0xbc 0x91	GREEK SMALL LETTER EPSILON WITH DASIA
					*p = 0x1F19;	//	á¼™	0xe1 0xbc 0x99	GREEK CAPITAL LETTER EPSILON WITH DASIA
					break;
				case 0x1F12:		//	á¼’	0xe1 0xbc 0x92	GREEK SMALL LETTER EPSILON WITH PSILI AND VARIA
					*p = 0x1F1A;	//	á¼š	0xe1 0xbc 0x9a	GREEK CAPITAL LETTER EPSILON WITH PSILI AND VARIA
					break;
				case 0x1F13:		//	á¼“	0xe1 0xbc 0x93	GREEK SMALL LETTER EPSILON WITH DASIA AND VARIA
					*p = 0x1F1B;	//	á¼›	0xe1 0xbc 0x9b	GREEK CAPITAL LETTER EPSILON WITH DASIA AND VARIA
					break;
				case 0x1F14:		//	á¼”	0xe1 0xbc 0x94	GREEK SMALL LETTER EPSILON WITH PSILI AND OXIA
					*p = 0x1F1C;	//	á¼œ	0xe1 0xbc 0x9c	GREEK CAPITAL LETTER EPSILON WITH PSILI AND OXIA
					break;
				case 0x1F15:		//	á¼•	0xe1 0xbc 0x95	GREEK SMALL LETTER EPSILON WITH DASIA AND OXIA
					*p = 0x1F1D;	//	á¼	0xe1 0xbc 0x9d	GREEK CAPITAL LETTER EPSILON WITH DASIA AND OXIA
					break;
				case 0x1F20:		//	á¼ 	0xe1 0xbc 0xa0	GREEK SMALL LETTER ETA WITH PSILI
					*p = 0x1F28;	//	á¼¨	0xe1 0xbc 0xa8	GREEK CAPITAL LETTER ETA WITH PSILI
					break;
				case 0x1F21:		//	á¼¡	0xe1 0xbc 0xa1	GREEK SMALL LETTER ETA WITH DASIA
					*p = 0x1F29;	//	á¼©	0xe1 0xbc 0xa9	GREEK CAPITAL LETTER ETA WITH DASIA
					break;
				case 0x1F22:		//	á¼¢	0xe1 0xbc 0xa2	GREEK SMALL LETTER ETA WITH PSILI AND VARIA
					*p = 0x1F2A;	//	á¼ª	0xe1 0xbc 0xaa	GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA
					break;
				case 0x1F23:		//	á¼£	0xe1 0xbc 0xa3	GREEK SMALL LETTER ETA WITH DASIA AND VARIA
					*p = 0x1F2B;	//	á¼«	0xe1 0xbc 0xab	GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA
					break;
				case 0x1F24:		//	á¼¤	0xe1 0xbc 0xa4	GREEK SMALL LETTER ETA WITH PSILI AND OXIA
					*p = 0x1F2C;	//	á¼¬	0xe1 0xbc 0xac	GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA
					break;
				case 0x1F25:		//	á¼¥	0xe1 0xbc 0xa5	GREEK SMALL LETTER ETA WITH DASIA AND OXIA
					*p = 0x1F2D;	//	á¼­	0xe1 0xbc 0xad	GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA
					break;
				case 0x1F26:		//	á¼¦	0xe1 0xbc 0xa6	GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI
					*p = 0x1F2E;	//	á¼®	0xe1 0xbc 0xae	GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F27:		//	á¼§	0xe1 0xbc 0xa7	GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI
					*p = 0x1F2F;	//	á¼¯	0xe1 0xbc 0xaf	GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F30:		//	á¼°	0xe1 0xbc 0xb0	GREEK SMALL LETTER IOTA WITH PSILI
					*p = 0x1F38;	//	á¼¸	0xe1 0xbc 0xb8	GREEK CAPITAL LETTER IOTA WITH PSILI
					break;
				case 0x1F31:		//	á¼±	0xe1 0xbc 0xb1	GREEK SMALL LETTER IOTA WITH DASIA
					*p = 0x1F39;	//	á¼¹	0xe1 0xbc 0xb9	GREEK CAPITAL LETTER IOTA WITH DASIA
					break;
				case 0x1F32:		//	á¼²	0xe1 0xbc 0xb2	GREEK SMALL LETTER IOTA WITH PSILI AND VARIA
					*p = 0x1F3A;	//	á¼º	0xe1 0xbc 0xba	GREEK CAPITAL LETTER IOTA WITH PSILI AND VARIA
					break;
				case 0x1F33:		//	á¼³	0xe1 0xbc 0xb3	GREEK SMALL LETTER IOTA WITH DASIA AND VARIA
					*p = 0x1F3B;	//	á¼»	0xe1 0xbc 0xbb	GREEK CAPITAL LETTER IOTA WITH DASIA AND VARIA
					break;
				case 0x1F34:		//	á¼´	0xe1 0xbc 0xb4	GREEK SMALL LETTER IOTA WITH PSILI AND OXIA
					*p = 0x1F3C;	//	á¼¼	0xe1 0xbc 0xbc	GREEK CAPITAL LETTER IOTA WITH PSILI AND OXIA
					break;
				case 0x1F35:		//	á¼µ	0xe1 0xbc 0xb5	GREEK SMALL LETTER IOTA WITH DASIA AND OXIA
					*p = 0x1F3D;	//	á¼½	0xe1 0xbc 0xbd	GREEK CAPITAL LETTER IOTA WITH DASIA AND OXIA
					break;
				case 0x1F36:		//	á¼¶	0xe1 0xbc 0xb6	GREEK SMALL LETTER IOTA WITH PSILI AND PERISPOMENI
					*p = 0x1F3E;	//	á¼¾	0xe1 0xbc 0xbe	GREEK CAPITAL LETTER IOTA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F37:		//	á¼·	0xe1 0xbc 0xb7	GREEK SMALL LETTER IOTA WITH DASIA AND PERISPOMENI
					*p = 0x1F3F;	//	á¼¿	0xe1 0xbc 0xbf	GREEK CAPITAL LETTER IOTA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F40:		//	á½€	0xe1 0xbd 0x80	GREEK SMALL LETTER OMICRON WITH PSILI
					*p = 0x1F48;	//	á½ˆ	0xe1 0xbd 0x88	GREEK CAPITAL LETTER OMICRON WITH PSILI
					break;
				case 0x1F41:		//	á½	0xe1 0xbd 0x81	GREEK SMALL LETTER OMICRON WITH DASIA
					*p = 0x1F49;	//	á½‰	0xe1 0xbd 0x89	GREEK CAPITAL LETTER OMICRON WITH DASIA
					break;
				case 0x1F42:		//	á½‚	0xe1 0xbd 0x82	GREEK SMALL LETTER OMICRON WITH PSILI AND VARIA
					*p = 0x1F4A;	//	á½Š	0xe1 0xbd 0x8a	GREEK CAPITAL LETTER OMICRON WITH PSILI AND VARIA
					break;
				case 0x1F43:		//	á½ƒ	0xe1 0xbd 0x83	GREEK SMALL LETTER OMICRON WITH DASIA AND VARIA
					*p = 0x1F4B;	//	á½‹	0xe1 0xbd 0x8b	GREEK CAPITAL LETTER OMICRON WITH DASIA AND VARIA
					break;
				case 0x1F44:		//	á½„	0xe1 0xbd 0x84	GREEK SMALL LETTER OMICRON WITH PSILI AND OXIA
					*p = 0x1F4C;	//	á½Œ	0xe1 0xbd 0x8c	GREEK CAPITAL LETTER OMICRON WITH PSILI AND OXIA
					break;
				case 0x1F45:		//	á½…	0xe1 0xbd 0x85	GREEK SMALL LETTER OMICRON WITH DASIA AND OXIA
					*p = 0x1F4D;	//	á½	0xe1 0xbd 0x8d	GREEK CAPITAL LETTER OMICRON WITH DASIA AND OXIA
					break;
				case 0x1F51:		//	á½‘	0xe1 0xbd 0x91	GREEK SMALL LETTER UPSILON WITH DASIA
					*p = 0x1F59;	//	á½™	0xe1 0xbd 0x99	GREEK CAPITAL LETTER UPSILON WITH DASIA
					break;
				case 0x1F53:		//	á½“	0xe1 0xbd 0x93	GREEK SMALL LETTER UPSILON WITH DASIA AND VARIA
					*p = 0x1F5B;	//	á½›	0xe1 0xbd 0x9b	GREEK CAPITAL LETTER UPSILON WITH DASIA AND VARIA
					break;
				case 0x1F55:		//	á½•	0xe1 0xbd 0x95	GREEK SMALL LETTER UPSILON WITH DASIA AND OXIA
					*p = 0x1F5D;	//	á½	0xe1 0xbd 0x9d	GREEK CAPITAL LETTER UPSILON WITH DASIA AND OXIA
					break;
				case 0x1F57:		//	á½—	0xe1 0xbd 0x97	GREEK SMALL LETTER UPSILON WITH DASIA AND PERISPOMENI
					*p = 0x1F5F;	//	á½Ÿ	0xe1 0xbd 0x9f	GREEK CAPITAL LETTER UPSILON WITH DASIA AND PERISPOMENI
					break;
				case 0x1F60:		//	á½ 	0xe1 0xbd 0xa0	GREEK SMALL LETTER OMEGA WITH PSILI
					*p = 0x1F68;	//	á½¨	0xe1 0xbd 0xa8	GREEK CAPITAL LETTER OMEGA WITH PSILI
					break;
				case 0x1F61:		//	á½¡	0xe1 0xbd 0xa1	GREEK SMALL LETTER OMEGA WITH DASIA
					*p = 0x1F69;	//	á½©	0xe1 0xbd 0xa9	GREEK CAPITAL LETTER OMEGA WITH DASIA
					break;
				case 0x1F62:		//	á½¢	0xe1 0xbd 0xa2	GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA
					*p = 0x1F6A;	//	á½ª	0xe1 0xbd 0xaa	GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA
					break;
				case 0x1F63:		//	á½£	0xe1 0xbd 0xa3	GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA
					*p = 0x1F6B;	//	á½«	0xe1 0xbd 0xab	GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA
					break;
				case 0x1F64:		//	á½¤	0xe1 0xbd 0xa4	GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA
					*p = 0x1F6C;	//	á½¬	0xe1 0xbd 0xac	GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA
					break;
				case 0x1F65:		//	á½¥	0xe1 0xbd 0xa5	GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA
					*p = 0x1F6D;	//	á½­	0xe1 0xbd 0xad	GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA
					break;
				case 0x1F66:		//	á½¦	0xe1 0xbd 0xa6	GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI
					*p = 0x1F6E;	//	á½®	0xe1 0xbd 0xae	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F67:		//	á½§	0xe1 0xbd 0xa7	GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI
					*p = 0x1F6F;	//	á½¯	0xe1 0xbd 0xaf	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F70:		//	á½°	0xe1 0xbd 0xb0	GREEK SMALL LETTER ALPHA WITH VARIA
					*p = 0x1FBA;	//	á¾º	0xe1 0xbe 0xba	GREEK CAPITAL LETTER ALPHA WITH VARIA
					break;
				case 0x1F71:		//	á½±	0xe1 0xbd 0xb1	GREEK SMALL LETTER ALPHA WITH OXIA
					*p = 0x1FBB;	//	á¾»	0xe1 0xbe 0xbb	GREEK CAPITAL LETTER ALPHA WITH OXIA
					break;
				case 0x1F72:		//	á½²	0xe1 0xbd 0xb2	GREEK SMALL LETTER EPSILON WITH VARIA
					*p = 0x1FC8;	//	á¿ˆ	0xe1 0xbf 0x88	GREEK CAPITAL LETTER EPSILON WITH VARIA
					break;
				case 0x1F73:		//	á½³	0xe1 0xbd 0xb3	GREEK SMALL LETTER EPSILON WITH OXIA
					*p = 0x1FC9;	//	á¿‰	0xe1 0xbf 0x89	GREEK CAPITAL LETTER EPSILON WITH OXIA
					break;
				case 0x1F74:		//	á½´	0xe1 0xbd 0xb4	GREEK SMALL LETTER ETA WITH VARIA
					*p = 0x1FCA;	//	á¿Š	0xe1 0xbf 0x8a	GREEK CAPITAL LETTER ETA WITH VARIA
					break;
				case 0x1F75:		//	á½µ	0xe1 0xbd 0xb5	GREEK SMALL LETTER ETA WITH OXIA
					*p = 0x1FCB;	//	á¿‹	0xe1 0xbf 0x8b	GREEK CAPITAL LETTER ETA WITH OXIA
					break;
				case 0x1F76:		//	á½¶	0xe1 0xbd 0xb6	GREEK SMALL LETTER IOTA WITH VARIA
					*p = 0x1FDA;	//	á¿š	0xe1 0xbf 0x9a	GREEK CAPITAL LETTER IOTA WITH VARIA
					break;
				case 0x1F77:		//	á½·	0xe1 0xbd 0xb7	GREEK SMALL LETTER IOTA WITH OXIA
					*p = 0x1FDB;	//	á¿›	0xe1 0xbf 0x9b	GREEK CAPITAL LETTER IOTA WITH OXIA
					break;
				case 0x1F78:		//	á½¸	0xe1 0xbd 0xb8	GREEK SMALL LETTER OMICRON WITH VARIA
					*p = 0x1FF8;	//	á¿¸	0xe1 0xbf 0xb8	GREEK CAPITAL LETTER OMICRON WITH VARIA
					break;
				case 0x1F79:		//	á½¹	0xe1 0xbd 0xb9	GREEK SMALL LETTER OMICRON WITH OXIA
					*p = 0x1FF9;	//	á¿¹	0xe1 0xbf 0xb9	GREEK CAPITAL LETTER OMICRON WITH OXIA
					break;
				case 0x1F7A:		//	á½º	0xe1 0xbd 0xba	GREEK SMALL LETTER UPSILON WITH VARIA
					*p = 0x1FEA;	//	á¿ª	0xe1 0xbf 0xaa	GREEK CAPITAL LETTER UPSILON WITH VARIA
					break;
				case 0x1F7B:		//	á½»	0xe1 0xbd 0xbb	GREEK SMALL LETTER UPSILON WITH OXIA
					*p = 0x1FEB;	//	á¿«	0xe1 0xbf 0xab	GREEK CAPITAL LETTER UPSILON WITH OXIA
					break;
				case 0x1F7C:		//	á½¼	0xe1 0xbd 0xbc	GREEK SMALL LETTER OMEGA WITH VARIA
					*p = 0x1FFA;	//	á¿º	0xe1 0xbf 0xba	GREEK CAPITAL LETTER OMEGA WITH VARIA
					break;
				case 0x1F7D:		//	á½½	0xe1 0xbd 0xbd	GREEK SMALL LETTER OMEGA WITH OXIA
					*p = 0x1FFB;	//	á¿»	0xe1 0xbf 0xbb	GREEK CAPITAL LETTER OMEGA WITH OXIA
					break;
				case 0x1F80:		//	á¾€	0xe1 0xbe 0x80	GREEK SMALL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1F88;	//	á¾ˆ	0xe1 0xbe 0x88	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1F81:		//	á¾	0xe1 0xbe 0x81	GREEK SMALL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1F89;	//	á¾‰	0xe1 0xbe 0x89	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1F82:		//	á¾‚	0xe1 0xbe 0x82	GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F8A;	//	á¾Š	0xe1 0xbe 0x8a	GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F83:		//	á¾ƒ	0xe1 0xbe 0x83	GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F8B;	//	á¾‹	0xe1 0xbe 0x8b	GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F84:		//	á¾„	0xe1 0xbe 0x84	GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F8C;	//	á¾Œ	0xe1 0xbe 0x8c	GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F85:		//	á¾…	0xe1 0xbe 0x85	GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F8D;	//	á¾	0xe1 0xbe 0x8d	GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F86:		//	á¾†	0xe1 0xbe 0x86	GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F8E;	//	á¾Ž	0xe1 0xbe 0x8e	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F87:		//	á¾‡	0xe1 0xbe 0x87	GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F8F;	//	á¾	0xe1 0xbe 0x8f	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F90:		//	á¾	0xe1 0xbe 0x90	GREEK SMALL LETTER ETA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1F98;	//	á¾˜	0xe1 0xbe 0x98	GREEK CAPITAL LETTER ETA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1F91:		//	á¾‘	0xe1 0xbe 0x91	GREEK SMALL LETTER ETA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1F99;	//	á¾™	0xe1 0xbe 0x99	GREEK CAPITAL LETTER ETA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1F92:		//	á¾’	0xe1 0xbe 0x92	GREEK SMALL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F9A;	//	á¾š	0xe1 0xbe 0x9a	GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F93:		//	á¾“	0xe1 0xbe 0x93	GREEK SMALL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F9B;	//	á¾›	0xe1 0xbe 0x9b	GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F94:		//	á¾”	0xe1 0xbe 0x94	GREEK SMALL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F9C;	//	á¾œ	0xe1 0xbe 0x9c	GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F95:		//	á¾•	0xe1 0xbe 0x95	GREEK SMALL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F9D;	//	á¾	0xe1 0xbe 0x9d	GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F96:		//	á¾–	0xe1 0xbe 0x96	GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F9E;	//	á¾ž	0xe1 0xbe 0x9e	GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F97:		//	á¾—	0xe1 0xbe 0x97	GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F9F;	//	á¾Ÿ	0xe1 0xbe 0x9f	GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FA0:		//	á¾ 	0xe1 0xbe 0xa0	GREEK SMALL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1FA8;	//	á¾¨	0xe1 0xbe 0xa8	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1FA1:		//	á¾¡	0xe1 0xbe 0xa1	GREEK SMALL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1FA9;	//	á¾©	0xe1 0xbe 0xa9	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1FA2:		//	á¾¢	0xe1 0xbe 0xa2	GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1FAA;	//	á¾ª	0xe1 0xbe 0xaa	GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1FA3:		//	á¾£	0xe1 0xbe 0xa3	GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1FAB;	//	á¾«	0xe1 0xbe 0xab	GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1FA4:		//	á¾¤	0xe1 0xbe 0xa4	GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1FAC;	//	á¾¬	0xe1 0xbe 0xac	GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1FA5:		//	á¾¥	0xe1 0xbe 0xa5	GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1FAD;	//	á¾­	0xe1 0xbe 0xad	GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1FA6:		//	á¾¦	0xe1 0xbe 0xa6	GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1FAE;	//	á¾®	0xe1 0xbe 0xae	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FA7:		//	á¾§	0xe1 0xbe 0xa7	GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1FAF;	//	á¾¯	0xe1 0xbe 0xaf	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FB0:		//	á¾°	0xe1 0xbe 0xb0	GREEK SMALL LETTER ALPHA WITH VRACHY
					*p = 0x1FB8;	//	á¾¸	0xe1 0xbe 0xb8	GREEK CAPITAL LETTER ALPHA WITH VRACHY
					break;
				case 0x1FB1:		//	á¾±	0xe1 0xbe 0xb1	GREEK SMALL LETTER ALPHA WITH MACRON
					*p = 0x1FB9;	//	á¾¹	0xe1 0xbe 0xb9	GREEK CAPITAL LETTER ALPHA WITH MACRON
					break;
				case 0x1FB3:		//	á¾³	0xe1 0xbe 0xb3	GREEK SMALL LETTER ALPHA WITH PROSGEGRAMMENI
					*p = 0x1FBC;	//	á¾¼	0xe1 0xbe 0xbc	GREEK CAPITAL LETTER ALPHA WITH PROSGEGRAMMENI
					break;
				case 0x1FC3:		//	á¿ƒ	0xe1 0xbf 0x83	GREEK SMALL LETTER ETA WITH PROSGEGRAMMENI
					*p = 0x1FCC;	//	á¿Œ	0xe1 0xbf 0x8c	GREEK CAPITAL LETTER ETA WITH PROSGEGRAMMENI
					break;
				case 0x1FD0:		//	á¿	0xe1 0xbf 0x90	GREEK SMALL LETTER IOTA WITH VRACHY
					*p = 0x1FD8;	//	á¿˜	0xe1 0xbf 0x98	GREEK CAPITAL LETTER IOTA WITH VRACHY
					break;
				case 0x1FD1:		//	á¿‘	0xe1 0xbf 0x91	GREEK SMALL LETTER IOTA WITH MACRON
					*p = 0x1FD9;	//	á¿™	0xe1 0xbf 0x99	GREEK CAPITAL LETTER IOTA WITH MACRON
					break;
				case 0x1FE0:		//	á¿ 	0xe1 0xbf 0xa0	GREEK SMALL LETTER UPSILON WITH VRACHY
					*p = 0x1FE8;	//	á¿¨	0xe1 0xbf 0xa8	GREEK CAPITAL LETTER UPSILON WITH VRACHY
					break;
				case 0x1FE1:		//	á¿¡	0xe1 0xbf 0xa1	GREEK SMALL LETTER UPSILON WITH MACRON
					*p = 0x1FE9;	//	á¿©	0xe1 0xbf 0xa9	GREEK CAPITAL LETTER UPSILON WITH MACRON
					break;
				case 0x1FE5:		//	á¿¥	0xe1 0xbf 0xa5	GREEK SMALL LETTER RHO WITH DASIA
					*p = 0x1FEC;	//	á¿¬	0xe1 0xbf 0xac	GREEK CAPITAL LETTER RHO WITH DASIA
					break;
				case 0x1FF3:		//	á¿³	0xe1 0xbf 0xb3	GREEK SMALL LETTER OMEGA WITH PROSGEGRAMMENI
					*p = 0x1FFC;	//	á¿¼	0xe1 0xbf 0xbc	GREEK CAPITAL LETTER OMEGA WITH PROSGEGRAMMENI
					break;
				case 0x2C30:		//	â°°	0xe2 0xb0 0xb0	GLAGOLITIC SMALL LETTER AZU
					*p = 0x2C00;	//	â°€	0xe2 0xb0 0x80	GLAGOLITIC CAPITAL LETTER AZU
					break;
				case 0x2C31:		//	â°±	0xe2 0xb0 0xb1	GLAGOLITIC SMALL LETTER BUKY
					*p = 0x2C01;	//	â°	0xe2 0xb0 0x81	GLAGOLITIC CAPITAL LETTER BUKY
					break;
				case 0x2C32:		//	â°²	0xe2 0xb0 0xb2	GLAGOLITIC SMALL LETTER VEDE
					*p = 0x2C02;	//	â°‚	0xe2 0xb0 0x82	GLAGOLITIC CAPITAL LETTER VEDE
					break;
				case 0x2C33:		//	â°³	0xe2 0xb0 0xb3	GLAGOLITIC SMALL LETTER GLAGOLI
					*p = 0x2C03;	//	â°ƒ	0xe2 0xb0 0x83	GLAGOLITIC CAPITAL LETTER GLAGOLI
					break;
				case 0x2C34:		//	â°´	0xe2 0xb0 0xb4	GLAGOLITIC SMALL LETTER DOBRO
					*p = 0x2C04;	//	â°„	0xe2 0xb0 0x84	GLAGOLITIC CAPITAL LETTER DOBRO
					break;
				case 0x2C35:		//	â°µ	0xe2 0xb0 0xb5	GLAGOLITIC SMALL LETTER YESTU
					*p = 0x2C05;	//	â°…	0xe2 0xb0 0x85	GLAGOLITIC CAPITAL LETTER YESTU
					break;
				case 0x2C36:		//	â°¶	0xe2 0xb0 0xb6	GLAGOLITIC SMALL LETTER ZHIVETE
					*p = 0x2C06;	//	â°†	0xe2 0xb0 0x86	GLAGOLITIC CAPITAL LETTER ZHIVETE
					break;
				case 0x2C37:		//	â°·	0xe2 0xb0 0xb7	GLAGOLITIC SMALL LETTER DZELO
					*p = 0x2C07;	//	â°‡	0xe2 0xb0 0x87	GLAGOLITIC CAPITAL LETTER DZELO
					break;
				case 0x2C38:		//	â°¸	0xe2 0xb0 0xb8	GLAGOLITIC SMALL LETTER ZEMLJA
					*p = 0x2C08;	//	â°ˆ	0xe2 0xb0 0x88	GLAGOLITIC CAPITAL LETTER ZEMLJA
					break;
				case 0x2C39:		//	â°¹	0xe2 0xb0 0xb9	GLAGOLITIC SMALL LETTER IZHE
					*p = 0x2C09;	//	â°‰	0xe2 0xb0 0x89	GLAGOLITIC CAPITAL LETTER IZHE
					break;
				case 0x2C3A:		//	â°º	0xe2 0xb0 0xba	GLAGOLITIC SMALL LETTER INITIAL IZHE
					*p = 0x2C0A;	//	â°Š	0xe2 0xb0 0x8a	GLAGOLITIC CAPITAL LETTER INITIAL IZHE
					break;
				case 0x2C3B:		//	â°»	0xe2 0xb0 0xbb	GLAGOLITIC SMALL LETTER I
					*p = 0x2C0B;	//	â°‹	0xe2 0xb0 0x8b	GLAGOLITIC CAPITAL LETTER I
					break;
				case 0x2C3C:		//	â°¼	0xe2 0xb0 0xbc	GLAGOLITIC SMALL LETTER DJERVI
					*p = 0x2C0C;	//	â°Œ	0xe2 0xb0 0x8c	GLAGOLITIC CAPITAL LETTER DJERVI
					break;
				case 0x2C3D:		//	â°½	0xe2 0xb0 0xbd	GLAGOLITIC SMALL LETTER KAKO
					*p = 0x2C0D;	//	â°	0xe2 0xb0 0x8d	GLAGOLITIC CAPITAL LETTER KAKO
					break;
				case 0x2C3E:		//	â°¾	0xe2 0xb0 0xbe	GLAGOLITIC SMALL LETTER LJUDIJE
					*p = 0x2C0E;	//	â°Ž	0xe2 0xb0 0x8e	GLAGOLITIC CAPITAL LETTER LJUDIJE
					break;
				case 0x2C3F:		//	â°¿	0xe2 0xb0 0xbf	GLAGOLITIC SMALL LETTER MYSLITE
					*p = 0x2C0F;	//	â°	0xe2 0xb0 0x8f	GLAGOLITIC CAPITAL LETTER MYSLITE
					break;
				case 0x2C40:		//	â±€	0xe2 0xb1 0x80	GLAGOLITIC SMALL LETTER NASHI
					*p = 0x2C10;	//	â°	0xe2 0xb0 0x90	GLAGOLITIC CAPITAL LETTER NASHI
					break;
				case 0x2C41:		//	â±	0xe2 0xb1 0x81	GLAGOLITIC SMALL LETTER ONU
					*p = 0x2C11;	//	â°‘	0xe2 0xb0 0x91	GLAGOLITIC CAPITAL LETTER ONU
					break;
				case 0x2C42:		//	â±‚	0xe2 0xb1 0x82	GLAGOLITIC SMALL LETTER POKOJI
					*p = 0x2C12;	//	â°’	0xe2 0xb0 0x92	GLAGOLITIC CAPITAL LETTER POKOJI
					break;
				case 0x2C43:		//	â±ƒ	0xe2 0xb1 0x83	GLAGOLITIC SMALL LETTER RITSI
					*p = 0x2C13;	//	â°“	0xe2 0xb0 0x93	GLAGOLITIC CAPITAL LETTER RITSI
					break;
				case 0x2C44:		//	â±„	0xe2 0xb1 0x84	GLAGOLITIC SMALL LETTER SLOVO
					*p = 0x2C14;	//	â°”	0xe2 0xb0 0x94	GLAGOLITIC CAPITAL LETTER SLOVO
					break;
				case 0x2C45:		//	â±…	0xe2 0xb1 0x85	GLAGOLITIC SMALL LETTER TVRIDO
					*p = 0x2C15;	//	â°•	0xe2 0xb0 0x95	GLAGOLITIC CAPITAL LETTER TVRIDO
					break;
				case 0x2C46:		//	â±†	0xe2 0xb1 0x86	GLAGOLITIC SMALL LETTER UKU
					*p = 0x2C16;	//	â°–	0xe2 0xb0 0x96	GLAGOLITIC CAPITAL LETTER UKU
					break;
				case 0x2C47:		//	â±‡	0xe2 0xb1 0x87	GLAGOLITIC SMALL LETTER FRITU
					*p = 0x2C17;	//	â°—	0xe2 0xb0 0x97	GLAGOLITIC CAPITAL LETTER FRITU
					break;
				case 0x2C48:		//	â±ˆ	0xe2 0xb1 0x88	GLAGOLITIC SMALL LETTER HERU
					*p = 0x2C18;	//	â°˜	0xe2 0xb0 0x98	GLAGOLITIC CAPITAL LETTER HERU
					break;
				case 0x2C49:		//	â±‰	0xe2 0xb1 0x89	GLAGOLITIC SMALL LETTER OTU
					*p = 0x2C19;	//	â°™	0xe2 0xb0 0x99	GLAGOLITIC CAPITAL LETTER OTU
					break;
				case 0x2C4A:		//	â±Š	0xe2 0xb1 0x8a	GLAGOLITIC SMALL LETTER PE
					*p = 0x2C1A;	//	â°š	0xe2 0xb0 0x9a	GLAGOLITIC CAPITAL LETTER PE
					break;
				case 0x2C4B:		//	â±‹	0xe2 0xb1 0x8b	GLAGOLITIC SMALL LETTER SHTA
					*p = 0x2C1B;	//	â°›	0xe2 0xb0 0x9b	GLAGOLITIC CAPITAL LETTER SHTA
					break;
				case 0x2C4C:		//	â±Œ	0xe2 0xb1 0x8c	GLAGOLITIC SMALL LETTER TSI
					*p = 0x2C1C;	//	â°œ	0xe2 0xb0 0x9c	GLAGOLITIC CAPITAL LETTER TSI
					break;
				case 0x2C4D:		//	â±	0xe2 0xb1 0x8d	GLAGOLITIC SMALL LETTER CHRIVI
					*p = 0x2C1D;	//	â°	0xe2 0xb0 0x9d	GLAGOLITIC CAPITAL LETTER CHRIVI
					break;
				case 0x2C4E:		//	â±Ž	0xe2 0xb1 0x8e	GLAGOLITIC SMALL LETTER SHA
					*p = 0x2C1E;	//	â°ž	0xe2 0xb0 0x9e	GLAGOLITIC CAPITAL LETTER SHA
					break;
				case 0x2C4F:		//	â±	0xe2 0xb1 0x8f	GLAGOLITIC SMALL LETTER YERU
					*p = 0x2C1F;	//	â°Ÿ	0xe2 0xb0 0x9f	GLAGOLITIC CAPITAL LETTER YERU
					break;
				case 0x2C50:		//	â±	0xe2 0xb1 0x90	GLAGOLITIC SMALL LETTER YERI
					*p = 0x2C20;	//	â° 	0xe2 0xb0 0xa0	GLAGOLITIC CAPITAL LETTER YERI
					break;
				case 0x2C51:		//	â±‘	0xe2 0xb1 0x91	GLAGOLITIC SMALL LETTER YATI
					*p = 0x2C21;	//	â°¡	0xe2 0xb0 0xa1	GLAGOLITIC CAPITAL LETTER YATI
					break;
				case 0x2C52:		//	â±’	0xe2 0xb1 0x92	GLAGOLITIC SMALL LETTER SPIDERY HA
					*p = 0x2C22;	//	â°¢	0xe2 0xb0 0xa2	GLAGOLITIC CAPITAL LETTER SPIDERY HA
					break;
				case 0x2C53:		//	â±“	0xe2 0xb1 0x93	GLAGOLITIC SMALL LETTER YU
					*p = 0x2C23;	//	â°£	0xe2 0xb0 0xa3	GLAGOLITIC CAPITAL LETTER YU
					break;
				case 0x2C54:		//	â±”	0xe2 0xb1 0x94	GLAGOLITIC SMALL LETTER SMALL YUS
					*p = 0x2C24;	//	â°¤	0xe2 0xb0 0xa4	GLAGOLITIC CAPITAL LETTER SMALL YUS
					break;
				case 0x2C55:		//	â±•	0xe2 0xb1 0x95	GLAGOLITIC SMALL LETTER SMALL YUS WITH TAIL
					*p = 0x2C25;	//	â°¥	0xe2 0xb0 0xa5	GLAGOLITIC CAPITAL LETTER SMALL YUS WITH TAIL
					break;
				case 0x2C56:		//	â±–	0xe2 0xb1 0x96	GLAGOLITIC SMALL LETTER YO
					*p = 0x2C26;	//	â°¦	0xe2 0xb0 0xa6	GLAGOLITIC CAPITAL LETTER YO
					break;
				case 0x2C57:		//	â±—	0xe2 0xb1 0x97	GLAGOLITIC SMALL LETTER IOTATED SMALL YUS
					*p = 0x2C27;	//	â°§	0xe2 0xb0 0xa7	GLAGOLITIC CAPITAL LETTER IOTATED SMALL YUS
					break;
				case 0x2C58:		//	â±˜	0xe2 0xb1 0x98	GLAGOLITIC SMALL LETTER BIG YUS
					*p = 0x2C28;	//	â°¨	0xe2 0xb0 0xa8	GLAGOLITIC CAPITAL LETTER BIG YUS
					break;
				case 0x2C59:		//	â±™	0xe2 0xb1 0x99	GLAGOLITIC SMALL LETTER IOTATED BIG YUS
					*p = 0x2C29;	//	â°©	0xe2 0xb0 0xa9	GLAGOLITIC CAPITAL LETTER IOTATED BIG YUS
					break;
				case 0x2C5A:		//	â±š	0xe2 0xb1 0x9a	GLAGOLITIC SMALL LETTER FITA
					*p = 0x2C2A;	//	â°ª	0xe2 0xb0 0xaa	GLAGOLITIC CAPITAL LETTER FITA
					break;
				case 0x2C5B:		//	â±›	0xe2 0xb1 0x9b	GLAGOLITIC SMALL LETTER IZHITSA
					*p = 0x2C2B;	//	â°«	0xe2 0xb0 0xab	GLAGOLITIC CAPITAL LETTER IZHITSA
					break;
				case 0x2C5C:		//	â±œ	0xe2 0xb1 0x9c	GLAGOLITIC SMALL LETTER SHTAPIC
					*p = 0x2C2C;	//	â°¬	0xe2 0xb0 0xac	GLAGOLITIC CAPITAL LETTER SHTAPIC
					break;
				case 0x2C5D:		//	â±	0xe2 0xb1 0x9d	GLAGOLITIC SMALL LETTER TROKUTASTI A
					*p = 0x2C2D;	//	â°­	0xe2 0xb0 0xad	GLAGOLITIC CAPITAL LETTER TROKUTASTI A
					break;
				case 0x2C5E:		//	â±ž	0xe2 0xb1 0x9e	GLAGOLITIC SMALL LETTER LATINATE MYSLITE
					*p = 0x2C2E;	//	â°®	0xe2 0xb0 0xae	GLAGOLITIC CAPITAL LETTER LATINATE MYSLITE
					break;
				case 0x2C61:		//	â±¡	0xe2 0xb1 0xa1	LATIN SMALL LETTER L WITH DOUBLE BAR
					*p = 0x2C60;	//	â± 	0xe2 0xb1 0xa0	LATIN CAPITAL LETTER L WITH DOUBLE BAR
					break;
				case 0x2C65:		//	â±¥	0xe2 0xb1 0xa5	LATIN SMALL LETTER A WITH STROKE
					*p = 0x023A;	//	Èº	0xc8 0xba	LATIN CAPITAL LETTER A WITH STROKE
					break;
				case 0x2C66:		//	â±¦	0xe2 0xb1 0xa6	LATIN SMALL LETTER T WITH DIAGONAL STROKE
					*p = 0x023E;	//	È¾	0xc8 0xbe	LATIN CAPITAL LETTER T WITH DIAGONAL STROKE
					break;
				case 0x2C68:		//	â±¨	0xe2 0xb1 0xa8	LATIN SMALL LETTER H WITH DESCENDER
					*p = 0x2C67;	//	â±§	0xe2 0xb1 0xa7	LATIN CAPITAL LETTER H WITH DESCENDER
					break;
				case 0x2C6A:		//	â±ª	0xe2 0xb1 0xaa	LATIN SMALL LETTER K WITH DESCENDER
					*p = 0x2C69;	//	â±©	0xe2 0xb1 0xa9	LATIN CAPITAL LETTER K WITH DESCENDER
					break;
				case 0x2C6C:		//	â±¬	0xe2 0xb1 0xac	LATIN SMALL LETTER Z WITH DESCENDER
					*p = 0x2C6B;	//	â±«	0xe2 0xb1 0xab	LATIN CAPITAL LETTER Z WITH DESCENDER
					break;
				case 0x2C73:		//	â±³	0xe2 0xb1 0xb3	LATIN SMALL LETTER W WITH HOOK
					*p = 0x2C72;	//	â±²	0xe2 0xb1 0xb2	LATIN CAPITAL LETTER W WITH HOOK
					break;
				case 0x2C76:		//	â±¶	0xe2 0xb1 0xb6	LATIN SMALL LETTER HALF H
					*p = 0x2C75;	//	â±µ	0xe2 0xb1 0xb5	LATIN CAPITAL LETTER HALF H
					break;
				case 0x2C81:		//	â²	0xe2 0xb2 0x81	COPTIC SMALL LETTER ALFA
					*p = 0x2C80;	//	â²€	0xe2 0xb2 0x80	COPTIC CAPITAL LETTER ALFA
					break;
				case 0x2C83:		//	â²ƒ	0xe2 0xb2 0x83	COPTIC SMALL LETTER VIDA
					*p = 0x2C82;	//	â²‚	0xe2 0xb2 0x82	COPTIC CAPITAL LETTER VIDA
					break;
				case 0x2C85:		//	â²…	0xe2 0xb2 0x85	COPTIC SMALL LETTER GAMMA
					*p = 0x2C84;	//	â²„	0xe2 0xb2 0x84	COPTIC CAPITAL LETTER GAMMA
					break;
				case 0x2C87:		//	â²‡	0xe2 0xb2 0x87	COPTIC SMALL LETTER DALDA
					*p = 0x2C86;	//	â²†	0xe2 0xb2 0x86	COPTIC CAPITAL LETTER DALDA
					break;
				case 0x2C89:		//	â²‰	0xe2 0xb2 0x89	COPTIC SMALL LETTER EIE
					*p = 0x2C88;	//	â²ˆ	0xe2 0xb2 0x88	COPTIC CAPITAL LETTER EIE
					break;
				case 0x2C8B:		//	â²‹	0xe2 0xb2 0x8b	COPTIC SMALL LETTER SOU
					*p = 0x2C8A;	//	â²Š	0xe2 0xb2 0x8a	COPTIC CAPITAL LETTER SOU
					break;
				case 0x2C8D:		//	â²	0xe2 0xb2 0x8d	COPTIC SMALL LETTER ZATA
					*p = 0x2C8C;	//	â²Œ	0xe2 0xb2 0x8c	COPTIC CAPITAL LETTER ZATA
					break;
				case 0x2C8F:		//	â²	0xe2 0xb2 0x8f	COPTIC SMALL LETTER HATE
					*p = 0x2C8E;	//	â²Ž	0xe2 0xb2 0x8e	COPTIC CAPITAL LETTER HATE
					break;
				case 0x2C91:		//	â²‘	0xe2 0xb2 0x91	COPTIC SMALL LETTER THETHE
					*p = 0x2C90;	//	â²	0xe2 0xb2 0x90	COPTIC CAPITAL LETTER THETHE
					break;
				case 0x2C93:		//	â²“	0xe2 0xb2 0x93	COPTIC SMALL LETTER IAUDA
					*p = 0x2C92;	//	â²’	0xe2 0xb2 0x92	COPTIC CAPITAL LETTER IAUDA
					break;
				case 0x2C95:		//	â²•	0xe2 0xb2 0x95	COPTIC SMALL LETTER KAPA
					*p = 0x2C94;	//	â²”	0xe2 0xb2 0x94	COPTIC CAPITAL LETTER KAPA
					break;
				case 0x2C97:		//	â²—	0xe2 0xb2 0x97	COPTIC SMALL LETTER LAULA
					*p = 0x2C96;	//	â²–	0xe2 0xb2 0x96	COPTIC CAPITAL LETTER LAULA
					break;
				case 0x2C99:		//	â²™	0xe2 0xb2 0x99	COPTIC SMALL LETTER MI
					*p = 0x2C98;	//	â²˜	0xe2 0xb2 0x98	COPTIC CAPITAL LETTER MI
					break;
				case 0x2C9B:		//	â²›	0xe2 0xb2 0x9b	COPTIC SMALL LETTER NI
					*p = 0x2C9A;	//	â²š	0xe2 0xb2 0x9a	COPTIC CAPITAL LETTER NI
					break;
				case 0x2C9D:		//	â²	0xe2 0xb2 0x9d	COPTIC SMALL LETTER KSI
					*p = 0x2C9C;	//	â²œ	0xe2 0xb2 0x9c	COPTIC CAPITAL LETTER KSI
					break;
				case 0x2C9F:		//	â²Ÿ	0xe2 0xb2 0x9f	COPTIC SMALL LETTER O
					*p = 0x2C9E;	//	â²ž	0xe2 0xb2 0x9e	COPTIC CAPITAL LETTER O
					break;
				case 0x2CA1:		//	â²¡	0xe2 0xb2 0xa1	COPTIC SMALL LETTER PI
					*p = 0x2CA0;	//	â² 	0xe2 0xb2 0xa0	COPTIC CAPITAL LETTER PI
					break;
				case 0x2CA3:		//	â²£	0xe2 0xb2 0xa3	COPTIC SMALL LETTER RO
					*p = 0x2CA2;	//	â²¢	0xe2 0xb2 0xa2	COPTIC CAPITAL LETTER RO
					break;
				case 0x2CA5:		//	â²¥	0xe2 0xb2 0xa5	COPTIC SMALL LETTER SIMA
					*p = 0x2CA4;	//	â²¤	0xe2 0xb2 0xa4	COPTIC CAPITAL LETTER SIMA
					break;
				case 0x2CA7:		//	â²§	0xe2 0xb2 0xa7	COPTIC SMALL LETTER TAU
					*p = 0x2CA6;	//	â²¦	0xe2 0xb2 0xa6	COPTIC CAPITAL LETTER TAU
					break;
				case 0x2CA9:		//	â²©	0xe2 0xb2 0xa9	COPTIC SMALL LETTER UA
					*p = 0x2CA8;	//	â²¨	0xe2 0xb2 0xa8	COPTIC CAPITAL LETTER UA
					break;
				case 0x2CAB:		//	â²«	0xe2 0xb2 0xab	COPTIC SMALL LETTER FI
					*p = 0x2CAA;	//	â²ª	0xe2 0xb2 0xaa	COPTIC CAPITAL LETTER FI
					break;
				case 0x2CAD:		//	â²­	0xe2 0xb2 0xad	COPTIC SMALL LETTER KHI
					*p = 0x2CAC;	//	â²¬	0xe2 0xb2 0xac	COPTIC CAPITAL LETTER KHI
					break;
				case 0x2CAF:		//	â²¯	0xe2 0xb2 0xaf	COPTIC SMALL LETTER PSI
					*p = 0x2CAE;	//	â²®	0xe2 0xb2 0xae	COPTIC CAPITAL LETTER PSI
					break;
				case 0x2CB1:		//	â²±	0xe2 0xb2 0xb1	COPTIC SMALL LETTER OOU
					*p = 0x2CB0;	//	â²°	0xe2 0xb2 0xb0	COPTIC CAPITAL LETTER OOU
					break;
				case 0x2CB3:		//	â²³	0xe2 0xb2 0xb3	COPTIC SMALL LETTER DIALECT-P ALEF
					*p = 0x2CB2;	//	â²²	0xe2 0xb2 0xb2	COPTIC CAPITAL LETTER DIALECT-P ALEF
					break;
				case 0x2CB5:		//	â²µ	0xe2 0xb2 0xb5	COPTIC SMALL LETTER OLD COPTIC AIN
					*p = 0x2CB4;	//	â²´	0xe2 0xb2 0xb4	COPTIC CAPITAL LETTER OLD COPTIC AIN
					break;
				case 0x2CB7:		//	â²·	0xe2 0xb2 0xb7	COPTIC SMALL LETTER CRYPTOGRAMMIC EIE
					*p = 0x2CB6;	//	â²¶	0xe2 0xb2 0xb6	COPTIC CAPITAL LETTER CRYPTOGRAMMIC EIE
					break;
				case 0x2CB9:		//	â²¹	0xe2 0xb2 0xb9	COPTIC SMALL LETTER DIALECT-P KAPA
					*p = 0x2CB8;	//	â²¸	0xe2 0xb2 0xb8	COPTIC CAPITAL LETTER DIALECT-P KAPA
					break;
				case 0x2CBB:		//	â²»	0xe2 0xb2 0xbb	COPTIC SMALL LETTER DIALECT-P NI
					*p = 0x2CBA;	//	â²º	0xe2 0xb2 0xba	COPTIC CAPITAL LETTER DIALECT-P NI
					break;
				case 0x2CBD:		//	â²½	0xe2 0xb2 0xbd	COPTIC SMALL LETTER CRYPTOGRAMMIC NI
					*p = 0x2CBC;	//	â²¼	0xe2 0xb2 0xbc	COPTIC CAPITAL LETTER CRYPTOGRAMMIC NI
					break;
				case 0x2CBF:		//	â²¿	0xe2 0xb2 0xbf	COPTIC SMALL LETTER OLD COPTIC OOU
					*p = 0x2CBE;	//	â²¾	0xe2 0xb2 0xbe	COPTIC CAPITAL LETTER OLD COPTIC OOU
					break;
				case 0x2CC1:		//	â³	0xe2 0xb3 0x81	COPTIC SMALL LETTER SAMPI
					*p = 0x2CC0;	//	â³€	0xe2 0xb3 0x80	COPTIC CAPITAL LETTER SAMPI
					break;
				case 0x2CC3:		//	â³ƒ	0xe2 0xb3 0x83	COPTIC SMALL LETTER CROSSED SHEI
					*p = 0x2CC2;	//	â³‚	0xe2 0xb3 0x82	COPTIC CAPITAL LETTER CROSSED SHEI
					break;
				case 0x2CC5:		//	â³…	0xe2 0xb3 0x85	COPTIC SMALL LETTER OLD COPTIC SHEI
					*p = 0x2CC4;	//	â³„	0xe2 0xb3 0x84	COPTIC CAPITAL LETTER OLD COPTIC SHEI
					break;
				case 0x2CC7:		//	â³‡	0xe2 0xb3 0x87	COPTIC SMALL LETTER OLD COPTIC ESH
					*p = 0x2CC6;	//	â³†	0xe2 0xb3 0x86	COPTIC CAPITAL LETTER OLD COPTIC ESH
					break;
				case 0x2CC9:		//	â³‰	0xe2 0xb3 0x89	COPTIC SMALL LETTER AKHMIMIC KHEI
					*p = 0x2CC8;	//	â³ˆ	0xe2 0xb3 0x88	COPTIC CAPITAL LETTER AKHMIMIC KHEI
					break;
				case 0x2CCB:		//	â³‹	0xe2 0xb3 0x8b	COPTIC SMALL LETTER DIALECT-P HORI
					*p = 0x2CCA;	//	â³Š	0xe2 0xb3 0x8a	COPTIC CAPITAL LETTER DIALECT-P HORI
					break;
				case 0x2CCD:		//	â³	0xe2 0xb3 0x8d	COPTIC SMALL LETTER OLD COPTIC HORI
					*p = 0x2CCC;	//	â³Œ	0xe2 0xb3 0x8c	COPTIC CAPITAL LETTER OLD COPTIC HORI
					break;
				case 0x2CCF:		//	â³	0xe2 0xb3 0x8f	COPTIC SMALL LETTER OLD COPTIC HA
					*p = 0x2CCE;	//	â³Ž	0xe2 0xb3 0x8e	COPTIC CAPITAL LETTER OLD COPTIC HA
					break;
				case 0x2CD1:		//	â³‘	0xe2 0xb3 0x91	COPTIC SMALL LETTER L-SHAPED HA
					*p = 0x2CD0;	//	â³	0xe2 0xb3 0x90	COPTIC CAPITAL LETTER L-SHAPED HA
					break;
				case 0x2CD3:		//	â³“	0xe2 0xb3 0x93	COPTIC SMALL LETTER OLD COPTIC HEI
					*p = 0x2CD2;	//	â³’	0xe2 0xb3 0x92	COPTIC CAPITAL LETTER OLD COPTIC HEI
					break;
				case 0x2CD5:		//	â³•	0xe2 0xb3 0x95	COPTIC SMALL LETTER OLD COPTIC HAT
					*p = 0x2CD4;	//	â³”	0xe2 0xb3 0x94	COPTIC CAPITAL LETTER OLD COPTIC HAT
					break;
				case 0x2CD7:		//	â³—	0xe2 0xb3 0x97	COPTIC SMALL LETTER OLD COPTIC GANGIA
					*p = 0x2CD6;	//	â³–	0xe2 0xb3 0x96	COPTIC CAPITAL LETTER OLD COPTIC GANGIA
					break;
				case 0x2CD9:		//	â³™	0xe2 0xb3 0x99	COPTIC SMALL LETTER OLD COPTIC DJA
					*p = 0x2CD8;	//	â³˜	0xe2 0xb3 0x98	COPTIC CAPITAL LETTER OLD COPTIC DJA
					break;
				case 0x2CDB:		//	â³›	0xe2 0xb3 0x9b	COPTIC SMALL LETTER OLD COPTIC SHIMA
					*p = 0x2CDA;	//	â³š	0xe2 0xb3 0x9a	COPTIC CAPITAL LETTER OLD COPTIC SHIMA
					break;
				case 0x2CDD:		//	â³	0xe2 0xb3 0x9d	COPTIC SMALL LETTER OLD NUBIAN SHIMA
					*p = 0x2CDC;	//	â³œ	0xe2 0xb3 0x9c	COPTIC CAPITAL LETTER OLD NUBIAN SHIMA
					break;
				case 0x2CDF:		//	â³Ÿ	0xe2 0xb3 0x9f	COPTIC SMALL LETTER OLD NUBIAN NGI
					*p = 0x2CDE;	//	â³ž	0xe2 0xb3 0x9e	COPTIC CAPITAL LETTER OLD NUBIAN NGI
					break;
				case 0x2CE1:		//	â³¡	0xe2 0xb3 0xa1	COPTIC SMALL LETTER OLD NUBIAN NYI
					*p = 0x2CE0;	//	â³ 	0xe2 0xb3 0xa0	COPTIC CAPITAL LETTER OLD NUBIAN NYI
					break;
				case 0x2CE3:		//	â³£	0xe2 0xb3 0xa3	COPTIC SMALL LETTER OLD NUBIAN WAU
					*p = 0x2CE2;	//	â³¢	0xe2 0xb3 0xa2	COPTIC CAPITAL LETTER OLD NUBIAN WAU
					break;
				case 0x2CEC:		//	â³¬	0xe2 0xb3 0xac	COPTIC SMALL LETTER CRYPTOGRAMMIC SHEI
					*p = 0x2CEB;	//	â³«	0xe2 0xb3 0xab	COPTIC CAPITAL LETTER CRYPTOGRAMMIC SHEI
					break;
				case 0x2CEE:		//	â³®	0xe2 0xb3 0xae	COPTIC SMALL LETTER CRYPTOGRAMMIC GANGIA
					*p = 0x2CED;	//	â³­	0xe2 0xb3 0xad	COPTIC CAPITAL LETTER CRYPTOGRAMMIC GANGIA
					break;
				case 0x2CF3:		//	â³³	0xe2 0xb3 0xb3	COPTIC SMALL LETTER BOHAIRIC KHEI
					*p = 0x2CF2;	//	â³²	0xe2 0xb3 0xb2	COPTIC CAPITAL LETTER BOHAIRIC KHEI
					break;
				case 0x2D00:		//	â´€	0xe2 0xb4 0x80	GEORGIAN LETTER AN
					*p = 0x1C90;	//	á²	0xe1 0xb2 0x90	GEORGIAN CAPITAL LETTER AN
					break;
				case 0x2D01:		//	â´	0xe2 0xb4 0x81	GEORGIAN LETTER BAN
					*p = 0x1C91;	//	á²‘	0xe1 0xb2 0x91	GEORGIAN CAPITAL LETTER BAN
					break;
				case 0x2D02:		//	â´‚	0xe2 0xb4 0x82	GEORGIAN LETTER GAN
					*p = 0x1C92;	//	á²’	0xe1 0xb2 0x92	GEORGIAN CAPITAL LETTER GAN
					break;
				case 0x2D03:		//	â´ƒ	0xe2 0xb4 0x83	GEORGIAN LETTER DON
					*p = 0x1C93;	//	á²“	0xe1 0xb2 0x93	GEORGIAN CAPITAL LETTER DON
					break;
				case 0x2D04:		//	â´„	0xe2 0xb4 0x84	GEORGIAN LETTER EN
					*p = 0x1C94;	//	á²”	0xe1 0xb2 0x94	GEORGIAN CAPITAL LETTER EN
					break;
				case 0x2D05:		//	â´…	0xe2 0xb4 0x85	GEORGIAN LETTER VIN
					*p = 0x1C95;	//	á²•	0xe1 0xb2 0x95	GEORGIAN CAPITAL LETTER VIN
					break;
				case 0x2D06:		//	â´†	0xe2 0xb4 0x86	GEORGIAN LETTER ZEN
					*p = 0x1C96;	//	á²–	0xe1 0xb2 0x96	GEORGIAN CAPITAL LETTER ZEN
					break;
				case 0x2D07:		//	â´‡	0xe2 0xb4 0x87	GEORGIAN LETTER TAN
					*p = 0x1C97;	//	á²—	0xe1 0xb2 0x97	GEORGIAN CAPITAL LETTER TAN
					break;
				case 0x2D08:		//	â´ˆ	0xe2 0xb4 0x88	GEORGIAN LETTER IN
					*p = 0x1C98;	//	á²˜	0xe1 0xb2 0x98	GEORGIAN CAPITAL LETTER IN
					break;
				case 0x2D09:		//	â´‰	0xe2 0xb4 0x89	GEORGIAN LETTER KAN
					*p = 0x1C99;	//	á²™	0xe1 0xb2 0x99	GEORGIAN CAPITAL LETTER KAN
					break;
				case 0x2D0A:		//	â´Š	0xe2 0xb4 0x8a	GEORGIAN LETTER LAS
					*p = 0x1C9A;	//	á²š	0xe1 0xb2 0x9a	GEORGIAN CAPITAL LETTER LAS
					break;
				case 0x2D0B:		//	â´‹	0xe2 0xb4 0x8b	GEORGIAN LETTER MAN
					*p = 0x1C9B;	//	á²›	0xe1 0xb2 0x9b	GEORGIAN CAPITAL LETTER MAN
					break;
				case 0x2D0C:		//	â´Œ	0xe2 0xb4 0x8c	GEORGIAN LETTER NAR
					*p = 0x1C9C;	//	á²œ	0xe1 0xb2 0x9c	GEORGIAN CAPITAL LETTER NAR
					break;
				case 0x2D0D:		//	â´	0xe2 0xb4 0x8d	GEORGIAN LETTER ON
					*p = 0x1C9D;	//	á²	0xe1 0xb2 0x9d	GEORGIAN CAPITAL LETTER ON
					break;
				case 0x2D0E:		//	â´Ž	0xe2 0xb4 0x8e	GEORGIAN LETTER PAR
					*p = 0x1C9E;	//	á²ž	0xe1 0xb2 0x9e	GEORGIAN CAPITAL LETTER PAR
					break;
				case 0x2D0F:		//	â´	0xe2 0xb4 0x8f	GEORGIAN LETTER ZHAR
					*p = 0x1C9F;	//	á²Ÿ	0xe1 0xb2 0x9f	GEORGIAN CAPITAL LETTER ZHAR
					break;
				case 0x2D10:		//	â´	0xe2 0xb4 0x90	GEORGIAN LETTER RAE
					*p = 0x1CA0;	//	á² 	0xe1 0xb2 0xa0	GEORGIAN CAPITAL LETTER RAE
					break;
				case 0x2D11:		//	â´‘	0xe2 0xb4 0x91	GEORGIAN LETTER SAN
					*p = 0x1CA1;	//	á²¡	0xe1 0xb2 0xa1	GEORGIAN CAPITAL LETTER SAN
					break;
				case 0x2D12:		//	â´’	0xe2 0xb4 0x92	GEORGIAN LETTER TAR
					*p = 0x1CA2;	//	á²¢	0xe1 0xb2 0xa2	GEORGIAN CAPITAL LETTER TAR
					break;
				case 0x2D13:		//	â´“	0xe2 0xb4 0x93	GEORGIAN LETTER UN
					*p = 0x1CA3;	//	á²£	0xe1 0xb2 0xa3	GEORGIAN CAPITAL LETTER UN
					break;
				case 0x2D14:		//	â´”	0xe2 0xb4 0x94	GEORGIAN LETTER PHAR
					*p = 0x1CA4;	//	á²¤	0xe1 0xb2 0xa4	GEORGIAN CAPITAL LETTER PHAR
					break;
				case 0x2D15:		//	â´•	0xe2 0xb4 0x95	GEORGIAN LETTER KHAR
					*p = 0x1CA5;	//	á²¥	0xe1 0xb2 0xa5	GEORGIAN CAPITAL LETTER KHAR
					break;
				case 0x2D16:		//	â´–	0xe2 0xb4 0x96	GEORGIAN LETTER GHAN
					*p = 0x1CA6;	//	á²¦	0xe1 0xb2 0xa6	GEORGIAN CAPITAL LETTER GHAN
					break;
				case 0x2D17:		//	â´—	0xe2 0xb4 0x97	GEORGIAN LETTER QAR
					*p = 0x1CA7;	//	á²§	0xe1 0xb2 0xa7	GEORGIAN CAPITAL LETTER QAR
					break;
				case 0x2D18:		//	â´˜	0xe2 0xb4 0x98	GEORGIAN LETTER SHIN
					*p = 0x1CA8;	//	á²¨	0xe1 0xb2 0xa8	GEORGIAN CAPITAL LETTER SHIN
					break;
				case 0x2D19:		//	â´™	0xe2 0xb4 0x99	GEORGIAN LETTER CHIN
					*p = 0x1CA9;	//	á²©	0xe1 0xb2 0xa9	GEORGIAN CAPITAL LETTER CHIN
					break;
				case 0x2D1A:		//	â´š	0xe2 0xb4 0x9a	GEORGIAN LETTER CAN
					*p = 0x1CAA;	//	á²ª	0xe1 0xb2 0xaa	GEORGIAN CAPITAL LETTER CAN
					break;
				case 0x2D1B:		//	â´›	0xe2 0xb4 0x9b	GEORGIAN LETTER JIL
					*p = 0x1CAB;	//	á²«	0xe1 0xb2 0xab	GEORGIAN CAPITAL LETTER JIL
					break;
				case 0x2D1C:		//	â´œ	0xe2 0xb4 0x9c	GEORGIAN LETTER CIL
					*p = 0x1CAC;	//	á²¬	0xe1 0xb2 0xac	GEORGIAN CAPITAL LETTER CIL
					break;
				case 0x2D1D:		//	â´	0xe2 0xb4 0x9d	GEORGIAN LETTER CHAR
					*p = 0x1CAD;	//	á²­	0xe1 0xb2 0xad	GEORGIAN CAPITAL LETTER CHAR
					break;
				case 0x2D1E:		//	â´ž	0xe2 0xb4 0x9e	GEORGIAN LETTER XAN
					*p = 0x1CAE;	//	á²®	0xe1 0xb2 0xae	GEORGIAN CAPITAL LETTER XAN
					break;
				case 0x2D1F:		//	â´Ÿ	0xe2 0xb4 0x9f	GEORGIAN LETTER JHAN
					*p = 0x1CAF;	//	á²¯	0xe1 0xb2 0xaf	GEORGIAN CAPITAL LETTER JHAN
					break;
				case 0x2D20:		//	â´ 	0xe2 0xb4 0xa0	GEORGIAN LETTER HAE
					*p = 0x1CB0;	//	á²°	0xe1 0xb2 0xb0	GEORGIAN CAPITAL LETTER HAE
					break;
				case 0x2D21:		//	â´¡	0xe2 0xb4 0xa1	GEORGIAN LETTER HE
					*p = 0x1CB1;	//	á²±	0xe1 0xb2 0xb1	GEORGIAN CAPITAL LETTER HE
					break;
				case 0x2D22:		//	â´¢	0xe2 0xb4 0xa2	GEORGIAN LETTER HIE
					*p = 0x1CB2;	//	á²²	0xe1 0xb2 0xb2	GEORGIAN CAPITAL LETTER HIE
					break;
				case 0x2D23:		//	â´£	0xe2 0xb4 0xa3	GEORGIAN LETTER WE
					*p = 0x1CB3;	//	á²³	0xe1 0xb2 0xb3	GEORGIAN CAPITAL LETTER WE
					break;
				case 0x2D24:		//	â´¤	0xe2 0xb4 0xa4	GEORGIAN LETTER HAR
					*p = 0x1CB4;	//	á²´	0xe1 0xb2 0xb4	GEORGIAN CAPITAL LETTER HAR
					break;
				case 0x2D25:		//	â´¥	0xe2 0xb4 0xa5	GEORGIAN LETTER HOE
					*p = 0x1CB5;	//	á²µ	0xe1 0xb2 0xb5	GEORGIAN CAPITAL LETTER HOE
					break;
				case 0x2D27:		//	â´§	0xe2 0xb4 0xa7	GEORGIAN LETTER YN
					*p = 0x1CB7;	//	á²·	0xe1 0xb2 0xb7	GEORGIAN CAPITAL LETTER YN
					break;
				case 0x2D2D:		//	â´­	0xe2 0xb4 0xad	GEORGIAN LETTER AEN
					*p = 0x1CBD;	//	á²½	0xe1 0xb2 0xbd	GEORGIAN CAPITAL LETTER AEN
					break;
				case 0xA641:		//	ê™	0xea 0x99 0x81	CYRILLIC SMALL LETTER ZEMLYA
					*p = 0xA640;	//	ê™€	0xea 0x99 0x80	CYRILLIC CAPITAL LETTER ZEMLYA
					break;
				case 0xA643:		//	ê™ƒ	0xea 0x99 0x83	CYRILLIC SMALL LETTER DZELO
					*p = 0xA642;	//	ê™‚	0xea 0x99 0x82	CYRILLIC CAPITAL LETTER DZELO
					break;
				case 0xA645:		//	ê™…	0xea 0x99 0x85	CYRILLIC SMALL LETTER REVERSED DZE
					*p = 0xA644;	//	ê™„	0xea 0x99 0x84	CYRILLIC CAPITAL LETTER REVERSED DZE
					break;
				case 0xA647:		//	ê™‡	0xea 0x99 0x87	CYRILLIC SMALL LETTER IOTA
					*p = 0xA646;	//	ê™†	0xea 0x99 0x86	CYRILLIC CAPITAL LETTER IOTA
					break;
				case 0xA649:		//	ê™‰	0xea 0x99 0x89	CYRILLIC SMALL LETTER DJERV
					*p = 0xA648;	//	ê™ˆ	0xea 0x99 0x88	CYRILLIC CAPITAL LETTER DJERV
					break;
				case 0xA64B:		//	ê™‹	0xea 0x99 0x8b	CYRILLIC SMALL LETTER MONOGRAPH UK
					*p = 0xA64A;	//	ê™Š	0xea 0x99 0x8a	CYRILLIC CAPITAL LETTER MONOGRAPH UK
					break;
				case 0xA64D:		//	ê™	0xea 0x99 0x8d	CYRILLIC SMALL LETTER BROAD OMEGA
					*p = 0xA64C;	//	ê™Œ	0xea 0x99 0x8c	CYRILLIC CAPITAL LETTER BROAD OMEGA
					break;
				case 0xA64F:		//	ê™	0xea 0x99 0x8f	CYRILLIC SMALL LETTER NEUTRAL YER
					*p = 0xA64E;	//	ê™Ž	0xea 0x99 0x8e	CYRILLIC CAPITAL LETTER NEUTRAL YER
					break;
				case 0xA651:		//	ê™‘	0xea 0x99 0x91	CYRILLIC SMALL LETTER YERU WITH BACK YER
					*p = 0xA650;	//	ê™	0xea 0x99 0x90	CYRILLIC CAPITAL LETTER YERU WITH BACK YER
					break;
				case 0xA653:		//	ê™“	0xea 0x99 0x93	CYRILLIC SMALL LETTER IOTIFIED YAT
					*p = 0xA652;	//	ê™’	0xea 0x99 0x92	CYRILLIC CAPITAL LETTER IOTIFIED YAT
					break;
				case 0xA655:		//	ê™•	0xea 0x99 0x95	CYRILLIC SMALL LETTER REVERSED YU
					*p = 0xA654;	//	ê™”	0xea 0x99 0x94	CYRILLIC CAPITAL LETTER REVERSED YU
					break;
				case 0xA657:		//	ê™—	0xea 0x99 0x97	CYRILLIC SMALL LETTER IOTIFIED A
					*p = 0xA656;	//	ê™–	0xea 0x99 0x96	CYRILLIC CAPITAL LETTER IOTIFIED A
					break;
				case 0xA659:		//	ê™™	0xea 0x99 0x99	CYRILLIC SMALL LETTER CLOSED LITTLE YUS
					*p = 0xA658;	//	ê™˜	0xea 0x99 0x98	CYRILLIC CAPITAL LETTER CLOSED LITTLE YUS
					break;
				case 0xA65B:		//	ê™›	0xea 0x99 0x9b	CYRILLIC SMALL LETTER BLENDED YUS
					*p = 0xA65A;	//	ê™š	0xea 0x99 0x9a	CYRILLIC CAPITAL LETTER BLENDED YUS
					break;
				case 0xA65D:		//	ê™	0xea 0x99 0x9d	CYRILLIC SMALL LETTER IOTIFIED CLOSED LITTLE YUS
					*p = 0xA65C;	//	ê™œ	0xea 0x99 0x9c	CYRILLIC CAPITAL LETTER IOTIFIED CLOSED LITTLE YUS
					break;
				case 0xA65F:		//	ê™Ÿ	0xea 0x99 0x9f	CYRILLIC SMALL LETTER YN
					*p = 0xA65E;	//	ê™ž	0xea 0x99 0x9e	CYRILLIC CAPITAL LETTER YN
					break;
				case 0xA661:		//	ê™¡	0xea 0x99 0xa1	CYRILLIC SMALL LETTER REVERSED TSE
					*p = 0xA660;	//	ê™ 	0xea 0x99 0xa0	CYRILLIC CAPITAL LETTER REVERSED TSE
					break;
				case 0xA663:		//	ê™£	0xea 0x99 0xa3	CYRILLIC SMALL LETTER SOFT DE
					*p = 0xA662;	//	ê™¢	0xea 0x99 0xa2	CYRILLIC CAPITAL LETTER SOFT DE
					break;
				case 0xA665:		//	ê™¥	0xea 0x99 0xa5	CYRILLIC SMALL LETTER SOFT EL
					*p = 0xA664;	//	ê™¤	0xea 0x99 0xa4	CYRILLIC CAPITAL LETTER SOFT EL
					break;
				case 0xA667:		//	ê™§	0xea 0x99 0xa7	CYRILLIC SMALL LETTER SOFT EM
					*p = 0xA666;	//	ê™¦	0xea 0x99 0xa6	CYRILLIC CAPITAL LETTER SOFT EM
					break;
				case 0xA669:		//	ê™©	0xea 0x99 0xa9	CYRILLIC SMALL LETTER MONOCULAR O
					*p = 0xA668;	//	ê™¨	0xea 0x99 0xa8	CYRILLIC CAPITAL LETTER MONOCULAR O
					break;
				case 0xA66B:		//	ê™«	0xea 0x99 0xab	CYRILLIC SMALL LETTER BINOCULAR O
					*p = 0xA66A;	//	ê™ª	0xea 0x99 0xaa	CYRILLIC CAPITAL LETTER BINOCULAR O
					break;
				case 0xA66D:		//	ê™­	0xea 0x99 0xad	CYRILLIC SMALL LETTER DOUBLE MONOCULAR O
					*p = 0xA66C;	//	ê™¬	0xea 0x99 0xac	CYRILLIC CAPITAL LETTER DOUBLE MONOCULAR O
					break;
				case 0xA681:		//	êš	0xea 0x9a 0x81	CYRILLIC SMALL LETTER DWE
					*p = 0xA680;	//	êš€	0xea 0x9a 0x80	CYRILLIC CAPITAL LETTER DWE
					break;
				case 0xA683:		//	êšƒ	0xea 0x9a 0x83	CYRILLIC SMALL LETTER DZWE
					*p = 0xA682;	//	êš‚	0xea 0x9a 0x82	CYRILLIC CAPITAL LETTER DZWE
					break;
				case 0xA685:		//	êš…	0xea 0x9a 0x85	CYRILLIC SMALL LETTER ZHWE
					*p = 0xA684;	//	êš„	0xea 0x9a 0x84	CYRILLIC CAPITAL LETTER ZHWE
					break;
				case 0xA687:		//	êš‡	0xea 0x9a 0x87	CYRILLIC SMALL LETTER CCHE
					*p = 0xA686;	//	êš†	0xea 0x9a 0x86	CYRILLIC CAPITAL LETTER CCHE
					break;
				case 0xA689:		//	êš‰	0xea 0x9a 0x89	CYRILLIC SMALL LETTER DZZE
					*p = 0xA688;	//	êšˆ	0xea 0x9a 0x88	CYRILLIC CAPITAL LETTER DZZE
					break;
				case 0xA68B:		//	êš‹	0xea 0x9a 0x8b	CYRILLIC SMALL LETTER TE WITH MIDDLE HOOK
					*p = 0xA68A;	//	êšŠ	0xea 0x9a 0x8a	CYRILLIC CAPITAL LETTER TE WITH MIDDLE HOOK
					break;
				case 0xA68D:		//	êš	0xea 0x9a 0x8d	CYRILLIC SMALL LETTER TWE
					*p = 0xA68C;	//	êšŒ	0xea 0x9a 0x8c	CYRILLIC CAPITAL LETTER TWE
					break;
				case 0xA68F:		//	êš	0xea 0x9a 0x8f	CYRILLIC SMALL LETTER TSWE
					*p = 0xA68E;	//	êšŽ	0xea 0x9a 0x8e	CYRILLIC CAPITAL LETTER TSWE
					break;
				case 0xA691:		//	êš‘	0xea 0x9a 0x91	CYRILLIC SMALL LETTER TSSE
					*p = 0xA690;	//	êš	0xea 0x9a 0x90	CYRILLIC CAPITAL LETTER TSSE
					break;
				case 0xA693:		//	êš“	0xea 0x9a 0x93	CYRILLIC SMALL LETTER TCHE
					*p = 0xA692;	//	êš’	0xea 0x9a 0x92	CYRILLIC CAPITAL LETTER TCHE
					break;
				case 0xA695:		//	êš•	0xea 0x9a 0x95	CYRILLIC SMALL LETTER HWE
					*p = 0xA694;	//	êš”	0xea 0x9a 0x94	CYRILLIC CAPITAL LETTER HWE
					break;
				case 0xA697:		//	êš—	0xea 0x9a 0x97	CYRILLIC SMALL LETTER SHWE
					*p = 0xA696;	//	êš–	0xea 0x9a 0x96	CYRILLIC CAPITAL LETTER SHWE
					break;
				case 0xA699:		//	êš™	0xea 0x9a 0x99	CYRILLIC SMALL LETTER DOUBLE O
					*p = 0xA698;	//	êš˜	0xea 0x9a 0x98	CYRILLIC CAPITAL LETTER DOUBLE O
					break;
				case 0xA69B:		//	êš›	0xea 0x9a 0x9b	CYRILLIC SMALL LETTER CROSSED O
					*p = 0xA69A;	//	êšš	0xea 0x9a 0x9a	CYRILLIC CAPITAL LETTER CROSSED O
					break;
				case 0xA723:		//	êœ£	0xea 0x9c 0xa3	LATIN SMALL LETTER EGYPTOLOGICAL ALEF
					*p = 0xA722;	//	êœ¢	0xea 0x9c 0xa2	LATIN CAPITAL LETTER EGYPTOLOGICAL ALEF
					break;
				case 0xA725:		//	êœ¥	0xea 0x9c 0xa5	LATIN SMALL LETTER EGYPTOLOGICAL AIN
					*p = 0xA724;	//	êœ¤	0xea 0x9c 0xa4	LATIN CAPITAL LETTER EGYPTOLOGICAL AIN
					break;
				case 0xA727:		//	êœ§	0xea 0x9c 0xa7	LATIN SMALL LETTER HENG
					*p = 0xA726;	//	êœ¦	0xea 0x9c 0xa6	LATIN CAPITAL LETTER HENG
					break;
				case 0xA729:		//	êœ©	0xea 0x9c 0xa9	LATIN SMALL LETTER TZ
					*p = 0xA728;	//	êœ¨	0xea 0x9c 0xa8	LATIN CAPITAL LETTER TZ
					break;
				case 0xA72B:		//	êœ«	0xea 0x9c 0xab	LATIN SMALL LETTER TRESILLO
					*p = 0xA72A;	//	êœª	0xea 0x9c 0xaa	LATIN CAPITAL LETTER TRESILLO
					break;
				case 0xA72D:		//	êœ­	0xea 0x9c 0xad	LATIN SMALL LETTER CUATRILLO
					*p = 0xA72C;	//	êœ¬	0xea 0x9c 0xac	LATIN CAPITAL LETTER CUATRILLO
					break;
				case 0xA72F:		//	êœ¯	0xea 0x9c 0xaf	LATIN SMALL LETTER CUATRILLO WITH COMMA
					*p = 0xA72E;	//	êœ®	0xea 0x9c 0xae	LATIN CAPITAL LETTER CUATRILLO WITH COMMA
					break;
				case 0xA733:		//	êœ³	0xea 0x9c 0xb3	LATIN SMALL LETTER AA
					*p = 0xA732;	//	êœ²	0xea 0x9c 0xb2	LATIN CAPITAL LETTER AA
					break;
				case 0xA735:		//	êœµ	0xea 0x9c 0xb5	LATIN SMALL LETTER AO
					*p = 0xA734;	//	êœ´	0xea 0x9c 0xb4	LATIN CAPITAL LETTER AO
					break;
				case 0xA737:		//	êœ·	0xea 0x9c 0xb7	LATIN SMALL LETTER AU
					*p = 0xA736;	//	êœ¶	0xea 0x9c 0xb6	LATIN CAPITAL LETTER AU
					break;
				case 0xA739:		//	êœ¹	0xea 0x9c 0xb9	LATIN SMALL LETTER AV
					*p = 0xA738;	//	êœ¸	0xea 0x9c 0xb8	LATIN CAPITAL LETTER AV
					break;
				case 0xA73B:		//	êœ»	0xea 0x9c 0xbb	LATIN SMALL LETTER AV WITH HORIZONTAL BAR
					*p = 0xA73A;	//	êœº	0xea 0x9c 0xba	LATIN CAPITAL LETTER AV WITH HORIZONTAL BAR
					break;
				case 0xA73D:		//	êœ½	0xea 0x9c 0xbd	LATIN SMALL LETTER AY
					*p = 0xA73C;	//	êœ¼	0xea 0x9c 0xbc	LATIN CAPITAL LETTER AY
					break;
				case 0xA73F:		//	êœ¿	0xea 0x9c 0xbf	LATIN SMALL LETTER REVERSED C WITH DOT
					*p = 0xA73E;	//	êœ¾	0xea 0x9c 0xbe	LATIN CAPITAL LETTER REVERSED C WITH DOT
					break;
				case 0xA741:		//	ê	0xea 0x9d 0x81	LATIN SMALL LETTER K WITH STROKE
					*p = 0xA740;	//	ê€	0xea 0x9d 0x80	LATIN CAPITAL LETTER K WITH STROKE
					break;
				case 0xA743:		//	êƒ	0xea 0x9d 0x83	LATIN SMALL LETTER K WITH DIAGONAL STROKE
					*p = 0xA742;	//	ê‚	0xea 0x9d 0x82	LATIN CAPITAL LETTER K WITH DIAGONAL STROKE
					break;
				case 0xA745:		//	ê…	0xea 0x9d 0x85	LATIN SMALL LETTER K WITH STROKE AND DIAGONAL STROKE
					*p = 0xA744;	//	ê„	0xea 0x9d 0x84	LATIN CAPITAL LETTER K WITH STROKE AND DIAGONAL STROKE
					break;
				case 0xA747:		//	ê‡	0xea 0x9d 0x87	LATIN SMALL LETTER BROKEN L
					*p = 0xA746;	//	ê†	0xea 0x9d 0x86	LATIN CAPITAL LETTER BROKEN L
					break;
				case 0xA749:		//	ê‰	0xea 0x9d 0x89	LATIN SMALL LETTER L WITH HIGH STROKE
					*p = 0xA748;	//	êˆ	0xea 0x9d 0x88	LATIN CAPITAL LETTER L WITH HIGH STROKE
					break;
				case 0xA74B:		//	ê‹	0xea 0x9d 0x8b	LATIN SMALL LETTER O WITH LONG STROKE OVERLAY
					*p = 0xA74A;	//	êŠ	0xea 0x9d 0x8a	LATIN CAPITAL LETTER O WITH LONG STROKE OVERLAY
					break;
				case 0xA74D:		//	ê	0xea 0x9d 0x8d	LATIN SMALL LETTER O WITH LOOP
					*p = 0xA74C;	//	êŒ	0xea 0x9d 0x8c	LATIN CAPITAL LETTER O WITH LOOP
					break;
				case 0xA74F:		//	ê	0xea 0x9d 0x8f	LATIN SMALL LETTER OO
					*p = 0xA74E;	//	êŽ	0xea 0x9d 0x8e	LATIN CAPITAL LETTER OO
					break;
				case 0xA751:		//	ê‘	0xea 0x9d 0x91	LATIN SMALL LETTER P WITH STROKE THROUGH DESCENDER
					*p = 0xA750;	//	ê	0xea 0x9d 0x90	LATIN CAPITAL LETTER P WITH STROKE THROUGH DESCENDER
					break;
				case 0xA753:		//	ê“	0xea 0x9d 0x93	LATIN SMALL LETTER P WITH FLOURISH
					*p = 0xA752;	//	ê’	0xea 0x9d 0x92	LATIN CAPITAL LETTER P WITH FLOURISH
					break;
				case 0xA755:		//	ê•	0xea 0x9d 0x95	LATIN SMALL LETTER P WITH SQUIRREL TAIL
					*p = 0xA754;	//	ê”	0xea 0x9d 0x94	LATIN CAPITAL LETTER P WITH SQUIRREL TAIL
					break;
				case 0xA757:		//	ê—	0xea 0x9d 0x97	LATIN SMALL LETTER Q WITH STROKE THROUGH DESCENDER
					*p = 0xA756;	//	ê–	0xea 0x9d 0x96	LATIN CAPITAL LETTER Q WITH STROKE THROUGH DESCENDER
					break;
				case 0xA759:		//	ê™	0xea 0x9d 0x99	LATIN SMALL LETTER Q WITH DIAGONAL STROKE
					*p = 0xA758;	//	ê˜	0xea 0x9d 0x98	LATIN CAPITAL LETTER Q WITH DIAGONAL STROKE
					break;
				case 0xA75B:		//	ê›	0xea 0x9d 0x9b	LATIN SMALL LETTER R ROTUNDA
					*p = 0xA75A;	//	êš	0xea 0x9d 0x9a	LATIN CAPITAL LETTER R ROTUNDA
					break;
				case 0xA75D:		//	ê	0xea 0x9d 0x9d	LATIN SMALL LETTER RUM ROTUNDA
					*p = 0xA75C;	//	êœ	0xea 0x9d 0x9c	LATIN CAPITAL LETTER RUM ROTUNDA
					break;
				case 0xA75F:		//	êŸ	0xea 0x9d 0x9f	LATIN SMALL LETTER V WITH DIAGONAL STROKE
					*p = 0xA75E;	//	êž	0xea 0x9d 0x9e	LATIN CAPITAL LETTER V WITH DIAGONAL STROKE
					break;
				case 0xA761:		//	ê¡	0xea 0x9d 0xa1	LATIN SMALL LETTER VY
					*p = 0xA760;	//	ê 	0xea 0x9d 0xa0	LATIN CAPITAL LETTER VY
					break;
				case 0xA763:		//	ê£	0xea 0x9d 0xa3	LATIN SMALL LETTER VISIGOTHIC Z
					*p = 0xA762;	//	ê¢	0xea 0x9d 0xa2	LATIN CAPITAL LETTER VISIGOTHIC Z
					break;
				case 0xA765:		//	ê¥	0xea 0x9d 0xa5	LATIN SMALL LETTER THORN WITH STROKE
					*p = 0xA764;	//	ê¤	0xea 0x9d 0xa4	LATIN CAPITAL LETTER THORN WITH STROKE
					break;
				case 0xA767:		//	ê§	0xea 0x9d 0xa7	LATIN SMALL LETTER THORN WITH STROKE THROUGH DESCENDER
					*p = 0xA766;	//	ê¦	0xea 0x9d 0xa6	LATIN CAPITAL LETTER THORN WITH STROKE THROUGH DESCENDER
					break;
				case 0xA769:		//	ê©	0xea 0x9d 0xa9	LATIN SMALL LETTER VEND
					*p = 0xA768;	//	ê¨	0xea 0x9d 0xa8	LATIN CAPITAL LETTER VEND
					break;
				case 0xA76B:		//	ê«	0xea 0x9d 0xab	LATIN SMALL LETTER ET
					*p = 0xA76A;	//	êª	0xea 0x9d 0xaa	LATIN CAPITAL LETTER ET
					break;
				case 0xA76D:		//	ê­	0xea 0x9d 0xad	LATIN SMALL LETTER IS
					*p = 0xA76C;	//	ê¬	0xea 0x9d 0xac	LATIN CAPITAL LETTER IS
					break;
				case 0xA76F:		//	ê¯	0xea 0x9d 0xaf	LATIN SMALL LETTER CON
					*p = 0xA76E;	//	ê®	0xea 0x9d 0xae	LATIN CAPITAL LETTER CON
					break;
				case 0xA77A:		//	êº	0xea 0x9d 0xba	LATIN SMALL LETTER INSULAR D
					*p = 0xA779;	//	ê¹	0xea 0x9d 0xb9	LATIN CAPITAL LETTER INSULAR D
					break;
				case 0xA77C:		//	ê¼	0xea 0x9d 0xbc	LATIN SMALL LETTER INSULAR F
					*p = 0xA77B;	//	ê»	0xea 0x9d 0xbb	LATIN CAPITAL LETTER INSULAR F
					break;
				case 0xA77F:		//	ê¿	0xea 0x9d 0xbf	LATIN SMALL LETTER TURNED INSULAR G
					*p = 0xA77E;	//	ê¾	0xea 0x9d 0xbe	LATIN CAPITAL LETTER TURNED INSULAR G
					break;
				case 0xA781:		//	êž	0xea 0x9e 0x81	LATIN SMALL LETTER TURNED L
					*p = 0xA780;	//	êž€	0xea 0x9e 0x80	LATIN CAPITAL LETTER TURNED L
					break;
				case 0xA783:		//	êžƒ	0xea 0x9e 0x83	LATIN SMALL LETTER INSULAR R
					*p = 0xA782;	//	êž‚	0xea 0x9e 0x82	LATIN CAPITAL LETTER INSULAR R
					break;
				case 0xA785:		//	êž…	0xea 0x9e 0x85	LATIN SMALL LETTER INSULAR S
					*p = 0xA784;	//	êž„	0xea 0x9e 0x84	LATIN CAPITAL LETTER INSULAR S
					break;
				case 0xA787:		//	êž‡	0xea 0x9e 0x87	LATIN SMALL LETTER INSULAR T
					*p = 0xA786;	//	êž†	0xea 0x9e 0x86	LATIN CAPITAL LETTER INSULAR T
					break;
				case 0xA78C:		//	êžŒ	0xea 0x9e 0x8c	LATIN SMALL LETTER SALTILLO
					*p = 0xA78B;	//	êž‹	0xea 0x9e 0x8b	LATIN CAPITAL LETTER SALTILLO
					break;
				case 0xA791:		//	êž‘	0xea 0x9e 0x91	LATIN SMALL LETTER N WITH DESCENDER
					*p = 0xA790;	//	êž	0xea 0x9e 0x90	LATIN CAPITAL LETTER N WITH DESCENDER
					break;
				case 0xA793:		//	êž“	0xea 0x9e 0x93	LATIN SMALL LETTER C WITH BAR
					*p = 0xA792;	//	êž’	0xea 0x9e 0x92	LATIN CAPITAL LETTER C WITH BAR
					break;
				case 0xA794:		//	êž”	0xea 0x9e 0x94	LATIN SMALL LETTER C WITH PALATAL HOOK
					*p = 0xA7C4;	//	êŸ„	0xea 0x9f 0x84	LATIN CAPITAL LETTER C WITH PALATAL HOOK
					break;
				case 0xA797:		//	êž—	0xea 0x9e 0x97	LATIN SMALL LETTER B WITH FLOURISH
					*p = 0xA796;	//	êž–	0xea 0x9e 0x96	LATIN CAPITAL LETTER B WITH FLOURISH
					break;
				case 0xA799:		//	êž™	0xea 0x9e 0x99	LATIN SMALL LETTER F WITH STROKE
					*p = 0xA798;	//	êž˜	0xea 0x9e 0x98	LATIN CAPITAL LETTER F WITH STROKE
					break;
				case 0xA79B:		//	êž›	0xea 0x9e 0x9b	LATIN SMALL LETTER VOLAPUK AE
					*p = 0xA79A;	//	êžš	0xea 0x9e 0x9a	LATIN CAPITAL LETTER VOLAPUK AE
					break;
				case 0xA79D:		//	êž	0xea 0x9e 0x9d	LATIN SMALL LETTER VOLAPUK OE
					*p = 0xA79C;	//	êžœ	0xea 0x9e 0x9c	LATIN CAPITAL LETTER VOLAPUK OE
					break;
				case 0xA79F:		//	êžŸ	0xea 0x9e 0x9f	LATIN SMALL LETTER VOLAPUK UE
					*p = 0xA79E;	//	êžž	0xea 0x9e 0x9e	LATIN CAPITAL LETTER VOLAPUK UE
					break;
				case 0xA7A1:		//	êž¡	0xea 0x9e 0xa1	LATIN SMALL LETTER G WITH OBLIQUE STROKE
					*p = 0xA7A0;	//	êž 	0xea 0x9e 0xa0	LATIN CAPITAL LETTER G WITH OBLIQUE STROKE
					break;
				case 0xA7A3:		//	êž£	0xea 0x9e 0xa3	LATIN SMALL LETTER K WITH OBLIQUE STROKE
					*p = 0xA7A2;	//	êž¢	0xea 0x9e 0xa2	LATIN CAPITAL LETTER K WITH OBLIQUE STROKE
					break;
				case 0xA7A5:		//	êž¥	0xea 0x9e 0xa5	LATIN SMALL LETTER N WITH OBLIQUE STROKE
					*p = 0xA7A4;	//	êž¤	0xea 0x9e 0xa4	LATIN CAPITAL LETTER N WITH OBLIQUE STROKE
					break;
				case 0xA7A7:		//	êž§	0xea 0x9e 0xa7	LATIN SMALL LETTER R WITH OBLIQUE STROKE
					*p = 0xA7A6;	//	êž¦	0xea 0x9e 0xa6	LATIN CAPITAL LETTER R WITH OBLIQUE STROKE
					break;
				case 0xA7A9:		//	êž©	0xea 0x9e 0xa9	LATIN SMALL LETTER S WITH OBLIQUE STROKE
					*p = 0xA7A8;	//	êž¨	0xea 0x9e 0xa8	LATIN CAPITAL LETTER S WITH OBLIQUE STROKE
					break;
				case 0xA7B5:		//	êžµ	0xea 0x9e 0xb5	LATIN SMALL LETTER BETA
					*p = 0xA7B4;	//	êž´	0xea 0x9e 0xb4	LATIN CAPITAL LETTER BETA
					break;
				case 0xA7B7:		//	êž·	0xea 0x9e 0xb7	LATIN SMALL LETTER OMEGA
					*p = 0xA7B6;	//	êž¶	0xea 0x9e 0xb6	LATIN CAPITAL LETTER OMEGA
					break;
				case 0xA7B9:		//	êž¹	0xea 0x9e 0xb9	LATIN SMALL LETTER U WITH STROKE
					*p = 0xA7B8;	//	êž¸	0xea 0x9e 0xb8	LATIN CAPITAL LETTER U WITH STROKE
					break;
				case 0xA7BB:		//	êž»	0xea 0x9e 0xbb	LATIN SMALL LETTER GLOTTAL A
					*p = 0xA7BA;	//	êžº	0xea 0x9e 0xba	LATIN CAPITAL LETTER GLOTTAL A
					break;
				case 0xA7BD:		//	êž½	0xea 0x9e 0xbd	LATIN SMALL LETTER GLOTTAL I
					*p = 0xA7BC;	//	êž¼	0xea 0x9e 0xbc	LATIN CAPITAL LETTER GLOTTAL I
					break;
				case 0xA7BF:		//	êž¿	0xea 0x9e 0xbf	LATIN SMALL LETTER GLOTTAL U
					*p = 0xA7BE;	//	êž¾	0xea 0x9e 0xbe	LATIN CAPITAL LETTER GLOTTAL U
					break;
				case 0xA7C3:		//	êŸƒ	0xea 0x9f 0x83	LATIN SMALL LETTER ANGLICANA W
					*p = 0xA7C2;	//	êŸ‚	0xea 0x9f 0x82	LATIN CAPITAL LETTER ANGLICANA W
					break;
				case 0xA7C8:		//	êŸˆ	0xea 0x9f 0x88	LATIN SMALL LETTER D WITH SHORT STROKE OVERLAY
					*p = 0xA7C7;	//	êŸ‡	0xea 0x9f 0x87	LATIN CAPITAL LETTER D WITH SHORT STROKE OVERLAY
					break;
				case 0xA7CA:		//	êŸŠ	0xea 0x9f 0x8a	LATIN SMALL LETTER S WITH SHORT STROKE OVERLAY
					*p = 0xA7C9;	//	êŸ‰	0xea 0x9f 0x89	LATIN CAPITAL LETTER S WITH SHORT STROKE OVERLAY
					break;
				case 0xA7F6:		//	êŸ¶	0xea 0x9f 0xb6	LATIN SMALL LETTER REVERSED HALF H
					*p = 0xA7F5;	//	êŸµ	0xea 0x9f 0xb5	LATIN CAPITAL LETTER REVERSED HALF H
					break;
				case 0xAB53:		//	ê­“	0xea 0xad 0x93	LATIN SMALL LETTER CHI
					*p = 0xA7B3;	//	êž³	0xea 0x9e 0xb3	LATIN CAPITAL LETTER CHI
					break;
				case 0xAB70:		//	ê­°	0xea 0xad 0xb0	CHEROKEE SMALL LETTER A
					*p = 0x13A0;	//	áŽ 	0xe1 0x8e 0xa0	CHEROKEE CAPITAL LETTER A
					break;
				case 0xAB71:		//	ê­±	0xea 0xad 0xb1	CHEROKEE SMALL LETTER E
					*p = 0x13A1;	//	áŽ¡	0xe1 0x8e 0xa1	CHEROKEE CAPITAL LETTER E
					break;
				case 0xAB72:		//	ê­²	0xea 0xad 0xb2	CHEROKEE SMALL LETTER I
					*p = 0x13A2;	//	áŽ¢	0xe1 0x8e 0xa2	CHEROKEE CAPITAL LETTER I
					break;
				case 0xAB73:		//	ê­³	0xea 0xad 0xb3	CHEROKEE SMALL LETTER O
					*p = 0x13A3;	//	áŽ£	0xe1 0x8e 0xa3	CHEROKEE CAPITAL LETTER O
					break;
				case 0xAB74:		//	ê­´	0xea 0xad 0xb4	CHEROKEE SMALL LETTER U
					*p = 0x13A4;	//	áŽ¤	0xe1 0x8e 0xa4	CHEROKEE CAPITAL LETTER U
					break;
				case 0xAB75:		//	ê­µ	0xea 0xad 0xb5	CHEROKEE SMALL LETTER V
					*p = 0x13A5;	//	áŽ¥	0xe1 0x8e 0xa5	CHEROKEE CAPITAL LETTER V
					break;
				case 0xAB76:		//	ê­¶	0xea 0xad 0xb6	CHEROKEE SMALL LETTER GA
					*p = 0x13A6;	//	áŽ¦	0xe1 0x8e 0xa6	CHEROKEE CAPITAL LETTER GA
					break;
				case 0xAB77:		//	ê­·	0xea 0xad 0xb7	CHEROKEE SMALL LETTER KA
					*p = 0x13A7;	//	áŽ§	0xe1 0x8e 0xa7	CHEROKEE CAPITAL LETTER KA
					break;
				case 0xAB78:		//	ê­¸	0xea 0xad 0xb8	CHEROKEE SMALL LETTER GE
					*p = 0x13A8;	//	áŽ¨	0xe1 0x8e 0xa8	CHEROKEE CAPITAL LETTER GE
					break;
				case 0xAB79:		//	ê­¹	0xea 0xad 0xb9	CHEROKEE SMALL LETTER GI
					*p = 0x13A9;	//	áŽ©	0xe1 0x8e 0xa9	CHEROKEE CAPITAL LETTER GI
					break;
				case 0xAB7A:		//	ê­º	0xea 0xad 0xba	CHEROKEE SMALL LETTER GO
					*p = 0x13AA;	//	áŽª	0xe1 0x8e 0xaa	CHEROKEE CAPITAL LETTER GO
					break;
				case 0xAB7B:		//	ê­»	0xea 0xad 0xbb	CHEROKEE SMALL LETTER GU
					*p = 0x13AB;	//	áŽ«	0xe1 0x8e 0xab	CHEROKEE CAPITAL LETTER GU
					break;
				case 0xAB7C:		//	ê­¼	0xea 0xad 0xbc	CHEROKEE SMALL LETTER GV
					*p = 0x13AC;	//	áŽ¬	0xe1 0x8e 0xac	CHEROKEE CAPITAL LETTER GV
					break;
				case 0xAB7D:		//	ê­½	0xea 0xad 0xbd	CHEROKEE SMALL LETTER HA
					*p = 0x13AD;	//	áŽ­	0xe1 0x8e 0xad	CHEROKEE CAPITAL LETTER HA
					break;
				case 0xAB7E:		//	ê­¾	0xea 0xad 0xbe	CHEROKEE SMALL LETTER HE
					*p = 0x13AE;	//	áŽ®	0xe1 0x8e 0xae	CHEROKEE CAPITAL LETTER HE
					break;
				case 0xAB7F:		//	ê­¿	0xea 0xad 0xbf	CHEROKEE SMALL LETTER HI
					*p = 0x13AF;	//	áŽ¯	0xe1 0x8e 0xaf	CHEROKEE CAPITAL LETTER HI
					break;
				case 0xAB80:		//	ê®€	0xea 0xae 0x80	CHEROKEE SMALL LETTER HO
					*p = 0x13B0;	//	áŽ°	0xe1 0x8e 0xb0	CHEROKEE CAPITAL LETTER HO
					break;
				case 0xAB81:		//	ê®	0xea 0xae 0x81	CHEROKEE SMALL LETTER HU
					*p = 0x13B1;	//	áŽ±	0xe1 0x8e 0xb1	CHEROKEE CAPITAL LETTER HU
					break;
				case 0xAB82:		//	ê®‚	0xea 0xae 0x82	CHEROKEE SMALL LETTER HV
					*p = 0x13B2;	//	áŽ²	0xe1 0x8e 0xb2	CHEROKEE CAPITAL LETTER HV
					break;
				case 0xAB83:		//	ê®ƒ	0xea 0xae 0x83	CHEROKEE SMALL LETTER LA
					*p = 0x13B3;	//	áŽ³	0xe1 0x8e 0xb3	CHEROKEE CAPITAL LETTER LA
					break;
				case 0xAB84:		//	ê®„	0xea 0xae 0x84	CHEROKEE SMALL LETTER LE
					*p = 0x13B4;	//	áŽ´	0xe1 0x8e 0xb4	CHEROKEE CAPITAL LETTER LE
					break;
				case 0xAB85:		//	ê®…	0xea 0xae 0x85	CHEROKEE SMALL LETTER LI
					*p = 0x13B5;	//	áŽµ	0xe1 0x8e 0xb5	CHEROKEE CAPITAL LETTER LI
					break;
				case 0xAB86:		//	ê®†	0xea 0xae 0x86	CHEROKEE SMALL LETTER LO
					*p = 0x13B6;	//	áŽ¶	0xe1 0x8e 0xb6	CHEROKEE CAPITAL LETTER LO
					break;
				case 0xAB87:		//	ê®‡	0xea 0xae 0x87	CHEROKEE SMALL LETTER LU
					*p = 0x13B7;	//	áŽ·	0xe1 0x8e 0xb7	CHEROKEE CAPITAL LETTER LU
					break;
				case 0xAB88:		//	ê®ˆ	0xea 0xae 0x88	CHEROKEE SMALL LETTER LV
					*p = 0x13B8;	//	áŽ¸	0xe1 0x8e 0xb8	CHEROKEE CAPITAL LETTER LV
					break;
				case 0xAB89:		//	ê®‰	0xea 0xae 0x89	CHEROKEE SMALL LETTER MA
					*p = 0x13B9;	//	áŽ¹	0xe1 0x8e 0xb9	CHEROKEE CAPITAL LETTER MA
					break;
				case 0xAB8A:		//	ê®Š	0xea 0xae 0x8a	CHEROKEE SMALL LETTER ME
					*p = 0x13BA;	//	áŽº	0xe1 0x8e 0xba	CHEROKEE CAPITAL LETTER ME
					break;
				case 0xAB8B:		//	ê®‹	0xea 0xae 0x8b	CHEROKEE SMALL LETTER MI
					*p = 0x13BB;	//	áŽ»	0xe1 0x8e 0xbb	CHEROKEE CAPITAL LETTER MI
					break;
				case 0xAB8C:		//	ê®Œ	0xea 0xae 0x8c	CHEROKEE SMALL LETTER MO
					*p = 0x13BC;	//	áŽ¼	0xe1 0x8e 0xbc	CHEROKEE CAPITAL LETTER MO
					break;
				case 0xAB8D:		//	ê®	0xea 0xae 0x8d	CHEROKEE SMALL LETTER MU
					*p = 0x13BD;	//	áŽ½	0xe1 0x8e 0xbd	CHEROKEE CAPITAL LETTER MU
					break;
				case 0xAB8E:		//	ê®Ž	0xea 0xae 0x8e	CHEROKEE SMALL LETTER NA
					*p = 0x13BE;	//	áŽ¾	0xe1 0x8e 0xbe	CHEROKEE CAPITAL LETTER NA
					break;
				case 0xAB8F:		//	ê®	0xea 0xae 0x8f	CHEROKEE SMALL LETTER HNA
					*p = 0x13BF;	//	áŽ¿	0xe1 0x8e 0xbf	CHEROKEE CAPITAL LETTER HNA
					break;
				case 0xAB90:		//	ê®	0xea 0xae 0x90	CHEROKEE SMALL LETTER NAH
					*p = 0x13C0;	//	á€	0xe1 0x8f 0x80	CHEROKEE CAPITAL LETTER NAH
					break;
				case 0xAB91:		//	ê®‘	0xea 0xae 0x91	CHEROKEE SMALL LETTER NE
					*p = 0x13C1;	//	á	0xe1 0x8f 0x81	CHEROKEE CAPITAL LETTER NE
					break;
				case 0xAB92:		//	ê®’	0xea 0xae 0x92	CHEROKEE SMALL LETTER NI
					*p = 0x13C2;	//	á‚	0xe1 0x8f 0x82	CHEROKEE CAPITAL LETTER NI
					break;
				case 0xAB93:		//	ê®“	0xea 0xae 0x93	CHEROKEE SMALL LETTER NO
					*p = 0x13C3;	//	áƒ	0xe1 0x8f 0x83	CHEROKEE CAPITAL LETTER NO
					break;
				case 0xAB94:		//	ê®”	0xea 0xae 0x94	CHEROKEE SMALL LETTER NU
					*p = 0x13C4;	//	á„	0xe1 0x8f 0x84	CHEROKEE CAPITAL LETTER NU
					break;
				case 0xAB95:		//	ê®•	0xea 0xae 0x95	CHEROKEE SMALL LETTER NV
					*p = 0x13C5;	//	á…	0xe1 0x8f 0x85	CHEROKEE CAPITAL LETTER NV
					break;
				case 0xAB96:		//	ê®–	0xea 0xae 0x96	CHEROKEE SMALL LETTER QUA
					*p = 0x13C6;	//	á†	0xe1 0x8f 0x86	CHEROKEE CAPITAL LETTER QUA
					break;
				case 0xAB97:		//	ê®—	0xea 0xae 0x97	CHEROKEE SMALL LETTER QUE
					*p = 0x13C7;	//	á‡	0xe1 0x8f 0x87	CHEROKEE CAPITAL LETTER QUE
					break;
				case 0xAB98:		//	ê®˜	0xea 0xae 0x98	CHEROKEE SMALL LETTER QUI
					*p = 0x13C8;	//	áˆ	0xe1 0x8f 0x88	CHEROKEE CAPITAL LETTER QUI
					break;
				case 0xAB99:		//	ê®™	0xea 0xae 0x99	CHEROKEE SMALL LETTER QUO
					*p = 0x13C9;	//	á‰	0xe1 0x8f 0x89	CHEROKEE CAPITAL LETTER QUO
					break;
				case 0xAB9A:		//	ê®š	0xea 0xae 0x9a	CHEROKEE SMALL LETTER QUU
					*p = 0x13CA;	//	áŠ	0xe1 0x8f 0x8a	CHEROKEE CAPITAL LETTER QUU
					break;
				case 0xAB9B:		//	ê®›	0xea 0xae 0x9b	CHEROKEE SMALL LETTER QUV
					*p = 0x13CB;	//	á‹	0xe1 0x8f 0x8b	CHEROKEE CAPITAL LETTER QUV
					break;
				case 0xAB9C:		//	ê®œ	0xea 0xae 0x9c	CHEROKEE SMALL LETTER SA
					*p = 0x13CC;	//	áŒ	0xe1 0x8f 0x8c	CHEROKEE CAPITAL LETTER SA
					break;
				case 0xAB9D:		//	ê®	0xea 0xae 0x9d	CHEROKEE SMALL LETTER S
					*p = 0x13CD;	//	á	0xe1 0x8f 0x8d	CHEROKEE CAPITAL LETTER S
					break;
				case 0xAB9E:		//	ê®ž	0xea 0xae 0x9e	CHEROKEE SMALL LETTER SE
					*p = 0x13CE;	//	áŽ	0xe1 0x8f 0x8e	CHEROKEE CAPITAL LETTER SE
					break;
				case 0xAB9F:		//	ê®Ÿ	0xea 0xae 0x9f	CHEROKEE SMALL LETTER SI
					*p = 0x13CF;	//	á	0xe1 0x8f 0x8f	CHEROKEE CAPITAL LETTER SI
					break;
				case 0xABA0:		//	ê® 	0xea 0xae 0xa0	CHEROKEE SMALL LETTER SO
					*p = 0x13D0;	//	á	0xe1 0x8f 0x90	CHEROKEE CAPITAL LETTER SO
					break;
				case 0xABA1:		//	ê®¡	0xea 0xae 0xa1	CHEROKEE SMALL LETTER SU
					*p = 0x13D1;	//	á‘	0xe1 0x8f 0x91	CHEROKEE CAPITAL LETTER SU
					break;
				case 0xABA2:		//	ê®¢	0xea 0xae 0xa2	CHEROKEE SMALL LETTER SV
					*p = 0x13D2;	//	á’	0xe1 0x8f 0x92	CHEROKEE CAPITAL LETTER SV
					break;
				case 0xABA3:		//	ê®£	0xea 0xae 0xa3	CHEROKEE SMALL LETTER DA
					*p = 0x13D3;	//	á“	0xe1 0x8f 0x93	CHEROKEE CAPITAL LETTER DA
					break;
				case 0xABA4:		//	ê®¤	0xea 0xae 0xa4	CHEROKEE SMALL LETTER TA
					*p = 0x13D4;	//	á”	0xe1 0x8f 0x94	CHEROKEE CAPITAL LETTER TA
					break;
				case 0xABA5:		//	ê®¥	0xea 0xae 0xa5	CHEROKEE SMALL LETTER DE
					*p = 0x13D5;	//	á•	0xe1 0x8f 0x95	CHEROKEE CAPITAL LETTER DE
					break;
				case 0xABA6:		//	ê®¦	0xea 0xae 0xa6	CHEROKEE SMALL LETTER TE
					*p = 0x13D6;	//	á–	0xe1 0x8f 0x96	CHEROKEE CAPITAL LETTER TE
					break;
				case 0xABA7:		//	ê®§	0xea 0xae 0xa7	CHEROKEE SMALL LETTER DI
					*p = 0x13D7;	//	á—	0xe1 0x8f 0x97	CHEROKEE CAPITAL LETTER DI
					break;
				case 0xABA8:		//	ê®¨	0xea 0xae 0xa8	CHEROKEE SMALL LETTER TI
					*p = 0x13D8;	//	á˜	0xe1 0x8f 0x98	CHEROKEE CAPITAL LETTER TI
					break;
				case 0xABA9:		//	ê®©	0xea 0xae 0xa9	CHEROKEE SMALL LETTER DO
					*p = 0x13D9;	//	á™	0xe1 0x8f 0x99	CHEROKEE CAPITAL LETTER DO
					break;
				case 0xABAA:		//	ê®ª	0xea 0xae 0xaa	CHEROKEE SMALL LETTER DU
					*p = 0x13DA;	//	áš	0xe1 0x8f 0x9a	CHEROKEE CAPITAL LETTER DU
					break;
				case 0xABAB:		//	ê®«	0xea 0xae 0xab	CHEROKEE SMALL LETTER DV
					*p = 0x13DB;	//	á›	0xe1 0x8f 0x9b	CHEROKEE CAPITAL LETTER DV
					break;
				case 0xABAC:		//	ê®¬	0xea 0xae 0xac	CHEROKEE SMALL LETTER DLA
					*p = 0x13DC;	//	áœ	0xe1 0x8f 0x9c	CHEROKEE CAPITAL LETTER DLA
					break;
				case 0xABAD:		//	ê®­	0xea 0xae 0xad	CHEROKEE SMALL LETTER TLA
					*p = 0x13DD;	//	á	0xe1 0x8f 0x9d	CHEROKEE CAPITAL LETTER TLA
					break;
				case 0xABAE:		//	ê®®	0xea 0xae 0xae	CHEROKEE SMALL LETTER TLE
					*p = 0x13DE;	//	áž	0xe1 0x8f 0x9e	CHEROKEE CAPITAL LETTER TLE
					break;
				case 0xABAF:		//	ê®¯	0xea 0xae 0xaf	CHEROKEE SMALL LETTER TLI
					*p = 0x13DF;	//	áŸ	0xe1 0x8f 0x9f	CHEROKEE CAPITAL LETTER TLI
					break;
				case 0xABB0:		//	ê®°	0xea 0xae 0xb0	CHEROKEE SMALL LETTER TLO
					*p = 0x13E0;	//	á 	0xe1 0x8f 0xa0	CHEROKEE CAPITAL LETTER TLO
					break;
				case 0xABB1:		//	ê®±	0xea 0xae 0xb1	CHEROKEE SMALL LETTER TLU
					*p = 0x13E1;	//	á¡	0xe1 0x8f 0xa1	CHEROKEE CAPITAL LETTER TLU
					break;
				case 0xABB2:		//	ê®²	0xea 0xae 0xb2	CHEROKEE SMALL LETTER TLV
					*p = 0x13E2;	//	á¢	0xe1 0x8f 0xa2	CHEROKEE CAPITAL LETTER TLV
					break;
				case 0xABB3:		//	ê®³	0xea 0xae 0xb3	CHEROKEE SMALL LETTER TSA
					*p = 0x13E3;	//	á£	0xe1 0x8f 0xa3	CHEROKEE CAPITAL LETTER TSA
					break;
				case 0xABB4:		//	ê®´	0xea 0xae 0xb4	CHEROKEE SMALL LETTER TSE
					*p = 0x13E4;	//	á¤	0xe1 0x8f 0xa4	CHEROKEE CAPITAL LETTER TSE
					break;
				case 0xABB5:		//	ê®µ	0xea 0xae 0xb5	CHEROKEE SMALL LETTER TSI
					*p = 0x13E5;	//	á¥	0xe1 0x8f 0xa5	CHEROKEE CAPITAL LETTER TSI
					break;
				case 0xABB6:		//	ê®¶	0xea 0xae 0xb6	CHEROKEE SMALL LETTER TSO
					*p = 0x13E6;	//	á¦	0xe1 0x8f 0xa6	CHEROKEE CAPITAL LETTER TSO
					break;
				case 0xABB7:		//	ê®·	0xea 0xae 0xb7	CHEROKEE SMALL LETTER TSU
					*p = 0x13E7;	//	á§	0xe1 0x8f 0xa7	CHEROKEE CAPITAL LETTER TSU
					break;
				case 0xABB8:		//	ê®¸	0xea 0xae 0xb8	CHEROKEE SMALL LETTER TSV
					*p = 0x13E8;	//	á¨	0xe1 0x8f 0xa8	CHEROKEE CAPITAL LETTER TSV
					break;
				case 0xABB9:		//	ê®¹	0xea 0xae 0xb9	CHEROKEE SMALL LETTER WA
					*p = 0x13E9;	//	á©	0xe1 0x8f 0xa9	CHEROKEE CAPITAL LETTER WA
					break;
				case 0xABBA:		//	ê®º	0xea 0xae 0xba	CHEROKEE SMALL LETTER WE
					*p = 0x13EA;	//	áª	0xe1 0x8f 0xaa	CHEROKEE CAPITAL LETTER WE
					break;
				case 0xABBB:		//	ê®»	0xea 0xae 0xbb	CHEROKEE SMALL LETTER WI
					*p = 0x13EB;	//	á«	0xe1 0x8f 0xab	CHEROKEE CAPITAL LETTER WI
					break;
				case 0xABBC:		//	ê®¼	0xea 0xae 0xbc	CHEROKEE SMALL LETTER WO
					*p = 0x13EC;	//	á¬	0xe1 0x8f 0xac	CHEROKEE CAPITAL LETTER WO
					break;
				case 0xABBD:		//	ê®½	0xea 0xae 0xbd	CHEROKEE SMALL LETTER WU
					*p = 0x13ED;	//	á­	0xe1 0x8f 0xad	CHEROKEE CAPITAL LETTER WU
					break;
				case 0xABBE:		//	ê®¾	0xea 0xae 0xbe	CHEROKEE SMALL LETTER WV
					*p = 0x13EE;	//	á®	0xe1 0x8f 0xae	CHEROKEE CAPITAL LETTER WV
					break;
				case 0xABBF:		//	ê®¿	0xea 0xae 0xbf	CHEROKEE SMALL LETTER YA
					*p = 0x13EF;	//	á¯	0xe1 0x8f 0xaf	CHEROKEE CAPITAL LETTER YA
					break;
				case 0xFF41:		//	ï½	0xef 0xbd 0x81	FULLWIDTH LATIN SMALL LETTER A
					*p = 0xFF21;	//	ï¼¡	0xef 0xbc 0xa1	FULLWIDTH LATIN CAPITAL LETTER A
					break;
				case 0xFF42:		//	ï½‚	0xef 0xbd 0x82	FULLWIDTH LATIN SMALL LETTER B
					*p = 0xFF22;	//	ï¼¢	0xef 0xbc 0xa2	FULLWIDTH LATIN CAPITAL LETTER B
					break;
				case 0xFF43:		//	ï½ƒ	0xef 0xbd 0x83	FULLWIDTH LATIN SMALL LETTER C
					*p = 0xFF23;	//	ï¼£	0xef 0xbc 0xa3	FULLWIDTH LATIN CAPITAL LETTER C
					break;
				case 0xFF44:		//	ï½„	0xef 0xbd 0x84	FULLWIDTH LATIN SMALL LETTER D
					*p = 0xFF24;	//	ï¼¤	0xef 0xbc 0xa4	FULLWIDTH LATIN CAPITAL LETTER D
					break;
				case 0xFF45:		//	ï½…	0xef 0xbd 0x85	FULLWIDTH LATIN SMALL LETTER E
					*p = 0xFF25;	//	ï¼¥	0xef 0xbc 0xa5	FULLWIDTH LATIN CAPITAL LETTER E
					break;
				case 0xFF46:		//	ï½†	0xef 0xbd 0x86	FULLWIDTH LATIN SMALL LETTER F
					*p = 0xFF26;	//	ï¼¦	0xef 0xbc 0xa6	FULLWIDTH LATIN CAPITAL LETTER F
					break;
				case 0xFF47:		//	ï½‡	0xef 0xbd 0x87	FULLWIDTH LATIN SMALL LETTER G
					*p = 0xFF27;	//	ï¼§	0xef 0xbc 0xa7	FULLWIDTH LATIN CAPITAL LETTER G
					break;
				case 0xFF48:		//	ï½ˆ	0xef 0xbd 0x88	FULLWIDTH LATIN SMALL LETTER H
					*p = 0xFF28;	//	ï¼¨	0xef 0xbc 0xa8	FULLWIDTH LATIN CAPITAL LETTER H
					break;
				case 0xFF49:		//	ï½‰	0xef 0xbd 0x89	FULLWIDTH LATIN SMALL LETTER I
					*p = 0xFF29;	//	ï¼©	0xef 0xbc 0xa9	FULLWIDTH LATIN CAPITAL LETTER I
					break;
				case 0xFF4A:		//	ï½Š	0xef 0xbd 0x8a	FULLWIDTH LATIN SMALL LETTER J
					*p = 0xFF2A;	//	ï¼ª	0xef 0xbc 0xaa	FULLWIDTH LATIN CAPITAL LETTER J
					break;
				case 0xFF4B:		//	ï½‹	0xef 0xbd 0x8b	FULLWIDTH LATIN SMALL LETTER K
					*p = 0xFF2B;	//	ï¼«	0xef 0xbc 0xab	FULLWIDTH LATIN CAPITAL LETTER K
					break;
				case 0xFF4C:		//	ï½Œ	0xef 0xbd 0x8c	FULLWIDTH LATIN SMALL LETTER L
					*p = 0xFF2C;	//	ï¼¬	0xef 0xbc 0xac	FULLWIDTH LATIN CAPITAL LETTER L
					break;
				case 0xFF4D:		//	ï½	0xef 0xbd 0x8d	FULLWIDTH LATIN SMALL LETTER M
					*p = 0xFF2D;	//	ï¼­	0xef 0xbc 0xad	FULLWIDTH LATIN CAPITAL LETTER M
					break;
				case 0xFF4E:		//	ï½Ž	0xef 0xbd 0x8e	FULLWIDTH LATIN SMALL LETTER N
					*p = 0xFF2E;	//	ï¼®	0xef 0xbc 0xae	FULLWIDTH LATIN CAPITAL LETTER N
					break;
				case 0xFF4F:		//	ï½	0xef 0xbd 0x8f	FULLWIDTH LATIN SMALL LETTER O
					*p = 0xFF2F;	//	ï¼¯	0xef 0xbc 0xaf	FULLWIDTH LATIN CAPITAL LETTER O
					break;
				case 0xFF50:		//	ï½	0xef 0xbd 0x90	FULLWIDTH LATIN SMALL LETTER P
					*p = 0xFF30;	//	ï¼°	0xef 0xbc 0xb0	FULLWIDTH LATIN CAPITAL LETTER P
					break;
				case 0xFF51:		//	ï½‘	0xef 0xbd 0x91	FULLWIDTH LATIN SMALL LETTER Q
					*p = 0xFF31;	//	ï¼±	0xef 0xbc 0xb1	FULLWIDTH LATIN CAPITAL LETTER Q
					break;
				case 0xFF52:		//	ï½’	0xef 0xbd 0x92	FULLWIDTH LATIN SMALL LETTER R
					*p = 0xFF32;	//	ï¼²	0xef 0xbc 0xb2	FULLWIDTH LATIN CAPITAL LETTER R
					break;
				case 0xFF53:		//	ï½“	0xef 0xbd 0x93	FULLWIDTH LATIN SMALL LETTER S
					*p = 0xFF33;	//	ï¼³	0xef 0xbc 0xb3	FULLWIDTH LATIN CAPITAL LETTER S
					break;
				case 0xFF54:		//	ï½”	0xef 0xbd 0x94	FULLWIDTH LATIN SMALL LETTER T
					*p = 0xFF34;	//	ï¼´	0xef 0xbc 0xb4	FULLWIDTH LATIN CAPITAL LETTER T
					break;
				case 0xFF55:		//	ï½•	0xef 0xbd 0x95	FULLWIDTH LATIN SMALL LETTER U
					*p = 0xFF35;	//	ï¼µ	0xef 0xbc 0xb5	FULLWIDTH LATIN CAPITAL LETTER U
					break;
				case 0xFF56:		//	ï½–	0xef 0xbd 0x96	FULLWIDTH LATIN SMALL LETTER V
					*p = 0xFF36;	//	ï¼¶	0xef 0xbc 0xb6	FULLWIDTH LATIN CAPITAL LETTER V
					break;
				case 0xFF57:		//	ï½—	0xef 0xbd 0x97	FULLWIDTH LATIN SMALL LETTER W
					*p = 0xFF37;	//	ï¼·	0xef 0xbc 0xb7	FULLWIDTH LATIN CAPITAL LETTER W
					break;
				case 0xFF58:		//	ï½˜	0xef 0xbd 0x98	FULLWIDTH LATIN SMALL LETTER X
					*p = 0xFF38;	//	ï¼¸	0xef 0xbc 0xb8	FULLWIDTH LATIN CAPITAL LETTER X
					break;
				case 0xFF59:		//	ï½™	0xef 0xbd 0x99	FULLWIDTH LATIN SMALL LETTER Y
					*p = 0xFF39;	//	ï¼¹	0xef 0xbc 0xb9	FULLWIDTH LATIN CAPITAL LETTER Y
					break;
				case 0xFF5A:		//	ï½š	0xef 0xbd 0x9a	FULLWIDTH LATIN SMALL LETTER Z
					*p = 0xFF3A;	//	ï¼º	0xef 0xbc 0xba	FULLWIDTH LATIN CAPITAL LETTER Z
					break;
				case 0x10428:		//	ð¨	0xf0 0x90 0x90 0xa8	DESERET SMALL LETTER LONG I
					*p = 0x10400;	//	ð€	0xf0 0x90 0x90 0x80	DESERET CAPITAL LETTER LONG I
					break;
				case 0x10429:		//	ð©	0xf0 0x90 0x90 0xa9	DESERET SMALL LETTER LONG E
					*p = 0x10401;	//	ð	0xf0 0x90 0x90 0x81	DESERET CAPITAL LETTER LONG E
					break;
				case 0x1042A:		//	ðª	0xf0 0x90 0x90 0xaa	DESERET SMALL LETTER LONG A
					*p = 0x10402;	//	ð‚	0xf0 0x90 0x90 0x82	DESERET CAPITAL LETTER LONG A
					break;
				case 0x1042B:		//	ð«	0xf0 0x90 0x90 0xab	DESERET SMALL LETTER LONG AH
					*p = 0x10403;	//	ðƒ	0xf0 0x90 0x90 0x83	DESERET CAPITAL LETTER LONG AH
					break;
				case 0x1042C:		//	ð¬	0xf0 0x90 0x90 0xac	DESERET SMALL LETTER LONG O
					*p = 0x10404;	//	ð„	0xf0 0x90 0x90 0x84	DESERET CAPITAL LETTER LONG O
					break;
				case 0x1042D:		//	ð­	0xf0 0x90 0x90 0xad	DESERET SMALL LETTER LONG OO
					*p = 0x10405;	//	ð…	0xf0 0x90 0x90 0x85	DESERET CAPITAL LETTER LONG OO
					break;
				case 0x1042E:		//	ð®	0xf0 0x90 0x90 0xae	DESERET SMALL LETTER SHORT I
					*p = 0x10406;	//	ð†	0xf0 0x90 0x90 0x86	DESERET CAPITAL LETTER SHORT I
					break;
				case 0x1042F:		//	ð¯	0xf0 0x90 0x90 0xaf	DESERET SMALL LETTER SHORT E
					*p = 0x10407;	//	ð‡	0xf0 0x90 0x90 0x87	DESERET CAPITAL LETTER SHORT E
					break;
				case 0x10430:		//	ð°	0xf0 0x90 0x90 0xb0	DESERET SMALL LETTER SHORT A
					*p = 0x10408;	//	ðˆ	0xf0 0x90 0x90 0x88	DESERET CAPITAL LETTER SHORT A
					break;
				case 0x10431:		//	ð±	0xf0 0x90 0x90 0xb1	DESERET SMALL LETTER SHORT AH
					*p = 0x10409;	//	ð‰	0xf0 0x90 0x90 0x89	DESERET CAPITAL LETTER SHORT AH
					break;
				case 0x10432:		//	ð²	0xf0 0x90 0x90 0xb2	DESERET SMALL LETTER SHORT O
					*p = 0x1040A;	//	ðŠ	0xf0 0x90 0x90 0x8a	DESERET CAPITAL LETTER SHORT O
					break;
				case 0x10433:		//	ð³	0xf0 0x90 0x90 0xb3	DESERET SMALL LETTER SHORT OO
					*p = 0x1040B;	//	ð‹	0xf0 0x90 0x90 0x8b	DESERET CAPITAL LETTER SHORT OO
					break;
				case 0x10434:		//	ð´	0xf0 0x90 0x90 0xb4	DESERET SMALL LETTER AY
					*p = 0x1040C;	//	ðŒ	0xf0 0x90 0x90 0x8c	DESERET CAPITAL LETTER AY
					break;
				case 0x10435:		//	ðµ	0xf0 0x90 0x90 0xb5	DESERET SMALL LETTER OW
					*p = 0x1040D;	//	ð	0xf0 0x90 0x90 0x8d	DESERET CAPITAL LETTER OW
					break;
				case 0x10436:		//	ð¶	0xf0 0x90 0x90 0xb6	DESERET SMALL LETTER WU
					*p = 0x1040E;	//	ðŽ	0xf0 0x90 0x90 0x8e	DESERET CAPITAL LETTER WU
					break;
				case 0x10437:		//	ð·	0xf0 0x90 0x90 0xb7	DESERET SMALL LETTER YEE
					*p = 0x1040F;	//	ð	0xf0 0x90 0x90 0x8f	DESERET CAPITAL LETTER YEE
					break;
				case 0x10438:		//	ð¸	0xf0 0x90 0x90 0xb8	DESERET SMALL LETTER H
					*p = 0x10410;	//	ð	0xf0 0x90 0x90 0x90	DESERET CAPITAL LETTER H
					break;
				case 0x10439:		//	ð¹	0xf0 0x90 0x90 0xb9	DESERET SMALL LETTER PEE
					*p = 0x10411;	//	ð‘	0xf0 0x90 0x90 0x91	DESERET CAPITAL LETTER PEE
					break;
				case 0x1043A:		//	ðº	0xf0 0x90 0x90 0xba	DESERET SMALL LETTER BEE
					*p = 0x10412;	//	ð’	0xf0 0x90 0x90 0x92	DESERET CAPITAL LETTER BEE
					break;
				case 0x1043B:		//	ð»	0xf0 0x90 0x90 0xbb	DESERET SMALL LETTER TEE
					*p = 0x10413;	//	ð“	0xf0 0x90 0x90 0x93	DESERET CAPITAL LETTER TEE
					break;
				case 0x1043C:		//	ð¼	0xf0 0x90 0x90 0xbc	DESERET SMALL LETTER DEE
					*p = 0x10414;	//	ð”	0xf0 0x90 0x90 0x94	DESERET CAPITAL LETTER DEE
					break;
				case 0x1043D:		//	ð½	0xf0 0x90 0x90 0xbd	DESERET SMALL LETTER CHEE
					*p = 0x10415;	//	ð•	0xf0 0x90 0x90 0x95	DESERET CAPITAL LETTER CHEE
					break;
				case 0x1043E:		//	ð¾	0xf0 0x90 0x90 0xbe	DESERET SMALL LETTER JEE
					*p = 0x10416;	//	ð–	0xf0 0x90 0x90 0x96	DESERET CAPITAL LETTER JEE
					break;
				case 0x1043F:		//	ð¿	0xf0 0x90 0x90 0xbf	DESERET SMALL LETTER KAY
					*p = 0x10417;	//	ð—	0xf0 0x90 0x90 0x97	DESERET CAPITAL LETTER KAY
					break;
				case 0x10440:		//	ð‘€	0xf0 0x90 0x91 0x80	DESERET SMALL LETTER GAY
					*p = 0x10418;	//	ð˜	0xf0 0x90 0x90 0x98	DESERET CAPITAL LETTER GAY
					break;
				case 0x10441:		//	ð‘	0xf0 0x90 0x91 0x81	DESERET SMALL LETTER EF
					*p = 0x10419;	//	ð™	0xf0 0x90 0x90 0x99	DESERET CAPITAL LETTER EF
					break;
				case 0x10442:		//	ð‘‚	0xf0 0x90 0x91 0x82	DESERET SMALL LETTER VEE
					*p = 0x1041A;	//	ðš	0xf0 0x90 0x90 0x9a	DESERET CAPITAL LETTER VEE
					break;
				case 0x10443:		//	ð‘ƒ	0xf0 0x90 0x91 0x83	DESERET SMALL LETTER ETH
					*p = 0x1041B;	//	ð›	0xf0 0x90 0x90 0x9b	DESERET CAPITAL LETTER ETH
					break;
				case 0x10444:		//	ð‘„	0xf0 0x90 0x91 0x84	DESERET SMALL LETTER THEE
					*p = 0x1041C;	//	ðœ	0xf0 0x90 0x90 0x9c	DESERET CAPITAL LETTER THEE
					break;
				case 0x10445:		//	ð‘…	0xf0 0x90 0x91 0x85	DESERET SMALL LETTER ES
					*p = 0x1041D;	//	ð	0xf0 0x90 0x90 0x9d	DESERET CAPITAL LETTER ES
					break;
				case 0x10446:		//	ð‘†	0xf0 0x90 0x91 0x86	DESERET SMALL LETTER ZEE
					*p = 0x1041E;	//	ðž	0xf0 0x90 0x90 0x9e	DESERET CAPITAL LETTER ZEE
					break;
				case 0x10447:		//	ð‘‡	0xf0 0x90 0x91 0x87	DESERET SMALL LETTER ESH
					*p = 0x1041F;	//	ðŸ	0xf0 0x90 0x90 0x9f	DESERET CAPITAL LETTER ESH
					break;
				case 0x10448:		//	ð‘ˆ	0xf0 0x90 0x91 0x88	DESERET SMALL LETTER ZHEE
					*p = 0x10420;	//	ð 	0xf0 0x90 0x90 0xa0	DESERET CAPITAL LETTER ZHEE
					break;
				case 0x10449:		//	ð‘‰	0xf0 0x90 0x91 0x89	DESERET SMALL LETTER ER
					*p = 0x10421;	//	ð¡	0xf0 0x90 0x90 0xa1	DESERET CAPITAL LETTER ER
					break;
				case 0x1044A:		//	ð‘Š	0xf0 0x90 0x91 0x8a	DESERET SMALL LETTER EL
					*p = 0x10422;	//	ð¢	0xf0 0x90 0x90 0xa2	DESERET CAPITAL LETTER EL
					break;
				case 0x1044B:		//	ð‘‹	0xf0 0x90 0x91 0x8b	DESERET SMALL LETTER EM
					*p = 0x10423;	//	ð£	0xf0 0x90 0x90 0xa3	DESERET CAPITAL LETTER EM
					break;
				case 0x1044C:		//	ð‘Œ	0xf0 0x90 0x91 0x8c	DESERET SMALL LETTER EN
					*p = 0x10424;	//	ð¤	0xf0 0x90 0x90 0xa4	DESERET CAPITAL LETTER EN
					break;
				case 0x1044D:		//	ð‘	0xf0 0x90 0x91 0x8d	DESERET SMALL LETTER ENG
					*p = 0x10425;	//	ð¥	0xf0 0x90 0x90 0xa5	DESERET CAPITAL LETTER ENG
					break;
				case 0x1044E:		//	ð‘Ž	0xf0 0x90 0x91 0x8e	DESERET SMALL LETTER OI
					*p = 0x10426;	//	ð¦	0xf0 0x90 0x90 0xa6	DESERET CAPITAL LETTER OI
					break;
				case 0x1044F:		//	ð‘	0xf0 0x90 0x91 0x8f	DESERET SMALL LETTER EW
					*p = 0x10427;	//	ð§	0xf0 0x90 0x90 0xa7	DESERET CAPITAL LETTER EW
					break;
				case 0x104D8:		//	ð“˜	0xf0 0x90 0x93 0x98	OSAGE SMALL LETTER A
					*p = 0x104B0;	//	ð’°	0xf0 0x90 0x92 0xb0	OSAGE CAPITAL LETTER A
					break;
				case 0x104D9:		//	ð“™	0xf0 0x90 0x93 0x99	OSAGE SMALL LETTER AI
					*p = 0x104B1;	//	ð’±	0xf0 0x90 0x92 0xb1	OSAGE CAPITAL LETTER AI
					break;
				case 0x104DA:		//	ð“š	0xf0 0x90 0x93 0x9a	OSAGE SMALL LETTER AIN
					*p = 0x104B2;	//	ð’²	0xf0 0x90 0x92 0xb2	OSAGE CAPITAL LETTER AIN
					break;
				case 0x104DB:		//	ð“›	0xf0 0x90 0x93 0x9b	OSAGE SMALL LETTER AH
					*p = 0x104B3;	//	ð’³	0xf0 0x90 0x92 0xb3	OSAGE CAPITAL LETTER AH
					break;
				case 0x104DC:		//	ð“œ	0xf0 0x90 0x93 0x9c	OSAGE SMALL LETTER BRA
					*p = 0x104B4;	//	ð’´	0xf0 0x90 0x92 0xb4	OSAGE CAPITAL LETTER BRA
					break;
				case 0x104DD:		//	ð“	0xf0 0x90 0x93 0x9d	OSAGE SMALL LETTER CHA
					*p = 0x104B5;	//	ð’µ	0xf0 0x90 0x92 0xb5	OSAGE CAPITAL LETTER CHA
					break;
				case 0x104DE:		//	ð“ž	0xf0 0x90 0x93 0x9e	OSAGE SMALL LETTER EHCHA
					*p = 0x104B6;	//	ð’¶	0xf0 0x90 0x92 0xb6	OSAGE CAPITAL LETTER EHCHA
					break;
				case 0x104DF:		//	ð“Ÿ	0xf0 0x90 0x93 0x9f	OSAGE SMALL LETTER E
					*p = 0x104B7;	//	ð’·	0xf0 0x90 0x92 0xb7	OSAGE CAPITAL LETTER E
					break;
				case 0x104E0:		//	ð“ 	0xf0 0x90 0x93 0xa0	OSAGE SMALL LETTER EIN
					*p = 0x104B8;	//	ð’¸	0xf0 0x90 0x92 0xb8	OSAGE CAPITAL LETTER EIN
					break;
				case 0x104E1:		//	ð“¡	0xf0 0x90 0x93 0xa1	OSAGE SMALL LETTER HA
					*p = 0x104B9;	//	ð’¹	0xf0 0x90 0x92 0xb9	OSAGE CAPITAL LETTER HA
					break;
				case 0x104E2:		//	ð“¢	0xf0 0x90 0x93 0xa2	OSAGE SMALL LETTER HYA
					*p = 0x104BA;	//	ð’º	0xf0 0x90 0x92 0xba	OSAGE CAPITAL LETTER HYA
					break;
				case 0x104E3:		//	ð“£	0xf0 0x90 0x93 0xa3	OSAGE SMALL LETTER I
					*p = 0x104BB;	//	ð’»	0xf0 0x90 0x92 0xbb	OSAGE CAPITAL LETTER I
					break;
				case 0x104E4:		//	ð“¤	0xf0 0x90 0x93 0xa4	OSAGE SMALL LETTER KA
					*p = 0x104BC;	//	ð’¼	0xf0 0x90 0x92 0xbc	OSAGE CAPITAL LETTER KA
					break;
				case 0x104E5:		//	ð“¥	0xf0 0x90 0x93 0xa5	OSAGE SMALL LETTER EHKA
					*p = 0x104BD;	//	ð’½	0xf0 0x90 0x92 0xbd	OSAGE CAPITAL LETTER EHKA
					break;
				case 0x104E6:		//	ð“¦	0xf0 0x90 0x93 0xa6	OSAGE SMALL LETTER KYA
					*p = 0x104BE;	//	ð’¾	0xf0 0x90 0x92 0xbe	OSAGE CAPITAL LETTER KYA
					break;
				case 0x104E7:		//	ð“§	0xf0 0x90 0x93 0xa7	OSAGE SMALL LETTER LA
					*p = 0x104BF;	//	ð’¿	0xf0 0x90 0x92 0xbf	OSAGE CAPITAL LETTER LA
					break;
				case 0x104E8:		//	ð“¨	0xf0 0x90 0x93 0xa8	OSAGE SMALL LETTER MA
					*p = 0x104C0;	//	ð“€	0xf0 0x90 0x93 0x80	OSAGE CAPITAL LETTER MA
					break;
				case 0x104E9:		//	ð“©	0xf0 0x90 0x93 0xa9	OSAGE SMALL LETTER NA
					*p = 0x104C1;	//	ð“	0xf0 0x90 0x93 0x81	OSAGE CAPITAL LETTER NA
					break;
				case 0x104EA:		//	ð“ª	0xf0 0x90 0x93 0xaa	OSAGE SMALL LETTER O
					*p = 0x104C2;	//	ð“‚	0xf0 0x90 0x93 0x82	OSAGE CAPITAL LETTER O
					break;
				case 0x104EB:		//	ð“«	0xf0 0x90 0x93 0xab	OSAGE SMALL LETTER OIN
					*p = 0x104C3;	//	ð“ƒ	0xf0 0x90 0x93 0x83	OSAGE CAPITAL LETTER OIN
					break;
				case 0x104EC:		//	ð“¬	0xf0 0x90 0x93 0xac	OSAGE SMALL LETTER PA
					*p = 0x104C4;	//	ð“„	0xf0 0x90 0x93 0x84	OSAGE CAPITAL LETTER PA
					break;
				case 0x104ED:		//	ð“­	0xf0 0x90 0x93 0xad	OSAGE SMALL LETTER EHPA
					*p = 0x104C5;	//	ð“…	0xf0 0x90 0x93 0x85	OSAGE CAPITAL LETTER EHPA
					break;
				case 0x104EE:		//	ð“®	0xf0 0x90 0x93 0xae	OSAGE SMALL LETTER SA
					*p = 0x104C6;	//	ð“†	0xf0 0x90 0x93 0x86	OSAGE CAPITAL LETTER SA
					break;
				case 0x104EF:		//	ð“¯	0xf0 0x90 0x93 0xaf	OSAGE SMALL LETTER SHA
					*p = 0x104C7;	//	ð“‡	0xf0 0x90 0x93 0x87	OSAGE CAPITAL LETTER SHA
					break;
				case 0x104F0:		//	ð“°	0xf0 0x90 0x93 0xb0	OSAGE SMALL LETTER TA
					*p = 0x104C8;	//	ð“ˆ	0xf0 0x90 0x93 0x88	OSAGE CAPITAL LETTER TA
					break;
				case 0x104F1:		//	ð“±	0xf0 0x90 0x93 0xb1	OSAGE SMALL LETTER EHTA
					*p = 0x104C9;	//	ð“‰	0xf0 0x90 0x93 0x89	OSAGE CAPITAL LETTER EHTA
					break;
				case 0x104F2:		//	ð“²	0xf0 0x90 0x93 0xb2	OSAGE SMALL LETTER TSA
					*p = 0x104CA;	//	ð“Š	0xf0 0x90 0x93 0x8a	OSAGE CAPITAL LETTER TSA
					break;
				case 0x104F3:		//	ð“³	0xf0 0x90 0x93 0xb3	OSAGE SMALL LETTER EHTSA
					*p = 0x104CB;	//	ð“‹	0xf0 0x90 0x93 0x8b	OSAGE CAPITAL LETTER EHTSA
					break;
				case 0x104F4:		//	ð“´	0xf0 0x90 0x93 0xb4	OSAGE SMALL LETTER TSHA
					*p = 0x104CC;	//	ð“Œ	0xf0 0x90 0x93 0x8c	OSAGE CAPITAL LETTER TSHA
					break;
				case 0x104F5:		//	ð“µ	0xf0 0x90 0x93 0xb5	OSAGE SMALL LETTER DHA
					*p = 0x104CD;	//	ð“	0xf0 0x90 0x93 0x8d	OSAGE CAPITAL LETTER DHA
					break;
				case 0x104F6:		//	ð“¶	0xf0 0x90 0x93 0xb6	OSAGE SMALL LETTER U
					*p = 0x104CE;	//	ð“Ž	0xf0 0x90 0x93 0x8e	OSAGE CAPITAL LETTER U
					break;
				case 0x104F7:		//	ð“·	0xf0 0x90 0x93 0xb7	OSAGE SMALL LETTER WA
					*p = 0x104CF;	//	ð“	0xf0 0x90 0x93 0x8f	OSAGE CAPITAL LETTER WA
					break;
				case 0x104F8:		//	ð“¸	0xf0 0x90 0x93 0xb8	OSAGE SMALL LETTER KHA
					*p = 0x104D0;	//	ð“	0xf0 0x90 0x93 0x90	OSAGE CAPITAL LETTER KHA
					break;
				case 0x104F9:		//	ð“¹	0xf0 0x90 0x93 0xb9	OSAGE SMALL LETTER GHA
					*p = 0x104D1;	//	ð“‘	0xf0 0x90 0x93 0x91	OSAGE CAPITAL LETTER GHA
					break;
				case 0x104FA:		//	ð“º	0xf0 0x90 0x93 0xba	OSAGE SMALL LETTER ZA
					*p = 0x104D2;	//	ð“’	0xf0 0x90 0x93 0x92	OSAGE CAPITAL LETTER ZA
					break;
				case 0x104FB:		//	ð“»	0xf0 0x90 0x93 0xbb	OSAGE SMALL LETTER ZHA
					*p = 0x104D3;	//	ð““	0xf0 0x90 0x93 0x93	OSAGE CAPITAL LETTER ZHA
					break;
				case 0x10CC0:		//	ð³€	0xf0 0x90 0xb3 0x80	OLD HUNGARIAN SMALL LETTER A
					*p = 0x10C80;	//	ð²€	0xf0 0x90 0xb2 0x80	OLD HUNGARIAN CAPITAL LETTER A
					break;
				case 0x10CC1:		//	ð³	0xf0 0x90 0xb3 0x81	OLD HUNGARIAN SMALL LETTER AA
					*p = 0x10C81;	//	ð²	0xf0 0x90 0xb2 0x81	OLD HUNGARIAN CAPITAL LETTER AA
					break;
				case 0x10CC2:		//	ð³‚	0xf0 0x90 0xb3 0x82	OLD HUNGARIAN SMALL LETTER EB
					*p = 0x10C82;	//	ð²‚	0xf0 0x90 0xb2 0x82	OLD HUNGARIAN CAPITAL LETTER EB
					break;
				case 0x10CC3:		//	ð³ƒ	0xf0 0x90 0xb3 0x83	OLD HUNGARIAN SMALL LETTER AMB
					*p = 0x10C83;	//	ð²ƒ	0xf0 0x90 0xb2 0x83	OLD HUNGARIAN CAPITAL LETTER AMB
					break;
				case 0x10CC4:		//	ð³„	0xf0 0x90 0xb3 0x84	OLD HUNGARIAN SMALL LETTER EC
					*p = 0x10C84;	//	ð²„	0xf0 0x90 0xb2 0x84	OLD HUNGARIAN CAPITAL LETTER EC
					break;
				case 0x10CC5:		//	ð³…	0xf0 0x90 0xb3 0x85	OLD HUNGARIAN SMALL LETTER ENC
					*p = 0x10C85;	//	ð²…	0xf0 0x90 0xb2 0x85	OLD HUNGARIAN CAPITAL LETTER ENC
					break;
				case 0x10CC6:		//	ð³†	0xf0 0x90 0xb3 0x86	OLD HUNGARIAN SMALL LETTER ECS
					*p = 0x10C86;	//	ð²†	0xf0 0x90 0xb2 0x86	OLD HUNGARIAN CAPITAL LETTER ECS
					break;
				case 0x10CC7:		//	ð³‡	0xf0 0x90 0xb3 0x87	OLD HUNGARIAN SMALL LETTER ED
					*p = 0x10C87;	//	ð²‡	0xf0 0x90 0xb2 0x87	OLD HUNGARIAN CAPITAL LETTER ED
					break;
				case 0x10CC8:		//	ð³ˆ	0xf0 0x90 0xb3 0x88	OLD HUNGARIAN SMALL LETTER AND
					*p = 0x10C88;	//	ð²ˆ	0xf0 0x90 0xb2 0x88	OLD HUNGARIAN CAPITAL LETTER AND
					break;
				case 0x10CC9:		//	ð³‰	0xf0 0x90 0xb3 0x89	OLD HUNGARIAN SMALL LETTER E
					*p = 0x10C89;	//	ð²‰	0xf0 0x90 0xb2 0x89	OLD HUNGARIAN CAPITAL LETTER E
					break;
				case 0x10CCA:		//	ð³Š	0xf0 0x90 0xb3 0x8a	OLD HUNGARIAN SMALL LETTER CLOSE E
					*p = 0x10C8A;	//	ð²Š	0xf0 0x90 0xb2 0x8a	OLD HUNGARIAN CAPITAL LETTER CLOSE E
					break;
				case 0x10CCB:		//	ð³‹	0xf0 0x90 0xb3 0x8b	OLD HUNGARIAN SMALL LETTER EE
					*p = 0x10C8B;	//	ð²‹	0xf0 0x90 0xb2 0x8b	OLD HUNGARIAN CAPITAL LETTER EE
					break;
				case 0x10CCC:		//	ð³Œ	0xf0 0x90 0xb3 0x8c	OLD HUNGARIAN SMALL LETTER EF
					*p = 0x10C8C;	//	ð²Œ	0xf0 0x90 0xb2 0x8c	OLD HUNGARIAN CAPITAL LETTER EF
					break;
				case 0x10CCD:		//	ð³	0xf0 0x90 0xb3 0x8d	OLD HUNGARIAN SMALL LETTER EG
					*p = 0x10C8D;	//	ð²	0xf0 0x90 0xb2 0x8d	OLD HUNGARIAN CAPITAL LETTER EG
					break;
				case 0x10CCE:		//	ð³Ž	0xf0 0x90 0xb3 0x8e	OLD HUNGARIAN SMALL LETTER EGY
					*p = 0x10C8E;	//	ð²Ž	0xf0 0x90 0xb2 0x8e	OLD HUNGARIAN CAPITAL LETTER EGY
					break;
				case 0x10CCF:		//	ð³	0xf0 0x90 0xb3 0x8f	OLD HUNGARIAN SMALL LETTER EH
					*p = 0x10C8F;	//	ð²	0xf0 0x90 0xb2 0x8f	OLD HUNGARIAN CAPITAL LETTER EH
					break;
				case 0x10CD0:		//	ð³	0xf0 0x90 0xb3 0x90	OLD HUNGARIAN SMALL LETTER I
					*p = 0x10C90;	//	ð²	0xf0 0x90 0xb2 0x90	OLD HUNGARIAN CAPITAL LETTER I
					break;
				case 0x10CD1:		//	ð³‘	0xf0 0x90 0xb3 0x91	OLD HUNGARIAN SMALL LETTER II
					*p = 0x10C91;	//	ð²‘	0xf0 0x90 0xb2 0x91	OLD HUNGARIAN CAPITAL LETTER II
					break;
				case 0x10CD2:		//	ð³’	0xf0 0x90 0xb3 0x92	OLD HUNGARIAN SMALL LETTER EJ
					*p = 0x10C92;	//	ð²’	0xf0 0x90 0xb2 0x92	OLD HUNGARIAN CAPITAL LETTER EJ
					break;
				case 0x10CD3:		//	ð³“	0xf0 0x90 0xb3 0x93	OLD HUNGARIAN SMALL LETTER EK
					*p = 0x10C93;	//	ð²“	0xf0 0x90 0xb2 0x93	OLD HUNGARIAN CAPITAL LETTER EK
					break;
				case 0x10CD4:		//	ð³”	0xf0 0x90 0xb3 0x94	OLD HUNGARIAN SMALL LETTER AK
					*p = 0x10C94;	//	ð²”	0xf0 0x90 0xb2 0x94	OLD HUNGARIAN CAPITAL LETTER AK
					break;
				case 0x10CD5:		//	ð³•	0xf0 0x90 0xb3 0x95	OLD HUNGARIAN SMALL LETTER UNK
					*p = 0x10C95;	//	ð²•	0xf0 0x90 0xb2 0x95	OLD HUNGARIAN CAPITAL LETTER UNK
					break;
				case 0x10CD6:		//	ð³–	0xf0 0x90 0xb3 0x96	OLD HUNGARIAN SMALL LETTER EL
					*p = 0x10C96;	//	ð²–	0xf0 0x90 0xb2 0x96	OLD HUNGARIAN CAPITAL LETTER EL
					break;
				case 0x10CD7:		//	ð³—	0xf0 0x90 0xb3 0x97	OLD HUNGARIAN SMALL LETTER ELY
					*p = 0x10C97;	//	ð²—	0xf0 0x90 0xb2 0x97	OLD HUNGARIAN CAPITAL LETTER ELY
					break;
				case 0x10CD8:		//	ð³˜	0xf0 0x90 0xb3 0x98	OLD HUNGARIAN SMALL LETTER EM
					*p = 0x10C98;	//	ð²˜	0xf0 0x90 0xb2 0x98	OLD HUNGARIAN CAPITAL LETTER EM
					break;
				case 0x10CD9:		//	ð³™	0xf0 0x90 0xb3 0x99	OLD HUNGARIAN SMALL LETTER EN
					*p = 0x10C99;	//	ð²™	0xf0 0x90 0xb2 0x99	OLD HUNGARIAN CAPITAL LETTER EN
					break;
				case 0x10CDA:		//	ð³š	0xf0 0x90 0xb3 0x9a	OLD HUNGARIAN SMALL LETTER ENY
					*p = 0x10C9A;	//	ð²š	0xf0 0x90 0xb2 0x9a	OLD HUNGARIAN CAPITAL LETTER ENY
					break;
				case 0x10CDB:		//	ð³›	0xf0 0x90 0xb3 0x9b	OLD HUNGARIAN SMALL LETTER O
					*p = 0x10C9B;	//	ð²›	0xf0 0x90 0xb2 0x9b	OLD HUNGARIAN CAPITAL LETTER O
					break;
				case 0x10CDC:		//	ð³œ	0xf0 0x90 0xb3 0x9c	OLD HUNGARIAN SMALL LETTER OO
					*p = 0x10C9C;	//	ð²œ	0xf0 0x90 0xb2 0x9c	OLD HUNGARIAN CAPITAL LETTER OO
					break;
				case 0x10CDD:		//	ð³	0xf0 0x90 0xb3 0x9d	OLD HUNGARIAN SMALL LETTER NIKOLSBURG OE
					*p = 0x10C9D;	//	ð²	0xf0 0x90 0xb2 0x9d	OLD HUNGARIAN CAPITAL LETTER NIKOLSBURG OE
					break;
				case 0x10CDE:		//	ð³ž	0xf0 0x90 0xb3 0x9e	OLD HUNGARIAN SMALL LETTER RUDIMENTA OE
					*p = 0x10C9E;	//	ð²ž	0xf0 0x90 0xb2 0x9e	OLD HUNGARIAN CAPITAL LETTER RUDIMENTA OE
					break;
				case 0x10CDF:		//	ð³Ÿ	0xf0 0x90 0xb3 0x9f	OLD HUNGARIAN SMALL LETTER OEE
					*p = 0x10C9F;	//	ð²Ÿ	0xf0 0x90 0xb2 0x9f	OLD HUNGARIAN CAPITAL LETTER OEE
					break;
				case 0x10CE0:		//	ð³ 	0xf0 0x90 0xb3 0xa0	OLD HUNGARIAN SMALL LETTER EP
					*p = 0x10CA0;	//	ð² 	0xf0 0x90 0xb2 0xa0	OLD HUNGARIAN CAPITAL LETTER EP
					break;
				case 0x10CE1:		//	ð³¡	0xf0 0x90 0xb3 0xa1	OLD HUNGARIAN SMALL LETTER EMP
					*p = 0x10CA1;	//	ð²¡	0xf0 0x90 0xb2 0xa1	OLD HUNGARIAN CAPITAL LETTER EMP
					break;
				case 0x10CE2:		//	ð³¢	0xf0 0x90 0xb3 0xa2	OLD HUNGARIAN SMALL LETTER ER
					*p = 0x10CA2;	//	ð²¢	0xf0 0x90 0xb2 0xa2	OLD HUNGARIAN CAPITAL LETTER ER
					break;
				case 0x10CE3:		//	ð³£	0xf0 0x90 0xb3 0xa3	OLD HUNGARIAN SMALL LETTER SHORT ER
					*p = 0x10CA3;	//	ð²£	0xf0 0x90 0xb2 0xa3	OLD HUNGARIAN CAPITAL LETTER SHORT ER
					break;
				case 0x10CE4:		//	ð³¤	0xf0 0x90 0xb3 0xa4	OLD HUNGARIAN SMALL LETTER ES
					*p = 0x10CA4;	//	ð²¤	0xf0 0x90 0xb2 0xa4	OLD HUNGARIAN CAPITAL LETTER ES
					break;
				case 0x10CE5:		//	ð³¥	0xf0 0x90 0xb3 0xa5	OLD HUNGARIAN SMALL LETTER ESZ
					*p = 0x10CA5;	//	ð²¥	0xf0 0x90 0xb2 0xa5	OLD HUNGARIAN CAPITAL LETTER ESZ
					break;
				case 0x10CE6:		//	ð³¦	0xf0 0x90 0xb3 0xa6	OLD HUNGARIAN SMALL LETTER ET
					*p = 0x10CA6;	//	ð²¦	0xf0 0x90 0xb2 0xa6	OLD HUNGARIAN CAPITAL LETTER ET
					break;
				case 0x10CE7:		//	ð³§	0xf0 0x90 0xb3 0xa7	OLD HUNGARIAN SMALL LETTER ENT
					*p = 0x10CA7;	//	ð²§	0xf0 0x90 0xb2 0xa7	OLD HUNGARIAN CAPITAL LETTER ENT
					break;
				case 0x10CE8:		//	ð³¨	0xf0 0x90 0xb3 0xa8	OLD HUNGARIAN SMALL LETTER ETY
					*p = 0x10CA8;	//	ð²¨	0xf0 0x90 0xb2 0xa8	OLD HUNGARIAN CAPITAL LETTER ETY
					break;
				case 0x10CE9:		//	ð³©	0xf0 0x90 0xb3 0xa9	OLD HUNGARIAN SMALL LETTER ECH
					*p = 0x10CA9;	//	ð²©	0xf0 0x90 0xb2 0xa9	OLD HUNGARIAN CAPITAL LETTER ECH
					break;
				case 0x10CEA:		//	ð³ª	0xf0 0x90 0xb3 0xaa	OLD HUNGARIAN SMALL LETTER U
					*p = 0x10CAA;	//	ð²ª	0xf0 0x90 0xb2 0xaa	OLD HUNGARIAN CAPITAL LETTER U
					break;
				case 0x10CEB:		//	ð³«	0xf0 0x90 0xb3 0xab	OLD HUNGARIAN SMALL LETTER UU
					*p = 0x10CAB;	//	ð²«	0xf0 0x90 0xb2 0xab	OLD HUNGARIAN CAPITAL LETTER UU
					break;
				case 0x10CEC:		//	ð³¬	0xf0 0x90 0xb3 0xac	OLD HUNGARIAN SMALL LETTER NIKOLSBURG UE
					*p = 0x10CAC;	//	ð²¬	0xf0 0x90 0xb2 0xac	OLD HUNGARIAN CAPITAL LETTER NIKOLSBURG UE
					break;
				case 0x10CED:		//	ð³­	0xf0 0x90 0xb3 0xad	OLD HUNGARIAN SMALL LETTER RUDIMENTA UE
					*p = 0x10CAD;	//	ð²­	0xf0 0x90 0xb2 0xad	OLD HUNGARIAN CAPITAL LETTER RUDIMENTA UE
					break;
				case 0x10CEE:		//	ð³®	0xf0 0x90 0xb3 0xae	OLD HUNGARIAN SMALL LETTER EV
					*p = 0x10CAE;	//	ð²®	0xf0 0x90 0xb2 0xae	OLD HUNGARIAN CAPITAL LETTER EV
					break;
				case 0x10CEF:		//	ð³¯	0xf0 0x90 0xb3 0xaf	OLD HUNGARIAN SMALL LETTER EZ
					*p = 0x10CAF;	//	ð²¯	0xf0 0x90 0xb2 0xaf	OLD HUNGARIAN CAPITAL LETTER EZ
					break;
				case 0x10CF0:		//	ð³°	0xf0 0x90 0xb3 0xb0	OLD HUNGARIAN SMALL LETTER EZS
					*p = 0x10CB0;	//	ð²°	0xf0 0x90 0xb2 0xb0	OLD HUNGARIAN CAPITAL LETTER EZS
					break;
				case 0x10CF1:		//	ð³±	0xf0 0x90 0xb3 0xb1	OLD HUNGARIAN SMALL LETTER ENT-SHAPED SIGN
					*p = 0x10CB1;	//	ð²±	0xf0 0x90 0xb2 0xb1	OLD HUNGARIAN CAPITAL LETTER ENT-SHAPED SIGN
					break;
				case 0x10CF2:		//	ð³²	0xf0 0x90 0xb3 0xb2	OLD HUNGARIAN SMALL LETTER US
					*p = 0x10CB2;	//	ð²²	0xf0 0x90 0xb2 0xb2	OLD HUNGARIAN CAPITAL LETTER US
					break;
				case 0x118C0:		//	ð‘£€	0xf0 0x91 0xa3 0x80	WARANG CITI SMALL LETTER NGAA
					*p = 0x118A0;	//	ð‘¢ 	0xf0 0x91 0xa2 0xa0	WARANG CITI CAPITAL LETTER NGAA
					break;
				case 0x118C1:		//	ð‘£	0xf0 0x91 0xa3 0x81	WARANG CITI SMALL LETTER A
					*p = 0x118A1;	//	ð‘¢¡	0xf0 0x91 0xa2 0xa1	WARANG CITI CAPITAL LETTER A
					break;
				case 0x118C2:		//	ð‘£‚	0xf0 0x91 0xa3 0x82	WARANG CITI SMALL LETTER WI
					*p = 0x118A2;	//	ð‘¢¢	0xf0 0x91 0xa2 0xa2	WARANG CITI CAPITAL LETTER WI
					break;
				case 0x118C3:		//	ð‘£ƒ	0xf0 0x91 0xa3 0x83	WARANG CITI SMALL LETTER YU
					*p = 0x118A3;	//	ð‘¢£	0xf0 0x91 0xa2 0xa3	WARANG CITI CAPITAL LETTER YU
					break;
				case 0x118C4:		//	ð‘£„	0xf0 0x91 0xa3 0x84	WARANG CITI SMALL LETTER YA
					*p = 0x118A4;	//	ð‘¢¤	0xf0 0x91 0xa2 0xa4	WARANG CITI CAPITAL LETTER YA
					break;
				case 0x118C5:		//	ð‘£…	0xf0 0x91 0xa3 0x85	WARANG CITI SMALL LETTER YO
					*p = 0x118A5;	//	ð‘¢¥	0xf0 0x91 0xa2 0xa5	WARANG CITI CAPITAL LETTER YO
					break;
				case 0x118C6:		//	ð‘£†	0xf0 0x91 0xa3 0x86	WARANG CITI SMALL LETTER II
					*p = 0x118A6;	//	ð‘¢¦	0xf0 0x91 0xa2 0xa6	WARANG CITI CAPITAL LETTER II
					break;
				case 0x118C7:		//	ð‘£‡	0xf0 0x91 0xa3 0x87	WARANG CITI SMALL LETTER UU
					*p = 0x118A7;	//	ð‘¢§	0xf0 0x91 0xa2 0xa7	WARANG CITI CAPITAL LETTER UU
					break;
				case 0x118C8:		//	ð‘£ˆ	0xf0 0x91 0xa3 0x88	WARANG CITI SMALL LETTER E
					*p = 0x118A8;	//	ð‘¢¨	0xf0 0x91 0xa2 0xa8	WARANG CITI CAPITAL LETTER E
					break;
				case 0x118C9:		//	ð‘£‰	0xf0 0x91 0xa3 0x89	WARANG CITI SMALL LETTER O
					*p = 0x118A9;	//	ð‘¢©	0xf0 0x91 0xa2 0xa9	WARANG CITI CAPITAL LETTER O
					break;
				case 0x118CA:		//	ð‘£Š	0xf0 0x91 0xa3 0x8a	WARANG CITI SMALL LETTER ANG
					*p = 0x118AA;	//	ð‘¢ª	0xf0 0x91 0xa2 0xaa	WARANG CITI CAPITAL LETTER ANG
					break;
				case 0x118CB:		//	ð‘£‹	0xf0 0x91 0xa3 0x8b	WARANG CITI SMALL LETTER GA
					*p = 0x118AB;	//	ð‘¢«	0xf0 0x91 0xa2 0xab	WARANG CITI CAPITAL LETTER GA
					break;
				case 0x118CC:		//	ð‘£Œ	0xf0 0x91 0xa3 0x8c	WARANG CITI SMALL LETTER KO
					*p = 0x118AC;	//	ð‘¢¬	0xf0 0x91 0xa2 0xac	WARANG CITI CAPITAL LETTER KO
					break;
				case 0x118CD:		//	ð‘£	0xf0 0x91 0xa3 0x8d	WARANG CITI SMALL LETTER ENY
					*p = 0x118AD;	//	ð‘¢­	0xf0 0x91 0xa2 0xad	WARANG CITI CAPITAL LETTER ENY
					break;
				case 0x118CE:		//	ð‘£Ž	0xf0 0x91 0xa3 0x8e	WARANG CITI SMALL LETTER YUJ
					*p = 0x118AE;	//	ð‘¢®	0xf0 0x91 0xa2 0xae	WARANG CITI CAPITAL LETTER YUJ
					break;
				case 0x118CF:		//	ð‘£	0xf0 0x91 0xa3 0x8f	WARANG CITI SMALL LETTER UC
					*p = 0x118AF;	//	ð‘¢¯	0xf0 0x91 0xa2 0xaf	WARANG CITI CAPITAL LETTER UC
					break;
				case 0x118D0:		//	ð‘£	0xf0 0x91 0xa3 0x90	WARANG CITI SMALL LETTER ENN
					*p = 0x118B0;	//	ð‘¢°	0xf0 0x91 0xa2 0xb0	WARANG CITI CAPITAL LETTER ENN
					break;
				case 0x118D1:		//	ð‘£‘	0xf0 0x91 0xa3 0x91	WARANG CITI SMALL LETTER ODD
					*p = 0x118B1;	//	ð‘¢±	0xf0 0x91 0xa2 0xb1	WARANG CITI CAPITAL LETTER ODD
					break;
				case 0x118D2:		//	ð‘£’	0xf0 0x91 0xa3 0x92	WARANG CITI SMALL LETTER TTE
					*p = 0x118B2;	//	ð‘¢²	0xf0 0x91 0xa2 0xb2	WARANG CITI CAPITAL LETTER TTE
					break;
				case 0x118D3:		//	ð‘£“	0xf0 0x91 0xa3 0x93	WARANG CITI SMALL LETTER NUNG
					*p = 0x118B3;	//	ð‘¢³	0xf0 0x91 0xa2 0xb3	WARANG CITI CAPITAL LETTER NUNG
					break;
				case 0x118D4:		//	ð‘£”	0xf0 0x91 0xa3 0x94	WARANG CITI SMALL LETTER DA
					*p = 0x118B4;	//	ð‘¢´	0xf0 0x91 0xa2 0xb4	WARANG CITI CAPITAL LETTER DA
					break;
				case 0x118D5:		//	ð‘£•	0xf0 0x91 0xa3 0x95	WARANG CITI SMALL LETTER AT
					*p = 0x118B5;	//	ð‘¢µ	0xf0 0x91 0xa2 0xb5	WARANG CITI CAPITAL LETTER AT
					break;
				case 0x118D6:		//	ð‘£–	0xf0 0x91 0xa3 0x96	WARANG CITI SMALL LETTER AM
					*p = 0x118B6;	//	ð‘¢¶	0xf0 0x91 0xa2 0xb6	WARANG CITI CAPITAL LETTER AM
					break;
				case 0x118D7:		//	ð‘£—	0xf0 0x91 0xa3 0x97	WARANG CITI SMALL LETTER BU
					*p = 0x118B7;	//	ð‘¢·	0xf0 0x91 0xa2 0xb7	WARANG CITI CAPITAL LETTER BU
					break;
				case 0x118D8:		//	ð‘£˜	0xf0 0x91 0xa3 0x98	WARANG CITI SMALL LETTER PU
					*p = 0x118B8;	//	ð‘¢¸	0xf0 0x91 0xa2 0xb8	WARANG CITI CAPITAL LETTER PU
					break;
				case 0x118D9:		//	ð‘£™	0xf0 0x91 0xa3 0x99	WARANG CITI SMALL LETTER HIYO
					*p = 0x118B9;	//	ð‘¢¹	0xf0 0x91 0xa2 0xb9	WARANG CITI CAPITAL LETTER HIYO
					break;
				case 0x118DA:		//	ð‘£š	0xf0 0x91 0xa3 0x9a	WARANG CITI SMALL LETTER HOLO
					*p = 0x118BA;	//	ð‘¢º	0xf0 0x91 0xa2 0xba	WARANG CITI CAPITAL LETTER HOLO
					break;
				case 0x118DB:		//	ð‘£›	0xf0 0x91 0xa3 0x9b	WARANG CITI SMALL LETTER HORR
					*p = 0x118BB;	//	ð‘¢»	0xf0 0x91 0xa2 0xbb	WARANG CITI CAPITAL LETTER HORR
					break;
				case 0x118DC:		//	ð‘£œ	0xf0 0x91 0xa3 0x9c	WARANG CITI SMALL LETTER HAR
					*p = 0x118BC;	//	ð‘¢¼	0xf0 0x91 0xa2 0xbc	WARANG CITI CAPITAL LETTER HAR
					break;
				case 0x118DD:		//	ð‘£	0xf0 0x91 0xa3 0x9d	WARANG CITI SMALL LETTER SSUU
					*p = 0x118BD;	//	ð‘¢½	0xf0 0x91 0xa2 0xbd	WARANG CITI CAPITAL LETTER SSUU
					break;
				case 0x118DE:		//	ð‘£ž	0xf0 0x91 0xa3 0x9e	WARANG CITI SMALL LETTER SII
					*p = 0x118BE;	//	ð‘¢¾	0xf0 0x91 0xa2 0xbe	WARANG CITI CAPITAL LETTER SII
					break;
				case 0x118DF:		//	ð‘£Ÿ	0xf0 0x91 0xa3 0x9f	WARANG CITI SMALL LETTER VIYO
					*p = 0x118BF;	//	ð‘¢¿	0xf0 0x91 0xa2 0xbf	WARANG CITI CAPITAL LETTER VIYO
					break;
				case 0x16E60:		//	ð–¹ 	0xf0 0x96 0xb9 0xa0	MEDEFAIDRIN SMALL LETTER M
					*p = 0x16E40;	//	ð–¹€	0xf0 0x96 0xb9 0x80	MEDEFAIDRIN CAPITAL LETTER M
					break;
				case 0x16E61:		//	ð–¹¡	0xf0 0x96 0xb9 0xa1	MEDEFAIDRIN SMALL LETTER S
					*p = 0x16E41;	//	ð–¹	0xf0 0x96 0xb9 0x81	MEDEFAIDRIN CAPITAL LETTER S
					break;
				case 0x16E62:		//	ð–¹¢	0xf0 0x96 0xb9 0xa2	MEDEFAIDRIN SMALL LETTER V
					*p = 0x16E42;	//	ð–¹‚	0xf0 0x96 0xb9 0x82	MEDEFAIDRIN CAPITAL LETTER V
					break;
				case 0x16E63:		//	ð–¹£	0xf0 0x96 0xb9 0xa3	MEDEFAIDRIN SMALL LETTER W
					*p = 0x16E43;	//	ð–¹ƒ	0xf0 0x96 0xb9 0x83	MEDEFAIDRIN CAPITAL LETTER W
					break;
				case 0x16E64:		//	ð–¹¤	0xf0 0x96 0xb9 0xa4	MEDEFAIDRIN SMALL LETTER ATIU
					*p = 0x16E44;	//	ð–¹„	0xf0 0x96 0xb9 0x84	MEDEFAIDRIN CAPITAL LETTER ATIU
					break;
				case 0x16E65:		//	ð–¹¥	0xf0 0x96 0xb9 0xa5	MEDEFAIDRIN SMALL LETTER Z
					*p = 0x16E45;	//	ð–¹…	0xf0 0x96 0xb9 0x85	MEDEFAIDRIN CAPITAL LETTER Z
					break;
				case 0x16E66:		//	ð–¹¦	0xf0 0x96 0xb9 0xa6	MEDEFAIDRIN SMALL LETTER KP
					*p = 0x16E46;	//	ð–¹†	0xf0 0x96 0xb9 0x86	MEDEFAIDRIN CAPITAL LETTER KP
					break;
				case 0x16E67:		//	ð–¹§	0xf0 0x96 0xb9 0xa7	MEDEFAIDRIN SMALL LETTER P
					*p = 0x16E47;	//	ð–¹‡	0xf0 0x96 0xb9 0x87	MEDEFAIDRIN CAPITAL LETTER P
					break;
				case 0x16E68:		//	ð–¹¨	0xf0 0x96 0xb9 0xa8	MEDEFAIDRIN SMALL LETTER T
					*p = 0x16E48;	//	ð–¹ˆ	0xf0 0x96 0xb9 0x88	MEDEFAIDRIN CAPITAL LETTER T
					break;
				case 0x16E69:		//	ð–¹©	0xf0 0x96 0xb9 0xa9	MEDEFAIDRIN SMALL LETTER G
					*p = 0x16E49;	//	ð–¹‰	0xf0 0x96 0xb9 0x89	MEDEFAIDRIN CAPITAL LETTER G
					break;
				case 0x16E6A:		//	ð–¹ª	0xf0 0x96 0xb9 0xaa	MEDEFAIDRIN SMALL LETTER F
					*p = 0x16E4A;	//	ð–¹Š	0xf0 0x96 0xb9 0x8a	MEDEFAIDRIN CAPITAL LETTER F
					break;
				case 0x16E6B:		//	ð–¹«	0xf0 0x96 0xb9 0xab	MEDEFAIDRIN SMALL LETTER I
					*p = 0x16E4B;	//	ð–¹‹	0xf0 0x96 0xb9 0x8b	MEDEFAIDRIN CAPITAL LETTER I
					break;
				case 0x16E6C:		//	ð–¹¬	0xf0 0x96 0xb9 0xac	MEDEFAIDRIN SMALL LETTER K
					*p = 0x16E4C;	//	ð–¹Œ	0xf0 0x96 0xb9 0x8c	MEDEFAIDRIN CAPITAL LETTER K
					break;
				case 0x16E6D:		//	ð–¹­	0xf0 0x96 0xb9 0xad	MEDEFAIDRIN SMALL LETTER A
					*p = 0x16E4D;	//	ð–¹	0xf0 0x96 0xb9 0x8d	MEDEFAIDRIN CAPITAL LETTER A
					break;
				case 0x16E6E:		//	ð–¹®	0xf0 0x96 0xb9 0xae	MEDEFAIDRIN SMALL LETTER J
					*p = 0x16E4E;	//	ð–¹Ž	0xf0 0x96 0xb9 0x8e	MEDEFAIDRIN CAPITAL LETTER J
					break;
				case 0x16E6F:		//	ð–¹¯	0xf0 0x96 0xb9 0xaf	MEDEFAIDRIN SMALL LETTER E
					*p = 0x16E4F;	//	ð–¹	0xf0 0x96 0xb9 0x8f	MEDEFAIDRIN CAPITAL LETTER E
					break;
				case 0x16E70:		//	ð–¹°	0xf0 0x96 0xb9 0xb0	MEDEFAIDRIN SMALL LETTER B
					*p = 0x16E50;	//	ð–¹	0xf0 0x96 0xb9 0x90	MEDEFAIDRIN CAPITAL LETTER B
					break;
				case 0x16E71:		//	ð–¹±	0xf0 0x96 0xb9 0xb1	MEDEFAIDRIN SMALL LETTER C
					*p = 0x16E51;	//	ð–¹‘	0xf0 0x96 0xb9 0x91	MEDEFAIDRIN CAPITAL LETTER C
					break;
				case 0x16E72:		//	ð–¹²	0xf0 0x96 0xb9 0xb2	MEDEFAIDRIN SMALL LETTER U
					*p = 0x16E52;	//	ð–¹’	0xf0 0x96 0xb9 0x92	MEDEFAIDRIN CAPITAL LETTER U
					break;
				case 0x16E73:		//	ð–¹³	0xf0 0x96 0xb9 0xb3	MEDEFAIDRIN SMALL LETTER YU
					*p = 0x16E53;	//	ð–¹“	0xf0 0x96 0xb9 0x93	MEDEFAIDRIN CAPITAL LETTER YU
					break;
				case 0x16E74:		//	ð–¹´	0xf0 0x96 0xb9 0xb4	MEDEFAIDRIN SMALL LETTER L
					*p = 0x16E54;	//	ð–¹”	0xf0 0x96 0xb9 0x94	MEDEFAIDRIN CAPITAL LETTER L
					break;
				case 0x16E75:		//	ð–¹µ	0xf0 0x96 0xb9 0xb5	MEDEFAIDRIN SMALL LETTER Q
					*p = 0x16E55;	//	ð–¹•	0xf0 0x96 0xb9 0x95	MEDEFAIDRIN CAPITAL LETTER Q
					break;
				case 0x16E76:		//	ð–¹¶	0xf0 0x96 0xb9 0xb6	MEDEFAIDRIN SMALL LETTER HP
					*p = 0x16E56;	//	ð–¹–	0xf0 0x96 0xb9 0x96	MEDEFAIDRIN CAPITAL LETTER HP
					break;
				case 0x16E77:		//	ð–¹·	0xf0 0x96 0xb9 0xb7	MEDEFAIDRIN SMALL LETTER NY
					*p = 0x16E57;	//	ð–¹—	0xf0 0x96 0xb9 0x97	MEDEFAIDRIN CAPITAL LETTER NY
					break;
				case 0x16E78:		//	ð–¹¸	0xf0 0x96 0xb9 0xb8	MEDEFAIDRIN SMALL LETTER X
					*p = 0x16E58;	//	ð–¹˜	0xf0 0x96 0xb9 0x98	MEDEFAIDRIN CAPITAL LETTER X
					break;
				case 0x16E79:		//	ð–¹¹	0xf0 0x96 0xb9 0xb9	MEDEFAIDRIN SMALL LETTER D
					*p = 0x16E59;	//	ð–¹™	0xf0 0x96 0xb9 0x99	MEDEFAIDRIN CAPITAL LETTER D
					break;
				case 0x16E7A:		//	ð–¹º	0xf0 0x96 0xb9 0xba	MEDEFAIDRIN SMALL LETTER OE
					*p = 0x16E5A;	//	ð–¹š	0xf0 0x96 0xb9 0x9a	MEDEFAIDRIN CAPITAL LETTER OE
					break;
				case 0x16E7B:		//	ð–¹»	0xf0 0x96 0xb9 0xbb	MEDEFAIDRIN SMALL LETTER N
					*p = 0x16E5B;	//	ð–¹›	0xf0 0x96 0xb9 0x9b	MEDEFAIDRIN CAPITAL LETTER N
					break;
				case 0x16E7C:		//	ð–¹¼	0xf0 0x96 0xb9 0xbc	MEDEFAIDRIN SMALL LETTER R
					*p = 0x16E5C;	//	ð–¹œ	0xf0 0x96 0xb9 0x9c	MEDEFAIDRIN CAPITAL LETTER R
					break;
				case 0x16E7D:		//	ð–¹½	0xf0 0x96 0xb9 0xbd	MEDEFAIDRIN SMALL LETTER O
					*p = 0x16E5D;	//	ð–¹	0xf0 0x96 0xb9 0x9d	MEDEFAIDRIN CAPITAL LETTER O
					break;
				case 0x16E7E:		//	ð–¹¾	0xf0 0x96 0xb9 0xbe	MEDEFAIDRIN SMALL LETTER AI
					*p = 0x16E5E;	//	ð–¹ž	0xf0 0x96 0xb9 0x9e	MEDEFAIDRIN CAPITAL LETTER AI
					break;
				case 0x16E7F:		//	ð–¹¿	0xf0 0x96 0xb9 0xbf	MEDEFAIDRIN SMALL LETTER Y
					*p = 0x16E5F;	//	ð–¹Ÿ	0xf0 0x96 0xb9 0x9f	MEDEFAIDRIN CAPITAL LETTER Y
					break;
				case 0x1E922:		//	ðž¤¢	0xf0 0x9e 0xa4 0xa2	ADLAM SMALL LETTER ALIF
					*p = 0x1E900;	//	ðž¤€	0xf0 0x9e 0xa4 0x80	ADLAM CAPITAL LETTER ALIF
					break;
				case 0x1E923:		//	ðž¤£	0xf0 0x9e 0xa4 0xa3	ADLAM SMALL LETTER DAALI
					*p = 0x1E901;	//	ðž¤	0xf0 0x9e 0xa4 0x81	ADLAM CAPITAL LETTER DAALI
					break;
				case 0x1E924:		//	ðž¤¤	0xf0 0x9e 0xa4 0xa4	ADLAM SMALL LETTER LAAM
					*p = 0x1E902;	//	ðž¤‚	0xf0 0x9e 0xa4 0x82	ADLAM CAPITAL LETTER LAAM
					break;
				case 0x1E925:		//	ðž¤¥	0xf0 0x9e 0xa4 0xa5	ADLAM SMALL LETTER MIIM
					*p = 0x1E903;	//	ðž¤ƒ	0xf0 0x9e 0xa4 0x83	ADLAM CAPITAL LETTER MIIM
					break;
				case 0x1E926:		//	ðž¤¦	0xf0 0x9e 0xa4 0xa6	ADLAM SMALL LETTER BA
					*p = 0x1E904;	//	ðž¤„	0xf0 0x9e 0xa4 0x84	ADLAM CAPITAL LETTER BA
					break;
				case 0x1E927:		//	ðž¤§	0xf0 0x9e 0xa4 0xa7	ADLAM SMALL LETTER SINNYIIYHE
					*p = 0x1E905;	//	ðž¤…	0xf0 0x9e 0xa4 0x85	ADLAM CAPITAL LETTER SINNYIIYHE
					break;
				case 0x1E928:		//	ðž¤¨	0xf0 0x9e 0xa4 0xa8	ADLAM SMALL LETTER PE
					*p = 0x1E906;	//	ðž¤†	0xf0 0x9e 0xa4 0x86	ADLAM CAPITAL LETTER PE
					break;
				case 0x1E929:		//	ðž¤©	0xf0 0x9e 0xa4 0xa9	ADLAM SMALL LETTER BHE
					*p = 0x1E907;	//	ðž¤‡	0xf0 0x9e 0xa4 0x87	ADLAM CAPITAL LETTER BHE
					break;
				case 0x1E92A:		//	ðž¤ª	0xf0 0x9e 0xa4 0xaa	ADLAM SMALL LETTER RA
					*p = 0x1E908;	//	ðž¤ˆ	0xf0 0x9e 0xa4 0x88	ADLAM CAPITAL LETTER RA
					break;
				case 0x1E92B:		//	ðž¤«	0xf0 0x9e 0xa4 0xab	ADLAM SMALL LETTER E
					*p = 0x1E909;	//	ðž¤‰	0xf0 0x9e 0xa4 0x89	ADLAM CAPITAL LETTER E
					break;
				case 0x1E92C:		//	ðž¤¬	0xf0 0x9e 0xa4 0xac	ADLAM SMALL LETTER FA
					*p = 0x1E90A;	//	ðž¤Š	0xf0 0x9e 0xa4 0x8a	ADLAM CAPITAL LETTER FA
					break;
				case 0x1E92D:		//	ðž¤­	0xf0 0x9e 0xa4 0xad	ADLAM SMALL LETTER I
					*p = 0x1E90B;	//	ðž¤‹	0xf0 0x9e 0xa4 0x8b	ADLAM CAPITAL LETTER I
					break;
				case 0x1E92E:		//	ðž¤®	0xf0 0x9e 0xa4 0xae	ADLAM SMALL LETTER O
					*p = 0x1E90C;	//	ðž¤Œ	0xf0 0x9e 0xa4 0x8c	ADLAM CAPITAL LETTER O
					break;
				case 0x1E92F:		//	ðž¤¯	0xf0 0x9e 0xa4 0xaf	ADLAM SMALL LETTER DHA
					*p = 0x1E90D;	//	ðž¤	0xf0 0x9e 0xa4 0x8d	ADLAM CAPITAL LETTER DHA
					break;
				case 0x1E930:		//	ðž¤°	0xf0 0x9e 0xa4 0xb0	ADLAM SMALL LETTER YHE
					*p = 0x1E90E;	//	ðž¤Ž	0xf0 0x9e 0xa4 0x8e	ADLAM CAPITAL LETTER YHE
					break;
				case 0x1E931:		//	ðž¤±	0xf0 0x9e 0xa4 0xb1	ADLAM SMALL LETTER WAW
					*p = 0x1E90F;	//	ðž¤	0xf0 0x9e 0xa4 0x8f	ADLAM CAPITAL LETTER WAW
					break;
				case 0x1E932:		//	ðž¤²	0xf0 0x9e 0xa4 0xb2	ADLAM SMALL LETTER NUN
					*p = 0x1E910;	//	ðž¤	0xf0 0x9e 0xa4 0x90	ADLAM CAPITAL LETTER NUN
					break;
				case 0x1E933:		//	ðž¤³	0xf0 0x9e 0xa4 0xb3	ADLAM SMALL LETTER KAF
					*p = 0x1E911;	//	ðž¤‘	0xf0 0x9e 0xa4 0x91	ADLAM CAPITAL LETTER KAF
					break;
				case 0x1E934:		//	ðž¤´	0xf0 0x9e 0xa4 0xb4	ADLAM SMALL LETTER YA
					*p = 0x1E912;	//	ðž¤’	0xf0 0x9e 0xa4 0x92	ADLAM CAPITAL LETTER YA
					break;
				case 0x1E935:		//	ðž¤µ	0xf0 0x9e 0xa4 0xb5	ADLAM SMALL LETTER U
					*p = 0x1E913;	//	ðž¤“	0xf0 0x9e 0xa4 0x93	ADLAM CAPITAL LETTER U
					break;
				case 0x1E936:		//	ðž¤¶	0xf0 0x9e 0xa4 0xb6	ADLAM SMALL LETTER JIIM
					*p = 0x1E914;	//	ðž¤”	0xf0 0x9e 0xa4 0x94	ADLAM CAPITAL LETTER JIIM
					break;
				case 0x1E937:		//	ðž¤·	0xf0 0x9e 0xa4 0xb7	ADLAM SMALL LETTER CHI
					*p = 0x1E915;	//	ðž¤•	0xf0 0x9e 0xa4 0x95	ADLAM CAPITAL LETTER CHI
					break;
				case 0x1E938:		//	ðž¤¸	0xf0 0x9e 0xa4 0xb8	ADLAM SMALL LETTER HA
					*p = 0x1E916;	//	ðž¤–	0xf0 0x9e 0xa4 0x96	ADLAM CAPITAL LETTER HA
					break;
				case 0x1E939:		//	ðž¤¹	0xf0 0x9e 0xa4 0xb9	ADLAM SMALL LETTER QAAF
					*p = 0x1E917;	//	ðž¤—	0xf0 0x9e 0xa4 0x97	ADLAM CAPITAL LETTER QAAF
					break;
				case 0x1E93A:		//	ðž¤º	0xf0 0x9e 0xa4 0xba	ADLAM SMALL LETTER GA
					*p = 0x1E918;	//	ðž¤˜	0xf0 0x9e 0xa4 0x98	ADLAM CAPITAL LETTER GA
					break;
				case 0x1E93B:		//	ðž¤»	0xf0 0x9e 0xa4 0xbb	ADLAM SMALL LETTER NYA
					*p = 0x1E919;	//	ðž¤™	0xf0 0x9e 0xa4 0x99	ADLAM CAPITAL LETTER NYA
					break;
				case 0x1E93C:		//	ðž¤¼	0xf0 0x9e 0xa4 0xbc	ADLAM SMALL LETTER TU
					*p = 0x1E91A;	//	ðž¤š	0xf0 0x9e 0xa4 0x9a	ADLAM CAPITAL LETTER TU
					break;
				case 0x1E93D:		//	ðž¤½	0xf0 0x9e 0xa4 0xbd	ADLAM SMALL LETTER NHA
					*p = 0x1E91B;	//	ðž¤›	0xf0 0x9e 0xa4 0x9b	ADLAM CAPITAL LETTER NHA
					break;
				case 0x1E93E:		//	ðž¤¾	0xf0 0x9e 0xa4 0xbe	ADLAM SMALL LETTER VA
					*p = 0x1E91C;	//	ðž¤œ	0xf0 0x9e 0xa4 0x9c	ADLAM CAPITAL LETTER VA
					break;
				case 0x1E93F:		//	ðž¤¿	0xf0 0x9e 0xa4 0xbf	ADLAM SMALL LETTER KHA
					*p = 0x1E91D;	//	ðž¤	0xf0 0x9e 0xa4 0x9d	ADLAM CAPITAL LETTER KHA
					break;
				case 0x1E940:		//	ðž¥€	0xf0 0x9e 0xa5 0x80	ADLAM SMALL LETTER GBE
					*p = 0x1E91E;	//	ðž¤ž	0xf0 0x9e 0xa4 0x9e	ADLAM CAPITAL LETTER GBE
					break;
				case 0x1E941:		//	ðž¥	0xf0 0x9e 0xa5 0x81	ADLAM SMALL LETTER ZAL
					*p = 0x1E91F;	//	ðž¤Ÿ	0xf0 0x9e 0xa4 0x9f	ADLAM CAPITAL LETTER ZAL
					break;
				case 0x1E942:		//	ðž¥‚	0xf0 0x9e 0xa5 0x82	ADLAM SMALL LETTER KPO
					*p = 0x1E920;	//	ðž¤ 	0xf0 0x9e 0xa4 0xa0	ADLAM CAPITAL LETTER KPO
					break;
				case 0x1E943:		//	ðž¥ƒ	0xf0 0x9e 0xa5 0x83	ADLAM SMALL LETTER SHA
					*p = 0x1E921;	//	ðž¤¡	0xf0 0x9e 0xa4 0xa1	ADLAM CAPITAL LETTER SHA
					break;
				}
			}
			p++;
		}
	}
	return pUtf32;
}
Utf32Char* StrToLwrUtf32(Utf32Char* pUtf32)
{
	Utf32Char* p = pUtf32;
	if (pUtf32 && *pUtf32) {
		while (*p) {
			if ((*p >= 0x41) && (*p <= 0x5a)) /* US ASCII */
				(*p) += 0x20;
			else {
				switch (*p) {
				case 0x00C0:		//	Ã€	0xc3 0x80	LATIN CAPITAL LETTER A WITH GRAVE
					*p = 0x00E0;	//	Ã 	0xc3 0xa0	LATIN SMALL LETTER A WITH GRAVE
					break;
				case 0x00C1:		//	Ã	0xc3 0x81	LATIN CAPITAL LETTER A WITH ACUTE
					*p = 0x00E1;	//	Ã¡	0xc3 0xa1	LATIN SMALL LETTER A WITH ACUTE
					break;
				case 0x00C2:		//	Ã‚	0xc3 0x82	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
					*p = 0x00E2;	//	Ã¢	0xc3 0xa2	LATIN SMALL LETTER A WITH CIRCUMFLEX
					break;
				case 0x00C3:		//	Ãƒ	0xc3 0x83	LATIN CAPITAL LETTER A WITH TILDE
					*p = 0x00E3;	//	Ã£	0xc3 0xa3	LATIN SMALL LETTER A WITH TILDE
					break;
				case 0x00C4:		//	Ã„	0xc3 0x84	LATIN CAPITAL LETTER A WITH DIAERESIS
					*p = 0x00E4;	//	Ã¤	0xc3 0xa4	LATIN SMALL LETTER A WITH DIAERESIS
					break;
				case 0x00C5:		//	Ã…	0xc3 0x85	LATIN CAPITAL LETTER A WITH RING ABOVE
					*p = 0x00E5;	//	Ã¥	0xc3 0xa5	LATIN SMALL LETTER A WITH RING ABOVE
					break;
				case 0x00C6:		//	Ã†	0xc3 0x86	LATIN CAPITAL LETTER AE
					*p = 0x00E6;	//	Ã¦	0xc3 0xa6	LATIN SMALL LETTER AE
					break;
				case 0x00C7:		//	Ã‡	0xc3 0x87	LATIN CAPITAL LETTER C WITH CEDILLA
					*p = 0x00E7;	//	Ã§	0xc3 0xa7	LATIN SMALL LETTER C WITH CEDILLA
					break;
				case 0x00C8:		//	Ãˆ	0xc3 0x88	LATIN CAPITAL LETTER E WITH GRAVE
					*p = 0x00E8;	//	Ã¨	0xc3 0xa8	LATIN SMALL LETTER E WITH GRAVE
					break;
				case 0x00C9:		//	Ã‰	0xc3 0x89	LATIN CAPITAL LETTER E WITH ACUTE
					*p = 0x00E9;	//	Ã©	0xc3 0xa9	LATIN SMALL LETTER E WITH ACUTE
					break;
				case 0x00CA:		//	ÃŠ	0xc3 0x8a	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
					*p = 0x00EA;	//	Ãª	0xc3 0xaa	LATIN SMALL LETTER E WITH CIRCUMFLEX
					break;
				case 0x00CB:		//	Ã‹	0xc3 0x8b	LATIN CAPITAL LETTER E WITH DIAERESIS
					*p = 0x00EB;	//	Ã«	0xc3 0xab	LATIN SMALL LETTER E WITH DIAERESIS
					break;
				case 0x00CC:		//	ÃŒ	0xc3 0x8c	LATIN CAPITAL LETTER I WITH GRAVE
					*p = 0x00EC;	//	Ã¬	0xc3 0xac	LATIN SMALL LETTER I WITH GRAVE
					break;
				case 0x00CD:		//	Ã	0xc3 0x8d	LATIN CAPITAL LETTER I WITH ACUTE
					*p = 0x00ED;	//	Ã­	0xc3 0xad	LATIN SMALL LETTER I WITH ACUTE
					break;
				case 0x00CE:		//	ÃŽ	0xc3 0x8e	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
					*p = 0x00EE;	//	Ã®	0xc3 0xae	LATIN SMALL LETTER I WITH CIRCUMFLEX
					break;
				case 0x00CF:		//	Ã	0xc3 0x8f	LATIN CAPITAL LETTER I WITH DIAERESIS
					*p = 0x00EF;	//	Ã¯	0xc3 0xaf	LATIN SMALL LETTER I WITH DIAERESIS
					break;
				case 0x00D0:		//	Ã	0xc3 0x90	LATIN CAPITAL LETTER ETH
					*p = 0x00F0;	//	Ã°	0xc3 0xb0	LATIN SMALL LETTER ETH
					break;
				case 0x00D1:		//	Ã‘	0xc3 0x91	LATIN CAPITAL LETTER N WITH TILDE
					*p = 0x00F1;	//	Ã±	0xc3 0xb1	LATIN SMALL LETTER N WITH TILDE
					break;
				case 0x00D2:		//	Ã’	0xc3 0x92	LATIN CAPITAL LETTER O WITH GRAVE
					*p = 0x00F2;	//	Ã²	0xc3 0xb2	LATIN SMALL LETTER O WITH GRAVE
					break;
				case 0x00D3:		//	Ã“	0xc3 0x93	LATIN CAPITAL LETTER O WITH ACUTE
					*p = 0x00F3;	//	Ã³	0xc3 0xb3	LATIN SMALL LETTER O WITH ACUTE
					break;
				case 0x00D4:		//	Ã”	0xc3 0x94	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
					*p = 0x00F4;	//	Ã´	0xc3 0xb4	LATIN SMALL LETTER O WITH CIRCUMFLEX
					break;
				case 0x00D5:		//	Ã•	0xc3 0x95	LATIN CAPITAL LETTER O WITH TILDE
					*p = 0x00F5;	//	Ãµ	0xc3 0xb5	LATIN SMALL LETTER O WITH TILDE
					break;
				case 0x00D6:		//	Ã–	0xc3 0x96	LATIN CAPITAL LETTER O WITH DIAERESIS
					*p = 0x00F6;	//	Ã¶	0xc3 0xb6	LATIN SMALL LETTER O WITH DIAERESIS
					break;
				case 0x00D8:		//	Ã˜	0xc3 0x98	LATIN CAPITAL LETTER O WITH STROKE
					*p = 0x00F8;	//	Ã¸	0xc3 0xb8	LATIN SMALL LETTER O WITH STROKE
					break;
				case 0x00D9:		//	Ã™	0xc3 0x99	LATIN CAPITAL LETTER U WITH GRAVE
					*p = 0x00F9;	//	Ã¹	0xc3 0xb9	LATIN SMALL LETTER U WITH GRAVE
					break;
				case 0x00DA:		//	Ãš	0xc3 0x9a	LATIN CAPITAL LETTER U WITH ACUTE
					*p = 0x00FA;	//	Ãº	0xc3 0xba	LATIN SMALL LETTER U WITH ACUTE
					break;
				case 0x00DB:		//	Ã›	0xc3 0x9b	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
					*p = 0x00FB;	//	Ã»	0xc3 0xbb	LATIN SMALL LETTER U WITH CIRCUMFLEX
					break;
				case 0x00DC:		//	Ãœ	0xc3 0x9c	LATIN CAPITAL LETTER U WITH DIAERESIS
					*p = 0x00FC;	//	Ã¼	0xc3 0xbc	LATIN SMALL LETTER U WITH DIAERESIS
					break;
				case 0x00DD:		//	Ã	0xc3 0x9d	LATIN CAPITAL LETTER Y WITH ACUTE
					*p = 0x00FD;	//	Ã½	0xc3 0xbd	LATIN SMALL LETTER Y WITH ACUTE
					break;
				case 0x00DE:		//	Ãž	0xc3 0x9e	LATIN CAPITAL LETTER THORN
					*p = 0x00FE;	//	Ã¾	0xc3 0xbe	LATIN SMALL LETTER THORN
					break;
				case 0x0100:		//	Ä€	0xc4 0x80	LATIN CAPITAL LETTER A WITH MACRON
					*p = 0x0101;	//	Ä	0xc4 0x81	LATIN SMALL LETTER A WITH MACRON
					break;
				case 0x0102:		//	Ä‚	0xc4 0x82	LATIN CAPITAL LETTER A WITH BREVE
					*p = 0x0103;	//	Äƒ	0xc4 0x83	LATIN SMALL LETTER A WITH BREVE
					break;
				case 0x0104:		//	Ä„	0xc4 0x84	LATIN CAPITAL LETTER A WITH OGONEK
					*p = 0x0105;	//	Ä…	0xc4 0x85	LATIN SMALL LETTER A WITH OGONEK
					break;
				case 0x0106:		//	Ä†	0xc4 0x86	LATIN CAPITAL LETTER C WITH ACUTE
					*p = 0x0107;	//	Ä‡	0xc4 0x87	LATIN SMALL LETTER C WITH ACUTE
					break;
				case 0x0108:		//	Äˆ	0xc4 0x88	LATIN CAPITAL LETTER C WITH CIRCUMFLEX
					*p = 0x0109;	//	Ä‰	0xc4 0x89	LATIN SMALL LETTER C WITH CIRCUMFLEX
					break;
				case 0x010A:		//	ÄŠ	0xc4 0x8a	LATIN CAPITAL LETTER C WITH DOT ABOVE
					*p = 0x010B;	//	Ä‹	0xc4 0x8b	LATIN SMALL LETTER C WITH DOT ABOVE
					break;
				case 0x010C:		//	ÄŒ	0xc4 0x8c	LATIN CAPITAL LETTER C WITH CARON
					*p = 0x010D;	//	Ä	0xc4 0x8d	LATIN SMALL LETTER C WITH CARON
					break;
				case 0x010E:		//	ÄŽ	0xc4 0x8e	LATIN CAPITAL LETTER D WITH CARON
					*p = 0x010F;	//	Ä	0xc4 0x8f	LATIN SMALL LETTER D WITH CARON
					break;
				case 0x0110:		//	Ä	0xc4 0x90	LATIN CAPITAL LETTER D WITH STROKE
					*p = 0x0111;	//	Ä‘	0xc4 0x91	LATIN SMALL LETTER D WITH STROKE
					break;
				case 0x0112:		//	Ä’	0xc4 0x92	LATIN CAPITAL LETTER E WITH MACRON
					*p = 0x0113;	//	Ä“	0xc4 0x93	LATIN SMALL LETTER E WITH MACRON
					break;
				case 0x0114:		//	Ä”	0xc4 0x94	LATIN CAPITAL LETTER E WITH BREVE
					*p = 0x0115;	//	Ä•	0xc4 0x95	LATIN SMALL LETTER E WITH BREVE
					break;
				case 0x0116:		//	Ä–	0xc4 0x96	LATIN CAPITAL LETTER E WITH DOT ABOVE
					*p = 0x0117;	//	Ä—	0xc4 0x97	LATIN SMALL LETTER E WITH DOT ABOVE
					break;
				case 0x0118:		//	Ä˜	0xc4 0x98	LATIN CAPITAL LETTER E WITH OGONEK
					*p = 0x0119;	//	Ä™	0xc4 0x99	LATIN SMALL LETTER E WITH OGONEK
					break;
				case 0x011A:		//	Äš	0xc4 0x9a	LATIN CAPITAL LETTER E WITH CARON
					*p = 0x011B;	//	Ä›	0xc4 0x9b	LATIN SMALL LETTER E WITH CARON
					break;
				case 0x011C:		//	Äœ	0xc4 0x9c	LATIN CAPITAL LETTER G WITH CIRCUMFLEX
					*p = 0x011D;	//	Ä	0xc4 0x9d	LATIN SMALL LETTER G WITH CIRCUMFLEX
					break;
				case 0x011E:		//	Äž	0xc4 0x9e	LATIN CAPITAL LETTER G WITH BREVE
					*p = 0x011F;	//	ÄŸ	0xc4 0x9f	LATIN SMALL LETTER G WITH BREVE
					break;
				case 0x0120:		//	Ä 	0xc4 0xa0	LATIN CAPITAL LETTER G WITH DOT ABOVE
					*p = 0x0121;	//	Ä¡	0xc4 0xa1	LATIN SMALL LETTER G WITH DOT ABOVE
					break;
				case 0x0122:		//	Ä¢	0xc4 0xa2	LATIN CAPITAL LETTER G WITH CEDILLA
					*p = 0x0123;	//	Ä£	0xc4 0xa3	LATIN SMALL LETTER G WITH CEDILLA
					break;
				case 0x0124:		//	Ä¤	0xc4 0xa4	LATIN CAPITAL LETTER H WITH CIRCUMFLEX
					*p = 0x0125;	//	Ä¥	0xc4 0xa5	LATIN SMALL LETTER H WITH CIRCUMFLEX
					break;
				case 0x0126:		//	Ä¦	0xc4 0xa6	LATIN CAPITAL LETTER H WITH STROKE
					*p = 0x0127;	//	Ä§	0xc4 0xa7	LATIN SMALL LETTER H WITH STROKE
					break;
				case 0x0128:		//	Ä¨	0xc4 0xa8	LATIN CAPITAL LETTER I WITH TILDE
					*p = 0x0129;	//	Ä©	0xc4 0xa9	LATIN SMALL LETTER I WITH TILDE
					break;
				case 0x012A:		//	Äª	0xc4 0xaa	LATIN CAPITAL LETTER I WITH MACRON
					*p = 0x012B;	//	Ä«	0xc4 0xab	LATIN SMALL LETTER I WITH MACRON
					break;
				case 0x012C:		//	Ä¬	0xc4 0xac	LATIN CAPITAL LETTER I WITH BREVE
					*p = 0x012D;	//	Ä­	0xc4 0xad	LATIN SMALL LETTER I WITH BREVE
					break;
				case 0x012E:		//	Ä®	0xc4 0xae	LATIN CAPITAL LETTER I WITH OGONEK
					*p = 0x012F;	//	Ä¯	0xc4 0xaf	LATIN SMALL LETTER I WITH OGONEK
					break;
				case 0x0130:		//	Ä°	0xc4 0xb0	LATIN CAPITAL LETTER I WITH DOT ABOVE
					*p = 0x0069;	//	i	0x69	LATIN SMALL LETTER I
					break;
				case 0x0132:		//	Ä²	0xc4 0xb2	LATIN CAPITAL LETTER LIGATURE IJ
					*p = 0x0133;	//	Ä³	0xc4 0xb3	LATIN SMALL LETTER LIGATURE IJ
					break;
				case 0x0134:		//	Ä´	0xc4 0xb4	LATIN CAPITAL LETTER J WITH CIRCUMFLEX
					*p = 0x0135;	//	Äµ	0xc4 0xb5	LATIN SMALL LETTER J WITH CIRCUMFLEX
					break;
				case 0x0136:		//	Ä¶	0xc4 0xb6	LATIN CAPITAL LETTER K WITH CEDILLA
					*p = 0x0137;	//	Ä·	0xc4 0xb7	LATIN SMALL LETTER K WITH CEDILLA
					break;
				case 0x0139:		//	Ä¹	0xc4 0xb9	LATIN CAPITAL LETTER L WITH ACUTE
					*p = 0x013A;	//	Äº	0xc4 0xba	LATIN SMALL LETTER L WITH ACUTE
					break;
				case 0x013B:		//	Ä»	0xc4 0xbb	LATIN CAPITAL LETTER L WITH CEDILLA
					*p = 0x013C;	//	Ä¼	0xc4 0xbc	LATIN SMALL LETTER L WITH CEDILLA
					break;
				case 0x013D:		//	Ä½	0xc4 0xbd	LATIN CAPITAL LETTER L WITH CARON
					*p = 0x013E;	//	Ä¾	0xc4 0xbe	LATIN SMALL LETTER L WITH CARON
					break;
				case 0x013F:		//	Ä¿	0xc4 0xbf	LATIN CAPITAL LETTER L WITH MIDDLE DOT
					*p = 0x0140;	//	Å€	0xc5 0x80	LATIN SMALL LETTER L WITH MIDDLE DOT
					break;
				case 0x0141:		//	Å	0xc5 0x81	LATIN CAPITAL LETTER L WITH STROKE
					*p = 0x0142;	//	Å‚	0xc5 0x82	LATIN SMALL LETTER L WITH STROKE
					break;
				case 0x0143:		//	Åƒ	0xc5 0x83	LATIN CAPITAL LETTER N WITH ACUTE
					*p = 0x0144;	//	Å„	0xc5 0x84	LATIN SMALL LETTER N WITH ACUTE
					break;
				case 0x0145:		//	Å…	0xc5 0x85	LATIN CAPITAL LETTER N WITH CEDILLA
					*p = 0x0146;	//	Å†	0xc5 0x86	LATIN SMALL LETTER N WITH CEDILLA
					break;
				case 0x0147:		//	Å‡	0xc5 0x87	LATIN CAPITAL LETTER N WITH CARON
					*p = 0x0148;	//	Åˆ	0xc5 0x88	LATIN SMALL LETTER N WITH CARON
					break;
				case 0x014A:		//	ÅŠ	0xc5 0x8a	LATIN CAPITAL LETTER ENG
					*p = 0x014B;	//	Å‹	0xc5 0x8b	LATIN SMALL LETTER ENG
					break;
				case 0x014C:		//	ÅŒ	0xc5 0x8c	LATIN CAPITAL LETTER O WITH MACRON
					*p = 0x014D;	//	Å	0xc5 0x8d	LATIN SMALL LETTER O WITH MACRON
					break;
				case 0x014E:		//	ÅŽ	0xc5 0x8e	LATIN CAPITAL LETTER O WITH BREVE
					*p = 0x014F;	//	Å	0xc5 0x8f	LATIN SMALL LETTER O WITH BREVE
					break;
				case 0x0150:		//	Å	0xc5 0x90	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
					*p = 0x0151;	//	Å‘	0xc5 0x91	LATIN SMALL LETTER O WITH DOUBLE ACUTE
					break;
				case 0x0152:		//	Å’	0xc5 0x92	LATIN CAPITAL LETTER LIGATURE OE
					*p = 0x0153;	//	Å“	0xc5 0x93	LATIN SMALL LETTER LIGATURE OE
					break;
				case 0x0154:		//	Å”	0xc5 0x94	LATIN CAPITAL LETTER R WITH ACUTE
					*p = 0x0155;	//	Å•	0xc5 0x95	LATIN SMALL LETTER R WITH ACUTE
					break;
				case 0x0156:		//	Å–	0xc5 0x96	LATIN CAPITAL LETTER R WITH CEDILLA
					*p = 0x0157;	//	Å—	0xc5 0x97	LATIN SMALL LETTER R WITH CEDILLA
					break;
				case 0x0158:		//	Å˜	0xc5 0x98	LATIN CAPITAL LETTER R WITH CARON
					*p = 0x0159;	//	Å™	0xc5 0x99	LATIN SMALL LETTER R WITH CARON
					break;
				case 0x015A:		//	Åš	0xc5 0x9a	LATIN CAPITAL LETTER S WITH ACUTE
					*p = 0x015B;	//	Å›	0xc5 0x9b	LATIN SMALL LETTER S WITH ACUTE
					break;
				case 0x015C:		//	Åœ	0xc5 0x9c	LATIN CAPITAL LETTER S WITH CIRCUMFLEX
					*p = 0x015D;	//	Å	0xc5 0x9d	LATIN SMALL LETTER S WITH CIRCUMFLEX
					break;
				case 0x015E:		//	Åž	0xc5 0x9e	LATIN CAPITAL LETTER S WITH CEDILLA
					*p = 0x015F;	//	ÅŸ	0xc5 0x9f	LATIN SMALL LETTER S WITH CEDILLA
					break;
				case 0x0160:		//	Å 	0xc5 0xa0	LATIN CAPITAL LETTER S WITH CARON
					*p = 0x0161;	//	Å¡	0xc5 0xa1	LATIN SMALL LETTER S WITH CARON
					break;
				case 0x0162:		//	Å¢	0xc5 0xa2	LATIN CAPITAL LETTER T WITH CEDILLA
					*p = 0x0163;	//	Å£	0xc5 0xa3	LATIN SMALL LETTER T WITH CEDILLA
					break;
				case 0x0164:		//	Å¤	0xc5 0xa4	LATIN CAPITAL LETTER T WITH CARON
					*p = 0x0165;	//	Å¥	0xc5 0xa5	LATIN SMALL LETTER T WITH CARON
					break;
				case 0x0166:		//	Å¦	0xc5 0xa6	LATIN CAPITAL LETTER T WITH STROKE
					*p = 0x0167;	//	Å§	0xc5 0xa7	LATIN SMALL LETTER T WITH STROKE
					break;
				case 0x0168:		//	Å¨	0xc5 0xa8	LATIN CAPITAL LETTER U WITH TILDE
					*p = 0x0169;	//	Å©	0xc5 0xa9	LATIN SMALL LETTER U WITH TILDE
					break;
				case 0x016A:		//	Åª	0xc5 0xaa	LATIN CAPITAL LETTER U WITH MACRON
					*p = 0x016B;	//	Å«	0xc5 0xab	LATIN SMALL LETTER U WITH MACRON
					break;
				case 0x016C:		//	Å¬	0xc5 0xac	LATIN CAPITAL LETTER U WITH BREVE
					*p = 0x016D;	//	Å­	0xc5 0xad	LATIN SMALL LETTER U WITH BREVE
					break;
				case 0x016E:		//	Å®	0xc5 0xae	LATIN CAPITAL LETTER U WITH RING ABOVE
					*p = 0x016F;	//	Å¯	0xc5 0xaf	LATIN SMALL LETTER U WITH RING ABOVE
					break;
				case 0x0170:		//	Å°	0xc5 0xb0	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
					*p = 0x0171;	//	Å±	0xc5 0xb1	LATIN SMALL LETTER U WITH DOUBLE ACUTE
					break;
				case 0x0172:		//	Å²	0xc5 0xb2	LATIN CAPITAL LETTER U WITH OGONEK
					*p = 0x0173;	//	Å³	0xc5 0xb3	LATIN SMALL LETTER U WITH OGONEK
					break;
				case 0x0174:		//	Å´	0xc5 0xb4	LATIN CAPITAL LETTER W WITH CIRCUMFLEX
					*p = 0x0175;	//	Åµ	0xc5 0xb5	LATIN SMALL LETTER W WITH CIRCUMFLEX
					break;
				case 0x0176:		//	Å¶	0xc5 0xb6	LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
					*p = 0x0177;	//	Å·	0xc5 0xb7	LATIN SMALL LETTER Y WITH CIRCUMFLEX
					break;
				case 0x0178:		//	Å¸	0xc5 0xb8	LATIN CAPITAL LETTER Y WITH DIAERESIS
					*p = 0x00FF;	//	Ã¿	0xc3 0xbf	LATIN SMALL LETTER Y WITH DIAERESIS
					break;
				case 0x0179:		//	Å¹	0xc5 0xb9	LATIN CAPITAL LETTER Z WITH ACUTE
					*p = 0x017A;	//	Åº	0xc5 0xba	LATIN SMALL LETTER Z WITH ACUTE
					break;
				case 0x017B:		//	Å»	0xc5 0xbb	LATIN CAPITAL LETTER Z WITH DOT ABOVE
					*p = 0x017C;	//	Å¼	0xc5 0xbc	LATIN SMALL LETTER Z WITH DOT ABOVE
					break;
				case 0x017D:		//	Å½	0xc5 0xbd	LATIN CAPITAL LETTER Z WITH CARON
					*p = 0x017E;	//	Å¾	0xc5 0xbe	LATIN SMALL LETTER Z WITH CARON
					break;
				case 0x0181:		//	Æ	0xc6 0x81	LATIN CAPITAL LETTER B WITH HOOK
					*p = 0x0253;	//	É“	0xc9 0x93	LATIN SMALL LETTER B WITH HOOK
					break;
				case 0x0182:		//	Æ‚	0xc6 0x82	LATIN CAPITAL LETTER B WITH TOPBAR
					*p = 0x0183;	//	Æƒ	0xc6 0x83	LATIN SMALL LETTER B WITH TOPBAR
					break;
				case 0x0184:		//	Æ„	0xc6 0x84	LATIN CAPITAL LETTER TONE SIX
					*p = 0x0185;	//	Æ…	0xc6 0x85	LATIN SMALL LETTER TONE SIX
					break;
				case 0x0186:		//	Æ†	0xc6 0x86	LATIN CAPITAL LETTER OPEN O
					*p = 0x0254;	//	É”	0xc9 0x94	LATIN SMALL LETTER OPEN O
					break;
				case 0x0187:		//	Æ‡	0xc6 0x87	LATIN CAPITAL LETTER C WITH HOOK
					*p = 0x0188;	//	Æˆ	0xc6 0x88	LATIN SMALL LETTER C WITH HOOK
					break;
				case 0x0189:		//	Æ‰	0xc6 0x89	LATIN CAPITAL LETTER AFRICAN D
					*p = 0x0256;	//	É–	0xc9 0x96	LATIN SMALL LETTER AFRICAN D
					break;
				case 0x018A:		//	ÆŠ	0xc6 0x8a	LATIN CAPITAL LETTER D WITH HOOK
					*p = 0x0257;	//	É—	0xc9 0x97	LATIN SMALL LETTER D WITH HOOK
					break;
				case 0x018B:		//	Æ‹	0xc6 0x8b	LATIN CAPITAL LETTER D WITH TOPBAR
					*p = 0x018C;	//	ÆŒ	0xc6 0x8c	LATIN SMALL LETTER D WITH TOPBAR
					break;
				case 0x018E:		//	ÆŽ	0xc6 0x8e	LATIN CAPITAL LETTER REVERSED E
					*p = 0x01DD;	//	Ç	0xC7 0x9D	LATIN SMALL LETTER TURNED E (look mid page 297 https://www.unicode.org/versions/Unicode14.0.0/ch07.pdf)
					break;
				case 0x018F:		//	Æ	0xc6 0x8f	LATIN CAPITAL LETTER SCHWA
					*p = 0x0259;	//	É™	0xc9 0x99	LATIN SMALL LETTER SCHWA
					break;
				case 0x0190:		//	Æ	0xc6 0x90	LATIN CAPITAL LETTER OPEN E
					*p = 0x025B;	//	É›	0xc9 0x9b	LATIN SMALL LETTER OPEN E
					break;
				case 0x0191:		//	Æ‘	0xc6 0x91	LATIN CAPITAL LETTER F WITH HOOK
					*p = 0x0192;	//	Æ’	0xc6 0x92	LATIN SMALL LETTER F WITH HOOK
					break;
				case 0x0193:		//	Æ“	0xc6 0x93	LATIN CAPITAL LETTER G WITH HOOK
					*p = 0x0260;	//	É 	0xc9 0xa0	LATIN SMALL LETTER G WITH HOOK
					break;
				case 0x0194:		//	Æ”	0xc6 0x94	LATIN CAPITAL LETTER GAMMA
					*p = 0x0263;	//	É£	0xc9 0xa3	LATIN SMALL LETTER GAMMA
					break;
				case 0x0196:		//	Æ–	0xc6 0x96	LATIN CAPITAL LETTER IOTA
					*p = 0x0269;	//	É©	0xc9 0xa9	LATIN SMALL LETTER IOTA
					break;
				case 0x0197:		//	Æ—	0xc6 0x97	LATIN CAPITAL LETTER I WITH STROKE
					*p = 0x0268;	//	É¨	0xc9 0xa8	LATIN SMALL LETTER I WITH STROKE
					break;
				case 0x0198:		//	Æ˜	0xc6 0x98	LATIN CAPITAL LETTER K WITH HOOK
					*p = 0x0199;	//	Æ™	0xc6 0x99	LATIN SMALL LETTER K WITH HOOK
					break;
				case 0x019C:		//	Æœ	0xc6 0x9c	LATIN CAPITAL LETTER TURNED M
					*p = 0x026F;	//	É¯	0xc9 0xaf	LATIN SMALL LETTER TURNED M
					break;
				case 0x019D:		//	Æ	0xc6 0x9d	LATIN CAPITAL LETTER N WITH LEFT HOOK
					*p = 0x0272;	//	É²	0xc9 0xb2	LATIN SMALL LETTER N WITH LEFT HOOK
					break;
				case 0x019F:		//	ÆŸ	0xc6 0x9f	LATIN CAPITAL LETTER O WITH MIDDLE TILDE
					*p = 0x0275;	//	Éµ	0xc9 0xb5	LATIN SMALL LETTER O WITH MIDDLE TILDE
					break;
				case 0x01A0:		//	Æ 	0xc6 0xa0	LATIN CAPITAL LETTER O WITH HORN
					*p = 0x01A1;	//	Æ¡	0xc6 0xa1	LATIN SMALL LETTER O WITH HORN
					break;
				case 0x01A2:		//	Æ¢	0xc6 0xa2	LATIN CAPITAL LETTER OI
					*p = 0x01A3;	//	Æ£	0xc6 0xa3	LATIN SMALL LETTER OI
					break;
				case 0x01A4:		//	Æ¤	0xc6 0xa4	LATIN CAPITAL LETTER P WITH HOOK
					*p = 0x01A5;	//	Æ¥	0xc6 0xa5	LATIN SMALL LETTER P WITH HOOK
					break;
				case 0x01A7:		//	Æ§	0xc6 0xa7	LATIN CAPITAL LETTER TONE TWO
					*p = 0x01A8;	//	Æ¨	0xc6 0xa8	LATIN SMALL LETTER TONE TWO
					break;
				case 0x01A9:		//	Æ©	0xc6 0xa9	LATIN CAPITAL LETTER ESH
					*p = 0x0283;	//	Êƒ	0xca 0x83	LATIN SMALL LETTER ESH
					break;
				case 0x01AC:		//	Æ¬	0xc6 0xac	LATIN CAPITAL LETTER T WITH HOOK
					*p = 0x01AD;	//	Æ­	0xc6 0xad	LATIN SMALL LETTER T WITH HOOK
					break;
				case 0x01AE:		//	Æ®	0xc6 0xae	LATIN CAPITAL LETTER T WITH RETROFLEX HOOK
					*p = 0x0288;	//	Êˆ	0xca 0x88	LATIN SMALL LETTER T WITH RETROFLEX HOOK
					break;
				case 0x01AF:		//	Æ¯	0xc6 0xaf	LATIN CAPITAL LETTER U WITH HORN
					*p = 0x01B0;	//	Æ°	0xc6 0xb0	LATIN SMALL LETTER U WITH HORN
					break;
				case 0x01B1:		//	Æ±	0xc6 0xb1	LATIN CAPITAL LETTER UPSILON
					*p = 0x028A;	//	ÊŠ	0xca 0x8a	LATIN SMALL LETTER UPSILON
					break;
				case 0x01B2:		//	Æ²	0xc6 0xb2	LATIN CAPITAL LETTER V WITH HOOK
					*p = 0x028B;	//	Ê‹	0xca 0x8b	LATIN SMALL LETTER V WITH HOOK
					break;
				case 0x01B3:		//	Æ³	0xc6 0xb3	LATIN CAPITAL LETTER Y WITH HOOK
					*p = 0x01B4;	//	Æ´	0xc6 0xb4	LATIN SMALL LETTER Y WITH HOOK
					break;
				case 0x01B5:		//	Æµ	0xc6 0xb5	LATIN CAPITAL LETTER Z WITH STROKE
					*p = 0x01B6;	//	Æ¶	0xc6 0xb6	LATIN SMALL LETTER Z WITH STROKE
					break;
				case 0x01B7:		//	Æ·	0xc6 0xb7	LATIN CAPITAL LETTER EZH
					*p = 0x0292;	//	Ê’	0xca 0x92	LATIN SMALL LETTER EZH
					break;
				case 0x01B8:		//	Æ¸	0xc6 0xb8	LATIN CAPITAL LETTER EZH REVERSED
					*p = 0x01B9;	//	Æ¹	0xc6 0xb9	LATIN SMALL LETTER EZH REVERSED
					break;
				case 0x01BC:		//	Æ¼	0xc6 0xbc	LATIN CAPITAL LETTER TONE FIVE
					*p = 0x01BD;	//	Æ½	0xc6 0xbd	LATIN SMALL LETTER TONE FIVE
					break;
				case 0x01C5:		//	Ç…	0xc7 0x85	LATIN CAPITAL LETTER D WITH SMALL Z WITH CARON
				case 0x01C4:		//	Ç„	0xc7 0x84	LATIN CAPITAL LETTER DZ WITH CARON
					*p = 0x01C6;	//	Ç†	0xc7 0x86	LATIN SMALL LETTER DZ WITH CARON
					break;
				case 0x01C8:		//	Çˆ	0xc7 0x88	LATIN CAPITAL LETTER L WITH SMALL J
				case 0x01C7:		//	Ç‡	0xc7 0x87	LATIN CAPITAL LETTER LJ
					*p = 0x01C9;	//	Ç‰	0xc7 0x89	LATIN SMALL LETTER LJ
					break;
				case 0x01CB:		//	Ç‹	0xc7 0x8b	LATIN CAPITAL LETTER N WITH SMALL J
				case 0x01CA:		//	ÇŠ	0xc7 0x8a	LATIN CAPITAL LETTER NJ
					*p = 0x01CC;	//	ÇŒ	0xc7 0x8c	LATIN SMALL LETTER NJ
					break;
				case 0x01CD:		//	Ç	0xc7 0x8d	LATIN CAPITAL LETTER A WITH CARON
					*p = 0x01CE;	//	ÇŽ	0xc7 0x8e	LATIN SMALL LETTER A WITH CARON
					break;
				case 0x01CF:		//	Ç	0xc7 0x8f	LATIN CAPITAL LETTER I WITH CARON
					*p = 0x01D0;	//	Ç	0xc7 0x90	LATIN SMALL LETTER I WITH CARON
					break;
				case 0x01D1:		//	Ç‘	0xc7 0x91	LATIN CAPITAL LETTER O WITH CARON
					*p = 0x01D2;	//	Ç’	0xc7 0x92	LATIN SMALL LETTER O WITH CARON
					break;
				case 0x01D3:		//	Ç“	0xc7 0x93	LATIN CAPITAL LETTER U WITH CARON
					*p = 0x01D4;	//	Ç”	0xc7 0x94	LATIN SMALL LETTER U WITH CARON
					break;
				case 0x01D5:		//	Ç•	0xc7 0x95	LATIN CAPITAL LETTER U WITH DIAERESIS AND MACRON
					*p = 0x01D6;	//	Ç–	0xc7 0x96	LATIN SMALL LETTER U WITH DIAERESIS AND MACRON
					break;
				case 0x01D7:		//	Ç—	0xc7 0x97	LATIN CAPITAL LETTER U WITH DIAERESIS AND ACUTE
					*p = 0x01D8;	//	Ç˜	0xc7 0x98	LATIN SMALL LETTER U WITH DIAERESIS AND ACUTE
					break;
				case 0x01D9:		//	Ç™	0xc7 0x99	LATIN CAPITAL LETTER U WITH DIAERESIS AND CARON
					*p = 0x01DA;	//	Çš	0xc7 0x9a	LATIN SMALL LETTER U WITH DIAERESIS AND CARON
					break;
				case 0x01DB:		//	Ç›	0xc7 0x9b	LATIN CAPITAL LETTER U WITH DIAERESIS AND GRAVE
					*p = 0x01DC;	//	Çœ	0xc7 0x9c	LATIN SMALL LETTER U WITH DIAERESIS AND GRAVE
					break;
				case 0x01DE:		//	Çž	0xc7 0x9e	LATIN CAPITAL LETTER A WITH DIAERESIS AND MACRON
					*p = 0x01DF;	//	ÇŸ	0xc7 0x9f	LATIN SMALL LETTER A WITH DIAERESIS AND MACRON
					break;
				case 0x01E0:		//	Ç 	0xc7 0xa0	LATIN CAPITAL LETTER A WITH DOT ABOVE AND MACRON
					*p = 0x01E1;	//	Ç¡	0xc7 0xa1	LATIN SMALL LETTER A WITH DOT ABOVE AND MACRON
					break;
				case 0x01E2:		//	Ç¢	0xc7 0xa2	LATIN CAPITAL LETTER AE WITH MACRON
					*p = 0x01E3;	//	Ç£	0xc7 0xa3	LATIN SMALL LETTER AE WITH MACRON
					break;
				case 0x01E4:		//	Ç¤	0xc7 0xa4	LATIN CAPITAL LETTER G WITH STROKE
					*p = 0x01E5;	//	Ç¥	0xc7 0xa5	LATIN SMALL LETTER G WITH STROKE
					break;
				case 0x01E6:		//	Ç¦	0xc7 0xa6	LATIN CAPITAL LETTER G WITH CARON
					*p = 0x01E7;	//	Ç§	0xc7 0xa7	LATIN SMALL LETTER G WITH CARON
					break;
				case 0x01E8:		//	Ç¨	0xc7 0xa8	LATIN CAPITAL LETTER K WITH CARON
					*p = 0x01E9;	//	Ç©	0xc7 0xa9	LATIN SMALL LETTER K WITH CARON
					break;
				case 0x01EA:		//	Çª	0xc7 0xaa	LATIN CAPITAL LETTER O WITH OGONEK
					*p = 0x01EB;	//	Ç«	0xc7 0xab	LATIN SMALL LETTER O WITH OGONEK
					break;
				case 0x01EC:		//	Ç¬	0xc7 0xac	LATIN CAPITAL LETTER O WITH OGONEK AND MACRON
					*p = 0x01ED;	//	Ç­	0xc7 0xad	LATIN SMALL LETTER O WITH OGONEK AND MACRON
					break;
				case 0x01EE:		//	Ç®	0xc7 0xae	LATIN CAPITAL LETTER EZH WITH CARON
					*p = 0x01EF;	//	Ç¯	0xc7 0xaf	LATIN SMALL LETTER EZH WITH CARON
					break;
				case 0x01F1:		//	Ç±	0xc7 0xb1	LATIN CAPITAL LETTER DZ
				case 0x01F2:		//	Ç²	0xc7 0xb2	LATIN CAPITAL LETTER D WITH SMALL Z
					*p = 0x01F3;	//	Ç³	0xc7 0xb3	LATIN SMALL LETTER DZ
					break;
				case 0x01F4:		//	Ç´	0xc7 0xb4	LATIN CAPITAL LETTER G WITH ACUTE
					*p = 0x01F5;	//	Çµ	0xc7 0xb5	LATIN SMALL LETTER G WITH ACUTE
					break;
				case 0x01F6:		//	Ç¶	0xc7 0xb6	LATIN CAPITAL LETTER HWAIR
					*p = 0x0195;	//	Æ•	0xc6 0x95	LATIN SMALL LETTER HWAIR
					break;
				case 0x01F7:		//	Ç·	0xc7 0xb7	LATIN CAPITAL LETTER WYNN
					*p = 0x01BF;	//	Æ¿	0xc6 0xbf	LATIN SMALL LETTER WYNN
					break;
				case 0x01F8:		//	Ç¸	0xc7 0xb8	LATIN CAPITAL LETTER N WITH GRAVE
					*p = 0x01F9;	//	Ç¹	0xc7 0xb9	LATIN SMALL LETTER N WITH GRAVE
					break;
				case 0x01FA:		//	Çº	0xc7 0xba	LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE
					*p = 0x01FB;	//	Ç»	0xc7 0xbb	LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE
					break;
				case 0x01FC:		//	Ç¼	0xc7 0xbc	LATIN CAPITAL LETTER AE WITH ACUTE
					*p = 0x01FD;	//	Ç½	0xc7 0xbd	LATIN SMALL LETTER AE WITH ACUTE
					break;
				case 0x01FE:		//	Ç¾	0xc7 0xbe	LATIN CAPITAL LETTER O WITH STROKE AND ACUTE
					*p = 0x01FF;	//	Ç¿	0xc7 0xbf	LATIN SMALL LETTER O WITH STROKE AND ACUTE
					break;
				case 0x0200:		//	È€	0xc8 0x80	LATIN CAPITAL LETTER A WITH DOUBLE GRAVE
					*p = 0x0201;	//	È	0xc8 0x81	LATIN SMALL LETTER A WITH DOUBLE GRAVE
					break;
				case 0x0202:		//	È‚	0xc8 0x82	LATIN CAPITAL LETTER A WITH INVERTED BREVE
					*p = 0x0203;	//	Èƒ	0xc8 0x83	LATIN SMALL LETTER A WITH INVERTED BREVE
					break;
				case 0x0204:		//	È„	0xc8 0x84	LATIN CAPITAL LETTER E WITH DOUBLE GRAVE
					*p = 0x0205;	//	È…	0xc8 0x85	LATIN SMALL LETTER E WITH DOUBLE GRAVE
					break;
				case 0x0206:		//	È†	0xc8 0x86	LATIN CAPITAL LETTER E WITH INVERTED BREVE
					*p = 0x0207;	//	È‡	0xc8 0x87	LATIN SMALL LETTER E WITH INVERTED BREVE
					break;
				case 0x0208:		//	Èˆ	0xc8 0x88	LATIN CAPITAL LETTER I WITH DOUBLE GRAVE
					*p = 0x0209;	//	È‰	0xc8 0x89	LATIN SMALL LETTER I WITH DOUBLE GRAVE
					break;
				case 0x020A:		//	ÈŠ	0xc8 0x8a	LATIN CAPITAL LETTER I WITH INVERTED BREVE
					*p = 0x020B;	//	È‹	0xc8 0x8b	LATIN SMALL LETTER I WITH INVERTED BREVE
					break;
				case 0x020C:		//	ÈŒ	0xc8 0x8c	LATIN CAPITAL LETTER O WITH DOUBLE GRAVE
					*p = 0x020D;	//	È	0xc8 0x8d	LATIN SMALL LETTER O WITH DOUBLE GRAVE
					break;
				case 0x020E:		//	ÈŽ	0xc8 0x8e	LATIN CAPITAL LETTER O WITH INVERTED BREVE
					*p = 0x020F;	//	È	0xc8 0x8f	LATIN SMALL LETTER O WITH INVERTED BREVE
					break;
				case 0x0210:		//	È	0xc8 0x90	LATIN CAPITAL LETTER R WITH DOUBLE GRAVE
					*p = 0x0211;	//	È‘	0xc8 0x91	LATIN SMALL LETTER R WITH DOUBLE GRAVE
					break;
				case 0x0212:		//	È’	0xc8 0x92	LATIN CAPITAL LETTER R WITH INVERTED BREVE
					*p = 0x0213;	//	È“	0xc8 0x93	LATIN SMALL LETTER R WITH INVERTED BREVE
					break;
				case 0x0214:		//	È”	0xc8 0x94	LATIN CAPITAL LETTER U WITH DOUBLE GRAVE
					*p = 0x0215;	//	È•	0xc8 0x95	LATIN SMALL LETTER U WITH DOUBLE GRAVE
					break;
				case 0x0216:		//	È–	0xc8 0x96	LATIN CAPITAL LETTER U WITH INVERTED BREVE
					*p = 0x0217;	//	È—	0xc8 0x97	LATIN SMALL LETTER U WITH INVERTED BREVE
					break;
				case 0x0218:		//	È˜	0xc8 0x98	LATIN CAPITAL LETTER S WITH COMMA BELOW
					*p = 0x0219;	//	È™	0xc8 0x99	LATIN SMALL LETTER S WITH COMMA BELOW
					break;
				case 0x021A:		//	Èš	0xc8 0x9a	LATIN CAPITAL LETTER T WITH COMMA BELOW
					*p = 0x021B;	//	È›	0xc8 0x9b	LATIN SMALL LETTER T WITH COMMA BELOW
					break;
				case 0x021C:		//	Èœ	0xc8 0x9c	LATIN CAPITAL LETTER YOGH
					*p = 0x021D;	//	È	0xc8 0x9d	LATIN SMALL LETTER YOGH
					break;
				case 0x021E:		//	Èž	0xc8 0x9e	LATIN CAPITAL LETTER H WITH CARON
					*p = 0x021F;	//	ÈŸ	0xc8 0x9f	LATIN SMALL LETTER H WITH CARON
					break;
				case 0x0220:		//	È 	0xc8 0xa0	LATIN CAPITAL LETTER N WITH LONG RIGHT LEG
					*p = 0x019E;	//	Æž	0xc6 0x9e	LATIN SMALL LETTER N WITH LONG RIGHT LEG
					break;
				case 0x0222:		//	È¢	0xc8 0xa2	LATIN CAPITAL LETTER OU
					*p = 0x0223;	//	È£	0xc8 0xa3	LATIN SMALL LETTER OU
					break;
				case 0x0224:		//	È¤	0xc8 0xa4	LATIN CAPITAL LETTER Z WITH HOOK
					*p = 0x0225;	//	È¥	0xc8 0xa5	LATIN SMALL LETTER Z WITH HOOK
					break;
				case 0x0226:		//	È¦	0xc8 0xa6	LATIN CAPITAL LETTER A WITH DOT ABOVE
					*p = 0x0227;	//	È§	0xc8 0xa7	LATIN SMALL LETTER A WITH DOT ABOVE
					break;
				case 0x0228:		//	È¨	0xc8 0xa8	LATIN CAPITAL LETTER E WITH CEDILLA
					*p = 0x0229;	//	È©	0xc8 0xa9	LATIN SMALL LETTER E WITH CEDILLA
					break;
				case 0x022A:		//	Èª	0xc8 0xaa	LATIN CAPITAL LETTER O WITH DIAERESIS AND MACRON
					*p = 0x022B;	//	È«	0xc8 0xab	LATIN SMALL LETTER O WITH DIAERESIS AND MACRON
					break;
				case 0x022C:		//	È¬	0xc8 0xac	LATIN CAPITAL LETTER O WITH TILDE AND MACRON
					*p = 0x022D;	//	È­	0xc8 0xad	LATIN SMALL LETTER O WITH TILDE AND MACRON
					break;
				case 0x022E:		//	È®	0xc8 0xae	LATIN CAPITAL LETTER O WITH DOT ABOVE
					*p = 0x022F;	//	È¯	0xc8 0xaf	LATIN SMALL LETTER O WITH DOT ABOVE
					break;
				case 0x0230:		//	È°	0xc8 0xb0	LATIN CAPITAL LETTER O WITH DOT ABOVE AND MACRON
					*p = 0x0231;	//	È±	0xc8 0xb1	LATIN SMALL LETTER O WITH DOT ABOVE AND MACRON
					break;
				case 0x0232:		//	È²	0xc8 0xb2	LATIN CAPITAL LETTER Y WITH MACRON
					*p = 0x0233;	//	È³	0xc8 0xb3	LATIN SMALL LETTER Y WITH MACRON
					break;
				case 0x023A:		//	Èº	0xc8 0xba	LATIN CAPITAL LETTER A WITH STROKE
					*p = 0x2C65;	//	â±¥	0xe2 0xb1 0xa5	LATIN SMALL LETTER A WITH STROKE
					break;
				case 0x023B:		//	È»	0xc8 0xbb	LATIN CAPITAL LETTER C WITH STROKE
					*p = 0x023C;	//	È¼	0xc8 0xbc	LATIN SMALL LETTER C WITH STROKE
					break;
				case 0x023D:		//	È½	0xc8 0xbd	LATIN CAPITAL LETTER L WITH BAR
					*p = 0x019A;	//	Æš	0xc6 0x9a	LATIN SMALL LETTER L WITH BAR
					break;
				case 0x023E:		//	È¾	0xc8 0xbe	LATIN CAPITAL LETTER T WITH DIAGONAL STROKE
					*p = 0x2C66;	//	â±¦	0xe2 0xb1 0xa6	LATIN SMALL LETTER T WITH DIAGONAL STROKE
					break;
				case 0x0241:		//	É	0xc9 0x81	LATIN CAPITAL LETTER GLOTTAL STOP
					*p = 0x0242;	//	É‚	0xc9 0x82	LATIN SMALL LETTER GLOTTAL STOP
					break;
				case 0x0243:		//	Éƒ	0xc9 0x83	LATIN CAPITAL LETTER B WITH STROKE
					*p = 0x0180;	//	Æ€	0xc6 0x80	LATIN SMALL LETTER B WITH STROKE
					break;
				case 0x0244:		//	É„	0xc9 0x84	LATIN CAPITAL LETTER U BAR
					*p = 0x0289;	//	Ê‰	0xca 0x89	LATIN SMALL LETTER U BAR
					break;
				case 0x0245:		//	É…	0xc9 0x85	LATIN CAPITAL LETTER TURNED V
					*p = 0x028C;	//	ÊŒ	0xca 0x8c	LATIN SMALL LETTER TURNED V
					break;
				case 0x0246:		//	É†	0xc9 0x86	LATIN CAPITAL LETTER E WITH STROKE
					*p = 0x0247;	//	É‡	0xc9 0x87	LATIN SMALL LETTER E WITH STROKE
					break;
				case 0x0248:		//	Éˆ	0xc9 0x88	LATIN CAPITAL LETTER J WITH STROKE
					*p = 0x0249;	//	É‰	0xc9 0x89	LATIN SMALL LETTER J WITH STROKE
					break;
				case 0x024A:		//	ÉŠ	0xc9 0x8a	LATIN CAPITAL LETTER SMALL Q WITH HOOK TAIL
					*p = 0x024B;	//	É‹	0xc9 0x8b	LATIN SMALL LETTER SMALL Q WITH HOOK TAIL
					break;
				case 0x024C:		//	ÉŒ	0xc9 0x8c	LATIN CAPITAL LETTER R WITH STROKE
					*p = 0x024D;	//	É	0xc9 0x8d	LATIN SMALL LETTER R WITH STROKE
					break;
				case 0x024E:		//	ÉŽ	0xc9 0x8e	LATIN CAPITAL LETTER Y WITH STROKE
					*p = 0x024F;	//	É	0xc9 0x8f	LATIN SMALL LETTER Y WITH STROKE
					break;
				case 0x0370:		//	Í°	0xcd 0xb0	GREEK CAPITAL LETTER HETA
					*p = 0x0371;	//	Í±	0xcd 0xb1	GREEK SMALL LETTER HETA
					break;
				case 0x0372:		//	Í²	0xcd 0xb2	GREEK CAPITAL LETTER ARCHAIC SAMPI
					*p = 0x0373;	//	Í³	0xcd 0xb3	GREEK SMALL LETTER ARCHAIC SAMPI
					break;
				case 0x0376:		//	Í¶	0xcd 0xb6	GREEK CAPITAL LETTER PAMPHYLIAN DIGAMMA
					*p = 0x0377;	//	Í·	0xcd 0xb7	GREEK SMALL LETTER PAMPHYLIAN DIGAMMA
					break;
				case 0x037F:		//	Í¿	0xcd 0xbf	GREEK CAPITAL LETTER YOT
					*p = 0x03F3;	//	Ï³	0xcf 0xb3	GREEK SMALL LETTER YOT
					break;
				case 0x0386:		//	Î†	0xce 0x86	GREEK CAPITAL LETTER ALPHA WITH TONOS
					*p = 0x03AC;	//	Î¬	0xce 0xac	GREEK SMALL LETTER ALPHA WITH TONOS
					break;
				case 0x0388:		//	Îˆ	0xce 0x88	GREEK CAPITAL LETTER EPSILON WITH TONOS
					*p = 0x03AD;	//	Î­	0xce 0xad	GREEK SMALL LETTER EPSILON WITH TONOS
					break;
				case 0x0389:		//	Î‰	0xce 0x89	GREEK CAPITAL LETTER ETA WITH TONOS
					*p = 0x03AE;	//	Î®	0xce 0xae	GREEK SMALL LETTER ETA WITH TONOS
					break;
				case 0x038A:		//	ÎŠ	0xce 0x8a	GREEK CAPITAL LETTER IOTA WITH TONOS
					*p = 0x03AF;	//	Î¯	0xce 0xaf	GREEK SMALL LETTER IOTA WITH TONOS
					break;
				case 0x038C:		//	ÎŒ	0xce 0x8c	GREEK CAPITAL LETTER OMICRON WITH TONOS
					*p = 0x03CC;	//	ÏŒ	0xcf 0x8c	GREEK SMALL LETTER OMICRON WITH TONOS
					break;
				case 0x038E:		//	ÎŽ	0xce 0x8e	GREEK CAPITAL LETTER UPSILON WITH TONOS
					*p = 0x03CD;	//	Ï	0xcf 0x8d	GREEK SMALL LETTER UPSILON WITH TONOS
					break;
				case 0x038F:		//	Î	0xce 0x8f	GREEK CAPITAL LETTER OMEGA WITH TONOS
					*p = 0x03CE;	//	ÏŽ	0xcf 0x8e	GREEK SMALL LETTER OMEGA WITH TONOS
					break;
				case 0x0391:		//	Î‘	0xce 0x91	GREEK CAPITAL LETTER ALPHA
					*p = 0x03B1;	//	Î±	0xce 0xb1	GREEK SMALL LETTER ALPHA
					break;
				case 0x0392:		//	Î’	0xce 0x92	GREEK CAPITAL LETTER BETA
					*p = 0x03B2;	//	Î²	0xce 0xb2	GREEK SMALL LETTER BETA
					break;
				case 0x0393:		//	Î“	0xce 0x93	GREEK CAPITAL LETTER GAMMA
					*p = 0x03B3;	//	Î³	0xce 0xb3	GREEK SMALL LETTER GAMMA
					break;
				case 0x0394:		//	Î”	0xce 0x94	GREEK CAPITAL LETTER DELTA
					*p = 0x03B4;	//	Î´	0xce 0xb4	GREEK SMALL LETTER DELTA
					break;
				case 0x0395:		//	Î•	0xce 0x95	GREEK CAPITAL LETTER EPSILON
					*p = 0x03B5;	//	Îµ	0xce 0xb5	GREEK SMALL LETTER EPSILON
					break;
				case 0x0396:		//	Î–	0xce 0x96	GREEK CAPITAL LETTER ZETA
					*p = 0x03B6;	//	Î¶	0xce 0xb6	GREEK SMALL LETTER ZETA
					break;
				case 0x0397:		//	Î—	0xce 0x97	GREEK CAPITAL LETTER ETA
					*p = 0x03B7;	//	Î·	0xce 0xb7	GREEK SMALL LETTER ETA
					break;
				case 0x0398:		//	Î˜	0xce 0x98	GREEK CAPITAL LETTER THETA
					*p = 0x03B8;	//	Î¸	0xce 0xb8	GREEK SMALL LETTER THETA
					break;
				case 0x0399:		//	Î™	0xce 0x99	GREEK CAPITAL LETTER IOTA
					*p = 0x03B9;	//	Î¹	0xce 0xb9	GREEK SMALL LETTER IOTA
					break;
				case 0x039A:		//	Îš	0xce 0x9a	GREEK CAPITAL LETTER KAPPA
					*p = 0x03BA;	//	Îº	0xce 0xba	GREEK SMALL LETTER KAPPA
					break;
				case 0x039B:		//	Î›	0xce 0x9b	GREEK CAPITAL LETTER LAMDA
					*p = 0x03BB;	//	Î»	0xce 0xbb	GREEK SMALL LETTER LAMDA
					break;
				case 0x039C:		//	Îœ	0xce 0x9c	GREEK CAPITAL LETTER MU
					*p = 0x03BC;	//	Î¼	0xce 0xbc	GREEK SMALL LETTER MU
					break;
				case 0x039D:		//	Î	0xce 0x9d	GREEK CAPITAL LETTER NU
					*p = 0x03BD;	//	Î½	0xce 0xbd	GREEK SMALL LETTER NU
					break;
				case 0x039E:		//	Îž	0xce 0x9e	GREEK CAPITAL LETTER XI
					*p = 0x03BE;	//	Î¾	0xce 0xbe	GREEK SMALL LETTER XI
					break;
				case 0x039F:		//	ÎŸ	0xce 0x9f	GREEK CAPITAL LETTER OMICRON
					*p = 0x03BF;	//	Î¿	0xce 0xbf	GREEK SMALL LETTER OMICRON
					break;
				case 0x03A0:		//	Î 	0xce 0xa0	GREEK CAPITAL LETTER PI
					*p = 0x03C0;	//	Ï€	0xcf 0x80	GREEK SMALL LETTER PI
					break;
				case 0x03A1:		//	Î¡	0xce 0xa1	GREEK CAPITAL LETTER RHO
					*p = 0x03C1;	//	Ï	0xcf 0x81	GREEK SMALL LETTER RHO
					break;
				case 0x03C2:		//	Ï‚	0xcf 0x82	GREEK SMALL FINAL SIGMA // To handle sigma as the same character in search
				case 0x03A3:		//	Î£	0xce 0xa3	GREEK CAPITAL LETTER SIGMA
					*p = 0x03C3;	//	Ïƒ	0xcf 0x83	GREEK SMALL LETTER SIGMA
					break;
				case 0x03A4:		//	Î¤	0xce 0xa4	GREEK CAPITAL LETTER TAU
					*p = 0x03C4;	//	Ï„	0xcf 0x84	GREEK SMALL LETTER TAU
					break;
				case 0x03A5:		//	Î¥	0xce 0xa5	GREEK CAPITAL LETTER UPSILON
					*p = 0x03C5;	//	Ï…	0xcf 0x85	GREEK SMALL LETTER UPSILON
					break;
				case 0x03A6:		//	Î¦	0xce 0xa6	GREEK CAPITAL LETTER PHI
					*p = 0x03C6;	//	Ï†	0xcf 0x86	GREEK SMALL LETTER PHI
					break;
				case 0x03A7:		//	Î§	0xce 0xa7	GREEK CAPITAL LETTER CHI
					*p = 0x03C7;	//	Ï‡	0xcf 0x87	GREEK SMALL LETTER CHI
					break;
				case 0x03A8:		//	Î¨	0xce 0xa8	GREEK CAPITAL LETTER PSI
					*p = 0x03C8;	//	Ïˆ	0xcf 0x88	GREEK SMALL LETTER PSI
					break;
				case 0x03A9:		//	Î©	0xce 0xa9	GREEK CAPITAL LETTER OMEGA
					*p = 0x03C9;	//	Ï‰	0xcf 0x89	GREEK SMALL LETTER OMEGA
					break;
				case 0x03AA:		//	Îª	0xce 0xaa	GREEK CAPITAL LETTER IOTA WITH DIALYTIKA
					*p = 0x03CA;	//	ÏŠ	0xcf 0x8a	GREEK SMALL LETTER IOTA WITH DIALYTIKA
					break;
				case 0x03AB:		//	Î«	0xce 0xab	GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA
					*p = 0x03CB;	//	Ï‹	0xcf 0x8b	GREEK SMALL LETTER UPSILON WITH DIALYTIKA
					break;
				case 0x03CF:		//	Ï	0xcf 0x8f	GREEK CAPITAL LETTER KAI SYMBOL
					*p = 0x03D7;	//	Ï—	0xcf 0x97	GREEK SMALL LETTER KAI SYMBOL
					break;
				case 0x03D8:		//	Ï˜	0xcf 0x98	GREEK CAPITAL LETTER ARCHAIC KOPPA
					*p = 0x03D9;	//	Ï™	0xcf 0x99	GREEK SMALL LETTER ARCHAIC KOPPA
					break;
				case 0x03DA:		//	Ïš	0xcf 0x9a	GREEK CAPITAL LETTER STIGMA
					*p = 0x03DB;	//	Ï›	0xcf 0x9b	GREEK SMALL LETTER STIGMA
					break;
				case 0x03DC:		//	Ïœ	0xcf 0x9c	GREEK CAPITAL LETTER DIGAMMA
					*p = 0x03DD;	//	Ï	0xcf 0x9d	GREEK SMALL LETTER DIGAMMA
					break;
				case 0x03DE:		//	Ïž	0xcf 0x9e	GREEK CAPITAL LETTER KOPPA
					*p = 0x03DF;	//	ÏŸ	0xcf 0x9f	GREEK SMALL LETTER KOPPA
					break;
				case 0x03E0:		//	Ï 	0xcf 0xa0	GREEK CAPITAL LETTER SAMPI
					*p = 0x03E1;	//	Ï¡	0xcf 0xa1	GREEK SMALL LETTER SAMPI
					break;
				case 0x03E2:		//	Ï¢	0xcf 0xa2	COPTIC CAPITAL LETTER SHEI
					*p = 0x03E3;	//	Ï£	0xcf 0xa3	COPTIC SMALL LETTER SHEI
					break;
				case 0x03E4:		//	Ï¤	0xcf 0xa4	COPTIC CAPITAL LETTER FEI
					*p = 0x03E5;	//	Ï¥	0xcf 0xa5	COPTIC SMALL LETTER FEI
					break;
				case 0x03E6:		//	Ï¦	0xcf 0xa6	COPTIC CAPITAL LETTER KHEI
					*p = 0x03E7;	//	Ï§	0xcf 0xa7	COPTIC SMALL LETTER KHEI
					break;
				case 0x03E8:		//	Ï¨	0xcf 0xa8	COPTIC CAPITAL LETTER HORI
					*p = 0x03E9;	//	Ï©	0xcf 0xa9	COPTIC SMALL LETTER HORI
					break;
				case 0x03EA:		//	Ïª	0xcf 0xaa	COPTIC CAPITAL LETTER GANGIA
					*p = 0x03EB;	//	Ï«	0xcf 0xab	COPTIC SMALL LETTER GANGIA
					break;
				case 0x03EC:		//	Ï¬	0xcf 0xac	COPTIC CAPITAL LETTER SHIMA
					*p = 0x03ED;	//	Ï­	0xcf 0xad	COPTIC SMALL LETTER SHIMA
					break;
				case 0x03EE:		//	Ï®	0xcf 0xae	COPTIC CAPITAL LETTER DEI
					*p = 0x03EF;	//	Ï¯	0xcf 0xaf	COPTIC SMALL LETTER DEI
					break;
				case 0x03F4:		//	Ï´	0xcf 0xb4	GREEK CAPITAL LETTER THETA SYMBOL
					*p = 0x03D1;	//	Ï‘	0xcf 0x91	GREEK SMALL LETTER THETA SYMBOL
					break;
				case 0x03F7:		//	Ï·	0xcf 0xb7	GREEK CAPITAL LETTER SHO
					*p = 0x03F8;	//	Ï¸	0xcf 0xb8	GREEK SMALL LETTER SHO
					break;
				case 0x03F9:		//	Ï¹	0xcf 0xb9	GREEK CAPITAL LETTER LUNATE SIGMA SYMBOL
					*p = 0x03F2;	//	Ï²	0xcf 0xb2	GREEK SMALL LETTER LUNATE SIGMA SYMBOL
					break;
				case 0x03FA:		//	Ïº	0xcf 0xba	GREEK CAPITAL LETTER SAN
					*p = 0x03FB;	//	Ï»	0xcf 0xbb	GREEK SMALL LETTER SAN
					break;
				case 0x03FD:		//	Ï½	0xcf 0xbd	GREEK CAPITAL LETTER REVERSED LUNATE SIGMA SYMBOL
					*p = 0x037B;	//	Í»	0xcd 0xbb	GREEK SMALL LETTER REVERSED LUNATE SIGMA SYMBOL
					break;
				case 0x03FE:		//	Ï¾	0xcf 0xbe	GREEK CAPITAL LETTER DOTTED LUNATE SIGMA SYMBOL
					*p = 0x037C;	//	Í¼	0xcd 0xbc	GREEK SMALL LETTER DOTTED LUNATE SIGMA SYMBOL
					break;
				case 0x03FF:		//	Ï¿	0xcf 0xbf	GREEK CAPITAL LETTER REVERSED DOTTED LUNATE SIGMA SYMBOL
					*p = 0x037D;	//	Í½	0xcd 0xbd	GREEK SMALL LETTER REVERSED DOTTED LUNATE SIGMA SYMBOL
					break;
				case 0x0400:		//	Ð€	0xd0 0x80	CYRILLIC CAPITAL LETTER IE WITH GRAVE
					*p = 0x0450;	//	Ñ	0xd1 0x90	CYRILLIC SMALL LETTER IE WITH GRAVE
					break;
				case 0x0401:		//	Ð	0xd0 0x81	CYRILLIC CAPITAL LETTER IO
					*p = 0x0451;	//	Ñ‘	0xd1 0x91	CYRILLIC SMALL LETTER IO
					break;
				case 0x0402:		//	Ð‚	0xd0 0x82	CYRILLIC CAPITAL LETTER DJE
					*p = 0x0452;	//	Ñ’	0xd1 0x92	CYRILLIC SMALL LETTER DJE
					break;
				case 0x0403:		//	Ðƒ	0xd0 0x83	CYRILLIC CAPITAL LETTER GJE
					*p = 0x0453;	//	Ñ“	0xd1 0x93	CYRILLIC SMALL LETTER GJE
					break;
				case 0x0404:		//	Ð„	0xd0 0x84	CYRILLIC CAPITAL LETTER UKRAINIAN IE
					*p = 0x0454;	//	Ñ”	0xd1 0x94	CYRILLIC SMALL LETTER UKRAINIAN IE
					break;
				case 0x0405:		//	Ð…	0xd0 0x85	CYRILLIC CAPITAL LETTER DZE
					*p = 0x0455;	//	Ñ•	0xd1 0x95	CYRILLIC SMALL LETTER DZE
					break;
				case 0x0406:		//	Ð†	0xd0 0x86	CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
					*p = 0x0456;	//	Ñ–	0xd1 0x96	CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
					break;
				case 0x0407:		//	Ð‡	0xd0 0x87	CYRILLIC CAPITAL LETTER YI
					*p = 0x0457;	//	Ñ—	0xd1 0x97	CYRILLIC SMALL LETTER YI
					break;
				case 0x0408:		//	Ðˆ	0xd0 0x88	CYRILLIC CAPITAL LETTER JE
					*p = 0x0458;	//	Ñ˜	0xd1 0x98	CYRILLIC SMALL LETTER JE
					break;
				case 0x0409:		//	Ð‰	0xd0 0x89	CYRILLIC CAPITAL LETTER LJE
					*p = 0x0459;	//	Ñ™	0xd1 0x99	CYRILLIC SMALL LETTER LJE
					break;
				case 0x040A:		//	ÐŠ	0xd0 0x8a	CYRILLIC CAPITAL LETTER NJE
					*p = 0x045A;	//	Ñš	0xd1 0x9a	CYRILLIC SMALL LETTER NJE
					break;
				case 0x040B:		//	Ð‹	0xd0 0x8b	CYRILLIC CAPITAL LETTER TSHE
					*p = 0x045B;	//	Ñ›	0xd1 0x9b	CYRILLIC SMALL LETTER TSHE
					break;
				case 0x040C:		//	ÐŒ	0xd0 0x8c	CYRILLIC CAPITAL LETTER KJE
					*p = 0x045C;	//	Ñœ	0xd1 0x9c	CYRILLIC SMALL LETTER KJE
					break;
				case 0x040D:		//	Ð	0xd0 0x8d	CYRILLIC CAPITAL LETTER I WITH GRAVE
					*p = 0x045D;	//	Ñ	0xd1 0x9d	CYRILLIC SMALL LETTER I WITH GRAVE
					break;
				case 0x040E:		//	ÐŽ	0xd0 0x8e	CYRILLIC CAPITAL LETTER SHORT U
					*p = 0x045E;	//	Ñž	0xd1 0x9e	CYRILLIC SMALL LETTER SHORT U
					break;
				case 0x040F:		//	Ð	0xd0 0x8f	CYRILLIC CAPITAL LETTER DZHE
					*p = 0x045F;	//	ÑŸ	0xd1 0x9f	CYRILLIC SMALL LETTER DZHE
					break;
				case 0x0410:		//	Ð	0xd0 0x90	CYRILLIC CAPITAL LETTER A
					*p = 0x0430;	//	Ð°	0xd0 0xb0	CYRILLIC SMALL LETTER A
					break;
				case 0x0411:		//	Ð‘	0xd0 0x91	CYRILLIC CAPITAL LETTER BE
					*p = 0x0431;	//	Ð±	0xd0 0xb1	CYRILLIC SMALL LETTER BE
					break;
				case 0x0412:		//	Ð’	0xd0 0x92	CYRILLIC CAPITAL LETTER VE
					*p = 0x0432;	//	Ð²	0xd0 0xb2	CYRILLIC SMALL LETTER VE
					break;
				case 0x0413:		//	Ð“	0xd0 0x93	CYRILLIC CAPITAL LETTER GHE
					*p = 0x0433;	//	Ð³	0xd0 0xb3	CYRILLIC SMALL LETTER GHE
					break;
				case 0x0414:		//	Ð”	0xd0 0x94	CYRILLIC CAPITAL LETTER DE
					*p = 0x0434;	//	Ð´	0xd0 0xb4	CYRILLIC SMALL LETTER DE
					break;
				case 0x0415:		//	Ð•	0xd0 0x95	CYRILLIC CAPITAL LETTER IE
					*p = 0x0435;	//	Ðµ	0xd0 0xb5	CYRILLIC SMALL LETTER IE
					break;
				case 0x0416:		//	Ð–	0xd0 0x96	CYRILLIC CAPITAL LETTER ZHE
					*p = 0x0436;	//	Ð¶	0xd0 0xb6	CYRILLIC SMALL LETTER ZHE
					break;
				case 0x0417:		//	Ð—	0xd0 0x97	CYRILLIC CAPITAL LETTER ZE
					*p = 0x0437;	//	Ð·	0xd0 0xb7	CYRILLIC SMALL LETTER ZE
					break;
				case 0x0418:		//	Ð˜	0xd0 0x98	CYRILLIC CAPITAL LETTER I
					*p = 0x0438;	//	Ð¸	0xd0 0xb8	CYRILLIC SMALL LETTER I
					break;
				case 0x0419:		//	Ð™	0xd0 0x99	CYRILLIC CAPITAL LETTER SHORT I
					*p = 0x0439;	//	Ð¹	0xd0 0xb9	CYRILLIC SMALL LETTER SHORT I
					break;
				case 0x041A:		//	Ðš	0xd0 0x9a	CYRILLIC CAPITAL LETTER KA
					*p = 0x043A;	//	Ðº	0xd0 0xba	CYRILLIC SMALL LETTER KA
					break;
				case 0x041B:		//	Ð›	0xd0 0x9b	CYRILLIC CAPITAL LETTER EL
					*p = 0x043B;	//	Ð»	0xd0 0xbb	CYRILLIC SMALL LETTER EL
					break;
				case 0x041C:		//	Ðœ	0xd0 0x9c	CYRILLIC CAPITAL LETTER EM
					*p = 0x043C;	//	Ð¼	0xd0 0xbc	CYRILLIC SMALL LETTER EM
					break;
				case 0x041D:		//	Ð	0xd0 0x9d	CYRILLIC CAPITAL LETTER EN
					*p = 0x043D;	//	Ð½	0xd0 0xbd	CYRILLIC SMALL LETTER EN
					break;
				case 0x041E:		//	Ðž	0xd0 0x9e	CYRILLIC CAPITAL LETTER O
					*p = 0x043E;	//	Ð¾	0xd0 0xbe	CYRILLIC SMALL LETTER O
					break;
				case 0x041F:		//	ÐŸ	0xd0 0x9f	CYRILLIC CAPITAL LETTER PE
					*p = 0x043F;	//	Ð¿	0xd0 0xbf	CYRILLIC SMALL LETTER PE
					break;
				case 0x0420:		//	Ð 	0xd0 0xa0	CYRILLIC CAPITAL LETTER ER
					*p = 0x0440;	//	Ñ€	0xd1 0x80	CYRILLIC SMALL LETTER ER
					break;
				case 0x0421:		//	Ð¡	0xd0 0xa1	CYRILLIC CAPITAL LETTER ES
					*p = 0x0441;	//	Ñ	0xd1 0x81	CYRILLIC SMALL LETTER ES
					break;
				case 0x0422:		//	Ð¢	0xd0 0xa2	CYRILLIC CAPITAL LETTER TE
					*p = 0x0442;	//	Ñ‚	0xd1 0x82	CYRILLIC SMALL LETTER TE
					break;
				case 0x0423:		//	Ð£	0xd0 0xa3	CYRILLIC CAPITAL LETTER U
					*p = 0x0443;	//	Ñƒ	0xd1 0x83	CYRILLIC SMALL LETTER U
					break;
				case 0x0424:		//	Ð¤	0xd0 0xa4	CYRILLIC CAPITAL LETTER EF
					*p = 0x0444;	//	Ñ„	0xd1 0x84	CYRILLIC SMALL LETTER EF
					break;
				case 0x0425:		//	Ð¥	0xd0 0xa5	CYRILLIC CAPITAL LETTER HA
					*p = 0x0445;	//	Ñ…	0xd1 0x85	CYRILLIC SMALL LETTER HA
					break;
				case 0x0426:		//	Ð¦	0xd0 0xa6	CYRILLIC CAPITAL LETTER TSE
					*p = 0x0446;	//	Ñ†	0xd1 0x86	CYRILLIC SMALL LETTER TSE
					break;
				case 0x0427:		//	Ð§	0xd0 0xa7	CYRILLIC CAPITAL LETTER CHE
					*p = 0x0447;	//	Ñ‡	0xd1 0x87	CYRILLIC SMALL LETTER CHE
					break;
				case 0x0428:		//	Ð¨	0xd0 0xa8	CYRILLIC CAPITAL LETTER SHA
					*p = 0x0448;	//	Ñˆ	0xd1 0x88	CYRILLIC SMALL LETTER SHA
					break;
				case 0x0429:		//	Ð©	0xd0 0xa9	CYRILLIC CAPITAL LETTER SHCHA
					*p = 0x0449;	//	Ñ‰	0xd1 0x89	CYRILLIC SMALL LETTER SHCHA
					break;
				case 0x042A:		//	Ðª	0xd0 0xaa	CYRILLIC CAPITAL LETTER HARD SIGN
					*p = 0x044A;	//	ÑŠ	0xd1 0x8a	CYRILLIC SMALL LETTER HARD SIGN
					break;
				case 0x042B:		//	Ð«	0xd0 0xab	CYRILLIC CAPITAL LETTER YERU
					*p = 0x044B;	//	Ñ‹	0xd1 0x8b	CYRILLIC SMALL LETTER YERU
					break;
				case 0x042C:		//	Ð¬	0xd0 0xac	CYRILLIC CAPITAL LETTER SOFT SIGN
					*p = 0x044C;	//	ÑŒ	0xd1 0x8c	CYRILLIC SMALL LETTER SOFT SIGN
					break;
				case 0x042D:		//	Ð­	0xd0 0xad	CYRILLIC CAPITAL LETTER E
					*p = 0x044D;	//	Ñ	0xd1 0x8d	CYRILLIC SMALL LETTER E
					break;
				case 0x042E:		//	Ð®	0xd0 0xae	CYRILLIC CAPITAL LETTER YU
					*p = 0x044E;	//	ÑŽ	0xd1 0x8e	CYRILLIC SMALL LETTER YU
					break;
				case 0x042F:		//	Ð¯	0xd0 0xaf	CYRILLIC CAPITAL LETTER YA
					*p = 0x044F;	//	Ñ	0xd1 0x8f	CYRILLIC SMALL LETTER YA
					break;
				case 0x0460:		//	Ñ 	0xd1 0xa0	CYRILLIC CAPITAL LETTER OMEGA
					*p = 0x0461;	//	Ñ¡	0xd1 0xa1	CYRILLIC SMALL LETTER OMEGA
					break;
				case 0x0462:		//	Ñ¢	0xd1 0xa2	CYRILLIC CAPITAL LETTER YAT
					*p = 0x0463;	//	Ñ£	0xd1 0xa3	CYRILLIC SMALL LETTER YAT
					break;
				case 0x0464:		//	Ñ¤	0xd1 0xa4	CYRILLIC CAPITAL LETTER IOTIFIED E
					*p = 0x0465;	//	Ñ¥	0xd1 0xa5	CYRILLIC SMALL LETTER IOTIFIED E
					break;
				case 0x0466:		//	Ñ¦	0xd1 0xa6	CYRILLIC CAPITAL LETTER LITTLE YUS
					*p = 0x0467;	//	Ñ§	0xd1 0xa7	CYRILLIC SMALL LETTER LITTLE YUS
					break;
				case 0x0468:		//	Ñ¨	0xd1 0xa8	CYRILLIC CAPITAL LETTER IOTIFIED LITTLE YUS
					*p = 0x0469;	//	Ñ©	0xd1 0xa9	CYRILLIC SMALL LETTER IOTIFIED LITTLE YUS
					break;
				case 0x046A:		//	Ñª	0xd1 0xaa	CYRILLIC CAPITAL LETTER BIG YUS
					*p = 0x046B;	//	Ñ«	0xd1 0xab	CYRILLIC SMALL LETTER BIG YUS
					break;
				case 0x046C:		//	Ñ¬	0xd1 0xac	CYRILLIC CAPITAL LETTER IOTIFIED BIG YUS
					*p = 0x046D;	//	Ñ­	0xd1 0xad	CYRILLIC SMALL LETTER IOTIFIED BIG YUS
					break;
				case 0x046E:		//	Ñ®	0xd1 0xae	CYRILLIC CAPITAL LETTER KSI
					*p = 0x046F;	//	Ñ¯	0xd1 0xaf	CYRILLIC SMALL LETTER KSI
					break;
				case 0x0470:		//	Ñ°	0xd1 0xb0	CYRILLIC CAPITAL LETTER PSI
					*p = 0x0471;	//	Ñ±	0xd1 0xb1	CYRILLIC SMALL LETTER PSI
					break;
				case 0x0472:		//	Ñ²	0xd1 0xb2	CYRILLIC CAPITAL LETTER FITA
					*p = 0x0473;	//	Ñ³	0xd1 0xb3	CYRILLIC SMALL LETTER FITA
					break;
				case 0x0474:		//	Ñ´	0xd1 0xb4	CYRILLIC CAPITAL LETTER IZHITSA
					*p = 0x0475;	//	Ñµ	0xd1 0xb5	CYRILLIC SMALL LETTER IZHITSA
					break;
				case 0x0476:		//	Ñ¶	0xd1 0xb6	CYRILLIC CAPITAL LETTER IZHITSA WITH DOUBLE GRAVE ACCENT
					*p = 0x0477;	//	Ñ·	0xd1 0xb7	CYRILLIC SMALL LETTER IZHITSA WITH DOUBLE GRAVE ACCENT
					break;
				case 0x0478:		//	Ñ¸	0xd1 0xb8	CYRILLIC CAPITAL LETTER UK
					*p = 0x0479;	//	Ñ¹	0xd1 0xb9	CYRILLIC SMALL LETTER UK
					break;
				case 0x047A:		//	Ñº	0xd1 0xba	CYRILLIC CAPITAL LETTER ROUND OMEGA
					*p = 0x047B;	//	Ñ»	0xd1 0xbb	CYRILLIC SMALL LETTER ROUND OMEGA
					break;
				case 0x047C:		//	Ñ¼	0xd1 0xbc	CYRILLIC CAPITAL LETTER OMEGA WITH TITLO
					*p = 0x047D;	//	Ñ½	0xd1 0xbd	CYRILLIC SMALL LETTER OMEGA WITH TITLO
					break;
				case 0x047E:		//	Ñ¾	0xd1 0xbe	CYRILLIC CAPITAL LETTER OT
					*p = 0x047F;	//	Ñ¿	0xd1 0xbf	CYRILLIC SMALL LETTER OT
					break;
				case 0x0480:		//	Ò€	0xd2 0x80	CYRILLIC CAPITAL LETTER KOPPA
					*p = 0x0481;	//	Ò	0xd2 0x81	CYRILLIC SMALL LETTER KOPPA
					break;
				case 0x048A:		//	ÒŠ	0xd2 0x8a	CYRILLIC CAPITAL LETTER SHORT I WITH TAIL
					*p = 0x048B;	//	Ò‹	0xd2 0x8b	CYRILLIC SMALL LETTER SHORT I WITH TAIL
					break;
				case 0x048C:		//	ÒŒ	0xd2 0x8c	CYRILLIC CAPITAL LETTER SEMISOFT SIGN
					*p = 0x048D;	//	Ò	0xd2 0x8d	CYRILLIC SMALL LETTER SEMISOFT SIGN
					break;
				case 0x048E:		//	ÒŽ	0xd2 0x8e	CYRILLIC CAPITAL LETTER ER WITH TICK
					*p = 0x048F;	//	Ò	0xd2 0x8f	CYRILLIC SMALL LETTER ER WITH TICK
					break;
				case 0x0490:		//	Ò	0xd2 0x90	CYRILLIC CAPITAL LETTER GHE WITH UPTURN
					*p = 0x0491;	//	Ò‘	0xd2 0x91	CYRILLIC SMALL LETTER GHE WITH UPTURN
					break;
				case 0x0492:		//	Ò’	0xd2 0x92	CYRILLIC CAPITAL LETTER GHE WITH STROKE
					*p = 0x0493;	//	Ò“	0xd2 0x93	CYRILLIC SMALL LETTER GHE WITH STROKE
					break;
				case 0x0494:		//	Ò”	0xd2 0x94	CYRILLIC CAPITAL LETTER GHE WITH MIDDLE HOOK
					*p = 0x0495;	//	Ò•	0xd2 0x95	CYRILLIC SMALL LETTER GHE WITH MIDDLE HOOK
					break;
				case 0x0496:		//	Ò–	0xd2 0x96	CYRILLIC CAPITAL LETTER ZHE WITH DESCENDER
					*p = 0x0497;	//	Ò—	0xd2 0x97	CYRILLIC SMALL LETTER ZHE WITH DESCENDER
					break;
				case 0x0498:		//	Ò˜	0xd2 0x98	CYRILLIC CAPITAL LETTER ZE WITH DESCENDER
					*p = 0x0499;	//	Ò™	0xd2 0x99	CYRILLIC SMALL LETTER ZE WITH DESCENDER
					break;
				case 0x049A:		//	Òš	0xd2 0x9a	CYRILLIC CAPITAL LETTER KA WITH DESCENDER
					*p = 0x049B;	//	Ò›	0xd2 0x9b	CYRILLIC SMALL LETTER KA WITH DESCENDER
					break;
				case 0x049C:		//	Òœ	0xd2 0x9c	CYRILLIC CAPITAL LETTER KA WITH VERTICAL STROKE
					*p = 0x049D;	//	Ò	0xd2 0x9d	CYRILLIC SMALL LETTER KA WITH VERTICAL STROKE
					break;
				case 0x049E:		//	Òž	0xd2 0x9e	CYRILLIC CAPITAL LETTER KA WITH STROKE
					*p = 0x049F;	//	ÒŸ	0xd2 0x9f	CYRILLIC SMALL LETTER KA WITH STROKE
					break;
				case 0x04A0:		//	Ò 	0xd2 0xa0	CYRILLIC CAPITAL LETTER BASHKIR KA
					*p = 0x04A1;	//	Ò¡	0xd2 0xa1	CYRILLIC SMALL LETTER BASHKIR KA
					break;
				case 0x04A2:		//	Ò¢	0xd2 0xa2	CYRILLIC CAPITAL LETTER EN WITH DESCENDER
					*p = 0x04A3;	//	Ò£	0xd2 0xa3	CYRILLIC SMALL LETTER EN WITH DESCENDER
					break;
				case 0x04A4:		//	Ò¤	0xd2 0xa4	CYRILLIC CAPITAL LETTER LIGATURE EN GHE
					*p = 0x04A5;	//	Ò¥	0xd2 0xa5	CYRILLIC SMALL LETTER LIGATURE EN GHE
					break;
				case 0x04A6:		//	Ò¦	0xd2 0xa6	CYRILLIC CAPITAL LETTER PE WITH MIDDLE HOOK
					*p = 0x04A7;	//	Ò§	0xd2 0xa7	CYRILLIC SMALL LETTER PE WITH MIDDLE HOOK
					break;
				case 0x04A8:		//	Ò¨	0xd2 0xa8	CYRILLIC CAPITAL LETTER ABKHASIAN HA
					*p = 0x04A9;	//	Ò©	0xd2 0xa9	CYRILLIC SMALL LETTER ABKHASIAN HA
					break;
				case 0x04AA:		//	Òª	0xd2 0xaa	CYRILLIC CAPITAL LETTER ES WITH DESCENDER
					*p = 0x04AB;	//	Ò«	0xd2 0xab	CYRILLIC SMALL LETTER ES WITH DESCENDER
					break;
				case 0x04AC:		//	Ò¬	0xd2 0xac	CYRILLIC CAPITAL LETTER TE WITH DESCENDER
					*p = 0x04AD;	//	Ò­	0xd2 0xad	CYRILLIC SMALL LETTER TE WITH DESCENDER
					break;
				case 0x04AE:		//	Ò®	0xd2 0xae	CYRILLIC CAPITAL LETTER STRAIGHT U
					*p = 0x04AF;	//	Ò¯	0xd2 0xaf	CYRILLIC SMALL LETTER STRAIGHT U
					break;
				case 0x04B0:		//	Ò°	0xd2 0xb0	CYRILLIC CAPITAL LETTER STRAIGHT U WITH STROKE
					*p = 0x04B1;	//	Ò±	0xd2 0xb1	CYRILLIC SMALL LETTER STRAIGHT U WITH STROKE
					break;
				case 0x04B2:		//	Ò²	0xd2 0xb2	CYRILLIC CAPITAL LETTER HA WITH DESCENDER
					*p = 0x04B3;	//	Ò³	0xd2 0xb3	CYRILLIC SMALL LETTER HA WITH DESCENDER
					break;
				case 0x04B4:		//	Ò´	0xd2 0xb4	CYRILLIC CAPITAL LETTER LIGATURE TE TSE
					*p = 0x04B5;	//	Òµ	0xd2 0xb5	CYRILLIC SMALL LETTER LIGATURE TE TSE
					break;
				case 0x04B6:		//	Ò¶	0xd2 0xb6	CYRILLIC CAPITAL LETTER CHE WITH DESCENDER
					*p = 0x04B7;	//	Ò·	0xd2 0xb7	CYRILLIC SMALL LETTER CHE WITH DESCENDER
					break;
				case 0x04B8:		//	Ò¸	0xd2 0xb8	CYRILLIC CAPITAL LETTER CHE WITH VERTICAL STROKE
					*p = 0x04B9;	//	Ò¹	0xd2 0xb9	CYRILLIC SMALL LETTER CHE WITH VERTICAL STROKE
					break;
				case 0x04BA:		//	Òº	0xd2 0xba	CYRILLIC CAPITAL LETTER SHHA
					*p = 0x04BB;	//	Ò»	0xd2 0xbb	CYRILLIC SMALL LETTER SHHA
					break;
				case 0x04BC:		//	Ò¼	0xd2 0xbc	CYRILLIC CAPITAL LETTER ABKHASIAN CHE
					*p = 0x04BD;	//	Ò½	0xd2 0xbd	CYRILLIC SMALL LETTER ABKHASIAN CHE
					break;
				case 0x04BE:		//	Ò¾	0xd2 0xbe	CYRILLIC CAPITAL LETTER ABKHASIAN CHE WITH DESCENDER
					*p = 0x04BF;	//	Ò¿	0xd2 0xbf	CYRILLIC SMALL LETTER ABKHASIAN CHE WITH DESCENDER
					break;
				case 0x04C0:		//	Ó€	0xd3 0x80	CYRILLIC CAPITAL LETTER PALOCHKA
					*p = 0x04CF;	//	Ó	0xd3 0x8f	CYRILLIC SMALL LETTER PALOCHKA
					break;
				case 0x04C1:		//	Ó	0xd3 0x81	CYRILLIC CAPITAL LETTER ZHE WITH BREVE
					*p = 0x04C2;	//	Ó‚	0xd3 0x82	CYRILLIC SMALL LETTER ZHE WITH BREVE
					break;
				case 0x04C3:		//	Óƒ	0xd3 0x83	CYRILLIC CAPITAL LETTER KA WITH HOOK
					*p = 0x04C4;	//	Ó„	0xd3 0x84	CYRILLIC SMALL LETTER KA WITH HOOK
					break;
				case 0x04C5:		//	Ó…	0xd3 0x85	CYRILLIC CAPITAL LETTER EL WITH TAIL
					*p = 0x04C6;	//	Ó†	0xd3 0x86	CYRILLIC SMALL LETTER EL WITH TAIL
					break;
				case 0x04C7:		//	Ó‡	0xd3 0x87	CYRILLIC CAPITAL LETTER EN WITH HOOK
					*p = 0x04C8;	//	Óˆ	0xd3 0x88	CYRILLIC SMALL LETTER EN WITH HOOK
					break;
				case 0x04C9:		//	Ó‰	0xd3 0x89	CYRILLIC CAPITAL LETTER EN WITH TAIL
					*p = 0x04CA;	//	ÓŠ	0xd3 0x8a	CYRILLIC SMALL LETTER EN WITH TAIL
					break;
				case 0x04CB:		//	Ó‹	0xd3 0x8b	CYRILLIC CAPITAL LETTER KHAKASSIAN CHE
					*p = 0x04CC;	//	ÓŒ	0xd3 0x8c	CYRILLIC SMALL LETTER KHAKASSIAN CHE
					break;
				case 0x04CD:		//	Ó	0xd3 0x8d	CYRILLIC CAPITAL LETTER EM WITH TAIL
					*p = 0x04CE;	//	ÓŽ	0xd3 0x8e	CYRILLIC SMALL LETTER EM WITH TAIL
					break;
				case 0x04D0:		//	Ó	0xd3 0x90	CYRILLIC CAPITAL LETTER A WITH BREVE
					*p = 0x04D1;	//	Ó‘	0xd3 0x91	CYRILLIC SMALL LETTER A WITH BREVE
					break;
				case 0x04D2:		//	Ó’	0xd3 0x92	CYRILLIC CAPITAL LETTER A WITH DIAERESIS
					*p = 0x04D3;	//	Ó“	0xd3 0x93	CYRILLIC SMALL LETTER A WITH DIAERESIS
					break;
				case 0x04D4:		//	Ó”	0xd3 0x94	CYRILLIC CAPITAL LETTER LIGATURE A IE
					*p = 0x04D5;	//	Ó•	0xd3 0x95	CYRILLIC SMALL LETTER LIGATURE A IE
					break;
				case 0x04D6:		//	Ó–	0xd3 0x96	CYRILLIC CAPITAL LETTER IE WITH BREVE
					*p = 0x04D7;	//	Ó—	0xd3 0x97	CYRILLIC SMALL LETTER IE WITH BREVE
					break;
				case 0x04D8:		//	Ó˜	0xd3 0x98	CYRILLIC CAPITAL LETTER SCHWA
					*p = 0x04D9;	//	Ó™	0xd3 0x99	CYRILLIC SMALL LETTER SCHWA
					break;
				case 0x04DA:		//	Óš	0xd3 0x9a	CYRILLIC CAPITAL LETTER SCHWA WITH DIAERESIS
					*p = 0x04DB;	//	Ó›	0xd3 0x9b	CYRILLIC SMALL LETTER SCHWA WITH DIAERESIS
					break;
				case 0x04DC:		//	Óœ	0xd3 0x9c	CYRILLIC CAPITAL LETTER ZHE WITH DIAERESIS
					*p = 0x04DD;	//	Ó	0xd3 0x9d	CYRILLIC SMALL LETTER ZHE WITH DIAERESIS
					break;
				case 0x04DE:		//	Óž	0xd3 0x9e	CYRILLIC CAPITAL LETTER ZE WITH DIAERESIS
					*p = 0x04DF;	//	ÓŸ	0xd3 0x9f	CYRILLIC SMALL LETTER ZE WITH DIAERESIS
					break;
				case 0x04E0:		//	Ó 	0xd3 0xa0	CYRILLIC CAPITAL LETTER ABKHASIAN DZE
					*p = 0x04E1;	//	Ó¡	0xd3 0xa1	CYRILLIC SMALL LETTER ABKHASIAN DZE
					break;
				case 0x04E2:		//	Ó¢	0xd3 0xa2	CYRILLIC CAPITAL LETTER I WITH MACRON
					*p = 0x04E3;	//	Ó£	0xd3 0xa3	CYRILLIC SMALL LETTER I WITH MACRON
					break;
				case 0x04E4:		//	Ó¤	0xd3 0xa4	CYRILLIC CAPITAL LETTER I WITH DIAERESIS
					*p = 0x04E5;	//	Ó¥	0xd3 0xa5	CYRILLIC SMALL LETTER I WITH DIAERESIS
					break;
				case 0x04E6:		//	Ó¦	0xd3 0xa6	CYRILLIC CAPITAL LETTER O WITH DIAERESIS
					*p = 0x04E7;	//	Ó§	0xd3 0xa7	CYRILLIC SMALL LETTER O WITH DIAERESIS
					break;
				case 0x04E8:		//	Ó¨	0xd3 0xa8	CYRILLIC CAPITAL LETTER BARRED O
					*p = 0x04E9;	//	Ó©	0xd3 0xa9	CYRILLIC SMALL LETTER BARRED O
					break;
				case 0x04EA:		//	Óª	0xd3 0xaa	CYRILLIC CAPITAL LETTER BARRED O WITH DIAERESIS
					*p = 0x04EB;	//	Ó«	0xd3 0xab	CYRILLIC SMALL LETTER BARRED O WITH DIAERESIS
					break;
				case 0x04EC:		//	Ó¬	0xd3 0xac	CYRILLIC CAPITAL LETTER E WITH DIAERESIS
					*p = 0x04ED;	//	Ó­	0xd3 0xad	CYRILLIC SMALL LETTER E WITH DIAERESIS
					break;
				case 0x04EE:		//	Ó®	0xd3 0xae	CYRILLIC CAPITAL LETTER U WITH MACRON
					*p = 0x04EF;	//	Ó¯	0xd3 0xaf	CYRILLIC SMALL LETTER U WITH MACRON
					break;
				case 0x04F0:		//	Ó°	0xd3 0xb0	CYRILLIC CAPITAL LETTER U WITH DIAERESIS
					*p = 0x04F1;	//	Ó±	0xd3 0xb1	CYRILLIC SMALL LETTER U WITH DIAERESIS
					break;
				case 0x04F2:		//	Ó²	0xd3 0xb2	CYRILLIC CAPITAL LETTER U WITH DOUBLE ACUTE
					*p = 0x04F3;	//	Ó³	0xd3 0xb3	CYRILLIC SMALL LETTER U WITH DOUBLE ACUTE
					break;
				case 0x04F4:		//	Ó´	0xd3 0xb4	CYRILLIC CAPITAL LETTER CHE WITH DIAERESIS
					*p = 0x04F5;	//	Óµ	0xd3 0xb5	CYRILLIC SMALL LETTER CHE WITH DIAERESIS
					break;
				case 0x04F6:		//	Ó¶	0xd3 0xb6	CYRILLIC CAPITAL LETTER GHE WITH DESCENDER
					*p = 0x04F7;	//	Ó·	0xd3 0xb7	CYRILLIC SMALL LETTER GHE WITH DESCENDER
					break;
				case 0x04F8:		//	Ó¸	0xd3 0xb8	CYRILLIC CAPITAL LETTER YERU WITH DIAERESIS
					*p = 0x04F9;	//	Ó¹	0xd3 0xb9	CYRILLIC SMALL LETTER YERU WITH DIAERESIS
					break;
				case 0x04FA:		//	Óº	0xd3 0xba	CYRILLIC CAPITAL LETTER GHE WITH STROKE AND HOOK
					*p = 0x04FB;	//	Ó»	0xd3 0xbb	CYRILLIC SMALL LETTER GHE WITH STROKE AND HOOK
					break;
				case 0x04FC:		//	Ó¼	0xd3 0xbc	CYRILLIC CAPITAL LETTER HA WITH HOOK
					*p = 0x04FD;	//	Ó½	0xd3 0xbd	CYRILLIC SMALL LETTER HA WITH HOOK
					break;
				case 0x04FE:		//	Ó¾	0xd3 0xbe	CYRILLIC CAPITAL LETTER HA WITH STROKE
					*p = 0x04FF;	//	Ó¿	0xd3 0xbf	CYRILLIC SMALL LETTER HA WITH STROKE
					break;
				case 0x0500:		//	Ô€	0xd4 0x80	CYRILLIC CAPITAL LETTER KOMI DE
					*p = 0x0501;	//	Ô	0xd4 0x81	CYRILLIC SMALL LETTER KOMI DE
					break;
				case 0x0502:		//	Ô‚	0xd4 0x82	CYRILLIC CAPITAL LETTER KOMI DJE
					*p = 0x0503;	//	Ôƒ	0xd4 0x83	CYRILLIC SMALL LETTER KOMI DJE
					break;
				case 0x0504:		//	Ô„	0xd4 0x84	CYRILLIC CAPITAL LETTER KOMI ZJE
					*p = 0x0505;	//	Ô…	0xd4 0x85	CYRILLIC SMALL LETTER KOMI ZJE
					break;
				case 0x0506:		//	Ô†	0xd4 0x86	CYRILLIC CAPITAL LETTER KOMI DZJE
					*p = 0x0507;	//	Ô‡	0xd4 0x87	CYRILLIC SMALL LETTER KOMI DZJE
					break;
				case 0x0508:		//	Ôˆ	0xd4 0x88	CYRILLIC CAPITAL LETTER KOMI LJE
					*p = 0x0509;	//	Ô‰	0xd4 0x89	CYRILLIC SMALL LETTER KOMI LJE
					break;
				case 0x050A:		//	ÔŠ	0xd4 0x8a	CYRILLIC CAPITAL LETTER KOMI NJE
					*p = 0x050B;	//	Ô‹	0xd4 0x8b	CYRILLIC SMALL LETTER KOMI NJE
					break;
				case 0x050C:		//	ÔŒ	0xd4 0x8c	CYRILLIC CAPITAL LETTER KOMI SJE
					*p = 0x050D;	//	Ô	0xd4 0x8d	CYRILLIC SMALL LETTER KOMI SJE
					break;
				case 0x050E:		//	ÔŽ	0xd4 0x8e	CYRILLIC CAPITAL LETTER KOMI TJE
					*p = 0x050F;	//	Ô	0xd4 0x8f	CYRILLIC SMALL LETTER KOMI TJE
					break;
				case 0x0510:		//	Ô	0xd4 0x90	CYRILLIC CAPITAL LETTER REVERSED ZE
					*p = 0x0511;	//	Ô‘	0xd4 0x91	CYRILLIC SMALL LETTER REVERSED ZE
					break;
				case 0x0512:		//	Ô’	0xd4 0x92	CYRILLIC CAPITAL LETTER EL WITH HOOK
					*p = 0x0513;	//	Ô“	0xd4 0x93	CYRILLIC SMALL LETTER EL WITH HOOK
					break;
				case 0x0514:		//	Ô”	0xd4 0x94	CYRILLIC CAPITAL LETTER LHA
					*p = 0x0515;	//	Ô•	0xd4 0x95	CYRILLIC SMALL LETTER LHA
					break;
				case 0x0516:		//	Ô–	0xd4 0x96	CYRILLIC CAPITAL LETTER RHA
					*p = 0x0517;	//	Ô—	0xd4 0x97	CYRILLIC SMALL LETTER RHA
					break;
				case 0x0518:		//	Ô˜	0xd4 0x98	CYRILLIC CAPITAL LETTER YAE
					*p = 0x0519;	//	Ô™	0xd4 0x99	CYRILLIC SMALL LETTER YAE
					break;
				case 0x051A:		//	Ôš	0xd4 0x9a	CYRILLIC CAPITAL LETTER QA
					*p = 0x051B;	//	Ô›	0xd4 0x9b	CYRILLIC SMALL LETTER QA
					break;
				case 0x051C:		//	Ôœ	0xd4 0x9c	CYRILLIC CAPITAL LETTER WE
					*p = 0x051D;	//	Ô	0xd4 0x9d	CYRILLIC SMALL LETTER WE
					break;
				case 0x051E:		//	Ôž	0xd4 0x9e	CYRILLIC CAPITAL LETTER ALEUT KA
					*p = 0x051F;	//	ÔŸ	0xd4 0x9f	CYRILLIC SMALL LETTER ALEUT KA
					break;
				case 0x0520:		//	Ô 	0xd4 0xa0	CYRILLIC CAPITAL LETTER EL WITH MIDDLE HOOK
					*p = 0x0521;	//	Ô¡	0xd4 0xa1	CYRILLIC SMALL LETTER EL WITH MIDDLE HOOK
					break;
				case 0x0522:		//	Ô¢	0xd4 0xa2	CYRILLIC CAPITAL LETTER EN WITH MIDDLE HOOK
					*p = 0x0523;	//	Ô£	0xd4 0xa3	CYRILLIC SMALL LETTER EN WITH MIDDLE HOOK
					break;
				case 0x0524:		//	Ô¤	0xd4 0xa4	CYRILLIC CAPITAL LETTER PE WITH DESCENDER
					*p = 0x0525;	//	Ô¥	0xd4 0xa5	CYRILLIC SMALL LETTER PE WITH DESCENDER
					break;
				case 0x0526:		//	Ô¦	0xd4 0xa6	CYRILLIC CAPITAL LETTER SHHA WITH DESCENDER
					*p = 0x0527;	//	Ô§	0xd4 0xa7	CYRILLIC SMALL LETTER SHHA WITH DESCENDER
					break;
				case 0x0528:		//	Ô¨	0xd4 0xa8	CYRILLIC CAPITAL LETTER EN WITH LEFT HOOK
					*p = 0x0529;	//	Ô©	0xd4 0xa9	CYRILLIC SMALL LETTER EN WITH LEFT HOOK
					break;
				case 0x052A:		//	Ôª	0xd4 0xaa	CYRILLIC CAPITAL LETTER DZZHE
					*p = 0x052B;	//	Ô«	0xd4 0xab	CYRILLIC SMALL LETTER DZZHE
					break;
				case 0x052C:		//	Ô¬	0xd4 0xac	CYRILLIC CAPITAL LETTER DCHE
					*p = 0x052D;	//	Ô­	0xd4 0xad	CYRILLIC SMALL LETTER DCHE
					break;
				case 0x052E:		//	Ô®	0xd4 0xae	CYRILLIC CAPITAL LETTER EL WITH DESCENDER
					*p = 0x052F;	//	Ô¯	0xd4 0xaf	CYRILLIC SMALL LETTER EL WITH DESCENDER
					break;
				case 0x0531:		//	Ô±	0xd4 0xb1	ARMENIAN CAPITAL LETTER AYB
					*p = 0x0561;	//	Õ¡	0xd5 0xa1	ARMENIAN SMALL LETTER AYB
					break;
				case 0x0532:		//	Ô²	0xd4 0xb2	ARMENIAN CAPITAL LETTER BEN
					*p = 0x0562;	//	Õ¢	0xd5 0xa2	ARMENIAN SMALL LETTER BEN
					break;
				case 0x0533:		//	Ô³	0xd4 0xb3	ARMENIAN CAPITAL LETTER GIM
					*p = 0x0563;	//	Õ£	0xd5 0xa3	ARMENIAN SMALL LETTER GIM
					break;
				case 0x0534:		//	Ô´	0xd4 0xb4	ARMENIAN CAPITAL LETTER DA
					*p = 0x0564;	//	Õ¤	0xd5 0xa4	ARMENIAN SMALL LETTER DA
					break;
				case 0x0535:		//	Ôµ	0xd4 0xb5	ARMENIAN CAPITAL LETTER ECH
					*p = 0x0565;	//	Õ¥	0xd5 0xa5	ARMENIAN SMALL LETTER ECH
					break;
				case 0x0536:		//	Ô¶	0xd4 0xb6	ARMENIAN CAPITAL LETTER ZA
					*p = 0x0566;	//	Õ¦	0xd5 0xa6	ARMENIAN SMALL LETTER ZA
					break;
				case 0x0537:		//	Ô·	0xd4 0xb7	ARMENIAN CAPITAL LETTER EH
					*p = 0x0567;	//	Õ§	0xd5 0xa7	ARMENIAN SMALL LETTER EH
					break;
				case 0x0538:		//	Ô¸	0xd4 0xb8	ARMENIAN CAPITAL LETTER ET
					*p = 0x0568;	//	Õ¨	0xd5 0xa8	ARMENIAN SMALL LETTER ET
					break;
				case 0x0539:		//	Ô¹	0xd4 0xb9	ARMENIAN CAPITAL LETTER TO
					*p = 0x0569;	//	Õ©	0xd5 0xa9	ARMENIAN SMALL LETTER TO
					break;
				case 0x053A:		//	Ôº	0xd4 0xba	ARMENIAN CAPITAL LETTER ZHE
					*p = 0x056A;	//	Õª	0xd5 0xaa	ARMENIAN SMALL LETTER ZHE
					break;
				case 0x053B:		//	Ô»	0xd4 0xbb	ARMENIAN CAPITAL LETTER INI
					*p = 0x056B;	//	Õ«	0xd5 0xab	ARMENIAN SMALL LETTER INI
					break;
				case 0x053C:		//	Ô¼	0xd4 0xbc	ARMENIAN CAPITAL LETTER LIWN
					*p = 0x056C;	//	Õ¬	0xd5 0xac	ARMENIAN SMALL LETTER LIWN
					break;
				case 0x053D:		//	Ô½	0xd4 0xbd	ARMENIAN CAPITAL LETTER XEH
					*p = 0x056D;	//	Õ­	0xd5 0xad	ARMENIAN SMALL LETTER XEH
					break;
				case 0x053E:		//	Ô¾	0xd4 0xbe	ARMENIAN CAPITAL LETTER CA
					*p = 0x056E;	//	Õ®	0xd5 0xae	ARMENIAN SMALL LETTER CA
					break;
				case 0x053F:		//	Ô¿	0xd4 0xbf	ARMENIAN CAPITAL LETTER KEN
					*p = 0x056F;	//	Õ¯	0xd5 0xaf	ARMENIAN SMALL LETTER KEN
					break;
				case 0x0540:		//	Õ€	0xd5 0x80	ARMENIAN CAPITAL LETTER HO
					*p = 0x0570;	//	Õ°	0xd5 0xb0	ARMENIAN SMALL LETTER HO
					break;
				case 0x0541:		//	Õ	0xd5 0x81	ARMENIAN CAPITAL LETTER JA
					*p = 0x0571;	//	Õ±	0xd5 0xb1	ARMENIAN SMALL LETTER JA
					break;
				case 0x0542:		//	Õ‚	0xd5 0x82	ARMENIAN CAPITAL LETTER GHAD
					*p = 0x0572;	//	Õ²	0xd5 0xb2	ARMENIAN SMALL LETTER GHAD
					break;
				case 0x0543:		//	Õƒ	0xd5 0x83	ARMENIAN CAPITAL LETTER CHEH
					*p = 0x0573;	//	Õ³	0xd5 0xb3	ARMENIAN SMALL LETTER CHEH
					break;
				case 0x0544:		//	Õ„	0xd5 0x84	ARMENIAN CAPITAL LETTER MEN
					*p = 0x0574;	//	Õ´	0xd5 0xb4	ARMENIAN SMALL LETTER MEN
					break;
				case 0x0545:		//	Õ…	0xd5 0x85	ARMENIAN CAPITAL LETTER YI
					*p = 0x0575;	//	Õµ	0xd5 0xb5	ARMENIAN SMALL LETTER YI
					break;
				case 0x0546:		//	Õ†	0xd5 0x86	ARMENIAN CAPITAL LETTER NOW
					*p = 0x0576;	//	Õ¶	0xd5 0xb6	ARMENIAN SMALL LETTER NOW
					break;
				case 0x0547:		//	Õ‡	0xd5 0x87	ARMENIAN CAPITAL LETTER SHA
					*p = 0x0577;	//	Õ·	0xd5 0xb7	ARMENIAN SMALL LETTER SHA
					break;
				case 0x0548:		//	Õˆ	0xd5 0x88	ARMENIAN CAPITAL LETTER VO
					*p = 0x0578;	//	Õ¸	0xd5 0xb8	ARMENIAN SMALL LETTER VO
					break;
				case 0x0549:		//	Õ‰	0xd5 0x89	ARMENIAN CAPITAL LETTER CHA
					*p = 0x0579;	//	Õ¹	0xd5 0xb9	ARMENIAN SMALL LETTER CHA
					break;
				case 0x054A:		//	ÕŠ	0xd5 0x8a	ARMENIAN CAPITAL LETTER PEH
					*p = 0x057A;	//	Õº	0xd5 0xba	ARMENIAN SMALL LETTER PEH
					break;
				case 0x054B:		//	Õ‹	0xd5 0x8b	ARMENIAN CAPITAL LETTER JHEH
					*p = 0x057B;	//	Õ»	0xd5 0xbb	ARMENIAN SMALL LETTER JHEH
					break;
				case 0x054C:		//	ÕŒ	0xd5 0x8c	ARMENIAN CAPITAL LETTER RA
					*p = 0x057C;	//	Õ¼	0xd5 0xbc	ARMENIAN SMALL LETTER RA
					break;
				case 0x054D:		//	Õ	0xd5 0x8d	ARMENIAN CAPITAL LETTER SEH
					*p = 0x057D;	//	Õ½	0xd5 0xbd	ARMENIAN SMALL LETTER SEH
					break;
				case 0x054E:		//	ÕŽ	0xd5 0x8e	ARMENIAN CAPITAL LETTER VEW
					*p = 0x057E;	//	Õ¾	0xd5 0xbe	ARMENIAN SMALL LETTER VEW
					break;
				case 0x054F:		//	Õ	0xd5 0x8f	ARMENIAN CAPITAL LETTER TIWN
					*p = 0x057F;	//	Õ¿	0xd5 0xbf	ARMENIAN SMALL LETTER TIWN
					break;
				case 0x0550:		//	Õ	0xd5 0x90	ARMENIAN CAPITAL LETTER REH
					*p = 0x0580;	//	Ö€	0xd6 0x80	ARMENIAN SMALL LETTER REH
					break;
				case 0x0551:		//	Õ‘	0xd5 0x91	ARMENIAN CAPITAL LETTER CO
					*p = 0x0581;	//	Ö	0xd6 0x81	ARMENIAN SMALL LETTER CO
					break;
				case 0x0552:		//	Õ’	0xd5 0x92	ARMENIAN CAPITAL LETTER YIWN
					*p = 0x0582;	//	Ö‚	0xd6 0x82	ARMENIAN SMALL LETTER YIWN
					break;
				case 0x0553:		//	Õ“	0xd5 0x93	ARMENIAN CAPITAL LETTER PIWR
					*p = 0x0583;	//	Öƒ	0xd6 0x83	ARMENIAN SMALL LETTER PIWR
					break;
				case 0x0554:		//	Õ”	0xd5 0x94	ARMENIAN CAPITAL LETTER KEH
					*p = 0x0584;	//	Ö„	0xd6 0x84	ARMENIAN SMALL LETTER KEH
					break;
				case 0x0555:		//	Õ•	0xd5 0x95	ARMENIAN CAPITAL LETTER OH
					*p = 0x0585;	//	Ö…	0xd6 0x85	ARMENIAN SMALL LETTER OH
					break;
				case 0x0556:		//	Õ–	0xd5 0x96	ARMENIAN CAPITAL LETTER FEH
					*p = 0x0586;	//	Ö†	0xd6 0x86	ARMENIAN SMALL LETTER FEH
					break;
				case 0x10A0:		//	á‚ 	0xe1 0x82 0xa0	GEORGIAN LETTER AN
					*p = 0x10D0;	//	áƒ	0xe1 0x83 0x90	GEORGIAN SMALL LETTER AN
					break;
				case 0x10A1:		//	á‚¡	0xe1 0x82 0xa1	GEORGIAN LETTER BAN
					*p = 0x10D1;	//	áƒ‘	0xe1 0x83 0x91	GEORGIAN SMALL LETTER BAN
					break;
				case 0x10A2:		//	á‚¢	0xe1 0x82 0xa2	GEORGIAN LETTER GAN
					*p = 0x10D2;	//	áƒ’	0xe1 0x83 0x92	GEORGIAN SMALL LETTER GAN
					break;
				case 0x10A3:		//	á‚£	0xe1 0x82 0xa3	GEORGIAN LETTER DON
					*p = 0x10D3;	//	áƒ“	0xe1 0x83 0x93	GEORGIAN SMALL LETTER DON
					break;
				case 0x10A4:		//	á‚¤	0xe1 0x82 0xa4	GEORGIAN LETTER EN
					*p = 0x10D4;	//	áƒ”	0xe1 0x83 0x94	GEORGIAN SMALL LETTER EN
					break;
				case 0x10A5:		//	á‚¥	0xe1 0x82 0xa5	GEORGIAN LETTER VIN
					*p = 0x10D5;	//	áƒ•	0xe1 0x83 0x95	GEORGIAN SMALL LETTER VIN
					break;
				case 0x10A6:		//	á‚¦	0xe1 0x82 0xa6	GEORGIAN LETTER ZEN
					*p = 0x10D6;	//	áƒ–	0xe1 0x83 0x96	GEORGIAN SMALL LETTER ZEN
					break;
				case 0x10A7:		//	á‚§	0xe1 0x82 0xa7	GEORGIAN LETTER TAN
					*p = 0x10D7;	//	áƒ—	0xe1 0x83 0x97	GEORGIAN SMALL LETTER TAN
					break;
				case 0x10A8:		//	á‚¨	0xe1 0x82 0xa8	GEORGIAN LETTER IN
					*p = 0x10D8;	//	áƒ˜	0xe1 0x83 0x98	GEORGIAN SMALL LETTER IN
					break;
				case 0x10A9:		//	á‚©	0xe1 0x82 0xa9	GEORGIAN LETTER KAN
					*p = 0x10D9;	//	áƒ™	0xe1 0x83 0x99	GEORGIAN SMALL LETTER KAN
					break;
				case 0x10AA:		//	á‚ª	0xe1 0x82 0xaa	GEORGIAN LETTER LAS
					*p = 0x10DA;	//	áƒš	0xe1 0x83 0x9a	GEORGIAN SMALL LETTER LAS
					break;
				case 0x10AB:		//	á‚«	0xe1 0x82 0xab	GEORGIAN LETTER MAN
					*p = 0x10DB;	//	áƒ›	0xe1 0x83 0x9b	GEORGIAN SMALL LETTER MAN
					break;
				case 0x10AC:		//	á‚¬	0xe1 0x82 0xac	GEORGIAN LETTER NAR
					*p = 0x10DC;	//	áƒœ	0xe1 0x83 0x9c	GEORGIAN SMALL LETTER NAR
					break;
				case 0x10AD:		//	á‚­	0xe1 0x82 0xad	GEORGIAN LETTER ON
					*p = 0x10DD;	//	áƒ	0xe1 0x83 0x9d	GEORGIAN SMALL LETTER ON
					break;
				case 0x10AE:		//	á‚®	0xe1 0x82 0xae	GEORGIAN LETTER PAR
					*p = 0x10DE;	//	áƒž	0xe1 0x83 0x9e	GEORGIAN SMALL LETTER PAR
					break;
				case 0x10AF:		//	á‚¯	0xe1 0x82 0xaf	GEORGIAN LETTER ZHAR
					*p = 0x10DF;	//	áƒŸ	0xe1 0x83 0x9f	GEORGIAN SMALL LETTER ZHAR
					break;
				case 0x10B0:		//	á‚°	0xe1 0x82 0xb0	GEORGIAN LETTER RAE
					*p = 0x10E0;	//	áƒ 	0xe1 0x83 0xa0	GEORGIAN SMALL LETTER RAE
					break;
				case 0x10B1:		//	á‚±	0xe1 0x82 0xb1	GEORGIAN LETTER SAN
					*p = 0x10E1;	//	áƒ¡	0xe1 0x83 0xa1	GEORGIAN SMALL LETTER SAN
					break;
				case 0x10B2:		//	á‚²	0xe1 0x82 0xb2	GEORGIAN LETTER TAR
					*p = 0x10E2;	//	áƒ¢	0xe1 0x83 0xa2	GEORGIAN SMALL LETTER TAR
					break;
				case 0x10B3:		//	á‚³	0xe1 0x82 0xb3	GEORGIAN LETTER UN
					*p = 0x10E3;	//	áƒ£	0xe1 0x83 0xa3	GEORGIAN SMALL LETTER UN
					break;
				case 0x10B4:		//	á‚´	0xe1 0x82 0xb4	GEORGIAN LETTER PHAR
					*p = 0x10E4;	//	áƒ¤	0xe1 0x83 0xa4	GEORGIAN SMALL LETTER PHAR
					break;
				case 0x10B5:		//	á‚µ	0xe1 0x82 0xb5	GEORGIAN LETTER KHAR
					*p = 0x10E5;	//	áƒ¥	0xe1 0x83 0xa5	GEORGIAN SMALL LETTER KHAR
					break;
				case 0x10B6:		//	á‚¶	0xe1 0x82 0xb6	GEORGIAN LETTER GHAN
					*p = 0x10E6;	//	áƒ¦	0xe1 0x83 0xa6	GEORGIAN SMALL LETTER GHAN
					break;
				case 0x10B7:		//	á‚·	0xe1 0x82 0xb7	GEORGIAN LETTER QAR
					*p = 0x10E7;	//	áƒ§	0xe1 0x83 0xa7	GEORGIAN SMALL LETTER QAR
					break;
				case 0x10B8:		//	á‚¸	0xe1 0x82 0xb8	GEORGIAN LETTER SHIN
					*p = 0x10E8;	//	áƒ¨	0xe1 0x83 0xa8	GEORGIAN SMALL LETTER SHIN
					break;
				case 0x10B9:		//	á‚¹	0xe1 0x82 0xb9	GEORGIAN LETTER CHIN
					*p = 0x10E9;	//	áƒ©	0xe1 0x83 0xa9	GEORGIAN SMALL LETTER CHIN
					break;
				case 0x10BA:		//	á‚º	0xe1 0x82 0xba	GEORGIAN LETTER CAN
					*p = 0x10EA;	//	áƒª	0xe1 0x83 0xaa	GEORGIAN SMALL LETTER CAN
					break;
				case 0x10BB:		//	á‚»	0xe1 0x82 0xbb	GEORGIAN LETTER JIL
					*p = 0x10EB;	//	áƒ«	0xe1 0x83 0xab	GEORGIAN SMALL LETTER JIL
					break;
				case 0x10BC:		//	á‚¼	0xe1 0x82 0xbc	GEORGIAN LETTER CIL
					*p = 0x10EC;	//	áƒ¬	0xe1 0x83 0xac	GEORGIAN SMALL LETTER CIL
					break;
				case 0x10BD:		//	á‚½	0xe1 0x82 0xbd	GEORGIAN LETTER CHAR
					*p = 0x10ED;	//	áƒ­	0xe1 0x83 0xad	GEORGIAN SMALL LETTER CHAR
					break;
				case 0x10BE:		//	á‚¾	0xe1 0x82 0xbe	GEORGIAN LETTER XAN
					*p = 0x10EE;	//	áƒ®	0xe1 0x83 0xae	GEORGIAN SMALL LETTER XAN
					break;
				case 0x10BF:		//	á‚¿	0xe1 0x82 0xbf	GEORGIAN LETTER JHAN
					*p = 0x10EF;	//	áƒ¯	0xe1 0x83 0xaf	GEORGIAN SMALL LETTER JHAN
					break;
				case 0x10C0:		//	áƒ€	0xe1 0x83 0x80	GEORGIAN LETTER HAE
					*p = 0x10F0;	//	áƒ°	0xe1 0x83 0xb0	GEORGIAN SMALL LETTER HAE
					break;
				case 0x10C1:		//	áƒ	0xe1 0x83 0x81	GEORGIAN LETTER HE
					*p = 0x10F1;	//	áƒ±	0xe1 0x83 0xb1	GEORGIAN SMALL LETTER HE
					break;
				case 0x10C2:		//	áƒ‚	0xe1 0x83 0x82	GEORGIAN LETTER HIE
					*p = 0x10F2;	//	áƒ²	0xe1 0x83 0xb2	GEORGIAN SMALL LETTER HIE
					break;
				case 0x10C3:		//	áƒƒ	0xe1 0x83 0x83	GEORGIAN LETTER WE
					*p = 0x10F3;	//	áƒ³	0xe1 0x83 0xb3	GEORGIAN SMALL LETTER WE
					break;
				case 0x10C4:		//	áƒ„	0xe1 0x83 0x84	GEORGIAN LETTER HAR
					*p = 0x10F4;	//	áƒ´	0xe1 0x83 0xb4	GEORGIAN SMALL LETTER HAR
					break;
				case 0x10C5:		//	áƒ…	0xe1 0x83 0x85	GEORGIAN LETTER HOE
					*p = 0x10F5;	//	áƒµ	0xe1 0x83 0xb5	GEORGIAN SMALL LETTER HOE
					break;
				case 0x10C7:		//	áƒ‡	0xe1 0x83 0x87	GEORGIAN LETTER YN
					*p = 0x10F7;	//	áƒ·	0xe1 0x83 0xb7	GEORGIAN SMALL LETTER YN
					break;
				case 0x10CD:		//	áƒ	0xe1 0x83 0x8d	GEORGIAN LETTER AEN
					*p = 0x10FD;	//	áƒ½	0xe1 0x83 0xbd	GEORGIAN SMALL LETTER AEN
					break;
				case 0x13A0:		//	áŽ 	0xe1 0x8e 0xa0	CHEROKEE CAPITAL LETTER A
					*p = 0xAB70;	//	ê­°	0xea 0xad 0xb0	CHEROKEE SMALL LETTER A
					break;
				case 0x13A1:		//	áŽ¡	0xe1 0x8e 0xa1	CHEROKEE CAPITAL LETTER E
					*p = 0xAB71;	//	ê­±	0xea 0xad 0xb1	CHEROKEE SMALL LETTER E
					break;
				case 0x13A2:		//	áŽ¢	0xe1 0x8e 0xa2	CHEROKEE CAPITAL LETTER I
					*p = 0xAB72;	//	ê­²	0xea 0xad 0xb2	CHEROKEE SMALL LETTER I
					break;
				case 0x13A3:		//	áŽ£	0xe1 0x8e 0xa3	CHEROKEE CAPITAL LETTER O
					*p = 0xAB73;	//	ê­³	0xea 0xad 0xb3	CHEROKEE SMALL LETTER O
					break;
				case 0x13A4:		//	áŽ¤	0xe1 0x8e 0xa4	CHEROKEE CAPITAL LETTER U
					*p = 0xAB74;	//	ê­´	0xea 0xad 0xb4	CHEROKEE SMALL LETTER U
					break;
				case 0x13A5:		//	áŽ¥	0xe1 0x8e 0xa5	CHEROKEE CAPITAL LETTER V
					*p = 0xAB75;	//	ê­µ	0xea 0xad 0xb5	CHEROKEE SMALL LETTER V
					break;
				case 0x13A6:		//	áŽ¦	0xe1 0x8e 0xa6	CHEROKEE CAPITAL LETTER GA
					*p = 0xAB76;	//	ê­¶	0xea 0xad 0xb6	CHEROKEE SMALL LETTER GA
					break;
				case 0x13A7:		//	áŽ§	0xe1 0x8e 0xa7	CHEROKEE CAPITAL LETTER KA
					*p = 0xAB77;	//	ê­·	0xea 0xad 0xb7	CHEROKEE SMALL LETTER KA
					break;
				case 0x13A8:		//	áŽ¨	0xe1 0x8e 0xa8	CHEROKEE CAPITAL LETTER GE
					*p = 0xAB78;	//	ê­¸	0xea 0xad 0xb8	CHEROKEE SMALL LETTER GE
					break;
				case 0x13A9:		//	áŽ©	0xe1 0x8e 0xa9	CHEROKEE CAPITAL LETTER GI
					*p = 0xAB79;	//	ê­¹	0xea 0xad 0xb9	CHEROKEE SMALL LETTER GI
					break;
				case 0x13AA:		//	áŽª	0xe1 0x8e 0xaa	CHEROKEE CAPITAL LETTER GO
					*p = 0xAB7A;	//	ê­º	0xea 0xad 0xba	CHEROKEE SMALL LETTER GO
					break;
				case 0x13AB:		//	áŽ«	0xe1 0x8e 0xab	CHEROKEE CAPITAL LETTER GU
					*p = 0xAB7B;	//	ê­»	0xea 0xad 0xbb	CHEROKEE SMALL LETTER GU
					break;
				case 0x13AC:		//	áŽ¬	0xe1 0x8e 0xac	CHEROKEE CAPITAL LETTER GV
					*p = 0xAB7C;	//	ê­¼	0xea 0xad 0xbc	CHEROKEE SMALL LETTER GV
					break;
				case 0x13AD:		//	áŽ­	0xe1 0x8e 0xad	CHEROKEE CAPITAL LETTER HA
					*p = 0xAB7D;	//	ê­½	0xea 0xad 0xbd	CHEROKEE SMALL LETTER HA
					break;
				case 0x13AE:		//	áŽ®	0xe1 0x8e 0xae	CHEROKEE CAPITAL LETTER HE
					*p = 0xAB7E;	//	ê­¾	0xea 0xad 0xbe	CHEROKEE SMALL LETTER HE
					break;
				case 0x13AF:		//	áŽ¯	0xe1 0x8e 0xaf	CHEROKEE CAPITAL LETTER HI
					*p = 0xAB7F;	//	ê­¿	0xea 0xad 0xbf	CHEROKEE SMALL LETTER HI
					break;
				case 0x13B0:		//	áŽ°	0xe1 0x8e 0xb0	CHEROKEE CAPITAL LETTER HO
					*p = 0xAB80;	//	ê®€	0xea 0xae 0x80	CHEROKEE SMALL LETTER HO
					break;
				case 0x13B1:		//	áŽ±	0xe1 0x8e 0xb1	CHEROKEE CAPITAL LETTER HU
					*p = 0xAB81;	//	ê®	0xea 0xae 0x81	CHEROKEE SMALL LETTER HU
					break;
				case 0x13B2:		//	áŽ²	0xe1 0x8e 0xb2	CHEROKEE CAPITAL LETTER HV
					*p = 0xAB82;	//	ê®‚	0xea 0xae 0x82	CHEROKEE SMALL LETTER HV
					break;
				case 0x13B3:		//	áŽ³	0xe1 0x8e 0xb3	CHEROKEE CAPITAL LETTER LA
					*p = 0xAB83;	//	ê®ƒ	0xea 0xae 0x83	CHEROKEE SMALL LETTER LA
					break;
				case 0x13B4:		//	áŽ´	0xe1 0x8e 0xb4	CHEROKEE CAPITAL LETTER LE
					*p = 0xAB84;	//	ê®„	0xea 0xae 0x84	CHEROKEE SMALL LETTER LE
					break;
				case 0x13B5:		//	áŽµ	0xe1 0x8e 0xb5	CHEROKEE CAPITAL LETTER LI
					*p = 0xAB85;	//	ê®…	0xea 0xae 0x85	CHEROKEE SMALL LETTER LI
					break;
				case 0x13B6:		//	áŽ¶	0xe1 0x8e 0xb6	CHEROKEE CAPITAL LETTER LO
					*p = 0xAB86;	//	ê®†	0xea 0xae 0x86	CHEROKEE SMALL LETTER LO
					break;
				case 0x13B7:		//	áŽ·	0xe1 0x8e 0xb7	CHEROKEE CAPITAL LETTER LU
					*p = 0xAB87;	//	ê®‡	0xea 0xae 0x87	CHEROKEE SMALL LETTER LU
					break;
				case 0x13B8:		//	áŽ¸	0xe1 0x8e 0xb8	CHEROKEE CAPITAL LETTER LV
					*p = 0xAB88;	//	ê®ˆ	0xea 0xae 0x88	CHEROKEE SMALL LETTER LV
					break;
				case 0x13B9:		//	áŽ¹	0xe1 0x8e 0xb9	CHEROKEE CAPITAL LETTER MA
					*p = 0xAB89;	//	ê®‰	0xea 0xae 0x89	CHEROKEE SMALL LETTER MA
					break;
				case 0x13BA:		//	áŽº	0xe1 0x8e 0xba	CHEROKEE CAPITAL LETTER ME
					*p = 0xAB8A;	//	ê®Š	0xea 0xae 0x8a	CHEROKEE SMALL LETTER ME
					break;
				case 0x13BB:		//	áŽ»	0xe1 0x8e 0xbb	CHEROKEE CAPITAL LETTER MI
					*p = 0xAB8B;	//	ê®‹	0xea 0xae 0x8b	CHEROKEE SMALL LETTER MI
					break;
				case 0x13BC:		//	áŽ¼	0xe1 0x8e 0xbc	CHEROKEE CAPITAL LETTER MO
					*p = 0xAB8C;	//	ê®Œ	0xea 0xae 0x8c	CHEROKEE SMALL LETTER MO
					break;
				case 0x13BD:		//	áŽ½	0xe1 0x8e 0xbd	CHEROKEE CAPITAL LETTER MU
					*p = 0xAB8D;	//	ê®	0xea 0xae 0x8d	CHEROKEE SMALL LETTER MU
					break;
				case 0x13BE:		//	áŽ¾	0xe1 0x8e 0xbe	CHEROKEE CAPITAL LETTER NA
					*p = 0xAB8E;	//	ê®Ž	0xea 0xae 0x8e	CHEROKEE SMALL LETTER NA
					break;
				case 0x13BF:		//	áŽ¿	0xe1 0x8e 0xbf	CHEROKEE CAPITAL LETTER HNA
					*p = 0xAB8F;	//	ê®	0xea 0xae 0x8f	CHEROKEE SMALL LETTER HNA
					break;
				case 0x13C0:		//	á€	0xe1 0x8f 0x80	CHEROKEE CAPITAL LETTER NAH
					*p = 0xAB90;	//	ê®	0xea 0xae 0x90	CHEROKEE SMALL LETTER NAH
					break;
				case 0x13C1:		//	á	0xe1 0x8f 0x81	CHEROKEE CAPITAL LETTER NE
					*p = 0xAB91;	//	ê®‘	0xea 0xae 0x91	CHEROKEE SMALL LETTER NE
					break;
				case 0x13C2:		//	á‚	0xe1 0x8f 0x82	CHEROKEE CAPITAL LETTER NI
					*p = 0xAB92;	//	ê®’	0xea 0xae 0x92	CHEROKEE SMALL LETTER NI
					break;
				case 0x13C3:		//	áƒ	0xe1 0x8f 0x83	CHEROKEE CAPITAL LETTER NO
					*p = 0xAB93;	//	ê®“	0xea 0xae 0x93	CHEROKEE SMALL LETTER NO
					break;
				case 0x13C4:		//	á„	0xe1 0x8f 0x84	CHEROKEE CAPITAL LETTER NU
					*p = 0xAB94;	//	ê®”	0xea 0xae 0x94	CHEROKEE SMALL LETTER NU
					break;
				case 0x13C5:		//	á…	0xe1 0x8f 0x85	CHEROKEE CAPITAL LETTER NV
					*p = 0xAB95;	//	ê®•	0xea 0xae 0x95	CHEROKEE SMALL LETTER NV
					break;
				case 0x13C6:		//	á†	0xe1 0x8f 0x86	CHEROKEE CAPITAL LETTER QUA
					*p = 0xAB96;	//	ê®–	0xea 0xae 0x96	CHEROKEE SMALL LETTER QUA
					break;
				case 0x13C7:		//	á‡	0xe1 0x8f 0x87	CHEROKEE CAPITAL LETTER QUE
					*p = 0xAB97;	//	ê®—	0xea 0xae 0x97	CHEROKEE SMALL LETTER QUE
					break;
				case 0x13C8:		//	áˆ	0xe1 0x8f 0x88	CHEROKEE CAPITAL LETTER QUI
					*p = 0xAB98;	//	ê®˜	0xea 0xae 0x98	CHEROKEE SMALL LETTER QUI
					break;
				case 0x13C9:		//	á‰	0xe1 0x8f 0x89	CHEROKEE CAPITAL LETTER QUO
					*p = 0xAB99;	//	ê®™	0xea 0xae 0x99	CHEROKEE SMALL LETTER QUO
					break;
				case 0x13CA:		//	áŠ	0xe1 0x8f 0x8a	CHEROKEE CAPITAL LETTER QUU
					*p = 0xAB9A;	//	ê®š	0xea 0xae 0x9a	CHEROKEE SMALL LETTER QUU
					break;
				case 0x13CB:		//	á‹	0xe1 0x8f 0x8b	CHEROKEE CAPITAL LETTER QUV
					*p = 0xAB9B;	//	ê®›	0xea 0xae 0x9b	CHEROKEE SMALL LETTER QUV
					break;
				case 0x13CC:		//	áŒ	0xe1 0x8f 0x8c	CHEROKEE CAPITAL LETTER SA
					*p = 0xAB9C;	//	ê®œ	0xea 0xae 0x9c	CHEROKEE SMALL LETTER SA
					break;
				case 0x13CD:		//	á	0xe1 0x8f 0x8d	CHEROKEE CAPITAL LETTER S
					*p = 0xAB9D;	//	ê®	0xea 0xae 0x9d	CHEROKEE SMALL LETTER S
					break;
				case 0x13CE:		//	áŽ	0xe1 0x8f 0x8e	CHEROKEE CAPITAL LETTER SE
					*p = 0xAB9E;	//	ê®ž	0xea 0xae 0x9e	CHEROKEE SMALL LETTER SE
					break;
				case 0x13CF:		//	á	0xe1 0x8f 0x8f	CHEROKEE CAPITAL LETTER SI
					*p = 0xAB9F;	//	ê®Ÿ	0xea 0xae 0x9f	CHEROKEE SMALL LETTER SI
					break;
				case 0x13D0:		//	á	0xe1 0x8f 0x90	CHEROKEE CAPITAL LETTER SO
					*p = 0xABA0;	//	ê® 	0xea 0xae 0xa0	CHEROKEE SMALL LETTER SO
					break;
				case 0x13D1:		//	á‘	0xe1 0x8f 0x91	CHEROKEE CAPITAL LETTER SU
					*p = 0xABA1;	//	ê®¡	0xea 0xae 0xa1	CHEROKEE SMALL LETTER SU
					break;
				case 0x13D2:		//	á’	0xe1 0x8f 0x92	CHEROKEE CAPITAL LETTER SV
					*p = 0xABA2;	//	ê®¢	0xea 0xae 0xa2	CHEROKEE SMALL LETTER SV
					break;
				case 0x13D3:		//	á“	0xe1 0x8f 0x93	CHEROKEE CAPITAL LETTER DA
					*p = 0xABA3;	//	ê®£	0xea 0xae 0xa3	CHEROKEE SMALL LETTER DA
					break;
				case 0x13D4:		//	á”	0xe1 0x8f 0x94	CHEROKEE CAPITAL LETTER TA
					*p = 0xABA4;	//	ê®¤	0xea 0xae 0xa4	CHEROKEE SMALL LETTER TA
					break;
				case 0x13D5:		//	á•	0xe1 0x8f 0x95	CHEROKEE CAPITAL LETTER DE
					*p = 0xABA5;	//	ê®¥	0xea 0xae 0xa5	CHEROKEE SMALL LETTER DE
					break;
				case 0x13D6:		//	á–	0xe1 0x8f 0x96	CHEROKEE CAPITAL LETTER TE
					*p = 0xABA6;	//	ê®¦	0xea 0xae 0xa6	CHEROKEE SMALL LETTER TE
					break;
				case 0x13D7:		//	á—	0xe1 0x8f 0x97	CHEROKEE CAPITAL LETTER DI
					*p = 0xABA7;	//	ê®§	0xea 0xae 0xa7	CHEROKEE SMALL LETTER DI
					break;
				case 0x13D8:		//	á˜	0xe1 0x8f 0x98	CHEROKEE CAPITAL LETTER TI
					*p = 0xABA8;	//	ê®¨	0xea 0xae 0xa8	CHEROKEE SMALL LETTER TI
					break;
				case 0x13D9:		//	á™	0xe1 0x8f 0x99	CHEROKEE CAPITAL LETTER DO
					*p = 0xABA9;	//	ê®©	0xea 0xae 0xa9	CHEROKEE SMALL LETTER DO
					break;
				case 0x13DA:		//	áš	0xe1 0x8f 0x9a	CHEROKEE CAPITAL LETTER DU
					*p = 0xABAA;	//	ê®ª	0xea 0xae 0xaa	CHEROKEE SMALL LETTER DU
					break;
				case 0x13DB:		//	á›	0xe1 0x8f 0x9b	CHEROKEE CAPITAL LETTER DV
					*p = 0xABAB;	//	ê®«	0xea 0xae 0xab	CHEROKEE SMALL LETTER DV
					break;
				case 0x13DC:		//	áœ	0xe1 0x8f 0x9c	CHEROKEE CAPITAL LETTER DLA
					*p = 0xABAC;	//	ê®¬	0xea 0xae 0xac	CHEROKEE SMALL LETTER DLA
					break;
				case 0x13DD:		//	á	0xe1 0x8f 0x9d	CHEROKEE CAPITAL LETTER TLA
					*p = 0xABAD;	//	ê®­	0xea 0xae 0xad	CHEROKEE SMALL LETTER TLA
					break;
				case 0x13DE:		//	áž	0xe1 0x8f 0x9e	CHEROKEE CAPITAL LETTER TLE
					*p = 0xABAE;	//	ê®®	0xea 0xae 0xae	CHEROKEE SMALL LETTER TLE
					break;
				case 0x13DF:		//	áŸ	0xe1 0x8f 0x9f	CHEROKEE CAPITAL LETTER TLI
					*p = 0xABAF;	//	ê®¯	0xea 0xae 0xaf	CHEROKEE SMALL LETTER TLI
					break;
				case 0x13E0:		//	á 	0xe1 0x8f 0xa0	CHEROKEE CAPITAL LETTER TLO
					*p = 0xABB0;	//	ê®°	0xea 0xae 0xb0	CHEROKEE SMALL LETTER TLO
					break;
				case 0x13E1:		//	á¡	0xe1 0x8f 0xa1	CHEROKEE CAPITAL LETTER TLU
					*p = 0xABB1;	//	ê®±	0xea 0xae 0xb1	CHEROKEE SMALL LETTER TLU
					break;
				case 0x13E2:		//	á¢	0xe1 0x8f 0xa2	CHEROKEE CAPITAL LETTER TLV
					*p = 0xABB2;	//	ê®²	0xea 0xae 0xb2	CHEROKEE SMALL LETTER TLV
					break;
				case 0x13E3:		//	á£	0xe1 0x8f 0xa3	CHEROKEE CAPITAL LETTER TSA
					*p = 0xABB3;	//	ê®³	0xea 0xae 0xb3	CHEROKEE SMALL LETTER TSA
					break;
				case 0x13E4:		//	á¤	0xe1 0x8f 0xa4	CHEROKEE CAPITAL LETTER TSE
					*p = 0xABB4;	//	ê®´	0xea 0xae 0xb4	CHEROKEE SMALL LETTER TSE
					break;
				case 0x13E5:		//	á¥	0xe1 0x8f 0xa5	CHEROKEE CAPITAL LETTER TSI
					*p = 0xABB5;	//	ê®µ	0xea 0xae 0xb5	CHEROKEE SMALL LETTER TSI
					break;
				case 0x13E6:		//	á¦	0xe1 0x8f 0xa6	CHEROKEE CAPITAL LETTER TSO
					*p = 0xABB6;	//	ê®¶	0xea 0xae 0xb6	CHEROKEE SMALL LETTER TSO
					break;
				case 0x13E7:		//	á§	0xe1 0x8f 0xa7	CHEROKEE CAPITAL LETTER TSU
					*p = 0xABB7;	//	ê®·	0xea 0xae 0xb7	CHEROKEE SMALL LETTER TSU
					break;
				case 0x13E8:		//	á¨	0xe1 0x8f 0xa8	CHEROKEE CAPITAL LETTER TSV
					*p = 0xABB8;	//	ê®¸	0xea 0xae 0xb8	CHEROKEE SMALL LETTER TSV
					break;
				case 0x13E9:		//	á©	0xe1 0x8f 0xa9	CHEROKEE CAPITAL LETTER WA
					*p = 0xABB9;	//	ê®¹	0xea 0xae 0xb9	CHEROKEE SMALL LETTER WA
					break;
				case 0x13EA:		//	áª	0xe1 0x8f 0xaa	CHEROKEE CAPITAL LETTER WE
					*p = 0xABBA;	//	ê®º	0xea 0xae 0xba	CHEROKEE SMALL LETTER WE
					break;
				case 0x13EB:		//	á«	0xe1 0x8f 0xab	CHEROKEE CAPITAL LETTER WI
					*p = 0xABBB;	//	ê®»	0xea 0xae 0xbb	CHEROKEE SMALL LETTER WI
					break;
				case 0x13EC:		//	á¬	0xe1 0x8f 0xac	CHEROKEE CAPITAL LETTER WO
					*p = 0xABBC;	//	ê®¼	0xea 0xae 0xbc	CHEROKEE SMALL LETTER WO
					break;
				case 0x13ED:		//	á­	0xe1 0x8f 0xad	CHEROKEE CAPITAL LETTER WU
					*p = 0xABBD;	//	ê®½	0xea 0xae 0xbd	CHEROKEE SMALL LETTER WU
					break;
				case 0x13EE:		//	á®	0xe1 0x8f 0xae	CHEROKEE CAPITAL LETTER WV
					*p = 0xABBE;	//	ê®¾	0xea 0xae 0xbe	CHEROKEE SMALL LETTER WV
					break;
				case 0x13EF:		//	á¯	0xe1 0x8f 0xaf	CHEROKEE CAPITAL LETTER YA
					*p = 0xABBF;	//	ê®¿	0xea 0xae 0xbf	CHEROKEE SMALL LETTER YA
					break;
				case 0x13F0:		//	á°	0xe1 0x8f 0xb0	CHEROKEE CAPITAL LETTER YE
					*p = 0x13F8;	//	á¸	0xe1 0x8f 0xb8	CHEROKEE SMALL LETTER YE
					break;
				case 0x13F1:		//	á±	0xe1 0x8f 0xb1	CHEROKEE CAPITAL LETTER YI
					*p = 0x13F9;	//	á¹	0xe1 0x8f 0xb9	CHEROKEE SMALL LETTER YI
					break;
				case 0x13F2:		//	á²	0xe1 0x8f 0xb2	CHEROKEE CAPITAL LETTER YO
					*p = 0x13FA;	//	áº	0xe1 0x8f 0xba	CHEROKEE SMALL LETTER YO
					break;
				case 0x13F3:		//	á³	0xe1 0x8f 0xb3	CHEROKEE CAPITAL LETTER YU
					*p = 0x13FB;	//	á»	0xe1 0x8f 0xbb	CHEROKEE SMALL LETTER YU
					break;
				case 0x13F4:		//	á´	0xe1 0x8f 0xb4	CHEROKEE CAPITAL LETTER YV
					*p = 0x13FC;	//	á¼	0xe1 0x8f 0xbc	CHEROKEE SMALL LETTER YV
					break;
				case 0x13F5:		//	áµ	0xe1 0x8f 0xb5	CHEROKEE CAPITAL LETTER MV
					*p = 0x13FD;	//	á½	0xe1 0x8f 0xbd	CHEROKEE SMALL LETTER MV
					break;
				case 0x1C90:		//	á²	0xe1 0xb2 0x90	GEORGIAN CAPITAL LETTER AN
					*p = 0x10D0;	//	áƒ	0xe1 0x83 0x90	GEORGIAN SMALL LETTER AN
					break;
				case 0x1C91:		//	á²‘	0xe1 0xb2 0x91	GEORGIAN CAPITAL LETTER BAN
					*p = 0x10D1;	//	áƒ‘	0xe1 0x83 0x91	GEORGIAN SMALL LETTER BAN
					break;
				case 0x1C92:		//	á²’	0xe1 0xb2 0x92	GEORGIAN CAPITAL LETTER GAN
					*p = 0x10D2;	//	áƒ’	0xe1 0x83 0x92	GEORGIAN SMALL LETTER GAN
					break;
				case 0x1C93:		//	á²“	0xe1 0xb2 0x93	GEORGIAN CAPITAL LETTER DON
					*p = 0x10D3;	//	áƒ“	0xe1 0x83 0x93	GEORGIAN SMALL LETTER DON
					break;
				case 0x1C94:		//	á²”	0xe1 0xb2 0x94	GEORGIAN CAPITAL LETTER EN
					*p = 0x10D4;	//	áƒ”	0xe1 0x83 0x94	GEORGIAN SMALL LETTER EN
					break;
				case 0x1C95:		//	á²•	0xe1 0xb2 0x95	GEORGIAN CAPITAL LETTER VIN
					*p = 0x10D5;	//	áƒ•	0xe1 0x83 0x95	GEORGIAN SMALL LETTER VIN
					break;
				case 0x1C96:		//	á²–	0xe1 0xb2 0x96	GEORGIAN CAPITAL LETTER ZEN
					*p = 0x10D6;	//	áƒ–	0xe1 0x83 0x96	GEORGIAN SMALL LETTER ZEN
					break;
				case 0x1C97:		//	á²—	0xe1 0xb2 0x97	GEORGIAN CAPITAL LETTER TAN
					*p = 0x10D7;	//	áƒ—	0xe1 0x83 0x97	GEORGIAN SMALL LETTER TAN
					break;
				case 0x1C98:		//	á²˜	0xe1 0xb2 0x98	GEORGIAN CAPITAL LETTER IN
					*p = 0x10D8;	//	áƒ˜	0xe1 0x83 0x98	GEORGIAN SMALL LETTER IN
					break;
				case 0x1C99:		//	á²™	0xe1 0xb2 0x99	GEORGIAN CAPITAL LETTER KAN
					*p = 0x10D9;	//	áƒ™	0xe1 0x83 0x99	GEORGIAN SMALL LETTER KAN
					break;
				case 0x1C9A:		//	á²š	0xe1 0xb2 0x9a	GEORGIAN CAPITAL LETTER LAS
					*p = 0x10DA;	//	áƒš	0xe1 0x83 0x9a	GEORGIAN SMALL LETTER LAS
					break;
				case 0x1C9B:		//	á²›	0xe1 0xb2 0x9b	GEORGIAN CAPITAL LETTER MAN
					*p = 0x10DB;	//	áƒ›	0xe1 0x83 0x9b	GEORGIAN SMALL LETTER MAN
					break;
				case 0x1C9C:		//	á²œ	0xe1 0xb2 0x9c	GEORGIAN CAPITAL LETTER NAR
					*p = 0x10DC;	//	áƒœ	0xe1 0x83 0x9c	GEORGIAN SMALL LETTER NAR
					break;
				case 0x1C9D:		//	á²	0xe1 0xb2 0x9d	GEORGIAN CAPITAL LETTER ON
					*p = 0x10DD;	//	áƒ	0xe1 0x83 0x9d	GEORGIAN SMALL LETTER ON
					break;
				case 0x1C9E:		//	á²ž	0xe1 0xb2 0x9e	GEORGIAN CAPITAL LETTER PAR
					*p = 0x10DE;	//	áƒž	0xe1 0x83 0x9e	GEORGIAN SMALL LETTER PAR
					break;
				case 0x1C9F:		//	á²Ÿ	0xe1 0xb2 0x9f	GEORGIAN CAPITAL LETTER ZHAR
					*p = 0x10DF;	//	áƒŸ	0xe1 0x83 0x9f	GEORGIAN SMALL LETTER ZHAR
					break;
				case 0x1CA0:		//	á² 	0xe1 0xb2 0xa0	GEORGIAN CAPITAL LETTER RAE
					*p = 0x10E0;	//	áƒ 	0xe1 0x83 0xa0	GEORGIAN SMALL LETTER RAE
					break;
				case 0x1CA1:		//	á²¡	0xe1 0xb2 0xa1	GEORGIAN CAPITAL LETTER SAN
					*p = 0x10E1;	//	áƒ¡	0xe1 0x83 0xa1	GEORGIAN SMALL LETTER SAN
					break;
				case 0x1CA2:		//	á²¢	0xe1 0xb2 0xa2	GEORGIAN CAPITAL LETTER TAR
					*p = 0x10E2;	//	áƒ¢	0xe1 0x83 0xa2	GEORGIAN SMALL LETTER TAR
					break;
				case 0x1CA3:		//	á²£	0xe1 0xb2 0xa3	GEORGIAN CAPITAL LETTER UN
					*p = 0x10E3;	//	áƒ£	0xe1 0x83 0xa3	GEORGIAN SMALL LETTER UN
					break;
				case 0x1CA4:		//	á²¤	0xe1 0xb2 0xa4	GEORGIAN CAPITAL LETTER PHAR
					*p = 0x10E4;	//	áƒ¤	0xe1 0x83 0xa4	GEORGIAN SMALL LETTER PHAR
					break;
				case 0x1CA5:		//	á²¥	0xe1 0xb2 0xa5	GEORGIAN CAPITAL LETTER KHAR
					*p = 0x10E5;	//	áƒ¥	0xe1 0x83 0xa5	GEORGIAN SMALL LETTER KHAR
					break;
				case 0x1CA6:		//	á²¦	0xe1 0xb2 0xa6	GEORGIAN CAPITAL LETTER GHAN
					*p = 0x10E6;	//	áƒ¦	0xe1 0x83 0xa6	GEORGIAN SMALL LETTER GHAN
					break;
				case 0x1CA7:		//	á²§	0xe1 0xb2 0xa7	GEORGIAN CAPITAL LETTER QAR
					*p = 0x10E7;	//	áƒ§	0xe1 0x83 0xa7	GEORGIAN SMALL LETTER QAR
					break;
				case 0x1CA8:		//	á²¨	0xe1 0xb2 0xa8	GEORGIAN CAPITAL LETTER SHIN
					*p = 0x10E8;	//	áƒ¨	0xe1 0x83 0xa8	GEORGIAN SMALL LETTER SHIN
					break;
				case 0x1CA9:		//	á²©	0xe1 0xb2 0xa9	GEORGIAN CAPITAL LETTER CHIN
					*p = 0x10E9;	//	áƒ©	0xe1 0x83 0xa9	GEORGIAN SMALL LETTER CHIN
					break;
				case 0x1CAA:		//	á²ª	0xe1 0xb2 0xaa	GEORGIAN CAPITAL LETTER CAN
					*p = 0x10EA;	//	áƒª	0xe1 0x83 0xaa	GEORGIAN SMALL LETTER CAN
					break;
				case 0x1CAB:		//	á²«	0xe1 0xb2 0xab	GEORGIAN CAPITAL LETTER JIL
					*p = 0x10EB;	//	áƒ«	0xe1 0x83 0xab	GEORGIAN SMALL LETTER JIL
					break;
				case 0x1CAC:		//	á²¬	0xe1 0xb2 0xac	GEORGIAN CAPITAL LETTER CIL
					*p = 0x10EC;	//	áƒ¬	0xe1 0x83 0xac	GEORGIAN SMALL LETTER CIL
					break;
				case 0x1CAD:		//	á²­	0xe1 0xb2 0xad	GEORGIAN CAPITAL LETTER CHAR
					*p = 0x10ED;	//	áƒ­	0xe1 0x83 0xad	GEORGIAN SMALL LETTER CHAR
					break;
				case 0x1CAE:		//	á²®	0xe1 0xb2 0xae	GEORGIAN CAPITAL LETTER XAN
					*p = 0x10EE;	//	áƒ®	0xe1 0x83 0xae	GEORGIAN SMALL LETTER XAN
					break;
				case 0x1CAF:		//	á²¯	0xe1 0xb2 0xaf	GEORGIAN CAPITAL LETTER JHAN
					*p = 0x10EF;	//	áƒ¯	0xe1 0x83 0xaf	GEORGIAN SMALL LETTER JHAN
					break;
				case 0x1CB0:		//	á²°	0xe1 0xb2 0xb0	GEORGIAN CAPITAL LETTER HAE
					*p = 0x10F0;	//	áƒ°	0xe1 0x83 0xb0	GEORGIAN SMALL LETTER HAE
					break;
				case 0x1CB1:		//	á²±	0xe1 0xb2 0xb1	GEORGIAN CAPITAL LETTER HE
					*p = 0x10F1;	//	áƒ±	0xe1 0x83 0xb1	GEORGIAN SMALL LETTER HE
					break;
				case 0x1CB2:		//	á²²	0xe1 0xb2 0xb2	GEORGIAN CAPITAL LETTER HIE
					*p = 0x10F2;	//	áƒ²	0xe1 0x83 0xb2	GEORGIAN SMALL LETTER HIE
					break;
				case 0x1CB3:		//	á²³	0xe1 0xb2 0xb3	GEORGIAN CAPITAL LETTER WE
					*p = 0x10F3;	//	áƒ³	0xe1 0x83 0xb3	GEORGIAN SMALL LETTER WE
					break;
				case 0x1CB4:		//	á²´	0xe1 0xb2 0xb4	GEORGIAN CAPITAL LETTER HAR
					*p = 0x10F4;	//	áƒ´	0xe1 0x83 0xb4	GEORGIAN SMALL LETTER HAR
					break;
				case 0x1CB5:		//	á²µ	0xe1 0xb2 0xb5	GEORGIAN CAPITAL LETTER HOE
					*p = 0x10F5;	//	áƒµ	0xe1 0x83 0xb5	GEORGIAN SMALL LETTER HOE
					break;
				case 0x1CB6:		//	á²¶	0xe1 0xb2 0xb6	GEORGIAN CAPITAL LETTER FI
					*p = 0x10F6;	//	áƒ¶	0xe1 0x83 0xb6	GEORGIAN SMALL LETTER FI
					break;
				case 0x1CB7:		//	á²·	0xe1 0xb2 0xb7	GEORGIAN CAPITAL LETTER YN
					*p = 0x10F7;	//	áƒ·	0xe1 0x83 0xb7	GEORGIAN SMALL LETTER YN
					break;
				case 0x1CB8:		//	á²¸	0xe1 0xb2 0xb8	GEORGIAN CAPITAL LETTER ELIFI
					*p = 0x10F8;	//	áƒ¸	0xe1 0x83 0xb8	GEORGIAN SMALL LETTER ELIFI
					break;
				case 0x1CB9:		//	á²¹	0xe1 0xb2 0xb9	GEORGIAN CAPITAL LETTER TURNED GAN
					*p = 0x10F9;	//	áƒ¹	0xe1 0x83 0xb9	GEORGIAN SMALL LETTER TURNED GAN
					break;
				case 0x1CBA:		//	á²º	0xe1 0xb2 0xba	GEORGIAN CAPITAL LETTER AIN
					*p = 0x10FA;	//	áƒº	0xe1 0x83 0xba	GEORGIAN SMALL LETTER AIN
					break;
				case 0x1CBD:		//	á²½	0xe1 0xb2 0xbd	GEORGIAN CAPITAL LETTER AEN
					*p = 0x10FD;	//	áƒ½	0xe1 0x83 0xbd	GEORGIAN SMALL LETTER AEN
					break;
				case 0x1CBE:		//	á²¾	0xe1 0xb2 0xbe	GEORGIAN CAPITAL LETTER HARD SIGN
					*p = 0x10FE;	//	áƒ¾	0xe1 0x83 0xbe	GEORGIAN SMALL LETTER HARD SIGN
					break;
				case 0x1CBF:		//	á²¿	0xe1 0xb2 0xbf	GEORGIAN CAPITAL LETTER LABIAL SIGN
					*p = 0x10FF;	//	áƒ¿	0xe1 0x83 0xbf	GEORGIAN SMALL LETTER LABIAL SIGN
					break;
				case 0x1E00:		//	á¸€	0xe1 0xb8 0x80	LATIN CAPITAL LETTER A WITH RING BELOW
					*p = 0x1E01;	//	á¸	0xe1 0xb8 0x81	LATIN SMALL LETTER A WITH RING BELOW
					break;
				case 0x1E02:		//	á¸‚	0xe1 0xb8 0x82	LATIN CAPITAL LETTER B WITH DOT ABOVE
					*p = 0x1E03;	//	á¸ƒ	0xe1 0xb8 0x83	LATIN SMALL LETTER B WITH DOT ABOVE
					break;
				case 0x1E04:		//	á¸„	0xe1 0xb8 0x84	LATIN CAPITAL LETTER B WITH DOT BELOW
					*p = 0x1E05;	//	á¸…	0xe1 0xb8 0x85	LATIN SMALL LETTER B WITH DOT BELOW
					break;
				case 0x1E06:		//	á¸†	0xe1 0xb8 0x86	LATIN CAPITAL LETTER B WITH LINE BELOW
					*p = 0x1E07;	//	á¸‡	0xe1 0xb8 0x87	LATIN SMALL LETTER B WITH LINE BELOW
					break;
				case 0x1E08:		//	á¸ˆ	0xe1 0xb8 0x88	LATIN CAPITAL LETTER C WITH CEDILLA AND ACUTE
					*p = 0x1E09;	//	á¸‰	0xe1 0xb8 0x89	LATIN SMALL LETTER C WITH CEDILLA AND ACUTE
					break;
				case 0x1E0A:		//	á¸Š	0xe1 0xb8 0x8a	LATIN CAPITAL LETTER D WITH DOT ABOVE
					*p = 0x1E0B;	//	á¸‹	0xe1 0xb8 0x8b	LATIN SMALL LETTER D WITH DOT ABOVE
					break;
				case 0x1E0C:		//	á¸Œ	0xe1 0xb8 0x8c	LATIN CAPITAL LETTER D WITH DOT BELOW
					*p = 0x1E0D;	//	á¸	0xe1 0xb8 0x8d	LATIN SMALL LETTER D WITH DOT BELOW
					break;
				case 0x1E0E:		//	á¸Ž	0xe1 0xb8 0x8e	LATIN CAPITAL LETTER D WITH LINE BELOW
					*p = 0x1E0F;	//	á¸	0xe1 0xb8 0x8f	LATIN SMALL LETTER D WITH LINE BELOW
					break;
				case 0x1E10:		//	á¸	0xe1 0xb8 0x90	LATIN CAPITAL LETTER D WITH CEDILLA
					*p = 0x1E11;	//	á¸‘	0xe1 0xb8 0x91	LATIN SMALL LETTER D WITH CEDILLA
					break;
				case 0x1E12:		//	á¸’	0xe1 0xb8 0x92	LATIN CAPITAL LETTER D WITH CIRCUMFLEX BELOW
					*p = 0x1E13;	//	á¸“	0xe1 0xb8 0x93	LATIN SMALL LETTER D WITH CIRCUMFLEX BELOW
					break;
				case 0x1E14:		//	á¸”	0xe1 0xb8 0x94	LATIN CAPITAL LETTER E WITH MACRON AND GRAVE
					*p = 0x1E15;	//	á¸•	0xe1 0xb8 0x95	LATIN SMALL LETTER E WITH MACRON AND GRAVE
					break;
				case 0x1E16:		//	á¸–	0xe1 0xb8 0x96	LATIN CAPITAL LETTER E WITH MACRON AND ACUTE
					*p = 0x1E17;	//	á¸—	0xe1 0xb8 0x97	LATIN SMALL LETTER E WITH MACRON AND ACUTE
					break;
				case 0x1E18:		//	á¸˜	0xe1 0xb8 0x98	LATIN CAPITAL LETTER E WITH CIRCUMFLEX BELOW
					*p = 0x1E19;	//	á¸™	0xe1 0xb8 0x99	LATIN SMALL LETTER E WITH CIRCUMFLEX BELOW
					break;
				case 0x1E1A:		//	á¸š	0xe1 0xb8 0x9a	LATIN CAPITAL LETTER E WITH TILDE BELOW
					*p = 0x1E1B;	//	á¸›	0xe1 0xb8 0x9b	LATIN SMALL LETTER E WITH TILDE BELOW
					break;
				case 0x1E1C:		//	á¸œ	0xe1 0xb8 0x9c	LATIN CAPITAL LETTER E WITH CEDILLA AND BREVE
					*p = 0x1E1D;	//	á¸	0xe1 0xb8 0x9d	LATIN SMALL LETTER E WITH CEDILLA AND BREVE
					break;
				case 0x1E1E:		//	á¸ž	0xe1 0xb8 0x9e	LATIN CAPITAL LETTER F WITH DOT ABOVE
					*p = 0x1E1F;	//	á¸Ÿ	0xe1 0xb8 0x9f	LATIN SMALL LETTER F WITH DOT ABOVE
					break;
				case 0x1E20:		//	á¸ 	0xe1 0xb8 0xa0	LATIN CAPITAL LETTER G WITH MACRON
					*p = 0x1E21;	//	á¸¡	0xe1 0xb8 0xa1	LATIN SMALL LETTER G WITH MACRON
					break;
				case 0x1E22:		//	á¸¢	0xe1 0xb8 0xa2	LATIN CAPITAL LETTER H WITH DOT ABOVE
					*p = 0x1E23;	//	á¸£	0xe1 0xb8 0xa3	LATIN SMALL LETTER H WITH DOT ABOVE
					break;
				case 0x1E24:		//	á¸¤	0xe1 0xb8 0xa4	LATIN CAPITAL LETTER H WITH DOT BELOW
					*p = 0x1E25;	//	á¸¥	0xe1 0xb8 0xa5	LATIN SMALL LETTER H WITH DOT BELOW
					break;
				case 0x1E26:		//	á¸¦	0xe1 0xb8 0xa6	LATIN CAPITAL LETTER H WITH DIAERESIS
					*p = 0x1E27;	//	á¸§	0xe1 0xb8 0xa7	LATIN SMALL LETTER H WITH DIAERESIS
					break;
				case 0x1E28:		//	á¸¨	0xe1 0xb8 0xa8	LATIN CAPITAL LETTER H WITH CEDILLA
					*p = 0x1E29;	//	á¸©	0xe1 0xb8 0xa9	LATIN SMALL LETTER H WITH CEDILLA
					break;
				case 0x1E2A:		//	á¸ª	0xe1 0xb8 0xaa	LATIN CAPITAL LETTER H WITH BREVE BELOW
					*p = 0x1E2B;	//	á¸«	0xe1 0xb8 0xab	LATIN SMALL LETTER H WITH BREVE BELOW
					break;
				case 0x1E2C:		//	á¸¬	0xe1 0xb8 0xac	LATIN CAPITAL LETTER I WITH TILDE BELOW
					*p = 0x1E2D;	//	á¸­	0xe1 0xb8 0xad	LATIN SMALL LETTER I WITH TILDE BELOW
					break;
				case 0x1E2E:		//	á¸®	0xe1 0xb8 0xae	LATIN CAPITAL LETTER I WITH DIAERESIS AND ACUTE
					*p = 0x1E2F;	//	á¸¯	0xe1 0xb8 0xaf	LATIN SMALL LETTER I WITH DIAERESIS AND ACUTE
					break;
				case 0x1E30:		//	á¸°	0xe1 0xb8 0xb0	LATIN CAPITAL LETTER K WITH ACUTE
					*p = 0x1E31;	//	á¸±	0xe1 0xb8 0xb1	LATIN SMALL LETTER K WITH ACUTE
					break;
				case 0x1E32:		//	á¸²	0xe1 0xb8 0xb2	LATIN CAPITAL LETTER K WITH DOT BELOW
					*p = 0x1E33;	//	á¸³	0xe1 0xb8 0xb3	LATIN SMALL LETTER K WITH DOT BELOW
					break;
				case 0x1E34:		//	á¸´	0xe1 0xb8 0xb4	LATIN CAPITAL LETTER K WITH LINE BELOW
					*p = 0x1E35;	//	á¸µ	0xe1 0xb8 0xb5	LATIN SMALL LETTER K WITH LINE BELOW
					break;
				case 0x1E36:		//	á¸¶	0xe1 0xb8 0xb6	LATIN CAPITAL LETTER L WITH DOT BELOW
					*p = 0x1E37;	//	á¸·	0xe1 0xb8 0xb7	LATIN SMALL LETTER L WITH DOT BELOW
					break;
				case 0x1E38:		//	á¸¸	0xe1 0xb8 0xb8	LATIN CAPITAL LETTER L WITH DOT BELOW AND MACRON
					*p = 0x1E39;	//	á¸¹	0xe1 0xb8 0xb9	LATIN SMALL LETTER L WITH DOT BELOW AND MACRON
					break;
				case 0x1E3A:		//	á¸º	0xe1 0xb8 0xba	LATIN CAPITAL LETTER L WITH LINE BELOW
					*p = 0x1E3B;	//	á¸»	0xe1 0xb8 0xbb	LATIN SMALL LETTER L WITH LINE BELOW
					break;
				case 0x1E3C:		//	á¸¼	0xe1 0xb8 0xbc	LATIN CAPITAL LETTER L WITH CIRCUMFLEX BELOW
					*p = 0x1E3D;	//	á¸½	0xe1 0xb8 0xbd	LATIN SMALL LETTER L WITH CIRCUMFLEX BELOW
					break;
				case 0x1E3E:		//	á¸¾	0xe1 0xb8 0xbe	LATIN CAPITAL LETTER M WITH ACUTE
					*p = 0x1E3F;	//	á¸¿	0xe1 0xb8 0xbf	LATIN SMALL LETTER M WITH ACUTE
					break;
				case 0x1E40:		//	á¹€	0xe1 0xb9 0x80	LATIN CAPITAL LETTER M WITH DOT ABOVE
					*p = 0x1E41;	//	á¹	0xe1 0xb9 0x81	LATIN SMALL LETTER M WITH DOT ABOVE
					break;
				case 0x1E42:		//	á¹‚	0xe1 0xb9 0x82	LATIN CAPITAL LETTER M WITH DOT BELOW
					*p = 0x1E43;	//	á¹ƒ	0xe1 0xb9 0x83	LATIN SMALL LETTER M WITH DOT BELOW
					break;
				case 0x1E44:		//	á¹„	0xe1 0xb9 0x84	LATIN CAPITAL LETTER N WITH DOT ABOVE
					*p = 0x1E45;	//	á¹…	0xe1 0xb9 0x85	LATIN SMALL LETTER N WITH DOT ABOVE
					break;
				case 0x1E46:		//	á¹†	0xe1 0xb9 0x86	LATIN CAPITAL LETTER N WITH DOT BELOW
					*p = 0x1E47;	//	á¹‡	0xe1 0xb9 0x87	LATIN SMALL LETTER N WITH DOT BELOW
					break;
				case 0x1E48:		//	á¹ˆ	0xe1 0xb9 0x88	LATIN CAPITAL LETTER N WITH LINE BELOW
					*p = 0x1E49;	//	á¹‰	0xe1 0xb9 0x89	LATIN SMALL LETTER N WITH LINE BELOW
					break;
				case 0x1E4A:		//	á¹Š	0xe1 0xb9 0x8a	LATIN CAPITAL LETTER N WITH CIRCUMFLEX BELOW
					*p = 0x1E4B;	//	á¹‹	0xe1 0xb9 0x8b	LATIN SMALL LETTER N WITH CIRCUMFLEX BELOW
					break;
				case 0x1E4C:		//	á¹Œ	0xe1 0xb9 0x8c	LATIN CAPITAL LETTER O WITH TILDE AND ACUTE
					*p = 0x1E4D;	//	á¹	0xe1 0xb9 0x8d	LATIN SMALL LETTER O WITH TILDE AND ACUTE
					break;
				case 0x1E4E:		//	á¹Ž	0xe1 0xb9 0x8e	LATIN CAPITAL LETTER O WITH TILDE AND DIAERESIS
					*p = 0x1E4F;	//	á¹	0xe1 0xb9 0x8f	LATIN SMALL LETTER O WITH TILDE AND DIAERESIS
					break;
				case 0x1E50:		//	á¹	0xe1 0xb9 0x90	LATIN CAPITAL LETTER O WITH MACRON AND GRAVE
					*p = 0x1E51;	//	á¹‘	0xe1 0xb9 0x91	LATIN SMALL LETTER O WITH MACRON AND GRAVE
					break;
				case 0x1E52:		//	á¹’	0xe1 0xb9 0x92	LATIN CAPITAL LETTER O WITH MACRON AND ACUTE
					*p = 0x1E53;	//	á¹“	0xe1 0xb9 0x93	LATIN SMALL LETTER O WITH MACRON AND ACUTE
					break;
				case 0x1E54:		//	á¹”	0xe1 0xb9 0x94	LATIN CAPITAL LETTER P WITH ACUTE
					*p = 0x1E55;	//	á¹•	0xe1 0xb9 0x95	LATIN SMALL LETTER P WITH ACUTE
					break;
				case 0x1E56:		//	á¹–	0xe1 0xb9 0x96	LATIN CAPITAL LETTER P WITH DOT ABOVE
					*p = 0x1E57;	//	á¹—	0xe1 0xb9 0x97	LATIN SMALL LETTER P WITH DOT ABOVE
					break;
				case 0x1E58:		//	á¹˜	0xe1 0xb9 0x98	LATIN CAPITAL LETTER R WITH DOT ABOVE
					*p = 0x1E59;	//	á¹™	0xe1 0xb9 0x99	LATIN SMALL LETTER R WITH DOT ABOVE
					break;
				case 0x1E5A:		//	á¹š	0xe1 0xb9 0x9a	LATIN CAPITAL LETTER R WITH DOT BELOW
					*p = 0x1E5B;	//	á¹›	0xe1 0xb9 0x9b	LATIN SMALL LETTER R WITH DOT BELOW
					break;
				case 0x1E5C:		//	á¹œ	0xe1 0xb9 0x9c	LATIN CAPITAL LETTER R WITH DOT BELOW AND MACRON
					*p = 0x1E5D;	//	á¹	0xe1 0xb9 0x9d	LATIN SMALL LETTER R WITH DOT BELOW AND MACRON
					break;
				case 0x1E5E:		//	á¹ž	0xe1 0xb9 0x9e	LATIN CAPITAL LETTER R WITH LINE BELOW
					*p = 0x1E5F;	//	á¹Ÿ	0xe1 0xb9 0x9f	LATIN SMALL LETTER R WITH LINE BELOW
					break;
				case 0x1E60:		//	á¹ 	0xe1 0xb9 0xa0	LATIN CAPITAL LETTER S WITH DOT ABOVE
					*p = 0x1E61;	//	á¹¡	0xe1 0xb9 0xa1	LATIN SMALL LETTER S WITH DOT ABOVE
					break;
				case 0x1E62:		//	á¹¢	0xe1 0xb9 0xa2	LATIN CAPITAL LETTER S WITH DOT BELOW
					*p = 0x1E63;	//	á¹£	0xe1 0xb9 0xa3	LATIN SMALL LETTER S WITH DOT BELOW
					break;
				case 0x1E64:		//	á¹¤	0xe1 0xb9 0xa4	LATIN CAPITAL LETTER S WITH ACUTE AND DOT ABOVE
					*p = 0x1E65;	//	á¹¥	0xe1 0xb9 0xa5	LATIN SMALL LETTER S WITH ACUTE AND DOT ABOVE
					break;
				case 0x1E66:		//	á¹¦	0xe1 0xb9 0xa6	LATIN CAPITAL LETTER S WITH CARON AND DOT ABOVE
					*p = 0x1E67;	//	á¹§	0xe1 0xb9 0xa7	LATIN SMALL LETTER S WITH CARON AND DOT ABOVE
					break;
				case 0x1E68:		//	á¹¨	0xe1 0xb9 0xa8	LATIN CAPITAL LETTER S WITH DOT BELOW AND DOT ABOVE
					*p = 0x1E69;	//	á¹©	0xe1 0xb9 0xa9	LATIN SMALL LETTER S WITH DOT BELOW AND DOT ABOVE
					break;
				case 0x1E6A:		//	á¹ª	0xe1 0xb9 0xaa	LATIN CAPITAL LETTER T WITH DOT ABOVE
					*p = 0x1E6B;	//	á¹«	0xe1 0xb9 0xab	LATIN SMALL LETTER T WITH DOT ABOVE
					break;
				case 0x1E6C:		//	á¹¬	0xe1 0xb9 0xac	LATIN CAPITAL LETTER T WITH DOT BELOW
					*p = 0x1E6D;	//	á¹­	0xe1 0xb9 0xad	LATIN SMALL LETTER T WITH DOT BELOW
					break;
				case 0x1E6E:		//	á¹®	0xe1 0xb9 0xae	LATIN CAPITAL LETTER T WITH LINE BELOW
					*p = 0x1E6F;	//	á¹¯	0xe1 0xb9 0xaf	LATIN SMALL LETTER T WITH LINE BELOW
					break;
				case 0x1E70:		//	á¹°	0xe1 0xb9 0xb0	LATIN CAPITAL LETTER T WITH CIRCUMFLEX BELOW
					*p = 0x1E71;	//	á¹±	0xe1 0xb9 0xb1	LATIN SMALL LETTER T WITH CIRCUMFLEX BELOW
					break;
				case 0x1E72:		//	á¹²	0xe1 0xb9 0xb2	LATIN CAPITAL LETTER U WITH DIAERESIS BELOW
					*p = 0x1E73;	//	á¹³	0xe1 0xb9 0xb3	LATIN SMALL LETTER U WITH DIAERESIS BELOW
					break;
				case 0x1E74:		//	á¹´	0xe1 0xb9 0xb4	LATIN CAPITAL LETTER U WITH TILDE BELOW
					*p = 0x1E75;	//	á¹µ	0xe1 0xb9 0xb5	LATIN SMALL LETTER U WITH TILDE BELOW
					break;
				case 0x1E76:		//	á¹¶	0xe1 0xb9 0xb6	LATIN CAPITAL LETTER U WITH CIRCUMFLEX BELOW
					*p = 0x1E77;	//	á¹·	0xe1 0xb9 0xb7	LATIN SMALL LETTER U WITH CIRCUMFLEX BELOW
					break;
				case 0x1E78:		//	á¹¸	0xe1 0xb9 0xb8	LATIN CAPITAL LETTER U WITH TILDE AND ACUTE
					*p = 0x1E79;	//	á¹¹	0xe1 0xb9 0xb9	LATIN SMALL LETTER U WITH TILDE AND ACUTE
					break;
				case 0x1E7A:		//	á¹º	0xe1 0xb9 0xba	LATIN CAPITAL LETTER U WITH MACRON AND DIAERESIS
					*p = 0x1E7B;	//	á¹»	0xe1 0xb9 0xbb	LATIN SMALL LETTER U WITH MACRON AND DIAERESIS
					break;
				case 0x1E7C:		//	á¹¼	0xe1 0xb9 0xbc	LATIN CAPITAL LETTER V WITH TILDE
					*p = 0x1E7D;	//	á¹½	0xe1 0xb9 0xbd	LATIN SMALL LETTER V WITH TILDE
					break;
				case 0x1E7E:		//	á¹¾	0xe1 0xb9 0xbe	LATIN CAPITAL LETTER V WITH DOT BELOW
					*p = 0x1E7F;	//	á¹¿	0xe1 0xb9 0xbf	LATIN SMALL LETTER V WITH DOT BELOW
					break;
				case 0x1E80:		//	áº€	0xe1 0xba 0x80	LATIN CAPITAL LETTER W WITH GRAVE
					*p = 0x1E81;	//	áº	0xe1 0xba 0x81	LATIN SMALL LETTER W WITH GRAVE
					break;
				case 0x1E82:		//	áº‚	0xe1 0xba 0x82	LATIN CAPITAL LETTER W WITH ACUTE
					*p = 0x1E83;	//	áºƒ	0xe1 0xba 0x83	LATIN SMALL LETTER W WITH ACUTE
					break;
				case 0x1E84:		//	áº„	0xe1 0xba 0x84	LATIN CAPITAL LETTER W WITH DIAERESIS
					*p = 0x1E85;	//	áº…	0xe1 0xba 0x85	LATIN SMALL LETTER W WITH DIAERESIS
					break;
				case 0x1E86:		//	áº†	0xe1 0xba 0x86	LATIN CAPITAL LETTER W WITH DOT ABOVE
					*p = 0x1E87;	//	áº‡	0xe1 0xba 0x87	LATIN SMALL LETTER W WITH DOT ABOVE
					break;
				case 0x1E88:		//	áºˆ	0xe1 0xba 0x88	LATIN CAPITAL LETTER W WITH DOT BELOW
					*p = 0x1E89;	//	áº‰	0xe1 0xba 0x89	LATIN SMALL LETTER W WITH DOT BELOW
					break;
				case 0x1E8A:		//	áºŠ	0xe1 0xba 0x8a	LATIN CAPITAL LETTER X WITH DOT ABOVE
					*p = 0x1E8B;	//	áº‹	0xe1 0xba 0x8b	LATIN SMALL LETTER X WITH DOT ABOVE
					break;
				case 0x1E8C:		//	áºŒ	0xe1 0xba 0x8c	LATIN CAPITAL LETTER X WITH DIAERESIS
					*p = 0x1E8D;	//	áº	0xe1 0xba 0x8d	LATIN SMALL LETTER X WITH DIAERESIS
					break;
				case 0x1E8E:		//	áºŽ	0xe1 0xba 0x8e	LATIN CAPITAL LETTER Y WITH DOT ABOVE
					*p = 0x1E8F;	//	áº	0xe1 0xba 0x8f	LATIN SMALL LETTER Y WITH DOT ABOVE
					break;
				case 0x1E90:		//	áº	0xe1 0xba 0x90	LATIN CAPITAL LETTER Z WITH CIRCUMFLEX
					*p = 0x1E91;	//	áº‘	0xe1 0xba 0x91	LATIN SMALL LETTER Z WITH CIRCUMFLEX
					break;
				case 0x1E92:		//	áº’	0xe1 0xba 0x92	LATIN CAPITAL LETTER Z WITH DOT BELOW
					*p = 0x1E93;	//	áº“	0xe1 0xba 0x93	LATIN SMALL LETTER Z WITH DOT BELOW
					break;
				case 0x1E94:		//	áº”	0xe1 0xba 0x94	LATIN CAPITAL LETTER Z WITH LINE BELOW
					*p = 0x1E95;	//	áº•	0xe1 0xba 0x95	LATIN SMALL LETTER Z WITH LINE BELOW
					break;
				case 0x1E9E:		//	áºž	0xe1 0xba 0x9e	LATIN CAPITAL LETTER SHARP S
					*p = 0x00DF;	//	ÃŸ	0xc3 0x9f	LATIN SMALL LETTER SHARP S
					break;
				case 0x1EA0:		//	áº 	0xe1 0xba 0xa0	LATIN CAPITAL LETTER A WITH DOT BELOW
					*p = 0x1EA1;	//	áº¡	0xe1 0xba 0xa1	LATIN SMALL LETTER A WITH DOT BELOW
					break;
				case 0x1EA2:		//	áº¢	0xe1 0xba 0xa2	LATIN CAPITAL LETTER A WITH HOOK ABOVE
					*p = 0x1EA3;	//	áº£	0xe1 0xba 0xa3	LATIN SMALL LETTER A WITH HOOK ABOVE
					break;
				case 0x1EA4:		//	áº¤	0xe1 0xba 0xa4	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND ACUTE
					*p = 0x1EA5;	//	áº¥	0xe1 0xba 0xa5	LATIN SMALL LETTER A WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1EA6:		//	áº¦	0xe1 0xba 0xa6	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND GRAVE
					*p = 0x1EA7;	//	áº§	0xe1 0xba 0xa7	LATIN SMALL LETTER A WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1EA8:		//	áº¨	0xe1 0xba 0xa8	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1EA9;	//	áº©	0xe1 0xba 0xa9	LATIN SMALL LETTER A WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1EAA:		//	áºª	0xe1 0xba 0xaa	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND TILDE
					*p = 0x1EAB;	//	áº«	0xe1 0xba 0xab	LATIN SMALL LETTER A WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1EAC:		//	áº¬	0xe1 0xba 0xac	LATIN CAPITAL LETTER A WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1EAD;	//	áº­	0xe1 0xba 0xad	LATIN SMALL LETTER A WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EAE:		//	áº®	0xe1 0xba 0xae	LATIN CAPITAL LETTER A WITH BREVE AND ACUTE
					*p = 0x1EAF;	//	áº¯	0xe1 0xba 0xaf	LATIN SMALL LETTER A WITH BREVE AND ACUTE
					break;
				case 0x1EB0:		//	áº°	0xe1 0xba 0xb0	LATIN CAPITAL LETTER A WITH BREVE AND GRAVE
					*p = 0x1EB1;	//	áº±	0xe1 0xba 0xb1	LATIN SMALL LETTER A WITH BREVE AND GRAVE
					break;
				case 0x1EB2:		//	áº²	0xe1 0xba 0xb2	LATIN CAPITAL LETTER A WITH BREVE AND HOOK ABOVE
					*p = 0x1EB3;	//	áº³	0xe1 0xba 0xb3	LATIN SMALL LETTER A WITH BREVE AND HOOK ABOVE
					break;
				case 0x1EB4:		//	áº´	0xe1 0xba 0xb4	LATIN CAPITAL LETTER A WITH BREVE AND TILDE
					*p = 0x1EB5;	//	áºµ	0xe1 0xba 0xb5	LATIN SMALL LETTER A WITH BREVE AND TILDE
					break;
				case 0x1EB6:		//	áº¶	0xe1 0xba 0xb6	LATIN CAPITAL LETTER A WITH BREVE AND DOT BELOW
					*p = 0x1EB7;	//	áº·	0xe1 0xba 0xb7	LATIN SMALL LETTER A WITH BREVE AND DOT BELOW
					break;
				case 0x1EB8:		//	áº¸	0xe1 0xba 0xb8	LATIN CAPITAL LETTER E WITH DOT BELOW
					*p = 0x1EB9;	//	áº¹	0xe1 0xba 0xb9	LATIN SMALL LETTER E WITH DOT BELOW
					break;
				case 0x1EBA:		//	áºº	0xe1 0xba 0xba	LATIN CAPITAL LETTER E WITH HOOK ABOVE
					*p = 0x1EBB;	//	áº»	0xe1 0xba 0xbb	LATIN SMALL LETTER E WITH HOOK ABOVE
					break;
				case 0x1EBC:		//	áº¼	0xe1 0xba 0xbc	LATIN CAPITAL LETTER E WITH TILDE
					*p = 0x1EBD;	//	áº½	0xe1 0xba 0xbd	LATIN SMALL LETTER E WITH TILDE
					break;
				case 0x1EBE:		//	áº¾	0xe1 0xba 0xbe	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND ACUTE
					*p = 0x1EBF;	//	áº¿	0xe1 0xba 0xbf	LATIN SMALL LETTER E WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1EC0:		//	á»€	0xe1 0xbb 0x80	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND GRAVE
					*p = 0x1EC1;	//	á»	0xe1 0xbb 0x81	LATIN SMALL LETTER E WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1EC2:		//	á»‚	0xe1 0xbb 0x82	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1EC3;	//	á»ƒ	0xe1 0xbb 0x83	LATIN SMALL LETTER E WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1EC4:		//	á»„	0xe1 0xbb 0x84	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND TILDE
					*p = 0x1EC5;	//	á»…	0xe1 0xbb 0x85	LATIN SMALL LETTER E WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1EC6:		//	á»†	0xe1 0xbb 0x86	LATIN CAPITAL LETTER E WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1EC7;	//	á»‡	0xe1 0xbb 0x87	LATIN SMALL LETTER E WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EC8:		//	á»ˆ	0xe1 0xbb 0x88	LATIN CAPITAL LETTER I WITH HOOK ABOVE
					*p = 0x1EC9;	//	á»‰	0xe1 0xbb 0x89	LATIN SMALL LETTER I WITH HOOK ABOVE
					break;
				case 0x1ECA:		//	á»Š	0xe1 0xbb 0x8a	LATIN CAPITAL LETTER I WITH DOT BELOW
					*p = 0x1ECB;	//	á»‹	0xe1 0xbb 0x8b	LATIN SMALL LETTER I WITH DOT BELOW
					break;
				case 0x1ECC:		//	á»Œ	0xe1 0xbb 0x8c	LATIN CAPITAL LETTER O WITH DOT BELOW
					*p = 0x1ECD;	//	á»	0xe1 0xbb 0x8d	LATIN SMALL LETTER O WITH DOT BELOW
					break;
				case 0x1ECE:		//	á»Ž	0xe1 0xbb 0x8e	LATIN CAPITAL LETTER O WITH HOOK ABOVE
					*p = 0x1ECF;	//	á»	0xe1 0xbb 0x8f	LATIN SMALL LETTER O WITH HOOK ABOVE
					break;
				case 0x1ED0:		//	á»	0xe1 0xbb 0x90	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND ACUTE
					*p = 0x1ED1;	//	á»‘	0xe1 0xbb 0x91	LATIN SMALL LETTER O WITH CIRCUMFLEX AND ACUTE
					break;
				case 0x1ED2:		//	á»’	0xe1 0xbb 0x92	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND GRAVE
					*p = 0x1ED3;	//	á»“	0xe1 0xbb 0x93	LATIN SMALL LETTER O WITH CIRCUMFLEX AND GRAVE
					break;
				case 0x1ED4:		//	á»”	0xe1 0xbb 0x94	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
					*p = 0x1ED5;	//	á»•	0xe1 0xbb 0x95	LATIN SMALL LETTER O WITH CIRCUMFLEX AND HOOK ABOVE
					break;
				case 0x1ED6:		//	á»–	0xe1 0xbb 0x96	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND TILDE
					*p = 0x1ED7;	//	á»—	0xe1 0xbb 0x97	LATIN SMALL LETTER O WITH CIRCUMFLEX AND TILDE
					break;
				case 0x1ED8:		//	á»˜	0xe1 0xbb 0x98	LATIN CAPITAL LETTER O WITH CIRCUMFLEX AND DOT BELOW
					*p = 0x1ED9;	//	á»™	0xe1 0xbb 0x99	LATIN SMALL LETTER O WITH CIRCUMFLEX AND DOT BELOW
					break;
				case 0x1EDA:		//	á»š	0xe1 0xbb 0x9a	LATIN CAPITAL LETTER O WITH HORN AND ACUTE
					*p = 0x1EDB;	//	á»›	0xe1 0xbb 0x9b	LATIN SMALL LETTER O WITH HORN AND ACUTE
					break;
				case 0x1EDC:		//	á»œ	0xe1 0xbb 0x9c	LATIN CAPITAL LETTER O WITH HORN AND GRAVE
					*p = 0x1EDD;	//	á»	0xe1 0xbb 0x9d	LATIN SMALL LETTER O WITH HORN AND GRAVE
					break;
				case 0x1EDE:		//	á»ž	0xe1 0xbb 0x9e	LATIN CAPITAL LETTER O WITH HORN AND HOOK ABOVE
					*p = 0x1EDF;	//	á»Ÿ	0xe1 0xbb 0x9f	LATIN SMALL LETTER O WITH HORN AND HOOK ABOVE
					break;
				case 0x1EE0:		//	á» 	0xe1 0xbb 0xa0	LATIN CAPITAL LETTER O WITH HORN AND TILDE
					*p = 0x1EE1;	//	á»¡	0xe1 0xbb 0xa1	LATIN SMALL LETTER O WITH HORN AND TILDE
					break;
				case 0x1EE2:		//	á»¢	0xe1 0xbb 0xa2	LATIN CAPITAL LETTER O WITH HORN AND DOT BELOW
					*p = 0x1EE3;	//	á»£	0xe1 0xbb 0xa3	LATIN SMALL LETTER O WITH HORN AND DOT BELOW
					break;
				case 0x1EE4:		//	á»¤	0xe1 0xbb 0xa4	LATIN CAPITAL LETTER U WITH DOT BELOW
					*p = 0x1EE5;	//	á»¥	0xe1 0xbb 0xa5	LATIN SMALL LETTER U WITH DOT BELOW
					break;
				case 0x1EE6:		//	á»¦	0xe1 0xbb 0xa6	LATIN CAPITAL LETTER U WITH HOOK ABOVE
					*p = 0x1EE7;	//	á»§	0xe1 0xbb 0xa7	LATIN SMALL LETTER U WITH HOOK ABOVE
					break;
				case 0x1EE8:		//	á»¨	0xe1 0xbb 0xa8	LATIN CAPITAL LETTER U WITH HORN AND ACUTE
					*p = 0x1EE9;	//	á»©	0xe1 0xbb 0xa9	LATIN SMALL LETTER U WITH HORN AND ACUTE
					break;
				case 0x1EEA:		//	á»ª	0xe1 0xbb 0xaa	LATIN CAPITAL LETTER U WITH HORN AND GRAVE
					*p = 0x1EEB;	//	á»«	0xe1 0xbb 0xab	LATIN SMALL LETTER U WITH HORN AND GRAVE
					break;
				case 0x1EEC:		//	á»¬	0xe1 0xbb 0xac	LATIN CAPITAL LETTER U WITH HORN AND HOOK ABOVE
					*p = 0x1EED;	//	á»­	0xe1 0xbb 0xad	LATIN SMALL LETTER U WITH HORN AND HOOK ABOVE
					break;
				case 0x1EEE:		//	á»®	0xe1 0xbb 0xae	LATIN CAPITAL LETTER U WITH HORN AND TILDE
					*p = 0x1EEF;	//	á»¯	0xe1 0xbb 0xaf	LATIN SMALL LETTER U WITH HORN AND TILDE
					break;
				case 0x1EF0:		//	á»°	0xe1 0xbb 0xb0	LATIN CAPITAL LETTER U WITH HORN AND DOT BELOW
					*p = 0x1EF1;	//	á»±	0xe1 0xbb 0xb1	LATIN SMALL LETTER U WITH HORN AND DOT BELOW
					break;
				case 0x1EF2:		//	á»²	0xe1 0xbb 0xb2	LATIN CAPITAL LETTER Y WITH GRAVE
					*p = 0x1EF3;	//	á»³	0xe1 0xbb 0xb3	LATIN SMALL LETTER Y WITH GRAVE
					break;
				case 0x1EF4:		//	á»´	0xe1 0xbb 0xb4	LATIN CAPITAL LETTER Y WITH DOT BELOW
					*p = 0x1EF5;	//	á»µ	0xe1 0xbb 0xb5	LATIN SMALL LETTER Y WITH DOT BELOW
					break;
				case 0x1EF6:		//	á»¶	0xe1 0xbb 0xb6	LATIN CAPITAL LETTER Y WITH HOOK ABOVE
					*p = 0x1EF7;	//	á»·	0xe1 0xbb 0xb7	LATIN SMALL LETTER Y WITH HOOK ABOVE
					break;
				case 0x1EF8:		//	á»¸	0xe1 0xbb 0xb8	LATIN CAPITAL LETTER Y WITH TILDE
					*p = 0x1EF9;	//	á»¹	0xe1 0xbb 0xb9	LATIN SMALL LETTER Y WITH TILDE
					break;
				case 0x1EFA:		//	á»º	0xe1 0xbb 0xba	LATIN CAPITAL LETTER MIDDLE-WELSH LL
					*p = 0x1EFB;	//	á»»	0xe1 0xbb 0xbb	LATIN SMALL LETTER MIDDLE-WELSH LL
					break;
				case 0x1EFC:		//	á»¼	0xe1 0xbb 0xbc	LATIN CAPITAL LETTER MIDDLE-WELSH V
					*p = 0x1EFD;	//	á»½	0xe1 0xbb 0xbd	LATIN SMALL LETTER MIDDLE-WELSH V
					break;
				case 0x1EFE:		//	á»¾	0xe1 0xbb 0xbe	LATIN CAPITAL LETTER Y WITH LOOP
					*p = 0x1EFF;	//	á»¿	0xe1 0xbb 0xbf	LATIN SMALL LETTER Y WITH LOOP
					break;
				case 0x1F08:		//	á¼ˆ	0xe1 0xbc 0x88	GREEK CAPITAL LETTER ALPHA WITH PSILI
					*p = 0x1F00;	//	á¼€	0xe1 0xbc 0x80	GREEK SMALL LETTER ALPHA WITH PSILI
					break;
				case 0x1F09:		//	á¼‰	0xe1 0xbc 0x89	GREEK CAPITAL LETTER ALPHA WITH DASIA
					*p = 0x1F01;	//	á¼	0xe1 0xbc 0x81	GREEK SMALL LETTER ALPHA WITH DASIA
					break;
				case 0x1F0A:		//	á¼Š	0xe1 0xbc 0x8a	GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA
					*p = 0x1F02;	//	á¼‚	0xe1 0xbc 0x82	GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA
					break;
				case 0x1F0B:		//	á¼‹	0xe1 0xbc 0x8b	GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA
					*p = 0x1F03;	//	á¼ƒ	0xe1 0xbc 0x83	GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA
					break;
				case 0x1F0C:		//	á¼Œ	0xe1 0xbc 0x8c	GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA
					*p = 0x1F04;	//	á¼„	0xe1 0xbc 0x84	GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA
					break;
				case 0x1F0D:		//	á¼	0xe1 0xbc 0x8d	GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA
					*p = 0x1F05;	//	á¼…	0xe1 0xbc 0x85	GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA
					break;
				case 0x1F0E:		//	á¼Ž	0xe1 0xbc 0x8e	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI
					*p = 0x1F06;	//	á¼†	0xe1 0xbc 0x86	GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F0F:		//	á¼	0xe1 0xbc 0x8f	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI
					*p = 0x1F07;	//	á¼‡	0xe1 0xbc 0x87	GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F18:		//	á¼˜	0xe1 0xbc 0x98	GREEK CAPITAL LETTER EPSILON WITH PSILI
					*p = 0x1F10;	//	á¼	0xe1 0xbc 0x90	GREEK SMALL LETTER EPSILON WITH PSILI
					break;
				case 0x1F19:		//	á¼™	0xe1 0xbc 0x99	GREEK CAPITAL LETTER EPSILON WITH DASIA
					*p = 0x1F11;	//	á¼‘	0xe1 0xbc 0x91	GREEK SMALL LETTER EPSILON WITH DASIA
					break;
				case 0x1F1A:		//	á¼š	0xe1 0xbc 0x9a	GREEK CAPITAL LETTER EPSILON WITH PSILI AND VARIA
					*p = 0x1F12;	//	á¼’	0xe1 0xbc 0x92	GREEK SMALL LETTER EPSILON WITH PSILI AND VARIA
					break;
				case 0x1F1B:		//	á¼›	0xe1 0xbc 0x9b	GREEK CAPITAL LETTER EPSILON WITH DASIA AND VARIA
					*p = 0x1F13;	//	á¼“	0xe1 0xbc 0x93	GREEK SMALL LETTER EPSILON WITH DASIA AND VARIA
					break;
				case 0x1F1C:		//	á¼œ	0xe1 0xbc 0x9c	GREEK CAPITAL LETTER EPSILON WITH PSILI AND OXIA
					*p = 0x1F14;	//	á¼”	0xe1 0xbc 0x94	GREEK SMALL LETTER EPSILON WITH PSILI AND OXIA
					break;
				case 0x1F1D:		//	á¼	0xe1 0xbc 0x9d	GREEK CAPITAL LETTER EPSILON WITH DASIA AND OXIA
					*p = 0x1F15;	//	á¼•	0xe1 0xbc 0x95	GREEK SMALL LETTER EPSILON WITH DASIA AND OXIA
					break;
				case 0x1F28:		//	á¼¨	0xe1 0xbc 0xa8	GREEK CAPITAL LETTER ETA WITH PSILI
					*p = 0x1F20;	//	á¼ 	0xe1 0xbc 0xa0	GREEK SMALL LETTER ETA WITH PSILI
					break;
				case 0x1F29:		//	á¼©	0xe1 0xbc 0xa9	GREEK CAPITAL LETTER ETA WITH DASIA
					*p = 0x1F21;	//	á¼¡	0xe1 0xbc 0xa1	GREEK SMALL LETTER ETA WITH DASIA
					break;
				case 0x1F2A:		//	á¼ª	0xe1 0xbc 0xaa	GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA
					*p = 0x1F22;	//	á¼¢	0xe1 0xbc 0xa2	GREEK SMALL LETTER ETA WITH PSILI AND VARIA
					break;
				case 0x1F2B:		//	á¼«	0xe1 0xbc 0xab	GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA
					*p = 0x1F23;	//	á¼£	0xe1 0xbc 0xa3	GREEK SMALL LETTER ETA WITH DASIA AND VARIA
					break;
				case 0x1F2C:		//	á¼¬	0xe1 0xbc 0xac	GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA
					*p = 0x1F24;	//	á¼¤	0xe1 0xbc 0xa4	GREEK SMALL LETTER ETA WITH PSILI AND OXIA
					break;
				case 0x1F2D:		//	á¼­	0xe1 0xbc 0xad	GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA
					*p = 0x1F25;	//	á¼¥	0xe1 0xbc 0xa5	GREEK SMALL LETTER ETA WITH DASIA AND OXIA
					break;
				case 0x1F2E:		//	á¼®	0xe1 0xbc 0xae	GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI
					*p = 0x1F26;	//	á¼¦	0xe1 0xbc 0xa6	GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F2F:		//	á¼¯	0xe1 0xbc 0xaf	GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI
					*p = 0x1F27;	//	á¼§	0xe1 0xbc 0xa7	GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F38:		//	á¼¸	0xe1 0xbc 0xb8	GREEK CAPITAL LETTER IOTA WITH PSILI
					*p = 0x1F30;	//	á¼°	0xe1 0xbc 0xb0	GREEK SMALL LETTER IOTA WITH PSILI
					break;
				case 0x1F39:		//	á¼¹	0xe1 0xbc 0xb9	GREEK CAPITAL LETTER IOTA WITH DASIA
					*p = 0x1F31;	//	á¼±	0xe1 0xbc 0xb1	GREEK SMALL LETTER IOTA WITH DASIA
					break;
				case 0x1F3A:		//	á¼º	0xe1 0xbc 0xba	GREEK CAPITAL LETTER IOTA WITH PSILI AND VARIA
					*p = 0x1F32;	//	á¼²	0xe1 0xbc 0xb2	GREEK SMALL LETTER IOTA WITH PSILI AND VARIA
					break;
				case 0x1F3B:		//	á¼»	0xe1 0xbc 0xbb	GREEK CAPITAL LETTER IOTA WITH DASIA AND VARIA
					*p = 0x1F33;	//	á¼³	0xe1 0xbc 0xb3	GREEK SMALL LETTER IOTA WITH DASIA AND VARIA
					break;
				case 0x1F3C:		//	á¼¼	0xe1 0xbc 0xbc	GREEK CAPITAL LETTER IOTA WITH PSILI AND OXIA
					*p = 0x1F34;	//	á¼´	0xe1 0xbc 0xb4	GREEK SMALL LETTER IOTA WITH PSILI AND OXIA
					break;
				case 0x1F3D:		//	á¼½	0xe1 0xbc 0xbd	GREEK CAPITAL LETTER IOTA WITH DASIA AND OXIA
					*p = 0x1F35;	//	á¼µ	0xe1 0xbc 0xb5	GREEK SMALL LETTER IOTA WITH DASIA AND OXIA
					break;
				case 0x1F3E:		//	á¼¾	0xe1 0xbc 0xbe	GREEK CAPITAL LETTER IOTA WITH PSILI AND PERISPOMENI
					*p = 0x1F36;	//	á¼¶	0xe1 0xbc 0xb6	GREEK SMALL LETTER IOTA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F3F:		//	á¼¿	0xe1 0xbc 0xbf	GREEK CAPITAL LETTER IOTA WITH DASIA AND PERISPOMENI
					*p = 0x1F37;	//	á¼·	0xe1 0xbc 0xb7	GREEK SMALL LETTER IOTA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F48:		//	á½ˆ	0xe1 0xbd 0x88	GREEK CAPITAL LETTER OMICRON WITH PSILI
					*p = 0x1F40;	//	á½€	0xe1 0xbd 0x80	GREEK SMALL LETTER OMICRON WITH PSILI
					break;
				case 0x1F49:		//	á½‰	0xe1 0xbd 0x89	GREEK CAPITAL LETTER OMICRON WITH DASIA
					*p = 0x1F41;	//	á½	0xe1 0xbd 0x81	GREEK SMALL LETTER OMICRON WITH DASIA
					break;
				case 0x1F4A:		//	á½Š	0xe1 0xbd 0x8a	GREEK CAPITAL LETTER OMICRON WITH PSILI AND VARIA
					*p = 0x1F42;	//	á½‚	0xe1 0xbd 0x82	GREEK SMALL LETTER OMICRON WITH PSILI AND VARIA
					break;
				case 0x1F4B:		//	á½‹	0xe1 0xbd 0x8b	GREEK CAPITAL LETTER OMICRON WITH DASIA AND VARIA
					*p = 0x1F43;	//	á½ƒ	0xe1 0xbd 0x83	GREEK SMALL LETTER OMICRON WITH DASIA AND VARIA
					break;
				case 0x1F4C:		//	á½Œ	0xe1 0xbd 0x8c	GREEK CAPITAL LETTER OMICRON WITH PSILI AND OXIA
					*p = 0x1F44;	//	á½„	0xe1 0xbd 0x84	GREEK SMALL LETTER OMICRON WITH PSILI AND OXIA
					break;
				case 0x1F4D:		//	á½	0xe1 0xbd 0x8d	GREEK CAPITAL LETTER OMICRON WITH DASIA AND OXIA
					*p = 0x1F45;	//	á½…	0xe1 0xbd 0x85	GREEK SMALL LETTER OMICRON WITH DASIA AND OXIA
					break;
				case 0x1F59:		//	á½™	0xe1 0xbd 0x99	GREEK CAPITAL LETTER UPSILON WITH DASIA
					*p = 0x1F51;	//	á½‘	0xe1 0xbd 0x91	GREEK SMALL LETTER UPSILON WITH DASIA
					break;
				case 0x1F5B:		//	á½›	0xe1 0xbd 0x9b	GREEK CAPITAL LETTER UPSILON WITH DASIA AND VARIA
					*p = 0x1F53;	//	á½“	0xe1 0xbd 0x93	GREEK SMALL LETTER UPSILON WITH DASIA AND VARIA
					break;
				case 0x1F5D:		//	á½	0xe1 0xbd 0x9d	GREEK CAPITAL LETTER UPSILON WITH DASIA AND OXIA
					*p = 0x1F55;	//	á½•	0xe1 0xbd 0x95	GREEK SMALL LETTER UPSILON WITH DASIA AND OXIA
					break;
				case 0x1F5F:		//	á½Ÿ	0xe1 0xbd 0x9f	GREEK CAPITAL LETTER UPSILON WITH DASIA AND PERISPOMENI
					*p = 0x1F57;	//	á½—	0xe1 0xbd 0x97	GREEK SMALL LETTER UPSILON WITH DASIA AND PERISPOMENI
					break;
				case 0x1F68:		//	á½¨	0xe1 0xbd 0xa8	GREEK CAPITAL LETTER OMEGA WITH PSILI
					*p = 0x1F60;	//	á½ 	0xe1 0xbd 0xa0	GREEK SMALL LETTER OMEGA WITH PSILI
					break;
				case 0x1F69:		//	á½©	0xe1 0xbd 0xa9	GREEK CAPITAL LETTER OMEGA WITH DASIA
					*p = 0x1F61;	//	á½¡	0xe1 0xbd 0xa1	GREEK SMALL LETTER OMEGA WITH DASIA
					break;
				case 0x1F6A:		//	á½ª	0xe1 0xbd 0xaa	GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA
					*p = 0x1F62;	//	á½¢	0xe1 0xbd 0xa2	GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA
					break;
				case 0x1F6B:		//	á½«	0xe1 0xbd 0xab	GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA
					*p = 0x1F63;	//	á½£	0xe1 0xbd 0xa3	GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA
					break;
				case 0x1F6C:		//	á½¬	0xe1 0xbd 0xac	GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA
					*p = 0x1F64;	//	á½¤	0xe1 0xbd 0xa4	GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA
					break;
				case 0x1F6D:		//	á½­	0xe1 0xbd 0xad	GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA
					*p = 0x1F65;	//	á½¥	0xe1 0xbd 0xa5	GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA
					break;
				case 0x1F6E:		//	á½®	0xe1 0xbd 0xae	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI
					*p = 0x1F66;	//	á½¦	0xe1 0xbd 0xa6	GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI
					break;
				case 0x1F6F:		//	á½¯	0xe1 0xbd 0xaf	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI
					*p = 0x1F67;	//	á½§	0xe1 0xbd 0xa7	GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI
					break;
				case 0x1F88:		//	á¾ˆ	0xe1 0xbe 0x88	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1F80;	//	á¾€	0xe1 0xbe 0x80	GREEK SMALL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1F89:		//	á¾‰	0xe1 0xbe 0x89	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1F81;	//	á¾	0xe1 0xbe 0x81	GREEK SMALL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1F8A:		//	á¾Š	0xe1 0xbe 0x8a	GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F82;	//	á¾‚	0xe1 0xbe 0x82	GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F8B:		//	á¾‹	0xe1 0xbe 0x8b	GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F83;	//	á¾ƒ	0xe1 0xbe 0x83	GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F8C:		//	á¾Œ	0xe1 0xbe 0x8c	GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F84;	//	á¾„	0xe1 0xbe 0x84	GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F8D:		//	á¾	0xe1 0xbe 0x8d	GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F85;	//	á¾…	0xe1 0xbe 0x85	GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F8E:		//	á¾Ž	0xe1 0xbe 0x8e	GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F86;	//	á¾†	0xe1 0xbe 0x86	GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F8F:		//	á¾	0xe1 0xbe 0x8f	GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F87;	//	á¾‡	0xe1 0xbe 0x87	GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F98:		//	á¾˜	0xe1 0xbe 0x98	GREEK CAPITAL LETTER ETA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1F90;	//	á¾	0xe1 0xbe 0x90	GREEK SMALL LETTER ETA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1F99:		//	á¾™	0xe1 0xbe 0x99	GREEK CAPITAL LETTER ETA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1F91;	//	á¾‘	0xe1 0xbe 0x91	GREEK SMALL LETTER ETA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1F9A:		//	á¾š	0xe1 0xbe 0x9a	GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F92;	//	á¾’	0xe1 0xbe 0x92	GREEK SMALL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F9B:		//	á¾›	0xe1 0xbe 0x9b	GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1F93;	//	á¾“	0xe1 0xbe 0x93	GREEK SMALL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1F9C:		//	á¾œ	0xe1 0xbe 0x9c	GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F94;	//	á¾”	0xe1 0xbe 0x94	GREEK SMALL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F9D:		//	á¾	0xe1 0xbe 0x9d	GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1F95;	//	á¾•	0xe1 0xbe 0x95	GREEK SMALL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1F9E:		//	á¾ž	0xe1 0xbe 0x9e	GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F96;	//	á¾–	0xe1 0xbe 0x96	GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1F9F:		//	á¾Ÿ	0xe1 0xbe 0x9f	GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1F97;	//	á¾—	0xe1 0xbe 0x97	GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FA8:		//	á¾¨	0xe1 0xbe 0xa8	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI
					*p = 0x1FA0;	//	á¾ 	0xe1 0xbe 0xa0	GREEK SMALL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI
					break;
				case 0x1FA9:		//	á¾©	0xe1 0xbe 0xa9	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI
					*p = 0x1FA1;	//	á¾¡	0xe1 0xbe 0xa1	GREEK SMALL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI
					break;
				case 0x1FAA:		//	á¾ª	0xe1 0xbe 0xaa	GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					*p = 0x1FA2;	//	á¾¢	0xe1 0xbe 0xa2	GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1FAB:		//	á¾«	0xe1 0xbe 0xab	GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					*p = 0x1FA3;	//	á¾£	0xe1 0xbe 0xa3	GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI
					break;
				case 0x1FAC:		//	á¾¬	0xe1 0xbe 0xac	GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					*p = 0x1FA4;	//	á¾¤	0xe1 0xbe 0xa4	GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1FAD:		//	á¾­	0xe1 0xbe 0xad	GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					*p = 0x1FA5;	//	á¾¥	0xe1 0xbe 0xa5	GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI
					break;
				case 0x1FAE:		//	á¾®	0xe1 0xbe 0xae	GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1FA6;	//	á¾¦	0xe1 0xbe 0xa6	GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FAF:		//	á¾¯	0xe1 0xbe 0xaf	GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					*p = 0x1FA7;	//	á¾§	0xe1 0xbe 0xa7	GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
					break;
				case 0x1FB8:		//	á¾¸	0xe1 0xbe 0xb8	GREEK CAPITAL LETTER ALPHA WITH VRACHY
					*p = 0x1FB0;	//	á¾°	0xe1 0xbe 0xb0	GREEK SMALL LETTER ALPHA WITH VRACHY
					break;
				case 0x1FB9:		//	á¾¹	0xe1 0xbe 0xb9	GREEK CAPITAL LETTER ALPHA WITH MACRON
					*p = 0x1FB1;	//	á¾±	0xe1 0xbe 0xb1	GREEK SMALL LETTER ALPHA WITH MACRON
					break;
				case 0x1FBA:		//	á¾º	0xe1 0xbe 0xba	GREEK CAPITAL LETTER ALPHA WITH VARIA
					*p = 0x1F70;	//	á½°	0xe1 0xbd 0xb0	GREEK SMALL LETTER ALPHA WITH VARIA
					break;
				case 0x1FBB:		//	á¾»	0xe1 0xbe 0xbb	GREEK CAPITAL LETTER ALPHA WITH OXIA
					*p = 0x1F71;	//	á½±	0xe1 0xbd 0xb1	GREEK SMALL LETTER ALPHA WITH OXIA
					break;
				case 0x1FBC:		//	á¾¼	0xe1 0xbe 0xbc	GREEK CAPITAL LETTER ALPHA WITH PROSGEGRAMMENI
					*p = 0x1FB3;	//	á¾³	0xe1 0xbe 0xb3	GREEK SMALL LETTER ALPHA WITH PROSGEGRAMMENI
					break;
				case 0x1FC8:		//	á¿ˆ	0xe1 0xbf 0x88	GREEK CAPITAL LETTER EPSILON WITH VARIA
					*p = 0x1F72;	//	á½²	0xe1 0xbd 0xb2	GREEK SMALL LETTER EPSILON WITH VARIA
					break;
				case 0x1FC9:		//	á¿‰	0xe1 0xbf 0x89	GREEK CAPITAL LETTER EPSILON WITH OXIA
					*p = 0x1F73;	//	á½³	0xe1 0xbd 0xb3	GREEK SMALL LETTER EPSILON WITH OXIA
					break;
				case 0x1FCA:		//	á¿Š	0xe1 0xbf 0x8a	GREEK CAPITAL LETTER ETA WITH VARIA
					*p = 0x1F74;	//	á½´	0xe1 0xbd 0xb4	GREEK SMALL LETTER ETA WITH VARIA
					break;
				case 0x1FCB:		//	á¿‹	0xe1 0xbf 0x8b	GREEK CAPITAL LETTER ETA WITH OXIA
					*p = 0x1F75;	//	á½µ	0xe1 0xbd 0xb5	GREEK SMALL LETTER ETA WITH OXIA
					break;
				case 0x1FCC:		//	á¿Œ	0xe1 0xbf 0x8c	GREEK CAPITAL LETTER ETA WITH PROSGEGRAMMENI
					*p = 0x1FC3;	//	á¿ƒ	0xe1 0xbf 0x83	GREEK SMALL LETTER ETA WITH PROSGEGRAMMENI
					break;
				case 0x1FD8:		//	á¿˜	0xe1 0xbf 0x98	GREEK CAPITAL LETTER IOTA WITH VRACHY
					*p = 0x1FD0;	//	á¿	0xe1 0xbf 0x90	GREEK SMALL LETTER IOTA WITH VRACHY
					break;
				case 0x1FD9:		//	á¿™	0xe1 0xbf 0x99	GREEK CAPITAL LETTER IOTA WITH MACRON
					*p = 0x1FD1;	//	á¿‘	0xe1 0xbf 0x91	GREEK SMALL LETTER IOTA WITH MACRON
					break;
				case 0x1FDA:		//	á¿š	0xe1 0xbf 0x9a	GREEK CAPITAL LETTER IOTA WITH VARIA
					*p = 0x1F76;	//	á½¶	0xe1 0xbd 0xb6	GREEK SMALL LETTER IOTA WITH VARIA
					break;
				case 0x1FDB:		//	á¿›	0xe1 0xbf 0x9b	GREEK CAPITAL LETTER IOTA WITH OXIA
					*p = 0x1F77;	//	á½·	0xe1 0xbd 0xb7	GREEK SMALL LETTER IOTA WITH OXIA
					break;
				case 0x1FE8:		//	á¿¨	0xe1 0xbf 0xa8	GREEK CAPITAL LETTER UPSILON WITH VRACHY
					*p = 0x1FE0;	//	á¿ 	0xe1 0xbf 0xa0	GREEK SMALL LETTER UPSILON WITH VRACHY
					break;
				case 0x1FE9:		//	á¿©	0xe1 0xbf 0xa9	GREEK CAPITAL LETTER UPSILON WITH MACRON
					*p = 0x1FE1;	//	á¿¡	0xe1 0xbf 0xa1	GREEK SMALL LETTER UPSILON WITH MACRON
					break;
				case 0x1FEA:		//	á¿ª	0xe1 0xbf 0xaa	GREEK CAPITAL LETTER UPSILON WITH VARIA
					*p = 0x1F7A;	//	á½º	0xe1 0xbd 0xba	GREEK SMALL LETTER UPSILON WITH VARIA
					break;
				case 0x1FEB:		//	á¿«	0xe1 0xbf 0xab	GREEK CAPITAL LETTER UPSILON WITH OXIA
					*p = 0x1F7B;	//	á½»	0xe1 0xbd 0xbb	GREEK SMALL LETTER UPSILON WITH OXIA
					break;
				case 0x1FEC:		//	á¿¬	0xe1 0xbf 0xac	GREEK CAPITAL LETTER RHO WITH DASIA
					*p = 0x1FE5;	//	á¿¥	0xe1 0xbf 0xa5	GREEK SMALL LETTER RHO WITH DASIA
					break;
				case 0x1FF8:		//	á¿¸	0xe1 0xbf 0xb8	GREEK CAPITAL LETTER OMICRON WITH VARIA
					*p = 0x1F78;	//	á½¸	0xe1 0xbd 0xb8	GREEK SMALL LETTER OMICRON WITH VARIA
					break;
				case 0x1FF9:		//	á¿¹	0xe1 0xbf 0xb9	GREEK CAPITAL LETTER OMICRON WITH OXIA
					*p = 0x1F79;	//	á½¹	0xe1 0xbd 0xb9	GREEK SMALL LETTER OMICRON WITH OXIA
					break;
				case 0x1FFA:		//	á¿º	0xe1 0xbf 0xba	GREEK CAPITAL LETTER OMEGA WITH VARIA
					*p = 0x1F7C;	//	á½¼	0xe1 0xbd 0xbc	GREEK SMALL LETTER OMEGA WITH VARIA
					break;
				case 0x1FFB:		//	á¿»	0xe1 0xbf 0xbb	GREEK CAPITAL LETTER OMEGA WITH OXIA
					*p = 0x1F7D;	//	á½½	0xe1 0xbd 0xbd	GREEK SMALL LETTER OMEGA WITH OXIA
					break;
				case 0x1FFC:		//	á¿¼	0xe1 0xbf 0xbc	GREEK CAPITAL LETTER OMEGA WITH PROSGEGRAMMENI
					*p = 0x1FF3;	//	á¿³	0xe1 0xbf 0xb3	GREEK SMALL LETTER OMEGA WITH PROSGEGRAMMENI
					break;
				case 0x2C00:		//	â°€	0xe2 0xb0 0x80	GLAGOLITIC CAPITAL LETTER AZU
					*p = 0x2C30;	//	â°°	0xe2 0xb0 0xb0	GLAGOLITIC SMALL LETTER AZU
					break;
				case 0x2C01:		//	â°	0xe2 0xb0 0x81	GLAGOLITIC CAPITAL LETTER BUKY
					*p = 0x2C31;	//	â°±	0xe2 0xb0 0xb1	GLAGOLITIC SMALL LETTER BUKY
					break;
				case 0x2C02:		//	â°‚	0xe2 0xb0 0x82	GLAGOLITIC CAPITAL LETTER VEDE
					*p = 0x2C32;	//	â°²	0xe2 0xb0 0xb2	GLAGOLITIC SMALL LETTER VEDE
					break;
				case 0x2C03:		//	â°ƒ	0xe2 0xb0 0x83	GLAGOLITIC CAPITAL LETTER GLAGOLI
					*p = 0x2C33;	//	â°³	0xe2 0xb0 0xb3	GLAGOLITIC SMALL LETTER GLAGOLI
					break;
				case 0x2C04:		//	â°„	0xe2 0xb0 0x84	GLAGOLITIC CAPITAL LETTER DOBRO
					*p = 0x2C34;	//	â°´	0xe2 0xb0 0xb4	GLAGOLITIC SMALL LETTER DOBRO
					break;
				case 0x2C05:		//	â°…	0xe2 0xb0 0x85	GLAGOLITIC CAPITAL LETTER YESTU
					*p = 0x2C35;	//	â°µ	0xe2 0xb0 0xb5	GLAGOLITIC SMALL LETTER YESTU
					break;
				case 0x2C06:		//	â°†	0xe2 0xb0 0x86	GLAGOLITIC CAPITAL LETTER ZHIVETE
					*p = 0x2C36;	//	â°¶	0xe2 0xb0 0xb6	GLAGOLITIC SMALL LETTER ZHIVETE
					break;
				case 0x2C07:		//	â°‡	0xe2 0xb0 0x87	GLAGOLITIC CAPITAL LETTER DZELO
					*p = 0x2C37;	//	â°·	0xe2 0xb0 0xb7	GLAGOLITIC SMALL LETTER DZELO
					break;
				case 0x2C08:		//	â°ˆ	0xe2 0xb0 0x88	GLAGOLITIC CAPITAL LETTER ZEMLJA
					*p = 0x2C38;	//	â°¸	0xe2 0xb0 0xb8	GLAGOLITIC SMALL LETTER ZEMLJA
					break;
				case 0x2C09:		//	â°‰	0xe2 0xb0 0x89	GLAGOLITIC CAPITAL LETTER IZHE
					*p = 0x2C39;	//	â°¹	0xe2 0xb0 0xb9	GLAGOLITIC SMALL LETTER IZHE
					break;
				case 0x2C0A:		//	â°Š	0xe2 0xb0 0x8a	GLAGOLITIC CAPITAL LETTER INITIAL IZHE
					*p = 0x2C3A;	//	â°º	0xe2 0xb0 0xba	GLAGOLITIC SMALL LETTER INITIAL IZHE
					break;
				case 0x2C0B:		//	â°‹	0xe2 0xb0 0x8b	GLAGOLITIC CAPITAL LETTER I
					*p = 0x2C3B;	//	â°»	0xe2 0xb0 0xbb	GLAGOLITIC SMALL LETTER I
					break;
				case 0x2C0C:		//	â°Œ	0xe2 0xb0 0x8c	GLAGOLITIC CAPITAL LETTER DJERVI
					*p = 0x2C3C;	//	â°¼	0xe2 0xb0 0xbc	GLAGOLITIC SMALL LETTER DJERVI
					break;
				case 0x2C0D:		//	â°	0xe2 0xb0 0x8d	GLAGOLITIC CAPITAL LETTER KAKO
					*p = 0x2C3D;	//	â°½	0xe2 0xb0 0xbd	GLAGOLITIC SMALL LETTER KAKO
					break;
				case 0x2C0E:		//	â°Ž	0xe2 0xb0 0x8e	GLAGOLITIC CAPITAL LETTER LJUDIJE
					*p = 0x2C3E;	//	â°¾	0xe2 0xb0 0xbe	GLAGOLITIC SMALL LETTER LJUDIJE
					break;
				case 0x2C0F:		//	â°	0xe2 0xb0 0x8f	GLAGOLITIC CAPITAL LETTER MYSLITE
					*p = 0x2C3F;	//	â°¿	0xe2 0xb0 0xbf	GLAGOLITIC SMALL LETTER MYSLITE
					break;
				case 0x2C10:		//	â°	0xe2 0xb0 0x90	GLAGOLITIC CAPITAL LETTER NASHI
					*p = 0x2C40;	//	â±€	0xe2 0xb1 0x80	GLAGOLITIC SMALL LETTER NASHI
					break;
				case 0x2C11:		//	â°‘	0xe2 0xb0 0x91	GLAGOLITIC CAPITAL LETTER ONU
					*p = 0x2C41;	//	â±	0xe2 0xb1 0x81	GLAGOLITIC SMALL LETTER ONU
					break;
				case 0x2C12:		//	â°’	0xe2 0xb0 0x92	GLAGOLITIC CAPITAL LETTER POKOJI
					*p = 0x2C42;	//	â±‚	0xe2 0xb1 0x82	GLAGOLITIC SMALL LETTER POKOJI
					break;
				case 0x2C13:		//	â°“	0xe2 0xb0 0x93	GLAGOLITIC CAPITAL LETTER RITSI
					*p = 0x2C43;	//	â±ƒ	0xe2 0xb1 0x83	GLAGOLITIC SMALL LETTER RITSI
					break;
				case 0x2C14:		//	â°”	0xe2 0xb0 0x94	GLAGOLITIC CAPITAL LETTER SLOVO
					*p = 0x2C44;	//	â±„	0xe2 0xb1 0x84	GLAGOLITIC SMALL LETTER SLOVO
					break;
				case 0x2C15:		//	â°•	0xe2 0xb0 0x95	GLAGOLITIC CAPITAL LETTER TVRIDO
					*p = 0x2C45;	//	â±…	0xe2 0xb1 0x85	GLAGOLITIC SMALL LETTER TVRIDO
					break;
				case 0x2C16:		//	â°–	0xe2 0xb0 0x96	GLAGOLITIC CAPITAL LETTER UKU
					*p = 0x2C46;	//	â±†	0xe2 0xb1 0x86	GLAGOLITIC SMALL LETTER UKU
					break;
				case 0x2C17:		//	â°—	0xe2 0xb0 0x97	GLAGOLITIC CAPITAL LETTER FRITU
					*p = 0x2C47;	//	â±‡	0xe2 0xb1 0x87	GLAGOLITIC SMALL LETTER FRITU
					break;
				case 0x2C18:		//	â°˜	0xe2 0xb0 0x98	GLAGOLITIC CAPITAL LETTER HERU
					*p = 0x2C48;	//	â±ˆ	0xe2 0xb1 0x88	GLAGOLITIC SMALL LETTER HERU
					break;
				case 0x2C19:		//	â°™	0xe2 0xb0 0x99	GLAGOLITIC CAPITAL LETTER OTU
					*p = 0x2C49;	//	â±‰	0xe2 0xb1 0x89	GLAGOLITIC SMALL LETTER OTU
					break;
				case 0x2C1A:		//	â°š	0xe2 0xb0 0x9a	GLAGOLITIC CAPITAL LETTER PE
					*p = 0x2C4A;	//	â±Š	0xe2 0xb1 0x8a	GLAGOLITIC SMALL LETTER PE
					break;
				case 0x2C1B:		//	â°›	0xe2 0xb0 0x9b	GLAGOLITIC CAPITAL LETTER SHTA
					*p = 0x2C4B;	//	â±‹	0xe2 0xb1 0x8b	GLAGOLITIC SMALL LETTER SHTA
					break;
				case 0x2C1C:		//	â°œ	0xe2 0xb0 0x9c	GLAGOLITIC CAPITAL LETTER TSI
					*p = 0x2C4C;	//	â±Œ	0xe2 0xb1 0x8c	GLAGOLITIC SMALL LETTER TSI
					break;
				case 0x2C1D:		//	â°	0xe2 0xb0 0x9d	GLAGOLITIC CAPITAL LETTER CHRIVI
					*p = 0x2C4D;	//	â±	0xe2 0xb1 0x8d	GLAGOLITIC SMALL LETTER CHRIVI
					break;
				case 0x2C1E:		//	â°ž	0xe2 0xb0 0x9e	GLAGOLITIC CAPITAL LETTER SHA
					*p = 0x2C4E;	//	â±Ž	0xe2 0xb1 0x8e	GLAGOLITIC SMALL LETTER SHA
					break;
				case 0x2C1F:		//	â°Ÿ	0xe2 0xb0 0x9f	GLAGOLITIC CAPITAL LETTER YERU
					*p = 0x2C4F;	//	â±	0xe2 0xb1 0x8f	GLAGOLITIC SMALL LETTER YERU
					break;
				case 0x2C20:		//	â° 	0xe2 0xb0 0xa0	GLAGOLITIC CAPITAL LETTER YERI
					*p = 0x2C50;	//	â±	0xe2 0xb1 0x90	GLAGOLITIC SMALL LETTER YERI
					break;
				case 0x2C21:		//	â°¡	0xe2 0xb0 0xa1	GLAGOLITIC CAPITAL LETTER YATI
					*p = 0x2C51;	//	â±‘	0xe2 0xb1 0x91	GLAGOLITIC SMALL LETTER YATI
					break;
				case 0x2C22:		//	â°¢	0xe2 0xb0 0xa2	GLAGOLITIC CAPITAL LETTER SPIDERY HA
					*p = 0x2C52;	//	â±’	0xe2 0xb1 0x92	GLAGOLITIC SMALL LETTER SPIDERY HA
					break;
				case 0x2C23:		//	â°£	0xe2 0xb0 0xa3	GLAGOLITIC CAPITAL LETTER YU
					*p = 0x2C53;	//	â±“	0xe2 0xb1 0x93	GLAGOLITIC SMALL LETTER YU
					break;
				case 0x2C24:		//	â°¤	0xe2 0xb0 0xa4	GLAGOLITIC CAPITAL LETTER SMALL YUS
					*p = 0x2C54;	//	â±”	0xe2 0xb1 0x94	GLAGOLITIC SMALL LETTER SMALL YUS
					break;
				case 0x2C25:		//	â°¥	0xe2 0xb0 0xa5	GLAGOLITIC CAPITAL LETTER SMALL YUS WITH TAIL
					*p = 0x2C55;	//	â±•	0xe2 0xb1 0x95	GLAGOLITIC SMALL LETTER SMALL YUS WITH TAIL
					break;
				case 0x2C26:		//	â°¦	0xe2 0xb0 0xa6	GLAGOLITIC CAPITAL LETTER YO
					*p = 0x2C56;	//	â±–	0xe2 0xb1 0x96	GLAGOLITIC SMALL LETTER YO
					break;
				case 0x2C27:		//	â°§	0xe2 0xb0 0xa7	GLAGOLITIC CAPITAL LETTER IOTATED SMALL YUS
					*p = 0x2C57;	//	â±—	0xe2 0xb1 0x97	GLAGOLITIC SMALL LETTER IOTATED SMALL YUS
					break;
				case 0x2C28:		//	â°¨	0xe2 0xb0 0xa8	GLAGOLITIC CAPITAL LETTER BIG YUS
					*p = 0x2C58;	//	â±˜	0xe2 0xb1 0x98	GLAGOLITIC SMALL LETTER BIG YUS
					break;
				case 0x2C29:		//	â°©	0xe2 0xb0 0xa9	GLAGOLITIC CAPITAL LETTER IOTATED BIG YUS
					*p = 0x2C59;	//	â±™	0xe2 0xb1 0x99	GLAGOLITIC SMALL LETTER IOTATED BIG YUS
					break;
				case 0x2C2A:		//	â°ª	0xe2 0xb0 0xaa	GLAGOLITIC CAPITAL LETTER FITA
					*p = 0x2C5A;	//	â±š	0xe2 0xb1 0x9a	GLAGOLITIC SMALL LETTER FITA
					break;
				case 0x2C2B:		//	â°«	0xe2 0xb0 0xab	GLAGOLITIC CAPITAL LETTER IZHITSA
					*p = 0x2C5B;	//	â±›	0xe2 0xb1 0x9b	GLAGOLITIC SMALL LETTER IZHITSA
					break;
				case 0x2C2C:		//	â°¬	0xe2 0xb0 0xac	GLAGOLITIC CAPITAL LETTER SHTAPIC
					*p = 0x2C5C;	//	â±œ	0xe2 0xb1 0x9c	GLAGOLITIC SMALL LETTER SHTAPIC
					break;
				case 0x2C2D:		//	â°­	0xe2 0xb0 0xad	GLAGOLITIC CAPITAL LETTER TROKUTASTI A
					*p = 0x2C5D;	//	â±	0xe2 0xb1 0x9d	GLAGOLITIC SMALL LETTER TROKUTASTI A
					break;
				case 0x2C2E:		//	â°®	0xe2 0xb0 0xae	GLAGOLITIC CAPITAL LETTER LATINATE MYSLITE
					*p = 0x2C5E;	//	â±ž	0xe2 0xb1 0x9e	GLAGOLITIC SMALL LETTER LATINATE MYSLITE
					break;
				case 0x2C60:		//	â± 	0xe2 0xb1 0xa0	LATIN CAPITAL LETTER L WITH DOUBLE BAR
					*p = 0x2C61;	//	â±¡	0xe2 0xb1 0xa1	LATIN SMALL LETTER L WITH DOUBLE BAR
					break;
				case 0x2C62:		//	â±¢	0xe2 0xb1 0xa2	LATIN CAPITAL LETTER L WITH MIDDLE TILDE
					*p = 0x026B;	//	É«	0xc9 0xab	LATIN SMALL LETTER L WITH MIDDLE TILDE
					break;
				case 0x2C63:		//	â±£	0xe2 0xb1 0xa3	LATIN CAPITAL LETTER P WITH STROKE
					*p = 0x1D7D;	//	áµ½	0xe1 0xb5 0xbd	LATIN SMALL LETTER P WITH STROKE
					break;
				case 0x2C64:		//	â±¤	0xe2 0xb1 0xa4	LATIN CAPITAL LETTER R WITH TAIL
					*p = 0x027D;	//	É½	0xc9 0xbd	LATIN SMALL LETTER R WITH TAIL
					break;
				case 0x2C67:		//	â±§	0xe2 0xb1 0xa7	LATIN CAPITAL LETTER H WITH DESCENDER
					*p = 0x2C68;	//	â±¨	0xe2 0xb1 0xa8	LATIN SMALL LETTER H WITH DESCENDER
					break;
				case 0x2C69:		//	â±©	0xe2 0xb1 0xa9	LATIN CAPITAL LETTER K WITH DESCENDER
					*p = 0x2C6A;	//	â±ª	0xe2 0xb1 0xaa	LATIN SMALL LETTER K WITH DESCENDER
					break;
				case 0x2C6B:		//	â±«	0xe2 0xb1 0xab	LATIN CAPITAL LETTER Z WITH DESCENDER
					*p = 0x2C6C;	//	â±¬	0xe2 0xb1 0xac	LATIN SMALL LETTER Z WITH DESCENDER
					break;
				case 0x2C6D:		//	â±­	0xe2 0xb1 0xad	LATIN CAPITAL LETTER ALPHA
					*p = 0x0251;	//	É‘	0xc9 0x91	LATIN SMALL LETTER ALPHA
					break;
				case 0x2C6E:		//	â±®	0xe2 0xb1 0xae	LATIN CAPITAL LETTER M WITH HOOK
					*p = 0x0271;	//	É±	0xc9 0xb1	LATIN SMALL LETTER M WITH HOOK
					break;
				case 0x2C6F:		//	â±¯	0xe2 0xb1 0xaf	LATIN CAPITAL LETTER TURNED A
					*p = 0x0250;	//	É	0xc9 0x90	LATIN SMALL LETTER TURNED A
					break;
				case 0x2C70:		//	â±°	0xe2 0xb1 0xb0	LATIN CAPITAL LETTER TURNED ALPHA
					*p = 0x0252;	//	É’	0xc9 0x92	LATIN SMALL LETTER TURNED ALPHA
					break;
				case 0x2C72:		//	â±²	0xe2 0xb1 0xb2	LATIN CAPITAL LETTER W WITH HOOK
					*p = 0x2C73;	//	â±³	0xe2 0xb1 0xb3	LATIN SMALL LETTER W WITH HOOK
					break;
				case 0x2C75:		//	â±µ	0xe2 0xb1 0xb5	LATIN CAPITAL LETTER HALF H
					*p = 0x2C76;	//	â±¶	0xe2 0xb1 0xb6	LATIN SMALL LETTER HALF H
					break;
				case 0x2C7E:		//	â±¾	0xe2 0xb1 0xbe	LATIN CAPITAL LETTER S WITH SWASH TAIL
					*p = 0x023F;	//	È¿	0xc8 0xbf	LATIN SMALL LETTER S WITH SWASH TAIL
					break;
				case 0x2C7F:		//	â±¿	0xe2 0xb1 0xbf	LATIN CAPITAL LETTER Z WITH SWASH TAIL
					*p = 0x0240;	//	É€	0xc9 0x80	LATIN SMALL LETTER Z WITH SWASH TAIL
					break;
				case 0x2C80:		//	â²€	0xe2 0xb2 0x80	COPTIC CAPITAL LETTER ALFA
					*p = 0x2C81;	//	â²	0xe2 0xb2 0x81	COPTIC SMALL LETTER ALFA
					break;
				case 0x2C82:		//	â²‚	0xe2 0xb2 0x82	COPTIC CAPITAL LETTER VIDA
					*p = 0x2C83;	//	â²ƒ	0xe2 0xb2 0x83	COPTIC SMALL LETTER VIDA
					break;
				case 0x2C84:		//	â²„	0xe2 0xb2 0x84	COPTIC CAPITAL LETTER GAMMA
					*p = 0x2C85;	//	â²…	0xe2 0xb2 0x85	COPTIC SMALL LETTER GAMMA
					break;
				case 0x2C86:		//	â²†	0xe2 0xb2 0x86	COPTIC CAPITAL LETTER DALDA
					*p = 0x2C87;	//	â²‡	0xe2 0xb2 0x87	COPTIC SMALL LETTER DALDA
					break;
				case 0x2C88:		//	â²ˆ	0xe2 0xb2 0x88	COPTIC CAPITAL LETTER EIE
					*p = 0x2C89;	//	â²‰	0xe2 0xb2 0x89	COPTIC SMALL LETTER EIE
					break;
				case 0x2C8A:		//	â²Š	0xe2 0xb2 0x8a	COPTIC CAPITAL LETTER SOU
					*p = 0x2C8B;	//	â²‹	0xe2 0xb2 0x8b	COPTIC SMALL LETTER SOU
					break;
				case 0x2C8C:		//	â²Œ	0xe2 0xb2 0x8c	COPTIC CAPITAL LETTER ZATA
					*p = 0x2C8D;	//	â²	0xe2 0xb2 0x8d	COPTIC SMALL LETTER ZATA
					break;
				case 0x2C8E:		//	â²Ž	0xe2 0xb2 0x8e	COPTIC CAPITAL LETTER HATE
					*p = 0x2C8F;	//	â²	0xe2 0xb2 0x8f	COPTIC SMALL LETTER HATE
					break;
				case 0x2C90:		//	â²	0xe2 0xb2 0x90	COPTIC CAPITAL LETTER THETHE
					*p = 0x2C91;	//	â²‘	0xe2 0xb2 0x91	COPTIC SMALL LETTER THETHE
					break;
				case 0x2C92:		//	â²’	0xe2 0xb2 0x92	COPTIC CAPITAL LETTER IAUDA
					*p = 0x2C93;	//	â²“	0xe2 0xb2 0x93	COPTIC SMALL LETTER IAUDA
					break;
				case 0x2C94:		//	â²”	0xe2 0xb2 0x94	COPTIC CAPITAL LETTER KAPA
					*p = 0x2C95;	//	â²•	0xe2 0xb2 0x95	COPTIC SMALL LETTER KAPA
					break;
				case 0x2C96:		//	â²–	0xe2 0xb2 0x96	COPTIC CAPITAL LETTER LAULA
					*p = 0x2C97;	//	â²—	0xe2 0xb2 0x97	COPTIC SMALL LETTER LAULA
					break;
				case 0x2C98:		//	â²˜	0xe2 0xb2 0x98	COPTIC CAPITAL LETTER MI
					*p = 0x2C99;	//	â²™	0xe2 0xb2 0x99	COPTIC SMALL LETTER MI
					break;
				case 0x2C9A:		//	â²š	0xe2 0xb2 0x9a	COPTIC CAPITAL LETTER NI
					*p = 0x2C9B;	//	â²›	0xe2 0xb2 0x9b	COPTIC SMALL LETTER NI
					break;
				case 0x2C9C:		//	â²œ	0xe2 0xb2 0x9c	COPTIC CAPITAL LETTER KSI
					*p = 0x2C9D;	//	â²	0xe2 0xb2 0x9d	COPTIC SMALL LETTER KSI
					break;
				case 0x2C9E:		//	â²ž	0xe2 0xb2 0x9e	COPTIC CAPITAL LETTER O
					*p = 0x2C9F;	//	â²Ÿ	0xe2 0xb2 0x9f	COPTIC SMALL LETTER O
					break;
				case 0x2CA0:		//	â² 	0xe2 0xb2 0xa0	COPTIC CAPITAL LETTER PI
					*p = 0x2CA1;	//	â²¡	0xe2 0xb2 0xa1	COPTIC SMALL LETTER PI
					break;
				case 0x2CA2:		//	â²¢	0xe2 0xb2 0xa2	COPTIC CAPITAL LETTER RO
					*p = 0x2CA3;	//	â²£	0xe2 0xb2 0xa3	COPTIC SMALL LETTER RO
					break;
				case 0x2CA4:		//	â²¤	0xe2 0xb2 0xa4	COPTIC CAPITAL LETTER SIMA
					*p = 0x2CA5;	//	â²¥	0xe2 0xb2 0xa5	COPTIC SMALL LETTER SIMA
					break;
				case 0x2CA6:		//	â²¦	0xe2 0xb2 0xa6	COPTIC CAPITAL LETTER TAU
					*p = 0x2CA7;	//	â²§	0xe2 0xb2 0xa7	COPTIC SMALL LETTER TAU
					break;
				case 0x2CA8:		//	â²¨	0xe2 0xb2 0xa8	COPTIC CAPITAL LETTER UA
					*p = 0x2CA9;	//	â²©	0xe2 0xb2 0xa9	COPTIC SMALL LETTER UA
					break;
				case 0x2CAA:		//	â²ª	0xe2 0xb2 0xaa	COPTIC CAPITAL LETTER FI
					*p = 0x2CAB;	//	â²«	0xe2 0xb2 0xab	COPTIC SMALL LETTER FI
					break;
				case 0x2CAC:		//	â²¬	0xe2 0xb2 0xac	COPTIC CAPITAL LETTER KHI
					*p = 0x2CAD;	//	â²­	0xe2 0xb2 0xad	COPTIC SMALL LETTER KHI
					break;
				case 0x2CAE:		//	â²®	0xe2 0xb2 0xae	COPTIC CAPITAL LETTER PSI
					*p = 0x2CAF;	//	â²¯	0xe2 0xb2 0xaf	COPTIC SMALL LETTER PSI
					break;
				case 0x2CB0:		//	â²°	0xe2 0xb2 0xb0	COPTIC CAPITAL LETTER OOU
					*p = 0x2CB1;	//	â²±	0xe2 0xb2 0xb1	COPTIC SMALL LETTER OOU
					break;
				case 0x2CB2:		//	â²²	0xe2 0xb2 0xb2	COPTIC CAPITAL LETTER DIALECT-P ALEF
					*p = 0x2CB3;	//	â²³	0xe2 0xb2 0xb3	COPTIC SMALL LETTER DIALECT-P ALEF
					break;
				case 0x2CB4:		//	â²´	0xe2 0xb2 0xb4	COPTIC CAPITAL LETTER OLD COPTIC AIN
					*p = 0x2CB5;	//	â²µ	0xe2 0xb2 0xb5	COPTIC SMALL LETTER OLD COPTIC AIN
					break;
				case 0x2CB6:		//	â²¶	0xe2 0xb2 0xb6	COPTIC CAPITAL LETTER CRYPTOGRAMMIC EIE
					*p = 0x2CB7;	//	â²·	0xe2 0xb2 0xb7	COPTIC SMALL LETTER CRYPTOGRAMMIC EIE
					break;
				case 0x2CB8:		//	â²¸	0xe2 0xb2 0xb8	COPTIC CAPITAL LETTER DIALECT-P KAPA
					*p = 0x2CB9;	//	â²¹	0xe2 0xb2 0xb9	COPTIC SMALL LETTER DIALECT-P KAPA
					break;
				case 0x2CBA:		//	â²º	0xe2 0xb2 0xba	COPTIC CAPITAL LETTER DIALECT-P NI
					*p = 0x2CBB;	//	â²»	0xe2 0xb2 0xbb	COPTIC SMALL LETTER DIALECT-P NI
					break;
				case 0x2CBC:		//	â²¼	0xe2 0xb2 0xbc	COPTIC CAPITAL LETTER CRYPTOGRAMMIC NI
					*p = 0x2CBD;	//	â²½	0xe2 0xb2 0xbd	COPTIC SMALL LETTER CRYPTOGRAMMIC NI
					break;
				case 0x2CBE:		//	â²¾	0xe2 0xb2 0xbe	COPTIC CAPITAL LETTER OLD COPTIC OOU
					*p = 0x2CBF;	//	â²¿	0xe2 0xb2 0xbf	COPTIC SMALL LETTER OLD COPTIC OOU
					break;
				case 0x2CC0:		//	â³€	0xe2 0xb3 0x80	COPTIC CAPITAL LETTER SAMPI
					*p = 0x2CC1;	//	â³	0xe2 0xb3 0x81	COPTIC SMALL LETTER SAMPI
					break;
				case 0x2CC2:		//	â³‚	0xe2 0xb3 0x82	COPTIC CAPITAL LETTER CROSSED SHEI
					*p = 0x2CC3;	//	â³ƒ	0xe2 0xb3 0x83	COPTIC SMALL LETTER CROSSED SHEI
					break;
				case 0x2CC4:		//	â³„	0xe2 0xb3 0x84	COPTIC CAPITAL LETTER OLD COPTIC SHEI
					*p = 0x2CC5;	//	â³…	0xe2 0xb3 0x85	COPTIC SMALL LETTER OLD COPTIC SHEI
					break;
				case 0x2CC6:		//	â³†	0xe2 0xb3 0x86	COPTIC CAPITAL LETTER OLD COPTIC ESH
					*p = 0x2CC7;	//	â³‡	0xe2 0xb3 0x87	COPTIC SMALL LETTER OLD COPTIC ESH
					break;
				case 0x2CC8:		//	â³ˆ	0xe2 0xb3 0x88	COPTIC CAPITAL LETTER AKHMIMIC KHEI
					*p = 0x2CC9;	//	â³‰	0xe2 0xb3 0x89	COPTIC SMALL LETTER AKHMIMIC KHEI
					break;
				case 0x2CCA:		//	â³Š	0xe2 0xb3 0x8a	COPTIC CAPITAL LETTER DIALECT-P HORI
					*p = 0x2CCB;	//	â³‹	0xe2 0xb3 0x8b	COPTIC SMALL LETTER DIALECT-P HORI
					break;
				case 0x2CCC:		//	â³Œ	0xe2 0xb3 0x8c	COPTIC CAPITAL LETTER OLD COPTIC HORI
					*p = 0x2CCD;	//	â³	0xe2 0xb3 0x8d	COPTIC SMALL LETTER OLD COPTIC HORI
					break;
				case 0x2CCE:		//	â³Ž	0xe2 0xb3 0x8e	COPTIC CAPITAL LETTER OLD COPTIC HA
					*p = 0x2CCF;	//	â³	0xe2 0xb3 0x8f	COPTIC SMALL LETTER OLD COPTIC HA
					break;
				case 0x2CD0:		//	â³	0xe2 0xb3 0x90	COPTIC CAPITAL LETTER L-SHAPED HA
					*p = 0x2CD1;	//	â³‘	0xe2 0xb3 0x91	COPTIC SMALL LETTER L-SHAPED HA
					break;
				case 0x2CD2:		//	â³’	0xe2 0xb3 0x92	COPTIC CAPITAL LETTER OLD COPTIC HEI
					*p = 0x2CD3;	//	â³“	0xe2 0xb3 0x93	COPTIC SMALL LETTER OLD COPTIC HEI
					break;
				case 0x2CD4:		//	â³”	0xe2 0xb3 0x94	COPTIC CAPITAL LETTER OLD COPTIC HAT
					*p = 0x2CD5;	//	â³•	0xe2 0xb3 0x95	COPTIC SMALL LETTER OLD COPTIC HAT
					break;
				case 0x2CD6:		//	â³–	0xe2 0xb3 0x96	COPTIC CAPITAL LETTER OLD COPTIC GANGIA
					*p = 0x2CD7;	//	â³—	0xe2 0xb3 0x97	COPTIC SMALL LETTER OLD COPTIC GANGIA
					break;
				case 0x2CD8:		//	â³˜	0xe2 0xb3 0x98	COPTIC CAPITAL LETTER OLD COPTIC DJA
					*p = 0x2CD9;	//	â³™	0xe2 0xb3 0x99	COPTIC SMALL LETTER OLD COPTIC DJA
					break;
				case 0x2CDA:		//	â³š	0xe2 0xb3 0x9a	COPTIC CAPITAL LETTER OLD COPTIC SHIMA
					*p = 0x2CDB;	//	â³›	0xe2 0xb3 0x9b	COPTIC SMALL LETTER OLD COPTIC SHIMA
					break;
				case 0x2CDC:		//	â³œ	0xe2 0xb3 0x9c	COPTIC CAPITAL LETTER OLD NUBIAN SHIMA
					*p = 0x2CDD;	//	â³	0xe2 0xb3 0x9d	COPTIC SMALL LETTER OLD NUBIAN SHIMA
					break;
				case 0x2CDE:		//	â³ž	0xe2 0xb3 0x9e	COPTIC CAPITAL LETTER OLD NUBIAN NGI
					*p = 0x2CDF;	//	â³Ÿ	0xe2 0xb3 0x9f	COPTIC SMALL LETTER OLD NUBIAN NGI
					break;
				case 0x2CE0:		//	â³ 	0xe2 0xb3 0xa0	COPTIC CAPITAL LETTER OLD NUBIAN NYI
					*p = 0x2CE1;	//	â³¡	0xe2 0xb3 0xa1	COPTIC SMALL LETTER OLD NUBIAN NYI
					break;
				case 0x2CE2:		//	â³¢	0xe2 0xb3 0xa2	COPTIC CAPITAL LETTER OLD NUBIAN WAU
					*p = 0x2CE3;	//	â³£	0xe2 0xb3 0xa3	COPTIC SMALL LETTER OLD NUBIAN WAU
					break;
				case 0x2CEB:		//	â³«	0xe2 0xb3 0xab	COPTIC CAPITAL LETTER CRYPTOGRAMMIC SHEI
					*p = 0x2CEC;	//	â³¬	0xe2 0xb3 0xac	COPTIC SMALL LETTER CRYPTOGRAMMIC SHEI
					break;
				case 0x2CED:		//	â³­	0xe2 0xb3 0xad	COPTIC CAPITAL LETTER CRYPTOGRAMMIC GANGIA
					*p = 0x2CEE;	//	â³®	0xe2 0xb3 0xae	COPTIC SMALL LETTER CRYPTOGRAMMIC GANGIA
					break;
				case 0x2CF2:		//	â³²	0xe2 0xb3 0xb2	COPTIC CAPITAL LETTER BOHAIRIC KHEI
					*p = 0x2CF3;	//	â³³	0xe2 0xb3 0xb3	COPTIC SMALL LETTER BOHAIRIC KHEI
					break;
				case 0x2D00:		//	â´€	0xe2 0xb4 0x80	GEORGIAN LETTER AN
					*p = 0x10D0;	//	áƒ	0xe1 0x83 0x90	GEORGIAN SMALL LETTER AN
					break;
				case 0x2D01:		//	â´	0xe2 0xb4 0x81	GEORGIAN LETTER BAN
					*p = 0x10D1;	//	áƒ‘	0xe1 0x83 0x91	GEORGIAN SMALL LETTER BAN
					break;
				case 0x2D02:		//	â´‚	0xe2 0xb4 0x82	GEORGIAN LETTER GAN
					*p = 0x10D2;	//	áƒ’	0xe1 0x83 0x92	GEORGIAN SMALL LETTER GAN
					break;
				case 0x2D03:		//	â´ƒ	0xe2 0xb4 0x83	GEORGIAN LETTER DON
					*p = 0x10D3;	//	áƒ“	0xe1 0x83 0x93	GEORGIAN SMALL LETTER DON
					break;
				case 0x2D04:		//	â´„	0xe2 0xb4 0x84	GEORGIAN LETTER EN
					*p = 0x10D4;	//	áƒ”	0xe1 0x83 0x94	GEORGIAN SMALL LETTER EN
					break;
				case 0x2D05:		//	â´…	0xe2 0xb4 0x85	GEORGIAN LETTER VIN
					*p = 0x10D5;	//	áƒ•	0xe1 0x83 0x95	GEORGIAN SMALL LETTER VIN
					break;
				case 0x2D06:		//	â´†	0xe2 0xb4 0x86	GEORGIAN LETTER ZEN
					*p = 0x10D6;	//	áƒ–	0xe1 0x83 0x96	GEORGIAN SMALL LETTER ZEN
					break;
				case 0x2D07:		//	â´‡	0xe2 0xb4 0x87	GEORGIAN LETTER TAN
					*p = 0x10D7;	//	áƒ—	0xe1 0x83 0x97	GEORGIAN SMALL LETTER TAN
					break;
				case 0x2D08:		//	â´ˆ	0xe2 0xb4 0x88	GEORGIAN LETTER IN
					*p = 0x10D8;	//	áƒ˜	0xe1 0x83 0x98	GEORGIAN SMALL LETTER IN
					break;
				case 0x2D09:		//	â´‰	0xe2 0xb4 0x89	GEORGIAN LETTER KAN
					*p = 0x10D9;	//	áƒ™	0xe1 0x83 0x99	GEORGIAN SMALL LETTER KAN
					break;
				case 0x2D0A:		//	â´Š	0xe2 0xb4 0x8a	GEORGIAN LETTER LAS
					*p = 0x10DA;	//	áƒš	0xe1 0x83 0x9a	GEORGIAN SMALL LETTER LAS
					break;
				case 0x2D0B:		//	â´‹	0xe2 0xb4 0x8b	GEORGIAN LETTER MAN
					*p = 0x10DB;	//	áƒ›	0xe1 0x83 0x9b	GEORGIAN SMALL LETTER MAN
					break;
				case 0x2D0C:		//	â´Œ	0xe2 0xb4 0x8c	GEORGIAN LETTER NAR
					*p = 0x10DC;	//	áƒœ	0xe1 0x83 0x9c	GEORGIAN SMALL LETTER NAR
					break;
				case 0x2D0D:		//	â´	0xe2 0xb4 0x8d	GEORGIAN LETTER ON
					*p = 0x10DD;	//	áƒ	0xe1 0x83 0x9d	GEORGIAN SMALL LETTER ON
					break;
				case 0x2D0E:		//	â´Ž	0xe2 0xb4 0x8e	GEORGIAN LETTER PAR
					*p = 0x10DE;	//	áƒž	0xe1 0x83 0x9e	GEORGIAN SMALL LETTER PAR
					break;
				case 0x2D0F:		//	â´	0xe2 0xb4 0x8f	GEORGIAN LETTER ZHAR
					*p = 0x10DF;	//	áƒŸ	0xe1 0x83 0x9f	GEORGIAN SMALL LETTER ZHAR
					break;
				case 0x2D10:		//	â´	0xe2 0xb4 0x90	GEORGIAN LETTER RAE
					*p = 0x10E0;	//	áƒ 	0xe1 0x83 0xa0	GEORGIAN SMALL LETTER RAE
					break;
				case 0x2D11:		//	â´‘	0xe2 0xb4 0x91	GEORGIAN LETTER SAN
					*p = 0x10E1;	//	áƒ¡	0xe1 0x83 0xa1	GEORGIAN SMALL LETTER SAN
					break;
				case 0x2D12:		//	â´’	0xe2 0xb4 0x92	GEORGIAN LETTER TAR
					*p = 0x10E2;	//	áƒ¢	0xe1 0x83 0xa2	GEORGIAN SMALL LETTER TAR
					break;
				case 0x2D13:		//	â´“	0xe2 0xb4 0x93	GEORGIAN LETTER UN
					*p = 0x10E3;	//	áƒ£	0xe1 0x83 0xa3	GEORGIAN SMALL LETTER UN
					break;
				case 0x2D14:		//	â´”	0xe2 0xb4 0x94	GEORGIAN LETTER PHAR
					*p = 0x10E4;	//	áƒ¤	0xe1 0x83 0xa4	GEORGIAN SMALL LETTER PHAR
					break;
				case 0x2D15:		//	â´•	0xe2 0xb4 0x95	GEORGIAN LETTER KHAR
					*p = 0x10E5;	//	áƒ¥	0xe1 0x83 0xa5	GEORGIAN SMALL LETTER KHAR
					break;
				case 0x2D16:		//	â´–	0xe2 0xb4 0x96	GEORGIAN LETTER GHAN
					*p = 0x10E6;	//	áƒ¦	0xe1 0x83 0xa6	GEORGIAN SMALL LETTER GHAN
					break;
				case 0x2D17:		//	â´—	0xe2 0xb4 0x97	GEORGIAN LETTER QAR
					*p = 0x10E7;	//	áƒ§	0xe1 0x83 0xa7	GEORGIAN SMALL LETTER QAR
					break;
				case 0x2D18:		//	â´˜	0xe2 0xb4 0x98	GEORGIAN LETTER SHIN
					*p = 0x10E8;	//	áƒ¨	0xe1 0x83 0xa8	GEORGIAN SMALL LETTER SHIN
					break;
				case 0x2D19:		//	â´™	0xe2 0xb4 0x99	GEORGIAN LETTER CHIN
					*p = 0x10E9;	//	áƒ©	0xe1 0x83 0xa9	GEORGIAN SMALL LETTER CHIN
					break;
				case 0x2D1A:		//	â´š	0xe2 0xb4 0x9a	GEORGIAN LETTER CAN
					*p = 0x10EA;	//	áƒª	0xe1 0x83 0xaa	GEORGIAN SMALL LETTER CAN
					break;
				case 0x2D1B:		//	â´›	0xe2 0xb4 0x9b	GEORGIAN LETTER JIL
					*p = 0x10EB;	//	áƒ«	0xe1 0x83 0xab	GEORGIAN SMALL LETTER JIL
					break;
				case 0x2D1C:		//	â´œ	0xe2 0xb4 0x9c	GEORGIAN LETTER CIL
					*p = 0x10EC;	//	áƒ¬	0xe1 0x83 0xac	GEORGIAN SMALL LETTER CIL
					break;
				case 0x2D1D:		//	â´	0xe2 0xb4 0x9d	GEORGIAN LETTER CHAR
					*p = 0x10ED;	//	áƒ­	0xe1 0x83 0xad	GEORGIAN SMALL LETTER CHAR
					break;
				case 0x2D1E:		//	â´ž	0xe2 0xb4 0x9e	GEORGIAN LETTER XAN
					*p = 0x10EE;	//	áƒ®	0xe1 0x83 0xae	GEORGIAN SMALL LETTER XAN
					break;
				case 0x2D1F:		//	â´Ÿ	0xe2 0xb4 0x9f	GEORGIAN LETTER JHAN
					*p = 0x10EF;	//	áƒ¯	0xe1 0x83 0xaf	GEORGIAN SMALL LETTER JHAN
					break;
				case 0x2D20:		//	â´ 	0xe2 0xb4 0xa0	GEORGIAN LETTER HAE
					*p = 0x10F0;	//	áƒ°	0xe1 0x83 0xb0	GEORGIAN SMALL LETTER HAE
					break;
				case 0x2D21:		//	â´¡	0xe2 0xb4 0xa1	GEORGIAN LETTER HE
					*p = 0x10F1;	//	áƒ±	0xe1 0x83 0xb1	GEORGIAN SMALL LETTER HE
					break;
				case 0x2D22:		//	â´¢	0xe2 0xb4 0xa2	GEORGIAN LETTER HIE
					*p = 0x10F2;	//	áƒ²	0xe1 0x83 0xb2	GEORGIAN SMALL LETTER HIE
					break;
				case 0x2D23:		//	â´£	0xe2 0xb4 0xa3	GEORGIAN LETTER WE
					*p = 0x10F3;	//	áƒ³	0xe1 0x83 0xb3	GEORGIAN SMALL LETTER WE
					break;
				case 0x2D24:		//	â´¤	0xe2 0xb4 0xa4	GEORGIAN LETTER HAR
					*p = 0x10F4;	//	áƒ´	0xe1 0x83 0xb4	GEORGIAN SMALL LETTER HAR
					break;
				case 0x2D25:		//	â´¥	0xe2 0xb4 0xa5	GEORGIAN LETTER HOE
					*p = 0x10F5;	//	áƒµ	0xe1 0x83 0xb5	GEORGIAN SMALL LETTER HOE
					break;
				case 0x2D27:		//	â´§	0xe2 0xb4 0xa7	GEORGIAN LETTER YN
					*p = 0x10F7;	//	áƒ·	0xe1 0x83 0xb7	GEORGIAN SMALL LETTER YN
					break;
				case 0x2D2D:		//	â´­	0xe2 0xb4 0xad	GEORGIAN LETTER AEN
					*p = 0x10FD;	//	áƒ½	0xe1 0x83 0xbd	GEORGIAN SMALL LETTER AEN
					break;
				case 0xA640:		//	ê™€	0xea 0x99 0x80	CYRILLIC CAPITAL LETTER ZEMLYA
					*p = 0xA641;	//	ê™	0xea 0x99 0x81	CYRILLIC SMALL LETTER ZEMLYA
					break;
				case 0xA642:		//	ê™‚	0xea 0x99 0x82	CYRILLIC CAPITAL LETTER DZELO
					*p = 0xA643;	//	ê™ƒ	0xea 0x99 0x83	CYRILLIC SMALL LETTER DZELO
					break;
				case 0xA644:		//	ê™„	0xea 0x99 0x84	CYRILLIC CAPITAL LETTER REVERSED DZE
					*p = 0xA645;	//	ê™…	0xea 0x99 0x85	CYRILLIC SMALL LETTER REVERSED DZE
					break;
				case 0xA646:		//	ê™†	0xea 0x99 0x86	CYRILLIC CAPITAL LETTER IOTA
					*p = 0xA647;	//	ê™‡	0xea 0x99 0x87	CYRILLIC SMALL LETTER IOTA
					break;
				case 0xA648:		//	ê™ˆ	0xea 0x99 0x88	CYRILLIC CAPITAL LETTER DJERV
					*p = 0xA649;	//	ê™‰	0xea 0x99 0x89	CYRILLIC SMALL LETTER DJERV
					break;
				case 0xA64A:		//	ê™Š	0xea 0x99 0x8a	CYRILLIC CAPITAL LETTER MONOGRAPH UK
					*p = 0xA64B;	//	ê™‹	0xea 0x99 0x8b	CYRILLIC SMALL LETTER MONOGRAPH UK
					break;
				case 0xA64C:		//	ê™Œ	0xea 0x99 0x8c	CYRILLIC CAPITAL LETTER BROAD OMEGA
					*p = 0xA64D;	//	ê™	0xea 0x99 0x8d	CYRILLIC SMALL LETTER BROAD OMEGA
					break;
				case 0xA64E:		//	ê™Ž	0xea 0x99 0x8e	CYRILLIC CAPITAL LETTER NEUTRAL YER
					*p = 0xA64F;	//	ê™	0xea 0x99 0x8f	CYRILLIC SMALL LETTER NEUTRAL YER
					break;
				case 0xA650:		//	ê™	0xea 0x99 0x90	CYRILLIC CAPITAL LETTER YERU WITH BACK YER
					*p = 0xA651;	//	ê™‘	0xea 0x99 0x91	CYRILLIC SMALL LETTER YERU WITH BACK YER
					break;
				case 0xA652:		//	ê™’	0xea 0x99 0x92	CYRILLIC CAPITAL LETTER IOTIFIED YAT
					*p = 0xA653;	//	ê™“	0xea 0x99 0x93	CYRILLIC SMALL LETTER IOTIFIED YAT
					break;
				case 0xA654:		//	ê™”	0xea 0x99 0x94	CYRILLIC CAPITAL LETTER REVERSED YU
					*p = 0xA655;	//	ê™•	0xea 0x99 0x95	CYRILLIC SMALL LETTER REVERSED YU
					break;
				case 0xA656:		//	ê™–	0xea 0x99 0x96	CYRILLIC CAPITAL LETTER IOTIFIED A
					*p = 0xA657;	//	ê™—	0xea 0x99 0x97	CYRILLIC SMALL LETTER IOTIFIED A
					break;
				case 0xA658:		//	ê™˜	0xea 0x99 0x98	CYRILLIC CAPITAL LETTER CLOSED LITTLE YUS
					*p = 0xA659;	//	ê™™	0xea 0x99 0x99	CYRILLIC SMALL LETTER CLOSED LITTLE YUS
					break;
				case 0xA65A:		//	ê™š	0xea 0x99 0x9a	CYRILLIC CAPITAL LETTER BLENDED YUS
					*p = 0xA65B;	//	ê™›	0xea 0x99 0x9b	CYRILLIC SMALL LETTER BLENDED YUS
					break;
				case 0xA65C:		//	ê™œ	0xea 0x99 0x9c	CYRILLIC CAPITAL LETTER IOTIFIED CLOSED LITTLE YUS
					*p = 0xA65D;	//	ê™	0xea 0x99 0x9d	CYRILLIC SMALL LETTER IOTIFIED CLOSED LITTLE YUS
					break;
				case 0xA65E:		//	ê™ž	0xea 0x99 0x9e	CYRILLIC CAPITAL LETTER YN
					*p = 0xA65F;	//	ê™Ÿ	0xea 0x99 0x9f	CYRILLIC SMALL LETTER YN
					break;
				case 0xA660:		//	ê™ 	0xea 0x99 0xa0	CYRILLIC CAPITAL LETTER REVERSED TSE
					*p = 0xA661;	//	ê™¡	0xea 0x99 0xa1	CYRILLIC SMALL LETTER REVERSED TSE
					break;
				case 0xA662:		//	ê™¢	0xea 0x99 0xa2	CYRILLIC CAPITAL LETTER SOFT DE
					*p = 0xA663;	//	ê™£	0xea 0x99 0xa3	CYRILLIC SMALL LETTER SOFT DE
					break;
				case 0xA664:		//	ê™¤	0xea 0x99 0xa4	CYRILLIC CAPITAL LETTER SOFT EL
					*p = 0xA665;	//	ê™¥	0xea 0x99 0xa5	CYRILLIC SMALL LETTER SOFT EL
					break;
				case 0xA666:		//	ê™¦	0xea 0x99 0xa6	CYRILLIC CAPITAL LETTER SOFT EM
					*p = 0xA667;	//	ê™§	0xea 0x99 0xa7	CYRILLIC SMALL LETTER SOFT EM
					break;
				case 0xA668:		//	ê™¨	0xea 0x99 0xa8	CYRILLIC CAPITAL LETTER MONOCULAR O
					*p = 0xA669;	//	ê™©	0xea 0x99 0xa9	CYRILLIC SMALL LETTER MONOCULAR O
					break;
				case 0xA66A:		//	ê™ª	0xea 0x99 0xaa	CYRILLIC CAPITAL LETTER BINOCULAR O
					*p = 0xA66B;	//	ê™«	0xea 0x99 0xab	CYRILLIC SMALL LETTER BINOCULAR O
					break;
				case 0xA66C:		//	ê™¬	0xea 0x99 0xac	CYRILLIC CAPITAL LETTER DOUBLE MONOCULAR O
					*p = 0xA66D;	//	ê™­	0xea 0x99 0xad	CYRILLIC SMALL LETTER DOUBLE MONOCULAR O
					break;
				case 0xA680:		//	êš€	0xea 0x9a 0x80	CYRILLIC CAPITAL LETTER DWE
					*p = 0xA681;	//	êš	0xea 0x9a 0x81	CYRILLIC SMALL LETTER DWE
					break;
				case 0xA682:		//	êš‚	0xea 0x9a 0x82	CYRILLIC CAPITAL LETTER DZWE
					*p = 0xA683;	//	êšƒ	0xea 0x9a 0x83	CYRILLIC SMALL LETTER DZWE
					break;
				case 0xA684:		//	êš„	0xea 0x9a 0x84	CYRILLIC CAPITAL LETTER ZHWE
					*p = 0xA685;	//	êš…	0xea 0x9a 0x85	CYRILLIC SMALL LETTER ZHWE
					break;
				case 0xA686:		//	êš†	0xea 0x9a 0x86	CYRILLIC CAPITAL LETTER CCHE
					*p = 0xA687;	//	êš‡	0xea 0x9a 0x87	CYRILLIC SMALL LETTER CCHE
					break;
				case 0xA688:		//	êšˆ	0xea 0x9a 0x88	CYRILLIC CAPITAL LETTER DZZE
					*p = 0xA689;	//	êš‰	0xea 0x9a 0x89	CYRILLIC SMALL LETTER DZZE
					break;
				case 0xA68A:		//	êšŠ	0xea 0x9a 0x8a	CYRILLIC CAPITAL LETTER TE WITH MIDDLE HOOK
					*p = 0xA68B;	//	êš‹	0xea 0x9a 0x8b	CYRILLIC SMALL LETTER TE WITH MIDDLE HOOK
					break;
				case 0xA68C:		//	êšŒ	0xea 0x9a 0x8c	CYRILLIC CAPITAL LETTER TWE
					*p = 0xA68D;	//	êš	0xea 0x9a 0x8d	CYRILLIC SMALL LETTER TWE
					break;
				case 0xA68E:		//	êšŽ	0xea 0x9a 0x8e	CYRILLIC CAPITAL LETTER TSWE
					*p = 0xA68F;	//	êš	0xea 0x9a 0x8f	CYRILLIC SMALL LETTER TSWE
					break;
				case 0xA690:		//	êš	0xea 0x9a 0x90	CYRILLIC CAPITAL LETTER TSSE
					*p = 0xA691;	//	êš‘	0xea 0x9a 0x91	CYRILLIC SMALL LETTER TSSE
					break;
				case 0xA692:		//	êš’	0xea 0x9a 0x92	CYRILLIC CAPITAL LETTER TCHE
					*p = 0xA693;	//	êš“	0xea 0x9a 0x93	CYRILLIC SMALL LETTER TCHE
					break;
				case 0xA694:		//	êš”	0xea 0x9a 0x94	CYRILLIC CAPITAL LETTER HWE
					*p = 0xA695;	//	êš•	0xea 0x9a 0x95	CYRILLIC SMALL LETTER HWE
					break;
				case 0xA696:		//	êš–	0xea 0x9a 0x96	CYRILLIC CAPITAL LETTER SHWE
					*p = 0xA697;	//	êš—	0xea 0x9a 0x97	CYRILLIC SMALL LETTER SHWE
					break;
				case 0xA698:		//	êš˜	0xea 0x9a 0x98	CYRILLIC CAPITAL LETTER DOUBLE O
					*p = 0xA699;	//	êš™	0xea 0x9a 0x99	CYRILLIC SMALL LETTER DOUBLE O
					break;
				case 0xA69A:		//	êšš	0xea 0x9a 0x9a	CYRILLIC CAPITAL LETTER CROSSED O
					*p = 0xA69B;	//	êš›	0xea 0x9a 0x9b	CYRILLIC SMALL LETTER CROSSED O
					break;
				case 0xA722:		//	êœ¢	0xea 0x9c 0xa2	LATIN CAPITAL LETTER EGYPTOLOGICAL ALEF
					*p = 0xA723;	//	êœ£	0xea 0x9c 0xa3	LATIN SMALL LETTER EGYPTOLOGICAL ALEF
					break;
				case 0xA724:		//	êœ¤	0xea 0x9c 0xa4	LATIN CAPITAL LETTER EGYPTOLOGICAL AIN
					*p = 0xA725;	//	êœ¥	0xea 0x9c 0xa5	LATIN SMALL LETTER EGYPTOLOGICAL AIN
					break;
				case 0xA726:		//	êœ¦	0xea 0x9c 0xa6	LATIN CAPITAL LETTER HENG
					*p = 0xA727;	//	êœ§	0xea 0x9c 0xa7	LATIN SMALL LETTER HENG
					break;
				case 0xA728:		//	êœ¨	0xea 0x9c 0xa8	LATIN CAPITAL LETTER TZ
					*p = 0xA729;	//	êœ©	0xea 0x9c 0xa9	LATIN SMALL LETTER TZ
					break;
				case 0xA72A:		//	êœª	0xea 0x9c 0xaa	LATIN CAPITAL LETTER TRESILLO
					*p = 0xA72B;	//	êœ«	0xea 0x9c 0xab	LATIN SMALL LETTER TRESILLO
					break;
				case 0xA72C:		//	êœ¬	0xea 0x9c 0xac	LATIN CAPITAL LETTER CUATRILLO
					*p = 0xA72D;	//	êœ­	0xea 0x9c 0xad	LATIN SMALL LETTER CUATRILLO
					break;
				case 0xA72E:		//	êœ®	0xea 0x9c 0xae	LATIN CAPITAL LETTER CUATRILLO WITH COMMA
					*p = 0xA72F;	//	êœ¯	0xea 0x9c 0xaf	LATIN SMALL LETTER CUATRILLO WITH COMMA
					break;
				case 0xA732:		//	êœ²	0xea 0x9c 0xb2	LATIN CAPITAL LETTER AA
					*p = 0xA733;	//	êœ³	0xea 0x9c 0xb3	LATIN SMALL LETTER AA
					break;
				case 0xA734:		//	êœ´	0xea 0x9c 0xb4	LATIN CAPITAL LETTER AO
					*p = 0xA735;	//	êœµ	0xea 0x9c 0xb5	LATIN SMALL LETTER AO
					break;
				case 0xA736:		//	êœ¶	0xea 0x9c 0xb6	LATIN CAPITAL LETTER AU
					*p = 0xA737;	//	êœ·	0xea 0x9c 0xb7	LATIN SMALL LETTER AU
					break;
				case 0xA738:		//	êœ¸	0xea 0x9c 0xb8	LATIN CAPITAL LETTER AV
					*p = 0xA739;	//	êœ¹	0xea 0x9c 0xb9	LATIN SMALL LETTER AV
					break;
				case 0xA73A:		//	êœº	0xea 0x9c 0xba	LATIN CAPITAL LETTER AV WITH HORIZONTAL BAR
					*p = 0xA73B;	//	êœ»	0xea 0x9c 0xbb	LATIN SMALL LETTER AV WITH HORIZONTAL BAR
					break;
				case 0xA73C:		//	êœ¼	0xea 0x9c 0xbc	LATIN CAPITAL LETTER AY
					*p = 0xA73D;	//	êœ½	0xea 0x9c 0xbd	LATIN SMALL LETTER AY
					break;
				case 0xA73E:		//	êœ¾	0xea 0x9c 0xbe	LATIN CAPITAL LETTER REVERSED C WITH DOT
					*p = 0xA73F;	//	êœ¿	0xea 0x9c 0xbf	LATIN SMALL LETTER REVERSED C WITH DOT
					break;
				case 0xA740:		//	ê€	0xea 0x9d 0x80	LATIN CAPITAL LETTER K WITH STROKE
					*p = 0xA741;	//	ê	0xea 0x9d 0x81	LATIN SMALL LETTER K WITH STROKE
					break;
				case 0xA742:		//	ê‚	0xea 0x9d 0x82	LATIN CAPITAL LETTER K WITH DIAGONAL STROKE
					*p = 0xA743;	//	êƒ	0xea 0x9d 0x83	LATIN SMALL LETTER K WITH DIAGONAL STROKE
					break;
				case 0xA744:		//	ê„	0xea 0x9d 0x84	LATIN CAPITAL LETTER K WITH STROKE AND DIAGONAL STROKE
					*p = 0xA745;	//	ê…	0xea 0x9d 0x85	LATIN SMALL LETTER K WITH STROKE AND DIAGONAL STROKE
					break;
				case 0xA746:		//	ê†	0xea 0x9d 0x86	LATIN CAPITAL LETTER BROKEN L
					*p = 0xA747;	//	ê‡	0xea 0x9d 0x87	LATIN SMALL LETTER BROKEN L
					break;
				case 0xA748:		//	êˆ	0xea 0x9d 0x88	LATIN CAPITAL LETTER L WITH HIGH STROKE
					*p = 0xA749;	//	ê‰	0xea 0x9d 0x89	LATIN SMALL LETTER L WITH HIGH STROKE
					break;
				case 0xA74A:		//	êŠ	0xea 0x9d 0x8a	LATIN CAPITAL LETTER O WITH LONG STROKE OVERLAY
					*p = 0xA74B;	//	ê‹	0xea 0x9d 0x8b	LATIN SMALL LETTER O WITH LONG STROKE OVERLAY
					break;
				case 0xA74C:		//	êŒ	0xea 0x9d 0x8c	LATIN CAPITAL LETTER O WITH LOOP
					*p = 0xA74D;	//	ê	0xea 0x9d 0x8d	LATIN SMALL LETTER O WITH LOOP
					break;
				case 0xA74E:		//	êŽ	0xea 0x9d 0x8e	LATIN CAPITAL LETTER OO
					*p = 0xA74F;	//	ê	0xea 0x9d 0x8f	LATIN SMALL LETTER OO
					break;
				case 0xA750:		//	ê	0xea 0x9d 0x90	LATIN CAPITAL LETTER P WITH STROKE THROUGH DESCENDER
					*p = 0xA751;	//	ê‘	0xea 0x9d 0x91	LATIN SMALL LETTER P WITH STROKE THROUGH DESCENDER
					break;
				case 0xA752:		//	ê’	0xea 0x9d 0x92	LATIN CAPITAL LETTER P WITH FLOURISH
					*p = 0xA753;	//	ê“	0xea 0x9d 0x93	LATIN SMALL LETTER P WITH FLOURISH
					break;
				case 0xA754:		//	ê”	0xea 0x9d 0x94	LATIN CAPITAL LETTER P WITH SQUIRREL TAIL
					*p = 0xA755;	//	ê•	0xea 0x9d 0x95	LATIN SMALL LETTER P WITH SQUIRREL TAIL
					break;
				case 0xA756:		//	ê–	0xea 0x9d 0x96	LATIN CAPITAL LETTER Q WITH STROKE THROUGH DESCENDER
					*p = 0xA757;	//	ê—	0xea 0x9d 0x97	LATIN SMALL LETTER Q WITH STROKE THROUGH DESCENDER
					break;
				case 0xA758:		//	ê˜	0xea 0x9d 0x98	LATIN CAPITAL LETTER Q WITH DIAGONAL STROKE
					*p = 0xA759;	//	ê™	0xea 0x9d 0x99	LATIN SMALL LETTER Q WITH DIAGONAL STROKE
					break;
				case 0xA75A:		//	êš	0xea 0x9d 0x9a	LATIN CAPITAL LETTER R ROTUNDA
					*p = 0xA75B;	//	ê›	0xea 0x9d 0x9b	LATIN SMALL LETTER R ROTUNDA
					break;
				case 0xA75C:		//	êœ	0xea 0x9d 0x9c	LATIN CAPITAL LETTER RUM ROTUNDA
					*p = 0xA75D;	//	ê	0xea 0x9d 0x9d	LATIN SMALL LETTER RUM ROTUNDA
					break;
				case 0xA75E:		//	êž	0xea 0x9d 0x9e	LATIN CAPITAL LETTER V WITH DIAGONAL STROKE
					*p = 0xA75F;	//	êŸ	0xea 0x9d 0x9f	LATIN SMALL LETTER V WITH DIAGONAL STROKE
					break;
				case 0xA760:		//	ê 	0xea 0x9d 0xa0	LATIN CAPITAL LETTER VY
					*p = 0xA761;	//	ê¡	0xea 0x9d 0xa1	LATIN SMALL LETTER VY
					break;
				case 0xA762:		//	ê¢	0xea 0x9d 0xa2	LATIN CAPITAL LETTER VISIGOTHIC Z
					*p = 0xA763;	//	ê£	0xea 0x9d 0xa3	LATIN SMALL LETTER VISIGOTHIC Z
					break;
				case 0xA764:		//	ê¤	0xea 0x9d 0xa4	LATIN CAPITAL LETTER THORN WITH STROKE
					*p = 0xA765;	//	ê¥	0xea 0x9d 0xa5	LATIN SMALL LETTER THORN WITH STROKE
					break;
				case 0xA766:		//	ê¦	0xea 0x9d 0xa6	LATIN CAPITAL LETTER THORN WITH STROKE THROUGH DESCENDER
					*p = 0xA767;	//	ê§	0xea 0x9d 0xa7	LATIN SMALL LETTER THORN WITH STROKE THROUGH DESCENDER
					break;
				case 0xA768:		//	ê¨	0xea 0x9d 0xa8	LATIN CAPITAL LETTER VEND
					*p = 0xA769;	//	ê©	0xea 0x9d 0xa9	LATIN SMALL LETTER VEND
					break;
				case 0xA76A:		//	êª	0xea 0x9d 0xaa	LATIN CAPITAL LETTER ET
					*p = 0xA76B;	//	ê«	0xea 0x9d 0xab	LATIN SMALL LETTER ET
					break;
				case 0xA76C:		//	ê¬	0xea 0x9d 0xac	LATIN CAPITAL LETTER IS
					*p = 0xA76D;	//	ê­	0xea 0x9d 0xad	LATIN SMALL LETTER IS
					break;
				case 0xA76E:		//	ê®	0xea 0x9d 0xae	LATIN CAPITAL LETTER CON
					*p = 0xA76F;	//	ê¯	0xea 0x9d 0xaf	LATIN SMALL LETTER CON
					break;
				case 0xA779:		//	ê¹	0xea 0x9d 0xb9	LATIN CAPITAL LETTER INSULAR D
					*p = 0xA77A;	//	êº	0xea 0x9d 0xba	LATIN SMALL LETTER INSULAR D
					break;
				case 0xA77B:		//	ê»	0xea 0x9d 0xbb	LATIN CAPITAL LETTER INSULAR F
					*p = 0xA77C;	//	ê¼	0xea 0x9d 0xbc	LATIN SMALL LETTER INSULAR F
					break;
				case 0xA77D:		//	ê½	0xea 0x9d 0xbd	LATIN CAPITAL LETTER INSULAR G
					*p = 0x1D79;	//	áµ¹	0xe1 0xb5 0xb9	LATIN SMALL LETTER INSULAR G
					break;
				case 0xA77E:		//	ê¾	0xea 0x9d 0xbe	LATIN CAPITAL LETTER TURNED INSULAR G
					*p = 0xA77F;	//	ê¿	0xea 0x9d 0xbf	LATIN SMALL LETTER TURNED INSULAR G
					break;
				case 0xA780:		//	êž€	0xea 0x9e 0x80	LATIN CAPITAL LETTER TURNED L
					*p = 0xA781;	//	êž	0xea 0x9e 0x81	LATIN SMALL LETTER TURNED L
					break;
				case 0xA782:		//	êž‚	0xea 0x9e 0x82	LATIN CAPITAL LETTER INSULAR R
					*p = 0xA783;	//	êžƒ	0xea 0x9e 0x83	LATIN SMALL LETTER INSULAR R
					break;
				case 0xA784:		//	êž„	0xea 0x9e 0x84	LATIN CAPITAL LETTER INSULAR S
					*p = 0xA785;	//	êž…	0xea 0x9e 0x85	LATIN SMALL LETTER INSULAR S
					break;
				case 0xA786:		//	êž†	0xea 0x9e 0x86	LATIN CAPITAL LETTER INSULAR T
					*p = 0xA787;	//	êž‡	0xea 0x9e 0x87	LATIN SMALL LETTER INSULAR T
					break;
				case 0xA78B:		//	êž‹	0xea 0x9e 0x8b	LATIN CAPITAL LETTER SALTILLO
					*p = 0xA78C;	//	êžŒ	0xea 0x9e 0x8c	LATIN SMALL LETTER SALTILLO
					break;
				case 0xA78D:		//	êž	0xea 0x9e 0x8d	LATIN CAPITAL LETTER TURNED H
					*p = 0x0265;	//	É¥	0xc9 0xa5	LATIN SMALL LETTER TURNED H
					break;
				case 0xA790:		//	êž	0xea 0x9e 0x90	LATIN CAPITAL LETTER N WITH DESCENDER
					*p = 0xA791;	//	êž‘	0xea 0x9e 0x91	LATIN SMALL LETTER N WITH DESCENDER
					break;
				case 0xA792:		//	êž’	0xea 0x9e 0x92	LATIN CAPITAL LETTER C WITH BAR
					*p = 0xA793;	//	êž“	0xea 0x9e 0x93	LATIN SMALL LETTER C WITH BAR
					break;
				case 0xA796:		//	êž–	0xea 0x9e 0x96	LATIN CAPITAL LETTER B WITH FLOURISH
					*p = 0xA797;	//	êž—	0xea 0x9e 0x97	LATIN SMALL LETTER B WITH FLOURISH
					break;
				case 0xA798:		//	êž˜	0xea 0x9e 0x98	LATIN CAPITAL LETTER F WITH STROKE
					*p = 0xA799;	//	êž™	0xea 0x9e 0x99	LATIN SMALL LETTER F WITH STROKE
					break;
				case 0xA79A:		//	êžš	0xea 0x9e 0x9a	LATIN CAPITAL LETTER VOLAPUK AE
					*p = 0xA79B;	//	êž›	0xea 0x9e 0x9b	LATIN SMALL LETTER VOLAPUK AE
					break;
				case 0xA79C:		//	êžœ	0xea 0x9e 0x9c	LATIN CAPITAL LETTER VOLAPUK OE
					*p = 0xA79D;	//	êž	0xea 0x9e 0x9d	LATIN SMALL LETTER VOLAPUK OE
					break;
				case 0xA79E:		//	êžž	0xea 0x9e 0x9e	LATIN CAPITAL LETTER VOLAPUK UE
					*p = 0xA79F;	//	êžŸ	0xea 0x9e 0x9f	LATIN SMALL LETTER VOLAPUK UE
					break;
				case 0xA7A0:		//	êž 	0xea 0x9e 0xa0	LATIN CAPITAL LETTER G WITH OBLIQUE STROKE
					*p = 0xA7A1;	//	êž¡	0xea 0x9e 0xa1	LATIN SMALL LETTER G WITH OBLIQUE STROKE
					break;
				case 0xA7A2:		//	êž¢	0xea 0x9e 0xa2	LATIN CAPITAL LETTER K WITH OBLIQUE STROKE
					*p = 0xA7A3;	//	êž£	0xea 0x9e 0xa3	LATIN SMALL LETTER K WITH OBLIQUE STROKE
					break;
				case 0xA7A4:		//	êž¤	0xea 0x9e 0xa4	LATIN CAPITAL LETTER N WITH OBLIQUE STROKE
					*p = 0xA7A5;	//	êž¥	0xea 0x9e 0xa5	LATIN SMALL LETTER N WITH OBLIQUE STROKE
					break;
				case 0xA7A6:		//	êž¦	0xea 0x9e 0xa6	LATIN CAPITAL LETTER R WITH OBLIQUE STROKE
					*p = 0xA7A7;	//	êž§	0xea 0x9e 0xa7	LATIN SMALL LETTER R WITH OBLIQUE STROKE
					break;
				case 0xA7A8:		//	êž¨	0xea 0x9e 0xa8	LATIN CAPITAL LETTER S WITH OBLIQUE STROKE
					*p = 0xA7A9;	//	êž©	0xea 0x9e 0xa9	LATIN SMALL LETTER S WITH OBLIQUE STROKE
					break;
				case 0xA7AA:		//	êžª	0xea 0x9e 0xaa	LATIN CAPITAL LETTER H WITH HOOK
					*p = 0x0266;	//	É¦	0xc9 0xa6	LATIN SMALL LETTER H WITH HOOK
					break;
				case 0xA7AB:		//	êž«	0xea 0x9e 0xab	LATIN CAPITAL LETTER REVERSED OPEN E
					*p = 0x025C;	//	Éœ	0xc9 0x9c	LATIN SMALL LETTER REVERSED OPEN E
					break;
				case 0xA7AC:		//	êž¬	0xea 0x9e 0xac	LATIN CAPITAL LETTER SCRIPT G
					*p = 0x0261;	//	É¡	0xc9 0xa1	LATIN SMALL LETTER SCRIPT G
					break;
				case 0xA7AD:		//	êž­	0xea 0x9e 0xad	LATIN CAPITAL LETTER L WITH BELT
					*p = 0x026C;	//	É¬	0xc9 0xac	LATIN SMALL LETTER L WITH BELT
					break;
				case 0xA7AE:		//	êž®	0xea 0x9e 0xae	LATIN CAPITAL LETTER SMALL CAPITAL I
					*p = 0x026A;	//	Éª	0xc9 0xaa	LATIN SMALL LETTER SMALL CAPITAL I
					break;
				case 0xA7B0:		//	êž°	0xea 0x9e 0xb0	LATIN CAPITAL LETTER TURNED K
					*p = 0x029E;	//	Êž	0xca 0x9e	LATIN SMALL LETTER TURNED K
					break;
				case 0xA7B1:		//	êž±	0xea 0x9e 0xb1	LATIN CAPITAL LETTER TURNED T
					*p = 0x0287;	//	Ê‡	0xca 0x87	LATIN SMALL LETTER TURNED T
					break;
				case 0xA7B2:		//	êž²	0xea 0x9e 0xb2	LATIN CAPITAL LETTER J WITH CROSSED-TAIL
					*p = 0x029D;	//	Ê	0xca 0x9d	LATIN SMALL LETTER J WITH CROSSED-TAIL
					break;
				case 0xA7B3:		//	êž³	0xea 0x9e 0xb3	LATIN CAPITAL LETTER CHI
					*p = 0xAB53;	//	ê­“	0xea 0xad 0x93	LATIN SMALL LETTER CHI
					break;
				case 0xA7B4:		//	êž´	0xea 0x9e 0xb4	LATIN CAPITAL LETTER BETA
					*p = 0xA7B5;	//	êžµ	0xea 0x9e 0xb5	LATIN SMALL LETTER BETA
					break;
				case 0xA7B6:		//	êž¶	0xea 0x9e 0xb6	LATIN CAPITAL LETTER OMEGA
					*p = 0xA7B7;	//	êž·	0xea 0x9e 0xb7	LATIN SMALL LETTER OMEGA
					break;
				case 0xA7B8:		//	êž¸	0xea 0x9e 0xb8	LATIN CAPITAL LETTER U WITH STROKE
					*p = 0xA7B9;	//	êž¹	0xea 0x9e 0xb9	LATIN SMALL LETTER U WITH STROKE
					break;
				case 0xA7BA:		//	êžº	0xea 0x9e 0xba	LATIN CAPITAL LETTER GLOTTAL A
					*p = 0xA7BB;	//	êž»	0xea 0x9e 0xbb	LATIN SMALL LETTER GLOTTAL A
					break;
				case 0xA7BC:		//	êž¼	0xea 0x9e 0xbc	LATIN CAPITAL LETTER GLOTTAL I
					*p = 0xA7BD;	//	êž½	0xea 0x9e 0xbd	LATIN SMALL LETTER GLOTTAL I
					break;
				case 0xA7BE:		//	êž¾	0xea 0x9e 0xbe	LATIN CAPITAL LETTER GLOTTAL U
					*p = 0xA7BF;	//	êž¿	0xea 0x9e 0xbf	LATIN SMALL LETTER GLOTTAL U
					break;
				case 0xA7C2:		//	êŸ‚	0xea 0x9f 0x82	LATIN CAPITAL LETTER ANGLICANA W
					*p = 0xA7C3;	//	êŸƒ	0xea 0x9f 0x83	LATIN SMALL LETTER ANGLICANA W
					break;
				case 0xA7C4:		//	êŸ„	0xea 0x9f 0x84	LATIN CAPITAL LETTER C WITH PALATAL HOOK
					*p = 0xA794;	//	êž”	0xea 0x9e 0x94	LATIN SMALL LETTER C WITH PALATAL HOOK
					break;
				case 0xA7C5:		//	êŸ…	0xea 0x9f 0x85	LATIN CAPITAL LETTER S WITH HOOK
					*p = 0x0282;	//	Ê‚	0xca 0x82	LATIN SMALL LETTER S WITH HOOK
					break;
				case 0xA7C6:		//	êŸ†	0xea 0x9f 0x86	LATIN CAPITAL LETTER Z WITH PALATAL HOOK
					*p = 0x1D8E;	//	á¶Ž	0xe1 0xb6 0x8e	LATIN SMALL LETTER Z WITH PALATAL HOOK
					break;
				case 0xA7C7:		//	êŸ‡	0xea 0x9f 0x87	LATIN CAPITAL LETTER D WITH SHORT STROKE OVERLAY
					*p = 0xA7C8;	//	êŸˆ	0xea 0x9f 0x88	LATIN SMALL LETTER D WITH SHORT STROKE OVERLAY
					break;
				case 0xA7C9:		//	êŸ‰	0xea 0x9f 0x89	LATIN CAPITAL LETTER S WITH SHORT STROKE OVERLAY
					*p = 0xA7CA;	//	êŸŠ	0xea 0x9f 0x8a	LATIN SMALL LETTER S WITH SHORT STROKE OVERLAY
					break;
				case 0xA7F5:		//	êŸµ	0xea 0x9f 0xb5	LATIN CAPITAL LETTER REVERSED HALF H
					*p = 0xA7F6;	//	êŸ¶	0xea 0x9f 0xb6	LATIN SMALL LETTER REVERSED HALF H
					break;
				case 0xFF21:		//	ï¼¡	0xef 0xbc 0xa1	FULLWIDTH LATIN CAPITAL LETTER A
					*p = 0xFF41;	//	ï½	0xef 0xbd 0x81	FULLWIDTH LATIN SMALL LETTER A
					break;
				case 0xFF22:		//	ï¼¢	0xef 0xbc 0xa2	FULLWIDTH LATIN CAPITAL LETTER B
					*p = 0xFF42;	//	ï½‚	0xef 0xbd 0x82	FULLWIDTH LATIN SMALL LETTER B
					break;
				case 0xFF23:		//	ï¼£	0xef 0xbc 0xa3	FULLWIDTH LATIN CAPITAL LETTER C
					*p = 0xFF43;	//	ï½ƒ	0xef 0xbd 0x83	FULLWIDTH LATIN SMALL LETTER C
					break;
				case 0xFF24:		//	ï¼¤	0xef 0xbc 0xa4	FULLWIDTH LATIN CAPITAL LETTER D
					*p = 0xFF44;	//	ï½„	0xef 0xbd 0x84	FULLWIDTH LATIN SMALL LETTER D
					break;
				case 0xFF25:		//	ï¼¥	0xef 0xbc 0xa5	FULLWIDTH LATIN CAPITAL LETTER E
					*p = 0xFF45;	//	ï½…	0xef 0xbd 0x85	FULLWIDTH LATIN SMALL LETTER E
					break;
				case 0xFF26:		//	ï¼¦	0xef 0xbc 0xa6	FULLWIDTH LATIN CAPITAL LETTER F
					*p = 0xFF46;	//	ï½†	0xef 0xbd 0x86	FULLWIDTH LATIN SMALL LETTER F
					break;
				case 0xFF27:		//	ï¼§	0xef 0xbc 0xa7	FULLWIDTH LATIN CAPITAL LETTER G
					*p = 0xFF47;	//	ï½‡	0xef 0xbd 0x87	FULLWIDTH LATIN SMALL LETTER G
					break;
				case 0xFF28:		//	ï¼¨	0xef 0xbc 0xa8	FULLWIDTH LATIN CAPITAL LETTER H
					*p = 0xFF48;	//	ï½ˆ	0xef 0xbd 0x88	FULLWIDTH LATIN SMALL LETTER H
					break;
				case 0xFF29:		//	ï¼©	0xef 0xbc 0xa9	FULLWIDTH LATIN CAPITAL LETTER I
					*p = 0xFF49;	//	ï½‰	0xef 0xbd 0x89	FULLWIDTH LATIN SMALL LETTER I
					break;
				case 0xFF2A:		//	ï¼ª	0xef 0xbc 0xaa	FULLWIDTH LATIN CAPITAL LETTER J
					*p = 0xFF4A;	//	ï½Š	0xef 0xbd 0x8a	FULLWIDTH LATIN SMALL LETTER J
					break;
				case 0xFF2B:		//	ï¼«	0xef 0xbc 0xab	FULLWIDTH LATIN CAPITAL LETTER K
					*p = 0xFF4B;	//	ï½‹	0xef 0xbd 0x8b	FULLWIDTH LATIN SMALL LETTER K
					break;
				case 0xFF2C:		//	ï¼¬	0xef 0xbc 0xac	FULLWIDTH LATIN CAPITAL LETTER L
					*p = 0xFF4C;	//	ï½Œ	0xef 0xbd 0x8c	FULLWIDTH LATIN SMALL LETTER L
					break;
				case 0xFF2D:		//	ï¼­	0xef 0xbc 0xad	FULLWIDTH LATIN CAPITAL LETTER M
					*p = 0xFF4D;	//	ï½	0xef 0xbd 0x8d	FULLWIDTH LATIN SMALL LETTER M
					break;
				case 0xFF2E:		//	ï¼®	0xef 0xbc 0xae	FULLWIDTH LATIN CAPITAL LETTER N
					*p = 0xFF4E;	//	ï½Ž	0xef 0xbd 0x8e	FULLWIDTH LATIN SMALL LETTER N
					break;
				case 0xFF2F:		//	ï¼¯	0xef 0xbc 0xaf	FULLWIDTH LATIN CAPITAL LETTER O
					*p = 0xFF4F;	//	ï½	0xef 0xbd 0x8f	FULLWIDTH LATIN SMALL LETTER O
					break;
				case 0xFF30:		//	ï¼°	0xef 0xbc 0xb0	FULLWIDTH LATIN CAPITAL LETTER P
					*p = 0xFF50;	//	ï½	0xef 0xbd 0x90	FULLWIDTH LATIN SMALL LETTER P
					break;
				case 0xFF31:		//	ï¼±	0xef 0xbc 0xb1	FULLWIDTH LATIN CAPITAL LETTER Q
					*p = 0xFF51;	//	ï½‘	0xef 0xbd 0x91	FULLWIDTH LATIN SMALL LETTER Q
					break;
				case 0xFF32:		//	ï¼²	0xef 0xbc 0xb2	FULLWIDTH LATIN CAPITAL LETTER R
					*p = 0xFF52;	//	ï½’	0xef 0xbd 0x92	FULLWIDTH LATIN SMALL LETTER R
					break;
				case 0xFF33:		//	ï¼³	0xef 0xbc 0xb3	FULLWIDTH LATIN CAPITAL LETTER S
					*p = 0xFF53;	//	ï½“	0xef 0xbd 0x93	FULLWIDTH LATIN SMALL LETTER S
					break;
				case 0xFF34:		//	ï¼´	0xef 0xbc 0xb4	FULLWIDTH LATIN CAPITAL LETTER T
					*p = 0xFF54;	//	ï½”	0xef 0xbd 0x94	FULLWIDTH LATIN SMALL LETTER T
					break;
				case 0xFF35:		//	ï¼µ	0xef 0xbc 0xb5	FULLWIDTH LATIN CAPITAL LETTER U
					*p = 0xFF55;	//	ï½•	0xef 0xbd 0x95	FULLWIDTH LATIN SMALL LETTER U
					break;
				case 0xFF36:		//	ï¼¶	0xef 0xbc 0xb6	FULLWIDTH LATIN CAPITAL LETTER V
					*p = 0xFF56;	//	ï½–	0xef 0xbd 0x96	FULLWIDTH LATIN SMALL LETTER V
					break;
				case 0xFF37:		//	ï¼·	0xef 0xbc 0xb7	FULLWIDTH LATIN CAPITAL LETTER W
					*p = 0xFF57;	//	ï½—	0xef 0xbd 0x97	FULLWIDTH LATIN SMALL LETTER W
					break;
				case 0xFF38:		//	ï¼¸	0xef 0xbc 0xb8	FULLWIDTH LATIN CAPITAL LETTER X
					*p = 0xFF58;	//	ï½˜	0xef 0xbd 0x98	FULLWIDTH LATIN SMALL LETTER X
					break;
				case 0xFF39:		//	ï¼¹	0xef 0xbc 0xb9	FULLWIDTH LATIN CAPITAL LETTER Y
					*p = 0xFF59;	//	ï½™	0xef 0xbd 0x99	FULLWIDTH LATIN SMALL LETTER Y
					break;
				case 0xFF3A:		//	ï¼º	0xef 0xbc 0xba	FULLWIDTH LATIN CAPITAL LETTER Z
					*p = 0xFF5A;	//	ï½š	0xef 0xbd 0x9a	FULLWIDTH LATIN SMALL LETTER Z
					break;
				case 0x10400:		//	ð€	0xf0 0x90 0x90 0x80	DESERET CAPITAL LETTER LONG I
					*p = 0x10428;	//	ð¨	0xf0 0x90 0x90 0xa8	DESERET SMALL LETTER LONG I
					break;
				case 0x10401:		//	ð	0xf0 0x90 0x90 0x81	DESERET CAPITAL LETTER LONG E
					*p = 0x10429;	//	ð©	0xf0 0x90 0x90 0xa9	DESERET SMALL LETTER LONG E
					break;
				case 0x10402:		//	ð‚	0xf0 0x90 0x90 0x82	DESERET CAPITAL LETTER LONG A
					*p = 0x1042A;	//	ðª	0xf0 0x90 0x90 0xaa	DESERET SMALL LETTER LONG A
					break;
				case 0x10403:		//	ðƒ	0xf0 0x90 0x90 0x83	DESERET CAPITAL LETTER LONG AH
					*p = 0x1042B;	//	ð«	0xf0 0x90 0x90 0xab	DESERET SMALL LETTER LONG AH
					break;
				case 0x10404:		//	ð„	0xf0 0x90 0x90 0x84	DESERET CAPITAL LETTER LONG O
					*p = 0x1042C;	//	ð¬	0xf0 0x90 0x90 0xac	DESERET SMALL LETTER LONG O
					break;
				case 0x10405:		//	ð…	0xf0 0x90 0x90 0x85	DESERET CAPITAL LETTER LONG OO
					*p = 0x1042D;	//	ð­	0xf0 0x90 0x90 0xad	DESERET SMALL LETTER LONG OO
					break;
				case 0x10406:		//	ð†	0xf0 0x90 0x90 0x86	DESERET CAPITAL LETTER SHORT I
					*p = 0x1042E;	//	ð®	0xf0 0x90 0x90 0xae	DESERET SMALL LETTER SHORT I
					break;
				case 0x10407:		//	ð‡	0xf0 0x90 0x90 0x87	DESERET CAPITAL LETTER SHORT E
					*p = 0x1042F;	//	ð¯	0xf0 0x90 0x90 0xaf	DESERET SMALL LETTER SHORT E
					break;
				case 0x10408:		//	ðˆ	0xf0 0x90 0x90 0x88	DESERET CAPITAL LETTER SHORT A
					*p = 0x10430;	//	ð°	0xf0 0x90 0x90 0xb0	DESERET SMALL LETTER SHORT A
					break;
				case 0x10409:		//	ð‰	0xf0 0x90 0x90 0x89	DESERET CAPITAL LETTER SHORT AH
					*p = 0x10431;	//	ð±	0xf0 0x90 0x90 0xb1	DESERET SMALL LETTER SHORT AH
					break;
				case 0x1040A:		//	ðŠ	0xf0 0x90 0x90 0x8a	DESERET CAPITAL LETTER SHORT O
					*p = 0x10432;	//	ð²	0xf0 0x90 0x90 0xb2	DESERET SMALL LETTER SHORT O
					break;
				case 0x1040B:		//	ð‹	0xf0 0x90 0x90 0x8b	DESERET CAPITAL LETTER SHORT OO
					*p = 0x10433;	//	ð³	0xf0 0x90 0x90 0xb3	DESERET SMALL LETTER SHORT OO
					break;
				case 0x1040C:		//	ðŒ	0xf0 0x90 0x90 0x8c	DESERET CAPITAL LETTER AY
					*p = 0x10434;	//	ð´	0xf0 0x90 0x90 0xb4	DESERET SMALL LETTER AY
					break;
				case 0x1040D:		//	ð	0xf0 0x90 0x90 0x8d	DESERET CAPITAL LETTER OW
					*p = 0x10435;	//	ðµ	0xf0 0x90 0x90 0xb5	DESERET SMALL LETTER OW
					break;
				case 0x1040E:		//	ðŽ	0xf0 0x90 0x90 0x8e	DESERET CAPITAL LETTER WU
					*p = 0x10436;	//	ð¶	0xf0 0x90 0x90 0xb6	DESERET SMALL LETTER WU
					break;
				case 0x1040F:		//	ð	0xf0 0x90 0x90 0x8f	DESERET CAPITAL LETTER YEE
					*p = 0x10437;	//	ð·	0xf0 0x90 0x90 0xb7	DESERET SMALL LETTER YEE
					break;
				case 0x10410:		//	ð	0xf0 0x90 0x90 0x90	DESERET CAPITAL LETTER H
					*p = 0x10438;	//	ð¸	0xf0 0x90 0x90 0xb8	DESERET SMALL LETTER H
					break;
				case 0x10411:		//	ð‘	0xf0 0x90 0x90 0x91	DESERET CAPITAL LETTER PEE
					*p = 0x10439;	//	ð¹	0xf0 0x90 0x90 0xb9	DESERET SMALL LETTER PEE
					break;
				case 0x10412:		//	ð’	0xf0 0x90 0x90 0x92	DESERET CAPITAL LETTER BEE
					*p = 0x1043A;	//	ðº	0xf0 0x90 0x90 0xba	DESERET SMALL LETTER BEE
					break;
				case 0x10413:		//	ð“	0xf0 0x90 0x90 0x93	DESERET CAPITAL LETTER TEE
					*p = 0x1043B;	//	ð»	0xf0 0x90 0x90 0xbb	DESERET SMALL LETTER TEE
					break;
				case 0x10414:		//	ð”	0xf0 0x90 0x90 0x94	DESERET CAPITAL LETTER DEE
					*p = 0x1043C;	//	ð¼	0xf0 0x90 0x90 0xbc	DESERET SMALL LETTER DEE
					break;
				case 0x10415:		//	ð•	0xf0 0x90 0x90 0x95	DESERET CAPITAL LETTER CHEE
					*p = 0x1043D;	//	ð½	0xf0 0x90 0x90 0xbd	DESERET SMALL LETTER CHEE
					break;
				case 0x10416:		//	ð–	0xf0 0x90 0x90 0x96	DESERET CAPITAL LETTER JEE
					*p = 0x1043E;	//	ð¾	0xf0 0x90 0x90 0xbe	DESERET SMALL LETTER JEE
					break;
				case 0x10417:		//	ð—	0xf0 0x90 0x90 0x97	DESERET CAPITAL LETTER KAY
					*p = 0x1043F;	//	ð¿	0xf0 0x90 0x90 0xbf	DESERET SMALL LETTER KAY
					break;
				case 0x10418:		//	ð˜	0xf0 0x90 0x90 0x98	DESERET CAPITAL LETTER GAY
					*p = 0x10440;	//	ð‘€	0xf0 0x90 0x91 0x80	DESERET SMALL LETTER GAY
					break;
				case 0x10419:		//	ð™	0xf0 0x90 0x90 0x99	DESERET CAPITAL LETTER EF
					*p = 0x10441;	//	ð‘	0xf0 0x90 0x91 0x81	DESERET SMALL LETTER EF
					break;
				case 0x1041A:		//	ðš	0xf0 0x90 0x90 0x9a	DESERET CAPITAL LETTER VEE
					*p = 0x10442;	//	ð‘‚	0xf0 0x90 0x91 0x82	DESERET SMALL LETTER VEE
					break;
				case 0x1041B:		//	ð›	0xf0 0x90 0x90 0x9b	DESERET CAPITAL LETTER ETH
					*p = 0x10443;	//	ð‘ƒ	0xf0 0x90 0x91 0x83	DESERET SMALL LETTER ETH
					break;
				case 0x1041C:		//	ðœ	0xf0 0x90 0x90 0x9c	DESERET CAPITAL LETTER THEE
					*p = 0x10444;	//	ð‘„	0xf0 0x90 0x91 0x84	DESERET SMALL LETTER THEE
					break;
				case 0x1041D:		//	ð	0xf0 0x90 0x90 0x9d	DESERET CAPITAL LETTER ES
					*p = 0x10445;	//	ð‘…	0xf0 0x90 0x91 0x85	DESERET SMALL LETTER ES
					break;
				case 0x1041E:		//	ðž	0xf0 0x90 0x90 0x9e	DESERET CAPITAL LETTER ZEE
					*p = 0x10446;	//	ð‘†	0xf0 0x90 0x91 0x86	DESERET SMALL LETTER ZEE
					break;
				case 0x1041F:		//	ðŸ	0xf0 0x90 0x90 0x9f	DESERET CAPITAL LETTER ESH
					*p = 0x10447;	//	ð‘‡	0xf0 0x90 0x91 0x87	DESERET SMALL LETTER ESH
					break;
				case 0x10420:		//	ð 	0xf0 0x90 0x90 0xa0	DESERET CAPITAL LETTER ZHEE
					*p = 0x10448;	//	ð‘ˆ	0xf0 0x90 0x91 0x88	DESERET SMALL LETTER ZHEE
					break;
				case 0x10421:		//	ð¡	0xf0 0x90 0x90 0xa1	DESERET CAPITAL LETTER ER
					*p = 0x10449;	//	ð‘‰	0xf0 0x90 0x91 0x89	DESERET SMALL LETTER ER
					break;
				case 0x10422:		//	ð¢	0xf0 0x90 0x90 0xa2	DESERET CAPITAL LETTER EL
					*p = 0x1044A;	//	ð‘Š	0xf0 0x90 0x91 0x8a	DESERET SMALL LETTER EL
					break;
				case 0x10423:		//	ð£	0xf0 0x90 0x90 0xa3	DESERET CAPITAL LETTER EM
					*p = 0x1044B;	//	ð‘‹	0xf0 0x90 0x91 0x8b	DESERET SMALL LETTER EM
					break;
				case 0x10424:		//	ð¤	0xf0 0x90 0x90 0xa4	DESERET CAPITAL LETTER EN
					*p = 0x1044C;	//	ð‘Œ	0xf0 0x90 0x91 0x8c	DESERET SMALL LETTER EN
					break;
				case 0x10425:		//	ð¥	0xf0 0x90 0x90 0xa5	DESERET CAPITAL LETTER ENG
					*p = 0x1044D;	//	ð‘	0xf0 0x90 0x91 0x8d	DESERET SMALL LETTER ENG
					break;
				case 0x10426:		//	ð¦	0xf0 0x90 0x90 0xa6	DESERET CAPITAL LETTER OI
					*p = 0x1044E;	//	ð‘Ž	0xf0 0x90 0x91 0x8e	DESERET SMALL LETTER OI
					break;
				case 0x10427:		//	ð§	0xf0 0x90 0x90 0xa7	DESERET CAPITAL LETTER EW
					*p = 0x1044F;	//	ð‘	0xf0 0x90 0x91 0x8f	DESERET SMALL LETTER EW
					break;
				case 0x104B0:		//	ð’°	0xf0 0x90 0x92 0xb0	OSAGE CAPITAL LETTER A
					*p = 0x104D8;	//	ð“˜	0xf0 0x90 0x93 0x98	OSAGE SMALL LETTER A
					break;
				case 0x104B1:		//	ð’±	0xf0 0x90 0x92 0xb1	OSAGE CAPITAL LETTER AI
					*p = 0x104D9;	//	ð“™	0xf0 0x90 0x93 0x99	OSAGE SMALL LETTER AI
					break;
				case 0x104B2:		//	ð’²	0xf0 0x90 0x92 0xb2	OSAGE CAPITAL LETTER AIN
					*p = 0x104DA;	//	ð“š	0xf0 0x90 0x93 0x9a	OSAGE SMALL LETTER AIN
					break;
				case 0x104B3:		//	ð’³	0xf0 0x90 0x92 0xb3	OSAGE CAPITAL LETTER AH
					*p = 0x104DB;	//	ð“›	0xf0 0x90 0x93 0x9b	OSAGE SMALL LETTER AH
					break;
				case 0x104B4:		//	ð’´	0xf0 0x90 0x92 0xb4	OSAGE CAPITAL LETTER BRA
					*p = 0x104DC;	//	ð“œ	0xf0 0x90 0x93 0x9c	OSAGE SMALL LETTER BRA
					break;
				case 0x104B5:		//	ð’µ	0xf0 0x90 0x92 0xb5	OSAGE CAPITAL LETTER CHA
					*p = 0x104DD;	//	ð“	0xf0 0x90 0x93 0x9d	OSAGE SMALL LETTER CHA
					break;
				case 0x104B6:		//	ð’¶	0xf0 0x90 0x92 0xb6	OSAGE CAPITAL LETTER EHCHA
					*p = 0x104DE;	//	ð“ž	0xf0 0x90 0x93 0x9e	OSAGE SMALL LETTER EHCHA
					break;
				case 0x104B7:		//	ð’·	0xf0 0x90 0x92 0xb7	OSAGE CAPITAL LETTER E
					*p = 0x104DF;	//	ð“Ÿ	0xf0 0x90 0x93 0x9f	OSAGE SMALL LETTER E
					break;
				case 0x104B8:		//	ð’¸	0xf0 0x90 0x92 0xb8	OSAGE CAPITAL LETTER EIN
					*p = 0x104E0;	//	ð“ 	0xf0 0x90 0x93 0xa0	OSAGE SMALL LETTER EIN
					break;
				case 0x104B9:		//	ð’¹	0xf0 0x90 0x92 0xb9	OSAGE CAPITAL LETTER HA
					*p = 0x104E1;	//	ð“¡	0xf0 0x90 0x93 0xa1	OSAGE SMALL LETTER HA
					break;
				case 0x104BA:		//	ð’º	0xf0 0x90 0x92 0xba	OSAGE CAPITAL LETTER HYA
					*p = 0x104E2;	//	ð“¢	0xf0 0x90 0x93 0xa2	OSAGE SMALL LETTER HYA
					break;
				case 0x104BB:		//	ð’»	0xf0 0x90 0x92 0xbb	OSAGE CAPITAL LETTER I
					*p = 0x104E3;	//	ð“£	0xf0 0x90 0x93 0xa3	OSAGE SMALL LETTER I
					break;
				case 0x104BC:		//	ð’¼	0xf0 0x90 0x92 0xbc	OSAGE CAPITAL LETTER KA
					*p = 0x104E4;	//	ð“¤	0xf0 0x90 0x93 0xa4	OSAGE SMALL LETTER KA
					break;
				case 0x104BD:		//	ð’½	0xf0 0x90 0x92 0xbd	OSAGE CAPITAL LETTER EHKA
					*p = 0x104E5;	//	ð“¥	0xf0 0x90 0x93 0xa5	OSAGE SMALL LETTER EHKA
					break;
				case 0x104BE:		//	ð’¾	0xf0 0x90 0x92 0xbe	OSAGE CAPITAL LETTER KYA
					*p = 0x104E6;	//	ð“¦	0xf0 0x90 0x93 0xa6	OSAGE SMALL LETTER KYA
					break;
				case 0x104BF:		//	ð’¿	0xf0 0x90 0x92 0xbf	OSAGE CAPITAL LETTER LA
					*p = 0x104E7;	//	ð“§	0xf0 0x90 0x93 0xa7	OSAGE SMALL LETTER LA
					break;
				case 0x104C0:		//	ð“€	0xf0 0x90 0x93 0x80	OSAGE CAPITAL LETTER MA
					*p = 0x104E8;	//	ð“¨	0xf0 0x90 0x93 0xa8	OSAGE SMALL LETTER MA
					break;
				case 0x104C1:		//	ð“	0xf0 0x90 0x93 0x81	OSAGE CAPITAL LETTER NA
					*p = 0x104E9;	//	ð“©	0xf0 0x90 0x93 0xa9	OSAGE SMALL LETTER NA
					break;
				case 0x104C2:		//	ð“‚	0xf0 0x90 0x93 0x82	OSAGE CAPITAL LETTER O
					*p = 0x104EA;	//	ð“ª	0xf0 0x90 0x93 0xaa	OSAGE SMALL LETTER O
					break;
				case 0x104C3:		//	ð“ƒ	0xf0 0x90 0x93 0x83	OSAGE CAPITAL LETTER OIN
					*p = 0x104EB;	//	ð“«	0xf0 0x90 0x93 0xab	OSAGE SMALL LETTER OIN
					break;
				case 0x104C4:		//	ð“„	0xf0 0x90 0x93 0x84	OSAGE CAPITAL LETTER PA
					*p = 0x104EC;	//	ð“¬	0xf0 0x90 0x93 0xac	OSAGE SMALL LETTER PA
					break;
				case 0x104C5:		//	ð“…	0xf0 0x90 0x93 0x85	OSAGE CAPITAL LETTER EHPA
					*p = 0x104ED;	//	ð“­	0xf0 0x90 0x93 0xad	OSAGE SMALL LETTER EHPA
					break;
				case 0x104C6:		//	ð“†	0xf0 0x90 0x93 0x86	OSAGE CAPITAL LETTER SA
					*p = 0x104EE;	//	ð“®	0xf0 0x90 0x93 0xae	OSAGE SMALL LETTER SA
					break;
				case 0x104C7:		//	ð“‡	0xf0 0x90 0x93 0x87	OSAGE CAPITAL LETTER SHA
					*p = 0x104EF;	//	ð“¯	0xf0 0x90 0x93 0xaf	OSAGE SMALL LETTER SHA
					break;
				case 0x104C8:		//	ð“ˆ	0xf0 0x90 0x93 0x88	OSAGE CAPITAL LETTER TA
					*p = 0x104F0;	//	ð“°	0xf0 0x90 0x93 0xb0	OSAGE SMALL LETTER TA
					break;
				case 0x104C9:		//	ð“‰	0xf0 0x90 0x93 0x89	OSAGE CAPITAL LETTER EHTA
					*p = 0x104F1;	//	ð“±	0xf0 0x90 0x93 0xb1	OSAGE SMALL LETTER EHTA
					break;
				case 0x104CA:		//	ð“Š	0xf0 0x90 0x93 0x8a	OSAGE CAPITAL LETTER TSA
					*p = 0x104F2;	//	ð“²	0xf0 0x90 0x93 0xb2	OSAGE SMALL LETTER TSA
					break;
				case 0x104CB:		//	ð“‹	0xf0 0x90 0x93 0x8b	OSAGE CAPITAL LETTER EHTSA
					*p = 0x104F3;	//	ð“³	0xf0 0x90 0x93 0xb3	OSAGE SMALL LETTER EHTSA
					break;
				case 0x104CC:		//	ð“Œ	0xf0 0x90 0x93 0x8c	OSAGE CAPITAL LETTER TSHA
					*p = 0x104F4;	//	ð“´	0xf0 0x90 0x93 0xb4	OSAGE SMALL LETTER TSHA
					break;
				case 0x104CD:		//	ð“	0xf0 0x90 0x93 0x8d	OSAGE CAPITAL LETTER DHA
					*p = 0x104F5;	//	ð“µ	0xf0 0x90 0x93 0xb5	OSAGE SMALL LETTER DHA
					break;
				case 0x104CE:		//	ð“Ž	0xf0 0x90 0x93 0x8e	OSAGE CAPITAL LETTER U
					*p = 0x104F6;	//	ð“¶	0xf0 0x90 0x93 0xb6	OSAGE SMALL LETTER U
					break;
				case 0x104CF:		//	ð“	0xf0 0x90 0x93 0x8f	OSAGE CAPITAL LETTER WA
					*p = 0x104F7;	//	ð“·	0xf0 0x90 0x93 0xb7	OSAGE SMALL LETTER WA
					break;
				case 0x104D0:		//	ð“	0xf0 0x90 0x93 0x90	OSAGE CAPITAL LETTER KHA
					*p = 0x104F8;	//	ð“¸	0xf0 0x90 0x93 0xb8	OSAGE SMALL LETTER KHA
					break;
				case 0x104D1:		//	ð“‘	0xf0 0x90 0x93 0x91	OSAGE CAPITAL LETTER GHA
					*p = 0x104F9;	//	ð“¹	0xf0 0x90 0x93 0xb9	OSAGE SMALL LETTER GHA
					break;
				case 0x104D2:		//	ð“’	0xf0 0x90 0x93 0x92	OSAGE CAPITAL LETTER ZA
					*p = 0x104FA;	//	ð“º	0xf0 0x90 0x93 0xba	OSAGE SMALL LETTER ZA
					break;
				case 0x104D3:		//	ð““	0xf0 0x90 0x93 0x93	OSAGE CAPITAL LETTER ZHA
					*p = 0x104FB;	//	ð“»	0xf0 0x90 0x93 0xbb	OSAGE SMALL LETTER ZHA
					break;
				case 0x10C80:		//	ð²€	0xf0 0x90 0xb2 0x80	OLD HUNGARIAN CAPITAL LETTER A
					*p = 0x10CC0;	//	ð³€	0xf0 0x90 0xb3 0x80	OLD HUNGARIAN SMALL LETTER A
					break;
				case 0x10C81:		//	ð²	0xf0 0x90 0xb2 0x81	OLD HUNGARIAN CAPITAL LETTER AA
					*p = 0x10CC1;	//	ð³	0xf0 0x90 0xb3 0x81	OLD HUNGARIAN SMALL LETTER AA
					break;
				case 0x10C82:		//	ð²‚	0xf0 0x90 0xb2 0x82	OLD HUNGARIAN CAPITAL LETTER EB
					*p = 0x10CC2;	//	ð³‚	0xf0 0x90 0xb3 0x82	OLD HUNGARIAN SMALL LETTER EB
					break;
				case 0x10C83:		//	ð²ƒ	0xf0 0x90 0xb2 0x83	OLD HUNGARIAN CAPITAL LETTER AMB
					*p = 0x10CC3;	//	ð³ƒ	0xf0 0x90 0xb3 0x83	OLD HUNGARIAN SMALL LETTER AMB
					break;
				case 0x10C84:		//	ð²„	0xf0 0x90 0xb2 0x84	OLD HUNGARIAN CAPITAL LETTER EC
					*p = 0x10CC4;	//	ð³„	0xf0 0x90 0xb3 0x84	OLD HUNGARIAN SMALL LETTER EC
					break;
				case 0x10C85:		//	ð²…	0xf0 0x90 0xb2 0x85	OLD HUNGARIAN CAPITAL LETTER ENC
					*p = 0x10CC5;	//	ð³…	0xf0 0x90 0xb3 0x85	OLD HUNGARIAN SMALL LETTER ENC
					break;
				case 0x10C86:		//	ð²†	0xf0 0x90 0xb2 0x86	OLD HUNGARIAN CAPITAL LETTER ECS
					*p = 0x10CC6;	//	ð³†	0xf0 0x90 0xb3 0x86	OLD HUNGARIAN SMALL LETTER ECS
					break;
				case 0x10C87:		//	ð²‡	0xf0 0x90 0xb2 0x87	OLD HUNGARIAN CAPITAL LETTER ED
					*p = 0x10CC7;	//	ð³‡	0xf0 0x90 0xb3 0x87	OLD HUNGARIAN SMALL LETTER ED
					break;
				case 0x10C88:		//	ð²ˆ	0xf0 0x90 0xb2 0x88	OLD HUNGARIAN CAPITAL LETTER AND
					*p = 0x10CC8;	//	ð³ˆ	0xf0 0x90 0xb3 0x88	OLD HUNGARIAN SMALL LETTER AND
					break;
				case 0x10C89:		//	ð²‰	0xf0 0x90 0xb2 0x89	OLD HUNGARIAN CAPITAL LETTER E
					*p = 0x10CC9;	//	ð³‰	0xf0 0x90 0xb3 0x89	OLD HUNGARIAN SMALL LETTER E
					break;
				case 0x10C8A:		//	ð²Š	0xf0 0x90 0xb2 0x8a	OLD HUNGARIAN CAPITAL LETTER CLOSE E
					*p = 0x10CCA;	//	ð³Š	0xf0 0x90 0xb3 0x8a	OLD HUNGARIAN SMALL LETTER CLOSE E
					break;
				case 0x10C8B:		//	ð²‹	0xf0 0x90 0xb2 0x8b	OLD HUNGARIAN CAPITAL LETTER EE
					*p = 0x10CCB;	//	ð³‹	0xf0 0x90 0xb3 0x8b	OLD HUNGARIAN SMALL LETTER EE
					break;
				case 0x10C8C:		//	ð²Œ	0xf0 0x90 0xb2 0x8c	OLD HUNGARIAN CAPITAL LETTER EF
					*p = 0x10CCC;	//	ð³Œ	0xf0 0x90 0xb3 0x8c	OLD HUNGARIAN SMALL LETTER EF
					break;
				case 0x10C8D:		//	ð²	0xf0 0x90 0xb2 0x8d	OLD HUNGARIAN CAPITAL LETTER EG
					*p = 0x10CCD;	//	ð³	0xf0 0x90 0xb3 0x8d	OLD HUNGARIAN SMALL LETTER EG
					break;
				case 0x10C8E:		//	ð²Ž	0xf0 0x90 0xb2 0x8e	OLD HUNGARIAN CAPITAL LETTER EGY
					*p = 0x10CCE;	//	ð³Ž	0xf0 0x90 0xb3 0x8e	OLD HUNGARIAN SMALL LETTER EGY
					break;
				case 0x10C8F:		//	ð²	0xf0 0x90 0xb2 0x8f	OLD HUNGARIAN CAPITAL LETTER EH
					*p = 0x10CCF;	//	ð³	0xf0 0x90 0xb3 0x8f	OLD HUNGARIAN SMALL LETTER EH
					break;
				case 0x10C90:		//	ð²	0xf0 0x90 0xb2 0x90	OLD HUNGARIAN CAPITAL LETTER I
					*p = 0x10CD0;	//	ð³	0xf0 0x90 0xb3 0x90	OLD HUNGARIAN SMALL LETTER I
					break;
				case 0x10C91:		//	ð²‘	0xf0 0x90 0xb2 0x91	OLD HUNGARIAN CAPITAL LETTER II
					*p = 0x10CD1;	//	ð³‘	0xf0 0x90 0xb3 0x91	OLD HUNGARIAN SMALL LETTER II
					break;
				case 0x10C92:		//	ð²’	0xf0 0x90 0xb2 0x92	OLD HUNGARIAN CAPITAL LETTER EJ
					*p = 0x10CD2;	//	ð³’	0xf0 0x90 0xb3 0x92	OLD HUNGARIAN SMALL LETTER EJ
					break;
				case 0x10C93:		//	ð²“	0xf0 0x90 0xb2 0x93	OLD HUNGARIAN CAPITAL LETTER EK
					*p = 0x10CD3;	//	ð³“	0xf0 0x90 0xb3 0x93	OLD HUNGARIAN SMALL LETTER EK
					break;
				case 0x10C94:		//	ð²”	0xf0 0x90 0xb2 0x94	OLD HUNGARIAN CAPITAL LETTER AK
					*p = 0x10CD4;	//	ð³”	0xf0 0x90 0xb3 0x94	OLD HUNGARIAN SMALL LETTER AK
					break;
				case 0x10C95:		//	ð²•	0xf0 0x90 0xb2 0x95	OLD HUNGARIAN CAPITAL LETTER UNK
					*p = 0x10CD5;	//	ð³•	0xf0 0x90 0xb3 0x95	OLD HUNGARIAN SMALL LETTER UNK
					break;
				case 0x10C96:		//	ð²–	0xf0 0x90 0xb2 0x96	OLD HUNGARIAN CAPITAL LETTER EL
					*p = 0x10CD6;	//	ð³–	0xf0 0x90 0xb3 0x96	OLD HUNGARIAN SMALL LETTER EL
					break;
				case 0x10C97:		//	ð²—	0xf0 0x90 0xb2 0x97	OLD HUNGARIAN CAPITAL LETTER ELY
					*p = 0x10CD7;	//	ð³—	0xf0 0x90 0xb3 0x97	OLD HUNGARIAN SMALL LETTER ELY
					break;
				case 0x10C98:		//	ð²˜	0xf0 0x90 0xb2 0x98	OLD HUNGARIAN CAPITAL LETTER EM
					*p = 0x10CD8;	//	ð³˜	0xf0 0x90 0xb3 0x98	OLD HUNGARIAN SMALL LETTER EM
					break;
				case 0x10C99:		//	ð²™	0xf0 0x90 0xb2 0x99	OLD HUNGARIAN CAPITAL LETTER EN
					*p = 0x10CD9;	//	ð³™	0xf0 0x90 0xb3 0x99	OLD HUNGARIAN SMALL LETTER EN
					break;
				case 0x10C9A:		//	ð²š	0xf0 0x90 0xb2 0x9a	OLD HUNGARIAN CAPITAL LETTER ENY
					*p = 0x10CDA;	//	ð³š	0xf0 0x90 0xb3 0x9a	OLD HUNGARIAN SMALL LETTER ENY
					break;
				case 0x10C9B:		//	ð²›	0xf0 0x90 0xb2 0x9b	OLD HUNGARIAN CAPITAL LETTER O
					*p = 0x10CDB;	//	ð³›	0xf0 0x90 0xb3 0x9b	OLD HUNGARIAN SMALL LETTER O
					break;
				case 0x10C9C:		//	ð²œ	0xf0 0x90 0xb2 0x9c	OLD HUNGARIAN CAPITAL LETTER OO
					*p = 0x10CDC;	//	ð³œ	0xf0 0x90 0xb3 0x9c	OLD HUNGARIAN SMALL LETTER OO
					break;
				case 0x10C9D:		//	ð²	0xf0 0x90 0xb2 0x9d	OLD HUNGARIAN CAPITAL LETTER NIKOLSBURG OE
					*p = 0x10CDD;	//	ð³	0xf0 0x90 0xb3 0x9d	OLD HUNGARIAN SMALL LETTER NIKOLSBURG OE
					break;
				case 0x10C9E:		//	ð²ž	0xf0 0x90 0xb2 0x9e	OLD HUNGARIAN CAPITAL LETTER RUDIMENTA OE
					*p = 0x10CDE;	//	ð³ž	0xf0 0x90 0xb3 0x9e	OLD HUNGARIAN SMALL LETTER RUDIMENTA OE
					break;
				case 0x10C9F:		//	ð²Ÿ	0xf0 0x90 0xb2 0x9f	OLD HUNGARIAN CAPITAL LETTER OEE
					*p = 0x10CDF;	//	ð³Ÿ	0xf0 0x90 0xb3 0x9f	OLD HUNGARIAN SMALL LETTER OEE
					break;
				case 0x10CA0:		//	ð² 	0xf0 0x90 0xb2 0xa0	OLD HUNGARIAN CAPITAL LETTER EP
					*p = 0x10CE0;	//	ð³ 	0xf0 0x90 0xb3 0xa0	OLD HUNGARIAN SMALL LETTER EP
					break;
				case 0x10CA1:		//	ð²¡	0xf0 0x90 0xb2 0xa1	OLD HUNGARIAN CAPITAL LETTER EMP
					*p = 0x10CE1;	//	ð³¡	0xf0 0x90 0xb3 0xa1	OLD HUNGARIAN SMALL LETTER EMP
					break;
				case 0x10CA2:		//	ð²¢	0xf0 0x90 0xb2 0xa2	OLD HUNGARIAN CAPITAL LETTER ER
					*p = 0x10CE2;	//	ð³¢	0xf0 0x90 0xb3 0xa2	OLD HUNGARIAN SMALL LETTER ER
					break;
				case 0x10CA3:		//	ð²£	0xf0 0x90 0xb2 0xa3	OLD HUNGARIAN CAPITAL LETTER SHORT ER
					*p = 0x10CE3;	//	ð³£	0xf0 0x90 0xb3 0xa3	OLD HUNGARIAN SMALL LETTER SHORT ER
					break;
				case 0x10CA4:		//	ð²¤	0xf0 0x90 0xb2 0xa4	OLD HUNGARIAN CAPITAL LETTER ES
					*p = 0x10CE4;	//	ð³¤	0xf0 0x90 0xb3 0xa4	OLD HUNGARIAN SMALL LETTER ES
					break;
				case 0x10CA5:		//	ð²¥	0xf0 0x90 0xb2 0xa5	OLD HUNGARIAN CAPITAL LETTER ESZ
					*p = 0x10CE5;	//	ð³¥	0xf0 0x90 0xb3 0xa5	OLD HUNGARIAN SMALL LETTER ESZ
					break;
				case 0x10CA6:		//	ð²¦	0xf0 0x90 0xb2 0xa6	OLD HUNGARIAN CAPITAL LETTER ET
					*p = 0x10CE6;	//	ð³¦	0xf0 0x90 0xb3 0xa6	OLD HUNGARIAN SMALL LETTER ET
					break;
				case 0x10CA7:		//	ð²§	0xf0 0x90 0xb2 0xa7	OLD HUNGARIAN CAPITAL LETTER ENT
					*p = 0x10CE7;	//	ð³§	0xf0 0x90 0xb3 0xa7	OLD HUNGARIAN SMALL LETTER ENT
					break;
				case 0x10CA8:		//	ð²¨	0xf0 0x90 0xb2 0xa8	OLD HUNGARIAN CAPITAL LETTER ETY
					*p = 0x10CE8;	//	ð³¨	0xf0 0x90 0xb3 0xa8	OLD HUNGARIAN SMALL LETTER ETY
					break;
				case 0x10CA9:		//	ð²©	0xf0 0x90 0xb2 0xa9	OLD HUNGARIAN CAPITAL LETTER ECH
					*p = 0x10CE9;	//	ð³©	0xf0 0x90 0xb3 0xa9	OLD HUNGARIAN SMALL LETTER ECH
					break;
				case 0x10CAA:		//	ð²ª	0xf0 0x90 0xb2 0xaa	OLD HUNGARIAN CAPITAL LETTER U
					*p = 0x10CEA;	//	ð³ª	0xf0 0x90 0xb3 0xaa	OLD HUNGARIAN SMALL LETTER U
					break;
				case 0x10CAB:		//	ð²«	0xf0 0x90 0xb2 0xab	OLD HUNGARIAN CAPITAL LETTER UU
					*p = 0x10CEB;	//	ð³«	0xf0 0x90 0xb3 0xab	OLD HUNGARIAN SMALL LETTER UU
					break;
				case 0x10CAC:		//	ð²¬	0xf0 0x90 0xb2 0xac	OLD HUNGARIAN CAPITAL LETTER NIKOLSBURG UE
					*p = 0x10CEC;	//	ð³¬	0xf0 0x90 0xb3 0xac	OLD HUNGARIAN SMALL LETTER NIKOLSBURG UE
					break;
				case 0x10CAD:		//	ð²­	0xf0 0x90 0xb2 0xad	OLD HUNGARIAN CAPITAL LETTER RUDIMENTA UE
					*p = 0x10CED;	//	ð³­	0xf0 0x90 0xb3 0xad	OLD HUNGARIAN SMALL LETTER RUDIMENTA UE
					break;
				case 0x10CAE:		//	ð²®	0xf0 0x90 0xb2 0xae	OLD HUNGARIAN CAPITAL LETTER EV
					*p = 0x10CEE;	//	ð³®	0xf0 0x90 0xb3 0xae	OLD HUNGARIAN SMALL LETTER EV
					break;
				case 0x10CAF:		//	ð²¯	0xf0 0x90 0xb2 0xaf	OLD HUNGARIAN CAPITAL LETTER EZ
					*p = 0x10CEF;	//	ð³¯	0xf0 0x90 0xb3 0xaf	OLD HUNGARIAN SMALL LETTER EZ
					break;
				case 0x10CB0:		//	ð²°	0xf0 0x90 0xb2 0xb0	OLD HUNGARIAN CAPITAL LETTER EZS
					*p = 0x10CF0;	//	ð³°	0xf0 0x90 0xb3 0xb0	OLD HUNGARIAN SMALL LETTER EZS
					break;
				case 0x10CB1:		//	ð²±	0xf0 0x90 0xb2 0xb1	OLD HUNGARIAN CAPITAL LETTER ENT-SHAPED SIGN
					*p = 0x10CF1;	//	ð³±	0xf0 0x90 0xb3 0xb1	OLD HUNGARIAN SMALL LETTER ENT-SHAPED SIGN
					break;
				case 0x10CB2:		//	ð²²	0xf0 0x90 0xb2 0xb2	OLD HUNGARIAN CAPITAL LETTER US
					*p = 0x10CF2;	//	ð³²	0xf0 0x90 0xb3 0xb2	OLD HUNGARIAN SMALL LETTER US
					break;
				case 0x118A0:		//	ð‘¢ 	0xf0 0x91 0xa2 0xa0	WARANG CITI CAPITAL LETTER NGAA
					*p = 0x118C0;	//	ð‘£€	0xf0 0x91 0xa3 0x80	WARANG CITI SMALL LETTER NGAA
					break;
				case 0x118A1:		//	ð‘¢¡	0xf0 0x91 0xa2 0xa1	WARANG CITI CAPITAL LETTER A
					*p = 0x118C1;	//	ð‘£	0xf0 0x91 0xa3 0x81	WARANG CITI SMALL LETTER A
					break;
				case 0x118A2:		//	ð‘¢¢	0xf0 0x91 0xa2 0xa2	WARANG CITI CAPITAL LETTER WI
					*p = 0x118C2;	//	ð‘£‚	0xf0 0x91 0xa3 0x82	WARANG CITI SMALL LETTER WI
					break;
				case 0x118A3:		//	ð‘¢£	0xf0 0x91 0xa2 0xa3	WARANG CITI CAPITAL LETTER YU
					*p = 0x118C3;	//	ð‘£ƒ	0xf0 0x91 0xa3 0x83	WARANG CITI SMALL LETTER YU
					break;
				case 0x118A4:		//	ð‘¢¤	0xf0 0x91 0xa2 0xa4	WARANG CITI CAPITAL LETTER YA
					*p = 0x118C4;	//	ð‘£„	0xf0 0x91 0xa3 0x84	WARANG CITI SMALL LETTER YA
					break;
				case 0x118A5:		//	ð‘¢¥	0xf0 0x91 0xa2 0xa5	WARANG CITI CAPITAL LETTER YO
					*p = 0x118C5;	//	ð‘£…	0xf0 0x91 0xa3 0x85	WARANG CITI SMALL LETTER YO
					break;
				case 0x118A6:		//	ð‘¢¦	0xf0 0x91 0xa2 0xa6	WARANG CITI CAPITAL LETTER II
					*p = 0x118C6;	//	ð‘£†	0xf0 0x91 0xa3 0x86	WARANG CITI SMALL LETTER II
					break;
				case 0x118A7:		//	ð‘¢§	0xf0 0x91 0xa2 0xa7	WARANG CITI CAPITAL LETTER UU
					*p = 0x118C7;	//	ð‘£‡	0xf0 0x91 0xa3 0x87	WARANG CITI SMALL LETTER UU
					break;
				case 0x118A8:		//	ð‘¢¨	0xf0 0x91 0xa2 0xa8	WARANG CITI CAPITAL LETTER E
					*p = 0x118C8;	//	ð‘£ˆ	0xf0 0x91 0xa3 0x88	WARANG CITI SMALL LETTER E
					break;
				case 0x118A9:		//	ð‘¢©	0xf0 0x91 0xa2 0xa9	WARANG CITI CAPITAL LETTER O
					*p = 0x118C9;	//	ð‘£‰	0xf0 0x91 0xa3 0x89	WARANG CITI SMALL LETTER O
					break;
				case 0x118AA:		//	ð‘¢ª	0xf0 0x91 0xa2 0xaa	WARANG CITI CAPITAL LETTER ANG
					*p = 0x118CA;	//	ð‘£Š	0xf0 0x91 0xa3 0x8a	WARANG CITI SMALL LETTER ANG
					break;
				case 0x118AB:		//	ð‘¢«	0xf0 0x91 0xa2 0xab	WARANG CITI CAPITAL LETTER GA
					*p = 0x118CB;	//	ð‘£‹	0xf0 0x91 0xa3 0x8b	WARANG CITI SMALL LETTER GA
					break;
				case 0x118AC:		//	ð‘¢¬	0xf0 0x91 0xa2 0xac	WARANG CITI CAPITAL LETTER KO
					*p = 0x118CC;	//	ð‘£Œ	0xf0 0x91 0xa3 0x8c	WARANG CITI SMALL LETTER KO
					break;
				case 0x118AD:		//	ð‘¢­	0xf0 0x91 0xa2 0xad	WARANG CITI CAPITAL LETTER ENY
					*p = 0x118CD;	//	ð‘£	0xf0 0x91 0xa3 0x8d	WARANG CITI SMALL LETTER ENY
					break;
				case 0x118AE:		//	ð‘¢®	0xf0 0x91 0xa2 0xae	WARANG CITI CAPITAL LETTER YUJ
					*p = 0x118CE;	//	ð‘£Ž	0xf0 0x91 0xa3 0x8e	WARANG CITI SMALL LETTER YUJ
					break;
				case 0x118AF:		//	ð‘¢¯	0xf0 0x91 0xa2 0xaf	WARANG CITI CAPITAL LETTER UC
					*p = 0x118CF;	//	ð‘£	0xf0 0x91 0xa3 0x8f	WARANG CITI SMALL LETTER UC
					break;
				case 0x118B0:		//	ð‘¢°	0xf0 0x91 0xa2 0xb0	WARANG CITI CAPITAL LETTER ENN
					*p = 0x118D0;	//	ð‘£	0xf0 0x91 0xa3 0x90	WARANG CITI SMALL LETTER ENN
					break;
				case 0x118B1:		//	ð‘¢±	0xf0 0x91 0xa2 0xb1	WARANG CITI CAPITAL LETTER ODD
					*p = 0x118D1;	//	ð‘£‘	0xf0 0x91 0xa3 0x91	WARANG CITI SMALL LETTER ODD
					break;
				case 0x118B2:		//	ð‘¢²	0xf0 0x91 0xa2 0xb2	WARANG CITI CAPITAL LETTER TTE
					*p = 0x118D2;	//	ð‘£’	0xf0 0x91 0xa3 0x92	WARANG CITI SMALL LETTER TTE
					break;
				case 0x118B3:		//	ð‘¢³	0xf0 0x91 0xa2 0xb3	WARANG CITI CAPITAL LETTER NUNG
					*p = 0x118D3;	//	ð‘£“	0xf0 0x91 0xa3 0x93	WARANG CITI SMALL LETTER NUNG
					break;
				case 0x118B4:		//	ð‘¢´	0xf0 0x91 0xa2 0xb4	WARANG CITI CAPITAL LETTER DA
					*p = 0x118D4;	//	ð‘£”	0xf0 0x91 0xa3 0x94	WARANG CITI SMALL LETTER DA
					break;
				case 0x118B5:		//	ð‘¢µ	0xf0 0x91 0xa2 0xb5	WARANG CITI CAPITAL LETTER AT
					*p = 0x118D5;	//	ð‘£•	0xf0 0x91 0xa3 0x95	WARANG CITI SMALL LETTER AT
					break;
				case 0x118B6:		//	ð‘¢¶	0xf0 0x91 0xa2 0xb6	WARANG CITI CAPITAL LETTER AM
					*p = 0x118D6;	//	ð‘£–	0xf0 0x91 0xa3 0x96	WARANG CITI SMALL LETTER AM
					break;
				case 0x118B7:		//	ð‘¢·	0xf0 0x91 0xa2 0xb7	WARANG CITI CAPITAL LETTER BU
					*p = 0x118D7;	//	ð‘£—	0xf0 0x91 0xa3 0x97	WARANG CITI SMALL LETTER BU
					break;
				case 0x118B8:		//	ð‘¢¸	0xf0 0x91 0xa2 0xb8	WARANG CITI CAPITAL LETTER PU
					*p = 0x118D8;	//	ð‘£˜	0xf0 0x91 0xa3 0x98	WARANG CITI SMALL LETTER PU
					break;
				case 0x118B9:		//	ð‘¢¹	0xf0 0x91 0xa2 0xb9	WARANG CITI CAPITAL LETTER HIYO
					*p = 0x118D9;	//	ð‘£™	0xf0 0x91 0xa3 0x99	WARANG CITI SMALL LETTER HIYO
					break;
				case 0x118BA:		//	ð‘¢º	0xf0 0x91 0xa2 0xba	WARANG CITI CAPITAL LETTER HOLO
					*p = 0x118DA;	//	ð‘£š	0xf0 0x91 0xa3 0x9a	WARANG CITI SMALL LETTER HOLO
					break;
				case 0x118BB:		//	ð‘¢»	0xf0 0x91 0xa2 0xbb	WARANG CITI CAPITAL LETTER HORR
					*p = 0x118DB;	//	ð‘£›	0xf0 0x91 0xa3 0x9b	WARANG CITI SMALL LETTER HORR
					break;
				case 0x118BC:		//	ð‘¢¼	0xf0 0x91 0xa2 0xbc	WARANG CITI CAPITAL LETTER HAR
					*p = 0x118DC;	//	ð‘£œ	0xf0 0x91 0xa3 0x9c	WARANG CITI SMALL LETTER HAR
					break;
				case 0x118BD:		//	ð‘¢½	0xf0 0x91 0xa2 0xbd	WARANG CITI CAPITAL LETTER SSUU
					*p = 0x118DD;	//	ð‘£	0xf0 0x91 0xa3 0x9d	WARANG CITI SMALL LETTER SSUU
					break;
				case 0x118BE:		//	ð‘¢¾	0xf0 0x91 0xa2 0xbe	WARANG CITI CAPITAL LETTER SII
					*p = 0x118DE;	//	ð‘£ž	0xf0 0x91 0xa3 0x9e	WARANG CITI SMALL LETTER SII
					break;
				case 0x118BF:		//	ð‘¢¿	0xf0 0x91 0xa2 0xbf	WARANG CITI CAPITAL LETTER VIYO
					*p = 0x118DF;	//	ð‘£Ÿ	0xf0 0x91 0xa3 0x9f	WARANG CITI SMALL LETTER VIYO
					break;
				case 0x16E40:		//	ð–¹€	0xf0 0x96 0xb9 0x80	MEDEFAIDRIN CAPITAL LETTER M
					*p = 0x16E60;	//	ð–¹ 	0xf0 0x96 0xb9 0xa0	MEDEFAIDRIN SMALL LETTER M
					break;
				case 0x16E41:		//	ð–¹	0xf0 0x96 0xb9 0x81	MEDEFAIDRIN CAPITAL LETTER S
					*p = 0x16E61;	//	ð–¹¡	0xf0 0x96 0xb9 0xa1	MEDEFAIDRIN SMALL LETTER S
					break;
				case 0x16E42:		//	ð–¹‚	0xf0 0x96 0xb9 0x82	MEDEFAIDRIN CAPITAL LETTER V
					*p = 0x16E62;	//	ð–¹¢	0xf0 0x96 0xb9 0xa2	MEDEFAIDRIN SMALL LETTER V
					break;
				case 0x16E43:		//	ð–¹ƒ	0xf0 0x96 0xb9 0x83	MEDEFAIDRIN CAPITAL LETTER W
					*p = 0x16E63;	//	ð–¹£	0xf0 0x96 0xb9 0xa3	MEDEFAIDRIN SMALL LETTER W
					break;
				case 0x16E44:		//	ð–¹„	0xf0 0x96 0xb9 0x84	MEDEFAIDRIN CAPITAL LETTER ATIU
					*p = 0x16E64;	//	ð–¹¤	0xf0 0x96 0xb9 0xa4	MEDEFAIDRIN SMALL LETTER ATIU
					break;
				case 0x16E45:		//	ð–¹…	0xf0 0x96 0xb9 0x85	MEDEFAIDRIN CAPITAL LETTER Z
					*p = 0x16E65;	//	ð–¹¥	0xf0 0x96 0xb9 0xa5	MEDEFAIDRIN SMALL LETTER Z
					break;
				case 0x16E46:		//	ð–¹†	0xf0 0x96 0xb9 0x86	MEDEFAIDRIN CAPITAL LETTER KP
					*p = 0x16E66;	//	ð–¹¦	0xf0 0x96 0xb9 0xa6	MEDEFAIDRIN SMALL LETTER KP
					break;
				case 0x16E47:		//	ð–¹‡	0xf0 0x96 0xb9 0x87	MEDEFAIDRIN CAPITAL LETTER P
					*p = 0x16E67;	//	ð–¹§	0xf0 0x96 0xb9 0xa7	MEDEFAIDRIN SMALL LETTER P
					break;
				case 0x16E48:		//	ð–¹ˆ	0xf0 0x96 0xb9 0x88	MEDEFAIDRIN CAPITAL LETTER T
					*p = 0x16E68;	//	ð–¹¨	0xf0 0x96 0xb9 0xa8	MEDEFAIDRIN SMALL LETTER T
					break;
				case 0x16E49:		//	ð–¹‰	0xf0 0x96 0xb9 0x89	MEDEFAIDRIN CAPITAL LETTER G
					*p = 0x16E69;	//	ð–¹©	0xf0 0x96 0xb9 0xa9	MEDEFAIDRIN SMALL LETTER G
					break;
				case 0x16E4A:		//	ð–¹Š	0xf0 0x96 0xb9 0x8a	MEDEFAIDRIN CAPITAL LETTER F
					*p = 0x16E6A;	//	ð–¹ª	0xf0 0x96 0xb9 0xaa	MEDEFAIDRIN SMALL LETTER F
					break;
				case 0x16E4B:		//	ð–¹‹	0xf0 0x96 0xb9 0x8b	MEDEFAIDRIN CAPITAL LETTER I
					*p = 0x16E6B;	//	ð–¹«	0xf0 0x96 0xb9 0xab	MEDEFAIDRIN SMALL LETTER I
					break;
				case 0x16E4C:		//	ð–¹Œ	0xf0 0x96 0xb9 0x8c	MEDEFAIDRIN CAPITAL LETTER K
					*p = 0x16E6C;	//	ð–¹¬	0xf0 0x96 0xb9 0xac	MEDEFAIDRIN SMALL LETTER K
					break;
				case 0x16E4D:		//	ð–¹	0xf0 0x96 0xb9 0x8d	MEDEFAIDRIN CAPITAL LETTER A
					*p = 0x16E6D;	//	ð–¹­	0xf0 0x96 0xb9 0xad	MEDEFAIDRIN SMALL LETTER A
					break;
				case 0x16E4E:		//	ð–¹Ž	0xf0 0x96 0xb9 0x8e	MEDEFAIDRIN CAPITAL LETTER J
					*p = 0x16E6E;	//	ð–¹®	0xf0 0x96 0xb9 0xae	MEDEFAIDRIN SMALL LETTER J
					break;
				case 0x16E4F:		//	ð–¹	0xf0 0x96 0xb9 0x8f	MEDEFAIDRIN CAPITAL LETTER E
					*p = 0x16E6F;	//	ð–¹¯	0xf0 0x96 0xb9 0xaf	MEDEFAIDRIN SMALL LETTER E
					break;
				case 0x16E50:		//	ð–¹	0xf0 0x96 0xb9 0x90	MEDEFAIDRIN CAPITAL LETTER B
					*p = 0x16E70;	//	ð–¹°	0xf0 0x96 0xb9 0xb0	MEDEFAIDRIN SMALL LETTER B
					break;
				case 0x16E51:		//	ð–¹‘	0xf0 0x96 0xb9 0x91	MEDEFAIDRIN CAPITAL LETTER C
					*p = 0x16E71;	//	ð–¹±	0xf0 0x96 0xb9 0xb1	MEDEFAIDRIN SMALL LETTER C
					break;
				case 0x16E52:		//	ð–¹’	0xf0 0x96 0xb9 0x92	MEDEFAIDRIN CAPITAL LETTER U
					*p = 0x16E72;	//	ð–¹²	0xf0 0x96 0xb9 0xb2	MEDEFAIDRIN SMALL LETTER U
					break;
				case 0x16E53:		//	ð–¹“	0xf0 0x96 0xb9 0x93	MEDEFAIDRIN CAPITAL LETTER YU
					*p = 0x16E73;	//	ð–¹³	0xf0 0x96 0xb9 0xb3	MEDEFAIDRIN SMALL LETTER YU
					break;
				case 0x16E54:		//	ð–¹”	0xf0 0x96 0xb9 0x94	MEDEFAIDRIN CAPITAL LETTER L
					*p = 0x16E74;	//	ð–¹´	0xf0 0x96 0xb9 0xb4	MEDEFAIDRIN SMALL LETTER L
					break;
				case 0x16E55:		//	ð–¹•	0xf0 0x96 0xb9 0x95	MEDEFAIDRIN CAPITAL LETTER Q
					*p = 0x16E75;	//	ð–¹µ	0xf0 0x96 0xb9 0xb5	MEDEFAIDRIN SMALL LETTER Q
					break;
				case 0x16E56:		//	ð–¹–	0xf0 0x96 0xb9 0x96	MEDEFAIDRIN CAPITAL LETTER HP
					*p = 0x16E76;	//	ð–¹¶	0xf0 0x96 0xb9 0xb6	MEDEFAIDRIN SMALL LETTER HP
					break;
				case 0x16E57:		//	ð–¹—	0xf0 0x96 0xb9 0x97	MEDEFAIDRIN CAPITAL LETTER NY
					*p = 0x16E77;	//	ð–¹·	0xf0 0x96 0xb9 0xb7	MEDEFAIDRIN SMALL LETTER NY
					break;
				case 0x16E58:		//	ð–¹˜	0xf0 0x96 0xb9 0x98	MEDEFAIDRIN CAPITAL LETTER X
					*p = 0x16E78;	//	ð–¹¸	0xf0 0x96 0xb9 0xb8	MEDEFAIDRIN SMALL LETTER X
					break;
				case 0x16E59:		//	ð–¹™	0xf0 0x96 0xb9 0x99	MEDEFAIDRIN CAPITAL LETTER D
					*p = 0x16E79;	//	ð–¹¹	0xf0 0x96 0xb9 0xb9	MEDEFAIDRIN SMALL LETTER D
					break;
				case 0x16E5A:		//	ð–¹š	0xf0 0x96 0xb9 0x9a	MEDEFAIDRIN CAPITAL LETTER OE
					*p = 0x16E7A;	//	ð–¹º	0xf0 0x96 0xb9 0xba	MEDEFAIDRIN SMALL LETTER OE
					break;
				case 0x16E5B:		//	ð–¹›	0xf0 0x96 0xb9 0x9b	MEDEFAIDRIN CAPITAL LETTER N
					*p = 0x16E7B;	//	ð–¹»	0xf0 0x96 0xb9 0xbb	MEDEFAIDRIN SMALL LETTER N
					break;
				case 0x16E5C:		//	ð–¹œ	0xf0 0x96 0xb9 0x9c	MEDEFAIDRIN CAPITAL LETTER R
					*p = 0x16E7C;	//	ð–¹¼	0xf0 0x96 0xb9 0xbc	MEDEFAIDRIN SMALL LETTER R
					break;
				case 0x16E5D:		//	ð–¹	0xf0 0x96 0xb9 0x9d	MEDEFAIDRIN CAPITAL LETTER O
					*p = 0x16E7D;	//	ð–¹½	0xf0 0x96 0xb9 0xbd	MEDEFAIDRIN SMALL LETTER O
					break;
				case 0x16E5E:		//	ð–¹ž	0xf0 0x96 0xb9 0x9e	MEDEFAIDRIN CAPITAL LETTER AI
					*p = 0x16E7E;	//	ð–¹¾	0xf0 0x96 0xb9 0xbe	MEDEFAIDRIN SMALL LETTER AI
					break;
				case 0x16E5F:		//	ð–¹Ÿ	0xf0 0x96 0xb9 0x9f	MEDEFAIDRIN CAPITAL LETTER Y
					*p = 0x16E7F;	//	ð–¹¿	0xf0 0x96 0xb9 0xbf	MEDEFAIDRIN SMALL LETTER Y
					break;
				case 0x1E900:		//	ðž¤€	0xf0 0x9e 0xa4 0x80	ADLAM CAPITAL LETTER ALIF
					*p = 0x1E922;	//	ðž¤¢	0xf0 0x9e 0xa4 0xa2	ADLAM SMALL LETTER ALIF
					break;
				case 0x1E901:		//	ðž¤	0xf0 0x9e 0xa4 0x81	ADLAM CAPITAL LETTER DAALI
					*p = 0x1E923;	//	ðž¤£	0xf0 0x9e 0xa4 0xa3	ADLAM SMALL LETTER DAALI
					break;
				case 0x1E902:		//	ðž¤‚	0xf0 0x9e 0xa4 0x82	ADLAM CAPITAL LETTER LAAM
					*p = 0x1E924;	//	ðž¤¤	0xf0 0x9e 0xa4 0xa4	ADLAM SMALL LETTER LAAM
					break;
				case 0x1E903:		//	ðž¤ƒ	0xf0 0x9e 0xa4 0x83	ADLAM CAPITAL LETTER MIIM
					*p = 0x1E925;	//	ðž¤¥	0xf0 0x9e 0xa4 0xa5	ADLAM SMALL LETTER MIIM
					break;
				case 0x1E904:		//	ðž¤„	0xf0 0x9e 0xa4 0x84	ADLAM CAPITAL LETTER BA
					*p = 0x1E926;	//	ðž¤¦	0xf0 0x9e 0xa4 0xa6	ADLAM SMALL LETTER BA
					break;
				case 0x1E905:		//	ðž¤…	0xf0 0x9e 0xa4 0x85	ADLAM CAPITAL LETTER SINNYIIYHE
					*p = 0x1E927;	//	ðž¤§	0xf0 0x9e 0xa4 0xa7	ADLAM SMALL LETTER SINNYIIYHE
					break;
				case 0x1E906:		//	ðž¤†	0xf0 0x9e 0xa4 0x86	ADLAM CAPITAL LETTER PE
					*p = 0x1E928;	//	ðž¤¨	0xf0 0x9e 0xa4 0xa8	ADLAM SMALL LETTER PE
					break;
				case 0x1E907:		//	ðž¤‡	0xf0 0x9e 0xa4 0x87	ADLAM CAPITAL LETTER BHE
					*p = 0x1E929;	//	ðž¤©	0xf0 0x9e 0xa4 0xa9	ADLAM SMALL LETTER BHE
					break;
				case 0x1E908:		//	ðž¤ˆ	0xf0 0x9e 0xa4 0x88	ADLAM CAPITAL LETTER RA
					*p = 0x1E92A;	//	ðž¤ª	0xf0 0x9e 0xa4 0xaa	ADLAM SMALL LETTER RA
					break;
				case 0x1E909:		//	ðž¤‰	0xf0 0x9e 0xa4 0x89	ADLAM CAPITAL LETTER E
					*p = 0x1E92B;	//	ðž¤«	0xf0 0x9e 0xa4 0xab	ADLAM SMALL LETTER E
					break;
				case 0x1E90A:		//	ðž¤Š	0xf0 0x9e 0xa4 0x8a	ADLAM CAPITAL LETTER FA
					*p = 0x1E92C;	//	ðž¤¬	0xf0 0x9e 0xa4 0xac	ADLAM SMALL LETTER FA
					break;
				case 0x1E90B:		//	ðž¤‹	0xf0 0x9e 0xa4 0x8b	ADLAM CAPITAL LETTER I
					*p = 0x1E92D;	//	ðž¤­	0xf0 0x9e 0xa4 0xad	ADLAM SMALL LETTER I
					break;
				case 0x1E90C:		//	ðž¤Œ	0xf0 0x9e 0xa4 0x8c	ADLAM CAPITAL LETTER O
					*p = 0x1E92E;	//	ðž¤®	0xf0 0x9e 0xa4 0xae	ADLAM SMALL LETTER O
					break;
				case 0x1E90D:		//	ðž¤	0xf0 0x9e 0xa4 0x8d	ADLAM CAPITAL LETTER DHA
					*p = 0x1E92F;	//	ðž¤¯	0xf0 0x9e 0xa4 0xaf	ADLAM SMALL LETTER DHA
					break;
				case 0x1E90E:		//	ðž¤Ž	0xf0 0x9e 0xa4 0x8e	ADLAM CAPITAL LETTER YHE
					*p = 0x1E930;	//	ðž¤°	0xf0 0x9e 0xa4 0xb0	ADLAM SMALL LETTER YHE
					break;
				case 0x1E90F:		//	ðž¤	0xf0 0x9e 0xa4 0x8f	ADLAM CAPITAL LETTER WAW
					*p = 0x1E931;	//	ðž¤±	0xf0 0x9e 0xa4 0xb1	ADLAM SMALL LETTER WAW
					break;
				case 0x1E910:		//	ðž¤	0xf0 0x9e 0xa4 0x90	ADLAM CAPITAL LETTER NUN
					*p = 0x1E932;	//	ðž¤²	0xf0 0x9e 0xa4 0xb2	ADLAM SMALL LETTER NUN
					break;
				case 0x1E911:		//	ðž¤‘	0xf0 0x9e 0xa4 0x91	ADLAM CAPITAL LETTER KAF
					*p = 0x1E933;	//	ðž¤³	0xf0 0x9e 0xa4 0xb3	ADLAM SMALL LETTER KAF
					break;
				case 0x1E912:		//	ðž¤’	0xf0 0x9e 0xa4 0x92	ADLAM CAPITAL LETTER YA
					*p = 0x1E934;	//	ðž¤´	0xf0 0x9e 0xa4 0xb4	ADLAM SMALL LETTER YA
					break;
				case 0x1E913:		//	ðž¤“	0xf0 0x9e 0xa4 0x93	ADLAM CAPITAL LETTER U
					*p = 0x1E935;	//	ðž¤µ	0xf0 0x9e 0xa4 0xb5	ADLAM SMALL LETTER U
					break;
				case 0x1E914:		//	ðž¤”	0xf0 0x9e 0xa4 0x94	ADLAM CAPITAL LETTER JIIM
					*p = 0x1E936;	//	ðž¤¶	0xf0 0x9e 0xa4 0xb6	ADLAM SMALL LETTER JIIM
					break;
				case 0x1E915:		//	ðž¤•	0xf0 0x9e 0xa4 0x95	ADLAM CAPITAL LETTER CHI
					*p = 0x1E937;	//	ðž¤·	0xf0 0x9e 0xa4 0xb7	ADLAM SMALL LETTER CHI
					break;
				case 0x1E916:		//	ðž¤–	0xf0 0x9e 0xa4 0x96	ADLAM CAPITAL LETTER HA
					*p = 0x1E938;	//	ðž¤¸	0xf0 0x9e 0xa4 0xb8	ADLAM SMALL LETTER HA
					break;
				case 0x1E917:		//	ðž¤—	0xf0 0x9e 0xa4 0x97	ADLAM CAPITAL LETTER QAAF
					*p = 0x1E939;	//	ðž¤¹	0xf0 0x9e 0xa4 0xb9	ADLAM SMALL LETTER QAAF
					break;
				case 0x1E918:		//	ðž¤˜	0xf0 0x9e 0xa4 0x98	ADLAM CAPITAL LETTER GA
					*p = 0x1E93A;	//	ðž¤º	0xf0 0x9e 0xa4 0xba	ADLAM SMALL LETTER GA
					break;
				case 0x1E919:		//	ðž¤™	0xf0 0x9e 0xa4 0x99	ADLAM CAPITAL LETTER NYA
					*p = 0x1E93B;	//	ðž¤»	0xf0 0x9e 0xa4 0xbb	ADLAM SMALL LETTER NYA
					break;
				case 0x1E91A:		//	ðž¤š	0xf0 0x9e 0xa4 0x9a	ADLAM CAPITAL LETTER TU
					*p = 0x1E93C;	//	ðž¤¼	0xf0 0x9e 0xa4 0xbc	ADLAM SMALL LETTER TU
					break;
				case 0x1E91B:		//	ðž¤›	0xf0 0x9e 0xa4 0x9b	ADLAM CAPITAL LETTER NHA
					*p = 0x1E93D;	//	ðž¤½	0xf0 0x9e 0xa4 0xbd	ADLAM SMALL LETTER NHA
					break;
				case 0x1E91C:		//	ðž¤œ	0xf0 0x9e 0xa4 0x9c	ADLAM CAPITAL LETTER VA
					*p = 0x1E93E;	//	ðž¤¾	0xf0 0x9e 0xa4 0xbe	ADLAM SMALL LETTER VA
					break;
				case 0x1E91D:		//	ðž¤	0xf0 0x9e 0xa4 0x9d	ADLAM CAPITAL LETTER KHA
					*p = 0x1E93F;	//	ðž¤¿	0xf0 0x9e 0xa4 0xbf	ADLAM SMALL LETTER KHA
					break;
				case 0x1E91E:		//	ðž¤ž	0xf0 0x9e 0xa4 0x9e	ADLAM CAPITAL LETTER GBE
					*p = 0x1E940;	//	ðž¥€	0xf0 0x9e 0xa5 0x80	ADLAM SMALL LETTER GBE
					break;
				case 0x1E91F:		//	ðž¤Ÿ	0xf0 0x9e 0xa4 0x9f	ADLAM CAPITAL LETTER ZAL
					*p = 0x1E941;	//	ðž¥	0xf0 0x9e 0xa5 0x81	ADLAM SMALL LETTER ZAL
					break;
				case 0x1E920:		//	ðž¤ 	0xf0 0x9e 0xa4 0xa0	ADLAM CAPITAL LETTER KPO
					*p = 0x1E942;	//	ðž¥‚	0xf0 0x9e 0xa5 0x82	ADLAM SMALL LETTER KPO
					break;
				case 0x1E921:		//	ðž¤¡	0xf0 0x9e 0xa4 0xa1	ADLAM CAPITAL LETTER SHA
					*p = 0x1E943;	//	ðž¥ƒ	0xf0 0x9e 0xa5 0x83	ADLAM SMALL LETTER SHA
					break;
				}
			}
			p++;
		}
	}
	return pUtf32;
}
/*******************************************************************************************

UTF32

*******************************************************************************************/
size_t StrLenUtf32(const Utf32Char* str)
{
	Utf32Char* s;
	for (s = (Utf32Char*)str; *s; ++s);
	return(s - str);
}
Utf32Char* StrCpyUtf32(Utf32Char* dest, const Utf32Char* src)
{
	return (Utf32Char*)memcpy(dest, src, (StrLenUtf32(src) + 1) * sizeof(Utf32Char));
}
Utf32Char* StrCatUtf32(Utf32Char* dest, const Utf32Char* src)
{
	return StrCpyUtf32(dest + StrLenUtf32(dest), src);
}
int StrnCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2, size_t ztCount)
{
	if (Utf32s1 && *Utf32s1 && Utf32s2 && *Utf32s2) {
		for (; ztCount--; Utf32s1++, Utf32s2++) {
			int iDiff = *Utf32s1 - *Utf32s2;
			if (iDiff != 0 || !*Utf32s1 || !*Utf32s2)
				return iDiff;
		}
		return 0;
	}
	return (-1);
}
int StrnCiCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2, size_t ztCount)
{
	Utf32Char* pStr1Low = 0;
	Utf32Char* pStr2Low = 0;
	int iResult = (-1);

	if (Utf32s1 && *Utf32s1 && Utf32s2 && *Utf32s2) {
		pStr1Low = (Utf32Char*)calloc(StrLenUtf32(Utf32s1) + 1, sizeof(Utf32Char));
		if (pStr1Low) {
			pStr2Low = (Utf32Char*)calloc(StrLenUtf32(Utf32s2) + 1, sizeof(Utf32Char));
			if (pStr2Low) {
				memcpy((Utf32Char*)pStr1Low, Utf32s1, StrLenUtf32(Utf32s1) * sizeof(Utf32Char));
				memcpy((Utf32Char*)pStr2Low, Utf32s2, StrLenUtf32(Utf32s2) * sizeof(Utf32Char));
				StrToLwrUtf32(pStr1Low);
				StrToLwrUtf32(pStr2Low);
				iResult = StrnCmpUtf32(pStr1Low, pStr2Low, ztCount);
				free(pStr1Low);
				free(pStr2Low);
			}
			else
				free(pStr1Low);
		}
	}
	return iResult;
}
int StrCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2)
{
	return StrnCmpUtf32(Utf32s1, Utf32s2, (size_t)(-1));
}
int StrCiCmpUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2)
{
	return StrnCiCmpUtf32(Utf32s1, Utf32s2, (size_t)(-1));
}
Utf32Char* StrCiStrUtf32(const Utf32Char* Utf32s1, const Utf32Char* Utf32s2)
{
	Utf32Char* p = (Utf32Char*)Utf32s1;
	size_t len = 0;

	if (Utf32s1 && *Utf32s1 && Utf32s2 && *Utf32s2) {
		len = StrLenUtf32(Utf32s2);
		while (*p) {
			if (StrnCiCmpUtf32(p, Utf32s2, len) == 0)
				return (Utf32Char*)p;
			p++;
		}
	}
	return (0);
}
/*******************************************************************************************

UTF16

*******************************************************************************************/
size_t StrLenUtf16(const Utf16Char* str)
{
	Utf16Char* s;
	for (s = (Utf16Char*)str; *s; ++s);
	return(s - str);
}
Utf16Char* StrCpyUtf16(Utf16Char* dest, const Utf16Char* src)
{
	return (Utf16Char*)memcpy(dest, src, (StrLenUtf16(src) + 1) * sizeof(Utf16Char));
}
Utf16Char* StrCatUtf16(Utf16Char* dest, const Utf16Char* src)
{
	return StrCpyUtf16(dest + StrLenUtf16(dest), src);
}
int StrnCmpUtf16(const Utf16Char* Utf16s1, const Utf16Char* Utf16s2, size_t ztCount)
{
	if (Utf16s1 && *Utf16s1 && Utf16s2 && *Utf16s2) {
		for (; ztCount--; Utf16s1++, Utf16s2++) {
			int iDiff = (int)(*Utf16s1 - *Utf16s2);
			if (iDiff != 0 || !*Utf16s1 || !*Utf16s2)
				return iDiff;
		}
		return 0;
	}
	return (-1);
}
int StrCmpUtf16(const Utf16Char* Utf16s1, const Utf16Char* Utf16s2)
{
	return StrnCmpUtf16(Utf16s1, Utf16s2, (size_t)(-1));
}
size_t CharLenUtf16(const Utf16Char* pUtf16)
{
	size_t ztUtf16 = 0;
	size_t ztNumUtf16Chars = 0;

	while (pUtf16[ztUtf16] != 0) {
		ztNumUtf16Chars++;

		if (!(((uint16_t)pUtf16[ztUtf16] - 0xd800u) < 0x800u))
			// 1 byte code point
			ztUtf16 += 1;
		else if ((((uint16_t)pUtf16[ztUtf16] & 0xfffffc00u) == 0xd800u) && ((((uint16_t)pUtf16[ztUtf16 + 1] & 0xfffffc00u) == 0xdc00u)))
			// 2 byte code point
			ztUtf16 += 2;
	}
	return ztNumUtf16Chars;
}
Utf16Char* ForwardUtf16Chars(const Utf16Char* pUtf16, size_t ztForwardUtf16Chars)
{
	size_t ztUtf16 = 0;
	size_t ztNumUtf16Chars = 0;

	while ((ztNumUtf16Chars < ztForwardUtf16Chars) && ((uint16_t)pUtf16[ztUtf16] != 0)) {
		ztNumUtf16Chars++;
		if (!(((uint16_t)pUtf16[ztUtf16] - 0xd800u) < 0x800u))
			// 1 byte code point
			ztUtf16 += 1;
		else if ((((uint16_t)pUtf16[ztUtf16] & 0xfffffc00u) == 0xd800u) && ((((uint16_t)pUtf16[ztUtf16 + 1] & 0xfffffc00u) == 0xdc00u)))
			// 2 byte code point
			ztUtf16 += 2;
	}
	return (Utf16Char*)&pUtf16[ztUtf16];
}
size_t StrLenUtf32AsUtf16(const Utf32Char* pUtf32)
{
	size_t ztUtf32 = 0;
	size_t ztNumUtf16Chars = 0;
	while (pUtf32[ztUtf32] != 0) {
		if ((pUtf32[ztUtf32] >= 0x10000u)
			&& (pUtf32[ztUtf32] <= 0x10FFFFu))
			// 2 byte code point
			ztNumUtf16Chars++;
		ztNumUtf16Chars++;
		ztUtf32++;
	}
	return ztNumUtf16Chars;
}
Utf16Char* Utf32ToUtf16(const Utf32Char* pUtf32)
{
	size_t ztUtf32 = 0;
	Utf16Char* pUtf16 = (Utf16Char*)calloc(StrLenUtf32AsUtf16(pUtf32) + 1, sizeof(Utf16Char));
	if (pUtf16) {
		// https://datatracker.ietf.org/doc/html/rfc3629#page-4
		// This function skips data when UTF bytes are out of bounds
		while (pUtf32[ztUtf32] != 0) {
			if (pUtf32[ztUtf32] < 0x10000u)
				pUtf16[StrLenUtf16((const Utf16Char*)pUtf16)] = (Utf16Char)pUtf32[ztUtf32];
			else if (pUtf32[ztUtf32] <= 0x10FFFFu) {
				pUtf16[StrLenUtf16((const Utf16Char*)pUtf16)] = (Utf16Char)(((((uint32_t)pUtf32[ztUtf32] - 0x10000u) << 12) >> 22) + 0xD800u);
				pUtf16[StrLenUtf16((const Utf16Char*)pUtf16)] = (Utf16Char)(((((uint32_t)pUtf32[ztUtf32] - 0x10000u) << 22) >> 22) + 0xDC00u);
			}
			// else out of RFC3629 Utf16 bounds doing nothing
			ztUtf32++;
		}
		return pUtf16;
	}
	return 0;
}
Utf32Char* Utf16ToUtf32(const Utf16Char* pUtf16)
{
	size_t ztUtf16 = 0;
	size_t ztUtf32 = 0;
	Utf32Char* pUtf32 = (Utf32Char*)calloc(CharLenUtf16(pUtf16) + 1, sizeof(Utf32Char));
	if (pUtf32) {
		// https://datatracker.ietf.org/doc/html/rfc3629#page-4
		// This function skips data when UTF bytes are out of bounds
		while ((uint16_t)pUtf16[ztUtf16] != 0) {
			if (!(((uint16_t)pUtf16[ztUtf16] - 0xd800u) < 0x800u)) { // The same just copy
				pUtf32[ztUtf32] = (uint16_t)pUtf16[ztUtf16];
				ztUtf16++;
			}
			else if ((((uint16_t)pUtf16[ztUtf16] & 0xfffffc00u) == 0xd800u) && ((((uint16_t)pUtf16[ztUtf16 + 1] & 0xfffffc00u) == 0xdc00u))) {
				pUtf32[ztUtf32] = ((uint16_t)pUtf16[ztUtf16] << 10) + (uint16_t)pUtf16[ztUtf16 + 1] - 0x35fdc00u;
				ztUtf16 += 2;
			}
			// else ERROR
			ztUtf32++;
		}
		return pUtf32;
	}
	return 0;
}
Utf8Char* Utf16ToUtf8(const Utf16Char* pUtf16)
{
	Utf32Char* pUtf32 = Utf16ToUtf32((const Utf16Char*)pUtf16);
	if (pUtf32) {
		Utf8Char* pUtf8 = Utf32ToUtf8((const Utf32Char*)pUtf32);
		free(pUtf32);
		if (pUtf8)
			return pUtf8;
		return 0;
	}
	return 0;
}
Utf16Char* Utf16StrMakeUprUtf16Str(const Utf16Char* pUtf16)
{
	Utf32Char* pUtf32 = Utf16ToUtf32(pUtf16);
	if (pUtf32 && *pUtf32) {
		StrToUprUtf32(pUtf32);
		{
			Utf16Char* pUtf16Upr = Utf32ToUtf16(pUtf32);
			free(pUtf32);
			return pUtf16Upr;
		}
	}
	return 0; // calloc() failure
}
Utf16Char* Utf16StrMakeLwrUtf16Str(const Utf16Char* pUtf16)
{
	Utf32Char* pUtf32 = Utf16ToUtf32(pUtf16);
	if (pUtf32 && *pUtf32) {
		StrToLwrUtf32(pUtf32);
		{
			Utf16Char* pUtf16Lwr = Utf32ToUtf16(pUtf32);
			free(pUtf32);
			return pUtf16Lwr;
		}
	}
	return 0; // calloc() failure
}
int StrnCiCmpUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2, size_t ztCount)
{
	Utf32Char* pUtf32s1 = Utf16ToUtf32(pUtf16s1);
	if (pUtf32s1) {
		Utf32Char* pUtf32s2 = Utf16ToUtf32(pUtf16s2);
		if (pUtf32s2) {
			int iDiff = StrnCiCmpUtf32(pUtf32s1, pUtf32s2, ztCount);
			free(pUtf32s1);
			free(pUtf32s2);
			return iDiff;
		}
		free(pUtf32s1);
		return (-1); // calloc() failure
	}
	return (-1); // calloc() failure
}
int StrCiCmpUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2)
{
	return StrnCiCmpUtf16(pUtf16s1, pUtf16s2, (size_t)(-1));
}
Utf16Char* StrCiStrUtf16(const Utf16Char* pUtf16s1, const Utf16Char* pUtf16s2)
{
	Utf32Char* pUtf32s1 = Utf16ToUtf32(pUtf16s1);
	if (pUtf32s1) {
		Utf32Char* pUtf32s2 = Utf16ToUtf32(pUtf16s2);
		Utf32Char* pUtf32Found = StrCiStrUtf32(pUtf32s1, pUtf32s2);
		if (pUtf32s2) {
			Utf16Char* pUtf16Found = 0;
			size_t ztSteps = pUtf32Found - pUtf32s1;
			if (pUtf32Found)
				pUtf16Found = ForwardUtf16Chars(pUtf16s1, ztSteps);
			free(pUtf32s1);
			free(pUtf32s2);
			return pUtf16Found;
		}
		free(pUtf32s1);
		return 0; // calloc() failure
	}
	return 0; // calloc() failure
}
/*******************************************************************************************

UTF8

*******************************************************************************************/
size_t StrLenUtf8(const Utf8Char* str)
{
	Utf8Char* s;
	for (s = (Utf8Char*)str; *s; ++s);
	return(s - str);
}
int StrnCmpUtf8(const Utf8Char* Utf8s1, const Utf8Char* Utf8s2, size_t ztCount)
{
	if (Utf8s1 && *Utf8s1 && Utf8s2 && *Utf8s2) {
		for (; ztCount--; Utf8s1++, Utf8s2++) {
			int iDiff = *Utf8s1 - *Utf8s2;
			if (iDiff != 0 || !*Utf8s1 || !*Utf8s2)
				return iDiff;
		}
		return 0;
	}
	return (-1);
}
int StrCmpUtf8(const Utf8Char* Utf8s1, const Utf8Char* Utf8s2)
{
	return StrnCmpUtf8(Utf8s1, Utf8s2, (size_t)(-1));
}
size_t CharLenUtf8(const Utf8Char* pUtf8)
{
	size_t ztUtf8 = 0;
	size_t ztNumUtf8Chars = 0;

	while (pUtf8[ztUtf8] != 0) {
		ztNumUtf8Chars++;

		if ((pUtf8[ztUtf8] & 0b10000000) == 0)
			// 1 byte code point
			ztUtf8 += 1;
		else if ((pUtf8[ztUtf8] & 0b11100000) == 0b11000000)
			// 2 byte code point
			ztUtf8 += 2;
		else if ((pUtf8[ztUtf8] & 0b11110000) == 0b11100000)
			// 3 byte code point
			ztUtf8 += 3;
		else
			// 4 byte code point
			ztUtf8 += 4;
	}
	return ztNumUtf8Chars;
}
Utf8Char* ForwardUtf8Chars(const Utf8Char* pUtf8, size_t ztForwardUtf8Chars)
{
	size_t ztUtf8 = 0;
	size_t ztNumUtf8Chars = 0;

	while ((ztNumUtf8Chars < ztForwardUtf8Chars) && (pUtf8[ztUtf8] != 0)) {
		ztNumUtf8Chars++;

		if ((pUtf8[ztUtf8] & 0b10000000) == 0)
			// 1 byte code point
			ztUtf8 += 1;
		else if ((pUtf8[ztUtf8] & 0b11100000) == 0b11000000)
			// 2 byte code point
			ztUtf8 += 2;
		else if ((pUtf8[ztUtf8] & 0b11110000) == 0b11100000)
			// 3 byte code point
			ztUtf8 += 3;
		else
			// 4 byte code point
			ztUtf8 += 4;
	}
	return (Utf8Char*)&pUtf8[ztUtf8];
}
size_t StrLenUtf32AsUtf8(const Utf32Char* pUtf32)
{
	size_t ztUtf32 = 0;
	size_t ztNumUtf8Chars = 0;
	while (pUtf32[ztUtf32] != 0) {
		if (pUtf32[ztUtf32] >= 0x10000u)
			ztNumUtf8Chars += 3;
		else if (pUtf32[ztUtf32] >= 0x800u)
			ztNumUtf8Chars += 2;
		else if (pUtf32[ztUtf32] >= 0x80u)
			ztNumUtf8Chars += 1;
		ztNumUtf8Chars++;
		ztUtf32++;
	}
	return ztNumUtf8Chars;
}
Utf8Char* Utf32ToUtf8(const Utf32Char* pUtf32)
{
	size_t ztUtf32 = 0;
	Utf8Char* pUtf8 = (Utf8Char*)calloc(StrLenUtf32AsUtf8(pUtf32) + 1, sizeof(Utf8Char));
	if (pUtf8) {
		// https://datatracker.ietf.org/doc/html/rfc3629#page-4
		// This function skips data when UTF bytes are out of bounds
		while (pUtf32[ztUtf32] != 0) {
			if (pUtf32[ztUtf32] <= 0x7Fu) {
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)pUtf32[ztUtf32];
			}
			else if (pUtf32[ztUtf32] <= 0x7FFu) {
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0xC0u | (pUtf32[ztUtf32] >> 6));				/* 110xxxxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | (pUtf32[ztUtf32] & 0x3Fu));			/* 10xxxxxx */
			}
			else if (pUtf32[ztUtf32] <= 0xFFFFu) {
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0xE0u | (pUtf32[ztUtf32] >> 12));			/* 1110xxxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | ((pUtf32[ztUtf32] >> 6) & 0x3Fu));	/* 10xxxxxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | (pUtf32[ztUtf32] & 0x3Fu));			/* 10xxxxxx */
			}
			else if (pUtf32[ztUtf32] <= 0x10FFFFu) {
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0xF0u | (pUtf32[ztUtf32] >> 18));			/* 11110xxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | ((pUtf32[ztUtf32] >> 12) & 0x3Fu));	/* 10xxxxxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | ((pUtf32[ztUtf32] >> 6) & 0x3Fu));	/* 10xxxxxx */
				pUtf8[StrLenUtf8((const Utf8Char*)pUtf8)] = (Utf8Char)(0x80u | (pUtf32[ztUtf32] & 0x3Fu));			/* 10xxxxxx */
			}
			// else out of RFC3629 UTF8 bounds doing nothing
			ztUtf32++;
		}
		return pUtf8;
	}
	return 0;
}
Utf32Char* Utf8ToUtf32(const Utf8Char* pUtf8)
{
	size_t ztUtf8 = 0;
	size_t ztUtf32 = 0;
	Utf32Char* pUtf32 = (Utf32Char*)calloc(CharLenUtf8(pUtf8) + 1, sizeof(Utf32Char));
	if (pUtf32) {
		// https://datatracker.ietf.org/doc/html/rfc3629#page-4
		// This function skips data when UTF bytes are out of bounds
		while (pUtf8[ztUtf8] != 0) {
			if ((pUtf8[ztUtf8] & 0b10000000) == 0) {
				// 1 byte code point, ASCII
				pUtf32[ztUtf32] = (pUtf8[ztUtf8] & 0b01111111);
				ztUtf8 += 1;
			}
			else if ((pUtf8[ztUtf8] & 0b11100000) == 0b11000000) {
				// 2 byte code point
				pUtf32[ztUtf32] = (pUtf8[ztUtf8] & 0b00011111) << 6 | (pUtf8[ztUtf8 + 1] & 0b00111111);
				if (pUtf32[ztUtf32] < 0x80u)				// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data  
				else if (pUtf32[ztUtf32] > 0x7ffu)		// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data 
				ztUtf8 += 2;
			}
			else if ((pUtf8[ztUtf8] & 0b11110000) == 0b11100000) {
				// 3 byte code point
				pUtf32[ztUtf32] = (pUtf8[ztUtf8] & 0b00001111) << 12 | (pUtf8[ztUtf8 + 1] & 0b00111111) << 6 | (pUtf8[ztUtf8 + 2] & 0b00111111);
				if (pUtf32[ztUtf32] < 0x800u)			// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data  
				else if (pUtf32[ztUtf32] > 0xffffu)		// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data  
				ztUtf8 += 3;
			}
			else {
				// 4 byte code point
				if (pUtf8[ztUtf8] <= 0xf4u)				// RFC3629 UTF8 do not allow larger values
					pUtf32[ztUtf32] = (pUtf8[ztUtf8] & 0b00000111) << 18 | (pUtf8[ztUtf8 + 1] & 0b00111111) << 12 | (pUtf8[ztUtf8 + 2] & 0b00111111) << 6 | (pUtf8[ztUtf8 + 3] & 0b00111111);
				else									// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data 
				if (pUtf32[ztUtf32] < 0x10000u)			// Not a valid result, Wrong encoding
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data  
				else if (pUtf32[ztUtf32] > 0x10FFFFu)	// Not a valid result, Wrong encoding 
					pUtf32[ztUtf32] = 0;				// Out of UTF8 bound, skip data  
				ztUtf8 += 4;
			}
			if (pUtf32[ztUtf32]) // Only step when valid data
				ztUtf32++;
		}
		return pUtf32;
	}
	return 0;
}
Utf16Char* Utf8ToUtf16(const Utf8Char* pUtf8)
{
	Utf32Char* pUtf32 = Utf8ToUtf32((const Utf8Char*)pUtf8);
	if (pUtf32) {
		Utf16Char* pUtf16 = Utf32ToUtf16((const Utf32Char*)pUtf32);
		free(pUtf32);
		if (pUtf16)
			return pUtf16;
		return 0;
	}
	return 0;
}
Utf8Char* Utf8StrMakeUprUtf8Str(const Utf8Char* pUtf8)
{
	Utf32Char* pUtf32 = Utf8ToUtf32(pUtf8);
	if (pUtf32 && *pUtf32) {
		StrToUprUtf32(pUtf32);
		{
			Utf8Char* pUtf8Upr = Utf32ToUtf8(pUtf32);
			free(pUtf32);
			return pUtf8Upr;
		}
	}
	return 0; // calloc() failure
}
Utf8Char* Utf8StrMakeLwrUtf8Str(const Utf8Char* pUtf8)
{
	Utf32Char* pUtf32 = Utf8ToUtf32(pUtf8);
	if (pUtf32 && *pUtf32) {
		StrToLwrUtf32(pUtf32);
		{
			Utf8Char* pUtf8Lwr = Utf32ToUtf8(pUtf32);
			free(pUtf32);
			return pUtf8Lwr;
		}
	}
	return 0; // calloc() failure
}
int StrnCiCmpUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2, size_t ztCount)
{
	Utf32Char* pUtf32s1 = Utf8ToUtf32(pUtf8s1);
	if (pUtf32s1) {
		Utf32Char* pUtf32s2 = Utf8ToUtf32(pUtf8s2);
		if (pUtf32s2) {
			int iDiff = StrnCiCmpUtf32(pUtf32s1, pUtf32s2, ztCount);
			free(pUtf32s1);
			free(pUtf32s2);
			return iDiff;
		}
		free(pUtf32s1);
		return (-1); // calloc() failure
	}
	return (-1); // calloc() failure
}
int StrCiCmpUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2)
{
	return StrnCiCmpUtf8(pUtf8s1, pUtf8s2, (size_t)(-1));
}
Utf8Char* StrCiStrUtf8(const Utf8Char* pUtf8s1, const Utf8Char* pUtf8s2)
{
	Utf32Char* pUtf32s1 = Utf8ToUtf32(pUtf8s1);
	if (pUtf32s1) {
		Utf32Char* pUtf32s2 = Utf8ToUtf32(pUtf8s2);
		Utf32Char* pUtf32Found = StrCiStrUtf32(pUtf32s1, pUtf32s2);
		if (pUtf32s2) {
			Utf8Char* pUtf8Found = 0;
			size_t ztSteps = pUtf32Found - pUtf32s1;
			if (pUtf32Found)
				pUtf8Found = ForwardUtf8Chars(pUtf8s1, ztSteps);
			free(pUtf32s1);
			free(pUtf32s2);
			return pUtf8Found;
		}
		free(pUtf32s1);
		return 0; // calloc() failure
	}
	return 0; // calloc() failure
}
#ifdef UTF32_TEST
/******************/
/* Test functions */
/******************/
#include <stdio.h>
size_t FileWrite(char* pFileName, char* pBuffer)
{
	/* Open the file on disk */
	FILE* pFile = fopen((const char*)pFileName, (const char*)"wb");
	if (!pFile)
		return 0; /* File not found or opened */
	{
		size_t ztWritten = fwrite(pBuffer, sizeof(char), StrLenUtf8(pBuffer), pFile);
		fclose(pFile);
		return ztWritten;
	}
}

int main(void) {

	Utf8Char Base8[] = { "Text Ã†Ã‡Ãˆð’·ð’¶ðï¼´êž«123Ã‰ÃªÃŸêžµÃ«Ã¬Ã­Ã®Ã¯Ã°Ã‘Ã’Ã“Ã”Ã•Ã–Ã˜Ã™ÃšÃ›ÃœÃÃžÄ€Ä‚Ä„" };
	Utf8Char Target8[] = { "ð“žð‘…ï¼´êž«123Ã©ÃªÃŸêžµÃ«" };
	Utf8Char Copy8[] = { "Hello ð“žð‘…ï¼´êž«123Ã©ÃªÃŸêžµÃ«123" };
	Utf8Char* pLwr8 = Utf8StrMakeLwrUtf8Str(Base8);
	Utf8Char* pUpr8 = Utf8StrMakeUprUtf8Str(pLwr8);
	Utf8Char* pLwr8B = Utf8StrMakeLwrUtf8Str(pUpr8);
	int iDiff8 = StrnCiCmpUtf8(pLwr8, pUpr8, 2);
	int iDiff8B = StrCiCmpUtf8(pLwr8, pUpr8);
	Utf8Char* pSearch8 = StrCiStrUtf8(Base8, Target8);

	Utf16Char* pBase16 = Utf8ToUtf16(Base8);
	Utf16Char* pTarget16 = Utf8ToUtf16(Target8);
	Utf16Char* pLwr16 = Utf16StrMakeLwrUtf16Str(pBase16);
	Utf16Char* pUpr16 = Utf16StrMakeUprUtf16Str(pLwr16);
	Utf16Char* pLwr16B = Utf16StrMakeLwrUtf16Str(pUpr16);
	int iDiff16 = StrnCiCmpUtf16(pLwr16, pUpr16, 2);
	int iDiff16B = StrCiCmpUtf16(pLwr16, pUpr16);
	Utf16Char* pSearch16 = StrCiStrUtf16(pBase16, pTarget16);

	Utf16Char* pCopy16 = Utf8ToUtf16(Copy8);
	Utf16Char* pCopyTest16 = calloc(200, sizeof(Utf16Char));
	StrCpyUtf16(pCopyTest16, pTarget16);
	StrCpyUtf16(pCopyTest16, pCopy16);
	StrCatUtf16(pCopyTest16, pTarget16);

	Utf32Char* pTarget32 = Utf8ToUtf32(Target8);
	Utf32Char* pCopy32 = Utf8ToUtf32(Copy8);
	Utf32Char* pCopyTest32 = calloc(200, sizeof(Utf32Char));
	StrCpyUtf32(pCopyTest32, pTarget32);
	StrCpyUtf32(pCopyTest32, pCopy32);
	StrCatUtf32(pCopyTest32, pTarget32);

	Utf8Char* pBase16_8 = Utf16ToUtf8(pBase16);
	Utf8Char* pLwr16_8 = Utf16ToUtf8(pLwr16);
	Utf8Char* pUpr16_8 = Utf16ToUtf8(pUpr16);
	Utf8Char* pLwr16B_8 = Utf16ToUtf8(pLwr16B);
	Utf8Char* pTarget16_8 = Utf16ToUtf8(pTarget16);
	Utf8Char* pSearch16_8 = Utf16ToUtf8(pSearch16);

	Utf8Char* pCopy16_8 = Utf16ToUtf8(pCopy16);
	Utf8Char* pCopyTest16_8 = Utf16ToUtf8(pCopyTest16);
	Utf8Char* pCopy32_8 = Utf32ToUtf8(pCopy32);
	Utf8Char* pCopyTest32_8 = Utf32ToUtf8(pCopyTest32);

	char Buffer[10000];
	sprintf(Buffer, "\xEF\xBB\xBF\nBase8  = %s\nBase16 = %s\npUpr8  = %s\npUpr16 = %s\npLwr8  = %s\npLwr8B = %s\npLwr16 = %s\npLwr16B= %s\npTarget8  = %s\npTarget16 = %s\npSearch8  = %s\npSearch16 = %s\nDiff8 = %i\nDiff8b = %i\nDiff16 = %i\nDiff16b = %i\npCopy16_8  = %s\npCopyTest16_8  = %s\npCopy32_8  = %s\npCopyTest32_8  = %s\n",
		Base8, pBase16_8, pUpr8, pUpr16_8, pLwr8, pLwr8B, pLwr16_8, pLwr16B_8, Target8, pTarget16_8, pSearch8, pSearch16_8, iDiff8, iDiff8B, iDiff16, iDiff16B, pCopy16_8, pCopyTest16_8, pCopy32_8, pCopyTest32_8);
	printf("% s", Buffer);
	FileWrite("UTF8_checked.txt", Buffer);
	free(pLwr8);
	free(pUpr8);
	free(pLwr8B);

	free(pBase16);
	free(pTarget16);
	free(pLwr16);
	free(pUpr16);
	free(pLwr16B);

	free(pBase16_8);
	free(pLwr16_8);
	free(pUpr16_8);
	free(pLwr16B_8);
	free(pTarget16_8);
	free(pSearch16_8);

	free(pCopy16);
	free(pCopyTest16);
	free(pCopy32);
	free(pCopyTest32);
	free(pCopy16_8);
	free(pCopyTest16_8);
	free(pCopy32_8);
	free(pCopyTest32_8);

	return 0;
}
#endif