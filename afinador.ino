#define LENGTH 256 // Define el largo de la adquisición. 256 es lo maximo que se banca usando floats.
#define FREQ 38461 // Frecuencia de sampleo medida 
#define LOG_OUT 1

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
      float2bytes(input[i], &output[i*4]);
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


float compute_mean(int* input, int len)
{
  float mean;        // Computed mean value to be returned
  int i;           // Loop counter
  
  // Loop to compute mean
  mean = 0.0;
  for (i=0; i<len; i++)
    mean += ((float)input[i]) / len;

  return(mean);
};



int detect_frequency(
                     int* input,
                     float threshold_ratio,
                     float* corr,
                     int len
                     )
{
    bool stop_condition = false;
    int max_idx = 0;
    float Norm= compute_mean(input, len)/32786;

    // Iteración para distintos lags
    for(int lag=0; lag < len; lag++)
    {
        // Computa correlación para el lag actual
        for (int i=0; i<(len-lag); i++)
            corr[lag] += (((float)input[i]/32768 - Norm) * ((float)input[i+lag]/32736 - Norm));
        corr[lag] = corr[lag] / (len - lag);

        // Detecta si el lag anterior es un pico
        if (lag > 10) {
            if (corr[lag-1]-corr[lag-2] > 0 && corr[lag-1]-corr[lag] > 0)
            {
                // Se fija si el valor de pico es mayor a threshold_ratio * corr[0]
                if (corr[lag-1] > threshold_ratio * corr[0]) {
                    max_idx = lag-1;
                    stop_condition = true;
                }
            }
        }

        if (stop_condition) break;
    };

    return max_idx;
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

    float autocorr[LENGTH];
    int max_idx;
    
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
//      fht_input[i] = k; // put real data into bins
    }

//    

    int idx = detect_frequency(rawData, 0.98, autocorr,LENGTH);


    Serial.print(FREQ/idx);
    Serial.println("");


// COSAS VIEJAS COMENTADAS

//#include <FHT.h>
//#include "FHT.h"


//void compute_autocorrelation(int* input, int len, double* output)
//{ 
//  for(int lag=0; lag < len; lag++)
//    {
//      double mean = compute_mean(input, len);
//            
//      // Loop to compute autovariance
//      output[lag] = 0.0;
//      for (int i=0; i<(len-lag); i++)
//      {
//        output[lag] += (((double) input[i]/mean - 1) * ((double) input[i+lag]/mean - 1));
//      }
//      output[lag] = output[lag] / (len - lag);
//      //Serial.println(output[lag]);
//    }
//    //Serial.println("Fin autocorr");
//};
//
//
//int detect_frequency(
//                     int input[],
//                     double threshold_ratio
//                     )
//{
//    double corr[LENGTH];
//
//    bool stop_condition = false;
//    int max_idx = 0;
//    double mean = compute_mean(input, LENGTH);
//
//    // Iteración para distintos lags
//    for(int lag=0; lag < LENGTH; lag++)
//    {
//        // Computa correlación para el lag actual
//        for (int i=0; i<(LENGTH-lag); i++)
//            corr[lag] += ((input[i]/mean - 1) * (input[i+lag]/mean - 1));
//        corr[lag] = corr[lag] / (LENGTH - lag);
//
//        // Detecta si el lag anterior es un pico
//        if (lag > 1) {
//            if (corr[lag-1]-corr[lag-2] > 0 && corr[lag-1]-corr[lag] > 0)
//            {
//                // Se fija si el valor de pico es mayor a threshold_ratio * corr[0]
//                if (corr[lag-1] > threshold_ratio * corr[0]) {
//                    max_idx = lag-1;
//                    stop_condition = true;
//                }
//            }
//        }
//
//        if (stop_condition) break;
//    };
//
//    return max_idx;
//};


//fht_window(); // window the data for better frequency response
//    fht_reorder(); // reorder the data before doing the fht
//    fht_run(); // process the data in the fht
//    fht_mag_log(); // take the output of the fht


//    Serial.println("Autocorr START");
//    compute_autocorrelation(rawData, LENGTH, autocorr);
//    Serial.println("Autocorr OK");
//    max_idx = find_max(fht_log_out, LENGTH/2, freq_offset);

//    Serial.println("Hola mundo");
//    Serial.println(max_idx);

//    for (int i=0; i<LENGTH; i++){
//      Serial.print(autocorr[i]);
//      Serial.print(",");
//    };
//    Serial.println("");



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

//    sei();
//      }
