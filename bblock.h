
enum blockType {
	ENTRY = 1,
	EXIT = 2,
	ORD = 3
};

enum basics {
	B_FUNCTION = 1,
	B_PRINT = 2,
	B_EVAL = 3
};

enum StatementType {
	ST_PARAM = 1,
	ST_REG= 3,
	ST_CALL = 4,
	ST_OP = 5,
	ST_RETURN = 6,
	ST_PRINT = 7,
	ST_EVAL = 8,
	ST_EXIT = 9,
	ST_GET_REG= 10,
	ST_CONST= 11,
	ST_SUB_BLOCK_REG = 12
};

struct available_regs
{
	struct statment_value* asgn;
	struct available_regs* next;
	struct available_regs* prev;
};

struct statment_value {
	int RGnum;
	int SSAnum;
	enum StatementType type;
	char* title;
	int returnType;
	int token;
	int isNot;
	struct statment_value* op_values_next;
};

struct statment_s{
	int index;
	struct basic_block* block;
	struct statment_value* LHS;
	struct statment_value* RHS;
	struct statment_s* next;
	struct statment_s* prev;
};

struct condition_value
{
	struct statment_value* value;
	struct condition_value* next;
	struct condition_value* prev;
};

struct condition_chain{	
	struct condition_value* first;
	struct condition_value* last;
	int count;
};

struct basic_block{
	int index;
	int bbNum;
	int startingNode;
	int endingNode;
	int visited;
	int returnType;
	enum blockType type;
	struct statment_s* firstStmt;
	struct statment_s* lastStmt;
	struct condition_chain* conditionChain;
	struct statment_value* cond;
	struct basic_block* parent;
	struct basic_block* firstBB;
	struct basic_block* secondBB;
	struct basic_block* closureBB;
    struct basic_block* next; 
    struct basic_block* prev; 
};

struct FunctionBlock { 
	char* title;
	enum basics type;
	struct available_regs* firstAvailableReg; 
	struct available_regs* lastAvailableReg; 
	struct ssaList* registersSSA;
    struct basic_block* firstBlock;
    struct basic_block* lastBlock; 
    struct FunctionBlock* next; 
    struct FunctionBlock* prev; 
}; 

struct FunctionBlock* firstFunction;
struct FunctionBlock* lastFunction;
struct ssaList* argumentsSSA;
struct ssaList* returnsSSA;
struct ssaList* programRegistersSSA;

extern void initBasicBlocks();
extern void addOperand(struct statment_value* operand, struct ast* node);
extern struct FunctionBlock* createFunctionBlock(struct basic_block* block, enum basics type, char* title);
extern struct FunctionBlock* creatEntry(int id, char* title, enum basics type, int startingNode, int endingNode, int returnType);
extern struct basic_block* creatExit(struct basic_block* parentBlock,int id);
extern struct basic_block* creatSubBlock(struct basic_block* parentBlock,int id,int bbNum, int startingNode, int endingNode);
extern struct statment_value* createStatment(struct basic_block* block, enum StatementType type , struct ast* node, int noRegNumUpdate);
extern struct statment_s* AddStatement(struct FunctionBlock* funcBlock, struct basic_block* block, struct statment_value* LHS, struct statment_value* RHS,  struct ast* node, int noRegNumUpdated);
extern void insertStatement(struct basic_block* block,struct statment_s* statment);
extern void insertStatementValue(struct basic_block* block,struct statment_s* statment);
extern void AddIFBranching(struct basic_block* block, struct basic_block* block1, struct basic_block* block2, struct basic_block* closureBlock );
extern void AddBranching(struct basic_block* block, struct basic_block* branching);
extern void updateRegNum(struct basic_block* block);
extern struct statment_s*  cpyS(struct basic_block* block, struct statment_s*  cpyStat);
extern struct statment_value* cpyStatment(struct statment_value* oldStat);
extern void resetRegCount();
extern void generatePath(struct basic_block* block);
extern void addStatmentValueToChain(struct condition_chain* chain, struct statment_value* reg);
extern struct statment_value* createNotReg( struct statment_value* reg);
extern void freeConditionChain(struct basic_block* block);
extern void AddBlockToFunction(struct basic_block* block);
extern int visit_cfg(int (*f)(struct FunctionBlock* function_block));
extern struct ssaList* generateSSA(struct FunctionBlock* FunctionBlock);
extern void free_statment_value(struct statment_value* statment_val);
extern void free_statment(struct statment_s* statment);
extern void free_block(struct FunctionBlock* functionBlock, struct basic_block* block);
extern struct FunctionBlock* getFunctionByTitle(char* title);
extern void removeAvailableAsgn(struct FunctionBlock* functionBlock,struct statment_value* value);
extern void inserAsgn(struct FunctionBlock* functionBlock, struct statment_value* value);
extern void lookupInsertAsgn(struct FunctionBlock* functionBlock, struct statment_value* value);
extern void createAvailableAsgnList();
extern void clearAvailableAsgn(struct FunctionBlock* functionBlock);
