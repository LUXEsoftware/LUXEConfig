# How to get started with Key4hep for LUXE simulation and reconstruction

This is a guide to get started with simulations for LUXE with Key4hep. Follow this list of commands.

# Download the LUXE geometry repository
```bash
git clone https://github.com/LUXESoftware/luxegeo
```
# Source the dependencies 
## Setup the software stack with the following command
```bash
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```
This are nightly releases with latests fixes to the key4hep stack not available yet in proper releases. To run a stable proper release change `sw-nightlies` to `sw`. If this works out then you are all set up to build and install luxegeo.

# Building and installing
## Execute the following lines of commands from inside the luxegeo directory

```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=20 \
-DCMAKE_INSTALL_PREFIX=../install \
..
make install
```
It is possible to check if the installation was succesful doing:
```bash 
cd build
ctest --output-on-failure
```

# Running a simulation
First, from the luxegeo directory, source the luxegeo you have just installed using the commands
```bash
source install/bin/thisluxegeo.sh
export luxegeo_DIR=$(pwd)/install/share/luxegeo/compact
```
Once the geometry is is setup you can select a geometry compact file to produce the ddsim simulation. Below there is an example to shoot 100 positrons into the LUXE_v0 geometry (Magnetic field, tracker and ECALp). Specify the compact file path and the name of the output file, which will have EDM4hep format.
```bash
ddsim --compactFile ${luxegeo_DIR}/LUXE/LUXE_v0.xml \
  --numberOfEvents 1 \
  --enableGun \
  --gun.multiplicity 100 \
  --gun.particle e+ \
  --outputFile positrons_LUXE_v0_edm4hep.root
  ```

It is also possible to use a steering file to define all setting of a simulation, then one can easily reproduce the simulation with the same parameters in the future. To run a ddsim simulation using a steering file do
```bash
ddsim --compactFile ${luxegeo_DIR}/LUXE/LUXE_v0.xml   --numberOfEvents 10000   --enableGun   --gun.multiplicity 1   --gun.particle e+   --outputFile positrons_LUXE_v0_sf_edm4hep.root --steeringFile ddsim_pgun.py 
```
where ddsim_pgun.py is the steering file. An example of a steering file can be found in `https://github.com/LUXEsoftware/LUXEConfig/blob/main/simulation/ddsim_pgun.py` 

# EDM4hep output format

## Intro
 The EDM4hep file format are Root files with trees and branches in which the Collection data is stored, for example, an events tree with a branch depending on the detectors and data that was decided to be saved when running the simulation.

 To see the structure of the EDM4hep file and the collections included in it do
```bash
podio-dump <name-of-edm4hep-file>.root
```
 For example, to check the contents in the file made in the example above one should run the command 
```bash
podio-dump positrons_LUXE_v0_sf_edm4hep.root
```
The output from running this command looks like this

```text
input file: positrons_LUXE_v0_sf_edm4hep.root
            (written with podio version: 1.7.0)

datamodel model definitions stored in this file:
- edm4hep (1.0.0)

Frame categories in this file:
Name      Entries  
--------  -------  
runs      1        
metadata  1        
meta      1        
events    10000    
################################### events: 0 ####################################
Collections:
Name (s = subset collection)        ValueType                     Size  ID          
----------------------------------  ----------------------------  ----  --------    
EventHeader                         edm4hep::EventHeader          1     d793ab91    
MCParticles                         edm4hep::MCParticle           1     a1cba250    
PixelSiEcalCollection               edm4hep::SimCalorimeterHit    0     2c095026    
PixelSiEcalCollectionContributions  edm4hep::CaloHitContribution  0     58369ff1    
SiHits                              edm4hep::SimTrackerHit        4     9e42194a    

Parameters:
Name  Type  Elements  
----  ----  --------  
```

In the EDM4hep file, each collection has a name and a ValueType, the ValueType tells us the properties of the elements in a collection. More information about this properties can be found in the EDM4hep documentation (https://edm4hep.web.cern.ch/).
## Collections
Before learning how to read the files, let's learn about the collections and the information one can extract from them. 
### MCParticles
This collection contains information of the particles generated during the simulation, these can be the particles from the gun or secondary particles generated after the interactions with the materials. From this collection some information available about the particles are their energy, mass, vertex position (x,y and z), among others. 

### SiHits
This collection contains the properties of all hits in the tracker produced during the simulation, separated by event. A hit in this collection contains information about the hit coordinates in the tracker material, energy deposited during the hit, as well as information related to the particle which produced the hit. From a tracker hit, one can retrieve the particle in the MCParticle collection and get its energy, or any of the properties of the MCParticle collection. 
### PixelSiEcalCollection
Similarly to the SiHits collection in the simulation file, this collection contains the hit properties (coordinates, deposited energy, etc.) in the calorimeter material. The name 'Pixel' in this collection name refers to the segmentation of the calorimeter being applied in the simulation geometry definition.

## Read files with a Root macro
