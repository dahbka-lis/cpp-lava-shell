#pragma once

#include <string>
#include <vector>

namespace Lavash {
class Subcommand {
public:
    explicit Subcommand(char **env = nullptr);

    void Execute();

    void AddArg(const std::string &args);

    void SetInputFile(const std::string &input_file);

    const char *GetInputFile() const;

    void SetOutputFile(const std::string &output_file);

    const char *GetOutputFile() const;

    const char *GetArg(int index) const;

private:
    std::vector<char *> args_;
    std::string input_file_;
    std::string output_file_;
    char **env_ = nullptr;
};

class Command {
    struct FileDescriptor {
        int in;
        int out;
    };

public:
    int Execute();

    void AddSubcommand(Subcommand &&subcommand);

    void CloseDescriptors() const;

private:
    std::vector<Subcommand> subcommands_;
    FileDescriptor std_fd_;
    FileDescriptor file_fd_;
};
} // namespace Lavash
