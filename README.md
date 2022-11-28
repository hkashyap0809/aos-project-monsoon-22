Advanced Operating System Project
Page replacement policies simulation

COMPILE - g++ --std=c++17 clock.cpp ws.cpp wsclock.cpp main.cpp -o a.out
RUN - ./a.out


1. There are few algorithms implemented separately in their cpp programs.
2. Rest of the algorithms are implemented in main.cpp.
3. The program simulates the following Page Replacement Algorithms :
    a. Random
    b.Optimal or MIN algorithm
    c. NRU (Not Recently Used)
    d. FIFO (First-In-First-Out)
    e. FIFO with second chance
    f. Clock
    g. LRU (Least Recently Used)
    h. NFU (Not Frequently Used)
    i. Working Set
    j. Aging (approximate LRU)
    k. WSClock
4. The program takes no of frames and page sequence as input 
5. As of now the input sequence is fixed (100 page sequence). However it can be changed and user can give their own page sequence by commenting out line 74 and uncommenting line 72 and 73 of main.cpp.
6. The program is menu based and will prompt the user to either simulate single page replacement policy or compare all the above mentioned page replacemtn policies
7. In simulating all the policies, each algorithm will run and display the simulation along with a graph which shows the comparison of hit rate of all the page repalcement policies.

