// RUN: %clang_cc1 -std=c++17 -fsyntax-only -load PrintClasses.dylib -plugin
// print-classes %s 2>&1 | FileCheck %s

struct Secret {
private:
  int secret_code;

protected:
  int protected_data;

public:
  int public_data;
  void show() {}
};

// CHECK: Secret
// CHECK: |_Fields
// CHECK: secret_code (int|private)
// CHECK: protected_data (int|protected)
// CHECK: public_data (int|public)
// CHECK: |_Methods
// CHECK: show (void()|public)
