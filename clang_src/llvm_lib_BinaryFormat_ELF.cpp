//===- llvm/BinaryFormat/ELF.cpp - The ELF format ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm_include_llvm_BinaryFormat_ELF.h"
#include "llvm_include_llvm_ADT_DenseMapInfo.h"
#include "llvm_include_llvm_ADT_StringSwitch.h"

using namespace llvm;
using namespace ELF;

/// Convert an architecture name into ELF's e_machine value.
uint16_t ELF::convertArchNameToEMachine(StringRef Arch) {
  std::string LowerArch = Arch.lower();
  return StringSwitch<uint16_t>(LowerArch)
      .Case("none", EM_NONE)
      .Case("m32", EM_M32)
      .Case("sparc", EM_SPARC)
      .Case("386", EM_386)
      .Case("68k", EM_68K)
      .Case("88k", EM_88K)
      .Case("iamcu", EM_IAMCU)
      .Case("860", EM_860)
      .Case("mips", EM_MIPS)
      .Case("s370", EM_S370)
      .Case("mips_rs3_le", EM_MIPS_RS3_LE)
      .Case("parisc", EM_PARISC)
      .Case("vpp500", EM_VPP500)
      .Case("sparc32plus", EM_SPARC32PLUS)
      .Case("960", EM_960)
      .Case("ppc", EM_PPC)
      .Case("ppc64", EM_PPC64)
      .Case("s390", EM_S390)
      .Case("spu", EM_SPU)
      .Case("v800", EM_V800)
      .Case("fr20", EM_FR20)
      .Case("rh32", EM_RH32)
      .Case("rce", EM_RCE)
      .Case("arm", EM_ARM)
      .Case("alpha", EM_ALPHA)
      .Case("sh", EM_SH)
      .Case("sparcv9", EM_SPARCV9)
      .Case("tricore", EM_TRICORE)
      .Case("arc", EM_ARC)
      .Case("h8_300", EM_H8_300)
      .Case("h8_300h", EM_H8_300H)
      .Case("h8s", EM_H8S)
      .Case("h8_500", EM_H8_500)
      .Case("ia_64", EM_IA_64)
      .Case("mips_x", EM_MIPS_X)
      .Case("coldfire", EM_COLDFIRE)
      .Case("68hc12", EM_68HC12)
      .Case("mma", EM_MMA)
      .Case("pcp", EM_PCP)
      .Case("ncpu", EM_NCPU)
      .Case("ndr1", EM_NDR1)
      .Case("starcore", EM_STARCORE)
      .Case("me16", EM_ME16)
      .Case("st100", EM_ST100)
      .Case("tinyj", EM_TINYJ)
      .Case("x86_64", EM_X86_64)
      .Case("pdsp", EM_PDSP)
      .Case("pdp10", EM_PDP10)
      .Case("pdp11", EM_PDP11)
      .Case("fx66", EM_FX66)
      .Case("st9plus", EM_ST9PLUS)
      .Case("st7", EM_ST7)
      .Case("68hc16", EM_68HC16)
      .Case("68hc11", EM_68HC11)
      .Case("68hc08", EM_68HC08)
      .Case("68hc05", EM_68HC05)
      .Case("svx", EM_SVX)
      .Case("st19", EM_ST19)
      .Case("vax", EM_VAX)
      .Case("cris", EM_CRIS)
      .Case("javelin", EM_JAVELIN)
      .Case("firepath", EM_FIREPATH)
      .Case("zsp", EM_ZSP)
      .Case("mmix", EM_MMIX)
      .Case("huany", EM_HUANY)
      .Case("prism", EM_PRISM)
      .Case("avr", EM_AVR)
      .Case("fr30", EM_FR30)
      .Case("d10v", EM_D10V)
      .Case("d30v", EM_D30V)
      .Case("v850", EM_V850)
      .Case("m32r", EM_M32R)
      .Case("mn10300", EM_MN10300)
      .Case("mn10200", EM_MN10200)
      .Case("pj", EM_PJ)
      .Case("openrisc", EM_OPENRISC)
      .Case("arc_compact", EM_ARC_COMPACT)
      .Case("xtensa", EM_XTENSA)
      .Case("videocore", EM_VIDEOCORE)
      .Case("tmm_gpp", EM_TMM_GPP)
      .Case("ns32k", EM_NS32K)
      .Case("tpc", EM_TPC)
      .Case("snp1k", EM_SNP1K)
      .Case("st200", EM_ST200)
      .Case("ip2k", EM_IP2K)
      .Case("max", EM_MAX)
      .Case("cr", EM_CR)
      .Case("f2mc16", EM_F2MC16)
      .Case("msp430", EM_MSP430)
      .Case("blackfin", EM_BLACKFIN)
      .Case("se_c33", EM_SE_C33)
      .Case("sep", EM_SEP)
      .Case("arca", EM_ARCA)
      .Case("unicore", EM_UNICORE)
      .Case("excess", EM_EXCESS)
      .Case("dxp", EM_DXP)
      .Case("altera_nios2", EM_ALTERA_NIOS2)
      .Case("crx", EM_CRX)
      .Case("xgate", EM_XGATE)
      .Case("c166", EM_C166)
      .Case("m16c", EM_M16C)
      .Case("dspic30f", EM_DSPIC30F)
      .Case("ce", EM_CE)
      .Case("m32c", EM_M32C)
      .Case("tsk3000", EM_TSK3000)
      .Case("rs08", EM_RS08)
      .Case("sharc", EM_SHARC)
      .Case("ecog2", EM_ECOG2)
      .Case("score7", EM_SCORE7)
      .Case("dsp24", EM_DSP24)
      .Case("videocore3", EM_VIDEOCORE3)
      .Case("latticemico32", EM_LATTICEMICO32)
      .Case("se_c17", EM_SE_C17)
      .Case("ti_c6000", EM_TI_C6000)
      .Case("ti_c2000", EM_TI_C2000)
      .Case("ti_c5500", EM_TI_C5500)
      .Case("mmdsp_plus", EM_MMDSP_PLUS)
      .Case("cypress_m8c", EM_CYPRESS_M8C)
      .Case("r32c", EM_R32C)
      .Case("trimedia", EM_TRIMEDIA)
      .Case("hexagon", EM_HEXAGON)
      .Case("8051", EM_8051)
      .Case("stxp7x", EM_STXP7X)
      .Case("nds32", EM_NDS32)
      .Case("ecog1", EM_ECOG1)
      .Case("ecog1x", EM_ECOG1X)
      .Case("maxq30", EM_MAXQ30)
      .Case("ximo16", EM_XIMO16)
      .Case("manik", EM_MANIK)
      .Case("craynv2", EM_CRAYNV2)
      .Case("rx", EM_RX)
      .Case("metag", EM_METAG)
      .Case("mcst_elbrus", EM_MCST_ELBRUS)
      .Case("ecog16", EM_ECOG16)
      .Case("cr16", EM_CR16)
      .Case("etpu", EM_ETPU)
      .Case("sle9x", EM_SLE9X)
      .Case("l10m", EM_L10M)
      .Case("k10m", EM_K10M)
      .Case("aarch64", EM_AARCH64)
      .Case("avr32", EM_AVR32)
      .Case("stm8", EM_STM8)
      .Case("tile64", EM_TILE64)
      .Case("tilepro", EM_TILEPRO)
      .Case("cuda", EM_CUDA)
      .Case("tilegx", EM_TILEGX)
      .Case("cloudshield", EM_CLOUDSHIELD)
      .Case("corea_1st", EM_COREA_1ST)
      .Case("corea_2nd", EM_COREA_2ND)
      .Case("arc_compact2", EM_ARC_COMPACT2)
      .Case("open8", EM_OPEN8)
      .Case("rl78", EM_RL78)
      .Case("videocore5", EM_VIDEOCORE5)
      .Case("78kor", EM_78KOR)
      .Case("56800ex", EM_56800EX)
      .Case("ba1", EM_BA1)
      .Case("ba2", EM_BA2)
      .Case("xcore", EM_XCORE)
      .Case("mchp_pic", EM_MCHP_PIC)
      .Case("intel205", EM_INTEL205)
      .Case("intel206", EM_INTEL206)
      .Case("intel207", EM_INTEL207)
      .Case("intel208", EM_INTEL208)
      .Case("intel209", EM_INTEL209)
      .Case("km32", EM_KM32)
      .Case("kmx32", EM_KMX32)
      .Case("kmx16", EM_KMX16)
      .Case("kmx8", EM_KMX8)
      .Case("kvarc", EM_KVARC)
      .Case("cdp", EM_CDP)
      .Case("coge", EM_COGE)
      .Case("cool", EM_COOL)
      .Case("norc", EM_NORC)
      .Case("csr_kalimba", EM_CSR_KALIMBA)
      .Case("amdgpu", EM_AMDGPU)
      .Case("riscv", EM_RISCV)
      .Case("lanai", EM_LANAI)
      .Case("bpf", EM_BPF)
      .Case("ve", EM_VE)
      .Case("csky", EM_CSKY)
      .Case("loongarch", EM_LOONGARCH)
      .Default(EM_NONE);
}

/// Convert an ELF's e_machine value into an architecture name.
StringRef ELF::convertEMachineToArchName(uint16_t EMachine) {
  switch (EMachine) {
  case EM_NONE:
    return "None";
  case EM_M32:
    return "m32";
  case EM_SPARC:
    return "sparc";
  case EM_386:
    return "386";
  case EM_68K:
    return "68k";
  case EM_88K:
    return "88k";
  case EM_IAMCU:
    return "iamcu";
  case EM_860:
    return "860";
  case EM_MIPS:
    return "mips";
  case EM_S370:
    return "s370";
  case EM_MIPS_RS3_LE:
    return "mips_rs3_le";
  case EM_PARISC:
    return "parisc";
  case EM_VPP500:
    return "vpp500";
  case EM_SPARC32PLUS:
    return "sparc32plus";
  case EM_960:
    return "960";
  case EM_PPC:
    return "ppc";
  case EM_PPC64:
    return "ppc64";
  case EM_S390:
    return "s390";
  case EM_SPU:
    return "spu";
  case EM_V800:
    return "v800";
  case EM_FR20:
    return "fr20";
  case EM_RH32:
    return "rh32";
  case EM_RCE:
    return "rce";
  case EM_ARM:
    return "arm";
  case EM_ALPHA:
    return "alpha";
  case EM_SH:
    return "sh";
  case EM_SPARCV9:
    return "sparcv9";
  case EM_TRICORE:
    return "tricore";
  case EM_ARC:
    return "arc";
  case EM_H8_300:
    return "h8_300";
  case EM_H8_300H:
    return "h8_300h";
  case EM_H8S:
    return "h8s";
  case EM_H8_500:
    return "h8_500";
  case EM_IA_64:
    return "ia_64";
  case EM_MIPS_X:
    return "mips_x";
  case EM_COLDFIRE:
    return "coldfire";
  case EM_68HC12:
    return "68hc12";
  case EM_MMA:
    return "mma";
  case EM_PCP:
    return "pcp";
  case EM_NCPU:
    return "ncpu";
  case EM_NDR1:
    return "ndr1";
  case EM_STARCORE:
    return "starcore";
  case EM_ME16:
    return "me16";
  case EM_ST100:
    return "st100";
  case EM_TINYJ:
    return "tinyj";
  case EM_X86_64:
    return "x86_64";
  case EM_PDSP:
    return "pdsp";
  case EM_PDP10:
    return "pdp10";
  case EM_PDP11:
    return "pdp11";
  case EM_FX66:
    return "fx66";
  case EM_ST9PLUS:
    return "st9plus";
  case EM_ST7:
    return "st7";
  case EM_68HC16:
    return "68hc16";
  case EM_68HC11:
    return "68hc11";
  case EM_68HC08:
    return "68hc08";
  case EM_68HC05:
    return "68hc05";
  case EM_SVX:
    return "svx";
  case EM_ST19:
    return "st19";
  case EM_VAX:
    return "vax";
  case EM_CRIS:
    return "cris";
  case EM_JAVELIN:
    return "javelin";
  case EM_FIREPATH:
    return "firepath";
  case EM_ZSP:
    return "zsp";
  case EM_MMIX:
    return "mmix";
  case EM_HUANY:
    return "huany";
  case EM_PRISM:
    return "prism";
  case EM_AVR:
    return "avr";
  case EM_FR30:
    return "fr30";
  case EM_D10V:
    return "d10v";
  case EM_D30V:
    return "d30v";
  case EM_V850:
    return "v850";
  case EM_M32R:
    return "m32r";
  case EM_MN10300:
    return "mn10300";
  case EM_MN10200:
    return "mn10200";
  case EM_PJ:
    return "pj";
  case EM_OPENRISC:
    return "openrisc";
  case EM_ARC_COMPACT:
    return "arc_compact";
  case EM_XTENSA:
    return "xtensa";
  case EM_VIDEOCORE:
    return "videocore";
  case EM_TMM_GPP:
    return "tmm_gpp";
  case EM_NS32K:
    return "ns32k";
  case EM_TPC:
    return "tpc";
  case EM_SNP1K:
    return "snp1k";
  case EM_ST200:
    return "st200";
  case EM_IP2K:
    return "ip2k";
  case EM_MAX:
    return "max";
  case EM_CR:
    return "cr";
  case EM_F2MC16:
    return "f2mc16";
  case EM_MSP430:
    return "msp430";
  case EM_BLACKFIN:
    return "blackfin";
  case EM_SE_C33:
    return "se_c33";
  case EM_SEP:
    return "sep";
  case EM_ARCA:
    return "arca";
  case EM_UNICORE:
    return "unicore";
  case EM_EXCESS:
    return "excess";
  case EM_DXP:
    return "dxp";
  case EM_ALTERA_NIOS2:
    return "altera_nios2";
  case EM_CRX:
    return "crx";
  case EM_XGATE:
    return "xgate";
  case EM_C166:
    return "c166";
  case EM_M16C:
    return "m16c";
  case EM_DSPIC30F:
    return "dspic30f";
  case EM_CE:
    return "ce";
  case EM_M32C:
    return "m32c";
  case EM_TSK3000:
    return "tsk3000";
  case EM_RS08:
    return "rs08";
  case EM_SHARC:
    return "sharc";
  case EM_ECOG2:
    return "ecog2";
  case EM_SCORE7:
    return "score7";
  case EM_DSP24:
    return "dsp24";
  case EM_VIDEOCORE3:
    return "videocore3";
  case EM_LATTICEMICO32:
    return "latticemico32";
  case EM_SE_C17:
    return "se_c17";
  case EM_TI_C6000:
    return "ti_c6000";
  case EM_TI_C2000:
    return "ti_c2000";
  case EM_TI_C5500:
    return "ti_c5500";
  case EM_MMDSP_PLUS:
    return "mmdsp_plus";
  case EM_CYPRESS_M8C:
    return "cypress_m8c";
  case EM_R32C:
    return "r32c";
  case EM_TRIMEDIA:
    return "trimedia";
  case EM_HEXAGON:
    return "hexagon";
  case EM_8051:
    return "8051";
  case EM_STXP7X:
    return "stxp7x";
  case EM_NDS32:
    return "nds32";
  case EM_ECOG1:
    return "ecog1";
  case EM_MAXQ30:
    return "maxq30";
  case EM_XIMO16:
    return "ximo16";
  case EM_MANIK:
    return "manik";
  case EM_CRAYNV2:
    return "craynv2";
  case EM_RX:
    return "rx";
  case EM_METAG:
    return "metag";
  case EM_MCST_ELBRUS:
    return "mcst_elbrus";
  case EM_ECOG16:
    return "ecog16";
  case EM_CR16:
    return "cr16";
  case EM_ETPU:
    return "etpu";
  case EM_SLE9X:
    return "sle9x";
  case EM_L10M:
    return "l10m";
  case EM_K10M:
    return "k10m";
  case EM_AARCH64:
    return "AArch64";
  case EM_AVR32:
    return "avr32";
  case EM_STM8:
    return "stm8";
  case EM_TILE64:
    return "tile64";
  case EM_TILEPRO:
    return "tilepro";
  case EM_CUDA:
    return "cuda";
  case EM_TILEGX:
    return "tilegx";
  case EM_CLOUDSHIELD:
    return "cloudshield";
  case EM_COREA_1ST:
    return "corea_1st";
  case EM_COREA_2ND:
    return "corea_2nd";
  case EM_ARC_COMPACT2:
    return "arc_compact2";
  case EM_OPEN8:
    return "open8";
  case EM_RL78:
    return "rl78";
  case EM_VIDEOCORE5:
    return "videocore5";
  case EM_78KOR:
    return "78kor";
  case EM_56800EX:
    return "56800ex";
  case EM_BA1:
    return "ba1";
  case EM_BA2:
    return "ba2";
  case EM_XCORE:
    return "xcore";
  case EM_MCHP_PIC:
    return "mchp_pic";
  case EM_INTEL205:
    return "intel205";
  case EM_INTEL206:
    return "intel206";
  case EM_INTEL207:
    return "intel207";
  case EM_INTEL208:
    return "intel208";
  case EM_INTEL209:
    return "intel209";
  case EM_KM32:
    return "km32";
  case EM_KMX32:
    return "kmx32";
  case EM_KMX16:
    return "kmx16";
  case EM_KMX8:
    return "kmx8";
  case EM_KVARC:
    return "kvarc";
  case EM_CDP:
    return "cdp";
  case EM_COGE:
    return "coge";
  case EM_COOL:
    return "cool";
  case EM_NORC:
    return "norc";
  case EM_CSR_KALIMBA:
    return "csr_kalimba";
  case EM_AMDGPU:
    return "amdgpu";
  case EM_RISCV:
    return "riscv";
  case EM_LANAI:
    return "lanai";
  case EM_BPF:
    return "bpf";
  case EM_VE:
    return "ve";
  case EM_CSKY:
    return "csky";
  case EM_LOONGARCH:
    return "loongarch";
  default:
    return "None";
  }
}
