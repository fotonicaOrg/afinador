//#include <FHT.h>
//#include "FHT.h"

#define LENGTH 512
#define FREQ 38500
#define FHT_N 512
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
      double mean = compute_mean(input, len);
            
      // Loop to compute autovariance
      output[lag] = 0.0;
      for (int i=0; i<(len-lag); i++)
      {
        output[lag] += (((double) input[i]/mean - 1) * ((double) input[i+lag]/mean - 1));
      }
      output[lag] = output[lag] / (len - lag);
      Serial.println(output[lag]);
    }
    Serial.println("Fin autocorr");
};


int detect_frequency(
                     int input[],
                     double threshold_ratio
                     )
{
    double corr[LENGTH];

    bool stop_condition = false;
    int max_idx = 0;
    double mean = compute_mean(input, LENGTH);

    // Iteración para distintos lags
    for(int lag=0; lag < LENGTH; lag++)
    {
        // Computa correlación para el lag actual
        for (int i=0; i<(LENGTH-lag); i++)
            corr[lag] += ((input[i]/mean - 1) * (input[i+lag]/mean - 1));
        corr[lag] = corr[lag] / (LENGTH - lag);

        // Detecta si el lag anterior es un pico
        if (lag > 1) {
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


int detect_frequency(
                     int* input,
                     double threshold_ratio,
                     double* corr
                     )
{
    bool stop_condition = false;
    int max_idx = 0;
    double mean = compute_mean(input, LENGTH);

    // Iteración para distintos lags
    for(int lag=0; lag < LENGTH; lag++)
    {
        // Computa correlación para el lag actual
        for (int i=0; i<(LENGTH-lag); i++)
            corr[lag] += (((double)input[i]/mean - 1) * ((double)input[i+lag]/mean - 1));
        corr[lag] = corr[lag] / (LENGTH - lag);

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

    double autocorr[LENGTH];
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

//    fht_window(); // window the data for better frequency response
//    fht_reorder(); // reorder the data before doing the fht
//    fht_run(); // process the data in the fht
//    fht_mag_log(); // take the output of the fht


//    Serial.println("Autocorr START");
    compute_autocorrelation(rawData, LENGTH, autocorr);
//    Serial.println("Autocorr OK");
//    max_idx = find_max(fht_log_out, LENGTH/2, freq_offset);

//    Serial.println("Hola mundo");
//    Serial.println(max_idx);

//    int idx = detect_frequency(rawData, 0.95, autocorr);

    for (int i=0; i<LENGTH; i++){
      Serial.print(autocorr[i]);
      Serial.print(",");
    };
    Serial.println("");
//    Serial.println(FREQ/idx);
//      Serial.println(max_idx);


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
