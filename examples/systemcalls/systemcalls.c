#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "errno.h"
#include "fcntl.h"
#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    bool ret = true;

    int sys_ret = system(cmd);

    if( 0 != sys_ret)
    {
        ret = false;
    }

    return ret;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    int child_stat;
    bool ret = true;

    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        execv(command[0], command); 
        
        exit(errno);
    }
    else{
        // Parent process
        wait(&child_stat);  // suspends execution of the parent

        if (WIFEXITED(child_stat)) // Check if the child terminated normally
        {
            ret = WEXITSTATUS(child_stat) == 0 ? true: false;
        }
    }

    va_end(args);

    return ret;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    bool ret = true;
    int child_stat;
    
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process      
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execv(command[0], command);
    } else {
        // Parent process
        wait(&child_stat);  // suspends execution of the parent
        
        if (WIFEXITED(child_stat)) // Check if the child terminated normally
        {
            ret = WEXITSTATUS(child_stat) == 0 ? true: false;
        }
        
        close(fd);
    }

    va_end(args);

    return ret;
}
