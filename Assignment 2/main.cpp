#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>

#define TOK_BUFFER_SIZE 64
#define READ_LINE_BUFFER_SIZE 1024
#define TOK_DELIM " =\t\r\n\a"
#define ZERO 0
#define ONE 1
#define NEWLINE '\n'
#define MAXCOMS 100
#define err(x) cerr << red << x << reset << endl;
const std::string red("\033[0;31m");
const std::string green("\033[0;32m");
const std::string blue("\033[0;34m");
const std::string yellow("\033[0;33m");
const std::string cyan("\033[0;36m");
const std::string reset("\033[0m");

using namespace std;

//TODO: Redirection:
// 1. input redirection - DONE
//2. output redirection - DONE
//3. input output redirection - DONE
//4. Clean the redirection function


//  INBUILT FUNCTIONS //
const char* builtin_str[] = {
    "cd",
    "help",
    "exit",
    "printenv",
    "setenv",
    "unsetenv",
    "history",
    "echo"};


int number_of_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

//IMPLEMENTATION OF BUILTIN FUNCTIONS
int my_cd(char **args)
{
    if (args[ONE] == NULL)
    {
        cout << "Changing directory to /home/user" << endl;
        char *home = getenv("HOME");
        chdir(home);
    }
    else
    {
        if (chdir(args[ONE]) != ZERO)
        {
            err("Error in chdir");
        }
    }

    return ONE;
}

int my_help(char **args)
{
    cout << green << "Gunjan Dhanuka's Mini-Shell!" << reset << endl;
    cout << blue << "Type the name of program and arguments, press Enter to run." << reset << endl;
    cout << blue << "The following functions are implemented: " << reset << endl;

    int i = ZERO;
    while (i < number_of_builtins())
    {
        cout << yellow << " " << builtin_str[i] << reset << endl;
        i++;
    }

    cout << red << "Use the \"man\" command to get information on other programs." << reset << endl;
    return ONE;
}

int my_exit(char **args)
{
    return ZERO;
}

int my_printenv(char **args)
{
    //cout << "PRINTENV" << endl;
    if (args[ONE] != NULL)
    {
        //cout << "arg "<<args[ONE] << endl;
        char *ans = getenv(args[ONE]);
        if (ans != NULL)
            cout << ans << endl;
        else
            cout << endl;
    }
    else
    {
        //USER, HOME, SHELL, TERM, PATH
        cout << getenv("USER") << endl;
        cout << getenv("HOME") << endl;
        cout << getenv("SHELL") << endl;
        cout << getenv("TERM") << endl;
        cout << getenv("PATH") << endl;
    }
    return ONE;
}

int my_setenv(char **args)
{

    if (args[1] == NULL || args[2] == NULL)
    {
        err("Values for setenv not provided!");
    }
    else
        int value = setenv(args[1], args[2], 1);
    return ONE;
}

int my_unsetenv(char **args)
{
    if (args[ONE] == NULL)
    {
        err("No arguments provided!");
    }
    else
    {
        int value = setenv(args[ONE], "", 1);
    }
    return ONE;
}

int my_history(char **args)
{
    ifstream hist("/tmp/history.txt");

    if (hist.is_open())
    {
        cout << hist.rdbuf();
    }
    return ONE;
}

int my_echo(char **args)
{
    int i = 1;
    while (args[i] != NULL)
    {
        //cout << "i is " << i << endl;
        if (args[i][0] == '$')
        {
            char val[20];
            int j = 1;
            // cout << "val is " << val << endl;
            // cout << "j is " << j << endl;
            while (args[i][j] != '\0')
            {
                val[j - 1] = args[i][j];
                j++;
            }
            val[j - 1] = '\0';
            char *ans = getenv((const char *)val);
            cout << ans << " ";
        }
        else
        {
            cout << args[i] << " ";
        }
        i++;
    }
    cout << endl;
    return ONE;
}

void printCurrentDir()
{
    char cwd[READ_LINE_BUFFER_SIZE];
    getcwd(cwd, sizeof(cwd));
    cout << yellow << cwd << " " << reset;
}

int read_input(char *s)
{
    int sizebuf = READ_LINE_BUFFER_SIZE;
    int index = ZERO;
    char *buf = (char *)malloc(sizeof(char) * READ_LINE_BUFFER_SIZE);
    int character;

    if (!buf)
    {
        err("Allocation error in read input");
        exit(EXIT_FAILURE);
    }

    for (;;)
    {
        character = getchar();

        switch (character)
        {
        case NEWLINE:
            buf[index] = '\0';
            strcpy(s, buf);

            if (strlen(s) == 0)
                return ONE;
            else
            {
                return ZERO;
            }

        case EOF:
            buf[index] = '\0';
            strcpy(s, buf);
            if (strlen(s) == 0)
                return ONE;
            else
            {
                return ZERO;
            }

        default:
            buf[index] = character;
            break;
        }

        index = index + ONE;

        if (index >= sizebuf)
        {
            sizebuf += READ_LINE_BUFFER_SIZE;
            buf = (char *)realloc(buf, sizebuf);

            if (!buf)
            {
                err("Allocation error in readline!");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **split_input(char *s)
{
    int buffersize = TOK_BUFFER_SIZE;
    long index = ZERO;

    char **tokenarr = (char **)malloc(sizeof(char *) * buffersize);
    char *token;

    if (!tokenarr)
    {
        err("Allocation error in split input");
        exit(EXIT_FAILURE);
    }

    token = strtok(s, TOK_DELIM);
    for (; token != NULL;)
    {
        tokenarr[index] = token;
        index++;

        if (index >= buffersize)
        {
            buffersize = buffersize + TOK_BUFFER_SIZE;
            tokenarr = (char **)realloc(tokenarr, sizeof(char *) * buffersize);

            if (!tokenarr)
            {
                err("Allocation error in split input");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokenarr[index] = NULL;
    return tokenarr;
}

void parser(char *s, char **args)
{
    int i = 0;
    while (i < MAXCOMS)
    {
        args[i] = strsep(&s, " =");

        if (args[i] == NULL)
        {
            break;
        }

        if (strlen(args[i]) == 0)
            i--;

        i = i + 1;
    }
}

int launcher(char **args)
{
    pid_t pid, wpid;
    int alive = 1;

    pid = fork();
    if (pid == ZERO)
    {
        //pid == ZERO shows child process being run
        if (execvp(args[ZERO], args) == -ONE)
        {
            err("Error encountered!");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < ZERO)
    {
        //Error while forking
        err("Error encountered!");
    }
    else
    {

        // This stands for the parent process
        //WIFEEXITED returns nonzero if child process terminated normally
        //WIFSIGNALED returns nonzero if child process terminated because it received a singal that was not handled

        while (!WIFEXITED(alive) && !WIFSIGNALED(alive))
        {
            wpid = waitpid(pid, &alive, WUNTRACED);
        }
    }

    return ONE;
}

void runsource(int pfd[], char **args)
{
    int pid;

    switch (pid = fork())
    {
    case 0:
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(args[0], args);
        err(args[0]);
    default:
        break;
    case -1:
        err("Error while forking!");
        exit(1);
    }
}

void rundest(int pfd[], char **args)
{
    int pid;

    switch (pid = fork())
    {
    case 0:
        dup2(pfd[0], 0);
        close(pfd[1]);
        execvp(args[0], args);
        err(args[0]);
    default:
        break;
    case -1:
        err("Error while forking!");
        exit(1);
    }
}

int pipeLauncher(char **parsed, char **piped)
{
    //cout << "pipelauncher entry\n";
    int pipefd[2];
    int pid, status;
    pipe(pipefd);

    runsource(pipefd, parsed);
    rundest(pipefd, piped);
    close(pipefd[0]);
    close(pipefd[1]);

    while ((pid = wait(&status)) != -1)
    {
        //pick up dead children
        //cerr << "process " << pid << " exits with " << WEXITSTATUS(status) << endl;
    }
    return 1;
}

int execute(char **args)
{
    //int i;

    if (args[ZERO] == NULL)
    {
        //Empty command.
        return ONE;
    }
    return launcher(args);
}

int inbuiltHandler(char **args)
{
    int i = 0;
    int var = 0;

    while (i < number_of_builtins())
    {
        if (strcmp(args[0], builtin_str[i]) == ZERO)
        {
            var = i + 1;
            break;
        }
        i++;
    }

    if (var == 1)
    {
        my_cd(args);
        return 1;
    }
    else if (var == 2)
    {
        my_help(args);
        return 1;
    }
    else if (var == 3)
    {
        my_exit(args);
        exit(EXIT_SUCCESS);
    }
    else if (var == 4)
    {
        my_printenv(args);
        return 1;
    }
    else if (var == 5)
    {
        my_setenv(args);
        return 1;
    }
    else if (var == 6)
    {
        my_unsetenv(args);
        return 1;
    }
    else if (var == 7)
    {
        my_history(args);
        return 1;
    }
    else if (var == 8)
    {
        my_echo(args);
        return 1;
    }
    else
    {
        return 0;
    }
}

int parserPipe(char *str, char **pipedarr)
{
    int i = ZERO;
    while (i < 2)
    {
        pipedarr[i] = strsep(&str, "|");
        if (pipedarr[i] == NULL)
            break;
        i++;
    }

    //cout << pipedarr[0] << " , " << pipedarr[1] << endl;

    if (pipedarr[1] == NULL)
        return ZERO;
    else
        return ONE;
}
int redirection(char **args)
{

    //cout << "enter redirection\n";
    int status;
    int size = 0;
    while (args[size] != NULL)
    {
        size++;
    }
    switch (fork())
    {
    case -1:
        err("Error in forking!");
        break;
    case 0:
    {
        char *args_clean[size];
        int cleanidx = 0;
        int in, out;

        for (int j = 0; j < size; j++)
        {
            if (!strcmp(args[j], "<"))
            { // looking for input character
                ++j;
                if ((in = open(args[j], O_RDONLY)) < 0)
                { // open file for reading
                    err("Error in opening file!");
                    return 1;
                }
                //cout << args[j] << " File open for reading" << endl;
                dup2(in, STDIN_FILENO); // duplicate stdin to input file
                close(in);              // close after use
                continue;
            } // end input chech

            if (!strcmp(args[j], ">"))
            { // looking for output character
                ++j;
                out = creat(args[j], 0644); // create new output file
                //cout << args[j] << " open for output" << endl;
                dup2(out, STDOUT_FILENO); // redirect stdout to file
                close(out);               // close after usere
                continue;
            } // end output check

            if (!strcmp(args[j], ">>"))
            { // looking for append
                ++j;
                int append = open(args[j], O_CREAT | O_RDWR | O_APPEND, 0644);
                //cout << args[j] << " open for output" << endl;
                dup2(append, STDOUT_FILENO);
                close(append);
                continue;
            }

            args_clean[cleanidx++] = args[j];
            //cout << args[j] << " added to clean at index " << cleanidx - 1;
        } // end loop

        args_clean[cleanidx] = NULL;
        //cout << "Running in parent" << endl;
        execvp(args_clean[0], args_clean);  // execute in parent
        err("Error in child process execution"); // error
        exit(EXIT_SUCCESS);
        //return 1;
    }
    default:
        wait(&status);
    }
    return 0;
}
int isRedirection(char **args)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            //cout << "> detected\n";
            return 1;
        }

        if (strcmp(args[i], "<") == 0)
        {
            //cout << "<< detected\n";
            return 1;
        }

        if (strcmp(args[i], ">>") == 0)
        {
            //cout << ">> detected\n";
            return 1;
        }
    }
    return 0;
}

int processInput(char *str, char **parsed, char **piped)
{
    //cout << "Process input entry\n";
    char *strpiped[2];
    int isPiped = ZERO;

    isPiped = parserPipe(str, strpiped);

    if (isPiped)
    {
        //cout << "Piped!\n";
        parser(strpiped[0], parsed);
        parser(strpiped[1], piped);
    }
    else
    {
        //cout << "Not piped!\n";
        parser(str, parsed);
    }

    if (isRedirection(parsed))
    {
        return 3;
    }

    //cout << "Out of redirection\n";

    if (inbuiltHandler(parsed))
    {
        return 0;
    }
    else
    {
        return isPiped + 1;
    }
}

void main_process()
{
    char s[100];
    char *args[MAXCOMS];
    char *pipedArgs[MAXCOMS];
    //int alive = 1;

    while (true)
    {
        printCurrentDir();
        cout << cyan << "200101038-shell: $ " << reset;
        if (read_input(s))
            continue;
        // args = split_input(s);

        //store history in /tmp/history.txt
        ofstream history_file;
        history_file.open("/tmp/history.txt", ios_base::app); //append mode
        history_file << s << endl;
        int type = processInput(s, args, pipedArgs);

        if (type == 1)
        {
            if (execute(args) == 0)
                break;
        }

        if (type == 2)
        {
            if (pipeLauncher(args, pipedArgs) == 0)
                break;
        }

        if (type == 3)
        {
            if (redirection(args))
                break;
        }

        //free up space
    }
}

int main()
{
    main_process();

    return EXIT_SUCCESS;
}