#include <V9203.h>
#include <SPI.h>
#define  V9203EXT
	
/*=========================================================================================\n
* @function_name: Bronco_PMCtrl
* @function_file: Bronco.c
* @description: 
* 
* @parameters: 
* @return: 
* @author:   Vango Application Teams
* @note: 
	*-------------------------------------------------------------------------------------------
* @modifier:  
* @modify content: 
	===========================================================================================*/
void Bronco_PMCtrl(unsigned char pm)
{
//Power mode is hardwired in devkit, otherwise set both bits high
/*
if(pm==Work_normal)
	    {
	       GPIO_SetBits(GPIOE, GPIO_Pin_10|GPIO_Pin_11);	
	    }
		*/
	}
	
	static unsigned char tdo_m,tdo_c;
	static unsigned int  tdo_d;
	static unsigned char spi_err;
	S_JBPM  gs_JbPm;
	static unsigned int  _cs = 9;
	
	void set_data_cmd_flash(unsigned char cmd, unsigned int dat)
	{
	
	    unsigned char cksum,cmdb;
	    unsigned int send_dat;
	    cmdb = (0x3f&cmd)|0x80;
	    cksum = ~((dat&0x00ff) + (dat>>8) + cmdb);
	 //	cksum = 0;
		send_dat=dat;
		
	
		SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE3));
		digitalWrite(_cs,LOW);
		tdo_m=SPI.transfer(cmdb);
		tdo_d=SPI.transfer((send_dat>>8));
		tdo_d=tdo_d<<8;
		tdo_d+=SPI.transfer((send_dat));
		tdo_c=SPI.transfer(cksum);
		digitalWrite(_cs,HIGH);
		SPI.endTransaction();
	}
	
	void spi_wr_flash(unsigned int addr, unsigned int dat_h, unsigned int dat_l)
	{
	    set_data_cmd_flash(0x08,dat_l);
	    set_data_cmd_flash(0x0a,dat_h);
	    set_data_cmd_flash(0x0c,addr );
	}
	
	unsigned long spi_rd_flash(unsigned int addr)
	{
	    unsigned long mt_dout;
	
	    set_data_cmd_flash(0x10, addr); 
	    set_data_cmd_flash(0x12, addr);
	    mt_dout = (unsigned long)tdo_d;
	    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
		{
	        spi_err = 1;
	    }
		else
		{
	        spi_err = 0;
	    }
	    set_data_cmd_flash(0x14, addr);
	    mt_dout += (((unsigned long)tdo_d)<<16);
	    if((unsigned char)(tdo_m+(unsigned char)(tdo_d&0x00ff)+(unsigned char)(tdo_d>>8))!=(unsigned char)(~tdo_c))
		{
	        spi_err = 1;
	    }
		else
		{
	        spi_err = 0;
	    }
	    
	    return mt_dout;
	}
	
	const unsigned short CrcTab[256]={ // X16+X12+X5+1 余式表
	    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
	};
	
	/*=========================================================================================\n
	* @function_name: do_CRC
	* @function_file: Api.c
	* @description: CRC verify
	* 
	* 
	* @parameter: 
	* @param:data  Data address
	* @param:len  Data length
	* 
	* @return: 
	* @return: uint16 CRC verify value
	* @author:   lwb (2012-02-29)
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	unsigned short do_CRC(unsigned char *str,unsigned char len)
	{
	/*  Word16 CrcCode;
	    CrcCode.word=0;
	    while(len--)
	    {   
	        CrcCode.word = (CrcCode.word >> 8) ^ CrcTab[(CrcCode.word ^(*(str++))) & 0xff];
	    }
	    return CrcCode.word;
	*/
	    unsigned char ucData;
	    unsigned short crc;
	
	    crc = 0x5555;
	
	    for( ;len > 0; len--)
	    {
	        ucData = (unsigned char) (crc / 0x100);    // Temporary crc high 8 bits
	        crc <<= 8;
	        crc ^= CrcTab[ucData ^ (*str)];     // High byte and current data XOR re-check table
	        str++;
	    }
	
	    return crc;
	}
	
	/*=========================================================================================\n
	* @function_name: WriteBronco
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	
	unsigned char  WriteBronco(unsigned int Data,unsigned short Addr)
	{
	    //spi_wr_flash(unsigned int addr, unsigned int dat_h, unsigned int dat_l)  
	    spi_wr_flash(Addr, (Data>>16), Data);
	    return 0;
	}
	
	/*=========================================================================================\n
	* @function_name: ReadBronco
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	unsigned int  ReadBronco(unsigned short Addr)
	{
	   return spi_rd_flash(Addr);
	}
	
	/*=========================================================================================\n
	* @function_name: BroncoInit
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	void BroncoInit(void)
	{
	    static unsigned int ready;
	    //Bronco_PMCtrl(Work_normal);
		pinMode(_cs,OUTPUT );
		//pinMode(energy_WO,INPUT );
	 
		/* Enable SPI */  
		SPI.begin();
		
	    ready = 0;
	    while(ready!=0x100000ff)
		{
				Serial.println("Waiting for chip");
	            WriteBronco(0x100000ff,0xc000);
	            ready=ReadBronco(0xc000);
				Serial.print("0x");
				Serial.println(ready,HEX);
				delay(1000);
	    }
	    for(unsigned char i=0;i<56;i++)
	    {
	        WriteBronco(0,(0xC800+i));
	    } 
	
	
	    /*static unsigned int spiready;
	    spiready=0;
	        while(spiready!=0x100000ff)
	        {
	           WriteBronco(0x100000ff,0xc000);
	              spiready=ReadBronco(0xc000);
	        }
	        for(unsigned char i=0;i<56;i++)
	        {
	            WriteBronco(0x0,0xC800+i);
	        } */
	}
	
	
	/*=========================================================================================\n
	* @function_name: UpdateBroncoPara
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	void UpdateBroncoPara(void)
	{
	    unsigned int sum;
	    sum = 0;
	    
	    //BE_ReadP(EEP_JBTOTAL,(unsigned char*)&gs_JbPm,sizeof(S_JBPM));
	    WriteBronco(0xaa000000,RegMTPARA0); //spi_wr_flash(0xc000,0xaa00,0x0000);	//MT_PARA0			2.27 add Anti-CF flash
	    delay(20);
	    WriteBronco(0x000000ff,RegMTPARA0);//spi_wr_flash(0xC000,0x0000,0x00ff);	//MT_PARA0
	    sum+=0x000000ff;
	    WriteBronco(gs_JbPm.RacMTPARA1,RegMTPARA1); //spi_wr_flash(0xC001,0x0004,0x0404);	//MT_PARA1
	    sum+= gs_JbPm.RacMTPARA1;
	    WriteBronco(gs_JbPm.RacMTPARA2,RegMTPARA2); //spi_wr_flash(0xC002,0x0701,0x80ff);	//MT_PARA2 - Unsegmented，distribute 32bit separately twice，DMA enable UA，non-did，AD all-enable
	    sum+=gs_JbPm.RacMTPARA2;
	    WriteBronco(0x00000007,ZZAPPA); //spi_wr_flash(0xEC05,	0x0000,0x0007);  // Full-wave / fundamental combined phase power combination 	 A+B+C 
	    sum+=0x00000007; 
	    WriteBronco(0x00000001,ZZCPSEL); //spi_wr_flash(0xEC15,	0x0000,0x0001);  // Start the creep mode selection           according to the current rms value
	    sum+=0x00000001;
	    WriteBronco(gs_JbPm.RacANCtrl0,RegANCtrl0); //spi_wr_flash(0x8000,0x000f,0x1111);	// Analog control register 0       0x000f1111      Voltage gain double Current gain 4 times
	    sum+=gs_JbPm.RacANCtrl0;
	    WriteBronco(gs_JbPm.RacANCtrl1,RegANCtrl1); //spi_wr_flash(0x8001,0x0000,0x0000);	// Analog control register 1
	    sum+=gs_JbPm.RacANCtrl1;
	    WriteBronco(gs_JbPm.RacANCtrl2,RegANCtrl2); //spi_wr_flash(0x8002,0x7700,0x5440);	// Analog control register 2   0x77005440
	    sum+=gs_JbPm.RacANCtrl2;
	    WriteBronco(gs_JbPm.RacANCtrl3,RegANCtrl3); //spi_wr_flash(0x8003,0x0000,0x0406);	// Analog control register 3 // Down frequency 0x000d0406
	    sum+=gs_JbPm.RacANCtrl3;
	    WriteBronco( gs_JbPm.RacEGYTH,RegZZEGYTHL);  //spi_wr_flash(0xEC1F,0x2C7B,0xDF00);	// Low threshold
	    sum+= gs_JbPm.RacEGYTH;
	    WriteBronco(0x00000000,RegZZEGYTHH); //spi_wr_flash(0xEC1E,0x0000,0x0000);	// High threshold
	    sum+=0x00000000;
	    WriteBronco(gs_JbPm.RacCTHH,RegCTHH); //spi_wr_flash(0xE8AA,	0x0002,0x21E5);                   // enable threshold limit
	    sum+=gs_JbPm.RacCTHH;
	    WriteBronco(gs_JbPm.RacCTHL,RegCTHL); //spi_wr_flash(0xE8AB,	0x0001,0xEB4E);                   // enable threshold lower limit
	    sum+=gs_JbPm.RacCTHL;
	    WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAPTA0); //spi_wr_flash(0xE95A,0x0000,0x0000);	// Active A difference ratio 0	
	    sum+=gs_JbPm.gs_JBA.RacWAPT;
	    WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAPTB0); //spi_wr_flash(0xE95E,0x0000,0x0000);  // Active B difference ratio 0
	    sum += (gs_JbPm.gs_JBB.RacWAPT);
	    WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAPTC0); //spi_wr_flash(0xE962,0x0000,0x0000);	// Active C difference ratio 0  
	    sum+=gs_JbPm.gs_JBC.RacWAPT;
	   WriteBronco(gs_JbPm.gs_JBA.RacWAPT,RegWAQTA); //spi_wr_flash(0xE965,0x0000,0x0000);	// Reactive A difference ratio
	   sum+=gs_JbPm.gs_JBA.RacWAPT;
	   WriteBronco(gs_JbPm.gs_JBB.RacWAPT,RegWAQTB); //spi_wr_flash(0xE966,0x0000,0x0000);	// Reactive B difference ratio
	   sum+=gs_JbPm.gs_JBB.RacWAPT;
	   WriteBronco(gs_JbPm.gs_JBC.RacWAPT,RegWAQTC); //spi_wr_flash(0xE967,0x0000,0x0000);	// Reactive C difference ratio
	   sum+=gs_JbPm.gs_JBC.RacWAPT;
	
	    /*WriteBronco( gs_JbPm.gs_JBA.RacWWAPT,RegWWAPTA); //spi_wr_flash(0xE967,0x0000,0x0000); // Active A2 compensation ratio
	    sum+= gs_JbPm.gs_JBB.RacWWAPT;
	    WriteBronco( gs_JbPm.gs_JBB.RacWWAPT,RegWWAPTB); //spi_wr_flash(0xE967,0x0000,0x0000); // Active B2 compensation ratio
	    sum+= gs_JbPm.gs_JBB.RacWWAPT;
	    WriteBronco( gs_JbPm.gs_JBC.RacWWAPT,RegWWAPTC); //spi_wr_flash(0xE967,0x0000,0x0000); // Active C2 compensation ratio
	    sum+= gs_JbPm.gs_JBB.RacWWAPT;*/
	
	   
	   WriteBronco( gs_JbPm.gs_JBA.RacWARTI,RegWARTIA); 	//A Current rms difference
	   sum+=gs_JbPm.gs_JBA.RacWARTI;
	   WriteBronco( gs_JbPm.gs_JBB.RacWARTI,RegWARTIB); 	//B Current rms difference
	   sum+=gs_JbPm.gs_JBB.RacWARTI;
	   WriteBronco( gs_JbPm.gs_JBC.RacWARTI,RegWARTIC); 	//C Current rms difference
	   sum+=gs_JbPm.gs_JBC.RacWARTI;
	    WriteBronco( gs_JbPm.gs_JBA.RacWARTU,RegWARTUA); 	//A RMS voltage difference
	   sum+=gs_JbPm.gs_JBA.RacWARTU;
	   WriteBronco( gs_JbPm.gs_JBB.RacWARTU,RegWARTUB); 	//B RMS voltage difference
	   sum+=gs_JbPm.gs_JBB.RacWARTU;
	   WriteBronco(gs_JbPm.gs_JBC.RacWARTU,RegWARTUC); 	//C RMS voltage difference
	   sum+=gs_JbPm.gs_JBC.RacWARTU;
	   WriteBronco(0x00000000,RegWBPTA); //spi_wr_flash(0xE970,0x0000,0x0000);	// Fundamental wave A compensation ratio 0	
	   sum+=0x00000000;
	   WriteBronco(0x00000000,RegWBPTB); //spi_wr_flash(0xE971,0x0000,0x0000);  // Fundamental wave B compensation ratio 0
	   sum+=0x00000000;
	   WriteBronco(0x00000000,RegWBPTC); //spi_wr_flash(0xE972,0x0000,0x0000);	// Fundamental wave C compensation ratio 0  
	   sum+=0x00000000;
	  WriteBronco(0x00000000,RegWBQTA); // spi_wr_flash(0xE973,0x0000,0x0000);	// Fundamental reactive power A compensation ratio
	   sum+=0x00000000;
	   WriteBronco(0x00000000,RegWBQTB); //spi_wr_flash(0xE974,0x0000,0x0000);	// Fundamental reactive power B compensation ratio
	   sum+=0x00000000;
	   WriteBronco(0x00000000,RegWBQTC); //spi_wr_flash(0xE975,0x0000,0x0000);	// Fundamental reactive power C compensation ratio
	   sum+=0x00000000;
	   WriteBronco(gs_JbPm.RacWAEC0,RegWAEC0); //spi_wr_flash(0xE954,0x0082,0x8282);	// Angle difference
	   sum+=gs_JbPm.RacWAEC0;
	   WriteBronco(gs_JbPm.RacZZDCUM,RegZZDCUM);  //spi_wr_flash(0xEC1D,0xfff0,0x0000);	//UMChannel DC component  preset negative value  current detection interrupted
	   sum+=gs_JbPm.RacZZDCUM;
	  WriteBronco(0x00000015,0xEC23); //spi_wr_flash(0xEC23,0x0000,0x0015);	// Active and phase 0 configuration
	   sum+=0x15;
	   WriteBronco(0x0000002A,0xEC24); //spi_wr_flash(0xEC24,0x0000,0x002A);	// Active and phase 1 configuration
	   sum+=0x2a;
	    WriteBronco(0x00000015,0xEC47); //spi_wr_flash(0xEC47,0x0000,0x0015);	// Reactive and phase 0 configuration
	   sum+=0x15;
	    WriteBronco(0x0000002A,0xEC48);  //spi_wr_flash(0xEC48,0x0000,0x002A);	// Reactive and phase 1 configuration
	   sum+=0x2a;
	    WriteBronco(0x00000007,0xEC05); //spi_wr_flash(0xEC05,0x0000,0x0007);	// Visual and phase configuration
	   sum+=0x07;
	   WriteBronco(0x00002211,0xEC34); //spi_wr_flash(0xEC34,0x0000,0x2211);	// High-speed CF source selection
	   sum+=0x2211;
	   WriteBronco(0x00008000,0xA000); //spi_wr_flash(0xa000,0x0000,0x8000);	// interrupted 0
	   sum+=0x8000;
	   WriteBronco(0x00008000,0xA001); //spi_wr_flash(0xa001,0x0000,0x8000);	// interrupted 1
	   sum+=0x8000;
	   sum+=0xff000000;	  //ANA2
	   sum+=0x00000005;	  //ANA3
	   sum=0xffffffff-sum;
	   WriteBronco(sum,0xC003); //spi_wr_flash(0xC003,(sum&0xffff0000)>>16,(sum&0x0000ffff));	//mt_para3 Self-test and  SUM+x=0xffffffff
	   delay(10);
	   WriteBronco(0,0xa002); //spi_wr_flash(0xa002,0,0);	// Interrupt flag is cleared to 0  
	}
	
	/*=========================================================================================\n
	* @function_name: JbpmInit
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	
	void JbpmInit(void)
	{
	    //S_JBPM g_JBPm;
	    
	    //BE_ReadP(EEP_JBTOTAL,(unsigned char*)&gs_JbPm,sizeof(S_JBPM));
		/*
	    if(gs_JbPm.ui_JbCRC == do_CRC((unsigned char *)&gs_JbPm,sizeof(S_JBPM)-2))      // Whether the data in RAM is complete
	    {                                                                       // Because the following parameter is defined in the two structures
	        EnyB_JbPm_Updata();
	    }
	    else
	    {
			*/
	        gs_JbPm.ui_MeterC=1200;                 // Table constant
	        gs_JbPm.ui_Un=22000;                  // Nominal voltage
	        gs_JbPm.ui_Ib=5000;                   // Nominal current
	        gs_JbPm.ui_Resve1=0;
	    
	        gs_JbPm.RacEGYTH   = 0x2fd3ff5;      // Energy accumulation threshold  0x2C7BDF00  0x04fBDF00 0x5aa8c57
	        gs_JbPm.RacCTHH= 0x000221E5;      //Enable，creep threshold
	        gs_JbPm.RacCTHL= 0x0001EB4E;      // Enable，creep threshold
	        gs_JbPm.RacZZDCUM =  0xfff00000;       //0x0134 Current detection threshold
	        gs_JbPm.RacWAEC0 = 0x00000000;        // Angle difference
	    
	        gs_JbPm.RacMTPARA0 = 0x000000ff;
	        gs_JbPm.RacMTPARA1 = 0x00000000;
	        gs_JbPm.RacMTPARA2 = 0x070080ff;
	        gs_JbPm.RacANCtrl0 = 0x00000333;     
	        gs_JbPm.RacANCtrl1 = 0x00000000;      
	        gs_JbPm.RacANCtrl2 = 0x77005400;      //0x77005400;  0xF7005400;  
	        gs_JbPm.RacANCtrl3 = 0x00000406;      //0x00000406; 
	        gs_JbPm.gs_JBA.RacWARTU = 0xFC9A0D98;  // Full-wave voltage rms ratio difference register
	        gs_JbPm.gs_JBA.RacWARTI = 0x21A8301B;  // Full-wave current rms ratio difference register
	        gs_JbPm.gs_JBA.RacWAPT = 0x21E51894;   // Full-wave active power ratio difference register 0xEBA74B27
	        gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  // Full-wave active power secondary compensation register
	        gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  // Full-wave reactive power secondary compensation register
	    
	        gs_JbPm.gs_JBB.RacWARTU = 0xFD6F2E2F;  // Full-wave voltage rms ratio difference register
	        gs_JbPm.gs_JBB.RacWARTI = 0xE4913EB;  // Full-wave current rms ratio difference register
	        gs_JbPm.gs_JBB.RacWAPT = 0xF5DC2F3;   // Full-wave active power ratio difference register 0xECC04599
	        gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  // Full-wave active power secondary compensation register
	        gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  // Full-wave reactive power secondary compensation register
	    
	        gs_JbPm.gs_JBC.RacWARTU = 0xFD1996B1;  // Full-wave voltage rms ratio difference register
	        gs_JbPm.gs_JBC.RacWARTI = 0xE38E38E;  // Full-wave current rms ratio difference register
	        gs_JbPm.gs_JBC.RacWAPT = 0xEF92325;   //Full-wave active power ratio difference register0xEC4A811B
	        gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	        gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//2        
	//        gs_JbPm.gs_JBA.RacWARTU = 0xFD8BCFF6;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWARTI = 0x2D1D0388;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWAPT = 0x2EB09874;   //Full-wave active power ratio difference register 0xEBA74B27
	//        gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBB.RacWARTU = 0xFD1996B1;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWARTI = 0x169F2EBE;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWAPT = 0x17CA20E5;   //Full-wave active power ratio difference register0xECC04599
	//        gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBC.RacWARTU = 0xFD9A25E9;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWARTI = 0x18D14C87;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWAPT = 0x1A2EDF25;   //Full-wave active power ratio difference register0xEC4A811B
	//        gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	        
	//3        
	//        gs_JbPm.gs_JBA.RacWARTU = 0xFE72B913;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWARTI = 0x2D95363A;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWAPT = 0x306B4343;   //Full-wave active power ratio difference register 0xEBA74B27
	//        gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBB.RacWARTU = 0xFDC53B1D;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWARTI = 0x201EBE39;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWAPT = 0x21BF01E9;   //Full-wave active power ratio difference register0xECC04599
	//        gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBC.RacWARTU = 0xFE0D4AF2;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWARTI = 0x1E2E50BC;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWAPT = 0x2065452E;   //Full-wave active power ratio difference register0xEC4A811B
	//        gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	        
	//4        
	//        gs_JbPm.gs_JBA.RacWARTU = 0xFEACF849;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWARTI = 0x204BB4AD;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWAPT = 0x234E5C42;   //Full-wave active power ratio difference register 0xEBA74B27
	//        gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBB.RacWARTU = 0xFDB6DB6E;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWARTI = 0x18B7F5BB;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWAPT = 0x1A95CF74;   //Full-wave active power ratio difference register0xECC04599
	//        gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBC.RacWARTU = 0xFE9E6375;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWARTI = 0x151D5883;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWAPT = 0x17C5F09A;   //Full-wave active power ratio difference register0xEC4A811B
	//        gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	        
	//5        
	//        gs_JbPm.gs_JBA.RacWARTU = 0xFD60E224;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWARTI = 0x04966844;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBA.RacWAPT = 0x059958ED;   //Full-wave active power ratio difference register 0xEBA74B27
	//        gs_JbPm.gs_JBA.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBA.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBB.RacWARTU = 0xFD7D7D7E;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWARTI = 0x06BCA1AF;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBB.RacWAPT = 0x07EA144B;   //Full-wave active power ratio difference register0xECC04599
	//        gs_JbPm.gs_JBB.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBB.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	//    
	//        gs_JbPm.gs_JBC.RacWARTU = 0xFDB6DB6E;  //Full-wave voltage rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWARTI = 0x05800DAB;  //Full-wave current rms ratio difference register
	//        gs_JbPm.gs_JBC.RacWAPT = 0x06F825B1;   //Full-wave active power ratio difference register0xEC4A811B
	//        gs_JbPm.gs_JBC.RacWWAPT = 0x00000000;  //Full-wave active power secondary compensation register
	//        gs_JbPm.gs_JBC.RacREWWAPT = 0x00000000;  //Full-wave reactive power secondary compensation register 
	        
	        gs_JbPm.ui_Resve2=0;
	        gs_JbPm.ul_PG=0x10B;               //Power proportional coefficient
	        gs_JbPm.ul_URmG=0x513b;             // Voltage channel proportional coefficient
	        gs_JbPm.ul_I1RmG=0x1A2C0;            // Current channel 1 proportional coefficient
	    
	        gs_JbPm.ui_JbCRC=do_CRC((unsigned char*)&gs_JbPm,sizeof(S_JBPM)-2);     // The CRC result of the calibration parameter
	        //BE_WriteP(EEP_JBTOTAL,(unsigned char*)&gs_JbPm,sizeof(S_JBPM));
	    //}
	    
	}
	
	const unsigned short RMSAddrTab[] = {RegARTUA,RegARTIA,RegARTUB,RegARTIB,RegARTUC,RegARTIC};
	/*=========================================================================================\n
	* @function_name: Brc_CheckDMAData
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	/*
	unsigned char Brc_CheckDMAData(short* DMAaddr, unsigned char ucType)
	{
	    short iValue,iValueMax;
	    unsigned char i,j;
	    
	    for (j = 0;j<6;j++)
	    {
	        iValueMax = 0;
	        for (i = 0; i <= ucType; i++) 
	        {
	            iValue = *(DMAaddr+6*i+j) - gs_DMACheck.uiDCValue[j];
	            if (iValueMax < abs(iValue))
	            {
	                iValueMax = abs(iValue);
	            }
	        }
	        if (j%2==0) //Voltage, less than 100V
	        {
	            if ((iValueMax>400)||((iValueMax<288)&&(iValueMax>200)))
	            {
	                //return Ret_Err;
	                gs_DMACheck.ucPhase[j] = true;    //Determine items
	            }
	        }
	        else        // Electric current
	        {
	            if ((iValueMax>388)||((iValueMax<288)&&(iValueMax>200)))
	            {
	                gs_DMACheck.ucPhase[j] = true;    //Determine items
	            }
	        }     
	    }
	    
	    for(i = 0;i<6;i++)
	    {
	        if(gs_DMACheck.ucPhase[i] == true)
	        {
	            return Ret_Err;
	        }
	    }
	//  iValue = iDMAdata - gs_DMACheck.uiDCValue[ucType];
	//  ucTemp = ucType%2;
	//  if(ucTemp == 0)
	//  {
	//      if((iValue>388)||(iValue<-388))
	//      {
	//          return Ret_Err;
	//      }
	//  }
	//  else
	//  {
	//      if((iValue>10000)||(iValue<-10000))
	//      {
	//          return Ret_Err;
	//      }
	//  }
	    return Ret_OK;
	}
	*/
	/*=========================================================================================\n
	* @function_name: Recording
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	/*
	void Recording(void)
	{
	    unsigned short addr = 0;
	    GetExtRTC();  
	    gs_UIP.ulSecond = gs_ClkTmp.ucSecond; 
	    gs_UIP.ulMinute = gs_ClkTmp.ucMinute;
	    gs_UIP.ulHour = gs_ClkTmp.ucHour;
	    gs_UIP.ulDay = gs_ClkTmp.ucDay;
	    gs_UIP.ul_Energy = gs_EnergyA.lCP + Eny_GetEp1(0,0,ABCPhy,active);
	    gs_UIP.ul_NEnergy =gs_NEnergyA.lCP + Eny_GetEp1(0,0,ABCPhy,reactive);
	    for(unsigned char i=0;i<3;i++)
	    {
	        gs_UIP.ul_U[i] = ReadBronco(RMSUA+i)/gs_JbPm.ul_URmG;
	        gs_UIP.ul_I[i] = ReadBronco(RMSI1A+i)/gs_JbPm.ul_I1RmG;
	        gs_UIP.ul_Power[i] = ReadBronco(DATAPA+i)/gs_JbPm.ul_PG ;
	        gs_UIP.ul_Npower[i] = ReadBronco(DATAQA+i)/gs_JbPm.ul_PG;
	    }
	    SysE2ReadData(EEP_EPFH_PT,(unsigned char*)&addr,2);
	    if(addr >= (EEP_EPFH_PT +2 + 4*24*6) )
	    {
	        addr = EEP_EPFH_PT +2;
	    }
	    SysE2ParaSetManage(addr,(unsigned char*)&gs_UIP,36);
	    addr +=36;
	    SysE2ParaSetManage(EEP_EPFH_PT,(unsigned char*)&addr,2);
	}
	*/
	/*256 click hanning window*/
	/*
	const float Hannwindow[256]=
	{
	    0,
	    0.0001518,
	    0.0006070,
	    0.0013654,
	    0.0024265,
	    0.0037897,
	    0.0054542,
	    0.0074189,
	    0.0096826,
	    0.0122440,
	    0.0151015,
	    0.0182534,
	    0.0216978,
	    0.0254325,
	    0.0294554,
	    0.0337639,
	    0.0383554,
	    0.0432273,
	    0.0483764,
	    0.0537997,
	    0.0594939,
	    0.0654555,
	    0.0716810,
	    0.0781664,
	    0.0849080,
	    0.0919015,
	    0.0991429,
	    0.1066275,
	    0.1143510,
	    0.1223086,
	    0.1304955,
	    0.1389068,
	    0.1475372,
	    0.1563817,
	    0.1654347,
	    0.1746908,
	    0.1841445,
	    0.1937899,
	    0.2036212,
	    0.2136324,
	    0.2238175,
	    0.2341703,
	    0.2446844,
	    0.2553535,
	    0.2661712,
	    0.2771308,
	    0.2882257,
	    0.2994492,
	    0.3107945,
	    0.3222546,
	    0.3338226,
	    0.3454915,
	    0.3572542,
	    0.3691036,
	    0.3810324,
	    0.3930335,
	    0.4050995,
	    0.4172231,
	    0.4293969,
	    0.4416136,
	    0.4538658,
	    0.466146,
	    0.4784467,
	    0.4907605,
	    0.5030800,
	    0.5153975,
	    0.5277057,
	    0.5399971,
	    0.5522642,
	    0.5644996,
	    0.5766958,
	    0.5888455,
	    0.6009412,
	    0.6129756,
	    0.6249415,
	    0.6368315,
	    0.6486384,
	    0.6603551,
	    0.6719745,
	    0.6834894,
	    0.6948929,
	    0.7061782,
	    0.7173382,
	    0.7283663,
	    0.7392558,
	    0.75,
	    0.7605924,
	    0.7710267,
	    0.7812964,
	    0.7913953,
	    0.8013173,
	    0.8110564,
	    0.8206067,
	    0.8299623,
	    0.8391176,
	    0.8480670,
	    0.8568051,
	    0.8653266,
	    0.8736263,
	    0.8816991,
	    0.8895403,
	    0.8971449,
	    0.9045085,
	    0.9116265,
	    0.9184946,
	    0.9251086,
	    0.9314645,
	    0.9375585,
	    0.9433868,
	    0.9489460,
	    0.9542326,
	    0.9592435,
	    0.9639755,
	    0.9684259,
	    0.9725919,
	    0.976471,
	    0.9800608,
	    0.9833592,
	    0.9863641,
	    0.9890738,
	    0.9914865,
	    0.9936009,
	    0.9954156,
	    0.9969296,
	    0.9981418,
	    0.9990517,
	    0.9996585,
	    0.9999621,
	    0.9999621,
	    0.9996585,
	    0.9990517,
	    0.9981418,
	    0.9969296,
	    0.9954156,
	    0.9936009,
	    0.9914865,
	    0.9890738,
	    0.9863641,
	    0.9833592,
	    0.9800608,
	    0.976471,
	    0.9725919,
	    0.9684259,
	    0.9639755,
	    0.9592435,
	    0.9542326,
	    0.9489460,
	    0.9433868,
	    0.9375585,
	    0.9314645,
	    0.9251086,
	    0.9184946,
	    0.9116265,
	    0.9045085,
	    0.8971449,
	    0.8895403,
	    0.8816991,
	    0.8736263,
	    0.8653266,
	    0.8568051,
	    0.8480670,
	    0.8391176,
	    0.8299623,
	    0.8206067,
	    0.8110564,
	    0.8013173,
	    0.7913953,
	    0.7812964,
	    0.7710267,
	    0.7605924,
	    0.75,
	    0.7392558,
	    0.7283663,
	    0.7173382,
	    0.7061782,
	    0.6948929,
	    0.6834894,
	    0.6719745,
	    0.6603551,
	    0.6486384,
	    0.6368315,
	    0.6249415,
	    0.6129756,
	    0.6009412,
	    0.5888455,
	    0.5766958,
	    0.5644996,
	    0.5522642,
	    0.5399971,
	    0.5277057,
	    0.5153975,
	    0.5030800,
	    0.4907605,
	    0.4784467,
	    0.466146,
	    0.4538658,
	    0.4416136,
	    0.4293969,
	    0.4172231,
	    0.4050995,
	    0.3930335,
	    0.3810324,
	    0.3691036,
	    0.3572542,
	    0.3454915,
	    0.3338226,
	    0.3222546,
	    0.3107945,
	    0.2994492,
	    0.2882257,
	    0.2771308,
	    0.2661712,
	    0.2553535,
	    0.2446844,
	    0.2341703,
	    0.2238175,
	    0.2136324,
	    0.2036212,
	    0.1937899,
	    0.1841445,
	    0.1746908,
	    0.1654347,
	    0.1563817,
	    0.1475372,
	    0.1389068,
	    0.1304955,
	    0.1223086,
	    0.1143510,
	    0.1066275,
	    0.0991429,
	    0.0919015,
	    0.0849080,
	    0.0781664,
	    0.0716810,
	    0.0654555,
	    0.0594939,
	    0.0537997,
	    0.0483764,
	    0.0432273,
	    0.0383554,
	    0.0337639,
	    0.0294554,
	    0.0254325,
	    0.0216978,
	    0.0182534,
	    0.0151015,
	    0.0122440,
	    0.0096826,
	    0.0074189,
	    0.0054542,
	    0.0037897,
	    0.0024265,
	    0.0013654,
	    0.0006070,
	    0.0001518,
	    0,
	};
	*/
	//COMPLEX fftx[256];
	/*
	unsigned char get_order(unsigned short size)
	{
	    short order;
	    size = (size - 1) >> (0);
	    order = -1;
	    do {
	        size >>= 1;
	        order++;
	    } while (size);
	    return order;
	}
	
	
	void fft(COMPLEX *x, unsigned short Num)			
	{   
	    float temp_re; 
	    float temp_im;
	    unsigned short i,j,k,P; 
	    unsigned short  LH,K,B;
	    unsigned char num; 
	    COMPLEX tmp;
	    LH=Num/2; 
	    j=LH; 
	   
	    for(i=1;i<Num-1;i++) // Reverse the highest bit plus 1, carry to the second high when 2 occurred
	    { 
	        if(i<j) 
	        { 
	            tmp.real=x[j].real;
	            tmp.imag=x[j].imag;
	            x[j].real =x[i].real;
	            x[j].imag =x[i].imag;
	            x[i].real=tmp.real;
	            x[i].imag=tmp.imag; 
	        } 
	        K=Num/2; 
	        while((j<K)==0) 
	        { 
	            j=j-K; 
	            K=K/2; 
	        } 
	        j=j+K; 
	    } 
		num=get_order(Num)+1;		          // The bit is entered for the Num point input sequence
	    for(i=1;i<=num;i++)                // The first re-cycle controls the number of butterfly stages 
	    { 
	        B=pow(2,(i-1)); 
	        for(j=0;j<=(B-1);j++)     // Control the butterfly and rotation factor for each level 
	        { 
	        //         P=pow(2,(i-1))-1; 
	            P=pow(2,(num-i))*j; 
	            temp_re=cos(2*3.14159*P/Num); 
	            temp_im=sin(2*3.14159*P/Num); 
	            for(k=j;k<Num;k=k+pow(2,i)) 
	            { 
	                COMPLEX FFT_temp1,FFT_temp2;
	                FFT_temp1.real=x[k].real; 
	                FFT_temp2.real=x[k+B].real; 
	                FFT_temp1.imag=x[k].imag; 
	                FFT_temp2.imag=x[k+B].imag; 
	                
	                x[k].real=FFT_temp1.real + FFT_temp2.real*temp_re + FFT_temp2.imag*temp_im; 
	                x[k].imag=FFT_temp1.imag + FFT_temp2.imag*temp_re - FFT_temp2.real*temp_im; 
	                x[k+B].real=FFT_temp1.real - FFT_temp2.real*temp_re - FFT_temp2.imag*temp_im; 
	                x[k+B].imag=FFT_temp1.imag - FFT_temp2.imag*temp_re + FFT_temp2.real*temp_im; 
	            } 
	        } 
	    } 
	}
	*/
	
	/*=========================================================================================\n
	* @function_name: FFT_Task
	* @function_file: Bronco.c
	* @description: 
	* 
	* @parameter: 
	* @return: 
	* @author:   Vango Application Teams
	* @note: 
	*-------------------------------------------------------------------------------------------
	* @modifier:  
	* @modify content: 
	===========================================================================================*/
	/*
	void FFT_Task(short *indata)
	{
	    unsigned short i;	
	    unsigned short  buffSize;  
	    buffSize = 256;
	    for(i=0; i<buffSize ;i++)
	    {								  
	        fftx[i].real = (indata[i]*Hannwindow[i]);	
	        //fftx[i].real = indata[i];// Read the stored sample data
	        fftx[i].imag = 0;               
	    }	
	    fft(fftx,buffSize);				   //FFT
	    for(i=0; i<buffSize; i++)		   // Take mold
	    {
	        fftx[i].real = sqrt((double)(fftx[i].real*fftx[i].real) + (double)(fftx[i].imag*fftx[i].imag));
	    }
	//    for(i=0;i<buffSize;i++)
	//    {
	//        gs_RawData.Raw_UA[i] = fftx[i].real;
	//    }
	}
	//const unsigned short FFTE2SaveTab[] = {EEP_FFTUA_PT,EEP_FFTIA_PT,
	//                               EEP_FFTUB_PT,EEP_FFTIB_PT,
	//                               EEP_FFTUC_PT,EEP_FFTIC_PT};
	//void Data_FFTProc(void)
	//{
	////             bcd = 0x22;          
	////            byte[0]: n-th harmonic content
	////            byte[1]: Phase
	////            byte[3]: UI
	////           Frequency resolution of 6.25, can be calculated up to 15 harmonics
	////
	//    unsigned char  ucCurPt;
	//    unsigned char  j;
	//    unsigned short i,pt;
	//    unsigned short ucTemp,ucSampLen;
	//    Word16 uiSamp;
	//
	//    ucSampLen = BUFFSIZE/6;
	//    for(i=0,pt=0;i<350;i++,pt=pt+2)
	//    {
	//        for(j=0;j<6;j++)
	//        {
	//            ucTemp = 2*(i*6+j);
	//            ucCurPt = (SPI1_Buffer_Rx[ucTemp] >> 1) & 0x07;
	//            uiSamp.word =(short)((((SPI1_Buffer_Rx[ucTemp]&0x7ff0)<<7)|((SPI1_Buffer_Rx[ucTemp+1]&0x7ff0)>>4))>>6);
	//            BE_WriteP(FFTE2SaveTab[ucCurPt]+pt,(unsigned char*)&uiSamp.word,2);
	//        }
	//    }
	//    
	//}
	*/
