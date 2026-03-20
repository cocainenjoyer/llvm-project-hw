// RUN: %clang_cc1 -std=c++17 -fsyntax-only -load PrintClasses.dylib -plugin
// print-classes %s 2>&1 | FileCheck %s

struct Animal {
  virtual void move() = 0;
  virtual void eat() {}
};

struct Dog : Animal {
  void move() override {}
  void eat() override {}
  void bark() {}
};

// CHECK: Animal
// CHECK: |_Methods
// CHECK: move (void()|public|virtual|pure)
// CHECK: eat (void()|public|virtual)

// CHECK: Dog -> Animal
// CHECK: |_Methods
// CHECK: move (void()|public|override)
// CHECK: eat (void()|public|override)
// CHECK: bark (void()|public)
