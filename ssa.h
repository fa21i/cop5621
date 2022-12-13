struct usedChain{
	struct statment_s* statment;
	struct usedChain* next;
	struct usedChain* prev;
};
struct ssaItem{
	int ssaNum;
	struct statment_value* reg;
	struct statment_s* statment;
	struct usedChain* firstUsed;
	struct usedChain* lastUsed;
	struct ssaItem* next;
	struct ssaItem* prev;
};

struct ssa{
	int ssaReached;
	int rgNum;
	struct ssaItem* firstItem;
	struct ssaItem* lastItem;
	struct ssa* next;
	struct ssa* prev;
};

struct ssaList{
	struct ssa* firstSSA;
	struct ssa* lastSSA;
	enum StatementType listRegType;
};

struct SSABlock{
	struct ssaList* regeistersList;
	struct ssaList* argumentsList;
	struct ssaList* returnList;
};


extern void checkUsedInStatments(struct ssaItem* item,struct basic_block* block, struct statment_s* statment);
extern void addUsedInStatment(struct ssaItem* item, struct statment_s* statment);
// extern void removeUsedIntStatment(struct ssaItem* item, struct statment_s* statment);
extern struct ssaList* createSSAlist(enum StatementType listRegType);
extern struct ssa* createSSA(struct statment_value* value);
extern struct ssaItem* createSSAItem(struct statment_value* value, struct statment_s* statment, int num);
extern int removeSSAItem(struct ssa* tmp, struct ssaItem* item, struct statment_value* value);
extern int removeSSA(struct ssa* tmp, struct ssaList* list);
extern int lookupRemoveSSAItem(struct ssaList* list, struct statment_value* value);
extern void updateSSA(struct ssa* item);
extern void clearAllSSAlists(struct SSABlock* block);
extern void clearList(struct ssaList* list);
extern struct ssa* insertSSA(struct ssaList* list, struct statment_s* statment, struct statment_value* value);
extern struct ssaItem* lookupReg(struct ssaList* list, struct statment_value* reg);
extern struct ssaItem* insertReg(struct ssa* item, struct statment_s* statment, struct statment_value* value);
extern struct ssaItem* lookupInsertReg(struct ssaList* list, struct statment_s* statment, struct statment_value* value);
extern struct ssaList* getListByType(struct SSABlock* block, enum StatementType listRegType);
extern int visit_SSA(struct ssaList* list, int (*f)(struct ssa* ssa));