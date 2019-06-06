//  plays a MIDI note.
void noteOn(int pitch, int velocity) {
//  cmd = constrain(cmd, 127, 255);
  pitch = constrain(pitch, 0, 127);
  velocity = constrain(velocity, 0, 127);
  Serial1.write(0x90);
  Serial1.write(pitch);
  Serial1.write(velocity);
}

void noteOff(int pitch, int velocity){
  pitch = constrain(pitch, 0, 127);
  velocity = constrain(velocity, 0, 127);
  Serial1.write(0x80);
  Serial1.write(pitch);
  Serial1.write(velocity);
}

void MIDICommand(int statusByte, int data1Byte, int data2Byte){
  Serial1.write(statusByte);
  Serial1.write(data1Byte);
  Serial1.write(data2Byte);
}

void shortMIDICommand(int statusByte, int dataByte){
  Serial1.write(statusByte);
  Serial1.write(dataByte);
}

void changeInstrument(int funChannel, int funInstrumentDesignation){
  shortMIDICommand(0xC0 + funChannel, funInstrumentDesignation);
}

void setRangeLimits(){
  // Set the lower bound of which notes are playable
  int massagedRangeThreshold;
  int funAnalogValue = analogRead(controllerPinArrangement[5]);
  lowerThreshold = map(funAnalogValue, 0, 1023, 0, 115);
  // Set the range of notes that will be accessed. It must always
  // be at least one octave but never go above 127.
  funAnalogValue = analogRead(controllerPinArrangement[4]);
  massagedRangeThreshold = map(funAnalogValue, 0, 1023, 12, 127);
  upperThreshold = lowerThreshold + massagedRangeThreshold;
  upperThreshold = constrain(upperThreshold, lowerThreshold + 12, 127);
}

void setCenterWeightingVariable(){
  int funPinData = analogRead(controllerPinArrangement[6]);
  centerWeightingFactor = map(funPinData, 0, 1023, 0, numberOfCenterWeightingOptions + 0);
  centerWeightingFactor = constrain(centerWeightingFactor, 0, numberOfCenterWeightingOptions);
}

void setTempo(){
  if((millis() - MIDITempoActive[0]) < 10000){
    int funPinData = analogRead(controllerPinArrangement[7]);
    tempoFactor = map(funPinData, 0, 1023, -10, 11);
    tempoFactor = constrain(tempoFactor, -10, 10);
    if (tempoFactor < 0){
      mainTempo = MIDITempoActive[1] * (-1*tempoFactor);
    }else if(tempoFactor > 0){
      mainTempo = MIDITempoActive[1] / (tempoFactor);
    }else{
      mainTempo = MIDITempoActive[1];
    }
    // If the system is using TRNG, cap the mainTempo at 535 which
    // is 112 BPM
    if (TRNG && (mainTempo > 535)){
      mainTempo = 535;
    }
    mainBPM = 60000 / mainTempo;
//    printBPM();
  }else{
    // Analog pin 0 controls the tempo and is reported in BPM
    MIDISignalBlank = true;
    int funPinData = analogRead(controllerPinArrangement[7]);
    // If the system is using TRNG, cap the mainBPM at 112 BPM
    if (TRNG){
      mainBPM = map(funPinData, 0, 1023, 1, 112);
    }else{
      mainBPM = map(funPinData, 0, 1023, 1, 600);
    }
    mainTempo = (60000 / mainBPM);
  }
}

void changeScale(boolean funDelta){
  if (funDelta){
    scaleType++;
    if (scaleType >= numberOfScaleTypes){
      scaleType = 0;
    }
    if (scaleType < 0){
      scaleType = 0;
    }
  }else{
    scaleType--;
    if (scaleType < 0){
      scaleType = numberOfScaleTypes - 1;
    }
    if (scaleType > numberOfScaleTypes){
      scaleType = 0;
    }
  }
}

void resetCommand(){
  // Send the reset command.
  MIDICommand(0xB0, 0x79, 0x00);
  // Send an "all notes off" command to each channel.
  for (int i = 0; i < 16; i++){
    MIDICommand((0xB0 + i), 0x78, 0x00);
  }
  scaleType = 0;
  chordType = 0;
  mainNote = 60;
  // Look at each module for certin variables
  for (int i = 0; i < numberOfModules; i++){
    // Change all the pitch offset variables to zero.
    moduleSettings[i][1] = 0;
    // Reset all the chord variables to zero
    moduleSettings[i][2] = 0;
    // Reset all the play period variables to zero
    moduleSettings[i][3] = 0;
  }
  
  // Toggle the TRNG bit
  if (TRNG){
    TRNG = false;
  }else{
    TRNG = true;
  }
  
//  // Set the instrument to GM 0 "Acoustic Grand Piano"
//  changeInstrument(0x0, 0x00);
  if (TRNG){
    writeToAllScreens("  Reset: TRNG   ", "  Experimental  ");
  }else{
    writeToAllScreens("     Reset      ", "     Reset      ");
    randomSeed(Entropy.random(0, 4294967295));
  }
  boolean funSwitch0 = true;
  boolean funSwitch1 = true;
  while (funSwitch0 && funSwitch1){
    funSwitch0 = !digitalRead(controllerPinArrangement[0]);
    funSwitch1 = !digitalRead(controllerPinArrangement[3]);
  }
  for (int i=0; i<playListDimensions; i++){
    for (int k=0; k<100; k++){
      notePlayList[i][k] = 0;
    }
  }
  setTempo();
}
