/*******************************************************************************************
*
*   raylib [shapes] example - raylib logo animation
*
*   Example originally created with raylib 2.5, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vitasdk.h>
#include <pib.h>
#include <raylib.h>
extern uint32_t sceLibcHeapSize = 2194304;
bool flag=true;
int xflag=0;

#define ANALOG_CENTER 128
#define ANALOG_THRESHOLD 64
#define ANALOG_SENSITIVITY 16
int enter_button=0;
enum PadButtons {
  PAD_UP,
  PAD_DOWN,
  PAD_LEFT,
  PAD_RIGHT,
  PAD_LTRIGGER,
  PAD_RTRIGGER,
  PAD_TRIANGLE,
  PAD_CIRCLE,
  PAD_CROSS,
  PAD_SQUARE,
  PAD_START,
  PAD_SELECT,
  PAD_ENTER,
  PAD_CANCEL,
  PAD_LEFT_ANALOG_UP,
  PAD_LEFT_ANALOG_DOWN,
  PAD_LEFT_ANALOG_LEFT,
  PAD_LEFT_ANALOG_RIGHT,
  PAD_RIGHT_ANALOG_UP,
  PAD_RIGHT_ANALOG_DOWN,
  PAD_RIGHT_ANALOG_LEFT,
  PAD_RIGHT_ANALOG_RIGHT,
  PAD_N_BUTTONS
};

typedef uint8_t Pad[PAD_N_BUTTONS];

SceCtrlData pad;
Pad old_pad, current_pad, pressed_pad, released_pad, hold_pad, hold2_pad;
Pad hold_count, hold2_count;

void readPad() {
  memset(&pad, 0, sizeof(SceCtrlData));
  sceCtrlPeekBufferPositive(0, &pad, 1);

  memcpy(&old_pad, current_pad, sizeof(Pad));
  memset(&current_pad, 0, sizeof(Pad));

  if (pad.buttons & SCE_CTRL_UP)
    current_pad[PAD_UP] = 1;
  if (pad.buttons & SCE_CTRL_DOWN)
    current_pad[PAD_DOWN] = 1;
  if (pad.buttons & SCE_CTRL_LEFT)
    current_pad[PAD_LEFT] = 1;
  if (pad.buttons & SCE_CTRL_RIGHT)
    current_pad[PAD_RIGHT] = 1;
  if (pad.buttons & SCE_CTRL_LTRIGGER)
    current_pad[PAD_LTRIGGER] = 1;
  if (pad.buttons & SCE_CTRL_RTRIGGER)
    current_pad[PAD_RTRIGGER] = 1;
  if (pad.buttons & SCE_CTRL_TRIANGLE)
    current_pad[PAD_TRIANGLE] = 1;
  if (pad.buttons & SCE_CTRL_CIRCLE)
    current_pad[PAD_CIRCLE] = 1;
  if (pad.buttons & SCE_CTRL_CROSS)
    current_pad[PAD_CROSS] = 1;
  if (pad.buttons & SCE_CTRL_SQUARE)
    current_pad[PAD_SQUARE] = 1;
  if (pad.buttons & SCE_CTRL_START)
    current_pad[PAD_START] = 1;
  if (pad.buttons & SCE_CTRL_SELECT)
    current_pad[PAD_SELECT] = 1;

  if (pad.ly < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_UP] = 1;
  } else if (pad.ly > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_DOWN] = 1;
  }

  if (pad.lx < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_LEFT] = 1;
  } else if (pad.lx > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_LEFT_ANALOG_RIGHT] = 1;
  }

  if (pad.ry < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_UP] = 1;
  } else if (pad.ry > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_DOWN] = 1;
  }

  if (pad.rx < ANALOG_CENTER - ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_LEFT] = 1;
  } else if (pad.rx > ANALOG_CENTER + ANALOG_THRESHOLD) {
    current_pad[PAD_RIGHT_ANALOG_RIGHT] = 1;
  }

  int i;
  for (i = 0; i < PAD_N_BUTTONS; i++) {
    pressed_pad[i] = current_pad[i] & ~old_pad[i];
    released_pad[i] = ~current_pad[i] & old_pad[i];

    hold_pad[i] = pressed_pad[i];
    hold2_pad[i] = pressed_pad[i];

    if (current_pad[i]) {
      if (hold_count[i] >= 10) {
        hold_pad[i] = 1;
        hold_count[i] = 6;
      }

      if (hold2_count[i] >= 10) {
        hold2_pad[i] = 1;
        hold2_count[i] = 10;
      }

      hold_count[i]++;
      hold2_count[i]++;
    } else {
      hold_count[i] = 0;
      hold2_count[i] = 0;
    }
  }

  if (enter_button == SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE) {
    old_pad[PAD_ENTER] = old_pad[PAD_CIRCLE];
    current_pad[PAD_ENTER] = current_pad[PAD_CIRCLE];
    pressed_pad[PAD_ENTER] = pressed_pad[PAD_CIRCLE];
    released_pad[PAD_ENTER] = released_pad[PAD_CIRCLE];
    hold_pad[PAD_ENTER] = hold_pad[PAD_CIRCLE];
    hold2_pad[PAD_ENTER] = hold2_pad[PAD_CIRCLE];

    old_pad[PAD_CANCEL] = old_pad[PAD_CROSS];
    current_pad[PAD_CANCEL] = current_pad[PAD_CROSS];
    pressed_pad[PAD_CANCEL] = pressed_pad[PAD_CROSS];
    released_pad[PAD_CANCEL] = released_pad[PAD_CROSS];
    hold_pad[PAD_CANCEL] = hold_pad[PAD_CROSS];
    hold2_pad[PAD_CANCEL] = hold2_pad[PAD_CROSS];
  } else {
    old_pad[PAD_ENTER] = old_pad[PAD_CROSS];
    current_pad[PAD_ENTER] = current_pad[PAD_CROSS];
    pressed_pad[PAD_ENTER] = pressed_pad[PAD_CROSS];
    released_pad[PAD_ENTER] = released_pad[PAD_CROSS];
    hold_pad[PAD_ENTER] = hold_pad[PAD_CROSS];
    hold2_pad[PAD_ENTER] = hold2_pad[PAD_CROSS];

    old_pad[PAD_CANCEL] = old_pad[PAD_CIRCLE];
    current_pad[PAD_CANCEL] = current_pad[PAD_CIRCLE];
    pressed_pad[PAD_CANCEL] = pressed_pad[PAD_CIRCLE];
    released_pad[PAD_CANCEL] = released_pad[PAD_CIRCLE];
    hold_pad[PAD_CANCEL] = hold_pad[PAD_CIRCLE];
    hold2_pad[PAD_CANCEL] = hold2_pad[PAD_CIRCLE];
  }
}

int holdButtons(SceCtrlData *pad, uint32_t buttons, uint64_t time) {
  if ((pad->buttons & buttons) == buttons) {
    uint64_t time_start = sceKernelGetProcessTimeWide();

    while ((pad->buttons & buttons) == buttons) {
      sceKernelDelayThread(10 * 1000);
      sceCtrlPeekBufferPositive(0, pad, 1);

      if ((sceKernelGetProcessTimeWide() - time_start) >= time) {
        return 1;
      }
    }
  }

  return 0;
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 960;
    const int screenHeight = 544;
    debugNetInit("192.168.1.46",18194,3);
    sleep(1);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - raylib logo animation");
    
    int logoPositionX = screenWidth/2 - 128;
    int logoPositionY = screenHeight/2 - 128;

    int framesCounter = 0;
    int lettersCount = 0;

    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;

    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;

    int state = 0;                  // Tracking animation states (State Machine)
    float alpha = 1.0f;             // Useful for fading

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (flag)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        readPad();
        if(pressed_pad[PAD_CROSS]) xflag=1;
        if (state == 0)                 // State 0: Small box blinking
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;      // Reset counter... will be used later...
            }
        }
        else if (state == 1)            // State 1: Top and left bars growing
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bottom and right bars growing
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter/12)       // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    state = 4;
                }
            }
        }
        else if (state == 4)            // State 4: Reset and Replay
        {
            if (xflag)
            {
                framesCounter = 0;
                lettersCount = 0;

                topSideRecWidth = 16;
                leftSideRecHeight = 16;

                bottomSideRecWidth = 16;
                rightSideRecHeight = 16;

                alpha = 1.0f;
                state = 0;          // Return to State 0
                xflag=0;
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));
            }
            else if (state == 4)
            {
                DrawText("[X] REPLAY", screenWidth/2-60, screenHeight/2-10, 20, BLACK);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
        sceKernelExitProcess(0);

    return 0;
}
