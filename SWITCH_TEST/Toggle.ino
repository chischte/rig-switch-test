int toggle_on_off()

{

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
