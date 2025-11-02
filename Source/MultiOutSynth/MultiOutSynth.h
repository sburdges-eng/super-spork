#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "../Assets/DemoUtilities.h"

using namespace juce;

//==============================================================================
class MultiOutSynth final : public AudioProcessor
{
public:
    enum
    {
        maxMidiChannel    = 16,
        maxNumberOfVoices = 5
    };

    //==============================================================================
    MultiOutSynth()
        : AudioProcessor (BusesProperties()
                          .withOutput ("Output #1",  AudioChannelSet::stereo(), true)
                          .withOutput ("Output #2",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #3",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #4",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #5",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #6",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #7",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #8",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #9",  AudioChannelSet::stereo(), false)
                          .withOutput ("Output #10", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #11", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #12", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #13", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #14", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #15", AudioChannelSet::stereo(), false)
                          .withOutput ("Output #16", AudioChannelSet::stereo(), false))
    {
        // initialize other stuff (not related to buses)
        formatManager.registerBasicFormats();

        for (int midiChannel = 0; midiChannel < maxMidiChannel; ++midiChannel)
        {
            synth.add (new Synthesiser());

            for (int i = 0; i < maxNumberOfVoices; ++i)
                synth[midiChannel]->addVoice (new SamplerVoice());
        }

        loadNewSample (createAssetInputStream ("singing.ogg"), "wav");
    }

    //==============================================================================
    bool canAddBus    (bool isInput) const override   { return ! isInput; }
    bool canRemoveBus (bool isInput) const override   { return ! isInput; }

    //==============================================================================
    void prepareToPlay (double newSampleRate, int samplesPerBlock) override
    {
        ignoreUnused (samplesPerBlock);

        for (auto* s : synth)
            s->setCurrentPlaybackSampleRate (newSampleRate);
    }

    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiBuffer) override
    {
        auto busCount = getBusCount (false);

        for (auto busNr = 0; busNr < busCount; ++busNr)
        {
            if (synth.size() <= busNr)
                continue;

            auto midiChannelBuffer = filterMidiMessagesForChannel (midiBuffer, busNr + 1);
            auto audioBusBuffer = getBusBuffer (buffer, false, busNr);

            // Voices add to the contents of the buffer. Make sure the buffer is clear before
            // rendering, just in case the host left old data in the buffer.
            audioBusBuffer.clear();

            synth [busNr]->renderNextBlock (audioBusBuffer, midiChannelBuffer, 0, audioBusBuffer.getNumSamples());
        }
    }

    using AudioProcessor::processBlock;

    //==============================================================================
    AudioProcessorEditor* createEditor() override          { return new GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override                        { return true; }

    //==============================================================================
    const String getName() const override                  { return "Multi Out Synth PlugIn"; }
    bool acceptsMidi() const override                      { return true; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0.0; }
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const String&) override   {}

    bool isBusesLayoutSupported (const BusesLayout& layout) const override
    {
        const auto& outputs = layout.outputBuses;

        return layout.inputBuses.isEmpty()
            && 1 <= outputs.size()
            && std::all_of (outputs.begin(), outputs.end(), [] (const auto& bus)
               {
                   return bus.isDisabled() || bus == AudioChannelSet::stereo();
               });
    }

    //==============================================================================
    void getStateInformation (MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

private:
    //==============================================================================
    static MidiBuffer filterMidiMessagesForChannel (const MidiBuffer& input, int channel)
    {
        MidiBuffer output;

        for (const auto metadata : input)
        {
            const auto message = metadata.getMessage();

            if (message.getChannel() == channel)
                output.addEvent (message, metadata.samplePosition);
        }

        return output;
    }

    void loadNewSample (std::unique_ptr<InputStream> soundBuffer, const char* format)
    {
        std::unique_ptr<AudioFormatReader> formatReader (formatManager.findFormatForFileExtension (format)->createReaderFor (soundBuffer.release(), true));

        BigInteger midiNotes;
        midiNotes.setRange (0, 126, true);
        SynthesiserSound::Ptr newSound = new SamplerSound ("Voice", *formatReader, midiNotes, 0x40, 0.0, 0.0, 10.0);

        for (auto* s : synth)
            s->removeSound (0);

        sound = newSound;

        for (auto* s : synth)
            s->addSound (sound);
    }

    //==============================================================================
    AudioFormatManager formatManager;
    OwnedArray<Synthesiser> synth;
    SynthesiserSound::Ptr sound;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiOutSynth)
};
