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
#include <string>
#include <vector>

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

    //persistent storages
    std::vector<unsigned char> pixelData;
    Texture2D noiseTexture = {0};

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
    int colorModeActive = 0;
    bool alphaChecked = false;
    bool colorModeEditMode = false;

    int imageSizeActive = 0;
    bool imageSizeEditMode = false;
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
            pixelData = generate(widthValue,heightValue,octaveValue,persisValue,freqValue,ampValue,lacunarityValue);
            Image image = {0};
            image.data = pixelData.data();
            image.width = widthValue;
            image.height = heightValue;
            image.mipmaps = 1;
            image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8;
            noiseTexture = LoadTextureFromImage(image);
            //printf("WIDTH: %d | HEIGHT: %d | OCTAVE: %f | Persistent: %f | FREQUENCY: %f | AMPLITUDE: %f | LACUNARITY: %f\n",widthValue,heightValue,octaveValue,persisValue,freqValue,ampValue,lacunarityValue);
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 
            DrawTexture(noiseTexture, screenWidth - noiseTexture.width, 0, WHITE);
            // raygui: controls drawing
            //----------------------------------------------------------------------------------

            //buttons
            generatePressed = GuiButton(Rectangle{ 320, 248, 120, 24 }, "Generate"); 

            //digit boxes
            //if (GuiValueBox(Rectangle{ 80, 24, 120, 24 }, "WIDTH", &widthValue, 2, 1000, widthEditMode)) widthEditMode = !widthEditMode;
            //if (GuiValueBox(Rectangle{ 80, 48, 120, 24 }, "HEIGHT", &heightValue, 2, 1000, heightEditMode)) heightEditMode = !heightEditMode;

            //sliders
            GuiSlider(Rectangle{ 100, 112, 120, 16 }, "PERSISTENT", NULL, &persisValue, 0.0f, 1.0f);
            GuiSlider(Rectangle{ 100, 136, 120, 16 }, "LA", NULL, &lacunarityValue, 1.5f, 4.0f);
            GuiSlider(Rectangle{ 100, 160, 120, 16 }, "FREQUENCY", NULL, &freqValue, 0.001f, 0.02f);
            GuiSlider(Rectangle{ 100, 184, 120, 16 }, "AMPLITUDE", NULL, &ampValue, 0.0f, 1.0f);
            GuiSlider(Rectangle{ 100, 208, 120, 16 }, "OCTAVE", NULL, &octaveValue, 1.0f, 16.0f);

            //slider values
            GuiLabel(Rectangle{ 246, 112, 120, 16 }, std::to_string(persisValue).c_str());
            GuiLabel(Rectangle{ 246, 136, 120, 16 }, std::to_string(lacunarityValue).c_str());
            GuiLabel(Rectangle{ 246, 160, 120, 16 }, std::to_string(freqValue).c_str());
            GuiLabel(Rectangle{ 246, 184, 120, 16 }, std::to_string(ampValue).c_str());
            GuiLabel(Rectangle{ 246, 208, 120, 16 }, std::to_string(octaveValue).c_str());

            //checkboxes
            GuiCheckBox(Rectangle{ 430, 32, 24, 24 }, "ALPHA", &alphaChecked);

            //dropdown menus
            if (GuiDropdownBox(Rectangle{ 300, 32, 120, 24 }, "GREY;RGB", &colorModeActive, colorModeEditMode)) colorModeEditMode = !colorModeEditMode;
            if (GuiDropdownBox(Rectangle{ 100, 24, 120, 24 }, "2;4;8;16;32;64;128;256;512;1024;2048;4096", &imageSizeActive, imageSizeEditMode)) imageSizeEditMode = !imageSizeEditMode;

            //dropdown menus descriptions
            GuiLabel(Rectangle{ 0, 24, 100, 24 }, "IMAGE SIZE");
            GuiLabel(Rectangle{ 228, 32, 120, 24 }, "COLOUR MODE");
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

