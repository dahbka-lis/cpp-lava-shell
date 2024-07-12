#pragma once

#include <string>
#include <vector>

namespace Lavash {
class Subcommand {
public:
    void Execute();

    void AddArg(const std::string &args);

    void SetInputFile(const std::string &input_file);

    void SetOutputFile(const std::string &output_file);

private:
    std::vector<char *> args_;
    std::string input_file_;
    std::string output_file_;
};

class Command {
public:
    int Execute();

    void AddSubcommand(Subcommand &&subcommand);

private:
    std::vector<Subcommand> subcommands_;
};
} // namespace Lavash
