/* Copyright 2003-2004 The MathWorks, Inc. */

// *******************************************************************
// **** To build this mex function use: mex sfun_cppcount_cpp.cpp ****
// *******************************************************************


#include "UseFunc.h"

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  UseFunc

// Need to include simstruc.h for the definition of the SimStruct and
// its associated macro definitions.
#include "simstruc.h"
#include "mex.h" 


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

void UseFun_StartSock(SimStruct *S);
void UseFun_SentData(SimStruct *S, const real_T *data, int DataNum);
void UseFun_CloseSock(SimStruct *S);


#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

// Function: mdlInitializeSizes ===============================================
// Abstract:
//    The sizes information is used by Simulink to determine the S-function
//    block's characteristics (number of inputs, outputs, states, etc.).
static void mdlInitializeSizes(SimStruct *S)
{
    // No expected parameters
    ssSetNumSFcnParams(S, 0);

    // Parameter mismatch will be reported by Simulink
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    // Specify I/O
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);

    ssSetNumSampleTimes(S, 1);

    // Reserve place for C++ object
    ssSetNumPWork(S, 3);

    ssSetOptions(S,
                 SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE);

}


// Function: mdlInitializeSampleTimes =========================================
// Abstract:
//   This function is used to specify the sample time(s) for your
//   S-function. You must register the same number of sample times as
//   specified in ssSetNumSampleTimes.
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}

// Function: mdlStart =======================================================
// Abstract:
//   This function is called once at start of model execution. If you
//   have states that should be initialized once, this is the place
//   to do it.
#define MDL_START
static void mdlStart(SimStruct *S)
{
    // Store new C++ object in the pointers vector
    DoubleAdder *da  = new DoubleAdder();
    ssGetPWork(S)[0] = da;

    UseFun_StartSock(S);
}

// Function: mdlOutputs =======================================================
// Abstract:
//   In this function, you compute the outputs of your S-function
//   block.
static void mdlOutputs(SimStruct *S, int_T tid)
{
    // Retrieve C++ object from the pointers vector
    DoubleAdder *da = static_cast<DoubleAdder *>(ssGetPWork(S)[0]);
    
    // Get data addresses of I/O
    InputRealPtrsType  u = ssGetInputPortRealSignalPtrs(S,0);
               real_T *y = ssGetOutputPortRealSignal(S, 0);

    int InputNum = ssGetInputPortWidth(S, 0);
    for(int i=0;i<InputNum;i++)
    {

        y[i] = *u[i];
    }

    UseFun_SentData(S, y, InputNum);
}

// Function: mdlTerminate =====================================================
// Abstract:
//   In this function, you should perform any actions that are necessary
//   at the termination of a simulation.  For example, if memory was
//   allocated in mdlStart, this is the place to free it.
static void mdlTerminate(SimStruct *S)
{
    // Retrieve and destroy C++ object
    DoubleAdder *da = static_cast<DoubleAdder *>(ssGetPWork(S)[0]);
    delete da;

    UseFun_CloseSock(S);
}

void UseFun_StartSock(SimStruct *S)
{
    int iResult;
    WSADATA wsaData;

    SOCKET *pSendSocket = new SOCKET;
    *pSendSocket = INVALID_SOCKET;

    sockaddr_in *pRecvAddr = new sockaddr_in;

    unsigned short Port = 54377;
    
    printf("Start socket communication, please wait...\n");

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return ;
    }

    //---------------------------------------------
    // Create a socket for sending data
    *pSendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*pSendSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return ;
    }

    //---------------------------------------------
    // Set up the RecvAddr structure with the IP address of
    // the receiver (in this example case "192.168.1.1")
    // and the specified port number.
    pRecvAddr->sin_family = AF_INET;
    pRecvAddr->sin_port = htons(Port);
    pRecvAddr->sin_addr.s_addr = inet_addr("127.0.0.1");

    ssGetPWork(S)[1] = pSendSocket;
    ssGetPWork(S)[2] = pRecvAddr;
}

void UseFun_SentData(SimStruct *S, const real_T *data, int DataNum)
{
    int iResult;

    char SendBuf[1024]={'\0'};
    int BufLen = 1024;

    SOCKET *pSendSocket    = static_cast<SOCKET *>(ssGetPWork(S)[1]);
    sockaddr_in *pRecvAddr = static_cast<sockaddr_in *>(ssGetPWork(S)[2]);

    if (*pSendSocket == SOCKET_ERROR) {
        printf("SOCKET_ERROR error: %d\n", WSAGetLastError());
        closesocket(*pSendSocket);
        WSACleanup();
        return ;
    }

    //---------------------------------------------
    // Send a datagram to the receiver
    //printf("Sending a datagram to the receiver...\n");
    int ValidateBufLen = 0;
    for(int i=0;i<DataNum;i++)
    {
        ValidateBufLen = strlen(SendBuf);
        sprintf(SendBuf+ValidateBufLen, "%g;", data[i]);
    }

    iResult = sendto(*pSendSocket,
        SendBuf, BufLen, 0, (SOCKADDR *)pRecvAddr, sizeof(sockaddr_in));
}

void UseFun_CloseSock(SimStruct *S)
{
    SOCKET *pSendSocket    = static_cast<SOCKET *>(ssGetPWork(S)[1]);
    sockaddr_in *pRecvAddr = static_cast<sockaddr_in *>(ssGetPWork(S)[2]);

    //---------------------------------------------
    // When the application is finished sending, close the socket.
    printf("Finished socket communication, Closing socket.\n");

    if (closesocket(*pSendSocket) == SOCKET_ERROR)
    {
        printf("closesocket failed with error: %d\n", WSAGetLastError());
    }
    //---------------------------------------------
    // Clean up and quit.

    WSACleanup();

    delete pSendSocket;
    pSendSocket = NULL;

    delete pRecvAddr;
    pRecvAddr = NULL;
}

// Required S-function trailer
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif