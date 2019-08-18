/* 
    Copyright (c) 2019, Michael Fisher <mfisher@kushview.net>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once

#include <lvtk/lvtk.hpp>
#include <lv2/dynmanifest/dynmanifest.h>
#include <sstream>
#include <cstdio>

namespace lvtk {

/** Dynamic Manifest helper class
    
    To create a dynamic manifest, subclass this an implement it's pure virtual
    methods. Then implement lvtk_create_dyn_manifest().
    @headerfile lvtk/dynmanifest.hpp
 */
class DynManifest
{
public:
    DynManifest() = default;
    virtual ~DynManifest() = default;

    /** Get the subjects
        @param lines    Add each line to this string vector
     */
    virtual void subjects (std::vector<std::string>& lines) =0;

    /** Get the subjects
        @param uri      The subject URI to get data for
        @param lines    Add each line to this string vector
     */
    virtual void get_data (const std::string& uri, std::vector<std::string>& lines) =0;
    
    /** @internal used to send data back to the host */
    static void write_lines (const std::vector<std::string>& lines, FILE* fp) {
        for (const auto& l : lines) {
            std::string line = l + "\n";
            fwrite (line.c_str(), sizeof(char), line.size(), fp);
        }
    }
};

}

/** Implement this and return your subclassed DynManifest object */
lvtk::DynManifest* lvtk_create_dyn_manifest();

extern "C" {

LV2_SYMBOL_EXPORT
int lv2_dyn_manifest_open (LV2_Dyn_Manifest_Handle *handle, const LV2_Feature *const *features)
{
    auto* manifest = lvtk_create_dyn_manifest();
    *handle = static_cast<LV2_Dyn_Manifest_Handle> (manifest);
    return 0;
}

LV2_SYMBOL_EXPORT
int lv2_dyn_manifest_get_subjects (LV2_Dyn_Manifest_Handle handle, FILE *fp)
{
    auto* manifest = static_cast<lvtk::DynManifest*> (handle);
    std::vector<std::string> lines;
    manifest->subjects (lines);
    lvtk::DynManifest::write_lines (lines, fp);
    return 0;
}

LV2_SYMBOL_EXPORT
int lv2_dyn_manifest_get_data (LV2_Dyn_Manifest_Handle handle, FILE *fp, const char *uri)
{
    auto* manifest = static_cast<lvtk::DynManifest*> (handle);
    std::vector<std::string> lines;
    manifest->get_data (uri, lines);
    lvtk::DynManifest::write_lines (lines, fp);
    return 0;
}

LV2_SYMBOL_EXPORT
void lv2_dyn_manifest_close (LV2_Dyn_Manifest_Handle handle)
{
    delete static_cast<lvtk::DynManifest*> (handle);
}

}