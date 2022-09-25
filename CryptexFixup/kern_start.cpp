//
//  kern_start.cpp
//  CryptexFixup.kext
//
//  Copyright © 2022 Mykola Grymalyuk. All rights reserved.
//

#include <Headers/plugin_start.hpp>
#include <Headers/kern_api.hpp>
#include <Headers/kern_user.hpp>
#include <Headers/kern_devinfo.hpp>

#define MODULE_SHORT "crypt_fix"

static mach_vm_address_t orig_cs_validate {};
static mach_vm_address_t orig_root_hash_validate {};

static const char *ramrodPath = "UpdateBrainLibrary";

static const uint8_t kCryptexFind[] = {
    // cryptex-system-x86_64
    0x63, 0x72, 0x79, 0x70, 0x74, 0x65, 0x78, 0x2D,
    0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2D,
    0x78, 0x38, 0x36, 0x5F, 0x36, 0x34
};

static const uint8_t kCryptexReplace[] = {
    // cryptex-system-arm64e
    0x63, 0x72, 0x79, 0x70, 0x74, 0x65, 0x78, 0x2D,
    0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x2D,
    0x61, 0x72, 0x6D, 0x36, 0x34, 0x65
};


#pragma mark - Kernel patching code

template <size_t findSize, size_t replaceSize>
static inline void searchAndPatch(const void *haystack, size_t haystackSize, const char *path, const uint8_t (&needle)[findSize], const uint8_t (&patch)[replaceSize], const char *name) {
    if (UNLIKELY(KernelPatcher::findAndReplace(const_cast<void *>(haystack), haystackSize, needle, findSize, patch, replaceSize))) {
        DBGLOG(MODULE_SHORT, "found function %s to patch at %s!", name, path);
    }
}


#pragma mark - Patched functions

static void patched_cs_validate_page(vnode_t vp, memory_object_t pager, memory_object_offset_t page_offset, const void *data, int *validated_p, int *tainted_p, int *nx_p) {
    char path[PATH_MAX];
    int pathlen = PATH_MAX;
    FunctionCast(patched_cs_validate_page, orig_cs_validate)(vp, pager, page_offset, data, validated_p, tainted_p, nx_p);

    if (vn_getpath(vp, path, &pathlen) == 0) {
        // Binary is copied into a tmp location, thus partial match
        if (UNLIKELY(strstr(path, ramrodPath) != NULL)) {
            searchAndPatch(data, PAGE_SIZE, path, kCryptexFind, kCryptexReplace, "Cryptex Disk Image");
        }
    }
}

#pragma mark - Patches on start/stop

static void pluginStart() {
	DBGLOG(MODULE_SHORT, "start");
	lilu.onPatcherLoadForce([](void *user, KernelPatcher &patcher) {
        if (BaseDeviceInfo::get().cpuHasAvx2) {
            SYSLOG(MODULE_SHORT, "system natively support AVX2.0, skipping");
            return;
        }
        
        KernelPatcher::RouteRequest csRoute = KernelPatcher::RouteRequest("_cs_validate_page", patched_cs_validate_page, orig_cs_validate);
        if (!patcher.routeMultipleLong(KernelPatcher::KernelID, &csRoute, 1))
            SYSLOG(MODULE_SHORT, "failed to route cs validation pages");
	});
}

// Boot args.
static const char *bootargOff[] {
    "-cryptoff"
};
static const char *bootargDebug[] {
    "-cryptdbg"
};
static const char *bootargBeta[] {
    "-cryptbeta"
};

// Plugin configuration.
PluginConfiguration ADDPR(config) {
    xStringify(PRODUCT_NAME),
    parseModuleVersion(xStringify(MODULE_VERSION)),
    LiluAPI::AllowNormal | LiluAPI::AllowInstallerRecovery | LiluAPI::AllowSafeMode,
    bootargOff,
    arrsize(bootargOff),
    bootargDebug,
    arrsize(bootargDebug),
    bootargBeta,
    arrsize(bootargBeta),
    KernelVersion::Ventura,
    KernelVersion::Ventura,
    pluginStart
};
