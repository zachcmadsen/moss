#include <expected>
#include <format>
#include <memory>
#include <span>
#include <string>

#include "moss/cpu.h"
#include "moss/moss.h"
#include "moss/types.h"

namespace moss {

/// Returns a word from `rom` at `offset`.
static u16 ReadWord(std::span<u8> rom, u16 offset) {
    auto low = rom[offset];
    auto high = rom[offset + 1];
    return static_cast<u16>(low | high << 8);
}

std::expected<void, std::string> Run(std::span<u8> rom) {
    if (rom.size() < 4) {
        return std::unexpected("incomplete header");
    }

    auto load_addr = ReadWord(rom, 0x0000);
    auto run_addr = ReadWord(rom, 0x0002);

    auto cpu = std::make_unique<Cpu>();
    if (!cpu->Load(std::span(rom).subspan(4), load_addr)) {
        return std::unexpected(
            std::format("image is too large to fit at 0x{:04X}", load_addr));
    }
    cpu->Pc(run_addr);

    // TODO: Move the check into the JMP opcode handler?
    u16 prev_pc;
    do {
        prev_pc = cpu->Pc();
        cpu->Step();
    } while (prev_pc != cpu->Pc());

    return {};
}

}