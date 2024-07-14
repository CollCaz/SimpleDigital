#include "Gates.hpp"
#include <raylib.h>

// Object

void Object::OffsetPosition(Vector2 offset) {
  this->position.x += offset.x;
  this->position.y += offset.y;
}

bool Object::DragMove() {
  if (this->held) {
    OffsetPosition(GetMouseDelta());
    return true;
  }
  return false;
}

// Point

Point::Point() {}

void Point::Draw() {
  DrawText(this->lable.c_str(), this->position.x, this->position.y, 10, BLACK);
}

void Point::CheckMouse() {
  Vector2 mp = GetMousePosition();
  if (mp.x > this->position.x && mp.x < this->position.x + this->size.x &&
      mp.y > this->position.y && mp.y < this->position.y + this->size.y) {
    this->mouseOn = true;
  } else {
    this->mouseOn = false;
  }

  // Holding
  if (this->mouseOn && IsMouseButtonPressed(0)) {
    this->held = true;
  }
  if (IsMouseButtonReleased(0)) {
    this->held = false;
  }

  // Right Click Holding
  if (this->mouseOn && IsMouseButtonPressed(1)) {
    this->rightClicking = true;
  }
  if (IsMouseButtonReleased(1)) {
    this->rightClicking = false;
  }
}

bool Point::DragToConnect() {
  if (this->rightClicking || this->Connected) {
    DrawLineV(this->ConnectionLoc, GetMousePosition(), BLACK);
    return true;
  }
  return false;
}

void Point::Cycle() {
  this->ConnectionLoc = this->position;
  this->CheckMouse();
  this->DragMove();
  this->DragToConnect();
}

// Gate

Gate::Gate() {
  this->size = Vector2{50, 50};
  this->position = GetMousePosition();
}

void Gate::Draw() {
  DrawRectangleV(this->position, this->size, BLACK);
  DrawText(this->lable.c_str(), this->position.x + (this->size.x / 2 - 8),
           this->position.y + (this->size.y / 2 - 4), 10, WHITE);
}

void Gate::ConnectToThis(Point *p) {
  if (!this->aIsConnected) {
    this->Connectiona = p;
  } else {
    this->Connectionb = p;
  }
}

void Gate::Cycle() {
  this->ConnectionLoc = this->position;
  this->CheckMouse();
  // this->DragMove();
  // this->DragToConnect();
}

//
// AndGate

bool AndGate::Solve() { return this->a && this->b; }
