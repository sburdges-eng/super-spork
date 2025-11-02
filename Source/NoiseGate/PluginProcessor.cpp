#include "NoiseGate.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGate();
}
