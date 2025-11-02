#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>

using namespace juce;

//==============================================================================
class NoiseGate final : public AudioProcessor
{
public:
    //==============================================================================
    NoiseGate()
        : AudioProcessor (BusesProperties().withInput  ("Input",     AudioChannelSet::stereo())
                                           .withOutput ("Output",    AudioChannelSet::stereo())
                                           .withInput  ("Sidechain", AudioChannelSet::stereo()))
    {
        addParameter (threshold = new AudioParameterFloat ({ "threshold", 1 }, "Threshold", 0.0f, 1.0f, 0.5f));
        addParameter (alpha     = new AudioParameterFloat ({ "alpha",     1 }, "Alpha",     0.0f, 1.0f, 0.8f));
    }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // the sidechain can take any layout, the main bus needs to be the same on the input and output
        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                 && ! layouts.getMainInputChannelSet().isDisabled();
    }

    //==============================================================================
    void prepareToPlay (double, int) override { lowPassCoeff = 0.0f; sampleCountDown = 0; }
    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        auto mainInputOutput = getBusBuffer (buffer, true, 0);
        auto sideChainInput  = getBusBuffer (buffer, true, 1);

        auto alphaCopy     = alpha->get();
        auto thresholdCopy = threshold->get();

        for (int j = 0; j < buffer.getNumSamples(); ++j)
        {
            auto mixedSamples = 0.0f;

            for (int i = 0; i < sideChainInput.getNumChannels(); ++i)
                mixedSamples += sideChainInput.getReadPointer (i)[j];

            mixedSamples /= static_cast<float> (sideChainInput.getNumChannels());
            lowPassCoeff = (alphaCopy * lowPassCoeff) + ((1.0f - alphaCopy) * mixedSamples);

            if (lowPassCoeff >= thresholdCopy)
                sampleCountDown = (int) getSampleRate();

            // very in-effective way of doing this
            for (int i = 0; i < mainInputOutput.getNumChannels(); ++i)
                *mainInputOutput.getWritePointer (i, j) = sampleCountDown > 0 ? *mainInputOutput.getReadPointer (i, j) : 0.0f;

            if (sampleCountDown > 0)
                --sampleCountDown;
        }
    }

    using AudioProcessor::processBlock;

    //==============================================================================
    AudioProcessorEditor* createEditor() override            { return new GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override                          { return true; }
    const String getName() const override                    { return "NoiseGate"; }
    bool acceptsMidi() const override                        { return false; }
    bool producesMidi() const override                       { return false; }
    double getTailLengthSeconds() const override             { return 0.0; }
    int getNumPrograms() override                            { return 1; }
    int getCurrentProgram() override                         { return 0; }
    void setCurrentProgram (int) override                    {}
    const String getProgramName (int) override               { return "None"; }
    void changeProgramName (int, const String&) override     {}
    bool isVST2() const noexcept                             { return (wrapperType == wrapperType_VST); }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream stream (destData, true);

        stream.writeFloat (*threshold);
        stream.writeFloat (*alpha);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);

        threshold->setValueNotifyingHost (stream.readFloat());
        alpha->setValueNotifyingHost     (stream.readFloat());
    }

private:
    //==============================================================================
    AudioParameterFloat* threshold;
    AudioParameterFloat* alpha;
    int sampleCountDown;

    float lowPassCoeff;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGate)
};
