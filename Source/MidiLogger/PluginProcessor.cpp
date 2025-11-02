#include "MidiLogger.h"

//==============================================================================
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiLoggerPluginDemoProcessor();
}
