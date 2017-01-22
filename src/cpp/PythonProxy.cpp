#ifndef TESTING_ONLY
#include "PyServerPrivatePCH.h"
#include "UnrealClient.h"
#include "ImageUtils.h"
#include "TimerManager.h"
#include "PyServerTickActor.h"
#else
#define LogTemp 0
#define Warning 0
#define TEXT(x) x
#define UE_LOG(arg1,arg2,...)  printf(__VA_ARGS__)

#endif


#include <dlfcn.h>



//#include  "/local/ori/anaconda3/include/python3.5m/Python.h"
extern "C" {
//#define PYTHON_LIB "/local/ori/anaconda3/lib/libpython3.5m.so"
//#define SYSPATH "local/learn/ur4/testplugin/Plugins/PyServer/Source/PyServer/Private"

//#include PYTHON_H
#include "PyConfig.h"



#define PYRUN(x) (*_PyRun_SimpleString)(x)
//#error stopping!


void (*_Py_Initialize)();
void (*_PyRun_SimpleString)(const char*);

PyObject *initModule=NULL;
void* phandle=NULL;

void InitPythonFunctions()
{
	//phandle=dlopen("/usr/lib/x87_64-linux-gnu/libpython3.5m.so",RTLD_LAZY);
	//phandle=dlopen(PYTHON_LIB,RTLD_LAZY);
	phandle=dlopen(getenv("PYTHON_LIB"),RTLD_GLOBAL | RTLD_NOW);
	_Py_Initialize=reinterpret_cast<void (*)()>(dlsym(phandle,"Py_Initialize"));
	_PyRun_SimpleString=reinterpret_cast<void (*)(const char*)>(dlsym(phandle,"PyRun_SimpleString"));
}


void LoadPythonInterperter()
{

	if (phandle==NULL)
	{
		char tmpstr[1024];
		UE_LOG(LogTemp, Warning, TEXT("Starting LoadPythonInterperter...\n"));
		UE_LOG(LogTemp, Warning, TEXT("---- %d %d PYTHON_LIB=%s\n"),phandle,_Py_Initialize,getenv("PYTHON_LIB"));
		InitPythonFunctions();
		(*_Py_Initialize)();
		snprintf(tmpstr,1024,"import sys;sys.path.append('%s')", getenv("SYSPATH"));
		PYRUN(tmpstr);
	}

}


void StopPythonInterperter()
{
	//TODO:
	//TODO: call PyFinalize
	UE_LOG(LogTemp, Warning, TEXT("StopPythonInterperter...\n")); 
}
//TFunction<void> SampleTimerExpired();

void mytick()
{
	PYRUN("pyinit.PyTick()");
}

void mybeginplay()
{
	PYRUN("from Wrappers import pyinit");
	char tmpstr[1024];
	sprintf(tmpstr,"pyinit.PyBeginPlay('%p')",reinterpret_cast<void*>(GWorld->GetWorld()));
	PYRUN(tmpstr);	
}

void myendplay()
{
	char tmpstr[1024];
	sprintf(tmpstr,"pyinit.PyEndPlay('%p')",reinterpret_cast<void*>(GWorld->GetWorld()));
	PYRUN(tmpstr);	
	
}

int calledfrompython()
{
	printf("yo!!!\n");
	UE_LOG(LogTemp, Warning, TEXT("Called from python\n")); 
	FString filename = "/tmp/test1.png";
	FScreenshotRequest::RequestScreenshot(filename, false, false);
	return 0;
}




}//extern "C"
