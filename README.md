#Project
[![Build Status](https://travis-ci.org/emerge-craziness/physics.png)](https://travis-ci.org/emerge-craziness/physics)<br><br>
This was a project for the municipal physics conference in my town.<br><br>
<b>I am really sorry for the code!</b>
<br><br>
/* <i>Seems something is wrong with drt. I'm working on it.<br>Plus, max has too old version...</i> */<br><br>
#Dependencies
- NTL
- FreeGLUT
- ncurses
- mpfr (for old versions)

#Installation
`Makefile` has the following targets:
- `calculation`       -- program for calculating
- `drt`               -- program for drawing graphs
- `comparison`        -- program for comparison graphs' forms
- `max`               -- program for searching the optimal angle of flight
- `all`               -- it all together
<br><br>
- `old_calculation`
- `old_drawgraph`  
- `old_max` 
- `everything` -- It is for old versions of the programs. They depend on MPFR library (instead of NTL) and have no calculating optimization features.      

#Usage
<b>`calculation`</b>` [datafile]` -- By default, "data" name is used.<br>
<b>`drt`</b>` datafile [datafile2 .. ]` -- Datafilenames you've used above (without extentions).<br>
<b>`compare`</b>` datafile [datafile2 .. ]` -- All the same.<br>
<b>`max`</b> -- Has no arguments.<br>
