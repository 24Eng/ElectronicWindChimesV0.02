int checkDiscreteInputs(){
  // Check the controller inputs for changes first.
  for(int i=0; i < discreteControllerInputs; i++){
    controllerPinOldValues[i] = controllerPinNewValues[i];
    controllerPinNewValues[i] = digitalRead(controllerPinArrangement[i]);
    if (controllerPinOldValues[i] > controllerPinNewValues[i]){
      // If a button or toggle has been activated,
      // return the number of that pin plus 100.
      activeModule = 0;
      return (controllerPinArrangement[i]+100);
    }
    if (controllerPinOldValues[i] < controllerPinNewValues[i]){
      // If a button or toggle goes back to a resting position,
      // send the number of the pin.
      activeModule = 0;
      return controllerPinArrangement[i];
    }
  }
  // Check the secondary modules for changes.
  for(int i=0; i < numberOfModules; i++){
    for(int k=0; k < discreteInputsPerModule; k++){
      modulePinOldValues[i][k] = modulePinNewValues[i][k];
      modulePinNewValues[i][k] = digitalRead(modulePinArrangement[i][k]);
      if (modulePinOldValues[i][k] > modulePinNewValues[i][k]){
        // If a button or toggle has been activated,
        // return the number of that pin plus 100.
        activeModule = i + 1;
//        return (modulePinArrangement[i][k]+100);
        return (k+100);
      }
      if (modulePinOldValues[i][k] < modulePinNewValues[i][k]){
        // If a button or toggle goes back to a resting position,
        // send the number of the pin.
        activeModule = i + 1;
//        return modulePinArrangement[i][k];
        return k;
      }
    }
  }
  activeModule = 999;
  // Return 999 if nothing has changed.
  return 999;
}

int checkAnalogInputs(){
  // Check the controller module first
  for(int i = discreteControllerInputs; i < controllerInputs; i++){
    controllerPinOldValues[i] = controllerPinNewValues[i];
    int checkAnalogValue = analogRead(controllerPinArrangement[i]);
    if ((controllerPinOldValues[i] - checkAnalogValue > analogJitterTolerance) || (checkAnalogValue - controllerPinOldValues[i] > analogJitterTolerance)){
      controllerPinNewValues[i] = checkAnalogValue;
      activeModule = 0;
      return (200 + controllerPinArrangement[i]);
    }
  }
  // Check the secondary modules next.
  for(int i=0; i < numberOfModules; i++){
    for(int k = discreteInputsPerModule; k < inputsPerModule; k++){
      modulePinOldValues[i][k] = modulePinNewValues[i][k];
      int checkAnalogValue = analogRead(modulePinArrangement[i][k]);
      if ((modulePinOldValues[i][k] - checkAnalogValue > analogJitterTolerance) || (checkAnalogValue - modulePinOldValues[i][k] > analogJitterTolerance)){
        modulePinNewValues[i][k] = checkAnalogValue;
        activeModule = i + 1;
//        return (200 + modulePinArrangement[i][k]);
        return (k + 200);
      }
    }
  }
  return 999;
}
