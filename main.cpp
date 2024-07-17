#include "State.hpp"
#include <raylib.h>

int main() {

  InitWindow(850, 450, "Hello World");
  State CurrentState = State();

  while (!WindowShouldClose()) {
    CurrentState.CycleAll();
    CurrentState.Controls();
    CurrentState.Solve();

    BeginDrawing();

    CurrentState.DrawAll();
    ClearBackground(WHITE);

    EndDrawing();
  }
  CloseWindow();
}
