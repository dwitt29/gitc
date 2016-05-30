int find_max_fd(clients,client_index)
int *clients;
int client_index;
{
  int i;
  int max_fd=0;

  for (i=0; i<client_index; i++)
    if (clients[i] > max_fd)
      max_fd=clients[i];

  return max_fd;
}
