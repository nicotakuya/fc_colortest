//---------------------------------------------------------------------------
// color test by takuya matsubara
// for famicom
//---------------------------------------------------------------------------
#include "fcsub.h"
#include "pattern.h"

const char str1[] = "FAMICOM COLOR TEST";

char sft=0;	//color オフセット
char palette[16];

//---- スクリーン
void screen_init(void)
{
    char pal;
    char idx;
    char x,y;
    char dx,dy;

    ppu_enable(0);    //表示OFF
    bg_printstr(1,2,(char *)str1);

    for(pal=0; pal<4; pal++){
        for (idx=0; idx < 4; idx++){
            x = (idx*8)+4;
            y = (pal*4)+4;
            for (dy=0; dy < 4; dy++){
                for (dx=0; dx < 4; dx++){
                    bg_printch(x+dx,y+dy,idx);
                }
            }
            bg_attrib(x,y,pal+(pal<<2)+(pal<<4)+(pal<<6));
        }
    }
    ppu_enable(1);
}

void palette_init(void)
{
    char pal;
    char idx;
    char color;
    char x,y;
    unsigned char tmpstr[3];

    for (pal=0; pal < 4; pal++){
        for (idx=0; idx < 4; idx++){
            if(idx==0){
                color= 0x0f;
            }else{
                color = ((idx-1) << 4)+pal+sft;
            }
            x = (idx*8)+2;
            y = (pal*4)+4;
            num_to_hex(tmpstr,color);
            bg_printstr(x,y,tmpstr);
            palette[(pal*4)+idx] = color;
        }
    }
    ppu_palette((char *)palette,0,4);
}

// 
char NesMain()
{
    unsigned char button;
    unsigned char tmpstr[3];
    char timeout;

    fc_init();

    ppu_pattern((char *)pattern,0,512);
    screen_init();

    while (1){
        ppu_vsync();    // (Vblank待ち)
        ppu_enable(0);    //表示OFF
        palette_init();
        ppu_vsync();    // (Vblank待ち)
        ppu_enable(1);    //表示ON

        timeout = 60;
        while (timeout--){
            button = controller1();
            num_to_hex(tmpstr,timeout);
            ppu_vsync();    // (Vblank待ち)
            bg_printstr(28,24,tmpstr);
            bg_scroll(0, 0);
        }
        sft++;
        if(sft==13       )sft=0x10;
        if(sft==(0x10+13))sft=0;
    }

    return 0;
}


