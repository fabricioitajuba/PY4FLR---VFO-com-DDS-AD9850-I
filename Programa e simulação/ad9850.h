///////////////////////// ad9850.h //////////////////////////// 
//      #define CLK       PIN_B4 
//      #define FQUP      PIN_B3 
//      #define BitData   PIN_B5 
//      #define REST      PIN_B2 
   void AD9850_init(void) 
   { 
      output_bit(REST, 0); 
      output_bit(FQUP, 0); 
      output_bit(CLK, 0); 
      output_bit(BitData, 0); 
   } 

   void AD9850_reset(void) 
   { 
     output_bit(CLK, 0); 
     output_bit(FQUP, 0); 
     //Reset signal 
     output_bit(REST, 0); 
     output_bit(REST, 1); 
     output_bit(REST, 0); 
     //Clk  signal 
     output_bit(CLK, 0); 
     output_bit(CLK, 1); 
     output_bit(CLK, 0); 
     //Fq-up signal 
     output_bit(FQUP, 0); 
     output_bit(FQUP, 1); 
     output_bit(FQUP, 0); 
   } 
    

   void AD9850_wr_serial(unsigned char w0,double frequence) 
   { 
     unsigned char i,w; 
     int32 y; 
     double x; 
      
     //Calculate the frequency of the HEX value 
     x=4294967295/125;//Suitable for 125M Crystal 
     frequence=frequence/1000000; 
     frequence=frequence*x; 
     y=frequence; 
      
     //write w4 
     w=(y>>=0); 
     for(i=0; i<8; i++) 
     { 
       output_bit(BitData, (w>>i)&0x01); 
       output_bit(CLK, 1); 
       output_bit(CLK, 0); 
     } 
     //write w3 
     w=(y>>8); 
     for(i=0; i<8; i++) 
     { 
       output_bit(BitData, (w>>i)&0x01); 
       output_bit(CLK, 1); 
       output_bit(CLK, 0); 
     } 
     //write w2 
     w=(y>>16); 
     for(i=0; i<8; i++) 
     { 
       output_bit(BitData, (w>>i)&0x01); 
       output_bit(CLK, 1); 
       output_bit(CLK, 0); 
     } 
     //write w1 
     w=(y>>24); 
     for(i=0; i<8; i++) 
     { 
       output_bit(BitData, (w>>i)&0x01); 
       output_bit(CLK, 1); 
       output_bit(CLK, 0); 
     } 
     //write w0 
     w=w0; 
     for(i=0; i<8; i++) 
     { 
       output_bit(BitData, (w>>i)&0x01); 
       output_bit(CLK, 1); 
       output_bit(CLK, 0); 
     } 
     output_bit(FQUP, 1); 
     output_bit(FQUP, 0); 
   }    

   void AD9850_wr_parrel(unsigned char w0,double frequence) 
   { 
      
   } 
    
   void AD9850_off(void) 
   { 
      AD9850_wr_serial(0b00000100, 100000); 
   } 
    
   void AD9850_sweep(int32 minsw, int32 maxsw, int16 delaysw, int16 swtime=200, boolean rw) 
   { 
  //        swtime   =  duracion sweep (segundos) 
  //        minsw    =  min sweep frec. 
  //        maxsw    =  max sweep frec. 
  //        delaysw  =  delay entre cada operacion 
  //        rw       =  vuelta activada (sweep inverso) 
      float iz=0; 
  
      for(iz=minsw; iz<=maxsw; iz=iz+(maxsw/swtime)) 
      { 
         AD9850_wr_serial(0x00, iz); 
         delay_ms(delaysw); 
      } 
      if (rw==true) 
      { 
         for(iz=maxsw; iz>=minsw; iz=iz-maxsw/swtime) 
         { 
            AD9850_wr_serial(0x00, iz); 
            delay_ms(delaysw); 
         } 
      } 
  }
