/* 
 * Accepts the following arguments:
 *
 * writefile: the first argument is a full path to a file (including filename) on the filesystem
 * writestr: the second argument is a text string which will be written within this file
 * 
 * Example: $ ./writer /tmp/aesd/assignment1/sample.txt ios
*/

#include "stdio.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

#include <errno.h>

static const int ARGMAXCNT = 2;     // program name not included

int main(int argc, char** argv)
{
    const char* writeFile = NULL;
    const char* writeStr = NULL;

    openlog(NULL, 0, LOG_USER);

    if (argc < (ARGMAXCNT+1))
    {        
        syslog(LOG_ERR, "Missing parameters: (%d) received, (%d) expected", (argc-1), ARGMAXCNT);
        return 1;
    }
    
    writeFile = argv[1];
    writeStr = argv[2];

    int fd = -1;
    fd = open(writeFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) 
    {
        syslog(LOG_ERR, "(%s) Invalid file path!", writeFile);
        return 1;
    }

    size_t strLen = strlen(writeStr);
    
    syslog(LOG_DEBUG, "Writing %s to %s", writeStr, writeFile);
    ssize_t resSize = write(fd, writeStr, strLen);
    
    if (resSize < 0)
    {
        syslog(LOG_ERR, "Value of errno: %d", errno);
        return 1;
    } 

    // Clean up
    if (fd != -1) {
        close(fd);
    }
    
    closelog ();

    return 0;
}
