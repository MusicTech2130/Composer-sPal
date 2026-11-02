/*
  ==============================================================================

    Signals.h
    Created: 8 Feb 2026 8:18:19pm
    Author:  Nelly Victoria Alexandra Garcia Sihuay

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>

//Write your Synthesiser Sound class

class MySignal : public juce::SynthesiserSound
{ // without this class your voices will not be heard
public:
    MySignal(){}
    bool appliesToNote(int) override {return true;}
    bool appliesToChannel(int) override {return true;}
};

class VoiceBase : public juce::SynthesiserVoice
{
public:
    VoiceBase() {}
    int oscSelection = 0;
    bool canPlaySound(juce::SynthesiserSound* sound) override {
        return dynamic_cast<MySignal*> (sound) != nullptr;
    }
    
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override {
        phase = 0.0f; //current angle
        level = velocity * 0.15f;
        auto cyclesPerSample = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) / getSampleRate();
        phaseDelta = (float)cyclesPerSample;
        adsr.noteOn();
    }

    void stopNote(float, bool allowTailOff) override {
        adsr.noteOff();
        if (!allowTailOff || !adsr.isActive()) clearCurrentNote();
    }

    virtual float getNextSample() = 0;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        while (--numSamples >= 0) {
            auto rawSample = getNextSample();
            auto env = adsr.getNextSample();
            auto finalSample = rawSample * env * level;

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample(i, startSample, finalSample);

            startSample++;
            
            if (!adsr.isActive()) clearCurrentNote();
        }
    }
    
    void setSampleRate(double newSampleRate) {
        adsr.setSampleRate(newSampleRate);
    }

    void updateAdsr(float a, float d, float s, float r,double sr,int oscChoice) {
        adsrParams.attack = a;
        adsrParams.decay = d;
        adsrParams.sustain = s;
        adsrParams.release = r;
        oscSelection = oscChoice;
        adsr.setSampleRate(sr);
        adsr.setParameters(adsrParams);
    }

protected:
    float phase = 0.0f, phaseDelta = 0.0f, level = 0.0f;
    juce::ADSR adsr;//adsr object
    juce::ADSR::Parameters adsrParams;//parameter
};



class SineVoice : public VoiceBase {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override {
            return oscSelection == 0 && dynamic_cast<MySignal*>(sound) != nullptr;
        }
    float getNextSample() override {
        auto sample = std::sin(phase * juce::MathConstants<float>::twoPi);
        phase = std::fmod(phase + phaseDelta, 1.0f);
        return sample;
    }
   
};

class SawVoice : public VoiceBase {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override {
            return oscSelection == 1 && dynamic_cast<MySignal*>(sound) != nullptr;
        }
    float getNextSample() override {
        auto sample = 2.0f * phase -1.0f;
        phase = std::fmod(phase + phaseDelta, 1.0f);
        return sample;
    }
};

class SquareVoice : public VoiceBase {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override {
            return oscSelection == 2 && dynamic_cast<MySignal*>(sound) != nullptr;
        }
    float getNextSample() override {
        //add white noise
        auto sample = (phase < 0.5f)? 1.0f : -1.0f;
        phase = std::fmod(phase + phaseDelta, 1.0f);
        return sample;
    }
};
