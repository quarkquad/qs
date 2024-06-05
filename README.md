# QS

QS is a library for encoding and decoding audio in [Quadraphonic Sound (Regular
Matrix)](https://en.wikipedia.org/wiki/QS_Regular_Matrix). The library is the core of [QUARK](https://quark.cykik.com), a DAW plugin for quadraphonic spatial audio production.

## Usage

While the project currently relies on [JUCE](https://juce.com) structures, but we hope to make this usable outside of
JUCE in the future. For now and simplicity, JUCE is the way.

### As a CMake Package

Clone and install the CMake package:

```shell
$ git clone https://github.com/quarkquad/qs.git qs
$ cd qs
$ cmake -B build-install -DCMAKE_INSTALL_PREFIX="..."
$ cmake --build build-install --target install
```

Add the following to your `CMakeLists.txt`:

```
find_package(JUCE CONFIG REQUIRED)
find_package(QS CONFIG REQUIRED)

target_link_libraries(MyPlugin
    PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    quark::qs
)
```

### As a Project Subdirectory

Clone this repository as a subdirectory in your JUCE project:

```shell
$ git clone https://github.com/quarkquad/qs.git qs
```

Then add the following to your `CMakeLists.txt`:

```
add_subdirectory(JUCE)
add_subdirectory(qs)

target_link_libraries(MyTarget
    PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    quark::qs
)
```

You can find a few JUCE plugin examples in the [examples/ directory](examples).

## Encoding

QS provides [a static encoder](qs/encode/Encoder.h) that encodes 4 input channels into 2 output channels. The
channels layout is as follows:

```
┌────────────┬────────────┬────────────┬────────────┐
│ Front Left │Front Right │ Rear Left  │ Rear Right │
│     0      │     1      │     2      │     3      │
└─────┬──────┴─────┬──────┴─────┬──────┴─────┬──────┘
      │            │            │            │       
┌─────▼────────────▼────────────▼────────────▼──────┐
│                                                   │
│                   Encoder (4-2)                   │
│                                                   │
└──────────────────┬────────────┬───────────────────┘
                   │            │                    
             ┌─────▼──────┬─────▼──────┐             
             │ Total Left │Total Right │             
             │     0      │     1      │             
             └────────────┴────────────┘
```

## Decoding

QS provides [an active multiband decoder](qs/decode/MultiBandDecoder.h) modeled after the Sansui Vario-Matrix
decoding process and QSD-1 hardware decoder. The QSD-1 was a tri-band decoder that processed high, mid and low
frequencies independently. While simplistic, the result is musical and convincing.

The channels layout is as follows:

```
             ┌────────────┬────────────┐             
             │ Total Left │Total Right │             
             │     0      │     1      │             
             └─────┬──────┴─────┬──────┘             
                   │            │                    
┌──────────────────▼────────────▼───────────────────┐
│                                                   │
│                   Decoder (2-4)                   │
│                                                   │
└──────┬───────────┬────────────┬────────────┬──────┘
       │           │            │            │       
┌──────▼─────┬─────▼──────┬─────▼──────┬─────▼──────┐
│ Front Left │Front Right │ Rear Left  │ Rear Right │
│     0      │     1      │     2      │     3      │
└────────────┴────────────┴────────────┴────────────┘
```

The frequency bands are produced by passing the input through a second-order Butterworth filter. Coefficients for the
filters are provided at initialization time. Coefficients are quantized between sample rates so it's recommended to
calculate coefficients for 48kHz. 

The active control logic behavior can be tuned by adjusting envelope timings, comparision width and gain of each frequency band.

## Generating Filter Coefficients

To calculate coefficients for the `MultiBandDecoder` you can use `script/coefficients/generate` utility. First construct
a Python venv so you can install some dependencies.

```shell
$ python3 -m venv coeffs-venv 
$ coeffs-venv/bin/
$ source coeffs-venv/bin/activate
(coeffs-venv) $ pip3 install -r script/coefficients/requirements.txt
```

Now run the script to generate coefficients. It accepts a list of band-stops and produces coefficients each band.

```shell
(coeffs-venv) $ script/coefficients/generate --stops 1000
1000.0 [[0.003916126660547369, 0.007832253321094738, 0.003916126660547369, 1.0, -1.815341082704568, 0.8310055893467575]]
1000.0 [[0.9115866680128315, -1.823173336025663, 0.9115866680128315, 1.0, -1.815341082704568, 0.8310055893467575]]
```
We've calculated coefficients for a dual-band decoder with a cross-over point of 1kHz. The bands are a lowpass at 1kHz
and a highpass at 1kHz.

Need more bands? Add more stops:

```shell
(coeffs-venv) $ script/coefficients/generate --stops 1000 4000
1000.0 [[0.003916126660547369, 0.007832253321094738, 0.003916126660547369, 1.0, -1.815341082704568, 0.8310055893467575]]
1000.0 4000.0 [[0.029954582208092474, 0.059909164416185004, 0.029954582208092526, 1.0, -1.4835932220566492, 0.6582736235325576], [1.0, -2.000000000000001, 1.0000000000000009, 1.0, -1.851415359120339, 0.8720718779575456]]
4000.0 [[0.6893061687675816, -1.378612337535163, 0.6893061687675816, 1.0, -1.2796324249978086, 0.477592250072517]]
```

Here we've calculated coefficients for a tri-band decoder with stops at 1kHz and 4kHz. The bands have the following
shape: lowpass at 1kHz, bandpass between 1Hz and 4kHz, highpass at 4kHz.

The `script/coefficients/generate` accepts a `--plot` flag to visualize the filters.

## Related Research

- [QS Quadrophonic Synthesizer: What It Does and How It Does It](https://secure.aes.org/forum/pubs/conventions/?elib=2497)
- [The Principles and Performance of the Sansui QS Vario-Matrix](https://www.aes.org/e-lib/browse.cfm?elib=1729)
- [Matrix "Quad" Systems](http://www.wendycarlos.com/surround/surround4.html)
- [The Design of Wideband Analog 90° Phase Differencing Networks without Large Spread of Capacitor Values](http://electronotes.netfirms.com/EN168-90degreePDN.PDF)
- [Calculation of the Poles of 90° Phase Difference Networks](http://electronotes.netfirms.com/MEHCh6aPart.PDF)
- [Design of RC Wide-Band 90-Degree Phase-Difference Network](http://ieeexplore.ieee.org/document/4051669/)
- Parameter Quantization in Direct-Form Recursive Audio Filters (Neunaber)
- Implementation and Evaluation of Autonomous Multi-track Fader Control (Mansbridge)
