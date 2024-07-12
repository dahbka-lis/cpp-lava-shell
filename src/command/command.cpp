#include "command.hpp"

#include <cstring>

namespace Lavash {
int Command::Execute() {
    return 0;
}

void Command::AddSubcommand(Subcommand &&subcommand) {
    subcommands_.push_back(std::move(subcommand));
}

void Subcommand::Execute() {
}

void Subcommand::AddArg(const std::string &args) {
    args_.push_back(strdup(args.c_str()));
}

void Subcommand::SetInputFile(const std::string &input_file) {
    input_file_ = input_file;
}

void Subcommand::SetOutputFile(const std::string &output_file) {
    output_file_ = output_file;
}
} // namespace Lavash
