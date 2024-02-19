#include "external/3ds.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
extern CoreData CORE;                   // Global CORE state context

static int GetGamepadButton(int button);

// Check if application should close
bool WindowShouldClose(void)
{
    if(CORE.Window.ready) return !aptMainLoop();
    else return true;
}

// Toggle fullscreen mode
void ToggleFullscreen(void)
{
    return; //The app is always in fullscreen
}

// Toggle borderless windowed mode
void ToggleBorderlessWindowed(void)
{
    return; //unsupporded
}

// Set window state: maximized, if resizable
void MaximizeWindow(void)
{
    return; //unsupported
}

// Set window state: minimized
void MinimizeWindow(void)
{
    return; //unsupported
}

// Set window state: not minimized/maximized
void RestoreWindow(void)
{
    return; //unsupported
}

// Set window configuration state using flags
void SetWindowState(unsigned int flags)
{
    return; //unsupported
}

// Clear window configuration state flags
void ClearWindowState(unsigned int flags)
{
    return; //unsupporded
}

// Set icon for window
void SetWindowIcon(Image image)
{
    (void) image;   //unsupported
}

// Set icon for window
void SetWindowIcons(Image *images, int count)
{
    (void) images; (void) count;    //unsupported
}


// Set title for window
void SetWindowTitle(const char *title)
{
    CORE.Window.title = title;
}

void SetWindowPosition(int x, int y)
{
    CORE.Window.position.x = x;
    CORE.Window.position.y = y;
}

// Set monitor for the current window
void SetWindowMonitor(int monitor)
{
    pglSelectScreen(monitor != 0, 0);
}

// Set window minimum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMinSize(int width, int height)
{
    CORE.Window.screenMin.width = width;
    CORE.Window.screenMin.height = height;
}

// Set window maximum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMaxSize(int width, int height)
{
    CORE.Window.screenMax.width = width;
    CORE.Window.screenMax.height = height;
}

// Set window dimensions
void SetWindowSize(int width, int height)
{
    (void) width;   //unsupported
    (void) height;
}

// Set window opacity, value opacity is between 0.0 and 1.0
void SetWindowOpacity(float opacity)
{
    (void) opacity; //unsupported
}

// Set window focused
void SetWindowFocused(void)
{
    return; //unsupported
}

void *GetWindowHandle(void)
{
    return (void *)NULL;    //unsupported
}

// Get number of monitors
int GetMonitorCount(void)
{
    return 1;   //unsupported
}

// Get number of monitors
int GetCurrentMonitor(void)
{
    return 0;   //unsupported
}

// Get selected monitor position
Vector2 GetMonitorPosition(int monitor)
{
    (void) monitor;
    return (Vector2){ 0.0f, 0.0f }; //Unsupported
}

// Get selected monitor width (currently used by monitor)
int GetMonitorWidth(int monitor)
{
    (void) monitor;
    return 400;     //3ds top screen width
}

// Get selected monitor height (currently used by monitor)
int GetMonitorHeight(int monitor)
{
    return 240;     //3ds screen height
}

// Get selected monitor physical width in millimetres
int GetMonitorPhysicalWidth(int monitor)
{
    (void) monitor;
    return 90;  //TODO : actually measure the screen
}

// Get selected monitor physical height in millimetres
int GetMonitorPhysicalHeight(int monitor)
{
    (void) monitor;
    return 50;  //TODO : actually measure the screen
}

// Get selected monitor refresh rate
int GetMonitorRefreshRate(int monitor)
{
    (void) monitor;
    return 60;
}

// Get the human-readable, UTF-8 encoded name of the selected monitor
const char *GetMonitorName(int monitor)
{
    (void) monitor;
    return "3DS screen";
}

// Get window position XY on monitor
Vector2 GetWindowPosition(void)
{
    return (Vector2){ 0.f, 0.f };
}

// Get window scale DPI factor for current monitor
Vector2 GetWindowScaleDPI(void)
{
    return (Vector2){ 1.0f, 1.0f };
}

// Set clipboard text content
void SetClipboardText(const char *text)
{
    (void) text;
}

// Get clipboard text content
const char *GetClipboardText(void)
{
    return "";
}

// Show mouse cursor
void ShowCursor(void)
{
    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void HideCursor(void)
{
    CORE.Input.Mouse.cursorHidden = true;
}

// Enables cursor (unlock cursor)
void EnableCursor(void)
{
    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void DisableCursor(void)
{
    CORE.Input.Mouse.cursorHidden = true;
}

// Swap back buffer with front buffer (screen drawing)
void SwapScreenBuffer(void)
{
    pglSwapBuffers();
}

// Get elapsed time measure in seconds
double GetTime(void)
{
    uint64_t time = svcGetSystemTick();
    return u64_to_double(time - CORE.Time.base) / TICKS_PER_SEC;
}


// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// Ref: https://github.com/raysan5/raylib/issues/686
void OpenURL(const char *url)
{
    (void) url;
}

// Set internal gamepad mappings
int SetGamepadMappings(const char *mappings)
{
    (void) mappings;
    return 0;
}

// Set mouse position XY
void SetMousePosition(int x, int y)
{
    CORE.Input.Mouse.currentPosition = (Vector2){ (float)x, (float)y };
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
}

// Set mouse cursor
void SetMouseCursor(int cursor)
{
    CORE.Input.Mouse.cursor = cursor;
}

// Register all input events
void PollInputEvents(void)
{
    // Reset key pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;

    // Register previous touch states
    for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];

    hidScanInput();

    CORE.Input.Gamepad.ready[0] = true;

    //Process buttons
    for (int k = 0; k < 12; k++)
    {
        const GamepadButton button = GetGamepadButton(k);

        CORE.Input.Gamepad.previousButtonState[0][button] = CORE.Input.Gamepad.currentButtonState[0][button];

        if (hidKeysHeld() & (1 << k))
        {
            CORE.Input.Gamepad.currentButtonState[0][button] = 1;
            CORE.Input.Gamepad.lastButtonPressed = button;
        }
        else
        {
            CORE.Input.Gamepad.currentButtonState[0][button] = 0;
        }
    }

    //Process axis
    circlePosition circlePos;

    hidCircleRead(&circlePos);
    CORE.Input.Gamepad.axisState[0][GAMEPAD_AXIS_LEFT_X] = circlePos.x / 160.0f;
    CORE.Input.Gamepad.axisState[0][GAMEPAD_AXIS_LEFT_Y] = circlePos.y / 160.0f;

    irrstCstickRead(&circlePos);
    CORE.Input.Gamepad.axisState[0][GAMEPAD_AXIS_RIGHT_X] = circlePos.x / 160.0f;
    CORE.Input.Gamepad.axisState[0][GAMEPAD_AXIS_RIGHT_Y] = circlePos.y / 160.0f;

    CORE.Input.Gamepad.axisCount[0] = 4;

    //Process touchscreen
    touchPosition touchPos;

    hidTouchRead(&touchPos);

    CORE.Input.Touch.position[0].x = touchPos.x;
    CORE.Input.Touch.position[0].y = touchPos.y;

    CORE.Input.Touch.currentTouchState[0] = (hidKeysHeld() & KEY_TOUCH) ? 1 : 0;

    //Process gestures
    GestureEvent gestureEvent = { 0 };

    gestureEvent.pointCount = CORE.Input.Touch.currentTouchState[0] | CORE.Input.Touch.previousTouchState[0];

    if(gestureEvent.pointCount)
    {
        if(hidKeysDown() & KEY_TOUCH)
            gestureEvent.touchAction = INPUT_TOUCH_DOWN;
        else if(hidKeysUp() & KEY_TOUCH)
            gestureEvent.touchAction = INPUT_TOUCH_UP;
        else
            gestureEvent.touchAction = INPUT_TOUCH_POSITION;

        gestureEvent.position[0] = CORE.Input.Touch.position[0];

        gestureEvent.position[0].x /= (float)GetScreenWidth();
        gestureEvent.position[0].y /= (float)GetScreenHeight();
    }

#if defined(SUPPORT_GESTURES_SYSTEM)        //TODO : make sure this is true
    ProcessGestureEvent(gestureEvent);
#endif
}

//----------------------------------------------------------------------------------
// Module Internal Functions Definition
//----------------------------------------------------------------------------------

int InitPlatform(void)
{
    //Initialize the LCD framebuffer
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    //Initialize picaGL
    pglInit();

    CORE.Time.previous = GetTime();

    CORE.Window.ready = true;

    //TOP screen   TODO : support multiple screen ?
    CORE.Window.display.width  = 400;
    CORE.Window.display.height = 240;

    // Overwrite the values passed in InitWindow
    CORE.Window.screen.width  = 400;
    CORE.Window.screen.height = 240;

    CORE.Window.render.width = CORE.Window.screen.width;
    CORE.Window.render.height = CORE.Window.screen.height;
    CORE.Window.currentFbo.width = CORE.Window.render.width;
    CORE.Window.currentFbo.height = CORE.Window.render.height;

    // Screen size security check
    if (CORE.Window.screen.width <= 0) CORE.Window.screen.width = CORE.Window.display.width;
    if (CORE.Window.screen.height <= 0) CORE.Window.screen.height = CORE.Window.display.height;

    // At this point we need to manage render size vs screen size
    // NOTE: This function use and modify global module variables: CORE.Window.screen.width/CORE.Window.screen.height and CORE.Window.render.width/CORE.Window.render.height and CORE.Window.screenScale
    SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);
}

// Close platform
void ClosePlatform(void)
{
    pglExit();
    gfxExit();
    //romfsExit();
}


// Get gamepad button 3ds
static int GetGamepadButton(int button)
{
    int btn = GAMEPAD_BUTTON_UNKNOWN;
    switch (button)
    {
        case 0: btn = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
        case 1: btn = GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
        case 2: btn = GAMEPAD_BUTTON_MIDDLE_LEFT; break;
        case 3: btn = GAMEPAD_BUTTON_MIDDLE_RIGHT; break;
        case 4: btn = GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
        case 5: btn = GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;
        case 6: btn = GAMEPAD_BUTTON_LEFT_FACE_UP; break;
        case 7: btn = GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
        case 8: btn = GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;
        case 9: btn = GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
        case 10: btn = GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
        case 11: btn = GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;
    }
    return btn;
}
