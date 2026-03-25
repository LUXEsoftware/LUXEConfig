# LUXEConfig

Gaudi and other configuration files for running LUXE simulation and
reconstruction workflows. The given examples aim at using a consistent example
configuration, including input and output filenames such that they can be run
step by step. At the moment these are all rather small examples but the aim is
to grow them to a fully fledged LUXE simulation and reconstruction suite.

## Prerequisites

All of the exapmles assume that you are in a suitable software environment, e.g.
a Key4hep stack which can be setup via

``` bash
source /cvmfs/sw.hsf.org/key4hep/setup.sh
```

(Replacing `sw.hsf.org` with `sw-nightlies.hsf.org` will give you the bleeding
edge nightly stack. This stack is less stable but might feature some fixes and
developments that have not yet landed in the releases).

### LUXE geometry

In order to use many of the algorithms and workflows here it is necessary to
have a usable LUXE geometry in DD4hep available. In order to do that follow the
installation instructions of [luxegeo](https://github.com/LUXESoftware/luxegeo).

Make sure to make it known to your software environment via

```bash
source <luxegeo-prefix>/bin/thisluxegeo.sh
export luxegeo_DIR=<luxegeo-prefix>/share/luxegeo/compact
```

### Clone this repository

In order to get all the necessary config and steering files clone this
repository via

```bash
git clone https://github.com/LUXESoftware/LUXEConfig
```

## Running examples and workflows

The commands in this (and other READMEs in the repository) generally assume that
you run the commands in the folders of the README. You will have to adjust
accordingly if you do not do that.

### Simulation

For a simple particle gun exapmle follow the instructions in [simulation](./simulation)

### Reconstruction

For runing a (partial) reconstruction of the simulation see the instructions in [reconstruction](./reconstruction).
