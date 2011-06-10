#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <getopt.h>
#include <pthread.h>
#include "xbmcclient.h"

#define VERSION "1.0"

static CXBMCClient xbmc("localhost");
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

	xbmc.SendHELO("Terminal Keyboard", ICON_NONE);
	ret1 = pthread_create( &ping_thread, NULL, xbmcPing, NULL);
	while (1) {
		xbmc.SendButton("right","R1",BTN_NO_REPEAT);
		sleep(5);
		xbmc.SendButton(195,"KB",BTN_NO_REPEAT);
		sleep(5);
		xbmc.SendButton(37,"KB",BTN_NO_REPEAT);
		sleep(5);
	}
	xbmc.SendNOTIFICATION("Closing", "Terminal Keyboard", ICON_NONE);
	return 0;
}