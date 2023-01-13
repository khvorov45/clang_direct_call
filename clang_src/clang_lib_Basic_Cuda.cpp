#include "clang_include_clang_Basic_Cuda.h"

#include "llvm_include_llvm_ADT_StringRef.h"
#include "llvm_include_llvm_ADT_Twine.h"
#include "llvm_include_llvm_Support_ErrorHandling.h"
#include "llvm_include_llvm_Support_VersionTuple.h"

namespace clang {

struct CudaVersionMapEntry {
  const char *Name;
  CudaVersion Version;
  llvm::VersionTuple TVersion;
};
#define CUDA_ENTRY(major, minor)                                               \
  {                                                                            \
#major "." #minor, CudaVersion::CUDA_##major##minor,                       \
        llvm::VersionTuple(major, minor)                                       \
  }

static const CudaVersionMapEntry CudaNameVersionMap[] = {
    CUDA_ENTRY(7, 0),
    CUDA_ENTRY(7, 5),
    CUDA_ENTRY(8, 0),
    CUDA_ENTRY(9, 0),
    CUDA_ENTRY(9, 1),
    CUDA_ENTRY(9, 2),
    CUDA_ENTRY(10, 0),
    CUDA_ENTRY(10, 1),
    CUDA_ENTRY(10, 2),
    CUDA_ENTRY(11, 0),
    CUDA_ENTRY(11, 1),
    CUDA_ENTRY(11, 2),
    CUDA_ENTRY(11, 3),
    CUDA_ENTRY(11, 4),
    CUDA_ENTRY(11, 5),
    CUDA_ENTRY(11, 6),
    CUDA_ENTRY(11, 7),
    CUDA_ENTRY(11, 8),
    {"", CudaVersion::NEW, llvm::VersionTuple(std::numeric_limits<int>::max())},
    {"unknown", CudaVersion::UNKNOWN, {}} // End of list tombstone.
};
#undef CUDA_ENTRY

const char *CudaVersionToString(CudaVersion V) {
  for (auto *I = CudaNameVersionMap; I->Version != CudaVersion::UNKNOWN; ++I)
    if (I->Version == V)
      return I->Name;

  return CudaVersionToString(CudaVersion::UNKNOWN);
}

CudaVersion CudaStringToVersion(const llvm::Twine &S) {
  std::string VS = S.str();
  for (auto *I = CudaNameVersionMap; I->Version != CudaVersion::UNKNOWN; ++I)
    if (I->Name == VS)
      return I->Version;
  return CudaVersion::UNKNOWN;
}

CudaVersion ToCudaVersion(llvm::VersionTuple Version) {
  for (auto *I = CudaNameVersionMap; I->Version != CudaVersion::UNKNOWN; ++I)
    if (I->TVersion == Version)
      return I->Version;
  return CudaVersion::UNKNOWN;
}

namespace {
struct CudaArchToStringMap {
  CudaArch arch;
  const char *arch_name;
  const char *virtual_arch_name;
};
} // namespace

#define SM2(sm, ca)                                                            \
  { CudaArch::SM_##sm, "sm_" #sm, ca }
#define SM(sm) SM2(sm, "compute_" #sm)
#define GFX(gpu)                                                               \
  { CudaArch::GFX##gpu, "gfx" #gpu, "compute_amdgcn" }
static const CudaArchToStringMap arch_names[] = {
    // clang-format off
    {CudaArch::UNUSED, "", ""},
    SM2(20, "compute_20"), SM2(21, "compute_20"), // Fermi
    SM(30), SM(32), SM(35), SM(37),  // Kepler
    SM(50), SM(52), SM(53),          // Maxwell
    SM(60), SM(61), SM(62),          // Pascal
    SM(70), SM(72),                  // Volta
    SM(75),                          // Turing
    SM(80), SM(86),                  // Ampere
    SM(87),                          // Jetson/Drive AGX Orin
    SM(89),                          // Ada Lovelace
    SM(90),                          // Hopper
    GFX(600),  // gfx600
    GFX(601),  // gfx601
    GFX(602),  // gfx602
    GFX(700),  // gfx700
    GFX(701),  // gfx701
    GFX(702),  // gfx702
    GFX(703),  // gfx703
    GFX(704),  // gfx704
    GFX(705),  // gfx705
    GFX(801),  // gfx801
    GFX(802),  // gfx802
    GFX(803),  // gfx803
    GFX(805),  // gfx805
    GFX(810),  // gfx810
    GFX(900),  // gfx900
    GFX(902),  // gfx902
    GFX(904),  // gfx903
    GFX(906),  // gfx906
    GFX(908),  // gfx908
    GFX(909),  // gfx909
    GFX(90a),  // gfx90a
    GFX(90c),  // gfx90c
    GFX(940),  // gfx940
    GFX(1010), // gfx1010
    GFX(1011), // gfx1011
    GFX(1012), // gfx1012
    GFX(1013), // gfx1013
    GFX(1030), // gfx1030
    GFX(1031), // gfx1031
    GFX(1032), // gfx1032
    GFX(1033), // gfx1033
    GFX(1034), // gfx1034
    GFX(1035), // gfx1035
    GFX(1036), // gfx1036
    GFX(1100), // gfx1100
    GFX(1101), // gfx1101
    GFX(1102), // gfx1102
    GFX(1103), // gfx1103
    {CudaArch::Generic, "generic", ""},
    // clang-format on
};
#undef SM
#undef SM2
#undef GFX

const char *CudaArchToString(CudaArch A) {
  auto result = std::find_if(
      std::begin(arch_names), std::end(arch_names),
      [A](const CudaArchToStringMap &map) { return A == map.arch; });
  if (result == std::end(arch_names))
    return "unknown";
  return result->arch_name;
}

const char *CudaArchToVirtualArchString(CudaArch A) {
  auto result = std::find_if(
      std::begin(arch_names), std::end(arch_names),
      [A](const CudaArchToStringMap &map) { return A == map.arch; });
  if (result == std::end(arch_names))
    return "unknown";
  return result->virtual_arch_name;
}

CudaArch StringToCudaArch(llvm::StringRef S) {
  auto result = std::find_if(
      std::begin(arch_names), std::end(arch_names),
      [S](const CudaArchToStringMap &map) { return S == map.arch_name; });
  if (result == std::end(arch_names))
    return CudaArch::UNKNOWN;
  return result->arch;
}

CudaVersion MinVersionForCudaArch(CudaArch A) {
  if (A == CudaArch::UNKNOWN)
    return CudaVersion::UNKNOWN;

  // AMD GPUs do not depend on CUDA versions.
  if (IsAMDGpuArch(A))
    return CudaVersion::CUDA_70;

  switch (A) {
  case CudaArch::SM_20:
  case CudaArch::SM_21:
  case CudaArch::SM_30:
  case CudaArch::SM_32:
  case CudaArch::SM_35:
  case CudaArch::SM_37:
  case CudaArch::SM_50:
  case CudaArch::SM_52:
  case CudaArch::SM_53:
    return CudaVersion::CUDA_70;
  case CudaArch::SM_60:
  case CudaArch::SM_61:
  case CudaArch::SM_62:
    return CudaVersion::CUDA_80;
  case CudaArch::SM_70:
    return CudaVersion::CUDA_90;
  case CudaArch::SM_72:
    return CudaVersion::CUDA_91;
  case CudaArch::SM_75:
    return CudaVersion::CUDA_100;
  case CudaArch::SM_80:
    return CudaVersion::CUDA_110;
  case CudaArch::SM_86:
    return CudaVersion::CUDA_111;
  case CudaArch::SM_87:
    return CudaVersion::CUDA_114;
  case CudaArch::SM_89:
  case CudaArch::SM_90:
    return CudaVersion::CUDA_118;
  default:
    llvm_unreachable("invalid enum");
  }
}

CudaVersion MaxVersionForCudaArch(CudaArch A) {
  // AMD GPUs do not depend on CUDA versions.
  if (IsAMDGpuArch(A))
    return CudaVersion::NEW;

  switch (A) {
  case CudaArch::UNKNOWN:
    return CudaVersion::UNKNOWN;
  case CudaArch::SM_20:
  case CudaArch::SM_21:
    return CudaVersion::CUDA_80;
  case CudaArch::SM_30:
    return CudaVersion::CUDA_110;
  default:
    return CudaVersion::NEW;
  }
}

bool CudaFeatureEnabled(llvm::VersionTuple  Version, CudaFeature Feature) {
  return CudaFeatureEnabled(ToCudaVersion(Version), Feature);
}

bool CudaFeatureEnabled(CudaVersion Version, CudaFeature Feature) {
  switch (Feature) {
  case CudaFeature::CUDA_USES_NEW_LAUNCH:
    return Version >= CudaVersion::CUDA_92;
  case CudaFeature::CUDA_USES_FATBIN_REGISTER_END:
    return Version >= CudaVersion::CUDA_101;
  }
  llvm_unreachable("Unknown CUDA feature.");
}
} // namespace clang
