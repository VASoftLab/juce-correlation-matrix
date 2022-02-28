#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MatrixLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MatrixLookAndFeel::MatrixLookAndFeel()
    {
        setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::yellow);
        setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::whitesmoke);
        setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::whitesmoke);

        /*setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
        setColour(juce::Label::textColourId, juce::Colours::black);
        setColour(juce::Label::textWhenEditingColourId, juce::Colours::black);
        setColour(juce::TextEditor::backgroundColourId, juce::Colours::lightgrey);
        setColour(juce::TextEditor::textColourId, juce::Colours::black);
        setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::black); */       
    }

    void MatrixLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        auto fontSize = juce::jmin(16.0f, (float)button.getHeight() * 0.75f);
        auto tickWidth = fontSize * 1.1f;        
        drawTickBox(g, button, 4.0f, ((float)button.getHeight() - tickWidth) * 0.5f,
            tickWidth, tickWidth,
            button.getToggleState(),
            button.isEnabled(),
            shouldDrawButtonAsHighlighted,
            shouldDrawButtonAsDown);

        g.setColour(button.findColour(juce::ToggleButton::textColourId));
        g.setFont(fontSize);

        if (!button.isEnabled())
            g.setOpacity(0.5f);

        g.drawFittedText(button.getButtonText(),
            button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10)
            .withTrimmedRight(2),
            juce::Justification::centredLeft, 10);
    }

    void MatrixLookAndFeel::drawTickBox(juce::Graphics& g, juce::Component& component,
        float x, float y, float w, float h,
        const bool ticked,
        const bool isEnabled,
        const bool shouldDrawButtonAsHighlighted,
        const bool shouldDrawButtonAsDown)
    {
        juce::ignoreUnused(isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        juce::Rectangle<float> tickBounds(x, y, w, h);

        // g.setColour(component.findColour(juce::ToggleButton::tickDisabledColourId));
        g.setColour(juce::Colours::orange);
        g.drawRoundedRectangle(tickBounds, 4.0f, 1.0f);

        if (ticked)
        {
            // g.setColour(component.findColour(juce::ToggleButton::tickColourId));
            g.setColour(juce::Colours::orange);
            auto tick = getTickShape(0.75f);
            g.fillPath(tick, tick.getTransformToScaleToFit(tickBounds.reduced(4, 5).toFloat(), false));
        }
    }
};

class MainComponent  : public juce::Component,
    private MatrixLookAndFeel,
    public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* b) override;

private:
    //==============================================================================
    // Your private member variables go here...
    int margin = 20;
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

    std::unique_ptr<juce::ToggleButton> buttonSelectAll;

    MatrixLookAndFeel matrixLookAndFeel;

    juce::OwnedArray<juce::StringArray> matrix;

    void Initialization();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
