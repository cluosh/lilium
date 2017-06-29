# Lilium

A LISP-Like Register Machine

---

### Development History

* Starting with last years course
* First version: Flex/Bison/C++ -> limited prototype
* Second version: Boost Spirit/C++ -> C++ template hell
* Final version: Rust

---

### Architecture

* Rust (nightly)
* lalrpop as LR(1) parser generator
* LISP-like language
* Translated to bytecode
* Token-threaded dispatch (inline assembly)

---

### Features

* Basic arithmetics: ```(+ 1 2), (* 1 2), (/ 1 2), (- 1 2)```
* Basic logic: ```(& 0 1), (| 0 1), (~ 0)```
* Basic comparison: ```(== 1 1), (> 1 0), (< 1 0), ...```
* Function definition: ```(def fun (a b) (+ a b))```
* Variable assignment: ```(let ((a 3) (b (+ 2 3))) (/ a b))```
* Conditionals: ```(if (> a 3) (funca a) (funcb b))```

---

### Bytecode

* 4-byte instructions
* 1-byte opcode
* Usually one byte target register
* Rest for operands

---

### LALRPOP Grammar

---

### Token-Threading in Rust

* Rust performs bound checks on...
   * array access
   * *match* statements
   * basically everthing
* Can we get computed goto C performance?

---

### Unsafe And Inline Assembly

* Array access without bound check -> use **unsafe**
* Token-Threading -> use **asm**
* Comparison of dispatchers in Rust:
  https://pliniker.github.io/post/dispatchers/

---

### Token-Threading in Rust

---

### Dispatch Loop

---

### Generated Code

---

### Performance Comparison

---

### Performance Comparison

---

### Demo
