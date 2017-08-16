#ifndef CMDOPTS_H_INCLUDED_
#define CMDOPTS_H_INCLUDED_
#include <string>
#include <vector>
using namespace std;

struct cmd_options {
    const char * path;
    const char * type;
    int width;
    int height;
    int quality;
    bool verbose;
    cmd_options() = default;
};

vector<string> process_opt_equal_sign(string arg);

bool check_opt(string short_name, string long_name, string target);

cmd_options process_options(int argc, char ** argv);

void print_help();

#endif
