/*******************************************************************************************
*
*   raylib example - procedural mesh generation
*
*   Example originally created with raylib 1.8, last time updated with raylib 4.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2017-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vitasdk.h>
#include <pib.h>

//#include <signal.h>
//#include <unistd.h>
#include <raylib.h>
extern uint32_t sceLibcHeapSize = 2194304;
bool flag=true;

#define ALIGN_CENTER(a, b) (((a) - (b)) / 2)
#define ALIGN_RIGHT(x, w) ((x) - (w))

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



bool l1flag=false;
bool r1flag=false;
#define NUM_MODELS  9               // Parametric 3d shapes to generate
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



const int screenWidth = 960;
const int screenHeight = 544;
static Mesh GenMeshCustom(void);    // Generate a simple triangle mesh from code

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    debugNetInit("192.168.1.46",18194,3);
    sleep(1);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    
    InitWindow(screenWidth, screenHeight, "raylib [models] example - mesh generation");
    
    // We generate a checked image for texturing
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model models[NUM_MODELS] = { 0 };

    models[0] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 3));
    models[1] = LoadModelFromMesh(GenMeshCube(2.0f, 1.0f, 2.0f));
    models[2] = LoadModelFromMesh(GenMeshSphere(2, 32, 32));
    models[3] = LoadModelFromMesh(GenMeshHemiSphere(2, 16, 16));
    models[4] = LoadModelFromMesh(GenMeshCylinder(1, 2, 16));
    models[5] = LoadModelFromMesh(GenMeshTorus(0.25f, 4.0f, 16, 32));
    models[6] = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
    models[7] = LoadModelFromMesh(GenMeshPoly(5, 2.0f));
    models[8] = LoadModelFromMesh(GenMeshCustom());
    
    // Generated meshes could be exported as .obj files
    //ExportMesh(models[0].meshes[0], "plane.obj");
    //ExportMesh(models[1].meshes[0], "cube.obj");
    //ExportMesh(models[2].meshes[0], "sphere.obj");
    //ExportMesh(models[3].meshes[0], "hemisphere.obj");
    //ExportMesh(models[4].meshes[0], "cylinder.obj");
    //ExportMesh(models[5].meshes[0], "torus.obj");
    //ExportMesh(models[6].meshes[0], "knot.obj");
    //ExportMesh(models[7].meshes[0], "poly.obj");
    //ExportMesh(models[8].meshes[0], "custom.obj");

    // Set checked texture as default diffuse component for all models material
    for (int i = 0; i < NUM_MODELS; i++) models[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    // Define the camera to look into our 3d world
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Model drawing position
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    int currentModel = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (flag)    // press triangle to go out
    {
        // Update
        //----------------------------------------------------------------------------------
        readPad();
        UpdateCamera(&camera, CAMERA_ORBITAL);


        if (pressed_pad[PAD_RIGHT])
        {
            currentModel++;
            if (currentModel >= NUM_MODELS) currentModel = 0;
        }
        else if (pressed_pad[PAD_LEFT])
        {
            currentModel--;
            if (currentModel < 0) currentModel = NUM_MODELS - 1;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

               DrawModel(models[currentModel], position, 1.0f, WHITE);
               DrawGrid(10, 1.0);

            EndMode3D();

            DrawRectangle(30, screenHeight-50, 310, 30, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(30, screenHeight-50, 310, 30, Fade(DARKBLUE, 0.5f));
            DrawText("Left/Right  BUTTON to CYCLE PROCEDURAL MODELS", 40, screenHeight-30, 10, BLUE);

            switch(currentModel)
            {
                case 0: DrawText("PLANE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 1: DrawText("CUBE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 2: DrawText("SPHERE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 3: DrawText("HEMISPHERE", screenWidth-140, 10, 20, DARKBLUE); break;
                case 4: DrawText("CYLINDER", screenWidth-140, 10, 20, DARKBLUE); break;
                case 5: DrawText("TORUS", screenWidth-140, 10, 20, DARKBLUE); break;
                case 6: DrawText("KNOT", screenWidth-140, 10, 20, DARKBLUE); break;
                case 7: DrawText("POLY", screenWidth-140, 10, 20, DARKBLUE); break;
                case 8: DrawText("Custom (triangle)", screenWidth-190, 10, 20, DARKBLUE); break;
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Unload texture

    // Unload models data (GPU VRAM)
    for (int i = 0; i < NUM_MODELS; i++) UnloadModel(models[i]);

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    sceKernelExitProcess(0);
    return 0;
}

// Generate a simple triangle mesh from code
static Mesh GenMeshCustom(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 1;
    mesh.vertexCount = mesh.triangleCount*3;
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));    // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount*2*sizeof(float));   // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount*3*sizeof(float));     // 3 vertices, 3 coordinates each (x, y, z)

    // Vertex at (0, 0, 0)
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    // Vertex at (1, 0, 2)
    mesh.vertices[3] = 1;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = 2;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = 0.5f;
    mesh.texcoords[3] = 1.0f;

    // Vertex at (2, 0, 0)
    mesh.vertices[6] = 2;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = 1;
    mesh.texcoords[5] =0;

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}
