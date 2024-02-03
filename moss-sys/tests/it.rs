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
struct Test<'a> {
    #[serde(rename = "initial")]
    start: CpuState,
    #[serde(rename = "final")]
    end: CpuState,
    #[serde(borrow)]
    cycles: Vec<(u16, u8, &'a str)>,
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

    let cpu = unsafe { moss_sys::cpu_new() };

    for test in tests {
        unsafe {
            moss_sys::cpu_set_pc(cpu, test.start.pc);
            moss_sys::cpu_set_a(cpu, test.start.a);
            moss_sys::cpu_set_x(cpu, test.start.x);
            moss_sys::cpu_set_y(cpu, test.start.y);
            moss_sys::cpu_set_s(cpu, test.start.s);
            moss_sys::cpu_set_p(cpu, test.start.p);
            for (addr, data) in test.start.ram {
                moss_sys::cpu_write(cpu, addr, data);
            }
            moss_sys::cpu_reset_cycles(cpu);

            moss_sys::cpu_step(cpu);

            assert_eq!(moss_sys::cpu_pc(cpu), test.end.pc);
            assert_eq!(moss_sys::cpu_a(cpu), test.end.a);
            assert_eq!(moss_sys::cpu_x(cpu), test.end.x);
            assert_eq!(moss_sys::cpu_y(cpu), test.end.y);
            assert_eq!(moss_sys::cpu_s(cpu), test.end.s);
            assert_eq!(moss_sys::cpu_p(cpu), test.end.p);
            for (addr, data) in test.end.ram {
                assert_eq!(moss_sys::cpu_read(cpu, addr), data);
            }
            assert_eq!(moss_sys::cpu_cycles(cpu), test.cycles.len() as u8);
            for (cycle, &(addr, data, _)) in test.cycles.iter().enumerate() {
                assert_eq!(moss_sys::cpu_addr_bus(cpu, cycle), addr);
                assert_eq!(moss_sys::cpu_data_bus(cpu, cycle), data);
            }
        }
    }

    unsafe { moss_sys::cpu_free(cpu) };
}

processor_test!(opc_a5, 0xA5);
processor_test!(opc_a9, 0xA9);
processor_test!(opc_ad, 0xAD);
processor_test!(opc_b5, 0xB5);
processor_test!(opc_bd, 0xBD);
