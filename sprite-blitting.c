#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define SPRITE_WIDTH 18
#define SPRITE_HEIGHT 28

typedef struct Glyph {
  int x;
  int y;
  Color fg;
} Glyph;


int main(void) {
  printf("Sprite Blitting Example using C and Raylib\n");

  // Initialize the RNG
  srand(time(NULL));

  // Initialize the window
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sprite Blitting Example - C/Raylib");

  // Load the spritesheet
  Texture2D spritesheet = LoadTexture("BrogueFont5.png");

  // In our SDL example, we created a texture in memory and blitted our 
  // random glyphs to it, swapping the buffers once every second. Raylib doesn't
  // allow such chicanery unless we decide to do away with its framebuffer 
  // management entirely.
  //
  // Additionally Raylib doesn't appear to have much functionality for 
  // manipulating textures in memory (for example, copying one texture onto 
  // another).
  //
  // So we're going to do this the way we probably should have done with the 
  // SDL example too - we're going to store a map of which glyph cell on the 
  // screen currently holds which glyph and what colors. And then we'll consult
  // this map each draw call to see what to draw.
  size_t screenGlyphHeight = WINDOW_HEIGHT / SPRITE_HEIGHT + 1;
  size_t screenGlyphWidth = WINDOW_WIDTH / SPRITE_WIDTH + 1;
  Glyph glyphMap[screenGlyphWidth][screenGlyphHeight];

  // Rectangle to hold the glyph to blit
  Rectangle glyphRec = { 0.0f, 0.0f, (float)SPRITE_WIDTH, (float)SPRITE_HEIGHT };
  // Position on the screen to blit to
  Vector2 position = { 0.0f, 0.0f };

  float ticks = 1.0; // Ensure we draw one frame right away

  SetTargetFPS(60);  

  // Main Loop
  while (!WindowShouldClose()) { // Detect when user clicks on X or presses ESC

    // Update
    ticks += GetFrameTime(); // Add elapsed time since last frame to counter

    if (ticks > 1) { // Every second, time to update the map! 
      for (size_t i = 0; i < screenGlyphWidth; i++) {
        for (size_t j = 0; j < screenGlyphHeight; j++) {
          glyphMap[i][j].fg = (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
          glyphMap[i][j].x = rand() % 16 * SPRITE_WIDTH;
          glyphMap[i][j].y = rand() % 13 * SPRITE_HEIGHT;
        }
      }
      ticks = 0;
    }

    // Drawing
    //
    // According to Raylib, the BeginDrawing() + EndDrawing() takes care of
    // double buffering for us - all drawing happens on a buffer that gets
    // swapped with the currently displayed buffer when the EndDrawing() call
    // happens. Cool!
    BeginDrawing();

    ClearBackground(BLACK);
    for (size_t i = 0; i < screenGlyphWidth; i++) {
      for (size_t j = 0; j < screenGlyphHeight; j++) {
          glyphRec.x = glyphMap[i][j].x;
          glyphRec.y = glyphMap[i][j].y;
          position.x = i * SPRITE_WIDTH;
          position.y = j * SPRITE_HEIGHT;
          // Can't do, can't set color to be the alpha channel?
          // DrawRectangle(position.x, position.y, SPRITE_WIDTH, SPRITE_HEIGHT, glyphMap[i][j].bg);
          DrawTextureRec(spritesheet, glyphRec, position, glyphMap[i][j].fg);
      }
    }
    EndDrawing();
  }

  // De-init
  UnloadTexture(spritesheet);
  CloseWindow();

  return 0;
}
