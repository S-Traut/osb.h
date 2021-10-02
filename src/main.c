#include "osb.h"

void background(Storyboard *storyboard, const char *path, int stime, int etime)
{
	short background = getpath(storyboard, path);
	Sprite sprite = sprc(background);
	dfevent(E_FADE, &sprite, 0, stime, stime + 1000, 0, 1);
	dfevent(E_FADE, &sprite, 0, etime, etime + 1000, 1, 0);
	sbpush(storyboard, &sprite);
}

int main(int argc, char const *argv[])
{
	Storyboard *storyboard = sbcreate();

	background(storyboard, "bg.jpg", 0, 10000);

	sbprint(storyboard);
	sbfree(storyboard);
	return 0;
}
