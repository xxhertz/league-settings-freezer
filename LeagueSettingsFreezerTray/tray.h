#pragma once
#include "resource.h"
#include <shellapi.h>

#define WM_MYMESSAGE (WM_USER + 11)

void update_tray(bool readonly, HINSTANCE instance, NOTIFYICONDATA* nid);
BOOL setup_tray(bool readonly, HINSTANCE instance, NOTIFYICONDATA* nid, WNDPROC WndProc);