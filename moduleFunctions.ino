int desiredNoteCalculator(int funModule, int chordLevel){
  int addPitchOffset = moduleSettings[funModule][1];
  int addChordLevel = chordValuesTable[moduleSettings[funModule][2]][chordLevel];
  // moduleSettings[i][1] + moduleSettings[i][2]
  return (mainNote + addPitchOffset + addChordLevel);
}

// Each time a new prandom note is generated, the modules are polled
// so they can arrange the notes they want to play based on the
// new prandom note.
void pollModules(){
  int desiredInstrument;
  int desiredNotes[4];
  long desiredTime;
  int desiredVelocity;
  for (int i=0; i<numberOfModules; i++){
    // If the velocity, moduleSettings[x][5], is not zero
    // the modules wants to play a note.
    // Record the note(s) to be played in the notePlayList array based
    // on the module settings.
    if ((moduleSettings[i][5] != 0) && ((notePlayingNow % moduleSettings[i][3] == 0) || (moduleSettings[i][3] > leastPlayableFrequency))){
      if (moduleSettings[i][3] > leastPlayableFrequency){
        int percentChanceToPlay = (moduleSettings[i][3] - leastPlayableFrequency);
        int prandomChanceToPlay = 0;
        if (TRNG){
          prandomChanceToPlay = Entropy.random(0, leastPlayableFrequency);
        }else{
          prandomChanceToPlay = random(0, leastPlayableFrequency);
        }
        if (percentChanceToPlay > prandomChanceToPlay){
          desiredInstrument = moduleSettings[i][0];
          desiredNotes[0] = desiredNoteCalculator(i, 0);
          desiredNotes[1] = desiredNoteCalculator(i, 1);
          desiredNotes[2] = desiredNoteCalculator(i, 2);
          desiredNotes[3] = desiredNoteCalculator(i, 3);
          desiredTime = millis() + moduleSettings[i][4];
          desiredVelocity = moduleSettings[i][5];
        }else{
//          desiredInstrument = moduleSettings[i][0];
//          desiredNotes[0] = desiredNoteCalculator(i, 0);
//          desiredNotes[1] = desiredNoteCalculator(i, 1);
//          desiredNotes[2] = desiredNoteCalculator(i, 2);
//          desiredNotes[3] = desiredNoteCalculator(i, 3);
//          desiredTime = millis();
          desiredVelocity = 0;
        }
      }else{
        desiredInstrument = moduleSettings[i][0];
        desiredNotes[0] = desiredNoteCalculator(i, 0);
        desiredNotes[1] = desiredNoteCalculator(i, 1);
        desiredNotes[2] = desiredNoteCalculator(i, 2);
        desiredNotes[3] = desiredNoteCalculator(i, 3);
        desiredTime = millis() + moduleSettings[i][4];
        desiredVelocity = moduleSettings[i][5];
      }
      // Find the first available spots in the notePlayList array
      for(int k=0; k<4; k++){
        if (desiredNotes[k] != 0){
          int m = 0;
          while (m < 100){
            if(notePlayList[2][m] == 0){
              notePlayList[0][m] = desiredNotes[k];
              notePlayList[1][m] = desiredVelocity;
              notePlayList[2][m] = desiredTime;
              notePlayList[3][m] = desiredInstrument;
              notePlayList[4][m] = i;
              notePlayList[5][m] = mainTempo - 5;
//              Serial.print("M: ");
//              Serial.println(m);
              m = 100;
            }
            m++;
          }
        }
      }
    }
  }
  notePlayingNow++;
  // 5040 is wholely divisible by all the natural numbers below 11.
  if (notePlayingNow > 5040){
    notePlayingNow = 1;
  }
}

void updateController(int funPin){
  // Over 200, means the change happened on an analog input
  if (funPin >= 200){
    // Pins two and three set the limits
    if (funPin==202 || funPin==203){
      setRangeLimits();
      lcd1.setCursor(0, 0);
      lcd1.print("Lower      Upper");
      lcd1.setCursor(0, 1);
      lcd1.print(lowerThreshold);
      lcd1.print("               ");
      lcd1.setCursor(12, 1);
      lcd1.print(upperThreshold);
      lcd1.print("   ");
    }else if(funPin == 201){
      setTempo();
      lcd1.setCursor(0, 0);
      lcd1.print("      Tempo     ");
      lcd1.setCursor(0, 1);
      lcd1.print(mainBPM);
      lcd1.print("               ");
      lcd1.setCursor(4, 1);
      lcd1.print("BPM");
      if((millis() - MIDITempoActive[0]) < 10000){
        lcd1.setCursor(12, 1);
        if (tempoFactor < 0){
          lcd1.print("1/");
          lcd1.print(-1*tempoFactor);
        }else if(tempoFactor > 0){
          lcd1.print(tempoFactor);
          lcd1.print("/1");
        }else{
          lcd1.print("1/1");
        }
      }else{
        lcd1.setCursor(13, 1);
        lcd1.print("   ");
      }
    }else if(funPin == 200){
      setCenterWeightingVariable();
      lcd1.setCursor(0, 0);
      lcd1.print("Center Weight: ");
      lcd1.print(centerWeightingFactor);
      lcd1.setCursor(0, 1);
      switch (centerWeightingFactor){
        case 0:
          lcd1.print("   Even Dist.   ");
          break;
        case 1:
          lcd1.print("+1 Center Weight");
          break;
        case 2:
          lcd1.print("+2 Center Weight");
          break;
        case 3:
          lcd1.print("+3 Center Weight");
          break;
        case 4:
          lcd1.print("+4 Center Weight");
          break;
        case 5:
          lcd1.print("+5 Center Weight");
          break;
      }
    }else{
      lcd1.setCursor(0, 0);
      lcd1.print(" Analog input:  ");
      lcd1.setCursor(0, 1);
      lcd1.print("Pot ");
      lcd1.print(funPin - 200);
      lcd1.print(" : ");
      lcd1.print(analogRead(funPin - 200));
      lcd1.print("        ");
    }
  }else if (funPin >= 100){
    if (funPin == 114 || funPin == 116){
      boolean funSwitch0 = !digitalRead(controllerPinArrangement[0]);
      boolean funSwitch1 = !digitalRead(controllerPinArrangement[3]);
      if (funSwitch0 && funSwitch1){
        resetCommand();
      }
    }
    if (funPin == 115){
      // Increment the style of scale to use.
      changeScale(1);
      Serial.println(scaleName[scaleType]);
      lcd1.setCursor(0, 0);
      lcd1.print(scaleName[scaleType]);
      lcd1.setCursor(0, 1);
      lcd1.print("                ");
    }
    if (funPin == 114){
      // Decrement the style of scale to use.
      changeScale(0);
      Serial.println(scaleName[scaleType]);
      lcd1.setCursor(0, 0);
      lcd1.print(scaleName[scaleType]);
      lcd1.setCursor(0, 1);
      lcd1.print("                ");
    }
//    lcd1.setCursor(0, 0);
//    lcd1.print(" Digital input: ");
//    lcd1.setCursor(0, 1);
//    lcd1.print(funPin - 100);
//    lcd1.print("                ");
  }else{
//    lcd1.setCursor(0, 0);
//    lcd1.print(" Control module ");
//    lcd1.setCursor(0, 1);
//    lcd1.print("               ");
  }
}

void updateModule1(int funPin){
  int currentModuleNumber = 0;
  // Two hundred or more indicates a change on an analog
  if (funPin >= 200){
    // 206 is the timeOffset moduleSettings[4][x] potentiometer
    if (funPin == 206){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][6]);
      moduleSettings[currentModuleNumber][4] = map(incomingTimeOffset, 0, 1023, 0, mainTempo);
      lcd2.setCursor(0, 0);
      lcd2.print("  Time Offset   ");
      lcd2.setCursor(0, 1);
      lcd2.print(moduleSettings[currentModuleNumber][4]);
      lcd2.print("   ");
      lcd2.setCursor(5, 1);
      lcd2.print("mS         ");
    }
    // 207 is the velocity moduleSettings[5][x] potentiometer
    if (funPin == 207){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][7]);
      moduleSettings[currentModuleNumber][5] = map(incomingTimeOffset, 0, 1023, 0, 127);
      lcd2.setCursor(0, 0);
      lcd2.print("    Velocity    ");
      lcd2.setCursor(0, 1);
      lcd2.print(moduleSettings[currentModuleNumber][5]);
      lcd2.print("   ");
      lcd2.setCursor(5, 1);
      lcd2.print(" / 127     ");
    }
  // One hundred or more indicates a discrete rising edge
  }else if (funPin >= 100){
    // 100 is the pin for decrementing the instrument variable
    if (funPin == 100){
      moduleSettings[currentModuleNumber][0]--;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd2.setCursor(0, 0);
      lcd2.print("Instrument  ");
      lcd2.print(moduleSettings[currentModuleNumber][0]);
      lcd2.print("   ");
      lcd2.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd2.print(buffer);
    }
    // 101 is the pin for incrementing the instrument moduleSettings[x][0] variable
    if (funPin == 101){
      moduleSettings[currentModuleNumber][0]++;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd2.setCursor(0, 0);
      lcd2.print("Instrument  ");
      lcd2.print(moduleSettings[currentModuleNumber][0]);
      lcd2.print("   ");
      lcd2.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd2.print(buffer);
    }
    // 102 is the pin for decrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 102){
      moduleSettings[currentModuleNumber][1]--;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = (-1 * greatestRangeInPitch);
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd2.setCursor(0, 0);
      lcd2.print(" Pitch Offset-- ");
      lcd2.setCursor(0, 1);
      lcd2.print(moduleSettings[currentModuleNumber][1]);
      lcd2.print("               ");
    }
    // 103 is the pin for incrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 103){
      moduleSettings[currentModuleNumber][1]++;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = -1 * greatestRangeInPitch;
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd2.setCursor(0, 0);
      lcd2.print(" Pitch Offset++ ");
      lcd2.setCursor(0, 1);
      lcd2.print(moduleSettings[currentModuleNumber][1]);
      lcd2.print("               ");
    }
    // 104 sets the value for the chords
    if (funPin == 104){
      moduleSettings[currentModuleNumber][2]++;
      if(moduleSettings[currentModuleNumber][2] >= numberOfChordTypes){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      if(moduleSettings[currentModuleNumber][2] < 0){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      lcd2.setCursor(0, 0);
      lcd2.print("  Chord change  ");
      lcd2.setCursor(0, 1);
      for (int i=0; i<16; i++){
        lcd2.print(chordList[moduleSettings[currentModuleNumber][2]][i]);
      }
    }
    // 105 is the button for changing how often a module plays a note
    // which is being called "Play Period".
    // The setting in moduleSettings[currentModuleNumber][3] determines
    // if every note, every other note, every third note, every fourth
    // note, etc, is played.
    if (funPin == 105){
      moduleSettings[currentModuleNumber][3]++;
      if(moduleSettings[currentModuleNumber][3] > leastPlayableFrequency + 9){
        moduleSettings[currentModuleNumber][3] = 0;
      }
      if(moduleSettings[currentModuleNumber][3] < 0){
        moduleSettings[currentModuleNumber][3] = leastPlayableFrequency;
        Serial.println("Playable notes is deccrementing. Oops.");
      }
      lcd2.setCursor(0, 0);
      lcd2.print(" Playable notes ");
      lcd2.setCursor(0, 1);
      lcd2.print("Every ");
      switch (moduleSettings[currentModuleNumber][3]){
        case 0:
          lcd2.setCursor(0, 1);
          lcd2.print(" Nothing plays  ");
          break;
        case 1:
          lcd2.print("note plays");
          break;
        case 2:
          lcd2.print("other note");
          break;
        case 3:
          lcd2.print("third note");
          break;
        case 4:
          lcd2.print("fourth    ");
          break;
        case 5:
          lcd2.print("fifth note");
          break;
        case 6:
          lcd2.print("sixth note");
          break;
        case 7:
          lcd2.print("seventh   ");
          break;
        case 8:
          lcd2.print("eighth    ");
          break;
        case 9:
          lcd2.print("nineth    ");
          break;
        case 10:
          lcd2.print("tenth note");
          break;
        default:
          lcd2.setCursor(0, 1);
          int percentChanceToPlay = (moduleSettings[currentModuleNumber][3] - leastPlayableFrequency);
          lcd2.print(percentChanceToPlay * 10);
          lcd2.print("% at prandom  ");
          break;
      }
    }
  // Less than one hundred indicates a discrete falling edge
  // We can ignore button releases.
  }else{
//    lcd2.setCursor(0, 0);
//    lcd2.print(" Instrument one ");
//    lcd2.setCursor(0, 1);
//    lcd2.print("                ");
  }
}

void updateModule2(int funPin){
  int currentModuleNumber = 1;
  // Two hundred or more indicates a change on an analog
  if (funPin >= 200){
    // 206 is the timeOffset moduleSettings[4][x] potentiometer
    if (funPin == 206){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][6]);
      moduleSettings[currentModuleNumber][4] = map(incomingTimeOffset, 0, 1023, 0, mainTempo);
      lcd3.setCursor(0, 0);
      lcd3.print("  Time Offset   ");
      lcd3.setCursor(0, 1);
      lcd3.print(moduleSettings[currentModuleNumber][4]);
      lcd3.print("   ");
      lcd3.setCursor(5, 1);
      lcd3.print("mS         ");
    }
    // 207 is the velocity moduleSettings[5][x] potentiometer
    if (funPin == 207){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][7]);
      moduleSettings[currentModuleNumber][5] = map(incomingTimeOffset, 0, 1023, 0, 127);
      lcd3.setCursor(0, 0);
      lcd3.print("    Velocity    ");
      lcd3.setCursor(0, 1);
      lcd3.print(moduleSettings[currentModuleNumber][5]);
      lcd3.print("   ");
      lcd3.setCursor(5, 1);
      lcd3.print(" / 127     ");
    }
  // One hundred or more indicates a discrete rising edge
  }else if (funPin >= 100){
    // 100 is the pin for decrementing the instrument variable
    if (funPin == 100){
      moduleSettings[currentModuleNumber][0]--;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd3.setCursor(0, 0);
      lcd3.print("Instrument  ");
      lcd3.print(moduleSettings[currentModuleNumber][0]);
      lcd3.print("   ");
      lcd3.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd3.print(buffer);
    }
    // 101 is the pin for incrementing the instrument moduleSettings[x][0] variable
    if (funPin == 101){
      moduleSettings[currentModuleNumber][0]++;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd3.setCursor(0, 0);
      lcd3.print("Instrument  ");
      lcd3.print(moduleSettings[currentModuleNumber][0]);
      lcd3.print("   ");
      lcd3.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd3.print(buffer);
    }
    // 102 is the pin for decrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 102){
      moduleSettings[currentModuleNumber][1]--;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = (-1 * greatestRangeInPitch);
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd3.setCursor(0, 0);
      lcd3.print(" Pitch Offset-- ");
      lcd3.setCursor(0, 1);
      lcd3.print(moduleSettings[currentModuleNumber][1]);
      lcd3.print("               ");
    }
    // 103 is the pin for incrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 103){
      moduleSettings[currentModuleNumber][1]++;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = -1 * greatestRangeInPitch;
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd3.setCursor(0, 0);
      lcd3.print(" Pitch Offset++ ");
      lcd3.setCursor(0, 1);
      lcd3.print(moduleSettings[currentModuleNumber][1]);
      lcd3.print("               ");
    }
    // 104 sets the value for the chords
    if (funPin == 104){
      moduleSettings[currentModuleNumber][2]++;
      if(moduleSettings[currentModuleNumber][2] >= numberOfChordTypes){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      if(moduleSettings[currentModuleNumber][2] < 0){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      lcd3.setCursor(0, 0);
      lcd3.print("  Chord change  ");
      lcd3.setCursor(0, 1);
      for (int i=0; i<16; i++){
        lcd3.print(chordList[moduleSettings[currentModuleNumber][2]][i]);
      }
    }
    // 105 is the button for changing how often a module plays a note
    // which is being called "Play Period".
    // The setting in moduleSettings[currentModuleNumber][3] determines
    // if every note, every other note, every third note, every fourth
    // note, etc, is played.
    if (funPin == 105){
      moduleSettings[currentModuleNumber][3]++;
      if(moduleSettings[currentModuleNumber][3] > leastPlayableFrequency + 9){
        moduleSettings[currentModuleNumber][3] = 0;
      }
      if(moduleSettings[currentModuleNumber][3] < 0){
        moduleSettings[currentModuleNumber][3] = leastPlayableFrequency;
        Serial.println("Playable notes is deccrementing. Oops.");
      }
      lcd3.setCursor(0, 0);
      lcd3.print(" Playable notes ");
      lcd3.setCursor(0, 1);
      lcd3.print("Every ");
      switch (moduleSettings[currentModuleNumber][3]){
        case 0:
          lcd3.setCursor(0, 1);
          lcd3.print(" Nothing plays  ");
          break;
        case 1:
          lcd3.print("note plays");
          break;
        case 2:
          lcd3.print("other note");
          break;
        case 3:
          lcd3.print("third note");
          break;
        case 4:
          lcd3.print("fourth    ");
          break;
        case 5:
          lcd3.print("fifth note");
          break;
        case 6:
          lcd3.print("sixth note");
          break;
        case 7:
          lcd3.print("seventh   ");
          break;
        case 8:
          lcd3.print("eighth    ");
          break;
        case 9:
          lcd3.print("nineth    ");
          break;
        case 10:
          lcd3.print("tenth note");
          break;
        default:
          lcd3.setCursor(0, 1);
          int percentChanceToPlay = (moduleSettings[currentModuleNumber][3] - leastPlayableFrequency);
          lcd3.print(percentChanceToPlay * 10);
          lcd3.print("% at prandom  ");
          break;
      }
    }
  // Less than one hundred indicates a discrete falling edge
  // We can ignore button releases.
  }else{
//    lcd3.setCursor(0, 0);
//    lcd3.print(" Instrument one ");
//    lcd3.setCursor(0, 1);
//    lcd3.print("                ");
  }
}

void updateModule3(int funPin){
  int currentModuleNumber = 2;
  // Two hundred or more indicates a change on an analog
  if (funPin >= 200){
    // 206 is the timeOffset moduleSettings[4][x] potentiometer
    if (funPin == 206){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][6]);
      moduleSettings[currentModuleNumber][4] = map(incomingTimeOffset, 0, 1023, 0, mainTempo);
      lcd4.setCursor(0, 0);
      lcd4.print("  Time Offset   ");
      lcd4.setCursor(0, 1);
      lcd4.print(moduleSettings[currentModuleNumber][4]);
      lcd4.print("   ");
      lcd4.setCursor(5, 1);
      lcd4.print("mS         ");
    }
    // 207 is the velocity moduleSettings[5][x] potentiometer
    if (funPin == 207){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][7]);
      moduleSettings[currentModuleNumber][5] = map(incomingTimeOffset, 0, 1023, 0, 127);
      lcd4.setCursor(0, 0);
      lcd4.print("    Velocity    ");
      lcd4.setCursor(0, 1);
      lcd4.print(moduleSettings[currentModuleNumber][5]);
      lcd4.print("   ");
      lcd4.setCursor(5, 1);
      lcd4.print(" / 127     ");
    }
  // One hundred or more indicates a discrete rising edge
  }else if (funPin >= 100){
    // 100 is the pin for decrementing the instrument variable
    if (funPin == 100){
      moduleSettings[currentModuleNumber][0]--;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd4.setCursor(0, 0);
      lcd4.print("Instrument  ");
      lcd4.print(moduleSettings[currentModuleNumber][0]);
      lcd4.print("   ");
      lcd4.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd4.print(buffer);
    }
    // 101 is the pin for incrementing the instrument moduleSettings[x][0] variable
    if (funPin == 101){
      moduleSettings[currentModuleNumber][0]++;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd4.setCursor(0, 0);
      lcd4.print("Instrument  ");
      lcd4.print(moduleSettings[currentModuleNumber][0]);
      lcd4.print("   ");
      lcd4.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd4.print(buffer);
    }
    // 102 is the pin for decrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 102){
      moduleSettings[currentModuleNumber][1]--;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = (-1 * greatestRangeInPitch);
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd4.setCursor(0, 0);
      lcd4.print(" Pitch Offset-- ");
      lcd4.setCursor(0, 1);
      lcd4.print(moduleSettings[currentModuleNumber][1]);
      lcd4.print("               ");
    }
    // 103 is the pin for incrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 103){
      moduleSettings[currentModuleNumber][1]++;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = -1 * greatestRangeInPitch;
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd4.setCursor(0, 0);
      lcd4.print(" Pitch Offset++ ");
      lcd4.setCursor(0, 1);
      lcd4.print(moduleSettings[currentModuleNumber][1]);
      lcd4.print("               ");
    }
    // 104 sets the value for the chords
    if (funPin == 104){
      moduleSettings[currentModuleNumber][2]++;
      if(moduleSettings[currentModuleNumber][2] >= numberOfChordTypes){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      if(moduleSettings[currentModuleNumber][2] < 0){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      lcd4.setCursor(0, 0);
      lcd4.print("  Chord change  ");
      lcd4.setCursor(0, 1);
      for (int i=0; i<16; i++){
        lcd4.print(chordList[moduleSettings[currentModuleNumber][2]][i]);
      }
    }
    // 105 is the button for changing how often a module plays a note
    // which is being called "Play Period".
    // The setting in moduleSettings[currentModuleNumber][3] determines
    // if every note, every other note, every third note, every fourth
    // note, etc, is played.
    if (funPin == 105){
      moduleSettings[currentModuleNumber][3]++;
      if(moduleSettings[currentModuleNumber][3] > leastPlayableFrequency + 9){
        moduleSettings[currentModuleNumber][3] = 0;
      }
      if(moduleSettings[currentModuleNumber][3] < 0){
        moduleSettings[currentModuleNumber][3] = leastPlayableFrequency;
        Serial.println("Playable notes is deccrementing. Oops.");
      }
      lcd4.setCursor(0, 0);
      lcd4.print(" Playable notes ");
      lcd4.setCursor(0, 1);
      lcd4.print("Every ");
      switch (moduleSettings[currentModuleNumber][3]){
        case 0:
          lcd4.setCursor(0, 1);
          lcd4.print(" Nothing plays  ");
          break;
        case 1:
          lcd4.print("note plays");
          break;
        case 2:
          lcd4.print("other note");
          break;
        case 3:
          lcd4.print("third note");
          break;
        case 4:
          lcd4.print("fourth    ");
          break;
        case 5:
          lcd4.print("fifth note");
          break;
        case 6:
          lcd4.print("sixth note");
          break;
        case 7:
          lcd4.print("seventh   ");
          break;
        case 8:
          lcd4.print("eighth    ");
          break;
        case 9:
          lcd4.print("nineth    ");
          break;
        case 10:
          lcd4.print("tenth note");
          break;
        default:
          lcd4.setCursor(0, 1);
          int percentChanceToPlay = (moduleSettings[currentModuleNumber][3] - leastPlayableFrequency);
          lcd4.print(percentChanceToPlay * 10);
          lcd4.print("% at prandom  ");
          break;
      }
    }
  // Less than one hundred indicates a discrete falling edge
  // We can ignore button releases.
  }else{
//    lcd4.setCursor(0, 0);
//    lcd4.print(" Instrument one ");
//    lcd4.setCursor(0, 1);
//    lcd4.print("                ");
  }
}

void updateModule4(int funPin){
  int currentModuleNumber = 3;
  // Two hundred or more indicates a change on an analog
  if (funPin >= 200){
    // 206 is the timeOffset moduleSettings[4][x] potentiometer
    if (funPin == 206){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][6]);
      moduleSettings[currentModuleNumber][4] = map(incomingTimeOffset, 0, 1023, 0, mainTempo);
      lcd5.setCursor(0, 0);
      lcd5.print("  Time Offset   ");
      lcd5.setCursor(0, 1);
      lcd5.print(moduleSettings[currentModuleNumber][4]);
      lcd5.print("   ");
      lcd5.setCursor(5, 1);
      lcd5.print("mS         ");
    }
    // 207 is the velocity moduleSettings[5][x] potentiometer
    if (funPin == 207){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][7]);
      moduleSettings[currentModuleNumber][5] = map(incomingTimeOffset, 0, 1023, 0, 127);
      lcd5.setCursor(0, 0);
      lcd5.print("    Velocity    ");
      lcd5.setCursor(0, 1);
      lcd5.print(moduleSettings[currentModuleNumber][5]);
      lcd5.print("   ");
      lcd5.setCursor(5, 1);
      lcd5.print(" / 127     ");
    }
  // One hundred or more indicates a discrete rising edge
  }else if (funPin >= 100){
    // 100 is the pin for decrementing the instrument variable
    if (funPin == 100){
      moduleSettings[currentModuleNumber][0]--;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd5.setCursor(0, 0);
      lcd5.print("Instrument  ");
      lcd5.print(moduleSettings[currentModuleNumber][0]);
      lcd5.print("   ");
      lcd5.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd5.print(buffer);
    }
    // 101 is the pin for incrementing the instrument moduleSettings[x][0] variable
    if (funPin == 101){
      moduleSettings[currentModuleNumber][0]++;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd5.setCursor(0, 0);
      lcd5.print("Instrument  ");
      lcd5.print(moduleSettings[currentModuleNumber][0]);
      lcd5.print("   ");
      lcd5.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd5.print(buffer);
    }
    // 102 is the pin for decrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 102){
      moduleSettings[currentModuleNumber][1]--;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = (-1 * greatestRangeInPitch);
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd5.setCursor(0, 0);
      lcd5.print(" Pitch Offset-- ");
      lcd5.setCursor(0, 1);
      lcd5.print(moduleSettings[currentModuleNumber][1]);
      lcd5.print("               ");
    }
    // 103 is the pin for incrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 103){
      moduleSettings[currentModuleNumber][1]++;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = -1 * greatestRangeInPitch;
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd5.setCursor(0, 0);
      lcd5.print(" Pitch Offset++ ");
      lcd5.setCursor(0, 1);
      lcd5.print(moduleSettings[currentModuleNumber][1]);
      lcd5.print("               ");
    }
    // 104 sets the value for the chords
    if (funPin == 104){
      moduleSettings[currentModuleNumber][2]++;
      if(moduleSettings[currentModuleNumber][2] >= numberOfChordTypes){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      if(moduleSettings[currentModuleNumber][2] < 0){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      lcd5.setCursor(0, 0);
      lcd5.print("  Chord change  ");
      lcd5.setCursor(0, 1);
      for (int i=0; i<16; i++){
        lcd5.print(chordList[moduleSettings[currentModuleNumber][2]][i]);
      }
    }
    // 105 is the button for changing how often a module plays a note
    // which is being called "Play Period".
    // The setting in moduleSettings[currentModuleNumber][3] determines
    // if every note, every other note, every third note, every fourth
    // note, etc, is played.
    if (funPin == 105){
      moduleSettings[currentModuleNumber][3]++;
      if(moduleSettings[currentModuleNumber][3] > leastPlayableFrequency + 9){
        moduleSettings[currentModuleNumber][3] = 0;
      }
      if(moduleSettings[currentModuleNumber][3] < 0){
        moduleSettings[currentModuleNumber][3] = leastPlayableFrequency;
        Serial.println("Playable notes is deccrementing. Oops.");
      }
      lcd5.setCursor(0, 0);
      lcd5.print(" Playable notes ");
      lcd5.setCursor(0, 1);
      lcd5.print("Every ");
      switch (moduleSettings[currentModuleNumber][3]){
        case 0:
          lcd5.setCursor(0, 1);
          lcd5.print(" Nothing plays  ");
          break;
        case 1:
          lcd5.print("note plays");
          break;
        case 2:
          lcd5.print("other note");
          break;
        case 3:
          lcd5.print("third note");
          break;
        case 4:
          lcd5.print("fourth    ");
          break;
        case 5:
          lcd5.print("fifth note");
          break;
        case 6:
          lcd5.print("sixth note");
          break;
        case 7:
          lcd5.print("seventh   ");
          break;
        case 8:
          lcd5.print("eighth    ");
          break;
        case 9:
          lcd5.print("nineth    ");
          break;
        case 10:
          lcd5.print("tenth note");
          break;
        default:
          lcd5.setCursor(0, 1);
          int percentChanceToPlay = (moduleSettings[currentModuleNumber][3] - leastPlayableFrequency);
          lcd5.print(percentChanceToPlay * 10);
          lcd5.print("% at prandom  ");
          break;
      }
    }
  // Less than one hundred indicates a discrete falling edge
  // We can ignore button releases.
  }else{
//    lcd5.setCursor(0, 0);
//    lcd5.print(" Instrument one ");
//    lcd5.setCursor(0, 1);
//    lcd5.print("                ");
  }
}

void updateModule5(int funPin){
  int currentModuleNumber = 4;
  // Two hundred or more indicates a change on an analog
  if (funPin >= 200){
    // 206 is the timeOffset moduleSettings[4][x] potentiometer
    if (funPin == 206){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][6]);
      moduleSettings[currentModuleNumber][4] = map(incomingTimeOffset, 0, 1023, 0, mainTempo);
      lcd6.setCursor(0, 0);
      lcd6.print("  Time Offset   ");
      lcd6.setCursor(0, 1);
      lcd6.print(moduleSettings[currentModuleNumber][4]);
      lcd6.print("   ");
      lcd6.setCursor(5, 1);
      lcd6.print("mS         ");
    }
    // 207 is the velocity moduleSettings[5][x] potentiometer
    if (funPin == 207){
      int incomingTimeOffset = analogRead(modulePinArrangement[currentModuleNumber][7]);
      moduleSettings[currentModuleNumber][5] = map(incomingTimeOffset, 0, 1023, 0, 127);
      lcd6.setCursor(0, 0);
      lcd6.print("    Velocity    ");
      lcd6.setCursor(0, 1);
      lcd6.print(moduleSettings[currentModuleNumber][5]);
      lcd6.print("   ");
      lcd6.setCursor(5, 1);
      lcd6.print(" / 127     ");
    }
  // One hundred or more indicates a discrete rising edge
  }else if (funPin >= 100){
    // 100 is the pin for decrementing the instrument variable
    if (funPin == 100){
      moduleSettings[currentModuleNumber][0]--;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd6.setCursor(0, 0);
      lcd6.print("Instrument  ");
      lcd6.print(moduleSettings[currentModuleNumber][0]);
      lcd6.print("   ");
      lcd6.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd6.print(buffer);
    }
    // 101 is the pin for incrementing the instrument moduleSettings[x][0] variable
    if (funPin == 101){
      moduleSettings[currentModuleNumber][0]++;
      if(moduleSettings[currentModuleNumber][0] > 127){
        moduleSettings[currentModuleNumber][0] = 0;
      }
      if(moduleSettings[currentModuleNumber][0] < 0){
        moduleSettings[currentModuleNumber][0] = 127;
      }
      changeInstrument(currentModuleNumber, moduleSettings[currentModuleNumber][0]);
      lcd6.setCursor(0, 0);
      lcd6.print("Instrument  ");
      lcd6.print(moduleSettings[currentModuleNumber][0]);
      lcd6.print("   ");
      lcd6.setCursor(0, 1);
      strcpy_P(buffer, (char *)pgm_read_word(&(MIDIList[moduleSettings[currentModuleNumber][0]])));  // Necessary casts and dereferencing, just copy.
      lcd6.print(buffer);
    }
    // 102 is the pin for decrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 102){
      moduleSettings[currentModuleNumber][1]--;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = (-1 * greatestRangeInPitch);
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd6.setCursor(0, 0);
      lcd6.print(" Pitch Offset-- ");
      lcd6.setCursor(0, 1);
      lcd6.print(moduleSettings[currentModuleNumber][1]);
      lcd6.print("               ");
    }
    // 103 is the pin for incrementing the pitch offset moduleSettings[x][1] variable
    if (funPin == 103){
      moduleSettings[currentModuleNumber][1]++;
      if(moduleSettings[currentModuleNumber][1] > greatestRangeInPitch){
        moduleSettings[currentModuleNumber][1] = -1 * greatestRangeInPitch;
      }
      if(moduleSettings[currentModuleNumber][1] < (-1 * greatestRangeInPitch)){
        moduleSettings[currentModuleNumber][1] = greatestRangeInPitch;
      }
      lcd6.setCursor(0, 0);
      lcd6.print(" Pitch Offset++ ");
      lcd6.setCursor(0, 1);
      lcd6.print(moduleSettings[currentModuleNumber][1]);
      lcd6.print("               ");
    }
    // 104 sets the value for the chords
    if (funPin == 104){
      moduleSettings[currentModuleNumber][2]++;
      if(moduleSettings[currentModuleNumber][2] >= numberOfChordTypes){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      if(moduleSettings[currentModuleNumber][2] < 0){
        moduleSettings[currentModuleNumber][2] = 0;
      }
      lcd6.setCursor(0, 0);
      lcd6.print("  Chord change  ");
      lcd6.setCursor(0, 1);
      for (int i=0; i<16; i++){
        lcd6.print(chordList[moduleSettings[currentModuleNumber][2]][i]);
      }
    }
    // 105 is the button for changing how often a module plays a note
    // which is being called "Play Period".
    // The setting in moduleSettings[currentModuleNumber][3] determines
    // if every note, every other note, every third note, every fourth
    // note, etc, is played.
    if (funPin == 105){
      moduleSettings[currentModuleNumber][3]++;
      if(moduleSettings[currentModuleNumber][3] > leastPlayableFrequency + 9){
        moduleSettings[currentModuleNumber][3] = 0;
      }
      if(moduleSettings[currentModuleNumber][3] < 0){
        moduleSettings[currentModuleNumber][3] = leastPlayableFrequency;
        Serial.println("Playable notes is deccrementing. Oops.");
      }
      lcd6.setCursor(0, 0);
      lcd6.print(" Playable notes ");
      lcd6.setCursor(0, 1);
      lcd6.print("Every ");
      switch (moduleSettings[currentModuleNumber][3]){
        case 0:
          lcd6.setCursor(0, 1);
          lcd6.print(" Nothing plays  ");
          break;
        case 1:
          lcd6.print("note plays");
          break;
        case 2:
          lcd6.print("other note");
          break;
        case 3:
          lcd6.print("third note");
          break;
        case 4:
          lcd6.print("fourth    ");
          break;
        case 5:
          lcd6.print("fifth note");
          break;
        case 6:
          lcd6.print("sixth note");
          break;
        case 7:
          lcd6.print("seventh   ");
          break;
        case 8:
          lcd6.print("eighth    ");
          break;
        case 9:
          lcd6.print("nineth    ");
          break;
        case 10:
          lcd6.print("tenth note");
          break;
        default:
          lcd6.setCursor(0, 1);
          int percentChanceToPlay = (moduleSettings[currentModuleNumber][3] - leastPlayableFrequency);
          lcd6.print(percentChanceToPlay * 10);
          lcd6.print("% at prandom  ");
          break;
      }
    }
  // Less than one hundred indicates a discrete falling edge
  // We can ignore button releases.
  }else{
//    lcd6.setCursor(0, 0);
//    lcd6.print(" Instrument one ");
//    lcd6.setCursor(0, 1);
//    lcd6.print("                ");
  }
}
