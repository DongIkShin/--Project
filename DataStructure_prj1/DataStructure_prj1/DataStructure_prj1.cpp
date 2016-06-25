#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
int cnt_user = 0;	
int cnt_friendship = 0;
int cnt_tweet = 0;

typedef struct Adj{
	char id[10];
	Adj *next;
}Adj;
void Adj_init(Adj *self) {
	strcpy(self->id,"");
	self->next = NULL;
}
typedef struct{
	char id_num[10];
	char id_date[50];
	char id_name[20];
}uinfo;
typedef struct User{
	uinfo id;
	User *left, *right, *next, *parent, *pre;
	int word;
	Adj *follow;
}User;
void User_init(User *self) {
	strcpy(self->id.id_num, "");
	strcpy(self->id.id_date, "");
	strcpy(self->id.id_name, "");
	self->left = NULL;
	self->right = NULL;
	self->word = 0;
	self->follow = NULL;
	self->next = NULL;
	self->parent = NULL;
	self->pre = NULL;
}

typedef struct Word{
	char content[1000];
	char date[50];
	char id[10];
	int cnt;
	Word *left, *right, *parent;
	Word *pre, *next;
	Adj *next_id;
}Word;
void Word_init(Word *self) {
	strcpy(self->date, "");
	strcpy(self->content, "");
	strcpy(self->id,"");
	self->cnt = 1;
	self->left = NULL;
	self->right = NULL;
	self->next_id = NULL;
	self->next = NULL;
	self->parent = NULL;
	self->pre = NULL;
}

typedef struct Friend {
	char id[10];
	Friend *next;
	Adj *fri;
}Friend;
void Friend_init(Friend *self) {
	self->fri = NULL;
	self->next = NULL;
	strcpy(self->id,"");
}
User* fscanf_user(FILE *file_user) {
	User *p = (User*)malloc(sizeof(User));
	char buff[100] = "";
	char *space = " ";
	int i = 0;
	
	if (fscanf(file_user, "%s", buff) == EOF)
		return NULL;
	else {
		User_init(p);
		strcpy(p->id.id_num, buff);
		while (i++ < 6) {
			char buffer[10] = "";
			fscanf(file_user, "%s", buffer);
			strcat(p->id.id_date, buffer);
			strcat(p->id.id_date, space);
		}
		fscanf(file_user, "%s", buff);
		strcpy(p->id.id_name, buff);
		return p;
	}
}
Word* fscanf_word(FILE *file_word) {
	Word *p = (Word*)malloc(sizeof(Word));
	char buff[1000] = "";
	char *space = " ";
	int i = 0;
	if (fscanf(file_word, "%s", buff) == EOF)
		return NULL;
	else {
		Word_init(p);
		strcpy(p->id,buff);
		while (i++ < 6) {
			char buffer[10];
			fscanf(file_word, "%s", buffer);
			strcat(p->date, buffer);
			strcat(p->date, space);
		}
		fscanf(file_word,"%s",buff);
		strcpy(p->content,buff);
		return p;
	}
}
User* get_User_info(FILE *file_user, char *f_User) {
	if (NULL != file_user) {
		User *root = fscanf_user(file_user);
		User *buff = root;
		User *p = root;
		while (buff) {
			//printf("get user %s completed\n",buff->id.id_num);
			cnt_user++;
			p->next = buff;
			buff->pre = p;
			p = buff;
			buff = fscanf_user(file_user);
		}
		fclose(file_user);
		return root;
	}
	else 
		printf("failed to open user file\n");
}
Friend* get_User_friend(FILE *file_friend, char *f_friend) {
	if (NULL != file_friend) {
		Friend *root = (Friend*)malloc(sizeof(Friend));
		Friend *p = root;
		Adj *s = (Adj*)malloc(sizeof(Adj));
		Adj_init(s);
		Friend_init(root);
		char buff1[10]="";
		char buff2[10]="";
		fscanf(file_friend, "%s", buff1);
		fscanf(file_friend, "%s", buff2);
		strcpy(root->id,buff1);
		strcpy(s->id,buff2);
		root->fri = s;
		cnt_friendship++;
		while (fscanf(file_friend, "%s", buff1) != EOF) {
			fscanf(file_friend,"%s",buff2);
			cnt_friendship++;
			Adj *k = (Adj*)malloc(sizeof(Adj));
			Adj_init(k);
			strcpy(k->id, buff2);
			if (strcmp(p->id, buff1) == 0) {
				k->next = p->fri;
				p->fri = k;
			}
			else {
				Friend *pp = (Friend*)malloc(sizeof(Friend));
				Friend_init(pp);
				strcpy(pp->id,buff1);
				pp->fri = k;
				p->next = pp;
				p = pp;
			}
		}
		return root;
	}
	else {
		printf("failed to open friend file\n");
		return NULL;
	}
}
Word* get_User_word(FILE *file_word,char *f_word) {
	if (NULL != file_word) {
		Word *root = fscanf_word(file_word);
		Word *buff = root;
		Word *p = root;
		while (buff) {
			cnt_tweet++;
			p->next = buff;
			buff->pre = p;
			p = buff;
			buff = fscanf_word(file_word);
		}
		fclose(file_word);
		return root;
	}
	else {
		printf("failed to open word file\n");
		return 0;
	}
}

void insert_user_bintree(User *r, User *src) {
	for (User *p = r;src != NULL;) {
		if (strcmp(p->id.id_num, src->id.id_num) > 0) {
			if (p->left == NULL) {
				p->left = src;
				src->parent = p;
				src = NULL;
			}
			else {
				p = p->left;
			}
		}
		else if (strcmp(p->id.id_num, src->id.id_num) < 0) {
			if (p->right == NULL) {
				p->right = src;
				src->parent = p;
				src = NULL;
			}
			else {
				p = p->right;
			}
		}
		else
			break;
	}
}
User* jump(int n, User *r) {
	User *p = r;
	for (int i = 0; i < n; i++) {
		if (p->next != NULL) {
			p = p->next;
		}
		else {
			p->pre->next = NULL;
			return p;
		}
	}
	return p;
}
void insert_word_bintree(Word *r, Word *src) {
	for (Word *p = r; src;) {
		if (strcmp(p->content, src->content) > 0) {
			if (p->left == NULL) {
				p->left = src;
				src->parent = p;
				src = NULL;
			}
			else {
				p = p->left;
			}
		}
		else if (strcmp(p->content, src->content)<0) {
			if (p->right == NULL) {
				p->right = src;
				src->parent = p;
				src = NULL;
			}
			else {
				p = p->right;
			}
		}
		else {
			p->cnt++;
			Adj *adj = (Adj*)malloc(sizeof(Adj));
			Adj_init(adj);
			strcpy(adj->id, src->id);
			adj->next = p->next_id;
			p->next_id = adj;

			if (src->pre != NULL) {
				if (src->next != NULL) {
					src->pre->next = src->next;
					src->next->pre = src->pre;
				}
				else {
					src->pre->next = NULL;
				}
			}
			else {
				src->next->pre = NULL;
			}
			src = NULL;
		}
	}
}

User* build_user_bintree(User *root) {
	User *r = root;
	r = jump(cnt_user/2,root);
	printf("now building user bintree\n");
	int i = 0;
	for (User *p = root ;p; p = p->next) {
		i++;
		insert_user_bintree(root, p);
		//printf("inserted user [%s] %d\n",p->id.id_num,i);
	}
	printf("build user tree completed\n");
	return root;
}
Word *build_word_bintree(Word *root) {
	printf("now building word bintree\n");
	for (Word *p = root; p; p = p->next) {
		insert_word_bintree(root,p);
	}
	printf("build word tree completed\n");
	return root;
}

void display_statistics(User *u, Word *w, Friend *f) {
	int averf = 0, maxf = 0, minf = 5000;
	int avert = 0, maxt = 0, mint = 5000;
	int n = 0;

	for (Friend *p = f; p; p = p->next) {
		n++;
		int m = 0;
		for (Adj *pp = p->fri; pp; pp = pp->next) 
			m++;
		averf += m;
		if (maxf < m)
			maxf = m;
		if (minf > m)
			minf = m;
	}
	averf /= n;
	for (Word *p = w; p; p = p->next) {
		if (p->next_id != NULL) {
			for (Adj *y = p->next_id; y; y = y->next) {
				for (User *k = u; k;) {
					if (strcmp(k->id.id_num, y->id)>0) {
						k = k->left;
					}
					else if (strcmp(k->id.id_num, y->id) < 0) {
						k = k->right;
					}
					else {
						k->word++;
						k = NULL;
					}
				}
			}
		}
	}
	n = 0;
	for (User *p = u; p; p = p->next) {
		n++;
		if (maxt < p->word)
			maxt = p->word;
		if (mint > p->word)
			mint = p->word;
		avert += n;
	}

	printf("Average number of friends : %d\n", averf);
	printf("Minimum friends : %d\n", minf);
	printf("Maximum friends : %d\n", maxf);

	printf("Average tweets per user : %d\n", avert);
	printf("Minimum tweets per user : %d\n", mint);
	printf("Maximum tweets per user : %d\n", maxt);
}
void top_5_word(Word *r) {
	int max[10] = {0};
	Word *max_word[10] = { NULL };
	for (Word *p = r; p; p = p->next) {
		for (int i = 0; i < 10; i++) {
			if (max[i] < p->cnt && strlen(p->content) > 2) {
				max_word[i] = p;
				max[i] = p->cnt;
				break;
			}
		}
	}
	for (int i = 0; i < 10; i++)
		printf(" [%d] %s : %d times\n", i + 1,max_word[i]->content,max[i]);
}
void top_5_user(User *r) {
	int max[10] = { 0 };
	User *max_user[10] = { NULL };
	for (User *p = r; p; p = p->next) {
		for (int i = 0; i < 10; i++) {
			if (max[i] < p->word) {
				max_user[i] = p;
				max[i] = p->word;
				break;
			}
		}
	}
	for (int i = 0; i < 10; i++)
		printf(" [%d] [%s]  %s %d times\n", i + 1, max_user[i]->id.id_num, max_user[i]->id.id_name,max[i]);
}
void inorder(Word*r) {
	if (r) {
		inorder(r->left);
		printf(" %s %d\n",r->content,r->cnt);
		int t = clock();
		while (t + 100 > clock());
		inorder(r->right);
	}
}
void inorder(User *r) {
	if (r) {
		inorder(r->left);
		printf(" %s\n", r->id.id_num);
		//int t = clock();
		//while (t + 100 > clock());
		inorder(r->right);
	}
}
int find_user(Word *r) {
	char src[1000];
	printf("What words would you want to find : ");
	scanf("%s",src);
	//inorder(r);
	for (Word *p = r; p;) {
		if (strcmp(p->content, src) > 0)
			p = p->left;
		else if (strcmp(p->content, src) < 0)
			p = p->right;
		else {
			printf("\n%s\n",p->id);
			for (Adj *s = p->next_id; s; s = s->next)
				printf("%s\n",s->id);
			return 0;
		}
	}
	printf(" Not exist word ( %s )\n",src);
	return 0;
}
void find_friend(User *u, Word *w, Friend *f) {

}
int main() {
	char *f_user = "user.txt";
	char *f_friend = "friend.txt";
	char *f_word = "word.txt";
	FILE* file_user = fopen(f_user, "r");
	FILE* file_word = fopen(f_word, "r");
	FILE* file_friend = fopen(f_friend, "r");

	User *root_user = get_User_info(file_user, f_user);
	Word *root_word = get_User_word(file_word, f_word);
	Friend *root_friend = get_User_friend(file_friend,f_friend);
	User *r = root_user;

	root_word = build_word_bintree(root_word);
	printf(" %s\n",root_user->next->id.id_num);
	//inorder(root_user);
	root_user = build_user_bintree(r);
	//inorder(root_user);

	int Select = 0;
	while (Select != 99) {
		printf(" 0. Read data files\n");
		printf(" 1. display statistics\n");
		printf(" 2. Top 5 most tweewted words\n");
		printf(" 3. Top 5 most tweeted users\n");
		printf(" 4. Find user who tweeted a word (e.g. , '연세대')\n");
		printf(" 5. Find all people who are friends of the above users\n");
		printf(" 6. Delete all mentions of a word\n");
		printf(" 7. Delete all users who mentioned a word\n");
		printf(" 8. Find strongly connected components\n");
		printf(" 9. FInd shortest path form a given user\n");
		printf(" 99. Quit\nSelect Menu: ");
		scanf("%d", &Select);
		switch (Select) {
		case 0:
			printf("Total user : %d\nTotal friendship records : %d\nTotal tweets : %d\n\n", cnt_user, cnt_friendship, cnt_tweet);
			break;
		case 1:
			display_statistics(root_user,root_word,root_friend);
			break;
		case 2:
			top_5_word(root_word);
			break;
		case 3:
			top_5_user(root_user);
			break;
		case 4:
			find_user(root_word);
			break;
		case 5:
			find_friend(root_user,root_word,root_friend);
			break;
		case 6:
			printf("준비중입니다.\n");
			break;
		case 7:
			printf("준비중입니다.\n");
			break;
		case 8:
			printf("준비중입니다.\n");
			break;
		case 9:
			printf("준비중입니다.\n");
			break;
		case 99:
			printf("Quit Program\n");
			break;
		default:
			printf("Wrong Input\n");
		}
	}
	return 0;
}
