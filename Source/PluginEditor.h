/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthADRAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthADRAudioProcessorEditor (SynthADRAudioProcessor&);
    ~SynthADRAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SynthADRAudioProcessor& audioProcessor;
    juce::Slider attackSlider,sustainSlider, releaseSlider,decaySlider;
    juce::ComboBox oscSelection;
    
    juce::ImageButton concertHallButton;
    juce::Image concertHallImage;
    juce::Image recitalHallImage;
    juce::Image outdoorAmpImage;
    juce::Image practiceRoomImage;
    juce::Image background;
    
    juce::ImageButton recitalHallButton;
    juce::ImageButton outdoorAmpButton;
    juce::ImageButton practiceRoomButton;
    juce::TextButton resetButton;
    //labels
    juce::Label concertHallLabel, recitalHallLabel, outdoorAmpLabel, practiceRoomLabel, revTitle, timbrelabel, revValues;
    //juce::TextButton revValues;
    juce::Slider timbreSlider;
    
    
    //Attachments

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> concertHallButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> recitalHallButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> outdoorAmpButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> practiceRoomButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectionAttachment;


        
    juce::MidiKeyboardComponent myKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthADRAudioProcessorEditor)
};
