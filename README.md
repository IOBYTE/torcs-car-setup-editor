This is a fork of the torcs car setup editor from https://sourceforge.net/projects/dreamofmotorsport/files/3rd%20Party/Tools/

The short term goal is to get it working with Speed Dreams cars.

The long term goal is to get it intregrated into Speed Dreams.

Progress
--------

I found out that the xml reading is fundamentally broken and I don't want to fix it only to replace it when the code moves over to speed dreams.  I want to use the speed dreams xml code but I want to do it out of that tree for now.  This is a show stopper for me.

I tried adding freeglut and glui to the speed dreams 3rdparty dependencies but I can't get the debug version to work the way I want it to. I don't want the 'd' suffix on the debug libraries and I tell CMake that I don't want them but the freeglut CMake file has it hard coded and I can't seem to override it.  I could just build the release versions but I don't want to work that way.  This is a show stopper for me.

I don't see the need for reading and writing 2 different file formats.  I would really like to just use the xml file format.  I don't understand the reason for the second format. I am considering removing the second file format because it's a real PITA to maintain.

Is there a need for seperate configurations for different tracks and drivers anymore?  Speed dreams now has in-game car and driver and track setup so this is redundant.  I'm considering removing this feature.
