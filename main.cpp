#include "Gates.hpp"
#include "State.hpp"
#include <raylib.h>

State CurrentState = State();

int main() {
  AndGate *g = new AndGate;
  Output *o = new Output;
  o->OffsetPosition(Vector2{200, 200});
  g->OffsetPosition(Vector2{100, 100});
  CurrentState.AddGate(g);
  CurrentState.AddOutput(o);
  InitWindow(850, 450, "Hello World");
  while (!WindowShouldClose()) {
    CurrentState.CycleAll();
    CurrentState.Controls();
    BeginDrawing();
    CurrentState.DrawAll();
    ClearBackground(WHITE);
    EndDrawing();
  }
  CloseWindow();
}
