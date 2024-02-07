Gnubox v1.0
===========

Introduction
------------
Gnubox is a simple program for the Nokia 7650 which allows you to configure 
the smartphone to access the Internet through an Infrared or Bluetooth access 
point.  In other words, it permits you to get IP connectivity without using 
the native phone stack (CSD or GPRS).  On a new 7650, the only app that you 
would be interested in doing this with is the Messaging client.  With Gnubox
you could send and receive mail through an IR or Bluetooth access point 
without having to pay for the packets at premium rates.  But this is only the
beginning.  The 7650 contains Symbian OS 6.1 which is an evolution of the 
powerful EPOC v5 platform that was at the heart of the Psion Series 5 and 
Psion Revo PDAs.  So there's no end of potential applications for this tool.
This author would be particularly interested in seeing VOIP working over 
Gnubox to a Bluetooth access point. Now that would be a *real* internet phone ;-)

Usage
-----
To use this program, simply install it on your 7650 and run it.  You will be 
presented with the current Internet access point (IAP) default settings.  If 
you have not used Gnubox before these will be the CSD/GPRS specific settings
and you should see "DATAPORT" as the CSY.

Steps for connecting using a Linux box operating as an IR Access Point
----------------------------------------------------------------------
1) You need to ensure that you have a functioning IR stack and ppp > 2.4
2) Ensure the following options are in /etc/ppp/options:
   (No - ignore this and see HowTo instead)

	  noauth
	  lock
	  modem
	  debug
	  silent
	  proxyarp
	  ms-dns <your primary dns server here from /etc/resolv.conf>	  
	  ms-dns <your secondary dns server here from /etc/resolv.conf>

3) Ensure you have IP forwarding working:

	  $ echo 1 > /proc/sys/net/ipv4/ip_forward

4) Now you are ready to fire up your pppd session.  Note that you need to 
employ two IP addresses here which are acceptable in terms of your current
network settings.  ie. If you are operating within a NAT address range
(eg. 192.168.1.255) you need to choose your addresses from within that range.
Here's the command for having pppd permanently enabled in server mode:

          $ pppd -detach /dev/ircomm0 LOCAL_IP_ADDR:REMOTE_IP_ADDR &

5) Align the 7650 IR port with the PC IR port.  Now upon connecting a client 
app such as the Messaging client over IrCOMM, you should see lots of PPP 
frames appear in your terminal and after a few seconds, a dialog indicating
that a connection is in progress.

Steps for connecting using a Linux box operating as an Bluetooth Access Point
-----------------------------------------------------------------------------
TBD - I have access to an Axis LANP point.

Steps for connecting using a Bluetooth access point
---------------------------------------------------
TBD

Known Issues
------------
1) The DialUp options won't work on 7650 - there is no dialup TSY to manage
AT commands over an IR/Bluetooth link to a modem-enabled phone.  The standard
TSY that Symbian OS uses to do this is called GSMBSC.TSY but it doesn't
ship with 7650.
2) There looks to be a problem with context deactivation with the 7650.  The way
this manifests itself is that if connection isn't made immediately over say 
IrCOMM, then the IR indicator keeps blinking on the 7650.  It would seem that
Gnubox needs to be modified to really bring down that connection in the event
of failure.
3) I have not tested any of this on any Windows OSes so I can't guarantee that 
it will work for those platforms.

Notes
-----
CSD  = Circuit Switched Data (standard 9600bps GSM connection).
GPRS = General Packet Radio System (always on).
ETEL = Symbian OS Telephony server
TSY  = Symbian OS ETEL plugin module.  GSMBSC.TSY is the two-box TSY.
C32  = Symbian OS Serial Comms server
CSY  = Symbian OS C32 plugin module.  IrCOMM.CSY for IR, BTCOMM.CSY for Bluetooth

Licence
-------
Gnubox is free software and is released under the GNU General Public Licence. 
Full source and binaries are freely available. See "COPYING.txt" for more details.

Contact
-------
Gnubox was written by Mal Minhas. If you have any comments or queries, you can 
contact me at mal@malm.co.uk
