#define MYBYTE unsigned char
#define MYWORD unsigned short
#define MYDWORD unsigned int

#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <time.h>
#include <tchar.h>
#include <ws2tcpip.h>
#include <limits.h>
#include <iphlpapi.h>
#include <math.h>

#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define MYLWORD unsigned long long
#else
#define MYLWORD unsigned int
#endif

#ifdef _MSC_VER
   #define strcasecmp _stricmp
   #pragma comment(lib, "ws2_32.lib")
   #pragma comment(lib, "iphlpapi.lib")
#endif

//Constants
#define my_inet_addr 	inet_addr
#define MAX_SERVERS 8

//Structs
struct home
{
	char alias[64];
	char target[256];
};

struct tftpConnType
{
	SOCKET sock;
	sockaddr_in addr;
	MYDWORD server;
	MYWORD port;
	bool loaded;
	bool ready;
};

struct acknowledgement
{
	MYWORD opcode;
	MYWORD block;
};

struct message
{
	MYWORD opcode;
	char buffer[514];
};

struct tftperror
{
	MYWORD opcode;
	MYWORD errorcode;
	char errormessage[512];
};

struct packet
{
	MYWORD opcode;
	MYWORD block;
	char buffer;
};

struct data12
{
	MYDWORD rangeStart;
	MYDWORD rangeEnd;
};

struct request
{
	timeval tv;
	fd_set readfds;
	time_t expiry;
	SOCKET sock;
	SOCKET knock;
	MYBYTE sockInd;
	MYBYTE attempt;
	char path[256];
	FILE *file;
	char *filename;
	char *mode;
	char *alias;
	MYDWORD tsize;
	MYDWORD fblock;
	int bytesReady;
	int bytesRecd;
	int bytesRead[2];
	packet* pkt[2];
	sockaddr_in client;
	socklen_t clientsize;
	union
	{
		tftperror serverError;
		message mesout;
		acknowledgement acout;
	};
	union
	{
		tftperror clientError;
		message mesin;
		acknowledgement acin;
	};
	MYWORD blksize;
	MYWORD timeout;
	MYWORD block;
	MYWORD tblock;
};

struct data1
{
	tftpConnType tftpConn[MAX_SERVERS];
	MYDWORD allServers[MAX_SERVERS];
	MYDWORD staticServers[MAX_SERVERS];
	MYDWORD listenServers[MAX_SERVERS];
	MYWORD listenPorts[MAX_SERVERS];
	SOCKET maxFD;
	bool ready;
	bool readyForChange;
};

struct data2
{
	WSADATA wsaData;
	home homes[8];
	FILE *logfile;
	data12 hostRanges[32];
	char fileRead;
	char fileWrite;
	char fileOverwrite;
	int minport;
	int maxport;
	MYBYTE logLevel;
	MYDWORD failureCount;
};

struct data15
{
	union
	{
		//MYDWORD ip;
		unsigned ip:32;
		MYBYTE octate[4];
	};
};

//Functions
bool detectChange();
void closeConn();
void getInterfaces(data1*);
void runProg();
void processRequest(LPVOID lpParam);
char* myGetToken(char*, MYBYTE);
char* myTrim(char*, char*);
void init(void*);
bool cleanReq(request*);
bool addServer(MYDWORD*, MYDWORD);
FILE* openSection(const char*, MYBYTE, char*);
char *readSection(char*, FILE*);
bool getSection(const char*, char*, MYBYTE, char*);
bool isIP(char*s);
char* myLower(char*);
char* myUpper(char*);
char* IP2String(char*, MYDWORD);
MYDWORD* findServer(MYDWORD*, MYDWORD);
void printWindowsError();
void logMess(request*, MYBYTE);
void logMess(char*, MYBYTE);
