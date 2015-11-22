#pragma once
#include "ProjectDefines.h"
#include "ConnectionInfo.h"

class RequestManager
{
public:
	RequestManager();
	~RequestManager();
	static int iteratePost(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
		const char *filename, const char *content_type,
		const char *transfer_encoding, const char *data,
		uint64_t off, size_t size);
	static void requestCompleted(void *cls, struct MHD_Connection *connection, //<-mozliwy blad przez static
							void **con_cls, enum MHD_RequestTerminationCode toe);


	static int requestHandler(void *cls, struct MHD_Connection *connection,
		const char *url,
		const char *method, const char *version,
		const char *upload_data,
		size_t *upload_data_size, void **con_cls);
	static int sendPage(struct MHD_Connection *connection, const char *page);

private:

	static const char* _askPage;
	static const char* _greatingPage;
	static const char* _errorPage;
};

