# Bassman Preamp

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/C0C5AYNGR)
 
An open source emulation project of the 5F6-A revision of the Fender Bassman preamp. A fork of the more ambitious project of emulating the Versatone Pan-O-Flex. Built to better understand C++, real-time DSP, JUCE, GUI, and Object Oriented Programming.

Current Technologies used:

Antiderivative based antialiasing to reduce the amount of oversampling to 2x the native samplerate. https://aaltodoc.aalto.fi/bitstream/handle/123456789/30723/article5.pdf?sequence=8&isAllowed=y

DISCRETIZATION OF THE ’59 FENDER BASSMAN TONE STACK https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.422.5685&rep=rep1&type=pdf

interstage highpass and miller effect lowpass filters as described in The Art of VA Filter Design https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_1.1.1.pdf



Dropping this here, makes it feel more optional to simulate tubes accurately to get good results. http://dafx10.iem.at/proceedings/papers/MacakSchimmel_DAFx10_P12.pdf
