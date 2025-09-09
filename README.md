# Sync_Robo_Verb

![Screenshot](data/screenshot.png)

A weird, metallic reverb that randomly flips its internal switches in time with your music.

Built on top of [Kushview's Roboverb](https://github.com/kushview/roboverb), this thing takes the classic "robotic" sound of Schroeder reverbs and makes it dance. Instead of just sitting there being static, the filter switches now flip on and off rhythmically, creating evolving textures that sync to your DAW's tempo.

## About Schroeder Reverbs

Back in 1962, Manfred Schroeder figured out how to make digital reverb using parallel comb filters feeding into series allpass filters. As detailed in [ValhallasDSP's excellent explanation](https://valhalladsp.com/2009/05/30/schroeder-reverbs-the-forgotten-algorithm/), it's a simple algorithm that can sound pretty metallic and ringy - which is usually considered a bug, but here it's the feature.

The basic idea is 8 comb filters running in parallel, then 4 allpass filters in series. Each filter can be switched on or off individually, which dramatically changes the sound. Traditional implementations keep these switches static, but that's boring.

## What's New

The randomization system lets you:
- Set how often switches flip (16th notes to 8 bars)  
- Control the probability of changes happening
- Target specific filter types (combs, allpass, or both)
- Watch it all happen in real-time on the interface

Turn it off and you get the original Roboverb behavior. Turn it up and things get chaotic pretty fast.

## Basic Controls

Same as the original: Room Size, Damping, Wet/Dry mix, Stereo Width, and Freeze mode. Plus the 12 filter switches you can flip manually or let the randomizer handle. Crossfade control sets how long switch changes take (instant to 1/4 note), smoothing transitions between filter states.


## License

This is free and open source software. Since it's built on Kushview's GPL-licensed Roboverb, this derivative work is also released under the GPL license.

## Acknowledgments

SyncRoboVerb builds upon the excellent foundation of the original Roboverb plugin, extending its distinctive Schroeder-based architecture with tempo-synchronized randomization capabilities. Special thanks to [Kushview](https://github.com/kushview) for creating such a solid base for experimentation.

## Building
SyncRoboVerb can be built with CMake.

```bash
git submodule update --init --recursive --depth=1
cmake -Bbuild -GNinja
cd build
ninja -j4
```

## macOS Installation Note

Since this isn't code-signed, macOS will quarantine it. After installing to the right folder, that is
`/Library/Audio/Plug-Ins/VST3/` for VST3 and
`/Library/Audio/Plug-Ins/Components/` for AU


you'll need to run:

```bash
# For VST3
sudo xattr -r -d com.apple.quarantine /Library/Audio/Plug-Ins/VST3/SyncRoboVerb.vst3

# For AU
sudo xattr -r -d com.apple.quarantine /Library/Audio/Plug-Ins/Components/SyncRoboVerb.component
```
