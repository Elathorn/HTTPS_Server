#pragma once
#include "ProjectDefines.h"
struct ConnectionInfo
{
	int _connectionType;
	char* _answerString;
	struct MHD_PostProcessor *_postProcessor;
};

