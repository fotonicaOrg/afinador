#define LENGTH 256
#define FHT_N 256
#define LOG_OUT 1

#include "FHT.h"

int rawData[LENGTH];
uint8_t init_array[] = {255,0,0,0};


void float2bytes(float val, byte* bytes_array){

  union {
    float float_variable;
    byte temp_array[4];
    } u;

  u.float_variable = val;
  memcpy(bytes_array, u.temp_array, 4);
};

void floatarr2bytearr(float input[], int len, byte output[]){

  for (int i=0; i < len; i++){
      float2bytes(input[i], output[i*4]);
  };

};


int find_max(uint8_t input[], int len, int offset)
{
  int c;
  int location = offset;
  int maximum = input[offset];
 
  for (c = offset+1; c < len; c++)
  {
    if (input[c] > maximum)
    {
       maximum  = input[c];
       location = c+1;
    }
  }

  return location;
};


double compute_mean(int* input, int len)
{
  double mean;        // Computed mean value to be returned
  int i;           // Loop counter
  
  // Loop to compute mean
  mean = 0.0;
  for (i=0; i<len; i++)
    mean += ((double)input[i]) / len;

  return(mean);
};


void compute_autocorrelation(int* input, int len, double* output)
{ 
  for(int lag=0; lag < len; lag++)
    {
      double mean;
      mean = compute_mean(rawData, len);
    
      // Loop to compute autovariance
      output[lag] = 0.0;
      for (int i=0; i<(len-lag); i++)
        output[lag] += (((double) input[i]/mean - 1) * ((double) input[i+lag]/mean - 1));
        output[lag] = output[lag] / (len - lag);
    }
};




void setup(){
  analogRead(A0);
  Serial.begin(115200);
  
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {

    double autocorr[LENGTH];
    int max_idx;
    int freq_offset = 5;
    
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < LENGTH ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      rawData[i] = k; // put real data into bins
      fht_input[i] = k; // put real data into bins
    }

    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei();

//    compute_autocorrelation(rawData, LENGTH, autocorr);
    max_idx = find_max(fht_log_out, LENGTH/2, freq_offset);

//    Serial.println("Hola mundo");
//    Serial.println(max_idx);


//    for (int i=0; i<LENGTH/2; i++){
//      Serial.print(fht_log_out[i]);
//      Serial.print(",");
//    };
//    Serial.println("");
      Serial.println(max_idx);


//    byte outbyte[LENGTH*sizeof(float)];
//    floatarr2bytearr(autocorr, LENGTH, outbyte);
//    Serial.write(init_array, sizeof(init_array));
//    Serial.write(outbyte, sizeof(outbyte));

}
    
//    for(int lag=0; lag < LENGTH; lag++)
//      {
//        int      i;           // Loop counter
//
//        double mean;
//        mean = compute_mean(rawData);
//      
//        // Loop to compute autovariance
//        autocv[lag] = 0.0;
//        for (i=0; i<(LENGTH - lag); i++)
//          autocv[lag] += ((rawData[i] - mean) * (rawData[i+lag] - mean));
//        autocv[lag] = (1.0 / (LENGTH - lag)) * autocv[lag];
//      }
