//
//  AoiWaveform.cpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#include "AoiWaveform.hpp"

AoiWaveform::AoiWaveform(AudioTransportSource& transportSource_)
:
transportSource(transportSource_),
thumbnailCache(2)
{
    formatManager.registerBasicFormats();
//    thumbnail->addChangeListener(this);
}

AoiWaveform::~AoiWaveform()
{
    thumbnail->removeChangeListener(this);
}
//==============================================================================
void AoiWaveform::init(int sourceSamplesPerThumbnailSample)
{
    thumbnail = std::make_unique<AudioThumbnail>(sourceSamplesPerThumbnailSample, formatManager, thumbnailCache);
    thumbnail->addChangeListener(this);
}

void AoiWaveform::readFromFile(File& file)
{
    thumbnail->setSource(new FileInputSource(file));
    //repaint();
}

void AoiWaveform::paint(Graphics& g)
{
    if (thumbnail->getTotalLength() > 0.0)
    {
        Rectangle<int> thumbArea (getLocalBounds());
        g.setColour(Colours::red);
        thumbnail->drawChannels (g, thumbArea.reduced (2),
                                 0.0, thumbnail->getTotalLength(), 1.0f);
    }
    else
    {
        g.setFont (24.0f);
        g.drawFittedText ("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
    }
}

void AoiWaveform::resized()
{
    
}

void AoiWaveform::setTransportFollowing(bool transportFollowing)
{
    enableTransportFollow = transportFollowing;
}

//==============================================================================
void AoiWaveform::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == thumbnail.get())
    {
        if(thumbnail->getTotalLength() > 0.0)
        {
            repaint();
        }
    }
}
