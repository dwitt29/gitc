#include <stdio.h>
#include <mysql/mysql.h>

int main()
{
  MYSQL_RES *results;
  MYSQL_ROW row;
  MYSQL *connection, mysql;
  int state;

  mysql_init(&mysql);
  connection=mysql_real_connect(&mysql,"d8100.witt.net","wittda","test123","test",0,0,0);

  if (connection == NULL)
  {
    printf(mysql_error(&mysql));
    return 1;
  }

  state=mysql_query(connection, "select * from dave");

  if (state != 0)
  {
    printf(mysql_error(connection));
    return 1;
  }

  results = mysql_store_result(connection);
  printf("Rows: %d\n",mysql_num_rows(results));

  while ( (row=mysql_fetch_row(results)) != NULL )
  {
    printf("Name = %s, Addr = %s, id = %d\n", row[0],row[1],atoi(row[2]));
  }

  mysql_free_result(results);
  mysql_close(connection);
  printf("Done.\n");
}
