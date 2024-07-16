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
  DrawLineV(this->position, this->pointingTo, BLACK);
}

void Point::CheckMouse() {
  Vector2 mp = GetMousePosition();
  if ((std::abs(mp.x - this->position.x)) <= this->radius &&
      (std::abs(mp.y - this->position.y)) <= this->radius) {
    this->mouseOn = true;
  } else {
    this->mouseOn = false;
  }

  // Holding
  if (this->mouseOn && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    this->held = true;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    this->held = false;
  }

  // Right Click Holding
  if (this->mouseOn && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    this->rightClicking = true;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
    this->rightClicking = false;
    this->connDragging = false;
  }
}

void Point::DragToConnect() {
  if (this->ConnectedToOutput || this->ConnectedToGate) {
    return;
  }
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
  this->connDragging = false;
  this->ConnectedToGate = true;
  this->GateConnectedTo = g;
  this->pointingTo = g->GetPosition();
}

void Point::ConnectThis(Output *o) {
  this->ConnectedToOutput = true;
  this->OutputConnectedTo = o;
  this->pointingTo = o->GetPosition();
}

bool Point::Solve() { return this->Value; }

void Point::Cycle() {
  this->CheckMouse();
  this->DragMove();
  this->DragToConnect();
}

// Switch
void Switch::Draw() {
  DrawCircleV(this->position, this->radius, this->color);
  DrawLineV(this->position, this->pointingTo, BLACK);
}

void Switch::SwitchValue() {
  std::cout << this->Value << std::endl;
  this->Value = !this->Value;
  this->color = this->Value ? GREEN : RED;
}

void Switch::Cycle() {
  // if (this->IsMouseOnThis()) {
  //   std::cout << "AAAA" << std::endl;
  // }
  if (this->IsMouseOnThis() && IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
    this->SwitchValue();
    if (this->ConnectedToGate) {
      this->GateConnectedTo->Forget();
    }
  }
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

void Output::Solve() {
  if (!this->IsConnectedTo()) {
    return;
  }

  this->value = this->Input->Solve();
  this->SetLabel();
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

Gate::Gate() { this->position = GetMousePosition(); }

void Gate::Draw() {
  DrawCircleV(this->position, this->radius, BLACK);
  DrawText(this->lable.c_str(),
           this->position.x - (this->lable.length() * 2 + 1),
           this->position.y - (this->lable.length() + 1), 10, WHITE);
  DrawLineV(this->position, this->pointingTo, BLACK);
}

void Gate::Forget() {
  this->aIsKnown = false;
  this->bIsKnown = false;
  if (this->ConnectedToGate) {
    this->GateConnectedTo->Forget();
  }
}

void Gate::ConnectToThis(Point *p) {
  if (!this->aIsConnected) {
    this->Connectiona = p;
    this->aIsConnected = true;
    p->ConnectThis(this);
    return;
  } else if (!this->bIsConnected) {
    this->Connectionb = p;
    this->bIsConnected = true;
    p->ConnectThis(this);
    return;
  } else {
    return;
  }
}

bool Gate::HasEmptyConn() {
  // NAND: only returns true if either or both are false
  return (!this->aIsConnected || !this->bIsConnected);
}

bool Gate::Solve() {
  if (!this->aIsConnected) {
    this->a = false;
    this->aIsKnown = true;
  }
  if (!this->bIsConnected) {
    this->b = false;
    this->bIsKnown = true;
  }
  if (!this->aIsKnown) {
    this->a = this->Connectiona->Solve();
    this->aIsKnown = true;
  }
  if (!this->bIsKnown) {
    this->b = this->Connectionb->Solve();
    this->bIsKnown = true;
  }
  return CalcGate();
}

void Gate::Cycle() {
  if (this->aIsConnected) {
    this->Connectiona->SetPointingTo(this->position);
  }
  if (this->bIsConnected) {
    this->Connectionb->SetPointingTo(this->position);
  }
  // this->CheckMouse();
  // this->DragMove();
  // this->DragToConnect();
}
