/// Base code from http://www.jonathanturner.org/2015/12/building-a-simple-jit-in-rust.html
extern crate libc;
use std::mem;
use std::ops::{Index, IndexMut};

// Import memset
extern "C" {
    fn memset(s: *mut libc::c_void, c: libc::uint32_t, n: libc::size_t) -> *mut libc::c_void;
}

const PAGE_SIZE: usize = 4096;

struct JitMemory {
    contents: *mut u8,
    size: usize,
}

impl Index<usize> for JitMemory {
    type Output = u8;

    fn index(&self, _index: usize) -> &u8 {
        unsafe { &*self.contents.offset(_index as isize) }
    }
}

impl IndexMut<usize> for JitMemory {
    fn index_mut(&mut self, _index: usize) -> &mut u8 {
        unsafe { &mut *self.contents.offset(_index as isize) }
    }
}

impl Drop for JitMemory {
    fn drop(&mut self) {
        unsafe {
            // Free JIT memory
            libc::free(mem::transmute(self.contents));
        }
    }
}

impl JitMemory {
    fn new(num_pages: usize) -> JitMemory {
        let contents: *mut u8;
        let size = num_pages * PAGE_SIZE;

        unsafe {
            // Allocate aligned memory
            let mut _contents: *mut libc::c_void = mem::uninitialized();
            libc::posix_memalign(&mut _contents, PAGE_SIZE, size);

            // Enable code execution for the memory
            libc::mprotect(_contents, size, libc::PROT_READ | libc::PROT_WRITE);

            // Fill memory with return instructions
            memset(_contents, 0xc3, size);
            contents = mem::transmute(_contents);
        }

        JitMemory {
            contents: contents,
            size: size,
        }
    }

    fn drop_privileges(&mut self) {
        unsafe {
            // Drop write privileges of the JIT memory
            let _contents: *mut libc::c_void = mem::transmute(self.contents);
            libc::mprotect(_contents, self.size, libc::PROT_EXEC | libc::PROT_READ);
        }
    }
}

fn main() {
    let mut jit: JitMemory = JitMemory::new(1);
    jit[0] = 0x48;
    jit[1] = 0x89;
    jit[2] = 0xf8;
    jit.drop_privileges();

    // Execute jitted function
    let fun: fn(arg: i64) -> i64 = unsafe { mem::transmute(jit.contents) };
    println!("{}", fun(5));
}
