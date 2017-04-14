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
public ChangeListener,
public Timer
{
public:
    AoiWaveform(AudioTransportSource& transportSource_, int sourceSamplesPerThumbnailSample_);
    ~AoiWaveform();
    //==============================================================================
    void readFromFile(File& file);
    void paint(Graphics& g) override;
    void resized() override;
    void setTransportFollowing(bool transportFollowing);
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void timerCallback() override;
    //==============================================================================
    AudioFormatManager formatManager;
    AudioTransportSource& transportSource;
    AudioThumbnailCache thumbnailCache;
    Range<double> transportPosition;
    std::unique_ptr<AudioThumbnail> thumbnail;
    std::unique_ptr<AudioFormatReader> formatReader;
    int sourceSamplesPerThumbnailSample;
    bool enableTransportFollow = false;
};

#endif /* AoiWaveform_hpp */
