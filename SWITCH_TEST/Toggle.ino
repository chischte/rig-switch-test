int toggle_on_off()

{
  buttonstate = digitalRead(blue_button);

  //DEBOUNCE BUTTON
  //**************************************************************************
  if (buttonstate != debounced_buttonstate && debouncetimer_set == false)
  {
    debouncetimer = millis() + 10;
    debouncetimer_set = true;
  }

  if (millis() >= debouncetimer )//...millis overflow possible!
  {
    debouncetimer_set = false;//debouncetimer can be reset
    if (buttonstate != debounced_buttonstate)
    {
          debounced_buttonstate = buttonstate;
    }
  }
  //START/STOP TEST_RIG
  //**************************************************************************
  if (debounced_buttonstate == HIGH && toggleclearance == true)
  {
    machine_running = !machine_running;
    toggleclearance = false;
  }

  if (debounced_buttonstate == LOW)
  {

    toggleclearance = true ;
  }
  digitalWrite((blue_light), machine_running);
}
