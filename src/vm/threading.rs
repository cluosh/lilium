macro_rules! label {
    ($name:expr) => {
        unsafe {
            asm!(concat!($name, ":")
                 :
                 :
                 :
                 : "volatile");
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! label_addr {
    ($name:expr) => {
        {
            let addr: usize;
            unsafe {
                asm!(concat!("leaq ", $name, "(%rip), $0")
                     : "=&r"(addr)
                     :
                     :
                     : "volatile");
            }
            addr
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! dispatch {
    ($vm:expr, $pc:expr, $jumptable:expr) => {
        unsafe {
            let opcode = $vm.code.get_unchecked($pc).opcode as usize;
            let addr = *$jumptable.get_unchecked(opcode);
            asm!("jmpq *$0"
                 :
                 : "r"(addr), "{rdx}"($pc)
                 :
                 : "volatile");
        }
    }
}

#[cfg(target_arch = "x86_64")]
macro_rules! do_and_dispatch {
    ($vm:expr, $jmptable:expr, $name:expr, $pc:expr, $action:expr) => {
        unsafe {
            asm!(concat!($name, ":")
                 : "={rdx}"($pc)
                 :
                 :
                 : "volatile");
        }

        {
            $action
        }

        dispatch!($vm, $pc, $jmptable);
    }
}
