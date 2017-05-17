#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

typedef Delegate<void()> ButtonInterruptDelegate;

class DebouncedButton
{
public:
	DebouncedButton(int8_t button_pin, uint32_t debounce_duration_ms=300, bool pulls_to_ground=true);
	~DebouncedButton();
	DebouncedButton(DebouncedButton&) = delete;
	DebouncedButton &operator= ( const DebouncedButton & ) = delete;
	bool isPressed();
	bool wasPressed();

private:
	void IRAM_ATTR buttonInterruptHandler();
	int8_t button_pin_ = 0;
	bool button_pulls_to_ground_ = true;

	uint32_t button_event_ctr_ = 0;
	uint32_t last_read_button_event_ctr_ = 0;
	uint32_t last_button_event_time_ = 0;
	uint32_t button_debounce_duration_ = 200;
};



#endif