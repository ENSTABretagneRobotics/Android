Description :
_ ASLogger : Enable to use a smartphone as a GPS tracker and AHRS. It records the GPS position, angles, speed, accelerations of the smartphone in Downloads/log.txt. This file can be imported in e.g. Microsoft Excel or in MATLAB and Google Earth using the tools provided in http://www.ensta-bretagne.fr/lebars/ASLogger_processing.zip. The GPS raw data are also recorded in Downloads/nmea.txt.
_ HelloIOIO : Modified version of the official IOIO sample to control 3 servomotors through the IOIO board connected to the smartphone.
_ BuggyTriangle : A buggy equipped with a smartphone and a IOIO board make autonomously a triangle defined by 3 GPS points (to modify in the source code).
_ CamSrv : Enable to use a smartphone as an IP camera. Transfer smartphone camera image by Wi-Fi. You can use e.g. VLC (if MJPEG is selected in RemoteWebcamMultiSrv.txt) or RemoteWebcamCli to view in realtime the video on another computer of your Wi-Fi network. The configuration file should be put in Downloads/RemoteWebcamMultiSrv.txt on the smartphone. Check also the IP settings, disable firewall and antivirus if needed. See https://github.com/ENSTABretagneRobotics/RemoteWebcam for the Windows/Linux computer version.
_ GPSSrv : Enable to use a smartphone as an IP GPS on TCP port 4001. Use e.g. telnet to display raw data.
_ AHRSSrv : Enable to use a smartphone as an IP Razor AHRS on TCP port 4007. Use e.g. telnet to display raw data.
_ IOIOSrv : Enable to use a smartphone with a IOIO board as an IP SSC-32 on TCP port 4004. Use e.g. telnet to send SSC-32 commands (e.g. #0P1500).
_ GPSSMS : Send repeatedly SMS with GPS info. Using SMS Backup & Restore (see http://android.riteshsahu.com/) to export the SMS in sms.xml and GPSSMS2KML (see File_conversions), you can export the trajectory to a KML file to display it in Google Earth.

Some similar applications are also partially available for the HTC HD2 (Windows Mobile 6.5), see http://www.ensta-bretagne.fr/lebars/WindowsMobile65.zip .

Installation :
_ From the smartphone, go to this webpage and launch the .apk file (in the bin folder of each project of this webpage). 
_ If you do not have Internet on the smartphone, connect it on a USB port of your computer, copy the .apk downloaded from your computer on the smartphone (e.g. in the Downloads folder of your smartphone), and from the smartphone launch the .apk file.

Prerequisites (to be able to modify the code) : 
_ See setup_android-sdk.pdf (and also http://www.ensta-bretagne.fr/lebars/)
_ App-IOIO0400 (for projects using the IOIO board, see https://github.com/ytai/ioio/raw/master/release/software/App-IOIO0400.zip)
_ android-ndk-r9-windows-x86 (for projects using OpenCV)
_ OpenCV-2.4.9-android-sdk (for projects using the camera)

Copy the projects in your Eclipse workspace and in Eclipse choose Import Existing projects into workspace.
