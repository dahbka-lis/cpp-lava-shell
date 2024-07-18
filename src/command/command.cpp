#include "command.hpp"

#include "../details/constants.hpp"

#include <cstring>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

namespace Lavash {
Subcommand::Subcommand(char **env) : env_(env) {
}

void Subcommand::Execute() {
    args_.push_back(nullptr);

    execvpe(args_[0], args_.data(), env_);
    perror("Lavash.Subcommand.Execute()");
    _exit(Details::Status::FAIL_EXEC);
}

void Subcommand::AddArg(const std::string &args) {
    args_.push_back(strdup(args.c_str()));
}

void Subcommand::SetInputFile(const std::string &input_file) {
    input_file_ = input_file;
}

const char *Subcommand::GetInputFile() const {
    return input_file_.c_str();
}

void Subcommand::SetOutputFile(const std::string &output_file) {
    output_file_ = output_file;
}

const char *Subcommand::GetOutputFile() const {
    return output_file_.c_str();
}

const char *Subcommand::GetFirstArg() const {
    return args_.front();
}

int Command::Execute() {
    int status = 0;
    pid_t pid;

    int std_in = dup(STDIN_FILENO);
    int std_out = dup(STDOUT_FILENO);

    int fd_in = dup(std_in);
    int fd_out = dup(std_out);

    for (size_t i = 0; i < subcommands_.size(); ++i) {
        auto sub = subcommands_[i];
        if (strcmp(sub.GetFirstArg(), Details::CustomCommands::kExit) == 0) {
            dup2(std_in, STDIN_FILENO);
            dup2(std_out, STDOUT_FILENO);
            close(std_in);
            close(std_out);

            close(fd_in);
            close(fd_out);

            _exit(Details::Status::SUCCESS);
        }

        if (strlen(sub.GetInputFile())) {
            close(fd_in);
            fd_in = open(sub.GetInputFile(), O_RDONLY);

            if (fd_in == -1) {
                status = Details::Status::FAIL_FILE;
                perror("Error opening input file");
                continue;
            }
        }

        dup2(fd_in, STDIN_FILENO);
        close(fd_in);

        if (i + 1 < subcommands_.size()) {
            int fd_pipe[2];
            if (pipe(fd_pipe) < 0) {
                status = Details::Status::FAIL_PIPE;
                perror("pipe");
                continue;
            }

            fd_in = fd_pipe[0];
            fd_out = fd_pipe[1];
        } else {
            fd_out = dup(std_out);
        }

        if (strlen(sub.GetOutputFile())) {
            close(fd_out);
            auto flags = O_WRONLY | O_CREAT | O_TRUNC;
            fd_out = open(sub.GetOutputFile(), flags, 0666);

            if (fd_out == -1) {
                status = Details::Status::FAIL_FILE;
                perror("Error opening output file");
                continue;
            }
        }

        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);

        pid = fork();
        if (pid == 0) {
            sub.Execute();
        } else if (pid < 0) {
            status = Details::Status::FAIL_FORK;
            perror("fork");
            continue;
        }

        waitpid(pid, &status, 0);
    }

    dup2(std_in, STDIN_FILENO);
    dup2(std_out, STDOUT_FILENO);
    close(std_in);
    close(std_out);

    return WEXITSTATUS(status);
}

void Command::AddSubcommand(Subcommand &&subcommand) {
    subcommands_.push_back(std::move(subcommand));
}
} // namespace Lavash
