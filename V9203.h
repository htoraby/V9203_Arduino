/* V9203 Energy Monitor Header
	
	   The MIT License (MIT) 
	
	  Copyright (c) 2016 whatnick
	
	  Permission is hereby granted, free of unsigned charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
	
	  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	
	  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	*/
	#ifndef __V9203_H__
	#define __V9203_H__
	
	#ifndef  V9203EXT
	    #define V9203EXT extern
	#else
	    #define V9203EXT
	#endif
	
	
	
	#define Work_normal  0x11
	//=============================================================
	//Analog control register
	//=============================================================
	#define   RegANCtrl0   0x8000
	#define   RegANCtrl1   0x8001
	#define   RegANCtrl2   0x8002
	#define   RegANCtrl3   0x8003
	
	
	//=============================================================
	//Metering configuration register
	//=============================================================
	#define   RegMTPARA0   0xC000
	#define   RegMTPARA1   0xC001
	#define   RegMTPARA2   0xC002
	#define   RegMTPARA3   0xC003
	
	#define   ZZCPSEL     0xEC15
	#define   ZZPA0          0xEC23
	#define   ZZPA1          0xEC24
	#define   ZZQA0         0xEC47
	#define   ZZQA1         0xEC48
	#define   ZZAPPA       0xEC05
	#define   ZZPCF0A     0XEC34
	
	
	//=============================================================
	//DC component register  
	//=============================================================
	#define   RegDCAU       0xF02C
	#define   RegDCAI        0xF02D
	#define   RegDCBU       0xF02E
	#define   RegDCBI        0xF02F
	#define   RegDCCU       0xF030
	#define   RegDCCI        0xF031
	
	//=============================================================
	//Frequency phase register  
	//=============================================================
	
	#define   RegAFREQ      0xC008
	#define   RegBFREQ      0xC009
	#define   RegCFREQ      0xC00A
	
	#define   RegBUPHA      0xC00B
	#define   RegCUPHA      0xC00C
	#define   RegAIPHA       0xC00D
	#define   RegBIPHA       0xC00E
	#define   RegCIPHA       0xC00F
	
	
	//=============================================================
	//Voltage / current instantaneous rms register  
	//=============================================================
	#define   RegARTIA     0xE83E
	#define   RegARTIB    0xE83F
	#define   RegARTIC     0xE840
	
	#define   RegARTUA     0xE842
	#define   RegARTUB    0xE843
	#define   RegARTUC     0xE844
	
	
	//=============================================================
	//Active / reactive power register 
	//=============================================================
	#define   RegMAPSUM0      0xE8EE
	#define   RegMAPSUM1      0xE8EF
	
	#define   RegMAQSUM0      0xE8F3
	#define   RegMAQSUM1      0xE8F4
	
	
	
	
	//=============================================================
	//Voltage / current rms correction register  
	//=============================================================
	#define   RegWARTIA     0xE968
	#define   RegWARTIB     0xE969
	#define   RegWARTIC     0xE96A
	
	#define   RegWARTUA     0xE96C
	#define   RegWARTUB     0xE96D
	#define   RegWARTUC     0xE96E
	
	
	//=============================================================
	//Full wave active / reactive power correction register 
	//=============================================================
	#define   RegWAPTA0      0xE95A
	#define   RegWAPTB0      0xE95E
	#define   RegWAPTC0      0xE962
	
	#define   RegWAQTA      0xE965
	#define   RegWAQTB      0xE966
	#define   RegWAQTC      0xE967
	
	#define  RegWAEC0      0XE954       // Angle difference
	
	
	//=============================================================
	//Baseband active / reactive power correction register
	//=============================================================
	#define   RegWBPTA      0xE970   
	#define   RegWBPTB      0xE971
	#define   RegWBPTC      0xE972
	
	#define   RegWBQTA      0xE973
	#define   RegWBQTB      0xE974
	#define   RegWBQTC      0xE975
	
	
	//=============================================================
	//Full wave active / reactive power secondary compensation correction register
	//=============================================================
	#define   RegWWAPTA     0xE98E
	#define   RegWWAPTB     0xE98F
	#define   RegWWAPTC     0xE990
	
	#define   RegWWAQTA      0xE991
	#define   RegWWAQTB      0xE992
	#define   RegWWAQTC      0xE993
	
	
	//=============================================================
	//Threshold register  
	//=============================================================
	#define   RegZZEGYTHH     0xEC1E
	#define   RegZZEGYTHL     0xEC1F
	
	#define   RegCTHH       0xE8AA
	#define   RegCTHL       0xE8AB
	
	
	//=============================================================
	//Checksum register 
	//=============================================================
	#define   RegCKSUM     0xC003
	//=============================================================
	//Current sense threshold register  
	//=============================================================
	#define  RegZZDCUM    0xEC1D
	
	
	//=============================================================
	//Full wave power factor register
	//=============================================================
	#define RegMAFA  0xE8E4
	#define RegMAFB  0xE8E5
	#define RegMAFC  0xE8E6
	#define RegMAFS   0xE8E7
	
	
	
	
	#define APhy  0x00
	#define BPhy  0x01
	#define CPhy  0x02
	#define ABCPhy 0x03
	
	
	#define RMSUA        0xE944
	#define RMSUB        0xE945
	#define RMSUC        0xE946
	
	#define RMSI1A       0xE90E
	#define RMSI1B       0xE90F
	#define RMSI1C       0xE910
	
	#define DATAPA       0xE8E8
	#define DATAPB      0xE8E9
	#define DATAPC       0xE8EA
	#define DATAPABC       0xE8EE
	
	#define DATAQA        0xE8F0
	#define DATAQB        0xE8F1
	#define DATAQC        0xE8F2
	#define DATAQABC       0xE8F3
	
	#define DATAFREQA    0xC008
	#define DATAFREQB    0xC009
	#define DATAFREQC    0xC00A
	
	
	#define DATAFREQ    0xC008
	
	
	
	
	//The following is for the compiler through
	#define DATAP       0x0119
	#define DATAQ       0x10d7
	#define RMSU        0x011B
	#define RMSI1       0x011C
	#define RMSI2       0x0117
	
	#define RAP         0x0102  // Full-wave active power original value
	#define RAQ         0x0103  // Full-wave reactive power original value
	#define ARRTU       0x0104  // Full-wave current rms original value
	#define ARRTI       0x0105  // full wave voltage rms original value
	// The above is for compile through
	
	
	
	
	
	enum
	{
	    Addr_UA=0,
	    Addr_IA,
	    Addr_UB,
	    Addr_IB,
	    Addr_UC,
	    Addr_IC,
	};
	
	typedef struct
	{
	    unsigned int  RacWARTU;           //Full-wave voltage rms ratio difference register
	    unsigned int  RacWARTI;           //Full-wave current rms ratio difference register
	    unsigned int  RacWAPT;            //Full-wave active power ratio difference register
	    unsigned int  RacWWAPT;           //Full-wave active power secondary compensation register                
	    unsigned int  RacREWWAPT;         //Full-wave reactive power secondary compensation register               
	}S_JBRE;
	
	
	typedef struct 
	{
	    unsigned short ui_MeterC;           // Table constant
	    unsigned short ui_Un;               // Nominal voltage
	    unsigned short ui_Ib;               // Nominal current
	    unsigned short ui_Resve1;           //Retention
	    
	    unsigned int  RacEGYTH;           //Active threshold register
	    unsigned int  RacCTHH;             //Start / creep judgment threshold register
	    unsigned int  RacCTHL;             //Start / creep judgment threshold register
	    unsigned int RacZZDCUM;        //Current detection threshold
	    unsigned int  RacWAEC0;         //Angle difference 0
	    unsigned int RacMTPARA0;
	    unsigned int RacMTPARA1;
	    unsigned int RacMTPARA2;
	    unsigned int  RacANCtrl0;         //Analog control register0
	    unsigned int  RacANCtrl1;         // Analog control register 1
	    unsigned int  RacANCtrl2;         // Analog control register 2
	    unsigned int  RacANCtrl3;         // Analog control register 2
	    S_JBRE  gs_JBA;             //A Compared to the difference group
	    S_JBRE  gs_JBB;             //B Compared to the difference group
	    S_JBRE  gs_JBC;             //C Compared to the difference group
	    unsigned int ul_PG;               //Power proportional coefficient
	    unsigned int ul_URmG;             //Voltage channel proportional coefficient
	    unsigned int ul_I1RmG;            //Current channel 1 proportional coefficient
	
	    unsigned short ui_Resve2;           // Retention
	    unsigned short ui_JbCRC;            // Calibration table parameters CRC results
	}S_JBPM;
	
	V9203EXT S_JBPM  gs_JbPm;            // Calibration table parameters
	/*
	typedef struct 
	{       // Construct complex structures
	    float  real;
	    float  imag;
	} COMPLEX;
	
	V9203EXT COMPLEX  fftx[256];
	*/
	V9203EXT volatile unsigned char guc_CommDelayTime;                                 //Communication delay
	V9203EXT unsigned char guc_CommDelayFlag;
	V9203EXT unsigned char guc_Phy;          // Phase
	V9203EXT unsigned char guc_DODMAFlg;
	//BRONCOEXT unsigned char guc_FFT_Flg;
	
	
	unsigned char  WriteBronco(unsigned int Data,unsigned short Addr);
	void Bronco_PMCtrl(unsigned char pm);
	unsigned int  ReadBronco(unsigned short Addr);
	void BroncoInit(void);
	void UpdateBroncoPara(void);
	void JbpmInit(void);
	//unsigned char Brc_CheckDMAData(short* DMAaddr, unsigned char ucType);
	//void Recording(void);
	//void FFT_Task(short *indata);
	//void Data_FFTProc(void);
	#endif
