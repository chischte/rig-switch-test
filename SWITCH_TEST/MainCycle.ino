void RunMainTestCycle()
{
  currentButtonState = testSwitch.requestButtonState;

  if (currentButtonState != previousButtonState)
  {
    shorttimeCounter++;
    longtimeCounter++;
    previousButtonState = currentButtonState;
  }
