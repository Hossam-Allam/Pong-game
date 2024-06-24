#define is_down(b)  input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_pos_1 = 0.f;
float player_pos_2 = 0.f;
float arena_half_size_x = 85;
float arena_half_size_y = 45;
float player_half_size_x = 2;
float player_half_size_y = 10;

float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;
float ball_speed_increase = 1.05f; 
float initial_ball_dp_x = 100;

int player1_score = 0, player2_score = 0;

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;

int button;
bool enemy_is_ai;


	static void simulate_game(Input * input, float dt)
	{
		clear_screen(0xA020F0);
		draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

		if (current_gamemode == GM_GAMEPLAY)
		{


			if (!enemy_is_ai) {
				if (is_down(BUTTON_W)) player_pos_1 += .2f;
				if (is_down(BUTTON_SHIFT) && is_down(BUTTON_W)) player_pos_1 += .3f;
				if (is_down(BUTTON_S)) player_pos_1 -= .2f;
				if (is_down(BUTTON_SHIFT) && is_down(BUTTON_S)) player_pos_1 -= .3f;
			}
			else {
				if (ball_p_y + 10 > player_pos_1) player_pos_1 += .1f;
				if (ball_p_y - 10 < player_pos_1) player_pos_1 -= .1f;
			}

		if (is_down(BUTTON_UP)) player_pos_2 += .2f;
		if (is_down(BUTTON_SHIFT) && is_down(BUTTON_UP)) player_pos_2 += .3f;
		if (is_down(BUTTON_DOWN)) player_pos_2 -= .2f;
		if (is_down(BUTTON_SHIFT) && is_down(BUTTON_DOWN)) player_pos_2 -= .3f;

		if (player_pos_1 + player_half_size_y > arena_half_size_y)
		{
			player_pos_1 = arena_half_size_y - player_half_size_y;
			player_pos_1 -= .5f;
		}

		if (player_pos_1 - player_half_size_y < -arena_half_size_y)
		{
			player_pos_1 = -arena_half_size_y + player_half_size_y;
			player_pos_1 += .5f;
		}

		if (player_pos_2 + player_half_size_y > arena_half_size_y)
		{
			player_pos_2 = arena_half_size_y - player_half_size_y;
			player_pos_2 -= .5f;
		}

		if (player_pos_2 - player_half_size_y < -arena_half_size_y)
		{
			player_pos_2 = -arena_half_size_y + player_half_size_y;
			player_pos_2 += .5f;
		}

		draw_rect(-80, player_pos_1, player_half_size_x, player_half_size_y, 0xA020F0);
		draw_rect(80, player_pos_2, player_half_size_x, player_half_size_y, 0xA020F0);

		//ball simulation
		{ //ball simulation
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;

			// Collision detection and response for player 2 (right side)
			if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
				ball_p_x - ball_half_size < 80 + player_half_size_x &&
				ball_p_y + ball_half_size > player_pos_2 - player_half_size_y &&
				ball_p_y - ball_half_size < player_pos_2 + player_half_size_y)
			{
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_x *= ball_speed_increase;
				ball_dp_y = (ball_p_y - player_pos_2) * 5;
			}
			// Collision detection and response for player 1 (left side)
			else if (ball_p_x + ball_half_size > -80 - player_half_size_x &&
				ball_p_x - ball_half_size < -80 + player_half_size_x &&
				ball_p_y + ball_half_size > player_pos_1 - player_half_size_y &&
				ball_p_y - ball_half_size < player_pos_1 + player_half_size_y)
			{
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_x *= ball_speed_increase;
				ball_dp_y = (ball_p_y - player_pos_1) * 3;
			}

			if (ball_p_y + ball_half_size > arena_half_size_y) { //hitting top
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			if (ball_p_x + ball_half_size > arena_half_size_x) { // hitting right side
				ball_dp_x = initial_ball_dp_x * -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) { // hitting left side
				ball_dp_x = initial_ball_dp_x;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player2_score++;
			}
		} //ball simulation

		draw_number(player1_score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player2_score, 10, 40, 1.f, 0xbbffbb);

		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff); // ball

	}
		else {
			if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) button = !button;

			if (pressed(BUTTON_ENTER))
			{
				current_gamemode = GM_GAMEPLAY;
				enemy_is_ai = button ? 0 : 1;
			}

			if (button == 0)
			{
				draw_rect(40, 0, 10, 10, 0xcccccc);
				draw_rect(-40, 0, 30, 30, 0xA020F0);
				draw_text("AI", -45, 2, 1, 0xff00ff);
			}
			else {
				draw_rect(40, 0, 30, 30, 0xA020F0);
				draw_rect(-40, 0, 10, 10, 0xcccccc);
				draw_text("MULTIPLAYER", 20, 2, 0.7, 0xff00ff);
			}

		}

}