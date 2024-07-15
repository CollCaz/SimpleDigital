#pragma once
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

  bool DragMove();
  void OffsetPosition(Vector2 offset);
};

class Gate;
class Output;
class Point : public Object {
protected:
  std::string lable;
  Vector2 size = Vector2{10, 10};
  Vector2 ConnectionLoc;
  Vector2 pointingTo;
  Gate *GateConnectedTo = nullptr;
  Output *OutputConnectTo = nullptr;

  bool Value;
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
  bool IsMouseOnThis();
  bool IsConnDragging();
  void ConnectThis(Gate *);
  void ConnectThis(Output *);
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
  Vector2 GetPosition() { return this->position; }
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
  bool aIsConnected = false;
  bool bIsConnected = false;

  std::string name;

  Point *Connectiona;
  Point *Connectionb;

  Vector2 ConnectionPointa;
  Vector2 ConnectionPointb;

public:
  Gate();
  virtual bool Solve() = 0;
  void ConnectToThis(Point *);
  bool HasEmptyConn();

  void Draw();
  void Cycle();
};
//
class AndGate : public Gate {
public:
  virtual bool Solve();
  AndGate() { this->lable = "And "; }
};

// class NandGate : public Gate {
// public:
//   NandGate() { this->lable = "Nand"; }
//
//   bool Solve() { return !(this->a && this->b); }
// };
//
// class OrGate : public Gate {
// public:
//   OrGate() { this->lable = "Or  "; }
//
//   bool Solve() { return this->a || this->b; }
// };
//
// class NorGate : public Gate {
// public:
//   NorGate() { this->lable = "Nor "; }
//
//   bool Solve() { return !(this->a || this->b); }
// };
//
// class XorGate : public Gate {
// public:
//   XorGate() { this->lable = "Xor "; }
//
//   bool Solve() { return this->a != this->b; }
// };
//
// class XnorGate : public Gate {
// public:
//   XnorGate() { this->lable = "Xnor"; }
//
//   bool Solve() { return this->a == this->b; }
// };
//
