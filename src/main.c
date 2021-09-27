#include "osb.h"

int main(int argc, char const *argv[])
{
	Storyboard *storyboard;

	StoryboardElement sprite;
	sfevent(E_FADE, &sprite, 0, 1);
	sbpush(storyboard, sprite);

	sbprint(storyboard);
	sbfree(storyboard);
	return 0;
}
