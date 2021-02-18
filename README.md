What's this?
============
This is a platform scroller (somewhat similar to Super Mario with few differences)
game skeleton to use with HGE library, which does very basic functions and leaves
lot of placeholder functions for you to code your very own side-scroller game.

![image](https://raw.githubusercontent.com/kvakvs/hge_skel/master/screen_for_readme.png)

How to make this work?
======================
This thing works with either HGE clone from here https://github.com/kvakvs/hge
or original HGE 1.8.1 located at http://hge.relishgames.com

1. Checkout HGE source from github.com/kvakvs/hge
2. Build HGE
3. Configure `CMakeLists.txt` of HGE_Skeleton to have correct path to the HGE 
	directory. It will be used to find include and link files.
4. Ensure you have recent copy of HGE.dll and BASS.dll in your `bin/`

To create solution file, run one of BAT files attached. You can also run CMake to
support some other IDE, see what it can do:
http://www.cmake.org/cmake/help/cmake-2-8-docs.html#section_Generators
(CLion IDE supports CMakeLists and does not need any BAT files to be executed).
