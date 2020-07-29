#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <raylib.h>

#include <vector>

class Engine {
public:
	~Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	static Engine* get_instance();

	void run();

private:
	Engine();

	void handle_input();
	void update();
	void render() const;

	void draw_points() const;
	void draw_hull() const;

	void step_algorithm(); //run one iteration of algorithm

	void randomize_points();
	void init_algorithm(); //initial setup for algorithm (should this be called reset or sth?)

	Vector2* const points; // the set of points given
	std::vector<Vector2> hull; //the points on the convex hull

	//Following are indices in the array of points corresponding to...
	size_t curr_idx;  // the most recent point found on hull
	size_t next_idx; // best guess for next point on hull
	size_t check_idx; // the point being checked
	size_t leftmost_idx; // self-explanatory ?

	bool calculating; //whether wer'e cuurently in process of calculating the hull

	Texture texture;
};


#endif // !ENGINE_HPP
