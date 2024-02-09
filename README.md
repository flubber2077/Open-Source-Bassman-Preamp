# Bassman Preamp

## Table of Contents

* Overview
* Features
* Building the Project
* Usage
* Compatibility
* License

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/C0C5AYNGR)
 
## Overview

An open source emulation project of the 5F6-A revision of the Fender Bassman preamp. A fork of the more ambitious project of emulating the Versatone Pan-O-Flex. Built to better understand C++, real-time DSP, JUCE, GUI, and Object Oriented Programming. Released under GPLv3

Current Technologies used:

Antiderivative based antialiasing to reduce the amount of oversampling to 2x the native samplerate. https://aaltodoc.aalto.fi/bitstream/handle/123456789/30723/article5.pdf?sequence=8&isAllowed=y

DISCRETIZATION OF THE ’59 FENDER BASSMAN TONE STACK https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.422.5685&rep=rep1&type=pdf

interstage highpass and miller effect lowpass filters as described in The Art of VA Filter Design https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_1.1.1.pdf



Dropping this here, makes it feel more optional to simulate tubes accurately to get good results. http://dafx10.iem.at/proceedings/papers/MacakSchimmel_DAFx10_P12.pdf


## Features

Vintage tone: Emulates the warm sound of a vintage tube-driven preamp guitar tone.

Parameter controls: Has controls to customize tone including volume, bass, middle, treble, gain, reverb, and cabinet simulation.

Bright switch: Emulates a bright switch found on vintage amps to enhance high-frequency response.

## Building the project

To get started with the plugin:

1. Clone the repository.
2. Open the project in your preferred C++ development environment 
3. Install and set up JUCE
4. Compile the project to generate the plugin binary for your desired platform (VST, AU, AAX).

## Usage

1. Load the plugin in your digital audio workstation (DAW) as a VST, AU, or AAX plugin.
2. Adjust the parameters to achieve your desired tone.

## Compatibility

The plugin is compatible with digital audio workstations (DAWs) including:

Ableton Live
Pro Tools
Logic Pro
FL Studio
Reaper

## License

The project is licensed under the GNU General Public License v3.0. Please see the LICENSE file for more info.
