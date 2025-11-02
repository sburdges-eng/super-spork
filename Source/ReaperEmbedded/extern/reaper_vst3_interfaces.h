#pragma once

#include <pluginterfaces/base/funknown.h>

namespace reaper
{
    JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wnon-virtual-dtor")

    class IReaperUIEmbedInterface : public Steinberg::FUnknown
    {
    public:
        virtual Steinberg::TPtrInt embed_message(int msg, Steinberg::TPtrInt parm2, Steinberg::TPtrInt parm3) = 0;

        static const Steinberg::TUID iid;
    };

    class IReaperHostApplication : public Steinberg::FUnknown
    {
    public:
        virtual void* getReaperApi(const char* name) = 0;

        static const Steinberg::TUID iid;
    };

    JUCE_END_IGNORE_WARNINGS_GCC_LIKE
}
