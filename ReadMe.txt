Description :
_ ASLogger : Enable to use a smartphone as a GPS tracker and AHRS. It records the GPS position, angles, speed, accelerations of the smartphone in Downloads/log.txt. This file can be imported in e.g. Microsoft Excel or in MATLAB and Google Earth using the tools provided in http://www.ensta-bretagne.fr/lebars/ASLogger_processing.zip. The GPS raw data are also recorded in Downloads/nmea.txt.
_ HelloIOIO : Modified version of the official IOIO sample to control 3 servomotors through the IOIO board connected to the smartphone.
_ BuggyTriangle : A buggy equipped with a smartphone and a IOIO board makes autonomously a triangle defined by 3 GPS points (to modify in the source code).
_ CamSvcSrv : Enable to use a smartphone as an IP camera. Transfer smartphone camera images by Wi-Fi. You can use e.g. VLC (if MJPEG is selected in RemoteWebcamMultiSrv.txt) or RemoteWebcamCli to view in realtime the video on another computer of your Wi-Fi network. The configuration file should be put in Downloads/RemoteWebcamMultiSrv.txt on the smartphone (change the encoding and line endings to Linux standards). If your smartphone has several cameras, you might need to change the camera id in the configuration file. Check also the IP settings, disable firewall and antivirus if needed. See https://github.com/ENSTABretagneRobotics/RemoteWebcam for the Windows/Linux computer version. You might need to launch and kill the app 2-3 times before it works... Might not work on some smartphones or versions of Android due to OpenCV incompatibilities. Tested on a Samsung Galaxy S3 Mini with Android 4.1.2.
_ GPSSvcSrv : Enable to use a smartphone as an IP GPS on TCP port 4001. Use e.g. telnet to display raw data.
_ AHRSSvcSrv : Enable to use a smartphone as an IP Razor AHRS on TCP port 4007. Use e.g. telnet to display raw data.
_ IOIOSvcSrv : Enable to use a smartphone with a IOIO board as an IP SSC-32 on TCP port 4004. Use e.g. telnet to send SSC-32 commands (e.g. #0P1500).
_ GPSSMS : Send repeatedly SMS with GPS info. Using SMS Backup & Restore (see http://android.riteshsahu.com/) to export the SMS in sms.xml and GPSSMS2KML (see File_conversions), you can export the trajectory to a KML file to display it in Google Earth.

Some similar applications are also partially available for the HTC HD2 (Windows Mobile 6.5), see http://www.ensta-bretagne.fr/lebars/WindowsMobile65.zip .

Installation :
_ From the smartphone, go to this webpage and launch the .apk file (in the bin folder of each project of this webpage). 
_ If you do not have Internet on the smartphone, connect it on a USB port of your computer, copy the .apk downloaded from your computer on the smartphone (e.g. in the Downloads folder of your smartphone), and from the smartphone launch the .apk file.

Prerequisites (to be able to modify the code) : 
_ See setup_android-sdk.pdf (and also http://www.ensta-bretagne.fr/lebars/)
_ App-IOIO0400 (for projects using the IOIO board, see https://github.com/ytai/ioio/raw/master/release/software/App-IOIO0400.zip)
_ android-ndk-r10e-windows-x86 (for projects using OpenCV)
_ OpenCV-2.4.13-android-sdk (for projects using the camera, see https://docs.opencv.org/2.4/doc/tutorials/introduction/android_binary_package/android_dev_intro.html)

Copy the projects in your Eclipse workspace and in Eclipse choose Import Existing projects into workspace.
