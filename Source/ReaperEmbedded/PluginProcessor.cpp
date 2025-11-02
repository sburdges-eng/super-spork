#include "ReaperEmbedded.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReaperEmbeddedViewDemo();
}
