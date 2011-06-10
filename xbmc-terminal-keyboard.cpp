#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <getopt.h>
#include <pthread.h>
#include "xbmcclient.h"

#define VERSION "1.0"

static CXBMCClient xbmc("htpc.local");
static bool disconnected = false;
static bool quit = false;

void* xbmcPing(void* data) {
	static int i = 0;
	while (1) {
		xbmc.SendLOG(LOGDEBUG,"Terminal Keyboard Ping",false);
		xbmc.SendPing();
		sleep(10);
	}
	pthread_exit(NULL);
}

int main (int argc, char **argv) {
	pthread_t ping_thread;
	int32_t ch;
	int ret1;
	char c;
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

	xbmc.SendHELO("Terminal Keyboard", ICON_NONE);
    mvprintw(0,0,"Type any character to see it in bold\n");
	sleep(1);
	ret1 = pthread_create( &ping_thread, NULL, xbmcPing, NULL);
	while (ch = getch()) {
		if (ch==3) break; // ^C
		mvprintw(1,0,"%3d     0x%2x\n",ch,ch);
		switch (ch) {
			case 1:
				xbmc.SendACTION("xbmc.ActivateWindow(virtualkeyboard)");
				break;
			case 181:
				xbmc.SendACTION("XBMC.ActivateWindow(VideoLibrary,MovieTitles)");
				break;
			case 160:
				xbmc.SendACTION("xbmc.ActivateWindow(VideoLibrary,TvShowTitles)");
				break;
			case 21:
				xbmc.SendACTION("xbmc.ActivateWindow(MusicLibrary,Artists)");
				break;
			case KEY_LEFT:
				xbmc.SendButton("left","R1",BTN_NO_REPEAT);
				break;
			case KEY_RIGHT:
				xbmc.SendButton("right","R1",BTN_NO_REPEAT);
				break;
			case KEY_UP:
				xbmc.SendButton("up","R1",BTN_NO_REPEAT);
				break;
			case KEY_DOWN:
				xbmc.SendButton("down","R1",BTN_NO_REPEAT);
				break;
			case 27:
				xbmc.SendButton("escape","KB",BTN_NO_REPEAT);
				break;
			case 10:
				xbmc.SendButton("enter","KB",BTN_NO_REPEAT);
				break;
			case 32:
				xbmc.SendButton("space","KB",BTN_NO_REPEAT);
				break;
			case 127:
				xbmc.SendButton(216,"KB",BTN_NO_REPEAT);
				break;
			case 48: // Number Keys
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
				xbmc.SendButton(ch + 0xF030,"KB",BTN_NO_REPEAT);
				break;
			case 65:
				xbmc.SendButton("A","KB",BTN_NO_REPEAT);
				break;
			case 66:
				xbmc.SendButton("B","KB",BTN_NO_REPEAT);
				break;
			case 67:
				xbmc.SendButton("C","KB",BTN_NO_REPEAT);
				break;
			case 97:
				xbmc.SendButton("a","KB",BTN_NO_REPEAT);
				break;
			case 98:
				xbmc.SendButton("b","KB",BTN_NO_REPEAT);
				break;
			case 99:
				xbmc.SendButton("c","KB",BTN_NO_REPEAT);
				break;
			case 112:
				xbmc.SendButton("p","KB",BTN_VKEY);
				break;
			default:
				if (0x41<=ch && ch<=0x5a) {
					ch+=0x2F000;
					mvprintw(2,0,"%3d     0x%2x\n",ch,ch);
					xbmc.SendButton(ch,"KB",BTN_NO_REPEAT);
				} else if (97<=ch && ch<=109) {
					mvprintw(2,0,"BTN_NO_REPEAT\n",ch,ch);
					xbmc.SendButton((char)ch,"KB",BTN_NO_REPEAT);
				} else if (109<ch && ch<=122) {
					mvprintw(2,0,"BTN_VKEY -> BTN_UP\n",ch,ch);
					xbmc.SendButton((char)ch,"KB",BTN_VKEY);
					xbmc.SendButton(0x01,"KB",BTN_UP);
				} else {
					xbmc.SendButton(ch,"KB",BTN_NO_REPEAT);
				}
		}
		refresh();
	}
	endwin();
	xbmc.SendNOTIFICATION("Closing", "Terminal Keyboard", ICON_NONE);
	return 0;
}
