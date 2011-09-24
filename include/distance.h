#pragma once

#include <string>

using std::string;

int get_levenshtein_distance(const string& first_line, const string& second_line, int max_distance);

int get_levenshtein_distance(const string& first_line, const string& second_line);

double get_ngramm_distance(const string& first_line, const string& second_line, int ngramm_length = 2);
