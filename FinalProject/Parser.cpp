#include <iostream>
#include <fstream>
#include <sstream>
//#include <stdlib.h>
//#include <stdio.h>

#include "Parser.h"

Parser::Parser()
{
	insNum = 0;
	attrNum = 0;
	addNewAttrNum = 0;
}
Parser::~Parser()
{
}

void Parser::datasetProfiler(char *inFile)
{
	std::ifstream is(inFile);
	std::string line;
	while (is.good())
	{
		std::getline(is, line);
		if (line.find("@ATTRIBUTE") == 0 || line.find("@attribute") == 0)
		{
			attrNum++;
			continue;
		}
		if (line.find("@DATA") == 0 || line.find("@data") == 0)
			break;
		else
			continue;
	}
	while (is.good())
	{
		std::getline(is, line);
		if (line == "")
			continue;
		else
			insNum++;
		
	}
	is.close();
	std::cout << attrNum << " " << insNum << std::endl;
}

void Parser::createStorage()
{
	relation = new std::string[2];
	attr = new attrInfo[attrNum];
	data = new dataInfo[insNum];
	for (int i = 0; i < insNum; i++)
	{
		data[i].instance = new std::string[attrNum];
	}
}

void Parser::readFromFile(char *inFile)
{
	int attrLocator(0);
	int insLocator(0);
	int flag(0);

	datasetProfiler(inFile);
	createStorage();
	std::ifstream is(inFile);
	std::string line;
	
	while (is.good())
	{
		std::getline(is, line);
		if (line.find("@RELATION") == 0 || line.find("@relation") == 0)
		{
			std::stringstream iss(line);
			iss >> relation[0] >> relation[1];
			continue;
		}
		if (line.find("@ATTRIBUTE") == 0 || line.find("@attribute") == 0)
		{

			std::size_t found1 = line.find(' ');
			std::string str1 = line.substr(0, found1);
			attr[attrLocator].label = str1;
			std::string str2 = line.substr(found1+1);

			std::size_t found2 = str2.find(' ');
			str1 = str2.substr(0, found2);
			attr[attrLocator].name = str1;
			attr[attrLocator].type = str2.substr(found2 + 1);
			attrLocator++;
			continue;
		}
		if (line.find("@DATA") == 0 || line.find("@data") == 0)
			break;
		else
			continue;
	}
	
	while (is.good())
	{
		if (insLocator == insNum)
			break;
		for (int i = 0; i < attrNum; i++)
		{
			if (i == attrNum - 1)
			{
				std::getline(is, line);
			}
			else
			{
				std::getline(is, line, ',');
			}
			data[insLocator].instance[i] = line;
		}
		insLocator++;
	}

/*	std::cout << "== Data summery ==" << std::endl;
	std::cout << relation[0] << " " << relation[1] << std::endl;
	for (int i = 0; i < attrNum; i++)
	{
		std::cout << attr[i].label << " " << attr[i].name << " " << attr[i].type << std::endl;
	}
	for (int i = 0; i < insNum; i++)
	{
		for (int j = 0; j < attrNum; j++)
		{
			std::cout << data[i].instance[j] << ",";
		}
		std::cout << "\n";
	}
	*/
}

int Parser::countNumericAttribute()
{
	int numericAttrNum(0); //number of numeric attributes
	for (int i = 0; i < attrNum; i++)
	{
		if (attr[i].type == "NUMERIC" || attr[i].type == "numeric" ||
			attr[i].type == "REAL" || attr[i].type == "real" ||
			attr[i].type == "INTEGER" || attr[i].type == "integer")
			numericAttrNum++;
		else
			continue;
	}
	return numericAttrNum;
}

void Parser::split()
{
	int i(0), j(0), k(0);
	int numericCount = countNumericAttribute();

	numericStack.count = numericCount;
	numericStack.sequence = new int[numericStack.count];
	otherAttrStack.count = attrNum - numericCount;
	otherAttrStack.sequence = new int[otherAttrStack.count];

	for (i = 0; i < attrNum; i++)
	{
		if (attr[i].type == "NUMERIC" || attr[i].type == "numeric" || 
			attr[i].type == "REAL" || attr[i].type == "real" ||
			attr[i].type == "INTEGER" || attr[i].type == "integer")
		{
			numericStack.sequence[j] = i;
			j++;
		}
		else
		{
			otherAttrStack.sequence[k] = i;
			k++;
		}
	}
/*	for (j = 0; j < numericStack.count; j++)
		std::cout << numericStack.sequence[j];
	std::cout << "\n";
	for (k = 0; k < otherAttrStack.count; k++)
		std::cout << otherAttrStack.sequence[k];
		*/

	numericAttr = new attrInfo[numericStack.count];
	for (j = 0; j < numericStack.count; j++)
	{
		numericAttr[j] = attr[numericStack.sequence[j]];
	}

	nonNumericAttr = new attrInfo[otherAttrStack.count];
	for (k = 0; k < otherAttrStack.count; k++)
	{
		nonNumericAttr[k] = attr[otherAttrStack.sequence[k]];
		if (nonNumericAttr[k].label == "@attribute")
			nonNumericAttr[k].label = "@ATTRIBUTE";
	}
	//print out numeric and non-numeric attribute info that we splited
/*	for (int j = 0; j < numericStack.count; j++)
	{
		std::cout << numericAttr[j].label << " " << numericAttr[j].name << " " << numericAttr[j].type << std::endl;
	}
	std::cout << "\n";
	for (int k = 0; k < otherAttrStack.count; k++)
	{
		std::cout << nonNumericAttr[k].label << " " << nonNumericAttr[k].name << " " << nonNumericAttr[k].type << std::endl;
	}*/

	numericData = new dataInfo[insNum];
	for (i = 0; i < insNum; i++)
	{
		numericData[i].instance = new std::string[numericStack.count];
		for (j = 0; j < numericStack.count; j++)
		{
			numericData[i].instance[j] = data[i].instance[numericStack.sequence[j]];
		}
	}

	nonNumericData = new dataInfo[insNum];
	for (i = 0; i < insNum; i++)
	{
		nonNumericData[i].instance = new std::string[otherAttrStack.count];
		for (k = 0; k < otherAttrStack.count; k++)
		{
			nonNumericData[i].instance[k] = data[i].instance[otherAttrStack.sequence[k]];
		}
	}
	//print out data info that we splited
/*	for (int i = 0; i < insNum; i++)
	{
		for (int j = 0; j < numericStack.count; j++)
		{
			std::cout << numericData[i].instance[j] << ",";
		}
		std::cout << "\n";
	}
	for (int i = 0; i < insNum; i++)
	{
		for (int k = 0; k < otherAttrStack.count; k++)
		{
			std::cout << nonNumericData[i].instance[k] << ",";
		}
		std::cout << "\n";
	}*/
	delete[] numericStack.sequence;
	delete[] otherAttrStack.sequence;
}

void Parser::sortNumericAttr(float *oneAttr, int size)
{
	float value;
	for (int i = 0; i < size; i++)
	{
		for (int j = 1; j < size - i; j++)
		{
			if (oneAttr[j] < oneAttr[j - 1])
			{
				value = oneAttr[j];
				oneAttr[j] = oneAttr[j - 1];
				oneAttr[j - 1] = value;
			}
		}
	}
}

int Parser::initCutStack(float *oneAttr, int size)
{
	int a(0);
	for (int i = 1; i < size; i++)
	{
		if (oneAttr[i] != oneAttr[i - 1])
			a++;
		else continue;
	}
	return a;
}

void Parser::findCutpoints()
{
	std::string::size_type sz;
	float **transArray;
	transArray = new float *[numericStack.count];
	for (int i = 0; i < numericStack.count; i++)
	{
		transArray[i] = new float[insNum];
	}
	for (int i = 0; i < numericStack.count; i++)
		for (int j = 0; j < insNum; j++)
			transArray[i][j] = std::stof(numericData[j].instance[i],&sz);

	//std::cout << transArray[0][0] + transArray[2][0] << std::endl; //test value conversion

	for (int i = 0; i < numericStack.count; i++)
	{
		sortNumericAttr(transArray[i], insNum);
	}

/*	for (int i = 0; i < numericStack.count; i++) //print out the results after sorting
	{
		for (int j = 0; j < insNum; j++)
		{
			std::cout << transArray[i][j] << " ";
		}
		std::cout << "\n";
	}*/

	cutPoints = new cutStack[numericStack.count];
	for (int i = 0; i < numericStack.count; i++)
	{
		int cutNum;
		float cutValue;
		int k(0);
		cutNum = initCutStack(transArray[i], insNum);
		cutPoints[i].cutCount = cutNum;
		cutPoints[i].cutSequence = new float[cutNum];
		for (int j = 1; j < insNum; j++)
		{
			if (transArray[i][j] != transArray[i][j - 1])
			{
				cutValue = (transArray[i][j] + transArray[i][j - 1]) / 2;
				cutPoints[i].cutSequence[k] = cutValue;
				k++;
			}
			else continue;
		}
	}
/*	for (int i = 0; i < numericStack.count; i++)
	{
		for (int j = 0; j < cutPoints[i].cutCount; j++)
		{
			std::cout << cutPoints[i].cutSequence[j] << " ";
		}
		std::cout << "\n";
	}*/

	delete[] transArray;
}

void Parser::createNewRelation()
{
	newRelation = new std::string[2];
	newRelation[0] = relation[0];
	if (newRelation[0] == "@relation")
		newRelation[0] = "@RELATION";
	newRelation[1] = "new_" + relation[1];
}

void Parser::createNewAttributes()
{
	int k(0);
	std::string s;

	for (int i = 0; i < numericStack.count; i++)
	{
		if (cutPoints[i].cutCount == 0)
			addNewAttrNum += 1;
		else
			addNewAttrNum += cutPoints[i].cutCount;
	}

	newAttr = new attrInfo[addNewAttrNum];

	for (int i = 0; i < numericStack.count; i++)
	{
		if (cutPoints[i].cutCount == 0)
		{
			newAttr[k].label = "@ATTRIBUTE";
			newAttr[k].name = numericAttr[i].name;
			newAttr[k].type = numericAttr[i].type;
			k++;
		}
		else
		{
			for (int j = 0; j < cutPoints[i].cutCount; j++)
			{
				s = std::to_string(cutPoints[i].cutSequence[j]);

				newAttr[k].label = "@ATTRIBUTE";
				newAttr[k].name = numericAttr[i].name + "_" + s;
				newAttr[k].type = "{<" + s + "," + ">" + s + "}";
				k++;
			}
		}
	}
		
	for (int i = 0; i < addNewAttrNum; i++)
		std::cout << newAttr[i].label << " " << newAttr[i].name << " " << newAttr[i].type <<std::endl;
	for (int i = 0; i < otherAttrStack.count; i++)
		std::cout << nonNumericAttr[i].label << " " << nonNumericAttr[i].name << " " << nonNumericAttr[i].type << std::endl;
}

void Parser::createNewData()
{
	std::string::size_type sz;
	std::string s;
	
	float value;

	newData = new dataInfo[insNum];
	for (int i = 0; i < insNum; i++)
		newData[i].instance = new std::string[addNewAttrNum + otherAttrStack.count];

	for (int k = 0; k < insNum; k++)
	{
		int g = 0;
		for (int i = 0; i < numericStack.count; i++)
		{
			if (cutPoints[i].cutCount == 0)
			{
				newData[k].instance[g] = numericData[k].instance[i];
				g++;
			}
			else
			{
				for (int j = 0; j < cutPoints[i].cutCount; j++)
				{
					value = std::stof(numericData[k].instance[i], &sz);
					s = std::to_string(cutPoints[i].cutSequence[j]);
					if (value < cutPoints[i].cutSequence[j])
					{

						newData[k].instance[g] = "<" + s;
					}
					else
					{
						newData[k].instance[g] = ">" + s;
					}
					g++;
				}
			}
		}
			
		for (int h = 0; h < otherAttrStack.count; h++)
		{
			newData[k].instance[g] = nonNumericData[k].instance[h];
			g++;
		}
	}
	
/*	for (int i = 0; i < insNum; i++)
	{
		for (int j = 0; j < addNewAttrNum + otherAttrStack.count; j++)
		{
			std::cout << newData[i].instance[j] << ",";
		}
		std::cout << "\n";
	}*/
}

void Parser::createNewDataset()
{
	createNewRelation();
//	std::cout << newRelation[0] << " " << newRelation[1] << std::endl;
	createNewAttributes();
	createNewData();
}

void Parser::writeIntoFile()
{
	std::ofstream ofs("irisNew.arff", std::ofstream::out);
	ofs << newRelation[0] << " " << newRelation[1] << "\n";
	ofs << "\n";
	for (int i = 0; i < addNewAttrNum; i++)
	{
		ofs << newAttr[i].label << " " << newAttr[i].name << " " << newAttr[i].type << "\n";
	}
	for (int i = 0; i < otherAttrStack.count; i++)
	{
		ofs << nonNumericAttr[i].label << " " << nonNumericAttr[i].name << " " << nonNumericAttr[i].type << "\n";
	}
	ofs << "\n";
	ofs << "@DATA";
	ofs << "\n";
	for (int i = 0; i < insNum; i++)
		for (int j = 0; j < addNewAttrNum + otherAttrStack.count; j++)
		{
			if (j == addNewAttrNum + otherAttrStack.count - 1)
			{
				ofs << newData[i].instance[j] << "\n";
			}
			else
				ofs << newData[i].instance[j] << ",";
		}
	delete[] relation;
	delete[] newRelation;
	delete[] attr;
	delete[] numericAttr;
	delete[] nonNumericAttr;
	delete[] newAttr;
	delete[] data;
	delete[] numericData;
	delete[] nonNumericData;
	delete[] newData;
	delete[] cutPoints;

}