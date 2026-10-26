#include <juce_audio_utils/juce_audio_utils.h>
#include "Engine.h"

class ScreechForge final : public juce::AudioProcessor {
public:
    juce::AudioProcessorValueTreeState state;
    ScreechEngine engine;
    float bend=0;
    std::array<std::atomic<float>*,11> values {};
    const std::array<const char*,11> ids {"fm","ratio","cutoff","resonance","drive","sweep","decay","release","motion","depth","gain"};
    ScreechForge():AudioProcessor(BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true)),
        state(*this,nullptr,"ScreechForge",layout()) {
        for(size_t i=0;i<ids.size();++i) values[i]=state.getRawParameterValue(ids[i]);
    }
    static juce::AudioProcessorValueTreeState::ParameterLayout layout() {
        juce::AudioProcessorValueTreeState::ParameterLayout l;
        auto add=[&](const char* id,const char* name,float lo,float hi,float def,float skew=1) {
            l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id,1},name,juce::NormalisableRange<float>(lo,hi,0,skew),def));
        };
        add("fm","FM bite",0,16,4); add("ratio","FM ratio",0.25f,8,2);
        add("cutoff","Filter Hz",80,16000,1600,0.3f); add("resonance","Resonance",0,1,0.65f);
        add("drive","Distortion",1,24,8); add("sweep","Pitch sweep (semitones)",-36,48,24);
        add("decay","Sweep decay (seconds)",0.01f,2,0.22f,0.4f);
        add("release","Release (seconds)",0.005f,2,0.12f,0.4f);
        add("motion","Motion Hz",0.1f,30,5,0.4f); add("depth","Motion depth",0,1,0.4f);
        add("gain","Output",0,0.8f,0.2f);
        // Presets are exposed through the host's program selector.
        return l;
    }
    const juce::String getName() const override { return "ScreechForge"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 12; }
    int program=0;
    int getNumPrograms() override { return 3; }
    int getCurrentProgram() override { return program; }
    const juce::String getProgramName(int n) override { return n==1?"Metal Howl":n==2?"Descending Terror":"Classic Screech"; }
    void changeProgramName(int,const juce::String&) override {}
    void setCurrentProgram(int n) override {
        program=juce::jlimit(0,2,n);
        const float presets[3][11]={{4,2,1600,.65f,8,24,.22f,.12f,5,.4f,.2f},
            {10,3.5f,2400,.85f,14,12,.35f,.2f,8,.6f,.16f},
            {6,1.5f,900,.75f,12,36,.6f,.25f,3,.7f,.18f}};
        for(int i=0;i<11;++i) { auto* param=state.getParameter(ids[size_t(i)]); param->setValueNotifyingHost(param->convertTo0to1(presets[program][i])); }
    }
    void prepareToPlay(double rate,int) override { engine.prepare(rate); bend=0; }
    void releaseResources() override {}
    bool isBusesLayoutSupported(const BusesLayout& b) const override {
        return b.getMainInputChannelSet().isDisabled() && (b.getMainOutputChannelSet()==juce::AudioChannelSet::stereo() || b.getMainOutputChannelSet()==juce::AudioChannelSet::mono());
    }
    void processBlock(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midi) override {
        juce::ScopedNoDenormals noDenormals;
        buffer.clear();
        engine.p={values[0]->load(),values[1]->load(),values[2]->load(),values[3]->load(),values[4]->load(),values[5]->load(),values[6]->load(),values[7]->load(),values[8]->load(),values[9]->load(),values[10]->load()};
        auto it=midi.cbegin();
        for(int i=0;i<buffer.getNumSamples();++i) {
            while(it!=midi.cend() && (*it).samplePosition<=i) {
                auto m=(*it).getMessage();
                if(m.isNoteOn()) engine.on(m.getNoteNumber(),m.getFloatVelocity());
                else if(m.isNoteOff()) engine.off(m.getNoteNumber());
                else if(m.isAllNotesOff() || m.isAllSoundOff()) engine.reset();
                else if(m.isPitchWheel()) bend=2.0f*(m.getPitchWheelValue()-8192)/8192.0f;
                ++it;
            }
            const float x=engine.sample(bend);
            for(int c=0;c<buffer.getNumChannels();++c) buffer.setSample(c,i,x);
        }
        midi.clear();
    }
    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override { return new juce::GenericAudioProcessorEditor(*this); }
    void getStateInformation(juce::MemoryBlock& dest) override {
        auto tree=state.copyState(); tree.setProperty("program",program,nullptr);
        if(auto xml=tree.createXml()) copyXmlToBinary(*xml,dest);
    }
    void setStateInformation(const void* data,int size) override {
        if(auto xml=getXmlFromBinary(data,size)) if(xml->hasTagName(state.state.getType())) {
            auto tree=juce::ValueTree::fromXml(*xml); program=juce::jlimit(0,2,int(tree.getProperty("program",0))); state.replaceState(tree);
        }
    }
};
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ScreechForge(); }
