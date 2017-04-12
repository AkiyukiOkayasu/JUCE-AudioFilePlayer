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
public Component,
public ChangeListener
{
public:
    AoiWaveform();
    ~AoiWaveform();
    //==============================================================================
    void init(int sourceSamplesPerThumbnailSample, bool showDigestWaveform_, bool showZoomableWaveform_);
    void readFromFile(File& file);
    void paint(Graphics& g) override;
    void resized() override;
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    //==============================================================================
    AudioFormatManager formatmanager;
    AudioThumbnailCache thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;
    int sourceSamplesPerThumbnailSample;
    bool showDigestWaveform;
    bool showZoomableWaveform;
};

#endif /* AoiWaveform_hpp */
