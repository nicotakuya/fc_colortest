// fcsub.h
//  by takuya matsubara
#define PPUCTRL      *(char*)0x2000    // PPU���䃌�W�X�^
#define PPUMASK      *(char*)0x2001    // PPU�}�X�N���W�X�^
#define PPUSTATUS    *(char*)0x2002    // PPU�X�e�[�^�X���W�X�^
#define OAMADDR      *(char*)0x2003    // OAM�A�h���X�|�[�g
#define OAMDATA      *(char*)0x2004    // OAM�f�[�^�|�[�g
#define PPUSCROLL    *(char*)0x2005    // PPU�X�N���[�����W�X�^
#define PPUADDR      *(char*)0x2006    // PPU�A�h���X���W�X�^
#define PPUDATA      *(char*)0x2007    // PPU�f�[�^���W�X�^
#define JOYPAD1      *(char*)0x4016    // �R���g���[���[1
#define JOYPAD2      *(char*)0x4017    // �R���g���[���[2
#define PULSE1CTR1   *(char*)0x4000    // ��`�g1���䃌�W�X�^1
#define PULSE1CTR2   *(char*)0x4001    // ��`�g1���䃌�W�X�^2
#define PULSE1FREQ1  *(char*)0x4002    // ��`�g1���g��1
#define PULSE1FREQ2  *(char*)0x4003    // ��`�g1���g��2
#define OAMDMA       *(char*)0x4014    // OAM DMA���W�X�^
#define SNDCHANNEL   *(char*)0x4015    // �T�E���h�`�����l������
#define SPWADDR      0x700             // �X�v���C�g���[�N�̊i�[�A�h���X

#define C_BLACK      0x0f  // �J���[�R�[�h ���F
#define C_RED        0x16  // �J���[�R�[�h �ԐF
#define C_BLUE       0x12  // �J���[�R�[�h �F
#define C_GREEN      0x1A  // �J���[�R�[�h �ΐF
#define C_YELLOW     0x28  // �J���[�R�[�h ���F
#define C_SKYBLUE    0x22  // �J���[�R�[�h ��F
#define C_LIMEGREEN  0x29  // �J���[�R�[�h ���ΐF
#define C_GRAY       0x00  // �J���[�R�[�h �D�F
#define C_LIGHTGRAY  0x10  // �J���[�R�[�h ���邢�D�F
#define C_WHITE      0x30  // �J���[�R�[�h ���F

#define BUTTON_A      0x80   // �{�^�� A
#define BUTTON_B      0x40   // �{�^�� B
#define BUTTON_SELECT 0x20   // �{�^�� SELECT
#define BUTTON_START  0x10   // �{�^�� START
#define BUTTON_UP     0x08   // �{�^�� ��
#define BUTTON_DOWN   0x04   // �{�^�� ��
#define BUTTON_LEFT   0x02   // �{�^�� ��
#define BUTTON_RIGHT  0x01   // �{�^�� �E

#define OUTSIDE      240     // ��ʊO�̍��W

// �S�@�\�̏�����
void fc_init(void);
// PPU�A�h���X�ݒ�(�A�h���X0x0000�`0xffff)
void ppu_address(unsigned short adr);
// BG�̏�����
void bg_cls(void);
// �X�v���C�g�̏�����
void sp_cls(void);
// �^�������̎擾
unsigned int random(void);
// �R���g���[���[1����
unsigned char controller1(void);
// �T�E���h������
void sound_init(void);
// ��`�g1�Đ�(���g��)
void pulse1(unsigned short hz);
// �J���[�p���b�g�ݒ�(�i�[��|�C���^,�I�t�Z�b�g0�`7,�p���b�g��1�`8)
void ppu_palette(char *ptable,char ofs,char size);
// �p�^�[���e�[�u���ݒ�(�i�[��|�C���^,�I�t�Z�b�g0�`511,����1�`512)
void ppu_pattern(unsigned char *ptable,short ofs,short size);
// ��ʕ\���̗L����(1=�\��ON/0=�\��OFF)
void ppu_enable(char on);
// V�u�����N��҂�
void ppu_vsync(void);
// BG�X�N���[���ݒ�(X���W�I�t�Z�b�g0�`511,Y���W�I�t�Z�b�g0�`239)
void bg_scroll(unsigned short x, unsigned char y);
// BG�L�����N�^�\��(X���W0�`31,Y���W0�`29,�^�C���ԍ�0�`255)
void bg_printch(char x,char y,unsigned char c);
// BG������\��(X���W0�`31,Y���W0�`29,������|�C���^)
void bg_printstr(char x,char y,char *str);
// BG�A�g���r���[�g�ݒ�(X���W0�`31,Y���W0�`29,�A�g���r���[�g0�`255)
void bg_attrib(char x,char y,unsigned char atr);
// 10�i��5�P�^�ϊ�(�i�[��|�C���^,���l0�`65535)
void num_to_str(char *str,unsigned short num);
// 16�i��2�P�^�ϊ�(�i�[��|�C���^,���l0�`255)
void num_to_hex(char *str,unsigned char num);
// �X�v���C�g���[�N��DMA�]�����J�n����
void sp_dmastart(void);
// �X�v���C�g4�� �ǉ�(X���W,Y���W,�^�C��0�`255,�A�g���r���[�g0�`255)
char sp_append(char x,char y,char tile,char attr);
// �X�v���C�g4�� ����(�X�v���C�g�ԍ�0�`15)
void sp_remove(char i);
// �X�v���C�g4�� �\���ύX(�X�v���C�g�ԍ�0�`15,�^�C��0�`255,�A�g���r���[�g0�`255)
void sp_chr(char num,unsigned char tile,unsigned  char attr);
// �X�v���C�g4�� �A�j������(�X�v���C�g�ԍ�0�`15)
void sp_anime(char num);
// �X�v���C�g4�� �ړ�(�X�v���C�g�ԍ�0�`15,X���W0�`255,Y���W0�`255)
void sp_ofs(char num,unsigned char x,unsigned  char y);
// �X�v���C�gX���W�擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_getx(char num);
// �X�v���C�gY���W�擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_gety(char num);
// �X�v���C�g�̃^�C���擾(�X�v���C�g�ԍ�0�`63)
unsigned char sp_gettile(char num);
// �X�v���C�g�Փ˔���(�X�v���C�g�ԍ�,X���W,Y���W)
char sp_hitcheck(char num,unsigned char x,unsigned char y);


