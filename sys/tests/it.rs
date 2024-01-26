use std::fs;

use serde::Deserialize;

macro_rules! processor_test {
    ($name:ident, $opc:expr) => {
        #[test]
        fn $name() {
            run($opc);
        }
    };
}

#[derive(Deserialize)]
struct Test {
    #[serde(rename = "initial")]
    start: CpuState,
    #[serde(rename = "final")]
    end: CpuState,
}

#[derive(Deserialize)]
struct CpuState {
    pc: u16,
    s: u8,
    a: u8,
    x: u8,
    y: u8,
    p: u8,
    ram: Vec<(u16, u8)>,
}

fn run(opc: u8) {
    let contents =
        fs::read(format!("processor_tests/{:02x}.json", opc)).unwrap();
    let tests: Vec<Test> = serde_json::from_slice(&contents).unwrap();

    let cpu = unsafe { sys::cpu_new() };

    for test in tests {
        unsafe {
            sys::cpu_set_pc(cpu, test.start.pc);
            sys::cpu_set_a(cpu, test.start.a);
            sys::cpu_set_x(cpu, test.start.x);
            sys::cpu_set_y(cpu, test.start.y);
            sys::cpu_set_s(cpu, test.start.s);
            sys::cpu_set_p(cpu, test.start.p);
            for (addr, data) in test.start.ram {
                sys::cpu_write(cpu, addr, data);
            }

            sys::cpu_step(cpu);

            assert_eq!(sys::cpu_pc(cpu), test.end.pc);
            assert_eq!(sys::cpu_a(cpu), test.end.a);
            assert_eq!(sys::cpu_x(cpu), test.end.x);
            assert_eq!(sys::cpu_y(cpu), test.end.y);
            assert_eq!(sys::cpu_s(cpu), test.end.s);
            assert_eq!(sys::cpu_p(cpu), test.end.p);
            for (addr, data) in test.end.ram {
                assert_eq!(sys::cpu_read(cpu, addr), data);
            }
        }
    }

    unsafe { sys::cpu_free(cpu) };
}

processor_test!(opc_a9, 0xA9);
processor_test!(opc_ad, 0xAD);
