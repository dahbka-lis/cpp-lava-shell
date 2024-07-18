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
    execvpe(GetArg(0), args_.data(), env_);
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

const char *Subcommand::GetArg(int index) const {
    return args_[index];
}

int Command::Execute() {
    int status = 0;
    pid_t pid;

    std_fd_ = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
    file_fd_ = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};

    for (size_t i = 0; i < subcommands_.size(); ++i) {
        auto sub = subcommands_[i];
        auto path = sub.GetArg(0);

        if (strcmp(path, Details::CustomCommands::kExit) == 0) {
            CloseDescriptors();
            _exit(Details::Status::SUCCESS);
        }

        if (strcmp(path, Details::CustomCommands::kChangeDir) == 0) {
            status = chdir(sub.GetArg(1));
            continue;
        }

        if (strlen(sub.GetInputFile())) {
            close(file_fd_.in);
            file_fd_.in = open(sub.GetInputFile(), O_RDONLY);

            if (file_fd_.in == -1) {
                status = Details::Status::FAIL_FILE;
                perror("Error opening input file");
                continue;
            }
        }

        dup2(file_fd_.in, STDIN_FILENO);
        close(file_fd_.in);

        if (i + 1 < subcommands_.size()) {
            int fd_pipe[2];
            if (pipe(fd_pipe) < 0) {
                status = Details::Status::FAIL_PIPE;
                perror("Error opening pipe");
                continue;
            }

            file_fd_.in = fd_pipe[0];
            file_fd_.out = fd_pipe[1];
        } else {
            file_fd_.out = dup(std_fd_.out);
        }

        if (strlen(sub.GetOutputFile())) {
            close(file_fd_.out);
            auto flags = O_WRONLY | O_CREAT | O_TRUNC;
            file_fd_.out = open(sub.GetOutputFile(), flags, 0666);

            if (file_fd_.out == -1) {
                status = Details::Status::FAIL_FILE;
                perror("Error opening output file");
                continue;
            }
        }

        dup2(file_fd_.out, STDOUT_FILENO);
        close(file_fd_.out);

        pid = fork();
        if (pid == 0) {
            sub.Execute();
        } else if (pid < 0) {
            status = Details::Status::FAIL_FORK;
            perror("Error forking");
            continue;
        }

        waitpid(pid, &status, 0);
    }

    CloseDescriptors();
    return WEXITSTATUS(status);
}

void Command::AddSubcommand(Subcommand &&subcommand) {
    subcommands_.push_back(std::move(subcommand));
}

void Command::CloseDescriptors() const {
    dup2(std_fd_.in, STDIN_FILENO);
    dup2(std_fd_.out, STDOUT_FILENO);

    close(std_fd_.in);
    close(std_fd_.out);
    close(file_fd_.in);
    close(file_fd_.out);
}
} // namespace Lavash
