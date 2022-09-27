CryptexFixup
==============

[![Build Status](https://github.com/khronokernel/CryptexFixup/workflows/CI/badge.svg?branch=master)](https://github.com/khronokernel/CryptexFixup/actions)

[Lilu](https://github.com/acidanthera/Lilu) Kernel extension for installing Rosetta Cryptex in macOS Ventura. Applicable for both OS installation and updates.

----------

With macOS Ventura, Apple finally dropped the last Mac that lacked the [AVX2.0 CPU instruction](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Advanced_Vector_Extensions_2), the 2013 Trash Can Mac Pro (MacPro6,1). With this, systems lacking AVX2.0 can no longer boot Ventura natively as Apple has stripped the legacy non-AVX2.0 dyld shared caches from the OS. However due to compatibility issues with Rosetta 2, Apple is forced to retain a pre-AVX2.0 dyld shared cache on Apple Silicon systems.

Thus to support older machines, this kext will force the macOS installer/updater to install the Apple Silicon Cryptex (OS.dmg) instead of the stock Intel variant. More information can be found under [macOS Ventura and OpenCore Legacy Patcher Support: Issue 998](https://github.com/dortania/OpenCore-Legacy-Patcher/issues/998). Additionally this kext will disable Cryptex hash verification in APFS.kext.


#### Additional notes:

- Delta Updates will not be supported with patched Cryptexes, Full Updates will be requested instead.
  - Delta: 1-3GB~
  - Full Update: 12GB
- If CryptexFixup determines your system already supports AVX2.0, it will not do anything.
  - Systems supporting AVX2.0 natively:
    - Intel Haswell and newer
    - AMD Excavator/Ryzen and newer
  - Systems lacking AVX2.0:
    - Intel Ivy Bridge and older
    - AMD Bulldozer/Piledriver/Steamroller and older
- This kext does not drop the requirement for AVX2.0 in Ventura's Graphics Stack
  - AMD Polaris, Vega and Navi Drivers in Ventura will not function with AVX2.0 support, end users will need to find alternative ways to achieve graphics acceleration


#### Boot arguments

- `-cryptoff` (or `-liluoff`) to disable
- `-cryptdbg` (or `-liludbgall`) to enable verbose logging (in DEBUG builds)
- `-cryptbeta` (or `-lilubetaall`) to enable on macOS newer than 13
- `-crypt_allow_hash_validation` to disable APFS.kext patching


#### Credits

- [Apple](https://www.apple.com) for macOS
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu)
- [DhinakG](https://github.com/dhinakg) for research and development
- [khronokernel](https://github.com/khronokernel) for research and development
