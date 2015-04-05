#Project
This was a project for the municipal physics conference in my town.
<b>I am really sorry for the code!</b>

#Dependences
- NTL
- FreeGLUT

#Installation
`Makefile` has the following targets:
- `calculation`       -- program for calculating
- `drt`               -- program for drawing graphs
- `comparison`        -- program for comparison graphs' forms
- `max`               -- program for searching the optimal angle of flight
- `all`               -- it all together

- `old_calculation` | -- old versions of the programs.
- `old_drawgraph`   |    They depends on MPFR library
- `old_max`         |    and have no calculating optimization features.
- `everything`        -- common target

#Usage
`<b>calculation</b> [datafile]` -- By default, "data" name is used.
`<b>drt</b> datafile [datafile2 .. ]` -- Datafilenames you've used there ↀ (without extentions).
`<b>compare</b> datafile [datafile2 .. ]` -- All the same.
`<b>max</b> -- Have no arguments.
