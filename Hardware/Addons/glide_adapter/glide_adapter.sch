EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Glidepoint/Cirque to FLipMouse Adapter"
Date "2021-12-09"
Rev "v0.1"
Comp "AsTeRICS Foundation"
Comment1 "(c) Benjamin Aigner, 2021"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x12_Female J2
U 1 1 61B1E70F
P 2100 3150
F 0 "J2" H 1900 3800 50  0000 L CNN
F 1 "Conn_01x12_Female" V 2150 2750 50  0000 L CNN
F 2 "Connector_FFC-FPC:Hirose_FH12-12S-0.5SH_1x12-1MP_P0.50mm_Horizontal" H 2100 3150 50  0001 C CNN
F 3 "~" H 2100 3150 50  0001 C CNN
F 4 "798-FH12-12S-0.5SH55" H 2100 3150 50  0001 C CNN "Mouser"
	1    2100 3150
	1    0    0    -1  
$EndComp
Text Notes 750  2350 0    50   ~ 0
original: XF2M-1215-1A (no footprint)\nhere: FH12-12S-0.5SH\npinning according to: DK-000013-03 devkit user manual
$Comp
L Connector:Conn_01x05_Female J1
U 1 1 61B1F13D
P 4350 1200
F 0 "J1" H 4378 1226 50  0000 L CNN
F 1 "Conn_01x05_Female" H 4000 1550 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 4350 1200 50  0001 C CNN
F 3 "~" H 4350 1200 50  0001 C CNN
F 4 "DON'T ORDER (437-8018700510001101)" H 4350 1200 50  0001 C CNN "Mouser"
	1    4350 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61B1FD4E
P 1150 1300
F 0 "R1" H 1220 1346 50  0000 L CNN
F 1 "4k7" H 1220 1255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1080 1300 50  0001 C CNN
F 3 "~" H 1150 1300 50  0001 C CNN
F 4 "603-AC0805FR-7W4K7" H 1150 1300 50  0001 C CNN "Mouser"
	1    1150 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR03
U 1 1 61B217C9
P 1900 2650
F 0 "#PWR03" H 1900 2500 50  0001 C CNN
F 1 "+3V3" V 1915 2778 50  0000 L CNN
F 2 "" H 1900 2650 50  0001 C CNN
F 3 "" H 1900 2650 50  0001 C CNN
	1    1900 2650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 61B22384
P 1900 2750
F 0 "#PWR04" H 1900 2500 50  0001 C CNN
F 1 "GND" V 1905 2622 50  0000 R CNN
F 2 "" H 1900 2750 50  0001 C CNN
F 3 "" H 1900 2750 50  0001 C CNN
	1    1900 2750
	0    1    1    0   
$EndComp
Text Label 1900 2850 2    50   ~ 0
SDA
Text Label 1900 2950 2    50   ~ 0
SCL
NoConn ~ 1900 3050
NoConn ~ 1900 3150
NoConn ~ 1900 3250
Text Notes 1800 3050 3    25   ~ 0
btn 1-3
Text Label 1900 3350 2    50   ~ 0
MOSI
Text Label 1900 3450 2    50   ~ 0
DRDY
Text Label 1900 3550 2    50   ~ 0
CS
Text Label 1900 3650 2    50   ~ 0
MISO
Text Label 1900 3750 2    50   ~ 0
SCK
$Comp
L Device:R R2
U 1 1 61B24538
P 1500 1300
F 0 "R2" H 1570 1346 50  0000 L CNN
F 1 "4k7" H 1570 1255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1430 1300 50  0001 C CNN
F 3 "~" H 1500 1300 50  0001 C CNN
F 4 "603-AC0805FR-7W4K7L" H 1500 1300 50  0001 C CNN "Mouser"
	1    1500 1300
	1    0    0    -1  
$EndComp
Text Label 1150 1450 3    50   ~ 0
SDA
Text Label 1500 1450 3    50   ~ 0
SCL
$Comp
L power:+3V3 #PWR01
U 1 1 61B248CF
P 1150 1150
F 0 "#PWR01" H 1150 1000 50  0001 C CNN
F 1 "+3V3" H 1165 1323 50  0000 C CNN
F 2 "" H 1150 1150 50  0001 C CNN
F 3 "" H 1150 1150 50  0001 C CNN
	1    1150 1150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR02
U 1 1 61B24BF2
P 1500 1150
F 0 "#PWR02" H 1500 1000 50  0001 C CNN
F 1 "+3V3" H 1515 1323 50  0000 C CNN
F 2 "" H 1500 1150 50  0001 C CNN
F 3 "" H 1500 1150 50  0001 C CNN
	1    1500 1150
	1    0    0    -1  
$EndComp
Wire Notes Line
	700  650  2000 650 
Wire Notes Line
	2000 650  2000 1850
Wire Notes Line
	2000 1850 700  1850
Wire Notes Line
	700  1850 700  650 
Text Notes 750  750  0    50   ~ 10
I2C pullups
Wire Notes Line
	3600 650  4900 650 
Wire Notes Line
	3600 1850 3600 650 
Text Notes 3650 750  0    50   ~ 10
FM connector - I2C (front)
Wire Notes Line
	4900 650  4900 1850
Wire Notes Line
	3600 1850 4900 1850
Wire Notes Line
	700  2000 3450 2000
Wire Notes Line
	3450 2000 3450 3950
Wire Notes Line
	3450 3950 700  3950
Wire Notes Line
	700  3950 700  2000
Text Notes 750  2100 0    50   ~ 10
FPC connector to Glide Touchpad
$Comp
L power:+3V3 #PWR05
U 1 1 61B36144
P 4150 1200
F 0 "#PWR05" H 4150 1050 50  0001 C CNN
F 1 "+3V3" V 4165 1328 50  0000 L CNN
F 2 "" H 4150 1200 50  0001 C CNN
F 3 "" H 4150 1200 50  0001 C CNN
	1    4150 1200
	0    -1   -1   0   
$EndComp
Text Label 4150 1400 2    50   ~ 0
SDA
Text Label 4150 1100 2    50   ~ 0
SCL
$Comp
L power:GND #PWR0101
U 1 1 61B36A18
P 4150 1300
F 0 "#PWR0101" H 4150 1050 50  0001 C CNN
F 1 "GND" V 4155 1172 50  0000 R CNN
F 2 "" H 4150 1300 50  0001 C CNN
F 3 "" H 4150 1300 50  0001 C CNN
	1    4150 1300
	0    1    1    0   
$EndComp
Text Label 4150 1000 2    50   ~ 0
DRDY
Text Notes 3700 1800 0    50   ~ 0
pin 4 (Teensy D21) is used\nas GND, because it has 20mA\ncurrent capability
$Comp
L Device:C C1
U 1 1 61B4FED6
P 3100 3200
F 0 "C1" H 3215 3246 50  0000 L CNN
F 1 "1u" H 3215 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 3138 3050 50  0001 C CNN
F 3 "~" H 3100 3200 50  0001 C CNN
F 4 "710-885012207051" H 3100 3200 50  0001 C CNN "Mouser"
	1    3100 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR06
U 1 1 61B50761
P 3100 3050
F 0 "#PWR06" H 3100 2900 50  0001 C CNN
F 1 "+3V3" H 3115 3223 50  0000 C CNN
F 2 "" H 3100 3050 50  0001 C CNN
F 3 "" H 3100 3050 50  0001 C CNN
	1    3100 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 61B50ADD
P 3100 3350
F 0 "#PWR07" H 3100 3100 50  0001 C CNN
F 1 "GND" H 3105 3177 50  0000 C CNN
F 2 "" H 3100 3350 50  0001 C CNN
F 3 "" H 3100 3350 50  0001 C CNN
	1    3100 3350
	1    0    0    -1  
$EndComp
Text Notes 6000 3050 0    50   ~ 10
Glidepoint/Cirque Trackpad to FLipMouse Adapter
Text Notes 6000 3900 0    50   ~ 0
compatible devices:\nTM023023\nTM035035\nTM040040\n(only available @Mouser)\n\nuse case:\nattach this adapter instead\nof the sensor PCB to the FLipMouse PCB,\nand use the different case from this addon folder
Wire Notes Line
	5950 2900 7950 2900
Wire Notes Line
	7950 2900 7950 4150
Wire Notes Line
	7950 4150 5950 4150
Wire Notes Line
	5950 4150 5950 2900
Text Notes 8150 3050 0    50   ~ 10
Notes/Hints
Wire Notes Line
	8100 2900 10050 2900
Wire Notes Line
	10050 2900 10050 4350
Wire Notes Line
	10050 4350 8100 4350
Wire Notes Line
	8100 4350 8100 2900
Text Notes 8150 3900 0    50   ~ 0
* different FM firmware is necessary\n* attached instead of FSR sensors\n* Teensy D21 pin must be: OUTPUT & LOW, \notherwise no GND here.\n* I2C1 (Wire1) is used for this device\n* data ready pin is Teensy D20\n\n* Remove the resistor on the Glide pad\ncorrespondingly! (R1 missing -> I2C;\nR1 = 470k -> SPI)
$Comp
L Connector:Conn_01x04_Female J3
U 1 1 61B70E3E
P 2900 1450
F 0 "J3" H 2792 1025 50  0000 C CNN
F 1 "Conn_01x04_Female" H 2792 1116 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Horizontal" H 2900 1450 50  0001 C CNN
F 3 "~" H 2900 1450 50  0001 C CNN
	1    2900 1450
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 61B71A08
P 2700 1550
F 0 "#PWR09" H 2700 1300 50  0001 C CNN
F 1 "GND" H 2705 1377 50  0000 C CNN
F 2 "" H 2700 1550 50  0001 C CNN
F 3 "" H 2700 1550 50  0001 C CNN
	1    2700 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR08
U 1 1 61B71CDF
P 2700 1450
F 0 "#PWR08" H 2700 1300 50  0001 C CNN
F 1 "+3V3" H 2715 1623 50  0000 C CNN
F 2 "" H 2700 1450 50  0001 C CNN
F 3 "" H 2700 1450 50  0001 C CNN
	1    2700 1450
	0    -1   -1   0   
$EndComp
Text Label 2700 1250 2    50   ~ 0
SDA
Text Label 2700 1350 2    50   ~ 0
SCL
Text Notes 2200 800  0    50   ~ 10
I2C display (SSD1306)
Wire Notes Line
	2150 1850 2150 650 
Wire Notes Line
	3450 650  3450 1850
Wire Notes Line
	2150 1850 3450 1850
Wire Notes Line
	2150 650  3450 650 
Text Label 1050 3300 2    50   ~ 0
MOSI
Text Label 1050 3400 2    50   ~ 0
CS
Text Label 1050 3500 2    50   ~ 0
MISO
Text Label 1050 3600 2    50   ~ 0
SCK
NoConn ~ 1050 3600
NoConn ~ 1050 3500
NoConn ~ 1050 3400
NoConn ~ 1050 3300
Text Notes 850  3850 1    50   ~ 0
SPI interface is unused.
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 61B90D39
P 3100 3050
F 0 "#FLG0101" H 3100 3125 50  0001 C CNN
F 1 "PWR_FLAG" V 3100 3177 50  0000 L CNN
F 2 "" H 3100 3050 50  0001 C CNN
F 3 "~" H 3100 3050 50  0001 C CNN
	1    3100 3050
	0    -1   -1   0   
$EndComp
Connection ~ 3100 3050
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 61B91308
P 3100 3350
F 0 "#FLG0102" H 3100 3425 50  0001 C CNN
F 1 "PWR_FLAG" V 3100 3477 50  0000 L CNN
F 2 "" H 3100 3350 50  0001 C CNN
F 3 "~" H 3100 3350 50  0001 C CNN
	1    3100 3350
	0    -1   -1   0   
$EndComp
Connection ~ 3100 3350
$EndSCHEMATC
