#include<string>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <iomanip>
using namespace std;

typedef char String[25];

//Struct to assign Book Record fields 
struct BookRec
{
	unsigned int isbn;
	String name;
	String author;
	int onhand;
	float price;
	String type;
};

//Declaring a datatype of TransactionType where the datatype is the set of all possible transactions 
enum TransactionType {Add, Delete, ChangeOnhand, ChangePrice};

//Struct to assign the Transaction Record fields
struct TransactionRec
{
	TransactionType ToDo;
	BookRec B;
};


// Method to output a Book Record to a file
	void writeBookRec( BookRec &buffer, fstream &outfile)
	{
		outfile.write((char *) &buffer, sizeof(BookRec));
	}

// Method to read a Book Record from a file
	void readBookRec( BookRec &buffer, fstream &outfile)
	{
		outfile.read((char *) &buffer, sizeof(BookRec));
	}


/*Outputs the current Book record being pointed to, to the output stream to be displayed on the screen*/
void outputBookRec(BookRec buffer)
{

	if( buffer.isbn < 1000000000)           /*Checks to see if isbn has a 0 at the end*/
	{
		cout << "0";
		cout.width(8);
	}
	cout << right<<  buffer.isbn;          /*Writes the ISBN of the current Record to the file*/

	cout.width(21);
	cout << right << buffer.name;          /*Writes the name of the current Record to the file*/

	cout.width(24);
	cout << right <<  buffer.author;          /*Writes the author of the current Record to the file*/

	cout.width(4);
	cout << right << buffer.onhand;          /*Writes the onhand of the current Record to the file*/
	
	cout.width(7);
	cout << right <<  buffer.price;          /*Writes the price of the current Record to the  output stream*/


	cout.width(10);
	cout << right <<  buffer.type ;         	 /*Writes the typr of the current Record to the output stream*/

	cout << endl;

}
//MAIN METHOD
// The main method runs the program. It reads through the input files (Transaction File, Master File), makes the new master file and the error file and outputs the records from the new master into the display.
int main (int argc, char * argv[])
{
	system("clear");
	
	// Check to see if empty master file
	fstream Mfile(argv[1], ios::in | ios::binary );			//Open the master file
	Mfile.seekg(0, ios::end);
	if (Mfile.tellg() == -1)
	{
		cout << "The master file is empty.  Program terminated" << endl;
		return 0;
	}
	Mfile.close();
	
	// Check to see if empty transaction file
	fstream Tfile(argv[1], ios::in | ios::binary );			//Open the master file
	Mfile.seekg(0, ios::end);
	if (Mfile.tellg() == -1)
	{
		cout << "The transaction file is empty.  Program terminated" << endl;
		return 0;
	}
	Tfile.close();
	
	//Declare how variables should be displayed
	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	cout.precision(2);

	  string fileNameM;	//Master File Name
	  string fileNameNewM;	// New Master File Name
	  string fileNameT;	// Transaction Name
	// If theres no parameters ask user for input files
	  if (argc==1)
  	{
       		 cout<<"Please enter the name of the master file, the transaction file and the new master file in this order?"<<endl;
       		 cout<<endl;
          	 system("ls");
        	 cout<<endl;
       	       	 cout<<"?> ";
        	 cin>>fileNameM;
        	 cin>>fileNameNewM;
        	 cin>>fileNameT;
  } 	//Store the input files from the command line arguemnt if they are passed in
  	else if (argc ==4)
  	{
        	fileNameM = argv[1];
        	fileNameT = argv[2];
        	fileNameNewM = argv[3];
		
  	}
	else // CLose the program if the right number of files is not passed throught the command line
	{
		cout << "A required file was not specified.\nProgram terminated" << endl;
		return 0;
	}
	


	fstream infile(fileNameM.c_str(), ios::in | ios::binary );			//Open the master file
	fstream outfile(fileNameNewM.c_str(), ios::out | ios::binary );		//Open the new master file
	fstream transactfile(fileNameT.c_str(), ios::in | ios::binary );	// Open the transaction file
	fstream errorfile("ERRORS", ios::out );		//Create an ERROR output file
	outfile.clear();	//Clear the contents of the new master file
	errorfile.clear();	// Clear the contents of the ERROR file



	BookRec bufferM;		//Create a Book record buffer to store a Book Record from the master file
	infile.read((char *) &bufferM, sizeof(BookRec)); // Read the first Book Record of the masterfile
	TransactionRec bufferT;	//Create a Transaction record buffer to store a Transaction Record from the transaction file
	BookRec bufferN;	//Create a temporary Book record buffer to store an updated Book Record that would be outputted to the new master file
	bool addLastRecord = false;	//Boolean Variable used to see whether the last record in the master file has been read
	int Tcount = 1;			// Variable to keep track of the transaction number of the transaction record being read
	
	//Reads in the transaction file and keeps looping until it reaches the last transaction file
       while (transactfile.read((char *) &bufferT, sizeof(TransactionRec)) )			//  Runs the loop a number of times equal to the number
	{	
		/* FOr the while loop below If the isbn in the transaction record is larger than the isbn in the book record
		in the master file add all the records in the master file until it reaches a book record 
		with an isbn lower an isbn equal to it and if it points to the last Record do not add the record*/
		while( bufferT.B.isbn > bufferM.isbn && addLastRecord == false)
		{
			writeBookRec(bufferM, outfile);
			readBookRec(bufferM, infile);
		// Checks to see if the last record in the master file has been read and if so sets the addLastRecord variable to true
		infile.ignore();
		if (infile.eof())
			addLastRecord = true;
		infile.unget();
		}
		/* The transaction record isbn is now less than or equal to the master file isbn unless it is the last record*/

		//If the transaction record is to add a book do the following
	  	if (bufferT.ToDo ==Add)
		{
			// Add the book record from the master file and read new record of masterfile
			// and output an error message saying book exists already
			if (bufferT.B.isbn == bufferM.isbn)
			{
				errorfile << "Error in transaction number " << Tcount 
					  << ": cannot add-duplicate key " << setw(10)<<setfill('0')<<bufferT.B.isbn << endl;
				writeBookRec(bufferM, outfile);
				readBookRec(bufferM, infile);
			}
			// Add the book record from the transaction file
			else if (bufferT.B.isbn < bufferM.isbn || addLastRecord == true)
				writeBookRec(bufferT.B, outfile);
		}

		//If the transaction record is to add a book do the following
	  	if (bufferT.ToDo ==Delete)
		{
			//Read in the next record in the master file without writing the record to the output file, thus it has been deleted
			if (bufferT.B.isbn == bufferM.isbn)
				readBookRec(bufferM, infile);
			else
				errorfile << "Error in transactiion number " << Tcount 
					  << ": cannot delete-no such key " <<setw(10)<<setfill('0')<< bufferT.B.isbn << endl;
		}

		//If the transaction record is to change the onhand of a book do the following
	  	if (bufferT.ToDo == ChangeOnhand)
		{

			if (bufferT.B.isbn == bufferM.isbn)
			{
				bufferN = bufferM;
				bufferN.onhand += bufferT.B.onhand;
				if ( bufferN.onhand < 0)
				{
					bufferN.onhand = 0;
					errorfile << "Error in transactiion number " << Tcount 
						  << ":count " << bufferN.onhand << "for key " <<setw(10)<<setfill('0')<< bufferN.isbn << endl;
				}
				writeBookRec(bufferN, outfile);			
				readBookRec(bufferM, infile);			
			}
			else
				errorfile << "Error in transactiion number " << Tcount 
					  << ": cannot change count-no such key " <<setw(10)<<setfill('0')<< bufferT.B.isbn << endl;
		}


		//If the transaction record is to change the Price of a book do the following
	  	if (bufferT.ToDo == ChangePrice)
		{
			if (bufferT.B.isbn == bufferM.isbn)
			{
				bufferN = bufferM;
				writeBookRec(bufferN, outfile);			
				readBookRec(bufferM, infile);			
			}
			else
				errorfile << "Error in transactiion number " << Tcount 
					  << ": cannot change price-no such key "<<setw(10)<<setfill('0') << bufferT.B.isbn << endl;
		}
		 // Increment the Transaction Number Counter
		Tcount++;
	}
	
	//CLosing all files
	infile.close();
	transactfile.close();
	outfile.close();
	BookRec buf;

	// Open the new Master File that has been created as an input binary file
	 infile.open(fileNameNewM.c_str(), ios::in | ios::binary );		
	cout << endl;
	cout << "New Master File Records:" << "\n\n";
	for ( int i=0; i< 76;i++) 
		cout << "*";
	cout << endl;
	// Outputs the records in the new master file to the screen
	while(infile.read((char *) &buf, sizeof(BookRec)) )	// Read a record from the the master file 
								// until it reaches the end of the file
	{
		outputBookRec(buf);			// Output the Book Records to the screen

	}
	for ( int i=0; i< 76;i++) 
		cout << "*";
	cout << endl;

	infile.close();					// Close the new Master file
	return 0;
}
