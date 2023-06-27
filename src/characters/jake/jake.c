#include "jake.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//Jake character structure
enum
{
	Jake_ArcMain_idle0,
	Jake_ArcMain_idle1,
	Jake_ArcMain_idle2,
	Jake_ArcMain_idle3,
	Jake_ArcMain_idle4,
	Jake_ArcMain_left0,
	Jake_ArcMain_left1,
	Jake_ArcMain_left2,
	Jake_ArcMain_left3,
	Jake_ArcMain_left4,
	Jake_ArcMain_down0,
	Jake_ArcMain_down1,
	Jake_ArcMain_down2,
	Jake_ArcMain_down3,
	Jake_ArcMain_down4,
	Jake_ArcMain_up0,
	Jake_ArcMain_up1,
	Jake_ArcMain_up2,
	Jake_ArcMain_up3,
	Jake_ArcMain_up4,
	Jake_ArcMain_up5,
	Jake_ArcMain_up6,
	Jake_ArcMain_right0,
	Jake_ArcMain_right1,
	Jake_ArcMain_right2,
	Jake_ArcMain_right3,

	Jake_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Jake_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Jake;

//Jake character definitions
static const u16 char_jake_icons[2][4] = {
	{0,0,41,25},
	{42,0,41,27}
};

static const CharFrame char_jake_frame[] = {
	{Jake_ArcMain_idle0,{0,0,173,109},{87,97}}, //0 Idle
	{Jake_ArcMain_idle0,{0,110,174,110},{87,98}}, //1 Idle
	{Jake_ArcMain_idle1,{0,0,178,112},{88,100}}, //2 Idle
	{Jake_ArcMain_idle1,{0,113,181,113},{91,101}}, //3 Idle
	{Jake_ArcMain_idle2,{0,0,179,116},{91,104}}, //4 Idle
	{Jake_ArcMain_idle3,{0,0,173,113},{85,101}}, //5 Idle
	{Jake_ArcMain_idle4,{0,0,173,113},{85,101}}, //6 Idle
	{Jake_ArcMain_idle4,{0,114,172,113},{85,101}}, //7 Idle
	
	{Jake_ArcMain_left0,{0,0,184,122},{121,110}}, //8 Left
	{Jake_ArcMain_left0,{0,123,214,121},{146,110}}, //9 Left
	{Jake_ArcMain_left1,{0,0,196,121},{146,110}}, //10 Left
	{Jake_ArcMain_left1,{0,123,196,121},{146,110}}, //11 Left
	{Jake_ArcMain_left2,{0,0,195,121},{145,110}}, //12 Left
	{Jake_ArcMain_left2,{0,123,195,121},{145,110}}, //13 Left
	{Jake_ArcMain_left3,{0,0,195,121},{145,110}}, //14 Left
	{Jake_ArcMain_left3,{0,123,195,121},{145,110}}, //15 Left
	{Jake_ArcMain_left4,{0,0,195,121},{146,110}}, //16 Left
	{Jake_ArcMain_left4,{0,123,195,121},{145,110}}, //17 Left
	
	{Jake_ArcMain_down0,{0,0,179,101},{88,90}}, //18 Down
	{Jake_ArcMain_down0,{0,103,209,101},{107,90}}, //19 Down
	{Jake_ArcMain_down1,{0,0,167,107},{100,91}}, //20 Down
	{Jake_ArcMain_down1,{0,109,168,101},{101,89}}, //21 Down
	{Jake_ArcMain_down2,{0,0,168,101},{102,89}}, //22 Down
	{Jake_ArcMain_down2,{0,103,168,100},{102,88}}, //23 Down
	{Jake_ArcMain_down3,{0,0,167,103},{102,90}}, //24 Down
	{Jake_ArcMain_down3,{0,104,169,101},{102,89}}, //25 Down
	{Jake_ArcMain_down4,{0,0,169,102},{102,90}}, //26 Down
	
	{Jake_ArcMain_up0,{0,0,179,186},{88,169}}, //27 Up
	{Jake_ArcMain_up1,{0,0,180,183},{88,166}}, //28 Up
	{Jake_ArcMain_up2,{0,0,180,184},{88,167}}, //29 Up
	{Jake_ArcMain_up3,{0,0,179,184},{88,167}}, //30 Up
	{Jake_ArcMain_up4,{0,0,179,184},{88,167}}, //31 Up
	{Jake_ArcMain_up5,{0,0,179,184},{88,167}}, //32 Up
	{Jake_ArcMain_up6,{0,0,179,184},{88,167}}, //33 Up
	
	{Jake_ArcMain_right0,{0,0,206,122},{66,110}}, //34 Right
	{Jake_ArcMain_right0,{0,124,202,121},{67,109}}, //35 Right
	{Jake_ArcMain_right1,{0,0,202,121},{67,109}}, //36 Right
	{Jake_ArcMain_right1,{0,123,202,121},{67,109}}, //37 Right
	{Jake_ArcMain_right2,{0,0,202,121},{67,109}}, //38 Right
	{Jake_ArcMain_right2,{0,123,201,121},{67,109}}, //39 Right
	{Jake_ArcMain_right3,{0,0,202,121},{67,109}}, //40 Right
	{Jake_ArcMain_right3,{0,123,202,121},{67,109}}, //41 Right
	

};

static const Animation char_jake_anim[CharAnim_Max] = {
	{12, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7,  ASCR_BACK, 1}},		//CharAnim_Idle
	{18, (const u8[]){ 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,  ASCR_BACK, 1}},		//CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_LeftAlt
	{18, (const u8[]){ 18, 19, 20, 21, 22, 23, 24, 25, 26,  ASCR_BACK, 1}},		//CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_DownAlt
	{18, (const u8[]){ 27, 28, 29, 30, 31, 32, 33,  ASCR_BACK, 1}},		//CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_UpAlt
	{18, (const u8[]){ 34, 35, 36, 37, 38, 39, 40, 41,  ASCR_BACK, 1}},		//CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_RightAlt

};

//Jake character functions
void Char_Jake_SetFrame(void *user, u8 frame)
{
	Char_Jake *this = (Char_Jake*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_jake_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Jake_Tick(Character *character)
{
	Char_Jake *this = (Char_Jake*)character;
	
	//Perform idle dance
	if ((character->pad_held & (stage.prefs.control_keys[0] | stage.prefs.control_keys[1] | stage.prefs.control_keys[2] | stage.prefs.control_keys[3])) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Jake_SetFrame);
	Character_Draw(character, &this->tex, &char_jake_frame[this->frame]);
}

void Char_Jake_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Jake_Free(Character *character)
{
	Char_Jake *this = (Char_Jake*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Jake_New(fixed_t x, fixed_t y)
{
	//Allocate jake object
	Char_Jake *this = Mem_Alloc(sizeof(Char_Jake));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Jake_New] Failed to allocate jake object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Jake_Tick;
	this->character.set_anim = Char_Jake_SetAnim;
	this->character.free = Char_Jake_Free;
	
	Animatable_Init(&this->character.animatable, char_jake_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_jake_icons, sizeof(char_jake_icons));
	
	//health bar color
	this->character.health_bar = 0xFF808080;
	
	this->character.focus_x = FIXED_DEC(30,1);
	this->character.focus_y = FIXED_DEC(-90,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\JAKE.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
		"idle1.tim",
		"idle2.tim",
		"idle3.tim",
		"idle4.tim",
		"left0.tim",
		"left1.tim",
		"left2.tim",
		"left3.tim",
		"left4.tim",
		"down0.tim",
		"down1.tim",
		"down2.tim",
		"down3.tim",
		"down4.tim",
		"up0.tim",
		"up1.tim",
		"up2.tim",
		"up3.tim",
		"up4.tim",
		"up5.tim",
		"up6.tim",
		"right0.tim",
		"right1.tim",
		"right2.tim",
		"right3.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
