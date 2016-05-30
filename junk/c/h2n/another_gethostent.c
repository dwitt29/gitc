#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define TRUE 1

static void prthost(struct hostent *h);

main(int argc, char *argv[])
{
   struct hostent *h;

/* sethostent() opens the prefix.ETC.HOSTS file, or when using a  */
/* nameserver, opens a TCP connection to the nameserver.          */

   sethostent(TRUE);

/* gethostent() reads the next sequential entry in the            */
/* prefix.ETC.HOSTS file. It returns a pointer to a "hostent"     */
/* structure.                                                     */

   while (h=gethostent())
      prthost(h);

/* endhostent() closes the prefix.ETC.HOSTS file, or the          */
/* connection to the nameserver.                                  */

   endhostent();
   exit(EXIT_SUCCESS);
}

/* prthost() prints the information returned by gethostent()      */
/* from the hostent structure.                                    */

static void prthost(struct hostent *h)
{
   char **p;

   /* Print primary name and aliases. */
   printf("\nname: %s\n",h->h_name);
   for (p=h->h_aliases; *p; p++)
      printf("alternate name: %s\n",*p);

   /* Handle unexpected situations gracefully. */
   if (h->h_addrtype != AF_INET) {
      printf("Not an internet address.\n");
      return;
   }
   if (h->h_length != sizeof(struct in_addr)) {
      printf("Invalid length: %d.\n",h->h_length);
      return;
   }

   /* Print the primary address and any alternates.  */
   for (p=h->h_addr_list; *p; p++) {
      printf("%s address: %s\n",
             p==h->h_addr_list ? "primary " : "alternate ",
             inet_ntoa((*(struct in_addr *)*p)) );
   }
}

