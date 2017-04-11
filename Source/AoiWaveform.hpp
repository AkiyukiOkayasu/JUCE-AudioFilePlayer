//
//  AoiWaveform.hpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#ifndef AoiWaveform_hpp
#define AoiWaveform_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class AoiWaveform:
private ChangeListener
{
public:
    AoiWaveform();
    ~AoiWaveform();
    //==============================================================================
    void init(int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManagerToUse);
    void readFromFile(File& file);
    
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    //==============================================================================
    AudioThumbnailCache thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;
};

#endif /* AoiWaveform_hpp */
