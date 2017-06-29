# Lilium

###  A LISP-Like Register Machine

Michael Pucher, 1425215

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

* Basic arithmetics: ```(+ 1 2), (* 1 2)```
* Basic logic: ```(& 0 1), (| 0 1), (~ 0)```
* Basic comparison: ```(== 1 1), (> 1 0)```
* Function definition: ```(def fun (a b) (+ a b))```
* Variable assignment: ```(let ((a 3)) (+ a 4))```
* Conditionals: ```(if (> a 3) (fa a) (fb b))```
* Read/print integers: ```(read), (write 123)```

---

### Bytecode

* 4-byte instructions
* 1-byte opcode
* Usually one byte target register
* Rest for operands

---

### LALRPOP Grammar

```rust
expression: Expression = {
    "(let" "(" <a:assignments> ")" <b:expressions> ")" => {
        Expression::VariableAssignment(a,b)
    },
    "(" <o:op_binary> <l:expression> <r:expression> ")" => {
        Expression::BinaryOp(o, Box::new(l), Box::new(r))
    },
    ...
};

identifier: String = {
    r"[a-zA-Z]+" => <>.to_string(),
};
```

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

```rust
#[cfg(target_arch = "x86_64")]
macro_rules! dispatch {
    ($vm:expr, $pc:expr, $jumptable:expr) => {
        unsafe {
            let opcode = $vm.code
                .get_unchecked($pc).opcode;
            let addr = *$jumptable
                .get_unchecked(opcode as usize);
            asm!("jmpq *$0"
                 :
                 : "r"(addr), "{rdx}"($pc)
                 :
                 : "volatile");
        }
    }
}
```

---

### Dispatch Loop

```rust
dispatch!(&thread, pc, ops);

do_and_dispatch!(&thread, ops, "op_ld", pc, {
    pc = op_ld(thread, pc);
});

do_and_dispatch!(&thread, ops, "op_ldb", pc, {
    pc = op_ldb(thread, pc);
});

do_and_dispatch!(&thread, ops, "op_ldr", pc, {
    pc = op_ldr(thread, pc);
});
```

---

### Generated Code

```asm
op_add:
	movzbl	2(%rax,%rdx,4), %edi
	addq	%rsi, %rdi
	movzbl	3(%rax,%rdx,4), %ebx
	addq	%rsi, %rbx
	movzbl	1(%rax,%rdx,4), %eax
	addq	%rsi, %rax
	movq	(%rcx,%rbx,8), %rsi
	addq	(%rcx,%rdi,8), %rsi
	movq	%rsi, (%rcx,%rax,8)
	movq	32(%r15), %rax
	movzbl	4(%rax,%rdx,4), %eax
	incq	%rdx
	movq	112(%rsp,%rax,8), %rax
	jmpq	*%rax
```
