/* ANSI-C code produced by gperf version 3.0.3 */
/* Command-line: gperf -CDEot -L ANSI-C -k '*' -N findColor -D -s 2 WebCore/platform/ColorData.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
&& ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
&& (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
&& ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
&& ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
&& ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
&& ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
&& ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
&& ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
&& ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
&& ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
&& ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
&& ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
&& ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
&& ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
&& ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
&& ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
&& ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
&& ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
&& ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
&& ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
&& ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
&& ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "WebCore/platform/ColorData.gperf"
struct NamedColor { const char *name; int RGBValue; };
/* maximum key range = 1053, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static const unsigned short asso_values[] =
	{
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056,    5,    0,  105,
		0,    0,   30,   40,   40,   10,    0,    0,   15,   60,
		0,    5,  255,   40,    0,   10,   15,  130,  300,  215,
		5,    0,    0, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056, 1056,
		1056, 1056, 1056, 1056, 1056, 1056, 1056
	};
  register int hval = len;
	
  switch (hval)
	{
		default:
			hval += asso_values[(unsigned char)str[19]];
      /*FALLTHROUGH*/
		case 19:
			hval += asso_values[(unsigned char)str[18]];
      /*FALLTHROUGH*/
		case 18:
			hval += asso_values[(unsigned char)str[17]];
      /*FALLTHROUGH*/
		case 17:
			hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
		case 16:
			hval += asso_values[(unsigned char)str[15]];
      /*FALLTHROUGH*/
		case 15:
			hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
		case 14:
			hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
		case 13:
			hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
		case 12:
			hval += asso_values[(unsigned char)str[11]];
      /*FALLTHROUGH*/
		case 11:
			hval += asso_values[(unsigned char)str[10]+1];
      /*FALLTHROUGH*/
		case 10:
			hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
		case 9:
			hval += asso_values[(unsigned char)str[8]];
      /*FALLTHROUGH*/
		case 8:
			hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
		case 7:
			hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
		case 6:
			hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
		case 5:
			hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
		case 4:
			hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
		case 3:
			hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
		case 2:
			hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
		case 1:
			hval += asso_values[(unsigned char)str[0]];
			break;
	}
  return hval;
}

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct NamedColor *
findColor (register const char *str, register unsigned int len)
{
  enum
	{
		TOTAL_KEYWORDS = 149,
		MIN_WORD_LENGTH = 3,
		MAX_WORD_LENGTH = 20,
		MIN_HASH_VALUE = 3,
		MAX_HASH_VALUE = 1055
	};
	
  static const struct NamedColor wordlist[] =
	{
#line 123 "WebCore/platform/ColorData.gperf"
		{"red", 0xff0000},
#line 35 "WebCore/platform/ColorData.gperf"
		{"darkred", 0x8b0000},
#line 140 "WebCore/platform/ColorData.gperf"
		{"tan", 0xd2b48c},
#line 88 "WebCore/platform/ColorData.gperf"
		{"linen", 0xfaf0e6},
#line 131 "WebCore/platform/ColorData.gperf"
		{"sienna", 0xa0522d},
#line 62 "WebCore/platform/ColorData.gperf"
		{"indianred", 0xcd5c5c},
#line 141 "WebCore/platform/ColorData.gperf"
		{"teal", 0x008080},
#line 57 "WebCore/platform/ColorData.gperf"
		{"grey", 0x808080},
#line 58 "WebCore/platform/ColorData.gperf"
		{"green", 0x008000},
#line 56 "WebCore/platform/ColorData.gperf"
		{"gray", 0x808080},
#line 28 "WebCore/platform/ColorData.gperf"
		{"darkgrey", 0xa9a9a9},
#line 29 "WebCore/platform/ColorData.gperf"
		{"darkgreen", 0x006400},
#line 8 "WebCore/platform/ColorData.gperf"
		{"beige", 0xf5f5dc},
#line 109 "WebCore/platform/ColorData.gperf"
		{"orange", 0xffa500},
#line 27 "WebCore/platform/ColorData.gperf"
		{"darkgray", 0xa9a9a9},
#line 110 "WebCore/platform/ColorData.gperf"
		{"orangered", 0xff4500},
#line 65 "WebCore/platform/ColorData.gperf"
		{"khaki", 0xf0e68c},
#line 129 "WebCore/platform/ColorData.gperf"
		{"seagreen", 0x2e8b57},
#line 54 "WebCore/platform/ColorData.gperf"
		{"gold", 0xffd700},
#line 33 "WebCore/platform/ColorData.gperf"
		{"darkorange", 0xff8c00},
#line 30 "WebCore/platform/ColorData.gperf"
		{"darkkhaki", 0xbdb76b},
#line 63 "WebCore/platform/ColorData.gperf"
		{"indigo", 0x4b0082},
#line 55 "WebCore/platform/ColorData.gperf"
		{"goldenrod", 0xdaa520},
#line 90 "WebCore/platform/ColorData.gperf"
		{"maroon", 0x800000},
#line 52 "WebCore/platform/ColorData.gperf"
		{"gainsboro", 0xdcdcdc},
#line 86 "WebCore/platform/ColorData.gperf"
		{"lime", 0x00ff00},
#line 59 "WebCore/platform/ColorData.gperf"
		{"greenyellow", 0xadff2f},
#line 26 "WebCore/platform/ColorData.gperf"
		{"darkgoldenrod", 0xb8860b},
#line 136 "WebCore/platform/ColorData.gperf"
		{"slategrey", 0x708090},
#line 135 "WebCore/platform/ColorData.gperf"
		{"slategray", 0x708090},
#line 127 "WebCore/platform/ColorData.gperf"
		{"salmon", 0xfa8072},
#line 37 "WebCore/platform/ColorData.gperf"
		{"darkseagreen", 0x8fbc8f},
#line 130 "WebCore/platform/ColorData.gperf"
		{"seashell", 0xfff5ee},
#line 36 "WebCore/platform/ColorData.gperf"
		{"darksalmon", 0xe9967a},
#line 143 "WebCore/platform/ColorData.gperf"
		{"tomato", 0xff6347},
#line 142 "WebCore/platform/ColorData.gperf"
		{"thistle", 0xd8bfd8},
#line 40 "WebCore/platform/ColorData.gperf"
		{"darkslategrey", 0x2f4f4f},
#line 23 "WebCore/platform/ColorData.gperf"
		{"cyan", 0x00ffff},
#line 50 "WebCore/platform/ColorData.gperf"
		{"forestgreen", 0x228b22},
#line 46 "WebCore/platform/ColorData.gperf"
		{"dimgrey", 0x696969},
#line 39 "WebCore/platform/ColorData.gperf"
		{"darkslategray", 0x2f4f4f},
#line 102 "WebCore/platform/ColorData.gperf"
		{"mistyrose", 0xffe4e1},
#line 45 "WebCore/platform/ColorData.gperf"
		{"dimgray", 0x696969},
#line 25 "WebCore/platform/ColorData.gperf"
		{"darkcyan", 0x008b8b},
#line 10 "WebCore/platform/ColorData.gperf"
		{"black", 0x000000},
#line 89 "WebCore/platform/ColorData.gperf"
		{"magenta", 0xff00ff},
#line 87 "WebCore/platform/ColorData.gperf"
		{"limegreen", 0x32cd32},
#line 19 "WebCore/platform/ColorData.gperf"
		{"coral", 0xff7f50},
#line 31 "WebCore/platform/ColorData.gperf"
		{"darkmagenta", 0x8b008b},
#line 7 "WebCore/platform/ColorData.gperf"
		{"azure", 0xf0ffff},
#line 12 "WebCore/platform/ColorData.gperf"
		{"blue", 0x0000ff},
#line 106 "WebCore/platform/ColorData.gperf"
		{"oldlace", 0xfdf5e6},
#line 21 "WebCore/platform/ColorData.gperf"
		{"cornsilk", 0xfff8dc},
#line 24 "WebCore/platform/ColorData.gperf"
		{"darkblue", 0x00008b},
#line 133 "WebCore/platform/ColorData.gperf"
		{"skyblue", 0x87ceeb},
#line 48 "WebCore/platform/ColorData.gperf"
		{"firebrick", 0xb22222},
#line 111 "WebCore/platform/ColorData.gperf"
		{"orchid", 0xda70d6},
#line 75 "WebCore/platform/ColorData.gperf"
		{"lightgrey", 0xd3d3d3},
#line 76 "WebCore/platform/ColorData.gperf"
		{"lightgreen", 0x90ee90},
#line 85 "WebCore/platform/ColorData.gperf"
		{"lightyellow", 0xffffe0},
#line 74 "WebCore/platform/ColorData.gperf"
		{"lightgray", 0xd3d3d3},
#line 34 "WebCore/platform/ColorData.gperf"
		{"darkorchid", 0x9932cc},
#line 125 "WebCore/platform/ColorData.gperf"
		{"royalblue", 0x4169e1},
#line 5 "WebCore/platform/ColorData.gperf"
		{"aqua", 0x00ffff},
#line 139 "WebCore/platform/ColorData.gperf"
		{"steelblue", 0x4682b4},
#line 9 "WebCore/platform/ColorData.gperf"
		{"bisque", 0xffe4c4},
#line 22 "WebCore/platform/ColorData.gperf"
		{"crimson", 0xdc143c},
#line 134 "WebCore/platform/ColorData.gperf"
		{"slateblue", 0x6a5acd},
#line 47 "WebCore/platform/ColorData.gperf"
		{"dodgerblue", 0x1e90ff},
#line 11 "WebCore/platform/ColorData.gperf"
		{"blanchedalmond", 0xffebcd},
#line 79 "WebCore/platform/ColorData.gperf"
		{"lightseagreen", 0x20b2aa},
#line 83 "WebCore/platform/ColorData.gperf"
		{"lightslategrey", 0x778899},
#line 82 "WebCore/platform/ColorData.gperf"
		{"lightslategray", 0x778899},
#line 14 "WebCore/platform/ColorData.gperf"
		{"brown", 0xa52a2a},
#line 78 "WebCore/platform/ColorData.gperf"
		{"lightsalmon", 0xffa07a},
#line 137 "WebCore/platform/ColorData.gperf"
		{"snow", 0xfffafa},
#line 72 "WebCore/platform/ColorData.gperf"
		{"lightcyan", 0xe0ffff},
#line 124 "WebCore/platform/ColorData.gperf"
		{"rosybrown", 0xbc8f8f},
#line 128 "WebCore/platform/ColorData.gperf"
		{"sandybrown", 0xf4a460},
#line 38 "WebCore/platform/ColorData.gperf"
		{"darkslateblue", 0x483d8b},
#line 150 "WebCore/platform/ColorData.gperf"
		{"yellow", 0xffff00},
#line 71 "WebCore/platform/ColorData.gperf"
		{"lightcoral", 0xf08080},
#line 101 "WebCore/platform/ColorData.gperf"
		{"mintcream", 0xf5fffa},
#line 6 "WebCore/platform/ColorData.gperf"
		{"aquamarine", 0x7fffd4},
#line 126 "WebCore/platform/ColorData.gperf"
		{"saddlebrown", 0x8b4513},
#line 60 "WebCore/platform/ColorData.gperf"
		{"honeydew", 0xf0fff0},
#line 119 "WebCore/platform/ColorData.gperf"
		{"pink", 0xffc0cb},
#line 70 "WebCore/platform/ColorData.gperf"
		{"lightblue", 0xadd8e6},
#line 16 "WebCore/platform/ColorData.gperf"
		{"cadetblue", 0x5f9ea0},
#line 147 "WebCore/platform/ColorData.gperf"
		{"wheat", 0xf5deb3},
#line 68 "WebCore/platform/ColorData.gperf"
		{"lawngreen", 0x7cfc00},
#line 148 "WebCore/platform/ColorData.gperf"
		{"white", 0xffffff},
#line 3 "WebCore/platform/ColorData.gperf"
		{"aliceblue", 0xf0f8ff},
#line 18 "WebCore/platform/ColorData.gperf"
		{"chocolate", 0xd2691e},
#line 151 "WebCore/platform/ColorData.gperf"
		{"yellowgreen", 0x9acd32},
#line 103 "WebCore/platform/ColorData.gperf"
		{"moccasin", 0xffe4b5},
#line 105 "WebCore/platform/ColorData.gperf"
		{"navy", 0x000080},
#line 17 "WebCore/platform/ColorData.gperf"
		{"chartreuse", 0x7fff00},
#line 64 "WebCore/platform/ColorData.gperf"
		{"ivory", 0xfffff0},
#line 113 "WebCore/platform/ColorData.gperf"
		{"palegreen", 0x98fb98},
#line 66 "WebCore/platform/ColorData.gperf"
		{"lavender", 0xe6e6fa},
#line 61 "WebCore/platform/ColorData.gperf"
		{"hotpink", 0xff69b4},
#line 107 "WebCore/platform/ColorData.gperf"
		{"olive", 0x808000},
#line 51 "WebCore/platform/ColorData.gperf"
		{"fuchsia", 0xff00ff},
#line 95 "WebCore/platform/ColorData.gperf"
		{"mediumseagreen", 0x3cb371},
#line 132 "WebCore/platform/ColorData.gperf"
		{"silver", 0xc0c0c0},
#line 108 "WebCore/platform/ColorData.gperf"
		{"olivedrab", 0x6b8e23},
#line 41 "WebCore/platform/ColorData.gperf"
		{"darkturquoise", 0x00ced1},
#line 144 "WebCore/platform/ColorData.gperf"
		{"turquoise", 0x40e0d0},
#line 145 "WebCore/platform/ColorData.gperf"
		{"violet", 0xee82ee},
#line 146 "WebCore/platform/ColorData.gperf"
		{"violetred", 0xd02090},
#line 42 "WebCore/platform/ColorData.gperf"
		{"darkviolet", 0x9400d3},
#line 112 "WebCore/platform/ColorData.gperf"
		{"palegoldenrod", 0xeee8aa},
#line 149 "WebCore/platform/ColorData.gperf"
		{"whitesmoke", 0xf5f5f5},
#line 138 "WebCore/platform/ColorData.gperf"
		{"springgreen", 0x00ff7f},
#line 15 "WebCore/platform/ColorData.gperf"
		{"burlywood", 0xdeb887},
#line 118 "WebCore/platform/ColorData.gperf"
		{"peru", 0xcd853f},
#line 49 "WebCore/platform/ColorData.gperf"
		{"floralwhite", 0xfffaf0},
#line 77 "WebCore/platform/ColorData.gperf"
		{"lightpink", 0xffb6c1},
#line 32 "WebCore/platform/ColorData.gperf"
		{"darkolivegreen", 0x556b2f},
#line 53 "WebCore/platform/ColorData.gperf"
		{"ghostwhite", 0xf8f8ff},
#line 92 "WebCore/platform/ColorData.gperf"
		{"mediumblue", 0x0000cd},
#line 93 "WebCore/platform/ColorData.gperf"
		{"mediumorchid", 0xba55d3},
#line 84 "WebCore/platform/ColorData.gperf"
		{"lightsteelblue", 0xb0c4de},
#line 81 "WebCore/platform/ColorData.gperf"
		{"lightslateblue", 0x8470ff},
#line 44 "WebCore/platform/ColorData.gperf"
		{"deepskyblue", 0x00bfff},
#line 80 "WebCore/platform/ColorData.gperf"
		{"lightskyblue", 0x87cefa},
#line 73 "WebCore/platform/ColorData.gperf"
		{"lightgoldenrodyellow", 0xfafad2},
#line 120 "WebCore/platform/ColorData.gperf"
		{"plum", 0xdda0dd},
#line 91 "WebCore/platform/ColorData.gperf"
		{"mediumaquamarine", 0x66cdaa},
#line 96 "WebCore/platform/ColorData.gperf"
		{"mediumslateblue", 0x7b68ee},
#line 13 "WebCore/platform/ColorData.gperf"
		{"blueviolet", 0x8a2be2},
#line 100 "WebCore/platform/ColorData.gperf"
		{"midnightblue", 0x191970},
#line 43 "WebCore/platform/ColorData.gperf"
		{"deeppink", 0xff1493},
#line 69 "WebCore/platform/ColorData.gperf"
		{"lemonchiffon", 0xfffacd},
#line 4 "WebCore/platform/ColorData.gperf"
		{"antiquewhite", 0xfaebd7},
#line 114 "WebCore/platform/ColorData.gperf"
		{"paleturquoise", 0xafeeee},
#line 121 "WebCore/platform/ColorData.gperf"
		{"powderblue", 0xb0e0e6},
#line 104 "WebCore/platform/ColorData.gperf"
		{"navajowhite", 0xffdead},
#line 97 "WebCore/platform/ColorData.gperf"
		{"mediumspringgreen", 0x00fa9a},
#line 20 "WebCore/platform/ColorData.gperf"
		{"cornflowerblue", 0x6495ed},
#line 115 "WebCore/platform/ColorData.gperf"
		{"palevioletred", 0xd87093},
#line 99 "WebCore/platform/ColorData.gperf"
		{"mediumvioletred", 0xc71585},
#line 122 "WebCore/platform/ColorData.gperf"
		{"purple", 0x800080},
#line 67 "WebCore/platform/ColorData.gperf"
		{"lavenderblush", 0xfff0f5},
#line 98 "WebCore/platform/ColorData.gperf"
		{"mediumturquoise", 0x48d1cc},
#line 117 "WebCore/platform/ColorData.gperf"
		{"peachpuff", 0xffdab9},
#line 94 "WebCore/platform/ColorData.gperf"
		{"mediumpurple", 0x9370d8},
#line 116 "WebCore/platform/ColorData.gperf"
		{"papayawhip", 0xffefd5}
	};
	
  static const short lookup[] =
	{
		-1,  -1,  -1,   0,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,   1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,   2,  -1,  -1,  -1,  -1,  -1,  -1,
		3,   4,  -1,  -1,   5,  -1,  -1,  -1,  -1,   6,
		-1,  -1,  -1,  -1,   7,   8,  -1,  -1,  -1,   9,
		-1,  -1,  -1,  10,  11,  12,  13,  -1,  14,  15,
		16,  -1,  -1,  17,  18,  19,  -1,  -1,  -1,  20,
		-1,  21,  -1,  -1,  22,  -1,  -1,  -1,  -1,  -1,
		-1,  23,  -1,  -1,  24,  -1,  -1,  -1,  -1,  25,
		-1,  26,  -1,  27,  28,  -1,  -1,  -1,  -1,  29,
		-1,  30,  31,  32,  -1,  -1,  -1,  -1,  -1,  -1,
		33,  34,  35,  36,  37,  -1,  38,  39,  40,  41,
		-1,  -1,  42,  43,  -1,  -1,  -1,  -1,  -1,  -1,
		44,  -1,  45,  -1,  46,  47,  48,  -1,  -1,  -1,
		49,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  50,
		-1,  -1,  51,  52,  -1,  -1,  -1,  -1,  53,  -1,
		-1,  -1,  54,  -1,  55,  -1,  56,  -1,  -1,  57,
		58,  59,  -1,  -1,  60,  61,  -1,  -1,  -1,  62,
		-1,  -1,  -1,  -1,  63,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  64,  -1,  65,  66,  -1,  67,
		68,  -1,  -1,  -1,  69,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  70,  71,
		-1,  -1,  -1,  -1,  72,  73,  -1,  -1,  -1,  -1,
		-1,  74,  -1,  -1,  75,  -1,  -1,  -1,  -1,  76,
		-1,  -1,  -1,  -1,  77,  78,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  79,  -1,  -1,  80,  -1,  -1,  -1,
		81,  -1,  -1,  -1,  82,  83,  84,  -1,  85,  86,
		-1,  -1,  -1,  -1,  87,  -1,  -1,  -1,  -1,  88,
		89,  -1,  -1,  -1,  90,  91,  -1,  -1,  -1,  92,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  93,  -1,  94,  -1,  95,  96,
		-1,  -1,  -1,  -1,  -1,  97,  -1,  -1,  -1,  -1,
		98,  -1,  -1,  -1,  99,  -1,  -1,  -1, 100,  -1,
		-1,  -1, 101,  -1,  -1, 102,  -1, 103,  -1, 104,
		-1, 105,  -1,  -1, 106,  -1,  -1,  -1, 107, 108,
		-1, 109,  -1,  -1, 110,  -1,  -1,  -1,  -1,  -1,
		111,  -1,  -1, 112,  -1, 113,  -1,  -1,  -1,  -1,
		-1, 114,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 115,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 116,
		-1, 117,  -1,  -1, 118,  -1,  -1,  -1,  -1, 119,
		120,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1, 121,  -1,  -1,  -1,  -1,
		-1,  -1, 122,  -1, 123,  -1,  -1,  -1,  -1, 124,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1, 125,  -1,  -1,  -1,  -1,  -1, 126,  -1,  -1,
		127,  -1,  -1,  -1, 128,  -1,  -1,  -1,  -1,  -1,
		-1, 129,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1, 130,  -1,  -1,  -1,  -1,
		131,  -1, 132,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 133,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1, 134,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1, 135,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 136,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		137,  -1,  -1,  -1,  -1,  -1, 138, 139,  -1, 140,
		-1,  -1,  -1, 141,  -1,  -1,  -1,  -1,  -1,  -1,
		142,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1, 143,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 144,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1, 145,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 146,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1, 147,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
		-1,  -1,  -1,  -1,  -1, 148
	};
	
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
	{
		register int key = hash (str, len);
		
		if (key <= MAX_HASH_VALUE && key >= 0)
		{
			register int index = lookup[key];
			
			if (index >= 0)
			{
				register const char *s = wordlist[index].name;
				
				if (*str == *s && !strcmp (str + 1, s + 1))
					return &wordlist[index];
			}
		}
	}
  return 0;
}
