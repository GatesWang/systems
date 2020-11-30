
typedef struct Tokens Tokens;
typedef struct Tokens{
	char * text;
	float frequency;
	int count;
 	Tokens *next;
} Tokens;

typedef struct FileStruct FileStruct;
typedef struct FileStruct{
	char * path;
	int tokenCount;
	FileStruct *nextFile;
	Tokens *tokens;
} FileStruct;

void * processDirectory(void* directory);
char * getFullPath(char * parent, char * name);
FileStruct * createFileStruct(char *path);

void printFiles();
void * processFile(void* file);
char* readFile(FILE *f);
char* substring(const char *source, int start, int n);
Tokens *insertToken(Tokens *tokens, char *substr);
Tokens *getTokens(char *input);
void sortFiles();

void iterateFiles();
int getTokenCount(Tokens *tokens);
void calculateFrequencies(Tokens *tokens, int total);
void printTokens(Tokens *tokens);
void compareFiles();
float calculateKLD(Tokens *t1, Tokens *t2);
float get(Tokens *t, char * word);

