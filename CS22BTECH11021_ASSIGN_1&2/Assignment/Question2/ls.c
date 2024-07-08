#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024

void print_entry(struct dirent *ent, char *dirname, int show_hidden, int sort_by_access_time, int long_format);

int main(int argc, char *argv[]) {
    char *dirname;
    DIR *dir;
    struct dirent *ent;
    int show_hidden = 0;
    int sort_by_access_time = 0;
    int long_format = 0;
    int opt;

    while ((opt = getopt(argc, argv, "alU")) != -1) {
        switch (opt) {
            case 'a':
                show_hidden = 1;
                break;
            case 'l':
                long_format = 1;
                break;
            case 'U':
                sort_by_access_time = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-a] [-l] [-U] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        dirname = argv[optind];
    } else {
        dirname = ".";
    }

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((ent = readdir(dir)) != NULL) {
        if (!show_hidden && ent->d_name[0] == '.') {
            continue;
        }
        print_entry(ent, dirname, show_hidden, sort_by_access_time, long_format);
    }

    closedir(dir);

    return 0;
}

void print_entry(struct dirent *ent, char *dirname, int show_hidden, int sort_by_access_time, int long_format) {
    struct stat statbuf;
    char path[PATH_MAX];
    char mtime[20];
    struct tm *tm;
    struct passwd *pwd;
    struct group *grp;

    snprintf(path, PATH_MAX, "%s/%s", dirname, ent->d_name);

    if (lstat(path, &statbuf) < 0) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if (sort_by_access_time) {
        if (long_format) {
            tm = localtime(&statbuf.st_atime);
            strftime(mtime, 20, "%b %e %H:%M", tm);
        }
    } else {
        if (long_format) {
            tm = localtime(&statbuf.st_mtime);
            strftime(mtime, 20, "%b %e %H:%M", tm);
        }
    }

    if (long_format) {
        printf("%c%s %3ld ", S_ISDIR(statbuf.st_mode) ? 'd' : '-', 
               statbuf.st_mode & S_IRUSR ? "r" : "-", statbuf.st_nlink);
        pwd = getpwuid(statbuf.st_uid);
        printf("%-8s ", pwd->pw_name);
        grp = getgrgid(statbuf.st_gid);
        printf("%-8s ", grp->gr_name);
        printf("%8lld ", (long long) statbuf.st_size);
        printf("%s ", mtime);
    }

    printf("%s\n", ent->d_name);
}


