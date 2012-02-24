//
//	コウビルド英英辞典[改訂第5版]をDictionary.app用XMLに変換する
//	llvm-g++用
//
//	使い方
//		CobuildConv DataDir [-w]
//		-w ... WordBank
//
//	例
//		CobuildConv "/Volumes/COBUILD5/data/hcp_en-cc5.trd"
//		CobuildConv "/Volumes/COBUILD5/data/hcp_en-wbk.trd" -w 
//

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

std::map<std::string,int> g_id;

struct cTable {
	int	Code;
	const char* Plain;
};

cTable 	CTable[] = {
	// 英小文字
	1, "a",	2, "b",	3, "c", 4, "d",	5, "e", 6, "f", 7, "g", 8, "h", 9, "i",
	10, "j", 11, "k", 12, "l", 13, "m", 14, "n",
	15, "o", 16, "p", 17, "q", 18, "r", 19, "s", 
	20, "t", 21, "u", 22, "v", 23, "w", 24, "x",
	25, "y", 26, "z",
	// 記号
	32, " ", 33, ".", 34, "<", 35, ">",
	36, ",", 37, ";", 38, "-",
	0, "NORMAL"
};

// *** 記号 ***
cTable	SyTable[] = {
	4, "§",
	9, "'",
	10, "“",
	11, "”",
	12, "･",
	13, "-",
	14, "--",
	15, "™",
	18, "€",
	19, "£",
	24, "©",
	28, "®",
	29, "°",
	30, "±",
	31, "²",
	32, "³",
	35, "・" ,
	36, "¹",
	40, "½",
	43, "×",
	44, "÷",
	0, "SYMBOL"
};

// *** 特殊記号 ***
cTable	SpTable[] = {
	1, "!",
	2, "\"",
	3, "#",
	4, "$",
	5, "%",
	6, "&amp;",
	7, "’",
	8, "(",
	9, ")",
	10, "*",
	11, "+",
	15, "/",
	16, "0", 17, "1", 18, "2", 19, "3", 20, "4", 21, "5", 22, "6", 23, "7", 24, "8", 25, "9",
	26, ":", 27, ";",
	28, "&lt;",
	29, "=",
	30, "&gt",
	31, "?",
	32, "@",
	33, "[",
	34, "\\",
	35, "]",
	36, "^",
	37, "_",
	38, "‘",
	39, "{",
	40, "|",
	41, "}",
	42, "~",
	43, "□",		// 仮
	0, "SPECIAL"
};

cTable	GrTable[] = {
	1, "à",
	5, "è",
	9, "ì",
	32, "À",
	36, "È",
	40, "Ì",
	0, "GRAVE"
};

cTable	AcTable[] = {
	1, "á",
	5, "é",
	9, "í",
	15, "ó",
	22, "ú",
	32, "Á",
	36, "É",
	40, "Í",
	46, "Ó",
	52, "Ú",
	0, "ACUTE"
};

cTable	CiTable[] = {
	1, "â",
	2, "",
	5, "ê",
	9, "î",
	15, "ô",
	21, "û",
	25, "ŷ",
	32, "Â",
	36, "Ê",
	52, "Û",
	0, "CIRC"
};

// *** tilde ***
cTable	TiTable[] = {
	1, "ã",
	14, "ñ",
	15, "õ",
	32, "Ã",
	45, "Ñ",
	46, "Õ",
	0, "TILDE"
};

// *** umlaut ***
cTable	UmTable[] = {
	1, "ä",
	9, "ï",
	15, "ö",
	21, "ü",
	32, "Ä",
	40, "Ï",
	46, "Ö",
	52, "Ü",
	0, "UML"
};

cTable	LaTable[] = {
	1, "å",
	5, "ě",
	15, "ö",
	32, "Å",
	36, "Ě",
	0, "LATIN"
};

cTable	La2Table[] = {
	50, " S",
	0, "LATIN2"
};

cTable	CeTable[] = {
	3, "ç",
	34, "Ç",
	0, "CEDIL"
};

cTable	SlTable[] = {
	15, "ø",
	46, "Ø",
	0, "SLASH"
};

cTable	ElTable[] = {
	1, "æ",
	15, "œ",
	32, "Æ",
	46, "Œ",
	0, "ELIG"
};

cTable	GkTable[] = {
	13, "μ",
	16, "π",
	22, "φ",
	53, "Φ",
	0, "GREEK"
};

// *** UNICODE ***
cTable	UnTable[] = {
	0x0259, "ə",	// schwa
	0x2070, "<sup>0</sup>",
	0x2074, "<sup>4</sup>",
	0x2075, "<sup>5</sup>",
	0x2079, "<sup>9</sup>",
	0x2153, "⅓",
	0x2192, "→",
	0x2283, "□",		// 仮
	0x25a0, "■",
	0x25a1, "□",
	0x25b2, "▲",
	0x25ba, "►",
	0x2605, "★",
	0x260e, "□", 		// 仮
	0x2660, "♠",
	0x2663, "♣",
	0x2665, "♥",
	0x2666, "♦",
	0x266D, "♭",
	0x266F, "♯",
	0x2726,	"✦",
	0x3003, "〃",
	0, "UNICODE"
};

// *** 発音記号 ***
cTable	PhTable[] = {
	' ', " ", '/', "/", '-', "-", '<', "<",
	'>', ">", ';', ";", 
	'~', "",		// ???
	'%', ",",
	'a', "a", 'b', "b", 'd', "d", 'e', "e",
	'f', "f", 'g', "g", 'h', "h", 'i', "i",
	'j', "j", 'k', "k", 'l', "l", 'm', "m",
	'n', "n", 'o', "o", 'p', "p", 'r', "r",
	's', "s", 't', "t", 'u', "u", 'v', "v",
	'w', "w", 'x', "x", 'z', "z",
	':', "ː",	// Triangular Colon
	'{', "æ",		// ae
	'@', "ə",	// Schwa
	'A', "ɑ",	// Alpha
	'D', "Ð",		// Eth
	'I', "ɪ",	// Capital I
	'N', "ŋ",	// Eng
	'O', "ɔ",	// Open O
	'Q', "ɒ",	// Turned Alpha
	'S', "ʃ",	// Esh
	'T', "θ",	// Theta
	'U', "ʊ",	// Upsilon
	'V', "ʌ",	// Turned V
	'Z', "ʒ",	// Ezh
	'3', "ɜ",	// Open E
	0, "PHONETIC"
};

// １文字復号する
std::string DecodeChar(int c, cTable* Table) {
	for (int i = 0; Table[i].Code != 0; ++i) {
		if ( Table[i].Code == c ){
            return Table[i].Plain;
		}
	}
    assert(false);
	return "";
}

//
//	復号
//
std::string decode( FILE *fp )
{
	/* 6bit毎に分ける */
    std::string compbuf;
    compbuf.reserve(4096);
	for (;;){
		const unsigned char c1 = fgetc( fp );
		const unsigned char d1 = ( c1 >> 2 ) & 0x3f;
		if ( d1 == 0 ) break;
		compbuf += d1;
	
		const unsigned char c2 = fgetc( fp );
		const unsigned char d2 = (( c1 << 4 ) & 0x30) | (( c2 >> 4 ) & 0x0f);
		if ( d2 == 0 ) break;
		compbuf += d2;
		
		const unsigned char c3 = fgetc( fp );
		const unsigned char d3 = (( c2 << 2 ) & 0x3c) | (( c3 >> 6 ) & 0x03);
		if ( d3 == 0 ) break;
		compbuf += d3;
	
        const unsigned char d4 = c3 & 0x3f;
		if ( d4 == 0 ) break;
		compbuf += d4;
	}

	/* 平文に変換する */
    std::string::const_iterator p;
    std::string ascbuf;
    ascbuf.reserve(4096);
	for (p = compbuf.begin(); *p; ++p){
		if ( *p < 40 ){ // 英小文字・通常記号
			ascbuf += DecodeChar( *p, CTable );
		} else if ( *p == 40 ){ // grave
			++p;
			ascbuf += DecodeChar( *p, GrTable );
		} else if ( *p == 41 ){ // acute
			++p;
			ascbuf += DecodeChar( *p, AcTable );
		} else if ( *p == 42 ){ // circ
			++p;
			ascbuf += DecodeChar( *p, CiTable );
		} else if ( *p == 43 ){ // tilde
			++p;
			ascbuf += DecodeChar( *p, TiTable );
		} else if ( *p == 44 ){ // umlaut
			++p;
			ascbuf += DecodeChar( *p, UmTable );
		} else if ( *p == 47 ){ // Latin
			++p;
			ascbuf += DecodeChar( *p, LaTable );
		} else if ( *p == 49 ){ // Cedil
			++p;
			ascbuf += DecodeChar( *p, CeTable );
		} else if ( *p == 50 ){ // Slash
			++p;
			ascbuf += DecodeChar( *p, SlTable );
		} else if ( *p == 51 ){ // elig
			++p;
			ascbuf += DecodeChar( *p, ElTable );
		} else if ( *p == 52 ){ // ギリシャ文字
			++p;
			ascbuf += DecodeChar( *p, GkTable );
		} else if ( *p == 58 ){ // Latin2
			++p;
			ascbuf += DecodeChar( *p, La2Table );
		} else if ( *p == 59 ){ // 記号
			++p;
			ascbuf += DecodeChar( *p, SyTable );
		} else if ( *p == 61 ){ // 英大文字
			++p;
			ascbuf += 'A' + *p - 32;
		} else if ( *p == 62 ){ // 特殊記号
			++p;
			ascbuf += DecodeChar( *p, SpTable );
		} else if ( *p == 63 ){ // UNICODE
			const unsigned int ucode = ((*(p+1)-1) << 12) + ((*(p+2)-1) << 8) + ((*(p+3)-1) << 4) + (*(p+4)-1);
			p += 4;
			ascbuf += DecodeChar( ucode, UnTable );
		} 
	}

	/* タグをhtml形式に変換する(その１) */
	/* 入れ子の外側 */
    std::string tagbuf1;
    tagbuf1.reserve(4096);
	for (p = ascbuf.begin(); *p; ++p){
		if (*p == '<') {
			if (*(p+1) == 'c') {
				/* 注釈 */
				int n = 0;
				p += 3;
				while (n != 0 || *p != '>') {
					if ( *p == '<' ) n++;
					if ( *p == '>' ) n--;
					tagbuf1 += *p++;
				}
				continue;
			} else if (*(p+1) == 'f') {
				/* 語句？ */
				int n = 0;
				p += 3;
				while (n != 0 || *p != '>') {
					if ( *p == '<' ) n++;
					if ( *p == '>' ) n--;
					tagbuf1 += *p++;
				}
				continue;
			}
		}	
		tagbuf1 += *p;
	}

	/* タグをhtml形式に変換する(その２) */
	/* 入れ子の内側 */	
    std::string tagbuf2;
    tagbuf2.reserve(4096);
	for (p = tagbuf1.begin(); *p; ++p) {
		if (*p != '<') {
			tagbuf2 += *p;
			continue;
		}
		switch (*(p+1)){
		case 'b': // 太字
			tagbuf2 += "<b>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</b>";
			break;
		case 'e': // 強調
			tagbuf2 += "<b>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</b>";
			break;
		case 'i': // 斜体
			tagbuf2 += "<i>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</i>";
			break;
		case 'g': // 文法要素
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			break;
		case 'h': // 分綴文字
            tagbuf2 += "･";
			p += 2;
			break;
		case 'm': // ？？？
			for (p += 3; *p != '>'; ++p) { }
			break;
		case 'n': // AM/STRONG
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			break;
		case 'u': // アンダーライン
            tagbuf2 += '\'';
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			break;
		case 'v': // ？？？
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			break;
		case 'w': // STRONG/WEAK
			for (p += 3; *p != '>'; ++p) { }
			break;
		case 'x': // 相互参照
			tagbuf2 += "<b>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</b>";
			break;
		case 'A':
		case 'B': // 発音アイコン
			for (p += 3; *p != '>'; ++p) { }
			break;
        case 'D': // <DW>
            p += 4; // "<DW>".size()
            break;
		case 'E': // 分綴(発音記号)
			tagbuf2 += "<sup>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</sup>";
			break;
		case 'F': // 上付き(発音記号)
			tagbuf2 += "<sup>";
			for (p += 3; *p != '>'; ++p) { tagbuf2 += *p; }
			tagbuf2 += "</sup>";
			break;
		case 'S': // 記号
			p += 3;
            if ( strncmp( &*p, "TCdiam", 6 ) == 0 ){ // 使用頻度のダイヤ
                tagbuf2 += "♦";
			}
			for ( ; *p != '>'; ++p) { }
			break;
		case 'q': // 変化形？ 
		case 'r': // 見出し?
			p += 2;
			break;
		default:
            tagbuf2 += *p;
		}
	}

	/* 発音記号を変換する */
    std::string htmtxt;
    htmtxt.reserve(4096);
	int accent = 0;
	for (p = tagbuf2.begin(); *p; ++p) {
		if (*p == '<' && *(p+1) == 'p') {
			int n = 0;
			for (p += 3; n != 0 || *p != '>'; ++p) {
				if (*p == '<') n++;
				else if (*p == '>') n--;
				
				if ( *p == '\'' ){ // アクセント
					if ( accent == 0 ){
						htmtxt += '\'';
						accent = 2;
					}
					continue;
				} else {
					htmtxt += DecodeChar( *p, PhTable );
				};
				if ( accent ) accent--;
			}
			continue;
		}
        htmtxt += *p;
	}
	
	return htmtxt;
}

//	変化形を加工する
std::vector<std::string> GetVariation(const std::string& var, const std::string& title)
{
    // titleが 2 word以上ならそれに対応する
    int twcnt = 1;
    for (std::string::const_iterator i = title.begin(); i != title.end(); ++i){
        if (*i == ' ') ++twcnt;
    }

    std::vector<std::string> ret;
    std::string s;
    int w = twcnt;
	for (std::string::const_iterator p = var.begin(); p != var.end(); ){
		if (*p == ' ') {
			p++;
            if ( ! s.empty()) {
                if (--w == 0) {
                    ret.push_back(s);
                    s = "";
                    w = twcnt;
                } else {
                    s += ' ';
                }
            }
            while (*p == ' ') ++p;
			continue;
		}
		s += *p++;
	}
    if ( ! s.empty()) {
        ret.push_back(s);
    }

    /*printf("[%s]%d[%s]", title.c_str(), twcnt, var.c_str());
    for (std::vector<std::string>::iterator it = ret.begin(), end = ret.end(); it != end; ++it) {
        printf("<%s>", it->c_str());
    }
    printf("\n");*/

    return ret;
}

//
//	HTMLに変換する
//
int ConvHtml( FILE *fp, FILE *OutFp, bool is_wordbank )
{
	// 本文項目数
	const int HonbunNum = fgetc( fp );
	
	// 見出し項目フラグ
	char Flg1 = fgetc( fp );

    std::string title;
    std::vector<std::string> variation;
    std::string phonetic;
    std::string syntax;
    std::string hyphen;
    std::string note;

	if ( Flg1 & 0x01 ){ // 見出し
		const char Flg2 = fgetc( fp );
		if ( Flg2 & 0x01 ){ // 見出し語
			title = decode( fp );
		}
		if ( Flg2 & 0x02 ){ // 異形
            syntax = decode( fp );
		}
		if ( Flg2 & 0x08 ){ // 品詞
            std::string dummy = decode( fp );
            // never come to here
		}
		if ( Flg2 & 0x10 ){ // 変化形
            assert( ! title.empty());
			variation = GetVariation( decode( fp ), title );
		}
		if ( Flg2 & 0x20 ){ // 用法
			std::string dummy = decode( fp );
            // never come to here
		}
		if ( Flg2 & 0x80 ){ // 発音
			phonetic = decode( fp );
		}
	}
	if ( Flg1 & 0x02 ){ // 追加データ
		const char Flg2 = fgetc( fp );
		if ( Flg2 & 0x01 ){ // 別見出しにした理由
			if ( ! syntax.empty() ) { syntax += "<br />"; }
            syntax += decode( fp );
		}
		if ( Flg2 & 0x02 ){ // 構文
			if ( ! syntax.empty() ) { syntax += "<br />"; }
            syntax += decode( fp );
		}
		if ( Flg2 & 0x04 ){ // 分綴??
			hyphen = decode( fp );
		}
		if ( Flg2 & 0x08 ){ // 注釈
			note = decode( fp );
		}
		if ( Flg2 & 0x10 ){ // 章節番号
			std::string dummy = decode( fp );
            // never come to here
		}
		if ( Flg2 & 0x20 ){ // 頻度
			const char dummy = fgetc( fp );
            // never come to here
		}
		
	}
	if ( Flg1 & 0x80 ){ // 音声データ
		for(;;){
			const char Flg2 = fgetc( fp );
			if ( Flg2 == 0 ) break;
			const char dummy1 = fgetc( fp );
			const char dummy2 = fgetc( fp );
			const char dummy3 = fgetc( fp );
		}
	}
	
	/* ワードバンクでないとき */
    std::string work;
	if ( !is_wordbank ){ // 見出し
		if ( ! hyphen.empty() ){
            work = hyphen;
		}
		
		if ( ! phonetic.empty() ){ // 発音記号
            work += " <span class=\"phonetic\">";
            work += phonetic;
            work += "</span>";
		}

		if ( ! variation.empty() ){ // 変化形
            work += " (";
            for (std::vector<std::string>::const_iterator it = variation.begin(), end = variation.end(); it != end; ++it) {
                if (it != variation.begin()) {
                    work += " ";
                }
                work += *it;
            }
            work += ")";
		}
		
		if ( ! note.empty() ){ // 注釈
			work += "<br />";
			work += note;
		}
		
		// 見出しを書く
        std::string id(title);
        std::map<std::string,int>::iterator it = g_id.find(title);
        if (it != g_id.end()) {
            int cnt = it->second + 1;
            g_id[title] = cnt;
            id += '_';
            id += "0123456789abcdef"[cnt];
        } else {
            g_id[title] = 1;
        }

        fprintf( OutFp, "<d:entry id=\"%s\" d:title=\"%s\" >\n", id.c_str(), title.c_str() );
		
        for (std::vector<std::string>::const_iterator it = variation.begin(), end = variation.end(); it != end; ++it) {
            if ((*it)[0] == '/') {
                // 先頭が / なら発音
                //fprintf( OutFp, "<span class=\"pron\">%s</span>\n", it->c_str());
            } else {
                fprintf( OutFp, "<d:index d:value=\"%s\" />\n", it->c_str());
            }
        }

        // ^1 などに対処
        std::string ptitle(title);
        size_t len = ptitle.size();
        if (len > 3 && ptitle[len-2] == '^') {
            ptitle = ptitle.substr(0, len-2) + "<sup>" + ptitle[len-1] + "</sup>";
            //printf("%s %s\n", title.c_str(), ptitle.c_str());
        }
		fprintf( OutFp, "<h1>%s</h1><span class=\"info\">%s</span>\n", ptitle.c_str(), work.c_str() );

		if ( ! syntax.empty() ){
			fprintf( OutFp, "<p>%s</p>\n", syntax.c_str() );
		}
	}
	
	for (int i = 0; i < HonbunNum; ++i) { // 本文
        std::string subtitle;
        std::string hinshi;
        std::string subno;
        std::string explain;
        std::string alternative;

		// 本文フラグ
		Flg1 = fgetc( fp );
	
		if ( Flg1 & 0x01 ){ // 小見出し
			const char Flg2 = fgetc( fp );
			if ( Flg2 & 0x01 ){ // 見出し: 小見出しは定義語とする
                subtitle = "<span class=\"subtitle\">";
                subtitle += decode( fp );
                subtitle += "</span>";
			}
			if ( Flg2 & 0x02 ){ // 別見出し
				if ( ! subtitle.empty()) { subtitle += ' '; }
                subtitle += decode( fp );
			}
			if ( Flg2 & 0x08 ){ // 品詞; Word Class
				hinshi = "<span class=\"wordclass\">";
                hinshi += decode( fp );
                hinshi += ":</span>";
			}
			if ( Flg2 & 0x10 ){ // 変化形
				std::string dummy = decode( fp );
                if ( ! dummy.empty()) {
                    //printf("VAR %s %s\n", title.c_str(), dummy.c_str());
                }
			}
			if ( Flg2 & 0x20 ){ // 見出し全体の説明
                explain = decode( fp );
			}
		}
		// 用法
		if ( Flg1 & 0x02 ){
			const char Flg2 = fgetc( fp );
			if ( Flg2 & 0x01 ){ // 特記事項; Alternative forms
				alternative = decode( fp );
			}
			if ( Flg2 & 0x02 ){ // 異形
				std::string dummy = decode( fp );
                // never come to here
			}
			if ( Flg2 & 0x04 ){ // 分綴
				std::string dummy = decode( fp );
                if ( ! dummy.empty()) {
                    //printf("BS %s\n", dummy.c_str());
                }
			}
			if ( Flg2 & 0x08 ){ // 注釈
				std::string dummy = decode( fp );
                // never come to here
			}
			if ( Flg2 & 0x10 ){ // 用法: disapproval, emphasis, etc
				std::string dummy = decode( fp );
                if ( ! dummy.empty()) {
                    //printf("YH %s\n", dummy.c_str());
                }
			}
			if ( Flg2 & 0x20 ){ // ？？
				const char dummy1 = fgetc( fp );
			}
			if ( Flg2 & 0x80 ){ // 番号または記号
				subno = decode( fp );
			}
		}

		/* 特記事項を書く */
		if ( ! alternative.empty() ){
			fprintf( OutFp, "<span class=\"alternative\"><p>%s</p></span>\n", alternative.c_str() );
		}
		
		/* 小見出しを書く */
        std::string work;
        if ( ! subno.empty() ){
            if ( subno == "@" ){
                subno = "♦";
            } else {
                work = "<span class=\"subno\">" + subno + "</span>";
            }
        }
        if ( ! subtitle.empty() ){
            if ( ! work.empty() ) { work += ' '; }
            work += subtitle;
        }
        if ( ! hinshi.empty() ){
            if ( ! work.empty() ) { work += ' '; }
            work += hinshi;
        }
        if ( ! explain.empty() ){
            if ( ! work.empty() ) { work += ' '; }
            work += "<span class=\"graminfo\">" + explain + "</span>";
        }
		if ( ! is_wordbank && ! work.empty() ){
            fprintf( OutFp, "<p>%s</p>\n", work.c_str() );
        }
		
		/* 0x04 */
		if ( Flg1 & 0x04 ){
			const char Flg2 = fgetc( fp );
			std::string dummy = decode( fp );
            // never come to here
		}
		/* 本文 */
		if ( Flg1 & 0x10 ){
			const char Flg2 = fgetc( fp );
			if ( Flg2 & 0x01 ){ // 意味
                std::string t = decode( fp );
				fprintf( OutFp, "  <p>%s</p>\n", t.c_str());
			}
			if ( Flg2 & 0x02 ){ // 例文
				const int cnt = fgetc(fp);
                if ( is_wordbank ){
                    for (int j = 0; j < cnt; ++j){
                        std::string t = decode(fp);
						fprintf( OutFp, "<dt>%s</dt>\n", title.c_str());
						fprintf( OutFp, "<dd><p>%s</p></dd>\n", t.c_str());
					}
				} else {
                    std::string examples;
                    for (int j = 0; j < cnt; ++j){
                        std::string t = decode(fp);
                        examples += "<li>";
                        examples += t;
                        examples += "</li>";
                    }
                    fprintf( OutFp, "  <ul>%s</ul>\n", examples.c_str());
                }
			}
			if ( Flg2 & 0x04 ){ // 表
			}
			if ( Flg2 & 0x08 ){ // 参照 ex: "BRIT; in AM, use <b>rambunctious</b>"
				const int cnt = fgetc( fp );
				for (int j = 0; j < cnt; ++j){
					std::string dummy = decode( fp );
                    //printf("%s\n", dummy.c_str() );
				}
			}
			if ( Flg2 & 0x10 ){ // ？？
				const int cnt = fgetc( fp );
				for (int j = 0; j < cnt; ++j){
                    std::string t = decode( fp );
					fprintf( OutFp, "  <p><i>%s</i></p>\n", t.c_str() );
				}
			}
			if ( Flg2 & 0x40 ){ // 参照
                std::string t = decode( fp );
				fprintf( OutFp, "  <p>%s</p>\n", t.c_str() );
			}
		}
		/* 関連語 */
		if ( Flg1 & 0x40 ){
			const char Flg2 = fgetc( fp );
			if ( Flg2 & 0x01 ){ // 同義語
                std::string t = decode( fp );
				fprintf( OutFp, "  <p class=\"synonyms\">＝ %s</p>\n", t.c_str() );
			}
			if ( Flg2 & 0x02 ){ // 同義語
                std::string t = decode( fp );
				fprintf( OutFp, "  <p class=\"antonyms\">⇔ %s</p>\n", t.c_str() );
			}
		}
		/* 画像 */
		if ( Flg1 & 0x80 ){
			const char Flg2 = fgetc( fp );
			const char dummy1 = fgetc( fp );
			const char dummy2 = fgetc( fp );
			const char dummy3 = fgetc( fp );
			const char dummy4 = fgetc( fp );
		}
	}
	if ( !is_wordbank ){
		fprintf( OutFp, "</d:entry>\n" );
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	bool is_wordbank = false;

    int		Base[4800];	// インデックス・ベース・テーブル
    unsigned short	Offset[40000];	// インデックス・オフセット・テーブル
    int		OffsetNum = 256;	// ベース当たりのオフセットの数

	if ( argc > 2 && strcmp( argv[2], "-w" ) == 0 ){
		is_wordbank = true;
		OffsetNum = 4;
	}
	
	/* 入力ファイル */
    std::string infile(argv[1]);
	FILE* fp = fopen( infile.c_str(), "rb" );
	if ( ! fp ) {
		printf( "入力ファイルがオープンできない\n" );
		return 1;
	}

	/* 出力ファイル */
    const std::string outfile("cobuild.xml");	
	FILE* OutFp = fopen( outfile.c_str(), "w" );
	if ( ! OutFp ){
		printf( "出力ファイルがオープンできない\n" );
		return 1;
	}

    struct cHead {
        char Copyright[64];
        char Dumy1[16];
        int	 DataNum;
        int	 Dumy2;
        int	 IndexBaseNum;
        int	 IndexOffsetNum;
        int	 Dumy3;
        int	 IndexPos;
        int	 DataPos;
        int	 Dumy4;
        char Dumy[16];
    };

    cHead header;

	// ヘッダを読む
	fread( &header, sizeof( cHead ), 1, fp );
	
	// インデックス部へシーク
	fseek( fp, header.IndexPos, SEEK_SET );
	
	// ベーステーブルを読む
	for (int i = 0;i < header.IndexBaseNum; ++i){
		fread( &Base[i], 4, 1, fp );
	}

	// オフセットテーブルを読む
	for (int i = 0;i < header.IndexBaseNum; ++i){
		for (int j = 0; j < OffsetNum; ++j){
			fread( &Offset[i * OffsetNum + j], 2, 1, fp );
		}
	}

	fprintf( OutFp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( OutFp, "<d:dictionary xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:d=\"http://www.apple.com/DTDs/DictionaryService-1.0.rng\">\n" );

	for (int i = 0; i < header.IndexBaseNum; ++i){
		for (int j = 0; j < OffsetNum; ++j){
			/* おしまい? */
			if ( header.IndexOffsetNum - 1 <= i * OffsetNum + j ) break;
			
			/* データ部へシーク */
			int st = fseek( fp, header.DataPos + Base[i] + Offset[i * OffsetNum + j] * 4, SEEK_SET );
			if ( st ){
				printf( "SEEK ERROR: i=%d j=%d\n", i, j );
			}
			st = ConvHtml( fp, OutFp, is_wordbank );
			if ( st ){
				printf( "CONV ERROR: i=%d j=%d\n", i, j );
			}
            if (i==133 && j==109) goto END;
		}
	}
END:
	fprintf( OutFp, "</d:dictionary>\n" );
	
	/* 後始末 */
	fclose( fp );
	fclose( OutFp );
}


// TODO: <b>xx</b> <b>xx</b> でスベース無視される？

