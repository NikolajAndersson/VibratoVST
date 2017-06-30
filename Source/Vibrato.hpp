/*
  ==============================================================================

    Vibrato.h
    Created: 30 Jun 2017 4:42:28pm
    Author:  Nikolaj Schwab Andersson
    
  ==============================================================================
    Vibrato plugin optimised with boost.simd

  
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Vibrato{
public:
    float rate = 5;
    float depth = 1;
    float mix = 1;
    Vibrato(){
        for (int ii = 0; ii < channels; ii++){
            for (int j = 0; j < bufferLength; j++){
                vBuffer[ii][j] = 0.0f;
            }
        }
    }
    
    float updateAngle(float freq){
        deltaAngle =  2 * float_Pi * (freq / sampleRate);
        return deltaAngle;
    }
    
    void initialise(double fs, int frame, float freq, float _depth){
        sampleRate = fs;
        samplesPerFrame = frame;
        deltaAngle = updateAngle(freq);
        updateDepth(_depth);
    }
    void updateRate(float newRate){
        rate = newRate;
        deltaAngle = updateAngle(rate);
    }
    void updateDepth(float newDepth){
        depth = newDepth;
        depthCopy = round(depth*sampleRate);
        delay = round(depth*sampleRate);
    }
    
    void updateMix(float newMix){
        mix = newMix;
    }
    
    void process(AudioSampleBuffer& buffer){        
        int numSamples = buffer.getNumSamples();

        float wet = mix;
        float dry = (1-mix);
        
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            int pos = writeIndex[channel];
            float* channelData = buffer.getWritePointer (channel);
            
            for (int i = 0; i < numSamples; i++)
            {
                vBuffer[channel][pos] = channelData[i];
                
                double modfreq = sin(phase[channel]);
                
                phase[channel] = phase[channel] + deltaAngle;
                
                if(phase[channel] > float_Pi * 2)
                    phase[channel] = phase[channel] - (2 * float_Pi);
                
                
                float tap = 1 + delay + depthCopy * modfreq;
                int n = floor(tap);
                
                float frac = tap - n;
                
                int rindex = floor(pos - n);
                
                if (rindex < 0)
                    rindex = rindex + bufferLength;
                
                float sample = 0;
                
                if(rindex == 0){
                    sample += vBuffer[channel][bufferLength-1]*frac + (1-frac)*vBuffer[channel][rindex];
                }
                else{
                    sample += vBuffer[channel][rindex-1]*frac + (1-frac)*vBuffer[channel][rindex];
                }
                
                pos = (pos + 1) % bufferLength;
                
                channelData[i] = dry*channelData[i] + wet*sample;
            }
            writeIndex[channel] = pos;
        }
    }
    
    ~Vibrato(){};
    
private:
    enum{
        bufferLength = 192001,
        channels = 2
    };
    float depthCopy = round(depth*sampleRate);
    float delay = round(depth*sampleRate);
    double sampleRate;
    double deltaAngle;
    int writeIndex[channels] = {0,0};
    float phase[channels] = {0,0};
    int sineindex = 0;
    int samplesPerFrame;
    float vBuffer[channels][bufferLength];
};
