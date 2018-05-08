#include <iostream>
#include "unp.h"
#include <arpa/inet.h>

int main(int argc,char ** argv)
{
	/**  inet_pton  */
	// char * ip = "1.1.1.1";
	// int a;
	// inet_pton(AF_INET,ip,&a);
	// std::cout<<a;
	

	/**  inet_ntop  */
	// int ip=16843022;
	// char dest[16];
	// inet_ntop(AF_INET,&ip,dest,16);
	// std::cout<<dest;

	auto value = htons(0x0001);
	std::cout<<value<<std::endl;
}