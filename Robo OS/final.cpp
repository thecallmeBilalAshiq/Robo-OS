//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//======================================================================================================================//
//====                                                                                                              ====//
//====                                                                                                              ====//
//====                                                                                                              ====//
//====                              ROBO OS (Opersting System Project)                                              ====//
//====                                 Muhammad Bilal (22F-3309)                                                    ====//
//====                                  Faeiz Tariq (22F-3873)                                                      ====//
//====                                                                                                              ====//
//====                                                                                                              ====//
//======================================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <sys/shm.h>
#include <cstring>
#include <list>
#include <queue>
#include <signal.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fstream>
#include <semaphore.h>

using namespace std;

int ssd, core,choice;
float ram,occupied_ram, total_ram;
int  occupied_core=0, occupied_ssd=0, timer , total_ssd, total_core;

struct memory_allocation{
  // structure used by child to ask from parent if desired ram,core, space is available or not
float process_ram; 
int process_choicee;
int core;
int permission = -1;
};
memory_allocation m;

struct interrupt 
{ // for IPC between processes and parent process for iterruption, minimization and closing of a process
int minimize=-1, close=-1, in_out_put=-1 ;
int process_choice; };
 interrupt message;



struct pcb
{ // pcb 
	int process_choice, size, priority;
	float ram;  
	pid_t process_id; int core_assigned ; 
};

list<pcb>pcb_list; // pcb blocks

void adding_process_to_pcb(int p_choice , int priorit_y, float ra_m , pid_t p_rocess_id, int core_assigne_d)
{
	pcb_list.push_back({p_choice, -1, priorit_y,ra_m, p_rocess_id, core_assigne_d });	
}

void  finding_process_in_pcb(int choicee){
	
	auto it =  pcb_list.begin();
	while(it!= pcb_list.end()){
		if (it->process_choice == message.process_choice) { 
        it = pcb_list.erase(it); 
        break;
    }
    else {
        ++it; 
    }
	}
}


struct ready_q{
	int p_choice_id =-1; pid_t process_id,priority=-1 ;
	int arrival_time=-1, burst_time=-1, turnaround_time=-1 ;
	
};
queue<ready_q>ready_q_priority; //top level in multilevel queue
queue<ready_q>ready_q_fcfs; // bottom level in multilevel queue
// functions

void adding_process_to_ready_q( int process_choice, pid_t process_id, int prio_rity){
if (process_choice == 1 || process_choice==2 || process_choice == 4 ||process_choice == 8  || process_choice == 9 || process_choice == 3 || process_choice ==17 ){
		ready_q_fcfs.push({process_choice, process_id, prio_rity});
	}
	 else
		 if (  process_choice ==5  || process_choice ==6  || process_choice == 7 
|| process_choice == 10 || process_choice == 11 ||
               process_choice == 12 || process_choice == 13 || process_choice == 14 || process_choice == 15 || process_choice == 16	  ){
		ready_q_priority.push({process_choice, process_id, prio_rity});
	}
}

float release_ram( int process_choicee_id){
	if(process_choicee_id == 4)   return 3.5;
		else if(process_choicee_id == 3)   return 0.5;
	else if(process_choicee_id == 5)   return 2.5;
	else if(process_choicee_id == 6)   return 1.5;
	else if(process_choicee_id == 7)   return 2.5;
	else if(process_choicee_id == 8)   return 1;
	else if(process_choicee_id == 9)   return 1.2;
	else if(process_choicee_id == 10)   return 1.4;
	else if(process_choicee_id == 11)   return 2;
	else if(process_choicee_id == 12)   return 1.4;
	else if(process_choicee_id == 13)   return 2;
	else if(process_choicee_id == 14)   return 3;
	else if(process_choicee_id == 15)   return 1.1;
	else if(process_choicee_id == 16)   return 1.5;
	else if(process_choicee_id == 17)   return 10.7;
	else return 1;
}
int main()
{
pid_t process;
	char new_processes = 'y';  int priority;
	int p_permission=-1; float p_ram=0;
total_ram =0, total_ssd = 100,total_core =0;
int process_in_ready_q= 0;
	int count = 1;


int shmid = shmget(IPC_PRIVATE, sizeof(memory_allocation), 0666 | IPC_CREAT); // for memory_allocation
memory_allocation* shmem = (memory_allocation*) shmat(shmid, NULL, 0);

//--------------------------------welcome message------------------------
cout << "\033[2J\033[1;1H";
cout<<"please enter desired RAM and Cores"<<endl;;
cin>> total_ram>>total_core;
cout << "\033[2J\033[1;1H";
cout<<"\t\t\tplease wait while window is loading"<<endl;
ram = total_ram; ssd = total_ssd; core=total_core;
cout << endl << endl;

cout << "-------- >>> ATTEMPTING TO LOGIN     <<< -----------\n" << endl;     sleep(1.5);
cout << "-------- >>> GENERATING LOG FILES    <<< ----------- \n" << endl;  sleep(1.5);
cout << "-------- >>> ETTING USER DATA        <<< ----------- \n" << endl;sleep(1.5);
cout << "-------- >>> LOADING THE DIRECTORIES <<< ----------- \n" << endl;   sleep(1.5);
cout << "-------- >>> CONNECTING TO THE MAIN OPERATING SYSTEM <<< -----------\n" << endl; sleep(1.5);
cout << "                                         WELCOME                                         " << endl;  sleep(2);
cout<<endl;
cout << "\033[2J\033[1;1H";


// ---------------------------------------------------------------------------------------------------------------------------------
	sf::Texture t;
	t.loadFromFile("woods.jpg");     // loading file for background
	sf::Sprite s(t);
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(),  "Operating System");
   sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  //  sf::RenderWindow window(sf::VideoMode(1366, 672), "SFML Window

    sf::Font font;
    if (!font.loadFromFile("Banana.ttf"))
    {
        // handle error
    }


// ===========================================  >>>>>>>> GUI made by "Chat Gpt"  <<<<<<< ============================================
    
    sf::RectangleShape calculator(sf::Vector2f(130, 40));
    calculator.setPosition(20,40);
    calculator.setFillColor(sf::Color::Transparent);
    sf::Text caltext("Calculator", font, 20);
    calculator.setOutlineColor(sf::Color::Red);
    calculator.setOutlineThickness(2);
    caltext.setOutlineThickness(5);
    // create a text object for the button label
    caltext.setFillColor(sf::Color::White);
    caltext.setPosition(calculator.getPosition().x + 40, calculator.getPosition().y + 5);
	
// ---------------------------------------------------------------------------------------------------------------------------------
    
    sf::RectangleShape calender(sf::Vector2f(130, 40));
    calender.setPosition(20, 100);
    calender.setFillColor(sf::Color::Transparent);
    calender.setOutlineColor(sf::Color::Red);
    calender.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text calendertxt("Calender", font, 20);
    calendertxt.setOutlineThickness(5);
    calendertxt.setFillColor(sf::Color::White);
    calendertxt.setPosition(calender.getPosition().x + 40, calender.getPosition().y + 5);

// ---------------------------------------------------------------------------------------------------------------------------------

    sf::RectangleShape numguess(sf::Vector2f(130, 40));
    numguess.setPosition(20, 160);
    numguess.setFillColor(sf::Color::Transparent);
    numguess.setOutlineColor(sf::Color::Red);
    numguess.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text numtxt("Number Guessing", font, 20);
    numtxt.setFillColor(sf::Color::White);
    numtxt.setPosition(numguess.getPosition().x + 10, numguess.getPosition().y + 7);
// ---------------------------------------------------------------------------------------------------------------------------------
   sf::RectangleShape notepad(sf::Vector2f(130, 40));
    notepad.setPosition(20, 220);
    notepad.setFillColor(sf::Color::Transparent);
    notepad.setOutlineColor(sf::Color::Red);
    notepad.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text notepadtxt("Notepad", font, 20);
    notepadtxt.setFillColor(sf::Color::White);
    notepadtxt.setPosition(notepad.getPosition().x + 40, notepad.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape copy(sf::Vector2f(130, 40));
    copy.setPosition(20, 280);
    copy.setFillColor(sf::Color::Transparent);
    copy.setOutlineColor(sf::Color::Red);
    copy.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text copytxt("Copy  File", font, 20);
    copytxt.setFillColor(sf::Color::White);
    copytxt.setPosition(copy.getPosition().x + 40, copy.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape delfile(sf::Vector2f(130, 40));
    delfile.setPosition(20, 340);
    delfile.setFillColor(sf::Color::Transparent);
    delfile.setOutlineColor(sf::Color::Red);
    delfile.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text delfiletxt("Delete  File", font, 20);
    delfiletxt.setFillColor(sf::Color::White);
    delfiletxt.setPosition(delfile.getPosition().x + 40, delfile.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape cmpf(sf::Vector2f(130, 40));
    cmpf.setPosition(20, 400);
    cmpf.setFillColor(sf::Color::Transparent);
    cmpf.setOutlineColor(sf::Color::Red);
    cmpf.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text cmpftxt("Compress  File", font, 20);
    cmpftxt.setFillColor(sf::Color::White);
    cmpftxt.setPosition(cmpf.getPosition().x + 20, cmpf.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape decomp(sf::Vector2f(130, 40));
    decomp.setPosition(1700, 400);
    decomp.setFillColor(sf::Color::Transparent);
    decomp.setOutlineColor(sf::Color::Red);
    decomp.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text decomptxt("Stop watch", font, 20);
    decomptxt.setFillColor(sf::Color::White);
    decomptxt.setPosition(decomp.getPosition().x + 15, decomp.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape file(sf::Vector2f(130, 40));
    file.setPosition(1700, 340);
    file.setFillColor(sf::Color::Transparent);
    file.setOutlineColor(sf::Color::Red);
    file.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text filetxt("File  Locations", font, 20);
    filetxt.setFillColor(sf::Color::White);
    filetxt.setPosition(file.getPosition().x + 15, file.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape increment(sf::Vector2f(130, 40));
    increment.setPosition(1700, 40);
    increment.setFillColor(sf::Color::Transparent);
    increment.setOutlineColor(sf::Color::Red);
    increment.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text incrementtxt("Hung man", font, 20);
    incrementtxt.setFillColor(sf::Color::White);
    incrementtxt.setPosition(increment.getPosition().x + 50, increment.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape tic(sf::Vector2f(130, 40));
    tic.setPosition(1700, 100);
    tic.setFillColor(sf::Color::Transparent);
    tic.setOutlineColor(sf::Color::Red);
    tic.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text tictxt("Tic  Tac  Toe", font, 20);
    tictxt.setFillColor(sf::Color::White);
    tictxt.setPosition(tic.getPosition().x + 35, tic.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape music(sf::Vector2f(130, 40));
    music.setPosition(1700, 160);
    music.setFillColor(sf::Color::Transparent);
    music.setOutlineColor(sf::Color::Red);
    music.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text musict("Music", font, 20);
    musict.setFillColor(sf::Color::White);
    musict.setPosition(music.getPosition().x + 55, music.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------
    sf::RectangleShape pic(sf::Vector2f(130, 40));
    pic.setPosition(1700, 220);
    pic.setFillColor(sf::Color::Transparent);
    pic.setOutlineColor(sf::Color::Red);
    pic.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text pict("multimedia", font, 20);
    pict.setFillColor(sf::Color::White);
    pict.setPosition(pic.getPosition().x + 20, pic.getPosition().y + 5);
// ---------------------------------------------------------------------------------------------------------------------------------

    sf::RectangleShape shut(sf::Vector2f(130, 40));
    shut.setPosition(1700, 280);
    shut.setFillColor(sf::Color::Transparent);
    shut.setOutlineColor(sf::Color::Red);
    shut.setOutlineThickness(2);
    // create a text object for the button label
    sf::Text shutt("ShutDown", font, 20);
    shutt.setFillColor(sf::Color::White);
    shutt.setPosition(shut.getPosition().x + 35, shut.getPosition().y + 5);
	
	pthread_t pid[15];
	bool threadcalled[15];
	for(int i=0;i<15;i++)
	threadcalled[i]=false;
	
// ---------------------------------------------------------------------------------------------------------------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // -----------------------  >>>>>>>  check if the mouse is over the button

       int i = 1;
      // for(i= 1 ;i<2; i++){
		   choice =0;
		
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && calculator.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        { // process 3
	      choice=5;
             pid_t pid = fork();

            if (pid == 0) {
                memory_allocation msg;
                msg.process_ram = 2.5; msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    cout<<"1"<<endl;
                    const char* args[] = {"gnome-terminal","--title=Calculator", "--", "bash", "-c", "./calculator; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
                
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0) ) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                    core--;
                    occupied_ram += m.process_ram;
                    occupied_core+=1;
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to calculator process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to  calculator process is denied" << endl;
                }
                priority =33;
                memcpy(shmem, &m, sizeof(memory_allocation));
                process= waitpid(pid,NULL,0);
            }
}

       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && calender.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {//process 4
			 ////////////////////////////////////////
	     choice=4;
            pid_t pid = fork();

            if (pid == 0) {
                memory_allocation msg;
                msg.process_ram = 3.5;  msg.core = 1;
                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    cout<<"1"<<endl;
                    const char* args[] = {"gnome-terminal","--title=Calender", "--", "bash", "-c", "./calender; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
			return 0;
			}
            else {
                 sleep(1);
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0) ) {
                    m.permission = 1;
                    ram = ram - m.process_ram;       core--;       
					occupied_ram += m.process_ram;      occupied_core+=1;
                    p_permission = m.permission;         	p_ram = m.process_ram;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to calender process is denied" << endl;
                }
                priority =5;
                memcpy(shmem, &m, sizeof(memory_allocation));
                 process=waitpid(pid,NULL,0);
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && numguess.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {//process 5
	       choice=6;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 1.5;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Random Number","--", "bash", "-c", "./randomnumber; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            }  else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) && (core - m.core >= 0) ) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                   core--;
                    occupied_ram += m.process_ram;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to randomnumber process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to randomnumber process is denied" << endl;
                }
                priority =41;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
           
        }
        
        else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && notepad.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
           choice=7;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 2.5;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Notepad", "--", "bash", "-c", "./notepad; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to notepad process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to notepad process is denied" << endl;
                }
                priority =30;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && copy.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
			 choice==8;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 1;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Copy a File", "--", "bash", "-c", "./copy; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to copy process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to copy process is denied" << endl;
                }
                priority =35;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
			
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && delfile.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			
        {
            choice==9;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 1.2;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Delete a file", "--", "bash", "-c", "./delete; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to copy process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to delete process is denied" << endl;
                }
                priority =34;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
		   
        }
        
       else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && cmpf.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
           choice=12;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 1.4;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Compress a File", "--", "bash", "-c", "./compress; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            }  else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to compress file process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to compress file process is denied" << endl;
                }
                priority =43;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && decomp.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
            choice=13;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 2;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal", "--title=Stop Watch","--", "bash", "-c", "./watch; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to decompress file process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
				    int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to decompress file process is denied" << endl;
                }
                priority =32;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && file.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
          
		   choice=11;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 2;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal", "--title=File Locations","--", "bash", "-c", "./file; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to file location process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to file location process is denied" << endl;
                }
                priority =30;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && increment.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
            choice==16;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 1.5;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=HangMan", "--", "bash", "-c", "./hungman; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to  guess game file process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to guess game file process is denied" << endl;
                }
                priority =32;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && tic.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
			choice=14;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 3,  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Tic Tac Toe", "--", "bash", "-c", "./tictac; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to tictac file process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to tictac file process is denied" << endl;
                }
                priority =32;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
      else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && music.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
          choice==17;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 0.7;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
					
                 const char* args[] = {"gnome-terminal", "--title=Music Player","--", "bash", "-c", "./music piano.mp3; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to copy process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to copy process is denied" << endl;
                }
                priority =35;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
       else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && pic.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
            	 choice==3;
            pid_t pid = fork();

            if (pid == 0) {
                  memory_allocation msg;
                msg.process_ram = 0.5;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
					
                 const char* args[] = {"gnome-terminal","--title=Video Player", "--", "bash", "-c", "./video; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                 sleep(1);
               // memory_allocation m;
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0) &&  (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                 //   ssd = ssd - m.process_size;
                    core--;
                    occupied_ram += m.process_ram;
                  //  occupied_ssd += m.process_size;
                    occupied_core+=1; 
					p_ram = m.process_ram;
					p_permission = m.permission;
                    cout << "permission to copy process is granted" << endl;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to copy process is denied" << endl;
                }
                priority =35;
                memcpy(shmem, &m, sizeof(memory_allocation));
               process =  waitpid(pid,NULL,0); // stores process id   
            }
        }
        
      else  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shut.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
        {
            sleep(5);
            cout << "\033[2J\033[1;1H";
cout<<"\t\t\t  @ahmad7867---@wajeeha---@Alisha--- " << endl << endl << "\t\t\tINITIATING OPERATING SYSTEM SHUTDOWN" << endl;

cout << "------------ >>>> Terminating All the Running Processes <<<<< --------------- \n" << endl;      sleep(1.5);
cout << "------------ >>>> Releasing System Resources <<<<< --------------- \n" << endl;  sleep(1.5);
cout << "------------ >>>> Saving USER DATA <<<<< --------------- \n" << endl;      sleep(1.5);
cout << "------------ >>>> Stopping system services... \n" << endl;   sleep(1.5);
cout << "------------ >>>> Flushing disk buffers... <<<<< --------------- \n" << endl;   sleep(1.5);
cout << "------------ >>>> Do not exit this window  <<<<< --------------- \n" << endl;   sleep(1.5);
cout << "--------3---------2----------1------------" << endl << endl;   sleep(1.5);
system("pkill -f gnome-terminal");   sleep(2);
        }
	//   }
	   
   //    if (i ==2 && choice != 0){
              if ( choice != 0){
			  adding_process_to_pcb( choice,priority,p_ram, process , p_permission );
			   	    if (p_permission == 0 ){
		    adding_process_to_ready_q(choice, process,priority);
			process_in_ready_q++;
		}
	//	else { core --;}
	
		}
//--------------------------------------------------------------------------------------------------------------------------------------------------------			 
			 			 // first read to check if already have been read or not
			  std::ifstream infile("IPC.txt");
			  int process_choice , terminate_signal , yet_to_read ;  char a;
			     infile >> yet_to_read>>a>>terminate_signal>>a>>process_choice; 
                    infile.close();
			 if(yet_to_read ==  1 && terminate_signal ==1){// haven't been read before 
			 
				ram += release_ram(process_choice);
			//	cout<<"new ram"<<ram<<endl;
				core++;   occupied_core--;
				finding_process_in_pcb(process_choice);
			 ////to make sure not to read the old data again
		   std::ofstream outfile("IPC.txt");
                 a = ',';
                 outfile <<0<<a<<0<<a<<0<<endl;
                 outfile.close();
			 /////////*/
			 }
			 
//-----------------------------------------------------------------------------------------------------------------------------------
  //while (core>0 && (!ready_q_fcfs.empty()   || !ready_q_priority.empty())){  
			     
			  if(!ready_q_priority.empty() && core>0){
				    pid_t pid = fork();
				    if (pid == 0) {
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
				string cpp_file ;
				cout<<"process_choice"<<shmem->process_choicee<<endl;
				
			     if(shmem->process_choicee == 5) {     
				 const char* args[] = {"gnome-terminal", "--title=Calculator","--", "bash", "-c", "./calculator; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
				else if  (shmem->process_choicee ==6 ) {
					 const char* args[] = {"gnome-terminal", "--title=Random Number","--", "bash", "-c", "./randomnumber; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
				else if  (shmem->process_choicee ==7 ) {
					 const char* args[] = {"gnome-terminal","--title=Notepad", "--", "bash", "-c", "./notepad; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
				
                      else if  (shmem->process_choicee ==10) {
					 const char* args[] = {"gnome-terminal", "--title=Increment Count","--", "bash", "-c", "./increment_count; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}

              else if  (shmem->process_choicee ==11) {
					 const char* args[] = {"gnome-terminal","--title=File Locations", "--", "bash", "-c", "./file; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
               else if  (shmem->process_choicee ==12) {
					 const char* args[] = {"gnome-terminal","--title=Compress a File", "--", "bash", "-c", "./compress; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}

              else if  (shmem->process_choicee ==13) {
					 const char* args[] = {"gnome-terminal","--title=Stop Watch", "--", "bash", "-c", "./watch; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
			else if  (shmem->process_choicee ==14) {
					 const char* args[] = {"gnome-terminal", "--title=Tic Tac Toe","--", "bash", "-c", "./tictac; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
			
	}	
             else if  (shmem->process_choicee ==15) {
					 const char* args[] = {"gnome-terminal","--title=Music Player", "--", "bash", "-c", "./music piano.mp3; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
				else if  (shmem->process_choicee ==16) {
					 const char* args[] = {"gnome-terminal","--title=Hangman", "--", "bash", "-c", "./hungman; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				}
				
                   
              //  }
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                // sleep(1);
				 m.process_choicee= ready_q_priority.front().p_choice_id;
				 core--;
				 	 process_in_ready_q--;
                memcpy(shmem, &m, sizeof(memory_allocation));
				ready_q_priority.pop();
                waitpid(pid,NULL,0);
                
            }
				  
				  
				  
			  }
			  else if (!ready_q_fcfs.empty() && core >0){
				    pid_t pid = fork();
            if (pid == 0) {
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
				string cpp_file ;
				 if(shmem->process_choicee == 3) {     
				 const char* args[] = {"gnome-terminal","--title=Video Player", "--", "bash", "-c", "./video; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				 }
				else  if(shmem->process_choicee == 4) {     
				 const char* args[] = {"gnome-terminal","--title=Calender", "--", "bash", "-c", "./calender; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				 }
				  else if(shmem->process_choicee == 8) {     
				 const char* args[] = {"gnome-terminal","--title=Copy a File", "--", "bash", "-c", "./copy; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				 }
				  else  if(shmem->process_choicee == 9) {     
				 const char* args[] = {"gnome-terminal","--title=Delete a File", "--", "bash", "-c", "./delete; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				 }
				 else  if(shmem->process_choicee == 17) {     
				 const char* args[] = {"gnome-terminal","--title=Music Player", "--", "bash", "-c", "./music piano.mp3; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
				 }
				
                return 0;
            } else if (pid < 0) {
               
                std::cerr << "Error: forking failed.\n";
                return 1;
            } else {
                // sleep(1);
				 m.process_choicee= ready_q_fcfs.front().p_choice_id;
				 core--;
				 process_in_ready_q--;
                memcpy(shmem, &m, sizeof(memory_allocation));
				ready_q_fcfs.pop();
                waitpid(pid,NULL,0);
                
            }
			 }
			 
//-----------------------------------------------------------------------------------------------------------------------------------

cout << "\033[2J\033[1;1H";			
cout<<" total  ram , ssd, core : "<<ram<<" "<<ssd<<" "<<core<<endl;
	   
//---------------------------------------------------------------------------------------------------------------------------------
       // window.clear();
         window.draw(s);
        window.draw(calculator); 
	window.draw(caltext);
        window.draw(calender);
        window.draw(calendertxt);
        window.draw(numguess);
        window.draw(numtxt);
        window.draw(notepad);
        window.draw(notepadtxt);
        window.draw(copy);
        window.draw(copytxt);
        window.draw(delfile);
        window.draw(delfiletxt);
        window.draw(cmpf);
        window.draw(cmpftxt);
        window.draw(decomp);
        window.draw(decomptxt);
        window.draw(file);
        window.draw(filetxt);
        window.draw(increment);
        window.draw(incrementtxt);
        window.draw(tic);
        window.draw(tictxt);
        window.draw(music);
        window.draw(musict);
        window.draw(pic);
        window.draw(pict);
        window.draw(shut);
        window.draw(shutt);
        window.display();      
        wait(NULL);
		
            while(count <=2){
				if (count==1)
				{
					  pid_t pid = fork();

            if (pid == 0) {
                memory_allocation msg;
                msg.process_ram = 1; msg.process_choicee = 1;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal","--title=Timer (KERNEL MODE) ", "--", "bash", "-c", "./timer; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
                cerr << "Error: forking failed." << endl;
                return 1;
            } else {
                  // parent is checking resources for the first kernel proceeses and giving permission 
                sleep(1);
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0)  && (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                    core--;
                    occupied_ram += m.process_ram;
                    occupied_core+=1;
					p_permission = m.permission;
					p_ram = m.process_ram;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to time process is denied" << endl;
                }
				priority =-1;
                memcpy(shmem, &m, sizeof(memory_allocation));
                 wait(NULL);
            }
				} 
				else if (count ==2){
					  pid_t pid = fork();

            if (pid == 0) {
                memory_allocation msg;
                msg.process_ram = 1; msg.process_choicee = 1;  msg.core = 1;

                memcpy(shmem, &msg, sizeof(memory_allocation));
                
                sleep(2);
                memory_allocation msg1;
                memcpy(&msg1, shmem, sizeof(memory_allocation));
                if (shmem->permission == 1) {
                    const char* args[] = {"gnome-terminal", "--title=System Monitor (KERNEL MODE)", "--", "bash", "-c", "./system_monitor; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
                }
                return 0;
            } else if (pid < 0) {
                cerr << "Error: forking failed." << endl;
                return 1;
            } else {
                  // parent is checking resources for the first kernel proceeses and giving permission 
                sleep(1);
                 memcpy(&m, shmem, sizeof(memory_allocation));
                if ((ram - m.process_ram >= 0)  && (core - m.core >= 0)) {
                    m.permission = 1;
                    ram = ram - m.process_ram;
                    core--;
                    occupied_ram += m.process_ram;
                    occupied_core+=1;
					p_permission = m.permission;
					p_ram = m.process_ram;
                } else {
                    m.permission = 0;
					p_ram = m.process_ram;
					p_permission = m.permission;
					 int signal = SIGTERM;  // Signal to terminate the child process
                    kill(pid, signal);
                    cout << "permission to time process is denied" << endl;
                }
				priority  = -1;
                memcpy(shmem, &m, sizeof(memory_allocation));
                 wait(NULL);
            }
					
				}
				
				  adding_process_to_pcb( choice,priority,p_ram, process , p_permission );
			   	    if (p_permission == 0){
		                  adding_process_to_ready_q(choice, process,priority);
		            }
				count++;
				
			}      
    }

shmdt(shmem);
//shmdt(object);
shmctl(shmid, IPC_RMID, NULL);
	for(int i=0;i<15;i++)
	{
		if(threadcalled[i])
		pthread_join(pid[i], NULL);	
		
	}
}
