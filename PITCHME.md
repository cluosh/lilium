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

---

### Bytecode

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

___

### Generated Code

---

### Performance Comparison

---

### Performance Comparison

---

### Demo
