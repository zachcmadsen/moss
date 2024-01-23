fn main() {
    cc::Build::new()
        .compiler("clang")
        .std("c2x")
        .include("../include")
        .file("../src/cpu.c")
        .compile("sys");

    println!("cargo:rerun-if-changed=../include");
    println!("cargo:rerun-if-changed=../src/cpu.c");
}
