#include "MultiOutSynth.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiOutSynth();
}
