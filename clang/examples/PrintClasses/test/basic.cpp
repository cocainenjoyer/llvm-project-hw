// RUN: %clang_cc1 -std=c++17 -fsyntax-only -load PrintClasses.dylib -plugin
// print-classes %s 2>&1 | FileCheck %s

struct Human {
  unsigned age;
  unsigned height;
  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : Human {
  unsigned salary;
  void sleep() override {}
  void eat() override {}
  void work() {}
};

// CHECK: Human
// CHECK: |_Fields
// CHECK: | |_ age (unsigned int
// CHECK: | |_ height (unsigned int
// CHECK: |_Methods
// CHECK: | |_ sleep (void()
// CHECK: | |_ eat (void()
// CHECK: Engineer -> Human
// CHECK: |_Fields
// CHECK: | |_ salary (unsigned int
// CHECK: |_Methods
// CHECK: | |_ sleep (void()
// CHECK: | |_ eat (void()
// CHECK: | |_ work (void()
