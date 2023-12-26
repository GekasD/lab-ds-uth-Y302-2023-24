#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

void printFileProperties(struct stat file_props) {
    printf("\n[ Properties of file ]\n");
    
    printf("File access: ");
    printf((file_props.st_mode & S_IRUSR) ? "read " : "");
    printf((file_props.st_mode & S_IWUSR) ? "write " : "");
    printf((file_props.st_mode & S_IXUSR) ? "execute " : "");
    printf("\n");

    printf("File size: %ld bytes\n", file_props.st_size);

    // Παράδειγμα εμφάνισης: 22-11-1977 20:53:12
    char time_str[21];
    strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", gmtime(&file_props.st_ctime));
    printf("Created on: %s\n", time_str);
    strftime(time_str, sizeof(time_str), "%d-%m-%Y %H:%M:%S", gmtime(&file_props.st_mtime));
    printf("Modified on: %s\n", time_str);
}

int main() {
    printf("Enter the name of a file: ");
    char file_path[PATH_MAX];
    scanf("%s", &file_path);

    if (access(file_path, F_OK) != -1) {
        struct stat buffer;
        stat(file_path, &buffer);
        printFileProperties(buffer);
    } else {
        printf("File (%s) does not exist", file_path);
    }

    return 0;
}