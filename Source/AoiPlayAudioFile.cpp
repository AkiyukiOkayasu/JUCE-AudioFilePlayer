//
//  AoiPlayAudioFile.cpp
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/07.
//
//

#include "AoiPlayAudioFile.hpp"

AoiPlayAudioFile::AoiPlayAudioFile(AudioDeviceManager& deviceManager)
{
    formatManager.registerBasicFormats();
    sourcePlayer.setSource(&transportSource);
    deviceManager.addAudioCallback(&sourcePlayer);
}

AoiPlayAudioFile::~AoiPlayAudioFile()
{
    
}
//==============================================================================

void AoiPlayAudioFile::openDialogAndSetSource()
{
    std::cout<<"openButton clicked"<<std::endl;
    transportSource.setSource(nullptr);
    FileChooser chooser("Select a audio file to play...",
                        File::nonexistent,
                        "*.wav, *.wave, *.aif, *.aiff");
    if(chooser.browseForFileToOpen())
    {
        File file(chooser.getResult());
        readerSource.reset(new AudioFormatReaderSource(formatManager.createReaderFor(file), true));
        formatReader.reset(formatManager.createReaderFor(file));
        transportSource.setSource(readerSource.get(),
                                  0,
                                  nullptr,
                                  formatReader->sampleRate,
                                  formatReader->numChannels);
        sourcePlayer.setSource(&transportSource);
    }
}

void AoiPlayAudioFile::setSource(File* file)
{
    if(file == nullptr)
    {
        if(transportSource.isPlaying())
        {
            transportSource.stop();
        }
        
        transportSource.setSource(nullptr);
        sourcePlayer.setSource(nullptr);
    }
    else
    {
        transportSource.setSource(nullptr);
        readerSource.reset(new AudioFormatReaderSource(formatManager.createReaderFor(*file), true));
        formatReader.reset(formatManager.createReaderFor(*file));
        transportSource.setSource(readerSource.get(),
                                  0,
                                  nullptr,
                                  formatReader->sampleRate,
                                  formatReader->numChannels);
        sourcePlayer.setSource(&transportSource);
    }
}

void AoiPlayAudioFile::play()
{
    if(!transportSource.isPlaying())
    {
        transportSource.start();
    }
}

void AoiPlayAudioFile::stop()
{
 if(transportSource.isPlaying())
 {
     transportSource.stop();
 }
}

void AoiPlayAudioFile::backToStartPoint()
{
    transportSource.setPosition(0.0);
}

bool AoiPlayAudioFile::isSettingSource()
{
//    undefined now...
}
