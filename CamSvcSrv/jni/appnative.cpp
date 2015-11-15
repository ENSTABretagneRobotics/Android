#include <string.h>
#include <jni.h>

// Because of the compiler option -x c++ (added due to OpenCV incompatibilities with C mode),
// the functions should have been compiled in C++ mode...
extern int AppNativeMain();
//extern "C" int AppNativeMain();

extern int AppNativeQuit();
//extern "C" int AppNativeQuit();

extern "C" 
{
	int Java_com_example_camsvcsrv_CamSvcSrv_AppNativeMain(JNIEnv* env, jobject thiz)
	{
		// Unreferenced parameters.
		(void)env;
		(void)thiz;

		return AppNativeMain();
	}

	int Java_com_example_camsvcsrv_CamSvcSrv_AppNativeQuit(JNIEnv* env, jobject thiz)
	{
		// Unreferenced parameters.
		(void)env;
		(void)thiz;

		return AppNativeQuit();
	}
}
