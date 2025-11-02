#include "SurroundProcessor.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SurroundProcessor();
}
