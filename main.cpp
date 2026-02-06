/*******************************************************************************************
*
*   Main v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/
#include "string"
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "perlin.cpp"
//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "main");

    // main: controls initialization
    //----------------------------------------------------------------------------------
    bool generatePressed = false;
    float octaveValue = 4;
    bool widthEditMode = false;
    int widthValue = 256;
    bool heightEditMode = false;
    int heightValue = 256;
    float persisValue = 0.6f;
    float lacunarityValue = 1.8f;
    float freqValue = 0.0005f;
    float ampValue = 1.0f;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------
        if(generatePressed){
            generate(widthValue,heightValue,octaveValue,persisValue,freqValue,ampValue,lacunarityValue);
             printf("WIDTH: %d | HEIGHT: %d | OCTAVE: %f | Persistent: %f | FREQUENCY: %f | AMPLITUDE: %f | LACUNARITY: %f\n",widthValue,heightValue,octaveValue,persisValue,freqValue,ampValue,lacunarityValue);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            generatePressed = GuiButton(Rectangle{ 320, 248, 120, 24 }, "Generate"); 
            if (GuiValueBox(Rectangle{ 80, 24, 120, 24 }, "WIDTH", &widthValue, 2, 1000, widthEditMode)) widthEditMode = !widthEditMode;
            if (GuiValueBox(Rectangle{ 80, 48, 120, 24 }, "HEIGHT", &heightValue, 2, 1000, heightEditMode)) heightEditMode = !heightEditMode;

            //sliders
            GuiSlider(Rectangle{ 88, 112, 120, 16 }, "PERSISTENT", NULL, &persisValue, 0.0f, 1.0f);
            GuiSlider(Rectangle{ 88, 136, 120, 16 }, "LA", NULL, &lacunarityValue, 1.5f, 4.0f);
            GuiSlider(Rectangle{ 88, 160, 120, 16 }, "FREQUENCY", NULL, &freqValue, 0.00001f, 0.001f);
            GuiSlider(Rectangle{ 88, 184, 120, 16 }, "AMPLITUDE", NULL, &ampValue, 0.0f, 1.0f);
            GuiSlider(Rectangle{ 88, 208, 120, 16 }, "OCTAVE", NULL, &octaveValue, 1.0f, 16.0f);

            //slider values
            GuiLabel(Rectangle{ 224, 112, 120, 24 }, std::to_string(persisValue).c_str());
            GuiLabel(Rectangle{ 224, 136, 120, 24 }, std::to_string(lacunarityValue).c_str());
            GuiLabel(Rectangle{ 224, 160, 120, 24 }, std::to_string(freqValue).c_str());
            GuiLabel(Rectangle{ 224, 184, 120, 24 }, std::to_string(ampValue).c_str());
            GuiLabel(Rectangle{ 224, 208, 120, 24 }, std::to_string(octaveValue).c_str());
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

