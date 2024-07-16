#include "Gates.hpp"
#include <iostream>
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

bool Object::IsMouseOnThis() { return this->mouseOn; }

// Point

Point::Point() {}

void Point::Draw() {
  DrawText(this->lable.c_str(), this->position.x, this->position.y, 10, BLACK);
  DrawLineV(this->ConnectionLoc, this->pointingTo, BLACK);
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

void Point::DragToConnect() {
  if (this->ConnectedToOutput || this->ConnectedToGate) {
    return;
  }
  std::cout << "AAA" << std::endl;
  if (this->rightClicking) {
    this->connDragging = true;
    this->pointingTo = GetMousePosition();
    return;
  } else {
    this->connDragging = false;
    this->pointingTo = this->position;
  }
}

bool Point::IsConnDragging() { return this->connDragging; }

void Point::ConnectThis(Gate *g) {
  this->ConnectedToGate = true;
  this->GateConnectedTo = g;
  this->pointingTo = Vector2{0, 0};
}

void Point::ConnectThis(Output *o) {
  this->ConnectedToOutput = true;
  this->OutputConnectedTo = o;
  this->pointingTo = o->GetPosition();
}

void Point::Cycle() {
  this->ConnectionLoc = this->position;
  this->CheckMouse();
  this->DragMove();
  this->DragToConnect();
}

// Output
Output::Output() {}
void Output::Draw() {
  DrawCircleV(this->position, this->radius, this->color);
  DrawText(this->label.c_str(), (this->position.x - 13), (this->position.y - 4),
           10, BLACK);
}

void Output::ConnectToThis(Point *p) {
  if (this->Input != nullptr) {
    return;
  }
  this->Input = p;
  p->ConnectThis(this);
  p->SetPointingTo(this->position);
}

void Output::Cycle() {
  this->CheckMouse();
  if (this->Input != nullptr) {
    this->Input->SetPointingTo(this->position);
  }
}

void Output::CheckMouse() {
  Vector2 mp = GetMousePosition();
  if ((std::abs(mp.x - this->position.x)) <= this->radius &&
      (std::abs(mp.y - this->position.y)) <= this->radius) {
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

// Gate

Gate::Gate() {
  this->size = Vector2{50, 50};
  this->position = GetMousePosition();
}

void Gate::Draw() {
  DrawRectangleV(this->position, this->size, BLACK);
  DrawText(this->lable.c_str(), this->position.x + (this->size.x / 2 - 8),
           this->position.y + (this->size.y / 2 - 4), 10, WHITE);
  this->connDragging = false;
  DrawLineV(this->ConnectionLoc, this->pointingTo, BLACK);
}

void Gate::ConnectToThis(Point *p) {
  if (!this->aIsConnected) {
    this->Connectiona = p;
    this->aIsConnected = true;
  } else if (!this->bIsConnected) {
    this->Connectionb = p;
    this->bIsConnected = true;
  } else {
    return;
  }
  p->ConnectThis(this);
}

bool Gate::HasEmptyConn() {
  // NAND: only returns true if either or both are false
  return (!this->aIsConnected || !this->bIsConnected);
}

void Gate::Cycle() {
  this->ConnectionPointa = this->position;
  this->ConnectionPointb = this->position;
  this->ConnectionLoc = this->position;
  if (aIsConnected) {
    this->Connectiona->SetPointingTo(this->ConnectionPointb);
  }
  if (bIsConnected) {
    this->Connectionb->SetPointingTo(this->ConnectionPointa);
  }
  // this->CheckMouse();
  // this->DragMove();
  // this->DragToConnect();
}

//
// AndGate

bool AndGate::Solve() { return this->a && this->b; }
