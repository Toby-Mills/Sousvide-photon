EESchema Schematic File Version 2
LIBS:sousvide-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:sousvide
LIBS:sousvide-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 56F10C3F
P 4850 3900
F 0 "#PWR01" H 4850 3650 50  0001 C CNN
F 1 "GND" H 4850 3750 50  0000 C CNN
F 2 "" H 4850 3900 50  0000 C CNN
F 3 "" H 4850 3900 50  0000 C CNN
	1    4850 3900
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR02
U 1 1 56F10C71
P 4750 4000
F 0 "#PWR02" H 4750 3850 50  0001 C CNN
F 1 "VCC" H 4750 4150 50  0000 C CNN
F 2 "" H 4750 4000 50  0000 C CNN
F 3 "" H 4750 4000 50  0000 C CNN
	1    4750 4000
	0    -1   -1   0   
$EndComp
Text Label 3950 4350 0    60   ~ 0
SDA
Text Label 3950 4250 0    60   ~ 0
SDL
Text Label 4900 4100 2    60   ~ 0
SDA
Text Label 4900 4200 2    60   ~ 0
SDL
$Comp
L LED-RESCUE-sousvide D2
U 1 1 56F1127F
P 5350 3400
F 0 "D2" H 5350 3500 50  0000 C CNN
F 1 "RELAY_LED" H 5350 3300 50  0000 C CNN
F 2 "LEDs:LED-5MM" H 5350 3400 50  0001 C CNN
F 3 "" H 5350 3400 50  0000 C CNN
	1    5350 3400
	-1   0    0    1   
$EndComp
$Comp
L LED-RESCUE-sousvide D1
U 1 1 56F11552
P 5350 3100
F 0 "D1" H 5350 3200 50  0000 C CNN
F 1 "POW_LED" H 5350 3000 50  0000 C CNN
F 2 "LEDs:LED-5MM" H 5350 3100 50  0001 C CNN
F 3 "" H 5350 3100 50  0000 C CNN
	1    5350 3100
	-1   0    0    1   
$EndComp
$Comp
L relay U4
U 1 1 56F1163E
P 5500 2450
F 0 "U4" H 5500 2250 60  0000 C CNN
F 1 "relay" H 5500 2750 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 5500 2450 60  0001 C CNN
F 3 "" H 5500 2450 60  0000 C CNN
	1    5500 2450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 56F117DD
P 5150 2250
F 0 "#PWR03" H 5150 2000 50  0001 C CNN
F 1 "GND" H 5150 2100 50  0000 C CNN
F 2 "" H 5150 2250 50  0000 C CNN
F 3 "" H 5150 2250 50  0000 C CNN
	1    5150 2250
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR04
U 1 1 56F117FD
P 5150 2550
F 0 "#PWR04" H 5150 2400 50  0001 C CNN
F 1 "VCC" H 5150 2700 50  0000 C CNN
F 2 "" H 5150 2550 50  0000 C CNN
F 3 "" H 5150 2550 50  0000 C CNN
	1    5150 2550
	-1   0    0    1   
$EndComp
NoConn ~ 5150 2450
Text Label 5050 2350 2    60   ~ 0
RELAY
Text Label 3950 3950 0    60   ~ 0
RELAY
$Comp
L DS18B20 U3
U 1 1 56F12551
P 5350 4950
F 0 "U3" H 5350 4850 60  0000 C CNN
F 1 "DS18B20" H 5350 5250 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 5400 4950 60  0001 C CNN
F 3 "" H 5400 4950 60  0000 C CNN
	1    5350 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 56F12592
P 5050 4750
F 0 "#PWR05" H 5050 4500 50  0001 C CNN
F 1 "GND" H 5050 4600 50  0000 C CNN
F 2 "" H 5050 4750 50  0000 C CNN
F 3 "" H 5050 4750 50  0000 C CNN
	1    5050 4750
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR06
U 1 1 56F125E8
P 5050 4950
F 0 "#PWR06" H 5050 4800 50  0001 C CNN
F 1 "VCC" H 5050 5100 50  0000 C CNN
F 2 "" H 5050 4950 50  0000 C CNN
F 3 "" H 5050 4950 50  0000 C CNN
	1    5050 4950
	-1   0    0    1   
$EndComp
Text Label 4950 4850 2    60   ~ 0
TEMP
Text Label 2100 3850 2    60   ~ 0
TEMP
Text Label 5000 3100 2    60   ~ 0
POW_LED
Text Label 5000 3400 2    60   ~ 0
REL_LED
Text Label 3950 3750 0    60   ~ 0
REL_LED
Text Label 3950 3650 0    60   ~ 0
POW_LED
$Comp
L GND #PWR07
U 1 1 56F12C61
P 5550 3400
F 0 "#PWR07" H 5550 3150 50  0001 C CNN
F 1 "GND" H 5550 3250 50  0000 C CNN
F 2 "" H 5550 3400 50  0000 C CNN
F 3 "" H 5550 3400 50  0000 C CNN
	1    5550 3400
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR08
U 1 1 56F12C83
P 5550 3100
F 0 "#PWR08" H 5550 2850 50  0001 C CNN
F 1 "GND" H 5550 2950 50  0000 C CNN
F 2 "" H 5550 3100 50  0000 C CNN
F 3 "" H 5550 3100 50  0000 C CNN
	1    5550 3100
	0    -1   -1   0   
$EndComp
NoConn ~ 2250 3350
NoConn ~ 2250 3450
NoConn ~ 2250 3550
NoConn ~ 2250 3750
NoConn ~ 2250 3950
NoConn ~ 2250 4050
NoConn ~ 3850 3150
NoConn ~ 3850 3050
$Comp
L VCC #PWR09
U 1 1 56F11B91
P 1900 5800
F 0 "#PWR09" H 1900 5650 50  0001 C CNN
F 1 "VCC" H 1900 5950 50  0000 C CNN
F 2 "" H 1900 5800 50  0000 C CNN
F 3 "" H 1900 5800 50  0000 C CNN
	1    1900 5800
	0    -1   -1   0   
$EndComp
Text Label 2500 6100 2    60   ~ 0
Button
Text Label 2550 5900 2    60   ~ 0
ROT_CH1
Text Label 2550 6000 2    60   ~ 0
ROT_CH2
$Comp
L GND #PWR010
U 1 1 56F11C72
P 2150 5700
F 0 "#PWR010" H 2150 5450 50  0001 C CNN
F 1 "GND" H 2150 5550 50  0000 C CNN
F 2 "" H 2150 5700 50  0000 C CNN
F 3 "" H 2150 5700 50  0000 C CNN
	1    2150 5700
	0    1    1    0   
$EndComp
Text Label 3950 4050 0    60   ~ 0
ROT_CH1
Text Label 3950 4150 0    60   ~ 0
ROT_CH2
Text Label 2150 4350 2    60   ~ 0
Button
NoConn ~ 2250 4250
NoConn ~ 2250 4150
$Comp
L PHOTON U1
U 1 1 56F0FE3F
P 3050 3750
F 0 "U1" H 3050 3150 60  0000 C CNN
F 1 "PHOTON" H 3050 4450 60  0000 C CNN
F 2 "particle:photon" H 3050 3850 60  0001 C CNN
F 3 "" H 3050 3850 60  0000 C CNN
	1    3050 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 56F11EF9
P 2250 3150
F 0 "#PWR011" H 2250 2900 50  0001 C CNN
F 1 "GND" H 2250 3000 50  0000 C CNN
F 2 "" H 2250 3150 50  0000 C CNN
F 3 "" H 2250 3150 50  0000 C CNN
	1    2250 3150
	0    1    1    0   
$EndComp
$Comp
L PWR_FLAG #FLG012
U 1 1 56F12CE9
P 1100 2200
F 0 "#FLG012" H 1100 2295 50  0001 C CNN
F 1 "PWR_FLAG" H 1100 2380 50  0000 C CNN
F 2 "" H 1100 2200 50  0000 C CNN
F 3 "" H 1100 2200 50  0000 C CNN
	1    1100 2200
	-1   0    0    1   
$EndComp
$Comp
L VSS #PWR013
U 1 1 56F13049
P 1950 3250
F 0 "#PWR013" H 1950 3100 50  0001 C CNN
F 1 "VSS" H 1950 3400 50  0000 C CNN
F 2 "" H 1950 3250 50  0000 C CNN
F 3 "" H 1950 3250 50  0000 C CNN
	1    1950 3250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 5723B0F8
P 1100 2200
F 0 "#PWR014" H 1100 2050 50  0001 C CNN
F 1 "VCC" H 1100 2350 50  0000 C CNN
F 2 "" H 1100 2200 50  0000 C CNN
F 3 "" H 1100 2200 50  0000 C CNN
	1    1100 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 5723B1FC
P 1500 2200
F 0 "#PWR015" H 1500 1950 50  0001 C CNN
F 1 "GND" H 1500 2050 50  0000 C CNN
F 2 "" H 1500 2200 50  0000 C CNN
F 3 "" H 1500 2200 50  0000 C CNN
	1    1500 2200
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG016
U 1 1 5723B21E
P 1500 2200
F 0 "#FLG016" H 1500 2295 50  0001 C CNN
F 1 "PWR_FLAG" H 1500 2380 50  0000 C CNN
F 2 "" H 1500 2200 50  0000 C CNN
F 3 "" H 1500 2200 50  0000 C CNN
	1    1500 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	4850 3900 5000 3900
Wire Wire Line
	3850 4250 3950 4250
Wire Wire Line
	3850 4350 3950 4350
Wire Wire Line
	4900 4100 5000 4100
Wire Wire Line
	4900 4200 5000 4200
Wire Wire Line
	4750 4000 5000 4000
Wire Wire Line
	5050 2350 5150 2350
Wire Wire Line
	3850 3950 3950 3950
Wire Wire Line
	4950 4850 5050 4850
Wire Wire Line
	5000 3100 5150 3100
Wire Wire Line
	5000 3400 5150 3400
Wire Wire Line
	3950 3750 3850 3750
Wire Wire Line
	3950 3650 3850 3650
Wire Wire Line
	3850 4050 3950 4050
Wire Wire Line
	3850 4150 3950 4150
Wire Wire Line
	2150 4350 2250 4350
Wire Wire Line
	2250 3250 1950 3250
Wire Wire Line
	2550 6000 2700 6000
Wire Wire Line
	2550 5900 2700 5900
Wire Wire Line
	2150 5700 2700 5700
$Comp
L rot_enc_2 U5
U 1 1 5723D442
P 3050 5850
F 0 "U5" H 3050 5500 60  0000 C CNN
F 1 "rot_enc_2" H 3050 6100 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05" H 3050 5850 60  0001 C CNN
F 3 "" H 3050 5850 60  0000 C CNN
	1    3050 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 6100 2500 6100
Wire Wire Line
	1900 5800 2700 5800
$Comp
L LCM1602_IIC_v1 U2
U 1 1 5723DDC4
P 5350 4050
F 0 "U2" H 5350 3700 60  0000 C CNN
F 1 "LCM1602_IIC_v1" H 5350 4400 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 5600 3800 60  0001 C CNN
F 3 "" H 5600 3800 60  0000 C CNN
	1    5350 4050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR017
U 1 1 57250331
P 2250 3050
F 0 "#PWR017" H 2250 2900 50  0001 C CNN
F 1 "VCC" H 2250 3200 50  0000 C CNN
F 2 "" H 2250 3050 50  0000 C CNN
F 3 "" H 2250 3050 50  0000 C CNN
	1    2250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3850 2250 3850
NoConn ~ 3850 3850
$EndSCHEMATC
