//
//  AoiPlayAudioFile.hpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#ifndef AoiPlayAudioFile_hpp
#define AoiPlayAudioFile_hpp

#include "../JuceLibraryCode/JuceHeader.h"

class AoiPlayAudioFile
{
public:
    AoiPlayAudioFile(AudioDeviceManager& deviceManager, AudioFormatManager& formatManager_, AudioTransportSource& transportSource_);
    ~AoiPlayAudioFile();
    //==============================================================================
    void openDialogAndSetSource();
    void setSource(File* file);
    void play();
    void stop();
    void backToStartPoint();
    bool isSettingSource();
    //==============================================================================
private:
    //==============================================================================
    //private menber
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    std::unique_ptr<AudioFormatReader> formatReader;
    AudioTransportSource& transportSource;
    AudioSourcePlayer sourcePlayer;
};

#endif /* AoiPlayAudioFile_hpp */
