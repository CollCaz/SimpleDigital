#pragma once
#include "Gates.hpp"
#include <iostream>
#include <raylib.h>
#include <vector>

class State {
  // Drawables
  std::vector<Object *> Objects;

  // Gates
  std::vector<Gate *> Gates;

public:
  State() {}
  inline void SetOutput(Output *o) { this->Out = o; }
  inline void AddObject(Object *ob) { this->Objects.push_back(ob); }

  inline void AddGate(Gate *ob) {
    AddObject(ob);
    this->Gates.push_back(ob);
  }

  inline void DrawAll() {
    for (auto object : this->Objects) {
      object->Draw();
    }
    this->Out->Draw();
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

    for (auto gate : this->Gates) {
      gate->DragToConnect();
      if (gate->IsConnDragging()) {
        this->DraggedConnection = gate;
      }
      if (gate->IsMouseOnThis() && !gate->IsConnDragging() &&
          gate->HasEmptyConn() && this->DraggedConnection != nullptr &&
          this->DraggedConnection != gate) {
        gate->ConnectToThis(this->DraggedConnection);
        std::cout << (this->DraggedConnection == nullptr) << std::endl;
        this->DraggedConnection = nullptr;
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
    case KEY_TWO:
      this->AddGate(new AndGate);
    }
  }
};
