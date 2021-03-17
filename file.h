#ifndef __FILE_H__
#define __FILE_H__

#define OK 0
#define NOT_OK 1
#define SPACE "    "
#define CREATE_FS "create fs"
#define DELETE_FS "delete fs"
#define LS "ls"
#define MKDIR "mkdir"
#define TOUCH "touch"
#define PWD "pwd"
#define CD "cd"
#define ROOT "/"
#define TREE "tree"
#define RMDIR "rmdir"
#define RM "rm"

#define MAX 500

typedef struct Directory Directory;
typedef struct File File;
typedef struct nod_director nod_director;
typedef struct nod_fisier nod_fisier;

struct nod_director {
	Directory *dir;
	nod_director *next;
};

struct nod_fisier {
	File *file;
	nod_fisier *next;
};

struct Directory {
	// The name of the directory
	char *name;

	// TODO: The list of files of the current directory
	nod_director *sub_directoare;

	// TODO: The list of directories of the current directory
	nod_fisier *sub_fisiere;

	// The parent directory of the current directory
	// (NULL for the root directory)
	Directory *parentDir;
};

// DO NOT MODIFY THIS STRUCTURE
struct File {
	// The name of the file
	char *name;

	// The size of the file
	int size;

	// The content of the file
	char *data;

	// The directory in which the file is located
	Directory *dir;
};


Directory *allocDirectory(char *name);

File *allocFile(char *name, int size, char *data, Directory *dir);

int insertFile(File *file, Directory *dir);
int inserDirectory(Directory *child, Directory *parent);

char *printFiles(Directory *dir);
char *printDirectories(Directory *dir);

int removeFile(char *name, Directory *dir);
int removeDirectory(char *name, Directory *dir);

void removeAllFiles(Directory *dir);
void removeAllDirectories(Directory *dir);

void treePrint(Directory *dir);

void treeFile(Directory *dir, int level);
void treeDirectory(Directory *dir, int level);

char *printWorkingDirectory(Directory *current);

Directory *changeDirectory(char *arg, Directory *current);
Directory *searchDirectory(char *name, Directory *dir);

#endif /* __FILE_H__ */


