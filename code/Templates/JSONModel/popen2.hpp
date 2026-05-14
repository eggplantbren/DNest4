#pragma once
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdexcept>
#include <string>

struct Popen2 {
    int to_child;     // write → child's stdin
    int from_child;   // read ← child's stdout
    pid_t pid;

    Popen2(const std::string& cmd) {
        int pipe_stdin[2];
        int pipe_stdout[2];

        if (pipe(pipe_stdin) < 0) throw std::runtime_error("pipe() failed");
        if (pipe(pipe_stdout) < 0) throw std::runtime_error("pipe() failed");

        pid = fork();
        if (pid < 0) throw std::runtime_error("fork() failed");

        if (pid == 0) {
            // Child process
            dup2(pipe_stdin[0], STDIN_FILENO);
            dup2(pipe_stdout[1], STDOUT_FILENO);

            close(pipe_stdin[1]);
            close(pipe_stdout[0]);

            execl("/bin/sh", "sh", "-c", cmd.c_str(), (char*)0);
            _exit(127);
        }

        // Parent
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        to_child = pipe_stdin[1];
        from_child = pipe_stdout[0];
    }

    ~Popen2() {
        close(to_child);
        close(from_child);
        waitpid(pid, nullptr, 0);
    }
};

