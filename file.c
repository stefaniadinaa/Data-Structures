#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Directory *allocDirectory(char *name) //alocare memorie directoare
{
	Directory *dir = malloc(sizeof(Directory));

	if (dir == NULL)
		return NULL;

	dir->name = strdup(name);

	if (dir->name == NULL) {
		free(dir);
		return NULL;
	}

	dir->sub_directoare = NULL;
	dir->sub_fisiere = NULL;
	dir->parentDir = NULL;

	return dir;
}

File *allocFile(char *name, int size, char *data, Directory *dir)
{
	File *file =  malloc(sizeof(File)); //alocare memorie fisier

	if (file == NULL)
		return NULL;

	file->name = strdup(name);

	if (file->name == NULL) {
		free(file);
		return NULL;
	}

	file->size = size;

	file->data = strdup(data);

	if (file->data == NULL) {
		free(file->name);
		free(file);
		return NULL;
	}

	file->dir = dir;

	return file;
}

int insertFile(File *file, Directory *dir) //creare fisier nou si calea
{
	nod_fisier *new_node = malloc(sizeof(nod_fisier));

	if (new_node == NULL)
		return NOT_OK;

	new_node->file = file;
	new_node->next = NULL;

	nod_fisier **head = &dir->sub_fisiere;
	nod_fisier *current = *head;

	if (*head == NULL || strcmp((*head)->file->name, file->name) >= 0) {
		new_node->next = *head;
		*head = new_node;
	} else {

		while (current->next != NULL &&
			strcmp(current->next->file->name, file->name) < 0)
			current = current->next;

		new_node->next = current->next;
		current->next = new_node;

	}

	return OK;
}

int inserDirectory(Directory *child, Directory *parent)
{
	nod_director *new_node = malloc(sizeof(nod_director));

	if (new_node == NULL) //creare Director nou si reactualizarea parintelui
		return NOT_OK;

	new_node->dir = child;
	new_node->next = NULL;

	nod_director **head = &parent->sub_directoare;
	nod_director *current = *head;

	if (*head == NULL || strcmp((*head)->dir->name, child->name) >= 0) {
		new_node->next = *head;
		*head = new_node;
	} else {

		while (current->next != NULL && strcmp(current->next->dir->name,
			child->name) < 0)
			current = current->next;

		new_node->next = current->next;
		current->next = new_node;

	}

	return OK;
}

char *printFiles(Directory *dir) //afisare lista fisere dintr-un director
{
	char *buffer = (char *) calloc(MAX, sizeof(char));

	if (buffer == NULL)
		return NULL;

	nod_fisier *head = dir->sub_fisiere;

	while (head != NULL) {
		strcat(buffer, head->file->name);
		strcat(buffer, " ");

		head = head->next;
	} //parcurgere si afisare pana se ajunge la pointarea null
	return buffer;
}

char *printDirectories(Directory *dir)
{
	char *buffer = (char *) calloc(MAX, sizeof(char));

	if (buffer == NULL)
		return NULL;

	nod_director *head = dir->sub_directoare;

	while (head != NULL) {
		strcat(buffer, head->dir->name);
		strcat(buffer, " ");

		head = head->next;
	} //parcurgere si afisare pana se ajunge la sfarsitul listei

	return buffer;
}

int removeFile(char *name, Directory *dir)
{
	nod_fisier **head = &dir->sub_fisiere;
	nod_fisier *prev = NULL;
	nod_fisier *temp = *head;
	//se ia un pointer nou initializat cu capul listei
	if (*head != NULL && strcmp(name, temp->file->name) == 0) {
		*head = temp->next;
		free(temp->file->name);
		free(temp->file->data);
		temp->file->dir = NULL;
		free(temp->file);
		free(temp); //preia numele si valoarea iar apoi se goleste
	} else {
		while (temp != NULL && strcmp(name, temp->file->name) != 0) {
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return NOT_OK;

		prev->next = temp->next;
		free(temp->file->name);
		free(temp->file->data);
		temp->file->dir = NULL;
		free(temp->file);
		free(temp);
	}
	return OK;
}

int removeDirectory(char *name, Directory *dir)
{
	nod_director **head = &dir->sub_directoare;
	nod_director *prev = NULL;
	nod_director *temp = *head;

	if (*head != NULL && strcmp(name, temp->dir->name) == 0) {
		removeAllFiles((*head)->dir); //se sterg fisierele din director
		removeAllDirectories((*head)->dir);
		*head = temp->next; //se sterg toate directoarele din director

		temp->dir->parentDir = NULL;
		free(temp->dir->sub_directoare);
		free(temp->dir->sub_fisiere);
		free(temp->dir->name);
		free(temp->dir);
		free(temp); //se sterge directorul
	} else {
		while (temp != NULL && strcmp(name, temp->dir->name) != 0) {
			prev = temp;
			temp = temp->next;
		}
		if (temp != NULL) {
			removeAllFiles(temp->dir);
			removeAllDirectories(temp->dir);
			prev->next = temp->next;

			temp->dir->parentDir = NULL;
			free(temp->dir->sub_directoare);
			free(temp->dir->sub_fisiere);
			free(temp->dir->name);
			free(temp->dir);
			free(temp);
		} else {
			return NOT_OK;
		}
	}
}

void removeAllFiles(Directory *dir)
{
	nod_fisier **head = &dir->sub_fisiere;
	nod_fisier *current = *head;
	nod_fisier *next = NULL;

	while (current != NULL) {
		next = current->next;
		free(current->file->name);
		free(current->file->data);
		current->file->dir = NULL;
		free(current->file);
		free(current);
		current = next;
	} //pana cand exista un element se goleste respectivul
	*head = NULL;
}

void removeAllDirectories(Directory *dir)
{
	nod_director **head = &dir->sub_directoare;
	nod_director *current = *head;
	nod_director *next = NULL;

	while (current != NULL) {
		if (current->dir != NULL) {
			removeAllFiles(current->dir);
			removeAllDirectories(current->dir);
			next = current->next;
			free(current->dir->name);
			free(current->dir->sub_directoare);
			free(current->dir->sub_fisiere);
			current->dir->parentDir = NULL;
			free(current->dir);
			free(current);
			current = next;
		}
	}
	*head = NULL;
}

void treePrint(Directory *dir)
{
	printf("%s\n", dir->name);
	int level = 1;

	treeFile(dir, level);
	treeDirectory(dir, level); //reunirea celor doua functii necesare
}

void treeFile(Directory *dir, int level)
{	int i;
	nod_fisier *current = dir->sub_fisiere;

	while (current != NULL)	{
		for (i = 0; i < level; i++)
			printf("%s", SPACE);

		printf("%s\n", current->file->name);
		current = current->next; //functie afisare fisiere
	}
}

void treeDirectory(Directory *dir, int level)
{
	int i;
	nod_director *current = dir->sub_directoare;

	while (current != NULL) {
		for (i = 0; i < level; i++)
			printf("%s", SPACE);

		printf("%s\n", current->dir->name);
		treeFile(current->dir, level + 1);
		treeDirectory(current->dir, level + 1);
		current = current->next; //functie afisare directoare
	}
}

char *printWorkingDirectory(Directory *current)
{
	char *buffer = (char *) calloc(MAX, sizeof(char));

	if (buffer == NULL)
		return NULL;

	if (strcmp(current->name, ROOT) == 0) {
		char *tmp = strdup(buffer);

		strcpy(buffer, "/"); //se foloseste si uneste caracterul "/"
		strcat(buffer, tmp);
		free(tmp);
	} else {
		while (current->parentDir != NULL) {
			char *tmp = strdup(buffer);

			strcpy(buffer, current->name);
			strcat(buffer, tmp);
			free(tmp);

			tmp = strdup(buffer);
			strcpy(buffer, "/");
			strcat(buffer, tmp);
			free(tmp);

			current = current->parentDir;
		}
	} //se merge din director in directorul curent si se printeaza

	return buffer;
}

Directory *searchDirectory(char *name, Directory *dir)
{
	nod_director *head = dir->sub_directoare;
	//se compara numele pana se gaseste
	while (head != NULL) {
		if (strcmp(name, head->dir->name) == 0)
			return head->dir; //se modifica inceputul listei

		head = head->next;
	}

	return NULL;
}

Directory *changeDirectory(char *arg, Directory *current)
{
	if (strcmp(arg, "..") == 0) {
		if (current->parentDir != NULL)
			return current->parentDir;
		else
			return current;
	//daca se folosesc punctele se afiseaza directorul parinte
	} else {
		Directory *toSearch = searchDirectory(arg, current);

		if (toSearch != NULL)
			return toSearch;

		printf("Cannot move to '%s': No such directory!\n", arg);
		return current;
	}
}
