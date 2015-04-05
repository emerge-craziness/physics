#Project
This was a project for the municipal physics conference in my town.<br>
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
<br>
- `old_calculation`
- `old_drawgraph`  
- `old_max` -↑ Old versions of the programs. They depends on MPFR library and have no calculating optimization features.
- `everything`        -- common target

#Usage
<b>`calculation`</b>` [datafile]` -- By default, "data" name is used.<br>
<b>`drt`</b>` datafile [datafile2 .. ]` -- Datafilenames you've used above (without extentions).<br>
<b>`compare`</b>` datafile [datafile2 .. ]` -- All the same.<br>
<b>`max`</b> -- Has no arguments.<br>
