# Simulation

This folder contains configuration files for running LUXE detector simulations.

## Positron particle gun

To run a simple positron particle gun do the following

```bash
ddsim --steeringFile ddsim_pgun.py \
    --compactFile ${luxegeo_DIR}/LUXE/LUXE_v0.xml \
    --outputFile luxe_positron_gun.edm4hep.root
```

This will enable a particle gun shooting positrons with a uniform energy
distribution between 0 and 14 GeV. They are all produced with a momentum vector
aligned with the beam axis and at the IP (0, 0, 0). To change the behavior
either edit the `ddsim_pgun.py` steering file or override them via the command
line, e.g.

- `-N <number of events>` to change the number of event
- `--gun.mulitplicity <n>` to change the number of particles per event
- `--outputFile` to change the name of the output file

Use `ddsim --help` to see all of the (many) available command line switches

## Inspecting the output file
For a quick overview use the [`podio-dump`
utility](https://key4hep.web.cern.ch/podio/tools.html#displaying-the-content-of-files-podio-dump).
An introduction to the EDM4hep format (and pointers to additional documentation)
can be found
[here](https://key4hep.github.io/key4hep-doc/main/how-tos/key4hep-tutorials/edm4hep_analysis/edm4hep_api_intro.html).
