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
    
    setSize (1080, 720);
    
//    formatManager.registerBasicFormats();
//    sourcePlayer.setSource(&transportSource);
//    deviceManager.addAudioCallback(&sourcePlayer);
    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addChangeListener(this);
    aoiPlay = std::make_unique<AoiPlayAudioFile>(deviceManager);
//    transportSource.addChangeListener(this);
    
}

MainContentComponent::~MainContentComponent()
{
    
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
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    
    // You can add your drawing code here!
}

void MainContentComponent::resized()
{
    const int margin_width = 30;
    const int button_hight = 30;

    openButton->setBounds(margin_width, 10, (getWidth() - margin_width * 2) / 2, button_hight);
    settingButton->setBounds(getWidth() / 2, 10, (getWidth() - margin_width * 2) / 2, button_hight);
    playButton->setBounds(margin_width, 50, (getWidth() - margin_width * 2) / 2, button_hight);
    stopButton->setBounds(getWidth() / 2, 50, (getWidth() - margin_width * 2) / 2, button_hight);
}

void MainContentComponent::buttonClicked(Button *button)
{
    if(button == openButton.get())
    {
        openButtonClicked();
    }else if(button == settingButton.get())
    {
        settingButtonClicked();
    }else if(button == playButton.get())
    {
        playButtonClicked();
    }else if(button == stopButton.get())
    {
        stopButtonClicked();
    }
}

//==============================================================================
//private
void MainContentComponent::openButtonClicked()
{
//    std::cout<<"openButton clicked"<<std::endl;
//    transportSource.setSource(nullptr);//
//    FileChooser chooser("Select a audio file to play...", File::nonexistent);
//    if(chooser.browseForFileToOpen())
//    {
//        int maxNumChannles = 2;
//        File file(chooser.getResult());
//        readerSource.reset(new AudioFormatReaderSource(formatManager.createReaderFor(file), true));
//        formatReader.reset(formatManager.createReaderFor(file));
//        double fileSampleRate = formatReader->sampleRate;
//        transportSource.setSource(readerSource.get(),
//                                  0,
//                                  nullptr,
//                                  fileSampleRate,
//                                  maxNumChannles);
//        playButton->setEnabled(true);
//        changeTransportState(TransportState::Stopped);
//    }
    aoiPlay->openDialogAndSetSource();
    changeTransportState(TransportState::Stop);
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
    changeTransportState(TransportState::Play);
//    if(state == TransportState::Stop)
//    {
//        changeTransportState(TransportState::Play);
//    }
//    else if(state == TransportState::Play)
//    {
//        changeTransportState(TransportState::Pause);
//    }
}

void MainContentComponent::stopButtonClicked()
{
    std::cout<<"stopButton clicked"<<std::endl;
    if(state == TransportState::Pause)
    {
        changeTransportState(TransportState::Stop);
    }
    else
    {
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
//            sourcePlayer.setSource(nullptr);
            aoiPlay->setSource(nullptr);
        }
    }
//    else if(source == &transportSource)
//    {
//        std::cout<<"AudioTransportSource Listner"<<std::endl;
//        if(transportSource.isPlaying())
//        {
//            changeTransportState(TransportState::Playing);
//        }
//        else
//        {
//            if(state == TransportState::Stoopping || state == TransportState::Playing)//if(state == TransportState::Stoopping)???
//            {
//                changeTransportState(TransportState::Stopped);
//            }
//            else if(state == TransportState::Pausing)
//            {
//                changeTransportState(TransportState::Paused);
//            }
//        }
//    }
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
                aoiPlay->play();
                break;
            case TransportState::Stop:
                std::cout<<"Stop"<<std::endl;
                playButton->setButtonText("Play");
                playButton->setEnabled(true);
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(false);
                aoiPlay->backToStartPoint();
                break;
            case TransportState::Pause:
                std::cout<<"Pause"<<std::endl;
                playButton->setButtonText("Restart");
                playButton->setEnabled(true);
                stopButton->setButtonText("Back to head");
                stopButton->setEnabled(true);
                aoiPlay->stop();
                break;
        }
    }
    
}
