typedef struct tree
{
	char* name;
	int size;
	struct Tree *child, *brother;
}Tree;

Tree *CreateTree(char *name);
Tree *NewNode(char *name);
void AddChild(char *name, Tree *parent, int num, ...);
void PrintTree(Tree *parent);


