typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;

struct TYPE
{
	enum {BASIC, ARRAY, STRUCTURE} Kind;
	union
	{
		int Basic;
		struct
		{
			Type elem;
			int size;
		} Array;
		FieldList Structure;
	} Union;
};

struct FIELDLIST
{
	char *name;
	Type type;
	FieldList next;
}
