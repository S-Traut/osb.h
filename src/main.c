#include "osb.h"
#include <time.h>

void ptime(const char* text, clock_t start) {
  clock_t c = clock();
  printf("%s - %f\n", text, (double)(c-start) / CLOCKS_PER_SEC);
  
}

void background(clock_t s, Storyboard *storyboard, const char *path, int stime, int etime)
{
  short background = getpath(storyboard, path);

  Sprite sprite = sprc(background);

  VEC2 spos = {30, 240};
  VEC2 epos = {40, 240};
  
  devent(E_MOVE, &sprite, 0, stime, etime, &spos, &epos);
  dfevent(E_FADE, &sprite, 0, stime, stime + 1000, 0, 1);
  dfevent(E_FADE, &sprite, 0, etime, etime + 1000, 1, 0);

  dfevent(E_FADE, &sprite, 0, 0, 1000, 0, 1);
  dfevent(E_FADE, &sprite, 0, 0, 1000, 0, 1);
  dfevent(E_FADE, &sprite, 0, 0, 1000, 0, 1);
  dfevent(E_FADE, &sprite, 0, 0, 1000, 0, 1);

  sbpush(storyboard, &sprite);
}

int main(int argc, char const *argv[])
{
  clock_t begin = clock();
  Storyboard *storyboard = sbcreate();
  ptime("CREATED SB", begin);


  for(int i = 0; i < 1000000; i++) {
    background(begin, storyboard, "bg.jpg", 5000, 10000);
  }

  //sbprint(storyboard);
  sbfree(storyboard);
  ptime("FREE", begin);
  return 0;
}
