/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthADRAudioProcessor::SynthADRAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),parameters(*this, nullptr, "parameters", createParameterLayout())
#endif
{
}

SynthADRAudioProcessor::~SynthADRAudioProcessor()
{
}

//==============================================================================
const juce::String SynthADRAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthADRAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthADRAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthADRAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthADRAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthADRAudioProcessor::getNumPrograms()
{
    return 1;
}

int SynthADRAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthADRAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthADRAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthADRAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthADRAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.clearVoices();
    synth.clearSounds();
    //Add sound
    synth.addSound(new MySignal());
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //Add voices (polyphony)
    for (int i=0; i< 3; i++) synth.addVoice(new SineVoice());
    for (int i=0; i< 3; i++) synth.addVoice(new SawVoice());
    for (int i=0; i< 3; i++) synth.addVoice(new SquareVoice());
    
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthADRAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthADRAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynthADRAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto numSamples = buffer.getNumSamples();
    auto currentRate = getSampleRate();
    auto osc = parameters.getRawParameterValue("oscSelection")->load();
    //DBG(osc);
    //The lines of code below connect to the four different venue buttons
    auto concertH = parameters.getRawParameterValue("concertHall")->load();
    auto recitalH = parameters.getRawParameterValue("recitalHall")->load();
    auto outdoorA = parameters.getRawParameterValue("outdoorAmp")->load();
    auto practiceR = parameters.getRawParameterValue("practiceRoom")->load();
    
    juce::Reverb::Parameters params;
    if(concertH == 1.0){ //button should return 1.0 when pressed
        //These values are based on suggestions from Google Gemini
        params.roomSize= 0.7;
        params.damping = 0.2;
        params.wetLevel = 0.3;
        params.width = 1.0;
        params.dryLevel = 0.5;
        reverb.setParameters(params);
        DBG("ConcertHButtonToggled");
    }else if(recitalH == 1.0){
        //These values are based on suggestions from Google Gemini
        params.roomSize= 0.4;
        params.damping = 0.4;
        params.wetLevel = 0.3;
        params.width = 0.8;
        params.dryLevel = 0.7;
        reverb.setParameters(params);
        DBG("RecitalHButtonToggled");
    }
    else if(outdoorA == 1.0){
        //These values are based on suggestions from Google Gemini
        params.roomSize= 0.3;
        params.damping = 0.4;
        params.wetLevel = 0.1;
        params.width = 1.0;
        params.dryLevel = 1.0;
        reverb.setParameters(params);
        DBG("OutdoorAButtonToggled");
    }
    else if(practiceR == 1.0){
        //These values are based on suggestions from Google Gemini
        params.roomSize= 0.2;
        params.damping = 0.8;
        params.wetLevel = 0.1;
        params.width = 0.7;
        params.dryLevel = 0.8;
        reverb.setParameters(params);
        DBG("PracticeRButtonToggled");
    }else{
        params.roomSize= 0.5;
        params.damping = 0.5;
        params.wetLevel = 0.5;
        params.width = 0.5;
        params.dryLevel = 0.5;
        reverb.setParameters(params);
    }
    //update every voice with these values
    for (int i = 0; i < synth.getNumVoices(); ++i){
        if (auto* v = dynamic_cast<VoiceBase*>(synth.getVoice(i))){
            if((int)osc == 0.0){ //Should be square wave
                //DBG("pianoChoice");
                auto a = 1.f;
                auto d = 5.f;
                auto s = 4.f;
                auto r = 2.f;
                v->updateAdsr( a, d, s, r,currentRate, (int)osc);
            }else if((int)osc == 1.0){ //Should be sine wave
                DBG("clarinetChoice");
                auto a = 40.f;
                auto d = 25.f;//(getSampleRate()/frequency-0.7)/4; This is the number in the paper
                DBG(d);
                auto s = 4.f;
                auto r = 500.f;
                v->updateAdsr( a, d, s, r,currentRate, (int)osc);
            }
        }
    }
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());

}

//==============================================================================
bool SynthADRAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SynthADRAudioProcessor::createEditor()
{
    return new SynthADRAudioProcessorEditor (*this);
}

//==============================================================================
void SynthADRAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
   
}

void SynthADRAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthADRAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthADRAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID {"oscSelection", 1},"oscSelection", juce::StringArray {"Sine - Piano", "Square - Clarinet"},0));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"concertHall", 1},"concertHall", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"recitalHall", 1},"recitalHall", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"outdoorAmp", 1},"outdoorAmp", 0.0f, 1.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"practiceRoom", 1},"practiceRoom", 0.0f, 1.0f, 0.0f));

    return {parameters.begin(),parameters.end()};
    
}
