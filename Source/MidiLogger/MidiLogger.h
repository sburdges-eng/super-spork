#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <iterator>

using namespace juce;

class MidiQueue
{
public:
    void push (const MidiBuffer& buffer)
    {
        for (const auto metadata : buffer)
            fifo.write (1).forEach ([&] (int dest) { messages[(size_t) dest] = metadata.getMessage(); });
    }

    template <typename OutputIt>
    void pop (OutputIt out)
    {
        fifo.read (fifo.getNumReady()).forEach ([&] (int source) { *out++ = messages[(size_t) source]; });
    }

private:
    static constexpr auto queueSize = 1 << 14;
    AbstractFifo fifo { queueSize };
    std::vector<MidiMessage> messages = std::vector<MidiMessage> (queueSize);
};

// Stores the last N messages. Safe to access from the message thread only.
class MidiListModel
{
public:
    template <typename It>
    void addMessages (It begin, It end)
    {
        if (begin == end)
            return;

        const auto numNewMessages = (int) std::distance (begin, end);
        const auto numToAdd = juce::jmin (numToStore, numNewMessages);
        const auto numToRemove = jmax (0, (int) messages.size() + numToAdd - numToStore);
        messages.erase (messages.begin(), std::next (messages.begin(), numToRemove));
        messages.insert (messages.end(), std::prev (end, numToAdd), end);

        NullCheckedInvocation::invoke (onChange);
    }

    void clear()
    {
        messages.clear();

        NullCheckedInvocation::invoke (onChange);
    }

    const MidiMessage& operator[] (size_t ind) const     { return messages[ind]; }

    size_t size() const                                  { return messages.size(); }

    std::function<void()> onChange;

private:
    static constexpr auto numToStore = 1000;
    std::vector<MidiMessage> messages;
};

//==============================================================================
class MidiTable final : public Component,
                        private TableListBoxModel
{
public:
    MidiTable (MidiListModel& m)
        : messages (m)
    {
        addAndMakeVisible (table);

        table.setModel (this);
        table.setClickingTogglesRowSelection (false);
        table.setHeader ([&]
        {
            auto header = std::make_unique<TableHeaderComponent>();
            header->addColumn ("Message", messageColumn, 200, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Time",    timeColumn,    100, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Channel", channelColumn, 100, 30, -1, TableHeaderComponent::notSortable);
            header->addColumn ("Data",    dataColumn,    200, 30, -1, TableHeaderComponent::notSortable);
            return header;
        }());

        messages.onChange = [&] { table.updateContent(); };
    }

    ~MidiTable() override { messages.onChange = nullptr; }

    void resized() override { table.setBounds (getLocalBounds()); }

private:
    enum
    {
        messageColumn = 1,
        timeColumn,
        channelColumn,
        dataColumn
    };

    int getNumRows() override          { return (int) messages.size(); }

    void paintRowBackground (Graphics&, int, int, int, bool) override {}
    void paintCell (Graphics&, int, int, int, int, bool)     override {}

    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool,
                                        Component* existingComponentToUpdate) override
    {
        delete existingComponentToUpdate;

        const auto index = (int) messages.size() - 1 - rowNumber;
        const auto message = messages[(size_t) index];

        return new Label ({}, [&]
        {
            switch (columnId)
            {
                case messageColumn: return getEventString (message);
                case timeColumn:    return String (message.getTimeStamp());
                case channelColumn: return String (message.getChannel());
                case dataColumn:    return getDataString (message);
                default:            break;
            }

            jassertfalse;
            return String();
        }());
    }

    static String getEventString (const MidiMessage& m)
    {
        if (m.isNoteOn())           return "Note on";
        if (m.isNoteOff())          return "Note off";
        if (m.isProgramChange())    return "Program change";
        if (m.isPitchWheel())       return "Pitch wheel";
        if (m.isAftertouch())       return "Aftertouch";
        if (m.isChannelPressure())  return "Channel pressure";
        if (m.isAllNotesOff())      return "All notes off";
        if (m.isAllSoundOff())      return "All sound off";
        if (m.isMetaEvent())        return "Meta event";

        if (m.isController())
        {
            const auto* name = MidiMessage::getControllerName (m.getControllerNumber());
            return "Controller " + (name == nullptr ? String (m.getControllerNumber()) : String (name));
        }

        return String::toHexString (m.getRawData(), m.getRawDataSize());
    }

    static String getDataString (const MidiMessage& m)
    {
        if (m.isNoteOn())           return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) + " Velocity " + String (m.getVelocity());
        if (m.isNoteOff())          return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) + " Velocity " + String (m.getVelocity());
        if (m.isProgramChange())    return String (m.getProgramChangeNumber());
        if (m.isPitchWheel())       return String (m.getPitchWheelValue());
        if (m.isAftertouch())       return MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
        if (m.isChannelPressure())  return String (m.getChannelPressureValue());
        if (m.isController())       return String (m.getControllerValue());

        return {};
    }

    MidiListModel& messages;
    TableListBox table;
};

//==============================================================================
class MidiLoggerPluginDemoProcessor final : public AudioProcessor,
                                            private Timer
{
public:
    MidiLoggerPluginDemoProcessor()
        : AudioProcessor (getBusesLayout())
    {
        state.addChild ({ "uiState", { { "width",  600 }, { "height", 300 } }, {} }, -1, nullptr);
        startTimerHz (60);
    }

    ~MidiLoggerPluginDemoProcessor() override { stopTimer(); }

    void processBlock (AudioBuffer<float>& audio,  MidiBuffer& midi) override { process (audio, midi); }
    void processBlock (AudioBuffer<double>& audio, MidiBuffer& midi) override { process (audio, midi); }

    bool isBusesLayoutSupported (const BusesLayout&) const override           { return true; }
    bool isMidiEffect() const override                                        { return true; }
    bool hasEditor() const override                                           { return true; }
    AudioProcessorEditor* createEditor() override                             { return new Editor (*this); }

    const String getName() const override                                     { return "MIDI Logger"; }
    bool acceptsMidi() const override                                         { return true; }
    bool producesMidi() const override                                        { return true; }
    double getTailLengthSeconds() const override                              { return 0.0; }

    int getNumPrograms() override                                             { return 0; }
    int getCurrentProgram() override                                          { return 0; }
    void setCurrentProgram (int) override                                     {}
    const String getProgramName (int) override                                { return "None"; }
    void changeProgramName (int, const String&) override                      {}

    void prepareToPlay (double, int) override                                 {}
    void releaseResources() override                                          {}

    void getStateInformation (MemoryBlock& destData) override
    {
        if (auto xmlState = state.createXml())
            copyXmlToBinary (*xmlState, destData);
    }

    void setStateInformation (const void* data, int size) override
    {
        if (auto xmlState = getXmlFromBinary (data, size))
            state = ValueTree::fromXml (*xmlState);
    }

private:
    class Editor final : public AudioProcessorEditor,
                         private Value::Listener
    {
    public:
        explicit Editor (MidiLoggerPluginDemoProcessor& ownerIn)
            : AudioProcessorEditor (ownerIn),
              owner (ownerIn),
              table (owner.model)
        {
            addAndMakeVisible (table);
            addAndMakeVisible (clearButton);

            setResizable (true, true);
            lastUIWidth .referTo (owner.state.getChildWithName ("uiState").getPropertyAsValue ("width",  nullptr));
            lastUIHeight.referTo (owner.state.getChildWithName ("uiState").getPropertyAsValue ("height", nullptr));
            setSize (lastUIWidth.getValue(), lastUIHeight.getValue());

            lastUIWidth. addListener (this);
            lastUIHeight.addListener (this);

            clearButton.onClick = [&] { owner.model.clear(); };
        }

        void paint (Graphics& g) override
        {
            g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        }

        void resized() override
        {
            auto bounds = getLocalBounds();

            clearButton.setBounds (bounds.removeFromBottom (30).withSizeKeepingCentre (50, 24));
            table.setBounds (bounds);

            lastUIWidth  = getWidth();
            lastUIHeight = getHeight();
        }

    private:
        void valueChanged (Value&) override
        {
            setSize (lastUIWidth.getValue(), lastUIHeight.getValue());
        }

        MidiLoggerPluginDemoProcessor& owner;

        MidiTable table;
        TextButton clearButton { "Clear" };

        Value lastUIWidth, lastUIHeight;
    };

    void timerCallback() override
    {
        std::vector<MidiMessage> messages;
        queue.pop (std::back_inserter (messages));
        model.addMessages (messages.begin(), messages.end());
    }

    template <typename Element>
    void process (AudioBuffer<Element>& audio, MidiBuffer& midi)
    {
        audio.clear();
        queue.push (midi);
    }

    static BusesProperties getBusesLayout()
    {
        // Live and Cakewalk don't like to load midi-only plugins, so we add an audio output there.
        const PluginHostType host;
        return host.isAbletonLive() || host.isSonar()
             ? BusesProperties().withOutput ("out", AudioChannelSet::stereo())
             : BusesProperties();
    }

    ValueTree state { "state" };
    MidiQueue queue;
    MidiListModel model; // The data to show in the UI. We keep it around in the processor so that
                         // the view is persistent even when the plugin UI is closed and reopened.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLoggerPluginDemoProcessor)
};
