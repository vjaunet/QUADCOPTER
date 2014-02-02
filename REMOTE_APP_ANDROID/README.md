Remote Control App
------------------

This folders contains the source files used to create the Remote control app
runing on an Android Device.

It is based on Gareth Owen's code (https://code.google.com/p/owenquad/) who
kindly authorized me to re-distribute it under the GPLv3 license.
Many thanks to him.

- Documentation
----------------
coming soon...

- History
-----------

- 02-02-2014
 - Built quadcontrol-v0.1.apk
 - remote contol Thrust and YPR data are now only sent when the PID is started :
   A pause and Resume timer functions are implemented
 - New shutdown button to shutdown the Rpi, the quad_pilot program starts now at boot

- 20-01-2014
 - Implementation of min max values for PID
 - Bug fix when sending PId constant (variable msg was passed by value instead of reference)

- 20-01-2014
 - Implementation of seekbars to finetune the PID controller constants
 - Brush-up of sending Message functions : Now only send PID values of 
   the PId of interest instead of all the PIDs
 -Brush colors of button for more clarity


