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
    // clang-format off
    switch (opc) {
        case 0x00: Brk(0); break;
        case 0x01: Ora(Idx()); break;
        case 0x03: Slo(Idx()); break;
        case 0x04: Nop(Zpg()); break;
        case 0x05: Ora(Zpg()); break;
        case 0x06: Asl(Zpg()); break;
        case 0x07: Slo(Zpg()); break;
        case 0x08: Php(0); break;
        case 0x09: Ora(Imm()); break;
        case 0x0A: AslA(0); break;
        case 0x0B: Anc(Imm()); break;
        case 0x0C: Nop(Abs()); break;
        case 0x0D: Ora(Abs()); break;
        case 0x0E: Asl(Abs()); break;
        case 0x0F: Slo(Abs()); break;
        case 0x10: Bpl(0); break;
        case 0x11: Ora(Idy<false>()); break;
        case 0x13: Slo(Idy<true>()); break;
        case 0x14: Nop(Zpx()); break;
        case 0x15: Ora(Zpx()); break;
        case 0x16: Asl(Zpx()); break;
        case 0x17: Slo(Zpx()); break;
        case 0x18: Clc(0); break;
        case 0x19: Ora(Aby<false>()); break;
        case 0x1A: Nop(Imp()); break;
        case 0x1B: Slo(Aby<true>()); break;
        case 0x1C: Nop(Abx<false>()); break;
        case 0x1D: Ora(Abx<false>()); break;
        case 0x1E: Asl(Abx<true>()); break;
        case 0x1F: Slo(Abx<true>()); break;
        case 0x20: Jsr(0); break;
        case 0x21: And(Idx()); break;
        case 0x23: Rla(Idx()); break;
        case 0x24: Bit(Zpg()); break;
        case 0x25: And(Zpg()); break;
        case 0x26: Rol(Zpg()); break;
        case 0x27: Rla(Zpg()); break;
        case 0x28: Plp(0); break;
        case 0x29: And(Imm()); break;
        case 0x2A: RolA(0); break;
        case 0x2B: Anc(Imm()); break;
        case 0x2C: Bit(Abs()); break;
        case 0x2D: And(Abs()); break;
        case 0x2E: Rol(Abs()); break;
        case 0x2F: Rla(Abs()); break;
        case 0x30: Bmi(0); break;
        case 0x31: And(Idy<false>()); break;
        case 0x33: Rla(Idy<true>()); break;
        case 0x34: Nop(Zpx()); break;
        case 0x35: And(Zpx()); break;
        case 0x36: Rol(Zpx()); break;
        case 0x37: Rla(Zpx()); break;
        case 0x38: Sec(0); break;
        case 0x39: And(Aby<false>()); break;
        case 0x3A: Nop(Imp()); break;
        case 0x3B: Rla(Aby<true>()); break;
        case 0x3C: Nop(Abx<false>()); break;
        case 0x3D: And(Abx<false>()); break;
        case 0x3E: Rol(Abx<true>()); break;
        case 0x3F: Rla(Abx<true>()); break;
        case 0x40: Rti(0); break;
        case 0x41: Eor(Idx()); break;
        case 0x43: Sre(Idx()); break;
        case 0x44: Nop(Zpg()); break;
        case 0x45: Eor(Zpg()); break;
        case 0x46: Lsr(Zpg()); break;
        case 0x47: Sre(Zpg()); break;
        case 0x48: Pha(0); break;
        case 0x49: Eor(Imm()); break;
        case 0x4A: LsrA(0); break;
        case 0x4B: Alr(Imm()); break;
        case 0x4C: Jmp(Abs()); break;
        case 0x4D: Eor(Abs()); break;
        case 0x4E: Lsr(Abs()); break;
        case 0x4F: Sre(Abs()); break;
        case 0x50: Bvc(0); break;
        case 0x51: Eor(Idy<false>()); break;
        case 0x53: Sre(Idy<true>()); break;
        case 0x54: Nop(Zpx()); break;
        case 0x55: Eor(Zpx()); break;
        case 0x56: Lsr(Zpx()); break;
        case 0x57: Sre(Zpx()); break;
        case 0x58: Cli(0); break;
        case 0x59: Eor(Aby<false>()); break;
        case 0x5A: Nop(Imp()); break;
        case 0x5B: Sre(Aby<true>()); break;
        case 0x5C: Nop(Abx<false>()); break;
        case 0x5D: Eor(Abx<false>()); break;
        case 0x5E: Lsr(Abx<true>()); break;
        case 0x5F: Sre(Abx<true>()); break;
        case 0x60: Rts(0); break;
        case 0x61: Adc(Idx()); break;
        case 0x63: Rra(Idx()); break;
        case 0x64: Nop(Zpg()); break;
        case 0x65: Adc(Zpg()); break;
        case 0x66: Ror(Zpg()); break;
        case 0x67: Rra(Zpg()); break;
        case 0x68: Pla(0); break;
        case 0x69: Adc(Imm()); break;
        case 0x6A: RorA(0); break;
        case 0x6B: Arr(Imm()); break;
        case 0x6C: Jmp(Ind()); break;
        case 0x6D: Adc(Abs()); break;
        case 0x6E: Ror(Abs()); break;
        case 0x6F: Rra(Abs()); break;
        case 0x70: Bvs(0); break;
        case 0x71: Adc(Idy<false>()); break;
        case 0x73: Rra(Idy<true>()); break;
        case 0x74: Nop(Zpx()); break;
        case 0x75: Adc(Zpx()); break;
        case 0x76: Ror(Zpx()); break;
        case 0x77: Rra(Zpx()); break;
        case 0x78: Sei(0); break;
        case 0x79: Adc(Aby<false>()); break;
        case 0x7A: Nop(Imp()); break;
        case 0x7B: Rra(Aby<true>()); break;
        case 0x7C: Nop(Abx<false>()); break;
        case 0x7D: Adc(Abx<false>()); break;
        case 0x7E: Ror(Abx<true>()); break;
        case 0x7F: Rra(Abx<true>()); break;
        case 0x80: Nop(Imm()); break;
        case 0x81: Sta(Idx()); break;
        case 0x82: Nop(Imm()); break;
        case 0x83: Sax(Idx()); break;
        case 0x84: Sty(Zpg()); break;
        case 0x85: Sta(Zpg()); break;
        case 0x86: Stx(Zpg()); break;
        case 0x87: Sax(Zpg()); break;
        case 0x88: Dey(0); break;
        case 0x89: Nop(Imm()); break;
        case 0x8A: Txa(0); break;
        case 0x8B: break; // ane
        case 0x8C: Sty(Abs()); break;
        case 0x8D: Sta(Abs()); break;
        case 0x8E: Stx(Abs()); break;
        case 0x8F: Sax(Abs()); break;
        case 0x90: Bcc(0); break;
        case 0x91: Sta(Idy<true>()); break;
        case 0x93: Sha(Idy<true>()); break;
        case 0x94: Sty(Zpx()); break;
        case 0x95: Sta(Zpx()); break;
        case 0x96: Stx(Zpy()); break;
        case 0x97: Sax(Zpy()); break;
        case 0x98: Tya(0); break;
        case 0x99: Sta(Aby<true>()); break;
        case 0x9A: Txs(0); break;
        case 0x9B: Tas(Aby<true>()); break;
        case 0x9C: Shy(Abx<true>()); break;
        case 0x9D: Sta(Abx<true>()); break;
        case 0x9E: Shx(Aby<true>()); break;
        case 0x9F: Sha(Aby<true>()); break;
        case 0xA0: Ldy(Imm()); break;
        case 0xA1: Lda(Idx()); break;
        case 0xA2: Ldx(Imm()); break;
        case 0xA3: Lax(Idx()); break;
        case 0xA4: Ldy(Zpg()); break;
        case 0xA5: Lda(Zpg()); break;
        case 0xA6: Ldx(Zpg()); break;
        case 0xA7: Lax(Zpg()); break;
        case 0xA8: Tay(0); break;
        case 0xA9: Lda(Imm()); break;
        case 0xAA: Tax(0); break;
        case 0xAB: break; //Lxa(Imm()); break;
        case 0xAC: Ldy(Abs()); break;
        case 0xAD: Lda(Abs()); break;
        case 0xAE: Ldx(Abs()); break;
        case 0xAF: Lax(Abs()); break;
        case 0xB0: Bcs(0); break;
        case 0xB1: Lda(Idy<false>()); break;
        case 0xB3: Lax(Idy<false>()); break;
        case 0xB4: Ldy(Zpx()); break;
        case 0xB5: Lda(Zpx()); break;
        case 0xB6: Ldx(Zpy()); break;
        case 0xB7: Lax(Zpy()); break;
        case 0xB8: Clv(0); break;
        case 0xB9: Lda(Aby<false>()); break;
        case 0xBA: Tsx(0); break;
        case 0xBB: Las(Aby<false>()); break;
        case 0xBC: Ldy(Abx<false>()); break;
        case 0xBD: Lda(Abx<false>()); break;
        case 0xBE: Ldx(Aby<false>()); break;
        case 0xBF: Lax(Aby<false>()); break;
        case 0xC0: Cpy(Imm()); break;
        case 0xC1: Cmp(Idx()); break;
        case 0xC2: Nop(Imm()); break;
        case 0xC3: Dcp(Idx()); break;
        case 0xC4: Cpy(Zpg()); break;
        case 0xC5: Cmp(Zpg()); break;
        case 0xC6: Dec(Zpg()); break;
        case 0xC7: Dcp(Zpg()); break;
        case 0xC8: Iny(0); break;
        case 0xC9: Cmp(Imm()); break;
        case 0xCA: Dex(0); break;
        case 0xCB: Sbx(Imm()); break;
        case 0xCC: Cpy(Abs()); break;
        case 0xCD: Cmp(Abs()); break;
        case 0xCE: Dec(Abs()); break;
        case 0xCF: Dcp(Abs()); break;
        case 0xD0: Bne(0); break;
        case 0xD1: Cmp(Idy<false>()); break;
        case 0xD3: Dcp(Idy<true>()); break;
        case 0xD4: Nop(Zpx()); break;
        case 0xD5: Cmp(Zpx()); break;
        case 0xD6: Dec(Zpx()); break;
        case 0xD7: Dcp(Zpx()); break;
        case 0xD8: Cld(0); break;
        case 0xD9: Cmp(Aby<false>()); break;
        case 0xDA: Nop(Imp()); break;
        case 0xDB: Dcp(Aby<true>()); break;
        case 0xDC: Nop(Abx<false>()); break;
        case 0xDD: Cmp(Abx<false>()); break;
        case 0xDE: Dec(Abx<true>()); break;
        case 0xDF: Dcp(Abx<true>()); break;
        case 0xE0: Cpx(Imm()); break;
        case 0xE1: Sbc(Idx()); break;
        case 0xE2: Nop(Imm()); break;
        case 0xE3: Isc(Idx()); break;
        case 0xE4: Cpx(Zpg()); break;
        case 0xE5: Sbc(Zpg()); break;
        case 0xE6: Inc(Zpg()); break;
        case 0xE7: Isc(Zpg()); break;
        case 0xE8: Inx(0); break;
        case 0xE9: Sbc(Imm()); break;
        case 0xEA: Nop(Imp()); break;
        case 0xEB: Sbc(Imm()); break;
        case 0xEC: Cpx(Abs()); break;
        case 0xED: Sbc(Abs()); break;
        case 0xEE: Inc(Abs()); break;
        case 0xEF: Isc(Abs()); break;
        case 0xF0: Beq(0); break;
        case 0xF1: Sbc(Idy<false>()); break;
        case 0xF3: Isc(Idy<true>()); break;
        case 0xF4: Nop(Zpx()); break;
        case 0xF5: Sbc(Zpx()); break;
        case 0xF6: Inc(Zpx()); break;
        case 0xF7: Isc(Zpx()); break;
        case 0xF8: Sed(0); break;
        case 0xF9: Sbc(Aby<false>()); break;
        case 0xFA: Nop(Imp()); break;
        case 0xFB: Isc(Aby<true>()); break;
        case 0xFC: Nop(Abx<false>()); break;
        case 0xFD: Sbc(Abx<false>()); break;
        case 0xFE: Inc(Abx<true>()); break;
        case 0xFF: Isc(Abx<true>()); break;
    }
    // clang-format on
}
}

TEST_CASE("ProcessorTests")
{
    auto cpu = std::make_unique<Moss::Cpu>();
    ondemand::parser parser;

    auto run = [&](std::uint8_t opc) {
        auto json =
          padded_string::load(std::format("processor_tests/{:02x}.json", opc));
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
            REQUIRE(cpu->Pc() == end["pc"].get_int64());
            REQUIRE(cpu->S() == end["s"].get_int64());
            REQUIRE(cpu->A() == end["a"].get_int64());
            REQUIRE(cpu->X() == end["x"].get_int64());
            REQUIRE(cpu->Y() == end["y"].get_int64());
            REQUIRE(cpu->P() == end["p"].get_int64());
            for (ondemand::array entry : end["ram"].get_array()) {
                auto it = entry.begin();
                uint16_t addr = static_cast<std::uint16_t>((*it).get_int64());
                ++it;
                uint8_t data = static_cast<std::uint8_t>((*it).get_int64());
                REQUIRE(cpu->Read(addr) == data);
            }
        }
    };

    // clang-format off
    SUBCASE("00") { run(0x00); }
    SUBCASE("01") { run(0x01); }
    SUBCASE("03") { run(0x03); }
    SUBCASE("04") { run(0x04); }
    SUBCASE("05") { run(0x05); }
    SUBCASE("06") { run(0x06); }
    SUBCASE("07") { run(0x07); }
    SUBCASE("08") { run(0x08); }
    SUBCASE("09") { run(0x09); }
    SUBCASE("0A") { run(0x0A); }
    SUBCASE("0B") { run(0x0B); }
    SUBCASE("0C") { run(0x0C); }
    SUBCASE("0D") { run(0x0D); }
    SUBCASE("0E") { run(0x0E); }
    SUBCASE("0F") { run(0x0F); }
    SUBCASE("10") { run(0x10); }
    SUBCASE("11") { run(0x11); }
    SUBCASE("13") { run(0x13); }
    SUBCASE("14") { run(0x14); }
    SUBCASE("15") { run(0x15); }
    SUBCASE("16") { run(0x16); }
    SUBCASE("17") { run(0x17); }
    SUBCASE("18") { run(0x18); }
    SUBCASE("19") { run(0x19); }
    SUBCASE("1A") { run(0x1A); }
    SUBCASE("1B") { run(0x1B); }
    SUBCASE("1C") { run(0x1C); }
    SUBCASE("1D") { run(0x1D); }
    SUBCASE("1E") { run(0x1E); }
    SUBCASE("1F") { run(0x1F); }
    SUBCASE("20") { run(0x20); }
    SUBCASE("21") { run(0x21); }
    SUBCASE("23") { run(0x23); }
    SUBCASE("24") { run(0x24); }
    SUBCASE("25") { run(0x25); }
    SUBCASE("26") { run(0x26); }
    SUBCASE("27") { run(0x27); }
    SUBCASE("28") { run(0x28); }
    SUBCASE("29") { run(0x29); }
    SUBCASE("2A") { run(0x2A); }
    SUBCASE("2B") { run(0x2B); }
    SUBCASE("2C") { run(0x2C); }
    SUBCASE("2D") { run(0x2D); }
    SUBCASE("2E") { run(0x2E); }
    SUBCASE("2F") { run(0x2F); }
    SUBCASE("30") { run(0x30); }
    SUBCASE("31") { run(0x31); }
    SUBCASE("33") { run(0x33); }
    SUBCASE("34") { run(0x34); }
    SUBCASE("35") { run(0x35); }
    SUBCASE("36") { run(0x36); }
    SUBCASE("37") { run(0x37); }
    SUBCASE("38") { run(0x38); }
    SUBCASE("39") { run(0x39); }
    SUBCASE("3A") { run(0x3A); }
    SUBCASE("3B") { run(0x3B); }
    SUBCASE("3C") { run(0x3C); }
    SUBCASE("3D") { run(0x3D); }
    SUBCASE("3E") { run(0x3E); }
    SUBCASE("3F") { run(0x3F); }
    SUBCASE("40") { run(0x40); }
    SUBCASE("41") { run(0x41); }
    SUBCASE("43") { run(0x43); }
    SUBCASE("44") { run(0x44); }
    SUBCASE("45") { run(0x45); }
    SUBCASE("46") { run(0x46); }
    SUBCASE("47") { run(0x47); }
    SUBCASE("48") { run(0x48); }
    SUBCASE("49") { run(0x49); }
    SUBCASE("4A") { run(0x4A); }
    SUBCASE("4B") { run(0x4B); }
    SUBCASE("4C") { run(0x4C); }
    SUBCASE("4D") { run(0x4D); }
    SUBCASE("4E") { run(0x4E); }
    SUBCASE("4F") { run(0x4F); }
    SUBCASE("50") { run(0x50); }
    SUBCASE("51") { run(0x51); }
    SUBCASE("53") { run(0x53); }
    SUBCASE("54") { run(0x54); }
    SUBCASE("55") { run(0x55); }
    SUBCASE("56") { run(0x56); }
    SUBCASE("57") { run(0x57); }
    SUBCASE("58") { run(0x58); }
    SUBCASE("59") { run(0x59); }
    SUBCASE("5A") { run(0x5A); }
    SUBCASE("5B") { run(0x5B); }
    SUBCASE("5C") { run(0x5C); }
    SUBCASE("5D") { run(0x5D); }
    SUBCASE("5E") { run(0x5E); }
    SUBCASE("5F") { run(0x5F); }
    SUBCASE("60") { run(0x60); }
    SUBCASE("61") { run(0x61); }
    SUBCASE("63") { run(0x63); }
    SUBCASE("64") { run(0x64); }
    SUBCASE("65") { run(0x65); }
    SUBCASE("66") { run(0x66); }
    SUBCASE("67") { run(0x67); }
    SUBCASE("68") { run(0x68); }
    SUBCASE("69") { run(0x69); }
    SUBCASE("6A") { run(0x6A); }
    SUBCASE("6B") { run(0x6B); }
    SUBCASE("6C") { run(0x6C); }
    SUBCASE("6D") { run(0x6D); }
    SUBCASE("6E") { run(0x6E); }
    SUBCASE("6F") { run(0x6F); }
    SUBCASE("70") { run(0x70); }
    SUBCASE("71") { run(0x71); }
    SUBCASE("73") { run(0x73); }
    SUBCASE("74") { run(0x74); }
    SUBCASE("75") { run(0x75); }
    SUBCASE("76") { run(0x76); }
    SUBCASE("77") { run(0x77); }
    SUBCASE("78") { run(0x78); }
    SUBCASE("79") { run(0x79); }
    SUBCASE("7A") { run(0x7A); }
    SUBCASE("7B") { run(0x7B); }
    SUBCASE("7C") { run(0x7C); }
    SUBCASE("7D") { run(0x7D); }
    SUBCASE("7E") { run(0x7E); }
    SUBCASE("7F") { run(0x7F); }
    SUBCASE("80") { run(0x80); }
    SUBCASE("81") { run(0x81); }
    SUBCASE("82") { run(0x82); }
    SUBCASE("83") { run(0x83); }
    SUBCASE("84") { run(0x84); }
    SUBCASE("85") { run(0x85); }
    SUBCASE("86") { run(0x86); }
    SUBCASE("87") { run(0x87); }
    SUBCASE("88") { run(0x88); }
    SUBCASE("89") { run(0x89); }
    SUBCASE("8A") { run(0x8A); }
    // SUBCASE("8B") { run(0x8B); }
    SUBCASE("8C") { run(0x8C); }
    SUBCASE("8D") { run(0x8D); }
    SUBCASE("8E") { run(0x8E); }
    SUBCASE("8F") { run(0x8F); }
    SUBCASE("90") { run(0x90); }
    SUBCASE("91") { run(0x91); }
    SUBCASE("93") { run(0x93); }
    SUBCASE("94") { run(0x94); }
    SUBCASE("95") { run(0x95); }
    SUBCASE("96") { run(0x96); }
    SUBCASE("97") { run(0x97); }
    SUBCASE("98") { run(0x98); }
    SUBCASE("99") { run(0x99); }
    SUBCASE("9A") { run(0x9A); }
    SUBCASE("9B") { run(0x9B); }
    SUBCASE("9C") { run(0x9C); }
    SUBCASE("9D") { run(0x9D); }
    SUBCASE("9E") { run(0x9E); }
    SUBCASE("9F") { run(0x9F); }
    SUBCASE("A0") { run(0xA0); }
    SUBCASE("A1") { run(0xA1); }
    SUBCASE("A2") { run(0xA2); }
    SUBCASE("A3") { run(0xA3); }
    SUBCASE("A4") { run(0xA4); }
    SUBCASE("A5") { run(0xA5); }
    SUBCASE("A6") { run(0xA6); }
    SUBCASE("A7") { run(0xA7); }
    SUBCASE("A8") { run(0xA8); }
    SUBCASE("A9") { run(0xA9); }
    SUBCASE("AA") { run(0xAA); }
    // SUBCASE("AB") { run(0xAB); }
    SUBCASE("AC") { run(0xAC); }
    SUBCASE("AD") { run(0xAD); }
    SUBCASE("AE") { run(0xAE); }
    SUBCASE("AF") { run(0xAF); }
    SUBCASE("B0") { run(0xB0); }
    SUBCASE("B1") { run(0xB1); }
    SUBCASE("B3") { run(0xB3); }
    SUBCASE("B4") { run(0xB4); }
    SUBCASE("B5") { run(0xB5); }
    SUBCASE("B6") { run(0xB6); }
    SUBCASE("B7") { run(0xB7); }
    SUBCASE("B8") { run(0xB8); }
    SUBCASE("B9") { run(0xB9); }
    SUBCASE("BA") { run(0xBA); }
    SUBCASE("BB") { run(0xBB); }
    SUBCASE("BC") { run(0xBC); }
    SUBCASE("BD") { run(0xBD); }
    SUBCASE("BE") { run(0xBE); }
    SUBCASE("BF") { run(0xBF); }
    SUBCASE("C0") { run(0xC0); }
    SUBCASE("C1") { run(0xC1); }
    SUBCASE("C2") { run(0xC2); }
    SUBCASE("C3") { run(0xC3); }
    SUBCASE("C4") { run(0xC4); }
    SUBCASE("C5") { run(0xC5); }
    SUBCASE("C6") { run(0xC6); }
    SUBCASE("C7") { run(0xC7); }
    SUBCASE("C8") { run(0xC8); }
    SUBCASE("C9") { run(0xC9); }
    SUBCASE("CA") { run(0xCA); }
    SUBCASE("CB") { run(0xCB); }
    SUBCASE("CC") { run(0xCC); }
    SUBCASE("CD") { run(0xCD); }
    SUBCASE("CE") { run(0xCE); }
    SUBCASE("CF") { run(0xCF); }
    SUBCASE("D0") { run(0xD0); }
    SUBCASE("D1") { run(0xD1); }
    SUBCASE("D3") { run(0xD3); }
    SUBCASE("D4") { run(0xD4); }
    SUBCASE("D5") { run(0xD5); }
    SUBCASE("D6") { run(0xD6); }
    SUBCASE("D7") { run(0xD7); }
    SUBCASE("D8") { run(0xD8); }
    SUBCASE("D9") { run(0xD9); }
    SUBCASE("DA") { run(0xDA); }
    SUBCASE("DB") { run(0xDB); }
    SUBCASE("DC") { run(0xDC); }
    SUBCASE("DD") { run(0xDD); }
    SUBCASE("DE") { run(0xDE); }
    SUBCASE("DF") { run(0xDF); }
    SUBCASE("E0") { run(0xE0); }
    SUBCASE("E1") { run(0xE1); }
    SUBCASE("E2") { run(0xE2); }
    SUBCASE("E3") { run(0xE3); }
    SUBCASE("E4") { run(0xE4); }
    SUBCASE("E5") { run(0xE5); }
    SUBCASE("E6") { run(0xE6); }
    SUBCASE("E7") { run(0xE7); }
    SUBCASE("E8") { run(0xE8); }
    SUBCASE("E9") { run(0xE9); }
    SUBCASE("EA") { run(0xEA); }
    SUBCASE("EB") { run(0xEB); }
    SUBCASE("EC") { run(0xEC); }
    SUBCASE("ED") { run(0xED); }
    SUBCASE("EE") { run(0xEE); }
    SUBCASE("EF") { run(0xEF); }
    SUBCASE("F0") { run(0xF0); }
    SUBCASE("F1") { run(0xF1); }
    SUBCASE("F3") { run(0xF3); }
    SUBCASE("F4") { run(0xF4); }
    SUBCASE("F5") { run(0xF5); }
    SUBCASE("F6") { run(0xF6); }
    SUBCASE("F7") { run(0xF7); }
    SUBCASE("F8") { run(0xF8); }
    SUBCASE("F9") { run(0xF9); }
    SUBCASE("FA") { run(0xFA); }
    SUBCASE("FB") { run(0xFB); }
    SUBCASE("FC") { run(0xFC); }
    SUBCASE("FD") { run(0xFD); }
    SUBCASE("FE") { run(0xFE); }
    SUBCASE("FF") { run(0xFF); }
    // clang-format on
}
