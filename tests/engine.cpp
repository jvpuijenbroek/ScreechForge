#include "../Source/Engine.h"
#include <cassert>
#include <iostream>
int main() {
    for(double sr:{44100.,48000.,96000.}) for(int note:{0,36,60,100,127}) {
        ScreechEngine e; e.prepare(sr); e.p.fm=16; e.p.ratio=8; e.p.drive=24;
        e.p.resonance=1; e.p.cutoff=16000; e.p.sweep=48;
        assert(e.sample()==0); e.on(note,1);
        double energy=0;
        for(int i=0;i<int(sr);++i) {float x=e.sample(); assert(std::isfinite(x)); assert(std::abs(x)<=e.p.gain); energy+=x*x;}
        assert(energy>0.001); e.off(note);
        float tail=0; for(int i=0;i<int(sr*4);++i) tail=e.sample();
        assert(std::abs(tail)<1e-6); e.reset(); assert(e.sample()==0);
    }
    ScreechEngine e; e.on(60,1); e.on(64,1); e.off(60); assert(e.gate); e.off(64); assert(!e.gate);
    std::cout<<"PASS: finite bounded audio, note response, release and reset at three sample rates\n";
}
