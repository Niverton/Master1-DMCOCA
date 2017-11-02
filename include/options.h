#pragma once

#include <string>


struct options {
    options(int argc, char** argv);
    ~options() = default;

    //Args
    int k; //
    //Optional
    std::string output = "-"; // -o
    bool display_adj = false; // -g: adjacency matrix
    bool coms = false; // -c: enable commentary
  private:
    static const char *opts;
};
