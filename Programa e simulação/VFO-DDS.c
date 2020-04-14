/////////////////////////////////////////////////////////////
////  Programa VFO com o DDS AD9850                      ////
////  Projeto: Eng. Fabrício de Lima Ribeiro             ////
////  07/03/2015                                         ////
/////////////////////////////////////////////////////////////

#include <16F877A.h>
#fuses HS,NOWDT,PROTECT,NOLVP
#use delay(clock=20000000)

#define CLK       PIN_C6 
#define FQUP      PIN_C5 
#define BitData   PIN_C7 
#define REST      PIN_C4 

#include <lcd.c>
#include <ad9850.h>

/*
step = 0 -> 1Hz
step = 1 -> 10Hz
step = 2 -> 100Hz
step = 3 -> 1kHz
step = 4 -> 10kHz
step = 5 -> 100kHz
step = 6 -> 1MHz
*/

//encoder
//signed int8 enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
signed int8 enc_states[] = {0,-1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
int16 enc_position = 0; 
signed int32 F_anterior = 0;

//sistema
int1 plota_display=1;
char step=0;
unsigned int32 step_valor=1;
signed int32 F;
unsigned int16 mega, kilo, cent;

void encoder0Loop(void) 
{         
   //enc_position <<= 2;
   enc_position = enc_position << 2;
   //enc_position |= (input_b() & 0x03);   
   enc_position = enc_position | (input_b() & 0x03);   
   //F +=step_valor*enc_states[(enc_position & 0x0F)];
   F = F+step_valor*enc_states[(enc_position & 0x0F)];
     
   if(F != F_anterior)
   {   
      if(F < 0)
         F = 0; 

      if(F >= 40000000)
         F = 40000000;      
      
      F_anterior = F; 
      plota_display = 1;    
   }
}

void main()
{
   set_tris_b(0b00000011);
   set_tris_c(0b00000001);

   lcd_init();

   AD9850_init();
   AD9850_reset();
   
   printf(lcd_putc,"\f    VFO-DDS    ");
   printf(lcd_putc,"\nPY4FLR/PX4C1428");   

   delay_ms(3000);
  
   //Teste
   F = 7000000;
   
   //port_b_pullups(TRUE);
   //ext_int_edge (H_TO_L);

   while (true)
   {
      if(plota_display == 1)
      {  
         mega = F/1000000;
         kilo = (F/1000)-(mega*1000);
         cent = F-(mega*1000000)-(kilo*1000);         
         printf(lcd_putc,"\f%2Lu.%03Lu.%03Lu Hz",mega, kilo, cent);
         
         if(step == 0)
            printf(lcd_putc,"\n             1Hz");
         else if(step == 1)
            printf(lcd_putc,"\n            10Hz");
         else if(step == 2)
            printf(lcd_putc,"\n           100Hz");
         else if(step == 3)
            printf(lcd_putc,"\n            1KHz");
         else if(step == 4)
            printf(lcd_putc,"\n           10KHz");
         else if(step == 5)
            printf(lcd_putc,"\n          100KHz");
         else if(step == 6)
            printf(lcd_putc,"\n            1MHz");            
         
         AD9850_wr_serial(0b00000000, F);
         plota_display = 0;
      }
      
      //Altera o STEP
      if(input(PIN_C0) == 0)
      {
         delay_ms(10);
         do{
         }while (input(PIN_C0) == 0);        
         
         step++;         
         if(step == 7)
            step=0;
         if(step == 0)
            step_valor=1;
         if(step == 1)
            step_valor=10;            
         if(step == 2)
            step_valor=100;
         if(step == 3)
            step_valor=1000;
         if(step == 4)
            step_valor=10000;
         if(step == 5)
            step_valor=100000;
         if(step == 6)
            step_valor=1000000;            
            
         plota_display = 1; 
      }      
      encoder0Loop(); 
   }

}


