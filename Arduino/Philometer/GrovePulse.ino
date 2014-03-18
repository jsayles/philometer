// Function: This program can be used to measure heart rate
// The lowest pulse in the program be set to 30.
// Use an external interrupt to measure it.
// Hardware: Grove - Ear-clip Heart Rate Sensor, Grove - Base Shield, Grove - LED
// Arduino IDE: Arduino-1.0
// Author: FrankieChu		
// Date: Jan 22, 2013
// Version: v1.0
// by www.seeedstudio.com

unsigned char counter;
unsigned long temp[21];
unsigned long sub;
bool data_effect=true;

unsigned int heart_rate = 0;

unsigned int last_base_reading = 0;

// You can change it follow your system's request.
// 2000 meams 2 seconds. System return error 
// if the duty overtrip 2 second.
const int max_heartpluse_duty = 2000; 

void setupHeartMonitor() {
   attachInterrupt(1, interrupt, RISING); 
}

int getHeartRate() {
   return heart_rate;
}

int getBaseHeartReading() {
   return last_base_reading;
}

int getPulseStatusColor() {
   if (last_base_reading > 200 && last_base_reading < 1500) {
      return GREEN;
   }
   return RED;
}

/*Function: calculate the heart rate*/
void sum() {
   if(data_effect) {
      heart_rate=1200000/(temp[20]-temp[0]); //60*20*1000/20_total_time 
      //Serial.print("Heart_rate_is:\t");
      //Serial.println(heart_rate);
   }
   data_effect=1; //sign bit
}

/*Function: Interrupt service routine.Get the sigal from the external interrupt*/
void interrupt() {
   temp[counter]=millis();
   //Serial.println(counter,DEC);
   //Serial.println(temp[counter]);
   switch(counter) {
      case 0:
         sub=temp[counter]-temp[20];
         //Serial.println(sub);
         break;
      default:
         sub=temp[counter]-temp[counter-1];
         //Serial.println(sub);
         last_base_reading = sub;
         break;
   }
       
   // Set 2 seconds as max heart pluse duty
   if(sub>max_heartpluse_duty) {
      data_effect=0; //sign bit
      counter=0;
      //Serial.println("Heart rate measure error,test will restart!" );
      heart_rate = 0;
      arrayInit();
   }

   if (counter==20&&data_effect) {
      counter=0;
      sum();
   } else if(counter!=20&&data_effect) {
      counter++;
   } else { 
      counter=0;
      data_effect=1;
   }
}

/*Function: Initialization for the array(temp)*/
void arrayInit() {
   for(unsigned char i=0;i < 20;i ++) {
      temp[i]=0;
   }
   temp[20]=millis();
}
