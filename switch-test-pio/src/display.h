#ifndef __DISPLAY_H_INCLUDED__
#define __DISPLAY_H_INCLUDED__


class Display{

private:
void new_send_to_nextion();




public:
Display();
static void nex_switch_play_pausePushCallback(void *ptr);

};

#endif /* DISPLAY_H */