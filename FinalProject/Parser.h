#include <string>

class Parser
{
public:
	Parser();
	~Parser();

	void datasetProfiler(char *inFile);
	void createStorage();
	void readFromFile(char *inFile); //read input file
	
	int countNumericAttribute();
	void split();
	void sortNumericAttr(float *oneAttr, int size);
	int initCutStack(float *oneAttr, int size);
	void findCutpoints();
	void createNewRelation();
	void createNewAttributes();
	void createNewData();
	void createNewDataset();
	void writeIntoFile();

	int insNum; //number of instances
	int attrNum; //number of attributes
	int addNewAttrNum;

	std::string *relation; //store @relation information from original dataset
	std::string *newRelation;

	typedef struct
	{
		std::string label;
		std::string name;
		std::string type;
	}attrInfo;
	attrInfo *attr; //store @attribute information from original dataset
	attrInfo *numericAttr; //store splited numeric attributes
	attrInfo *nonNumericAttr; //store splited non-numeric attributes
	attrInfo *newAttr; //store new attributes based on cutpoints

	typedef struct
	{
		std::string *instance;
	}dataInfo;
	dataInfo *data; //store @data infomation from original dataset
	dataInfo *numericData; //store splited numeric data
	dataInfo *nonNumericData; //store splited non-numeric data
	dataInfo *newData;

	typedef struct
	{
		int count;
		int *sequence;
	}stack;
	stack numericStack;
	stack otherAttrStack;

	typedef struct
	{
		int cutCount;
		float *cutSequence;
	}cutStack;
	cutStack *cutPoints;

private:

};

