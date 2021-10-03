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
#define etype(v) (v->header >> 12)
#define elayer(v) (v.header & 112) >> 4
#define ptype(v) (v.header & 3840) >> 8
#define isdynamic(event) (event->type & 128) >> 7

/**
 * @brief  Sprite parameter, Each StoryboardElement contains a list of Event structures.
 */
typedef struct Event
{
  short header;
  int stime;
  int etime;
  void *svalue;
  void *evalue;
} Event;

/**
 * @brief  Defines an element of a storyboard, which will be mostly a Sprite.
 */
typedef struct StoryboardElement
{
  int event_count;
  unsigned short header;
  unsigned short path_index;
  unsigned int pos_index;
  Event *events;
} StoryboardElement;

/**
 * @brief  Main structure of a storyboard, it handle all configurations data of a storyboard. Use the function sbcreate() to create a new instance.
 */
typedef struct Storyboard
{
  List *paths;
  List *vec2;
  List *vec3;
  StoryboardElement* elements;
  int layercounts[5];
  int size;
  int element_count;
} Storyboard;

/**
 * @brief  StoryboardElement typedef for better code clarity
 */
typedef StoryboardElement Sprite;

typedef struct Vector2
{
  float x, y;
} VEC2;

typedef struct Vector3
{
  float x, y, z;
} VEC3;

Storyboard sbcreate(StoryboardElement* elements, int size);
void sbfree(Storyboard *storyboard);
void sbprint(Storyboard storyboard);
void sbpush(Storyboard *storyboard, StoryboardElement element);

char const *sborigin(short header);
char const *sblayer(short header);

void sprfree(Sprite *sprite);
void sprprint(Storyboard storyboard, Sprite sprite);
Sprite sprcreate(unsigned short path, Byte layer, Byte origin, unsigned int position, int size);

/**
 * @brief  Quick sprcreate function, generate Layer, Origin and Position to initial values
 * 
 * @param  *path: Path to the sprite
 * @retval Sprite
 */
Sprite sprc(unsigned short path, int size);

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
void sfevent(Byte type, Sprite *spr, int time, float val);

void devent(Byte type, Sprite *spr, short easing, int stime, int etime, void *sval, void *eval);
void dfevent(Byte type, Sprite *spr, short easing, int stime, int etime, float sval, float eval);
short pheader(Byte type, Byte dynamic, short easing);

void layerprint(Storyboard storyboard, int id, const char *name, Sprite layer[], int size);
void parsevalue(Byte type, void *value, char const *buffer);

unsigned int getpath(Storyboard storyboard, const char *value);
unsigned int getv2(Storyboard storyboard, VEC2 value);
unsigned int getv3(Storyboard *storyboard, VEC3 value);

#endif
