int main(int argc, char **argv)
{
    pid_t kid[2];

    int AtoB[2];
    pipe(AtoB);

    int BtoA[2];
    pipe(BtoA);

    // fork A
    kid[0] = fork();
    if (kid[0] == 0) // child process: A
    {
        dup2(AtoB[1], STDOUT_FILENO);
        close(AtoB[0]);
        close(AtoB[1]);

        dup2(BtoA[0], STDIN_FILENO);
        close(BtoA[0]);
        close(BtoA[1]);

        exec(A);
        return 0;
    }
    else if (kid[0] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork A" << std::endl;
        return 1;
    }

    // fork B
    kid[1] = fork();
    if (kid[1] == 0) // child process: B
    {
        dup2(AtoB[0], STDIN_FILENO);
        close(AtoB[0]);
        close(AtoB[1]);

        dup2(BtoA[1], STDOUT_FILENO);
        close(BtoA[0]);
        close(BtoA[1]);

        exec(B);
        return 0;
    }
    else if (kid[1] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork B" << std::endl;
        return 1;
    }

    int res[2];
    for (int i = 0; i < 2; ++i)
    {
        waitpid(kid[i], &res[i], 0);
    }

    return 0;
}
