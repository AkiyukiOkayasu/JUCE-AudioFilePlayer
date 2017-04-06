//
//  MainComponent.h
//  PlayAudioFile
//
//  Created by Akiyuki Okayasu on 2017/04/06.
//
//

#ifndef MainComponent_h
#define MainComponent_h

#include "../JuceLibraryCode/JuceHeader.h"

enum class TransportState : int{
    Stopped,
    Starting,
    Playing,
    Pausing,
    Paused,
    Stoopping
};

class MainContentComponent   :
public AudioAppComponent,
public ButtonListener,
public ChangeListener
{
public:
    MainContentComponent();
    ~MainContentComponent();
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked(Button *button) override;
    //==============================================================================
private:
    void openButtonClicked();
    void settingButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void changeTransportState(TransportState newState);
    //==============================================================================
    // Your private member variables go here...
    std::unique_ptr<TextButton> openButton;
    std::unique_ptr<TextButton> playButton;
    std::unique_ptr<TextButton> stopButton;
    std::unique_ptr<TextButton> settingButton;
    AudioDeviceManager deviceManager;
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    std::unique_ptr<AudioFormatReader> formatReader;
    AudioTransportSource transportSource;
    AudioSourcePlayer sourcePlayer;
    TransportState state;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent);
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }

#endif /* MainComponent_h */
