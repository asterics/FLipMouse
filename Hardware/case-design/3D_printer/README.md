# 3D printed case

This case is a first draft of a 3D printed case for the FLipMouse.
If you decide to print the case, you need 4 additional parts:

* 2x M4 screw nuts (RS components: 278-607)
* 2x M3 screw nuts (RS components: 278-584)

WARNING: This case design is in beta state, it was printed a few times, but due to the lack of the screw nuts, we cannot guarantee a fully functional case.


The case design was done in FreeCAD (Debian Testing x86_64, FreeCAD v0.16 from apt), the exported STL files can be used with all different slicers for all different 3D printers.
You need to print every STL part once to have the FLipMouse case.

We also provide fully sliced G code files for the Prusa i3 MK2 3D printer, which was used by us to print the parts. We used the standard Prusa settings at quality level "normal", in this case the print of one case takes about four hours.
There are different types of sliced files:

* FLipMouse_Prusa_i3_MK2_PLA_NORMAL.g

This file can be used to print directly one FLipMouse case with the Prusa i3 MK2 and PLA material. We did not tested this file with any other materials!

* FLipMouse_Prusa_i3_MK2_PLA_NORMAL_MULTI1.g

TBD: This file will contain many same parts for production (e.g. only main cases)

* FLipMouse_Prusa_i3_MK2_PLA_NORMAL_MULTI2.g

TBD: This file will contain many same parts for production (e.g. only case covers)

* FLipMouse_Prusa_i3_MK2_PLA_NORMAL_MULTI3.g

TBD: This file will contain many same parts for production (e.g. only inside parts)
