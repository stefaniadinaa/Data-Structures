#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
	Directory *root;
	Directory *current;
	char line[100];

	while (1) {
		fgets(line, 100, stdin);
		line[strlen(line)-1] = '\0';

		if (strcmp(line, CREATE_FS) == 0) {
			root = allocDirectory(ROOT);
			current = root;
		}

		if (strcmp(line, LS) == 0) {
			char *buffer = (char *) calloc(MAX, sizeof(char));

			if (buffer == NULL)
				break;

			char *files = printFiles(current);
			char *directories = printDirectories(current);

			strcat(buffer, files);
			strcat(buffer, directories);

			if (strlen(buffer) > 0)
				buffer[strlen(buffer) - 1] = '\0';

			printf("%s\n", buffer);

			free(buffer);
			free(files);
			free(directories);
		}

		if (strstr(line, MKDIR)) {
			char *name = line + strlen(MKDIR) + 1;

			Directory *dir = allocDirectory(name);

			dir->parentDir = current;
			inserDirectory(dir, current);
		}

		if (strstr(line, TOUCH)) {
			char *aux = line + strlen(TOUCH) + 1;

			char *temp = strchr(aux, ' ');

			aux[temp - aux] = '\0';

			char *name = aux;
			char *content = temp + 1;

			File *file = allocFile(name, strlen(content),
				content, current);

			insertFile(file, current);
		}

		if (strstr(line, PWD)) {
			char *pwd = printWorkingDirectory(current);

			printf("%s\n", pwd);
			free(pwd);
		}

		if (strstr(line, CD)) {
			char *arg = line + strlen(CD) + 1;

			current = changeDirectory(arg, current);
		}

		if (strstr(line, TREE))
			treePrint(current);

		if (strstr(line, RMDIR)) {
			char *name = line + strlen(RMDIR) + 1;

			int ret = removeDirectory(name, current);

			if (ret == NOT_OK) {
				printf("Cannot remove '%s': ", name);
				printf("No such directory!\n");
			}
		}

		if (strstr(line, RM) && !strstr(line, RMDIR)) {
			char *name = line + strlen(RM) + 1;

			int ret = removeFile(name, current);

			if (ret == NOT_OK)
				printf("Cannot remove '%s': No such file!\n",
					name);
		}


		if (strcmp(line, DELETE_FS) == 0) {
			removeAllFiles(root);
			removeAllDirectories(root);
			free(root->sub_directoare);
			free(root->sub_fisiere);
			free(root->name);
			free(root);

			root = NULL;
			break;
		}
	}

	return 0;
}
