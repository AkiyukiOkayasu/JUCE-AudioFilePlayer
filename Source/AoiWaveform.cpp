//
//  AoiWaveform.cpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#include "AoiWaveform.hpp"

AoiWaveform::AoiWaveform(AudioTransportSource& transportSource_, int sourceSamplesPerThumbnailSample_)
:
transportSource(transportSource_),
thumbnailCache(2),
sourceSamplesPerThumbnailSample(sourceSamplesPerThumbnailSample_)
{
    formatManager.registerBasicFormats();
    transportPosition.setStart(0.0);
    thumbnail = std::make_unique<AudioThumbnail>(sourceSamplesPerThumbnailSample, formatManager, thumbnailCache);
    thumbnail->addChangeListener(this);
    startTimerHz(60);
}

AoiWaveform::~AoiWaveform()
{
}
//==============================================================================
void AoiWaveform::readFromFile(File& file)
{
    thumbnail->setSource(new FileInputSource(file));
    formatReader.reset(formatManager.createReaderFor(file));
    transportPosition = Range<double>(0.0, thumbnail->getTotalLength());
}

void AoiWaveform::paint(Graphics& g)
{
    if (thumbnail->getTotalLength() > 0.0)
    {
        Rectangle<int> thumbArea (getLocalBounds());
        g.setGradientFill(ColourGradient(Colour::fromRGBA(245, 0, 89, 216), thumbArea.getX(), thumbArea.getHeight() / 2, Colour::fromRGBA(25, 0, 170, 101), thumbArea.getWidth(), thumbArea.getHeight() / 2, false));
        if(transportSource.isPlaying())
        {
            if(enableTransportFollow)
            {
                thumbnail->drawChannels (g, thumbArea.reduced (2),
                                         transportPosition.getStart(), transportPosition.getEnd(), 1.0f);
            }
            else
            {
                thumbnail->drawChannels (g, thumbArea.reduced (2),
                                         0.0, thumbnail->getTotalLength(), 1.0f);
            }
        }
        else
        {
            thumbnail->drawChannels (g, thumbArea.reduced (2),
                                     0.0, thumbnail->getTotalLength(), 1.0f);
        }
    }
    else
    {
        g.setColour(Colours::black);
        g.setFont (28.0f);
        g.drawFittedText ("No audio file selected...", getLocalBounds(), Justification::centred, 2);
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

void AoiWaveform::timerCallback()
{
    double currentTime = transportSource.getCurrentPosition();
    transportPosition.setStart(currentTime);
    transportPosition.setEnd(currentTime + 4.0);
    repaint();
}
