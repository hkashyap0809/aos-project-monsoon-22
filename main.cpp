#include "clock.h"
#include "ws.h"
#include "wsclock.h"
#include<cstring>
#include<sstream>
#include<iostream>
#include<queue>
#include<vector>
using namespace std;
queue<int> q;
queue<int> q2;
	
// If page found, updates the second chance bit to true
static bool fifo_search(int pg,int arr[],
				int frames)

{
	int i;
	
	for(i = 0; i < frames; i++)
	{
		
		if(arr[i] == pg)
		{
     

			return true;
		}
        
	}
	
	

    q.push(pg);


    return false;
	
}

static void fifo_printHitsAndFaults(string reference_string,
											int frames)
{
	int pointer, i, l=0, pg;
    float pf;
    float ph;
	
	//initially we consider frame 0 is to be replaced
	int pos = 0;
	
	//number of page faults
	pf = 0;
    ph=0;
	
	// Create a array to hold page numbers
	int arr[frames];
	
	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));
	
	
	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l]=word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
    int replace_pg;
    int index;
	// l=the length of array
	
	for(i = 0; i < l; i++)
	{
		pg = stoi(str[i]);
		
		// Finds if there exists a need to replace
		// any page at all
        cout<<"\033[0;32;1m"<<pg<<"--->"<<"\033[0m";
         

          for(int i = 0 ; i < frames ;i++)
          cout<<"|"<<arr[i];

          cout<<"|";

      
   
		if(!fifo_search(pg,arr,frames))
		{
	
            if(pos < frames)
            {
                arr[pos] = pg;
                pos ++;
                cout<<endl;
            }
             else
             {
                replace_pg=q.front();
                q.pop();
               cout<<"\033[0;31;1m"<<"--->"<<replace_pg<<"\033[0m"<<endl;
             
            for(int i = 0 ; i < frames ;i++)
            {   
                 if(arr[i] ==replace_pg )
                 {  index = i;
                    break;

                 }


            }
                arr[index] = pg;
             }
			
			// Update page faults
			pf++;
		}
        else{
            cout<<endl;
            ph++;
        }
	}
	cout << "Total page faults = " << pf << "\n";
    cout << "Total page hits = " << ph << "\n";
    double r =ph/pf;  
     cout<<"hit to fault ratio =" << r <<endl;
}


	
// If page found, updates the second chance bit to true
static bool fifo_2nd_searchAndUpdate(int pg,int arr[],
				bool second_chance[],int frames)

{
	int i;
	
	for(i = 0; i < frames; i++)
	{
		
		if(arr[i] == pg)
		{
		
                   second_chance[i] = true;
              return true;
		}
	}
	
	// Return 'false' so that a page for replacement is selected
	// as he reuested page doesn't exist in memory
//	cout<<"FAULT"<<endl;
     q2.push(pg);
    return false;
	
}



static void fifo_2nd_printHitsAndFaults(string reference_string,
											int frames)
{
	int pointer, i, l=0, pg;
	float pf , ph;
	//initially we consider frame 0 is to be replaced
	int pos = 0;
    int replace_pg;
    int index;
	
	//number of page faults
	pf = 0;
    ph = 0;
    	
	// Create a array to hold page numbers
	int arr[frames];
	
	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));
	
	// Create second chance array.
	// Can also be a byte array for optimizing memory
	bool second_chance[frames];
	
	// Split the string into tokens,
	// that is page numbers, based on space
	
	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l]=word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
	// l=the length of array
	
	for(i = 0; i < l; i++)
	{
		pg = stoi(str[i]);
		
		// Finds if there exists a need to replace
		// any page at all
		 //cout<<pg<<"---->";
          cout<<"\033[0;32;1m"<<pg<<"--->"<<"\033[0m";
         

          for(int i = 0 ; i < frames ;i++)
          cout<<"|"<<arr[i];

          cout<<"|";
        if(!fifo_2nd_searchAndUpdate(pg,arr,second_chance,frames))
		{
	
		

            if(pos < frames)
            {
                arr[pos] = pg;
                pos ++;
                cout<<endl;
            }

            else{
                replace_pg=q2.front();
      

                   
            for(int i = 0 ; i < frames ;i++)
            {   
                 if(arr[i] ==replace_pg )
                 {  index = i;
                    break;

                 }


            }

            while(true)
	{
		
		// We found the page to replace
		if(!second_chance[index])
		{
			
            arr[index] = pg;
			q2.pop();
             cout<<"\033[0;31;1m"<<"--->"<<replace_pg<<"\033[0m"<<endl;
            // cout<<"----->"<<replace_pg<<endl;
            break;
			
			
		}

        else{
		
		// Mark it 'false' as it got one chance
		
        second_chance[index] = false;
        q2.pop();
        q2.push(replace_pg);

		
		
	          replace_pg=q2.front();
              

                   
            for(int i = 0 ; i < frames ;i++)
            {   
                 if(arr[i] ==replace_pg )
                 {  index = i;
                    break;

                 }


            }
        }
	} }


			
			// Update page faults
			pf++;
		}

        else{
            cout<<endl;
            ph++;

        }
	}
	cout << "Total page faults = " << pf << "\n";
    cout << "Total page hits = " << ph << "\n";
    double r =ph/pf;    
    std::cout.precision(2);
    std::cout.setf(std::ios::fixed);
    cout<<"hit to fault ratio =" << r <<endl;

}

bool optimal_search(int key, int fr[] , int fn)
{
	for (int i = 0; i < fn; i++)
		if (fr[i] == key)
			return true;
	return false;
}

// Function to find the frame that will not be used
// recently in future after given index in pg[0..pn-1]
int predict(string pg[], int fr[], int pn, int index , int fn)
{
	// Store the index of pages which are going
	// to be used recently in future
	int res = -1, farthest = index;
	int i =0;
    while(i<fn)
    {
        	int j;
            j=index;
            while(j<pn){
                	if (fr[i] == stoi(pg[j])) {
				if (j > farthest) {
					farthest = j;
					res = i;
				}
				break;
			}
                j++;


            }
		
      // If a page is never referenced in future,
		// return it.
		if (j == pn)
			return i;

        i++;
    }
   // If all of the frames were not in future,
	// return any of them, we return 0. Otherwise
	// we return res.
	return (res == -1) ? 0 : res;
}

void optimalPage(string reference_string, int fn)
{
	// Create an array for given number of
	// frames and initialize it as empty.
	//vector<int> fr;
    int fr[fn];
    

    string str[100];
	string word = "";
    int l =0;
    int pos =0;
    memset(fr, -1, sizeof(fr));
    
	for (auto x : reference_string)
	{
		if (x == ' ')
		{
			str[l]=word;
			word = "";
			l++;
		}
		else
		{
			word = word + x;
		}
	}
	str[l] = word;
	l++;

	// Traverse through page reference array
	// and check for miss and hit.
	float ph = 0;
    float pf = 0;
	for (int i = 0; i < l; i++) {
       
        int x = stoi(str[i]);
        cout<<"\033[0;32;1m"<<x<<"--->"<<"\033[0m";
       // cout<<x<<"---->";
         

          for(int i = 0 ; i < fn ;i++)
          cout<<"|"<<fr[i];

          cout<<"|";
		//Page found in a frame : HIT
		if (optimal_search(x, fr ,fn)) {
			
            ph++;
            cout<<endl;
			continue;
		}

		// Page not found in a frame : MISS
         
		// If there is space available in frames.
		if (pos < fn){
           // cout<<"FAULT"<<endl;
            pf++;
            fr[pos]=x;
            pos++;
			//fr.push_back(x);
             cout<<endl;}

		// Find the page to be replaced.
		else {
            pf++;
            // cout<<"FAULT"<<endl;
			int j = predict(str, fr, l, i + 1 , fn);
			//cout<<"Replace "<<fr[j]<<" with new page "<<x<<endl;
            cout<<"\033[0;31;1m"<<"--->"<<fr[j]<<"\033[0m"<<endl;
             //cout<<"----->"<<fr[j]<<endl;
            fr[j] = x;
		}
	}
	cout << "No. of hits = " << ph << endl;
	cout << "Total page faults were = " << pf << endl;
     double r =ph/pf;  
     cout<<"hit to fault ratio =" << r <<endl;
}



int main( int argc, const char * argv[]){
    
    if (argc<3) {
        cout<<"Invalid number of arguments"<<endl;
    }
    
    int frames=  stoi(argv[1]);
    
    //const vector<int> input{2,6,1,5,7,7,7,5,1,6,2,3,4,1,2,3,4,4,4};
    const vector<int> input{2, 5, 10, 1, 2, 2, 6, 9, 1, 2, 10, 2, 6, 1, 2, 1, 6, 9, 5, 1};
    string reference_string = "2 5 10 1 2 2 6 9 1 2 10 2 6 1 2 1 6 9 5 1";
 
    int flag;
    do{
        cout<<"Do you want to continue: press 1 to continue and 0 to exit! \n";
        cin>>flag;
        if(flag==0) break;

        if(flag==1){
            cout<<"Press any options to run corresponding algorithm.\n";
            cout<<"2. Optimal\n4. FIFO.\n5.FIFO with 2nd chance.\n6. Clock\n9. Working Set.\n11. WorkingSet Clock.\n";

            PageReplacementScheme * pr;
            int choice;
            cin>>choice;
            switch (choice)
            {
            case 2:
                 optimalPage(reference_string, frames);
                 break;
            case 4:
                fifo_printHitsAndFaults(reference_string,frames);
                break;
            case 5:
                 fifo_2nd_printHitsAndFaults(reference_string,frames);
                 break;
            case 6:
                pr=new Clock();
                 pr->simulatePageReplacement(input,frames);
                break;
            case 9:
                pr=new WS();
                 pr->simulatePageReplacement(input,frames);
                break;
            case 11:{

                pr= new WSClock();
                 pr->simulatePageReplacement(input,frames);
                }
                break;
            default:
                break;
            }
            
           

        }
    }while(flag==1);
};