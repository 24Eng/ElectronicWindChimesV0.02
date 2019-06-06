
void writeToAllScreens(char line1Message[16], char line2Message[16]){
  Serial.println(line1Message);
  Serial.println(line2Message);
  lcd1.setCursor(0, 0);
  lcd1.print(line1Message);
  lcd1.setCursor(0, 1);
  lcd1.print(line2Message);
  lcd2.setCursor(0, 0);
  lcd2.print(line1Message);
  lcd2.setCursor(0, 1);
  lcd2.print(line2Message);
  lcd3.setCursor(0, 0);
  lcd3.print(line1Message);
  lcd3.setCursor(0, 1);
  lcd3.print(line2Message);
  lcd4.setCursor(0, 0);
  lcd4.print(line1Message);
  lcd4.setCursor(0, 1);
  lcd4.print(line2Message);
  lcd5.setCursor(0, 0);
  lcd5.print(line1Message);
  lcd5.setCursor(0, 1);
  lcd5.print(line2Message);
  lcd6.setCursor(0, 0);
  lcd6.print(line1Message);
  lcd6.setCursor(0, 1);
  lcd6.print(line2Message);
}

void printBPM(){
  Serial.print("\t\t\tBPM: ");
  Serial.print(mainBPM);
  Serial.print("\n");
}
