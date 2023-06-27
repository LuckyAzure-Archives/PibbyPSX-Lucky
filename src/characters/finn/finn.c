#include "finn.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//Finn character structure
enum
{
	Finn_ArcMain_idle0,
	Finn_ArcMain_idle1,
	Finn_ArcMain_idle2,
	Finn_ArcMain_idle3,
	Finn_ArcMain_idle4,
	Finn_ArcMain_left0,
	Finn_ArcMain_left1,
	Finn_ArcMain_left2,
	Finn_ArcMain_left3,
	Finn_ArcMain_left4,
	Finn_ArcMain_left5,
	Finn_ArcMain_down0,
	Finn_ArcMain_down1,
	Finn_ArcMain_down2,
	Finn_ArcMain_down3,
	Finn_ArcMain_down4,
	Finn_ArcMain_down5,
	Finn_ArcMain_down6,
	Finn_ArcMain_down7,
	Finn_ArcMain_up0,
	Finn_ArcMain_up1,
	Finn_ArcMain_up2,
	Finn_ArcMain_up3,
	Finn_ArcMain_up4,
	Finn_ArcMain_up5,
	Finn_ArcMain_right0,
	Finn_ArcMain_right1,
	Finn_ArcMain_right2,
	Finn_ArcMain_right3,
	Finn_ArcMain_right4,
	Finn_ArcMain_right5,
	Finn_ArcMain_right6,

	Finn_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Finn_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Finn;

//Finn character definitions
static const u16 char_finn_icons[2][4] = {
	{86,0,34,36},
	{121,0,31,40}
};

static const CharFrame char_finn_frame[] = {
	{Finn_ArcMain_idle0,{0,0,126,181},{51,176}}, //0 Idle
	{Finn_ArcMain_idle1,{0,0,126,184},{51,179}}, //1 Idle
	{Finn_ArcMain_idle1,{128,0,123,185},{50,180}}, //2 Idle
	{Finn_ArcMain_idle2,{0,0,123,185},{49,180}}, //3 Idle
	{Finn_ArcMain_idle2,{124,0,123,185},{49,180}}, //4 Idle
	{Finn_ArcMain_idle3,{0,0,123,185},{49,180}}, //5 Idle
	{Finn_ArcMain_idle3,{124,0,123,185},{49,180}}, //6 Idle
	{Finn_ArcMain_idle4,{0,0,122,185},{49,180}}, //7 Idle
	
	{Finn_ArcMain_left0,{0,0,126,172},{75,167}}, //8 Left
	{Finn_ArcMain_left1,{0,0,129,171},{75,166}}, //9 Left
	{Finn_ArcMain_left2,{0,0,129,171},{74,166}}, //10 Left
	{Finn_ArcMain_left3,{0,0,129,171},{74,166}}, //11 Left
	{Finn_ArcMain_left4,{0,0,129,171},{74,166}}, //12 Left
	{Finn_ArcMain_left5,{0,0,129,171},{75,166}}, //13 Left
	
	{Finn_ArcMain_down0,{0,0,205,158},{50,143}}, //14 Down
	{Finn_ArcMain_down1,{0,0,180,181},{55,176}}, //15 Down
	{Finn_ArcMain_down2,{0,0,148,179},{59,174}}, //16 Down
	{Finn_ArcMain_down3,{0,0,141,179},{52,174}}, //17 Down
	{Finn_ArcMain_down4,{0,0,137,179},{49,174}}, //18 Down
	{Finn_ArcMain_down5,{0,0,138,179},{49,174}}, //19 Down
	{Finn_ArcMain_down6,{0,0,138,179},{50,174}}, //20 Down
	{Finn_ArcMain_down7,{0,0,138,179},{49,174}}, //21 Down
	
	{Finn_ArcMain_up0,{0,0,155,181},{75,176}}, //22 Up
	{Finn_ArcMain_up1,{0,0,152,178},{72,172}}, //23 Up
	{Finn_ArcMain_up2,{0,0,150,177},{70,171}}, //24 Up
	{Finn_ArcMain_up3,{0,0,150,176},{70,171}}, //25 Up
	{Finn_ArcMain_up4,{0,0,150,176},{70,171}}, //26 Up
	{Finn_ArcMain_up5,{0,0,150,176},{70,171}}, //27 Up
	
	{Finn_ArcMain_right0,{0,0,154,197},{47,158}}, //28 Right
	{Finn_ArcMain_right1,{0,0,180,163},{57,158}}, //29 Right
	{Finn_ArcMain_right2,{0,0,170,164},{55,159}}, //30 Right
	{Finn_ArcMain_right3,{0,0,161,164},{55,159}}, //31 Right
	{Finn_ArcMain_right4,{0,0,154,164},{55,159}}, //32 Right
	{Finn_ArcMain_right5,{0,0,155,164},{55,159}}, //33 Right
	{Finn_ArcMain_right6,{0,0,153,164},{55,159}}, //34 Right
	

};

static const Animation char_finn_anim[CharAnim_Max] = {
	{12, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7,  ASCR_BACK, 1}},		//CharAnim_Idle
	{18, (const u8[]){ 8, 9, 10, 11, 12, 13,  ASCR_BACK, 1}},		//CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_LeftAlt
	{18, (const u8[]){ 14, 15, 16, 17, 18, 19, 20, 21,  ASCR_BACK, 1}},		//CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_DownAlt
	{18, (const u8[]){ 22, 23, 24, 25, 26, 27,  ASCR_BACK, 1}},		//CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_UpAlt
	{18, (const u8[]){ 28, 29, 30, 31, 32, 33, 34,  ASCR_BACK, 1}},		//CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_RightAlt

};

//Finn character functions
void Char_Finn_SetFrame(void *user, u8 frame)
{
	Char_Finn *this = (Char_Finn*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_finn_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Finn_Tick(Character *character)
{
	Char_Finn *this = (Char_Finn*)character;
	
	//Perform idle dance
	if ((character->pad_held & (stage.prefs.control_keys[0] | stage.prefs.control_keys[1] | stage.prefs.control_keys[2] | stage.prefs.control_keys[3])) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Finn_SetFrame);
	Character_Draw(character, &this->tex, &char_finn_frame[this->frame]);
}

void Char_Finn_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Finn_Free(Character *character)
{
	Char_Finn *this = (Char_Finn*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Finn_New(fixed_t x, fixed_t y)
{
	//Allocate finn object
	Char_Finn *this = Mem_Alloc(sizeof(Char_Finn));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Finn_New] Failed to allocate finn object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Finn_Tick;
	this->character.set_anim = Char_Finn_SetAnim;
	this->character.free = Char_Finn_Free;
	
	Animatable_Init(&this->character.animatable, char_finn_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_finn_icons, sizeof(char_finn_icons));
	
	//health bar color
	this->character.health_bar = 0xFF89C6F8;
	
	this->character.focus_x = FIXED_DEC(50,1);
	this->character.focus_y = FIXED_DEC(-90,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\FINN.ARC;1");
	
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
		"left5.tim",
		"down0.tim",
		"down1.tim",
		"down2.tim",
		"down3.tim",
		"down4.tim",
		"down5.tim",
		"down6.tim",
		"down7.tim",
		"up0.tim",
		"up1.tim",
		"up2.tim",
		"up3.tim",
		"up4.tim",
		"up5.tim",
		"right0.tim",
		"right1.tim",
		"right2.tim",
		"right3.tim",
		"right4.tim",
		"right5.tim",
		"right6.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
