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



	* a) 

		* Answer

	* b) Assume that an attacker with IP address 192.168.1.66 sets up a rogue DHCP server to become a man in the middle between the new computer and wikipedia.com. Which of the pieces of information from part a) could he modify to become a man in the middle? What information would he send? How would the attack proceed (if there are any further steps required)? Give all possibilities for an attack.

		* Answer
	

	* c) Why could a rogue-DHCP attack fail? What possibilities does an attacker have to increase the chances of success?

		* Answer


3. **(20 points)** For each of the following three different port scan types

	* connect scan, 
	* SYN scan,
	* idle scan

	answer the following questions:

	* a) How does it work? What packets are being sent to probe whether the port is open, what answer packet(s) are expected if the port is open, what answer packet(s) are expected if it's closed?

		* **Connect scan**: The connect scan is not using raw packets but the underlying connect() system call of the operating system to connect to a remote port. If connect() succeeds then the port is open, if connect() fails then the port is closed. This can method relies on the Berkeley Sockets-API.
		* **SYN scan**: The SYN scan sends TCP SYN packets and receives a SYN/ACK packet if the port is open and a RST packet if the port is closed.
		* **Idle scan**: Idle scans rely on a so called zombie host and the fragment identification number (IPID) of IP packets. A zombie host is a idle machine on the network. First you have to probe the zombie's IPID and record it (IPID=X). Then forge a SYN packet from the zombie to the target host and port. Probe the zombie's IPID again (IPID=Y). If Y = X + 1 then the port is closed, if Y = X + 2 then the port is open.

	* b) The scans are listed in increasing order of "stealthiness". Explain briefly why this is the case by explaining how a system administrator could notice those scans and attribute their origin.
	
		* **Connect scan**: Connect scan takes the underlying socket api and performs full connects which reveals the ip address of the source and always closes its connections properly which let them appear in the server logs.
		* **SYN scan**: SYN scans also reveal the ip address of the source but SYN scans do not close the connections properly all the time and are left with a half open connection which may not appear in the server logs. The ip address is visible nevertheless and could be traced by any good intrusion system.
		* **Idle scan**: Idle scans have the advantage that they only reveal the ip address of the zombie host and not the original source. Idle scans also use SYN packets but perform their analysis of an open port based on the IPID.


4. **(20 points)** Consider the following iptables firewall script running on a laptop called **mylaptop**:

	* ```
		iptables -F
		iptables -P INPUT DROP
		iptables -P OUTPUT DROP
		iptables -P FORWARD DROP
		iptables -A OUTPUT -p tcp --dport 22 -j ACCEPT
		iptables -A OUTPUT -p tcp --dport 80 -j ACCEPT
		iptables -A OUTPUT -p tcp --dport 443 -j ACCEPT
		iptables -A OUTPUT -p icmp --icmp-type echo-request -j ACCEPT
		iptables -A INPUT -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT

	For each of the following tasks decide whether the firewall allows it or not. If the firewall does not allow it, give an iptables rule that enables it. In each part you can assume the presence of additional rules from the previous parts. **Note:** The rules have to be minimal and must not allow anything beyond the required functionality; in particular something like **iptables -P INPUT ACCEPT** is not a valid solution.

	* a) A web browser running on mylaptop tries to load the website at https://www.google.com

		* The firewall **allows** it based on the fact that outgoing connections on port 80 (http) and port 443 (https) are allowed and the resulting tcp handshake results into a established/related connection which is allowed by the last rule as input.

	* b) The user runs the ping utility on my laptop to test whether the host www.ru.nl is reachable.
	
		* The firewall **allows** it based on the fact that outgoing icmp echo-requests are allowed and and related/established connections are allowed as input.

	* c) A mail client on mylaptop retrieves e-mail from post.science.ru.nl through IMAPS (TCP port 993)
	
		* Is **not allowed** by the current firewall rules. Rule to allow IMAPS: ``` iptables -A OUTPUT -p tcp --dport 993 -j ACCEPT ```. You could do the same for port 143 (IMAP).

	* d) Another computer (not the laptop with the firewall) uses the ping utility to test whether mylaptop is reachable.

		* Is **not allowed** by the current firewall rules. Rule to allow incoming ping requests: ``` iptables -A OUTPUT -p icmp --icmp-type echo-reply -j ACCEPT ```

	* e) Somebody else from outside tries to connect to the SSH server running on port 22 of mylaptop.
	
		* Is **not allowed** by the current firewall rules. Rule to allow SSH from outside: ``` iptables -A INPUT -p tcp --dport 22 -j ACCEPT ```


5. **(20 points)** Consider a confidential e-mail being sent from a user A (using e-mail provider P_A) to another user B (using e-mail provider P_B). Consider the following independent cryptographic protections for this e-mail communication:

	* P1: User A is in a WPA2-protected WiFi using pre-shared keys.
	* P2: User A uses TLS to communicate with the SMTP server of P_A.
	* P3: User B uses TLS to communicate with the IMAP server of P_B.
	* P4: Provider P_A and P_B communicate through IPSec with encapsulated security payloads (ESP) in tunnel mode.
	* P5: User A obtains B's PGP public key from pgp.mit.edu and then encrypts the e-mail using PGP with this public key.

	Consider the following attacks against this e-mail communication:

	* A1: An attacker, who is not the in WiFi network that A is in, sniffs the WiFi traffic near A to read the e-mail.
	* A2: An attacker, who is in the WiFi network that A is in, sniffs the WiFi traffic to read the e-mail
	* A3: An attacker, who is in the same network that B is in, sniffs the network to read the e-mail.
	* A4: An attacker (controlling an Internet router) sniffs the traffic between A and the SMTP server of P_A.
	* A5: An attacker (controlling an Internet router) sniffs the traffic between P_A and P_B.
	* A6: A's provider is reading and analyzing the e-mail.
	* A7: B's provider is reading and analyzing the e-mail.			

	* a) Fill in a checkmark in each cell of the following table, if and only if the cryptographic protection alone is effective to prevent the attack:

		* ![SecureTable](img/table.PNG)

		**Note:** Don't forget to submit this sheet together with your exam or copy the table to your exam sheet.

		* ![FilledSecureTable](img/filled_table.png)

	* b) Can you think of an attack that would work against each of the protections (and any combination of those)?
	
		* Answer		






