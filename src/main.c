#include "osb.h"

void background(Storyboard *storyboard, char *path, int stime, int etime)
{
	short background = getpath(storyboard, path);
	Sprite sprite = sprc(background);
	sevent(E_MOVE, &sprite, 0, &((VEC2){288, 32}));
	dfevent(E_FADE, &sprite, 0, stime, stime + 1000, 0, 1);
	dfevent(E_FADE, &sprite, 0, etime, etime + 1000, 1, 0);
	sbpush(storyboard, &sprite);
	sprprint(storyboard, &sprite);
}

int main(int argc, char const *argv[])
{
	Storyboard *storyboard = sbcreate();

	background(storyboard, "bg.jpg", 0, 10000);

	sbfree(storyboard);
	return 0;
}
