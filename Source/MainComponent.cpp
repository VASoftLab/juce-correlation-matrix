#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    Initialization();    
}

MainComponent::~MainComponent()
{
}

void MainComponent::Initialization()
{
    // ========================================================================
    // Parts
    parts.clearQuick();
    parts.add("Achsschenkel");
    parts.add("Schubstrebe");
    parts.add("Zugstrebe");
    parts.add("Federlenker");
    parts.add("Querlenker Oben");
    parts.add("Drehstabgestaenge");
    parts.add("Spurstangenlenkungsbalg");
    parts.add("Antriebswelle");
    parts.add("Antriebswellenbalg");
    parts.add("Antriebswellenschelle");

    partsCount = parts.size();

    // ========================================================================
    // Cell Controls
    cellButtons.clear(true);
    cellLabels.clear(true);    
    
    // ========================================================================
    for (int i = 0; i < partsCount; i++)
    {
        for (int j = 0; j < partsCount; j++)
        {
            auto button = cellButtons.add(new juce::ToggleButton());
            button->setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::whitesmoke);
            button->setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::whitesmoke);
            button->setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::whitesmoke);
            addAndMakeVisible(button);
            // button->addListener(this);

            auto text = cellLabels.add(new juce::Label());            
            text->setFont(juce::Font(fontSize, juce::Font::plain));
            text->setJustificationType(juce::Justification::centredRight);
            text->setEditable(true, true, false);
            text->setColour(juce::Label::backgroundColourId, juce::Colours::black);
            text->setColour(juce::TextEditor::textColourId, juce::Colours::black);
            text->setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
            text->setText("123", juce::NotificationType::dontSendNotification);
            addAndMakeVisible(text);
            // text->addListener(this);
        }
    }

    // ========================================================================
    juce::Font font = juce::Font(fontSize);

    // ========================================================================
    // Constants Initialization
    //margin = 100;
    //marginT = 20;
    //maxWidth = 0;
    //cellWidth = 60;
    //cellHeight = 35;
    //fontSize = 16;

    //cellButtonWidth = 24;
    //cellButtonHeight = 24;

    //cellTextWidth = 32;
    //cellTextHeight = 24;
    // ========================================================================
    // Max Width Calculation
    auto tempWidth = 0;
    auto tempHeight = 0;

    for (auto p : parts)
    {
        tempWidth = font.getStringWidthFloat(p) + 0.5f;
        tempHeight = font.getHeight() + 0.5f;

        if (maxWidth < tempWidth)
            maxWidth = tempWidth;
    }
    maxWidth = maxWidth + marginT * 2;
    // ========================================================================
    // Set Component Size
    setSize(margin * 2 + maxWidth + partsCount * cellWidth, margin * 2 + maxWidth + partsCount * cellHeight);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);

    juce::Font font = juce::Font(fontSize);
    g.setFont (font);
    
    auto X0 = (float)margin + (float)maxWidth + 2;
    auto Y0 = (float)margin + (float)maxWidth + 2;

    juce::GlyphArrangement glyph;
    juce::Path path;
    
    g.setColour(juce::Colours::whitesmoke);
    for (int i = 0; i < partsCount; i++)
    {
        // ====================================================================
        // Columns
        g.drawRect(
            (float)margin + (float)maxWidth + ((float)cellWidth - 2) * i,
            (float)margin,
            (float)cellWidth,
            (float)maxWidth + 2,
            2.0f);

        glyph.clear();
        path.clear();

        // ====================================================================
        // Columns Text
        glyph.addLineOfText(
            juce::Font(fontSize),
            parts[i],
            (float)margin + (float)maxWidth + ((float)cellWidth - 2) * i,
            (float)margin);
        glyph.createPath(path);

        auto pathBounds = path.getBounds();
        path.applyTransform(juce::AffineTransform()
            .rotated(juce::degreesToRadians(-90.0f),
                pathBounds.getX(),
                pathBounds.getY())
            .translated((cellWidth - fontSize) * 0.5f + 2, (float)maxWidth)
        );

        g.setColour(juce::Colours::whitesmoke);
        g.fillPath(path);

        // ====================================================================
        // Rows
        g.drawRect(
            (float)margin,
            (float)margin + (float)maxWidth + ((float)cellHeight - 2) * i,
            (float)maxWidth + 2,
            (float)cellHeight,
            2.0f);
        
        g.setColour(juce::Colours::whitesmoke);
        g.drawFittedText(
            parts[i],
            juce::Rectangle<int>(
                (float)margin,
                (float)margin + (float)maxWidth + ((float)cellHeight - 2) * i,
                (float)maxWidth + 2 - marginT,
                (float)cellHeight),
            juce::Justification::centredRight, 1);
    }
    // ========================================================================
    // Cells    
    for (int i = 0; i < partsCount; i++)
    {
        for (int j = 0; j < partsCount; j++)
        {
            if (i != j)
            {
                g.setColour(juce::Colours::whitesmoke);
                g.drawRect(
                    (float)X0 + ((float)cellWidth - 2) * i - 2,
                    (float)Y0 + ((float)cellHeight - 2) * j - 2,
                    (float)cellWidth,
                    (float)cellHeight,
                    2.0f);
            }
        }
    }

    for (int i = 0; i < partsCount; i++)
    {
        for (int j = 0; j < partsCount; j++)
        {
            if (i == j)
            {
                g.setColour(juce::Colours::whitesmoke);
                g.fillRect(
                    (float)X0 + ((float)cellWidth - 2) * i - 2,
                    (float)Y0 + ((float)cellHeight - 2) * j - 2,
                    (float)cellWidth,
                    (float)cellHeight);

                g.setColour(juce::Colours::whitesmoke);
                g.drawRect(
                    (float)X0 + ((float)cellWidth - 2) * i - 2,
                    (float)Y0 + ((float)cellHeight - 2) * j - 2,
                    (float)cellWidth,
                    (float)cellHeight,
                    2.0f);
            }
        }
    }

    // ========================================================================
    // Select All Cell
    g.setColour(juce::Colours::whitesmoke);
    g.drawRect(
        (float)margin,
        (float)margin,
        (float)maxWidth + 2,
        (float)maxWidth + 2,
        2.0f);     
    
    // ========================================================================
    g.setColour(juce::Colours::lightblue);    
    g.drawRoundedRectangle(5, 5, getWidth() - 10, getHeight() - 10, 25.0f, 3.0f);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    auto X0 = (float)margin + (float)maxWidth + 2;
    auto Y0 = (float)margin + (float)maxWidth + 2;
    auto H0 = (cellHeight - cellButtonHeight) * 0.5;

    for (int i = 0; i < partsCount; i++)
    {
        for (int j = 0; j < partsCount; j++)
        {
            if (i == j)
                continue;

            cellButtons[i * partsCount + j]->setBounds(
                (float)X0 + ((float)cellWidth - 2) * i,
                (float)Y0 + ((float)cellHeight - 2) * j - 2 + H0,
                cellButtonWidth,
                cellButtonHeight
            );

            cellLabels[i * partsCount + j]->setBounds(
                (float)X0 + ((float)cellWidth - 2) * i + cellButtonWidth,
                (float)Y0 + ((float)cellHeight - 2) * j - 2 + H0,
                cellTextWidth,
                cellTextHeight
            );
        }
    }
}
