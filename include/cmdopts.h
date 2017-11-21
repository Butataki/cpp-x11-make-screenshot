#ifndef CMDOPTS_H_INCLUDED_
#define CMDOPTS_H_INCLUDED_
#include <string>
#include <vector>

/** @struct cmd_options
    @brief store options from command line arguments

    Structure for holding options set in command line
*/
struct cmd_options {
    const char * path;  /**< constnant charcter pointer path - location for screenshot file */
    const char * type;  /**< constnant charcter pointer type - file type jpg or png */
    int width;  /**< integer width - custom width for image scale */
    int height;  /**< integer height - custom height for image scale */
    int quality;  /**< integer quality - quality for jpeg image lib */
    bool verbose;  /**< boolean verbose - flag for veerbose output */
    cmd_options() = default;
};

/** @fn vector<string> process_opt_equal_sign(string arg)
    @brief process command line argument to split name from value if equal sign was used
    @param raw command line argument
    @return vectror of strings length of 2 where first is argument name and second it value
*/
std::vector<std::string> process_opt_equal_sign(std::string arg);

/** @fn bool check_opt(string short_name, string long_name, string target)
    @brief check option short and long name agains target string
    @param string short_name - option short name, for example "-w"
    @param string long_name - option long name, for example "--width"
    @param string target - value to check against names
    @return true if target is eather long name ot short name, false otherwise
*/
bool check_opt(std::string short_name, std::string long_name, std::string target);

/** @fn cmd_options process_options(int argc, char ** argv)
    @brief Iterate over argv and process options
    @param integer argc - argument counter
    @param pointer to character pointers - command line arguments
    @return filled cmd_options
*/
cmd_options process_options(int argc, char ** argv);

/** @fn print_help()
    @brief prints help message
*/
void print_help();

#endif
