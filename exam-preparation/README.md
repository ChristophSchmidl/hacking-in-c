# Hacking in C - Exam Preparation

## Lecture 1: Security in local-area networks

* Netcat example
	* Command in tyrion: ``` netcat -lp 51966 ```
	* Command on arya: ``` echo "Hi tyrion" | netcat tyrion 51966 ```
* How does arya know tyrions IP address? -> she sends and ARP request to the broadcast MAC address (ff:ff:ff:ff:ff:ff) and gets a arp reply from tyrion with "192.168.42.1 is at 50:75.9d.f3:db:29"
* The above example is sent over a TCP/IP connection
* Network sniffing through Hubs and Switched Ethernet. Switches are aware of the MAC addresses behind their ports and do not broadcast every packet.
* **ARP Cache poisoning/ARP spoofing** still works for Switched Ethernet though
	* **Gratuitous ARP packets** are announcements ("replies without a request"). There are good reasong for this kind of announcements like announcing IP+MAC at boot time, announcing changed IP address to other hosts or ip-adress takeover in high performance clusters
* In order to use arp spoofing, IP forwarding should be enabled: ``` echo 1 > /proc/sys/net/ipv4/ip_forward ```	
* Poisoning the ARP cache of 192.168.42.2: ``` arpspoof -t 192.168.42.2 192.168.42.3 ```
* **MAC flooding**: Send many Ethernet frames with different source MAC addresses which overflows the **Content addressable memory (CAM)** of the switch
	* The effect of this depends highly on the switch itself
	* Some switches will fall back to behave like a hub 
* **ARP-attack countermeasures**:
	* static ARP table entries
	* disable gratuitous ARP (may break stuff)
	* ARP filter -> arptables
	* arpwatch
	* Protection mechanisms on advanced switches like "Dynamic ARP Inspection"
	* Protection against ARP spoofing is hard in general because ARP does not have any authentication mechanism
* **VLANs**
	* Partioning of local-area network (LAN) into multiple virtual LANs (VLANs)
	* VLANs have separated broadcast domains
	* ARP requests/replies don't go from one VLAN to another
	* Can limit the damage caused by ARP-level attacks ("students can only attack each other")		
* **MAC address filtering**
	* Switches could use a whitelist of MAC addresses and ignore computers with unknown MAC addresses
	* **MAC spoofing** is an easy way to circumvent Mac address filtering
	* ``` ip link set dev eth0 down; ip link set dev eth0 address 42:42:42:42:42:42; ip link set dev eth0 up```
* Wireless networks: **IEEE 802.11**
	* Networks are logically separated by their network names (**service set identification, SSID**)
	* Communication is physically separated by using different **channels (frequencies)**
	* Two different modes of operation: **ad-hoc mode: peer-to-peer** and **infrastructure mode: communication through access point(AP)**
* Connecting to a WiFi network
	* Connections to a wireless network are handled through **management frames**
	* Other important management frames:
		* **Reassociate request/response frames: change the AP**
		* **Disassociate frame: leave the network**
	* APs send **beacon frames** (by default, 10/second) containing:
		* Timestamp
		* Beacon interval
		* SSID
		* Frequency-hopping parameters
* **Hidden SSID**
	* Clients need to know the SSID to authenticate/associate
	* Idea: Don't send this SSID in beacon frames and advertise this as "network cloaking"
	* Network is not really cloaked, SSID is also contained in other frames
* Encrypted WiFi
	* **WEP (Wired Equivalent Privacy)** uses RC4 with 40-bit key and 24-bit IV -> weak encryption
	* RC4/WEP can be broken after 40000 frames with probability > 50% and after 85000 frames with probability > 95%
	* WEP-104 can be broken in less than a minute
	* **WPA (WiFi Protected Access)** uses **Temporal Integrity Protocol (TKIP)** which is a wrapper around RC4
	* WPA is therefore still vulnerable
	* **WPA2** got rif of RC4 and uses CCMP instead with a 128-bit AES key but sometimes provides backwards-compatibility to TKIP
	* **WPS (WiFi Protected Setup)** is still a possibility to circumvent WPA2 but is disabled but most routers





## Mock Exam

1. **(6 points)** 


	* a) What should be filled in for the ... below to declare **x, y, z,** and **w** with the right types?

		* ```
			void f(){
				int* i;
				int* a[5];
				... x = &i;
				... y = *i;
				... z = a[3];
				... w = a;
			} 

		* Answer:




	* b) In the code sample above, will **a** be allocated on the heap or the stack?

		* Answer:


	* c) What are the values of **i** and **s** after executing the code below? Assume that **p, q, r** and **s**, have been declared with the correct types.

		* ```
			int32_t i = 5; // Recall: int32_t is the type of signed 32 bit integers
			p = &i;
			q = &p;
			r = *q;
			s = **q;
			(*p)++;
			(*q)++;
			(*r)++;
			s++;

		* Answer:


2. **(5 points)** Suppose the code below is compiled for a 64 bit architecture.

	* ```
		void f(){
			int32_t a[4]; // Recall: int32_t is the type of signed 32 bit integers
			char c;
			int32_t d[2];
			char b[7];
			int64_t i; // Recall: int64_t is the type of signed 64 bit integers
			...
		} 


	* a) What would be an obvious way for a compiler to minimise the amount of stack space needed for this function, without compromising execution speed?

		* Answer:

	* b) Write a piece of C code that could go in the place of the dots that would return "yes" if the compiler has optimised the stack layout as you suggested in a). Explain the idea behind the code.

		* Answer:
	



3. **(5 points)** Consider the following code

	* ```
		#include <stdio.h>
		#include <stdint.h>

		int main() {
			int32_t x[4];
			x[0] = -2;
			x[1] = 34;
			x[2] = 1|2|4; // | is bitwise OR

			printf("%lx \n", x);
			printf("%lx \n", x+2); // (a)
			printf("%lx \n", &x); // (b)
			printf("%lx \n", &x+2); // (c)
			printf("%lx \n", *(x+1) & 2); // (d) & is bitwise AND
			printf("%lx \n", *x + x[2]); // (e)
			return 0;
		}

	* Recall that **%lx** prints a long in hexadecimal notation. Assume that the target machine uses two's complement to represent negative integers. If the first call to **printf** prints **abcdabcdef00**, what do the other calls to **printf** print?	

		* Answer:



4. **(8 points)** Consider the code below. The code is legal C.

	* ```
		1. char* f(char* w) {
		2. 		char *groet = "hello";
		3. 		char *t = malloc(50);
		4. 		char *u = malloc(50);
		5. 		char z[50];
		6. 		strcpy(u,groet); // copies the string groet to u
		7. 		u[5] = ’!’;
		8. 		t = w;
		9. 		printf("String t is now %s.\n", t);
		10. 	t = u;
		11. 	printf("String t is now %s.\n", t);
		12. 	free(t);
		13. 	free(z);
		14. 	return u;
		15. }

	* a) What are the 5 errors in the program above? (One error occurs two times, so there are in fact 6 errors.) For each error, mention the line number(s) involved, and explain what is wrong.

		* Answer:

	* b) Could the print statement in line 9 cause a segmentation fault? Motivate your answer.
	
		* Answer:

	* c) Is it possible to say what the **printf** statement in line 11 will print (if the program does not crash before)? If so, say what it prints; if not, explain why.
	
		* Answer:		





5. **(8 points)** Consider the code below. We assume the compiler does not optimise away redundant variables such as **secret**. The code for functions **f** and **g** is not shown.

	* ```
		1. main () {
		2. 		proc1();
		3. }
		4.
		5. proc1() {
		6. 		int secret = 1234;
		7. 		proc2();
		8. }
		9.
		10. proc2() {
		11. 	int public = 024;
		12. 	f();
		13. 	printf("f is done");
		14. 	g();
		15. 	printf("The area code is %i", public);
		16. }


		

	* Suppose that the function **f** contains some memory weaknesses, which an attacker can exploit to inspect and/or corrupt the stack by supplying malicious input. We assume there are not countermeasures against this, such as stack canaries or a non-executable stack. In answering the questions below, when talking about stack frames always make it clear to which function these belong.


	* a) Could an attacker corrupt the stack during the call to **f** in such a way that after returning from **f** the print statement on line 13 is not executed, but in all other respects execution continues as it normally would? Moativate your answer, by explaining why not, or by explaining how the attacker might achieve this.
	
		* Answer:

	* b) Could an attacker corrupt the stack during the call to **f** so that the value of **secret** will be printed instead of the value of **public** when execution reaches line 15? Motivate your answer, by explaining why not, or by explaining how the attacker might achieve this.

		* Answer:

	* c) Would having a non-executable stack prevent any attacks you discussed in your answers to a) or b) above?
	
		* Answer:

	* d) To provide some additional security, the programmer replaces line 12 with the following lines:

		* ```
			12a.	int test = 2525;
			12b.	f();
			12c.	if (test != 2525) { exit(-1); }

		* Does this provide any additional security? Motivate your answer. (Here assume that the compiler does not optimise the superfluous if-statement away because the **then**-branch is unreachable.)

			* Answer:	

5. **(8 points)** The code running on the vulnerable server hackme.cs.ru.nl for the last assignment was something like

	* ```
		1. #include <stdio.h>
		2.
		3. void echostr(void) {
		4. 		char buffer[80];
		5. 		gets(buffer);
		6. 		printf(buffer);
		7. 		printf("\n");
		8. }
		9.
		10. int main(void) {
		11. 	while (1) { echostr(); }
		12. 	return 0;
		13. }

	* a) What are the two security vulnerabilities in this code?
	
		* Answer:

	* b) How would you fix these vulnerabilities?
	
		* Answer:

	* c) To craft the attack string to launch a shell, in addition to the actual shell code, the attacker needs still some additional information. What is additional information that the attacker needs?
	
		* Answer:

	* d) Did the vulnerable server that you had to take over using the given shell code run with a non-executable stack? Or was the impossible to tell from the attack? Motivate your answer.
	
		* Answer:				




