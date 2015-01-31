/* ------------------------------------------------ 
  * Encryption and Decryption using transpostion, 
  * reversing, and Vigenere's Cipher.
  *
  * Class: CS 141, Fall 2012.  Wed 8am lab.
  * System: Microsoft Visual Studio 2012
  * Version: August 22, 2012
  * Author: Carlos Carrillo
  * -------------------------------------------------
  */

#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

//Function Prototypes
void displayAllTheFunStuff(); // Prints out the basic stuff
void readInDictionary();  // Reads in dictionary
void readInStory(); //Reads in the long story
void allocateArrayDictionary(char**&);  // Allocates array for dictionary
void allocateArrayStory(char**&); // allocates array for story
void lookUpWord(char**); // Looks up a word to see if it's in dictionary
void encode(); // Encodes a user inputted phrase
void decodeUser(); // Decodes an encrypted code given a keyword
void decodeAssignment(); // Decodes the assignment 
char* changeDirection(char*); //Changes direction of a word
char* transposition(char*, int); //Changes each amount's value of word by same amount
void vigenereCipher(char*, char*, char*); // Uses vigenere's encryption
char* decodeVigenereCipher(char*, char*); //Decodes an encrypted message using Vigenere's Cipher
int binarySearch(char*); // Binary search of dictionary

//Global variables
const int MaxNumberOfWords = 21876; // Dictionary size
const int MaxNumberOfWords2 = 93776; // story size
const int Size = 20; // Character limit size
char** TheWords; //The dictionary
char** Analysis; //The story




int main(){
	readInStory();     //Stores Unique words only
	readInDictionary();  //Reads in Dictionary
	displayAllTheFunStuff(); /*Displays required information (name, TA, etc) 
							   and access to the menu options*/
	return 0;
}


//Reads in the dictionary from the silly.txt file
void readInDictionary(){
	ifstream inStream; // declare an input stream for my use
	int wordRow = 0; // Row for the current word

	allocateArrayDictionary(TheWords); // allocates Array for Dictionary

	inStream.open( "silly.txt");
	assert(! inStream.fail() ); // make sure file open was OK
	// Keep repeating while input from the file yields a word
	while (wordRow < MaxNumberOfWords && inStream >> TheWords[wordRow])
	{
		for (int i = 0; i < strlen(TheWords[wordRow]); i++)
			TheWords[wordRow][i] = tolower(TheWords[wordRow][i]);
		wordRow++;
	}
}


//Stores unique words from analysis.txt file
void readInStory(){
	ifstream inStream; // declare an input stream for my use
	int wordRow = 0, incrementer; // Row for the current word
	int addWord = 0;
	char* story;

	allocateArrayStory(Analysis);

	inStream.open( "TheSecretAgentByJosephConrad.txt");
	assert(! inStream.fail() ); // make sure file open was OK
	// Keep repeating while input from the file yields a word
	
	while (wordRow < MaxNumberOfWords2 && inStream >> Analysis[wordRow])
	{
		for(incrementer=0; incrementer < strlen(Analysis[wordRow]); incrementer++)
		{
			Analysis[wordRow][incrementer] = tolower(Analysis[wordRow][incrementer]);
			if(Analysis[wordRow][incrementer] >= 97 && Analysis[wordRow][incrementer] <= 122)
				Analysis[wordRow][incrementer] = Analysis[wordRow][incrementer];
		}
		wordRow++;
	}
}


//Allocates Array for the Dictionary
void allocateArrayDictionary(char** & dictionary){
	dictionary = new char*[MaxNumberOfWords];
	for(int i = 0; i < MaxNumberOfWords; i++){
		dictionary[i] = new char[ Size];
		for (int j = 0; j < Size; j++)
			dictionary[i][j] = NULL;
	}
}


//Allocates Array for the story
void allocateArrayStory(char** & story){
	story = new char*[MaxNumberOfWords2];
	for(int i = 0; i < MaxNumberOfWords2; i++){
		story[i] = new char[ Size];
		for (int j = 0; j < Size; j++)
			story[i][j] = NULL;
	}
}


//Displays menu option. Everything is done from here (encrypting/decrypting)
void displayAllTheFunStuff(){
	int choice;
	cout << "Carlos Carrillo\n";
	cout << "Program 4: Decrypt\n";
	cout << "TA: Sean Deitz Wednesday 8am Lab\n";
	cout << "November 09, 2012\n\n";
	cout << "Choose from the following options:\n";
	cout << "   1. Encode some text\n";
	cout << "   2. Decode using user-entered values\n";
	cout << "   3. Decode the ciphertext given with the assignment\n";
	cout << "   4. Exit Program\n";
	cout << "Your choice: ";
	cin >> choice;
	cout << "\n";
	switch (choice){  //Switch-case with options 1-4
	case 1: 
		encode();    
		break;
	case 2:
		decodeUser();
		break;
	case 3:
		decodeAssignment();
		break;
	case 4:
		cout << "Exiting Program...\n";
		exit(0);
	}
}


//Encrypts a user inputted message using reverse/transpose/vigenere's
void encode(){
	/*The userInput variable accepts the text to be encoded. I will then
	use newUserInput to manipulate the accepted text. I will be copying 
	the reverse, transposition, and vigenere's into newUserInput. Reason being, I want
	to remember the original text without having to reverse engineer newUserInput.*/
	char userInput[200], newUserInput[200]; 
	char direction[80], keyword[80];  //Used for the direction (Forwards/Backwards) and keyword both user inputted 
	int transpose;  //Transposition value

	cout << "Enter the text to be encoded: ";
	cin.ignore(200, '\n');
	cin.getline(userInput, 200);
	strcpy(newUserInput, userInput);
	cout << "Enter direction (Forwards or Reverse): ";
	cin >> direction;
	
	/*If user enters in reverse or Reverse, they will get sent to the reverse function
	which will reverse ever word of the string*/
	if (strcmp(direction, "reverse") == 0 || strcmp(direction, "Reverse") == 0)
		strcpy(newUserInput, changeDirection(newUserInput)); // copies reverse result
	cout << "Enter transposition value (0...25): ";
	cin >> transpose;
	strcpy(newUserInput,transposition(newUserInput, transpose)); //copies transposition result
	cout << "Enter a keyword for Vigenere encryption: ";
	cin >> keyword;
	cout << "Keyword is: " << keyword << endl << endl;
	vigenereCipher(userInput,keyword, newUserInput); //copies viginere cipher result
}


/*Accepts an encrypted message and decodes by using Vigenere's, transposition, 
and reverse. An encrypted message and a keyword are given. It'll check words 
against the dictionary in order to do this.*/
void decodeUser()
{
	char cipherText[80], vigenereKeywords[80], decodedWerdz[80];
	char reverseDecodedWerdz[80], decryptedText[80];
	char tempDecodedWerdz[80], tempReverseWerdz[80];
	char* reverseTruncateCipher[80];
	char* truncateKeyword[10], *truncateCipher[80];
	int checkSpaces = 0, position, numberOfHits = 0;
	int counter = 0, otherCounter = 0, vigenereCounter;
	int transpositionCounter = 0, checkWord;
	int checkDecipher = 0;

	cout << "Enter the cipherText to be decoded: ";
	cin.ignore(80, '\n');
	cin.getline(cipherText, 80);
	cout << "Enter the Vigenere keywords to be tried: ";
	cin.getline(vigenereKeywords, 80);
	cout << "String of possible keywords is: "<< endl << endl;;
	truncateKeyword[0] = strtok(vigenereKeywords, " ");
	
	while (truncateKeyword[counter] != NULL)
	{
		counter++;
		truncateKeyword[counter] = strtok(NULL, " ");
	}
	
	/*The string is already deciphered using Vigenere's Cipher and it is sent in 
	this nested for loop. Basically it'll transpose every string by 1. Then it
	will check against the dictionary to see if it is a word. It'll do the same
	for every word in the string.*/
	for(vigenereCounter = 0; vigenereCounter < counter; vigenereCounter++)
	{
		strcpy(decodedWerdz, decodeVigenereCipher(cipherText, truncateKeyword[vigenereCounter]));
		for(transpositionCounter = 0; transpositionCounter <= 25; transpositionCounter++)
		{
			strcpy(tempDecodedWerdz, decodedWerdz);
			truncateCipher[0] = strtok(decodedWerdz, " ");
			otherCounter = 0;
			while (truncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				truncateCipher[otherCounter] = strtok(NULL, " ");
			}

			for(checkWord = 0; checkWord < otherCounter; checkWord++)
				if(binarySearch(truncateCipher[checkWord]) >= 0)
					numberOfHits++;
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{						
				cout << "New best case. Dictionary hits: " << numberOfHits;
				cout << ", keyword: " << truncateKeyword[vigenereCounter] << ", transposition: ";
				cout << transpositionCounter << ", direction: Forwards" << endl;
				cout << tempDecodedWerdz<< endl << endl;
			}
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{
				strcpy(decryptedText, tempDecodedWerdz);
				checkDecipher = numberOfHits;
			}
			numberOfHits = 0;
			strcpy(decodedWerdz, tempDecodedWerdz);
			strcpy(decodedWerdz, transposition(decodedWerdz, 1));
			strcpy(tempDecodedWerdz, decodedWerdz);
			truncateCipher[0] = strtok(decodedWerdz, " ");
			otherCounter = 0;
			while (truncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				truncateCipher[otherCounter] = strtok(NULL, " ");
			}
			strcpy(decodedWerdz, tempDecodedWerdz);
		}
	}
	for(vigenereCounter = 0; vigenereCounter < counter; vigenereCounter++)
	{
		strcpy(reverseDecodedWerdz, decodeVigenereCipher(cipherText, truncateKeyword[vigenereCounter]));
		strcpy(reverseDecodedWerdz, changeDirection(reverseDecodedWerdz));
		for(transpositionCounter = 0; transpositionCounter <= 25; transpositionCounter++)
		{
			strcpy(tempReverseWerdz, reverseDecodedWerdz);
			reverseTruncateCipher[0] = strtok(reverseDecodedWerdz, " ");
			otherCounter = 0;
			while (reverseTruncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				reverseTruncateCipher[otherCounter] = strtok(NULL, " ");
			}
			for(checkWord = 0; checkWord < otherCounter; checkWord++)
				if(binarySearch(reverseTruncateCipher[checkWord]) >= 0)
					numberOfHits++;
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{							
				cout << "New best case. Dictionary hits: " << numberOfHits;
				cout << ", keyword: " << truncateKeyword[vigenereCounter] << ", transposition: ";
				cout << transpositionCounter << ", direction: Reverse" << endl;
				cout << tempReverseWerdz << endl << endl;
			}
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{
				strcpy(decryptedText, tempReverseWerdz);
				checkDecipher = numberOfHits;
			}
			numberOfHits = 0;
			strcpy(reverseDecodedWerdz, tempReverseWerdz);
			strcpy(reverseDecodedWerdz, transposition(reverseDecodedWerdz, 1));
			strcpy(tempReverseWerdz, reverseDecodedWerdz);
			reverseTruncateCipher[0] = strtok(reverseDecodedWerdz, " ");
			otherCounter = 0;
			while (reverseTruncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				reverseTruncateCipher[otherCounter] = strtok(NULL, " ");
			}

			strcpy(reverseDecodedWerdz, tempReverseWerdz);
		}
	}
	cout << "Decrypted text is: " << endl;
	cout << decryptedText << endl;
}


/*This whole function DOES THE EXACT SAME THING as the decodeUser function 
except it does more iterations because it has to find the keyword from 
the analysis.txt file*/
void decodeAssignment(){
	char assignment[42] = {"iqhn oh xalm ejg jyoq nbgnrv kmbqj ztnzzv"};
	char decodedWerdz[80];
	char reverseDecodedWerdz[80], decryptedText[80];
	char tempDecodedWerdz[80], tempReverseWerdz[80];
	char* reverseTruncateCipher[80], *truncateCipher[80];
	int checkSpaces = 0, position, numberOfHits = 0;
	int counter = 0, otherCounter = 0, vigenereCounter;
	int transpositionCounter = 0, checkWord;
	int checkDecipher = 0;

	cout << "cipherText is: IQHN OH XALM EJG JYOQ NBGNRV KMBQJ ZTNZZV\n\n";
	cout << "String of possible keywords taken from file: analysis.txt\n\n";

	for(vigenereCounter = 0; vigenereCounter < MaxNumberOfWords2; vigenereCounter++)
	{
		strcpy(decodedWerdz, decodeVigenereCipher(assignment, Analysis[vigenereCounter]));
		for(transpositionCounter = 0; transpositionCounter <= 25; transpositionCounter++)
		{
			strcpy(tempDecodedWerdz, decodedWerdz);
			truncateCipher[0] = strtok(decodedWerdz, " ");
			otherCounter = 0;
			while (truncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				truncateCipher[otherCounter] = strtok(NULL, " ");
			}

			for(checkWord = 0; checkWord < otherCounter; checkWord++)
				if(binarySearch(truncateCipher[checkWord]) >= 0)
					numberOfHits++;
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{						
				cout << "\n\nNew best case. Dictionary hits: " << numberOfHits;
				cout << ", keyword: " << Analysis[vigenereCounter] << ", transposition: ";
				cout << transpositionCounter << ", direction: Forwards" << endl;
				cout << tempDecodedWerdz<< endl << endl;
			}
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{
				strcpy(decryptedText, tempDecodedWerdz);
				checkDecipher = numberOfHits;
			}
			numberOfHits = 0;
			strcpy(decodedWerdz, tempDecodedWerdz);
			strcpy(decodedWerdz, transposition(decodedWerdz, 1));
			strcpy(tempDecodedWerdz, decodedWerdz);
			truncateCipher[0] = strtok(decodedWerdz, " ");
			otherCounter = 0;
			while (truncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				truncateCipher[otherCounter] = strtok(NULL, " ");
			}
			strcpy(decodedWerdz, tempDecodedWerdz);
		}
	}
	for(vigenereCounter = 0; vigenereCounter < MaxNumberOfWords2; vigenereCounter++)
	{
		strcpy(reverseDecodedWerdz, decodeVigenereCipher(assignment, Analysis[vigenereCounter]));
		strcpy(reverseDecodedWerdz, changeDirection(reverseDecodedWerdz));
		for(transpositionCounter = 0; transpositionCounter <= 25; transpositionCounter++)
		{
			strcpy(tempReverseWerdz, reverseDecodedWerdz);
			reverseTruncateCipher[0] = strtok(reverseDecodedWerdz, " ");
			otherCounter = 0;
			while (reverseTruncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				reverseTruncateCipher[otherCounter] = strtok(NULL, " ");
			}
			for(checkWord = 0; checkWord < otherCounter; checkWord++)
				if(binarySearch(reverseTruncateCipher[checkWord]) >= 0)
					numberOfHits++;
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{							
				cout << "\n\nNew best case. Dictionary hits: " << numberOfHits;
				cout << ", keyword: " << Analysis[vigenereCounter] << ", transposition: ";
				cout << transpositionCounter << ", direction: Reverse" << endl;
				cout << tempReverseWerdz << endl << endl;
			}
			if(numberOfHits > 0 && checkDecipher < numberOfHits)
			{
				strcpy(decryptedText, tempReverseWerdz);
				checkDecipher = numberOfHits;
			}
			numberOfHits = 0;
			strcpy(reverseDecodedWerdz, tempReverseWerdz);
			strcpy(reverseDecodedWerdz, transposition(reverseDecodedWerdz, 1));
			strcpy(tempReverseWerdz, reverseDecodedWerdz);
			reverseTruncateCipher[0] = strtok(reverseDecodedWerdz, " ");
			otherCounter = 0;
			while (reverseTruncateCipher[otherCounter] != NULL)
			{
				otherCounter++;
				reverseTruncateCipher[otherCounter] = strtok(NULL, " ");
			}

			strcpy(reverseDecodedWerdz, tempReverseWerdz);
		}
	}
	cout << "Decrypted text is: " << endl;
	cout << decryptedText << endl;
}


//Transposes each character of a word
char* transposition(char* userInput, int transpose){
	for(int value = 0; userInput[value] != '\0'; value++)
	{
		if(userInput[value] != 32)          //if the value is not a space
			userInput[value] += transpose; // it will add the transpose value
			if((unsigned char)userInput[value] > 122)  //however if the value is above the 'z' value
				userInput[value] = (userInput[value] - 122) + 96; //on the ASCII table it will start back
	}                                                             //from the 'a' value on the ASCII table
	return (userInput);  //Returns final result
}


//Reverses every word of the string
char* changeDirection(char* userInput)
{
	char reverseWord[80], tempUserInput[80]; //Used to store the reversed version of the string
	char* tempString[80]; //Used to store every word in an array
	
	strcpy(tempUserInput, userInput);

	int i = 0, y = 0;     //Counter variables for the for loop

   	tempString[0] = strtok(tempUserInput, " ");  //Using strtok to break tempUserInput
	while(tempString[i] != NULL)             //into a 2D char array
	{
		i++;
		tempString[i] = strtok(NULL, " ");
	}
	/*This is where the reverse happens.*/
	for(int a = 0; a < i; a++)   
		for(int x = strlen(tempString[a])-1; x >= 0; x--, y++)//while x is decrementing
		{                                             //y is incrementing storing the word
			reverseWord[y] = tempString[a][x];        //reversed into reverseWord. It'll do
			if (x == 0)                               //do this for every word in the 2d 
			{                                         //char array
				reverseWord[y+1] = ' ';
				y++;
			}
		}
	reverseWord[y] = '\0'; //adds a null character at the end of the string
	return(reverseWord);//returns the word reversed
}


//Uses Vigenere's Cipher to complete the encryption part of the assignment
void vigenereCipher(char* userInput, char* keyword, char* newUserInput)
{
	int longKeyword= strlen(keyword), valueOfKeyword = 0, l = 0;      
	int shortKeyword, vigenereLetter, stringLength = strlen(keyword);
	char cipherText[80], newKeyword[80];
	if(strlen(newUserInput) > strlen(keyword)) //Checks the length of the user input vs the keyword. If user
	{                                          //input is longer, then the keyword is lengthened to match the size
		for(longKeyword, valueOfKeyword; newUserInput[longKeyword] != '\0'; longKeyword++, valueOfKeyword++)
		{
			keyword[longKeyword] = keyword[valueOfKeyword];
			if(valueOfKeyword == stringLength) //Everytime the value reaches the string length
				valueOfKeyword = 0;            //it resets the keyword array to 0
		}
		keyword[longKeyword] = '\0';           //adding null characting to end of string
		for(vigenereLetter = 0; newUserInput[vigenereLetter] != '\0'; vigenereLetter++)
			if (newUserInput[vigenereLetter] != 32) //As long as the character doesn't equal a space, it'll change the letter.
				cipherText[vigenereLetter] = ((newUserInput[vigenereLetter]-97) + (keyword[vigenereLetter] - 97)) % 26 + 97; // formula for changing letter
			else  // if the character does equal a space, it'll make a space
				cipherText[vigenereLetter] = ' ';
		cipherText[vigenereLetter] = '\0';   //will add null character at the end of the string
	}
	else if (strlen(newUserInput) <= strlen(keyword)) //This part is the same as above only that if the user input is shorter
	{                                                 //than the keyword, it'll shorten the keyword to match the user input. 
		for(shortKeyword = 0; shortKeyword < strlen(userInput); shortKeyword++)
			newKeyword[shortKeyword] = keyword[shortKeyword];
		newKeyword[shortKeyword] = '\0';
		for(vigenereLetter = 0;  vigenereLetter < strlen(userInput); vigenereLetter++)
			cipherText[vigenereLetter] = ((newUserInput[vigenereLetter]-97) + (newKeyword[vigenereLetter] - 97)) % 26 + 97;
		cipherText[vigenereLetter] = '\0';
	}
	cout << "Keyword, plainText, and cipherText are:\n";
	if(strlen(userInput) > stringLength)  //Will output the correct keyword based on the
		cout << keyword << endl;          //length of the user input
	else
		cout << newKeyword << endl;
	cout << userInput << endl;
	cout << cipherText << endl;
}


//Decrypts a message using Vigenere's Cipher
char* decodeVigenereCipher(char* cipherText, char* keyword)
{
	char plainText[80], newKeyword[80], vigenereFunctionKeyword[80];
	int vigenereLetter = 0, valueOfKeyword = 0, longKeyword;
	int stringLength, shortKeyword;

	strcpy(vigenereFunctionKeyword, keyword);  // don't want to manipulate the keyword so I am using another 
	longKeyword= strlen(vigenereFunctionKeyword); //variable in place of it. I am setting values
	stringLength = strlen(vigenereFunctionKeyword);

	if(strlen(cipherText) > strlen(vigenereFunctionKeyword))
	{
		for(longKeyword, valueOfKeyword; cipherText[longKeyword] != '\0'; longKeyword++, valueOfKeyword++)
		{
			vigenereFunctionKeyword[longKeyword] = vigenereFunctionKeyword[valueOfKeyword];
			if(valueOfKeyword == stringLength)
				valueOfKeyword = 0;
		}
		vigenereFunctionKeyword[longKeyword] = '\0';
		for(vigenereLetter; vigenereLetter < strlen(cipherText); vigenereLetter++)
		{
			if (cipherText[vigenereLetter] != 32)
			{
				if((cipherText[vigenereLetter]-97) - (vigenereFunctionKeyword[vigenereLetter]-97) < 0)
					plainText[vigenereLetter] = ((cipherText[vigenereLetter]-71) - (vigenereFunctionKeyword[vigenereLetter]-97)) %26 + 97;
				else
					plainText[vigenereLetter] = ((cipherText[vigenereLetter]-97) - (vigenereFunctionKeyword[vigenereLetter]-97)) %26 + 97;
			}
			else
				plainText[vigenereLetter] = ' ';
		}
		plainText[vigenereLetter] = '\0';
	}
	else if (strlen(cipherText) <= strlen(keyword))
	{
		for(shortKeyword = 0; shortKeyword < strlen(cipherText); shortKeyword++)
			newKeyword[shortKeyword] = keyword[shortKeyword];
		newKeyword[shortKeyword] = '\0';
		for(vigenereLetter = 0;  vigenereLetter < strlen(cipherText); vigenereLetter++)
			plainText[vigenereLetter] = ((cipherText[vigenereLetter]-71) - (newKeyword[vigenereLetter] - 97)) % 26 + 97;
		plainText[vigenereLetter] = '\0';
	} 
	return (plainText);
}


//Does a binary search check against the dictionary
int binarySearch(char* searchWord)
{
	int low, mid, high;     // array indices for binary search
	int searchResult;       // Stores index of word if search succeeded, else -1


	// Binary search for word
	low = 0;
	high = MaxNumberOfWords - 1;
	while ( low <= high)  
	{
		mid = (low + high) / 2;
		// searchResult negative value means word is to the left, positive value means
		// word is to the right, value of 0 means word was found
		searchResult = strcmp( searchWord, TheWords[ mid]);	
		if ( searchResult == 0)  
			return mid;   // word IS in the dictionary    
		else if (searchResult < 0)                       
			high = mid - 1; // word should be located prior to mid location
		else
			low = mid + 1; // word should be located after mid location
	}
   return -1;       // Did not find the word
}//end binarySearch()