// Ensure that the note about to be played is within the limits.
int maintainNoteLimits(int funStartingPoint, int funRequestedChange, int funScaleType, int funScaleCount, int funLowerLimit, int funUpperLimit){
  int safelyRangedChange;
  boolean alteredOnce = true;
  int requestedNote = funStartingPoint + calculateScale(funRequestedChange, funScaleType, funScaleCount);
//  Serial.println();
//  Serial.print("Requested Change:\t");
//  Serial.print(funRequestedChange);
//  Serial.print("\tRequested Note:\t");
//  Serial.println(requestedNote);
//  Serial.println(calculateScale(funRequestedChange, funScaleType, funScaleCount));
  // Assess if the requested note needs to be raised to keep it above the lower threshold.
  if (requestedNote < funLowerLimit){
    // If the requested change is already greater than six, the limits were probably
    // changed recently so just keep going to a higher note. Else, add six make the change positive.
    if (funRequestedChange > 6){
      safelyRangedChange = funRequestedChange;
      alteredOnce = false;
    }else{
      safelyRangedChange = 6 + funRequestedChange;
      alteredOnce = false;
    }
  }
  // Assess if the requested note needs to be lowered to keep it below the upper threshold.
  if ((requestedNote > funUpperLimit) && alteredOnce){
    // If the requested change is already less than six, the limits were probably
    // changed recently so just keep going to a lower note. Else, subtract six to make the change negative.
    if (funRequestedChange < 6){
      safelyRangedChange = funRequestedChange;
    }else{
      safelyRangedChange = funRequestedChange - 6;
    }
  }
//  Serial.print("Somehow escaped\t ");
  return safelyRangedChange;
}


int calculateScale(int funDelta, int funScaleType, int funScaleTypes){
  // If an invalid scale type is sent, return 0, which will cause
  // the same note to be played repeatedly.
  if (funScaleType < 0 || funScaleType > funScaleTypes){
    return 0;
  }
  switch(funScaleType){
    case 0:
      return minorPentatonicScale[funDelta];
    case 1:
      return majorPentatonicScale[funDelta];
    case 2:
      return EgyptianPentatonicScale[funDelta];
    case 3:
      return bluesMinorPentatonicScale[funDelta];
    case 4:
      return bluesMajorPentatonicScale[funDelta];
    case 5:
      // This special mode will allow different things to be tested
      // on the same note for comparison's sake.
      return 0;
    return 0;
  }
}

void generateNewNote(){
    noteDelta = centerWeightedVariable(centerWeightingFactor);
    // Check to see that the note will be within the prescribed range.
    noteDelta = maintainNoteLimits(mainNote, noteDelta, scaleType, numberOfScaleTypes, lowerThreshold, upperThreshold);
    // Change the mainNote variable according to the selected scale.
    mainNote = mainNote + (calculateScale(noteDelta, scaleType, numberOfScaleTypes));
//    // Play the note, wait, and stop the note, then go back to the beginning of the loop()
//    playChord(mainNote, mainVelocity, chordType);
}

// The intensity input will determine how likely the note is to remain close to the mean.
// It will likely, at first, be set by a variable. It must be between 0 and 5.
// 0 will provide an even distribution.
// 5 will provide numbers tightly clustered around the mean.
int centerWeightedVariable(int funWeightingVar){
  int runningTotal = 0;
  int intensityArray[] = {1, 2, 3, 4, 6, 12};
  funWeightingVar = constrain(funWeightingVar, 0, 5);
  int funIntensity = intensityArray[funWeightingVar];
  for (int i = 0; i < funIntensity; i++){
    int dieValue = (12 / funIntensity) + 1;
    if (TRNG){
      runningTotal = runningTotal + Entropy.random(0, dieValue);
    }else{
      runningTotal = runningTotal + random(0, dieValue);
    }
  }
  return runningTotal;
}








// 24Eng
