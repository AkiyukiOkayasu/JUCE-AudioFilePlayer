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
    AoiWaveform(/*AudioFormatManager& formatManager_, AudioTransportSource &transportSource_*/);
    ~AoiWaveform();
    //==============================================================================
    void init(int sourceSamplesPerThumbnailSample);
    void readFromFile(File& file);
//    void drawDigest(Graphics &g, const Rectangle<int>& area);
    void paint(Graphics& g) override;
    void resized() override;
    
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    //==============================================================================
    AudioFormatManager formatmanager;
    AudioThumbnailCache thumbnailCache;
    std::unique_ptr<AudioThumbnail> thumbnail;
};

#endif /* AoiWaveform_hpp */
