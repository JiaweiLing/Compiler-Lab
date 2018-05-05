typedef struct tree
{
	char* name;
	int temp, empty, size;
	struct Tree *child, *brother;
}Tree;

Tree *CreateTree(char *name);
Tree *NewNode(char *name, int line);
Tree *AddChild(char *name, int line, int num, ...);
void PrintTree(Tree *parent, int blank);


