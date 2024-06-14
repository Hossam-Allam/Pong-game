#define is_down(b)  input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input -> buttons[b].changed )
#define released(b) (!input -> buttons[b].is_down && input -> buttons[b].changed)

float player_pos = 0.f;

static void simulate_game(Input* input)
{
	clear_screen(0xff5500);
	if (is_down(BUTTON_UP)) player_pos += .3f;
	if (is_down(BUTTON_SHIFT) && is_down(BUTTON_UP)) player_pos += .5f;
	if (is_down(BUTTON_DOWN)) player_pos -= .3f;
	if (is_down(BUTTON_SHIFT) && is_down(BUTTON_DOWN)) player_pos -= .5f;

	draw_rect(-80, player_pos, 2, 10, 0x00ff22);
}