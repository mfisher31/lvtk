/*
    options.hpp - Support file for writing LV2 plugins in C++

    Copyright (C) 2013 Michael Fisher <mfisher31@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 01222-1307  USA
*/

#pragma once

#include <lvtk/lvtk.hpp>
#include <lv2/lv2plug.in/ns/ext/options/options.h>

namespace lvtk {

/** Convenience enum to get LV2_Options_Option into a C++ namespace */
using Option = LV2_Options_Option;

/** Equivalent to LV2_Options_Context */
enum OptionsContext {
    /** This option applies to the instance itself.  The subject must be
        ignored. */
    OPTIONS_INSTANCE    = LV2_OPTIONS_INSTANCE,

    /** This option applies to some named resource.  The subject is a URI mapped
        to an integer (a LV2_URID, like the key) */
    OPTIONS_RESOURCE    = LV2_OPTIONS_RESOURCE,

    /** This option applies to some blank node.  The subject is a blank node
        identifier, which is valid only within the current local scope. */
    OPTIONS_BLANK       = LV2_OPTIONS_BLANK,

    /** This option applies to a port on the instance.  The subject is the
        port's index. */
    OPTIONS_PORT        = LV2_OPTIONS_PORT
};

/** Equivalent to LV2_Options_Status */
enum OptionsStatus {
    OPTIONS_SUCCESS         = LV2_OPTIONS_SUCCESS,          /**< Completed successfully. */
    OPTIONS_ERR_UNKNOWN     = LV2_OPTIONS_ERR_UNKNOWN,      /**< Unknown error. */
    OPTIONS_ERR_BAD_SUBJECT = LV2_OPTIONS_ERR_BAD_SUBJECT,  /**< Invalid/unsupported subject. */
    OPTIONS_ERR_BAD_KEY     = LV2_OPTIONS_ERR_BAD_KEY,      /**< Invalid/unsupported key. */
    OPTIONS_ERR_BAD_VALUE   = LV2_OPTIONS_ERR_BAD_VALUE     /**< Invalid/unsupported value. */
};

class OptionArray
{
public:
    OptionArray() {
        opts = (Option*) malloc (sizeof (Option));
        memset (&opts[0], 0, sizeof (Option));
        allocated = true;
        count = 1;
    }

    ~OptionArray() {
        if (allocated && opts != nullptr) {
            std::free (opts);
            opts = nullptr;
        }
    }

    void add (lvtk::OptionsContext  context, 
              uint32_t              subject, 
              LV2_URID              key,
              uint32_t              size, 
              LV2_URID              type,
              const void*           value)
    {
        add (static_cast<LV2_Options_Context> (context),
             subject, key, size, type, value);
    }

    void add (LV2_Options_Context   context, 
              uint32_t              subject, 
              LV2_URID              key,
              uint32_t              size, 
              LV2_URID              type,
              const void*           value)
    {
        if (! allocated)
            return;
        opts = (Option*) realloc (opts, ++count * sizeof (Option));
        memset (&opts[count - 1], 0, sizeof (Option));
        auto& opt = opts [count - 2];
        opt.context     = context;
        opt.subject     = subject;
        opt.key         = key;
        opt.size        = size;
        opt.type        = type;
        opt.value       = value;
    }

    size_t size() const { return count - 1; }
    const Option* c_obj() const { return opts; }

private:
    bool allocated = false;
    uint32_t count = 0;
    Option* opts = nullptr;
};

/** An simple iterator to use with a plain array of LV2_Options_Option's

    @code
        OptionIterator iter (get_options());
        while (const Option* opt = iter.next())
        {
            // handle the option
        }
    @endcode
*/
class OptionIterator
{
public:
    /** Create a new iterator
        @param options  The options array to scan
     */
    OptionIterator (const Option* options)
        : index (0),m_size (0), p_opts (options)
    {
        while (0 != next())
            ++m_size;
        index = 0;
    }

    /** Returns the next option or NULL if the end of the list is reached */
    const Option* next() const
    {
        if (p_opts == 0 || (p_opts[index].key   == 0 &&
                                p_opts[index].value == 0))
            return 0;

        return &p_opts [index++];
    }

    /** Returns the total number of options */
    uint32_t size() const { return m_size; }

private:
    mutable uint32_t   index = 0;
    uint32_t           m_size = 0;
    const Option*      p_opts = nullptr;
};

/** The LV2 Options Feature
 
    @headerfile lvtk/host_options.hpp
 */
struct HostOptions final {
    HostOptions() = default;

    /** Get the options passed by the host as an LV2_Feature

        @note The options array MUST NOT be modified by the plugin instance
        @return The options array or 0 if no options were supplied
     */
    const Option* get_options() const { return p_host_options; }

    /** Assign options from a feature
        @param feature  Should be a feature with LV2_OPTIONS__options
                        as the URI.
        
        @returns True if the feature data was set
     */
    bool set_feature (const Feature& feature) {
        if (strcmp (LV2_OPTIONS__options, feature.URI) == 0) {
            p_host_options = (Option*) feature.data;
            return true;
        }
        return false;
    }

private:
    Option* p_host_options = nullptr;
};

} /* namespace lvtk */
