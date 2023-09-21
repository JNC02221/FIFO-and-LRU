//These are all of the includes that were needed for this prohect
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


//This is the class that I made to be used as the obhects in the array
class Mem
{
	public:
		Mem()
		{
			address = -1;
			wr = 'a';
		}

		char wr;
		unsigned int address;

};



//This is the function that uses FIFO organization to arrange the memory
void Fifo(std::string fileName, int listNum, int pageNum, std::string print)
{
	std::string line, add;
	char wr;
	unsigned int x = -1,  read=0, writes=0, hit=0;	
	unsigned int address = -1;
	Mem pageList[pageNum];
	std::ifstream fileName1(fileName.c_str());
	
	//This loops through every adress in the files given
	for (int i=0;i<listNum;i++)
	{
		

                fileName1 >> add;
                std::istringstream iss(add);
                iss >> std::hex >> x;
                address = x/4096; //here the address is put into the corect amount of bits
		fileName1 >> wr;


		//This loops through all of the frames 
		for(int h = 0; h<pageNum;h++)
		{
			if(pageList[h].address == address)//This is for the case that it is found
			{
				if(wr == 'W' && pageList[h].wr == 'R')//This changes reads to writes in the saved memory
				{
					pageList[h].wr = 'W';
				}
				if(print == "debug")
				{
					std::cout << "Address: " << add << " " << wr << " hIT" << '\n';
				}			
				break;
			}
			if(pageList[h].address == -1)//This is the case that an empty spot in the frames is found
			{
				pageList[h].address = address;
				pageList[h].wr = wr;				
				read++;
				if(print == "debug")
                                {
                                        std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                                }
				break;				
			}
			if(h == pageNum-1)//This is the case that the address was not found
			{	
				read++;
				if(pageList[0].wr == 'W')//This adds to write if the address being poped was an write
				{
					writes++;
				}
				for(int k=0;k<pageNum;k++)//This moves all of the values in the frames up one and pops the first address
				{
					pageList[k].address = pageList[k+1].address;
					pageList[k].wr= pageList[k+1].wr;					
				}

				//here the new address is added to the back of the array
				pageList[pageNum-1].address = address;
				pageList[pageNum-1].wr = wr;

				if(print == "debug")
                                {
                                        std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                                }

			
			}
			
		}
	}

	std::cout << "Frames:" << pageNum << '\n';
	std::cout << "Traces:" << listNum << '\n';
	std::cout << "Reads:" << read << '\n';
	std::cout << "Writes:" << writes << '\n';
}


//This is the function that uses LRU to arrange the page table and figure out what is needed to be switched when there is a miss
void Lru(std::string fileName, int listNum, int pageNum, std::string print)
{
	std::string line, add;
        char wr, tempWr;
        unsigned int x = -1,  read=0, writes=0, hit=0, fill, tempAdd, lastIndex;
        unsigned int address = -1;
        Mem pageList[pageNum];
        std::ifstream fileName1(fileName.c_str());


	//This loops through every adress in the files given
        for (int i=0;i<listNum;i++)
        {

                fileName1 >> add;
                std::istringstream iss(add);
                iss >> std::hex >> x;
                address = x/4096; //here the address is put into the correct amount of bits
                fileName1 >> wr;


		//This loops through all of the frames
                for(int h = 0; h<pageNum;h++)
                {
                        if(pageList[h].address == address)//This is for the case that it is found
                        {
                                if(wr == 'W' && pageList[h].wr != 'W')//This changes reads to writes in the saved memory
                                {
                                        pageList[h].wr = 'W';
                                }

				//here i set the address found to a temp
				tempAdd = pageList[h].address;
				tempWr = pageList[h].wr;


				for(int k=h;k<lastIndex-1;k++)//here i loop and more up all of the address that need to be moved up
                                {
                                        pageList[k].address = pageList[k+1].address;
                                        pageList[k].wr= pageList[k+1].wr;
                                }

				//here I set the address hit to the last filled spot in the array
				pageList[lastIndex-1].address = tempAdd;
				pageList[lastIndex-1].wr = tempWr;
				
				if(print == "debug")
                                {
                                        std::cout << "Address: " << add << " " << wr << " hIT" << '\n';
                                }
                                break;
                        }


			if(pageList[h].wr == 'a')//This is the case that an empty spot in the frames is found
                        {
                                pageList[h].address = address;
                                pageList[h].wr = wr;
                                read++;
                                lastIndex = h+1;//this keeps track of the last index that was added to
				if(print == "debug")
                                {
                                        std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                                }
                                break;
                        }


                        if(h == pageNum-1)//This is the case that the address was not found
                        {
                                read++;
                                if(pageList[0].wr == 'W')//This adds to write if the address being popped was an write
				{
                                        writes++;
                                }
                                for(int k=0;k<pageNum;k++)//here the first address is popped and every other address is moved up one spot
                                {
                                        pageList[k].address = pageList[k+1].address;
                                        pageList[k].wr= pageList[k+1].wr;
                                }

				//here the last spot in the array is set the the new address needed to be addded 
                                pageList[pageNum-1].address = address;
                                pageList[pageNum-1].wr = wr;

				if(print == "debug")
                                {
                                        std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                                }		
				break;
                        }
                }
        }
        std::cout << "Frames:" << pageNum << '\n';
        std::cout << "Traces:" << listNum << '\n';
        std::cout << "Reads:" << read << '\n';
        std::cout << "Writes:" << writes << '\n';
}


//This is the function the computes segmented fifo
void Vms(std::string fileName, int listNum, int pageNum, std::string print, double per)
{
  	std::string add;
	bool flag = false;
    char wr, tempWr, tempWrS;
    unsigned int x = -1,  read=0, writes=0, hit=0, lastIndex=0, tempAdd, tempAddS, secSize, address = -1;
	double percent;
    std::ifstream fileName1(fileName.c_str());

	//this is the code that creates the two arrays in the right size 
	percent = per/100;
	secSize = pageNum * percent;
	
	unsigned int primSize = pageNum-secSize;

	Mem primary[primSize];
	Mem secondary[secSize];
	

	if(secSize == 0)//this is the case that the percent is 0
	{
		Fifo(fileName, listNum, pageNum, print);
	}
	else if((pageNum-secSize) == 0)//this is the case that the percent is 100
	{
		Lru(fileName, listNum, pageNum, print);
	}

	else//this is the case that the percent is not 0 or 100
	{
		//This loops through every adress in the files given
        for (int i=0;i<listNum;i++)
        {


            fileName1 >> add;
            std::istringstream iss(add);
            iss >> std::hex >> x;
            address = x/4096;
            fileName1 >> wr;

			//this is the loop that loops through the whole first array
            for(int h = 0; h<primSize;h++)
            {
				flag = true;
				if( primary[h].address == address)//This is the case that the address was found in the first array
                {
                    if(wr == 'W' && primary[h].wr != 'W')
                    {
                        primary[h].wr = 'W';
                    }

                    if(print == "debug")
                    {
                        std::cout << "Address: " << add << " " << wr << " hIT" << '\n';
                    }
					flag = false;
                	break;
                }

				else if(primary[h].address == -1)//this is the case that an empty spot in the first array was found before the address was found
                {
                    primary[h].address = address;
                    primary[h].wr = wr;
                    read++;
                    if(print == "debug")
                    {
                        std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                    }
					flag = false;
                    break;
                }
			}

			//This serves as the if that makes sure the first array is filled completly before the secound array is changed and looked through
			if(primary[primSize-1].address != -1 && flag)
			{
				//this is the loop that loops through all of the secound array
				for(int h=0;h<secSize;h++)
				{
					if( secondary[h].address == address )//This is the case that the addresss was found in the secound loop
					{
						if(wr == 'W' && secondary[h].wr != 'W')
                        {
                    		secondary[h].wr = 'W';
                		}
						tempAdd = secondary[h].address;
                        tempWr = secondary[h].wr;

						tempAddS = primary[0].address; 
						tempWrS = primary[0].wr;

						for(int k=0;k<(primSize)-1;k++)
                        {
                            primary[k].address = primary[k+1].address;
                            primary[k].wr= primary[k+1].wr;
                        }
                        primary[(primSize)-1].address = tempAdd;
                        primary[(primSize)-1].wr = tempWr;


						for(int k=h;k<lastIndex;k++)
                        {
                            secondary[k].address = secondary[k+1].address;
                            secondary[k].wr= secondary[k+1].wr;
                        }
						secondary[lastIndex-1].address = tempAddS;				
						secondary[lastIndex-1].wr = tempWrS;


 						if(print == "debug")
                        {
                         	std::cout << "Address: " << add << " " << wr << " hIT" << '\n';
                        }
						break;
					}

					if(secondary[h].address == -1)//this is the case that an empty spot in the secound array is found before the address is found
					{  
						read++;
						lastIndex++;
						tempAddS = primary[0].address;
                        tempWrS = primary[0].wr;


						for(int k=0;k<(pageNum-secSize)-1;k++)
                        {
                        	primary[k].address = primary[k+1].address;
                            primary[k].wr= primary[k+1].wr;
                        }
                        primary[(primSize)-1].address = address;
                        primary[(primSize)-1].wr = wr;
	
						secondary[h].address = tempAddS;
                		secondary[h].wr = tempWrS;


                        if(print == "debug")
                        {
                            std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                        }
						break;	
					}

					else if(h == secSize-1)//this is the case that the address was never found and must be added 
                    {
                        read++;

                        if(secondary[0].wr == 'W')
                        {
                            writes++;
                        }


						tempAdd = primary[0].address;
						tempWr = primary[0].wr;				
				

                        for(int k=0;k<(primSize);k++)
                        {
                        	primary[k].address = primary[k+1].address;
                        	primary[k].wr= primary[k+1].wr;
                        }
                        primary[(primSize)-1].address = address;
                        primary[(primSize)-1].wr = wr;
				


						for(int k=0;k<secSize;k++)
                        {
                            secondary[k].address = secondary[k+1].address;
                            secondary[k].wr= secondary[k+1].wr;
                        }
                        secondary[secSize-1].address = tempAdd;
                        secondary[secSize-1].wr = tempWr;
				

                        if(print == "debug")
                        {
                            std::cout << "Address: " << add << " " << wr << " MISS" << '\n';
                        }
						break;
					}
				}
			}

	
        }
    }

        std::cout << "Frames:" << pageNum << '\n';
        std::cout << "Traces:" << listNum << '\n';
        std::cout << "Reads:" << read << '\n';
        std::cout << "Writes:" << writes << '\n';

}





//This is the main function where it is decided what function is to be called
int main(int argc, char *argv[])
{
	std::string f, type,print,line;
	int n, per;
	if(argc == 5)//this is for setting the arguments for a lru or fifo
	{
		f = argv[1];
		n =atoi(argv[2]);
		type = argv[3];
		print = argv[4];
	}
	else if(argc == 6)//this is for setting up the arguments for a segmented fifo
	{
		f = argv[1];
                n = atoi(argv[2]);
                type = argv[3];
		per = atoi(argv[4]);
                print = argv[5];
	}

	std::string wr, add;
    	int numLines = 0;
   	std::ifstream my_file(argv[1]);
        int x = -1, address = -1;


	while (std::getline(my_file, line))//this is for getting the number of traces in the file to send to the other functions 
	{
            	++numLines;
        }
        my_file.close();
    

	//these are the function calls for the diffrent methods 
	if(type == "fifo")
	{
		Fifo(f,numLines,n,print);
	}
	else if(type == "lru")
	{
		Lru(f,numLines,n,print);
	}
	else if(type == "vms")
	{
		Vms(f,numLines,n,print,per);
	}

	return 0;
}
