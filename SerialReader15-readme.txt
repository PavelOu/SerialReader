SerialPlotter - readme
== = = = = = = = = = =

Actual version 15
= = = = = = = = =
Date of last finalising : 2026-10-26

Preface :
=========
This readme is for program SerialReadme - version 15 .

I have created this program for serial devices what can communicate 
via USB-RS232 like Arduino etc. and with my programs like SerialMeasurer or SerialPlotter 
created in Delphi XE Starter and worked under operation system Windows.

These programs were created  by Pavel Oupicky, rentier and home worker
and they were tested only on OS W11 64 bits.
These programs are distributed by author as freeware for end users .   
Author takes no liability towards of any use of his program.

At 25.10. 2026 					Pavel Oupicky

Protocol Meas
= = = = = = =

As my previous program SerialMeasurer the program SeriaReader can use this protocol,
what was created for Arduino Uno or another microcomputers (Mcu)
and what can communicate via serial protocols with PC .
Program from PC calls via RS232 (USB) with commands 
and one expects prepared answer.

Instead of SerialReader can read data from discs in another format 
and it can collect data wit protocols rf (rfbme) or rtd, simalar to my previous
protocol rtu of Modbus system. 

Used commands and answers in protocol Meas for program SerialReader
included in sketch Arduino_measurer_URC_en_3.ino
= = = = = = = = = = = = = = = = = = = = = = = = =
Note: CRLF are end marks of RS232 protocol, 
      Cr is carriage return, LF is Line Feed
Command Answere
- - - - - - - - 
A	*A,400,#,CRLF   number is readed analog data example
B	*B,399,342,346,420,#,CRLF	
C	*C,399,342,346,420,399,342,346,401,#,CRLF 
E20	*P,20,400,500,100,#,CRLF = returned all parameters	
	   20, data time step in mikroseconds, next parameters are not used

Example of using this protocol in .par file for program SerialReader15
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SerialReader/15/params
Measuring Voltage from inputs A0 to A7
Test of system Arduino and Measure module
Date:2026-08-19*15:22
Device:Arduino-Measurer A0 to A7
Protocol:MEAS/Measure data protocol
Port:Com,3,9600,8,1,0
Timing:Timer=10,Sleep=10,Wait=200,Timeout=100
Inputs:8
1:Meas/2,0,9.999,0.01/[V],U(A1),Voltage U in Volts
2:Meas/2,0,9.999,0.01/[V],U(A2),Voltage U in Volts
3:Meas/2,0,9.999,0.01/[V],U(A3),Voltage U in Volts
4:Meas/2,0,9.999,1.0/[V],U(A4),Voltage U in Volts
5:Meas/2,0,9.999,1.0/[V],U(A5),Voltage U in Volts
6:Meas/2,0,9999.9,1.0/[V],U(A6),Voltage U in Volts
7:Meas/2,0,9999.9,1.0/[V],U(A7),Voltage U in Volts
8:Meas/2,0,9.999,0.01/[V],U(A0),Voltage U in Volts
Tasks:3
Mer-AV,A,8,end
Mer-A0-A3,B,8,1,2,3,end             // Name of task,Command,inputs,end
Mer-A0-A7,C,8,1,2,3,4,5,6,7,end
Measured interval:0
Data:Data
Help:Help\DataSerial10-help.txt
Graf-par:Grp\Grf-SerialReader-basic-en.grp
end
- - - - - - - - - - - - - - - - - - - - - -


List of all used commands (example for SerialMeasured program):
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
Command	Answer
- - - - - - - -
A	*A,400,#,CRLF   number is readed analog data example
B	*B,399,342,346,420,#,CRLF	Cr is carriage return, LF is Line Feed
C	*C,399,342,346,420,399,342,346,401,#,CRLF - CRLF are marks from RS232 protocol
D	measuring in cycle - neither used nor realised
E20	*P,20,400,500,100,#,CRLF = returned all parameters	
	   20, data time step in mikroseconds
	      400, off-line readed data with osciloscope here not used
	          500, max - " -
	              100, offset for readed data (usually zero) hjere not used
F	read all transfer detail comments from Arduino (for tests with ESP)
G	set of TTL 0 (zero) of digital output D6
H	set of TTL 1 (zero) of digital output D6
J	*J,analogInput,dataInverted,#,CRLF = info about in devices setted parameters
I	*I,nr,#,CRLF = test of aktual connected slave device nr in ESP-NOW system
P	*P,20,300,500,10,#,CRLF = read parameters
Q	stop of reading details after teste of commands in  ESP-NOW system 
R	*R,20,300,time,10,#,CRLF = time is the previous data measuring time in microseconds
S	*S,20,300,time,10,#,CRLF = stop of reading B,C,D data in cycle here not used
V0	*V,0,#,CRLF - set ou analog input A0 of command A
V1	*V,1,#,CRLF - set ou analog input A1 - '' - etc.


= = = * * * end of readme * * * = = = 


