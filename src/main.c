#include "osb.h"

int main(int argc, char const *argv[])
{
	Storyboard *storyboard = sbcreate();
	short pixel = getpath(storyboard, "sb/p.png");

	Sprite sprite = sprc(pixel);
	sfevent(E_FADE, &sprite, 0, 1.5f);
	sbpush(storyboard, &sprite);

	sprprint(storyboard, &sprite);

	sbfree(storyboard);
	return 0;
}
