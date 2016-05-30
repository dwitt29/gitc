
msg_server: create_listener.o conn_mgr.o find_max_fd.o main.o open_log_file.o close_logfile.o write_logfile.o now.o
	gcc -o msg_server create_listener.o conn_mgr.o find_max_fd.o main.o open_log_file.o close_logfile.o write_logfile.o now.o

open_log_file.o: open_log_file.c
	gcc -g -c open_log_file.c

close_logfile.o: close_logfile.c
	gcc -g -c close_logfile.c

write_logfile.o: write_logfile.c
	gcc -g -c write_logfile.c

now.o: now.c
	gcc -g -c now.c

create_listener.o: create_listener.c
	gcc -g -c create_listener.c

conn_mgr.o: conn_mgr.c
	gcc -g -c conn_mgr.c

find_max_fd.o: find_max_fd.c
	gcc -g -c find_max_fd.c

main.o: main.c
	gcc -g -c main.c
