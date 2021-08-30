// fcsub.c
//  by takuya matsubara
#include "fcsub.h"

unsigned int rndwork;   // �^������

// �S�@�\�̏�����
void fc_init(void)
{
    ppu_vsync();      // V�u�����N�҂�
    ppu_enable(0);    // �\���𖳌���
    rndwork = 0x005a; // �^�������̃V�[�h�l��ݒ�
    sound_init();     // �T�E���h�̏�����
    bg_cls();         // BG�̏�����
    sp_cls();         // �X�v���C�g�̏�����
    bg_scroll(0, 0);  // BG�X�N���[���ݒ�
}

// PPU�A�h���X�ݒ�(�A�h���X0x0000�`0xffff)
void ppu_address(unsigned short adr)
{
    PPUADDR = adr >> 8;    // �A�h���X���8bit
    PPUADDR = adr & 0xff;  // �A�h���X����8bit
}

// BG�̏�����
void bg_cls(void)
{
    short i;
    ppu_address( 0x2000 );
    for(i=0; i<0x800; i++){
        PPUDATA = 0;    // �l�[���e�[�u���ƃA�g���r���[�g�̏���
    }
}

// �X�v���C�g�̏�����
void sp_cls(void)
{
    char i;
    for(i=0; i<16; i++){
        sp_remove(i);    // �X�v���C�g����ʊO��
    }
}

// �^�������̎擾
unsigned int random(void)
{
    rndwork ^= (rndwork << 3);
    rndwork ^= (rndwork >> 5);
    rndwork ^= (rndwork << 4);
    return(rndwork);   // �߂�l=����0�`0xffff
}

// �R���g���[���[1����
unsigned char controller1(void)
{
    unsigned char ret=0;
    char i;
    JOYPAD1 = 0x01;    // �{�^�����͂��J�n
    JOYPAD1 = 0x00;    // �{�^�����͂����b�`
    for(i=0; i<8; i++){
        ret <<= 1;
        if( JOYPAD1 & (1<<0) ) ret |= 1;  // �{�^���������ꂽ
    }
    return(ret);   // �߂�l=�{�^���̏��(8bit)
}

// �T�E���h������
void sound_init(void)
{
    #define DUTY 2      // �g�`�̃f���[�e�B��ݒ�(2=50%)
    #define VOLUME 8    // ���̃{�����[��(0�`15)
    #define SHIFT 2     // ���g���ω���(0�`7)
    #define DIVID 4     // ���g���ω�����(0�`7)
    #define DIR 1       // ���g���ω����@(1=�����ɕω�) 
    PULSE1CTR1 = (DUTY<<6)+(1<<5)+(1<<4)+VOLUME;    // ��`�g1���䃌�W�X�^1
    PULSE1CTR2 = (1<<7)+(DIVID<<4)+(DIR<<3)+SHIFT;   // ��`�g1���䃌�W�X�^2

    SNDCHANNEL = (1<<0);      // ��`�g1�݂̂��g�p����
}

// ��`�g1�Đ�(���g��)
void pulse1(unsigned short hz)
{
    unsigned short tmp;
    tmp = (unsigned short)((1790000/16)/hz)-1;
    PULSE1FREQ1 = tmp & 0xff; // ��`�g1���g��1(bit7�`0)
    PULSE1FREQ2 = tmp >> 8;   // ��`�g1���g��2(bit10�`8)
}

// �J���[�p���b�g�ݒ�(�i�[��|�C���^,�I�t�Z�b�g0�`7,�p���b�g��1�`8)
void ppu_palette(char *ptable,char ofs,char size)
{
    size *= 4;     // 1�p���b�g������4�o�C�g
    ppu_address( 0x3f00+(ofs*4) );    // �J���[�p���b�g�̃A�h���X
    while(size--){
        PPUDATA = *ptable++;
    }
}

// �p�^�[���e�[�u���ݒ�(�i�[��|�C���^,�I�t�Z�b�g0�`511,����1�`512)
void ppu_pattern(unsigned char *ptable,short ofs,short size)
{
    size *= 16;   // 1��������16�o�C�g
    ppu_address(ofs*16);   // �p�^�[���e�[�u���̃A�h���X
    while(size--){
        PPUDATA = *ptable++;
    }
}

// ��ʕ\���̗L����(1=�\��ON/0=�\��OFF)
void ppu_enable(char on)
{
    PPUMASK = on*((1<<4)+(1<<3))+6;    // PPU�}�X�N(bit4=SP�L��/bit3=BG�L��)
}

// V�u�����N��҂�
void ppu_vsync(void)
{
    while((PPUSTATUS & (1<<7))==0);    // bit7��0�̏ꍇ�A��ʕ`�撆
}

// BG�X�N���[���ݒ�(X���W�I�t�Z�b�g0�`511,Y���W�I�t�Z�b�g0�`239)
void bg_scroll(unsigned short x, unsigned char y)
{
    PPUSCROLL = (x & 0xff);   // X���W(0�`255)
    PPUSCROLL = y;            // Y���W
    PPUCTRL = 0x08+((x >> 8)& 1);    // X���W256�ȏ�̏ꍇ�͉��B
}

// BG�L�����N�^�\��(X���W0�`31,Y���W0�`29,�^�C���ԍ�0�`255)
void bg_printch(char x,char y,unsigned char c)
{
    // �����U�Ƃ��āAY���W��32�`61�̏ꍇ�͉��B�ɏ������݂��ł���
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //�l�[���e�[�u���̃A�h���X
    PPUDATA = c;
}

// BG������\��(X���W0�`31,Y���W0�`29,������|�C���^)
void bg_printstr(char x,char y,char *str)
{
    ppu_address( 0x2000+x+((unsigned short)y*32) );    //�l�[���e�[�u���̃A�h���X
    while (*str != 0){
        PPUDATA = *str++;
    }
}

// BG�A�g���r���[�g�ݒ�(X���W0�`31,Y���W0�`29,�A�g���r���[�g0�`255)
void bg_attrib(char x,char y,unsigned char atr)
{
    ppu_address( 0x23C0+(x/4)+((y/4)*8) );    // BG�A�g���r���[�g�̃A�h���X
    PPUDATA = atr;
}

// 10�i��5�P�^�ϊ�(�i�[��|�C���^,���l0�`65535)
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

// 16�i��2�P�^�ϊ�(�i�[��|�C���^,���l0�`255)
void num_to_hex(char *str,unsigned char num)
{
    unsigned char tmp;
    tmp = '0'+(num >> 4);             // 0�`9�ɕϊ�
    if(tmp > '9') tmp += 'A'-('9'+1); // A�`F�ɕϊ�
    str[0] = tmp;  // ���4bit
    tmp = '0'+(num & 0x0f);           // 0�`9�ɕϊ�
    if(tmp > '9')tmp += 'A'-('9'+1);  // A�`F�ɕϊ�
    str[1] = tmp;  // ����4bit
    str[2] = 0; // NULL
}

// �X�v���C�g���[�N��DMA�]�����J�n����
void sp_dmastart(void)
{
    OAMADDR = 0;            // �]����OAM�A�h���X
    OAMDMA = SPWADDR >> 8;  // �]�����A�h���X���8bit
}

// �X�v���C�g4�� �ǉ�(X���W,Y���W,�^�C��0�`255,�A�g���r���[�g0�`255)
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

// �X�v���C�g4�� ����(�X�v���C�g�ԍ�0�`15)
void sp_remove(char i)
{
    sp_ofs(i,OUTSIDE,OUTSIDE);    // �X�v���C�g����ʊO��
}

// �X�v���C�g4�� �\���ύX(�X�v���C�g�ԍ�0�`15,�^�C��0�`255,�A�g���r���[�g0�`255)
void sp_chr(char num,unsigned char tile,unsigned char attr)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // �X�v���C�g���[�N�������݃A�h���X
    ptr[1+(64*0)] = tile;    // �X�v���C�gnum+0 �^�C���ԍ�
    ptr[2+(64*0)] = attr;    // �X�v���C�gnum+0 �A�g���r���[�g
    ptr[1+(64*1)] = tile+1;  // �X�v���C�gnum+1 �^�C���ԍ�
    ptr[2+(64*1)] = attr;    // �X�v���C�gnum+1 �A�g���r���[�g
    ptr[1+(64*2)] = tile+2;  // �X�v���C�gnum+2 �^�C���ԍ�
    ptr[2+(64*2)] = attr;    // �X�v���C�gnum+2 �A�g���r���[�g
    ptr[1+(64*3)] = tile+3;  // �X�v���C�gnum+3 �^�C���ԍ�
    ptr[2+(64*3)] = attr;    // �X�v���C�gnum+3 �A�g���r���[�g
}

// �X�v���C�g4�� �A�j������(�X�v���C�g�ԍ�0�`15)
void sp_anime(char num)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // �X�v���C�g���[�N�A�h���X
    ptr[1+(64*0)] ^= 4;   // �X�v���C�gnum+0 �^�C���ԍ�
    ptr[1+(64*1)] ^= 4;   // �X�v���C�gnum+1 �^�C���ԍ�
    ptr[1+(64*2)] ^= 4;   // �X�v���C�gnum+2 �^�C���ԍ�
    ptr[1+(64*3)] ^= 4;   // �X�v���C�gnum+3 �^�C���ԍ�
}

// �X�v���C�g4�� �ړ�(�X�v���C�g�ԍ�0�`15,X���W0�`255,Y���W0�`255)
void sp_ofs(char num,unsigned char x,unsigned  char y)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // �X�v���C�g���[�N�A�h���X
    ptr[0+(64*0)] = y;       // �X�v���C�gnum+0 Y���W
    ptr[3+(64*0)] = x;       // �X�v���C�gnum+0 X���W
    ptr[0+(64*1)] = y;       // �X�v���C�gnum+1 Y���W
    ptr[3+(64*1)] = x+8;     // �X�v���C�gnum+1 X���W
    ptr[0+(64*2)] = y+8;     // �X�v���C�gnum+2 Y���W
    ptr[3+(64*2)] = x;       // �X�v���C�gnum+2 X���W
    ptr[0+(64*3)] = y+8;     // �X�v���C�gnum+3 Y���W
    ptr[3+(64*3)] = x+8;     // �X�v���C�gnum+3 X���W
}

// �X�v���C�gX���W�擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_getx(char num)
{
    return(*((unsigned char *)SPWADDR+3+(num*4)));
}

// �X�v���C�gY���W�擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_gety(char num)
{
    return(*((unsigned char *)SPWADDR+0+(num*4)));
}

// �X�v���C�g�̃^�C���擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_gettile(char num)
{
    return(*((unsigned char *)SPWADDR+1+(num*4))); // �߂�l=�^�C���ԍ�
}

// �X�v���C�g�Փ˔���(�X�v���C�g�ԍ�,X���W,Y���W)
char sp_hitcheck(char num,unsigned char x,unsigned char y)
{
    unsigned char *ptr;
    ptr = (unsigned char *)SPWADDR+(num*4); // �X�v���C�g���[�N�A�h���X
    x += 16;
    y += 16;
    y -= ptr[0];     // Y���W
    x -= ptr[3];     // X���W
    if((y < 32)&&(x < 32)) return(1);  // �߂�l=1�̏ꍇ�A�Փ˂���
    return(0);  // 0�̏ꍇ�A�Փ˂Ȃ�
}
