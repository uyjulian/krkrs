
#include "tjsCommHead.h"

#include "TVPScreen.h"
#include "Application.h"

int tTVPScreen::GetWidth() {
	return 2048;
}
int tTVPScreen::GetHeight() {
	return 2048;
}

int tTVPScreen::GetDesktopLeft() {
	return 0;
}
int tTVPScreen::GetDesktopTop() {
	return 0;
}
int tTVPScreen::GetDesktopWidth() {
	return GetWidth();
}
int tTVPScreen::GetDesktopHeight() {
	return GetHeight();
}

