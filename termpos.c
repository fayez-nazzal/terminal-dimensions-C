#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>


/* Returns X position, Y position, Width and Height of the terminal. Can be modified to return additional information */
/* X and Y position is the upper-left of the terminal without any graphics (without menu bar, border and title bar), to get the size of the full window simply remove some lines as explained below */

int get_terminal_dimensions(int *x, int *y, unsigned int *width, unsigned int *height)
{
  Display *dpy;
  Window termwin, parentwin, rootwin;
  int px, py;
  unsigned int bwidth, depth, pwidth, pheight, pbwidth, pdepth;
  Window *children;
  unsigned int nchildren;

  /* Get the current active window (which may be the terminal when the program runs) */
  /* There are some variables that are not used but should be included for the functions */

  int size, act_format;
  Atom a_req, a_type;
  unsigned long nitems, bytesafter;
  unsigned char *prob;
  
  dpy = XOpenDisplay(NULL);

  rootwin = XDefaultRootWindow(dpy);
  a_req = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
  
  XGetWindowProperty(dpy, rootwin, a_req, 0, (~0L), False, AnyPropertyType, &a_type, &act_format, &nitems, &bytesafter, &prob);
					   
  termwin = *((Window*) prob);

  /* Get parent of the terminal window */
  
  if (!XQueryTree(dpy, termwin, &rootwin, &parentwin, &children, &nchildren))
  {
    fprintf(stderr, "Error: No parent\n");
    return -1;
  }
  
  if(!XGetGeometry(dpy, parentwin, &rootwin, &px, &py, &pwidth, &pheight, &pbwidth, &pdepth))
    {
      fprintf(stderr, "Error: unable to find parent window geometry\n");
      return -1;
    }
  
  if (!XGetGeometry(dpy, termwin, &rootwin, x, y, width, height, &bwidth, &depth))
    {
      fprintf(stderr, "Error: unable to find terminal window geometry\n");
      return -1;
    }

   /* if you want to get the position with decorations (menu bar, borders and title bar) remove these two lines. */
  *x+=px;
  *y+=py;
  
  return 1;
}

/* for test */
int main() {
  int x,y;
  unsigned int width, height;
  if (!get_terminal_dimensions(&x, &y, &width, &height))
    printf("An error occured finding terminal dimensions\n");
  else
    printf("x position : %d \ny position : %d \nwidth : %d \nheight : %d\n", x, y, width, height);
  return 0;
}
