/*
 * ui_features.hpp
 *
 *  Created on: Dec 22, 2012
 *      Author: mfisher
 */

#ifndef UI_FEATURES_HPP_
#define UI_FEATURES_HPP_

namespace lvtk {

    /** The UIParent Mixin.
        @ingroup guimixins
        @see The internal struct I for details.
     */
    template<bool Required = false>
    struct UITouch
    {
        template<class Derived>
        struct I : Extension<Required>
        {
            I()
            {
                m_touch.handle = NULL;
                m_touch.touch  = NULL;
            }

            /** @internal */
            static void
            map_feature_handlers (FeatureHandlerMap& hmap)
            {
                hmap[LV2_UI__touch] = &I<Derived>::handle_feature;
            }

            /** @internal */
            static void
            handle_feature (void* instance, void* data)
            {
                Derived* d = reinterpret_cast<Derived*>(instance);
                I<Derived>* mixin = static_cast<I<Derived>*>(d);

                LV2UI_Touch* t (reinterpret_cast<LV2_Widget*>(data));
                mixin->m_touch.handle = t->handle;
                mixin->m_touch.touch  = t->touch;
                mixin->m_ok = true;
            }

            bool
            check_ok()
            {
                if (Required == false) {
                   this->m_ok = true;
                } else {
                   this->m_ok = p_resize != 0;
                }

                if (LVTK_DEBUG)
                {
                    std::clog << "    [UITouch] Validation "
                              << (this->m_ok ? "succeeded" : "failed")
                              << "." << std::endl;
                }

                return this->m_ok;
            }

        protected:

            void
            touch (uint32_t port, bool grabbed)
            {
                if (m_touch.handle != NULL && m_touch.touch  != NULL)
                    m_touch.touch (m_touch.handle, port, grabbed);
            }

        private:
            LV2UI_Touch m_touch;

        };
    };


    /** The UIParent Mixin.
        @ingroup guimixins
        @see The internal struct I for details.
     */
    template<bool Required = false>
    struct UIParent
    {
        template<class Derived>
        struct I : Extension<Required>
        {
            I() : p_parent (NULL) { }

            /** @internal */
            static void
            map_feature_handlers(FeatureHandlerMap& hmap)
            {
                hmap[LV2_UI__parent] = &I<Derived>::handle_feature;
            }

            /** @internal */
            static void
            handle_feature(void* instance, void* data)
            {
                Derived* d = reinterpret_cast<Derived*>(instance);
                I<Derived>* mixin = static_cast<I<Derived>*>(d);

                mixin->p_parent = reinterpret_cast<LV2_Widget*>(data);
                mixin->m_ok = true;
            }

            bool
            check_ok()
            {
                if (Required == false) {
                   this->m_ok = true;
                } else {
                   this->m_ok = p_resize != 0;
                }

                if (LVTK_DEBUG)
                {
                    std::clog << "    [UIParent] Validation "
                            << (this->m_ok ? "succeeded" : "failed")
                            << "." << std::endl;
                }
                return this->m_ok;
            }

        protected:

            /** Get the parent widget if any

                @return The parent LV2_Widget or NULL if not provided
            */
            LV2_Widget*
            get_parent()
            {
               return p_parent;
            }

        private:

            LV2_Widget* p_parent;

        };
    };

    /** The UIResize Mixin.
        @ingroup guimixins
        @see The internal struct I for details.
     */
    template<bool Required = true>
    struct UIResize
    {
        template<class Derived>
        struct I : Extension<Required>
        {
            I()
            {
                m_resize.handle    = NULL;
                m_resize.ui_resize = NULL;
            }

            /** @internal */
            static void
            map_feature_handlers(FeatureHandlerMap& hmap)
            {
                hmap[LV2_UI__resize] = &I<Derived>::handle_feature;
            }

            /** @internal */
            static void
            handle_feature(void* instance, void* data)
            {
                Derived* d = reinterpret_cast<Derived*>(instance);
                I<Derived>* mixin = static_cast<I<Derived>*>(d);

                LV2UI_Resize* rz (reinterpret_cast<LV2UI_Resize*> (data));
                mixin->m_resize.handle    = rz->handle;
                mixin->m_resize.ui_resize = rz->ui_resize;
                mixin->m_ok = true;
            }

            bool
            check_ok()
            {
                if (Required == false) {
                   this->m_ok = true;
                } else {
                   this->m_ok = (m_resize.ui_resize != NULL && m_resize.handle != NULL);
                }

                if (LVTK_DEBUG)
                {
                    std::clog << "    [UIResize] Validation "
                              << (this->m_ok ? "succeeded" : "failed")
                              << "." << std::endl;
                }
                return this->m_ok;
            }

        protected:

            /** */

            bool
            ui_resize(int width, int height)
            {
                if (m_resize.ui_resize != NULL && m_resize.handle != NULL)
                   return (0 == m_resize.ui_resize (m_resize.handle, width, height));

                return false;
            }

        private:
            LV2UI_Resize m_resize;

        };
    };

}

#endif /* UI_FEATURES_HPP_ */
