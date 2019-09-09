int toggle_on_off()

{
  //START/STOP TEST_RIG
  //**************************************************************************
  if (debouncedButtonState == HIGH && toggleClearance == true)
  {
    machineRunning = !machineRunning;
    toggleClearance = false;
  }

  if (debouncedButtonState == LOW)
  {
    toggleClearance = true ;
  }
}
