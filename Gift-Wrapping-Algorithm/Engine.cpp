#include "Engine.hpp"

#include "Constants.hpp"
#include "Util.hpp"

#include <raylib.h>

#include <random>
#include <string>

Engine::~Engine()
{
}

//singleton
Engine* Engine::get_instance()
{
	static Engine engine;
	return &engine;
}

void Engine::run()
{
	while (!WindowShouldClose()) {
		handle_input();
		update();
		render();
	}
}

void Engine::randomize_points()
{
	static std::uniform_int_distribution<int> dist_x(PADDING_X, (WIN_WIDTH - 1 - PADDING_X));
	static std::uniform_int_distribution<int> dist_y(PADDING_Y, (WIN_HEIGHT - 1 - PADDING_Y));

	static std::default_random_engine ran_gen(std::random_device().operator()());

	for (size_t i = 0; i < points.size(); ++i) {
		points[i].x = dist_x(ran_gen);
		points[i].y = dist_y(ran_gen);
	}
}

void Engine::init_algorithm()
{
	leftmost_idx = least_x(points.data(), points.size());

	curr_idx = leftmost_idx;
	next_idx = (curr_idx + 1) % points.size(); //get next point in array

	check_idx = 0;

	hull.clear();
	hull.push_back(points[curr_idx]); // push left-most point to hull
}

Engine::Engine()
	: points()
	, curr_idx(0)
	, next_idx(0)
	, check_idx(0)
	, leftmost_idx(0)
	, calculating(false)
{
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "BEHTREEN");
	SetTargetFPS(60);

	texture = LoadTexture("img.png");

	points.reserve(MAX_POINTS);
	points.assign(50, {});

	
	randomize_points();
	init_algorithm();
}

void Engine::handle_input()
{
	if (!calculating) {
		if (IsKeyPressed(KEY_SPACE)) {
			randomize_points();
			hull.clear();
		}
		else if (IsKeyPressed(KEY_ENTER)) {
			this->init_algorithm();
			this->calculating = true;
		}
		else if (IsKeyPressed(KEY_UP)) {
			int new_size = points.size() + 10;
			if (new_size <= MAX_POINTS) {
				for (int i = 0; i < 10; ++i) {
					points.push_back({});
				}
				randomize_points();
				init_algorithm();
			}

		}
		else if (IsKeyPressed(KEY_DOWN)) {
			int new_size = points.size() - 10;
			if (new_size >= MIN_POINTS) {
				points.erase(points.end() - 10, points.end());
				randomize_points();
				init_algorithm();
			}
		}

	}
	else if (IsKeyPressed(KEY_X)) {		//to abort current calculation
		calculating = false;
		init_algorithm();
	}
	
}

void Engine::update()
{
	if (calculating) {
		step_algorithm();
	}
}

void Engine::render() const
{
	BeginDrawing();
	ClearBackground(DARKPURPLE);

	draw_hull();

	if (calculating) {
		DrawLineEx(points[curr_idx], points[check_idx], POINT_SIZE / 2.0f, RAYWHITE);
		DrawLineEx(points[curr_idx], points[next_idx], POINT_SIZE / 2.0f, YELLOW);
	}

	draw_points();

	std::string text = "Points Count: " + std::to_string(points.size());
	DrawText(text.c_str(), WIN_WIDTH - 300, 10, 30, RED);

	//The most important line of code.
	DrawTextureEx(texture, {0.0f, 0.0f}, 0.0f, 0.25f, WHITE);

	EndDrawing();
}

void Engine::draw_points() const
{
	static const Color colors[] = {
		RED, YELLOW, GOLD, PURPLE, PINK, ORANGE, GRAY
	};

	static const size_t len = std::size(colors);

	//for choosing alternating colors for points
	size_t c = 0;

	for (size_t i = 0; i < points.size(); ++i) {
		DrawCircleV(points[i], POINT_SIZE, colors[c %  len]);
		++c;
	}
}

void Engine::draw_hull() const
{
	const size_t len = hull.size();
	
	if (len > 1) {	//since there is no line with a single point
		for (size_t i = 1; i < len; ++i) {
			DrawLineEx(hull[i - 1], hull[i], POINT_SIZE * 1.5, GREEN);
		}
		if (!calculating) {
			DrawLineEx(hull.front(), hull.back(), POINT_SIZE * 1.5, GREEN);
		}

		for (Vector2 p : hull) {
			DrawCircleLines(p.x, p.y, POINT_SIZE * 2.0f, RAYWHITE);
		}
	}

}

void Engine::step_algorithm()
{
	float orientation = get_orientation(points[curr_idx], points[check_idx], points[next_idx]);
	
	//find point which is farthest to right wrt current point
	if (orientation > 0.0f) {
		next_idx = check_idx;
	}
	else if (orientation == 0.0f) {  //special case if points are collinear
		float curr_next_dist = points_distance_sqrd(points[curr_idx], points[next_idx]);
		float curr_check_dist = points_distance_sqrd(points[curr_idx], points[check_idx]);

		// assign to the farthest point
		if (curr_check_dist > curr_next_dist) {
			next_idx = check_idx;
		}
	}

	++check_idx;

	if (check_idx == points.size()) {
		curr_idx = next_idx;
		if (curr_idx == leftmost_idx) {		//if we reach the start point, wer'e done.
			calculating = false;
		}
		else {
			hull.push_back(points[curr_idx]); //add new point to hull
			check_idx = 0;

			// now we start checking from point next to previously found point on hull
			next_idx = (curr_idx + 1) % points.size(); 
		}
	}
}


