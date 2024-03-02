#include "cpu.h"

#include <gtest/gtest.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weverything"
#include <simdjson.h>
#pragma GCC diagnostic pop

#include <memory>

namespace {

void RunProcessorTest(std::uint16_t opc) {
    auto cpu = std::make_unique<Moss::Cpu>();

    auto json = simdjson::padded_string::load(
        std::format("../../processor_tests/{:02x}.json", opc));
    simdjson::ondemand::parser parser;

    for (simdjson::ondemand::object test : parser.iterate(json)) {
        auto start = test["initial"];
        cpu->Pc(static_cast<std::uint16_t>(start["pc"].get_int64()));
        cpu->S(static_cast<std::uint8_t>(start["s"].get_int64()));
        cpu->A(static_cast<std::uint8_t>(start["a"].get_int64()));
        cpu->X(static_cast<std::uint8_t>(start["x"].get_int64()));
        cpu->Y(static_cast<std::uint8_t>(start["y"].get_int64()));
        cpu->P(static_cast<std::uint8_t>(start["p"].get_int64()));
        for (simdjson::ondemand::array entry : start["ram"].get_array()) {
            auto it = entry.begin();
            uint16_t addr = static_cast<std::uint16_t>((*it).get_int64());
            ++it;
            uint8_t data = static_cast<std::uint8_t>((*it).get_int64());
            cpu->Write(addr, data);
        }

        cpu->Step();

        simdjson::ondemand::object end = test["final"];
        ASSERT_TRUE(cpu->Pc() == end["pc"].get_int64());
        ASSERT_TRUE(cpu->S() == end["s"].get_int64());
        ASSERT_TRUE(cpu->A() == end["a"].get_int64());
        ASSERT_TRUE(cpu->X() == end["x"].get_int64());
        ASSERT_TRUE(cpu->Y() == end["y"].get_int64());
        ASSERT_TRUE(cpu->P() == end["p"].get_int64());
        for (simdjson::ondemand::array entry : end["ram"].get_array()) {
            auto it = entry.begin();
            uint16_t addr = static_cast<std::uint16_t>((*it).get_int64());
            ++it;
            uint8_t data = static_cast<std::uint8_t>((*it).get_int64());
            ASSERT_TRUE(cpu->Read(addr) == data);
        }
    }
}

// clang-format off
TEST(ProcessorTests, 00) { RunProcessorTest(0x00); };
TEST(ProcessorTests, 01) { RunProcessorTest(0x01); };
TEST(ProcessorTests, 03) { RunProcessorTest(0x03); };
TEST(ProcessorTests, 04) { RunProcessorTest(0x04); };
TEST(ProcessorTests, 05) { RunProcessorTest(0x05); };
TEST(ProcessorTests, 06) { RunProcessorTest(0x06); };
TEST(ProcessorTests, 07) { RunProcessorTest(0x07); };
TEST(ProcessorTests, 08) { RunProcessorTest(0x08); };
TEST(ProcessorTests, 09) { RunProcessorTest(0x09); };
TEST(ProcessorTests, 0A) { RunProcessorTest(0x0A); };
TEST(ProcessorTests, 0B) { RunProcessorTest(0x0B); };
TEST(ProcessorTests, 0C) { RunProcessorTest(0x0C); };
TEST(ProcessorTests, 0D) { RunProcessorTest(0x0D); };
TEST(ProcessorTests, 0E) { RunProcessorTest(0x0E); };
TEST(ProcessorTests, 0F) { RunProcessorTest(0x0F); };
TEST(ProcessorTests, 10) { RunProcessorTest(0x10); };
TEST(ProcessorTests, 11) { RunProcessorTest(0x11); };
TEST(ProcessorTests, 13) { RunProcessorTest(0x13); };
TEST(ProcessorTests, 14) { RunProcessorTest(0x14); };
TEST(ProcessorTests, 15) { RunProcessorTest(0x15); };
TEST(ProcessorTests, 16) { RunProcessorTest(0x16); };
TEST(ProcessorTests, 17) { RunProcessorTest(0x17); };
TEST(ProcessorTests, 18) { RunProcessorTest(0x18); };
TEST(ProcessorTests, 19) { RunProcessorTest(0x19); };
TEST(ProcessorTests, 1A) { RunProcessorTest(0x1A); };
TEST(ProcessorTests, 1B) { RunProcessorTest(0x1B); };
TEST(ProcessorTests, 1C) { RunProcessorTest(0x1C); };
TEST(ProcessorTests, 1D) { RunProcessorTest(0x1D); };
TEST(ProcessorTests, 1E) { RunProcessorTest(0x1E); };
TEST(ProcessorTests, 1F) { RunProcessorTest(0x1F); };
TEST(ProcessorTests, 20) { RunProcessorTest(0x20); };
TEST(ProcessorTests, 21) { RunProcessorTest(0x21); };
TEST(ProcessorTests, 23) { RunProcessorTest(0x23); };
TEST(ProcessorTests, 24) { RunProcessorTest(0x24); };
TEST(ProcessorTests, 25) { RunProcessorTest(0x25); };
TEST(ProcessorTests, 26) { RunProcessorTest(0x26); };
TEST(ProcessorTests, 27) { RunProcessorTest(0x27); };
TEST(ProcessorTests, 28) { RunProcessorTest(0x28); };
TEST(ProcessorTests, 29) { RunProcessorTest(0x29); };
TEST(ProcessorTests, 2A) { RunProcessorTest(0x2A); };
TEST(ProcessorTests, 2B) { RunProcessorTest(0x2B); };
TEST(ProcessorTests, 2C) { RunProcessorTest(0x2C); };
TEST(ProcessorTests, 2D) { RunProcessorTest(0x2D); };
TEST(ProcessorTests, 2E) { RunProcessorTest(0x2E); };
TEST(ProcessorTests, 2F) { RunProcessorTest(0x2F); };
TEST(ProcessorTests, 30) { RunProcessorTest(0x30); };
TEST(ProcessorTests, 31) { RunProcessorTest(0x31); };
TEST(ProcessorTests, 33) { RunProcessorTest(0x33); };
TEST(ProcessorTests, 34) { RunProcessorTest(0x34); };
TEST(ProcessorTests, 35) { RunProcessorTest(0x35); };
TEST(ProcessorTests, 36) { RunProcessorTest(0x36); };
TEST(ProcessorTests, 37) { RunProcessorTest(0x37); };
TEST(ProcessorTests, 38) { RunProcessorTest(0x38); };
TEST(ProcessorTests, 39) { RunProcessorTest(0x39); };
TEST(ProcessorTests, 3A) { RunProcessorTest(0x3A); };
TEST(ProcessorTests, 3B) { RunProcessorTest(0x3B); };
TEST(ProcessorTests, 3C) { RunProcessorTest(0x3C); };
TEST(ProcessorTests, 3D) { RunProcessorTest(0x3D); };
TEST(ProcessorTests, 3E) { RunProcessorTest(0x3E); };
TEST(ProcessorTests, 3F) { RunProcessorTest(0x3F); };
TEST(ProcessorTests, 40) { RunProcessorTest(0x40); };
TEST(ProcessorTests, 41) { RunProcessorTest(0x41); };
TEST(ProcessorTests, 43) { RunProcessorTest(0x43); };
TEST(ProcessorTests, 44) { RunProcessorTest(0x44); };
TEST(ProcessorTests, 45) { RunProcessorTest(0x45); };
TEST(ProcessorTests, 46) { RunProcessorTest(0x46); };
TEST(ProcessorTests, 47) { RunProcessorTest(0x47); };
TEST(ProcessorTests, 48) { RunProcessorTest(0x48); };
TEST(ProcessorTests, 49) { RunProcessorTest(0x49); };
TEST(ProcessorTests, 4A) { RunProcessorTest(0x4A); };
TEST(ProcessorTests, 4B) { RunProcessorTest(0x4B); };
TEST(ProcessorTests, 4C) { RunProcessorTest(0x4C); };
TEST(ProcessorTests, 4D) { RunProcessorTest(0x4D); };
TEST(ProcessorTests, 4E) { RunProcessorTest(0x4E); };
TEST(ProcessorTests, 4F) { RunProcessorTest(0x4F); };
TEST(ProcessorTests, 50) { RunProcessorTest(0x50); };
TEST(ProcessorTests, 51) { RunProcessorTest(0x51); };
TEST(ProcessorTests, 53) { RunProcessorTest(0x53); };
TEST(ProcessorTests, 54) { RunProcessorTest(0x54); };
TEST(ProcessorTests, 55) { RunProcessorTest(0x55); };
TEST(ProcessorTests, 56) { RunProcessorTest(0x56); };
TEST(ProcessorTests, 57) { RunProcessorTest(0x57); };
TEST(ProcessorTests, 58) { RunProcessorTest(0x58); };
TEST(ProcessorTests, 59) { RunProcessorTest(0x59); };
TEST(ProcessorTests, 5A) { RunProcessorTest(0x5A); };
TEST(ProcessorTests, 5B) { RunProcessorTest(0x5B); };
TEST(ProcessorTests, 5C) { RunProcessorTest(0x5C); };
TEST(ProcessorTests, 5D) { RunProcessorTest(0x5D); };
TEST(ProcessorTests, 5E) { RunProcessorTest(0x5E); };
TEST(ProcessorTests, 5F) { RunProcessorTest(0x5F); };
TEST(ProcessorTests, 60) { RunProcessorTest(0x60); };
TEST(ProcessorTests, 61) { RunProcessorTest(0x61); };
TEST(ProcessorTests, 63) { RunProcessorTest(0x63); };
TEST(ProcessorTests, 64) { RunProcessorTest(0x64); };
TEST(ProcessorTests, 65) { RunProcessorTest(0x65); };
TEST(ProcessorTests, 66) { RunProcessorTest(0x66); };
TEST(ProcessorTests, 67) { RunProcessorTest(0x67); };
TEST(ProcessorTests, 68) { RunProcessorTest(0x68); };
TEST(ProcessorTests, 69) { RunProcessorTest(0x69); };
TEST(ProcessorTests, 6A) { RunProcessorTest(0x6A); };
TEST(ProcessorTests, 6B) { RunProcessorTest(0x6B); };
TEST(ProcessorTests, 6C) { RunProcessorTest(0x6C); };
TEST(ProcessorTests, 6D) { RunProcessorTest(0x6D); };
TEST(ProcessorTests, 6E) { RunProcessorTest(0x6E); };
TEST(ProcessorTests, 6F) { RunProcessorTest(0x6F); };
TEST(ProcessorTests, 70) { RunProcessorTest(0x70); };
TEST(ProcessorTests, 71) { RunProcessorTest(0x71); };
TEST(ProcessorTests, 73) { RunProcessorTest(0x73); };
TEST(ProcessorTests, 74) { RunProcessorTest(0x74); };
TEST(ProcessorTests, 75) { RunProcessorTest(0x75); };
TEST(ProcessorTests, 76) { RunProcessorTest(0x76); };
TEST(ProcessorTests, 77) { RunProcessorTest(0x77); };
TEST(ProcessorTests, 78) { RunProcessorTest(0x78); };
TEST(ProcessorTests, 79) { RunProcessorTest(0x79); };
TEST(ProcessorTests, 7A) { RunProcessorTest(0x7A); };
TEST(ProcessorTests, 7B) { RunProcessorTest(0x7B); };
TEST(ProcessorTests, 7C) { RunProcessorTest(0x7C); };
TEST(ProcessorTests, 7D) { RunProcessorTest(0x7D); };
TEST(ProcessorTests, 7E) { RunProcessorTest(0x7E); };
TEST(ProcessorTests, 7F) { RunProcessorTest(0x7F); };
TEST(ProcessorTests, 80) { RunProcessorTest(0x80); };
TEST(ProcessorTests, 81) { RunProcessorTest(0x81); };
TEST(ProcessorTests, 82) { RunProcessorTest(0x82); };
TEST(ProcessorTests, 83) { RunProcessorTest(0x83); };
TEST(ProcessorTests, 84) { RunProcessorTest(0x84); };
TEST(ProcessorTests, 85) { RunProcessorTest(0x85); };
TEST(ProcessorTests, 86) { RunProcessorTest(0x86); };
TEST(ProcessorTests, 87) { RunProcessorTest(0x87); };
TEST(ProcessorTests, 88) { RunProcessorTest(0x88); };
TEST(ProcessorTests, 89) { RunProcessorTest(0x89); };
TEST(ProcessorTests, 8A) { RunProcessorTest(0x8A); };
// TEST(ProcessorTests, 8B) { RunProcessorTest(0x8B); };
TEST(ProcessorTests, 8C) { RunProcessorTest(0x8C); };
TEST(ProcessorTests, 8D) { RunProcessorTest(0x8D); };
TEST(ProcessorTests, 8E) { RunProcessorTest(0x8E); };
TEST(ProcessorTests, 8F) { RunProcessorTest(0x8F); };
TEST(ProcessorTests, 90) { RunProcessorTest(0x90); };
TEST(ProcessorTests, 91) { RunProcessorTest(0x91); };
TEST(ProcessorTests, 93) { RunProcessorTest(0x93); };
TEST(ProcessorTests, 94) { RunProcessorTest(0x94); };
TEST(ProcessorTests, 95) { RunProcessorTest(0x95); };
TEST(ProcessorTests, 96) { RunProcessorTest(0x96); };
TEST(ProcessorTests, 97) { RunProcessorTest(0x97); };
TEST(ProcessorTests, 98) { RunProcessorTest(0x98); };
TEST(ProcessorTests, 99) { RunProcessorTest(0x99); };
TEST(ProcessorTests, 9A) { RunProcessorTest(0x9A); };
TEST(ProcessorTests, 9B) { RunProcessorTest(0x9B); };
TEST(ProcessorTests, 9C) { RunProcessorTest(0x9C); };
TEST(ProcessorTests, 9D) { RunProcessorTest(0x9D); };
TEST(ProcessorTests, 9E) { RunProcessorTest(0x9E); };
TEST(ProcessorTests, 9F) { RunProcessorTest(0x9F); };
TEST(ProcessorTests, A0) { RunProcessorTest(0xA0); };
TEST(ProcessorTests, A1) { RunProcessorTest(0xA1); };
TEST(ProcessorTests, A2) { RunProcessorTest(0xA2); };
TEST(ProcessorTests, A3) { RunProcessorTest(0xA3); };
TEST(ProcessorTests, A4) { RunProcessorTest(0xA4); };
TEST(ProcessorTests, A5) { RunProcessorTest(0xA5); };
TEST(ProcessorTests, A6) { RunProcessorTest(0xA6); };
TEST(ProcessorTests, A7) { RunProcessorTest(0xA7); };
TEST(ProcessorTests, A8) { RunProcessorTest(0xA8); };
TEST(ProcessorTests, A9) { RunProcessorTest(0xA9); };
TEST(ProcessorTests, AA) { RunProcessorTest(0xAA); };
// TEST(ProcessorTests, AB) { RunProcessorTest(0xAB); };
TEST(ProcessorTests, AC) { RunProcessorTest(0xAC); };
TEST(ProcessorTests, AD) { RunProcessorTest(0xAD); };
TEST(ProcessorTests, AE) { RunProcessorTest(0xAE); };
TEST(ProcessorTests, AF) { RunProcessorTest(0xAF); };
TEST(ProcessorTests, B0) { RunProcessorTest(0xB0); };
TEST(ProcessorTests, B1) { RunProcessorTest(0xB1); };
TEST(ProcessorTests, B3) { RunProcessorTest(0xB3); };
TEST(ProcessorTests, B4) { RunProcessorTest(0xB4); };
TEST(ProcessorTests, B5) { RunProcessorTest(0xB5); };
TEST(ProcessorTests, B6) { RunProcessorTest(0xB6); };
TEST(ProcessorTests, B7) { RunProcessorTest(0xB7); };
TEST(ProcessorTests, B8) { RunProcessorTest(0xB8); };
TEST(ProcessorTests, B9) { RunProcessorTest(0xB9); };
TEST(ProcessorTests, BA) { RunProcessorTest(0xBA); };
TEST(ProcessorTests, BB) { RunProcessorTest(0xBB); };
TEST(ProcessorTests, BC) { RunProcessorTest(0xBC); };
TEST(ProcessorTests, BD) { RunProcessorTest(0xBD); };
TEST(ProcessorTests, BE) { RunProcessorTest(0xBE); };
TEST(ProcessorTests, BF) { RunProcessorTest(0xBF); };
TEST(ProcessorTests, C0) { RunProcessorTest(0xC0); };
TEST(ProcessorTests, C1) { RunProcessorTest(0xC1); };
TEST(ProcessorTests, C2) { RunProcessorTest(0xC2); };
TEST(ProcessorTests, C3) { RunProcessorTest(0xC3); };
TEST(ProcessorTests, C4) { RunProcessorTest(0xC4); };
TEST(ProcessorTests, C5) { RunProcessorTest(0xC5); };
TEST(ProcessorTests, C6) { RunProcessorTest(0xC6); };
TEST(ProcessorTests, C7) { RunProcessorTest(0xC7); };
TEST(ProcessorTests, C8) { RunProcessorTest(0xC8); };
TEST(ProcessorTests, C9) { RunProcessorTest(0xC9); };
TEST(ProcessorTests, CA) { RunProcessorTest(0xCA); };
TEST(ProcessorTests, CB) { RunProcessorTest(0xCB); };
TEST(ProcessorTests, CC) { RunProcessorTest(0xCC); };
TEST(ProcessorTests, CD) { RunProcessorTest(0xCD); };
TEST(ProcessorTests, CE) { RunProcessorTest(0xCE); };
TEST(ProcessorTests, CF) { RunProcessorTest(0xCF); };
TEST(ProcessorTests, D0) { RunProcessorTest(0xD0); };
TEST(ProcessorTests, D1) { RunProcessorTest(0xD1); };
TEST(ProcessorTests, D3) { RunProcessorTest(0xD3); };
TEST(ProcessorTests, D4) { RunProcessorTest(0xD4); };
TEST(ProcessorTests, D5) { RunProcessorTest(0xD5); };
TEST(ProcessorTests, D6) { RunProcessorTest(0xD6); };
TEST(ProcessorTests, D7) { RunProcessorTest(0xD7); };
TEST(ProcessorTests, D8) { RunProcessorTest(0xD8); };
TEST(ProcessorTests, D9) { RunProcessorTest(0xD9); };
TEST(ProcessorTests, DA) { RunProcessorTest(0xDA); };
TEST(ProcessorTests, DB) { RunProcessorTest(0xDB); };
TEST(ProcessorTests, DC) { RunProcessorTest(0xDC); };
TEST(ProcessorTests, DD) { RunProcessorTest(0xDD); };
TEST(ProcessorTests, DE) { RunProcessorTest(0xDE); };
TEST(ProcessorTests, DF) { RunProcessorTest(0xDF); };
TEST(ProcessorTests, E0) { RunProcessorTest(0xE0); };
TEST(ProcessorTests, E1) { RunProcessorTest(0xE1); };
TEST(ProcessorTests, E2) { RunProcessorTest(0xE2); };
TEST(ProcessorTests, E3) { RunProcessorTest(0xE3); };
TEST(ProcessorTests, E4) { RunProcessorTest(0xE4); };
TEST(ProcessorTests, E5) { RunProcessorTest(0xE5); };
TEST(ProcessorTests, E6) { RunProcessorTest(0xE6); };
TEST(ProcessorTests, E7) { RunProcessorTest(0xE7); };
TEST(ProcessorTests, E8) { RunProcessorTest(0xE8); };
TEST(ProcessorTests, E9) { RunProcessorTest(0xE9); };
TEST(ProcessorTests, EA) { RunProcessorTest(0xEA); };
TEST(ProcessorTests, EB) { RunProcessorTest(0xEB); };
TEST(ProcessorTests, EC) { RunProcessorTest(0xEC); };
TEST(ProcessorTests, ED) { RunProcessorTest(0xED); };
TEST(ProcessorTests, EE) { RunProcessorTest(0xEE); };
TEST(ProcessorTests, EF) { RunProcessorTest(0xEF); };
TEST(ProcessorTests, F0) { RunProcessorTest(0xF0); };
TEST(ProcessorTests, F1) { RunProcessorTest(0xF1); };
TEST(ProcessorTests, F3) { RunProcessorTest(0xF3); };
TEST(ProcessorTests, F4) { RunProcessorTest(0xF4); };
TEST(ProcessorTests, F5) { RunProcessorTest(0xF5); };
TEST(ProcessorTests, F6) { RunProcessorTest(0xF6); };
TEST(ProcessorTests, F7) { RunProcessorTest(0xF7); };
TEST(ProcessorTests, F8) { RunProcessorTest(0xF8); };
TEST(ProcessorTests, F9) { RunProcessorTest(0xF9); };
TEST(ProcessorTests, FA) { RunProcessorTest(0xFA); };
TEST(ProcessorTests, FB) { RunProcessorTest(0xFB); };
TEST(ProcessorTests, FC) { RunProcessorTest(0xFC); };
TEST(ProcessorTests, FD) { RunProcessorTest(0xFD); };
TEST(ProcessorTests, FE) { RunProcessorTest(0xFE); };
TEST(ProcessorTests, FF) { RunProcessorTest(0xFF); };
// clang-format on

}
