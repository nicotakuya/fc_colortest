// fcsub.c
//  by takuya matsubara
#include "fcsub.h"

unsigned int rndwork;   // 疑似乱数

// 全機能の初期化
void fc_init(void)
{
    ppu_vsync();      // Vブランク待ち
    ppu_enable(0);    // 表示を無効化
    rndwork = 0x005a; // 疑似乱数のシード値を設定
    sound_init();     // サウンドの初期化
    bg_cls();         // BGの初期化
    sp_cls();         // スプライトの初期化
    bg_scroll(0, 0);  // BGスクロール設定
}

// PPUアドレス設定(アドレス0x0000～0xffff)
void ppu_address(unsigned short adr)
{
    PPUADDR = adr >> 8;    // アドレス上位8bit
    PPUADDR = adr & 0xff;  // アドレス下位8bit
}

// BGの初期化
void bg_cls(void)
{
    short i;
    ppu_address( 0x2000 );
    for(i=0; i<0x800; i++){
        PPUDATA = 0;    // ネームテーブルとアトリビュートの消去
    }
}

// スプライトの初期化
void sp_cls(void)
{
    char i;
    for(i=0; i<16; i++){
        sp_remove(i);    // スプライトを画面外に
    }
}

// 疑似乱数の取得
unsigned int random(void)
{
    rndwork ^= (rndwork << 3);
    rndwork ^= (rndwork >> 5);
    rndwork ^= (rndwork << 4);
    return(rndwork);   // 戻り値=乱数0～0xffff
}

// コントローラー1入力
unsigned char controller1(void)
{
    unsigned char ret=0;
    char i;
    JOYPAD1 = 0x01;    // ボタン入力を開始
    JOYPAD1 = 0x00;    // ボタン入力をラッチ
    for(i=0; i<8; i++){
        ret <<= 1;
        if( JOYPAD1 & (1<<0) ) ret |= 1;  // ボタンが押された
    }
    return(ret);   // 戻り値=ボタンの状態(8bit)
}

// サウンド初期化
void sound_init(void)
{
    #define DUTY 2      // 波形のデューティ比設定(2=50%)
    #define VOLUME 8    // 音のボリューム(0～15)
    #define SHIFT 2     // 周波数変化量(0～7)
    #define DIVID 4     // 周波数変化周期(0～7)
    #define DIR 1       // 周波数変化方法(1=高音に変化) 
    PULSE1CTR1 = (DUTY<<6)+(1<<5)+(1<<4)+VOLUME;    // 矩形波1制御レジスタ1
    PULSE1CTR2 = (1<<7)+(DIVID<<4)+(DIR<<3)+SHIFT;   // 矩形波1制御レジスタ2

    SNDCHANNEL = (1<<0);      // 矩形波1のみを使用する
}

// 矩形波1再生(周波数)
void pulse1(unsigned short hz)
{
    unsigned short tmp;
    tmp = (unsigned short)((1790000/16)/hz)-1;
    PULSE1FREQ1 = tmp & 0xff; // 矩形波1周波数1(bit7～0)
    PULSE1FREQ2 = tmp >> 8;   // 矩形波1周波数2(bit10～8)
}

// カラーパレット設定(格納先ポインタ,オフセット0～7,パレット数1～8)
void ppu_palette(char *ptable,char ofs,char size)
{
    size *= 4;     // 1パレットあたり4バイト
    ppu_address( 0x3f00+(ofs*4) );    // カラーパレットのアドレス
    while(size--){
        PPUDATA = *ptable++;
    }
}

// パターンテーブル設定(格納先ポインタ,オフセット0～511,枚数1～512)
void ppu_pattern(unsigned char *ptable,short ofs,short size)
{
    size *= 16;   // 1枚あたり16バイト
    ppu_address(ofs*16);   // パターンテーブルのアドレス
    while(size--){
        PPUDATA = *ptable++;
    }
}

// 画面表示の有効化(1=表示ON/0=表示OFF)
void ppu_enable(char on)
{
    PPUMASK = on*((1<<4)+(1<<3))+6;    // PPUマスク(bit4=SP有効/bit3=BG有効)
}

// Vブランクを待つ
void ppu_vsync(void)
{
    while((PPUSTATUS & (1<<7))==0);    // bit7が0の場合、画面描画中
}

// BGスクロール設定(X座標オフセット0～511,Y座標オフセット0～239)
void bg_scroll(unsigned short x, unsigned char y)
{
    PPUSCROLL = (x & 0xff);   // X座標(0～255)
    PPUSCROLL = y;            // Y座標
    PPUCTRL = 0x08+((x >> 8)& 1);    // X座標256以上の場合は画面B
}

// BGキャラクタ表示(X座標0～31,Y座標0～29,タイル番号0～255)
void bg_printch(char x,char y,unsigned char c)
{
    // 裏ワザとして、Y座標が32～61の場合は画面Bに書き込みができる
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //ネームテーブルのアドレス
    PPUDATA = c;
}

// BG文字列表示(X座標0～31,Y座標0～29,文字列ポインタ)
void bg_printstr(char x,char y,char *str)
{
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //ネームテーブルのアドレス
    while (*str != 0){
        PPUDATA = *str++;
    }
}

// BGアトリビュート設定(X座標0～31,Y座標0～29,アトリビュート0～255)
void bg_attrib(char x,char y,unsigned char atr)
{
    ppu_address( 0x23C0+(x/4)+((y/4)*8) );    // BGアトリビュートのアドレス
    PPUDATA = atr;
}

// 10進数5ケタ変換(格納先ポインタ,数値0～65535)
void num_to_str(char *str,unsigned short num)
{
    char i;
    for(i=4;;i--){
        str[i] = (num % 10)+'0';
        num /= 10;
        if(i==0)break;
    }
    str[5] = 0; // NULL
}

// 16進数2ケタ変換(格納先ポインタ,数値0～255)
void num_to_hex(char *str,unsigned char num)
{
    unsigned char tmp;
    tmp = '0'+(num >> 4);             // 0～9に変換
    if(tmp > '9') tmp += 'A'-('9'+1); // A～Fに変換
    str[0] = tmp;  // 上位4bit
    tmp = '0'+(num & 0x0f);           // 0～9に変換
    if(tmp > '9')tmp += 'A'-('9'+1);  // A～Fに変換
    str[1] = tmp;  // 下位4bit
    str[2] = 0; // NULL
}

// スプライトワークのDMA転送を開始する
void sp_dmastart(void)
{
    OAMADDR = 0;            // 転送先OAMアドレス
    OAMDMA = SPWADDR >> 8;  // 転送元アドレス上位8bit
}

// スプライト4個 追加(X座標,Y座標,タイル0～255,アトリビュート0～255)
char sp_append(char x,char y,char tile,char attr)
{
    char i;
    for(i=0; i<16; i++){
        if(sp_gety(i)==OUTSIDE){
            sp_ofs(i, x,y);
            sp_chr(i, tile,attr);
            break;
        }
    }
    return(i);
}

// スプライト4個 消去(スプライト番号0～15)
void sp_remove(char i)
{
    sp_ofs(i,OUTSIDE,OUTSIDE);    // スプライトを画面外に
}

// スプライト4個 表示変更(スプライト番号0～15,タイル0～255,アトリビュート0～255)
void sp_chr(char num,unsigned char tile,unsigned char attr)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // スプライトワーク書き込みアドレス
    ptr[1+(64*0)] = tile;    // スプライトnum+0 タイル番号
    ptr[2+(64*0)] = attr;    // スプライトnum+0 アトリビュート
    ptr[1+(64*1)] = tile+1;  // スプライトnum+1 タイル番号
    ptr[2+(64*1)] = attr;    // スプライトnum+1 アトリビュート
    ptr[1+(64*2)] = tile+2;  // スプライトnum+2 タイル番号
    ptr[2+(64*2)] = attr;    // スプライトnum+2 アトリビュート
    ptr[1+(64*3)] = tile+3;  // スプライトnum+3 タイル番号
    ptr[2+(64*3)] = attr;    // スプライトnum+3 アトリビュート
}

// スプライト4個 アニメ処理(スプライト番号0～15)
void sp_anime(char num)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // スプライトワークアドレス
    ptr[1+(64*0)] ^= 4;   // スプライトnum+0 タイル番号
    ptr[1+(64*1)] ^= 4;   // スプライトnum+1 タイル番号
    ptr[1+(64*2)] ^= 4;   // スプライトnum+2 タイル番号
    ptr[1+(64*3)] ^= 4;   // スプライトnum+3 タイル番号
}

// スプライト4個 移動(スプライト番号0～15,X座標0～255,Y座標0～255)
void sp_ofs(char num,unsigned char x,unsigned  char y)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // スプライトワークアドレス
    ptr[0+(64*0)] = y;       // スプライトnum+0 Y座標
    ptr[3+(64*0)] = x;       // スプライトnum+0 X座標
    ptr[0+(64*1)] = y;       // スプライトnum+1 Y座標
    ptr[3+(64*1)] = x+8;     // スプライトnum+1 X座標
    ptr[0+(64*2)] = y+8;     // スプライトnum+2 Y座標
    ptr[3+(64*2)] = x;       // スプライトnum+2 X座標
    ptr[0+(64*3)] = y+8;     // スプライトnum+3 Y座標
    ptr[3+(64*3)] = x+8;     // スプライトnum+3 X座標
}

// スプライトX座標取得(スプライト番号0～63)
unsigned char sp_getx(char num)
{
    return(*((unsigned char *)SPWADDR+3+(num*4)));
}

// スプライトY座標取得(スプライト番号0～63)
unsigned char sp_gety(char num)
{
    return(*((unsigned char *)SPWADDR+0+(num*4)));
}

// スプライトのタイル取得(スプライト番号0～63)
unsigned char sp_gettile(char num)
{
    return(*((unsigned char *)SPWADDR+1+(num*4))); // 戻り値=タイル番号
}

// スプライト衝突判定(スプライト番号,X座標,Y座標)
char sp_hitcheck(char num,unsigned char x,unsigned char y)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // スプライトワークアドレス
    x += 16;
    y += 16;
    y -= ptr[0];     // Y座標
    x -= ptr[3];     // X座標
    if((y < 32)&&(x < 32)) return(1);  // 戻り値=1の場合、衝突あり
    return(0);  // 0の場合、衝突なし
}
