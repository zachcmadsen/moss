#include <format>
#include <memory>

#include <doctest/doctest.h>
#include <simdjson.h>

#include "moss/cpu.h"
#include "moss/types.h"

using namespace simdjson;

namespace moss {
namespace {

static void Run(u16 opc) {
    auto cpu = std::make_unique<Cpu>();

    dom::parser parser;
    dom::array tests =
        parser.load(std::format("../../roms/processor_tests/{:02x}.json", opc));

    for (dom::object test : tests) {
        dom::object start = test["initial"];
        cpu->A(static_cast<u8>(u64(start["a"])));
        cpu->X(static_cast<u8>(u64(start["x"])));
        cpu->Y(static_cast<u8>(u64(start["y"])));
        cpu->S(static_cast<u8>(u64(start["s"])));
        cpu->Pc(static_cast<u16>(u64(start["pc"])));
        cpu->P(static_cast<u8>(u64(start["p"])));

        for (dom::array entry : start["ram"]) {
            auto it = entry.begin();
            auto addr = u64(*it);
            ++it;
            auto data = u64(*it);
            cpu->Write(static_cast<u16>(addr), static_cast<u8>(data));
        }

        cpu->Step();

        // TODO: Assert bus activity.
        dom::object end = test["final"];
        REQUIRE(cpu->A() == u64(end["a"]));
        REQUIRE(cpu->X() == u64(end["x"]));
        REQUIRE(cpu->Y() == u64(end["y"]));
        REQUIRE(cpu->S() == u64(end["s"]));
        REQUIRE(cpu->Pc() == u64(end["pc"]));
        REQUIRE(cpu->P() == u64(end["p"]));
        for (dom::array entry : end["ram"]) {
            auto it = entry.begin();
            auto addr = u64(*it);
            ++it;
            auto data = u64(*it);
            REQUIRE(cpu->Read(static_cast<u16>(addr)) == data);
        }
    }
}

// JAM, LXA, and ANE are commented out for now.
// clang-format off
TEST_CASE("00") { Run(0x00); }
TEST_CASE("01") { Run(0x01); }
// TEST_CASE("02") { Run(0x02); }
TEST_CASE("03") { Run(0x03); }
TEST_CASE("04") { Run(0x04); }
TEST_CASE("05") { Run(0x05); }
TEST_CASE("06") { Run(0x06); }
TEST_CASE("07") { Run(0x07); }
TEST_CASE("08") { Run(0x08); }
TEST_CASE("09") { Run(0x09); }
TEST_CASE("0A") { Run(0x0A); }
TEST_CASE("0B") { Run(0x0B); }
TEST_CASE("0C") { Run(0x0C); }
TEST_CASE("0D") { Run(0x0D); }
TEST_CASE("0E") { Run(0x0E); }
TEST_CASE("0F") { Run(0x0F); }
TEST_CASE("10") { Run(0x10); }
TEST_CASE("11") { Run(0x11); }
// TEST_CASE("12") { Run(0x12); }
TEST_CASE("13") { Run(0x13); }
TEST_CASE("14") { Run(0x14); }
TEST_CASE("15") { Run(0x15); }
TEST_CASE("16") { Run(0x16); }
TEST_CASE("17") { Run(0x17); }
TEST_CASE("18") { Run(0x18); }
TEST_CASE("19") { Run(0x19); }
TEST_CASE("1A") { Run(0x1A); }
TEST_CASE("1B") { Run(0x1B); }
TEST_CASE("1C") { Run(0x1C); }
TEST_CASE("1D") { Run(0x1D); }
TEST_CASE("1E") { Run(0x1E); }
TEST_CASE("1F") { Run(0x1F); }
TEST_CASE("20") { Run(0x20); }
TEST_CASE("21") { Run(0x21); }
// TEST_CASE("22") { Run(0x22); }
TEST_CASE("23") { Run(0x23); }
TEST_CASE("24") { Run(0x24); }
TEST_CASE("25") { Run(0x25); }
TEST_CASE("26") { Run(0x26); }
TEST_CASE("27") { Run(0x27); }
TEST_CASE("28") { Run(0x28); }
TEST_CASE("29") { Run(0x29); }
TEST_CASE("2A") { Run(0x2A); }
TEST_CASE("2B") { Run(0x2B); }
TEST_CASE("2C") { Run(0x2C); }
TEST_CASE("2D") { Run(0x2D); }
TEST_CASE("2E") { Run(0x2E); }
TEST_CASE("2F") { Run(0x2F); }
TEST_CASE("30") { Run(0x30); }
TEST_CASE("31") { Run(0x31); }
// TEST_CASE("32") { Run(0x32); }
TEST_CASE("33") { Run(0x33); }
TEST_CASE("34") { Run(0x34); }
TEST_CASE("35") { Run(0x35); }
TEST_CASE("36") { Run(0x36); }
TEST_CASE("37") { Run(0x37); }
TEST_CASE("38") { Run(0x38); }
TEST_CASE("39") { Run(0x39); }
TEST_CASE("3A") { Run(0x3A); }
TEST_CASE("3B") { Run(0x3B); }
TEST_CASE("3C") { Run(0x3C); }
TEST_CASE("3D") { Run(0x3D); }
TEST_CASE("3E") { Run(0x3E); }
TEST_CASE("3F") { Run(0x3F); }
TEST_CASE("40") { Run(0x40); }
TEST_CASE("41") { Run(0x41); }
// TEST_CASE("42") { Run(0x42); }
TEST_CASE("43") { Run(0x43); }
TEST_CASE("44") { Run(0x44); }
TEST_CASE("45") { Run(0x45); }
TEST_CASE("46") { Run(0x46); }
TEST_CASE("47") { Run(0x47); }
TEST_CASE("48") { Run(0x48); }
TEST_CASE("49") { Run(0x49); }
TEST_CASE("4A") { Run(0x4A); }
TEST_CASE("4B") { Run(0x4B); }
TEST_CASE("4C") { Run(0x4C); }
TEST_CASE("4D") { Run(0x4D); }
TEST_CASE("4E") { Run(0x4E); }
TEST_CASE("4F") { Run(0x4F); }
TEST_CASE("50") { Run(0x50); }
TEST_CASE("51") { Run(0x51); }
// TEST_CASE("52") { Run(0x52); }
TEST_CASE("53") { Run(0x53); }
TEST_CASE("54") { Run(0x54); }
TEST_CASE("55") { Run(0x55); }
TEST_CASE("56") { Run(0x56); }
TEST_CASE("57") { Run(0x57); }
TEST_CASE("58") { Run(0x58); }
TEST_CASE("59") { Run(0x59); }
TEST_CASE("5A") { Run(0x5A); }
TEST_CASE("5B") { Run(0x5B); }
TEST_CASE("5C") { Run(0x5C); }
TEST_CASE("5D") { Run(0x5D); }
TEST_CASE("5E") { Run(0x5E); }
TEST_CASE("5F") { Run(0x5F); }
TEST_CASE("60") { Run(0x60); }
TEST_CASE("61") { Run(0x61); }
// TEST_CASE("62") { Run(0x62); }
TEST_CASE("63") { Run(0x63); }
TEST_CASE("64") { Run(0x64); }
TEST_CASE("65") { Run(0x65); }
TEST_CASE("66") { Run(0x66); }
TEST_CASE("67") { Run(0x67); }
TEST_CASE("68") { Run(0x68); }
TEST_CASE("69") { Run(0x69); }
TEST_CASE("6A") { Run(0x6A); }
TEST_CASE("6B") { Run(0x6B); }
TEST_CASE("6C") { Run(0x6C); }
TEST_CASE("6D") { Run(0x6D); }
TEST_CASE("6E") { Run(0x6E); }
TEST_CASE("6F") { Run(0x6F); }
TEST_CASE("70") { Run(0x70); }
TEST_CASE("71") { Run(0x71); }
// TEST_CASE("72") { Run(0x72); }
TEST_CASE("73") { Run(0x73); }
TEST_CASE("74") { Run(0x74); }
TEST_CASE("75") { Run(0x75); }
TEST_CASE("76") { Run(0x76); }
TEST_CASE("77") { Run(0x77); }
TEST_CASE("78") { Run(0x78); }
TEST_CASE("79") { Run(0x79); }
TEST_CASE("7A") { Run(0x7A); }
TEST_CASE("7B") { Run(0x7B); }
TEST_CASE("7C") { Run(0x7C); }
TEST_CASE("7D") { Run(0x7D); }
TEST_CASE("7E") { Run(0x7E); }
TEST_CASE("7F") { Run(0x7F); }
TEST_CASE("80") { Run(0x80); }
TEST_CASE("81") { Run(0x81); }
TEST_CASE("82") { Run(0x82); }
TEST_CASE("83") { Run(0x83); }
TEST_CASE("84") { Run(0x84); }
TEST_CASE("85") { Run(0x85); }
TEST_CASE("86") { Run(0x86); }
TEST_CASE("87") { Run(0x87); }
TEST_CASE("88") { Run(0x88); }
TEST_CASE("89") { Run(0x89); }
TEST_CASE("8A") { Run(0x8A); }
// TEST_CASE("8B") { Run(0x8B); }
TEST_CASE("8C") { Run(0x8C); }
TEST_CASE("8D") { Run(0x8D); }
TEST_CASE("8E") { Run(0x8E); }
TEST_CASE("8F") { Run(0x8F); }
TEST_CASE("90") { Run(0x90); }
TEST_CASE("91") { Run(0x91); }
// TEST_CASE("92") { Run(0x92); }
TEST_CASE("93") { Run(0x93); }
TEST_CASE("94") { Run(0x94); }
TEST_CASE("95") { Run(0x95); }
TEST_CASE("96") { Run(0x96); }
TEST_CASE("97") { Run(0x97); }
TEST_CASE("98") { Run(0x98); }
TEST_CASE("99") { Run(0x99); }
TEST_CASE("9A") { Run(0x9A); }
TEST_CASE("9B") { Run(0x9B); }
TEST_CASE("9C") { Run(0x9C); }
TEST_CASE("9D") { Run(0x9D); }
TEST_CASE("9E") { Run(0x9E); }
TEST_CASE("9F") { Run(0x9F); }
TEST_CASE("A0") { Run(0xA0); }
TEST_CASE("A1") { Run(0xA1); }
TEST_CASE("A2") { Run(0xA2); }
TEST_CASE("A3") { Run(0xA3); }
TEST_CASE("A4") { Run(0xA4); }
TEST_CASE("A5") { Run(0xA5); }
TEST_CASE("A6") { Run(0xA6); }
TEST_CASE("A7") { Run(0xA7); }
TEST_CASE("A8") { Run(0xA8); }
TEST_CASE("A9") { Run(0xA9); }
TEST_CASE("AA") { Run(0xAA); }
// TEST_CASE("AB") { Run(0xAB); }
TEST_CASE("AC") { Run(0xAC); }
TEST_CASE("AD") { Run(0xAD); }
TEST_CASE("AE") { Run(0xAE); }
TEST_CASE("AF") { Run(0xAF); }
TEST_CASE("B0") { Run(0xB0); }
TEST_CASE("B1") { Run(0xB1); }
// TEST_CASE("B2") { Run(0xB2); }
TEST_CASE("B3") { Run(0xB3); }
TEST_CASE("B4") { Run(0xB4); }
TEST_CASE("B5") { Run(0xB5); }
TEST_CASE("B6") { Run(0xB6); }
TEST_CASE("B7") { Run(0xB7); }
TEST_CASE("B8") { Run(0xB8); }
TEST_CASE("B9") { Run(0xB9); }
TEST_CASE("BA") { Run(0xBA); }
TEST_CASE("BB") { Run(0xBB); }
TEST_CASE("BC") { Run(0xBC); }
TEST_CASE("BD") { Run(0xBD); }
TEST_CASE("BE") { Run(0xBE); }
TEST_CASE("BF") { Run(0xBF); }
TEST_CASE("C0") { Run(0xC0); }
TEST_CASE("C1") { Run(0xC1); }
TEST_CASE("C2") { Run(0xC2); }
TEST_CASE("C3") { Run(0xC3); }
TEST_CASE("C4") { Run(0xC4); }
TEST_CASE("C5") { Run(0xC5); }
TEST_CASE("C6") { Run(0xC6); }
TEST_CASE("C7") { Run(0xC7); }
TEST_CASE("C8") { Run(0xC8); }
TEST_CASE("C9") { Run(0xC9); }
TEST_CASE("CA") { Run(0xCA); }
TEST_CASE("CB") { Run(0xCB); }
TEST_CASE("CC") { Run(0xCC); }
TEST_CASE("CD") { Run(0xCD); }
TEST_CASE("CE") { Run(0xCE); }
TEST_CASE("CF") { Run(0xCF); }
TEST_CASE("D0") { Run(0xD0); }
TEST_CASE("D1") { Run(0xD1); }
// TEST_CASE("D2") { Run(0xD2); }
TEST_CASE("D3") { Run(0xD3); }
TEST_CASE("D4") { Run(0xD4); }
TEST_CASE("D5") { Run(0xD5); }
TEST_CASE("D6") { Run(0xD6); }
TEST_CASE("D7") { Run(0xD7); }
TEST_CASE("D8") { Run(0xD8); }
TEST_CASE("D9") { Run(0xD9); }
TEST_CASE("DA") { Run(0xDA); }
TEST_CASE("DB") { Run(0xDB); }
TEST_CASE("DC") { Run(0xDC); }
TEST_CASE("DD") { Run(0xDD); }
TEST_CASE("DE") { Run(0xDE); }
TEST_CASE("DF") { Run(0xDF); }
TEST_CASE("E0") { Run(0xE0); }
TEST_CASE("E1") { Run(0xE1); }
TEST_CASE("E2") { Run(0xE2); }
TEST_CASE("E3") { Run(0xE3); }
TEST_CASE("E4") { Run(0xE4); }
TEST_CASE("E5") { Run(0xE5); }
TEST_CASE("E6") { Run(0xE6); }
TEST_CASE("E7") { Run(0xE7); }
TEST_CASE("E8") { Run(0xE8); }
TEST_CASE("E9") { Run(0xE9); }
TEST_CASE("EA") { Run(0xEA); }
TEST_CASE("EB") { Run(0xEB); }
TEST_CASE("EC") { Run(0xEC); }
TEST_CASE("ED") { Run(0xED); }
TEST_CASE("EE") { Run(0xEE); }
TEST_CASE("EF") { Run(0xEF); }
TEST_CASE("F0") { Run(0xF0); }
TEST_CASE("F1") { Run(0xF1); }
// TEST_CASE("F2") { Run(0xF2); }
TEST_CASE("F3") { Run(0xF3); }
TEST_CASE("F4") { Run(0xF4); }
TEST_CASE("F5") { Run(0xF5); }
TEST_CASE("F6") { Run(0xF6); }
TEST_CASE("F7") { Run(0xF7); }
TEST_CASE("F8") { Run(0xF8); }
TEST_CASE("F9") { Run(0xF9); }
TEST_CASE("FA") { Run(0xFA); }
TEST_CASE("FB") { Run(0xFB); }
TEST_CASE("FC") { Run(0xFC); }
TEST_CASE("FD") { Run(0xFD); }
TEST_CASE("FE") { Run(0xFE); }
TEST_CASE("FF") { Run(0xFF); }
// clang-format on

}
}