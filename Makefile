all:
	gcc -o log_bomb -O3 -pthread log_bomb.c 
	gcc -o rand_log_bomb -O3 -pthread rand_log_bomb.c
