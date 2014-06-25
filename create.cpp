#include<string>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

typedef char String[25];
struct BookRec
{
	unsigned int isbn;
	String name;
	String author;
	int onhand;
	float price;
	String type;
};

// converts the file into a string 
string read_file_into_string(string filename)
{
        ifstream infile (filename.c_str());
        ostringstream buf;
        char ch;

        while (buf && infile.get(ch))
                buf.put(ch);

        return buf.str();
}


// Counts the number of lines in a string
int countLines(string stringFile)
{
        int count = 0;
        int length = stringFile.length();
        for ( int i=0; i<length;i++)
        {
                if (stringFile[i] == '\n')
                        count++;
        }
        return count;
}

//Outpouts the current record being pointed to a file
void outputRecordFile( fstream& inOut, BookRec buffer)
{
	inOut << " ";
	if( buffer.isbn < 1000000000)			//Checks to see if isbn has a 0 at the end 
	{
		inOut << "0";				//Adds a zero to the isbn in the file
		inOut.width(9);				//Resets thw eifth of the isbn being outputtes
	}
	inOut << right<<  buffer.isbn;			//Writes the ISBN of the current Record to the file
	inOut.width(21);
	inOut << right << buffer.name; 	        	 //Writes the name of the current Record to the file

	inOut.width(24);
	inOut << right <<  buffer.author;         	 //Writes the author of the current Record to the file
		
	inOut.width(4);
	inOut << right << buffer.onhand;         	 //Writes the onhand of the current Record to the file

		inOut.width(7);
		inOut << right <<  buffer.price;          //Writes the price of the current Record to the file

	inOut.width(10);
	inOut << right <<  buffer.type ;          //Writes the price of the current Record to the file

	inOut << endl;
}

//Outpouts the current record being pointed to the output stream
void outputRecordOut(BookRec buffer)
{
/*
          cout<<setw(10)<<setfill('0')<<buffer.isbn
              <<setw(25)<<setfill(' ')<<buffer.name
              <<setw(25)<<buffer.author
              <<setw(3)<<buffer.onhand
              <<setw(6)<<buffer.price
              <<setw(10)<<buffer.type<<endl;
*/




	cout << " ";
	if( buffer.isbn < 1000000000)           //Checks to see if isbn has a 0 at the end
	{
		cout << "0";
		cout.width(9);
	}
	cout << right<<  buffer.isbn;          //Writes the ISBN of the current Record to the file

	cout.width(21);
	cout << right << buffer.name;          //Writes the name of the current Record to the file

	cout.width(24);
	cout << right <<  buffer.author;          //Writes the author of the current Record to the file

	cout.width(4);
	cout << right << buffer.onhand;          //Writes the onhand of the current Record to the file
	
/*
	//Converts the unsigned int variable price to a string
	ostringstream os;
	os << buffer.price;
	string str = os.str();
	
	 //Checks the price value and if does not have two first two decimal places filled it adds the decimal places and/or point
	 
	if ( buffer.price <10 && str.length()<4)
	{
		cout.width(6);
		cout << right <<  buffer.price;          //Writes the price of the current Record to the output stream
		cout << "0";
	}
	else if ( buffer.price <100 && buffer.price >= 10 && str.length()<3)
	{
		cout.width(4);
		cout << right <<  buffer.price;          //Writes the price of the current Record to the output stream
		cout << ".00";
	}
	else
	{
	*/
		cout.width(7);
		cout << right <<  buffer.price;          //Writes the price of the current Record to the  output stream
//	}

	cout.width(10);
	cout << right <<  buffer.type ;         	 //Writes the typr of the current Record to the output stream

	cout << endl;

}

// Main method
int main (int argc, char * argv[])
{
	system("clear");
	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	cout.precision(2);



  string fileName;
  if (argc==1)
  {
        cout<<"What file do you wish to view?"<<endl;
        cout<<endl;
        system("ls");
        cout<<endl;
        cout<<"?> ";
        cin>>fileName;
  }
  else
  {
        fileName = argv[1];
  }
        string stringFile = read_file_into_string(fileName); //converting the file into a string
       int size  = countLines(stringFile);                 // storing the number of lines in the file

	
	fstream infile(fileName.c_str(), ios::in | ios::out);			//Create an input file stream from library.dat
	fstream binfile("library.bin", ios::out );		//Create an output file stream from librart.out
	fstream outfile("library.out", ios::out | ios::binary );	// Create a binary file "library.bin"
//	char lines[200];
//	string line;
//
	binfile.clear();
	outfile.clear();
	BookRec buffer;					//Create an object of a BookRec named buffer
	char  ch = ' ';					
	bool output = true;				// Vaariable determines whether the Book Record is outputted or not
	unsigned int prevIsbn = 0; 			// holds the isbn value of the previous Book Record
	
	for (int i = 0; i < 21; i++)			//  Runs the loop a number of times equal to the number
							// of lines in the library.dat file
	{
		output = true;				
		infile.get(ch);				//Gets the first character in library.dat used to find negative isbns
		infile.seekg(-1, ios::cur);		//The get pointer moves back to the beginnig of the file

		infile >> buffer.isbn;			// Store the isbn in the buffer
		infile.ignore();			// Skips a character

		infile.getline(buffer.name, 25, '|');	// Store name of book in buffer

		infile.getline(buffer.author, 25, '|');	//Store the author in the buffer

		infile >> buffer.onhand;		// Store onhand of book in buffer
		infile.ignore();

		infile >> buffer.price;			// Store the price of the book in the buffer
		infile.ignore();

		infile.getline(buffer.type, 25, '\n');	//Store the author in the buffer
	//	infile >> buffer.type;			// Store the prioe of the book in the buffer
	//	infile.ignore();			
		
//		outputRecordFile(outfile, buffer);	// Output the contents of the buffer into the library.out file
	
		// Output error message if the isbn of the book is out of sequence	
		if (buffer.isbn <= prevIsbn)
		{
			cerr << "> Isbn out of sequence on line " << i+1<< " of data file." << endl;	
			outputRecordOut(buffer);
		}

		prevIsbn = buffer.isbn;	//Make the previous isbn buffer be equal to the current isbn in the Book record buffer
		
		// Output error message if the onhand of the book is a negative number
		if ( buffer.onhand <0)
		{
			output = false;
			cerr << "> Negative amount onhand on line " << i+1<< " of data file - record ignored." << endl;	
			outputRecordOut(buffer);
		}

		// Output error message if the price of the book is a negative number
		if ( buffer.price < 0)
		{
			output = false;
			cerr << "> Negative amount price on line " << i+1<< " of data file - record ignored." << endl;	
			outputRecordOut(buffer);
		}
		// Output an error message if the isbn number is negative
		if ( ch == '-')
		{
			cerr << "< Illegal isbn number encountered on line "<< i+1 << " of data file - record ignored." << endl;
		output = false;
		prevIsbn = 0;
		}

		// Output the Book Record if it satisfies the criteria of records to be outputted
		if (output == true)
		{
			outputRecordFile(binfile, buffer);
			outfile.write((char *) &buffer, sizeof(BookRec));
		}
	
	}
	infile.close();
	outfile.close();					// Close the output file
	binfile.close();					// Close the output file
	BookRec buf;

	cout << endl;
	 infile.open("library.out", ios::in | ios::binary );	// Create a binary file "library.bin"

	while( infile.read((char *) &buf, sizeof(BookRec)) )	// Read a record from the the binfile 
								// until it reaches the end of the file
	{
		outputRecordOut(buf);			// Output the Book Records to the screen

	}

	binfile.close();					// Close the Book records binary file

	return 0;
}
