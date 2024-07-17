#pragma once
#include <iostream>
#include <raylib.h>
#include <string>

class Object {
protected:
  Vector2 position = Vector2{100, 100};

  bool mouseOn;
  bool held = false;
  bool dragging = false;
  bool rightClicking = false;
  bool leftClicking = false;

public:
  virtual void Draw() = 0;
  virtual void Cycle() = 0;
  virtual void CheckMouse() = 0;
  bool IsMouseOnThis();

  bool DragMove();
  void OffsetPosition(Vector2 offset);
};

class Gate;
class Output;
class Point : public Object {
protected:
  std::string lable;
  float radius = 26;
  Color lampColor = RED;
  Vector2 pointingTo;
  Gate *GateConnectedTo = nullptr;
  Output *OutputConnectedTo = nullptr;

  bool Value = false;
  bool ConnectedToGate = false;
  bool ConnectedToOutput = false;
  bool connDragging = false;

public:
  Point();

  virtual void Cycle();

  void DragToConnect();

  void SetPointingTo(Vector2 v) { this->pointingTo = v; }

  virtual void Draw();
  virtual void CheckMouse();
  virtual bool Solve();
  bool IsConnDragging();
  void ConnectThis(Gate *);
  void ConnectThis(Output *);
};

// Switch
class Switch : public Point {
protected:
  Color color = RED;

public:
  Switch() {
    this->position = GetMousePosition();
    this->radius = 15;
  }
  void SwitchValue();
  virtual void Draw();
  virtual void Cycle();
};

// Output
//
class Output : public Object {
protected:
  Color color = RED;
  float radius = 30;
  Point *Input = nullptr;
  bool value = false;
  std::string label = "False";

public:
  Output();
  virtual void Draw();
  virtual void CheckMouse();
  virtual void Cycle();
  void ConnectToThis(Point *);
  void Solve();
  Vector2 GetPosition() { return this->position; }
  bool IsConnectedTo() { return (this->Input != nullptr); }
  Point *GetInput() { return this->Input; }
  void SetPosition(Vector2 v) { this->position = v; }
  void SetLabel() {
    if (this->value) {
      this->label = "True";
      this->color = GREEN;
    } else {
      this->label = "False";
      this->color = RED;
    }
  };
};
// Gates

class Gate : public Point {
protected:
  bool a;
  bool b;

  bool aIsKnown = false;
  bool bIsKnown = false;

  bool aIsConnected = false;
  bool bIsConnected = false;

  std::string name;

  Point *Connectiona;
  Point *Connectionb;

public:
  Gate();
  virtual bool CalcGate() = 0;
  virtual bool Solve();
  void ConnectToThis(Point *);
  bool HasEmptyConn();
  void Forget();
  Vector2 GetPosition() { return this->position; }

  void Draw();
  void Cycle();
};
//
class AndGate : public Gate {
public:
  AndGate() { this->lable = "And "; }

  virtual bool CalcGate() { return this->a && this->b; }
};

class NandGate : public Gate {
public:
  NandGate() { this->lable = "Nand"; }

  virtual bool CalcGate() { return !(this->a && this->b); }
};

class OrGate : public Gate {
public:
  OrGate() { this->lable = "Or"; }

  virtual bool CalcGate() { return this->a || this->b; }
};

class NorGate : public Gate {
public:
  NorGate() { this->lable = "Nor"; }

  virtual bool CalcGate() { return !(this->a || this->b); }
};

class XorGate : public Gate {
public:
  XorGate() { this->lable = "Xor"; }

  virtual bool CalcGate() { return this->a != this->b; }
};

class XnorGate : public Gate {
public:
  XnorGate() { this->lable = "Xnor"; }

  virtual bool CalcGate() { return this->a == this->b; }
};
