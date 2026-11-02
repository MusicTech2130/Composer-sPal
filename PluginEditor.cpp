/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthADRAudioProcessorEditor::SynthADRAudioProcessorEditor (SynthADRAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), myKeyboard(audioProcessor.keyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
        
    //ComboBox
    addAndMakeVisible(oscSelection);
    oscSelection.addItemList(juce::StringArray {"Piano", "Square wave synthesizer"}, 1);
    oscSelection.setJustificationType(juce::Justification::centred);
    oscSelectionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "oscSelection", oscSelection);
    
    //Styling buttons
    concertHallButton.setClickingTogglesState(true);
    concertHallImage= juce::ImageCache::getFromMemory(BinaryData::Carnigie_jpg, BinaryData::Carnigie_jpgSize);
    if(concertHallImage.isValid()){
        DBG("Validity Check");
        addAndMakeVisible(concertHallButton);
        concertHallButton.setImages(false,true, true, concertHallImage, 1.0f, juce::Colours::transparentBlack, concertHallImage,1.0f, juce::Colours::transparentBlack, concertHallImage, 1.0f, juce::Colours::white.withAlpha(0.2f));
    }
    concertHallLabel.setFont(juce::FontOptions("Arial",16.0f, juce::Font::bold));
    concertHallLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    concertHallLabel.setText("Concert Hall", juce::dontSendNotification);
    concertHallLabel.attachToComponent(&concertHallButton, false);
  //This changes the text, I want it to show up below though so I'll detach and line up in resize function
    
    addAndMakeVisible(recitalHallButton);
    recitalHallButton.setClickingTogglesState(true);
    recitalHallImage= juce::ImageCache::getFromMemory(BinaryData::RecitalHall_jpg, BinaryData::RecitalHall_jpgSize);
    if(recitalHallImage.isValid()){
        DBG("Validity Check");
        addAndMakeVisible(recitalHallButton);
        //addAndMakeVisible(fluteButton);
        recitalHallButton.setImages(false,true, true, recitalHallImage, 1.0f, juce::Colours::transparentBlack, recitalHallImage,1.0f, juce::Colours::transparentBlack, recitalHallImage, 1.0f, juce::Colours::white.withAlpha(0.2f));
    }
    recitalHallLabel.setFont(juce::FontOptions("Arial",16.0f, juce::Font::bold));
    recitalHallLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    recitalHallLabel.setText("Recital Hall", juce::dontSendNotification);
    recitalHallLabel.attachToComponent(&recitalHallButton, false);
    
    addAndMakeVisible(outdoorAmpButton);
    outdoorAmpButton.setClickingTogglesState(true);
    outdoorAmpImage= juce::ImageCache::getFromMemory(BinaryData::redRocks_png, BinaryData::redRocks_pngSize);
    if(outdoorAmpImage.isValid()){
        DBG("OAValidity Check");
        addAndMakeVisible(outdoorAmpButton);
        outdoorAmpButton.setImages(false,true, true, outdoorAmpImage, 1.0f, juce::Colours::transparentBlack, outdoorAmpImage,1.0f, juce::Colours::transparentBlack, outdoorAmpImage, 1.0f, juce::Colours::white.withAlpha(0.2f));
    }
    outdoorAmpLabel.setFont(juce::FontOptions("Arial",16.0f, juce::Font::bold));
    outdoorAmpLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    outdoorAmpLabel.setText("Outdoor Amphitheatre", juce::dontSendNotification);
    outdoorAmpLabel.attachToComponent(&outdoorAmpButton, false);
    
    addAndMakeVisible(practiceRoomButton);
    practiceRoomButton.setClickingTogglesState(true);
    practiceRoomImage= juce::ImageCache::getFromMemory(BinaryData::PracticeRoom_png, BinaryData::PracticeRoom_pngSize);
    if(practiceRoomImage.isValid()){
        DBG("Validity Check");
        addAndMakeVisible(practiceRoomButton);
        practiceRoomButton.setImages(false,true, true, practiceRoomImage, 1.0f, juce::Colours::transparentBlack, practiceRoomImage,1.0f, juce::Colours::transparentBlack, practiceRoomImage, 1.0f, juce::Colours::white.withAlpha(0.2f));
    }
    practiceRoomLabel.setFont(juce::FontOptions("Arial",16.0f, juce::Font::bold));
    practiceRoomLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    practiceRoomLabel.setText("Practice Room", juce::dontSendNotification);
    practiceRoomLabel.attachToComponent(&practiceRoomButton, false);
    
    addAndMakeVisible(resetButton);
    resetButton.setClickingTogglesState(true);
    resetButton.setButtonText("Reset Values");
    
    addAndMakeVisible(timbreSlider);
    timbreSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    timbreSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    addAndMakeVisible(timbrelabel);
    timbrelabel.setText("Change Timbre", juce::dontSendNotification);
    timbrelabel.setJustificationType(juce::Justification::centred);
    timbrelabel.attachToComponent(&timbreSlider, false);
    
    concertHallButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters,"concertHall",concertHallButton);
    recitalHallButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters,"recitalHall",recitalHallButton);
    outdoorAmpButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters,"outdoorAmp",outdoorAmpButton);
    practiceRoomButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters,"practiceRoom",practiceRoomButton);
    addAndMakeVisible(revTitle);
    revTitle.setText("Reverb JUCE Settings:", juce::dontSendNotification);
    revTitle.setFont(juce::FontOptions("Arial",16.0f, juce::Font::bold));
    revTitle.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(revValues);
    revValues.setFont(juce::FontOptions("Arial",12.0f, juce::Font::italic));
    revValues.setColour(juce::Label::textColourId, juce::Colours::black);
    
    revValues.setText("Room Size: 0.5, Damping: 0.5, Wet Level: 0.5, Width: 0.5, Dry Level: 0.5", juce::sendNotification);
    concertHallButton.onClick = [this] { //This line is from Google Gemini
            revValues.setText("Room Size: 0.7, Damping: 0.2, Wet Level: 0.3, Width: 1.0, Dry Level: 0.5", juce::sendNotification);
        };
    
    recitalHallButton.onClick = [this]{
        revValues.setText("Room Size: 0.4, Damping: 0.4, Wet Level: 0.3, Width: 0.8, Dry Level: 0.7", juce::sendNotification);
    };
    outdoorAmpButton.onClick = [this]{
         revValues.setText("Room Size: 0.3, Damping: 0.4, Wet Level: 0.1, Width: 1.0, Dry Level: 1.0", juce::sendNotification);
    };
    practiceRoomButton.onClick = [this]{
        revValues.setText("Room Size: 0.2, Damping: 0.8, Wet Level: 0.1, Width: 0.7, Dry Level: 0.8", juce::sendNotification);
    };
    resetButton.onClick = [this]{
        revValues.setText("Room Size: 0.5, Damping: 0.5, Wet Level: 0.5, Width: 0.5, Dry Level: 0.5", juce::sendNotification);
    };
    setSize (600, 450);
    addAndMakeVisible(&myKeyboard);
}

    SynthADRAudioProcessorEditor::~SynthADRAudioProcessorEditor()
{
}

//==============================================================================
void SynthADRAudioProcessorEditor::paint (juce::Graphics& g)
{

    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageAt(background,0,0);
    g.setColour (juce::Colours::black);
    g.setFont (juce::FontOptions (24.0f, juce::Font::bold));

    auto headerArea = getLocalBounds().removeFromTop(50);
        
    g.drawFittedText ("Composer's Pal", headerArea, juce::Justification::centred, 1);
}

void SynthADRAudioProcessorEditor::resized()
    {
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(50);

    auto topRow = area.removeFromTop(100);
    int topWidth = area.getWidth();
    concertHallButton.setBounds(topRow.removeFromLeft(topWidth/4));
    recitalHallButton.setBounds(topRow.removeFromLeft(topWidth/4));
    outdoorAmpButton.setBounds(topRow.removeFromLeft(topWidth/4));
    practiceRoomButton.setBounds(topRow.removeFromLeft(topWidth/4));
    area.removeFromTop(10);
    auto dropDown = area.removeFromTop(30);
    int dwidth = area.getWidth();
    oscSelection.setBounds(dropDown.removeFromLeft(dwidth/2));
    
    auto midRow = area.removeFromTop(100);
    int midWidth = midRow.getWidth();
    revTitle.setBounds(midRow.removeFromLeft(midWidth/3));
    revValues.setBounds(midRow.removeFromLeft(midWidth/3));
    resetButton.setBounds(450,225,75,45);
    
    myKeyboard.setBounds(area.removeFromBottom(140));
        
    }
