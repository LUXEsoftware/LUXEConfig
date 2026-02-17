# Running LUXE reconstruction

LUXE reconstruction is done using the (Key4hep flavored) Gaudi framework. At the
moment the whole reconstruction lives in one configuration (or options) file:
`luxe_reco.py`.

To run reconstruction do

```bash
k4run luxe_reco.py --IOSvc.Input=<input-file> \
                   --outputFile=luxe_reco.edm4hep.root \
                   --compactFile=<path/to/luxe/geometry.xml>
```

At the moment the `--compactFile` is optional and not passing one will not load
the geometry for reconstruction.

The input file (passed via `--IOSvc.Input`) is the output of a LUXE simulation
done via `ddsim`.
