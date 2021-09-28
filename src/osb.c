#include "osb.h"

Storyboard *sbcreate()
{
	Storyboard storyboard;
	Storyboard *pointer = malloc(sizeof(Storyboard));
	storyboard.elements = lscreate(sizeof(StoryboardElement));
	storyboard.paths = lscreate(LSMODE_DYNAMIC);
	storyboard.vec2 = lscreate(sizeof(VEC2));
	storyboard.vec3 = lscreate(sizeof(VEC3));
	memcpy(pointer, &storyboard, sizeof(Storyboard));
	getv2(pointer, SCREEN_CENTER);
	return pointer;
}

void sbpush(Storyboard *storyboard, StoryboardElement *element)
{
	lspush(storyboard->elements, element);
}

void sbfree(Storyboard *storyboard)
{
	// Free sprite parameters
	Node *current = storyboard->elements->first_element;
	while (current)
	{
		Sprite *sprite = current->value;
		lsfree(sprite->sparameters);
		lsfree(sprite->dparameters);
		current = current->next;
	}

	lsfree(storyboard->elements);
	lsfree(storyboard->paths);
	lsfree(storyboard->vec2);
	lsfree(storyboard->vec3);
	free(storyboard);
}

void sbprint(Storyboard *storyboard)
{
}

Sprite sprcreate(unsigned short path, Byte layer, Byte origin, unsigned int position)
{
	Sprite sprite;
	sprite.sparameters = lscreate(sizeof(StaticEvent));
	sprite.dparameters = lscreate(sizeof(DynamicEvent));
	sprite.path_index = path;
	sprite.pos_index = position;

	short header = 0;
	Byte base_position = position == 0;
	header |= origin << 8;
	header |= base_position << 7;
	header |= layer << 4;
	sprite.header = header;

	return sprite;
}

Sprite sprc(unsigned short path)
{
	return sprcreate(path, L_BACKGROUND, O_CENTRE, 0);
}

void sprfree(Sprite *sprite)
{
	lsfree(sprite->sparameters);
	lsfree(sprite->dparameters);
	free(sprite);
}

void sprprint(Storyboard *storyboard, Sprite *sprite)
{
	char *path = lsget(storyboard->paths, sprite->pos_index)->value;
	char *layer = sblayer(sprite->header);
	char *origin = sborigin(sprite->header);
	VEC2 *position = lsget(storyboard->vec2, sprite->pos_index)->value;
	printf("Sprite,%s,%s,\"%s\",%.8g,%.8g\n", layer, origin, path, position->x, position->y);

	Node *sparameter = sprite->sparameters->first_element;
	char *types[] = {"F", "S", "V", "M", "R", "C", "MX", "MY"};
	while (sparameter)
	{
		StaticEvent static_event = *((StaticEvent *)sparameter->value);

		char *type = types[static_event.type];
		printf(" %s,0,%i,,%.3g\n", type, static_event.time, static_event.value);
		sparameter = sparameter->next;
	}
}

void sfevent(Byte type, StoryboardElement *spr, int stime, float val)
{
	StaticEvent event;
	event.type = type;
	event.time = stime;
	event.value = val;

	lspush(spr->sparameters, &event);
}

char *sborigin(short header)
{
	char *origins[] = {
		"Centre",
		"TopLeft",
		"TopCenter",
		"TopRight",
		"LeftCentre",
		"RightCentre",
		"BottomLeft",
		"BottomCentre",
		"BottomRight",
	};
	Byte origin = (header & 3840) >> 8;
	return origins[origin];
}

char *sblayer(short header)
{
	char *layers[] = {
		"Background",
		"Fail",
		"Pass",
		"Foreground",
		"Overlay",
	};
	Byte layer = (header & 112) >> 4;
	return layers[layer];
}

unsigned int getpath(Storyboard *storyboard, char *value)
{
	Node *current = storyboard->paths->first_element;
	size_t i = 0;
	while (current)
	{
		char *check_value = current->value;
		if (strcmp(value, check_value) == 0)
			return i;

		current = current->next;
		i++;
	}

	lsdpush(storyboard->paths, value, strlen(value));
	return i;
}

unsigned int getv2(Storyboard *storyboard, VEC2 value)
{
	Node *current = storyboard->paths->first_element;
	size_t i = 0;
	while (current)
	{
		VEC2 check_value = *((VEC2 *)current->value);

		if (v2equal(value, check_value))
			return i;

		current = current->next;
		i++;
	}

	lspush(storyboard->vec2, &value);
	return i;
}

unsigned int getv3(Storyboard *storyboard, VEC3 value)
{
	return 0;
}
