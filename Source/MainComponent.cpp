/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"

MainContentComponent::MainContentComponent()
:
//C++11 style : Making smart pointer
//    openButton(new TextButton("openButton")),
//    playButton(new TextButton("playButton")),
//    stopButton(new TextButton("stopButton")),
//    settingButton(new TextButton("settingButton")),
state(TransportState::NoFile)
{
    formatManager.registerBasicFormats();
    
    //C++14 style? : Making smart pointer -> Does not working...
    //openButton = addAndMakeVisible(std::make_unique<TextButton>());
    
    //Another C++14 style : Making smart pointer and addAndMakeVisible().
    openButton = std::make_unique<TextButton>("openButton");
    addAndMakeVisible(openButton.get());
    openButton->setButtonText("Open Audio File...");
    openButton->addListener(this);
    
    playButton = std::make_unique<TextButton>("playButton");
    addAndMakeVisible(playButton.get());
    playButton->setButtonText("Play...");
    playButton->addListener(this);
    playButton->setEnabled(false);
    playButton->setColour(TextButton::buttonColourId, Colours::green);
    
    stopButton  = std::make_unique<TextButton>("stopButton");
    addAndMakeVisible(stopButton.get());
    stopButton->setButtonText("Stop...");
    stopButton->addListener(this);
    stopButton->setEnabled(false);
    stopButton->setColour(TextButton::buttonColourId, Colours::red);
    
    settingButton = std::make_unique<TextButton>("settingButton");
    addAndMakeVisible(settingButton.get());
    settingButton->setButtonText("Audio Preference");
    settingButton->addListener(this);
    
    transportFollowButton = std::make_unique<ToggleButton>("transportFollowToggle");
    addAndMakeVisible(transportFollowButton.get());
    transportFollowButton->setButtonText("Follow transport");
    transportFollowButton->addListener(this);
    
    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addChangeListener(this);
    sourcePlayer.setSource(&transportSource);
    deviceManager.addAudioCallback(&sourcePlayer);
    waveform = std::make_unique<AoiWaveform>(transportSource, 512);
    addAndMakeVisible(waveform.get());
    transportSource.addChangeListener(this);
    
    setSize (1280, 720);
    setOpaque(false);
}

MainContentComponent::~MainContentComponent()
{
    deviceManager.removeChangeListener(this);
}

//==============================================================================
void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
}

void MainContentComponent::releaseResources()
{
    
}

//==============================================================================
void MainContentComponent::paint (Graphics& g)
{
    g.fillAll(Colours::transparentWhite);
}

void MainContentComponent::resized()
{
    auto r = getLocalBounds();
    auto transportBounds = r.removeFromTop(30);
    int transportButtonWidth = transportBounds.getWidth() / 5;
    int reduceAmount = 3;
    openButton->setBounds(transportBounds.removeFromLeft(transportButtonWidth).reduced(reduceAmount));
    settingButton->setBounds(transportBounds.removeFromLeft(transportButtonWidth).reduced(reduceAmount));
    playButton->setBounds(transportBounds.removeFromLeft(transportButtonWidth).reduced(reduceAmount));
    stopButton->setBounds(transportBounds.removeFromLeft(transportButtonWidth).reduced(reduceAmount));
    transportFollowButton->setBounds(transportBounds.reduced(reduceAmount));
    waveform->setBounds(r);
}

void MainContentComponent::buttonClicked(Button *button)
{
    if(button == openButton.get())
    {
        openButtonClicked();
    }
    else if(button == settingButton.get())
    {
        settingButtonClicked();
    }
    else if(button == playButton.get())
    {
        playButtonClicked();
    }
    else if(button == stopButton.get())
    {
        stopButtonClicked();
    }
    else if(button == transportFollowButton.get())
    {
        waveform->setTransportFollowing(transportFollowButton->getToggleState());
    }
}

//==============================================================================
//private
void MainContentComponent::openButtonClicked()
{
    FileChooser chooser("Select a audio file to play...",
                        File::nonexistent,
                        "*.wav, *.wave, *.aif, *.aiff");
    if(chooser.browseForFileToOpen())
    {
        File file(chooser.getResult());
        waveform->readFromFile(file);
        setAudioFile(&file);
        changeTransportState(TransportState::Stop);
    }
}

void MainContentComponent::settingButtonClicked()
{
    std::cout<<"settingButton"<<std::endl;
    bool showMIDIInputOptions = false;
    bool showMIDIOutputOption = false;
    bool showChannelAsSterepPairs = true;
    bool hideAdvancedOptions = false;
    
    AudioDeviceSelectorComponent settings(deviceManager,
                                          0, 0,//AudioInputChannels: min/Max
                                          1, 2,//AudioOutputChannels: min/Max
                                          showMIDIInputOptions,
                                          showMIDIOutputOption,
                                          showChannelAsSterepPairs,
                                          hideAdvancedOptions);
    settings.setSize(500, 400);
    DialogWindow::showModalDialog(String("Audio Settings"),
                                  &settings,
                                  TopLevelWindow::getTopLevelWindow(0),
                                  Colours::white,
                                  true,//エスケープキーでウインドウを閉じる
                                  true,//ウインドウサイズ可変
                                  true);//ウインドウ右下のドラッグでウインドウサイズ可変
}

void MainContentComponent::playButtonClicked()
{
    std::cout<<"playButton clicked"<<std::endl;
    transportSource.start();
    changeTransportState(TransportState::Play);
}

void MainContentComponent::stopButtonClicked()
{
    std::cout<<"stopButton clicked"<<std::endl;
    if(state == TransportState::Pause)
    {
        transportSource.setPosition(0.0);
        changeTransportState(TransportState::Stop);
    }
    else
    {
        transportSource.stop();
        changeTransportState(TransportState::Pause);
    }
}

void MainContentComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &deviceManager)
    {
        AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.getAudioDeviceSetup(setup);
        
        if(setup.outputChannels.isZero())
        {
            setAudioFile(nullptr);
        }
    }
}

void MainContentComponent::changeTransportState(TransportState newState)
{
    if(state != newState)
    {
        state = newState;
        switch (state) {
            case TransportState::NoFile:
                std::cout<<"NoFile"<<std::endl;
                playButton->setEnabled(false);
                stopButton->setEnabled(false);
                break;
            case TransportState::Play:
                std::cout<<"Play"<<std::endl;
                playButton->setEnabled(false);
                stopButton->setButtonText("Pause");
                stopButton->setEnabled(true);
                break;
            case TransportState::Stop:
                std::cout<<"Stop"<<std::endl;
                playButton->setButtonText("Play");
                playButton->setEnabled(true);
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(false);
                break;
            case TransportState::Pause:
                std::cout<<"Pause"<<std::endl;
                playButton->setButtonText("Restart");
                playButton->setEnabled(true);
                stopButton->setButtonText("Back to head");
                stopButton->setEnabled(true);
                break;
        }
    }
}

void MainContentComponent::setAudioFile(File* file)
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
