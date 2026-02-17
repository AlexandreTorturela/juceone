#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceOneAudioProcessor::JuceOneAudioProcessor()
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
 #if ! JucePlugin_IsSynth
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
 #endif
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
    , apvts (*this, nullptr, "PARAMS", createParameterLayout())
{
}

const juce::String JuceOneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceOneAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool JuceOneAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool JuceOneAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double JuceOneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceOneAudioProcessor::getNumPrograms()               { return 1; }
int JuceOneAudioProcessor::getCurrentProgram()            { return 0; }
void JuceOneAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }
const juce::String JuceOneAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}
void JuceOneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void JuceOneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void JuceOneAudioProcessor::releaseResources()
{
}

bool JuceOneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono()
     && out != juce::AudioChannelSet::stereo())
        return false;

 #if ! JucePlugin_IsSynth
    if (out != layouts.getMainInputChannelSet())
        return false;
 #endif

    return true;
#endif
}

juce::AudioProcessorValueTreeState::ParameterLayout
JuceOneAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        paramGainDb,
        "Gain (dB)",
        juce::NormalisableRange<float> (-60.0f, 12.0f, 0.01f),
        0.0f
    ));

    params.push_back (std::make_unique<juce::AudioParameterBool>(
        paramBypass,
        "Bypass",
        false
    ));

    return { params.begin(), params.end() };
}

void JuceOneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                         juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    // limpa canais extras (template padrão)
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    const bool bypass = apvts.getRawParameterValue (paramBypass)->load() > 0.5f;
    if (bypass)
        return;

    const float gainDb = apvts.getRawParameterValue (paramGainDb)->load();
    const float gainLinear = juce::Decibels::decibelsToGain (gainDb);

    buffer.applyGain (gainLinear);
}

juce::AudioProcessorEditor* JuceOneAudioProcessor::createEditor()
{
    return new JuceOneAudioProcessorEditor (*this);
}

void JuceOneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void JuceOneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// factory do plugin: IMPORTANTÍSSIMO retornar a classe certa
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceOneAudioProcessor();
}
