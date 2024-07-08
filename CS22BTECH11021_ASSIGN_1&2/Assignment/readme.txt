I have made a different commands for each commands .so all commands have their own output.i have not merged this five commands.    

1)ls:
	myls [OPTION]
	This works as original command ls.
	This can be executed with following options:
	1) -l 2) -S 3) -U 4) -lS  5) -a
2)ps:
	myps [OPTION]
	This works as original command ps.
	This can be executed with following options:
	1) -r 2) -s 3) -t 4) -d 5) -ps
3)cp:
	mycp [OPTION] [SOURCE] [TARGET]
	mycp [SOURCE] [SOURCE]..... [DIRECTORY]
	This works as original command cp.
      1)-cp 2)-f 3)-i 4)-n 5)-v
	
4)mv:
	mymv [OPTION] [SOURCE] [TARGET]
	mymv [SOURCE] [SOURCE]..... [DIRECTORY]
	This works as original command mv.
      1)-v 2)-f 3)-n 4)-i 5)-mv
	
	 
5)grep:
	mygrep [OPTION] PATTERN [FILE] [FILE]...
	This works as original command grep.
      1)-H 2)-n 3)-p 4)-c

THERE IS NO MAKEFILE FOR QUESTION 2 COMPILE MANUALLY WITH -LREALINE LIBRARY .
THERE IS A MAKEFILE FOR QUESTION 1.

Type 'help' and ENTER for detailed usage of options for every command.
Type '[COMMAND] --help' and ENTER for detailed usage of options for a particular command.
	
Other commands are getting executed through system().
cd is getting executed with the help of chdir().

The requirements to execute this terminal are :
-> A Linux Ubuntu machine.
-> GCC should be installed in the machine:
	If GCC is not installed in your machine, do the following:
	Open the terminal on your machine and run the following commands:
		-> "sudo apt update"
		-> "sudo apt install build-essential"
		OR
		-> "sudo apt install gcc"
	To check if install then run the following command:
	        -> "gcc --version"
-> libreadline :
Install libreadline in your machine:
For that run the following command on your terminal:
		-> "sudo apt-get install libreadline8 libreadline-dev"
Now you are ready to run this terminal!

Steps to execute:

-> Change the directory to the directory where the files are located.
-> Now run the following commands: 
    -> "make" (To compile the executable files.)
    -> "./terminal" (To execute the terminal.)

Now you can run whichever command you want to run from the above mentioned commands.



