#include "list.h"

#ifndef osb
#define osb

typedef unsigned char Byte;

// Event types
#define E_FADE 0
#define E_SCALE 1
#define E_VSCALE 2
#define E_MOVE 3
#define E_ROTATE 4
#define E_COLOR 5
#define E_MOVEX 6
#define E_MOVEY 7

// Origins
#define O_CENTRE 0
#define O_TOPLEFT 1
#define O_TOPCENTER 2
#define O_TOPRIGHT 3
#define O_LEFTCENTRE 4
#define O_RIGHTCENTRE 5
#define O_BOTTOMLEFT 6
#define O_BOTTOMCENTRE 7
#define O_BOTTOMRIGHT 8

// Layers
#define L_BACKGROUND 0
#define L_FAIL 1
#define L_PASS 2
#define L_FOREGROUND 3
#define L_OVERLAY 4

// General helping values
#define SCREEN_CENTER \
	(VEC2) { 320, 240 }

#define v2equal(v1, v2) (v1.x == v2.x) && (v1.y == v2.y)

// Type definitions

typedef struct Storyboard
{
	List *paths;
	List *vec2;
	List *vec3;
	List *elements;
} Storyboard;

typedef struct StoryboardElement
{
	unsigned short header;
	unsigned short path_index;
	unsigned int pos_index;
	List *sparameters;
	List *dparameters;
} StoryboardElement;

typedef StoryboardElement Sprite;

typedef struct StaticEvent
{
	Byte type;
	int time;
	float value;
} StaticEvent;

typedef struct DynamicEvent
{
	Byte type;
	int stime;
	int etime;
	float svalue;
	float evalue;
} DynamicEvent;

typedef struct Vector2
{
	float x, y;
} VEC2;

typedef struct Vector3
{
	float x, y, z;
} VEC3;

Storyboard *sbcreate();
void sbfree(Storyboard *storyboard);
void sbprint(Storyboard *storyboard);
void sbpush(Storyboard *storyboard, StoryboardElement *element);

char *sborigin(short header);
char *sblayer(short header);

void sprfree(Sprite *sprite);
void sprprint(Storyboard *storyboard, Sprite *sprite);
Sprite sprcreate(unsigned short path, Byte layer, Byte origin, unsigned int position);

/**
 * @brief  Quick sprcreate function, generate Layer, Origin and Position to initial values
 * 
 * @param  *path: Path to the sprite
 * @retval Sprite
 */
Sprite sprc(unsigned short path);

/**
 * @brief  Add a static event to a Sprite
 * @note   
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: Pointer to the event value
 * @retval None
 */
void sevent(Byte type, Sprite *spr, int time, void *val);

/**
 * @brief  Add a static float value event to a Sprite
 * @note   
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: float value of the event
 * @retval None
 */
void sfevent(Byte type, Sprite *spr, int stime, float val);

void devent(Byte type, Sprite *spr, short easing, int stime, int etime, void *sval, void *eval);
void dfevent(Byte type, Sprite *spr, short easing, int stime, int etime, float sval, float eval);

unsigned int getpath(Storyboard *storyboard, char *value);
unsigned int getv2(Storyboard *storyboard, VEC2 value);
unsigned int getv3(Storyboard *storyboard, VEC3 value);
Byte getparam(Sprite *sprite);

#endif