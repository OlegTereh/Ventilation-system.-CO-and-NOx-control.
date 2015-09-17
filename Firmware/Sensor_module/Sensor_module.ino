//****SETTING*********************************************
#define DEBUG //need for a display service information, WITHOUT DEBUG program MORE faster!!!

//regulation values
int CO_reg = 512;
int NO_reg = 512;
int PU = 10; //hysteresis, determines point B(picture 1) relative to the fixed MIN, 10 ~= 0.065mm

//for smoothing
const int numReadings = 3; // numReadings UP = speed DOWN (1-5 is optimal for this program)
//********************************************************

int readings[2][numReadings];
int readIndex[2] = {0, 0};
int total[2] = {0, 0};
char first_calc_flag[2] = {0, 0};

#define PRE_PIN          8
#define VNOX_PIN         A0
#define VRED_PIN         A1

#define PRE_HEAT_SECONDS 10

int vnox_value = 0;
int vred_value = 0;

void setup() {
  
  // Setup preheater pin
  pinMode(PRE_PIN, OUTPUT);
  
  #ifdef DEBUG
  // Initialize serial port
  Serial.begin(9600);
  #endif

  analogReference(DEFAULT);
  // Wait for preheating (heating)
  digitalWrite(PRE_PIN, 1);
  delay(PRE_HEAT_SECONDS * 1000);
  digitalWrite(PRE_PIN, 0);
  Serial.println("Done");
}

int smooth(int AI, int num, char ch)
{
  int average;
  total[ch] = total[ch] - readings[ch][readIndex[ch]];
  readings[ch][readIndex[ch]] = AI;
  total[ch] = total[ch] + readings[ch][readIndex[ch]];
  readIndex[ch] = readIndex[ch] + 1;
  if (readIndex[ch] >= num)
  {
    first_calc_flag[ch] = 1;
    readIndex[ch] = 0;
  }
  if (first_calc_flag[ch])
  {
    average = total[ch] / num;
  }
  else average = 1111;

  return average;
}

void GasReg(int SuCO, int SuNO, int CO, int NO, int PU_v)
{
	
}

void loop() {
  
  // Read analog values, print them out, and wait
  vnox_value = smooth(analogRead(VNOX_PIN), numReadings, 0);  
  vred_value = smooth(analogRead(VRED_PIN), numReadings, 1); 
  
  #ifdef DEBUG
  Serial.print("Vnox: ");
  Serial.print(vnox_value, DEC);
  Serial.print(" Vred: ");
  Serial.println(vred_value, DEC);
  #endif
  
  if ((vnox_value != 1111)||(vnox_value != 1111)) 
  GasReg(CO_reg, NO_reg, vred_value, vnox_value, PU); 
  delay(100);
}
