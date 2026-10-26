# ScreechForge 0.1 — screech synthesizer prototype

Source project, not an installable VST3 binary. Windows x64 is the intended first target; the JUCE wrapper has not been compiled or tested in a DAW here.

## Build online with GitHub

See `BUILD-ON-GITHUB.md` for browser-only upload and build instructions.

## Build on Windows

1. Install Visual Studio 2022 with **Desktop development with C++**, Git, and CMake 3.22 or later. Enable CMake on PATH.
2. Extract this entire folder somewhere writable. Double-click `build-windows.bat`. Internet access is required to fetch pinned JUCE 8.0.6 and its dependencies on first configuration.
3. After a successful build, copy the entire `ScreechForge.vst3` bundle from `build/ScreechForge_artefacts/Release/VST3` into `C:\Program Files\Common Files\VST3` (Windows may request administrator permission).
4. Rescan plug-ins in Studio One, then drag ScreechForge from Instruments onto a new instrument track. Play MIDI notes or draw them in the piano roll.

The Standalone build is also generated. Select your audio/MIDI device in its settings to play it with a MIDI controller. The basic editor has parameter sliders, not a piano keyboard.

## Sound design

Start around MIDI notes 48–72. Short notes suit rhythmic screeches; held notes reveal filter movement.

- FM bite / ratio: metallic edge and harmonic character.
- Filter Hz / resonance: nasal, screaming emphasis.
- Distortion: saturation before the filter.
- Pitch sweep / sweep decay: initial rise or fall towards the played note. Positive values descend; negative values rise.
- Motion Hz / depth: free-running movement of filter frequency and FM intensity, not tempo synced.
- Release: how quickly the sound fades after note-off.
- Output: final level; start low.

Three presets are accessible through the host's program selector: Classic Screech, Metal Howl, Descending Terror. All sound parameters support host automation and session recall. Pitch bend is ±2 semitones. Velocity controls amplitude.

## Prototype limits

Monophonic, latest-note priority with no return to earlier held notes. MIDI channels share one voice. No sustain-pedal support, delay, reverb, randomize, or custom graphical skin. Parameters update per audio block without smoothing, so abrupt automation can click. The nonlinear core runs at 4x with simple box decimation: aliasing can remain, especially on high notes. This is an initial sound-design prototype, not a production-qualified commercial plugin.

## Verification

The standalone C++ engine test checks finite/bounded output at extreme settings, audible note response, note-off decay, reset, and overlapping note-offs at 44.1/48/96 kHz. Run `g++ -std=c++17 -O2 tests/engine.cpp -o engine-test` then run the executable. It does not validate the JUCE wrapper, VST3 host loading, GUI, automation, or session recall; these still require a full build and host testing.

## Dependencies and licensing

JUCE is fetched from https://github.com/juce-framework/JUCE at tag 8.0.6. JUCE and bundled SDK components have their own licensing requirements; review these before distributing binaries. Original project source is MIT licensed; see LICENSE. No JUCE or SDK source is included in this archive.
