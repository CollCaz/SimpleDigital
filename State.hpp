#pragma once
#include "Gates.hpp"
#include <raylib.h>
#include <vector>

class State {
  // Drawables
  std::vector<Object *> Objects;

  // Gates
  std::vector<Gate *> Gates;

  // Points
  std::vector<Point *> Points;

public:
  State() {}
  inline void SetOutput(Output *o) { this->Out = o; }
  inline void AddObject(Object *ob) { this->Objects.push_back(ob); }

  inline void AddGate(Gate *ob) {
    AddPoint(ob);
    this->Gates.push_back(ob);
  }

  inline void AddPoint(Point *p) {
    AddObject(p);
    this->Points.push_back(p);
  }

  inline void DrawAll() {
    for (auto object : this->Objects) {
      object->Draw();
    }
    this->Out->Draw();
  }

  inline void TestAll() {
    if (this->Out->IsConnectedTo()) {
      this->Out->Solve();
    }
  }

  inline void CycleAll() {
    for (auto object : this->Objects) {
      object->Cycle();
      object->CheckMouse();
      object->DragMove();
    }
    this->Out->Cycle();
    this->Out->CheckMouse();
    this->Out->DragMove();

    for (auto point : this->Points) {
      point->DragToConnect();
    }

    for (auto point : this->Points) {
      point->DragToConnect();
      if (point->IsConnDragging()) {
        this->DraggedConnection = point;
      }
    }
    for (auto gate : this->Gates) {
      if (this->DraggedConnection != nullptr && gate->IsMouseOnThis()) {
        if (!gate->IsConnDragging() && gate->IsMouseOnThis() &&
            gate != this->DraggedConnection) {
          gate->ConnectToThis(this->DraggedConnection);
          this->DraggedConnection = nullptr;
        }
      }
    }
    if (this->Out != nullptr && this->DraggedConnection != nullptr) {
      if (this->Out->IsMouseOnThis() && !this->Out->IsConnectedTo()) {
        this->Out->ConnectToThis(this->DraggedConnection);
        this->DraggedConnection->ConnectThis(this->Out);
        this->DraggedConnection = nullptr;
      }
    }
  }

  // If an object is currently being moved
  bool MovingObject;
  Point *DraggedConnection = nullptr;
  Output *Out = nullptr;

  inline void Controls() {
    switch (GetKeyPressed()) {
    case KEY_ONE:
      this->AddPoint(new Switch);
      break;
    case KEY_TWO:
      this->AddGate(new AndGate);
      break;
    case KEY_THREE:
      this->AddGate(new OrGate);
      break;
    case KEY_FOUR:
      this->AddGate(new NandGate);
      break;
    case KEY_FIVE:
      this->AddGate(new NorGate);
      break;
    case KEY_SIX:
      this->AddGate(new XorGate);
      break;
    case KEY_SEVEN:
      this->AddGate(new XnorGate);
      break;
    }
  }
};
