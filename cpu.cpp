#include <cstdint>
#include <memory>

#include <doctest.h>
#include <simdjson.h>

#include "cpu.h"

using namespace simdjson;

namespace Moss {
void
Cpu::Step()
{
    auto opc = ram[pc++];
    switch (opc) {
        case 0xA9:
            Lda(Imm());
            break;
    }
}
}

TEST_CASE("ProcessorTests")
{
    auto cpu = std::make_unique<Moss::Cpu>();

    ondemand::parser parser;
    auto json = padded_string::load("processor_tests/a9.json");

    for (ondemand::object test : parser.iterate(json)) {
        ondemand::object start = test["initial"];
        cpu->Pc(static_cast<std::uint16_t>(start["pc"].get_int64()));
        cpu->S(static_cast<std::uint8_t>(start["s"].get_int64()));
        cpu->A(static_cast<std::uint8_t>(start["a"].get_int64()));
        cpu->X(static_cast<std::uint8_t>(start["x"].get_int64()));
        cpu->Y(static_cast<std::uint8_t>(start["y"].get_int64()));
        cpu->P(static_cast<std::uint8_t>(start["p"].get_int64()));
        for (ondemand::array entry : start["ram"].get_array()) {
            auto it = entry.begin();
            uint16_t addr = static_cast<std::uint16_t>((*it).get_int64());
            ++it;
            uint8_t data = static_cast<std::uint8_t>((*it).get_int64());
            cpu->Write(addr, data);
        }

        cpu->Step();

        ondemand::object end = test["final"];
        CHECK(cpu->Pc() == end["pc"].get_int64());
        CHECK(cpu->S() == end["s"].get_int64());
        CHECK(cpu->A() == end["a"].get_int64());
        CHECK(cpu->X() == end["x"].get_int64());
        CHECK(cpu->Y() == end["y"].get_int64());
        CHECK(cpu->P() == end["p"].get_int64());
        for (ondemand::array entry : end["ram"].get_array()) {
            auto it = entry.begin();
            uint16_t addr = static_cast<std::uint16_t>((*it).get_int64());
            ++it;
            uint8_t data = static_cast<std::uint8_t>((*it).get_int64());
            CHECK(cpu->Read(addr) == data);
        }
    }
}
