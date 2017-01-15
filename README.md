# earthShape
An application built with openFrameworks C++ toolkit for drawing Earth based on actual elevation data. I used quaternionLatLongExample example project which draws sphere and couple of coordinates on it with OpenGL. Instead of showing Cities my application loads actual Earth elevation data and draws it.

![](https://optiklab.github.io/blog/img/earthShapeResult.jpg)

How to run this project
-----------------------

1. Download openFramework release library for Visual Studio 2015 from here http://openframeworks.cc/download/. For example, I have a of_v0.9.8_vs_release.

2. Put source files from this repository into \of_v0.9.8_vs_release\apps\earthShape\ fodler.

3. Download elevation data from open URL: https://dds.cr.usgs.gov/srtm/version2_1/SRTM30/

To do this you may use *wget* utility if you use *nix-like operating system. For Windows I wrote another small [application](https://github.com/optiklab/DirCopierApp) which allows to easily download all of this information. You may build and run it, or even simly run, because executable file is also [provided](https://github.com/optiklab/DirCopierApp/tree/master/DirCopierApp/bin/Release).

4. After elevation files was loaded all yoou need is to put *.hdr and *.dem files into \geodata\ subfolder. Just like this:

![](https://optiklab.github.io/blog/img/earthShape.JPG)

*.hdr file is header file, which describes content of *.dem file allowing to correct read this data.

You may get only few files for first test, but to show full Earth data you need to have 54 files in total.

5. When script is done with its work you may open project file earthShape.vcxproj in Visual Studio 2015 (Any version), compile and run by F5.
