# Wave Digital Filters

This repository contains code for real-time implementations of virtual analog circuit models using wave digital filters.

**NOTE: The WDF library in this repository is now maintained as a part of the
[chowdsp_utils](https://github.com/Chowdhury-DSP/chowdsp_utils) JUCE module. This repository now serves as a reference
of how to implement some example circuits using the WDF code.**

## Organization

`modules/` contains git submodules for JUCE and chowdsp_utils, the latter of which comtains the WDF library.

`WDFPrototyper/` contains a more advanced plugin that allows users to create and manipulate a WDF circuit model at run-time.

All other directories contain an example implementation of a single WDF circuit model.

## Background

Wave digital filters (WDFs) are a unique and exciting
circuit modelling technique that allows the model to be
broken down into individual components and connections.
See the resources below for more information.

## Resources

[1] Alfred Fettweis, "Wave Digital Filters: Theory and Practice",
1986, IEEE Invited Paper,
[link](https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=1457726).

[2] Julius Smith, "Wave Digital Filters", (Chapter from *Physical
Audio Signal Processing*) [link](https://ccrma.stanford.edu/~jos/pasp/Wave_Digital_Filters_I.html).

[3] David Yeh and Julius Smith, "Simulating Guitar Distortion Circuits
Using Wave Digital And Nonlinear State Space Formulations", Proc. of the
11th Int. Conference on Digital Audio Effects, 2008,
[link](http://legacy.spa.aalto.fi/dafx08/papers/dafx08_04.pdf).

[4] Kurt Werner, et al., "Wave Digital Filter Modeling of Circuits
with Operational Amplifiers", 24th European Signal Processing Conference,
2016, [link](https://www.eurasip.org/Proceedings/Eusipco/Eusipco2016/papers/1570255463.pdf).

[5] Kurt Werner, et al., "Resolving Wave Digital Filters with
Multiple/Multiport Nonlinearities", Proc. of the 18th Int. Conference
on Digital Audio Effects, 2015, [link](https://ccrma.stanford.edu/~jingjiez/portfolio/gtr-amp-sim/pdfs/Resolving%20Wave%20Digital%20Filters%20with%20MultipleMultiport%20Nonlinearities.pdf).

[6] Kurt Werner, "Virtual Analog Modeling of Audio Circuitry Using
Wave Digital Filters", PhD. Dissertation, Stanford University, 2016,
[link](https://stacks.stanford.edu/file/druid:jy057cz8322/KurtJamesWernerDissertation-augmented.pdf).

[7] Jingjie Zhang and Julius Smith, "Real-time Wave Digital Simulation
of Cascaded Vacuum Tube Amplifiers Using Modified Blockwise Method",
Proc. of the 21st International Conference on Digital Audio Effects,
2018, [link](https://www.dafx.de/paper-archive/2018/papers/DAFx2018_paper_25.pdf).

## Licensing

The code in this repository belongs to Jatin Chowdhury, except the
implementation of the Wright-Omega function approximations (`shared/omega.h`)
which belongs to Stefano D'Angelo. More licensing details are
still being worked out.
