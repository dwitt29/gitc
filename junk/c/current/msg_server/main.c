int log_fd=0;

main()
{
  int listen_limit=5;
  int listen_quan=0;
  int sockc[listen_limit];
  char *log="/tmp/msg_server.log";

  log_fd=open_log_file(log);

  while (listen_quan < listen_limit)
  {
    conn_mgr("test",sockc,&listen_limit);
  }

  close_log_file(&log_fd);
  
}
