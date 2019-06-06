void readMIDI(){
  if (Serial1.available()) {
    MIDITempoActive[0] = millis();
    int inByte = Serial1.read();
    if (inByte == 0x99){
      delay(1);
      incomingMIDICounter = 0;
      incomingMIDINote[0] = inByte;
      inByte = Serial1.read();
      incomingMIDINote[1] = inByte;
      inByte = Serial1.read();
      incomingMIDINote[2] = inByte;
      incomingMIDINote[3] = millis();
      if(MIDITempoActive[3] == incomingMIDINote[1]){
        int timeDifference = millis() - MIDITempoActive[2];
        MIDITempoActive[2] = millis();
        MIDITempoActive[1] = timeDifference;
        setTempo();
      }
      if(MIDISignalBlank){
        MIDITempoActive[3] = incomingMIDINote[1];
        MIDISignalBlank = false;
//        Serial.print("First MIDI note received: ");
//        printTwoHex(MIDITempoActive[3]);
//        Serial.println();
      }
      for (int i=0; i<3; i++){
//        printTwoHex(incomingMIDINote[i]);
//        Serial.print(" ");
        Serial1.write(incomingMIDINote[i]);
      }
//      Serial.print("\t");
//      Serial.println(incomingMIDINote[3]);
    }
  }
}

void printEightBits(int funByte){
      if (funByte < B10000000){
        Serial.print("0");
      }
      if (funByte < B1000000){
        Serial.print("0");
      }
      if (funByte < B100000){
        Serial.print("0");
      }
      if (funByte < B10000){
        Serial.print("0");
      }
      if (funByte < B1000){
        Serial.print("0");
      }
      if (funByte < B100){
        Serial.print("0");
      }
      if (funByte < B10){
        Serial.print("0");
      }
    Serial.print(funByte, BIN);
}

void printThreeDecs(int funByte){
    if (funByte < 100){
      Serial.print("0");
    }
    if (funByte < 10){
      Serial.print("0");
    }
    Serial.print(funByte, DEC);
}

void printTwoHex(int funByte){
  if (funByte < 0x10){
    Serial.print("0");
  }
  Serial.print(funByte, HEX);
}
