/** MASTER **/
#include <Wire.h>
#include <Keypad.h>

// The slave address to which this master will send data
#define I2C_SLAVE_ADDRESS 0x09

// Hardware interrupt time (s)
#define INTERRUPT_TIME 5

// KEYPAD MACROS
const byte numRows = 4; // number of rows on the keypad
const byte numCols = 4; // number of columns on the keypad

// keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

// Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {9, 8, 7, 6}; // Rows 0 to 3
byte colPins[numCols] = {5, 4, 3, 2}; // Columns 0 to 3

// initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
char keypressed;

// TEMPERATURE ALARM MACROS
int REF_HIGH_TEMP = 24;
int aux_temp;
float temp;
int out;
float vout;
float vout1;
int LED = 13;
int piezo = 10;
int o1, o2, o3, o4;

volatile bool timerFlag = false;
bool temperature_set_flag = false;
bool first_digit_input = false;
bool second_digit_input = false;

void setup()
{
    Serial.begin(9600);
    pinMode(A1, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(piezo, OUTPUT);

    // Configure Timer1
    cli();      // Disable interrupts
    TCCR1B = 0; // Clear TCCR1B register
    TCNT1 = 0;  // Initialize counter value to 0

    // Calculate the compare match value for 5 seconds at prescaler 1024
    // The formula is: compareMatch = (clockFreq * interruptDuration) / prescaler - 1
    // Assuming a clock frequency of 16MHz
    uint16_t compareMatch = ((16000000UL * INTERRUPT_TIME) / 1024) - 1;

    OCR1A = compareMatch;                // Set compare match value
    TCCR1B |= (1 << WGM12);              // Set CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set prescaler to 1024
    TIMSK1 |= (1 << OCIE1A);             // Enable Timer1 compare interrupt

    Wire.begin();
    sei(); // Enable interrupts
}

bool isDigit(char keypressed){
    if (keypressed >= 48 && keypressed <= 57) {
        return true;
    }
    return false;
}

void loop()
{
    keypressed = myKeypad.getKey();
    if (keypressed == 'A')
    {
        Serial.println("Setting the ref temp");
        temperature_set_flag = false;
        while (temperature_set_flag == false)
        {
            first_digit_input = false;
            second_digit_input = false;
            while (first_digit_input == false)
            {
                keypressed = myKeypad.getKey();
                if (keypressed != NO_KEY && isDigit(keypressed))
                {
                    //transform from ascii to number
                    aux_temp = (keypressed - 48) * 10;
                    Serial.println(keypressed);
                    first_digit_input = true;
                }
            }

            while (second_digit_input == false)
            {
                keypressed = myKeypad.getKey();
                if (keypressed != NO_KEY && isDigit(keypressed))
                {
                    aux_temp = aux_temp + (keypressed - 48);
                    REF_HIGH_TEMP = aux_temp;
                    Serial.println(keypressed);
                    second_digit_input = true;
                }
            }

            Serial.print("Ref temperature set to: ");
            Serial.println(REF_HIGH_TEMP);
            temperature_set_flag = true;
        }
    }

    if (timerFlag)
    {
        timerFlag = false;

        vout = analogRead(A1);
        vout1 = (vout / 1023.0) * 5000;
        temp = (vout1 - 500) / 10.0;

        if (temp >= REF_HIGH_TEMP)
        {
            digitalWrite(LED, HIGH);
            digitalWrite(piezo, HIGH);
        }
        else
        {
            digitalWrite(LED, LOW);
            digitalWrite(piezo, LOW);
        }

        out = temp * 100;
        o1 = out / 1000;
        o2 = (out / 100) % 10;
        o3 = (out / 10) % 10;
        o4 = out % 10;

        Wire.beginTransmission(I2C_SLAVE_ADDRESS);
        Wire.write(o1);
        Wire.write(o2);
        Wire.write(o3);
        Wire.write(o4);
        Wire.endTransmission();
    }
}

// Timer1 compare match interrupt service routine
ISR(TIMER1_COMPA_vect)
{
    timerFlag = true;
}
