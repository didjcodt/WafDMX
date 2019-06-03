EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "WafDMX - DMX controller"
Date "2019-06-03"
Rev "v1.0.0"
Comp "Kaizen tech"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:ESP32-WROOM-32D U?
U 1 1 5CF5AD65
P 2900 5800
F 0 "U?" H 2500 7200 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 3400 7200 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 2900 4300 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 2600 5850 50  0001 C CNN
	1    2900 5800
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 5CF5D66E
P 2100 1500
F 0 "U?" H 2100 1742 50  0000 C CNN
F 1 "AMS1117-3.3" H 2100 1651 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 2100 1700 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 2200 1250 50  0001 C CNN
	1    2100 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1800 2100 1900
$Comp
L power:GND #PWR?
U 1 1 5CF65F11
P 2100 1900
F 0 "#PWR?" H 2100 1650 50  0001 C CNN
F 1 "GND" H 2105 1727 50  0000 C CNN
F 2 "" H 2100 1900 50  0001 C CNN
F 3 "" H 2100 1900 50  0001 C CNN
	1    2100 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1800 2500 1900
$Comp
L power:GND #PWR?
U 1 1 5CF6792B
P 2500 1900
F 0 "#PWR?" H 2500 1650 50  0001 C CNN
F 1 "GND" H 2505 1727 50  0000 C CNN
F 2 "" H 2500 1900 50  0001 C CNN
F 3 "" H 2500 1900 50  0001 C CNN
	1    2500 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5CF6C8C7
P 2500 1700
F 0 "C?" H 2592 1746 50  0000 L CNN
F 1 "10 uF" H 2592 1655 50  0000 L CNN
F 2 "" H 2500 1700 50  0001 C CNN
F 3 "~" H 2500 1700 50  0001 C CNN
	1    2500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 1500 2500 1500
Wire Wire Line
	2500 1500 2500 1600
Wire Wire Line
	1200 1500 1200 1300
Wire Wire Line
	3000 1300 3000 1500
$Comp
L power:VCC #PWR?
U 1 1 5CF760E0
P 1200 1300
F 0 "#PWR?" H 1200 1150 50  0001 C CNN
F 1 "VCC" H 1217 1473 50  0000 C CNN
F 2 "" H 1200 1300 50  0001 C CNN
F 3 "" H 1200 1300 50  0001 C CNN
	1    1200 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5CF76827
P 3000 1300
F 0 "#PWR?" H 3000 1150 50  0001 C CNN
F 1 "+3V3" H 3015 1473 50  0000 C CNN
F 2 "" H 3000 1300 50  0001 C CNN
F 3 "" H 3000 1300 50  0001 C CNN
	1    3000 1300
	1    0    0    -1  
$EndComp
Connection ~ 1500 1500
Wire Wire Line
	1800 1500 1500 1500
Wire Wire Line
	1500 1500 1200 1500
$Comp
L power:GND #PWR?
U 1 1 5CF6503D
P 1500 1900
F 0 "#PWR?" H 1500 1650 50  0001 C CNN
F 1 "GND" H 1505 1727 50  0000 C CNN
F 2 "" H 1500 1900 50  0001 C CNN
F 3 "" H 1500 1900 50  0001 C CNN
	1    1500 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1800 1500 1900
Wire Wire Line
	1500 1500 1500 1600
$Comp
L Device:C_Small C?
U 1 1 5CF62083
P 1500 1700
F 0 "C?" H 1592 1746 50  0000 L CNN
F 1 "1 uF" H 1592 1655 50  0000 L CNN
F 2 "" H 1500 1700 50  0001 C CNN
F 3 "~" H 1500 1700 50  0001 C CNN
	1    1500 1700
	1    0    0    -1  
$EndComp
Connection ~ 2500 1500
Wire Wire Line
	2500 1500 2900 1500
Wire Wire Line
	2900 1800 2900 1900
$Comp
L power:GND #PWR?
U 1 1 5CF89A24
P 2900 1900
F 0 "#PWR?" H 2900 1650 50  0001 C CNN
F 1 "GND" H 2905 1727 50  0000 C CNN
F 2 "" H 2900 1900 50  0001 C CNN
F 3 "" H 2900 1900 50  0001 C CNN
	1    2900 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5CF89A2E
P 2900 1700
F 0 "C?" H 2992 1746 50  0000 L CNN
F 1 "1 uF" H 2992 1655 50  0000 L CNN
F 2 "" H 2900 1700 50  0001 C CNN
F 3 "~" H 2900 1700 50  0001 C CNN
	1    2900 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 1500 2900 1600
Connection ~ 2900 1500
Wire Wire Line
	2900 1500 3000 1500
$Comp
L power:+3V3 #PWR?
U 1 1 5CF96878
P 2900 3500
F 0 "#PWR?" H 2900 3350 50  0001 C CNN
F 1 "+3V3" H 2915 3673 50  0000 C CNN
F 2 "" H 2900 3500 50  0001 C CNN
F 3 "" H 2900 3500 50  0001 C CNN
	1    2900 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5CF98C2E
P 2500 4000
F 0 "#PWR?" H 2500 3750 50  0001 C CNN
F 1 "GND" H 2505 3827 50  0000 C CNN
F 2 "" H 2500 4000 50  0001 C CNN
F 3 "" H 2500 4000 50  0001 C CNN
	1    2500 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3900 2500 4000
$Comp
L Device:C_Small C?
U 1 1 5CF98C39
P 2500 3800
F 0 "C?" H 2592 3846 50  0000 L CNN
F 1 "0.1 uF" H 2592 3755 50  0000 L CNN
F 2 "" H 2500 3800 50  0001 C CNN
F 3 "~" H 2500 3800 50  0001 C CNN
	1    2500 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5CFAAC70
P 3300 4000
F 0 "#PWR?" H 3300 3750 50  0001 C CNN
F 1 "GND" H 3305 3827 50  0000 C CNN
F 2 "" H 3300 4000 50  0001 C CNN
F 3 "" H 3300 4000 50  0001 C CNN
	1    3300 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3900 3300 4000
$Comp
L Device:C_Small C?
U 1 1 5CFAAC7B
P 3300 3800
F 0 "C?" H 3392 3846 50  0000 L CNN
F 1 "0.1 uF" H 3392 3755 50  0000 L CNN
F 2 "" H 3300 3800 50  0001 C CNN
F 3 "~" H 3300 3800 50  0001 C CNN
	1    3300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3500 2900 3600
Wire Wire Line
	2900 3600 2500 3600
Wire Wire Line
	2500 3600 2500 3700
Wire Wire Line
	3300 3700 3300 3600
Wire Wire Line
	3300 3600 2900 3600
Connection ~ 2900 3600
Wire Wire Line
	2900 3600 2900 4400
Wire Wire Line
	2300 4600 1800 4600
$Comp
L Device:C_Small C?
U 1 1 5CFB0FC2
P 1800 4800
F 0 "C?" H 1892 4846 50  0000 L CNN
F 1 "0.1 uF" H 1892 4755 50  0000 L CNN
F 2 "" H 1800 4800 50  0001 C CNN
F 3 "~" H 1800 4800 50  0001 C CNN
	1    1800 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4600 1800 4700
Wire Wire Line
	1800 4900 1800 5000
$Comp
L power:GND #PWR?
U 1 1 5CFB8FD2
P 1800 5000
F 0 "#PWR?" H 1800 4750 50  0001 C CNN
F 1 "GND" H 1805 4827 50  0000 C CNN
F 2 "" H 1800 5000 50  0001 C CNN
F 3 "" H 1800 5000 50  0001 C CNN
	1    1800 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 5CFBD309
P 1800 4400
F 0 "R?" H 1859 4446 50  0000 L CNN
F 1 "R_Small" H 1859 4355 50  0000 L CNN
F 2 "" H 1800 4400 50  0001 C CNN
F 3 "~" H 1800 4400 50  0001 C CNN
	1    1800 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4600 1800 4500
Connection ~ 1800 4600
Wire Wire Line
	1800 4300 1800 4200
$Comp
L power:+3V3 #PWR?
U 1 1 5CFC5417
P 1800 4200
F 0 "#PWR?" H 1800 4050 50  0001 C CNN
F 1 "+3V3" H 1815 4373 50  0000 C CNN
F 2 "" H 1800 4200 50  0001 C CNN
F 3 "" H 1800 4200 50  0001 C CNN
	1    1800 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector:XLR3 J?
U 1 1 5CFCAE1A
P 9500 5700
F 0 "J?" H 9500 6065 50  0000 C CNN
F 1 "XLR3" H 9500 5974 50  0000 C CNN
F 2 "" H 9500 5700 50  0001 C CNN
F 3 " ~" H 9500 5700 50  0001 C CNN
	1    9500 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector:XLR3 J?
U 1 1 5CFCCF08
P 10500 5700
F 0 "J?" H 10500 6065 50  0000 C CNN
F 1 "XLR3" H 10500 5974 50  0000 C CNN
F 2 "" H 10500 5700 50  0001 C CNN
F 3 " ~" H 10500 5700 50  0001 C CNN
	1    10500 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 5600 7200 5600
Text Label 7200 5600 0    50   ~ 0
DMX_MAX_OUT
Wire Wire Line
	7800 5700 7200 5700
Text Label 7200 5700 0    50   ~ 0
DMX_RE
Wire Wire Line
	7800 5800 7200 5800
Text Label 7200 5800 0    50   ~ 0
DMX_DE
Wire Wire Line
	7800 5900 7200 5900
Text Label 7200 5900 0    50   ~ 0
DMX_MAX_IN
$Comp
L power:GND #PWR?
U 1 1 5CFF4493
P 9100 5900
F 0 "#PWR?" H 9100 5650 50  0001 C CNN
F 1 "GND" H 9105 5727 50  0000 C CNN
F 2 "" H 9100 5900 50  0001 C CNN
F 3 "" H 9100 5900 50  0001 C CNN
	1    9100 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 5700 9100 5700
Wire Wire Line
	9100 5700 9100 5900
$Comp
L power:GND #PWR?
U 1 1 5CFFD345
P 10100 5900
F 0 "#PWR?" H 10100 5650 50  0001 C CNN
F 1 "GND" H 10105 5727 50  0000 C CNN
F 2 "" H 10100 5900 50  0001 C CNN
F 3 "" H 10100 5900 50  0001 C CNN
	1    10100 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 5700 10100 5700
Wire Wire Line
	10100 5700 10100 5900
Wire Wire Line
	9500 6000 9500 6200
Wire Wire Line
	9500 6200 10500 6200
Wire Wire Line
	10500 6200 10500 6000
Wire Wire Line
	9800 5700 9900 5700
Wire Wire Line
	9900 5700 9900 5200
Wire Wire Line
	9900 5200 10900 5200
Wire Wire Line
	10900 5200 10900 5700
Wire Wire Line
	10900 5700 10800 5700
Wire Wire Line
	9500 6200 8800 6200
Wire Wire Line
	8800 6200 8800 5900
Wire Wire Line
	8800 5900 8600 5900
Connection ~ 9500 6200
Wire Wire Line
	8600 5600 8800 5600
Wire Wire Line
	8800 5600 8800 5200
Wire Wire Line
	8800 5200 9900 5200
Connection ~ 9900 5200
Wire Wire Line
	7800 4800 7800 4900
$Comp
L power:GND #PWR?
U 1 1 5D01DBFB
P 7800 4900
F 0 "#PWR?" H 7800 4650 50  0001 C CNN
F 1 "GND" H 7805 4727 50  0000 C CNN
F 2 "" H 7800 4900 50  0001 C CNN
F 3 "" H 7800 4900 50  0001 C CNN
	1    7800 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5D01DC05
P 7800 4700
F 0 "C?" H 7892 4746 50  0000 L CNN
F 1 "10 uF" H 7892 4655 50  0000 L CNN
F 2 "" H 7800 4700 50  0001 C CNN
F 3 "~" H 7800 4700 50  0001 C CNN
	1    7800 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 4800 8500 4900
$Comp
L power:GND #PWR?
U 1 1 5D01DC11
P 8500 4900
F 0 "#PWR?" H 8500 4650 50  0001 C CNN
F 1 "GND" H 8505 4727 50  0000 C CNN
F 2 "" H 8500 4900 50  0001 C CNN
F 3 "" H 8500 4900 50  0001 C CNN
	1    8500 4900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5D01DC1B
P 8500 4700
F 0 "C?" H 8592 4746 50  0000 L CNN
F 1 "1 uF" H 8592 4655 50  0000 L CNN
F 2 "" H 8500 4700 50  0001 C CNN
F 3 "~" H 8500 4700 50  0001 C CNN
	1    8500 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 5200 8200 4400
Wire Wire Line
	8200 4400 7800 4400
Wire Wire Line
	7800 4400 7800 4600
Wire Wire Line
	8200 4400 8500 4400
Wire Wire Line
	8500 4400 8500 4600
Connection ~ 8200 4400
$Comp
L Interface_UART:MAX3485 U?
U 1 1 5D03C498
P 8200 5700
F 0 "U?" H 8000 6200 50  0000 C CNN
F 1 "MAX3485" H 8400 6200 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 8200 5000 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX3483-MAX3491.pdf" H 8200 5750 50  0001 C CNN
	1    8200 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5D0450F3
P 8200 4200
F 0 "#PWR?" H 8200 4050 50  0001 C CNN
F 1 "+3V3" H 8215 4373 50  0000 C CNN
F 2 "" H 8200 4200 50  0001 C CNN
F 3 "" H 8200 4200 50  0001 C CNN
	1    8200 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 4200 8200 4400
Wire Wire Line
	3500 5700 4500 5700
Wire Wire Line
	3500 6100 4500 6100
Wire Wire Line
	3500 5900 4500 5900
Wire Wire Line
	3500 6000 4500 6000
Wire Wire Line
	3500 6400 4500 6400
Wire Wire Line
	3500 6300 4500 6300
Wire Wire Line
	3500 6500 4500 6500
Wire Wire Line
	3500 4600 4500 4600
Wire Wire Line
	3500 6200 4500 6200
Wire Wire Line
	3500 5800 4500 5800
Text Label 4500 5300 0    50   ~ 0
SPI_MOSI
Text Label 4500 5400 0    50   ~ 0
SPI_CLK
Text Label 4500 5200 0    50   ~ 0
SPI_MISO
Text Label 4500 5500 0    50   ~ 0
SPI_CS
Text Label 4500 5700 0    50   ~ 0
PHY_POWER
Text Label 4500 6100 0    50   ~ 0
EMAC_TXD1
Text Label 4500 5900 0    50   ~ 0
EMAC_TXD0
Text Label 4500 6000 0    50   ~ 0
EMAC_TX_EN
Text Label 4500 6400 0    50   ~ 0
EMAC_RXD1
Text Label 4500 6300 0    50   ~ 0
EMAC_RXD0
Text Label 4500 6500 0    50   ~ 0
EMAC_RX_DV
Text Label 4500 4600 0    50   ~ 0
EMAC_TX_CLK
Text Label 4500 6200 0    50   ~ 0
SMI_MDC
Text Label 4500 5800 0    50   ~ 0
SMI_MDIO
Wire Wire Line
	3500 5200 4500 5200
Wire Wire Line
	3500 5400 4500 5400
Wire Wire Line
	3500 5500 4500 5500
Wire Wire Line
	3500 5300 4500 5300
Text Notes 5000 5300 0    50   ~ 0
USE HSPI
$Comp
L Interface_Ethernet:LAN8720A U?
U 1 1 5D0A3359
P 7400 2400
F 0 "U?" H 7350 1211 50  0000 C CNN
F 1 "LAN8720A" H 7350 1120 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 7450 1350 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/8720a.pdf" H 7200 1450 50  0001 C CNN
	1    7400 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector:Wuerth_7499010121A J?
U 1 1 5D0A53C5
P 9500 2200
F 0 "J?" H 8971 2303 50  0000 R CNN
F 1 "Wuerth_7499010121A" H 8971 2212 50  0000 R CNN
F 2 "Connector_RJ:RJ45_Wuerth_7499151120_Horizontal" H 9500 1575 50  0001 C CNN
F 3 "http://katalog.we-online.de/pbs/datasheet/7499010121A.pdf" H 9085 1965 50  0001 L TNN
	1    9500 2200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8100 1900 8900 1900
Wire Wire Line
	8900 2100 8800 2100
Wire Wire Line
	8800 2100 8800 2000
Wire Wire Line
	8800 2000 8100 2000
Wire Wire Line
	8100 2100 8700 2100
Wire Wire Line
	8700 2100 8700 2300
Wire Wire Line
	8700 2300 8900 2300
Wire Wire Line
	8900 2500 8600 2500
Wire Wire Line
	8600 2500 8600 2200
Wire Wire Line
	8600 2200 8100 2200
$EndSCHEMATC