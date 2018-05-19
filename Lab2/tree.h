typedef struct tree
{
	char *name;
	char value[1000];
	int temp, empty, size, num;
	struct Tree *child, *brother;
}Tree;

Tree *CreateTree(char *name);
Tree *NewNode(char *name, char *value, int line);
Tree *AddChild(char *name, int line, int num, ...);
void PrintTree(Tree *parent, int blank);


