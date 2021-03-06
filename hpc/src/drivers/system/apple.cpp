// => src/drivers/system/apple.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/drivers/system/apple.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include <sstream>

using namespace hpc;

std::string drivers::AppleDriver::getLinkerPath() {
    return getXcodeToolchainPath() + DarwinDriver::getLinkerPath();
}

int drivers::AppleDriver::invokeLinker(opts::LinkOptions &options) {
    std::ostringstream flags;
    flags << getLinkerPath();
    flags << " -demangle";
    flags << " -dynamic";
    flags << " -arch " << llvm::Triple::getArchTypeName(llvm::Triple(llvm::sys::getDefaultTargetTriple()).getArch()).str();
    
    addOSLinkOptions(options, flags);
    
    flags << " -lc";
    flags << " -lhumanlogic";
    
    if (!options.outputFile.empty())
        flags << " -o " << util::bashEncode(options.outputFile);
    
    for (fsys::File *file : options.inputFiles) {
        flags << " " << util::bashEncode(file->getFileName());
    }
    
    int linkexit = std::system(flags.str().c_str());
    
    return WEXITSTATUS(linkexit);
}

std::string drivers::AppleDriver::getXcodeToolchainPath() {
    return "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain";
}

void drivers::MacOSDriver::addOSLinkOptions(opts::LinkOptions &options, std::ostringstream &flags) {
    flags << " -macosx_version_min 10.13.0"; // FIXME
    
    flags << " " << getXcodeToolchainPath() << "/usr/lib/clang/9.0.0/lib/darwin/libclang_rt.osx.a";
}




