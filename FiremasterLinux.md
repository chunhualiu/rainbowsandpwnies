# LATEST UPDATES #
  * POSIX threads will be used for threading
  * Function call notes for CUDA implementation have been posted in [Issue #6](https://code.google.com/p/rainbowsandpwnies/issues/detail?id=#6)
  * Make sure you download the latest gecko SDK before compiling : http://releases.mozilla.org/pub/mozilla.org/xulrunner/releases/1.9.2/sdk/xulrunner-1.9.2.en-US.linux-i686.sdk.tar.bz2
  * Make sure to modify the Makefile accordingly (point XUL\_INCLUDE and XUL\_LIB variables to the directory you installed gecko in)

Read more at http://www.rainbowsandpwnies.com/forums/

# Introduction #

FireMasterLinux is the Linux port of popular of FireMaster (http://securityxploded.com/tools.php#FireMaster) tool created by Nagareshwar Talekar. FireMaster is the first ever built tool to recover the lost master password of Firefox. In order to protect the stored login passwords, Firefox uses master password. If the master password is forgotten, then there is no way to recover the master password and user will lose all the stored login passwords as well.

In this direction, FireMaster helps in recovering lost master password to get back all the stored passwords. FireMaster uses combination of techniques such as dictionary, hybrid and brute force to recover the master password from the Firefox key database file.

Often it takes long hours and some times days together to completely recover the master password based on its length and complexity. Hence one end up performing recovery operation for days continuously. However Windows system often goes slower as the day passes and performance is not optimal when it comes to continuous processing.

In such a case where it involves processing for long hours with consistent high-performance, Linux becomes the ideal platform. To address this exact problem, we have started the project, FireMasterLinux....!


Curently project is under development and progress of the same will be updated on this page periodically.  If you would like to join hands in this project, please drop us a line at broseidon@blackopsecurity.com, or join the discussion on the forums at http://www.rainbowsandpwnies.com/forums/

# Team Members #
  * broseidon
  * endeavormac
  * oldgregg
  * MustardBedroomWrench

# To do: #
  * Port a working version (brute force only) to linux [16 MAY 2010](Completed.md)
  * Incorporate dictionary crack [24 MAY 2010](Completed.md)
  * Incorporate and optimize hybrid crack to include not just addition, but case changes within dictionary words [9 JUN 2010](Completed.md)
  * Use threading to improve application performance
  * Use with CUDA/OpenCL
  * Roll into Nagareshwar's FirePassword tool
  * Write a Metasploit module to grab requisite databases & automatically check for a master password set

http://www.rainbowsandpwnies.com/forums/