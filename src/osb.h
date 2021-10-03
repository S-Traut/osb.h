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
#define etype(v) (v.header >> 12)
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

/**
 * @brief  Representation of 2D vectors and points. His structure is used in some places to represent 2D positions and vectors
 */
typedef struct Vector2
{
  float x, y;
} VEC2;

/**
 * @brief  Representation of 3D vectors and points. His structure is used in some places to represent 3D positions and vectors
 */
typedef struct Vector3
{
  float x, y, z;
} VEC3;

/**
 * @brief  Create a storyboard structure and setup its data properly.
 * @note You can over allocate memory for the size and elements at a max size of 200 000, each StoryboardElement has a size of 24 bytes. 
 * Which means that for a Storyboard of 1000 Sprites the RAM allocation will be equal to 24000 Bytes (24kb).
 * 
 * @param  elements: The elements array where the StoryboardElement(s) should be stored. 
 * @param  size: The number of elements to allocate for the storyboard (It can be over allocated).
 * 
 * @retval Storyboard - Newly created storyboard structure
 */
Storyboard sbcreate(StoryboardElement* elements, int size);

/**
 * @brief  Free all the memory used for a given Storyboard structure.
 * @param  storyboard: Storyboard variable 
 * @retval None
 */
void sbfree(Storyboard storyboard);

/**
 * @brief  Format the data and print it out in the terminal.
 * @param  storyboard: Storyboard variable 
 * @retval None
 */
void sbprint(Storyboard storyboard);

/**
 * @brief  Pushes a new StoryboardElement into the Storyboard stack.
 * @note   Always be aware to allocate enough memory for you storyboard to store data. 
 * (In the case there is not enough space this function will throw an error and don't execute).
 * 
 * @param  *storyboard: Storyboard pointer
 * @param  element: StoryboardElement to push into the storyboard
 * 
 * @retval None
 */
void sbpush(Storyboard *storyboard, StoryboardElement element);

/**
 * @brief  Get the origin ID from a header 2byte structure
 * @param  header: 2bytes values of the Sprite header element (sprite.header). 
 * @retval Char const*
 */
char const *sborigin(short header);

/**
 * @brief  Get the layer ID from a header 2byte structure
 * @param  header: 2bytes values of the Sprite header element (sprite.header). 
 * @retval Char const*
 */
char const *sblayer(short header);

/**
 * @brief  Free the memory of the Sprite Event(s) pointer, and every start_value and end_value of an Event.
 * @param  sprite: Sprite variable to free
 * @retval None
 */
void sprfree(Sprite sprite);

/**
 * @brief  Format and output in the console a Sprite structure.
 * @param  storyboard: Storyboard reference
 * @param  sprite: Sprite that need to be formated and printed
 * @retval None
 */
void sprprint(Storyboard storyboard, Sprite sprite);

/**
 * @brief  Create a sprite structure and setup its data
 * @note   As same as Storyboard, always be aware to allocate enough memory for your Event(s) sturcture.
 * @param  path: ID of the path (Can be retreived using the getpath() function) 
 * @param  layer: Layer ID
 * @param  origin: Origin ID
 * @param  position: ID of the VEC2 (Can be retreived using the getv2() function)
 * @param  size: Number of Events the sprite will have
 * @retval Sprite - Created sprite structure
 */
Sprite sprcreate(unsigned short path, Byte layer, Byte origin, unsigned int position, int size);

/**
 * @brief  Quick sprcreate function, generate Layer, Origin and Position to initial values
 * 
 * @param  *path: Path to the sprite
 * @retval Sprite
 */
Sprite sprc(unsigned short path, int size);

/**
 * @brief  Add a static event to a Sprite.
 * @note   
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: Pointer to the event value
 * @retval None
 */
void sevent(Byte type, Sprite *spr, int time, void *val);

/**
 * @brief  Add a static float value event to a Sprite.
 * 
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: float value of the event
 * 
 * @retval None
 */
void sfevent(Byte type, Sprite *spr, int time, float val);

/**
 * @brief  Add a dynamic void* value event to a Sprite.
 * 
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: float value of the event
 * 
 * @retval None
 */
void devent(Byte type, Sprite *spr, short easing, int stime, int etime, void *sval, void *eval);

/**
 * @brief  Add a dynamic float value event to a Sprite.
 * 
 * @param  type: Type of the event (E_FADE, E_MOVE, ...)
 * @param  *spr: Sprite to modify
 * @param  time: Time of the event
 * @param  *val: float value of the event
 * 
 * @retval None
 */
void dfevent(Byte type, Sprite *spr, short easing, int stime, int etime, float sval, float eval);

/**
 * @brief  Create header 2byte data.
 * 
 * @param  type: Type of the event
 * @param  dynamic: If the parameter is dynamic or not
 * @param  easing: Easing ID
 * 
 * @retval short
 */
short pheader(Byte type, Byte dynamic, short easing);

/**
 * @brief  Print a specified layer of a storyboard. (Avoid calling this function directly and use sbprint() instead).
 * 
 * @param  storyboard: Storyboard structure
 * @param  id: int value of the current Layer
 * @param  name: Name of the layer
 * @param layer: Layer array which contains all sprites to print
 * @param size: 
 * 
 * @retval None
 */
void layerprint(Storyboard storyboard, int id, const char *name, Sprite layer[], int size);

/**
 * @brief  Fills up a buffer pointer from structured data types (VEC2/VEC3...) that can be used to generate parameters.
 * 
 * @param  type: Type of the Event, in byte format (can be retreived using btype() macro)
 * @param  *value: Value to parse
 * @param  *buffer: Buffer string to concatenate
 * 
 * @retval None
 */
void parsevalue(Byte type, void *value, char const *buffer);

/**
 * @brief  Get path from the paths List of a Storyboard.
 * 
 * @note   If the path doesn't exist, it'll automatically register a new entry in the list. 
 * @param  storyboard: Search source
 * @param  *value: Value to search / create
 * 
 * @retval unsigned int
 */
unsigned int getpath(Storyboard storyboard, const char *value);

/**
 * @brief  Get a VEC2 from the vec2 List of a Storyboard.
 * 
 * @note   If the VEC2 doesn't exist, it'll automatically register a new entry in the list. 
 * @param  storyboard: Search source
 * @param  *value: Value to search / create
 * 
 * @retval unsigned int
 */
unsigned int getv2(Storyboard storyboard, VEC2 value);

/**
 * @brief  Get a VEC3 from the vec3 List of a Storyboard.
 * 
 * @note   If the VEC3 doesn't exist, it'll automatically register a new entry in the list. 
 * @param  storyboard: Search source
 * @param  *value: Value to search / create
 * 
 * @retval unsigned int
 */
unsigned int getv3(Storyboard *storyboard, VEC3 value);

#endif