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
  inline void AddObject(Object *ob) { this->Objects.push_back(ob); }

  inline void AddGate(Gate *ob) {
    AddObject(ob);
    this->Gates.push_back(ob);
  }

  inline void DrawAll() {
    for (auto object : this->Objects) {
      object->Draw();
    }
  }

  inline void CycleAll() {
    for (auto object : this->Objects) {
      object->Cycle();
      object->DragMove();
    }

    for (auto gate : this->Gates) {
      if (gate->DragToConnect()) {
        this->DraggedConnection = gate;
      }
    }
  }

  // If an object is currently being moved
  bool MovingObject;
  Point *DraggedConnection = nullptr;

  void Controls() {
    switch (GetKeyPressed()) {
    case KEY_TWO:
      this->AddGate(new AndGate);
    }
  }
};
