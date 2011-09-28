#pragma once

#include <string>

using std::string;

double get_weighted_levenshtein_distance(
    const string& first_line,
    const string& second_line,
    int max_distance = 0,
    const double* cost_matrix = NULL);

int get_levenshtein_distance(
    const string& first_line,
    const string& second_line,
    int max_distance = 0);

double get_ngramm_jaccard_distance(
    const string& first_line,
    const string& second_line,
    int ngramm_length = 2);

double get_ngramm_dice_distance(
    const string& first_line,
    const string& second_line,
    int ngramm_length = 2);

int get_qwerty_keybord_distance(
    char first,
    char second);




