#include "Random.h"
std::mt19937 Random::rng(std::random_device{}());