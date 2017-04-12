//
//  AoiWaveform.cpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#include "AoiWaveform.hpp"

AoiWaveform::AoiWaveform(/*AudioFormatManager& formatManager_*/)
:
thumbnailCache(5)
{
    formatmanager.registerBasicFormats();
}

AoiWaveform::~AoiWaveform()
{
    thumbnail->removeChangeListener(this);
}
//==============================================================================
void AoiWaveform::init(int sourceSamplesPerThumbnailSample)
{
    thumbnail = std::make_unique<AudioThumbnail>(sourceSamplesPerThumbnailSample, formatmanager, thumbnailCache);
    thumbnail->addChangeListener(this);
}

void AoiWaveform::readFromFile(File& file)
{
    thumbnail->setSource(new FileInputSource(file));
}

//void AoiWaveform::drawDigest(Graphics& g , const Rectangle<int>& area)
//{
//    if(thumbnail->getTotalLength() > 0.0)
//    {
//        Logger::writeToLog("waveform draw...");
//        g.setColour(Colours::white);
//        g.fillRect(area);
//        g.setColour(Colours::red);
//        thumbnail->drawChannels(g, area, 0.0, thumbnail->getTotalLength(), 1.0f);
//    }
//    else
//    {
//        Logger::writeToLog("No file to show thumbnail...");
//    }
//}

void AoiWaveform::paint(Graphics& g)
{
    if (thumbnail->getTotalLength() > 0.0)
    {
        Logger::writeToLog("waveform painting");
//        Rectangle<int> thumbArea (getLocalBounds());
        Rectangle<int> thumbArea(0, 0, 300, 300);
//        thumbArea->removeFromBottom (scrollbar.getHeight() + 4);
        thumbnail->drawChannels (g, thumbArea.reduced (2),
                                0.0, thumbnail->getTotalLength(), 1.0f);
    }
    else
    {
        g.setFont (14.0f);
        g.drawFittedText ("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
    }
    //    if(thumbnail->getTotalLength() > 0.0)
//    {
//        Logger::writeToLog("waveform draw...");
////        g.setColour(Colours::white);
////        g.fillRect(getLocalBounds());
//        g.setColour(Colours::red);
//        thumbnail->drawChannels(g, getLocalBounds(), 0.0, thumbnail->getTotalLength(), 1.0f);
//    }
//    else
//    {
//        Logger::writeToLog("No file to show thumbnail...");
//    }
}

void AoiWaveform::resized()
{
    
}

//==============================================================================
void AoiWaveform::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == thumbnail.get())
    {
        Logger::writeToLog("AoiWaveform::thumbnail changeListnerCallback");
        repaint();
    }
}
