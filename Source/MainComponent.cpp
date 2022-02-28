#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    Initialization();    
}

MainComponent::~MainComponent()
{
    for (auto b : cellButtons)
        b->setLookAndFeel(nullptr);

    for (auto l : cellLabels)
        l->setLookAndFeel(nullptr);

    cellButtons.clear(true);
    cellLabels.clear(true);

    buttonSelectAll = nullptr;
    matrix.clear(true);

    setLookAndFeel(nullptr);
}

void MainComponent::Initialization()
{
    // ========================================================================
    // Matrix    
    juce::File fileToRead("D:\\Projects\\JUCE\\juce-correlation-matrix\\Documents\\matrix.csv");

    if (!fileToRead.existsAsFile())
        return;  // File doesn't exist

    juce::FileInputStream inputStream(fileToRead);

    if (!inputStream.openedOk())
        return;  // Failed to open

    juce::StringArray* arr;
    while (!inputStream.isExhausted())
    {
        arr = new juce::StringArray();
        auto line = inputStream.readNextLine();
        std::string str = line.toStdString();
        size_t pos = 0;
        std::string value;
        while ((pos = str.find(",")) != std::string::npos) {
            value = str.substr(0, pos);
            str.erase(0, pos + 1);
            arr->add(value);
        }
        matrix.add(arr);
    }
    // ========================================================================
    // Parts
    parts.clearQuick();
    parts.add("Achsschenkel");
    parts.add("Schubstrebe");
    parts.add("Zugstrebe");
    parts.add("Federlenker");
    parts.add("Querlenker Oben");
    parts.add("Querlenker Unten");
    parts.add("Spurstange");
    parts.add("Drehstab");
    parts.add("Drehstabgestaenge");
    parts.add("Bremssattel");
    parts.add("Federbein");
    parts.add("Rad");
    parts.add("Spurstangenlenkungsbalg");
    parts.add("Reifen");
    parts.add("Antriebswelle");
    parts.add("Antriebswellenbalg");
    parts.add("Antriebswellenschelle");
    parts.add("Drehwinkelsensor");
    parts.add("Lenkgetriebe");
    parts.add("Rohbau");
    // parts.add("Neues Bauteil");
    partsCount = parts.size();

    // ========================================================================
    // Cell Controls
    cellButtons.clear(true);
    cellLabels.clear(true);    
    
    // ========================================================================
    for (int i = 0; i < partsCount; i++)
    {
        auto elem = matrix[i];
        for (int j = 0; j < partsCount; j++)
        {
            auto str = elem->strings.getReference(j);
            
            // Check button
            auto button = cellButtons.add(new juce::ToggleButton());            
            button->setLookAndFeel(&matrixLookAndFeel);
            
            if (str.length() > 0)
                addAndMakeVisible(button);
            else
                addChildComponent(button);
            
            button->addListener(this);
            
            // Text Label
            auto text = cellLabels.add(new juce::Label());            
            text->setFont(juce::Font(fontSize, juce::Font::plain));
            text->setJustificationType(juce::Justification::centredRight);
            
            if (j > i)
                text->setEditable(true, true, false);
            else
            {
                str = cellLabels[j * partsCount + i]->getText(); // Transponent
                text->setEditable(false, false, false);
            }

            text->setText(str, juce::NotificationType::dontSendNotification);
            text->setLookAndFeel(&matrixLookAndFeel);
            if (str.length() > 0)
                addAndMakeVisible(text);
            else
                addChildComponent(text);            
            // text->addListener(this);
        }
    }

    // ========================================================================
    juce::Font font = juce::Font(fontSize);

    // ========================================================================
    // Constants Initialization
    
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
    // Select All
    buttonSelectAll.reset(new juce::ToggleButton("buttonSelectAll"));
    buttonSelectAll->setButtonText(TRANS("Select all"));
    buttonSelectAll->setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colour(0xffffffff));
    buttonSelectAll->setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colour(0xffbbbbbb));
    buttonSelectAll->setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colour(0xffffffff));
    buttonSelectAll->setLookAndFeel(&matrixLookAndFeel);
    addAndMakeVisible(buttonSelectAll.get());
    buttonSelectAll->addListener(this);
    
    // ========================================================================
    // Set Component Size    
    setSize(margin * 2 + maxWidth + partsCount * cellWidth - marginT * 2,
        margin * 2 + maxWidth + partsCount * cellHeight - marginT * 2);
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
        auto elem = matrix[i];
        for (int j = 0; j < partsCount; j++)
        {
            auto str = elem->strings.getReference(j);
            if (i != j)
            {
                if (i < j)
                {
                    if (str.length() == 0)
                        g.setColour(juce::Colours::darkgrey);
                    else
                        g.setColour(juce::Colours::black);

                    g.fillRect(
                        (float)X0 + ((float)cellWidth - 2) * j - 2,
                        (float)Y0 + ((float)cellHeight - 2) * i - 2,
                        (float)cellWidth,
                        (float)cellHeight);                    
                }

                g.setColour(juce::Colours::lightgrey);
                g.drawRect(
                    (float)X0 + ((float)cellWidth - 2) * j - 2,
                    (float)Y0 + ((float)cellHeight - 2) * i - 2,
                    (float)cellWidth,
                    (float)cellHeight,
                    2.0f);
            }
            else
            {
                // Main Diagonal
                g.setColour(juce::Colours::whitesmoke);
                g.fillRect(
                    (float)X0 + ((float)cellWidth - 2) * j - 2,
                    (float)Y0 + ((float)cellHeight - 2) * i - 2,
                    (float)cellWidth,
                    (float)cellHeight);

                g.setColour(juce::Colours::whitesmoke);
                g.drawRect(
                    (float)X0 + ((float)cellWidth - 2) * j - 2,
                    (float)Y0 + ((float)cellHeight - 2) * i - 2,
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
    // g.setColour(juce::Colours::lightblue);    
    // g.drawRoundedRectangle(5, 5, getWidth() - 10, getHeight() - 10, 25.0f, 3.0f);
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
            
            if (j > i)
            {
                cellButtons[i * partsCount + j]->setBounds(
                    (float)X0 + ((float)cellWidth - 2) * j,
                    (float)Y0 + ((float)cellHeight - 2) * i - 2 + H0,
                    cellButtonWidth,
                    cellButtonHeight
                );
            }

            cellLabels[i * partsCount + j]->setBounds(
                (float)X0 + ((float)cellWidth - 2) * j + cellButtonWidth,
                (float)Y0 + ((float)cellHeight - 2) * i - 2 + H0,
                cellTextWidth,
                cellTextHeight
            );
        }
    }

    // Select All
    buttonSelectAll->setBounds(
        (float)margin + (maxWidth - 100) * 0.5f + 10,
        (float)margin + (maxWidth - 32) * 0.5f,
        (float)100,
        (float)32);
}

void MainComponent::buttonClicked(juce::Button* b)
{
    if (b == buttonSelectAll.get())
    {
        auto state = b->getToggleState();
        for (int i = 0; i < cellButtons.size(); i++)
        {
            auto button = cellButtons[i];
            button->setToggleState(state, juce::dontSendNotification);
        }
    }
}
