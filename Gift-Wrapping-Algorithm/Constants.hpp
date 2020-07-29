#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr unsigned WIN_WIDTH = 1200;
constexpr unsigned WIN_HEIGHT = 670;

constexpr float PADDING_R = 10.0f; //padding percentage

constexpr unsigned PADDING_X = PADDING_R / 100.0f * WIN_WIDTH;
constexpr unsigned PADDING_Y = PADDING_R / 100.0f * WIN_HEIGHT;

constexpr size_t POINT_CNT = 70;

constexpr float POINT_SIZE = 4.0f; //radius of drawn points

#endif // !CONSTANTS_HPP
