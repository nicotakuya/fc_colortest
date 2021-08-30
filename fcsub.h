// fcsub.h
//  by takuya matsubara
#define PPUCTRL      *(char*)0x2000    // PPU制御レジスタ
#define PPUMASK      *(char*)0x2001    // PPUマスクレジスタ
#define PPUSTATUS    *(char*)0x2002    // PPUステータスレジスタ
#define OAMADDR      *(char*)0x2003    // OAMアドレスポート
#define OAMDATA      *(char*)0x2004    // OAMデータポート
#define PPUSCROLL    *(char*)0x2005    // PPUスクロールレジスタ
#define PPUADDR      *(char*)0x2006    // PPUアドレスレジスタ
#define PPUDATA      *(char*)0x2007    // PPUデータレジスタ
#define JOYPAD1      *(char*)0x4016    // コントローラー1
#define JOYPAD2      *(char*)0x4017    // コントローラー2
#define PULSE1CTR1   *(char*)0x4000    // 矩形波1制御レジスタ1
#define PULSE1CTR2   *(char*)0x4001    // 矩形波1制御レジスタ2
#define PULSE1FREQ1  *(char*)0x4002    // 矩形波1周波数1
#define PULSE1FREQ2  *(char*)0x4003    // 矩形波1周波数2
#define OAMDMA       *(char*)0x4014    // OAM DMAレジスタ
#define SNDCHANNEL   *(char*)0x4015    // サウンドチャンネル制御
#define SPWADDR      0x700             // スプライトワークの格納アドレス

#define C_BLACK      0x0f  // カラーコード 黒色
#define C_RED        0x16  // カラーコード 赤色
#define C_BLUE       0x12  // カラーコード 青色
#define C_GREEN      0x1A  // カラーコード 緑色
#define C_YELLOW     0x28  // カラーコード 黄色
#define C_SKYBLUE    0x22  // カラーコード 空色
#define C_LIMEGREEN  0x29  // カラーコード 黄緑色
#define C_GRAY       0x00  // カラーコード 灰色
#define C_LIGHTGRAY  0x10  // カラーコード 明るい灰色
#define C_WHITE      0x30  // カラーコード 白色

#define BUTTON_A      0x80   // ボタン A
#define BUTTON_B      0x40   // ボタン B
#define BUTTON_SELECT 0x20   // ボタン SELECT
#define BUTTON_START  0x10   // ボタン START
#define BUTTON_UP     0x08   // ボタン 上
#define BUTTON_DOWN   0x04   // ボタン 下
#define BUTTON_LEFT   0x02   // ボタン 左
#define BUTTON_RIGHT  0x01   // ボタン 右

#define OUTSIDE      240     // 画面外の座標

// 全機能の初期化
void fc_init(void);
// PPUアドレス設定(アドレス0x0000～0xffff)
void ppu_address(unsigned short adr);
// BGの初期化
void bg_cls(void);
// スプライトの初期化
void sp_cls(void);
// 疑似乱数の取得
unsigned int random(void);
// コントローラー1入力
unsigned char controller1(void);
// サウンド初期化
void sound_init(void);
// 矩形波1再生(周波数)
void pulse1(unsigned short hz);
// カラーパレット設定(格納先ポインタ,オフセット0～7,パレット数1～8)
void ppu_palette(char *ptable,char ofs,char size);
// パターンテーブル設定(格納先ポインタ,オフセット0～511,枚数1～512)
void ppu_pattern(unsigned char *ptable,short ofs,short size);
// 画面表示の有効化(1=表示ON/0=表示OFF)
void ppu_enable(char on);
// Vブランクを待つ
void ppu_vsync(void);
// BGスクロール設定(X座標オフセット0～511,Y座標オフセット0～239)
void bg_scroll(unsigned short x, unsigned char y);
// BGキャラクタ表示(X座標0～31,Y座標0～29,タイル番号0～255)
void bg_printch(char x,char y,unsigned char c);
// BG文字列表示(X座標0～31,Y座標0～29,文字列ポインタ)
void bg_printstr(char x,char y,char *str);
// BGアトリビュート設定(X座標0～31,Y座標0～29,アトリビュート0～255)
void bg_attrib(char x,char y,unsigned char atr);
// 10進数5ケタ変換(格納先ポインタ,数値0～65535)
void num_to_str(char *str,unsigned short num);
// 16進数2ケタ変換(格納先ポインタ,数値0～255)
void num_to_hex(char *str,unsigned char num);
// スプライトワークのDMA転送を開始する
void sp_dmastart(void);
// スプライト4個 追加(X座標,Y座標,タイル0～255,アトリビュート0～255)
char sp_append(char x,char y,char tile,char attr);
// スプライト4個 消去(スプライト番号0～15)
void sp_remove(char i);
// スプライト4個 表示変更(スプライト番号0～15,タイル0～255,アトリビュート0～255)
void sp_chr(char num,unsigned char tile,unsigned  char attr);
// スプライト4個 アニメ処理(スプライト番号0～15)
void sp_anime(char num);
// スプライト4個 移動(スプライト番号0～15,X座標0～255,Y座標0～255)
void sp_ofs(char num,unsigned char x,unsigned  char y);
// スプライトX座標取得(スプライト番号0～63)
unsigned char sp_getx(char num);
// スプライトY座標取得(スプライト番号0～63)
unsigned char sp_gety(char num);
// スプライトのタイル取得(スプライト番号0～63)
unsigned char sp_gettile(char num);
// スプライト衝突判定(スプライト番号,X座標,Y座標)
char sp_hitcheck(char num,unsigned char x,unsigned char y);


