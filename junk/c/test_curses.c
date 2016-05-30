#include	<curses.h>

WINDOW * dave_window;
char mychar;
chtype verch = '-';
chtype horch = '|';

main()
{
  initscr();
  dave_window=newwin(10,15,5,5); 
  box(dave_window, 0, 0);
 wrefresh(dave_window); 
  refresh();
  mychar=getch();
/*  addch(mychar); */
  getch();
  getch();
  endwin();
}
