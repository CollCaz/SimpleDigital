#include "Gates.hpp"
#include "State.hpp"
#include <raylib.h>

State CurrentState = State();

int main() {
  AndGate *g = new AndGate;
  Output *o = new Output;
  o->OffsetPosition(Vector2{200, 200});
  CurrentState.SetOutput(o);
  InitWindow(850, 450, "Hello World");
  while (!WindowShouldClose()) {
    CurrentState.CycleAll();
    CurrentState.Controls();
    CurrentState.TestAll();
    BeginDrawing();
    CurrentState.DrawAll();
    ClearBackground(WHITE);
    EndDrawing();
  }
  CloseWindow();
}
