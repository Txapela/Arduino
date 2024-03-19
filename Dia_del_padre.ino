int latchPin = 12; // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 13; // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 11; // Pin connected to DS of 74HC595（Pin14）
int LEDPin[] = {2, 3, 4, 5, 6, 7, 8, 9}; // column pin (cathode) of LED Matrix

// Define the pattern data for a smiling face
const int smilingFace[] = {
 0x00, 0x10, 0xD3, 0xFC, 0xFC, 0xD3, 0x10, 0x00
};

// Define the data of numbers and letters, and save them in flash area
const int data[] PROGMEM = {
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
 0x00, 0x00, 0x7F, 0x48, 0x48, 0x40, 0x00, 0x00, // "F"
 0x00, 0x00, 0x7F, 0x49, 0x49, 0x41, 0x00, 0x00, // "E"
 0x00, 0x00, 0x7F, 0x01, 0x01, 0x01, 0x00, 0x00, // "L"
 0x00, 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, // "I"
 0x00, 0x00, 0x43, 0x45, 0x49, 0x51, 0x61, 0x00, // "Z"
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
 0x00, 0x00, 0x7F, 0x41, 0x41, 0x3E, 0x00, 0x00, // "D"
 0x00, 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, // "I"
 0x00, 0x00, 0x3F, 0x44, 0x44, 0x3F, 0x00, 0x00, // "A"
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
 0x00, 0x00, 0x7F, 0x41, 0x41, 0x3E, 0x00, 0x00, // "D"
 0x00, 0x00, 0x7F, 0x49, 0x49, 0x41, 0x00, 0x00, // "E"
 0x00, 0x00, 0x7F, 0x01, 0x01, 0x01, 0x00, 0x00, // "L"
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "
 0x00, 0x00, 0x7F, 0x48, 0x48, 0x30, 0x00, 0x00, // "P"
 0x00, 0x00, 0x3F, 0x44, 0x44, 0x3F, 0x00, 0x00, // "A"
 0x00, 0x00, 0x7F, 0x41, 0x41, 0x3E, 0x00, 0x00, // "D"
 0x00, 0x00, 0x7F, 0x48, 0x4C, 0x33, 0x00, 0x00, // "R"
 0x00, 0x00, 0x7F, 0x49, 0x49, 0x41, 0x00, 0x00, // "E"
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // " "

};

void setup() {
 // set pins to output
 pinMode(latchPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(dataPin, OUTPUT);
 for (int i = 0; i < 8; i++) {
 pinMode(LEDPin[i], OUTPUT);
 }
 }

void loop() {
 // Define a one-byte variable (8 bits) which is used to represent the selected state of 8 columns.
 int cols;
 // Display the static smiling pattern
 for (int j = 0; j < 200; j++ ) { // repeat 200 times
 cols = 0x01; // Assign 0x01(binary 00000001) to the variable, which represents the first column is selected.
 for (int i = 0; i < 8; i++) { // display 8 column data by scanning
 matrixColsVal(cols); // select this column
 matrixRowsVal(smilingFace[i]);// display the data in this  column
 delay(1); // display them for a period of time
 matrixRowsVal(0x00); // clear the data of this column
 cols <<= 1; // shift "cols" 1 bit left to select the next column
 }
 }
 
 // Display the dynamic patterns of numbers and letters
 for (int i = 0; i < 160; i++) { // "space，0-9，A-F"16 letters ,each letter hold 8columns, total 136 columns. Firstly, display space ,then we need shift 128 times（136-8）
 for (int k = 0; k < 6; k++) { // repeat image of each frame 10 times.
 cols = 0x01; // Assign binary 00000001. Means the first column is selected.
 for (int j = i; j < 8 + i; j++) { // display image of each frame
 matrixColsVal(cols); // select this column
 matrixRowsVal(pgm_read_word_near(data + j));// display the data in this column
 delay(1); // display them for a period of time
 matrixRowsVal(0x00); // close the data of this column
 cols <<= 1; // shift "cols" 1 bit left to select the next column
 }
 }
 }
}

void matrixRowsVal(int value) {
 // make latchPin output low level
 digitalWrite(latchPin, LOW);
 // Send serial data to 74HC595
 shiftOut(dataPin, clockPin, LSBFIRST, value);
// make latchPin output high level, then 74HC595 will update the data to parallel output
 digitalWrite(latchPin, HIGH);
}

void matrixColsVal(byte value) {
 byte cols = 0x01;
 // Output the column data to the corresponding port.
 for (int i = 0; i < 8; i++) {
 digitalWrite(LEDPin[i], ((value & cols) == cols) ? LOW : HIGH);
 cols <<= 1;
 }
}
