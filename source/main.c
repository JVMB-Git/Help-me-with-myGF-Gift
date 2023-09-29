#include<stdio.h>
#include<assert.h>
#include<math.h>

#include<raylib.h>
#include"../include/test.h"

int main(void){

const int screenWidth = 800;
const int screenHeight = 450;

InitWindow(screenWidth, screenHeight, "basic window");

Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
Color ballColor = DARKBLUE;

SetTargetFPS(60);
    while (!WindowShouldClose())
    {
		 ballPosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) ballColor = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) ballColor = LIME;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) ballColor = DARKBLUE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) ballColor = PURPLE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) ballColor = YELLOW;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) ballColor = ORANGE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) ballColor = BEIGE;

			/*
			if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
			if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
			if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
			if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;
			*/
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("You title text ", 190, 200, 20, LIGHTGRAY); 
				DrawCircleV(ballPosition, 50, ballColor);
    
		  EndDrawing();
    }

    CloseWindow();

	return 0 ;
}
