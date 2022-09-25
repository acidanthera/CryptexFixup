CryptexFixup
==============

[![Build Status](https://github.com/khronokernel/CryptexFixup/workflows/CI/badge.svg?branch=master)](https://github.com/khronokernel/CryptexFixup/actions)

[Lilu](https://github.com/acidanthera/Lilu) Kernel extension for installing Rosetta Cryptex in macOS Ventura. Applicable for both OS installation and updates.

----------

With macOS Ventura, Apple finally dropped the last Mac that lacked the [AVX2.0 CPU instruction](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions#Advanced_Vector_Extensions_2), the 2013 Trash Can Mac Pro (MacPro6,1). With this, systems lacking AVX2.0 can no longer boot Ventura natively as Apple has stripped the legacy non-AVX2.0 dyld shared caches from the OS. However due to compatibility issues with Rosetta 2, Apple is forced to retain a pre-AVX2.0 dyld shared cache on Apple Silicon systems.

Thus to support older machines, this kext will force the macOS installer/updater to install the Apple Silicon Cryptex (OS.dmg) instead of the stock Intel variant. More information can be found under [macOS Ventura and OpenCore Legacy Patcher Support: Issue 998](https://github.com/dortania/OpenCore-Legacy-Patcher/issues/998).

For this kext to work, you'll also need the following Kernel->Patch installed in OpenCore:
```
Arch:        x86_64
Base:        _authenticate_root_hash
Count:       0
Find:       
Identifier:  com.apple.filesystems.apfs
Limit:       0
Mask:       
MaxKernel:  
MinKernel:   22.0.0
Replace:     B800000000C3
ReplaceMask: 
Skip:        0
```

Ommission of this kernel patch will result in APFS.kext Kernel Panicing due to Cryptex hash not matching expected hash.


#### Additional notes:

- Delta Updates will not be supported with patched Cryptexes, full updates will be requested instead.
  - Delta: 1-3GB~
  - Full Update: 12GB
- If CryptexFixup determines your system already supports AVX2.0, it will not do anything.
  - Systems supporting AVX2.0 natively:
    - Intel Haswell (2013) and newer
    - AMD Excavator/Ryzen (2015/2017) and newer
  - Systems lacking AVX2.0:
    - Intel Ivy Bridge and older
    - AMD Bulldozer/Piledriver/Steamroller and older

#### Boot arguments

- `-cryptoff` (or `-liluoff`) to disable
- `-cryptdbg` (or `-liludbgall`) to enable verbose logging (in DEBUG builds)
- `-cryptbeta` (or `-lilubetaall`) to enable on macOS newer than 13


#### Credits

- [Apple](https://www.apple.com) for macOS  
- [vit9696](https://github.com/vit9696) for [Lilu.kext](https://github.com/vit9696/Lilu)
- [DhinakG](https://github.com/dhinakg) for research and development
- [khronokernel](https://github.com/khronokernel) for research and development
