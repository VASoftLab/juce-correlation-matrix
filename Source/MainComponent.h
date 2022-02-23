#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    int margin = 100;
    int marginT = 20;
    int maxWidth = 0;
    int cellWidth = 60;
    int cellHeight = 35;
    int partsCount = 0;
    float fontSize = 16;

    int cellButtonWidth = 24;
    int cellButtonHeight = 24;
    int cellTextWidth = 32;
    int cellTextHeight = 24;

    juce::StringArray parts;
    juce::OwnedArray<juce::ToggleButton> cellButtons;
    juce::OwnedArray<juce::Label> cellLabels;

    void Initialization();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
