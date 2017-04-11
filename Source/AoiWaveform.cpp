//
//  AoiWaveform.cpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#include "AoiWaveform.hpp"

AoiWaveform::AoiWaveform()
:
thumbnailCache(2)
{
    
}

AoiWaveform::~AoiWaveform()
{
    
}
//==============================================================================
void AoiWaveform::init(int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManagerToUse)
{
    thumbnail = std::make_unique<AudioThumbnail>(sourceSamplesPerThumbnailSample, formatManagerToUse, thumbnailCache);
    thumbnail->addChangeListener(this);
}

void AoiWaveform::readFromFile(File& file)
{
    thumbnail->setSource(new FileInputSource(file));
}


//==============================================================================
void AoiWaveform::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == thumbnail.get())
    {
    }
}
