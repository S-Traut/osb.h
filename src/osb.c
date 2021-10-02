#include "osb.h"

Storyboard *sbcreate()
{
  Storyboard storyboard;
  Storyboard *pointer = (Storyboard *)malloc(sizeof(Storyboard));
  storyboard.elements = lscreate(sizeof(StoryboardElement));
  storyboard.paths = lscreate(LSMODE_DYNAMIC);
  storyboard.vec2 = lscreate(sizeof(VEC2));
  storyboard.vec3 = lscreate(sizeof(VEC3));
  storyboard.layercounts[0] = 0;
  storyboard.layercounts[1] = 0;
  storyboard.layercounts[2] = 0;
  storyboard.layercounts[3] = 0;
  storyboard.layercounts[4] = 0;
  memcpy(pointer, &storyboard, sizeof(Storyboard));
  getv2(pointer, SCREEN_CENTER);

  return pointer;
}

void sbpush(Storyboard *storyboard, StoryboardElement *element)
{
  Byte layer = elayer(element);
  storyboard->layercounts[layer]++;
  lspush(storyboard->elements, element);
}

void sbfree(Storyboard *storyboard)
{
  // Free sprite parameters
  Node *current = storyboard->elements->first_element;
  while (current)
  {
    StoryboardElement *sprite = (StoryboardElement*)current->value;
    if (etype(sprite) == 0)
    {
      sprfree(sprite);
    }

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
  // Build the layers
  printf("[Events]\n");
  printf("//Background and Video events\n");

  // Generating layer sizes
  Sprite *background[storyboard->layercounts[0]];
  Sprite *fail[storyboard->layercounts[1]];
  Sprite *pass[storyboard->layercounts[2]];
  Sprite *foreground[storyboard->layercounts[3]];
  Sprite *overlay[storyboard->layercounts[4]];

  // Filling layers
  Node *current = storyboard->elements->first_element;
  int b = 0, f = 0, x = 0, o = 0, t = 0;
  while (current)
  {
    Sprite *sprite = (Sprite *)current->value;
    Byte layer = elayer(sprite);
    switch (layer)
    {
      case L_BACKGROUND: {
			  background[b] = sprite;
			  b++;
			  break;
      }

		  case L_FAIL: {
        fail[x] = sprite;
        x++;
        break;
      }

      case L_PASS: {
        pass[o] = sprite;
        o++;
        break;
      }

		  case L_FOREGROUND: {
        foreground[f] = sprite;
        f++;
        break;
      }

      case L_OVERLAY: {
        overlay[t] = sprite;
        t++;
        break;
      }

      default: {
        background[b] = sprite;
        b++;
        break;
      }
    }
    current = current->next;
  }

  // Printing layers
  layerprint(storyboard, 0, "Background", background, storyboard->layercounts[0]);
  layerprint(storyboard, 1, "Fail", fail, storyboard->layercounts[1]);
  layerprint(storyboard, 2, "Pass", pass, storyboard->layercounts[2]);
  layerprint(storyboard, 3, "Foreground", foreground, storyboard->layercounts[3]);
  layerprint(storyboard, 4, "Overlay", overlay, storyboard->layercounts[4]);
}

void layerprint(Storyboard *storyboard, int id, const char *name, Sprite *layer[], int size)
{
  printf("Storyboard Layer %i (%s)\n", id, name);
  for (int i = 0; i < size; i++)
  {
    sprprint(storyboard, layer[i]);
  }
}

Sprite sprcreate(unsigned short path, Byte layer, Byte origin, unsigned int position)
{
  Sprite sprite;
  sprite.events = lscreate(sizeof(Event));
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
  Node *current = sprite->events->first_element;
  while (current)
  {
    Event *event = (Event *)current->value;
    free(event->svalue);
    free(event->evalue);
    current = current->next;
  }
  lsfree(sprite->events);
}

void sprprint(Storyboard *storyboard, Sprite *sprite)
{
  char *path = (char *)lsget(storyboard->paths, sprite->path_index)->value;
  char const *layer = sblayer(sprite->header);
  char const *origin = sborigin(sprite->header);

  VEC2 *position = (VEC2 *)lsget(storyboard->vec2, sprite->pos_index)->value;

  printf("Sprite,%s,%s,\"%s\",%.7g,%.7g\n", layer, origin, path, position->x, position->y);

  char const *types[] = {"F", "S", "V", "M", "R", "C", "MX", "MY"};
  Node *current = sprite->events->first_element;
  while (current)
  {
    Event *event = (Event *)current->value;
    Byte btype = ptype(event);
    Byte easing = event->header & 127;
    const char* type = types[btype];
    if (event->etime != -1)
    {
      char sbuffer[100];
      char ebuffer[100];
      parsevalue(btype, event->svalue, sbuffer);
      parsevalue(btype, event->evalue, ebuffer);
      printf(" %s,%i,%i,%i,%s,%s\n", type, easing, event->stime, event->etime, sbuffer, ebuffer);
    }
    else
    {
      char buffer[100];
      parsevalue(btype, event->svalue, buffer);
      printf(" %s,0,%i,,%s\n", type, event->stime, buffer);
    }
    current = current->next;
  }
}

void createevent(short header, Sprite *spr, int stime, int etime, void *sval, void *eval)
{
  Event event;
  event.header = header;
  event.stime = stime;
  event.etime = etime;

  switch ((header & 3840) >> 8)
  {
    case E_MOVE: {
      void *msptr = malloc(sizeof(VEC2));
      memcpy(msptr, sval, sizeof(VEC2));
      event.svalue = msptr;

      if (eval)
      {
        void *meptr = malloc(sizeof(VEC2));
        memcpy(meptr, eval, sizeof(VEC2));
        event.evalue = meptr;
      }
      else
        event.evalue = NULL;

        lspush(spr->events, &event);
      break;
    }

    default: {
      void *sptr = malloc(sizeof(float));
      memcpy(sptr, sval, sizeof(float));
      event.svalue = sptr;

      if (eval)
      {
        void *eptr = malloc(sizeof(float));
        memcpy(eptr, eval, sizeof(float));
        event.evalue = eptr;
      }
      else
        event.evalue = NULL;

      lspush(spr->events, &event);
      break;
    }
  }
}

void sevent(Byte type, Sprite *spr, int time, void *val)
{
  short header = pheader(type, 0, 0);
  createevent(header, spr, time, -1, val, NULL);
}

void sfevent(Byte type, StoryboardElement *spr, int time, float val)
{
  short header = pheader(type, 0, 0);
  createevent(header, spr, time, -1, &val, NULL);
}

void devent(Byte type, Sprite *spr, short easing, int stime, int etime, void *sval, void *eval)
{
  short header = pheader(type, 1, easing);
  createevent(header, spr, stime, etime, sval, eval);
}

void dfevent(Byte type, Sprite *spr, short easing, int stime, int etime, float sval, float eval)
{
  short header = pheader(type, 1, easing);
  createevent(header, spr, stime, etime, &sval, &eval);
}

void parsevalue(Byte type, void *value, char const *buffer)
{
  switch (type) {
    case E_MOVE: {
      VEC2 *v2 = (VEC2 *)value;
      sprintf((char *)buffer, "%.7g,%.7g", v2->x, v2->y);
      return;
      break;
    }

    default: {
      float *v = (float *)value;
      sprintf((char *)buffer, "%.4g", *v);
      return;
      break;
    }
  }
  buffer = "";
}

short pheader(Byte type, Byte dynamic, short easing)
{
  short header = 0;
  header |= 1 << 12;
  header |= type << 8;
  header |= dynamic << 7;
  header |= easing;
  return header;
}

char const *sborigin(short header)
{
  char const *origins[] = {
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

char const *sblayer(short header)
{
  char const *layers[] = {
    "Background",
    "Fail",
    "Pass",
    "Foreground",
    "Overlay",
  };
  Byte layer = (header & 112) >> 4;
  return layers[layer];
}

unsigned int getpath(Storyboard *storyboard, const char *value)
{
  Node *current = storyboard->paths->first_element;
  size_t i = 0;
  while (current)
  {
    char *check_value = (char *)current->value;
    if (strcmp(value, check_value) == 0)
      return i;

    current = current->next;
    i++;
  }

  lsdpush(storyboard->paths, (char *)value, strlen(value) + 1);
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
