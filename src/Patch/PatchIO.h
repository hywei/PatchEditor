#ifndef PATCHIO_H_
#define PATCHIO_H_

#include <string>

namespace PolyPatch{
    class PolyPatch;
    namespace IO{
        bool ReadPatch(PolyPatch& _patch, const std::string& patch_file_name);
        bool WritePatch(const PolyPatch&, const std::string& patch_file_name);
    }
}

#endif
