#pragma once
#include <algorithm>
#include <cmath>

// Monophonic core, four-times oversampled nonlinear oscillator/filter path.
struct ScreechEngine {
    struct Params {
        float fm=4, ratio=2, cutoff=1600, resonance=0.65f, drive=8;
        float sweep=24, decay=0.22f, release=0.12f, motion=5, depth=0.4f, gain=0.2f;
    } p;
    double sr=44100, phase=0, mod=0, lfo=0;
    float amp=0, pitchEnv=0, low=0, band=0, dcIn=0, dcOut=0, velocity=1;
    int note=-1; bool gate=false;
    void prepare(double rate) { sr=rate; reset(); }
    void reset() { phase=mod=lfo=0; amp=pitchEnv=low=band=dcIn=dcOut=0; note=-1; gate=false; }
    void on(int n,float v) { note=n; velocity=v; gate=true; pitchEnv=1; }
    void off(int n) { if(n==note) gate=false; }
    float sample(float bend=0) {
        constexpr double pi=3.141592653589793;
        const double rate=sr*4;
        float out=0;
        for(int k=0;k<4;++k) {
            amp += ((gate?velocity:0)-amp)*(1-std::exp(-1.0/(rate*(gate?0.003:std::max(0.005f,p.release)))));
            pitchEnv *= std::exp(-1.0/(rate*std::max(0.01f,p.decay)));
            const double hz=std::min(rate*0.12,440*std::pow(2.0,(note-69+bend+p.sweep*pitchEnv)/12));
            phase += hz/rate; phase -= std::floor(phase);
            mod += hz*p.ratio/rate; mod -= std::floor(mod);
            lfo += p.motion/rate; lfo -= std::floor(lfo);
            const float movement=std::sin(2*pi*lfo);
            float x=std::sin(2*pi*phase+p.fm*(1+0.25f*movement*p.depth)*std::sin(2*pi*mod));
            x=std::tanh(x*p.drive);
            const float fc=std::clamp(p.cutoff*std::pow(2.0f,p.depth*movement*2),80.0f,float(sr*0.42));
            // Topology-preserving state variable bandpass.
            const float g=std::tan(pi*fc/rate), damping=2-1.9f*p.resonance;
            const float a=1/(1+g*(g+damping));
            const float v1=a*(band+g*(x-low));
            const float v2=low+g*v1;
            band=2*v1-band; low=2*v2-low;
            out += std::tanh(v1*2)*amp*0.25f;
        }
        // DC blocker followed by bounded output. Simple box decimation is an
        // intentional prototype limitation; not a production anti-alias filter.
        float y=out-dcIn+0.995f*dcOut; dcIn=out; dcOut=y;
        return std::tanh(y)*p.gain;
    }
};
