#include <iostream>
#include <string>
#include <vector>
#include "cmdopts.h"
#include "version.h"

void print_help() {
    fprintf(
        stdout,
        "Usage: makeshot [OPTIONS]... FILE\n"
        "Take screenshot of a screen and save it to the designated file. Supported \"jpg\" or \"png\" files. \n"
        "\n"
        "Mandatory arguments to long options are mandatory for short options too.\n"
        "\n"
        "  -w, --width=NUMBER\tscale screenshot to this width\n"
        "  -h, --height=NUMBER\tscale screenshot to this height\n"
        "  -q, --quality=NUMBER\tfor jpeg image only, set quality for jpeg lib processing, maximum is 100 minimum value is 1\n"
        "      --verbose\t\tverbose output\n"
        "  -?, --help\t\tprint this message\n"
        "      --version\t\tprint version number\n"
        "\n"
        "NUMBER is a positive integer.\n"
    );
};

std::vector<std::string> process_opt_equal_sign(std::string arg) {
    std::string value = "";
    std::string name = "";
    size_t equal_sign_position = arg.find_first_of("=");
    if (equal_sign_position > arg.length()) { // not = sign in argument
        return {arg, value};
    }
    value = arg.substr(equal_sign_position + 1);
    name = arg.substr(0, equal_sign_position);
    return {name, value};
};

bool check_opt(std::string short_name, std::string long_name, std::string target) {
    if (target.length() == 2 and target == short_name) return true;
    if (target.length() > 2 and target == long_name) return true;
    return false;
};

cmd_options process_options(int argc, char ** argv) {
    cmd_options opts;
    // init defaults
    opts.width = 0;
    opts.height = 0;
    opts.quality = 100;
    opts.verbose = false;
    opts.path = "";
    opts.type = "";
    // process optional parameters
    if (argc <= 1) {
        print_help();
        exit(0);
    }
    //
    int i;
    for (i=1; i<argc; i++) {
        std::string sArgValue = std::string(argv[i]);
        std::vector<std::string> opt = process_opt_equal_sign(sArgValue);
        // check special commands first
        if (check_opt("", "--version", opt[0])) {
            std::cout << "makeshot version " << VERSION << std::endl;
            exit(0);
        }
        if (check_opt("-?", "--help", opt[0])) {
            print_help();
            exit(0);
        }
        // optional params
        if (check_opt("-w", "--width", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = std::string(argv[i+1]);
            }
            opts.width = atoi(opt[1].c_str());
            if (opts.width < 0) opts.width = opts.width * (-1);
            continue;
        }
        if (check_opt("-h", "--height", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = std::string(argv[i+1]);
            }
            opts.height = atoi(opt[1].c_str());
            if (opts.height < 0) opts.height = opts.height * (-1);
            continue;
        }
        if (check_opt("-q", "--quality", opt[0])) {
            if (opt[1].length() == 0 and argv[i+1] != NULL) {
                opt[1] = std::string(argv[i+1]);
            }
            opts.quality = atoi(opt[1].c_str());
            if (opts.quality < 0) opts.quality = opts.quality * (-1);
            if (opts.quality > 100) opts.quality = 100;
            continue;
        }
        if (check_opt("-v", "--verbose", opt[0])) {
            opts.verbose = true;
            continue;
        }
        // positional param
        if (i == argc - 1) {
            // pick positional argument `path` to where save screenshot
            const char * path = argv[i];
            // if path empty show help message
            if (path == NULL) print_help();
            opts.path = path;
            // get file extension and validate path
            std::string sPath = std::string(path);
            size_t dot_position = sPath.find_last_of(".");
            size_t slash_position = sPath.find_last_of("/");
            std::string file_name = sPath.substr(
                slash_position + 1, dot_position - slash_position - 1
            );
            if (dot_position > sPath.length()) {
                std::cerr << path << " - malformed path to file" << std::endl;
                exit(0);
            }
            if (file_name.length() == 0 ) {
                std::cerr << path << " - malformed path to file" << std::endl;
                exit(0);
            }
            std::string extension = sPath.substr(dot_position + 1);
            if (extension.length() == 0) {
                std::cerr << path << " - malformed path to file" << std::endl;
                exit(0);
            }
            if (extension == "jpeg" || extension == "jpg") opts.type = "jpg";
            else if (extension == "png") opts.type = "png";
            else {
                std::cerr << extension << " - is not supported file extension" << std::endl;
                exit(0);
            }
        }
    }

    return opts;
};
