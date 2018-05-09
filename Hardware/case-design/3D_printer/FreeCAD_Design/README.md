# 3D printed case

This case is a first draft of a 3D printed case for the FLipMouse.
If you decide to print the case, you need 6 additional parts:

* 6x M3 screw nuts (RS components: 278-584)

WARNING: This case design is in beta state, it was printed a few times, but due to the lack of the screw nuts, we cannot guarantee a fully functional case.


The case design was done in FreeCAD (Debian Testing x86_64, FreeCAD v0.16 from apt), the exported STL files can be used with all different slicers for all different 3D printers.
You need to print every STL part once to have the FLipMouse case.


* FLipMouse_Prusa_Slic3r_settings.ini

This is the settings file, which was used by the Slic3r (1.31.6-prusa3d-win64)


# FreeCAD - changing the version number

If you want to change the version string/version number you need to select a Font file (except you use Debian).
In this folder, there is the FreeSans.ttf, which was used. You need to select this file in the properties of the "versionNumer" shape file.
