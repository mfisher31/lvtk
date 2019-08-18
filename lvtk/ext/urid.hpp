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

#include <lvtk/ext/extension.hpp>
#include <lvtk/map.hpp>
#include <lvtk/unmap.hpp>

namespace lvtk {

/** Adds URID `map` and `unmap` to your instance
    @ingroup interfaces
*/
template<class I> 
struct URID : NullExtension
{
    /** @private */
    URID (const FeatureList& features) { 
        for (const auto& f : features) {
            map.set_feature (f);
            unmap.set_feature (f);
        }
    }

protected:
    /** Use this logger to log messages with the host. @see Logger */
    Map map;
    Unmap unmap;
};

}
