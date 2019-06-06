// The Entropy library provides true random numbers and can be obtained from:
// http://code.google.com/p/avr-hardware-random-number-generation/wiki/WikiAVRentropy
#include <Entropy.h>
boolean TRNG = false;

char welcomeText0[] = " 24HourEngineer ";
char welcomeText1[] = " Rev 20190429.A ";

int centerWeightIncrementor = 0;
const int centerWeightIncrementorMax = 20;
long centerWeightHistoryAverage = 0;
int centerWeightHistory[centerWeightIncrementorMax];

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd1(rs, (en - 0), d4, d5, d6, d7);
LiquidCrystal lcd2(rs, (en - 1), d4, d5, d6, d7);
LiquidCrystal lcd3(rs, (en - 2), d4, d5, d6, d7);
LiquidCrystal lcd4(rs, (en - 3), d4, d5, d6, d7);
LiquidCrystal lcd5(rs, (en - 4), d4, d5, d6, d7);
LiquidCrystal lcd6(rs, (en - 5), d4, d5, d6, d7);

int scaleType = 0;
const int numberOfScaleTypes = 6;
const int minorPentatonicScale[] = {-12, -9, -8, -5, -2, 0, 3, 4, 7, 10, 12};
const int majorPentatonicScale[] = {-12, -10, -8, -5, -3, 0, 2, 4, 7, 9, 12};
const int EgyptianPentatonicScale[] = {-12, -10, -7, -5, -2, 0, 2, 5, 7, 10, 12};
const int bluesMinorPentatonicScale[] = {-12, -9, -7, -4, -2, 0, 3, 5, 8, 10, 12};
const int bluesMajorPentatonicScale[] = {-12, -10, -7, -5, -3, 0, 2, 5, 7, 9, 12};
char scaleName[numberOfScaleTypes][17] = {
  {"  Minor Scale   "},
  {"  Major Scale   "},
  {" Egyptian Scale "},
  {"  Blues Minor   "},
  {"  Blues Major   "},
  {" Repeating Note "}
};

const char instrument_000[] PROGMEM = "Acou Grand Piano";
const char instrument_001[] PROGMEM = " Brt Acou Piano ";
const char instrument_002[] PROGMEM = "Elec Grand Piano";
const char instrument_003[] PROGMEM = "Honky-tonk Piano";
const char instrument_004[] PROGMEM = "Electric Piano 1";
const char instrument_005[] PROGMEM = "Electric Piano 2";
const char instrument_006[] PROGMEM = "  Harpsichord   ";
const char instrument_007[] PROGMEM = "    Clavinet    ";
const char instrument_008[] PROGMEM = "    Celesta     ";
const char instrument_009[] PROGMEM = "  Glockenspiel  ";
const char instrument_010[] PROGMEM = "   Music Box    ";
const char instrument_011[] PROGMEM = "   Vibraphone   ";
const char instrument_012[] PROGMEM = "    Marimba     ";
const char instrument_013[] PROGMEM = "   Xylophone    ";
const char instrument_014[] PROGMEM = " Tubular Bells  ";
const char instrument_015[] PROGMEM = "    Dulcimer    ";
const char instrument_016[] PROGMEM = " Drawbar Organ  ";
const char instrument_017[] PROGMEM = "Percussive Organ";
const char instrument_018[] PROGMEM = "   Rock Organ   ";
const char instrument_019[] PROGMEM = "  Church Organ  ";
const char instrument_020[] PROGMEM = "   Reed Organ   ";
const char instrument_021[] PROGMEM = "   Accordion    ";
const char instrument_022[] PROGMEM = "   Harmonica    ";
const char instrument_023[] PROGMEM = "Tango Accordion ";
const char instrument_024[] PROGMEM = "Ac Guitar(Nylon)";
const char instrument_025[] PROGMEM = "Ac Guitar(Steel)";
const char instrument_026[] PROGMEM = "Elc Guitar(Jazz)";
const char instrument_027[] PROGMEM = "Ec Guitar(Clean)";
const char instrument_028[] PROGMEM = "Ec Guitar(Muted)";
const char instrument_029[] PROGMEM = "Overdrive Guitar";
const char instrument_030[] PROGMEM = "  Dist Guitar   ";
const char instrument_031[] PROGMEM = "Guitar Harmonics";
const char instrument_032[] PROGMEM = " Acoustic Bass  ";
const char instrument_033[] PROGMEM = "Ec Bass (Finger)";
const char instrument_034[] PROGMEM = " Ec Bass (Pick) ";
const char instrument_035[] PROGMEM = " Fretless Bass  ";
const char instrument_036[] PROGMEM = "  Slap Bass 1   ";
const char instrument_037[] PROGMEM = "  Slap Bass 2   ";
const char instrument_038[] PROGMEM = "  Synth Bass 1  ";
const char instrument_039[] PROGMEM = "  Synth Bass 2  ";
const char instrument_040[] PROGMEM = "     Violin     ";
const char instrument_041[] PROGMEM = "     Viola      ";
const char instrument_042[] PROGMEM = "     Cello      ";
const char instrument_043[] PROGMEM = "   Contrabass   ";
const char instrument_044[] PROGMEM = "Tremolo Strings ";
const char instrument_045[] PROGMEM = "Pizzicato Stngs ";
const char instrument_046[] PROGMEM = "Orchestral Harp ";
const char instrument_047[] PROGMEM = "    Timpani     ";
const char instrument_048[] PROGMEM = "String Ensemble1";
const char instrument_049[] PROGMEM = "String Ensemble2";
const char instrument_050[] PROGMEM = "Synth Strings 1 ";
const char instrument_051[] PROGMEM = "Synth Strings 2 ";
const char instrument_052[] PROGMEM = "   Choir Aahs   ";
const char instrument_053[] PROGMEM = "   Voice Oohs   ";
const char instrument_054[] PROGMEM = "  Synth Choir   ";
const char instrument_055[] PROGMEM = " Orchestra Hit  ";
const char instrument_056[] PROGMEM = "    Trumpet     ";
const char instrument_057[] PROGMEM = "    Trombone    ";
const char instrument_058[] PROGMEM = "      Tuba      ";
const char instrument_059[] PROGMEM = " Muted Trumpet  ";
const char instrument_060[] PROGMEM = "  French Horn   ";
const char instrument_061[] PROGMEM = " Brass Section  ";
const char instrument_062[] PROGMEM = " Synth Brass 1  ";
const char instrument_063[] PROGMEM = " Synth Brass 2  ";
const char instrument_064[] PROGMEM = "  Soprano Sax   ";
const char instrument_065[] PROGMEM = "    Alto Sax    ";
const char instrument_066[] PROGMEM = "   Tenor Sax    ";
const char instrument_067[] PROGMEM = "  Baritone Sax  ";
const char instrument_068[] PROGMEM = "      Oboe      ";
const char instrument_069[] PROGMEM = "  English Horn  ";
const char instrument_070[] PROGMEM = "    Bassoon     ";
const char instrument_071[] PROGMEM = "    Clarinet    ";
const char instrument_072[] PROGMEM = "    Piccolo     ";
const char instrument_073[] PROGMEM = "     Flute      ";
const char instrument_074[] PROGMEM = "    Recorder    ";
const char instrument_075[] PROGMEM = "   Pan Flute    ";
const char instrument_076[] PROGMEM = "  Blown bottle  ";
const char instrument_077[] PROGMEM = "   Shakuhachi   ";
const char instrument_078[] PROGMEM = "    Whistle     ";
const char instrument_079[] PROGMEM = "    Ocarina     ";
const char instrument_080[] PROGMEM = "Lead 1 (Square) ";
const char instrument_081[] PROGMEM = "Lead 2(Sawtooth)";
const char instrument_082[] PROGMEM = "Lead 3(Calliope)";
const char instrument_083[] PROGMEM = " Lead 4 (Chiff) ";
const char instrument_084[] PROGMEM = "Lead 5 (Charang)";
const char instrument_085[] PROGMEM = " Lead 6 (Voice) ";
const char instrument_086[] PROGMEM = "Lead 7 (Fifths) ";
const char instrument_087[] PROGMEM = "Lead8(Bass+Lead)";
const char instrument_088[] PROGMEM = "Pad 1 (New Age) ";
const char instrument_089[] PROGMEM = "  Pad 2 (Warm)  ";
const char instrument_090[] PROGMEM = "Pad 3(Polysynth)";
const char instrument_091[] PROGMEM = " Pad 4 (Choir)  ";
const char instrument_092[] PROGMEM = " Pad 5 (Bowed)  ";
const char instrument_093[] PROGMEM = "Pad 6 (Metallic)";
const char instrument_094[] PROGMEM = "  Pad 7 (Halo)  ";
const char instrument_095[] PROGMEM = " Pad 8 (Sweep)  ";
const char instrument_096[] PROGMEM = "  FX 1 (Rain)   ";
const char instrument_097[] PROGMEM = "FX 2(Soundtrack)";
const char instrument_098[] PROGMEM = " FX 3 (Crystal) ";
const char instrument_099[] PROGMEM = "FX 4(Atmosphere)";
const char instrument_100[] PROGMEM = "FX 5(Brightness)";
const char instrument_101[] PROGMEM = " FX 6 (Goblins) ";
const char instrument_102[] PROGMEM = " FX 7 (Echoes)  ";
const char instrument_103[] PROGMEM = " FX 8 (Sci-fi)  ";
const char instrument_104[] PROGMEM = "     Sitar      ";
const char instrument_105[] PROGMEM = "     Banjo      ";
const char instrument_106[] PROGMEM = "    Shamisen    ";
const char instrument_107[] PROGMEM = "      Koto      ";
const char instrument_108[] PROGMEM = "    Kalimba     ";
const char instrument_109[] PROGMEM = "    Bagpipe     ";
const char instrument_110[] PROGMEM = "     Fiddle     ";
const char instrument_111[] PROGMEM = "     Shanai     ";
const char instrument_112[] PROGMEM = "  Tinkle Bell   ";
const char instrument_113[] PROGMEM = "     Agogo      ";
const char instrument_114[] PROGMEM = "  Steel Drums   ";
const char instrument_115[] PROGMEM = "   Woodblock    ";
const char instrument_116[] PROGMEM = "   Taiko Drum   ";
const char instrument_117[] PROGMEM = "  Melodic Tom   ";
const char instrument_118[] PROGMEM = "   Synth Drum   ";
const char instrument_119[] PROGMEM = " Reverse Cymbal ";
const char instrument_120[] PROGMEM = "  Guitar Fret   ";
const char instrument_121[] PROGMEM = "  Breath Noise  ";
const char instrument_122[] PROGMEM = "    Seashore    ";
const char instrument_123[] PROGMEM = "   Bird Tweet   ";
const char instrument_124[] PROGMEM = " Telephone Ring ";
const char instrument_125[] PROGMEM = "   Helicopter   ";
const char instrument_126[] PROGMEM = "    Applause    ";
const char instrument_127[] PROGMEM = "    Gunshot     ";

const char *const MIDIList[128] PROGMEM = {
  instrument_000,
  instrument_001,
  instrument_002,
  instrument_003,
  instrument_004,
  instrument_005,
  instrument_006,
  instrument_007,
  instrument_008,
  instrument_009,
  instrument_010,
  instrument_011,
  instrument_012,
  instrument_013,
  instrument_014,
  instrument_015,
  instrument_016,
  instrument_017,
  instrument_018,
  instrument_019,
  instrument_020,
  instrument_021,
  instrument_022,
  instrument_023,
  instrument_024,
  instrument_025,
  instrument_026,
  instrument_027,
  instrument_028,
  instrument_029,
  instrument_030,
  instrument_031,
  instrument_032,
  instrument_033,
  instrument_034,
  instrument_035,
  instrument_036,
  instrument_037,
  instrument_038,
  instrument_039,
  instrument_040,
  instrument_041,
  instrument_042,
  instrument_043,
  instrument_044,
  instrument_045,
  instrument_046,
  instrument_047,
  instrument_048,
  instrument_049,
  instrument_050,
  instrument_051,
  instrument_052,
  instrument_053,
  instrument_054,
  instrument_055,
  instrument_056,
  instrument_057,
  instrument_058,
  instrument_059,
  instrument_060,
  instrument_061,
  instrument_062,
  instrument_063,
  instrument_064,
  instrument_065,
  instrument_066,
  instrument_067,
  instrument_068,
  instrument_069,
  instrument_070,
  instrument_071,
  instrument_072,
  instrument_073,
  instrument_074,
  instrument_075,
  instrument_076,
  instrument_077,
  instrument_078,
  instrument_079,
  instrument_080,
  instrument_081,
  instrument_082,
  instrument_083,
  instrument_084,
  instrument_085,
  instrument_086,
  instrument_087,
  instrument_088,
  instrument_089,
  instrument_090,
  instrument_091,
  instrument_092,
  instrument_093,
  instrument_094,
  instrument_095,
  instrument_096,
  instrument_097,
  instrument_098,
  instrument_099,
  instrument_100,
  instrument_101,
  instrument_102,
  instrument_103,
  instrument_104,
  instrument_105,
  instrument_106,
  instrument_107,
  instrument_108,
  instrument_109,
  instrument_110,
  instrument_111,
  instrument_112,
  instrument_113,
  instrument_114,
  instrument_115,
  instrument_116,
  instrument_117,
  instrument_118,
  instrument_119,
  instrument_120,
  instrument_121,
  instrument_122,
  instrument_123,
  instrument_124,
  instrument_125,
  instrument_126,
  instrument_127
};
char buffer[16];

int chordType = 0;
const int numberOfChordTypes = 14;
int chordValuesTable[numberOfChordTypes][4] = {
  {0, 0, 0, 0},
  {0, 4, 7, 0},
  {0, 4, 7, 9},
  {0, 4, 7, 10},
  {0, 4, 7, 11},
  {0, 4, 8, 0},
  {0, 4, 8, 10},
  {0, 3, 7, 0},
  {0, 3, 7, 9},
  {0, 3, 7, 10},
  {0, 3, 7, 11},
  {0, 3, 6, 0},
  {0, 3, 6, 9},
  {0, 3, 6, 10}
};
char chordList[numberOfChordTypes][16] = {
  {"   No chords    "},
  {"  Major Triad   "},
  {"  Major Sixth   "},
  {"Dominant Seventh"},
  {" Major Seventh  "},
  {"Augmented Triad "},
  {" Augmented 7th  "},
  {"  Minor Triad   "},
  {"  Minor Sixth   "},
  {"  Minor Seventh "},
  {"Minor Major 7th "},
  {"Diminished Triad"},
  {" Diminished 7th "},
  {"1/2 Diminished 7"}
};


int noteDelta = 0;
int timeDivision = 16;
int mainNote = 60;
int mainVelocity = 0x45;
int mainTempo = 500;
int mainBPM = 60000 / mainTempo;
int tempoFactor = 0;
int centerWeightingFactor = 0;
const int numberOfCenterWeightingOptions = 6;

int activeButton = 999;
int activePot = 999;
int activeModule = 999;
int analogJitterTolerance = 5;
long nextNoteIteration = 0;
int upperThreshold = 90;
int lowerThreshold = 20;

const int discreteControllerInputs = 4;
const int analogControllerInputs = 4;
const int controllerInputs = discreteControllerInputs + analogControllerInputs;
const int numberOfModules = 5;
const int discreteInputsPerModule = 6;
const int analogInputsPerModule = 2;
const int inputsPerModule = analogInputsPerModule + discreteInputsPerModule;
// incomingMIDINote contains:
// [0] Command and channel
// [1] Note
// [2] Velocity
// [3] Time it arrived
long incomingMIDINote[4];
int incomingMIDICounter = 0;
boolean waitForNewSerialData = LOW;
// MIDITempoActive
// [0] Time from millis() when a serial signal was last received
// [1] Difference in milliseconds since last recognized note
// [2] Last time a recognizable note was received
// [3] Recognized note
long MIDITempoActive[4]={10000, 0, 0};
boolean MIDISignalBlank = true;

// {Scale--, Scale++, Channel--, Channel++, Note range (analog), Lower limit (analog), Weighting (analog), Tempo (analog)}
int controllerPinArrangement[8] = {14, 15, 17, 16, 2, 3, 0, 1};
// {Instrument--, Instrument++, Pitch offset--, Pitch offset++, Chords, Play period, Time offset (analog), Velocity (analog)}
int modulePinArrangement[numberOfModules][8] = {
  {26, 25, 24, 27, 22, 23, 4, 5},
  {30, 31, 32, 33, 28, 29, 6, 7},
  {36, 37, 38, 39, 34, 35, 8, 9},
  {43, 42, 44, 45, 40, 41, 10, 11},
  {48, 49, 50, 51, 46, 47, 12, 13},
};
int controllerPinNewValues[8];
int controllerPinOldValues[8];
int modulePinNewValues[numberOfModules][8];
int modulePinOldValues[numberOfModules][8];
// moduleSettings will hold the values for each setting contained
// in a module. 
// Instrument, pitch offset, chords, play period, time offset, velocity
int moduleSettings[numberOfModules][6] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0}
};
int greatestRangeInPitch = 115;
// In notePlayList:
// The first dimension  [0][x] is the NOTE to be played
// The second dimension [1][x] is the VELOCITY. Zero for off.
// The third dimension  [2][x] is the TIME to execute.
// The fourth dimension [3][x] is the INSTRUMENT.
// The fifth dimension  [4][x] is the CHANNEL.
// The sixth dimension  [5][x] is the duration in mS.
const int playListDimensions = 6;
long notePlayList[playListDimensions][100];
const int leastPlayableFrequency = 10;
int notePlayingNow = 0;

void setup(){
  // Set debugging serial port for USB terminal
//  Serial.begin(115200);
  Serial.begin(9600);

  //  Set MIDI baud rate
  // Since Arduino MEGA has multiple serial ports
  // there is no reason to disconnect 
  // the MIDI hardware when programming.
  Serial1.begin(31250);
  
  // Pin 13 is the onboard LED. It doesn't work well as an INPUT_PULLUP.
  pinMode(13, OUTPUT);
  
  // Initialize the Entropy library to ensure that the draws are truely random.
  Entropy.initialize();
  
//  // Seed the random() function with a value from the entropy library
  randomSeed(Entropy.random(0, 4294967295));

  // set up the LCD's number of columns and rows:
  lcd1.begin(16, 2);
  lcd2.begin(16, 2);
  lcd3.begin(16, 2);
  lcd4.begin(16, 2);
  lcd5.begin(16, 2);
  lcd6.begin(16, 2);
  // Print the module name to the LCDs.
  writeToAllScreens(welcomeText0, welcomeText1);
  // Print the module name to the LCDs.
  lcd1.print("  Main control  ");
  lcd2.print("  Channel one   ");
  lcd3.print("  Channel two   ");
  lcd4.print("  Channel three ");
  lcd5.print("  Channel four  ");
  lcd6.print("  Channel five  ");
  
  // Send an "all notes off" command to each channel.
  for (int i = 0; i < 16; i++){
    MIDICommand((0xB0 + i), 0x78, 0x00);
  }
  checkDiscreteInputs();
  activeModule = 999;
  checkAnalogInputs();
  activePot = 999;
  setRangeLimits();
  setTempo();
  for (int i=0; i<playListDimensions; i++){
    for (int k=0; k<100; k++){
      notePlayList[i][k] = 0;
    }
  }
  for (int i=0; i<numberOfModules; i++){
    for (int k=0; k<6; k++){
      moduleSettings[i][k] = 0;
    }
  }
}

void loop() {
  // Check for activity on the MIDI in serial port.
  readMIDI();
  // Check for changes in the inputs. Buttons first and analog second.
  // These functions will return 999 if there is no change otherwise
  // the number returned will be the the pin number. If the input
  // has been activated, it will be returned plus exactly 100. If it
  // has been deactivated, it will be returned without anything added.
  activeButton = checkDiscreteInputs();
  activePot = checkAnalogInputs();

  // Act if there are any changes in the buttons.
  if (activeModule != 999 && activeButton != 999){
    switch(activeModule){
      case 0:
        updateController(activeButton);
        break;
      case 1:
        updateModule1(activeButton);
        break;
      case 2:
        updateModule2(activeButton);
        break;
      case 3:
        updateModule3(activeButton);
        break;
      case 4:
        updateModule4(activeButton);
        break;
      case 5:
        updateModule5(activeButton);
        break;
      default:
        break;
    }
  }
  // Act if there are any changes in the potentiometers.
  if (activeModule != 999 && activePot != 999){
    switch(activeModule){
      case 0:
        updateController(activePot);
        break;
      case 1:
        updateModule1(activePot);
        break;
      case 2:
        updateModule2(activePot);
        break;
      case 3:
        updateModule3(activePot);
        break;
      case 4:
        updateModule4(activePot);
        break;
      case 5:
        updateModule5(activePot);
        break;
      default:
        break;
    }
  }

  if (millis() >= nextNoteIteration){
    generateNewNote();
    nextNoteIteration = millis() + mainTempo;
    pollModules();
  }else{
    // Check each point in the notePlayList array.
    for (int i=0; i<100; i++){
      // When one of the timers has expired
      if ((millis() >= notePlayList[2][i]) && (notePlayList[0][i] != 0)){
        // Play the note, or turn it off
        if (notePlayList[1][i] > 0){
//          Serial.print("Playing:\t");
//          for (int k=0; k<playListDimensions; k++){
//            Serial.print(notePlayList[k][i]);
//            Serial.print("\t");
//          }
//          Serial.print("\n");
//          changeInstrument(notePlayList[3][i]);
          MIDICommand(0x90 + notePlayList[4][i], notePlayList[0][i], notePlayList[1][i]);
          notePlayList[2][i] = millis() + notePlayList[5][i];
          notePlayList[1][i] = 0;
        }else{
//          Serial.print("Turn off:\t");
//          for (int k=0; k<playListDimensions; k++){
//            Serial.print(notePlayList[k][i]);
//            Serial.print("\t");
//          }
//          Serial.print("\n");
//          changeInstrument(notePlayList[3][i]);
          MIDICommand(0x80 + notePlayList[4][i], notePlayList[0][i], notePlayList[1][i]);
          for (int k=0; k<playListDimensions; k++){
            notePlayList[k][i] = 0;
          }
        }
      }
    }
  }
}













// 24 Hour Engineer
