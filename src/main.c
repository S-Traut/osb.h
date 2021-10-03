#include "osb.h"
#include <time.h>

int main(int argc, char const *argv[])
{
  int storyboard_size = 10;
  StoryboardElement elements[storyboard_size];
  Storyboard storyboard = sbcreate(elements, storyboard_size); 
  unsigned short pixel = getpath(storyboard, "sb/p.png");

  VEC2 pos = {320, 240};

  for(int i = 0; i < 1; i++) {
    Sprite sprite = sprc(pixel, 1);
    sevent(E_MOVE, &sprite, 0, &pos);
    sbpush(&storyboard, sprite);
  }

  sbprint(storyboard);
  sbfree(storyboard);
  return 0;
}
